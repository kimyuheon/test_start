// Arx_User.cpp: implementation of the CArx_User class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Arx_Draw.h"
#include "Arx_Math.h"
#include "Arx_Xdata.h"
#include "Arx_User.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArx_User::CArx_User()
{
	
}

CArx_User::~CArx_User()
{

}


///double 형을 string 형으로 바꾼다.
CString CArx_User::valtoa(double value)
{
	CString rr_str;
	rr_str.Format(_T("%-10.0f"), value);
	rr_str.TrimRight();
	return rr_str;
}

///double 형을 string 형으로 바꾼다.
CString CArx_User::valtoa(int value)
{
	
	CString rr_str;
	rr_str.Format(_T("%-10d"), value);
	rr_str.TrimRight();
	return rr_str;
}
//45도 단위 각도로 셋팅
void CArx_User::ads_setAngle45(ads_point ps, ads_point& pe)
{
	
	CArx_Math m_Math;
	CArx_Xdata m_Xdata;
	CArx_User m_User;
	CArx_Draw m_Draw;
	int inum;
	double ang;
	AcDbObjectId objId=NULL;
	ads_name ename;
	ads_point p1,p2;

	objId=m_Xdata.entitySSelect(pe,pe,_T("LINE"));
	ang = ads_angle(ps,pe);

	if ( ((int)(m_Math.RTD(ang)))%45 > 22.5)
		ang += m_Math.DTR(22.5);

	inum = (int)(ang/m_Math.DTR(45));

	ads_polar(ps, m_Math.DTR(45*inum), ads_distance(ps,pe), pe);
	if(objId != NULL){
		acdbGetAdsName(ename,objId);
		m_User.arx_adspt(p1,m_Xdata.lf_3DReturnList(ename,10));
		m_User.arx_adspt(p2,m_Xdata.lf_3DReturnList(ename,11));
		if(ads_inters(p1,p2,ps,pe,0,p1)==RTNORM)
			ads_point_set(p1,pe);
		ads_ssfree(ename);
	}
}
//사용자가 정의한 각도 단위로 셋팅
void CArx_User::ads_setAngleUser(double uang,ads_point ps, ads_point& pe)
{
	
	CArx_Math m_Math;
	
	int inum;
	double ang;
	ang = ads_angle(ps,pe);

	if ( ((int)(m_Math.RTD(ang)))%(int)uang > (uang/2.0))
		ang += m_Math.DTR(uang/2.0);

	inum = (int)(ang/m_Math.DTR(uang));

	ads_polar(ps, m_Math.DTR(uang*inum), ads_distance(ps,pe), pe);

}

void CArx_User::ads_ptCopy(ads_point ps,ads_point pe)
{
	ps[X]=pe[X];ps[Y]=pe[Y];ps[Z]=pe[Z];
}
// ADS 좌표를 ARX 좌표로 변환
void CArx_User::ads_arxpt(AcGePoint3d& ps,ads_point pe)
{
	ps.set(pe[X],pe[Y],pe[Z]);
}
// ARX 좌표를 ADS 좌표로 변환
void CArx_User::arx_adspt(ads_point& pe,AcGePoint3d ps)
{
	pe[X]=ps[0];pe[Y]=ps[1];pe[Z]=ps[2];
}

void CArx_User::arx_getLine(AcDbObjectId objIdd, AcGePoint3d& ps, AcGePoint3d& pe)
{
	AcDbLine* pLine;
	acdbOpenObject(pLine,objIdd,AcDb::kForRead);
	ps=pLine->startPoint();
	pe=pLine->endPoint();
	es=pLine->close();
}

void CArx_User::arx_exPoint(AcGePoint3d& p1, AcGePoint3d& p2)
{
	AcGePoint3d pt;
	pt = p1;
	p1 = p2;
	p2 = pt;
}


int CArx_User::arx_ptInter(AcGePoint3d pt1, AcGePoint3d pt2,
							AcGePoint3d pt3,AcGePoint3d pt4,
							int env,AcGePoint3d& pt5)
{
	int rev;
	ads_point p1,p2,p3,p4,p5;
	
	arx_adspt(p1,pt1);arx_adspt(p2,pt2);
	arx_adspt(p3,pt3);arx_adspt(p4,pt4);
	p1[2]=0;
	p2[2]=0;
	p3[2]=0;
	p4[2]=0;

	rev=ads_inters(p1,p2,p3,p4,env,p5);
	ads_arxpt(pt5,p5);
	return rev;
}

CString CArx_User::lfreturn_handle()
{
	CArx_Xdata m_Xdata;
	ads_name lst_name;
	
	if(ads_entlast(lst_name)!=RTNORM)
		return _T("");
	
	return m_Xdata.gfreturn_handle(lst_name);
}

void CArx_User::gfObjIDDel(AcDbObjectId objId)
{
	ads_name en;
	
	acdbGetAdsName(en,objId);
	gfenameDel(en);
	ads_ssfree(en);
}


void CArx_User::gfenameDel(ads_name en)
{
	AcDbObject* pobj;
	AcDbObjectId objId;

	if(acdbGetObjectId(objId,en) != Acad::eOk)
		return;
	if(acdbOpenObject(pobj,objId,AcDb::kForWrite)==Acad::eWasErased)
		return;
	pobj->erase();
	pobj->close();
}

int CArx_User::gfMulCount(CString mm)
{
	int loc,len,i=1;
	len = mm.GetLength();
	while(loc=mm.Find(_T(';')) != -1){
		mm = mm.Mid(loc+1,len-(loc+1));
		len = mm.GetLength();
		i++;
	}

	return i;
}
//메세지(msg)를 가지고 와서 선택한 객체의 명칭(en),관경(bore),
// 선택점(sel_pt), 시작점(stpt), 끝점(edpt)을 돌려줌
BOOL CArx_User::gf_entsel(TCHAR *msg,ads_name& en,int& bore,AcGePoint3d& sel_pt,
							 AcGePoint3d& stpt,AcGePoint3d& edpt)
{
	ads_point p1;
	CArx_Xdata m_Xdata;
	CArx_Math m_Math;
//	if(en[0]==0&&en[1]==0){
		if(ads_entsel(msg,en,p1) != RTNORM)
			return false;
		if(isOnLockedObj(en)) return false;
		ads_arxpt(sel_pt,p1);
//	}
	bore = m_Xdata.lf_IReturnList(en,1070);
	if(bore != 0){
		stpt=m_Xdata.lf_3DReturnList(en,10);
		edpt=m_Xdata.lf_3DReturnList(en,11);
		sel_pt=m_Math.gl_Perpt(stpt,edpt,sel_pt);
	}else
		return false;

	return true;
}

BOOL CArx_User::gf_dlentsel(ads_name en,double ang,ads_name& ename,AcGePoint3d inpt,
							AcGePoint3d& stpt,AcGePoint3d& edpt)
{
	int bore;
	double angle;
	CArx_Xdata m_Xdata;
	CArx_Math m_Math;
	ads_name ten;
	AcGePoint3d pt1,pt2;
	AcDbObjectId objId;

	if(ads_name_nil(en)){
		ads_printf(_T("\nInvalid Select."));
		return false;
	}
	bore = m_Xdata.lf_IReturnList(en,1070);
	
	pt1 = m_Xdata.lf_3DReturnList(en,10);
	pt2 = m_Xdata.lf_3DReturnList(en,11);
	angle = m_Math.arx_angleA(pt1,pt2)+ang;
	inpt = m_Math.Polar(inpt,bore/2.0,angle);
	if((objId=m_Xdata.entitySSelect(inpt,inpt,_T("LINE"))) != NULL){
		acdbGetAdsName(ten,objId);
		ads_name_set(ten,ename);
		stpt = m_Xdata.lf_3DReturnList(ten,10);
		edpt = m_Xdata.lf_3DReturnList(ten,11);
	}else
		return false;

	return true;
}


resbuf*  CArx_User::ptArrayToResbuf(const AcGePoint3dArray& ptArray,int cnt)
{
	ads_point p1;
	resbuf* ptList = NULL;		// overall list
	resbuf* lastRb = NULL;		// place holder to end of list
	resbuf* rb;
	//int len = ptArray.length();
	for (int i=0;i<cnt;i++) {
		if ((rb = ads_newrb(RT3DPOINT)) == NULL) {
			ads_relrb(ptList);
			return(NULL);
		}
		arx_adspt(p1,ptArray.at(i));
		ads_point_set(p1,rb->resval.rpoint);
		if (ptList == NULL) {
			ptList = rb;
			lastRb = rb;
		}
		else {
			lastRb->rbnext = rb;
			lastRb = rb;
		}
	}
	return(ptList);
}

// 정확한 파이프 선택
void CArx_User::GetPipe(ads_point pt1, ads_name &ename)
{
   struct resbuf *rtn=NULL,  *sys1=NULL, *sys2=NULL, *sys3=NULL;
   ads_point     pt2, pt3;
   ads_name      sname;
   ads_real      do1=0.0, yr1=0.0, vs=0.0 ,ss=0.0;
   short         pb=0;
   long          sl=0L;
   int           ret;

   sys1=ads_newrb(RTREAL);           /* sys1 must the allocated memory */
   sys2=ads_newrb(RT3DPOINT);
   sys3=ads_newrb(RTSHORT);

   ads_getvar(_T("VIEWSIZE"), sys1);     /* real */
   ads_getvar(_T("SCREENSIZE"), sys2);   /* point (real real) */
   ads_getvar(_T("PICKBOX"), sys3);      /* short */

   vs=sys1->resval.rreal;                       /* view size */
   ss=sys2->resval.rpoint[Y];        /* screensize */
   pb=sys3->resval.rint;                       /* pickbox */

   do1=vs/ss;                        /* view size / screensize */
   yr1=do1 * pb;                     /* real size of pickbox in screen */

   ads_polar(pt1, 0.25 * PI, yr1, pt2);
   ads_polar(pt1, 1.25 * PI, yr1, pt3);

   rtn=ads_buildlist(
                     RTDXF0, _T("LINE"),
					-4,		_T("<OR"),
					 6, _T("ILJUM30"),
					 6,	 _T("CENTER"),
					 -4,	_T("OR>"),
                     -3,
                     1001,   _T("PIP"),
                     RTNONE);

    ret=ads_ssget(_T("C"), pt2, pt3, rtn, sname);
         /* 메모리 해제 */
   if(rtn) ads_relrb(rtn);
   if(sys1)ads_relrb(sys1);
   if(sys2)ads_relrb(sys2);
   if(sys3)ads_relrb(sys3);
   if(ret==RTNORM){
	ssNoLock(sname);
	ads_sslength(sname, &sl);
   }else sl=0L;
   if(ret==RTNORM && sl>0L){
      ads_ssname(sname, 0L, ename);
      ads_ssfree(sname);
   }else{
      rtn=ads_buildlist(-4,       _T("<OR"),
                        RTDXF0, _T("LINE"),
                        RTDXF0, _T("ARC"),
                        -4,   _T("OR>"),
                        RTNONE);
      if(rtn){
         ret=ads_ssget(_T("C"), pt2, pt3, rtn, sname);
		 
         if(ret==RTNORM) {
			 ssNoLock(sname);
			 ads_sslength(sname, &sl);
		 }else            sl=0L;
         ads_relrb(rtn);
      }//end-if(rtn)
      if(ret==RTNORM && sl>0L){
         ads_ssname(sname, 0L, ename);
         ads_ssfree(sname);
      }//end-if
   }//end-if-else
}
// 객체명(ename)과 포인트(pnt)를 받아 정확한 점(pnt)을 반환
void CArx_User::gf_WPoint(ads_name ename, ads_point pnt)
{
	ads_point tem_pt1, tem_pt2, tem_pt3;
	double tem_ang;

	arx_adspt(tem_pt1,m_Xdata.lf_3DReturnList(ename,10));
	arx_adspt(tem_pt2,m_Xdata.lf_3DReturnList(ename,11));

	tem_ang=ads_angle(tem_pt1,tem_pt2);
	ads_polar(pnt, tem_ang+(0.5*PI), 1.0, tem_pt3);
	ads_inters(tem_pt1, tem_pt2, pnt, tem_pt3,0, pnt);
}
// 객체(cname),포인트(cpnt),관경(bore)을 받아서
// 양쪽 더블라인 객체(ent1,ent2)를 반환
void CArx_User::gf_WLentity(ads_name cname, ads_point cpnt, double bore, ads_name& ent1, ads_name& ent2)
{
	ads_point pnt;
	double ang;
	AcGePoint3d pt1,pt2,p1,p2,selpt;
	AcDbObjectId objId;
	
	gf_WPoint(cname, cpnt);  // 정확한 지시점
	pt1 = m_Xdata.lf_3DReturnList(cname,10);  // Start point
	pt2 = m_Xdata.lf_3DReturnList(cname,11);  // End   point
	ang = m_Math.arx_angleA(pt1,pt2); // 중심선의 각도

	ads_arxpt(selpt,cpnt);
	p1 = m_Math.Polar(selpt, bore*0.5, ang+90.0);
	p2 = m_Math.Polar(selpt, bore*0.5, ang-90.0);

	resbuf* filter = ads_buildlist(RTDXF0, _T("LINE"), -3, 1001, _T("PIP"), 0);
	arx_adspt(pnt,p1);
	if((objId=m_Xdata.DLineSelect(pnt,cname,_T("LINE"),filter)) != NULL)
		acdbGetAdsName(ent1,objId);
	arx_adspt(pnt,p2);
	if((objId=m_Xdata.DLineSelect(pnt,cname,_T("LINE"),filter)) != NULL)
		acdbGetAdsName(ent2,objId);

}

struct resbuf * CArx_User::ads_assoc(int grp, resbuf *rb)
{
        while((rb) && (rb->restype != grp))     rb=rb->rbnext;
        return rb;

}

void CArx_User::gf_SetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString vvalue)
{
	HKEY HKey2;
	CString sCompany, sProduct, prjname;
	
	TCHAR *pszBUF=NULL;
	DWORD stype;

	// 레지스트리 등록
	stype=REG_SZ;
	sCompany.Format(_T("Software\\%s"),strCompany);      // 회사명
	sProduct.Format(_T("%s\\%s"),sCompany,strProduct);		  // 제품명
	RegCreateKey(hKey, sProduct, &HKey2);

	// 프로젝트명을 레지스트리에 저장한다.
	pszBUF=(TCHAR*)(LPCTSTR)vvalue; // 값 
	RegSetValueEx(HKey2, (LPCTSTR)vname, NULL, REG_SZ, (const BYTE*)pszBUF, (lstrlen(pszBUF)+1)*sizeof(TCHAR));


	RegCloseKey(HKey2);
}

void CArx_User::gf_SetRegistry(CString vname, CString vvalue)
{
	// 비스타에서는 기본적으로 "HKEY_CURRENT_USER"만 쓰기 허락을 한다.
	gf_SetRegistry(HKEY_CURRENT_USER, _T("DCS_COME"), ComeVar::ComeVersion, vname, vvalue);
}

void CArx_User::gf_SetRegistry(HKEY hKey, CString vname, CString vvalue)
{
	gf_SetRegistry(hKey, _T("DCS_COME"), ComeVar::ComeVersion, vname, vvalue);
}


CString CArx_User::gf_GetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString& vvalue)
{
HKEY HKey2;
	CString sCompany, sProduct;
	CString strValue;


	// 레지스트리 등록
	sCompany.Format(_T("Software\\%s"),strCompany);			// 회사명
	sProduct.Format(_T("%s\\%s"),sCompany,strProduct);			// 제품명
	RegCreateKey(hKey, sProduct, &HKey2);

	// KEY값을 레지스트리로부터 읽어온다.
	DWORD dwType, dwCount;
	LPCTSTR lpszEntry=(LPCTSTR)vname;
	LONG lResult = RegQueryValueEx(HKey2, (LPTSTR)lpszEntry, NULL, &dwType,
		NULL, &dwCount);
	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_SZ);
		lResult = RegQueryValueEx(HKey2, (LPTSTR)lpszEntry, NULL, &dwType,
			(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
		strValue.ReleaseBuffer();
	}
	RegCloseKey(HKey2);

	vvalue=strValue;  // 값
	return strValue;
}

CString CArx_User::gf_GetRegistry(CString vname, CString& vvalue)
{
	return gf_GetRegistry(HKEY_CURRENT_USER, _T("DCS_COME"), ComeVar::ComeVersion, vname, vvalue);
}
CString CArx_User::gf_GetRegistry(HKEY hKey, CString vname, CString& vvalue)
{
	return gf_GetRegistry(hKey, _T("DCS_COME"), ComeVar::ComeVersion, vname, vvalue);
}




//블록의 속성(색상, 라인타입)을 추출하여 셋팅
Acad::ErrorStatus CArx_User::BlockAttSet(AcDbDatabase *pD)
{

	AcDbLayerTable *pLayerTable;
	AcDbLayerTableIterator* pIterator;
	AcDbLayerTableRecord* pRecord;
	TCHAR* name;
	AcCmColor color;
	AcDbObjectId linetypeId;
	int colorindex;

    if ((es=pD->getLayerTable(pLayerTable, AcDb::kForRead)) != Acad::eOk) {
		ads_printf(_T("\nError : Layer table"));
		return es;
    }
	es=pLayerTable->newIterator(pIterator,Adesk::kTrue,Adesk::kTrue);
	pLayerTable->close();

	for (pIterator->start(); !pIterator->done();pIterator->step()){
		if((es=pIterator->getRecord(pRecord, AcDb::kForRead))==Acad::eOk){
			pRecord->getName(name);    //layer name
			color = pRecord->color();  //layer color
			colorindex = color.colorIndex(); /// layer 각각에 대한  color
			linetypeId = pRecord->linetypeObjectId();  //layer linetypeid

	//createLayer(name,linetypeId,color);
			pRecord->close();
		}
	}
	delete pIterator;
	return es;
}
//150이상 관경(200=2A,250=2F)
CString CArx_User::gf_150Over(int bore1, int bore2)
{
	int na1,na2;
	CString mid1,mid2;

	if(bore1 > 150){
		na1=bore1%100;
		if(na1==0)
			mid1 = valtoa(bore1).Left(1)+_T("A");
		else
			mid1 = valtoa(bore1).Left(1)+_T("F");
	}else{
		mid1=valtoa(bore1).Left(2);
	}

	if(bore2 > 150){
		na2=bore2%100;
		if(na2==0)
			mid2 = valtoa(bore2).Left(1)+_T("A");
		else
			mid2 = valtoa(bore2).Left(1)+_T("F");
	}else{
		mid2=valtoa(bore2).Left(2);
	}

	return mid1+mid2;
}

//블록내 포인트 추출
struct resbuf* CArx_User::GetPntBlock(ads_name blk_ename)
{
	struct resbuf *lst=NULL, *tmplst=NULL;
	struct resbuf *from=NULL, *to=NULL, *pntl=NULL;
	CString		blkname, etype;
	ads_name	ben,ent;
	ads_real    d41, d42, d43;
	ads_point   pnt;
	TCHAR        str[32];
	int  rc;

	namecpy(ent, blk_ename);

	if((lst=ads_tblsearch(_T("UCS"), _T("OLDUCS"),1))){
		ads_command(RTSTR, _T(".UCS"),
		            RTSTR, _T("D"), 
					RTSTR, _T("OLDUCS"),
					0);
		ads_relrb(lst);
		lst=NULL;
	}//end-if
	ads_command(RTSTR, _T(".UCS"),
	            RTSTR, _T("S"), 
				RTSTR, _T("OLDUCS"),
				0);

	ads_command(RTSTR,   _T(".UCS"),
	            RTSTR,   _T("E"), 
				RTENAME, ent,
				0);

	if(!ads_name_nil(ent))tmplst=lst=ads_entget(ent);
	else tmplst=lst=NULL;
	memset(str, 0, sizeof(str));
	if(lst){
		_tcscpy(str, S(ads_assoc(2, lst)));  // 블럭 이름
		d41=R(ads_assoc(41, lst));   // x 길이
		d42=R(ads_assoc(42, lst));   // y
		d43=R(ads_assoc(43, lst));   // z
		ads_relrb(tmplst);
		tmplst=lst=NULL;
	}//end-if
	
	lst=ads_tblsearch(_T("BLOCK"), str,1);
	if(!lst) return NULL;
	namecpy(ben, E(ads_assoc(-2, lst)));
	ads_relrb(lst);
	lst=NULL;

	from=ads_newrb(RTSHORT);
	to  =ads_newrb(RTSHORT);
	from->resval.rint=1;
	to->resval.rint=0;

	if(!ads_name_nil(ben)) tmplst=lst=ads_entget(ben);
	else tmplst=lst=NULL;
	while(lst){
		etype=S(ads_assoc(0, lst));
		if(etype==_T("POINT")){
			ptcpy(pnt, P(ads_assoc(10, lst)));
			pnt[0]=pnt[0]*d41;
			pnt[1]=pnt[1]*d42;
			pnt[2]=pnt[2]*d43;
			ads_trans(pnt, from, to, 0, pnt);
			if(!pntl) pntl=tmplst=ads_newrb(RT3DPOINT);
			else tmplst=tmplst->rbnext=ads_newrb(RT3DPOINT);
			ptcpy(P(tmplst), pnt);
		}//end-if
		ads_relrb(lst);
		lst=NULL;
		rc=ads_entnext(ben, ben);
		if(!ads_name_nil(ben)){ 
			if(rc==RTNORM) lst=ads_entget(ben);
			else lst=NULL;
		}else{
			lst=NULL;
		}
	
	}//end-while
	ads_relrb(from);
	ads_relrb(to);

	ads_command(RTSTR, _T(".UCS"),
	            RTSTR, _T("R"), 
				RTSTR, _T("OLDUCS"),
				0);
	ads_command(RTSTR, _T(".UCS"),
	            RTSTR, _T("D"), 
				RTSTR, _T("OLDUCS"),
				0);

	return pntl;
}//end-GetPntBlock


//객체의 Layer가 Lock인가?
bool isOnLockedObj(AcDbObjectId objId)
{
    AcDbObject* obj;
    AcDbLayerTableRecord* layer;
    bool isLocked = false;
    Acad::ErrorStatus es;


	AcDbEntity* ent=NULL;
    es = acdbOpenObject(ent, objId, AcDb::kForRead);	
    if (es != Acad::eOk) return true;

    es = acdbOpenAcDbObject(obj, ent->layerId(), AcDb::kForRead);
    if (es == Acad::eOk) {
        layer = AcDbLayerTableRecord::cast(obj);
        if (layer)
            isLocked = layer->isLocked();
        else {
            ASSERT(0);
        }
        obj->close();
    }//end-if
	ent->close();


    return isLocked;
}

//객체의 Layer가 Lock인가?(ads_name)
bool isOnLockedObj(ads_name ename)
{
	AcDbObjectId objId=NULL;
	AcDbEntity *pEnt=NULL;
    bool isLocked = FALSE;
	

	acdbGetObjectId(objId ,ename);

	isLocked = isOnLockedObj(objId);

	return isLocked;
}

//LOCK Layer 제거
void ssNoLock(ads_name &tss)
{
	ads_name ename;
	long i=0;
	
	if (ads_sslength(tss,&i)!= RTNORM)return;

	for(long j=0;j<i;j++){
		if(ads_ssname(tss,j,ename)!=RTNORM)continue;
		if(isOnLockedObj(ename)) ads_ssdel(ename, tss);
	}//end-for
}


//현재 Layer가 Lock인가?
bool isOnLockedCurLayer()
{

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL)
		return false;


    AcDbObject* obj;
    AcDbLayerTableRecord* layer;
    int isLocked = 0;
    Acad::ErrorStatus es;



    es = acdbOpenAcDbObject(obj, pDb->clayer(), AcDb::kForRead);
    if (es == Acad::eOk) {
        layer = AcDbLayerTableRecord::cast(obj);
        if (layer)
            isLocked = layer->isLocked();
        else {
            ASSERT(0);
        }
        obj->close();
    }
    else {
        ASSERT(0);
		ads_printf(_T("Error->isOnLockedCurLayer!"));
    }
	if(isLocked)
		AfxMessageBox(_T("Current LAYER is locked. \nPlease unlock the layer."));

	return (isLocked==1);
}

