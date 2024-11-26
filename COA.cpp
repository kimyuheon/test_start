#include "stdafx.h"
#include "COA.h"
#include "afxdialogex.h"

// CCOA 대화 상자입니다.
IMPLEMENT_DYNAMIC(CCOA, CAcUiDialog)
	CCOA::CCOA(CWnd* pParent)
	: CAcUiDialog(CCOA::IDD, pParent)
	, m_ScaleStr(_T("")){
}

CCOA::~CCOA(){
}

void CCOA::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDTSCALE, m_ScaleCtrl);
	DDX_Text(pDX, IDC_EDTSCALE, m_ScaleStr);
	DDX_Control(pDX, IDC_CBXLAYER, m_CbxLayerCtr);
	DDX_Control(pDX, IDC_BSELECT, m_BtnSelect);
	DDX_Control(pDX, IDC_COABIT, m_ctrPic);
}

BEGIN_MESSAGE_MAP(CCOA, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CCOA::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CBXLAYER, &CCOA::OnCbnSelchangeCbxlayer)
	ON_BN_CLICKED(IDC_BCOLOR, &CCOA::OnBnClickedBcolor)
	ON_BN_CLICKED(IDC_BSELECT, &CCOA::OnBnClickedBselect)
	ON_BN_CLICKED(IDCANCEL, &CCOA::OnBnClickedCancel)
	ON_EN_UPDATE(IDC_EDTSCALE, &CCOA::OnEnUpdateEdtscale)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(ID_COA_HELP, &CCOA::OnBnClickedCoaHelp)
END_MESSAGE_MAP()

// CCOA 메시지 처리기입니다.
BOOL CCOA::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	CString scaleStr = iniFile.readINI(_T("config.ini"), _T("COA"), _T("MAINSCALE"));
	if(scaleStr.CompareNoCase(_T("")) == 0)
		scaleStr = _T("1000");

	SetDlgItemText(IDC_EDTSCALE, scaleStr);	
	m_BtnSelect.AutoLoad();

	//도면층 불러오기
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);
	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		pLayerTblRcd->getName(name);
		m_CbxLayerCtr.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;

	//변수 및 리소스 초기화
	m_CbxLayerCtr.SetCurSel(0);
	CString curLayerStr;
	m_CbxLayerCtr.GetLBString(m_CbxLayerCtr.GetCurSel(), curLayerStr);
	LayerChangeGetColor(curLayerStr);
	OnCbnSelchangeCbxlayer();
	setLayer = _T("0");
	setColorIdx = 7;

	return TRUE;
}

//실행
void CCOA::OnBnClickedOk(){	
	double scaleValue;
	UpdateData(TRUE);
	scaleValue = _ttof(m_ScaleStr);
	UpdateData(FALSE);
	CAcUiDialog::OnOK();
	acutPrintf(_T("%g"), scaleValue);
	ads_name en1, en2;
	ads_point pt1, pt2;
	ads_real inputDist;
	int result;

	auto IDStoCString = [](int a_ID) -> CString {
		CString IDS;
		IDS.LoadStringW(a_ID);
		return IDS;
		};

	CString curLayerStr;
	m_CbxLayerCtr.GetLBString(m_CbxLayerCtr.GetCurSel(), curLayerStr);

	BeginEditorCommand();
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	while(true){
		if(RTNORM == (result = acedEntSel(IDStoCString(IDS_COA_FIRST_SEL), en1, pt1))) { // "\n가각을 그릴 첫번째 선 선택: "
			if(RTNORM == (result = acedEntSel(IDStoCString(IDS_COA_SECOND_SEL), en2, pt2))){ // "\n가각을 그릴 두번째 선 선택: "
				if(RTNORM == (result = acedGetReal(IDStoCString(IDS_INPUT_COA_VALUE), &inputDist))){ // "\n가각 길이 입력: "
					break;
				}
			}
		}
		if(result == RTCAN)
			return;
	}
	CompleteEditorCommand();

	AcGePoint3d pPoint1 = asPnt3d(pt1);
	AcGePoint3d pPoint2 = asPnt3d(pt2);
	AcGePoint3d middleP, crossP;
	AcDbEntity* line1Ent;
	AcDbEntity* line2Ent;
	AcDbObjectId line1Id, line2Id;
	acdbGetObjectId(line1Id, en1);
	acdbGetObjectId(line2Id, en2);
	if(acdbOpenAcDbEntity(line1Ent, line1Id, AcDb::kForRead) != Acad::eOk)
		return;
	if(!line1Ent->isKindOf(AcDbLine::desc()) && !line1Ent->isKindOf(AcDbPolyline::desc())){
		line1Ent->close();
		return;	
	}
	if(acdbOpenAcDbEntity(line2Ent, line2Id, AcDb::kForRead) != Acad::eOk)
		return;
	if(!line2Ent->isKindOf(AcDbLine::desc()) && !line2Ent->isKindOf(AcDbPolyline::desc())){
		line1Ent->close();
		line2Ent->close();
		return;
	}
	AcDbObjectId tmpLine1, tmpLine2;
	AcDbLine* newLine1 = new AcDbLine();
	AcDbLine* newLine2 = new AcDbLine();

	if(line1Ent->isKindOf(AcDbPolyline::desc()))
	{
		AcDbPolyline* pLine1 = AcDbPolyline::cast(line1Ent);
		double Midpoint_parm;
		AcGePoint3d OnLpoint1;
		pLine1->getClosestPointTo(pPoint1,OnLpoint1);
		pLine1->getParamAtPoint(OnLpoint1,Midpoint_parm);
		AcGePoint3d pL1_Spoint, pL1_Epoint;
		if(floor(Midpoint_parm) < 1.0)
		{
			pLine1->getPointAtParam(floor(Midpoint_parm),pL1_Spoint);
			pLine1->getPointAtParam(Midpoint_parm+0.000001,pL1_Epoint);
		}
		else
		{
			pLine1->getPointAtParam(ceil(Midpoint_parm),pL1_Spoint);
			pLine1->getPointAtParam(Midpoint_parm-0.000001,pL1_Epoint);
		}
		pLine1->close();
		
		newLine1->setStartPoint(pL1_Spoint);
		newLine1->setEndPoint(pL1_Epoint);
		AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
		acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
		newLine1->setDatabaseDefaults();
		bTR->appendAcDbEntity(newLine1);
		newLine1->close();
		bTR->close();
		tmpLine1 = newLine1->objectId();
	}
	else if(line1Ent->isKindOf(AcDbLine::desc()))
	{
		AcDbLine* Line1 = AcDbLine::cast(line1Ent);
		
		newLine1->setStartPoint(Line1->startPoint());
		newLine1->setEndPoint(Line1->endPoint());
		Line1->close();

		AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
		acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
		newLine1->setDatabaseDefaults();
		bTR->appendAcDbEntity(newLine1);
		newLine1->close();
		bTR->close();
		tmpLine1 = newLine1->objectId();
	}

	if(line2Ent->isKindOf(AcDbPolyline::desc()))
	{
		AcDbPolyline* pLine2 = AcDbPolyline::cast(line2Ent);
		double Midpoint_parm;
		AcGePoint3d OnLpoint2;
		pLine2->getClosestPointTo(pPoint2,OnLpoint2);
		pLine2->getParamAtPoint(OnLpoint2,Midpoint_parm);		
		AcGePoint3d pL2_Spoint, pL2_Epoint;
		if(floor(Midpoint_parm) < 1.0) // 선택한 포인트점이 객체의 end 지점인지 start 지점인지 확인
		{
			pLine2->getPointAtParam(floor(Midpoint_parm),pL2_Spoint);
			pLine2->getPointAtParam(Midpoint_parm+0.000001,pL2_Epoint);
		}
		else
		{
			pLine2->getPointAtParam(ceil(Midpoint_parm),pL2_Spoint);
			pLine2->getPointAtParam(Midpoint_parm-0.000001,pL2_Epoint);
		}
		pLine2->close();

		newLine2->setStartPoint(pL2_Spoint);
		newLine2->setEndPoint(pL2_Epoint);
		AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
		acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
		newLine2->setDatabaseDefaults();
		bTR->appendAcDbEntity(newLine2);
		newLine2->close();
		bTR->close();
		tmpLine2 = newLine2->objectId();
	}
	else if(line2Ent->isKindOf(AcDbLine::desc()))
	{
		AcDbLine* Line2 = AcDbLine::cast(line2Ent);
		
		newLine2->setStartPoint(Line2->startPoint());
		newLine2->setEndPoint(Line2->endPoint());
		Line2->close();
		
		AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
		acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
		newLine2->setDatabaseDefaults();
		bTR->appendAcDbEntity(newLine2);
		newLine2->close();
		bTR->close();
		tmpLine2 = newLine2->objectId();
	}

	AcDbLine* line1 = new AcDbLine();
	line1->copyFrom(newLine1);
	AcDbLine* line2 = new AcDbLine();
	line2->copyFrom(newLine2);
	AcGeLine3d l1(line1->startPoint(), line1->endPoint());
	AcGeLine3d l2(line2->startPoint(), line2->endPoint());
	AcGePointOnCurve3d ptOn1, ptOn2;
	l1.getClosestPointTo(asPnt3d(pt1), ptOn1);
	l2.getClosestPointTo(asPnt3d(pt2), ptOn2);
	AcGePoint3dArray pts;	
	line1->intersectWith(line2, AcDb::kExtendBoth, pts);
	crossP = pts[0]; // 교차점
	pts.removeAll();
	AcGeVector3d v1, v2, cenv;
	v1 = (crossP-ptOn1.point3d()).normal();
	v2 = (crossP-ptOn2.point3d()).normal();
	double radian = v1.angleTo(v2) / 2;
	AcGePoint3d bPoint, cPoint;
	double sinX = ((inputDist * (1000 / scaleValue)) / 2) / sin(radian);
	//bPoint = crossP - v2 * sinX;
	//cPoint = crossP - v1 * sinX;
	
	if(line1Ent->isKindOf(AcDbPolyline::desc()))
	{
		AcDbPolyline* pLine1 = AcDbPolyline::cast(line1Ent);
		double Midpoint_parm;
		AcGePoint3d OnLpoint1;
		pLine1->getClosestPointTo(pPoint1,OnLpoint1);
		pLine1->getParamAtPoint(OnLpoint1,Midpoint_parm);
		AcGePoint3d pL1_Spoint, pL1_Epoint;
		if(floor(Midpoint_parm) < 1.0)
		{
			//pLine1->getPointAtDist(sinX,cPoint);
			cPoint = crossP - v1 * sinX;
		}
		else
		{
			double EndDist;
			AcGePoint3d EndPoint1;
			pLine1->getEndPoint(EndPoint1);
			pLine1->getDistAtPoint(EndPoint1,EndDist);
			pLine1->getPointAtDist(EndDist - sinX, cPoint);			
		}
		pLine1->close();
	}
	else if(line1Ent->isKindOf(AcDbLine::desc()))
	{
		cPoint = crossP - v1 * sinX;
	}
	if(line2Ent->isKindOf(AcDbPolyline::desc()))
	{
		AcDbPolyline* pLine2 = AcDbPolyline::cast(line2Ent);
		double Midpoint_parm;
		AcGePoint3d OnLpoint2;
		pLine2->getClosestPointTo(pPoint2,OnLpoint2);
		pLine2->getParamAtPoint(OnLpoint2,Midpoint_parm);		
		AcGePoint3d pL2_Spoint, pL2_Epoint;
		if(floor(Midpoint_parm) < 1.0)
		{
			//pLine2->getPointAtDist(sinX,bPoint);
			bPoint = crossP - v2 * sinX;
		}
		else
		{
			double EndDist;
			AcGePoint3d EndPoint2;
			pLine2->getEndPoint(EndPoint2);
			pLine2->getDistAtPoint(EndPoint2,EndDist);
			pLine2->getPointAtDist(EndDist - sinX, bPoint);			
		}
		pLine2->close();
	}
	else if(line2Ent->isKindOf(AcDbLine::desc()))
	{
		bPoint = crossP - v2 * sinX;
	}
	
	AcDbLine* newLine = new AcDbLine;
	newLine->setStartPoint(bPoint);
	newLine->setEndPoint(cPoint);
	newLine->setLayer(curLayerStr);
	AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	newLine->setDatabaseDefaults();
	bTR->appendAcDbEntity(newLine);
	newLine->close();
	bTR->close();
	line1->close();
	line2->close();

	acdbOpenObject(newLine1,tmpLine1,AcDb::kForWrite);
	newLine1->erase();
	newLine1->close();

	acdbOpenObject(newLine2,tmpLine2,AcDb::kForWrite);
	newLine2->erase();
	newLine2->close();

	iniFile.writeINI(_T("config.ini"), _T("COA"), _T("MAINSCALE"), m_ScaleStr);
}

//취소
void CCOA::OnBnClickedCancel(){	
	CAcUiDialog::OnCancel();
}

//도면층 변경시
void CCOA::OnCbnSelchangeCbxlayer(){
	int curLayer = m_CbxLayerCtr.GetCurSel();
	CString layerName;
	m_CbxLayerCtr.GetLBText(m_CbxLayerCtr.GetCurSel(), layerName);
	LayerChangeGetColor(layerName);	
}

//버튼 클릭시
void CCOA::OnBnClickedBcolor(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();
	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();
	acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor);
	SelectColorChange(selectColor);
}

//도면층별 색상 가져오기
void CCOA::LayerChangeGetColor(CString layerName){
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);
	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		AcCmColor color;
		int colorIndex;
		pLayerTblRcd->getName(name);
		color = pLayerTblRcd->color();
		colorIndex = color.colorIndex();
		if(layerName.CompareNoCase(name) == 0){
			SelectColorChange(colorIndex);
		}
	}
	delete pLayerTblIter;
	delete pLayerTbl;
}

//색상 변경
void CCOA::SelectColorChange(int colorIndex){
	CString colorIndexStr, strMsg;
	colorIndexStr.Format(_T("%d"), colorIndex);
	if(colorIndexStr.CompareNoCase(_T("0")) == 0){
		strMsg.LoadStringW(IDS_BYBLOCK); // "블록별"
		colorIndexStr.Format(strMsg);
	}
	else if(colorIndexStr.CompareNoCase(_T("256")) == 0){
		strMsg.LoadStringW(IDS_BYLAYER); // "도면층별"
		colorIndexStr.Format(strMsg);
	}
	btnAfterClr = acedGetRGB(colorIndex);
	setColorIdx = colorIndex;
	Invalidate();
}

//도면층 가져오기
void CCOA::OnBnClickedBselect(){
	ads_name en;
	ads_point pt;
	CString strMsg;
	BeginEditorCommand();
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	strMsg.LoadStringW(IDS_GET_LAYER_ENTITY); // "\n도면층을 가져올 객체 선택: "
	if(acedEntSel(strMsg, en, pt) == RTNORM){
		AcDbObjectId entityId, selectId;
		AcDbEntity* eObj;
		entityId.setFromOldId(en[0]);
		acdbOpenObject(eObj, entityId, AcDb::kForRead);
		selectId = eObj->layerId();
		AcDbLayerTableRecordPointer pSelectLayer(selectId, AcDb::kForRead);
		TCHAR* selectLayerName;
		pSelectLayer->getName(selectLayerName);

		//도면층 불러오기
		AcDbLayerTable* pLayerTbl = NULL;
		AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
		AcDbLayerTableIterator* pLayerTblIter;
		pLayerPoint->newIterator(pLayerTblIter);
		int cbxIndex = 0;
		AcDbObjectId LayerId;
		for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
			pLayerTblIter->getRecordId(LayerId);
			AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
			TCHAR* name;
			pLayerTblRcd->getName(name);
			if(0 == _tcscmp(selectLayerName, name)){
				m_CbxLayerCtr.SetCurSel(cbxIndex);
				LayerChangeGetColor(selectLayerName);
				break;
			}
			cbxIndex++;
		}
		delete pLayerTblIter;
		delete pLayerTbl;
		OnCbnSelchangeCbxlayer();
	}
	else{

	}
	CompleteEditorCommand();
}

//스케일 입력시
void CCOA::OnEnUpdateEdtscale(){
	UpdateData(TRUE);
	m_ScaleStr = CheckInputNumber(m_ScaleStr);
	UpdateData(FALSE);
	m_ScaleCtrl.SetSel(0, -1);
	m_ScaleCtrl.SetSel(-1, -1);
	UpdateData(TRUE);
}

//숫자 및 점(.) 입력 체크
CString CCOA::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	for(int i = 0; i < str.GetLength(); i++){
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i, 1);
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46))
			str.Remove(*getAtChar);
	}
	return str;
}

//버튼 색상 입히기
void CCOA::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct){
	if((nIDCtl == IDC_BCOLOR)){
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct -> hDC);						//버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;						//버튼영역 구하기
		dc.Draw3dRect(&rect,RGB(255, 255, 255), RGB(0, 0, 0));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect, btnAfterClr);					//버튼색상
		UINT state = lpDrawItemStruct-> itemState;				//버튼상태구하기
		if((state &ODS_SELECTED))
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		else
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
	}
	CAcUiDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CCOA::OnBnClickedCoaHelp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Help_Path = g_modulePath;
	int findNUM = Help_Path.ReverseFind('\\');
	Help_Path.Delete(findNUM, Help_Path.GetLength() - 1);

	CString cadlang;
	cadlang.Format(L"%s", g_CADLang);

	CString lang =
		ReadStringValueInRegistry(HKEY_CURRENT_USER, L"SOFTWARE\\WORKS", L"Language");

	CString helpFile = L"ZDREAM.chm";
#ifdef _ZYXCAD
	helpFile.Replace(L"ZDREAM.chm", L"WORKS.chm");
#endif
	helpFile = GetHelpPath(cadlang, lang, helpFile);

	Help_Path.Format(_T("%s\\%s::/1. html/COA.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
