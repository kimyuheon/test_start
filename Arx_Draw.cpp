// Arx_Draw.cpp: implementation of the CArx_Draw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Arx_Math.h"
#include "Arx_Xdata.h"
#include "Arx_User.h"
#include "Arx_Draw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArx_Draw::CArx_Draw()
{
	//	ads_printf(_T("\nDraw생성자\n"));
	_tcscpy(m_Entity_Layer, _T("0"));		// 기본 레이어로 "0" 레이어 설정
}

CArx_Draw::~CArx_Draw()
{
	//	ads_printf(_T("\nDraw소멸자\n"));
}

// 시스템 변수의 설정값을 CArx_Draw 멤버변수에 저장한다
Acad::ErrorStatus CArx_Draw::initDraw()
{
	CArx_User m_user;
	m_user.gf_GetRegistry(HKEY_LOCAL_MACHINE, _T("PATH"), gvAppPath);

	TCHAR *cdtextstyle;
	TCHAR *cdlayer;
	int cccolor;
	CString cclayer;
	CString cctextstyle;

	es=getSysVar((LPCTSTR)_T("clayer"), cclayer);
	es=getSysVar((LPCTSTR)_T("textstyle"), cctextstyle);
	es=getSysVar((LPCTSTR)_T("cecolor"), cccolor);

	int scl;
	es=getSysVar((LPCTSTR)_T("useri1"), scl);         //Scale 전역 축척값
	gvscl = scl;
	es=getSysVar((LPCTSTR)_T("useri5"), ds_clr);      //Drain,Soil Color
	if((ds_clr==0)||(ds_clr>255)) ds_clr=256;	// 부분 컬러가 BYBLOCK이거나 에러값일경우(255보다크면) BYLAYER로 색상설정
	setSysVar((LPCTSTR)_T("useri5"), ds_clr);      //Drain,Soil Color

	// 아래	시스템변수들은 모두 CArx_Draw 클래스 멤버변수에 설정된다
	es=getSysVar((LPCTSTR)_T("userr1"), bupum_size);  //Parts scale CArx_Draw::m_bupum_size에 대입된다
	es=getSysVar((LPCTSTR)_T("userr2"), txt_size);    //Text size
	es=getSysVar((LPCTSTR)_T("userr3"), LT_scale);    //LT scale
	es=getSysVar((LPCTSTR)_T("userr4"), HD_size);     //Hidden Size

	m_linetypeScale4Ent=0;

	setCurLinetype(_T("CONTINUOUS"));

	cdtextstyle = (TCHAR*) malloc((cctextstyle.GetLength()+1)*sizeof(TCHAR));
	_tcscpy(cdtextstyle, (LPTSTR)cctextstyle.operator LPCTSTR());
	setCurTextStyle(cdtextstyle);   //default layer set
	free(cdtextstyle);

	cdlayer = (TCHAR*) malloc((cclayer.GetLength()+1)*sizeof(TCHAR));
	_tcscpy(cdlayer, (LPTSTR)cclayer.operator LPCTSTR());
	setCurLayer(cdlayer);   //default layer set
	free(cdlayer);

	es=setSysVar((LPCTSTR)_T("auprec"), 3);
	es=setSysVar((LPCTSTR)_T("othmode"), 0);
	setCurColor(COLOR_BYLAYER);
	return Acad::eOk;
}


//레이어를 생성한다.
Acad::ErrorStatus 
	CArx_Draw::createLayer(TCHAR *layer,TCHAR *linetype,const AcCmColor color)
{//	ads_printf(_T("\n pipe layer=%s \n"),layer);
	AcDbLayerTable* pLayerTable;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL)
		return Acad::eNoDatabase;

	pDb->getLayerTable(pLayerTable,AcDb::kForWrite);

	if (pLayerTable->has(layer)!=Adesk::kTrue){
		AcDbLayerTableRecord* pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(layer);
		pLayerTableRecord->setColor(color);

		AcDbLinetypeTable* pLinetypeTable;
		pDb->getLinetypeTable(pLinetypeTable,AcDb::kForRead);

		AcDbObjectId linetypeId;
		pLinetypeTable->getAt(linetype,linetypeId);
		pLayerTableRecord->setLinetypeObjectId(linetypeId);
		pLayerTable->add(pLayerTableRecord);
		pLayerTable->close();
		pLayerTableRecord->close();
		//		if(pLayerTableRecord) delete pLayerTableRecord;
	}else{
		//	ads_printf(_T("\n이미 존재합니다."));
		pLayerTable->close();
	}

	return Acad::eOk;
}


// Block 삽입1
void CArx_Draw::gf_UserBlock_ins(AcDbObjectId blockId,
	AcGePoint3d inpt,
	double angle)
{
	//	ads_printf(_T("\n %s()1 시작. \n"),_T(__FUNCTION__));

	// 테스트
	if(inpt.x>ErrVal || inpt.y>ErrVal || inpt.z>ErrVal || angle>ErrVal){
		//	ads_printf(_T("\n 89124 Error! inpt.x=%.1f y=%.1f z=%.1f angle=%.1f "),inpt.x,inpt.y,inpt.z,angle);
	}//	ads_printf(_T("\n7865 x_scale=%.0f y_scale=%.0f "),x_scale,y_scale);



	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL) return;

	AcDbBlockReference *pBlkRef = new AcDbBlockReference;
	pBlkRef->setBlockTableRecord(blockId);

	AcGeVector3d normal(0.0, 0.0, 1.0);

	pBlkRef->setPosition(inpt);
	pBlkRef->setRotation(angle);
	pBlkRef->setNormal(normal);

	AcDbBlockTable *pBlockTable;
	pDb->getBlockTable(pBlockTable,AcDb::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForWrite);

	pBlockTable->close();

	// Append the block reference to the model space
	// block table record
	//
	AcDbObjectId newEntId;
	pBlockTableRecord->appendAcDbEntity(newEntId, pBlkRef);
	pBlockTableRecord->close();

	pBlkRef->close();
	//	if(pBlkRef) delete pBlkRef; 
}



// Block 삽입2, 원진호
void CArx_Draw::ads_DrawBlock(AcGePoint3d insertPt,	// 삽입점
	CString m_bname,			// 블록 이름
	double x_scale,				// x축척
	double y_scale,				// y축척
	double angle,				// 각도
	CString cclayer)			// 블록이 그려질 레이어
{
	//	ads_printf(_T("\n %s()2 시작. Lay=%s b_name=%s \n"),_T(__FUNCTION__),m_Entity_Layer,m_bname);//ads_printf(_T("\n 1878 x_scale=%.2f  y_scale=%.2f \n"),x_scale,y_scale);
	CString prevlayer;	// 이전레이어
	getSysVar((LPCTSTR)_T("clayer"), prevlayer);	// 이전레이어 보관

	// 테스트
	if(insertPt.x>ErrVal || insertPt.y>ErrVal || insertPt.z>ErrVal || x_scale>ErrVal || y_scale>ErrVal || angle>ErrVal){
		//ads_printf(_T("\n 89125 Error!! ads_DrawBlock1() insertPt.x=%.1f y=%.1f z=%.1f x_scale=%.1f y_scale=%.1f angle=%.1f \n"),insertPt.x,insertPt.y,insertPt.z,x_scale,y_scale,angle);
	}

	// 시스템변수 설정 clayer라는 현재 레이어 변수에 "dLineBlock" 레이어설정
	setSysVar((LPCTSTR)_T("clayer"), cclayer);
	setCurLayer(cclayer);

	ads_point inpt;

	inpt[X]=insertPt[0];
	inpt[Y]=insertPt[1];
	inpt[Z]=insertPt[2];

	ads_command(RTSTR,_T("REDRAW"),RTNONE);
	// 블록 삽입 명령	ZWCAD 2017 SP1_Preview 버전문제로 인한 arx 소스로 변경 2016-09-30 한대희
	ads_command(RTSTR, _T("-INSERT"), RTSTR, (LPTSTR)m_bname.operator LPCTSTR(), RTPOINT, inpt, RTREAL, x_scale, RTREAL, y_scale, RTREAL, angle, RTNONE);
	//DrawBlock_ZW2017(insertPt, m_bname, x_scale, y_scale, angle);
	



	ads_command(RTSTR,_T("REDRAW"),RTNONE);

	// 여기에 기존레이어 복구 코드 추가. 시스템변수 설정 clayer라는 현재 레이어 변수에 "0" 레이어설정(기존레이어로복구)
	setSysVar((LPCTSTR)_T("clayer"), prevlayer);
	setCurLayer(prevlayer);
	//	ads_printf(_T("\n9001 prevlayer=%s cclayer=%s"),prevlayer,cclayer);
	//	ads_printf(_T("\n %s1 완료. Lay=%s b_name=%s \n"),_T(__FUNCTION__),m_Entity_Layer,m_bname);
}

//// Block 삽입후 그룹핑 201406 한대희 추가
void CArx_Draw::ads_DrawBlock(AcGePoint3d insertPt,	// 삽입점
	CString m_bname,			// 블록 이름
	double x_scale,			// x축척
	double y_scale,			// y축척
	double angle,
	AcDbObjectIdArray& gIds)				// 각도
	//						  TCHAR* cclayer)			// 블록이 그려질 레이어
{
	//	ads_printf(_T("\n %s()3 시작. Lay=%s b_name=%s \n"),_T(__FUNCTION__),m_Entity_Layer,m_bname);
	//	ads_printf(_T("\n 1879 x_scale=%.2f  y_scale=%.2f \n"),x_scale,y_scale);

	// 테스트
	if(insertPt.x>ErrVal || insertPt.y>ErrVal || insertPt.z>ErrVal || x_scale>ErrVal || y_scale>ErrVal || angle>ErrVal ){
		//	ads_printf(_T("\n 89126 Error!! ads_DrawBlock1() insertPt.x=%.1f y=%.1f z=%.1f x_scale=%.1f y_scale=%.1f angle=%.1f \n"),insertPt.x,insertPt.y,insertPt.z,x_scale,y_scale,angle);
	}



	CString prevlayer;	// 이전레이어
	getSysVar((LPCTSTR)_T("clayer"), prevlayer);	// 이전레이어 보관

	// 시스템변수 설정 clayer라는 현재 레이어 변수에 "dLineBlock" 레이어설정
	setSysVar((LPCTSTR)_T("clayer"), clayer);
	setCurLayer(clayer);


	ads_point inpt;

	inpt[X]=insertPt[0];
	inpt[Y]=insertPt[1];
	inpt[Z]=insertPt[2];

	ads_command(RTSTR,_T("REDRAW"),RTNONE);
	// 블록 삽입 명령	ZWCAD 2017 SP1_Preview 버전문제로 인한 arx 소스로 변경 2016-09-30 한대희
	ads_command(RTSTR, _T("-INSERT"), RTSTR, (LPTSTR)m_bname.operator LPCTSTR(), RTPOINT, inpt, RTREAL, x_scale, RTREAL, y_scale, RTREAL, angle, RTNONE);
	//DrawBlock_ZW2017(insertPt, m_bname, x_scale, y_scale, angle);

	ads_command(RTSTR,_T("REDRAW"),RTNONE);

	// 여기에 기존레이어 복구 코드 추가. 시스템변수 설정 clayer라는 현재 레이어 변수에 "0" 레이어설정(기존레이어로복구)
	setSysVar((LPCTSTR)_T("clayer"), prevlayer);
	setCurLayer(prevlayer);
	getSysVar((LPCTSTR)_T("clayer"), clayer);
	//	ads_printf(_T("\n %s2 완료. Lay=%s b_name=%s \n"),_T(__FUNCTION__),m_Entity_Layer,m_bname);

	//*201406 한대희 블럭 그룹화 추가부분
	AcGePoint3d tmpP;
	AcDbObjectId entId;

	AcGeVector3d tmpV=AcGeVector3d::kXAxis;
	tmpV.rotateBy(45.0 * 3.141592 / 180.0, AcGeVector3d::kZAxis);
	tmpP = asPnt3d(inpt) + tmpV * 0.6 * (x_scale * 2);

	struct resbuf *filter = acutBuildList(RTDXF0, _T("INSERT"), 0);
	int ret = 0;
	ads_name sset, ename;
	ret = acedSSGet(_T("C"), inpt, asDblArray(tmpP), filter, sset);

	if(ret == RTNORM){
		long len;   
		acedSSLength(sset,&len);
		acedSSName(sset, 0L, ename);
		acdbGetObjectId(entId, ename);

		gIds.append(entId);
	}
	acedSSFree(sset);
	acutRelRb(filter);
	//*여기까지
}

// Block 삽입3
void CArx_Draw::ads_DrawBlock(AcGePoint3d insertPt,	// 삽입점
	CString m_bname,			// 블록 이름
	double x_scale,			// x축척
	double y_scale,			// y축척
	double angle)				// 각도
	//						  TCHAR* cclayer)			// 블록이 그려질 레이어
{
	//	ads_printf(_T("\n %s()3 시작. Lay=%s b_name=%s \n"),_T(__FUNCTION__),m_Entity_Layer,m_bname);
	//	ads_printf(_T("\n 1879 x_scale=%.2f  y_scale=%.2f \n"),x_scale,y_scale);

	// 테스트
	if(insertPt.x>ErrVal || insertPt.y>ErrVal || insertPt.z>ErrVal || x_scale>ErrVal || y_scale>ErrVal || angle>ErrVal ){
		//	ads_printf(_T("\n 89126 Error!! ads_DrawBlock1() insertPt.x=%.1f y=%.1f z=%.1f x_scale=%.1f y_scale=%.1f angle=%.1f \n"),insertPt.x,insertPt.y,insertPt.z,x_scale,y_scale,angle);
	}



	CString prevlayer;	// 이전레이어
	getSysVar((LPCTSTR)_T("clayer"), prevlayer);	// 이전레이어 보관

	ReadEnv();

	// 시스템변수 설정 clayer라는 현재 레이어 변수에 "dLineBlock" 레이어설정
	setSysVar((LPCTSTR)_T("clayer"), clayer);
	setCurLayer(clayer);


	ads_point inpt;

	inpt[X]=insertPt[0];
	inpt[Y]=insertPt[1];
	inpt[Z]=insertPt[2];

	ads_command(RTSTR,_T("REDRAW"),RTNONE);
	// 블록 삽입 명령
	//2016-09-30 한대희 수정
	ads_command(RTSTR, _T("-INSERT"), RTSTR, (LPTSTR)m_bname.operator LPCTSTR(), RTPOINT, inpt, RTREAL, x_scale, RTREAL, y_scale, RTREAL, angle, RTNONE);
	//DrawBlock_ZW2017(insertPt, m_bname, x_scale, y_scale, angle);

	ads_command(RTSTR,_T("REDRAW"),RTNONE);


	// 여기에 기존레이어 복구 코드 추가. 시스템변수 설정 clayer라는 현재 레이어 변수에 "0" 레이어설정(기존레이어로복구)
	setSysVar((LPCTSTR)_T("clayer"), prevlayer);
	setCurLayer(prevlayer);
	getSysVar((LPCTSTR)_T("clayer"), clayer);
	//	ads_printf(_T("\n %s2 완료. Lay=%s b_name=%s \n"),_T(__FUNCTION__),m_Entity_Layer,m_bname);
}



// 블록그리기 4
void CArx_Draw::DrawBlock(AcGePoint3d insertPt,
	CString m_bname,
	double x_scale,
	double y_scale,
	double angle)
{
	//	ads_printf(_T("\n %s()4 시작. 블록그리기 Lay=%s b_name=%s \n"),_T(__FUNCTION__),m_Entity_Layer,m_bname);
	//	ads_printf(_T("\n 1880 x_scale=%.2f  y_scale=%.2f \n"),x_scale,y_scale);

	// 테스트
	if(insertPt.x>ErrVal || insertPt.y>ErrVal || insertPt.z>ErrVal || x_scale>ErrVal || y_scale>ErrVal || angle>ErrVal){
		//	ads_printf(_T("\n 89127 Error!! ads_DrawBlock1() insertPt.x=%.1f y=%.1f z=%.1f x_scale=%.1f y_scale=%.1f angle=%.1f \n"),insertPt.x,insertPt.y,insertPt.z,x_scale,y_scale,angle);
	}

	
	//한대희 수정 2016-09-30
	CString strResource;
	AcDbObjectId blockId;
	CString bname;
	int loc,len;
	ads_point ins_pt;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL) return;

	AcDbDatabase* pDTemp = new AcDbDatabase(Adesk::kTrue);


	///block name 분리 
	bname = m_bname;
	if((loc=bname.ReverseFind( _T('/') )) == -1)
		loc=bname.ReverseFind( _T('\\') );
	len = bname.GetLength();
	bname=bname.Mid(loc+1,len-(loc+1));
	bname=bname.Left(bname.Find(_T(".")));

	es=pDTemp->readDwgFile(m_bname);

	if(pDb->insert(blockId,(LPTSTR)bname.operator LPCTSTR(),pDTemp,Adesk::kFalse)!=Acad::eOk){
		strResource.LoadStringW(IDS_STRING101);		// "\nBlock insert error"
		ads_printf(strResource);
		exit(0);
	}

	ins_pt[0] = insertPt.x;
	ins_pt[1] = insertPt.y;
	
	ads_command(RTSTR,_T("-INSERT"),RTSTR,(LPTSTR)bname.operator LPCTSTR(), RTPOINT,ins_pt,RTREAL,x_scale,RTREAL,y_scale,RTREAL,angle,RTNONE);
	//DrawBlock_ZW2017(insertPt, m_bname, x_scale, y_scale, angle);

	//        if(pDTemp)  delete pDTemp;

	//	ads_printf(_T("\n %s3 완료. 블록그리기 Lay=%s b_name=%s \n"),_T(__FUNCTION__),m_Entity_Layer,m_bname);
}

//ZWCAD 2017, ads_command Insert 명령 에러로 arx insert로 삽입 명령 추가
void CArx_Draw::DrawBlock_ZW2017(AcGePoint3d insertPt, CString m_bname, double x_scale, double y_scale, double angle){
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL){
		return;
	}
	AcDbDatabase* pDTemp = new AcDbDatabase(Adesk::kTrue);
	
	//block name 분리 
	AcDbObjectId blockId;
	CString strResource;
	CString bname = m_bname;
	int loc, len;
	if((loc=bname.ReverseFind( _T('/'))) == -1)
		loc=bname.ReverseFind( _T('\\'));
	len = bname.GetLength();
	bname=bname.Mid(loc+1,len-(loc+1));
	bname=bname.Left(bname.Find(_T(".")));

	es=pDTemp->readDwgFile(m_bname);
	
	AfxMessageBox(bname);
	if(pDb->insert(blockId,(LPTSTR)bname.operator LPCTSTR(),pDTemp,Adesk::kFalse)!=Acad::eOk){	
		strResource.LoadStringW(IDS_STRING101);		// "\nBlock insert error"
		ads_printf(strResource);
		exit(0);
	}
	AcDbBlockTable *pBT = NULL;
	AcDbBlockTableRecord *pBTRec = NULL;

	if(Acad::eOk != pDb->getBlockTable(pBT, AcDb::kForRead)){
		pBT->close();
		pBTRec->close();		
	}
	
	pBT->getAt(ACDB_MODEL_SPACE, pBTRec, AcDb::kForWrite);	

	AcDbBlockReference* blkRef = new AcDbBlockReference;
	blkRef->setBlockTableRecord(blockId);
	
	
	blkRef->setPosition(insertPt);
	blkRef->setScaleFactors(AcGeScale3d(x_scale, y_scale, 0));
	blkRef->setRotation(angle*3.1415926535897932384626433832795/180);

	blkRef->setDatabaseDefaults();
	pBTRec->appendAcDbEntity(blkRef);

	blkRef->close();
	pBTRec->close();
	pBT->close();

}



// 라인 작도1
//// Line을 생성한다.
Acad::ErrorStatus 
	CArx_Draw::DrawLine(AcGePoint3d spt,AcGePoint3d ept)
{

	CString strResource;
	Acad::ErrorStatus es;

	AcDbLine* m_Line = new AcDbLine(spt,ept);
	if(m_linetypeScale4Ent>0) {
		m_Line->setLinetypeScale(m_linetypeScale4Ent);
		m_linetypeScale4Ent=0;
	}

	if (!m_Line)
	{
		strResource.LoadStringW(IDS_STRING102);		// "선분을 생성할 메모리가 충분하지 않습니다!"
		ads_printf(strResource);
	}else{
		es=addToModelSpace(m_Line);
	}
	return es;
}

//201406 한대희 라인 작도 후 그룹핑
//// Line을 생성한다.
Acad::ErrorStatus 
	CArx_Draw::DrawLine(AcGePoint3d spt,AcGePoint3d ept, AcDbObjectIdArray& gIds)
{

	CString strResource;
	Acad::ErrorStatus es;

	AcDbLine* m_Line = new AcDbLine(spt,ept);

	if(m_linetypeScale4Ent>0) {
		m_Line->setLinetypeScale(m_linetypeScale4Ent);
		m_linetypeScale4Ent=0;
	}

	if (!m_Line)
	{
		strResource.LoadStringW(IDS_STRING102);		// "선분을 생성할 메모리가 충분하지 않습니다!"
		ads_printf(strResource);
	}else{
		es=addToModelSpace(m_Line);
		gIds.append(m_Line->id());
	}
	return es;
}

// 라인 작도2
Acad::ErrorStatus
	CArx_Draw::DrawLine(double x1,double y1,double x2,double y2)
{	
	CString strResource;
	Acad::ErrorStatus es;
	AcGePoint3d spt,ept;

	spt.x=x1;
	spt.y=y1;
	ept.x=x2;
	ept.y=y2;


	AcDbLine* m_Line = new AcDbLine(spt,ept);
	if (!m_Line)
	{
		strResource.LoadStringW(IDS_STRING103);		// "선분을 생성할 메모리가 충분하지 않습니다!"
		ads_printf(strResource);
	}
	else
	{
		es=addToModelSpace(m_Line);
	}
	return es;
}

// 라인 작도3
//// 지정한 색상으로 Line을 생성한다.
Acad::ErrorStatus 
	CArx_Draw::DrawLine(AcGePoint3d spt,AcGePoint3d ept, int iColor)
{
	CString strResource;
	Acad::ErrorStatus es;

	AcDbLine* m_Line = new AcDbLine(spt,ept);

	AcCmColor color;
	m_Line->setColorIndex(iColor);

	if (!m_Line)
	{
		strResource.LoadStringW(IDS_STRING102);		// "선분을 생성할 메모리가 충분하지 않습니다!"
		ads_printf(strResource);
	}else{
		es=addToModelSpace(m_Line,iColor);
	}
	return es;
}




// 라인 작도3
AcDbObjectId 
	CArx_Draw::DrawLineID(AcGePoint3d spt,AcGePoint3d ept)
{
	// 20081217 원진호 테스트 나중에 제거할것
	if(spt.x>ErrVal || spt.y>ErrVal || spt.z>ErrVal){
		//	ads_printf(_T("\n 89987 Error! f spt.x=%.1f spt.y=%.1f spt.z=%.1f "),spt.x,spt.y,spt.z);
		//	ads_printf(_T("\n 89987 Error! f spt.x=%s spt.y=%s spt.z=%s "),spt.x,spt.y,spt.z);
	}
	if(ept.x>ErrVal || ept.y>ErrVal || ept.z>ErrVal){
		//	ads_printf(_T("\n 89988 Error! s ept.x=%.1f ept.y=%.1f ept.z=%.1f "),ept.x,ept.y,ept.z);
		//	ads_printf(_T("\n 89988 Error! s ept.x=%s ept.y=%s ept.z=%s "),ept.x,ept.y,ept.z);
	}


	Acad::ErrorStatus es;
	AcDbObjectId objId=NULL;
	AcDbLine* m_Line = new AcDbLine(spt,ept);
	if(m_Line)	es=addToModelSpaceID(m_Line,objId);


	//	ads_printf(_T("\n DrawLineID() 완료. m_Cur_Color=%d \n"),m_Cur_Color);		
	return objId;
}


// 라인 작도4
// 20080424 원진호
Acad::ErrorStatus
	CArx_Draw::DrawLine(ads_point p1,ads_point p2)
{
	Acad::ErrorStatus es;
	CArx_User m_User;
	AcGePoint3d pt1,pt2;
	m_User.ads_arxpt(pt1,p1);
	m_User.ads_arxpt(pt2,p2);
	es = DrawLine(pt1,pt2);
	return es;
}


// 두 점간 거리 구하기
double CArx_Draw::getDistancePt(AcGePoint3d p1,AcGePoint3d p2)
{
	float ln2=(float)0, ln3=(float)0;
	double ln1=(double)0;//,ln2,ln3;

	// 피타고라스의 정리를 이용
	ln2 = (float)(p2.y - p1.y);
	ln3 = (float)(p2.x - p1.x);

	// 두 값은 길이이므로 절대 값을 구한다
	if(ln2<0)
		ln2=abs(ln2);
	if(ln3<0)
		ln3=abs(ln3);


	ln1 = ln2*ln2 + ln3*ln3;

	// c의 제곱근 구함 1894105.62714529
	ln1=sqrt(ln1);		// double 변수만 넣기

	return ln1;
}




Acad::ErrorStatus 
	CArx_Draw::DrawArc(const AcGePoint3d center,
	const AcGePoint3d spt,
	const AcGePoint3d ept)
{
	// 20081217 원진호 테스트 나중에 제거할것
	if(center.x>ErrVal || center.y>ErrVal || center.z>ErrVal){
		//	ads_printf(_T("\n 89995 Error! center.x=%.1f center.y=%.1f center.z=%.1f "),center.x,center.y,center.z);
	}
	if(spt.x>ErrVal || spt.y>ErrVal || spt.z>ErrVal){
		//	ads_printf(_T("\n 89996 Error! spt.x=%.1f spt.y=%.1f spt.z=%.1f "),spt.x,spt.y,spt.z);
	}
	if(ept.x>ErrVal || ept.y>ErrVal || ept.z>ErrVal){
		//	ads_printf(_T("\n 89997 Error! ept.x=%.1f ept.y=%.1f ept.z=%.1f "),ept.x,ept.y,ept.z);
	}

	double startAngle=(double)0;
	double endAngle=(double)0;
	double radius=(double)0;

	ads_point center_pt,spt_pt,ept_pt;

	center_pt[0] = center[0];center_pt[1] = center[1];center_pt[2] = center[2];
	spt_pt[0] = spt[0];spt_pt[1] = spt[1];spt_pt[2] = spt[2];
	ept_pt[0] = ept[0];ept_pt[1] = ept[1];ept_pt[2] = ept[2];

	startAngle=ads_angle(center_pt,spt_pt);
	endAngle = ads_angle(center_pt,ept_pt);
	radius = ads_distance(center_pt,spt_pt);

	AcDbArc* m_Arc = new AcDbArc(center,radius,startAngle,endAngle);

	return addToModelSpace(m_Arc);

}

//Circle을 생성한다.
Acad::ErrorStatus 
	CArx_Draw::DrawCircle(AcGePoint3d cpt,double radius)
{
	if(cpt.x>ErrVal || cpt.y>ErrVal || cpt.z>ErrVal){
		//	ads_printf(_T("\n 91234 Error! cpt.x=%.1f cpt.y=%.1f cpt.z=%.1f "),cpt.x,cpt.y,cpt.z);
	}
	if(radius>ErrVal){
		//	ads_printf(_T("\n 91235 Error! radius=%.1f "),radius);
	}

	AcGeVector3d normal(0,0,1);
	AcDbCircle* m_Circle = new AcDbCircle(cpt,normal,radius);

	return addToModelSpace(m_Circle);

}
///PolyLine create
Acad::ErrorStatus 
	CArx_Draw::DrawPLine(AcGePoint3dArray ptArr,double width)
{
	int length = ptArr.length();
	//	ads_printf(_T("\n length(%d) width(%.1f) \n"),length,width);

	AcDb2dPolyline *pNewPline = new AcDb2dPolyline(AcDb::k2dSimplePoly,ptArr,0,Adesk::kFalse,width,width,NULL);

	return addToModelSpace(pNewPline);
}

// PolyLine create 2  20091029 원진호 추가 Id가 필요해서 만들었다
Acad::ErrorStatus 
	CArx_Draw::DrawPLine(AcGePoint3dArray ptArr,double width, AcDbObjectId& objId)
{
	int length = ptArr.length();
	//	ads_printf(_T("\n length(%d) width(%.1f) \n"),length,width);

	AcDb2dPolyline *pNewPline = new AcDb2dPolyline(AcDb::k2dSimplePoly,ptArr,0,Adesk::kFalse,width,width,NULL);
	return addToModelSpaceID(pNewPline,objId);
}



Acad::ErrorStatus 
	CArx_Draw::DrawBox(AcGePoint3d lefttop,AcGePoint3d rightbottom)
{
	AcGePoint3d leftbottom(lefttop[0],rightbottom[1],0);
	AcGePoint3d rignttop(rightbottom[0],lefttop[1],0);
	Acad::ErrorStatus es;

	es=DrawLine(lefttop,rignttop);
	es=DrawLine(rignttop,rightbottom);
	es=DrawLine(rightbottom,leftbottom);
	es=DrawLine(leftbottom,lefttop);

	return Acad::eOk;
}

/////////////////////////////////////////////////////////////////////////////
// CCreateDrawFunc message handlers
//Entity를 Database에 쓴다.
Acad::ErrorStatus
	CArx_Draw::addToModelSpace(AcDbEntity* pEnt)
{

	CString strResource;
	CString					s;
	Acad::ErrorStatus		es;
	AcDbBlockTable*			pBlockTable = NULL;
	AcDbBlockTableRecord*	pBlockTableRecord = NULL;
	AcDbObjectId			idEnt;
	AcDbDatabase*			pDb=NULL;



	try{

		pDb = acdbHostApplicationServices()->workingDatabase();
		if (pDb == NULL) return Acad::eNoDatabase;

		//	ads_printf(_T("\n89115 m_Entity_Layer=%s lineType=%s \n"),m_Entity_Layer,m_Entity_Linetype);
		// Default to current database
		if(pEnt==NULL) return Acad::eNullObjectPointer;
		else {// Set Property
			if ((es=pEnt->setLayer(m_Entity_Layer)) != Acad::eOk) {
				strResource.LoadStringW(IDS_STRING104);		// "\n Layer(%s) Set Error: %s"
				s.Format(strResource, m_Entity_Layer,acadErrorStatusText(es));
				//           AfxMessageBox(s);
				//           acrx_abort("\n Layer Set Error: %s", acadErrorStatusText(es));
			}

			if ((es=pEnt->setColor(getCurColor(),Adesk::kTrue)) != Acad::eOk) {
				strResource.LoadStringW(IDS_STRING105);		// "\n Color Set Error: %s"
				s.Format(strResource, acadErrorStatusText(es));
				//			AfxMessageBox(s);
				//            acrx_abort("\n Color Set Error: %s", acadErrorStatusText(es));
			}

			if ((es=pEnt->setLinetype(m_Entity_Linetype)) != Acad::eOk) {
				strResource.LoadStringW(IDS_STRING106);		// "\n Linetype Set Error: %s"
				s.Format(strResource, acadErrorStatusText(es));
				//            AfxMessageBox(s);
				//            acrx_abort("\n Linetype Set Error: %s", acadErrorStatusText(es));
			}

		}//end- Set Property
		// ---------------------------------------------------------------------------------

		// Get block table
		if((es = pDb->getBlockTable( pBlockTable, AcDb::kForRead )) != Acad::eOk)
			return es;

		// Find block record
		if((es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite )) != Acad::eOk)
		{
			pBlockTable->close();
			return es;
		}


		// append new entity to block:
		es = pBlockTableRecord->appendAcDbEntity( idEnt, pEnt );

		pBlockTableRecord->close();
		pBlockTable->close();


		if(es==Acad::eOk){
			pEnt->close();
		}else 
			delete pEnt;
	}// end-try

	catch(const Acad::ErrorStatus es)
	{
		if(pEnt)
			delete pEnt;
		strResource.LoadStringW(IDS_STRING107);		// "addToModelSpace : es = %s"
		ads_printf(strResource,acadErrorStatusText(es));
	}
	catch(CException *e)
	{
		strResource.LoadStringW(IDS_STRING108);		// "addToModelSpace-> MFC Exception!"
		AfxMessageBox(strResource);
		e->Delete();
	}
	catch(...)
	{
		strResource.LoadStringW(IDS_STRING109);		// "\nBlock insert error"
		AfxMessageBox(strResource);
	}
	return es;
}

/////////////////////////////////////////////////////////////////////////////
// CCreateDrawFunc message handlers
// 사용자가 지정한 색으로 Entity를 Database에 쓴다. 20091031 원진호 추가
Acad::ErrorStatus
	CArx_Draw::addToModelSpace(AcDbEntity* pEnt, int iColor)
{

	CString strResource;
	CString					s;
	Acad::ErrorStatus		es;
	AcDbBlockTable*			pBlockTable = NULL;
	AcDbBlockTableRecord*	pBlockTableRecord = NULL;
	AcDbObjectId			idEnt;
	AcDbDatabase*			pDb=NULL;



	try{

		pDb = acdbHostApplicationServices()->workingDatabase();
		if (pDb == NULL) return Acad::eNoDatabase;

		//	ads_printf(_T("\n89115 m_Entity_Layer=%s lineType=%s \n"),m_Entity_Layer,m_Entity_Linetype);
		// Default to current database
		if(pEnt==NULL) return Acad::eNullObjectPointer;
		else {// Set Property
			if ((es=pEnt->setLayer(m_Entity_Layer)) != Acad::eOk) {
				strResource.LoadStringW(IDS_STRING104);		// "\n Layer(%s) Set Error: %s"
				s.Format(strResource, m_Entity_Layer,acadErrorStatusText(es));
				//           AfxMessageBox(s);
				//           acrx_abort("\n Layer Set Error: %s", acadErrorStatusText(es));
			}
			AcCmColor color;
			color.setColorIndex(iColor);	// 사용자가 지정한 컬러로 설정한다

			if ((es=pEnt->setColor(color,Adesk::kTrue)) != Acad::eOk) {
				strResource.LoadStringW(IDS_STRING105);		// "\n Color Set Error: %s"
				s.Format(strResource, acadErrorStatusText(es));
				//			AfxMessageBox(s);
				//            acrx_abort("\n Color Set Error: %s", acadErrorStatusText(es));
			}

			if ((es=pEnt->setLinetype(m_Entity_Linetype)) != Acad::eOk) {
				strResource.LoadStringW(IDS_STRING106);		// "\n Linetype Set Error: %s"
				s.Format(strResource, acadErrorStatusText(es));
				//            AfxMessageBox(s);
				//            acrx_abort("\n Linetype Set Error: %s", acadErrorStatusText(es));
			}

		}//end- Set Property
		// ---------------------------------------------------------------------------------

		// Get block table
		if((es = pDb->getBlockTable( pBlockTable, AcDb::kForRead )) != Acad::eOk)
			return es;

		// Find block record
		if((es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite )) != Acad::eOk)
		{
			pBlockTable->close();
			return es;
		}


		// append new entity to block:
		es = pBlockTableRecord->appendAcDbEntity( idEnt, pEnt );

		pBlockTableRecord->close();
		pBlockTable->close();


		if(es==Acad::eOk){
			pEnt->close();
		}else 
			delete pEnt;
	}// end-try

	catch(const Acad::ErrorStatus es)
	{
		if(pEnt)
			delete pEnt;
		strResource.LoadStringW(IDS_STRING107);		// "addToModelSpace : es = %s"
		ads_printf(strResource,acadErrorStatusText(es));
	}
	catch(CException *e)
	{
		strResource.LoadStringW(IDS_STRING108);		// "addToModelSpace-> MFC Exception!"
		AfxMessageBox(strResource);
		e->Delete();
	}
	catch(...)
	{
		strResource.LoadStringW(IDS_STRING109);		// "\nBlock insert error"
		AfxMessageBox(strResource);
	}
	return es;
}


Acad::ErrorStatus
	CArx_Draw::addToModelSpaceID(AcDbEntity* pEnt,AcDbObjectId& objId)
{

	CString strResource;
	CString					s;
	Acad::ErrorStatus		es;
	AcDbBlockTable*			pBlockTable = NULL;
	AcDbBlockTableRecord*	pBlockTableRecord = NULL;
	AcDbDatabase*			pDb=NULL;

	try
	{// Default to current database
		pDb = acdbHostApplicationServices()->workingDatabase();
		if (pDb == NULL) return Acad::eNoDatabase;

		//	ads_printf(_T("\n89120 m_Entity_Layer=%s lineType=%s \n"),m_Entity_Layer,m_Entity_Linetype);


		if(pEnt==NULL) return Acad::eNullObjectPointer;
		else {// Set Property
			if ((es=pEnt->setLayer(m_Entity_Layer)) != Acad::eOk) {
				strResource.LoadStringW(IDS_STRING110);		// "\n Layer(%s) Set Error: %s"
				s.Format(strResource, m_Entity_Layer,acadErrorStatusText(es));
				//           AfxMessageBox(s);
				//           acrx_abort("\n Layer Set Error: %s", acadErrorStatusText(es));
			}

			if ((es=pEnt->setColor(getCurColor(),Adesk::kTrue)) != Acad::eOk) {
				strResource.LoadStringW(IDS_STRING111);		// "\n Color Set Error: %s"
				s.Format(strResource, acadErrorStatusText(es));
				//			AfxMessageBox(s);
				//            acrx_abort("\n Color Set Error: %s", acadErrorStatusText(es));
			}

			if ((es=pEnt->setLinetype(m_Entity_Linetype)) != Acad::eOk) {
				strResource.LoadStringW(IDS_STRING112);		// "\n Linetype Set Error: %s"
				s.Format(strResource, acadErrorStatusText(es));		////////////////////////////////////////
				//            AfxMessageBox(s);
				//            acrx_abort("\n Linetype Set Error: %s", acadErrorStatusText(es));
			}
		}//end- Set Property
		// ---------------------------------------------------------------------------------


		// Get block table
		if((es = pDb->getBlockTable( pBlockTable, AcDb::kForRead )) != Acad::eOk)
			return es;

		// Find block record
		if((es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite )) != Acad::eOk)
		{
			pBlockTable->close();
			return es;
		}


		// append new entity to block:
		es = pBlockTableRecord->appendAcDbEntity( objId, pEnt );

		pBlockTableRecord->close();
		pBlockTable->close();



		if(es==Acad::eOk){
			pEnt->close();
		}else 
			delete pEnt;
	}
	catch(const Acad::ErrorStatus es)
	{
		if(pEnt)
			delete pEnt;
		strResource.LoadStringW(IDS_STRING113);		// "addToModelSpace : es = %s"
		ads_printf(strResource,acadErrorStatusText(es));
	}
	catch(CException *e)
	{
		strResource.LoadStringW(IDS_STRING114);		// "addToModelSpace-> MFC Exception!"
		AfxMessageBox(strResource);
		e->Delete();
	}
	catch(...)
	{
		strResource.LoadStringW(IDS_STRING115);		// "addToModelSpace-> Win32 Exception"
		AfxMessageBox(strResource);
	}
	return es;
}


AcDbObjectId CArx_Draw::getCLayer()
{
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL) return NULL;

	return pDb->clayer();
}

void CArx_Draw::setCLayer(TCHAR *layer)		// 거의 안쓰는 함수
{
	AcDbObjectId objId;
	AcDbLayerTable* pLayerTable;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL) return ;

	pDb->getLayerTable(pLayerTable,AcDb::kForRead);
	if(pLayerTable->getAt(layer,objId,Adesk::kFalse)!=Acad::eOk){
		pLayerTable->close();
		return;
	}
	pLayerTable->close();
	pDb->setClayer(objId);
}

void CArx_Draw::setCLayer(AcDbObjectId objId)
{
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL) return;
	pDb->setClayer(objId);
}
////Current Layer를 셋팅한다.	클래스 멤버변수에 설정
void CArx_Draw::setCurLayer(CString layer)
{
	_tcscpy(m_Entity_Layer, (LPCTSTR)layer);
}
////Current LineType를 셋팅한다.
void CArx_Draw::setCurLinetype(CString linetype)
{
	//m_Entity_Linetype = linetype;
	_tcscpy(m_Entity_Linetype, (LPCTSTR)linetype);
}

////Current Color를 셋팅한다. 멤버변수를 세팅한다
void CArx_Draw::setCurColor(int col)
{
	m_Cur_Color = col;
}



void CArx_Draw::setCurColor()	// 시스템변수와 멤버변수에 설정
{
	short color;
	int col;
	getSysVar((LPCTSTR)_T("USERI5"),col);	// 오배수관 색상
	color = (short)col;

	if(col == 0)
		ads_command(RTSTR,_T(".COLOR"),RTSTR,_T("BYBLOCK"),RTNONE);
	else if(col == 256)
		ads_command(RTSTR,_T(".COLOR"),RTSTR,_T("BYLAYER"),RTNONE);
	else	// 블록색지정
		ads_command(RTSTR,_T(".COLOR"),RTSHORT,color,RTNONE);

	m_Cur_Color = col;
}

////Current Color를 리턴한다. 멤버변수를 반환한다
const AcCmColor CArx_Draw::getCurColor()
{
	AcCmColor color;
	color.setColorIndex(m_Cur_Color);
	return color;
}

////Current Layer를 셋팅한다. 멤버변수를 반환한다
void CArx_Draw::setCurTextStyle(CString style)
{
	_tcscpy(m_Entity_Text_Style, (LPCTSTR)style);
}

///////////////////////////////////////////
///////////////////////////////////////////
Acad::ErrorStatus
	CArx_Draw::setVar(LPCTSTR varName, const resbuf* newVal){

		CString strResource;
		CString str;
		if(ads_setvar(varName, newVal) != RTNORM) {
			strResource.LoadStringW(IDS_STRING116);		// "Could not set system variable \"%s\"."
			str.Format(strResource, varName);//ArxDbgUtils::stopAlertBox(str);
			return(Acad::eInvalidInput);
		}else{
			return(Acad::eOk);
		}
}
Acad::ErrorStatus
	CArx_Draw::getSysVar(LPCTSTR varName, int& val)
{
	resbuf rb;
	if (ads_getvar(varName, &rb)== RTNORM) {
		ASSERT(rb.restype == RTSHORT);
		val = rb.resval.rint;
		return(Acad::eOk);
	}
	else{
		return(Acad::eInvalidInput);
	}
}

Acad::ErrorStatus
	CArx_Draw::getSysVar(LPCTSTR varName, double& val)
{
	resbuf rb;
	if (ads_getvar(varName, &rb) == RTNORM) {
		ASSERT(rb.restype == RTREAL);
		val = rb.resval.rreal;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}


Acad::ErrorStatus
	CArx_Draw::getSysVar(LPCTSTR varName, CString& val)
{
	resbuf rb;

	if (ads_getvar(varName, &rb) == RTNORM) {
		ASSERT(rb.restype == RTSTR);
		val = rb.resval.rstring;
		free(rb.resval.rstring);
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}


Acad::ErrorStatus
	CArx_Draw::getSysVar(LPCTSTR varName, AcGePoint2d& val)
{
	resbuf rb;
	if (ads_getvar(varName, &rb) == RTNORM) {
		ASSERT(rb.restype == RTPOINT);
		val.set(rb.resval.rpoint[X], rb.resval.rpoint[Y]);
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

Acad::ErrorStatus
	CArx_Draw::getSysVar(LPCTSTR varName, AcGePoint3d& val)
{
	resbuf rb;
	if (ads_getvar(varName, &rb) == RTNORM) {
		ASSERT(rb.restype == RT3DPOINT);
		val.set(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]);
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}


Acad::ErrorStatus
	CArx_Draw::setSysVar(LPCTSTR varName, int val)
{
	ASSERT(varName != NULL);

	resbuf rb;
	rb.restype = RTSHORT;
	rb.resval.rint = (short)val;

	return(setVar(varName, &rb));
}

Acad::ErrorStatus
	CArx_Draw::setSysVar(LPCTSTR varName, double val){
		ASSERT(varName != NULL);
		resbuf rb;
		rb.restype = RTREAL;
		rb.resval.rreal = val;
		return(setVar(varName, &rb));
}
Acad::ErrorStatus
	CArx_Draw::setSysVar(LPCTSTR varName, const TCHAR* val){
		ASSERT(varName != NULL);
		resbuf rb;
		rb.restype = RTSTR;
		rb.resval.rstring = const_cast<TCHAR*>(val);
		return(setVar(varName, &rb));
}


/*-
p1                 pint               p2
*------------------+------------------*

pint를 중심으로 왼쪽과  오른쪽으로 분리 한다.
*/

void CArx_Draw::arx_BLineBreak(ads_name& ename1, ads_name& ename2,ads_name ebase)
{//	ads_printf(_T("\n arx_BLineBreak 1 \n"));
	CString old_Linetype,new_Linetype;
	AcDbLine* pLine;
	AcGePoint3d pt1,pt2,pt3,pt4,pint;
	CArx_User m_User;
	CArx_Xdata m_Xdata;
	AcDbObjectId objId1,objId2;
	struct  resbuf  *pRb;
	Acad::ErrorStatus errs;
	old_Linetype=m_Entity_Linetype;
	errs=acdbGetObjectId(objId1,ename1);
	if(errs==Acad::eInvalidAdsName)return;
	errs=acdbGetObjectId(objId2,ebase);
	if(errs==Acad::eInvalidAdsName)return;
	m_User.arx_getLine(objId1, pt1,pt2);
	m_User.arx_getLine(objId2, pt3,pt4);
	if(m_User.arx_ptInter(pt1,pt2,pt3,pt4,0,pint) != RTNORM)
		return;
	acdbOpenObject(pLine,objId1,AcDb::kForWrite);
	new_Linetype=pLine->linetype();
	setCurLinetype(new_Linetype);
	pRb=pLine->xData(NULL);
	pLine->erase();
	pLine->close();
	objId2=DrawLineID(pint,pt2);
	m_Xdata.gl_AddXdata(pRb,objId2);
	ads_entlast(ename2);
	objId1=DrawLineID(pt1,pint);
	m_Xdata.gl_AddXdata(pRb,objId1);
	ads_entlast(ename1);
	if(pRb!=NULL)
		ads_relrb(pRb);
	ads_command(RTSTR,_T("REDRAW"),RTNONE); ///추가부분
	setCurLinetype(old_Linetype);
}


/*-
p1                 pint               p2
*------------------+------------------*

pint를 중심으로 왼쪽과  오른쪽으로 분리 한다.
*/
void CArx_Draw::arx_BLineBreak(AcDbObjectId objId, AcGePoint3d pint)
{
	AcDbLine* pLine;
	AcDbObjectId TmpId1,TmpId2;
	AcGePoint3d p1,p2;
	CArx_User m_User;

	m_User.arx_getLine(objId, p1,p2);
	TmpId1=DrawLineID(p1,pint);
	CopyProperty(TmpId1, objId);
	//삭제
	acdbOpenObject(pLine,objId,AcDb::kForWrite);
	pLine->erase();
	pLine->close();

	TmpId2=DrawLineID(pint,p2);
	CopyProperty(TmpId2, TmpId1);
}
/// pint를 기준으로 자르고, 도형 객체를 돌려줌 
void CArx_Draw::arx_BLineBreak(ads_name& en1,ads_name& en2,AcGePoint3d pint,
	AcGePoint3d pts,AcGePoint3d pte)
{
	AcDbLine* pLine;
	AcGePoint3d pt1,pt2;
	CArx_User m_User;
	CArx_Xdata m_Xdata;
	AcDbObjectId objId,objId1,objId2;
	struct  resbuf  *pRb=NULL;

	acdbGetObjectId(objId,en1);

	m_User.arx_getLine(objId, pt1,pt2);

	if(pts.distanceTo(pt1)<pte.distanceTo(pt1)){
		objId1=DrawLineID(pt1,pint);
		CopyProperty(objId1, objId);  //속성복사

		acdbOpenObject(pLine,objId,AcDb::kForWrite);
		pRb=pLine->xData(NULL);
		pLine->erase();
		pLine->close();
		m_Xdata.gl_AddXdata(pRb,objId1);  //확장 데이터
		ads_entlast(en1);

		objId2=DrawLineID(pint,pt2);
		CopyProperty(objId2, objId1);  //속성복사
		m_Xdata.gl_AddXdata(pRb,objId2);
		ads_entlast(en2);
	}else{
		objId1=DrawLineID(pint,pt2);
		CopyProperty(objId1, objId);  //속성복사

		acdbOpenObject(pLine,objId,AcDb::kForWrite);
		pRb=pLine->xData(NULL);
		pLine->erase();
		pLine->close();
		m_Xdata.gl_AddXdata(pRb,objId1); //확장 데이터
		ads_entlast(en1);

		objId2=DrawLineID(pt1,pint);
		CopyProperty(objId2, objId1);  //속성복사
		m_Xdata.gl_AddXdata(pRb,objId2);
		ads_entlast(en2);
	}
	if(pRb!=NULL)
		ads_relrb(pRb);
}

void CArx_Draw::arx_LineBreak(AcDbObjectId objId,AcGePoint3d p0,
	double left, double right)
{
	CArx_Math m_Math;
	CArx_User m_User;
	CArx_Xdata m_Xdata;
	AcDbObjectId objRId1,objRId2;
	AcGePoint3d p1,p2,ps1,ps2;
	AcDbLine* pLine;
	struct  resbuf  *pRb=NULL;

	if (left==0 && right==0) return;

	m_User.arx_getLine(objId, p1,p2);
	double ang = m_Math.arx_angle(p1,p2);

	ps1=m_Math.Polar(p0,left,m_Math.RTD(ang)-180);
	ps2=m_Math.Polar(p0,right,m_Math.RTD(ang));


	CArx_Draw m_Draw;

	int ip1 = m_Math.arx_perp(ps1, p1,p2);
	int ip2 = m_Math.arx_perp(ps2, p1,p2);



	try{
		if (ip1 && ip2){


			objRId1=DrawLineID(p1,ps1);
			CopyProperty(objRId1, objId);  //속성복사


			acdbOpenObject(pLine,objId,AcDb::kForWrite);
			pRb=pLine->xData(NULL);
			pLine->erase();
			pLine->close();

			m_Xdata.gl_AddXdata(pRb,objRId1);	
			objRId2=DrawLineID(ps2,p2);

			CopyProperty(objRId2, objRId1);  //속성복사
			m_Xdata.gl_AddXdata(pRb,objRId2);



		}else if (ip1){

			objRId1=DrawLineID(p1,ps1);
			CopyProperty(objRId1, objId);  //속성복사

			acdbOpenObject(pLine,objId,AcDb::kForWrite);
			pRb=pLine->xData(NULL);
			pLine->erase();
			pLine->close();	
			m_Xdata.gl_AddXdata(pRb,objRId1);




		}else if (ip2){

			objRId1=DrawLineID(ps2,p2);
			CopyProperty(objRId1, objId);  //속성복사

			acdbOpenObject(pLine,objId,AcDb::kForWrite);
			pRb=pLine->xData(NULL);
			pLine->erase();
			pLine->close();	
			m_Xdata.gl_AddXdata(pRb,objRId1);

		}

		if(pRb!=NULL)
			ads_relrb(pRb);
	}
	catch (...)
	{
		AfxMessageBox(_T("arx_LineBreak Error1"));
	}		


}


void CArx_Draw::arx_MLineBreak(AcDbObjectId objId,AcGePoint3d p0,
	double left, double right)
{
	CArx_Math m_Math;
	CArx_User m_User;
	CArx_Xdata m_Xdata;
	AcDbObjectId objRId;
	AcGePoint3d p1,p2;
	AcDbEntity *pLine = NULL;
	struct  resbuf  *pRb=NULL;
	TCHAR *old_Linetype,*new_Linetype;

	if (left==0 && right==0)
		return; 


	old_Linetype = (TCHAR *) malloc((_tcslen(m_Entity_Linetype)+1)*sizeof(TCHAR));
	_tcscpy(old_Linetype,m_Entity_Linetype);
	m_User.arx_getLine(objId, p1,p2);
	double ang = m_Math.arx_angle(p1,p0);

	Acad::ErrorStatus es=acdbOpenAcDbEntity(pLine, objId,AcDb::kForWrite);

	new_Linetype = (TCHAR *)malloc((_tcslen(pLine->linetype())+1)*sizeof(TCHAR));
	_tcscpy(new_Linetype,pLine->linetype());
	setCurLinetype(new_Linetype);
	free(new_Linetype);


	resbuf *xdata = pLine->xData(NULL);

	pLine->erase();
	pLine->close();


	p2=m_Math.Polar(p0,left,m_Math.RTD(ang)-180);
	objRId=DrawLineID(p1,p2);

	if(xdata != NULL)
	{
		m_Xdata.gl_AddXdata(xdata,objRId);
		ads_relrb(xdata);
	}

	setCurLinetype(old_Linetype);
	free(old_Linetype);
}

///도면상에 TEXT를 출력한다.
void CArx_Draw::TextOut(AcGePoint3d textorgpt, TCHAR *as_Str, 
	double ar_TextHeight, double ar_Angle,TCHAR *aligntype)
{
	//	ads_printf(_T("\n CArx_Draw::TextOut 들어옴 \n"));

	Acad::ErrorStatus es;
	AcDbObjectId textStyleId;
	CArx_Math m_Math;
	double xScale=(double)0;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL) return;
	//	AcDbTextStyleTable* pTextStyleTbl;
	AcDbText* pText = new AcDbText();
	/*	acdbCurDwg()->getTextStyleTable(pTextStyleTbl, AcDb::kForRead);
	if (pTextStyleTbl->getAt(m_Entity_Text_Style, textStyleId) != Acad::eOk)
	{
	pTextStyleTbl->getAt("STANDARD", textStyleId);
	}*/

	textStyleId=pDb->textstyle();
	//    pTextStyleTbl->close();
	pText->setTextStyle(textStyleId);
	AcDbTextStyleTableRecord *pTSTR=NULL;
	acdbOpenObject(pTSTR, textStyleId,AcDb::kForRead);
	double obliquingAngle=0.0;
	if(pTSTR){
		xScale=pTSTR->xScale();
		obliquingAngle=pTSTR->obliquingAngle();   // 2013-01-28 kslee
		pTSTR->close();
		pText->setWidthFactor(xScale);
	}else{
		pText->setHeight(ar_TextHeight);	// 여기 죽어도 안들어옴
	}
	pText->setHeight(ar_TextHeight);	// 20070903 원진호 글자 사이즈 값이 수정이 안되므로 코드추가
	pText->setPosition(textorgpt);
	pText->setTextString(as_Str);
	pText->setOblique(obliquingAngle);  // 2013-01-28 kslee
	pText->setRotation(m_Math.DTR(ar_Angle));
	pText->setVerticalMode(AcDb::kTextBase);
	if (_tcscmp(aligntype,_T("LEFT"))==0){
		es= pText->setHorizontalMode(AcDb::kTextLeft);
	}else if (_tcscmp(aligntype,_T("CENTER"))==0) {
		es= pText->setHorizontalMode(AcDb::kTextCenter);
	}else if (_tcscmp(aligntype,_T("RIGHT"))==0) {
		es= pText->setHorizontalMode(AcDb::kTextRight);
	}else if (_tcscmp(aligntype,_T("MID"))==0) {
		es= pText->setHorizontalMode(AcDb::kTextMid);
	}else	es= pText->setHorizontalMode(AcDb::kTextLeft);
	pText->setAlignmentPoint(textorgpt);
	if (pText != NULL) {
		es=addToModelSpace(pText);
		es = pText->close();
	}
}

//201406 한대희 도면상에 TEXT 출력후 그룹 추가
void CArx_Draw::TextOut(AcGePoint3d textorgpt, TCHAR *as_Str, 
	double ar_TextHeight, double ar_Angle,TCHAR *aligntype, AcDbObjectIdArray& gIds)
{
	//	ads_printf(_T("\n CArx_Draw::TextOut 들어옴 \n"));

	Acad::ErrorStatus es;
	AcDbObjectId textStyleId;
	CArx_Math m_Math;
	double xScale=(double)0;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL) return;
	//	AcDbTextStyleTable* pTextStyleTbl;
	AcDbText* pText = new AcDbText();
	/*	acdbCurDwg()->getTextStyleTable(pTextStyleTbl, AcDb::kForRead);
	if (pTextStyleTbl->getAt(m_Entity_Text_Style, textStyleId) != Acad::eOk)
	{
	pTextStyleTbl->getAt("STANDARD", textStyleId);
	}*/

	textStyleId=pDb->textstyle();
	//    pTextStyleTbl->close();
	pText->setTextStyle(textStyleId);
	AcDbTextStyleTableRecord *pTSTR=NULL;
	acdbOpenObject(pTSTR, textStyleId,AcDb::kForRead);
	double obliquingAngle=0.0;
	if(pTSTR){
		xScale=pTSTR->xScale();
		obliquingAngle=pTSTR->obliquingAngle();   // 2013-01-28 kslee
		pTSTR->close();
		pText->setWidthFactor(xScale);
	}else{
		pText->setHeight(ar_TextHeight);	// 여기 죽어도 안들어옴
	}
	pText->setHeight(ar_TextHeight);	// 20070903 원진호 글자 사이즈 값이 수정이 안되므로 코드추가
	pText->setPosition(textorgpt);
	pText->setTextString(as_Str);
	pText->setOblique(obliquingAngle);  // 2013-01-28 kslee
	pText->setRotation(m_Math.DTR(ar_Angle));
	pText->setVerticalMode(AcDb::kTextBase);
	if (_tcscmp(aligntype,_T("LEFT"))==0){
		es= pText->setHorizontalMode(AcDb::kTextLeft);
	}else if (_tcscmp(aligntype,_T("CENTER"))==0) {
		es= pText->setHorizontalMode(AcDb::kTextCenter);
	}else if (_tcscmp(aligntype,_T("RIGHT"))==0) {
		es= pText->setHorizontalMode(AcDb::kTextRight);
	}else if (_tcscmp(aligntype,_T("MID"))==0) {
		es= pText->setHorizontalMode(AcDb::kTextMid);
	}else	es= pText->setHorizontalMode(AcDb::kTextLeft);
	pText->setAlignmentPoint(textorgpt);
	if (pText != NULL) {
		es=addToModelSpace(pText);
		es = pText->close();
	}
	gIds.append(pText->id());
}

void CArx_Draw::gf_Linetypecolorchange(ads_name en)
{
	AcDbObjectId objId;
	AcDbLine* pLine;
	acdbGetObjectId(objId,en);
	acdbOpenObject(pLine,objId,AcDb::kForWrite);
	pLine->setLinetype(m_Entity_Linetype);
	pLine->setColor(getCurColor());
	pLine->close();
}
void CArx_Draw::gf_Linetypecolorchange(ads_name en,AcGePoint3d pt1,AcGePoint3d pt2)
{
	AcDbObjectId objId;
	AcDbLine* pLine;
	CArx_Xdata m_Xdata;
	struct  resbuf  *pRb=NULL;
	acdbGetObjectId(objId,en);
	acdbOpenObject(pLine,objId,AcDb::kForWrite);
	pRb=pLine->xData(NULL);
	pLine->erase();
	pLine->close();
	objId=DrawLineID(pt1,pt2);
	m_Xdata.gl_AddXdata(pRb,objId);
	if(pRb != NULL)
		ads_relrb(pRb);
}
CString CArx_Draw::gfGetSymName(int ptype)
{
	CString sNo;
	TCHAR *JOINT[115] = {_T("SF11L"),_T("SF10L"),_T("VALD01"),_T("SF20T"),_T("SF00L"),
		//부품(0~)
		_T("FICR01"),_T("FIER01"),_T("FIUN01"),_T("FIFL01"),_T("FIFL02"),
		_T("BCAB1"),_T("BHAN1"), _T("L"),_T("FILJ11"),_T("VALJ01"),
		_T("VALB03"),_T("VALE01"),_T("VALE02"),_T("VALS05"),_T("VALA01"),
		_T("DCWT01"),_T("FRWH01"),_T("BFLG1"),_T("UTRAP"),_T("BUSI"),
		_T("STRAP"),_T(""),_T(""),_T(""),_T(""),
		//위생(30~)
		_T("JCY022"),_T("JCYT22"),_T("KJRD03"),_T("JPTR"),_T(""),
		_T(""),_T(""),_T(""),_T(""),_T(""),
		//밸브(40~)
		_T("VALG01"),_T("VALG02"),_T("VALC01"),_T("VALB01"),_T("VAL201"),
		_T("VAL301"),_T("VALS03"),_T("VALB04"),_T("VALS01"),_T("VALS04"),
		_T("VALP01"),_T("VALP02"),_T("VALB02"),_T("VALG05"),_T("VALA02"),
		_T("VALT01"),_T("VALP03"),_T("VALG04"),_T("VALF02"),_T("VALP04"),
		_T("VALF01"),_T("VALA03"),_T("VALY01"),_T("NAVIPAS"),_T("NAVICHK"),
		_T(""),_T(""),_T(""),_T(""),_T(""),
		//어셈블리(70~)
		_T("ASM-2W"),_T("ASM-2WV"),_T("ASM-VAL"),_T("ASM-WC"),_T("ASM-ELC"),
		_T("ASM-KAM"),_T("ASM-CHA"),_T("ASM-PD"),_T("PMPASM"),_T("PMPASM1"),
		_T(""),_T(""),_T(""),_T(""),_T(""),
		//소방(84~)
		_T("FRWT01"),_T("FRWT02"),_T(""),_T("FRFA01"),_T("FRAR01"),
		_T("FRSP01"),_T("FRSP02"),_T("FRSP03"),_T("FRWH01"),_T("FRHY04"),
		_T("FRHY05"),_T("VALA06"),_T(""),_T(""),_T(""),
		_T(""),_T(""),_T(""),_T(""),_T(""),
		//납땜등(104~)
		_T("SFO11L"),_T("SFX11L"),_T("SFS11L"),_T("SFO10L"),_T("SFX10L"),
		_T("SFS10L"),_T("SFO20T"),_T("SFX20T"),_T("SFS20T")//입상입하(납땜,용접,턱걸이)
	};
	sNo = JOINT[ptype];
	return sNo;
}


double CArx_Draw::gfGetSymDST1(int ptype,double scl)
{
	double DIST[115] = { 1.7, 1.7, 0.0, 1.5, 0.9, //하향곡관,상향곡관,흐름방향,
		//부품
		1.5, 1.5, 1.0, 0.75, 0.0,
		0.0, 0.0, 0.0, 2.0,  2.5,
		1.0, 2.25, 2.5, 0.0, 0.0,
		1.5, 0.0, 0.0, 1.0,  1.0,//붓싱
		1.25, 0.0, 0.0, 0.0, 0.0,
		//위생
		1.5, 1.5, 1.3, 1.3, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		//밸브
		1.25, 1.25, 1.25, 1.25, 1.25, 
		1.5,  1.25, 1.25, 2.25, 0.0,
		1.25, 1.25, 1.25, 1.25, 0.0,
		0.0,  1.5,  5.0,  2.0,  1.5,
		1.5,  2.0,  1.5,  1.25, 1.25,
		0.0, 0.0, 0.0, 0.0, 0.0,
		//어셈블리
		11.0, 13.0, 11.0, 11.0, 11.0,
		11.0, 11.0, 11.0,  9.0,  9.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		//소방
		1.75, 1.0, 0.0, 2.0, 2.0,
		0.7,  0.7, 0.7, 0.0, 2.0,
		2.0,  0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		//입상입하(납땜,용접,턱걸이)
		1.3, 1.3, 1.3, 1.3, 1.3,
		1.3, 1.5, 1.5, 2.35 
	};   //  
	return (scl*DIST[ptype]);
}

double CArx_Draw::gfGetSymDST2(int ptype,double scl)
{
	double DIST[115] = { 1.7, 1.7, 0.0, 1.5, 0.9,   //
		//부품
		1.5, 1.5, 1.0, 0.75, 0.0,
		0.0, 0.0, 0.0, 2.0,  2.5,
		1.0, 2.25, 2.5, 0.0, 0.0,
		1.5, 0.0, 0.0, 1.0,  0.0,//붓싱
		1.25, 0.0, 0.0, 0.0, 0.0,
		//위생
		1.5, 1.5, 1.3, 1.3, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		//밸브
		1.25, 1.25, 1.25, 1.25, 1.25, 
		1.5,  1.25, 1.25, 2.25, 0.0,
		1.25, 1.25, 1.25, 1.25, 0.0,
		0.0,  1.5,  5.0,  2.0,  1.5,
		1.5,  2.0,  1.5,  1.25, 1.25,
		0.0, 0.0, 0.0, 0.0, 0.0,
		//어셈블리
		11.0, 13.0, 11.0, 11.0, 11.0,
		11.0, 11.0, 11.0,  9.0,  9.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		//소방
		1.75, 1.0, 0.0, 2.0, 2.0,
		0.7,  0.7, 0.7, 0.0, 2.0,
		2.0,  0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		//입상입하(납땜,용접,턱걸이)
		1.3, 1.3, 1.3, 1.3, 1.3,
		1.3, 1.5, 1.5, 2.35 
	};
	return (scl*DIST[ptype]);
}

//Mode=BlockNUmber,prompt1=삽입점,prompt2=방향,yesno=회전,path=block path,scale=scale
//trim=공백이면 절단,bend=끝쪽에 block 삽입,reverse=mirror
void CArx_Draw::gf_OneTwoYNIns(int Mode,CString prompt1,CString prompt2,CString YesNo,
	CString path,double scale,CString trim,CString bend,CString reverse)
{

	CAcModuleResourceOverride thisResource; // 20100911 원진호 이거 안해주면 일부 메세지가 엉뚱한 메세지로 나타나는 문제 발생한다  아마 여기서 리습으로 코일을 작도하기때문에 리습 리소스에 뭔가 문제가 있는것같다
	CString strResource;
	CArx_Math m_Math;
	CArx_User m_User;
	CArx_Xdata m_Xdata, m_Xdata1;
	int i,lstv_cnt,lstv1_cnt,lstv2_cnt;
	double angle1=(double)0,o_angle=(double)0,dst1=(double)0,dst2=(double)0;
	ads_name pcs1,pcs2;
	ads_point pcspt1,pcspt2;
	TCHAR option_str[132];
	CString enName1,enName2,m_bname,symName;
	AcGePoint3d spt,ept,pt1, pt1e, pt2,smpt,empt,pint,ppt1;
	AcGePoint3dArray lstv,lstv1,lstv2;
	struct resbuf *pRb=NULL;
	AcDbObjectId objId=NULL;
	BOOL entity_flg,tog_flg;
	AcDbDatabase acdb;
	AcCmColor	 acco;
	struct resbuf *osnap;

	osnap=NULL;
	osnap=ads_newrb(RTSHORT);
	ads_getvar(_T("OSMODE"), osnap); //Old_OSNAP
	acco.setColorIndex(256);
	symName = gfGetSymName(Mode);   //Symbol name
	dst1= gfGetSymDST1(Mode,scale); //거리-1
	dst2= gfGetSymDST2(Mode,scale); //거리-2

	//심볼 디렉토리 분류
	if((Mode>= 85 && Mode<105)||(Mode==21)){  //소방
		m_bname.Format(_T("%s\\fir\\%s.dwg"), path,symName);
	} else if((69<Mode) && (Mode<85)){			//어셈블리
		m_bname.Format(_T("%s\\air\\%s.dwg"), path,symName);
	} else {
		m_bname.Format(_T("%s\\plb\\%s.dwg"), path,symName);
	}

	bool bFirst=true;
	AcGePoint3d ptFirst1, ptFirst2;



	while(T){
		entity_flg=FALSE;  //객체 선택 여부
		if(Mode!=4) setSysVar((LPCTSTR)_T("OSMODE"),512); //입관(Mode=4)이 아닌 경우만
		//////////////객체 선택방식을 포인트에서 선택으로 전환//////////////////
		/*		resbuf* TmpBuf = NULL;
		ads_name entTemp;

		int rc=0;
		if(ads_entsel(prompt1,entTemp,pcspt1) == RTNORM)
		{
		TmpBuf = ads_entget(entTemp);
		if(TmpBuf != NULL)
		{	
		}
		else
		break;
		*/     
		if(ads_getpoint(NULL,prompt1,pcspt1) == RTNORM)
		{   //삽입위치 지정
			m_User.ads_arxpt(pt1,pcspt1);

			//일단 다른것에 영향을 미치지 않기 위해서 포인트를 옮기는 루틴을 넣자
			/*			if(objId=m_Xdata.entitySSelect(pcspt1,pcspt1,"LINE"))
			{
			AcGePoint3d sp, ep, selPt;
			ads_real ang;
			m_User.arx_getLine(objId, sp, ep);
			m_User.arx_adspt(pcspt1, selPt);
			///AfxMessageBox("dddd");
			if(selPt.isEqualTo(sp))
			{

			ang = m_Math.arx_angleA(sp,ep); //선분 각도

			selPt = m_Math.Polar(sp, sp.distanceTo(ep)/10.0,m_Math.RTD(ang));
			m_User.ads_arxpt(selPt, pcspt1);
			CString tmp;
			tmp.Format("s ang =%f", ang);
			///AfxMessageBox(tmp);
			}
			if(selPt.isEqualTo(ep))
			{

			ang = m_Math.arx_angleA(ep,sp); //선분 각도
			selPt = m_Math.Polar(ep, ep.distanceTo(ep)/10.0,m_Math.RTD(ang));
			m_User.ads_arxpt(selPt, pcspt1);
			CString tmp;
			tmp.Format("e ang =%f", ang);
			///AfxMessageBox(tmp);
			}
			else;

			}*/

			/////////////////////////////////////////////////////////////////////
			if((objId=m_Xdata.entitySSelect(pcspt1,pcspt1,_T("LINE")))
				||(objId=m_Xdata.entitySSelect(pcspt1,pcspt1,_T("POLYLINE")))||
				(objId=m_Xdata.entitySSelect(pcspt1,pcspt1,_T("LWPOLYLINE")))
				)
			{
				acdbGetAdsName(pcs1,objId);
			}else{
				entity_flg=TRUE;  //객체가 없는 경우
			}
		} else
			break;
		if(!prompt2.IsEmpty()){
			if(ads_entsel((LPTSTR)prompt2.operator LPCTSTR(),pcs2,pcspt2) == RTNORM){
				if(isOnLockedObj(pcs2)) return;
				m_User.ads_arxpt(pt2,pcspt2);
			}else
				break;
		}
		if(Mode!=4) setSysVar((LPCTSTR)_T("OSMODE"),0); //입관(Mode=4)이 아닌 경우만
		pRb=m_Xdata.gfenameToXdataL(pcs1);

		//블럭 삽입 
		if(entity_flg){	 //선택객체가 없을 경우 삽입
			ads_DrawBlock(pt1,m_bname,scale,scale,0);
			continue;	
		}else{
			ObjectConfigSet(pcs1,0);  //객체 환경
		}
		m_Xdata.lf_SReturnList(pcs1,0,enName1);
		lstv_cnt = 0;
		while(!lstv.isEmpty())
			lstv.removeLast();

		if(ads_name_nil(pcs1)){
			strResource.LoadStringW(IDS_STRING117);		// "\n선택이 잘못되었습니다."
			ads_printf(strResource);
			//			ads_printf(_T("\nvoid CArx_Draw::gf_OneTwoYNIns1 선택이 잘못되었습니다."));
			if(pRb != NULL)
				ads_relrb(pRb);
			return;
		}
		if(enName1.CompareNoCase(_T("LINE"))==0){
			spt=m_Xdata.lf_3DReturnList(pcs1,10);
			ept=m_Xdata.lf_3DReturnList(pcs1,11);
		}else if(enName1.CompareNoCase(_T("POLYLINE"))==0){
			m_Xdata.gl_PolyAnaly(pcs1,pt1,spt,ept,lstv,lstv_cnt);
		}else if(enName1.CompareNoCase(_T("LWPOLYLINE"))==0){
			m_Xdata.gl_LWPolyAnaly(pcs1,pt1,spt,ept,lstv,lstv_cnt);
		}else{
			strResource.LoadStringW(IDS_STRING118);		// "\n선택이 잘못되었습니다."
			ads_printf(strResource);
			if(pRb != NULL)
				ads_relrb(pRb);
			return;
		}

		pt1=m_Math.gl_Perpt(spt,ept,pt1);


		//-------------------------------------------------------------------------------------------------------------------------------------------------
		// 첫번째 지정한 점(ptFirst1)에서 선택된 선분과 직교하는 선과 다음에 지정한 점에서의 선택선과의 교점(ptInters)과의 거리가 일정거리(1200*scale)보다 작고
		// ptInters에서 지정한 점(pt1)까지의 거리가 2.5*scale보다 작은경우 pt1을 ptInters로 수정한다.
		// 2015-12-04, gslee
		//-------------------------------------------------------------------------------------------------------------------------------------------------
		if(bFirst){
			ptFirst1=pt1;
			AcGeVector3d vFirst=(ept-spt).normal();
			AcGeVector3d vFirstOrtho;
			AcGeContext::gOrthoVector(vFirst, vFirstOrtho);
			ptFirst2=ptFirst1+vFirstOrtho*2.0;
			bFirst=false;
		}else{
			ads_point ptInters;
			int nRC=ads_inters(asDblArray(spt), asDblArray(ept), asDblArray(ptFirst1), asDblArray(ptFirst2), false, ptInters);
			if(nRC=RTNORM){
				AcGePoint3d ptIns=asPnt3d(ptInters);
				if(ptIns.distanceTo(pt1)<2.5*scale && ptFirst1.distanceTo(ptIns)<1200*scale) pt1=ptIns;
			}//end-if		
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------

		o_angle=m_Math.arx_angleA(spt,ept); //선분 각도
		angle1=o_angle;
		if(!prompt2.IsEmpty()){
			pt2=m_Math.gl_Perpt(spt,ept,pt2);
			angle1=m_Math.arx_angleA(pt1,pt2);
		}

		if(bend.CompareNoCase(_T("END"))==0){  ///끝쪽에 block insert
			if(pt1.distanceTo(spt)<pt1.distanceTo(ept)){
				pt1=spt;
				pt1e=ept;
				angle1=m_Math.arx_angleA(ept,pt1);
			}else{
				pt1=ept;
				pt1e=spt;
				angle1=m_Math.arx_angleA(spt,pt1);
			}
		}
		/// //말단부 심볼
		if((Mode!=0)&&(Mode!=1)&&(Mode!=3)&&(Mode!=9)&&(Mode!=10)&&(Mode!=21)&&(Mode!=22)&&(Mode!=32)
			&& (Mode<104 && Mode >115))
		{		//추가(납땜, 용접, 턱걸이)


			if((135.0 <= angle1)&&(angle1 < 315.0))		//각도 보정
				angle1 = angle1+180.0;
		}


		//심볼 삽입
		if(Mode==20 || Mode==27) //증기Trap, P Trap(문자가 있는 원)
			ads_DrawBlock(pt1,m_bname,scale,scale,0.0);
		else
			ads_DrawBlock(pt1,m_bname,scale,scale,angle1);
		ads_entlast(pcs2);
		//회전
		if(!YesNo.IsEmpty()){
			strResource.LoadStringW(IDS_STRING119);		// "Yes No"
			ads_initget(0,strResource);

			strResource.LoadStringW(IDS_STRING120);		// "\n회전?(Yes/No)<N>: "
			switch (ads_getkword(strResource, option_str)) {
			case RTKWORD:
				ads_getinput(option_str);
			default:
				break;
			}

			strResource.LoadStringW(IDS_STRING121);		// "Yes"
			if(_tcscmp(option_str,strResource)== 0){
				m_User.gfenameDel(pcs2);
				angle1 += 180;
				ads_DrawBlock(pt1,m_bname,scale,scale,angle1);
			}
			ads_entlast(pcs2);
		}
		//반전
		if(!reverse.IsEmpty()){
			int iX=1,iY=1;
			strResource.LoadStringW(IDS_STRING122);		// "Yes No"
			ads_initget(0,strResource);

			strResource.LoadStringW(IDS_STRING123);		// "\n반전?(Yes/No)<N>: "
			switch (ads_getkword(strResource, option_str)) {
			case RTKWORD:
				ads_getinput(option_str);
			default:
				break;
			}

			strResource.LoadStringW(IDS_STRING124);		// "Yes"
			if(_tcscmp(option_str,strResource)== 0){
				if(!YesNo.IsEmpty())
					iX = -1;
				else{ //반전인 경우
					if(Mode==24){	//붓싱(한쪽만 절단)
						iX = -1;
						dst2=dst1;
						dst1=0.0;
					}
					if(Mode==32){   //R.D-2
						iX = -1;
						angle1 += 180;
					}else{
						iY = -1;
						if((Mode==5)||(Mode==15)||(Mode==16)||(Mode==64)) //레듀셔,볼조인트,신축이음쇠,나비체크
							iX = -1;
					}
				}
				m_User.gfenameDel(pcs2);
				ads_DrawBlock(pt1,m_bname,scale*iX,scale*iY,angle1);
			}
			ads_entlast(pcs2);
		}
		if(360.0 < angle1)
			angle1 = angle1-360.0;



		//Trim 위치 지정
		double dif=fabs(o_angle-angle1);

		//CString str;
		//str.Format(_T("o_angle=%g, angle1=%g"), o_angle, angle1);
		//AfxMessageBox(str);

		if(dif>1){   //두 객체 선택
			smpt = m_Math.Polar(pt1,dst1,angle1);
			empt = m_Math.Polar(pt1,dst2,angle1-180);

		}else{  //하나의 객체
			smpt = m_Math.Polar(pt1,dst2,angle1-180);
			empt = m_Math.Polar(pt1,dst1,angle1);
		}







		AcGePoint3d sempt=smpt+(empt-smpt).normal()*smpt.distanceTo(empt)*0.5;
		bool bLineDrawed=false;

		//Trim 작업
		//dst1=dst2=0.0;
		ads_point bpt, mpt;
		AcGeTol tol = AcGeContext::gTol;
		//CString strTmp;
		//strTmp.Format(_T("equalVector=%g"), tol.equalVector());
		//AfxMessageBox(strTmp);
		tol.setEqualVector(0.01);

		if(trim.IsEmpty()||(dst1>0.0)||(dst2>0.0)){
			ObjectConfigSet(pcs1,0);  //객체 환경
			m_User.gfenameDel(pcs1);

			if(enName1.CompareNoCase(_T("LINE"))==0){

				if(m_Math.fb_hand(spt,ept,smpt)==0 && (dst1>0.0)||(dst2>0.0)){


					bpt[0]= pt1.x;
					bpt[1]= pt1.y;
					bpt[2]= pt1.z;
					mpt[0]= smpt.x;
					mpt[1]= smpt.y;
					mpt[2]= smpt.z;

					double ang = m_Math.arx_angle(pt1, smpt);
					CString tmpStr;
					tmpStr.Format(_T("@%f<%f"), dst1, m_Math.RTD(ang));



					m_bname.MakeUpper();
					if(m_bname.Find(_T("KJRD03.DWG"),0) > 0)
					{
						ads_command(RTSTR, _T("_Move"), RTENAME, pcs2, RTSTR, _T(""), RTPOINT, bpt, RTSTR, tmpStr , RTNONE);
					}

					if(_tcscmp(option_str,_T("Yes"))== 0)
					{
						if(m_Math.arx_eqPoint(pt1, spt))
							DrawLine(smpt,ept);
						else
							DrawLine(smpt,spt);
					}
					else{


						if(bend.CompareNoCase(_T("END"))==0) {
							if(pt1e.distanceTo(smpt) < pt1e.distanceTo(empt)) DrawLine(smpt, pt1e);
							else DrawLine(empt,pt1e);
						}else{

							if(spt.distanceTo(smpt)< spt.distanceTo(empt)) {
								if(sempt.distanceTo(spt)>smpt.distanceTo(empt)*0.5) DrawLine(spt, smpt);
								//if((spt-sempt).normal().isEqualTo((smpt-sempt).normal() ,tol)) DrawLine(spt, smpt);
								//if(smpt.distanceTo(empt)*0.8<spt.distanceTo(smpt)) DrawLine(spt, smpt);
							}
							else {
								if(sempt.distanceTo(spt)>smpt.distanceTo(empt)*0.5) DrawLine(spt, empt);
								if((spt-sempt).normal().isEqualTo((empt-sempt).normal() ,tol)) DrawLine(spt, empt);
								//if(smpt.distanceTo(empt)*0.8<s pt.distanceTo(empt)) DrawLine(spt, empt);
							}

						}





					}
					ads_entlast(pcs1);
					if(pRb == NULL){ //확장 데이터 입력
						CString enn;
						enn=m_Xdata.gfreturn_handle(pcs1);
						m_Xdata.sym_XdataAdd((LPTSTR)enn.operator LPCTSTR(),pcs1);
						pRb = m_Xdata.gfenameToXdataL(pcs1);
					}else{
						m_Xdata.sym_XdataAdd(pRb);
					}
					bLineDrawed=true;
				}

				if(m_Math.fb_hand(spt,ept,empt)==0 && (dst1>0.0)||(dst2>0.0)){

					bpt[0]= pt1.x;
					bpt[1]= pt1.y;
					bpt[2]= pt1.z;
					mpt[0]= empt.x;
					mpt[1]= empt.y;
					mpt[2]= empt.z;
					double ang = m_Math.arx_angle(pt1, ept);
					CString tmpStr;
					tmpStr.Format(_T("@%f<%f"), dst1, m_Math.RTD(ang));
					m_bname.MakeUpper();
					if(m_bname.Find(_T("KJRD03.DWG"),0) > 0)
					{
						ads_command(RTSTR, _T("_Move"), RTENAME, pcs2, RTSTR, _T(""), RTPOINT, bpt, RTSTR, tmpStr , RTNONE);
					}

					if(bend.CompareNoCase(_T("END"))!=0){



						if(ept.distanceTo(smpt)< ept.distanceTo(empt)) {

							if(sempt.distanceTo(ept)>smpt.distanceTo(empt)*0.5) DrawLine(ept, smpt);
							//if((ept-sempt).normal().isEqualTo((smpt-sempt).normal() ,tol)) DrawLine(ept, smpt);

							//if(smpt.distanceTo(empt)*0.8<ept.distanceTo(smpt)) DrawLine(ept, smpt);
						}
						else {
							if(sempt.distanceTo(ept)>smpt.distanceTo(empt)*0.5) DrawLine(ept, empt);
							//if((ept-sempt).normal().isEqualTo((empt-sempt).normal() ,tol)) DrawLine(ept, empt);
							//if(smpt.distanceTo(empt)*0.8<ept.distanceTo(empt)) DrawLine(ept, empt);
						}



						ads_entlast(pcs1);
						if(pRb == NULL){ //확장 데이터 입력
							CString enn;
							enn=m_Xdata.gfreturn_handle(pcs1);
							m_Xdata.sym_XdataAdd((LPTSTR)enn.operator LPCTSTR(),pcs1);
							pRb = m_Xdata.gfenameToXdataL(pcs1);
						}else{
							m_Xdata.sym_XdataAdd(pRb);
						}
						bLineDrawed=true;
					}
				}


				if(bLineDrawed==false ){

					// && bend.CompareNoCase(_T("END"))!=0

					DrawLine(spt,ept);

					ads_entlast(pcs1);
					if(pRb == NULL){ //확장 데이터 입력
						CString enn;
						enn=m_Xdata.gfreturn_handle(pcs1);
						m_Xdata.sym_XdataAdd((LPTSTR)enn.operator LPCTSTR(),pcs1);
						pRb = m_Xdata.gfenameToXdataL(pcs1);
					}else{
						m_Xdata.sym_XdataAdd(pRb);
					}
				}


				if(pRb != NULL)
					m_Xdata.sym_XdataAdd(pRb,pcs2);



			}else{  ///POLYLINE,LWPOLYLINE
				tog_flg=FALSE;
				lstv1_cnt=0;
				lstv2_cnt=0;
				while(!lstv1.isEmpty())
					lstv1.removeLast();
				while(!lstv2.isEmpty())
					lstv2.removeLast();
				for(i=0;i<lstv_cnt;i++){
					ppt1=lstv.at(i);
					if(ppt1.isEqualTo(spt)){
						lstv1.append(ppt1);
						lstv1_cnt++;
						if(spt.distanceTo(smpt)<spt.distanceTo(empt)){
							if(m_Math.fb_hand(spt,ept,smpt)==0){
								lstv1.append(smpt);
								lstv1_cnt++;
							}
						}else{
							if(m_Math.fb_hand(spt,ept,empt)==0){
								lstv1.append(empt);
								lstv1_cnt++;
							}
						}
						tog_flg=TRUE;
						continue;
					}
					if(ppt1.isEqualTo(ept)){
						if(ept.distanceTo(smpt)<ept.distanceTo(empt)){
							if(m_Math.fb_hand(spt,ept,smpt)==0){
								lstv2.append(smpt);
								lstv2_cnt++;
							}
						}else{
							if(m_Math.fb_hand(spt,ept,empt)==0){
								lstv2.append(empt);
								lstv2_cnt++;
							}
						}
						lstv2.append(ppt1);
						lstv2_cnt++;
						continue;
					}
					if(!tog_flg){
						lstv1_cnt++;
						lstv1.append(ppt1);
					}else{
						lstv2_cnt++;
						lstv2.append(ppt1);
					}
				}
				if(lstv1_cnt > 1){
					DrawPLine(lstv1,0);
					if(pRb == NULL){
						CString enn;
						enn=m_Xdata.gfreturn_handle(pcs1);
						m_Xdata.sym_XdataAdd((LPTSTR)enn.operator LPCTSTR(),pcs1);
						pRb = m_Xdata.gfenameToXdataL(pcs1);
					}else{
						m_Xdata.sym_XdataAdd(pRb);
					}
				}
				if(lstv2_cnt > 1){
					DrawPLine(lstv2,0);
					if(pRb == NULL){
						CString enn;
						enn=m_Xdata.gfreturn_handle(pcs1);
						m_Xdata.sym_XdataAdd((LPTSTR)enn.operator LPCTSTR(),pcs1);
						pRb = m_Xdata.gfenameToXdataL(pcs1);
					}else{
						m_Xdata.sym_XdataAdd(pRb);
					}
				}
				if(pRb != NULL)
					m_Xdata.sym_XdataAdd(pRb,pcs2);
			}
		}else{
			if(pRb == NULL){
				CString enn;
				enn=m_Xdata.gfreturn_handle(pcs1);
				m_Xdata.sym_XdataAdd((LPTSTR)enn.operator LPCTSTR(),pcs1);
				pRb = m_Xdata.gfenameToXdataL(pcs1);
			}else{
				m_Xdata.sym_XdataAdd(pRb);
			}
			m_Xdata.sym_XdataAdd(pRb,pcs2);
		}//if-end
	}//while-end
	ads_ssfree(pcs1);
	ads_ssfree(pcs2);
	setSysVar((LPCTSTR)_T("OSMODE"),0);
	//OSNAP 복원
	ads_setvar(_T("OSMODE"), osnap);
	ads_relrb(osnap);

	if(pRb != NULL)
		ads_relrb(pRb);
}

void CArx_Draw::createGroup(CString gname,AcDbObjectId& groupId)
{
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL) return;

	AcDbGroup *pGroup = new AcDbGroup();
	AcDbDictionary *pGroupDict;
	pDb->getGroupDictionary(pGroupDict,AcDb::kForWrite);

	//AcDbObjectId groupId;
	pGroupDict->setAt((LPTSTR)(LPCTSTR)gname, pGroup, groupId);
	pGroupDict->close();
	pGroup->close();
	//	if(pGroup)  delete pGroup;
}

void CArx_Draw::appendGroup(CString gname)
{
	ads_name ename;
	ads_entlast(ename);
	//sdel_red = m_Xdata.gfreturn_handle(en1);//;그룹명
	appendGroup(gname,ename);
}
void CArx_Draw::appendGroup(CString gname,ads_name ename)
{
	AcDbObjectId entId,groupId=NULL;
	AcDbDictionary *pGroupDict;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL) return;
	pDb->getGroupDictionary(pGroupDict,AcDb::kForRead);

	pGroupDict->getAt((LPTSTR)(LPCTSTR)gname, groupId);
	pGroupDict->close();
	if(groupId==NULL)
		createGroup(gname,groupId);

	AcDbGroup *pGroup;
	acdbOpenObject(pGroup, groupId, AcDb::kForWrite);

	acdbGetObjectId(entId, ename);
	pGroup->append(entId);
	pGroup->close();
}

// Prompts the user to select objects to add to the group,

// then opens the group identified by "groupId" passed in as
// an argument, then adds the selected objects to the group.
//
/*
void CArx_Draw::makeGroup(AcDbObjectId groupId)
{
ads_name sset;
int err = ads_ssget(NULL, NULL, NULL, NULL, sset);
if (err != RTNORM) {
return;
}

AcDbGroup *pGroup;
acdbOpenObject(pGroup, groupId, AcDb::kForWrite);

// Traverse the selection set exchanging each ads_name
// for an objectId, then adding the object to the group.

//
long i, length;
ads_name ename;
AcDbObjectId entId;
ads_sslength(sset, &length);
for (i = 0; i < length; i++) {
ads_ssname(sset, i, ename);
acdbGetObjectId(entId, ename);
pGroup->append(entId);
}
pGroup->close();
ads_ssfree(sset);
}
*/
// Accepts an objectId of an AcDbGroup object, opens it,
// then iterates over the group removing all entities that
// are not AcDbLines and change all remaining entities in

// the group to color red.
//
void CArx_Draw::removeGroup(CString key)
{
	AcDbObjectId groupId;
	AcDbDictionary *pGroupDict;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL) return;
	pDb->getGroupDictionary(pGroupDict,AcDb::kForRead);
	pGroupDict->getAt((LPTSTR)(LPCTSTR)key, groupId);
	pGroupDict->close();
	AcDbGroup *pGroup;
	acdbOpenObject(pGroup, groupId, AcDb::kForWrite);
	AcDbGroupIterator *pIter = pGroup->newIterator();
	AcDbObject *pObj;
	for (; !pIter->done(); pIter->next()) {
		pIter->getObject(pObj, AcDb::kForWrite);
		//if (!pObj->isKindOf(AcDbLine::desc())) {
		pObj->erase();
		pObj->close();
		pGroup->remove(pIter->objectId());
	}
	//    delete pIter;
	pGroup->setColorIndex(1);
	pGroup->close();
	//	if(pIter) delete pIter;
}


void CArx_Draw::DrawGrLine(const AcGePoint3d& pt1, const AcGePoint3d& pt2)
{
	ads_point p1,p2;
	p1[X]=pt1[X]; p1[Y]=pt1[Y]; p1[Z]=pt1[Z];
	p2[X]=pt2[X]; p2[Y]=pt2[Y]; p2[Z]=pt2[Z];
	DrawGrLine(p1,p2);
}

void CArx_Draw::DrawGrLine(const ads_point& p1, const ads_point& p2)
{
	int es=0;
	es=acedGrDraw(p1,p2,1,0);
}

// 2. 점 표시
/*   PRESENTATION "X" DIRECTED POSITION  */
void CArx_Draw::pyosi(ads_point pt1)
{
	CArx_User u;

	// 원으로 점표시 원진호
	CArx_Draw d;AcGePoint3d _pt1;u.ads_arxpt(_pt1,pt1);//d.DrawCircle(_pt1,2);
	// 테스트 1217
	if(_pt1.x>ErrVal || _pt1.y>ErrVal || _pt1.z>ErrVal){
		//	ads_printf(_T("\n pyosi ERROR!! _pt1.x=%.1f y=%.1f z=%.1f "), _pt1.x, _pt1.y, _pt1.z);
	}
	struct resbuf  *ent=NULL, *sys1=NULL,  *sys2=NULL;
	ads_point      pt2, pt3;
	ads_real       ang[]={0.25, 0.75};
	ads_real       yr1=0.0, vs=0.0 ,ss=0.0;
	short          pb=0, dur=0;

	sys1=ads_newrb(RTREAL);       /* sys1 must the allocated memory */
	sys2=ads_newrb(RT3DPOINT);

	ads_getvar(_T("VIEWSIZE"), sys1);     /* real */
	ads_getvar(_T("SCREENSIZE"), sys2);   /* point (real real) */

	vs=sys1->resval.rreal;            /* view size */
	ss=sys2->resval.rpoint[Y];        /* screensize */
	ads_relrb(sys1);
	ads_relrb(sys2);

	yr1=vs/ss * 6;                    /* real size of pickbox in screen */

	// X 표시 
	{
		for(dur=0; dur<2; dur++){
			ads_polar(pt1, ang[dur] * PI, yr1, pt2);
			ads_polar(pt1, (ang[dur] + 1.0) * PI, yr1, pt3);
			ads_grdraw(pt2, pt3, 1, 0);
		}/* end-for */
	}
}
// 1. 점 표시 적색X로 표시
void CArx_Draw::pyosi(AcGePoint3d pt1)
{
	ads_point pt;
	pt[0]=pt1.x;
	pt[1]=pt1.y;
	pt[2]=pt1.z;
	pyosi(pt);
}
void CArx_Draw::setSysVar()
{

	setSysVar((LPCTSTR)_T("CECOLOR"),	ccolor); //Color
	setSysVar((LPCTSTR)_T("CLAYER"),		clayer);  //Layer
	setSysVar((LPCTSTR)_T("TEXTSTYLE"),	ctextstyle); //Textstyle
	setSysVar((LPCTSTR)_T("CELTYPE"),	cltype); //Linetype
	setSysVar((LPCTSTR)_T("OSMODE"),		cosnap);  //OSNAP
	setSysVar((LPCTSTR)_T("ORTHOMODE"),	orthomode);	// 직교모드
	//	ads_printf(_T("\n %s() 완료. \n"),_T(__FUNCTION__));
}

void CArx_Draw::ShowSysVar()
{
	//	ads_printf(_T("\n %s() 시작. \n"),_T(__FUNCTION__));
	CString sColor,sLayer,sTextStyle,sLineType;
	int iOsnap=0,iOrthoMode=0;
	int UserI1=0,UserI5=0;
	double UserR1=0,UserR2=0,UserR3=0,UserR4=0;

	// 시스템변수를 확인만 하는것이므로 멤버변수에 저장하지 말아야한다
	getSysVar((LPCTSTR)_T("CECOLOR"),	sColor);		// Color
	getSysVar((LPCTSTR)_T("CLAYER"),		sLayer);		// Layer
	getSysVar((LPCTSTR)_T("TEXTSTYLE"),	sTextStyle);// Textstyle
	getSysVar((LPCTSTR)_T("CELTYPE"),	sLineType);	// Linetype
	getSysVar((LPCTSTR)_T("OSMODE"),		iOsnap);		// OSNAP
	getSysVar((LPCTSTR)_T("ORTHOMODE"),	iOrthoMode);// 직교

	getSysVar((LPCTSTR)_T("USERI1"),	UserI1);
	getSysVar((LPCTSTR)_T("USERI5"),	UserI5);
	getSysVar((LPCTSTR)_T("USERR1"),	UserR1);
	getSysVar((LPCTSTR)_T("USERR2"),	UserR2);
	getSysVar((LPCTSTR)_T("USERR3"),	UserR3);
	getSysVar((LPCTSTR)_T("USERR4"),	UserR4);

	ads_printf(_T("\nCECOLOR=%s  CLAYER=%s  TEXTSTYLE=%s  CELTYPE=%s  OSMODE=%d  ORTHOMODE=%d"),sColor,sLayer,sTextStyle,sLineType,iOsnap,iOrthoMode);
	ads_printf(_T("\nUSERI1(Scale)=%d  USERI5(Drain Soil color)=%d  USERR1(bupum_size)=%.2f"),UserI1,UserI5,UserR1);
	ads_printf(_T("\nUSERR2(TextSize)=%.2f  USERR3(LTscale)=%.2f  USERR4(HiddenSize)=%.2f\n"),UserR2,UserR3,UserR4);
	//	ads_printf(_T("\n %s() 완료. \n"),_T(__FUNCTION__));
}

void CArx_Draw::ReadEnv()
{
	//	ads_printf(_T("\n %s() 시작. \n"),_T(__FUNCTION__));
	//	struct resbuf *lst=NULL;
	//추가
	getSysVar((LPCTSTR)_T("CECOLOR"), ccolor); //Color
	getSysVar((LPCTSTR)_T("CLAYER"), clayer);  //Layer
	getSysVar((LPCTSTR)_T("TEXTSTYLE"), ctextstyle); //Textstyle
	getSysVar((LPCTSTR)_T("CELTYPE"), cltype); //Linetype
	getSysVar((LPCTSTR)_T("OSMODE"), cosnap);  //OSNAP
	getSysVar((LPCTSTR)_T("ORTHOMODE"),orthomode);
	//	ads_printf(_T("\n %s() 완료. \n"),_T(__FUNCTION__));
}

void CArx_Draw::WriteEnv()
{
	//	ads_printf(_T("\n %s() 시작. \n"),_T(__FUNCTION__));
	setSysVar((LPCTSTR)_T("CECOLOR"), ccolor);
	setSysVar((LPCTSTR)_T("CLAYER"), clayer);
	setSysVar((LPCTSTR)_T("TEXTSTYLE"), ctextstyle);
	setSysVar((LPCTSTR)_T("CELTYPE"), cltype);
	setSysVar((LPCTSTR)_T("OSMODE"), cosnap);
	setSysVar((LPCTSTR)_T("ORTHOMODE"),orthomode);

	/*
	ads_command(RTSTR, ".UCS",
	RTSTR, "R", 
	RTSTR, "OLDUCS",
	0);
	ads_command(RTSTR, ".UCS",
	RTSTR, "D", 
	RTSTR, "OLDUCS",
	0);
	*/
	//	ads_printf(_T("\n CECOLOR=%s CLAYER=%s TEXTSTYLE=%s CELTYPE=%s OSMODE=%s ORTHOMODE=%s \n"),ccolor,clayer,ctextstyle,cltype,cosnap,orthomode);
	//	ads_printf(_T("\n %s() 완료. \n"),_T(__FUNCTION__));
}

int CArx_Draw::lf_IReturnList(ads_name ename, int type)
{
	struct resbuf* pRb=NULL,*pHead=NULL;
	int ret;
	CArx_Xdata m_Xdata;

	if(ads_name_nil(ename)) return -1;
	if(type < 1000) pRb=ads_entget(ename);
	else
		m_Xdata.lf_Xdata(ename,pRb);
	pHead=pRb;
	while ((pRb != NULL) && (pRb->restype != type)){
		pRb = pRb->rbnext;
	}
	if(pRb != NULL) ret = pRb->resval.rint;
	else ret=-1;

	if(pHead) ads_relrb(pHead);	

	return ret;
}


resbuf* LT_assoc(resbuf* ebuf,int restype)
{
	while((ebuf != NULL) && (ebuf->restype != restype))
		ebuf = ebuf->rbnext;
	return ebuf;
}


//LT 라인타입 취득
CString CArx_Draw::GetEntityLineType(ads_name  ent)
{
	CString rcStr(_T("")); //나중에 리턴할 문자열

	// -------------------------------------------
	// 레이어의 선 유형
	AcDbObjectId idEnt=AcDbObjectId::kNull;
	AcDbObjectId idLayer=AcDbObjectId::kNull;
	AcDbObjectId linetypeObjectId=AcDbObjectId::kNull;


	Acad::ErrorStatus es = acdbGetObjectId(idEnt, ent);
	if (es != Acad::eOk) return rcStr;

	AcDbEntity *pEnt = NULL;
	es=acdbOpenAcDbEntity(pEnt, idEnt, AcDb::kForRead);
	if(es==Acad::eOk){
		rcStr=pEnt->linetype();
		idLayer=pEnt->layerId();
		m_linetypeScale4Ent=pEnt->linetypeScale();
		pEnt->close();
	}else return rcStr;

	if(rcStr==_T("")){
		// 레이어의 선유형 
		AcDbLayerTableRecordPointer layerTableRec(idLayer, AcDb::kForRead);    
		// if ok    
		if (layerTableRec.openStatus() == Acad::eOk)    {			
			linetypeObjectId=layerTableRec->linetypeObjectId();
			layerTableRec->close();	
		}

		AcDbLinetypeTableRecordPointer ltypeTableRec(linetypeObjectId, AcDb::kForRead);
		if(ltypeTableRec.openStatus() == Acad::eOk)    {
			ACHAR* pLTypeName;
			ltypeTableRec->getName(pLTypeName);
			//AfxMessageBox(pLTypeName);
			rcStr=pLTypeName;
			if(pLTypeName!=NULL) delete[] pLTypeName;
			ltypeTableRec->close();	
		}
	}


	return rcStr;
}
//기존 객체 환경(ent:객체명, Mode:오배수관(3,4) 구별을 위한 변수)
void CArx_Draw::ObjectConfigSet(ads_name ent, int Mode)
{

	//	ads_printf(_T("\n 1787 %s(%d)시작. \n"),_T(__FUNCTION__),Mode);
	CArx_Xdata m_Xdata;
	CString lay, ltyp;
	int clr=-1;

	m_Xdata.lf_SReturnList(ent, 8, lay);		//Layer

	//ads_printf(_T("\n mode=%d,  1788 lay=%s \n"),Mode, lay);
	if(lay!=_T("")){
		setCurLayer(lay);
		ads_command(RTSTR, _T(".LAYER"),RTSTR,_T("S"),RTSTR,(LPCTSTR)lay,RTSTR,_T(""), 0); 
		//	ads_printf(_T("\n ObjectConfigSet setCurLayer \n"));
	}//end-if
	if((Mode==3||Mode==4)&&(ds_clr>0)){  //오배수 배관인 경우
		ads_printf(L"오배수색상: %d", ds_clr);
		if(ds_clr==0){
			ads_command(RTSTR, _T(".COLOR"), RTSTR, _T("BYBLOCK"), 0); //

			setCurColor(ds_clr);
		}else if(ds_clr==256){
			ads_command(RTSTR, _T(".COLOR"), RTSTR, _T("BYLAYER"), 0); 
			setCurColor(ds_clr);		          	
		}else{
			ads_command(RTSTR, _T(".COLOR"), RTSHORT, ds_clr, 0); 
			setCurColor(ds_clr);		          	
		}
	}else{
		clr=m_Xdata.arx_getColor(ent);

		if(clr==0){
			ads_command(RTSTR, _T(".COLOR"), RTSTR, _T("BYBLOCK"), 0); 

		}else if(clr>0){
			setCurColor(clr);		       		//기존 색상
			if(clr==256){
				ads_command(RTSTR, _T(".COLOR"), RTSTR, _T("BYLAYER"), 0); 
			}else{
				ads_command(RTSTR, _T(".COLOR"), RTSHORT, clr, 0); 
			}
		}else{
			setSysVar((LPCTSTR)_T("CECOLOR"), _T("BYLAYER"));
		}
	}


	//Linetype
	//	m_Xdata.lf_SReturnList(ent, 6, ltyp);  //Linetype
	ltyp=GetEntityLineType(ent);
	if(ltyp!=_T("")){
		setCurLinetype(ltyp);	//기존 LT
		ads_command(RTSTR, _T(".LINETYPE"), RTSTR , _T("S"), RTSTR, (LPCTSTR)ltyp, RTSTR, _T(""), 0); 
	}else{
		ads_command(RTSTR, _T(".LINETYPE"), RTSTR , _T("S"), RTSTR, (LPCTSTR)ltyp, RTSTR, _T(""), 0); 
	}
	//end-if
}


// 현재 색상을 변경한다. 
void CArx_Draw::setCecolor(AcCmColor m_cecolor)
{

	CString strResource;
	Acad::ErrorStatus rc;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL) return;
	rc=pDb->setCecolor(m_cecolor);
	if(rc!=Acad::eOk){
		strResource.LoadStringW(IDS_STRING125);		// "\n현재 색상을 변경할 수 없습니다."
		ads_printf(strResource);
	}
}

// 3
// AcDbLine형 일때
//선분의 위치를 재설정한다.
void CArx_Draw::setLine(AcDbLine* pEnt, AcGePoint3d ps, AcGePoint3d pe)
{
	if (!pEnt) return;
	pEnt->setStartPoint(ps);
	pEnt->setEndPoint(pe);
}

// 2
// AcDbObjectId형일때
bool CArx_Draw::setLine(AcDbObjectId EntId, AcGePoint3d ps, AcGePoint3d pe)
{
	bool ret=true;
	AcDbObject* obj = openObject(EntId, true, false);
	if (obj->isKindOf(AcDbLine::desc())){
		AcDbLine* pEnt = AcDbLine::cast(obj);
		setLine(pEnt, ps, pe);
	}
	else ret=false;
	obj->close();

	return ret;
}

// 함수를 오버로딩하여 매개변수가 다를때 마다 적용할수 있게끔
// ads_name객체일땐 함수1을 쓰고
// AcDbObject형일땐 함수2를 쓰고
// AcDbLine형일땐 함수3을 쓴다
// 잘만들었네
// 1 ads_name객체를 전달받아
bool CArx_Draw::setLine(ads_name ename,		// 선 객체
	AcGePoint3d pt1,	// 시작점
	AcGePoint3d pt2)	// 끝점
{
	CArx_User m_User;

	AcDbObjectId obj1;//	AcGePoint3d ps,pe;
	bool ret;

	// ads_name을 AcDbObjectId 형으로 바꾼다
	acdbGetObjectId(obj1, ename);//	m_User.ads_arxpt(ps, pt1);//	m_User.ads_arxpt(pe, pt2);

	// ads_name타입의 선객체를 속성 변경이 가능한 AcDbLine타입으로 변경하고
	// pt1을 시작점 pt2를 끝점으로 설정한다
	ret = setLine(obj1, pt1, pt2);

	if(ret)
		return true;
	else
		return false;
}



//---- Entity를 OPEN 한다.
AcDbObject* CArx_Draw::openObject(AcDbObjectId entID, BOOL bWritable, BOOL openErased)
{
	AcDbObject*	pObj;
	AcDb::OpenMode openMode = (bWritable ? AcDb::kForWrite : AcDb::kForRead);

	if (acdbOpenObject(pObj, entID, openMode) != Acad::eOk)
		return NULL;
	return pObj;
}


AcDbEntity* CArx_Draw::openEntity(AcDbObjectId entID, BOOL bWritable, BOOL openErased)
{
	return ((AcDbEntity*)openObject(entID, bWritable, openErased));
}


//속성 복사
void CArx_Draw::CopyProperty(AcDbEntity *destEnt, AcDbEntity *srcEnt)
{
	destEnt->setColorIndex(srcEnt->colorIndex());
	destEnt->setLayer(srcEnt->layer());
	destEnt->setLinetype(srcEnt->linetype());
	destEnt->setLinetypeScale(srcEnt->linetypeScale());
}


//속성 복사
void CArx_Draw::CopyProperty(AcDbObjectId destId, AcDbObjectId srcId)
{
	AcDbEntity *destEnt=NULL;
	AcDbEntity *srcEnt=NULL;

	acdbOpenAcDbEntity(destEnt, destId, AcDb::kForWrite);
	acdbOpenAcDbEntity(srcEnt, srcId, AcDb::kForRead);
	CopyProperty(destEnt, srcEnt);
	destEnt->close();
	srcEnt->close();

}


void CArx_Draw::CheckPt(CString FUNCTION, AcGePoint3d pt)
{
	if(pt.x>ErrVal || pt.y>ErrVal || pt.z>ErrVal){
		ads_printf(_T("\n AcGePoint3d Error! [%s] pt.X=%.1f Y=%.1f Z=%.1f "),FUNCTION,pt.x,pt.y,pt.z);
	}
}
