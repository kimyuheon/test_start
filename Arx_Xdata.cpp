// Arx_Xdata.cpp: implementation of the CArx_Xdata class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Arx_Draw.h"
#include "Arx_Math.h"
#include "Arx_User.h"
#include "Arx_Xdata.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArx_Xdata::CArx_Xdata()
{
}

CArx_Xdata::~CArx_Xdata()
{
}

CString CArx_Xdata::gfreturn_handle(ads_name lst_name)
{
	AcDbObjectId objId;
	AcDbHandle handle;
	TCHAR buf[50];
	buf[0]=_T('\0');

	AcDbObject* pObj;

	es=acdbGetObjectId(objId,lst_name);
	acdbOpenObject(pObj, objId, AcDb::kForRead);
	pObj->getAcDbHandle(handle);
	handle.getIntoAsciiBuffer(buf);
	pObj->close();



	return (CString)buf;
}


AcDbObjectId CArx_Xdata::entitySSelect(AcGePoint3d pt1,
									   AcGePoint3d pt2,
									   TCHAR *str)
{
	ads_point p1,p2;

	p1[X]=pt1[0];p1[Y]=pt1[1];p1[Z]=pt1[2];
	p2[X]=pt2[0];p2[Y]=pt2[1];p2[Z]=pt2[2];

	return entitySSelect(p1,p2,str);
}
AcDbObjectId CArx_Xdata::DLentitySSelect(AcGePoint3d pt1,
									   AcGePoint3d pt2,
									   TCHAR *str)
{
	ads_point p1,p2;

	p1[X]=pt1[0];p1[Y]=pt1[1];p1[Z]=pt1[2];
	p2[X]=pt2[0];p2[Y]=pt2[1];p2[Z]=pt2[2];

	return DLentitySSelect(p1,p2,str);
}

//객체 검색
AcDbObjectId CArx_Xdata::entitySSelect(const ads_point pt1,
									 const ads_point pt2,
									 TCHAR *str)
{
	ads_name ename,m_ss;
	long sslen;
	AcDbObjectId objId;
	ads_point p1,p2;
	CArx_Math m_Math;
	CArx_Draw m_Draw;
	CArx_User m_User;
	int old_pickbox;
	AcGePoint2d screenpt;
	struct resbuf *sys1=NULL, *sys2=NULL;
	ads_real vs, ss, yr1;
	CString s;
	int es=0;

	// Pick Box Size
	//********************************************************************
    sys1=ads_newrb(RTREAL);       /* sys1 must the allocated memory */
	sys2=ads_newrb(RT3DPOINT);

    ads_getvar(_T("VIEWSIZE"), sys1);     /* real */
	ads_getvar(_T("SCREENSIZE"), sys2);   /* point (real real) */

    vs=sys1->resval.rreal;            /* view size */
	ss=sys2->resval.rpoint[Y];        /* screensize */
	ads_relrb(sys1);

	ads_relrb(sys2);


	yr1=vs/ss*5;                    /* real size of pickbox in screen 5 */
    //******************************************************************

	es=m_Draw.getSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), 1);

	ads_polar(pt2,m_Math.DTR(45), yr1, p1);
	ads_polar(pt2,m_Math.DTR(225), yr1, p2);

//ads_command(RTSTR, "LINE",RTPOINT,p1,RTPOINT,p2,RTSTR,"",RTNONE);


	if (ads_ssget(_T("C"), p1, p2, NULL, m_ss) != RTNORM){
		es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
		return NULL;
	}
	ssNoLock(m_ss);
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);

	if (ads_sslength(m_ss, &sslen) != RTNORM)
		return NULL;
	double minVal=yr1*30.0;
	double dst=(double)0;
	AcGePoint3d sp, ep, gpt1, gpt2;
	gpt1.set(pt1[0],pt1[1],pt1[2]);
	AcDbObjectId clostId;
	clostId=NULL;

	for(int index=0;index < sslen;index++){
		if(ads_ssname(m_ss,index,ename) == RTNORM){
			lf_SReturnList(ename,0,s);
			acdbGetObjectId(objId,ename);
			if(s==_T("LINE") && str==_T("LINE")){ //선분인 경우
				m_User.arx_getLine(objId, sp, ep);
				m_Math.getclosestPoint(sp, ep, gpt1, gpt2);
				dst=gpt1.distanceTo(gpt2);
				if(dst<minVal && s==str) {
					clostId=objId;
					minVal=dst;
				}//end-if
			}else if(s==str){ //선분 이외
				clostId=objId;
				break;
			}//end-if
		}
	}//end-for 
	ads_ssfree(m_ss);
	ads_ssfree(ename);
	return clostId;
}


AcDbObjectId CArx_Xdata::entitySSelect(AcGePoint3d pt,
									   TCHAR *str,
									   resbuf* filter)
{
	ads_point p1;

	p1[X]=pt[0];p1[Y]=pt[1];p1[Z]=pt[2];

	return entitySSelect(p1,str,filter);
}

AcDbObjectId CArx_Xdata::entitySSelect(const ads_point pt,
									 TCHAR *str,
									 resbuf* filter)
{
	ads_name ename,m_ss;
	long sslen;
	AcDbObjectId objId;
	ads_point p1,p2;
	CArx_Math m_Math;
	CArx_Draw m_Draw;
	int old_pickbox;
	double viewr=(double)0,yr1=(double)0;
	AcGePoint2d screenpt;
	CString s;

	es=m_Draw.getSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), 1);
	es=m_Draw.getSysVar((LPCTSTR)_T("VIEWSIZE"),viewr);
	es=m_Draw.getSysVar((LPCTSTR)_T("SCREENSIZE"),screenpt);
	
	yr1 = viewr / screenpt[1];
	ads_polar(pt,m_Math.DTR(45), yr1, p1);
	ads_polar(pt,m_Math.DTR(225), yr1, p2);

	if (ads_ssget(_T("C"), p1, p2, filter, m_ss) != RTNORM){
		es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
		return NULL;
	}
	ssNoLock(m_ss);
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);

	if (ads_sslength(m_ss, &sslen) != RTNORM)
		return NULL;
	int index = 0;
	while(index < sslen){
		if(ads_ssname(m_ss,index,ename) == RTNORM){
			acdbGetObjectId(objId,ename);
			lf_SReturnList(ename,0,s);
			if(s==str){
				ads_ssfree(m_ss);
				ads_ssfree(ename);
				return objId;
			}
		}
		index++;
	} 

	ads_ssfree(m_ss);
	ads_ssfree(ename);

	return NULL;
}

AcDbObjectId CArx_Xdata::AllentitySSelect(AcGePoint3d pt1,
									   AcGePoint3d pt2)
{
	ads_point p1,p2;

	p1[X]=pt1[0];p1[Y]=pt1[1];p1[Z]=pt1[2];
	p2[X]=pt2[0];p2[Y]=pt2[1];p2[Z]=pt2[2];

	return AllentitySSelect(p1,p2);
}

AcDbObjectId CArx_Xdata::AllentitySSelect(const ads_point pt1,
									 const ads_point pt2)
{
	ads_name ename,m_ss;
	long sslen;
	AcDbObjectId objId;
	ads_point p1,p2;
	CArx_Math m_Math;
	CArx_Draw m_Draw;
	int old_pickbox;
	double viewr=(double)0,yr1=(double)0;
	AcGePoint2d screenpt;

	es=m_Draw.getSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), 1);
	es=m_Draw.getSysVar((LPCTSTR)_T("VIEWSIZE"),viewr);
	es=m_Draw.getSysVar((LPCTSTR)_T("SCREENSIZE"),screenpt);
	
	yr1 = viewr / screenpt[1];
	ads_polar(pt1,m_Math.DTR(45), yr1, p1);
	ads_polar(pt2,m_Math.DTR(225), yr1, p2);

	if (ads_ssget(_T("C"), p1, p2, NULL, m_ss) != RTNORM){
		es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
		return NULL;
	}
	ssNoLock(m_ss);
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);

	if (ads_sslength(m_ss, &sslen) != RTNORM)
		return NULL;
	if(ads_ssname(m_ss,0,ename) == RTNORM){
		acdbGetObjectId(objId,ename);
		ads_ssfree(m_ss);
		ads_ssfree(ename);
		return objId;
	} 

	ads_ssfree(m_ss);
	ads_ssfree(ename);

	return NULL;
}

AcDbObjectId CArx_Xdata::DLentitySSelect(ads_point pt1,
									 ads_point pt2,
									 TCHAR *str)
{
	ads_name ename,m_ss;
	long sslen;
	AcDbObjectId objId;
	ads_point p1,p2;
	CArx_Math m_Math;
	CArx_Draw m_Draw;
	CArx_User m_User;
	int old_pickbox;
	AcGePoint2d screenpt;
	AcGePoint3d spt,ept;
	double angle=(double)0,angle1=(double)0;
	struct resbuf *sys1=NULL, *sys2=NULL;
	ads_real vs, ss, yr1;
	CString s;

	// Pick Box Size
	//********************************************************************
    sys1=ads_newrb(RTREAL);       /* sys1 must the allocated memory */
	sys2=ads_newrb(RT3DPOINT);
    ads_getvar(_T("VIEWSIZE"), sys1);     /* real */
	ads_getvar(_T("SCREENSIZE"), sys2);   /* point (real real) */
    vs=sys1->resval.rreal;            /* view size */
	ss=sys2->resval.rpoint[Y];        /* screensize */
	ads_relrb(sys1);
	ads_relrb(sys2);

	yr1=vs/ss * 4;                    /* real size of pickbox in screen */
    //******************************************************************
	m_User.ads_arxpt(spt,pt1);m_User.ads_arxpt(ept,pt2);
	angle = floor(m_Math.arx_angleA(spt,ept));

	es=m_Draw.getSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), 1);

	ads_polar(pt2,m_Math.DTR(45), yr1, p1);
	ads_polar(pt2,m_Math.DTR(225), yr1, p2);
	if (ads_ssget(_T("C"), p1, p2, NULL, m_ss) != RTNORM){
		es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
		return NULL;
	}
	ssNoLock(m_ss);
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);

	if (ads_sslength(m_ss, &sslen) != RTNORM)
		return NULL;
	
	int index = 0;
	while(index < sslen){
		if(ads_ssname(m_ss,index,ename) == RTNORM){
			acdbGetObjectId(objId,ename);
			lf_SReturnList(ename,0,s);
			if(s==str){
				spt = lf_3DReturnList(ename,10);
				ept = lf_3DReturnList(ename,11);
				angle1 = floor(m_Math.arx_angleA(spt,ept));
				if(fabs(angle-angle1)<0.01){
					ads_ssfree(m_ss);
					ads_ssfree(ename);

					return objId;
				}
			}
		}
		index++;
	} 
	ads_ssfree(m_ss);
	ads_ssfree(ename);
	return NULL;
}

void  CArx_Xdata::AllentitySSelect(const ads_point pt1,
									 const ads_point pt2,
									 ads_name& _trename)
{
	ads_point p1,p2;
	CArx_Math m_Math;
	CArx_Draw m_Draw;
	int old_pickbox;
	double viewr=(double)0,yr1=(double)0;
	AcGePoint2d screenpt;

	es=m_Draw.getSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), 1);
	es=m_Draw.getSysVar((LPCTSTR)_T("VIEWSIZE"),viewr);
	es=m_Draw.getSysVar((LPCTSTR)_T("SCREENSIZE"),screenpt);
	
	yr1 = viewr / screenpt[1];
	ads_polar(pt1,m_Math.DTR(45), yr1, p1);
	ads_polar(pt2,m_Math.DTR(225), yr1, p2);

	ads_ssget(_T("C"), p1, p2, NULL, _trename);
	ssNoLock(_trename);
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
	return;
}

//동일한 객체인가?
bool CArx_Xdata::entityEqual(ads_name en1,
							 ads_name en2)
{
	AcDbObjectId objId1,objId2;
	
	acdbGetObjectId(objId1,en1);
	acdbGetObjectId(objId2,en2);
	if(objId1 == objId2)
		return true;
	else
		return false;
}


void CArx_Xdata::lf_Xdata(ads_name ename,resbuf*& pRb)
{
	
	CString strResource;
	AcDbObject* pObj;
	AcDbObjectId objId;

	acdbGetObjectId(objId,ename);
	if ((es = acdbOpenObject(pObj,objId,AcDb::kForRead)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING130);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
//       ads_printf(_T("\n CArx_Xdata::lf_Xdata 선택이 잘못되었습니다.\n"));
        return;
    }
	pRb=pObj->xData(NULL);
	pObj->close();
}

long CArx_Xdata::lf_LReturnList(ads_name ename,int type)
{
	struct resbuf* pRb=NULL,*pHead=NULL;
    int rt=0;
	long ret=0;

	if(ads_name_nil(ename)) return 0;
	if(type < 1000) pRb=ads_entget(ename);
	else
		lf_Xdata(ename,pRb);
	pHead=pRb;
	while ((pRb != NULL) && (pRb->restype != type)){
		pRb = pRb->rbnext;
	}
	if(pRb != NULL)
		rt=type_return(pRb->restype);

    switch (rt) {
    case RTLONG:
        ret = pRb->resval.rlong;
        break;
	default:
		ret = 0;
    }
	if(pHead) ads_relrb(pHead);	

	return ret;
}


// 리스트에서 ename 객체를 찾아 type에 해당하는 데이터를 얻어옴
// 관경이 정수형(1070)이고 관경값을 찾아온다
int CArx_Xdata::lf_IReturnList(ads_name ename,	// 
							   int type)		// 타입
{
	struct resbuf* pRb=NULL,*pHead=NULL;
    int ret=0,rt=0;

	// 객체가 없으면 종료
	if(ads_name_nil(ename))
		return 0;

	// 객체를 얻어옴
	if(type < 1000)
		pRb=ads_entget(ename);
	else // 1000이상일때
		lf_Xdata(ename,pRb);
	
	// 포인터 대입
	pHead=pRb;

	// 리스트의 끝까지 type을 찾는다
	while((pRb != NULL) && (pRb->restype != type))
	{
		pRb = pRb->rbnext;
	}
	// 리스트의 끝일때
	if(pRb != NULL)
		rt=type_return(pRb->restype);

    switch (rt)
	{
	case RTSHORT:  // short일때 16비트 정수일때
		// resval에 저장된 rint값을 대입한다
		ret = pRb->resval.rint;
		break;
	default:
		ret=0;
    }
  
	if(pHead) ads_relrb(pHead);	

	return ret;
}


void CArx_Xdata::lf_SReturnList(ads_name ename,int type, CString &buffer)
{
	struct resbuf* pRb=NULL, *pHead=NULL;
    int rt=0;

	buffer=_T("");

	if(ads_name_nil(ename)) return;
	if(type < 1000)	pRb = ads_entget(ename);
	else			lf_Xdata(ename,pRb);
	pHead=pRb;

	while ((pRb != NULL) && (pRb->restype != type))	pRb = pRb->rbnext;	

	if(pRb != NULL)	rt=type_return(pRb->restype);
	
    switch (rt) {
    case RTSTR:
		buffer.Format(_T("%s"), pRb->resval.rstring);
        break;
    }
	if(pHead) ads_relrb(pHead);	
}

void CArx_Xdata::lf_SHReturnList(ads_name ename,int type, CString& buffer)
{
	struct resbuf* pRb=NULL, *pHead=NULL;
    int rt=0;
	
	buffer=_T("");

	if(ads_name_nil(ename)) return;
	if(type < 1000)	pRb = ads_entget(ename);
	else
		lf_Xdata(ename,pRb);
	pHead=pRb;
	while ((pRb != NULL) && (pRb->restype != type)){
		pRb = pRb->rbnext;
	}
	pRb = pRb->rbnext;
	pRb = pRb->rbnext;

	if(pRb != NULL)
		rt=type_return(pRb->restype);

    switch (rt) {
    case RTSTR:
		buffer.Format(_T("%s"),pRb->resval.rstring); 
        break;
    }
	if(pHead)ads_relrb(pHead);	
}

CString CArx_Xdata::lf_PCIndexReturn(ads_name ename,int index)
{
	
	CString strResource;
	struct resbuf* pRb=NULL,*pHead=NULL;
    TCHAR buf[133];
	CString retval;
	buf[0]=_T('\0');
	AcDbObject* pObj;
	AcDbObjectId objId;
	retval.Empty();
	acdbGetObjectId(objId,ename);

	if ((es = acdbOpenObject(pObj,objId,AcDb::kForRead)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING131);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
 //       ads_printf(_T("\nCArx_Xdata::lf_PCIndexReturn 선택이 잘못되었습니다.\n"));
        return retval;
    }
	pHead=pRb=pObj->xData(_T("PC"));
	pObj->close();

	if(pRb != NULL){
		while ((pRb != NULL) && (pRb->restype != 1000)){
			pRb = pRb->rbnext;
		}
		switch(index){
		case 1:
			_stprintf(buf,_T("%s"), pRb->resval.rstring);
			break;
		case 2:
			pRb = pRb->rbnext;
			_stprintf(buf,_T("%s"), pRb->resval.rstring);
			break;
		case 3:
			pRb = pRb->rbnext;pRb = pRb->rbnext;
			_stprintf(buf,_T("%s"), pRb->resval.rstring);
			break;
		case 4:
			pRb = pRb->rbnext;pRb = pRb->rbnext;pRb = pRb->rbnext;
			_stprintf(buf,_T("%s"), pRb->resval.rstring);
			break;
		default:
			_stprintf(buf,_T("%s"), _T(""));
			break;
		}
		retval = buf;
		
	}

	if(pHead) ads_relrb(pHead);	
	return retval;
}
int CArx_Xdata::lf_FPIPIndexReturn(ads_name ename,int index)
{
	
	CString strResource;
	struct resbuf* pRb=NULL,*pHead=NULL;
	int bore=0;
	AcDbObject* pObj;
	AcDbObjectId objId;
	
	acdbGetObjectId(objId,ename);
	if ((es = acdbOpenObject(pObj,objId,AcDb::kForRead)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING132);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
//        ads_printf(_T("\nCArx_Xdata::lf_FPIPIndexReturn 선택이 잘못되었습니다.\n"));
        return bore;
    }
	pHead=pRb=pObj->xData(_T("FPIP"));
	pObj->close();
	
	if(pRb != NULL){
		while ((pRb != NULL) && (pRb->restype != 1070)){
			pRb = pRb->rbnext;
		}
		switch(index){
		case 1:
			bore= pRb->resval.rint;
			break;
		case 2:
			pRb = pRb->rbnext;
			bore= pRb->resval.rint;
			break;
		default:
			bore=0;
			break;
		}
	}

	if(pHead) ads_relrb(pHead);	
	return bore;
}

CString CArx_Xdata::lf_FIREStrReturn(ads_name ename)
{
	
	CString strResource;
	struct resbuf* pRb=NULL,*pHead=NULL;
    TCHAR buf[133];
	//char *buffer;
	//buffer = (char*) malloc(255);
	//strcpy(buffer,"");
	CString retval;
	buf[0]=_T('\0');
	AcDbObject* pObj;
	AcDbObjectId objId;
	retval.Empty();
	acdbGetObjectId(objId,ename);
	if ((es = acdbOpenObject(pObj,objId,AcDb::kForRead)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING133);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
 //       ads_printf(_T("\nCArx_Xdata::lf_FIREStrReturn 선택이 잘못되었습니다.\n"));
        return retval;
    }
	pHead=pRb=pObj->xData(_T("FIRHNO"));
	pObj->close();

	if(pRb != NULL){
		while ((pRb != NULL) && (pRb->restype != 1000)){
			pRb = pRb->rbnext;
		}
		_stprintf(buf,_T("%s"), pRb->resval.rstring);
		//strcpy(buffer,buf);
		retval = buf;
	}

	if(pHead) ads_relrb(pHead);	
	return retval;
}

AcGePoint3d CArx_Xdata::lf_FIRE3DReturn(ads_name ename)
{
	
	CString strResource;
	struct resbuf* pRb=NULL,*pHead=NULL;
    AcGePoint3d retpt;
	
	AcDbObject* pObj;
	AcDbObjectId objId;

	acdbGetObjectId(objId,ename);
	if ((es = acdbOpenObject(pObj,objId,AcDb::kForRead)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING134);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
//        ads_printf(_T("\nAcGePoint3d CArx_Xdata::lf_FIRE3DReturn 선택이 잘못되었습니다.\n"));
        return retpt;
    }
	pHead=pObj->xData(_T("FIRHNO"));
	pObj->close();

	if(pRb != NULL){
		while ((pRb != NULL) && (pRb->restype != 1010)){
			pRb = pRb->rbnext;
		}
		retpt[0]=pRb->resval.rpoint[X];
		retpt[1]=pRb->resval.rpoint[Y];
		retpt[2]=pRb->resval.rpoint[Z];
	}

	if(pHead) ads_relrb(pHead);	
	return retpt;
}




// ads_name으로 Entget해서 확장 엔티티(Point형) 추출(By Type)
AcGePoint3d CArx_Xdata::lf_3DReturnList(ads_name ename,int type)
{
	AcGePoint3d retpt;
	struct resbuf* pRb=NULL, *pHead=NULL;
    int rt=0;

	if(ads_name_nil(ename)) return retpt;
	if(type < 1000)
		pHead=pRb=ads_entget(ename);
	else
		lf_Xdata(ename,pRb);
	
	pHead=pRb;
	while ((pRb != NULL) && (pRb->restype != type)){
		pRb = pRb->rbnext;
	}
	if(pRb != NULL)
		rt=type_return(pRb->restype);

    switch (rt) {
	case RT3DPOINT:
		retpt[0]=pRb->resval.rpoint[X];
		retpt[1]=pRb->resval.rpoint[Y];
		retpt[2]=pRb->resval.rpoint[Z];
		break;
	default:
		//retpt.set(0,0,0);
		break;
    }

	if(pHead)ads_relrb(pHead);	

	return retpt;
}





// ads_name으로 Entget해서 확장 엔티티(Real형) 추출(By Type)
///Return for Real value
double CArx_Xdata::lf_RReturnList(ads_name ename,int type)
{
	struct resbuf* pRb=NULL, *pHead=NULL;
    int rt=0;
    double retval=0.0;

	if(ads_name_nil(ename)) return retval;
	if(type < 1000)
		pRb = ads_entget(ename);
	else
		lf_Xdata(ename,pRb);
	
	pHead=pRb;
	while ((pRb != NULL) && (pRb->restype != type)){
		pRb = pRb->rbnext;
	}
	if(pRb != NULL)
		rt=type_return(pRb->restype);
	
    switch (rt) {
	case RTREAL :
       	retval=pRb->resval.rreal;
		break;
	default:
		retval=0.0;
    }
	if(pHead)	ads_relrb(pHead);	

	return retval;
}





int CArx_Xdata::type_return(int val)
{

	if (val < 10) {
        return RTSTR;
	} else if (val < 38) {
        return RT3DPOINT;
	}else if (val < 60) {
        return RTREAL;
	}else if (val < 1010) {
        return RTSTR;
    } else if (val < 1040) {
        return RT3DPOINT;
    } else if (val < 1060) {
        return RTREAL;
    } else if (val < 1071) {
        return RTSHORT;
    } else if (val == 1071) {
        return RTLONG;
    }

	return -100;
}
void CArx_Xdata::gl_AddXdata(ads_name ename,
							  TCHAR saveName[],
							  int dia1,
							  int dia2)
{
	AcDbObjectId objId;
	acdbGetObjectId(objId,ename);
	gl_AddXdata(objId,saveName,dia1,dia2);
}
void CArx_Xdata::gl_AddXdata(AcDbObjectId eId,
							  TCHAR saveName[],
							  int dia1,
							  int dia2)
{
	
	CString strResource;
	Acad::ErrorStatus retStat;
	AcDbObject* pObj;
	
	if ((retStat = acdbOpenObject(pObj,eId,AcDb::kForWrite)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING135);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
		//ads_printf(_T("\n void CArx_Xdata::gl_AddXdata 선택이 잘못되었습니다.\n"));
        return;
    }
 
    TCHAR appName[100];
    appName[0] =_T('\0');
 
	_tcscpy(appName,saveName);
    
    struct  resbuf  *pRb, *pTemp;
    
    pRb = pObj->xData(appName);
 
    if (pRb != NULL) {
		pTemp = pRb;
		while ((pTemp->rbnext != NULL) && (pTemp->restype != 1070)){
			pTemp = pTemp->rbnext;
		}
		pTemp->resval.rint = dia1;
		pTemp = pTemp->rbnext;
		pTemp->resval.rint = dia2;
		while (pTemp->rbnext != NULL){
			pTemp = pTemp->rbnext;
		}
    } else {
        ads_regapp(appName);
 
        pRb = ads_newrb(AcDb::kDxfRegAppName);
        pTemp = pRb;

        pTemp->resval.rstring
            = (TCHAR*) malloc((_tcslen(appName)+1)*sizeof(TCHAR));
        _tcscpy(pTemp->resval.rstring, appName);
		pTemp->rbnext = ads_newrb(AcDb::kDxfXdInteger16);
		pTemp = pTemp->rbnext;
		pTemp->restype = 1070;
		pTemp->resval.rint = dia1;
		
		pTemp->rbnext = ads_newrb(AcDb::kDxfXdInteger16);
		pTemp = pTemp->rbnext;
		pTemp->restype = 1070;
		pTemp->resval.rint = dia2;
    }
	

    pObj->upgradeOpen();
    pObj->setXData(pRb);
    
    pObj->close();

    if(pRb) ads_relrb(pRb);

}

void CArx_Xdata::gl_AddXdata(ads_name ename,
							  TCHAR saveName[],
							  TCHAR Str1[],
							  TCHAR Str2[],
							  TCHAR Str3[],
							  TCHAR Str4[])
{
	AcDbObjectId objId;
	acdbGetObjectId(objId,ename);
	gl_AddXdata(objId,saveName,Str1,Str2,Str3,Str4);
}
void CArx_Xdata::gl_AddXdata(AcDbObjectId eId,
							  TCHAR saveName[],
							  TCHAR Str1[],
							  TCHAR Str2[],
							  TCHAR Str3[],
							  TCHAR Str4[])
{
	
	CString strResource;
	Acad::ErrorStatus retStat;
	AcDbObject* pObj;
	
	if ((retStat = acdbOpenObject(pObj,eId,AcDb::kForWrite)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING136);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
//        ads_printf(_T("\nvoid CArx_Xdata::gl_AddXdata 선택이 잘못되었습니다.\n"));
        return;
    }
 
    TCHAR appName[100], resStr1[30],resStr2[30],resStr3[30],resStr4[30];
    appName[0] = resStr1[0] = resStr2[0] =resStr3[0] =resStr4[0] =_T('\0');
 
	_tcscpy(appName,saveName);
	_tcscpy(resStr1,Str1);_tcscpy(resStr2,Str2);_tcscpy(resStr3,Str3);_tcscpy(resStr4,Str4);
    
    struct  resbuf  *pRb, *pTemp;
    
    pRb = pObj->xData(appName);
 
    if (pRb != NULL) {
        pTemp = pRb;
		while ((pTemp->rbnext != NULL) && (pTemp->restype != 1000)){
			pTemp = pTemp->rbnext;
		}
		pTemp->resval.rstring = (TCHAR*) malloc((_tcslen(resStr1)+1)*sizeof(TCHAR));
		_tcscpy(pTemp->resval.rstring, resStr1);
		pTemp = pTemp->rbnext;
		pTemp->resval.rstring = (TCHAR*) malloc((_tcslen(resStr2)+1)*sizeof(TCHAR));
		_tcscpy(pTemp->resval.rstring, resStr2);
		pTemp = pTemp->rbnext;
		pTemp->resval.rstring  = (TCHAR*) malloc((_tcslen(resStr3)+1)*sizeof(TCHAR));
		_tcscpy(pTemp->resval.rstring, resStr3);
		pTemp = pTemp->rbnext;
		pTemp->resval.rstring = (TCHAR*) malloc((_tcslen(resStr4)+1)*sizeof(TCHAR));
		_tcscpy(pTemp->resval.rstring, resStr4);
		while (pTemp->rbnext != NULL){
			pTemp = pTemp->rbnext;
		}
    } else {
        ads_regapp(appName);
 
        pRb = ads_newrb(AcDb::kDxfRegAppName);
        pTemp = pRb;

        pTemp->resval.rstring
            = (TCHAR*) malloc((_tcslen(appName)+1)*sizeof(TCHAR));
        _tcscpy(pTemp->resval.rstring, appName);
		pTemp->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
		pTemp = pTemp->rbnext;
		pTemp->restype = 1000;
		pTemp->resval.rstring = (TCHAR*) malloc((_tcslen(resStr1)+1)*sizeof(TCHAR));
		_tcscpy(pTemp->resval.rstring, resStr1);
 
		pTemp->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
		pTemp = pTemp->rbnext;
		pTemp->restype = 1005;
		pTemp->resval.rstring = (TCHAR*) malloc((_tcslen(resStr2)+1)*sizeof(TCHAR));
		_tcscpy(pTemp->resval.rstring, resStr2);
 
		pTemp->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
		pTemp = pTemp->rbnext;
		pTemp->restype = 1005;
		pTemp->resval.rstring  = (TCHAR*) malloc((_tcslen(resStr3)+1)*sizeof(TCHAR));
		_tcscpy(pTemp->resval.rstring, resStr3);
 
		pTemp->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
		pTemp = pTemp->rbnext;
		pTemp->restype = 1005;
		pTemp->resval.rstring = (TCHAR*) malloc((_tcslen(resStr4)+1)*sizeof(TCHAR));
		_tcscpy(pTemp->resval.rstring, resStr4);
 
    }
 
    pObj->upgradeOpen();
    pObj->setXData(pRb);
    
    pObj->close();

    if(pRb) ads_relrb(pRb);

}

void CArx_Xdata::gl_AddXdata(ads_name ename,
							  TCHAR saveName[],
							  TCHAR Str1[],
							  int  dia,
							  double dia1)
{
	AcDbObjectId objId;
	acdbGetObjectId(objId,ename);
	gl_AddXdata(objId,saveName,Str1,dia,dia1);
}

void CArx_Xdata::gl_AddXdata(AcDbObjectId eId,
							  TCHAR saveName[],
							  TCHAR Str1[],
							  int  dia,
							  double dia1)
{
	
	CString strResource;
	Acad::ErrorStatus retStat;
	AcDbObject* pObj;
	
	if ((retStat = acdbOpenObject(pObj,eId,AcDb::kForWrite)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING137);	// "\n선택이 잘못되었습니다.\n"
        ads_printf(strResource);
        return;
    }
 
    TCHAR appName[100], resStr[30];
    appName[0] = resStr[0]=_T('\0');
 
	_tcscpy(appName,saveName);
	_tcscpy(resStr,Str1);
    
    struct  resbuf  *pRb, *pTemp1;
    
    pRb = pObj->xData(appName);
 
    if (pRb != NULL) {
        pTemp1 = pRb;
		while ((pTemp1->rbnext != NULL) && (pTemp1->restype != 1000)){
			pTemp1 = pTemp1->rbnext;
		}
		pTemp1->resval.rstring = (TCHAR*) malloc((_tcslen(resStr)+1)*sizeof(TCHAR));
		_tcscpy(pTemp1->resval.rstring, resStr);
		pTemp1 = pTemp1->rbnext;
		pTemp1->resval.rint = dia;
		pTemp1 = pTemp1->rbnext;
		pTemp1->resval.rreal  = dia1;
		while (pTemp1->rbnext != NULL){
			pTemp1 = pTemp1->rbnext;
		}
    } else {
        ads_regapp(appName);
 
        pRb = ads_newrb(AcDb::kDxfRegAppName);
        pTemp1 = pRb;

        pTemp1->resval.rstring  = (TCHAR*) malloc((_tcslen(appName)+1)*sizeof(TCHAR));
        _tcscpy(pTemp1->resval.rstring, appName);
		
		pTemp1->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
		pTemp1 = pTemp1->rbnext;
		pTemp1->restype = 1000;
		pTemp1->resval.rstring = (TCHAR*) malloc((_tcslen(resStr)+1)*sizeof(TCHAR));
		_tcscpy(pTemp1->resval.rstring, resStr);
 
		
		pTemp1->rbnext = ads_newrb(AcDb::kDxfXdInteger16);
		pTemp1 = pTemp1->rbnext;
		pTemp1->restype = 1070;
		pTemp1->resval.rint = dia;
 
		pTemp1->rbnext = ads_newrb(AcDb::kDxfXdReal);
		pTemp1 = pTemp1->rbnext;
		pTemp1->restype = 1040;
		pTemp1->resval.rreal  = dia1;
    }
  
    pObj->upgradeOpen();
    pObj->setXData(pRb);
    
    pObj->close();

    if(pRb) ads_relrb(pRb);

}
//확장 데이터 추가(객체명,확장명,정수,문자,문자,문자)
void CArx_Xdata::gl_AddXdata(ads_name ename,
							  TCHAR saveName[],
							  int  dia,
							  TCHAR Str1[],
							  TCHAR Str2[],
							  TCHAR Str3[])
{
	AcDbObjectId objId;
	acdbGetObjectId(objId,ename);
	gl_AddXdata(objId,saveName,dia,Str1,Str2,Str3);
}
void CArx_Xdata::gl_AddXdata(AcDbObjectId eId,
							  TCHAR saveName[],
							  int  dia,
							  TCHAR Str1[],
							  TCHAR Str2[],
							  TCHAR Str3[])
{
	
	CString strResource;
	Acad::ErrorStatus retStat;
	AcDbObject* pObj;
	
	if ((retStat = acdbOpenObject(pObj,eId,AcDb::kForWrite)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING138);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
//		ads_printf(_T("\nvoid CArx_Xdata::gl_AddXdata 선택이 잘못되었습니다.\n"));
        return;
    }
 
    TCHAR appName[100], resStr1[30],resStr2[30],resStr3[30];
    appName[0] = resStr1[0]=resStr2[0]=resStr3[0]=_T('\0');
 
	_tcscpy(appName,saveName);
	_tcscpy(resStr1,Str1);_tcscpy(resStr2,Str2);_tcscpy(resStr3,Str3);
    
    struct  resbuf  *pRb, *pTemp1;
    
    pRb = pObj->xData(appName);
 
    if (pRb != NULL) {
        pTemp1 = pRb;
		while ((pTemp1->rbnext != NULL) && (pTemp1->restype != 1070)){
			pTemp1 = pTemp1->rbnext;
		}
		pTemp1->resval.rint = dia;
		pTemp1 = pTemp1->rbnext;
		pTemp1->resval.rstring = (TCHAR*) malloc((_tcslen(resStr1)+1)*sizeof(TCHAR));
		_tcscpy(pTemp1->resval.rstring, resStr1);
		pTemp1 = pTemp1->rbnext;
		
		pTemp1->resval.rstring = (TCHAR*) malloc((_tcslen(resStr2)+1)*sizeof(TCHAR));
		_tcscpy(pTemp1->resval.rstring, resStr2);
		pTemp1 = pTemp1->rbnext;
		pTemp1->resval.rstring = (TCHAR*) malloc((_tcslen(resStr3)+1)*sizeof(TCHAR));
		_tcscpy(pTemp1->resval.rstring, resStr3);
		
		while (pTemp1->rbnext != NULL){
			pTemp1 = pTemp1->rbnext;
		}
    } else {
        ads_regapp(appName);
 
        pRb = ads_newrb(AcDb::kDxfRegAppName);
        pTemp1 = pRb;

        pTemp1->resval.rstring  = (TCHAR*) malloc((_tcslen(appName)+1)*sizeof(TCHAR));
        _tcscpy(pTemp1->resval.rstring, appName);
		
		pTemp1->rbnext = ads_newrb(AcDb::kDxfXdInteger16);
		pTemp1 = pTemp1->rbnext;
		pTemp1->restype = 1070;
		pTemp1->resval.rint = dia;

		pTemp1->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
		pTemp1 = pTemp1->rbnext;
		pTemp1->restype = 1000;
		pTemp1->resval.rstring = (TCHAR*) malloc((_tcslen(resStr1)+1)*sizeof(TCHAR));
		_tcscpy(pTemp1->resval.rstring, resStr1);
 
		pTemp1->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
		pTemp1 = pTemp1->rbnext;
		pTemp1->restype = 1000;
		pTemp1->resval.rstring = (TCHAR*) malloc((_tcslen(resStr2)+1)*sizeof(TCHAR));
		_tcscpy(pTemp1->resval.rstring, resStr2);

		pTemp1->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
		pTemp1 = pTemp1->rbnext;
		pTemp1->restype = 1000;
		pTemp1->resval.rstring = (TCHAR*) malloc((_tcslen(resStr3)+1)*sizeof(TCHAR));
		_tcscpy(pTemp1->resval.rstring, resStr3);
	
    }
  
    pObj->upgradeOpen();
    pObj->setXData(pRb);
    
    pObj->close();

    if(pRb) ads_relrb(pRb);

}
void CArx_Xdata::gl_FIREAddXdata(ads_name ename,
							  TCHAR saveName[],
							  TCHAR Str1[],
							  AcGePoint3d pt)
{
	
	CString strResource;
	Acad::ErrorStatus retStat;
	AcDbObject* pObj;
	AcDbObjectId eId;
	ads_point p1;
	p1[X]=pt[0];p1[Y]=pt[1];p1[Z]=pt[2];
	acdbGetObjectId(eId,ename);
	if ((retStat = acdbOpenObject(pObj,eId,AcDb::kForWrite)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING139);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
//        ads_printf(_T("\nvoid CArx_Xdata::gl_FIREAddXdata 선택이 잘못되었습니다.\n"));
        return;
    }
 
    TCHAR appName[100], resStr[30];
    appName[0] = resStr[0]=_T('\0');
 
	_tcscpy(appName,saveName);
	_tcscpy(resStr,Str1);
    
    struct  resbuf  *pRb, *pTemp1;
    
    pRb = pObj->xData(appName);
 
    if (pRb != NULL) {
        pTemp1 = pRb;
		while ((pTemp1->rbnext != NULL) && (pTemp1->restype != 1000)){
			pTemp1 = pTemp1->rbnext;
		}
		pTemp1->resval.rstring = (TCHAR*) malloc((_tcslen(resStr)+1)*sizeof(TCHAR));
		_tcscpy(pTemp1->resval.rstring, resStr);
		pTemp1 = pTemp1->rbnext;
		ads_point_set(p1, pTemp1->resval.rpoint);
		while (pTemp1->rbnext != NULL){
			pTemp1 = pTemp1->rbnext;
		}
    } else {
        ads_regapp(appName);
 
        pRb = ads_newrb(AcDb::kDxfRegAppName);
        pTemp1 = pRb;

        pTemp1->resval.rstring  = (TCHAR*) malloc((_tcslen(appName)+1)*sizeof(TCHAR));
        _tcscpy(pTemp1->resval.rstring, appName);

		pTemp1->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
		pTemp1 = pTemp1->rbnext;
		pTemp1->restype = 1000;
		pTemp1->resval.rstring = (TCHAR*) malloc((_tcslen(resStr)+1)*sizeof(TCHAR));
		_tcscpy(pTemp1->resval.rstring, resStr);
 
		
		pTemp1->rbnext = ads_newrb(1010);
		pTemp1 = pTemp1->rbnext;
		pTemp1->restype = 1010;
		ads_point_set(p1, pTemp1->resval.rpoint);
 
    }
     
    pObj->upgradeOpen();
    pObj->setXData(pRb);
    
    pObj->close();

    if(pRb) ads_relrb(pRb);
	

}
void CArx_Xdata::gl_AddXdata(struct resbuf* pRb,AcDbObjectId eId)
{
	
	CString strResource;
	AcDbObject* pObj;
	
	if ((es = acdbOpenObject(pObj,eId,AcDb::kForWrite)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING140);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
 //       ads_printf(_T("\nvoid CArx_Xdata::gl_AddXdata 선택이 잘못되었습니다.\n"));
        return;
    }
	
	pObj->upgradeOpen();
    pObj->setXData(pRb);
    
    pObj->close();
}

void CArx_Xdata::sym_XdataAdd(TCHAR *hand)
{
	ads_name lst_name;
	AcDbObjectId eId;
	TCHAR age[20];
	age[0]=_T('\0');
	_tcscpy(age,hand);
	if(ads_entlast(lst_name)!=RTNORM)
		return;
	acdbGetObjectId(eId,lst_name);

	gl_AddXdata(eId,_T("PC"),_T(""),_T("0"),_T("0"),age);
	
}
void CArx_Xdata::sym_XdataAdd(TCHAR *hand,ads_name lst_name)
{
	AcDbObjectId eId;
	TCHAR age[20];
	age[0]=_T('\0');
	_tcscpy(age,hand);
	acdbGetObjectId(eId,lst_name);

	gl_AddXdata(eId,_T("PC"),_T(""),_T("0"),_T("0"),age);
	
}
void CArx_Xdata::sym_XdataAdd(struct resbuf* pRb)
{
	ads_name lst_name;

	if(ads_entlast(lst_name)!=RTNORM)
		return;
	sym_XdataAdd(pRb,lst_name);
	
}
void CArx_Xdata::sym_XdataAdd(struct resbuf* pRb,ads_name lst_name)
{
	
	CString strResource;

	AcDbObjectId eId;
	AcDbObject* pObj;

	acdbGetObjectId(eId,lst_name);

	if ((es = acdbOpenObject(pObj,eId,AcDb::kForWrite)) != Acad::eOk)
	{
		strResource.LoadStringW(IDS_STRING141);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
 //       ads_printf(_T("\nvoid CArx_Xdata::sym_XdataAdd 선택이 잘못되었습니다.\n"));
        return;
    }
	
	pObj->upgradeOpen();
    pObj->setXData(pRb);
    
    pObj->close();
	
}
///관경 바꾼다.(PIP객체 수정)
void CArx_Xdata::gf_PIPXdataAdd(ads_name lst_name,int dia)
{
	
	CString strResource;

	AcDbObjectId eId;
	AcDbObject* pObj;
	struct resbuf *pRb,*pTemp;
	
	if(gfenameToXdataPiP(lst_name,_T("PIP"))!=NULL)
	{
		acdbGetObjectId(eId,lst_name);
		if ((es = acdbOpenObject(pObj,eId,AcDb::kForWrite)) != Acad::eOk)
		{
			strResource.LoadStringW(IDS_STRING142);	// "\n선택이 잘못되었습니다.\n"
			ads_printf(strResource);
//			ads_printf(_T("\nvoid CArx_Xdata::gf_PIPXdataAdd 선택이 잘못되었습니다.\n"));
			return;
		}
		pRb = pObj->xData(_T("PIP"));

		pTemp=pRb;
		while ((pTemp != NULL) && (pTemp->restype != 1070))
		{
			pTemp = pTemp->rbnext;
		}
		pTemp->resval.rint = dia;
		for (pTemp = pRb; pTemp->rbnext != NULL;pTemp = pTemp->rbnext)
            { ; }
		pObj->upgradeOpen();
		pObj->setXData(pRb);
    
		pObj->close();

		if(pRb!=NULL)
			ads_relrb(pRb);
	}
	else
	{
		acdbGetObjectId(eId,lst_name);
		gl_AddXdata(eId,_T("PIP"),_T(""),dia,0.0);
	}	
}



// string형은 잘모르겠고 int(1070)에는 호칭경을 float(1040)에는 실관경을 대입한다
// Xdata에는 string형 int형 float형 세가지 타입의 저장공간이 있다 이중 float형에 실관경을 대입한다
// 관경 바꾼다.(PIP객체 수정)
// 실관경 입력
void CArx_Xdata::gf_PIPXdataAddRbore(ads_name lst_name,double dia)	//  객체와 실수형 실관경값
{
	
	CString strResource;
//	ads_printf(_T("\n gf_PIPXdataAddRbore 시작 \n"));
	AcDbObjectId eId;
	AcDbObject* pObj;
	struct resbuf *pRb,*pTemp;
	
	if(gfenameToXdataPiP(lst_name,_T("PIP"))!=NULL)
	{
		acdbGetObjectId(eId,lst_name);
		if ((es = acdbOpenObject(pObj,eId,AcDb::kForWrite)) != Acad::eOk)
		{
			strResource.LoadStringW(IDS_STRING143);	// "\n선택이 잘못되었습니다.\n"
			ads_printf(strResource);
//			ads_printf(_T("\nvoid CArx_Xdata::gf_PIPXdataAdd 선택이 잘못되었습니다.\n"));
			return;
		}
		pRb = pObj->xData(_T("PIP"));

		pTemp=pRb;
		while ((pTemp != NULL) && (pTemp->restype != 1040))	// 1040이 실수형(float) 데이터형을 의미한다
		{
			pTemp = pTemp->rbnext;
		}
//		pTemp->resval.rint = dia;
		pTemp->resval.rreal = dia;
	//	ads_printf(_T("\n실수형 dia pTemp->resval.rreal에 대입됨 dia=%f resval=%f\n"),dia,pTemp->resval.rreal);
		for (pTemp = pRb; pTemp->rbnext != NULL;pTemp = pTemp->rbnext)
            { ; }
		pObj->upgradeOpen();
		pObj->setXData(pRb);
    
		pObj->close();

		if(pRb!=NULL)
			ads_relrb(pRb);
	}
	else
	{
		acdbGetObjectId(eId,lst_name);
		gl_AddXdata(eId,_T("PIP"),_T(""),0,dia);
	}
//	ads_printf(_T("\n gf_PIPXdataAddRbore 완료 \n"));
}







///
void CArx_Xdata::gf_PCXdataAdd(ads_name lst_name,
							  TCHAR *Str1,
							  TCHAR *Str2,
							  TCHAR *Str3,
							  TCHAR *Str4)
{
	
	CString strResource;

	AcDbObjectId eId;
	AcDbObject* pObj;
	struct resbuf *pRb,*pTemp;
	
	if(gfenameToXdataPiP(lst_name,_T("PC"))!=NULL){
		acdbGetObjectId(eId,lst_name);
		if ((es = acdbOpenObject(pObj,eId,AcDb::kForWrite)) != Acad::eOk)
		{
			strResource.LoadStringW(IDS_STRING144);	// "\n선택이 잘못되었습니다.\n"
			ads_printf(strResource);
			return;
		}
		pRb = pObj->xData(_T("PC"));

		pTemp=pRb;
		while ((pTemp != NULL) && (pTemp->restype != 1000)){
			pTemp = pTemp->rbnext;
		}
		pTemp->resval.rstring = _tcsdup(Str1);
		pTemp = pTemp->rbnext;
		pTemp->resval.rstring = _tcsdup(Str2);
		pTemp = pTemp->rbnext;
		pTemp->resval.rstring = _tcsdup(Str3);
		pTemp = pTemp->rbnext;
		pTemp->resval.rstring = _tcsdup(Str4);

		for (pTemp = pRb; pTemp->rbnext != NULL;pTemp = pTemp->rbnext)
            { ; }
		pObj->upgradeOpen();
		pObj->setXData(pRb);
    
		pObj->close();

		if(pRb!=NULL)
			ads_relrb(pRb);
	}else{
		acdbGetObjectId(eId,lst_name);
		gl_AddXdata(eId,_T("PC"), Str1, Str2, Str3, Str4);
	}
	
}
/// XDATA를 얻는다.지운다.
struct resbuf* CArx_Xdata::gfenameToXdata(ads_name en)
{
	CArx_User m_User;
	//AcDbLine* pLine;
	AcDbObject* pObj;
	AcDbObjectId objId;
	struct resbuf *pRb;

	if(acdbGetObjectId(objId,en) != Acad::eOk)
		return NULL;
	if(acdbOpenObject(pObj,objId,AcDb::kForWrite)==Acad::eWasErased)
		return NULL;
	pRb=pObj->xData(NULL);
	pObj->erase();
	pObj->close();
	
	return pRb;
}

// XDATA를 얻는다.지우지않는다.
struct resbuf* CArx_Xdata::gfenameToXdataL(ads_name en)
{
	CArx_User m_User;
	//AcDbLine* pLine;
	AcDbObject* pObj;
	AcDbObjectId objId;
	struct resbuf *pRb=NULL;

	if(acdbGetObjectId(objId,en) != Acad::eOk)
		return NULL;
	if(acdbOpenObject(pObj,objId,AcDb::kForWrite)==Acad::eWasErased)
		return NULL;
	pRb=pObj->xData(NULL);
	pObj->close();
	
	return pRb;
}
// XDATA를 얻는다.
struct resbuf* CArx_Xdata::gfenameToXdataPiP(ads_name en,TCHAR* str)
{
	CArx_User m_User;
	//AcDbLine* pLine;
	AcDbObject* pObj;
	AcDbObjectId objId;
	struct resbuf *pRb=NULL;

	if(acdbGetObjectId(objId,en) != Acad::eOk)
		return NULL;
	if(acdbOpenObject(pObj,objId,AcDb::kForWrite)==Acad::eWasErased)
		return NULL;
	pRb=pObj->xData(str);
	pObj->close();
	
	return pRb;
}
struct resbuf* CArx_Xdata::gfenameToXdataPiP(AcDbObjectId obj, TCHAR *str)
{
	ads_name ename;

	acdbGetAdsName(ename, obj);
	return gfenameToXdataPiP(ename,_T("PIP"));
}

///chk=true이면 uparray ;; false이면 downarry
///index != -1이면 index번호것을 현재것으로 대체한다.
void CArx_Xdata::arx_setObList(ads_name enn,int index,bool chk)
{
	AcDbObjectId objId;

	if(acdbGetObjectId(objId,enn) != Acad::eOk)
		return;
	if(chk && index == -1){
		uparry.append(objId);
	}else if(!chk && index == -1){
		downarry.append(objId);
	}else if(chk && index != -1){
		uparry.setAt(index,objId);
	}else if(!chk && index != -1){
		downarry.setAt(index,objId);
	}
}
///index=-1이면 모두삭제.
///chk=true이면 uparray ;; false이면 downarry
void CArx_Xdata::arx_getObList(ads_name& enn,int index,bool chk)
{
	AcDbObjectId objId;

	if(chk && index == -1){
		while(!uparry.isEmpty())
			uparry.removeLast();
		return;
	}else if(!chk && index == -1){
		while(!downarry.isEmpty())
			downarry.removeLast();
		return;
	}
	if(chk){
		objId = uparry.at(index);
	}else{
		objId = downarry.at(index);
	}
	acdbGetAdsName(enn,objId);

	return;
}

int CArx_Xdata::arx_getColor(ads_name enn)
{

	AcDbObjectId objId;
	AcDbEntity* pEnt;
	int  color=0;
	
	if(acdbGetObjectId(objId,enn) != Acad::eOk)
		return color;
	if(acdbOpenObject(pEnt,objId,AcDb::kForRead)==Acad::eWasErased)
		return color;
	color=pEnt->colorIndex();
	pEnt->close();
	return color;
}

void CArx_Xdata::arx_setColor(ads_name enn,int color)
{
	AcDbObjectId objId;
	AcDbEntity* pEnt;
	AcCmColor color1;

	color1.setColorIndex(color);
	if(acdbGetObjectId(objId,enn) != Acad::eOk)
		return;
	if(acdbOpenObject(pEnt,objId,AcDb::kForWrite)==Acad::eWasErased)
		return ;
	pEnt->setColor(color1);
	pEnt->close();
}

// 소트(디센딩)
//// 머하는거지?
void CArx_Xdata::lf_SortD(int cnt,AcGePoint3d dir_xpt)
{
	int i,j;
	ads_name tmp1,tmp2;
	AcGePoint3d pt1,pt2,p_pt1,p_pt2;
	CArx_Math m_Math;

	for(i=0;i<cnt-1;i++){
		arx_getObList(tmp1,i,true);
		pt1=lf_3DReturnList(tmp1,10);
		pt2=lf_3DReturnList(tmp1,11);
		p_pt1=m_Math.gl_Perpt(pt1,pt2,dir_xpt);	// tmp1에 수직 교차점 구하기(closest)
		for(j=i+1;j<cnt;j++){
			arx_getObList(tmp2,j,true);
			pt1=lf_3DReturnList(tmp2,10);
			pt2=lf_3DReturnList(tmp2,11);
			p_pt2=m_Math.gl_Perpt(pt1,pt2,dir_xpt);	// tmp2에 수직 교차점 구하기(closest)
			if(dir_xpt.distanceTo(p_pt2)>dir_xpt.distanceTo(p_pt1)){
				p_pt1=p_pt2;
				arx_getObList(tmp1,i,true);
				arx_setObList(tmp2,i,true);
				arx_setObList(tmp1,j,true);
			}
		}

	}
}
// 소트(어센딩)
void CArx_Xdata::lf_SortA(int cnt,AcGePoint3d dir_xpt)
{
	int i,j;
	ads_name tmp1,tmp2;
	AcGePoint3d pt1,pt2,p_pt1,p_pt2;
	CArx_Math m_Math;

	for(i=0;i<cnt-1;i++){
		arx_getObList(tmp1,i,true);
		pt1=lf_3DReturnList(tmp1,10);
		pt2=lf_3DReturnList(tmp1,11);
		p_pt1=m_Math.gl_Perpt(pt1,pt2,dir_xpt);
		for(j=i+1;j<cnt;j++){
			arx_getObList(tmp2,j,true);
			pt1=lf_3DReturnList(tmp2,10);
			pt2=lf_3DReturnList(tmp2,11);
			p_pt2=m_Math.gl_Perpt(pt1,pt2,dir_xpt);
			if(dir_xpt.distanceTo(p_pt2)<dir_xpt.distanceTo(p_pt1)){
				p_pt1=p_pt2;
				arx_getObList(tmp1,i,true);
				arx_setObList(tmp2,i,true);
				arx_setObList(tmp1,j,true);
			}
		}

	}
}

void CArx_Xdata::arx_highlight(ads_name enn)
{

	AcDbObjectId objId;
	AcDbEntity* pEnt;
	
	if(acdbGetObjectId(objId,enn) != Acad::eOk)
		return;
	if(acdbOpenObject(pEnt,objId,AcDb::kForRead)==Acad::eWasErased)
		return;
	pEnt->draw();
	pEnt->highlight();
	pEnt->close();
}

void CArx_Xdata::arx_unhighlight(ads_name enn)
{

	AcDbObjectId objId;
	AcDbEntity* pEnt;
	
	if(acdbGetObjectId(objId,enn) != Acad::eOk)
		return;
	if(acdbOpenObject(pEnt,objId,AcDb::kForRead)==Acad::eWasErased)
		return;
	pEnt->draw();
	pEnt->unhighlight();
	pEnt->close();
}
//POLYLINE 객체명(enn)과 지정 포인트(pt1)을 받아,
// pt1이 있는 선분의 시작점(spt),끝점(ept),좌표list(lst),수량(lstv_cnt)을 넘김
void CArx_Xdata::gl_PolyAnaly(ads_name enn,AcGePoint3d pt1,AcGePoint3d& spt,
								 AcGePoint3d& ept,AcGePoint3dArray& lst,
								 int& lstv_cnt)
{
	double dist=(double)0;
	CArx_Math m_Math;
	AcGePoint3d pint,ppt1,ppt2;
	BOOL flg=FALSE;

	AcDbObjectId plineId;
    AcDb2dPolyline *pPline;

	acdbGetObjectId(plineId,enn);
    acdbOpenObject(pPline, plineId, AcDb::kForRead);
 
    AcDbObjectIterator *pVertIter= pPline->vertexIterator();
    pPline->close();  // Finished with the pline header.
 
    AcDb2dVertex *pVertex;
	pint.set(0,0,0);ppt1.set(0,0,0);ppt2.set(0,0,0);
    AcDbObjectId vertexObjId;
    for (int vertexNumber = 0; !pVertIter->done();
        vertexNumber++, pVertIter->step())
    {
        vertexObjId = pVertIter->objectId();
        acdbOpenObject(pVertex, vertexObjId,
            AcDb::kForRead);
        ppt2 = pVertex->position();
        pVertex->close();
		
		if(flg){
			pint=m_Math.gl_Perpt(ppt1,ppt2,pt1);
			dist=pt1.distanceTo(pint);
			if((m_Math.fb_hand(ppt1,ppt2,pt1)==0)&&(dist < 10.0)){
				spt=ppt1;ept=ppt2;
			}
			lst.append(ppt2);
		}else{
			lst.append(ppt2);
		}
		ppt1=ppt2;
		flg=TRUE;
		lstv_cnt++;
    }
    delete pVertIter;
}
//LWPOLYLINE 객체명(enn)과 지정 포인트(pt1)을 받아,
// pt1이 있는 선분의 시작점(spt),끝점(ept),좌표list(lst),수량(lstv_cnt)을 넘김
void CArx_Xdata::gl_LWPolyAnaly(ads_name enn,AcGePoint3d pt1,AcGePoint3d& spt,
								 AcGePoint3d& ept,AcGePoint3dArray& lst,
								 int& lstv_cnt)
{
	
	CString strResource;
	double dist=(double)0;
	struct resbuf *pRb=NULL, *pHead=NULL;
	CArx_User m_User;
	CArx_Math m_Math;
	AcGePoint3d pint,ppt1,ppt2;
	BOOL flg=FALSE;

	if(ads_name_nil(enn)){
		strResource.LoadStringW(IDS_STRING145);	// "\n선택이 잘못되었습니다.\n"
		ads_printf(strResource);
		return;
	}
	pRb=ads_entget(enn);
	pHead=pRb;
	pint.set(0,0,0);
	ppt1.set(0,0,0);
	ppt2.set(0,0,0);
	while(pRb != NULL){
		while ((pRb != NULL) && (pRb->restype != 10)){
			pRb = pRb->rbnext;
		}
		if(pRb != NULL){
			ppt2[0]=pRb->resval.rpoint[X];
			ppt2[1]=pRb->resval.rpoint[Y];
			ppt2[2]=pRb->resval.rpoint[Z];

			if(flg){
				pint=m_Math.gl_Perpt(ppt1,ppt2,pt1);
				dist=pt1.distanceTo(pint);
				if((m_Math.fb_hand(ppt1,ppt2,pt1)==0)&&(dist < 10.0)){
					spt=ppt1;
					ept=ppt2;
				}
				lst.append(ppt2);
			}else{
				lst.append(ppt2);
			}//if
			ppt1=ppt2;
			flg=TRUE;
			pRb = pRb->rbnext;
			lstv_cnt++;
		}//if(pRb!=NULL)
	}
	if(pHead)
		ads_relrb(pHead);
}

// 객체 선택
BOOL CArx_Xdata::gf_ssget(ads_name& sname,resbuf* filter,long& len)
{
	
	CString strResource;
	ads_point p1,p2;
	CArx_Draw m_Draw;

	m_Draw.setSysVar((LPCTSTR)_T("OSMODE"),512);
	

	strResource.LoadStringW(IDS_STRING146);	// "\n선택이 잘못되었습니다.\n"

	if(ads_getpoint(NULL,strResource,p1)!=RTNORM)
		return FALSE;
	m_Draw.setSysVar((LPCTSTR)_T("OSMODE"),0);
	
		
	strResource.LoadStringW(IDS_STRING147);	// "\n선택이 잘못되었습니다.\n"

	if(ads_getcorner(p1,strResource,p2)!=RTNORM){

		if(ads_ssget(NULL,NULL,NULL,filter,sname)!=RTNORM)
			return FALSE;
		else 	ssNoLock(sname); //Lock Layer 제거

		if (ads_sslength(sname, &len) != RTNORM)
			return FALSE;
		return TRUE;
	}
	if(ads_ssget(_T("C"),p1,p2,filter,sname)!=RTNORM)
		return FALSE;
	else ssNoLock(sname); //Lock Layer 제거

	if (ads_sslength(sname, &len) != RTNORM)
			return FALSE;

	return TRUE;
}


// Filter가 있는 더블라인 찾기
AcDbObjectId CArx_Xdata::DLineSelect(ads_point pt1, ads_name ent, TCHAR *str, resbuf *filter)
{
	ads_name ename,m_ss;
	long sslen;
	AcDbObjectId objId;
	ads_point p1,p2;
	CArx_Math m_Math;
	CArx_Draw m_Draw;
	CArx_User m_User;
	int old_pickbox;
	AcGePoint2d screenpt;
	AcGePoint3d spt,ept;
	double angle=(double)0,angle1=(double)0,chk_ang1=(double)0,chk_ang2=(double)0;
	struct resbuf *sys1=NULL, *sys2=NULL;
	ads_real vs, ss, yr1;
	CString s;

	// Pick Box Size
	//********************************************************************
    sys1=ads_newrb(RTREAL);       /* sys1 must the allocated memory */
	sys2=ads_newrb(RT3DPOINT);
    ads_getvar(_T("VIEWSIZE"), sys1);     /* real */
	ads_getvar(_T("SCREENSIZE"), sys2);   /* point (real real) */
    vs=sys1->resval.rreal;            /* view size */
	ss=sys2->resval.rpoint[Y];        /* screensize */
	ads_relrb(sys1);
	ads_relrb(sys2);

	yr1=vs/ss * 3;                    /* real size of pickbox in screen */
    //******************************************************************
	if(ads_name_nil(ent))	return NULL;
	spt = lf_3DReturnList(ent,10);
	ept = lf_3DReturnList(ent,11);
	angle = floor(m_Math.arx_angleA(spt,ept));

	es=m_Draw.getSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), 1);
	ads_polar(pt1,m_Math.DTR(45), yr1, p1);
	ads_polar(pt1,m_Math.DTR(225), yr1, p2);

	if (ads_ssget(_T("C"), p1, p2, filter, m_ss) != RTNORM){
		es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);
		return NULL;
	}
	ssNoLock(m_ss); //Lock Layer 제거
	es=m_Draw.setSysVar((LPCTSTR)_T("PICKBOX"), old_pickbox);

	if (ads_sslength(m_ss, &sslen) != RTNORM)
		return NULL;

	int index = 0;
	while(index < sslen){
		if(ads_ssname(m_ss,index,ename) == RTNORM){
			acdbGetObjectId(objId,ename);
			//Line이며, 중심선이 아닌 경우
			lf_SReturnList(ename,0,s);
			if(s==str && !entityEqual(ent,ename)) {
				spt = lf_3DReturnList(ename,10);
				ept = lf_3DReturnList(ename,11);
				angle1 = floor(m_Math.arx_angleA(spt,ept));
				chk_ang1 = fabs(angle-angle1);
				chk_ang2 = fabs(angle-angle1-180.0);
				if(chk_ang1<0.01||chk_ang1==360.0||chk_ang2<0.01||chk_ang2==360.0){
					ads_ssfree(m_ss);
					ads_ssfree(ename);
					return objId;
				}//end-if
			}//end-if
		}//end-if
		index++;
	}//end-while

	ads_ssfree(m_ss);
	ads_ssfree(ename);
	return NULL;
}

//Xdata 여부 확인
bool CArx_Xdata::isXdata(AcDbObject *pEnt, LPCTSTR lpApp)
{
	struct resbuf* resbuf;
	resbuf = pEnt->xData(lpApp);
	if(resbuf){
//		acutRelRb(resbuf);
		ads_relrb(resbuf);
		return true;
	}else return false;

}

bool CArx_Xdata::isXdata(AcDbObjectId objId, LPCTSTR lpApp)
{
	bool es;
	AcDbObject*	pObj;
	if(acdbOpenObject(pObj, objId, AcDb::kForWrite) != Acad::eOk)
		return NULL;

	if(pObj){
		es=isXdata(pObj, lpApp);
		pObj->close();
		return es;
	};
	return false;

}

bool CArx_Xdata::isXdata(ads_name ename, LPCTSTR lpApp)
{
	AcDbObjectId obj;
	acdbGetObjectId(obj, ename);
	return isXdata(obj, lpApp);
}
//---- Entity를 OPEN 한다.
AcDbObject* uds_openObject(AcDbObjectId entID, BOOL bWritable, BOOL openErased)
{
	AcDbObject*	pObj;
	AcDb::OpenMode openMode = (bWritable ? AcDb::kForWrite : AcDb::kForRead);

	if (acdbOpenObject(pObj, entID, openMode) != Acad::eOk)
		return NULL;
	return pObj;
}


AcDbEntity* uds_openEntity(AcDbObjectId entID, BOOL bWritable, BOOL openErased)
{
	return ((AcDbEntity*)uds_openObject(entID, bWritable, openErased));
}

//관경을 얻는다.
int CArx_Xdata::gl_GetPipeBore(ads_name ename)
{
	int bore=0;
	AcDbObjectId objId;

	acdbGetObjectId(objId, ename);

	if(objId==NULL)return -1;

	AcDbObject* pObj=uds_openObject(objId, true);

	if (pObj){
		bore=gl_GetPipeBore(pObj);
		pObj->close();
		return bore;
	};

	return -1;
}

int CArx_Xdata::gl_GetPipeBore(AcDbObject* pEnt)
{
	CString sKind;
	int bore=0;
	double tmp_real=0.0;
	gl_GetPIPXdata(pEnt, sKind, bore, tmp_real);
	return bore;
}

int CArx_Xdata::gl_GetPipeBore(AcDbObjectId objId)
{
	int bore=0;
	if(objId=NULL)return -1;
	AcDbObject* pObj=uds_openObject(objId, true);
	if (pObj){
		bore=gl_GetPipeBore(pObj);
		pObj->close();
		return bore;
	};
	return -1;
}
/// 확장 엔티티 값을 얻는다.
void CArx_Xdata::gl_GetPIPXdata(ads_name ename, CString& sKind, int& bore, double& ins_thk)
{
	AcDbObjectId EntId;

	acdbGetObjectId(EntId, ename);
	if(EntId==NULL)	return;

	AcDbObject* pObj = uds_openObject(EntId, false, true);
	if (pObj){
		gl_GetPIPXdata(pObj, sKind, bore, ins_thk);
		pObj->close();
	}//end-if
}
void CArx_Xdata::gl_GetPIPXdata(AcDbObjectId EntId, CString& sKind, int& bore, double& ins_thk)
{
	AcDbObject* pObj = uds_openObject(EntId, false, true);
	if (pObj){
		gl_GetPIPXdata(pObj, sKind, bore, ins_thk);
		pObj->close();
	}//end-if
}

void CArx_Xdata::gl_GetPIPXdata(AcDbObject* pEnt, CString& sKind, int& bore, double& ins_thk)
{
	bore=0; ins_thk=0.0;
	struct resbuf* resbuf;
	resbuf = pEnt->xData(_T("PIP"));

	struct resbuf* rb = resbuf;
	if(rb){

		rb = rb->rbnext;
		if(rb==NULL){
			rb = ads_newrb(AcDb::kDxfXdAsciiString);			
			rb->restype = 1000;
			acutNewString(sKind, rb->resval.rstring);			
		}else sKind=rb->resval.rstring;		// 1000(문자열)
		rb = rb->rbnext;
		if(rb==NULL)
		{
			rb=ads_newrb(AcDb::kDxfXdInteger16);
			rb->restype=1070;
			rb->resval.rint=bore;
		}else bore=rb->resval.rint;			// 1070(정수)
		rb = rb->rbnext;
		if(rb==NULL)
		{
			rb=ads_newrb(AcDb::kDxfXdReal);
			rb->restype=1040;
			rb->resval.rreal=ins_thk;
		}else ins_thk=rb->resval.rreal;		// 1040(실수)
	
		ads_relrb(resbuf);
	}//end-if
}

//관종(sKind)을 얻는다.
CString CArx_Xdata::gl_GetPipeKind(ads_name ename)
{
	CString sKind;
	AcDbObjectId objId;

	acdbGetObjectId(objId, ename);

	sKind=_T("");
	if(objId==NULL) return sKind;
	AcDbObject* pObj=uds_openObject(objId, true);
	if (pObj){
		sKind=gl_GetPipeKind(pObj);
		pObj->close();
	};
	return sKind;
}
CString CArx_Xdata::gl_GetPipeKind(AcDbObject* pEnt)
{
	CString sKind,sFore,sBack,sMc;
	gl_GetPCXdata(pEnt, sKind, sFore, sBack, sMc);
	return sKind;
}
CString CArx_Xdata::gl_GetPipeKind(AcDbObjectId objId)
{
	CString sKind;
	sKind=_T("");

	if(objId==NULL)	return sKind;
	AcDbObject* pObj=uds_openObject(objId, true);
	if (pObj){
		sKind=gl_GetPipeKind(pObj);
		pObj->close();
	};
	return sKind;
}
//Get PC Xdata
void CArx_Xdata::gl_GetPCXdata(ads_name ename, CString& sKind, CString& sFore, CString& sBack, CString& sMc)
{
	AcDbObjectId EntId;

	acdbGetObjectId(EntId, ename);
	if(EntId==NULL)	return;

	AcDbObject* pObj = uds_openObject(EntId, false, true);
	if (pObj){
		gl_GetPCXdata(pObj, sKind, sFore, sBack, sMc);
		pObj->close();
	}//end-if
}
void CArx_Xdata::gl_GetPCXdata(AcDbObjectId EntId, CString& sKind, CString& sFore, CString& sBack, CString& sMc)
{
	AcDbObject* pObj = uds_openObject(EntId, false, true);
	if (pObj){
		gl_GetPCXdata(pObj, sKind, sFore, sBack, sMc);
		pObj->close();
	}//end-if
}
void CArx_Xdata::gl_GetPCXdata(AcDbObject* pEnt, CString& sKind, CString& sFore, CString& sBack, CString& sMc)
{
	struct resbuf* resbuf;
	resbuf = pEnt->xData(_T("PC"));

	struct resbuf* rb = resbuf;
	if(rb){

		rb = rb->rbnext;
		if(rb==NULL){
			rb=ads_newrb(AcDb::kDxfXdAsciiString);
			rb->restype=1000;
			acutNewString(sKind, rb->resval.rstring);
		}else sKind=rb->resval.rstring;		
		
		rb = rb->rbnext;
		if(rb==NULL){
			rb=ads_newrb(AcDb::kDxfXdAsciiString);
			rb->restype=1005;
			acutNewString(sFore, rb->resval.rstring);
		}else sFore=rb->resval.rstring;
		
		rb = rb->rbnext;
		if(rb==NULL)
		{
			rb=ads_newrb(AcDb::kDxfXdAsciiString);
			rb->restype=1005;
			acutNewString(sBack, rb->resval.rstring);
		}else sBack=rb->resval.rstring;

		rb = rb->rbnext;
		if(rb==NULL)
		{
			rb=ads_newrb(AcDb::kDxfXdAsciiString);
			rb->restype=1005;
			acutNewString(sMc, rb->resval.rstring);
		}else sMc=rb->resval.rstring;

	}//end-if
	ads_relrb(resbuf);
}

