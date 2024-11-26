// DSTA.cpp : 구현 파일입니다.

#include "stdafx.h"
#include "DSTA.h"
#include "afxdialogex.h"

extern CDSTA* pDSTA;

// CDSTA 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDSTA, CAcUiDialog)

	CDSTA::CDSTA(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CDSTA::IDD, pParent)
	, m_staDist(_T("20"))
	, m_staLineLength(_T("10"))
	, m_staStartPoint(_T("0"))
	, m_staCircleDist(_T("100"))
	, m_textDist(_T("100"))
	, m_textOffset(_T("2.5"))
	, m_textSize(_T("250"))
	, m_decimalPosition(_T("4")) {
}

CDSTA::~CDSTA(){
}

void CDSTA::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDTSTADIST, m_staDist);
	DDX_Text(pDX, IDC_EDTSTALINE, m_staLineLength);
	DDX_Text(pDX, IDC_EDTSTASTART, m_staStartPoint);
	DDX_Text(pDX, IDC_EDTSTACIRCLE, m_staCircleDist);
	DDX_Text(pDX, IDC_EDTTEXTDIST, m_textDist);
	DDX_Text(pDX, IDC_EDTTEXTOFFSET, m_textOffset);
	DDX_Text(pDX, IDC_EDTTEXTSIZE, m_textSize);
	DDX_Text(pDX, IDC_EDTDECIMALPOINT, m_decimalPosition);
	DDX_Control(pDX, IDC_EDTSTADIST, m_staDistCtrl);
	DDX_Control(pDX, IDC_EDTSTALINE, m_staLineLengthCtrl);
	DDX_Control(pDX, IDC_EDTSTASTART, m_staStartPointCtrl);
	DDX_Control(pDX, IDC_EDTSTACIRCLE, m_staCircleDistCtrl);
	DDX_Control(pDX, IDC_EDTTEXTDIST, m_textDistCtrl);
	DDX_Control(pDX, IDC_EDTTEXTOFFSET, m_textOffsetCtrl);
	DDX_Control(pDX, IDC_EDTTEXTSIZE, m_textSizeCtrl);
	DDX_Control(pDX, IDC_CBXSTALAYER, m_cbxStaLayer);
	DDX_Control(pDX, IDC_CBXTEXTSTYLE, m_cbxTextStyle);
	DDX_Control(pDX, IDC_CBXTEXTLAYER, m_cbxTextLayer);
	DDX_Control(pDX, IDC_CBXTEXTPOSITION, m_cbxTextPosition);
	DDX_Control(pDX, IDC_BTNSTASELECT, m_staSelectBtn);
	DDX_Control(pDX, IDC_BTNTEXTSELECT, m_textSelectBtn);
}

BEGIN_MESSAGE_MAP(CDSTA, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_RADSTA, &CDSTA::OnBnClickedRadsta)
	ON_BN_CLICKED(IDC_RADNO, &CDSTA::OnBnClickedRadno)
	ON_EN_UPDATE(IDC_EDTSTADIST, &CDSTA::OnEnUpdateEdtstadist)
	ON_EN_UPDATE(IDC_EDTSTALINE, &CDSTA::OnEnUpdateEdtstaline)
	ON_EN_UPDATE(IDC_EDTSTASTART, &CDSTA::OnEnUpdateEdtstastart)
	ON_EN_UPDATE(IDC_EDTSTACIRCLE, &CDSTA::OnEnUpdateEdtstacircle)
	ON_EN_UPDATE(IDC_EDTTEXTDIST, &CDSTA::OnEnUpdateEdttextdist)
	ON_EN_UPDATE(IDC_EDTTEXTOFFSET, &CDSTA::OnEnUpdateEdttextoffset)
	ON_EN_UPDATE(IDC_EDTTEXTSIZE, &CDSTA::OnEnUpdateEdttextsize)
	ON_BN_CLICKED(IDC_BTNSTASELECT, &CDSTA::OnBnClickedBtnstaselect)
	ON_BN_CLICKED(IDC_BTNTEXTSELECT, &CDSTA::OnBnClickedBtntextselect)
	ON_BN_CLICKED(IDC_CHKSTASTART, &CDSTA::OnBnClickedChkstastart)
	ON_BN_CLICKED(IDC_CHKTEXTVISIBLE, &CDSTA::OnBnClickedChktextvisible)
	ON_BN_CLICKED(IDC_CHKSTACIRCLE, &CDSTA::OnBnClickedChkstacircle)
	ON_BN_CLICKED(IDOK, &CDSTA::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDSTA::OnBnClickedCancel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPNDECIMALPOINT, &CDSTA::OnDeltaposSpndecimalpoint)
	ON_BN_CLICKED(IDC_BTNSTACOLOR, &CDSTA::OnBnClickedBtnstacolor)
	ON_BN_CLICKED(IDC_BTNTEXTCOLOR, &CDSTA::OnBnClickedBtntextcolor)
	ON_CBN_SELCHANGE(IDC_CBXSTALAYER, &CDSTA::OnCbnSelchangeCbxstalayer)
	ON_WM_DRAWITEM()
	ON_CBN_SELCHANGE(IDC_CBXTEXTLAYER, &CDSTA::OnCbnSelchangeCbxtextlayer)
	ON_BN_CLICKED(IDC_CHKTEXT, &CDSTA::OnBnClickedChktext)
	ON_CBN_SELCHANGE(IDC_CBXTEXTSTYLE, &CDSTA::OnCbnSelchangeCbxtextstyle)
	ON_BN_CLICKED(ID_DSTA_HELP, &CDSTA::OnBnClickedDstaHelp)
END_MESSAGE_MAP()

// CDSTA 메시지 처리기입니다.
//대화상자 초기화
BOOL CDSTA::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	/*bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);*/

	auto IDStoCString = [](int a_ID) -> CString {
		CString IDS;
		IDS.LoadStringW(a_ID);
		return IDS;
		};

	//문자 위치 콤보박스 초기화
	m_cbxTextPosition.AddString(IDStoCString(IDS_LINE_TOP));			// "선 위쪽"
	m_cbxTextPosition.AddString(IDStoCString(IDS_LINE_BOTTOM));			// "선 아래쪽"
	m_cbxTextPosition.AddString(IDStoCString(IDS_LINE_VERTICAL));		// "선과 수직"
	m_cbxTextPosition.AddString(IDStoCString(IDS_LINE_BOTTOM_VERT));	// "선 아래 수직"

	//저장 옵션 불러오기	
	CString textPosStr = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("TEXTPOSITION"));
	if(textPosStr.CompareNoCase(_T("")) == 0 || textPosStr.CompareNoCase(_T("0")) == 0)		
		m_cbxTextPosition.SetCurSel(0);
	else if(textPosStr.CompareNoCase(_T("1")) == 0)		m_cbxTextPosition.SetCurSel(1);
	else if(textPosStr.CompareNoCase(_T("2")) == 0)		m_cbxTextPosition.SetCurSel(2);
	else if (textPosStr.CompareNoCase(_T("3")) == 0)		m_cbxTextPosition.SetCurSel(3);

	m_staDist = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("STADIST"));
	if(m_staDist.CompareNoCase(_T("")) == 0)	m_staDist = _T("20");
	SetDlgItemText(IDC_EDTSTADIST, m_staDist);

	m_staLineLength = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("STALINELENGTH"));
	if(m_staLineLength.CompareNoCase(_T("")) == 0)	m_staLineLength = _T("2");
	SetDlgItemText(IDC_EDTSTALINE, m_staLineLength);

	m_staStartPoint = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("STASTARTPOINT"));
	if(m_staStartPoint.CompareNoCase(_T("")) == 0)	m_staStartPoint = _T("0");
	SetDlgItemText(IDC_EDTSTASTART, m_staStartPoint);

	CString SelStartPt = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("SELECTSTART"));
	if (SelStartPt.CompareNoCase(_T("")) == 0 || SelStartPt.CompareNoCase(_T("TRUE")) == 0) {
		selectStart = true;
		CheckDlgButton(IDC_CHKSTASTART, true);
	}
	else {
		selectStart = false;
		CheckDlgButton(IDC_CHKSTASTART, false);
	}

	CString makeCircleStr = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("MAKECIRCLE"));
	if(makeCircleStr.CompareNoCase(_T("")) == 0 || makeCircleStr.CompareNoCase(_T("TRUE")) == 0){
		makeCircle = true;
		CheckDlgButton(IDC_CHKSTACIRCLE, true);
	}
	else{
		makeCircle = false;
		CheckDlgButton(IDC_CHKSTACIRCLE, false);
	}

	CString makeTextStr = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("MAKETEXT"));
	if(makeTextStr.CompareNoCase(_T("")) == 0 || makeTextStr.CompareNoCase(_T("TRUE")) == 0){
		makeText = true;
		CheckDlgButton(IDC_CHKTEXT, true);
	}
	else{
		makeText = false;
		CheckDlgButton(IDC_CHKTEXT, false);
	}

	m_staCircleDist = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("CIRCLEDIST"));
	if(m_staCircleDist.CompareNoCase(_T("")) == 0)	m_staCircleDist = _T("100");
	SetDlgItemText(IDC_EDTSTACIRCLE, m_staCircleDist);

	m_textDist = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("TEXTDIST"));
	if(m_textDist.CompareNoCase(_T("")) == 0)	m_textDist = _T("100");
	SetDlgItemText(IDC_EDTTEXTDIST, m_textDist);

	m_textOffset = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("TEXTOFFSET"));
	if(m_textOffset.CompareNoCase(_T("")) == 0)	m_textOffset = _T("2.5");
	SetDlgItemText(IDC_EDTTEXTOFFSET, m_textOffset);

	m_textSize = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("TEXTSIZE"));
	if(m_textSize.CompareNoCase(_T("")) == 0)	m_textSize = _T("2.5");
	SetDlgItemText(IDC_EDTTEXTSIZE, m_textSize);

	m_decimalPosition = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("DECIMALPOSITION"));
	if(m_decimalPosition.CompareNoCase(_T("")) == 0)	m_decimalPosition = _T("4");
	SetDlgItemText(IDC_EDTDECIMALPOINT, m_decimalPosition);

	CString jobStr = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("JOBSTAT"));
	CString strMsg;
	if(jobStr.CompareNoCase(_T("NUM")) == 0)
	{
		CheckDlgButton(IDC_RADNO, true);
		strMsg.LoadStringW(IDS_ZERO_HIDE); // " +0 표기 생략"
		SetDlgItemText(IDC_CHKTEXTVISIBLE, strMsg); // 김성곤 대리 추가
		jobMode = 1;
	}
	else
	{
		CheckDlgButton(IDC_RADSTA, true);
		strMsg.LoadStringW(IDS_KM_HIDE); // " ㎞ 문자 표기 생략"
		SetDlgItemText(IDC_CHKTEXTVISIBLE, strMsg); // 김성곤 대리 추가
		jobMode = 0;		
	}

	CString LayerStr = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("LAYER"));
	if(LayerStr.CompareNoCase(_T("")) == 0){
		LayerStr = "0";
	}

	CString TextLayerStr = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("TEXTLAYER"));
	if(TextLayerStr.CompareNoCase(_T("")) == 0){
		TextLayerStr = "0";
	}

	CString TextStyleStr = iniFile.readINI(_T("config.ini"), _T("DSTA"), _T("TEXTSTYLE"));
	if(TextStyleStr.CompareNoCase(_T("")) == 0){
		TextStyleStr = "Standard";
	}

	//도면층 불러오기	
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);

	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		pLayerTblRcd->getName(name);
		pLayerTblRcd->close();
		m_cbxStaLayer.AddString(name);
		m_cbxTextLayer.AddString(name);
	}
	delete pLayerTblIter;	
	//m_cbxStaLayer.SetCurSel(0);
	//m_cbxTextLayer.SetCurSel(0);

	// 문자 스타일 불러오기	
	/*
	AcDbTextStyleTablePointer pStylePoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbTextStyleTableIterator* pStyleTblIter;
	pStylePoint->newIterator(pStyleTblIter);

	AcDbObjectId styleId;
	for(pStyleTblIter->start(); !pStyleTblIter->done(); pStyleTblIter->step()){
		pStyleTblIter->getRecordId(styleId);
		AcDbTextStyleTableRecordPointer pStyleTblRcd(styleId, AcDb::kForRead);
		TCHAR* name;		
		pStyleTblRcd->getName(name);
		pStyleTblRcd->close();
		m_cbxTextStyle.AddString(name);
	}
	delete pStyleTblIter;
	*/

	AcDbTextStyleTablePointer pStylePoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbTextStyleTableIterator* pStyleTblIter;
	pStylePoint->newIterator(pStyleTblIter);

	AcDbObjectId StyleId;
	for (pStyleTblIter->start(); !pStyleTblIter->done(); pStyleTblIter->step()) {
		pStyleTblIter->getRecordId(StyleId);
		AcDbTextStyleTableRecordPointer pStyleTblRcd(StyleId, AcDb::kForRead);
		if (pStyleTblRcd.openStatus() == Acad::eOk)
		{
			ACHAR* StyleName;
			pStyleTblRcd->getName(StyleName);
			if (_tcscmp(StyleName, _T("")) != 0)
			{
				m_cbxTextStyle.AddString(StyleName);
			}
		}
	}
	delete pStyleTblIter;
	//m_cbxTextStyle.SetCurSel(0);

	// 객체 레이어 설정
	int Layindex = m_cbxStaLayer.SelectString(-1,LayerStr);
	if(Layindex != -1)
	{
		m_cbxStaLayer.SetCurSel(Layindex);
	}
	else
	{
		m_cbxStaLayer.SetCurSel(0);
	}

	
	m_cbxStaLayer.GetLBString(m_cbxStaLayer.GetCurSel(), curLayerStr);
	LayerChangeGetColor(curLayerStr);
	// 문자 레이어 설정
	int TextLayindex = m_cbxTextLayer.SelectString(-1,TextLayerStr);
	if(TextLayindex != -1)
	{
		m_cbxTextLayer.SetCurSel(TextLayindex);
	}
	else
	{
		m_cbxTextLayer.SetCurSel(0);
	}

	CString curTextLayerStr;
	m_cbxTextLayer.GetLBString(m_cbxTextLayer.GetCurSel(), curTextLayerStr);
	LayerChangeGetColor(curTextLayerStr);
	// 문자 스타일 설정
	int TextStyindex = m_cbxTextStyle.SelectString(-1,TextStyleStr);
	if(TextStyindex != -1)
	{
		m_cbxTextStyle.SetCurSel(TextStyindex);
	}
	else
	{
		m_cbxTextStyle.SetCurSel(0);
	}

	CString curTextStyleStr;
	m_cbxTextStyle.GetLBString(m_cbxTextStyle.GetCurSel(), curTextStyleStr);
	OnCbnSelchangeCbxstalayer();
	OnCbnSelchangeCbxtextlayer();
	OnBnClickedChkstacircle();

	m_staSelectBtn.AutoLoad();
	m_textSelectBtn.AutoLoad();
	return TRUE;
}

//station 라디오 체크
void CDSTA::OnBnClickedRadsta(){
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	CString strMsg;
	strMsg.LoadStringW(IDS_KM_HIDE); // " ㎞ 문자 표기 생략"
	SetDlgItemText(IDC_CHKTEXTVISIBLE, strMsg);
	jobMode = 0;
}

//Number 라디오 체크
void CDSTA::OnBnClickedRadno(){
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	CString strMsg;
	strMsg.LoadStringW(IDS_ZERO_HIDE); // " +0 표기 생략"
	SetDlgItemText(IDC_CHKTEXTVISIBLE, strMsg);
	jobMode = 1;
}

//sta 측점 표기 간격 
void CDSTA::OnEnUpdateEdtstadist(){
	UpdateData(TRUE);
	for(int i = 0; i < m_staDist.GetLength(); i++) {
		CString str = m_staDist.Mid(i,1);
		TCHAR* tchr = (TCHAR*)(LPCTSTR)str;
		int ascii = __toascii(*tchr);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46)){
			m_staDist.Remove(*tchr);
		}
	}
	UpdateData( FALSE );
	m_staDistCtrl.SetSel(0,-1);
	m_staDistCtrl.SetSel(-1, -1);
}

//sta 측점 선의 길이
void CDSTA::OnEnUpdateEdtstaline(){
	UpdateData( TRUE );
	for(int i = 0 ; i < m_staLineLength.GetLength() ; i++) {
		CString str = m_staLineLength.Mid(i,1);
		TCHAR* tchr = (TCHAR*)(LPCTSTR)str;
		int ascii = __toascii(*tchr);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46)){
			m_staLineLength.Remove(*tchr);
		}
	}
	UpdateData( FALSE );
	m_staLineLengthCtrl.SetSel(0,-1);
	m_staLineLengthCtrl.SetSel(-1, -1);
}

//sta 측점 시작
void CDSTA::OnEnUpdateEdtstastart(){
	UpdateData( TRUE );
	for(int i = 0 ; i < m_staStartPoint.GetLength() ; i++) {
		CString str = m_staStartPoint.Mid(i,1);
		TCHAR* tchr = (TCHAR*)(LPCTSTR)str;
		int ascii = __toascii(*tchr);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46)){
			m_staStartPoint.Remove(*tchr);
		}
	}
	UpdateData( FALSE );
	m_staStartPointCtrl.SetSel(0,-1);
	m_staStartPointCtrl.SetSel(-1, -1);	
}

//sta 측점 원 표기 간격
void CDSTA::OnEnUpdateEdtstacircle(){
	UpdateData( TRUE );
	for(int i = 0 ; i < m_staCircleDist.GetLength() ; i++) {
		CString str = m_staCircleDist.Mid(i,1);
		TCHAR* tchr = (TCHAR*)(LPCTSTR)str;
		int ascii = __toascii(*tchr);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46)){
			m_staCircleDist.Remove(*tchr);
		}
	}
	UpdateData( FALSE );
	m_staCircleDistCtrl.SetSel(0,-1);
	m_staCircleDistCtrl.SetSel(-1, -1);	
}

//문자 표기 간격
void CDSTA::OnEnUpdateEdttextdist(){
	UpdateData( TRUE );
	for(int i = 0 ; i < m_textDist.GetLength() ; i++) {
		CString str = m_textDist.Mid(i,1);
		TCHAR* tchr = (TCHAR*)(LPCTSTR)str;
		int ascii = __toascii(*tchr);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46)){
			m_textDist.Remove(*tchr);
		}
	}
	UpdateData( FALSE );
	m_textDistCtrl.SetSel(0,-1);
	m_textDistCtrl.SetSel(-1, -1);	
}

//문자 이격 거리
void CDSTA::OnEnUpdateEdttextoffset(){
	UpdateData( TRUE );
	for(int i = 0 ; i < m_textOffset.GetLength() ; i++) {
		CString str = m_textOffset.Mid(i,1);
		TCHAR* tchr = (TCHAR*)(LPCTSTR)str;
		int ascii = __toascii(*tchr);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46)){
			m_textOffset.Remove(*tchr);
		}
	}
	UpdateData( FALSE );
	m_textOffsetCtrl.SetSel(0,-1);
	m_textOffsetCtrl.SetSel(-1, -1);	
}

//문자 크기
void CDSTA::OnEnUpdateEdttextsize(){
	UpdateData( TRUE );
	for(int i = 0 ; i < m_textSize.GetLength() ; i++) {
		CString str = m_textSize.Mid(i,1);
		TCHAR* tchr = (TCHAR*)(LPCTSTR)str;
		int ascii = __toascii(*tchr);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46)){
			m_textSize.Remove(*tchr);
		}
	}
	UpdateData( FALSE );
	m_textSizeCtrl.SetSel(0,-1);
	m_textSizeCtrl.SetSel(-1, -1);	
}

//선택 지점부터 표기 시작 체크
void CDSTA::OnBnClickedChkstastart(){
	selectStart = IsDlgButtonChecked(IDC_CHKSTASTART);
}

//원 표기 간격 체크
void CDSTA::OnBnClickedChkstacircle(){
	makeCircle = IsDlgButtonChecked(IDC_CHKSTACIRCLE);
	if(makeCircle)	GetDlgItem(IDC_EDTSTACIRCLE)->EnableWindow();
	else			GetDlgItem(IDC_EDTSTACIRCLE)->EnableWindow(false);
}

//문자 표기 생략 체크
void CDSTA::OnBnClickedChktextvisible(){
	textVisible = IsDlgButtonChecked(IDC_CHKTEXTVISIBLE);
}

//측점 옵션 가져오기 버튼
void CDSTA::OnBnClickedBtnstaselect(){
	ads_name en;
	ads_point pt;
	CString strMsg;
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	strMsg.LoadStringW(IDS_SELECT_ENTITY); // "\n객체 선택: "
	if(acedEntSel(strMsg, en, pt) != Acad::eOk)	CompleteEditorCommand();

	AcDbObjectId objId;
	if(acdbGetObjectId(objId, en) == Acad::eOk){
		AcDbEntityPointer pEnt(objId, AcDb::kForRead);
		if(pEnt.openStatus() == Acad::eOk){
			if(pEnt->isKindOf(AcDbEntity::desc())){
				AcDbEntity* pEntity = AcDbEntity::cast(pEnt);
				TCHAR* layerName = pEntity->layer();

				for(int i=0; i<m_cbxStaLayer.GetCount(); i++){
					CString curStyle;
					m_cbxStaLayer.GetLBText(i, curStyle);
					if(findObjectLayer(curStyle) == pEntity->layerId()){
						m_cbxStaLayer.SetCurSel(i);
						break;
					}
				}

				int colorIndex = pEntity->colorIndex();
				SelectColorChange(colorIndex, 1);
				pEntity->close();
			}
		}
		pEnt->close();
	}
}

// TextStyle 찾기
AcDbObjectId CDSTA::findObjectLayer(CString name){	
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);

	AcDbObjectId layerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(layerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(layerId, AcDb::kForRead);

		TCHAR* name2;
		pLayerTblRcd->getName(name2);
		pLayerTblRcd->close();
		if(wcscmp(name, name2) == 0){
			delete pLayerTblIter;
			delete pLayerTbl;
			return layerId;
		}
	}

	delete pLayerTblIter;
	delete pLayerTbl;

	return NULL;
}

//문자 옵션 가져오기 버튼
void CDSTA::OnBnClickedBtntextselect(){	
	ads_name en;
	ads_point pt;
	CString strMsg;
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	strMsg.LoadStringW(IDS_SELECT_ENTITY); // "\n객체 선택: "
	if(acedEntSel(strMsg, en, pt) != Acad::eOk)	CompleteEditorCommand();

	AcDbObjectId objId, styleId;
	if(acdbGetObjectId(objId, en) == Acad::eOk){
		AcDbEntityPointer pEnt(objId, AcDb::kForRead);
		if(pEnt.openStatus() == Acad::eOk){
			if(pEnt->isKindOf(AcDbText::desc())){
				AcDbText* pText = AcDbText::cast(pEnt);
				TCHAR* layerName = pText->layer();
				styleId = pText->textStyle();

				for(int i = 0; i < m_cbxTextStyle.GetCount(); i++){
					CString curStyle;
					m_cbxTextStyle.GetLBText(i, curStyle);
					if(findTextStyle(curStyle) == pText->textStyle()){
						m_cbxTextStyle.SetCurSel(i);
						break;
					}
				}
				int colorIndex = pText->colorIndex();
				SelectColorChange(colorIndex, 2);
				pText->close();
			}
		}
		pEnt->close();
	}
}

// TextStyle 찾기
AcDbObjectId CDSTA::findTextStyle(CString name){
	AcDbTextStyleTable* pTextTbl = NULL;
	AcDbTextStyleTablePointer pTextPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbTextStyleTableIterator* pTextTblIter;
	pTextPoint->newIterator(pTextTblIter);

	AcDbObjectId TextId;
	for(pTextTblIter->start(); !pTextTblIter->done(); pTextTblIter->step()){
		pTextTblIter->getRecordId(TextId);
		AcDbTextStyleTableRecordPointer pTextTblRcd(TextId, AcDb::kForRead);

		TCHAR* name2;
		pTextTblRcd->getName(name2);
		pTextTblRcd->close();
		if(wcscmp(name, name2) == 0)
			return TextId;
	}

	delete pTextTblIter;
	delete pTextTbl;

	return NULL;
}

int CDSTA::gcd(int a, int b)
{
	int n;

	while (b != 0) {
		n = a % b;
		a = b;
		b = n;
	}

	return a;
}

int CDSTA::lcm(int a, int b)
{
	return a * b / gcd(a, b);
}

double CDSTA::getRadian(double degree) {
	double rad = 0;
	rad = degree * 3.141592 / 180;
	return rad;
}

double CDSTA::getStartOffset(double& Dist, double& Point, double& Result)
{
	if (Dist < 0.00000001) // staDist == 0
		Result = abs(Point);
	else
		Result = abs(fmod(Dist, Point));

	return Result;
}

AcGePoint3d CDSTA::vecTmpPt(AcDbCurve* pObj, double totalDist, AcGePoint3d Pt, AcGePoint3d tmpPt)
{
	double st_Param, end_Param, DistChk, PtParam;
	AcGePoint3d t_spt;
	pObj->getDistAtPoint(Pt, DistChk);
	pObj->getParamAtPoint(Pt, PtParam);
	pObj->getStartParam(st_Param);
	pObj->getEndParam(end_Param);
	/*acutPrintf(L"\nst_Param = %f\n", st_Param);
	acutPrintf(L"\nend_Param = %f\n", end_Param);
	acutPrintf(L"\nPtParam = %f\n", PtParam);
	acutPrintf(L"\nDistChk = %f\n", DistChk);
	acutPrintf(L"\n(abs(PtParam - st_Param)) = %f\n", (abs(PtParam - st_Param)));*/
	//acutPrintf(L"\nPt = %g, %g, %g\n", Pt.x, Pt.y, Pt.z);
	
	if ((abs(PtParam - st_Param)) < 0.0001)
	{
		//acutPrintf(L"\n+ 0.0001\n");
		pObj->getPointAtDist(DistChk + 0.0001, tmpPt);
	}

	if ((abs(PtParam - end_Param)) < 0.0001)
	{ 
		//acutPrintf(L"\n- 0.0001\n");
		pObj->getPointAtDist(DistChk - 0.0001, tmpPt);
	}

	//acutPrintf(L"\nDistChk2 = %f\n", DistChk);
	//acutPrintf(L"\ntmpPt = %g, %g, %g\n", tmpPt.x, tmpPt.y, tmpPt.z);

	return tmpPt;
}

void CDSTA::BothLineDraw(AcGePoint3d spt, AcGePoint3d ept, AcGePoint3d stmpPt, AcGePoint3d etmpPt, CString curLayerStr)
{
	AcGeVector3d stVerVec = (spt - stmpPt).normal();
	stVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	AcGePoint3d stVerSp = spt + stVerVec * (_ttof(m_staLineLength) / 2.0);
	AcGePoint3d stVerEp = stVerSp - stVerVec * _ttof(m_staLineLength);

	AcGeVector3d ltVerVec = (etmpPt - ept).normal();
	ltVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	AcGePoint3d ltVerSp = ept + ltVerVec * (_ttof(m_staLineLength) / 2.0);
	AcGePoint3d ltVerEp = ltVerSp - ltVerVec * _ttof(m_staLineLength);

	AcDbLine* pStartDrawLine = new AcDbLine(stVerSp, stVerEp);
	pStartDrawLine->setLayer(curLayerStr);
	if (ChangeLayerColor == true)
		pStartDrawLine->setColorIndex(m_selectLineColorIndex);

	AcDbLine* pEndwDrawLine = new AcDbLine(ltVerSp, ltVerEp);
	pEndwDrawLine->setLayer(curLayerStr);
	if (ChangeLayerColor == true)
		pEndwDrawLine->setColorIndex(m_selectLineColorIndex);

	AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	pStartDrawLine->setDatabaseDefaults();
	pEndwDrawLine->setDatabaseDefaults();
	bTR->appendAcDbEntity(pStartDrawLine);
	bTR->appendAcDbEntity(pEndwDrawLine);
	pStartDrawLine->close();
	pEndwDrawLine->close();

	bTR->close();
}

void CDSTA::BothMakeCircle(AcGePoint3d spt, AcGePoint3d ept, CString curLayerStr)
{
	AcGePoint3d cenPt;
	AcDbCircle* drawStartCircle = new AcDbCircle();
	drawStartCircle->setCenter(spt);
	drawStartCircle->setRadius((_ttof(m_staLineLength) / 2.0));
	drawStartCircle->setLayer(curLayerStr);
	if (ChangeLayerColor == true)
		drawStartCircle->setColorIndex(m_selectLineColorIndex);

	AcDbCircle* drawEndCircle = new AcDbCircle();
	drawEndCircle->setCenter(ept);
	drawEndCircle->setRadius((_ttof(m_staLineLength) / 2.0));
	drawEndCircle->setLayer(curLayerStr);
	if (ChangeLayerColor == true)
		drawEndCircle->setColorIndex(m_selectLineColorIndex);

	AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	drawStartCircle->setDatabaseDefaults();
	drawEndCircle->setDatabaseDefaults();
	bTR->appendAcDbEntity(drawStartCircle);
	bTR->appendAcDbEntity(drawEndCircle);
	drawStartCircle->close();
	drawEndCircle->close();
	bTR->close();
}

void CDSTA::PostNcDestroy()
{
	delete this;
	pDSTA = NULL;
	//CAcUiDialog::PostNcDestroy();
}

//문자 소수점 위치 스핀
void CDSTA::OnDeltaposSpndecimalpoint(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int dotCnt = _ttoi(m_decimalPosition);
	if(pNMUpDown->iDelta < 0){
		if(dotCnt >= 4 ) return;
		dotCnt++;
	}
	else{
		if(dotCnt <= 0) return;
		dotCnt--;
	}
	m_decimalPosition.Format(_T("%d"), dotCnt);
	SetDlgItemText(IDC_EDTDECIMALPOINT, m_decimalPosition);
	*pResult = 0;
}

//STA 색상 클릭시
void CDSTA::OnBnClickedBtnstacolor(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();

	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();

	int selColor;
	if(acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor) != Acad::eOk){
		m_staColorBtn = acedGetRGB(nCurColor);	
		SelectColorChange(selectColor, 1);
	}
	ChangeLayerColor = true;
}

//문자 색상 클릭시
void CDSTA::OnBnClickedBtntextcolor(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();

	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();

	int selColor;
	if(acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor) != Acad::eOk){
		m_textColorBtn = acedGetRGB(nCurColor);	
		SelectColorChange(selectColor, 2);
	}
	ChangeTextColor = true;
}

//STA 도면층 콤보박스 변경시
void CDSTA::OnCbnSelchangeCbxstalayer(){	
	CString layerName;
	m_cbxStaLayer.GetLBText(m_cbxStaLayer.GetCurSel(), layerName);
	int colorIndex = LayerChangeGetColor(layerName);
	SelectColorChange(colorIndex, 1);
}

//문자 도면층 콤보박스 변경시
void CDSTA::OnCbnSelchangeCbxtextlayer(){
	CString layerName;
	m_cbxStaLayer.GetLBText(m_cbxTextLayer.GetCurSel(), layerName);
	int colorIndex = LayerChangeGetColor(layerName);
	SelectColorChange(colorIndex, 2);
}

//문자 스타일 콤보박스 변경시
void CDSTA::OnCbnSelchangeCbxtextstyle(){
}

//도면층별 색상 가져오기
int CDSTA::LayerChangeGetColor(CString layerName){
	//AcDbLayerTable* pLayerTbl = NULL;
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
			delete pLayerTblIter;
			//delete pLayerTbl;
			return colorIndex;
		}
		pLayerTblRcd->close();
	}
	delete pLayerTblIter;
	//delete pLayerTbl;
	return 0;
}

//색상 인덱스 표기
void CDSTA::SelectColorChange(int colorIndex, int stat){
	ChangeTextColor = false;
	ChangeLayerColor = false;

	CString colorIndexStr;
	colorIndexStr.Format(_T("%d"), colorIndex);
	if(colorIndexStr.CompareNoCase(_T("0")) == 0)
		colorIndexStr.Format(_T("By Block"));
	else if(colorIndexStr.CompareNoCase(_T("256")) == 0)
		colorIndexStr.Format(_T("By Layer"));

	if(stat == 1){
		SetDlgItemTextW(IDC_STTSTACOLORINDEX, colorIndexStr);
		m_staColorBtn = acedGetRGB(colorIndex);
		m_selectLineColorIndex = colorIndex;		
	}
	else if(stat == 2){
		SetDlgItemTextW(IDC_STTTEXTCOLORINDEX, colorIndexStr);
		m_textColorBtn = acedGetRGB(colorIndex);
		m_selectTextColorIndex = colorIndex;		
	}
	Invalidate();
}

//버튼 색상 입히기
void CDSTA::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct){
	if((nIDCtl == IDC_BTNSTACOLOR)){
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct->hDC);						//버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;						//버튼영역 구하기
		dc.Draw3dRect(&rect,RGB(255, 255, 255), RGB(0, 0, 0));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect, m_staColorBtn);					//버튼색상
		UINT state = lpDrawItemStruct->itemState;				//버튼상태구하기
		if((state &ODS_SELECTED)){
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		}
		else{
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
		}
		dc.Detach();											//버튼의 dc 풀어주기
	}
	else if((nIDCtl == IDC_BTNTEXTCOLOR)){
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct->hDC);						//버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;						//버튼영역 구하기
		dc.Draw3dRect(&rect,RGB(255, 255, 255), RGB(0, 0, 0));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect, m_textColorBtn);				//버튼색상
		UINT state = lpDrawItemStruct->itemState;				//버튼상태구하기
		if((state &ODS_SELECTED)){
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		}
		else{
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
		}
		dc.Detach();										//버튼의 dc 풀어주기
	}
	CAcUiDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

LRESULT CDSTA::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

//실행
void CDSTA::OnBnClickedOk(){
	UpdateData();
	CAcUiDialog::OnOK();

	acDocManagerPtr()->lockDocument(curDoc());

	CString saveTmpStr;
	int a = m_cbxTextPosition.GetCurSel();
	saveTmpStr.Format(_T("%d"), a);
	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("TEXTPOSITION"), saveTmpStr);
	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("STADIST"), m_staDist);
	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("STALINELENGTH"), m_staLineLength);
	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("STASTARTPOINT"), m_staStartPoint);
	if (IsDlgButtonChecked(IDC_CHKSTASTART))
		iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("SELECTSTART"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("SELECTSTART"), _T("FALSE"));

	if(IsDlgButtonChecked(IDC_CHKSTACIRCLE))
		iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("MAKECIRCLE"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("MAKECIRCLE"), _T("FALSE"));

	if(IsDlgButtonChecked(IDC_CHKTEXT))
		iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("MAKETEXT"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("MAKETEXT"), _T("FALSE"));

	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("CIRCLEDIST"), m_staCircleDist);
	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("TEXTDIST"), m_textDist);
	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("TEXTOFFSET"), m_textOffset);
	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("TEXTSIZE"), m_textSize);
	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("DECIMALPOSITION"), m_decimalPosition);

	if(IsDlgButtonChecked(IDC_RADNO))
		iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("JOBSTAT"), _T("NUM"));
	else
		iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("JOBSTAT"), _T("STA"));
	// 객체 layer, 문자 layer, 문자 style 정보 남도록 수정
	CString iniLay;
	m_cbxStaLayer.GetLBText(m_cbxStaLayer.GetCurSel(),iniLay);
	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("LAYER"), iniLay);

	CString iniTextLay;
	m_cbxTextLayer.GetLBText(m_cbxTextLayer.GetCurSel(),iniTextLay);
	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("TEXTLAYER"), iniTextLay);

	CString iniTextStyle;
	m_cbxTextStyle.GetLBText(m_cbxTextStyle.GetCurSel(),iniTextStyle);
	iniFile.writeINI(_T("config.ini"), _T("DSTA"), _T("TEXTSTYLE"), iniTextStyle);

	//int result;
	bool selectQuit = false;
	bool roop = true;
	//ads_name ss;

	/*resbuf* filter = ads_buildlist(RTDXF0, _T("LINE,LWPOLYLINE,ARC"), 0);
	if(acedSSGet(NULL, NULL, NULL ,filter, ss) != RTNORM){
		acutRelRb(filter);
		return;
	}
	acutRelRb(filter);*/

	ads_name en;
	ads_point pt;
	AcDbObjectId objId;
	//int result;
	
	int iVertex;
	int stat;
	bool bArc = false;
	CString strMsg;
	//BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while (true)
	{
		acedGetAcadDwgView()->SetFocus();
		//::SetFocus(adsw_acadDocWnd());
		// 미리 선택된 객체 선택 무효화
		acedSSSetFirst(NULL, NULL);
		strMsg.LoadStringW(IDS_SELECT_ENTITY); // "\n객체 선택: "
		stat = acedEntSel(strMsg, en, pt);
		as_pt = asPnt3d(pt);
		if (stat == RTCAN) {
			acedSSFree(en);
			//delete this;
			pDSTA = NULL;
			//CancelEditorCommand();
			break;
		}
		else if (stat == RTERROR) {
			resbuf zValueStr;
			ads_getvar(_T("ERRNO"), &zValueStr);
			if (zValueStr.resval.rint == 52) {
				acedSSFree(en);
				//delete this;
				pDSTA = NULL;
				//CancelEditorCommand();
				break;
			}
			else
				continue;
		}
		else if (stat == RTKWORD) {
			continue;
		}
		else if (stat == RTNORM) {
			
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForWrite);

			if (pEnt.openStatus() == Acad::eOk)
			{
				// 선
				if (pEnt->isKindOf(AcDbLine::desc()))
				{
					AcDbLine* pLine = AcDbLine::cast(pEnt);
					double t_dist;
					double pt_dist;
					pLine->getStartPoint(spt);
					pLine->getEndPoint(ept);
					pLine->getDistAtPoint(ept, t_dist);
					pLine->getClosestPointTo(asPnt3d(pt), as_pt);
					pLine->getDistAtPoint(as_pt, pt_dist);
					//선택 지점부터 표기 시작 시 처음점과 끝점 처리
					if (selectStart == 1)
					{
						// 끝에서 시작
						if ((t_dist / 2) < pt_dist)
						{
							pLine->getStartPoint(ept);
							pLine->getEndPoint(spt);
						}
					}
					pLine->close();
				}
				// 폴리선
				else if (pEnt->isKindOf(AcDbPolyline::desc()))
				{
					AcDbPolyline* pPLine = AcDbPolyline::cast(pEnt);
					double t_dist;
					double pt_dist;
					iVertex = pPLine->numVerts();
					pPLine->getStartPoint(spt);
					pPLine->getEndPoint(ept);
					pPLine->getDistAtPoint(ept, t_dist);
					pPLine->getClosestPointTo(asPnt3d(pt), as_pt);
					pPLine->getDistAtPoint(as_pt, pt_dist);
					//선택 지점부터 표기 시작 시 처음점과 끝점 처리
					if (selectStart == 1)
					{
						// 끝에서 시작
						if ((t_dist / 2) < pt_dist)
						{
							pPLine->getStartPoint(ept);
							pPLine->getEndPoint(spt);
						}
					}
					pPLine->close();
				}
					
				// 호
				else if (pEnt->isKindOf(AcDbArc::desc()))
				{
					AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
					acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);

					AcDbArc* pArc = AcDbArc::cast(pEnt);
					AcDbPolyline* pPLine = new AcDbPolyline;
					AcGePoint3d pStr, pEnd, pCen, pExt;

					bArc = true;

					pArc->getStartPoint(pStr);
					pArc->getEndPoint(pEnd);
					double dist = pStr.distanceTo(pEnd);
					AcGeVector3d v1;
					v1 = (pStr - pEnd).normal();
					pCen = pEnd + v1 * dist / 2;
					v1.rotateBy(getRadian(90), AcGeVector3d::kZAxis);

					double dist2 = pStr.distanceTo(pArc->center());
					pExt = pArc->center() + v1 * dist2;
					double bulge = pCen.distanceTo(pExt);
					bulge = bulge / (dist / 2);

					AcGePoint2d pStr2d, pEnd2d;
					pStr2d.set(pStr.x, pStr.y);
					pEnd2d.set(pEnd.x, pEnd.y);

					pPLine->addVertexAt(0, pStr2d, bulge);
					pPLine->addVertexAt(1, pEnd2d);
					pPLine->setElevation(pStr.z);
					if (pEnt->isKindOf(AcDbArc::desc())) {
						pArc->erase();
					}

					//선택 지점부터 표기 시작 시 처음점과 끝점 처리
					//AcDbPolyline* pPLine = AcDbPolyline::cast(pEnt);
					double t_dist;
					double pt_dist;
					iVertex = pPLine->numVerts();
					pPLine->getStartPoint(spt);
					pPLine->getEndPoint(ept);
					pPLine->getDistAtPoint(ept, t_dist);
					pPLine->getClosestPointTo(asPnt3d(pt), as_pt);
					pPLine->getDistAtPoint(as_pt, pt_dist);
					//선택 지점부터 표기 시작 시 처음점과 끝점 처리
					if (selectStart == 1)
					{
						// 끝에서 시작
						if ((t_dist / 2) < pt_dist)
						{
							pPLine->getStartPoint(ept);
							pPLine->getEndPoint(spt);
						}
					}
					bTR->appendAcDbEntity(pPLine);
					pPLine->close();
					bTR->close();
				}
				else
				{
					strMsg.LoadStringW(IDS_SEL_CHK_PLINE_LINE); // "선택한 객체는 선, 폴리선, 호가 아닙니다."
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();
			//acedSSFree(en);
			break;
		}
	}
	
	// [+] SUP-29742
	// long	len;
	// [-] END
	/*Adesk::Int32 len;
	if(acedSSLength(ss, &len) != RTNORM || len == 0){
		acutPrintf(_T("\n선택 취소!"));
	}*/
	if (stat == RTNORM) {
		//CString curLayerStr, curTextStr, curTextStyleStr;
		m_cbxStaLayer.GetLBText(m_cbxStaLayer.GetCurSel(), curLayerStr);
		m_cbxTextLayer.GetLBText(m_cbxTextLayer.GetCurSel(), curTextStr);
		m_cbxTextStyle.GetLBText(m_cbxTextStyle.GetCurSel(), curTextStyleStr);

		AcDbTextStyleTable* pTextTbl = NULL;
		AcDbTextStyleTablePointer pTextPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
		AcDbTextStyleTableIterator* pTextTblIter;
		pTextPoint->newIterator(pTextTblIter);
		AcDbObjectId curTextStyleId;
		for(pTextTblIter->start(); !pTextTblIter->done(); pTextTblIter->step()){
			pTextTblIter->getRecordId(curTextStyleId);
			AcDbTextStyleTableRecordPointer pTextTblRcd(curTextStyleId, AcDb::kForRead);
			TCHAR* name;
			pTextTblRcd->getName(name);
			pTextTblRcd->close();
			if(curTextStyleStr.CompareNoCase(name) == 0)		
				break;
		}
		delete pTextTblIter;
		delete pTextTbl;
		//pTextTbl->close();	

		//for(int i = 0; i < len; i++){
		double result = _ttof(m_staStartPoint);
		while(true){
			result = result - _ttof(m_staDist);
			if(result < 0){
				result = -result;
				break;
			}
		}

		double cirlceResult = _ttof(m_staStartPoint);
		while(true){
			cirlceResult = cirlceResult - _ttof(m_staCircleDist);
			if(cirlceResult < 0){
				cirlceResult = -cirlceResult;
				break;
			}
		}

		double textResult = _ttof(m_staStartPoint);
		while(true){
			textResult = textResult - _ttof(m_textDist);
			if(textResult < 0){
				textResult = -textResult;
				break;
			}
		}
		
		if (bArc)
		{ 
			if (acdbEntLast(en) != RTNORM) {
				acdbFail(L"No entities in drawing\n");
				return;
			}
			acdbGetObjectId(objId, en);
		}
		else 
			acdbGetObjectId(objId, en);
			
		AcDbEntityPointer pEnt(objId, AcDb::kForRead);
		if (pEnt->isKindOf(AcDbLine::desc())){
			AcDbLine* pLine = AcDbLine::cast(pEnt);
			Draw(pLine, spt, ept, curTextStyleId);
		}

		else if (pEnt->isKindOf(AcDbPolyline::desc())) {
			AcDbPolyline* pPline = AcDbPolyline::cast(pEnt);
			Draw(pPline, spt, ept, curTextStyleId);
		}

		else if (pEnt->isKindOf(AcDbArc::desc())) {
			AcDbArc* pArc = AcDbArc::cast(pEnt);
			Draw(pArc, spt, ept, curTextStyleId);
		}
	
		pEnt->close();
	}
	acedSSFree(en);
	acedGetAcadDwgView()->Invalidate(false);

	acDocManagerPtr()->unlockDocument(curDoc());

	CAcUiDialog::OnOK();
	this->ShowWindow(SW_SHOW);
	acedGetAcadDwgView()->SetFocus();
}

//취소
void CDSTA::OnBnClickedCancel(){
	DestroyWindow();
	//PostNcDestroy();
	//CAcUiDialog::OnCancel();
}

void CDSTA::OnBnClickedChktext(){
	makeText = IsDlgButtonChecked(IDC_CHKTEXT);
	if(makeText){
		GetDlgItem(IDC_EDTTEXTDIST)->EnableWindow();
		GetDlgItem(IDC_CBXTEXTPOSITION)->EnableWindow();
		GetDlgItem(IDC_EDTTEXTOFFSET)->EnableWindow();
		GetDlgItem(IDC_SPNDECIMALPOINT)->EnableWindow();
		GetDlgItem(IDC_CHKTEXTVISIBLE)->EnableWindow();
		GetDlgItem(IDC_EDTTEXTSIZE)->EnableWindow();
		GetDlgItem(IDC_BTNTEXTSELECT)->EnableWindow();
		GetDlgItem(IDC_CBXTEXTSTYLE)->EnableWindow();
		GetDlgItem(IDC_CBXTEXTLAYER)->EnableWindow();
		GetDlgItem(IDC_BTNTEXTCOLOR)->EnableWindow();
	}
	else{
		GetDlgItem(IDC_EDTTEXTDIST)->EnableWindow(false);
		GetDlgItem(IDC_CBXTEXTPOSITION)->EnableWindow(false);
		GetDlgItem(IDC_EDTTEXTOFFSET)->EnableWindow(false);
		GetDlgItem(IDC_SPNDECIMALPOINT)->EnableWindow(false);
		GetDlgItem(IDC_CHKTEXTVISIBLE)->EnableWindow(false);
		GetDlgItem(IDC_EDTTEXTSIZE)->EnableWindow(false);
		GetDlgItem(IDC_BTNTEXTSELECT)->EnableWindow(false);
		GetDlgItem(IDC_CBXTEXTSTYLE)->EnableWindow(false);
		GetDlgItem(IDC_CBXTEXTLAYER)->EnableWindow(false);
		GetDlgItem(IDC_BTNTEXTCOLOR)->EnableWindow(false);
	}
}

AcDbPolyline* CDSTA::CovertToPLine(AcDbCurve* pObj, AcDbPolyline* pPLine)
{
	AcDbPolyline* pline, pline1;
	if (pObj->isKindOf(AcDbArc::desc())) {
		pline = new AcDbPolyline;
		AcDbArc* pArc(AcDbArc::cast(pObj));
		AcGePoint3d pStr, pEnd, pCen, pExt;
		pArc->getStartPoint(pStr);
		pArc->getEndPoint(pEnd);
		double dist = pStr.distanceTo(pEnd);
		AcGeVector3d v1;
		v1 = (pStr - pEnd).normal();
		pCen = pEnd + v1 * dist / 2;
		v1.rotateBy(getRadian(90), AcGeVector3d::kZAxis);

		double dist2 = pStr.distanceTo(pArc->center());
		pExt = pArc->center() + v1 * dist2;
		double bulge = pCen.distanceTo(pExt);
		bulge = bulge / (dist / 2);

		AcGePoint2d pStr2d, pEnd2d;
		pStr2d.set(pStr.x, pStr.y);
		pEnd2d.set(pEnd.x, pEnd.y);

		pline->addVertexAt(0, pStr2d, bulge);
		pline->addVertexAt(1, pEnd2d);
		pline->setElevation(pStr.z);
		if (pObj->isKindOf(AcDbArc::desc())) {
			pObj->erase();
		}
		/*if (chkBtn) {
			pline->setColorIndex(setColorIndex);
			pline->setLayer(getName);
		}*/
		//ArxUtils::transformToWcs(pline, acdbHostApplicationServices()->workingDatabase());
		//ArxUtils::addToCurrentSpaceAndClose(pline);
	}
	return pline;
}

void CDSTA::Draw(AcDbCurve* pObj, AcGePoint3d spt, AcGePoint3d ept, AcDbObjectId curTextStyleId)
{
	//if (pEnt->isKindOf(AcDbLine::desc()))  // 김성곤 대리 수정
	//{
		//AcDbLine* pLine = AcDbLine::cast(pEnt);

		double totalDist = 0;
		double offsetDist = 0;
		double staDist = _ttof(m_staStartPoint);       //시작 측점
		double pointDist = _ttof(m_staDist);
		double circleDist = _ttof(m_staCircleDist);
		double textDist = _ttof(m_textDist);
		double EndParm = 0.0;
		pObj->getEndParam(EndParm);
		pObj->getDistAtParam(EndParm, totalDist);

		int cnt = totalDist / pointDist;

		int circleCnt = totalDist / circleDist;
		// 원 간격 / 측점 간격의 나머지가 0이 아닐 시 최소공배수
		if (fmod(circleDist, pointDist) != 0)
		{
			circleDist = lcm(circleDist, pointDist);
			circleCnt = totalDist / circleDist;
		}

		int textCnt = totalDist / textDist;
		// 문자 간격 / 측점 간격의 나머지가 0이 아닐 시 최소공배수
		if (fmod(textDist, pointDist) != 0)
		{
			textDist = lcm(textDist, pointDist);
			textCnt = totalDist / textDist;
		}

		AcGePoint3d startVerSp, startVerEp, stmpPt, etmpPt;
		AcGeVector3d startVerVec, endVerVec;
		double DistChk = 0;

		stmpPt = vecTmpPt(pObj, totalDist, spt, stmpPt);
		etmpPt = vecTmpPt(pObj, totalDist, ept, etmpPt);

		BothLineDraw(spt, ept, stmpPt, etmpPt, curLayerStr);

		auto IDStoCString = [](int a_ID) -> CString {
			CString IDS;
			IDS.LoadStringW(a_ID);
			return IDS;
			};

		if (makeCircle)
		{
			BothMakeCircle(spt, ept, curLayerStr);
		}

		if (makeText)
		{
			AcGePoint3d insPt;
			CString textPosition;
			CString textString;
			m_cbxTextPosition.GetLBString(m_cbxTextPosition.GetCurSel(), textPosition);

			AcDbText* insertStartText = new AcDbText();
			AcDbText* insertEndText = new AcDbText();

			// [시작점 문자 작성]
			stmpPt = vecTmpPt(pObj, totalDist, spt, stmpPt);

			AcGeVector3d lineVec1 = (spt - ept).normal();
			AcGeVector3d lineVec2 = (stmpPt - spt).normal();
			AcGePlane pln(AcGePoint3d::kOrigin, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis);
			double rotate;

			startVerVec = (spt - stmpPt).normal();
			startVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);

			if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_TOP)) == 0) {			// "선 위쪽"
				insPt = spt - startVerVec * _ttof(m_textOffset);
			}
			else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_BOTTOM)) == 0) {		// "선 아래쪽"
				insPt = spt + startVerVec * _ttof(m_textOffset);
			}
			else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_VERTICAL)) == 0) {		// "선과 수직"
				// 대상 기능과 맞는 회전으로 변경 (-90 -> 90)
				lineVec2.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
				insPt = spt + lineVec2 * _ttof(m_textOffset);
			}
			else { // 선 수직 아래
				// 대상 기능과 맞는 회전으로 변경 (-90 -> 90)
				lineVec2.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
				insPt = spt + lineVec2 * _ttof(m_textOffset);
			}

			rotate = lineVec2.angleOnPlane(pln);
			//측점 표기방법
			// STA
			if (jobMode == 0)
			{
				int dotCnt = _ttoi(m_decimalPosition);
				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
				{
					textString.Format(_T("STA %d"), int(staDist / 1000));
					textString = textString + L"+";
					textString.Format(textString + L"%%.%df", dotCnt);
					textString.Format(textString, fmod(int(staDist), 1000));
				}
				else // 문자 표기 생략
				{
					if (int(staDist) % 1000 == 0)
					{
						textString.Format(_T("STA %d"), int(staDist / 1000));
						textString = textString + L"+";
					}
					else textString.Empty();
					textString.Format(textString + L"%%.%df", dotCnt);
					textString.Format(textString, fmod(int(staDist), 1000));
				}
			}

			// Number
			else
			{
				//textString.Format(_T("NO 0"));
				int dotCnt = _ttoi(m_decimalPosition);
				textString.Format(_T("NO %d"), int((pointDist + staDist) / pointDist) - 1);
				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
				{
					textString = textString + L"+";
					textString.Format(textString + L"%%.%df", dotCnt);
					textString.Format(textString, fmod((pointDist + staDist), pointDist));
				}
			}

			insertStartText->setTextString(textString);
			insertStartText->setRotation(rotate);
			insertStartText->setHeight(_ttof(m_textSize));
			insertStartText->setLayer(curTextStr);
			if (ChangeTextColor == true)
				insertStartText->setColorIndex(m_selectTextColorIndex);
			insertStartText->setTextStyle(curTextStyleId);

			if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_TOP)) == 0) { // "선 위쪽"
				insertStartText->setHorizontalMode(AcDb::kTextCenter);
				insertStartText->setVerticalMode(AcDb::kTextBottom);
			}
			else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_BOTTOM)) == 0) { // "선 아래쪽"
				insertStartText->setVerticalMode(AcDb::kTextTop);
				insertStartText->setHorizontalMode(AcDb::kTextCenter);
			}
			else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_VERTICAL)) == 0) { // "선과 수직"
				insertStartText->setHorizontalMode(AcDb::kTextLeft);
				insertStartText->setVerticalMode(AcDb::kTextVertMid);
			}
			else { // 선 아래 수직
				insertStartText->setHorizontalMode(AcDb::kTextLeft);
				insertStartText->setVerticalMode(AcDb::kTextVertMid);
			}

			insertStartText->setAlignmentPoint(insPt);

			// [끝점 문자 작성]
			etmpPt = vecTmpPt(pObj, totalDist, ept, etmpPt);

			lineVec2 = (ept - etmpPt).normal();

			endVerVec = (etmpPt - ept).normal();
			endVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);

			if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_TOP)) == 0) { // "선 위쪽"
				insPt = ept - endVerVec * _ttof(m_textOffset);
			}
			else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_BOTTOM)) == 0) { // "선 아래쪽"
				insPt = ept + endVerVec * _ttof(m_textOffset);
			}
			else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_VERTICAL)) == 0) { // "선과 수직"
				lineVec2.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
				insPt = ept + lineVec2 * _ttof(m_textOffset);
			}
			// 대상 기능과 맞는 회전으로 변경 (-90 -> 90)
			else { // 선 아래 수직
				lineVec2.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
				insPt = ept + lineVec2 * _ttof(m_textOffset);
			}
			rotate = lineVec2.angleOnPlane(pln);

			// STA
			if (jobMode == 0) {
				//int num = lineDist / 1000;
				int num = totalDist / 1000;
				int dotCnt = _ttoi(m_decimalPosition);
				//double remainder = lineDist - (num * 1000);

				//textString.Format(_T("STA %d+%.*f"), num, dotCnt, remainder);
				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
				{
					textString.Format(_T("STA %d"), int((staDist + totalDist) / 1000));
					textString = textString + L"+";
					textString.Format(textString + L"%%.%df", dotCnt);
					textString.Format(textString, fmod((staDist + totalDist), 1000));
				}
				else // 문자 표기 생략
				{
					if (int((staDist + totalDist)) % 1000 == 0)
					{
						textString.Format(_T("STA %d"), int((staDist + totalDist) / 1000));
						textString = textString + L"+";
					}
					else textString.Empty();
					textString.Format(textString + L"%%.%df", dotCnt);
					textString.Format(textString, fmod((staDist + totalDist), 1000));
				}
			}

			// Number
			else {
				//int num = lineDist / _ttof(m_textDist);
				int num = totalDist / pointDist;
				int dotCnt = _ttoi(m_decimalPosition);
				/*double remainder = lineDist - (num * _ttof(m_textDist));
				textString.Format(_T("NO %d+%.*f"), num, dotCnt, remainder);*/

				textString.Format(_T("NO %d"), int((totalDist + staDist) / pointDist));
				if (fmod((totalDist + staDist), pointDist) > 0.00000001)
				{
					textString = textString + L"+";
					textString.Format(textString + L"%%.%df", dotCnt);
					textString.Format(textString, fmod((totalDist + staDist), pointDist));
				}
			}

			insertEndText->setTextString(textString);
			insertEndText->setRotation(rotate);
			insertEndText->setHeight(_ttof(m_textSize));
			insertEndText->setLayer(curTextStr);
			if (ChangeTextColor == true)
				insertEndText->setColorIndex(m_selectTextColorIndex);
			insertEndText->setTextStyle(curTextStyleId);

			if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_TOP)) == 0) { // "선 위쪽"
				insertEndText->setHorizontalMode(AcDb::kTextCenter);
				insertEndText->setVerticalMode(AcDb::kTextBottom);
			}
			else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_BOTTOM)) == 0) { // "선 아래쪽"
				insertEndText->setVerticalMode(AcDb::kTextTop);
				insertEndText->setHorizontalMode(AcDb::kTextCenter);
			}
			else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_VERTICAL)) == 0) { // "선과 수직"
				insertEndText->setHorizontalMode(AcDb::kTextLeft);
				insertEndText->setVerticalMode(AcDb::kTextVertMid);
			}
			else { // 선 아래 수직
				insertEndText->setHorizontalMode(AcDb::kTextLeft);
				insertEndText->setVerticalMode(AcDb::kTextVertMid);
			}

			insertEndText->setAlignmentPoint(insPt);

			AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
			acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
			insertStartText->setDatabaseDefaults();
			insertEndText->setDatabaseDefaults();
			bTR->appendAcDbEntity(insertStartText);
			bTR->appendAcDbEntity(insertEndText);
			insertStartText->close();
			insertEndText->close();
			bTR->close();
		}

		double sumNum = _ttof(m_textDist);
		double lineDist = 0, lineDist1;
		double eptDist;
		if (pObj->isKindOf(AcDbArc::desc()))
		{
			(AcDbArc*)pObj->getDistAtPoint(ept, eptDist);
			acutPrintf(L"\n%d , %d\n", ept.x, ept.y);
			acutPrintf(L"\n%f\n", eptDist);
		}
		else
			pObj->getDistAtPoint(ept, eptDist);

		// 객체 길이 < 측점 표시 간격 
		getStartOffset(staDist, pointDist, offsetDist);

		for (int i = 0; i < cnt + 1; i++)
		{
			AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
			// 시작점과 끝점이 반대일 경우 모두 포함하여 측점 거리
			// 총거리 + |끝거리 - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
			lineDist = totalDist - abs(eptDist - (pointDist * (i + 1) - offsetDist));
			// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
			lineDist1 = totalDist - abs((eptDist - 0.0001) - (pointDist * (i + 1) - offsetDist));

			//// 총거리 + |끝거리 - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
			//lineDist = totalDist - abs(eptDist - (pointDist * (i + 1) - fmod(staDist, pointDist)));
			//// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
			//lineDist1 = totalDist - abs((eptDist - 0.0001) - (pointDist * (i + 1) - fmod(staDist, pointDist)));

			// lineDist 가 시작점과 끝점이라면 작성 X
			if (lineDist > 0.00000001 && (totalDist - lineDist) > 0.00000001)
			{
				// 정방향과 반대방향에 대해서 선 작도
				pObj->getDistAtPoint(ept, DistChk);
				pObj->getPointAtDist(lineDist, checkPt);
				//pLine->getPointAtDist(loopDist + 0.0001, tempPt);
				pObj->getPointAtDist(lineDist1, tempPt);
				AcGeVector3d verVec = (checkPt - tempPt).normal();
				AcGeVector3d lineVerVec = (tempPt - checkPt).normal();
				verVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
				vecTempSp = checkPt + verVec * (_ttof(m_staLineLength) / 2.0);
				verTempEp = vecTempSp - verVec * _ttof(m_staLineLength);

				AcDbLine* pDrawLine = new AcDbLine(vecTempSp, verTempEp);
				pDrawLine->setLayer(curLayerStr);
				if (ChangeLayerColor == true)
					pDrawLine->setColorIndex(m_selectLineColorIndex);

				AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
				acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
				pDrawLine->setDatabaseDefaults();
				bTR->appendAcDbEntity(pDrawLine);
				pDrawLine->close();
				bTR->close();

				// 반대 방향 (DistChk == 0 라면)
				if (DistChk < 0.00000001)
					if ((lineDist - pointDist) < 0) break;
				// 정방향 (DistChk == totalDist 라면)
				if (fabs(DistChk - totalDist) < 0.00000001)
				{
					//if (txtDist > totalDist) break;
					if ((lineDist + pointDist) > totalDist) break;
				}
			}
			//loopDist = loopDist + pointDist;		
			/*if (loopDist >= totalDist)
				loopDist = totalDist;*/
		}

		double cirDist, cirDist1;
		// 객체 길이 < 원 표시 간격 
		// offsetDist 에 대입
		getStartOffset(staDist, circleDist, offsetDist);

		if (makeCircle)
		{
			//loopDist = _ttof(m_staCircleDist);
			for (int i = 0; i < circleCnt + 1; i++) {
				AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
				// 총거리 + |끝거리 - (문자 표시 간격 * (i+1) - 나머지(시작 측점, 문자 표시 간격))|
				cirDist = totalDist - abs(eptDist - (circleDist * (i + 1) - offsetDist));
				// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
				cirDist1 = totalDist - abs((eptDist - 0.0001) - (circleDist * (i + 1) - offsetDist));

				// cirDist 가 시작점과 끝점이라면 작성 X
				if (cirDist > 0.00000001 && (totalDist - cirDist) > 0.00000001)
				{
					// 정방향과 반대방향에 대해서 원 작도
					pObj->getDistAtPoint(ept, DistChk);
					pObj->getPointAtDist(cirDist, checkPt);
					pObj->getPointAtDist(cirDist1, tempPt);

					AcGeVector3d verVec = (checkPt - tempPt).normal();
					AcGeVector3d lineVerVec = (tempPt - checkPt).normal();
					verVec.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
					vecTempSp = checkPt + verVec * (_ttof(m_staLineLength) / 2.0);
					verTempEp = vecTempSp - verVec * _ttof(m_staLineLength);

					AcDbCircle* drawCiecle = new AcDbCircle();
					drawCiecle->setCenter(checkPt);
					drawCiecle->setRadius((_ttof(m_staLineLength) / 2.0));
					drawCiecle->setLayer(curLayerStr);
					if (ChangeLayerColor == true)
						drawCiecle->setColorIndex(m_selectLineColorIndex);

					AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
					acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
					drawCiecle->setDatabaseDefaults();
					bTR->appendAcDbEntity(drawCiecle);
					drawCiecle->close();
					bTR->close();

					// 반대 방향 (DistChk == 0 라면)
					if (DistChk < 0.00000001)
						if ((cirDist - circleDist) < 0) break;
					// 정방향 (DistChk == totalDist 라면)
					if (fabs(DistChk - totalDist) < 0.00000001)
					{
						//if (txtDist > totalDist) break;
						if ((cirDist + circleDist) > totalDist) break;
					}

					//loopDist = loopDist + _ttof(m_staCircleDist);
				}
			}
		}

		double txtDist, txtDist1;
		// 객체 길이 < 문자 표시 간격 
		// offsetDist 에 대입
		getStartOffset(staDist, textDist, offsetDist);

		if (makeText)
		{
			//loopDist = _ttof(m_textDist);
			for (int i = 0; i < textCnt + 1; i++) {
				AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
				// 총거리 + |끝거리 - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
				txtDist = totalDist - abs(eptDist - (textDist * (i + 1) - offsetDist));
				// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
				txtDist1 = totalDist - abs((eptDist - 0.0001) - (textDist * (i + 1) - offsetDist));

				// cirDist 가 시작점과 끝점이라면 작성 X
				if (txtDist > 0.00000001 && (totalDist - txtDist) > 0.00000001)
				{
					// 정방향과 반대방향에 대해서 원 작도
					pObj->getDistAtPoint(ept, DistChk);
					pObj->getPointAtDist(txtDist, checkPt);
					pObj->getPointAtDist(txtDist1, tempPt);

					AcGeVector3d verVec = (checkPt - tempPt).normal();
					AcGeVector3d lineVerVec = (tempPt - checkPt).normal();
					verVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
					vecTempSp = checkPt + verVec * (_ttof(m_staLineLength) / 2.0);
					verTempEp = vecTempSp - verVec * _ttof(m_staLineLength);

					CString textPosition;
					CString textString;
					CString textNum;
					m_cbxTextPosition.GetLBString(m_cbxTextPosition.GetCurSel(), textPosition);

					AcGePoint3d insPt;
					if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_TOP)) == 0) { // "선 위쪽"
						insPt = checkPt - verVec * _ttof(m_textOffset);
					}
					else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_BOTTOM)) == 0) { // "선 아래쪽"
						insPt = checkPt + verVec * _ttof(m_textOffset);
					}
					else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_VERTICAL)) == 0) { // "선과 수직"
						lineVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
						insPt = checkPt + lineVerVec * _ttof(m_textOffset);
					}
					else {// 선 아래 수직
						lineVerVec.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
						insPt = checkPt + lineVerVec * _ttof(m_textOffset);
					}

					double rotate;
					AcGePlane pln(AcGePoint3d::kOrigin, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis);
					rotate = lineVerVec.angleOnPlane(pln);
					AcDbText* insertText = new AcDbText();
					if (staDist == 0) txt = i;
					else txt = i + 1;

					// STA
					if (jobMode == 0)
					{
						int num = sumNum / 1000;
						int dotCnt = _ttoi(m_decimalPosition);
						double remainder;
						if (num == 0)
							remainder = sumNum;
						else
							remainder = sumNum - (num * 1000);
						double STAtxt = (textDist * (txt)) / 1000;
						if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
						{
							textString.Format(_T("STA %d"), int(STAtxt));
							textString = textString + L"+";
							textString.Format(textString + L"%%.%df", dotCnt);
							textString.Format(textString, fmod((textDist * (txt + ((int)(staDist / textDist)))), 1000));
							//textString.Format(textString, fmod((textDist* (i + 1)), 1000));
						}
						else // 문자 표기 생략
						{
							if (int((textDist * (txt))) % 1000 == 0)
							{
								textString.Format(_T("STA %d"), int(STAtxt));
								textString = textString + L"+";
							}
							else textString.Empty();
							textString.Format(textString + L"%%.%df", dotCnt);
							textString.Format(textString, fmod((textDist * (txt + ((int)(staDist / textDist)))), 1000));
						}
					}
					// Number
					else
					{	// 실제로 값이 들어가는 구간
						double num = double(txt + (int)(staDist / textDist));
						double chk = 0.0;
						int dotCnt = _ttoi(m_decimalPosition);
						double Numtxt = num * (_ttof(m_textDist) / _ttof(m_staDist));
						// 문자 간격 / 측점 간격 나머지가 0이 아니라면 간격 / 측점 으로 문자열 구하기
						if (fmod(_ttof(m_textDist), _ttof(m_staDist)) != 0)
						{
							if (DistChk == 0) Numtxt = ((totalDist - txtDist) + staDist) / pointDist;
							if (DistChk == totalDist) Numtxt = (txtDist + staDist) / pointDist;
						}
						double remainder = totalDist - (num * _ttof(m_staDist));
						//textString.Format(_T("NO %d+%%.%df"), (int)Numtxt, dotCnt);
						textString.Format(_T("NO %d"), (int)Numtxt);
						if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
						{
							textString = textString + L"+";
							textString.Format(textString + L"%%.%df", dotCnt);
							//textString.Format(textString, fmod(txtDist + staDist, textDist));
							if (DistChk == 0) textString.Format(textString, fmod((totalDist - txtDist) + staDist, textDist));
							if (DistChk == totalDist) textString.Format(textString, fmod((txtDist + staDist), textDist));
						}
					}

					insertText->setTextString(textString);
					insertText->setRotation(rotate);
					insertText->setHeight(_ttof(m_textSize));
					insertText->setLayer(curTextStr);
					if (ChangeTextColor == true)
						insertText->setColorIndex(m_selectTextColorIndex);
					insertText->setTextStyle(curTextStyleId);
					if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_TOP)) == 0) { // "선 위쪽"
						insertText->setHorizontalMode(AcDb::kTextCenter);
						insertText->setVerticalMode(AcDb::kTextBottom);
					}
					else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_BOTTOM)) == 0) { // "선 아래쪽"
						insertText->setVerticalMode(AcDb::kTextTop);
						insertText->setHorizontalMode(AcDb::kTextCenter);
					}
					else if (textPosition.CompareNoCase(IDStoCString(IDS_LINE_VERTICAL)) == 0) { // "선과 수직"
						insertText->setHorizontalMode(AcDb::kTextLeft);
						insertText->setVerticalMode(AcDb::kTextVertMid);
					}
					else { // 선 아래 수직
						insertText->setHorizontalMode(AcDb::kTextLeft);
						insertText->setVerticalMode(AcDb::kTextVertMid);
					}
					insertText->setAlignmentPoint(insPt);

					AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
					acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
					insertText->setDatabaseDefaults();
					bTR->appendAcDbEntity(insertText);
					insertText->close();
					bTR->close();
					sumNum = sumNum + _ttof(m_textDist);

					// 반대 방향 (DistChk == 0 라면)
					if (DistChk < 0.00000001)
						if ((txtDist - textDist) < 0) break;
					// 정방향 (DistChk == totalDist 라면)
					if (fabs(DistChk - totalDist) < 0.00000001)
					{
						//if (txtDist > totalDist) break;
						if ((txtDist + textDist) > totalDist) break;
					}
					//loopDist = loopDist + _ttof(m_textDist);
				}
			}
		}
		pObj->close();
	//}
}

BOOL CDSTA::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	pDSTA = NULL;

	return CAcUiDialog::DestroyWindow();
}

void CDSTA::OnBnClickedDstaHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/DSTA.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}

//	if(pEnt->isKindOf(AcDbLine::desc()))  // 김성곤 대리 수정
	//	{
	//		AcDbLine* pLine = AcDbLine::cast(pEnt);

	//		double totalDist = 0;
	//		double offsetDist = 0;
	//		double staDist = _ttof(m_staStartPoint);       //시작 측점
	//		double pointDist = _ttof(m_staDist);
	//		double circleDist = _ttof(m_staCircleDist);
	//		double textDist = _ttof(m_textDist);
	//		double EndParm = 0.0;
	//		pLine->getEndParam(EndParm);
	//		pLine->getDistAtParam(EndParm, totalDist);

	//		int cnt = totalDist / pointDist;

	//		int circleCnt = totalDist / circleDist;
	//		// 원 간격 / 측점 간격의 나머지가 0이 아닐 시 최소공배수
	//		if (fmod(circleDist, pointDist) != 0)
	//		{
	//			circleDist = lcm(circleDist, pointDist);
	//			circleCnt = totalDist / circleDist;
	//		}

	//		int textCnt = totalDist / textDist;
	//		// 문자 간격 / 측점 간격의 나머지가 0이 아닐 시 최소공배수
	//		if (fmod(textDist, pointDist) != 0)
	//		{
	//			textDist = lcm(textDist, pointDist);
	//			textCnt = totalDist / textDist;
	//		}

	//		AcGePoint3d startVerSp, startVerEp, stmpPt, etmpPt;
	//		AcGeVector3d startVerVec, endVerVec;
	//		double DistChk = 0;

	//		stmpPt = vecTmpPt(pLine, totalDist, spt, stmpPt);
	//		etmpPt = vecTmpPt(pLine, totalDist, ept, etmpPt);

	//		BothLineDraw(spt, ept, stmpPt, etmpPt, curLayerStr);

	//		/*AcGeVector3d vec = (pLine->endPoint() - pLine->startPoint()).normal();
	//		vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//		AcGeVector3d vec2 = (pLine->startPoint() - pLine->endPoint()).normal();
	//		double lineDist = pLine->endPoint().distanceTo(pLine->startPoint());

	//		AcGePoint3d cenPt, insPt, endPt;
	//		insPt = pLine->startPoint() + vec * (_ttof(m_staLineLength) / 2.0);
	//		endPt = insPt - vec * _ttof(m_staLineLength);

	//		AcDbLine* drawStartLine = new AcDbLine(insPt, endPt);
	//		drawStartLine->setLayer(curLayerStr);
	//		if(ChangeLayerColor == true)
	//			drawStartLine->setColorIndex(m_selectLineColorIndex);

	//		insPt = pLine->endPoint() + vec * (_ttof(m_staLineLength) / 2.0);
	//		endPt = insPt - vec * _ttof(m_staLineLength);

	//		AcDbLine* drawEndLine = new AcDbLine(insPt, endPt);
	//		drawEndLine->setLayer(curLayerStr);
	//		if(ChangeLayerColor == true)
	//			drawEndLine->setColorIndex(m_selectLineColorIndex);

	//		AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//		acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//		drawStartLine->setDatabaseDefaults();
	//		drawEndLine->setDatabaseDefaults();
	//		bTR->appendAcDbEntity(drawStartLine);
	//		bTR->appendAcDbEntity(drawEndLine);
	//		drawStartLine->close();
	//		drawEndLine->close();
	//		bTR->close();

	//		cenPt = pLine->startPoint() - vec2 * result;
	//		insPt = cenPt + vec * (_ttof(m_staLineLength) / 2.0);
	//		endPt = insPt - vec * _ttof(m_staLineLength);
	//		int cnt = (lineDist - result) / _ttof(m_staDist);
	//		for(int i = 0; i < cnt + 1; i++){
	//			AcDbLine* drawLine = new AcDbLine();
	//			drawLine->setStartPoint(insPt);
	//			drawLine->setEndPoint(endPt);
	//			drawLine->setLayer(curLayerStr);
	//			if(ChangeLayerColor == true)
	//				drawLine->setColorIndex(m_selectLineColorIndex);

	//			AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//			acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//			drawLine->setDatabaseDefaults();
	//			bTR->appendAcDbEntity(drawLine);
	//			drawLine->close();
	//			bTR->close();

	//			insPt = insPt - vec2 * _ttof(m_staDist);
	//			endPt = insPt - vec * _ttof(m_staLineLength);
	//		}
	//		*/
	//		if(makeCircle)
	//		{
	//			BothMakeCircle(spt, ept, curLayerStr);

	//			/*cenPt = pLine->startPoint();
	//			AcDbCircle* drawStartCircle = new AcDbCircle();
	//			drawStartCircle->setCenter(cenPt);
	//			drawStartCircle->setRadius((_ttof(m_staLineLength) / 2.0));
	//			drawStartCircle->setLayer(curLayerStr);
	//			if(ChangeLayerColor == true)
	//				drawStartCircle->setColorIndex(m_selectLineColorIndex);

	//			cenPt = pLine->endPoint();
	//			AcDbCircle* drawEndCircle = new AcDbCircle();
	//			drawEndCircle->setCenter(cenPt);
	//			drawEndCircle->setRadius((_ttof(m_staLineLength) / 2.0));
	//			drawEndCircle->setLayer(curLayerStr);
	//			if(ChangeLayerColor == true)
	//				drawEndCircle->setColorIndex(m_selectLineColorIndex);

	//			int cirlceCnt = lineDist / _ttof(m_staCircleDist);
	//			cenPt = pLine->startPoint() - vec2 * cirlceResult;
	//			for(int i = 0; i < cirlceCnt; i++){
	//				AcDbCircle* drawCiecle = new AcDbCircle();
	//				drawCiecle->setCenter(cenPt);
	//				drawCiecle->setRadius((_ttof(m_staLineLength) / 2.0));
	//				drawCiecle->setLayer(curLayerStr);
	//				if(ChangeLayerColor == true)
	//					drawCiecle->setColorIndex(m_selectLineColorIndex);

	//				bTR = new AcDbBlockTableRecord();
	//				acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//				drawCiecle->setDatabaseDefaults();
	//				bTR->appendAcDbEntity(drawCiecle);
	//				drawCiecle->close();
	//				bTR->close();
	//				cenPt = cenPt - vec2 * _ttof(m_staCircleDist);
	//			}

	//			bTR = new AcDbBlockTableRecord();
	//			acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);				
	//			drawStartCircle->setDatabaseDefaults();
	//			drawEndCircle->setDatabaseDefaults();
	//			bTR->appendAcDbEntity(drawStartCircle);
	//			bTR->appendAcDbEntity(drawEndCircle);
	//			drawStartCircle->close();
	//			drawEndCircle->close();
	//			bTR->close();*/
	//		}

	//		if(makeText)
	//		{
	//			AcGePoint3d insPt;
	//			CString textPosition;
	//			CString textString;
	//			m_cbxTextPosition.GetLBString(m_cbxTextPosition.GetCurSel(), textPosition);

	//			AcDbText* insertStartText = new AcDbText();
	//			AcDbText* insertEndText= new AcDbText();

	//			// [시작점 문자 작성]
	//			stmpPt = vecTmpPt(pLine, totalDist, spt, stmpPt);

	//			//AcGeVector3d lineVec1 = (pLine->startPoint() - pLine->endPoint()).normal();
	//			//AcGeVector3d lineVec2 = (pLine->endPoint() - pLine->startPoint()).normal();
	//			AcGeVector3d lineVec1 = (spt - ept).normal();
	//			AcGeVector3d lineVec2 = (stmpPt - spt).normal();
	//			AcGePlane pln(AcGePoint3d::kOrigin, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis);
	//			double rotate;

	//			startVerVec = (spt - stmpPt).normal();
	//			startVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);

	//			if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){
	//				insPt = spt - startVerVec * _ttof(m_textOffset);
	//				/*insPt = pLine->startPoint() + vec * _ttof(m_textOffset);
	//				insertStartText->setHorizontalMode(AcDb::kTextCenter);
	//				insertStartText->setVerticalMode(AcDb::kTextBottom);*/
	//			}
	//			else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){
	//				insPt = spt + startVerVec * _ttof(m_textOffset);
	//				/*insPt = pLine->startPoint() - vec * _ttof(m_textOffset);
	//				insertStartText->setHorizontalMode(AcDb::kTextCenter);
	//				insertStartText->setVerticalMode(AcDb::kTextTop);*/
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//				// 대상 기능과 맞는 회전으로 변경 (-90 -> 90)
	//				lineVec2.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = spt + lineVec2 * _ttof(m_textOffset);
	//				/*insPt = pLine->startPoint() - vec * _ttof(m_textOffset);
	//				insertStartText->setHorizontalMode(AcDb::kTextCenter);
	//				insertStartText->setVerticalMode(AcDb::kTextTop);*/
	//			}
	//			else{ // 선 수직 아래
	//				// 대상 기능과 맞는 회전으로 변경 (-90 -> 90)
	//				lineVec2.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = spt + lineVec2 * _ttof(m_textOffset);
	//				/*insPt = pLine->startPoint() + lineVec2 * _ttof(m_textOffset);
	//				insertStartText->setHorizontalMode(AcDb::kTextLeft);
	//				insertStartText->setVerticalMode(AcDb::kTextVertMid);*/
	//			}

	//			rotate = lineVec2.angleOnPlane(pln);
	//			//측점 표기방법
	//			// STA
	//			if(jobMode == 0)
	//			{
	//				//int dotCnt = _ttoi(m_decimalPosition); // 김성곤 대리
	//				//int num = _ttof(m_staStartPoint) / 1000;
	//				//double remainder = _ttof(m_staStartPoint) - (num * 1000);
	//				//textString.Format(_T("STA 0+000"));
	//				int dotCnt = _ttoi(m_decimalPosition);
	//				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//				{
	//					textString.Format(_T("STA %d"), int(staDist / 1000));
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod(int(staDist), 1000));
	//				}
	//				else // 문자 표기 생략
	//				{
	//					if (int(staDist) % 1000 == 0)
	//					{ 
	//						textString.Format(_T("STA %d"), int(staDist / 1000));
	//						textString = textString + "+";
	//					}
	//					else textString.Empty();
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod(int(staDist), 1000));
	//				}
	//			}

	//			// Number
	//			else
	//			{
	//				//textString.Format(_T("NO 0"));
	//				int dotCnt = _ttoi(m_decimalPosition);
	//				textString.Format(_T("NO %d"), int((pointDist + staDist) / pointDist) - 1);
	//				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//				{
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((pointDist + staDist), pointDist));
	//				}
	//			}

	//			insertStartText->setTextString(textString);
	//			insertStartText->setRotation(rotate);
	//			insertStartText->setHeight(_ttof(m_textSize));
	//			insertStartText->setLayer(curTextStr);
	//			if(ChangeTextColor == true)
	//				insertStartText->setColorIndex(m_selectTextColorIndex);
	//			insertStartText->setTextStyle(curTextStyleId);

	//			if (textPosition.CompareNoCase(_T("선 위쪽")) == 0) {
	//				insertStartText->setHorizontalMode(AcDb::kTextCenter);
	//				insertStartText->setVerticalMode(AcDb::kTextBottom);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선 아래쪽")) == 0) {
	//				insertStartText->setVerticalMode(AcDb::kTextTop);
	//				insertStartText->setHorizontalMode(AcDb::kTextCenter);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0){
	//				insertStartText->setHorizontalMode(AcDb::kTextLeft);
	//				insertStartText->setVerticalMode(AcDb::kTextVertMid);
	//			}
	//			else { // 선 아래 수직
	//				insertStartText->setHorizontalMode(AcDb::kTextLeft);
	//				insertStartText->setVerticalMode(AcDb::kTextVertMid);
	//			}

	//			insertStartText->setAlignmentPoint(insPt);

	//			// [끝점 문자 작성]
	//			etmpPt = vecTmpPt(pLine, totalDist, ept, etmpPt);

	//			lineVec2 = (ept - etmpPt).normal();

	//			endVerVec = (etmpPt - ept).normal();
	//			endVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);

	//			if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){	
	//				insPt = ept - endVerVec * _ttof(m_textOffset);
	//				//insPt = pLine->endPoint() + vec * _ttof(m_textOffset);
	//				//insertEndText->setHorizontalMode(AcDb::kTextCenter);
	//				//insertEndText->setVerticalMode(AcDb::kTextBottom);
	//			}
	//			else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){
	//				insPt = ept + endVerVec * _ttof(m_textOffset);
	//				//insPt = pLine->endPoint() - vec * _ttof(m_textOffset);
	//				//insertEndText->setHorizontalMode(AcDb::kTextCenter);
	//				//insertEndText->setVerticalMode(AcDb::kTextTop);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//				lineVec2.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = ept + lineVec2 * _ttof(m_textOffset);
	//			}
	//			// 대상 기능과 맞는 회전으로 변경 (-90 -> 90)
	//			else{ // 선 아래 수직
	//				lineVec2.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = ept + lineVec2 * _ttof(m_textOffset);
	//				//insPt = pLine->endPoint() + lineVec2 * _ttof(m_textOffset);
	//				//insertEndText->setHorizontalMode(AcDb::kTextLeft);
	//				//insertEndText->setVerticalMode(AcDb::kTextVertMid);
	//			}
	//			rotate = lineVec2.angleOnPlane(pln);

	//			// STA
	//			if(jobMode == 0){
	//				//int num = lineDist / 1000;
	//				int num = totalDist / 1000;
	//				int dotCnt = _ttoi(m_decimalPosition);
	//				//double remainder = lineDist - (num * 1000);
	//				
	//				//textString.Format(_T("STA %d+%.*f"), num, dotCnt, remainder);
	//				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//				{
	//					textString.Format(_T("STA %d"), int((staDist + totalDist) / 1000));
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((staDist + totalDist), 1000));
	//				}
	//				else // 문자 표기 생략
	//				{
	//					if (int((staDist + totalDist)) % 1000 == 0)
	//					{
	//						textString.Format(_T("STA %d"), int((staDist + totalDist) / 1000));
	//						textString = textString + "+";
	//					}
	//					else textString.Empty();
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((staDist + totalDist), 1000));
	//				}

	//				/*textString.Format(_T("STA %d"), int((staDist + totalDist) / 1000));
	//				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//				{
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((staDist + totalDist), 1000));
	//				}*/
	//			}

	//			// Number
	//			else{
	//				//int num = lineDist / _ttof(m_textDist);
	//				int num = totalDist / pointDist;
	//				int dotCnt = _ttoi(m_decimalPosition);
	//				/*double remainder = lineDist - (num * _ttof(m_textDist));
	//				textString.Format(_T("NO %d+%.*f"), num, dotCnt, remainder);*/

	//				textString.Format(_T("NO %d"), int((totalDist + staDist) / pointDist));
	//				if (fmod((totalDist + staDist), pointDist) > 0.00000001)
	//				{
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((totalDist + staDist), pointDist));
	//				}
	//			}

	//			insertEndText->setTextString(textString);
	//			insertEndText->setRotation(rotate);
	//			insertEndText->setHeight(_ttof(m_textSize));
	//			insertEndText->setLayer(curTextStr);
	//			if(ChangeTextColor == true)
	//				insertEndText->setColorIndex(m_selectTextColorIndex);
	//			insertEndText->setTextStyle(curTextStyleId);

	//			if (textPosition.CompareNoCase(_T("선 위쪽")) == 0) {
	//				insertEndText->setHorizontalMode(AcDb::kTextCenter);
	//				insertEndText->setVerticalMode(AcDb::kTextBottom);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선 아래쪽")) == 0) {
	//				insertEndText->setVerticalMode(AcDb::kTextTop);
	//				insertEndText->setHorizontalMode(AcDb::kTextCenter);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//				insertEndText->setHorizontalMode(AcDb::kTextLeft);
	//				insertEndText->setVerticalMode(AcDb::kTextVertMid);
	//			}
	//			else { // 선 아래 수직
	//				insertEndText->setHorizontalMode(AcDb::kTextLeft);
	//				insertEndText->setVerticalMode(AcDb::kTextVertMid);
	//			}

	//			insertEndText->setAlignmentPoint(insPt);

	//			AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//			acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//			insertStartText->setDatabaseDefaults();
	//			insertEndText->setDatabaseDefaults();
	//			bTR->appendAcDbEntity(insertStartText);
	//			bTR->appendAcDbEntity(insertEndText);
	//			insertStartText->close();
	//			insertEndText->close();
	//			bTR->close();

	//			/*if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){
	//				insPt = pLine->startPoint() + vec * _ttof(m_textOffset);
	//			}
	//			else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){
	//				insPt = pLine->startPoint() - vec * _ttof(m_textOffset);
	//			}
	//			else{					
	//				insPt = pLine->startPoint() - vec * _ttof(m_textOffset);
	//			}

	//			insPt = insPt - vec2 * textResult;
	//			int textCnt = lineDist / _ttof(m_textDist);
	//			double sumNum = _ttof(m_textDist);
	//			for(int i = 0; i < textCnt; i++){
	//				AcDbText* insertText = new AcDbText();
	//				if(jobMode == 0){
	//					int num = sumNum / 1000;
	//					double remainder;
	//					if(num == 0)
	//						remainder = sumNum;
	//					else
	//						remainder = sumNum - (num * 1000);

	//					textString.Format(_T("STA %d+%g"), num, remainder);
	//				}
	//				else{
	//					int num = i + 1;
	//					double remainder = lineDist - (num * _ttof(m_staDist));
	//					textString.Format(_T("NO %d"), num);
	//				}

	//				insertText->setTextString(textString);
	//				insertText->setRotation(rotate);
	//				insertText->setHeight(_ttof(m_textSize));
	//				insertText->setLayer(curTextStr);
	//				if(ChangeTextColor == true)
	//					insertText->setColorIndex(m_selectTextColorIndex);
	//				insertText->setTextStyle(curTextStyleId);
	//				if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){	
	//					insertText->setHorizontalMode(AcDb::kTextCenter);
	//					insertText->setVerticalMode(AcDb::kTextBottom);
	//				}
	//				else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){
	//					insertText->setHorizontalMode(AcDb::kTextCenter);
	//					insertText->setVerticalMode(AcDb::kTextTop);
	//				}
	//				else{
	//					insertText->setHorizontalMode(AcDb::kTextLeft);
	//					insertText->setVerticalMode(AcDb::kTextVertMid);
	//				}
	//				insertText->setAlignmentPoint(insPt);

	//				bTR = new AcDbBlockTableRecord();
	//				acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//				insertText->setDatabaseDefaults();
	//				bTR->appendAcDbEntity(insertText);
	//				insertText->close();
	//				bTR->close();
	//				insPt = insPt - vec2 * _ttof(m_textDist);
	//				sumNum = sumNum + _ttof(m_textDist);*/
	//		}

	//		double sumNum = _ttof(m_textDist);
	//		double lineDist = 0, lineDist1;
	//		double eptDist;
	//		pLine->getDistAtPoint(ept, eptDist);

	//		// 객체 길이 < 측점 표시 간격 
	//		getStartOffset(staDist, pointDist, offsetDist);

	//		for (int i = 0; i < cnt + 1; i++)
	//		{
	//			AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
	//			// 시작점과 끝점이 반대일 경우 모두 포함하여 측점 거리
	//			// 총거리 + |끝거리 - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//			lineDist = totalDist - abs(eptDist - (pointDist * (i + 1) - offsetDist));
	//			// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//			lineDist1 = totalDist - abs((eptDist - 0.0001) - (pointDist * (i + 1) - offsetDist));

	//			//// 총거리 + |끝거리 - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//			//lineDist = totalDist - abs(eptDist - (pointDist * (i + 1) - fmod(staDist, pointDist)));
	//			//// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//			//lineDist1 = totalDist - abs((eptDist - 0.0001) - (pointDist * (i + 1) - fmod(staDist, pointDist)));

	//			// lineDist 가 시작점과 끝점이라면 작성 X
	//			if (lineDist > 0.00000001 && (totalDist - lineDist) > 0.00000001)
	//			{
	//				// 정방향과 반대방향에 대해서 선 작도
	//				pLine->getDistAtPoint(ept, DistChk);
	//				pLine->getPointAtDist(lineDist, checkPt);
	//				//pLine->getPointAtDist(loopDist + 0.0001, tempPt);
	//				pLine->getPointAtDist(lineDist1, tempPt);
	//				AcGeVector3d verVec = (checkPt - tempPt).normal();
	//				AcGeVector3d lineVerVec = (tempPt - checkPt).normal();
	//				verVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				vecTempSp = checkPt + verVec * (_ttof(m_staLineLength) / 2.0);
	//				verTempEp = vecTempSp - verVec * _ttof(m_staLineLength);

	//				AcDbLine* pDrawLine = new AcDbLine(vecTempSp, verTempEp);
	//				pDrawLine->setLayer(curLayerStr);
	//				if (ChangeLayerColor == true)
	//					pDrawLine->setColorIndex(m_selectLineColorIndex);

	//				AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//				acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//				pDrawLine->setDatabaseDefaults();
	//				bTR->appendAcDbEntity(pDrawLine);
	//				pDrawLine->close();
	//				bTR->close();

	//				// 반대 방향 (DistChk == 0 라면)
	//				if (DistChk < 0.00000001)
	//					if ((lineDist - pointDist) < 0) break;
	//				// 정방향 (DistChk == totalDist 라면)
	//				if (fabs(DistChk - totalDist) < 0.00000001)
	//				{
	//					//if (txtDist > totalDist) break;
	//					if ((lineDist + pointDist) > totalDist) break;
	//				}
	//			}
	//			//loopDist = loopDist + pointDist;		
	//			/*if (loopDist >= totalDist)
	//				loopDist = totalDist;*/
	//		}

	//		double cirDist, cirDist1;
	//		// 객체 길이 < 원 표시 간격 
	//		// offsetDist 에 대입
	//		getStartOffset(staDist, circleDist, offsetDist);

	//		if (makeCircle)
	//		{
	//			//loopDist = _ttof(m_staCircleDist);
	//			for (int i = 0; i < circleCnt + 1; i++) {
	//				AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
	//				// 총거리 + |끝거리 - (문자 표시 간격 * (i+1) - 나머지(시작 측점, 문자 표시 간격))|
	//				cirDist = totalDist - abs(eptDist - (circleDist * (i + 1) - offsetDist));
	//				// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//				cirDist1 = totalDist - abs((eptDist - 0.0001) - (circleDist * (i + 1) - offsetDist));
	//				
	//				// cirDist 가 시작점과 끝점이라면 작성 X
	//				if (cirDist > 0.00000001 && (totalDist - cirDist) > 0.00000001)
	//				{
	//					// 정방향과 반대방향에 대해서 원 작도
	//					pLine->getDistAtPoint(ept, DistChk);
	//					pLine->getPointAtDist(cirDist, checkPt);
	//					pLine->getPointAtDist(cirDist1, tempPt);

	//					AcGeVector3d verVec = (checkPt - tempPt).normal();
	//					AcGeVector3d lineVerVec = (tempPt - checkPt).normal();
	//					verVec.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//					vecTempSp = checkPt + verVec * (_ttof(m_staLineLength) / 2.0);
	//					verTempEp = vecTempSp - verVec * _ttof(m_staLineLength);

	//					AcDbCircle* drawCiecle = new AcDbCircle();
	//					drawCiecle->setCenter(checkPt);
	//					drawCiecle->setRadius((_ttof(m_staLineLength) / 2.0));
	//					drawCiecle->setLayer(curLayerStr);
	//					if (ChangeLayerColor == true)
	//						drawCiecle->setColorIndex(m_selectLineColorIndex);

	//					AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//					acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//					drawCiecle->setDatabaseDefaults();
	//					bTR->appendAcDbEntity(drawCiecle);
	//					drawCiecle->close();
	//					bTR->close();

	//					// 반대 방향 (DistChk == 0 라면)
	//					if (DistChk < 0.00000001)
	//						if ((cirDist - circleDist) < 0) break;
	//					// 정방향 (DistChk == totalDist 라면)
	//					if (fabs(DistChk - totalDist) < 0.00000001)
	//					{
	//						//if (txtDist > totalDist) break;
	//						if ((cirDist + circleDist) > totalDist) break;
	//					}

	//					//loopDist = loopDist + _ttof(m_staCircleDist);
	//				}
	//			}
	//		}

	//		double txtDist, txtDist1;
	//		// 객체 길이 < 문자 표시 간격 
	//		// offsetDist 에 대입
	//		getStartOffset(staDist, textDist, offsetDist);

	//		if (makeText)
	//		{
	//			//loopDist = _ttof(m_textDist);
	//			for (int i = 0; i < textCnt + 1; i++) {
	//				AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
	//				// 총거리 + |끝거리 - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//				txtDist = totalDist - abs(eptDist - (textDist * (i + 1) - offsetDist));
	//				// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//				txtDist1 = totalDist - abs((eptDist - 0.0001) - (textDist * (i + 1) - offsetDist));
	//				
	//				// cirDist 가 시작점과 끝점이라면 작성 X
	//				if (txtDist > 0.00000001 && (totalDist - txtDist) > 0.00000001)
	//				{
	//					// 정방향과 반대방향에 대해서 원 작도
	//					pLine->getDistAtPoint(ept, DistChk);
	//					pLine->getPointAtDist(txtDist, checkPt);
	//					pLine->getPointAtDist(txtDist1, tempPt);

	//					AcGeVector3d verVec = (checkPt - tempPt).normal();
	//					AcGeVector3d lineVerVec = (tempPt - checkPt).normal();
	//					verVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//					vecTempSp = checkPt + verVec * (_ttof(m_staLineLength) / 2.0);
	//					verTempEp = vecTempSp - verVec * _ttof(m_staLineLength);

	//					CString textPosition;
	//					CString textString;
	//					CString textNum;
	//					m_cbxTextPosition.GetLBString(m_cbxTextPosition.GetCurSel(), textPosition);

	//					AcGePoint3d insPt;
	//					if (textPosition.CompareNoCase(_T("선 위쪽")) == 0) {
	//						insPt = checkPt - verVec * _ttof(m_textOffset);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선 아래쪽")) == 0) {
	//						insPt = checkPt + verVec * _ttof(m_textOffset);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//						lineVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//						insPt = checkPt + lineVerVec * _ttof(m_textOffset);
	//					}
	//					else {// 선 아래 수직
	//						lineVerVec.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//						insPt = checkPt + lineVerVec * _ttof(m_textOffset);
	//					}

	//					double rotate;
	//					AcGePlane pln(AcGePoint3d::kOrigin, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis);
	//					rotate = lineVerVec.angleOnPlane(pln);
	//					AcDbText* insertText = new AcDbText();
	//					
	//					if (staDist == 0) txt = i;
	//					else txt = (i + 1);
	//					// STA
	//					if (jobMode == 0)
	//					{
	//						int num = sumNum / 1000;
	//						int dotCnt = _ttoi(m_decimalPosition);
	//						double remainder;
	//						if (num == 0)
	//							remainder = sumNum;
	//						else
	//							remainder = sumNum - (num * 1000);
	//						double STAtxt = (textDist * (txt)) / 1000;
	//						if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//						{
	//							textString.Format(_T("STA %d"), int(STAtxt));
	//							textString = textString + "+";
	//							textString.Format(textString + L"%%.%df", dotCnt);
	//							textString.Format(textString, fmod((textDist * (txt + ((int)(staDist / textDist)))), 1000));
	//							//textString.Format(textString, fmod((textDist* (i + 1)), 1000));
	//						}
	//						else // 문자 표기 생략
	//						{
	//							if (int((textDist * (txt))) % 1000 == 0)
	//							{
	//								textString.Format(_T("STA %d"), int(STAtxt));
	//								textString = textString + "+";
	//							}
	//							else textString.Empty();
	//							textString.Format(textString + L"%%.%df", dotCnt);
	//							textString.Format(textString, fmod((textDist * (txt + ((int)(staDist / textDist)))), 1000));
	//						}

	//						/*textString.Format(_T("STA %d"), int(STAtxt));
	//						if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//						{
	//							textString = textString + "+";
	//							textString.Format(textString + L"%%.%df", dotCnt);
	//							textString.Format(textString, fmod((textDist * (i + 1)), 1000));
	//						}*/
	//					}
	//					// Number
	//					else
	//					{	// 실제로 값이 들어가는 구간
	//						double num = double(txt);
	//						double chk = 0.0;
	//						int dotCnt = _ttoi(m_decimalPosition);
	//						double Numtxt = num * (_ttof(m_textDist) / _ttof(m_staDist));
	//						// 문자 간격 / 측점 간격 나머지가 0이 아니라면 간격 / 측점 으로 문자열 구하기
	//						if (fmod(_ttof(m_textDist), _ttof(m_staDist)) != 0)
	//						{
	//							if (DistChk == 0) Numtxt = ((totalDist - txtDist) + staDist) / pointDist;
	//							if (DistChk == totalDist) Numtxt = (txtDist + staDist) / pointDist;
	//						}
	//						double remainder = totalDist - (num * _ttof(m_staDist));
	//						//textString.Format(_T("NO %d+%%.%df"), (int)Numtxt, dotCnt);
	//						textString.Format(_T("NO %d"), (int)Numtxt);
	//						if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//						{
	//							textString = textString + "+";
	//							textString.Format(textString + L"%%.%df", dotCnt);
	//							//textString.Format(textString, fmod(txtDist + staDist, textDist));
	//							if (DistChk == 0) textString.Format(textString, fmod((totalDist - txtDist) + staDist, textDist));
	//							if (DistChk == totalDist) textString.Format(textString, fmod((txtDist + staDist), textDist));
	//						}
	//					}

	//					insertText->setTextString(textString);
	//					insertText->setRotation(rotate);
	//					insertText->setHeight(_ttof(m_textSize));
	//					insertText->setLayer(curTextStr);
	//					if (ChangeTextColor == true)
	//						insertText->setColorIndex(m_selectTextColorIndex);
	//					insertText->setTextStyle(curTextStyleId);
	//					if (textPosition.CompareNoCase(_T("선 위쪽")) == 0) {
	//						insertText->setHorizontalMode(AcDb::kTextCenter);
	//						insertText->setVerticalMode(AcDb::kTextBottom);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선 아래쪽")) == 0) {
	//						insertText->setVerticalMode(AcDb::kTextTop);
	//						insertText->setHorizontalMode(AcDb::kTextCenter);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//						insertText->setHorizontalMode(AcDb::kTextLeft);
	//						insertText->setVerticalMode(AcDb::kTextVertMid);
	//					}
	//					else { // 선 아래 수직
	//						insertText->setHorizontalMode(AcDb::kTextLeft);
	//						insertText->setVerticalMode(AcDb::kTextVertMid);
	//					}
	//					insertText->setAlignmentPoint(insPt);

	//					AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//					acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//					insertText->setDatabaseDefaults();
	//					bTR->appendAcDbEntity(insertText);
	//					insertText->close();
	//					bTR->close();
	//					sumNum = sumNum + _ttof(m_textDist);

	//					// 반대 방향 (DistChk == 0 라면)
	//					if (DistChk < 0.00000001)
	//						if ((txtDist - textDist) < 0) break;
	//					// 정방향 (DistChk == totalDist 라면)
	//					if (fabs(DistChk - totalDist) < 0.00000001)
	//					{
	//						//if (txtDist > totalDist) break;
	//						if ((txtDist + textDist) > totalDist) break;
	//					}
	//					//loopDist = loopDist + _ttof(m_textDist);
	//				}
	//			}
	//		}
	//		pLine->close();
	//	}
	//	else if(pEnt->isKindOf(AcDbPolyline::desc()))	// 폴리라인인 경우
	//	{
	//		AcDbPolyline* pPline = AcDbPolyline::cast(pEnt);
	//		//AcGePoint3d lineSp, lineEp;

	//		double totalDist = 0;
	//		double offsetDist = 0;
	//		double staDist = _ttof(m_staStartPoint);       //시작 측점
	//		double pointDist = _ttof(m_staDist);
	//		double circleDist = _ttof(m_staCircleDist);
	//		double textDist = _ttof(m_textDist);
	//		double EndParm = 0.0;
	//		pPline->getEndParam(EndParm);
	//		pPline->getDistAtParam(EndParm, totalDist);
	//		/*
	//		for(int i = 0; i < pPline->numVerts() - 1; i++){
	//			AcGePoint3d verSp, verEp;
	//			pPline->getPointAt(i, verSp);
	//			pPline->getPointAt(i + 1, verEp);

	//			double verDist = verSp.distanceTo(verEp);
	//			totalDist = totalDist + verDist;
	//		}
	//		*/
	//		int cnt = totalDist / pointDist;

	//		int circleCnt = totalDist / circleDist;
	//		// 원 간격 / 측점 간격의 나머지가 0이 아닐 시 최소공배수
	//		if (fmod(circleDist, pointDist) != 0)
	//		{
	//			circleDist = lcm(circleDist, pointDist);
	//			circleCnt = totalDist / circleDist;
	//		}

	//		int textCnt = totalDist / textDist;
	//		// 문자 간격 / 측점 간격의 나머지가 0이 아닐 시 최소공배수
	//		if (fmod(textDist, pointDist) != 0)
	//		{
	//			textDist = lcm(textDist, pointDist);
	//			textCnt = totalDist / textDist;
	//		}

	//		AcGePoint3d startVerSp, startVerEp, stmpPt, etmpPt;
	//		AcGeVector3d startVerVec, endVerVec;
	//		double DistChk = 0;

	//		stmpPt = vecTmpPt(pPline, totalDist, spt, stmpPt);
	//		etmpPt = vecTmpPt(pPline, totalDist, ept, etmpPt);

	//		BothLineDraw(spt, ept, stmpPt, etmpPt, curLayerStr);

	//		if(makeCircle)
	//		{
	//			BothMakeCircle(spt, ept, curLayerStr);
	//		}

	//		if(makeText)
	//		{
	//			AcGePoint3d insPt;
	//			CString textPosition;
	//			CString textString;
	//			m_cbxTextPosition.GetLBString(m_cbxTextPosition.GetCurSel(), textPosition);

	//			AcDbText* insertStartText = new AcDbText();
	//			AcDbText* insertEndText= new AcDbText();

	//			// [시작점 문자 작성]
	//			stmpPt = vecTmpPt(pPline, totalDist, spt, stmpPt);

	//			AcGeVector3d lineVec1 = (spt - ept).normal();
	//			AcGeVector3d lineVec2 = (stmpPt - spt).normal();
	//			AcGePlane pln(AcGePoint3d::kOrigin, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis);
	//			double rotate;

	//			startVerVec = (spt - stmpPt).normal();
	//			startVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);

	//			if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){
	//				insPt = spt - startVerVec * _ttof(m_textOffset);
	//			}				
	//			else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){
	//				insPt = spt + startVerVec * _ttof(m_textOffset);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//				lineVec2.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = spt + lineVec2 * _ttof(m_textOffset);
	//			}
	//			else {// 선 아래 수직
	//				lineVec2.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = spt + lineVec2 * _ttof(m_textOffset);
	//			}
	//			rotate = lineVec2.angleOnPlane(pln);

	//			// STA
	//			if(jobMode == 0)
	//			{
	//				//int dotCnt = _ttoi(m_decimalPosition); // 김성곤 대리
	//				//int num = _ttof(m_staStartPoint) / 1000;
	//				//double remainder = _ttof(m_staStartPoint) - (num * 1000);
	//				int dotCnt = _ttoi(m_decimalPosition);
	//				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//				{
	//					textString.Format(_T("STA %d"), int(staDist / 1000));
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod(int(staDist), 1000));
	//				}
	//				else // 문자 표기 생략
	//				{
	//					if (int(staDist) % 1000 == 0)
	//					{
	//						textString.Format(_T("STA %d"), int(staDist / 1000));
	//						textString = textString + "+";
	//					}
	//					else textString.Empty();
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod(int(staDist), 1000));
	//				}
	//			}

	//			// Number
	//			else
	//			{ 
	//				int dotCnt = _ttoi(m_decimalPosition);
	//				textString.Format(_T("NO %d"), int((pointDist + staDist) / pointDist) - 1);
	//				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//				{
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((pointDist + staDist), pointDist));
	//				}
	//			}
	//			insertStartText->setTextString(textString);
	//			insertStartText->setRotation(rotate);
	//			insertStartText->setHeight(_ttof(m_textSize));
	//			insertStartText->setLayer(curTextStr);
	//			if(ChangeTextColor == true)
	//				insertStartText->setColorIndex(m_selectTextColorIndex);
	//			insertStartText->setTextStyle(curTextStyleId);

	//			if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){
	//				insertStartText->setHorizontalMode(AcDb::kTextCenter);
	//				insertStartText->setVerticalMode(AcDb::kTextBottom);
	//			}
	//			else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){	
	//				insertStartText->setVerticalMode(AcDb::kTextTop);
	//				insertStartText->setHorizontalMode(AcDb::kTextCenter);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//				insertStartText->setHorizontalMode(AcDb::kTextLeft);
	//				insertStartText->setVerticalMode(AcDb::kTextVertMid);
	//			}
	//			else { // 선 아래 수직
	//				insertStartText->setHorizontalMode(AcDb::kTextLeft);
	//				insertStartText->setVerticalMode(AcDb::kTextVertMid);
	//			}
	//			insertStartText->setAlignmentPoint(insPt);

	//			// [끝점 문자 작성]
	//			etmpPt = vecTmpPt(pPline, totalDist, ept, etmpPt);

	//			lineVec2 = (ept - etmpPt).normal();

	//			endVerVec = (etmpPt - ept).normal();
	//			endVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);

	//			if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){
	//				insPt = ept - endVerVec * _ttof(m_textOffset);
	//			}
	//			else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){	
	//				insPt = ept + endVerVec * _ttof(m_textOffset);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//				lineVec2.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = ept + lineVec2 * _ttof(m_textOffset);
	//			}
	//			else {// 선 아래 수직
	//				lineVec2.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = ept + lineVec2 * _ttof(m_textOffset);
	//			}
	//			rotate = lineVec2.angleOnPlane(pln);
	//		
	//			// STA
	//			if(jobMode == 0){
	//				int num = totalDist / 1000;
	//				int dotCnt = _ttoi(m_decimalPosition);
	//				double remainder = totalDist - (num * 1000);
	//				
	//				//textString.Format(_T("STA %d+%.*f"), num, dotCnt, remainder);
	//				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//				{
	//					textString.Format(_T("STA %d"), int((staDist + totalDist) / 1000));
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((staDist + totalDist), 1000));
	//				}
	//				else // 문자 표기 생략
	//				{
	//					if (int(staDist + totalDist) % 1000 == 0)
	//					{
	//						textString.Format(_T("STA %d"), int((staDist + totalDist) / 1000));
	//						textString = textString + "+";
	//					}
	//					else textString.Empty();
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((staDist + totalDist), 1000));
	//				}
	//			}

	//			// Number
	//			else{
	//				int num = totalDist / pointDist;
	//				int dotCnt = _ttoi(m_decimalPosition);
	//				//double remainder = totalDist - (num * pointDist);
	//				//textString.Format(_T("NO %d+%.*f"), num, dotCnt, remainder);
	//				textString.Format(_T("NO %d"), int((totalDist + staDist) / pointDist));
	//				if (fmod((totalDist + staDist), pointDist) > 0.00000001)
	//				{ 
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((totalDist + staDist), pointDist));
	//				}
	//			}

	//			insertEndText->setTextString(textString);
	//			insertEndText->setRotation(rotate);
	//			insertEndText->setHeight(_ttof(m_textSize));
	//			insertEndText->setLayer(curTextStr);
	//			if(ChangeTextColor == true)
	//				insertEndText->setColorIndex(m_selectTextColorIndex);
	//			insertEndText->setTextStyle(curTextStyleId);
	//			if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){
	//				insertEndText->setHorizontalMode(AcDb::kTextCenter);
	//				insertEndText->setVerticalMode(AcDb::kTextBottom);
	//			}
	//			else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){
	//				insertEndText->setVerticalMode(AcDb::kTextTop);
	//				insertEndText->setHorizontalMode(AcDb::kTextCenter);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//				insertEndText->setHorizontalMode(AcDb::kTextLeft);
	//				insertEndText->setVerticalMode(AcDb::kTextVertMid);
	//			}
	//			else { // 선 아래 수직
	//				insertEndText->setHorizontalMode(AcDb::kTextLeft);
	//				insertEndText->setVerticalMode(AcDb::kTextVertMid);
	//			}
	//			insertEndText->setAlignmentPoint(insPt);

	//			AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//			acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//			insertStartText->setDatabaseDefaults();
	//			insertEndText->setDatabaseDefaults();
	//			bTR->appendAcDbEntity(insertStartText);
	//			bTR->appendAcDbEntity(insertEndText);
	//			insertStartText->close();
	//			insertEndText->close();
	//			bTR->close();				
	//		}// start 포인트과 end 포인트의 대한 구함

	//		int vertax = 0;
	//		double vertaxDist, loopDist, loopDist1;
	//		loopDist = _ttof(m_staStartPoint);
	//		double sumNum = _ttof(m_textDist);
	//		double lineDist = 0, lineDist1;
	//	
	//	
	//		// 김성곤 대리 추가
	//		/*AcGePoint3d endPoint;
	//		double endDist = 0.0;
	//		pPline->getEndPoint(endPoint);
	//		pPline->getDistAtPoint(endPoint, endDist);*/
	//		// 김성곤 대리 추가 end
	//		double eptDist;
	//		pPline->getDistAtPoint(ept, eptDist);	
	//		// 객체 길이 < 측점 표시 간격 
	//		// offsetDist 에 대입
	//		getStartOffset(staDist, pointDist, offsetDist);
	//	
	//		for(int i = 0; i < cnt + 1; i++)
	//		{
	//			AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
	//			// 시작점과 끝점이 반대일 경우 모두 포함하여 측점 거리
	//			// 총거리 + |끝거리 - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//			lineDist = totalDist - abs(eptDist - (pointDist * (i + 1) - offsetDist));
	//			// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//			lineDist1 = totalDist - abs((eptDist - 0.0001) - (pointDist * (i + 1) - offsetDist));
	//			
	//			// lineDist 가 시작점과 끝점이라면 작성 X
	//			if (lineDist > 0.00000001 && (totalDist - lineDist) > 0.00000001)
	//			{
	//				// 정방향과 반대방향에 대해서 선 작도
	//				pPline->getDistAtPoint(ept, DistChk);
	//				pPline->getPointAtDist(lineDist, checkPt);
	//				//pPline->getPointAtDist(loopDist + 0.0001, tempPt);
	//				pPline->getPointAtDist(lineDist1, tempPt);
	//				AcGeVector3d verVec = (checkPt - tempPt).normal();
	//				AcGeVector3d lineVerVec = (tempPt - checkPt).normal();
	//				verVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				vecTempSp = checkPt + verVec * (_ttof(m_staLineLength) / 2.0);
	//				verTempEp = vecTempSp - verVec * _ttof(m_staLineLength);

	//				AcDbLine* pDrawLine = new AcDbLine(vecTempSp, verTempEp);
	//				pDrawLine->setLayer(curLayerStr);
	//				if (ChangeLayerColor == true)
	//					pDrawLine->setColorIndex(m_selectLineColorIndex);

	//				AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//				acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//				pDrawLine->setDatabaseDefaults();
	//				bTR->appendAcDbEntity(pDrawLine);
	//				pDrawLine->close();
	//				bTR->close();

	//				// 반대 방향 (DistChk == 0 라면)
	//				if (DistChk < 0.00000001)
	//					if ((lineDist - pointDist) < 0) break;
	//				// 정방향 (DistChk == totalDist 라면)
	//				if (fabs(DistChk - totalDist) < 0.00000001)
	//				{
	//					//if (txtDist > totalDist) break;
	//					if ((lineDist + pointDist) > totalDist) break;
	//				}

	//				//loopDist = loopDist + pointDist;		
	//				if (loopDist >= totalDist)
	//					loopDist = totalDist;
	//			}
	//		}
	//		double cirDist, cirDist1;
	//		// 객체 길이 < 측점 표시 간격 
	//		// offsetDist 에 대입
	//		getStartOffset(staDist, circleDist, offsetDist);

	//		if(makeCircle)
	//		{
	//			//loopDist = _ttof(m_staCircleDist);
	//			for(int i = 0; i < circleCnt + 1; i++){
	//				AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
	//				// 총거리 + |끝거리 - (문자 표시 간격 * (i+1) - 나머지(시작 측점, 문자 표시 간격))|
	//				cirDist = totalDist - abs(eptDist - (circleDist * (i + 1) - offsetDist));
	//				// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//				cirDist1 = totalDist - abs((eptDist - 0.0001) - (circleDist * (i + 1) - offsetDist));
	//				
	//				// lineDist 가 시작점과 끝점이라면 작성 X
	//				if (cirDist > 0.00000001 && (totalDist - cirDist) > 0.00000001)
	//				{
	//					// 정방향과 반대방향에 대해서 원 작도
	//					pPline->getDistAtPoint(ept, DistChk);
	//					pPline->getPointAtDist(cirDist, checkPt);
	//					pPline->getPointAtDist(cirDist1, tempPt);

	//					AcGeVector3d verVec = (checkPt - tempPt).normal();
	//					AcGeVector3d lineVerVec = (tempPt - checkPt).normal();
	//					verVec.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//					vecTempSp = checkPt + verVec * (_ttof(m_staLineLength) / 2.0);
	//					verTempEp = vecTempSp - verVec * _ttof(m_staLineLength);

	//					AcDbCircle* drawCiecle = new AcDbCircle();
	//					drawCiecle->setCenter(checkPt);
	//					drawCiecle->setRadius((_ttof(m_staLineLength) / 2.0));
	//					drawCiecle->setLayer(curLayerStr);
	//					if (ChangeLayerColor == true)
	//						drawCiecle->setColorIndex(m_selectLineColorIndex);

	//					AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//					acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//					drawCiecle->setDatabaseDefaults();
	//					bTR->appendAcDbEntity(drawCiecle);
	//					drawCiecle->close();
	//					bTR->close();

	//					// 반대 방향 (DistChk == 0 라면)
	//					if (DistChk < 0.00000001)
	//						if ((cirDist - circleDist) < 0) break;
	//					// 정방향 (DistChk == totalDist 라면)
	//					if (fabs(DistChk - totalDist) < 0.00000001)
	//					{
	//						//if (txtDist > totalDist) break;
	//						if ((cirDist + circleDist) > totalDist) break;
	//					}
	//					//loopDist = loopDist + _ttof(m_staCircleDist);
	//				}
	//			}
	//		}

	//		double txtDist, txtDist1;
	//		getStartOffset(staDist, textDist, offsetDist);

	//		if(makeText)
	//		{
	//			loopDist = _ttof(m_textDist);
	//			for(int i = 0; i < textCnt + 1; i++){
	//				AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
	//				// 총거리 + |끝거리 - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//				txtDist = totalDist - abs(eptDist - (textDist * (i + 1) - offsetDist));
	//				// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//				txtDist1 = totalDist - abs((eptDist - 0.0001) - (textDist * (i + 1) - offsetDist));

	//				// txtDist 가 시작점과 끝점이라면 작성 X
	//				if (txtDist > 0.00000001 && (totalDist - txtDist) > 0.00000001)
	//				{
	//					// 정방향과 반대방향에 대해서 원 작도
	//					pPline->getDistAtPoint(ept, DistChk);
	//					pPline->getPointAtDist(txtDist, checkPt);
	//					pPline->getPointAtDist(txtDist1, tempPt);

	//					AcGeVector3d verVec = (checkPt - tempPt).normal();
	//					AcGeVector3d lineVerVec = (tempPt - checkPt).normal();
	//					verVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//					vecTempSp = checkPt + verVec * (_ttof(m_staLineLength) / 2.0);
	//					verTempEp = vecTempSp - verVec * _ttof(m_staLineLength);

	//					CString textPosition;
	//					CString textString;
	//					CString textNum;
	//					m_cbxTextPosition.GetLBString(m_cbxTextPosition.GetCurSel(), textPosition);

	//					AcGePoint3d insPt;
	//					if (textPosition.CompareNoCase(_T("선 위쪽")) == 0) {
	//						insPt = checkPt - verVec * _ttof(m_textOffset);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선 아래쪽")) == 0) {
	//						insPt = checkPt + verVec * _ttof(m_textOffset);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//						lineVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//						insPt = checkPt + lineVerVec * _ttof(m_textOffset);
	//					}
	//					else {// 선 아래 수직
	//						lineVerVec.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//						insPt = checkPt + lineVerVec * _ttof(m_textOffset);
	//					}

	//					double rotate;
	//					AcGePlane pln(AcGePoint3d::kOrigin, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis);
	//					rotate = lineVerVec.angleOnPlane(pln);
	//					AcDbText* insertText = new AcDbText();
	//					if (staDist == 0) txt = (textDist * i);
	//					else txt = (textDist * (i + 1));

	//					// STA
	//					if (jobMode == 0)
	//					{
	//						int num = sumNum / 1000;
	//						int dotCnt = _ttoi(m_decimalPosition);
	//						double remainder;
	//						if (num == 0)
	//							remainder = sumNum;
	//						else
	//							remainder = sumNum - (num * 1000);
	//						double STAtxt = (textDist * (txt)) / 1000;
	//						if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//						{
	//							textString.Format(_T("STA %d"), int(STAtxt));
	//							textString = textString + "+";
	//							textString.Format(textString + L"%%.%df", dotCnt);
	//							textString.Format(textString, fmod((textDist * (txt + ((int)(staDist / textDist)))), 1000));
	//						}
	//						else // 문자 표기 생략
	//						{
	//							if (int(textDist * (txt)) % 1000 == 0)
	//							{
	//								textString.Format(_T("STA %d"), int(STAtxt));
	//								textString = textString + "+";
	//							}
	//							else textString.Empty();
	//							textString.Format(textString + L"%%.%df", dotCnt);
	//							textString.Format(textString, fmod((textDist * (txt + ((int)(staDist / textDist)))), 1000));
	//						}
	//						/*
	//						if(remainder != 0)
	//						{
	//							textString.Format(_T("STA %d+%g"), num, remainder);  // 김성곤 대리 수정
	//						}
	//						else
	//						{
	//							textString.Format(_T("STA %d+000"), num, dotCnt, remainder);
	//						}
	//						*/
	//					}
	//					// Number
	//					else
	//					{	// 실제로 값이 들어가는 구간
	//						double num = double(txt + (int)(staDist / textDist));
	//						double chk = 0.0;
	//						int dotCnt = _ttoi(m_decimalPosition);
	//						double Numtxt = num * (_ttof(m_textDist) / _ttof(m_staDist));
	//						// 문자 간격 / 측점 간격 나머지가 0이 아니라면 간격 / 측점 으로 문자열 구하기
	//						if (fmod(_ttof(m_textDist), _ttof(m_staDist)) != 0)
	//						{
	//							if (DistChk == 0) Numtxt = ((totalDist - txtDist) + staDist) / pointDist;
	//							if (DistChk == totalDist) Numtxt = (txtDist + staDist) / pointDist;
	//						}
	//						double remainder = totalDist - (num * _ttof(m_staDist));
	//						//textString.Format(_T("NO %d+%%.%df"), (int)Numtxt, dotCnt);
	//						textString.Format(_T("NO %d"), (int)Numtxt);
	//						if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//						{
	//							textString = textString + "+";
	//							textString.Format(textString + L"%%.%df", dotCnt);
	//							//textString.Format(textString, fmod(txtDist + staDist, textDist));
	//							if (DistChk == 0)			textString.Format(textString, fmod((totalDist - txtDist) + staDist, textDist));
	//							if (DistChk == totalDist)	textString.Format(textString, fmod((txtDist + staDist), textDist));
	//						}
	//						//if(pPline->getParamAtDist((num * _ttof(m_staDist)),chk) == Acad::eOk)
	//						//{
	//						//	textString.Format(_T("NO %f"), num * (_ttof(m_textDist) / _ttof(m_staDist)));
	//						//}
	//						//else
	//						//{
	//						//	double remainder = totalDist - (num * _ttof(m_staDist));
	//						//	//textString.Format(_T("NO %d+%.*f"), num, dotCnt, remainder);
	//						//	textString.Format(_T("NO %f+%f"), 
	//						//		num * (_ttof(m_textDist) / _ttof(m_staDist)), remainder);
	//						//}						
	//					}

	//					insertText->setTextString(textString);
	//					insertText->setRotation(rotate);
	//					insertText->setHeight(_ttof(m_textSize));
	//					insertText->setLayer(curTextStr);
	//					if (ChangeTextColor == true)
	//						insertText->setColorIndex(m_selectTextColorIndex);
	//					insertText->setTextStyle(curTextStyleId);
	//					if (textPosition.CompareNoCase(_T("선 위쪽")) == 0) {
	//						insertText->setHorizontalMode(AcDb::kTextCenter);
	//						insertText->setVerticalMode(AcDb::kTextBottom);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선 아래쪽")) == 0) {
	//						insertText->setVerticalMode(AcDb::kTextTop);
	//						insertText->setHorizontalMode(AcDb::kTextCenter);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//						insertText->setHorizontalMode(AcDb::kTextLeft);
	//						insertText->setVerticalMode(AcDb::kTextVertMid);
	//					}
	//					else { // 선 아래 수직
	//						insertText->setHorizontalMode(AcDb::kTextLeft);
	//						insertText->setVerticalMode(AcDb::kTextVertMid);
	//					}
	//					insertText->setAlignmentPoint(insPt);

	//					AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//					acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//					insertText->setDatabaseDefaults();
	//					bTR->appendAcDbEntity(insertText);
	//					insertText->close();
	//					bTR->close();
	//					sumNum = sumNum + _ttof(m_textDist);

	//					// 반대 방향 (DistChk == 0 라면)
	//					if (DistChk < 0.00000001)
	//						if ((txtDist - textDist) < 0) break;
	//					// 정방향 (DistChk == totalDist 라면)
	//					if (fabs(DistChk - totalDist) < 0.00000001)
	//					{
	//						//if (txtDist > totalDist) break;
	//						if ((txtDist + textDist) > totalDist) break;
	//					}
	//					//loopDist = loopDist + _ttof(m_textDist);
	//				}
	//			}
	//		}

	//		pPline->close();
	//	}
	//	else if(pEnt->isKindOf(AcDbArc::desc()))	// 아크인 경우
	//	{
	//		AcDbArc* pArc = AcDbArc::cast(pEnt);
	//		AcGePoint3d arcSp, arcEp, tempPt;
	//		double arcDist;
	//		double totalDist = 0;
	//		double offsetDist = 0;
	//		double staDist = _ttof(m_staStartPoint);       //시작 측점
	//		double pointDist = _ttof(m_staDist);
	//		double circleDist = _ttof(m_staCircleDist);
	//		double textDist = _ttof(m_textDist);
	//		double EndParm = 0.0;

	//		pArc->getEndParam(EndParm);
	//		pArc->getDistAtParam(EndParm, totalDist);

	//		/*pArc->getStartPoint(arcSp);
	//		pArc->getEndPoint(arcEp);
	//		pArc->getDistAtPoint(arcEp, arcDist);
	//		pArc->getPointAtDist(0.0001, tempPt);*/

	//		int cnt = totalDist / pointDist;

	//		int circleCnt = totalDist / circleDist;
	//		// 원 간격 / 측점 간격의 나머지가 0이 아닐 시 최소공배수
	//		if (fmod(circleDist, pointDist) != 0)
	//		{
	//			circleDist = lcm(circleDist, pointDist);
	//			circleCnt = totalDist / circleDist;
	//		}

	//		int textCnt = totalDist / textDist;
	//		// 문자 간격 / 측점 간격의 나머지가 0이 아닐 시 최소공배수
	//		if (fmod(textDist, pointDist) != 0)
	//		{
	//			textDist = lcm(textDist, pointDist);
	//			textCnt = totalDist / textDist;
	//		}

	//		//AcGePoint3d startArcSp, startArcEp;
	//		AcGePoint3d startVerSp, startVerEp, stmpPt, etmpPt;
	//		AcGeVector3d startVerVec, endVerVec;
	//		double DistChk = 0;

	//		stmpPt = vecTmpPt(pArc, totalDist, spt, stmpPt);
	//		etmpPt = vecTmpPt(pArc, totalDist, ept, etmpPt);
	//		BothLineDraw(spt, ept, stmpPt, etmpPt, curLayerStr);

	//		/*AcGeVector3d startArcVec = (arcSp - tempPt).normal();
	//		startArcVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//		startArcSp = arcSp + startArcVec * (_ttof(m_staLineLength) / 2.0);
	//		startArcEp = startArcSp - startArcVec * _ttof(m_staLineLength);

	//		AcDbLine* pStartDrawLine = new AcDbLine(startArcSp, startArcEp);
	//		pStartDrawLine->setLayer(curLayerStr);
	//		if(ChangeLayerColor == true)
	//			pStartDrawLine->setColorIndex(m_selectLineColorIndex);

	//		AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//		acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//		pStartDrawLine->setDatabaseDefaults();
	//		bTR->appendAcDbEntity(pStartDrawLine);
	//		pStartDrawLine->close();
	//		bTR->close();

	//		pArc->getPointAtDist(arcDist - 0.0001, tempPt);
	//		AcGePoint3d endArcSp, endArcEp;
	//		AcGeVector3d endArcVec = (arcEp - tempPt).normal();
	//		endArcVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//		endArcSp = arcEp + endArcVec * (_ttof(m_staLineLength) / 2.0);
	//		endArcEp = endArcSp - endArcVec * _ttof(m_staLineLength);

	//		AcDbLine* pEndtDrawLine = new AcDbLine(endArcSp, endArcEp);
	//		pEndtDrawLine->setLayer(curLayerStr);
	//		if(ChangeLayerColor == true)
	//			pEndtDrawLine->setColorIndex(m_selectLineColorIndex);

	//		bTR = new AcDbBlockTableRecord();
	//		acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//		pEndtDrawLine->setDatabaseDefaults();
	//		bTR->appendAcDbEntity(pEndtDrawLine);
	//		pEndtDrawLine->close();
	//		bTR->close();*/

	//		if(makeCircle){
	//			BothMakeCircle(spt, ept, curLayerStr);
	//			/*AcGePoint3d cenPt;
	//			pArc->getStartPoint(cenPt);
	//			AcDbCircle* drawStartCircle = new AcDbCircle();
	//			drawStartCircle->setCenter(cenPt);
	//			drawStartCircle->setRadius((_ttof(m_staLineLength) / 2.0));
	//			drawStartCircle->setLayer(curLayerStr);
	//			if(ChangeLayerColor == true)
	//				drawStartCircle->setColorIndex(m_selectLineColorIndex);

	//			pArc->getEndPoint(cenPt);
	//			AcDbCircle* drawEndCircle = new AcDbCircle();
	//			drawEndCircle->setCenter(cenPt);
	//			drawEndCircle->setRadius((_ttof(m_staLineLength) / 2.0));
	//			drawEndCircle->setLayer(curLayerStr);
	//			if(ChangeLayerColor == true)
	//				drawEndCircle->setColorIndex(m_selectLineColorIndex);

	//			bTR = new AcDbBlockTableRecord();
	//			acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//			drawStartCircle->setDatabaseDefaults();
	//			drawEndCircle->setDatabaseDefaults();
	//			bTR->appendAcDbEntity(drawStartCircle);
	//			bTR->appendAcDbEntity(drawEndCircle);
	//			drawStartCircle->close();
	//			drawEndCircle->close();
	//			bTR->close();*/
	//		}

	//		double sumNum = _ttof(m_textDist);
	//		if(makeText){
	//			AcGePoint3d insPt;
	//			CString textPosition;
	//			CString textString;
	//			m_cbxTextPosition.GetLBString(m_cbxTextPosition.GetCurSel(), textPosition);

	//			AcDbText* insertStartText = new AcDbText();
	//			AcDbText* insertEndText= new AcDbText();

	//			// [시작점 문자 작성]
	//			//acutPrintf(L"\nspt = %g, %g, %g\n", spt.x, spt.y, spt.z);
	//			stmpPt = vecTmpPt(pArc, totalDist, spt, stmpPt);
	//			//acutPrintf(L"\nstmpPt = %g, %g, %g\n", stmpPt.x, stmpPt.y, stmpPt.z);
	//			/*pArc->getStartPoint(arcSp);
	//			pArc->getDistAtPoint(arcEp, arcDist);
	//			pArc->getPointAtDist(0.0001, tempPt);*/
	//			AcGeVector3d lineVec1 = (spt - ept).normal();
	//			AcGeVector3d lineVec2 = (stmpPt - spt).normal();
	//			//lineVec1.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//			AcGePlane pln(AcGePoint3d::kOrigin, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis);
	//			double rotate;

	//			startVerVec = (spt - stmpPt).normal();
	//			startVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);

	//			if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){
	//				insPt = spt - startVerVec * _ttof(m_textOffset);
	//			}
	//			else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){
	//				insPt = spt + startVerVec * _ttof(m_textOffset);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//				lineVec2.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = spt + lineVec2 * _ttof(m_textOffset);
	//			}
	//			else {// 선 아래 수직
	//				lineVec2.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = spt + lineVec2 * _ttof(m_textOffset);
	//			}
	//			rotate = lineVec2.angleOnPlane(pln);

	//			// STA
	//			if(jobMode == 0){
	//				/*int num = _ttof(m_staStartPoint) / 1000;
	//				double remainder = _ttof(m_staStartPoint) - (num * 1000);
	//				textString.Format(_T("%d+%g"), num, remainder);*/
	//				int dotCnt = _ttoi(m_decimalPosition);
	//				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//				{
	//					textString.Format(_T("STA %d"), int(staDist / 1000));
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod(int(staDist), 1000));
	//				}
	//				else // 문자 표기 생략
	//				{
	//					if (int(staDist) % 1000 == 0)
	//					{
	//						textString.Format(_T("STA %d"), int(staDist / 1000));
	//						textString = textString + "+";
	//					}
	//					else textString.Empty();
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod(int(staDist), 1000));
	//				}
	//			}

	//			// Number
	//			else
	//			{
	//				// textString.Format(_T("NO 0+0"));
	//				int dotCnt = _ttoi(m_decimalPosition);
	//				textString.Format(_T("NO %d"), int((pointDist + staDist) / pointDist) - 1);
	//				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//				{
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((pointDist + staDist), pointDist));
	//				}
	//			}

	//			insertStartText->setTextString(textString);
	//			insertStartText->setRotation(rotate);
	//			insertStartText->setHeight(_ttof(m_textSize));
	//			insertStartText->setLayer(curTextStr);
	//			if(ChangeTextColor == true)
	//				insertStartText->setColorIndex(m_selectTextColorIndex);
	//			insertStartText->setTextStyle(curTextStyleId);
	//			if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){
	//				insertStartText->setHorizontalMode(AcDb::kTextCenter);
	//				insertStartText->setVerticalMode(AcDb::kTextBottom);
	//			}
	//			else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){
	//				insertStartText->setVerticalMode(AcDb::kTextTop);
	//				insertStartText->setHorizontalMode(AcDb::kTextCenter);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//				insertStartText->setHorizontalMode(AcDb::kTextLeft);
	//				insertStartText->setVerticalMode(AcDb::kTextVertMid);
	//			}
	//			else { // 선 아래 수직
	//				insertStartText->setHorizontalMode(AcDb::kTextLeft);
	//				insertStartText->setVerticalMode(AcDb::kTextVertMid);
	//			}
	//			insertStartText->setAlignmentPoint(insPt);

	//			// [끝점 문자 작성]
	//			etmpPt = vecTmpPt(pArc, totalDist, ept, etmpPt);
	//			//acutPrintf(L"\netmpPt = %g, %g, %g\n", etmpPt.x, etmpPt.y, etmpPt.z);
	//			/*pArc->getEndPoint(arcEp);
	//			pArc->getPointAtDist(arcDist - 0.0001, tempPt);*/
	//			//lineVec1 = (tempPt - arcEp).normal();
	//			lineVec2 = (ept - etmpPt).normal();

	//			endVerVec = (etmpPt - ept).normal();
	//			endVerVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//		
	//			if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){
	//				insPt = ept - endVerVec * _ttof(m_textOffset);
	//			}
	//			else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){
	//				insPt = ept + endVerVec * _ttof(m_textOffset);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//				lineVec2.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = ept + lineVec2 * _ttof(m_textOffset);
	//			}
	//			else {// 선 아래 수직
	//				lineVec2.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				insPt = ept + lineVec2 * _ttof(m_textOffset);
	//			}
	//			rotate = lineVec2.angleOnPlane(pln);

	//			if(jobMode == 0){
	//				//int num = arcDist / 1000;
	//				int dotCnt = _ttoi(m_decimalPosition);
	//				//double remainder = arcDist - (num * 1000);
	//				//textString.Format(_T("%d+%g"), num, remainder);
	//				if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//				{
	//					textString.Format(_T("STA %d"), int((staDist + totalDist) / 1000));
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((staDist + totalDist), 1000));
	//				}
	//				else // 문자 표기 생략
	//				{
	//					if (int(staDist + totalDist) % 1000 == 0)
	//					{
	//						textString.Format(_T("STA %d"), int((staDist + totalDist) / 1000));
	//						textString = textString + "+";
	//					}
	//					else textString.Empty();
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((staDist + totalDist), 1000));
	//				}
	//			}
	//			else{
	//				/*int num = arcDist / _ttof(m_staDist);
	//				double remainder = arcDist - (num * _ttof(m_staDist));
	//				textString.Format(_T("NO %d+%g"), num, remainder);*/
	//				int num = totalDist / pointDist;
	//				int dotCnt = _ttoi(m_decimalPosition);

	//				textString.Format(_T("NO %d"), int((totalDist + staDist) / pointDist));
	//				if (fmod((totalDist + staDist), pointDist) > 0.00000001)
	//				{
	//					textString = textString + "+";
	//					textString.Format(textString + L"%%.%df", dotCnt);
	//					textString.Format(textString, fmod((totalDist + staDist), pointDist));
	//				}
	//			}

	//			insertEndText->setTextString(textString);
	//			insertEndText->setRotation(rotate);
	//			insertEndText->setHeight(_ttof(m_textSize));
	//			insertEndText->setLayer(curTextStr);
	//			if(ChangeTextColor == true)
	//				insertEndText->setColorIndex(m_selectTextColorIndex);
	//			insertEndText->setTextStyle(curTextStyleId);
	//			if(textPosition.CompareNoCase(_T("선 위쪽")) == 0){
	//				insertEndText->setHorizontalMode(AcDb::kTextCenter);
	//				insertEndText->setVerticalMode(AcDb::kTextBottom);
	//			}
	//			else if(textPosition.CompareNoCase(_T("선 아래쪽")) == 0){
	//				insertEndText->setVerticalMode(AcDb::kTextTop);
	//				insertEndText->setHorizontalMode(AcDb::kTextCenter);
	//			}
	//			else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//				insertEndText->setHorizontalMode(AcDb::kTextLeft);
	//				insertEndText->setVerticalMode(AcDb::kTextVertMid);
	//			}
	//			else { // 선 아래 수직
	//				insertEndText->setHorizontalMode(AcDb::kTextLeft);
	//				insertEndText->setVerticalMode(AcDb::kTextVertMid);
	//			}
	//			insertEndText->setAlignmentPoint(insPt);

	//			AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//			acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//			insertStartText->setDatabaseDefaults();
	//			insertEndText->setDatabaseDefaults();
	//			bTR->appendAcDbEntity(insertStartText);
	//			bTR->appendAcDbEntity(insertEndText);
	//			insertStartText->close();
	//			insertEndText->close();
	//			bTR->close();
	//		}

	//		//int cnt = arcDist / pointDist;
	//		//int circleCnt = arcDist / _ttof(m_staCircleDist);
	//		//int textCnt = arcDist / _ttof(m_textDist);
	//		double loopDist = _ttof(m_staStartPoint);

	//		double lineDist = 0, lineDist1;
	//		double eptDist;
	//		pArc->getClosestPointTo(ept, ept);
	//		pArc->getDistAtPoint(ept, eptDist);
	//		//acutPrintf(L"\n%f\n", eptDist);

	//		getStartOffset(staDist, pointDist, offsetDist);

	//		for(int i = 0; i < cnt + 1; i++){
	//			/*AcGePoint3d checkPt, tempPt, lineSp, lineEp;
	//			pArc->getPointAtDist(loopDist, checkPt);
	//			pArc->getPointAtDist(loopDist + 0.0001, tempPt);*/
	//			AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
	//			// 시작점과 끝점이 반대일 경우 모두 포함하여 측점 거리
	//			// 총거리 + |끝거리 - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//			lineDist = totalDist - abs(eptDist - (pointDist * (i + 1) - offsetDist));
	//			// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//			lineDist1 = totalDist - abs((eptDist - 0.0001) - (pointDist * (i + 1) - offsetDist));

	//			if (lineDist > 0.00000001 && (totalDist - lineDist) > 0.00000001)
	//			{
	//				// 정방향과 반대방향에 대해서 선 작도
	//				pArc->getDistAtPoint(ept, DistChk);
	//				pArc->getPointAtDist(lineDist, checkPt);
	//				pArc->getPointAtDist(lineDist1, tempPt);
	//				AcGeVector3d vec = (checkPt - tempPt).normal();
	//				AcGeVector3d lineVec = (tempPt - checkPt).normal();
	//				vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//				vecTempSp = checkPt + vec * (_ttof(m_staLineLength) / 2.0);
	//				verTempEp = vecTempSp - vec * _ttof(m_staLineLength);

	//				//AcDbLine* drawLine = new AcDbLine(lineSp, lineEp);
	//				AcDbLine* drawLine = new AcDbLine(vecTempSp, verTempEp);
	//				drawLine->setLayer(curLayerStr);
	//				if (ChangeLayerColor == true)
	//					drawLine->setColorIndex(m_selectLineColorIndex);

	//				AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//				acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//				drawLine->setDatabaseDefaults();
	//				bTR->appendAcDbEntity(drawLine);
	//				drawLine->close();
	//				bTR->close();

	//				// 반대 방향 (DistChk == 0 라면)
	//				if (DistChk < 0.00000001)
	//					if ((lineDist - pointDist) < 0) break;
	//				// 정방향 (DistChk == totalDist 라면)
	//				if (fabs(DistChk - totalDist) < 0.00000001)
	//				{
	//					//if (txtDist > totalDist) break;
	//					if ((lineDist + pointDist) > totalDist) break;
	//				}

	//				//loopDist = loopDist + pointDist;
	//			}
	//		}

	//		double cirDist, cirDist1;

	//		getStartOffset(staDist, circleDist, offsetDist);

	//		if(makeCircle){
	//			loopDist = cirlceResult;
	//			for(int i = 0; i < circleCnt + 1; i++){
	//				//AcGePoint3d checkPt;
	//				AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
	//				// 총거리 + |끝거리 - (문자 표시 간격 * (i+1) - 나머지(시작 측점, 문자 표시 간격))|
	//				cirDist = totalDist - abs(eptDist - (circleDist * (i + 1) - offsetDist));
	//				// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//				cirDist1 = totalDist - abs((eptDist - 0.0001) - (circleDist * (i + 1) - offsetDist));
	//				//pArc->getPointAtDist(loopDist, checkPt);

	//				if (cirDist > 0.00000001 && (totalDist - cirDist) > 0.00000001)
	//				{
	//					pArc->getDistAtPoint(ept, DistChk);
	//					pArc->getPointAtDist(cirDist, checkPt);
	//					pArc->getPointAtDist(cirDist1, tempPt);

	//					AcGeVector3d verVec = (checkPt - tempPt).normal();
	//					AcGeVector3d lineVerVec = (tempPt - checkPt).normal();
	//					verVec.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//					vecTempSp = checkPt + verVec * (_ttof(m_staLineLength) / 2.0);
	//					verTempEp = vecTempSp - verVec * _ttof(m_staLineLength);

	//					AcDbCircle* drawCircle = new AcDbCircle();
	//					drawCircle->setCenter(checkPt);
	//					drawCircle->setRadius((_ttof(m_staLineLength) / 2.0));
	//					drawCircle->setLayer(curLayerStr);
	//					if (ChangeLayerColor == true)
	//						drawCircle->setColorIndex(m_selectLineColorIndex);

	//					AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//					acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//					drawCircle->setDatabaseDefaults();
	//					bTR->appendAcDbEntity(drawCircle);
	//					drawCircle->close();
	//					bTR->close();

	//					// 반대 방향 (DistChk == 0 라면)
	//					if (DistChk < 0.00000001)
	//						if ((cirDist - circleDist) < 0) break;
	//					// 정방향 (DistChk == totalDist 라면)
	//					if (fabs(DistChk - totalDist) < 0.00000001)
	//					{
	//						//if (txtDist > totalDist) break;
	//						if ((cirDist + circleDist) > totalDist) break;
	//					}

	//					//loopDist = loopDist + _ttof(m_staCircleDist);
	//				}
	//			}
	//		}

	//		double txtDist, txtDist1;

	//		getStartOffset(staDist, textDist, offsetDist);

	//		if(makeText){				
	//			//loopDist = textResult;
	//			for(int i = 0; i < textCnt + 1; i++){
	//				//AcGePoint3d checkPt, tempPt, lineSp, lineEp;
	//				AcGePoint3d verSp, verEp, checkPt, tempPt, vecTempSp, verTempEp;
	//				// 총거리 + |끝거리 - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//				txtDist = totalDist - abs(eptDist - (textDist * (i + 1) - fmod(staDist, textDist)));
	//				// 총거리 + |(끝거리 - 오차) - (측점 표시 간격 * (i+1) - 나머지(시작 측점, 측점 표시 간격))|
	//				txtDist1 = totalDist - abs((eptDist - 0.0001) - (textDist * (i + 1) - fmod(staDist, textDist)));
	//				
	//				if (txtDist > 0.00000001 && (totalDist - txtDist) > 0.00000001)
	//				{
	//					/*pArc->getPointAtDist(loopDist, checkPt);
	//					pArc->getPointAtDist(loopDist + 0.0001, tempPt);*/
	//					pArc->getDistAtPoint(ept, DistChk);
	//					pArc->getPointAtDist(txtDist, checkPt);
	//					pArc->getPointAtDist(txtDist1, tempPt);

	//					AcGeVector3d vec = (checkPt - tempPt).normal();
	//					AcGeVector3d lineVec = (tempPt - checkPt).normal();
	//					vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//					vecTempSp = checkPt + vec * (_ttof(m_staLineLength) / 2.0);
	//					verTempEp = vecTempSp - vec * _ttof(m_staLineLength);

	//					CString textPosition;
	//					CString textString;
	//					m_cbxTextPosition.GetLBString(m_cbxTextPosition.GetCurSel(), textPosition);

	//					AcGePoint3d insPt;
	//					if (textPosition.CompareNoCase(_T("선 위쪽")) == 0) {
	//						insPt = checkPt - vec * _ttof(m_textOffset);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선 아래쪽")) == 0) {
	//						insPt = checkPt + vec * _ttof(m_textOffset);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//						lineVec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//						insPt = checkPt + lineVec * _ttof(m_textOffset);
	//					}
	//					else {// 선 아래 수직
	//						lineVec.rotateBy(-90 * 3.141592 / 180, AcGeVector3d::kZAxis);
	//						insPt = checkPt + lineVec * _ttof(m_textOffset);
	//					}

	//					double rotate;
	//					AcGePlane pln(AcGePoint3d::kOrigin, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis);
	//					rotate = lineVec.angleOnPlane(pln);
	//					AcDbText* insertText = new AcDbText();
	//					if (staDist == 0) txt = i;
	//					else txt = (i + 1);

	//					// STA
	//					if (jobMode == 0) {
	//						/*int num = sumNum / 1000;
	//						double remainder;
	//						if(num == 0)
	//							remainder = sumNum;
	//						else
	//							remainder = sumNum - (num * 1000);

	//						textString.Format(_T("%d+%g"), num, remainder);*/
	//						int dotCnt = _ttoi(m_decimalPosition);
	//						double STAtxt = (textDist * (txt)) / 1000;
	//						if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//						{
	//							textString.Format(_T("STA %d"), int(STAtxt));
	//							textString = textString + "+";
	//							textString.Format(textString + L"%%.%df", dotCnt);
	//							textString.Format(textString, fmod((textDist* (txt + ((int)(staDist / textDist)))), 1000));
	//						}
	//						else // 문자 표기 생략
	//						{
	//							if (int(textDist * (txt)) % 1000 == 0)
	//							{
	//								textString.Format(_T("STA %d"), int(STAtxt));
	//								textString = textString + "+";
	//							}
	//							else textString.Empty();
	//							textString.Format(textString + L"%%.%df", dotCnt);
	//							textString.Format(textString, fmod((textDist* (txt + ((int)(staDist / textDist)))), 1000));
	//						}
	//					}
	//					// Number
	//					else {
	//						double num = double(txt);
	//						//double remainder = arcDist - (num * _ttof(m_staDist));
	//						//textString.Format(_T("NO %d+%g"), num, remainder);
	//						int dotCnt = _ttoi(m_decimalPosition);
	//						double Numtxt = num * (_ttof(m_textDist) / _ttof(m_staDist));
	//						// 문자 간격 / 측점 간격 나머지가 0이 아니라면 간격 / 측점 으로 문자열 구하기
	//						if (fmod(_ttof(m_textDist), _ttof(m_staDist)) != 0)
	//						{
	//							if (DistChk == 0) Numtxt = ((totalDist - txtDist) + staDist) / pointDist;
	//							if (DistChk == totalDist) Numtxt = (txtDist + staDist) / pointDist;
	//						}
	//						textString.Format(_T("NO %d"), (int)Numtxt);
	//						if (IsDlgButtonChecked(IDC_CHKTEXTVISIBLE) == false)
	//						{
	//							textString = textString + "+";
	//							textString.Format(textString + L"%%.%df", dotCnt);
	//							if (DistChk == 0) textString.Format(textString, fmod((totalDist - txtDist) + staDist, textDist));
	//							if (DistChk == totalDist) textString.Format(textString, fmod((txtDist + staDist), textDist));
	//						}
	//					}

	//					insertText->setTextString(textString);
	//					insertText->setRotation(rotate);
	//					insertText->setHeight(_ttof(m_textSize));
	//					insertText->setLayer(curTextStr);
	//					if (ChangeTextColor == true)
	//						insertText->setColorIndex(m_selectTextColorIndex);
	//					insertText->setTextStyle(curTextStyleId);
	//					if (textPosition.CompareNoCase(_T("선 위쪽")) == 0) {
	//						insertText->setHorizontalMode(AcDb::kTextCenter);
	//						insertText->setVerticalMode(AcDb::kTextBottom);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선 아래쪽")) == 0) {
	//						insertText->setVerticalMode(AcDb::kTextTop);
	//						insertText->setHorizontalMode(AcDb::kTextCenter);
	//					}
	//					else if (textPosition.CompareNoCase(_T("선과 수직")) == 0) {
	//						insertText->setHorizontalMode(AcDb::kTextLeft);
	//						insertText->setVerticalMode(AcDb::kTextVertMid);
	//					}
	//					else { // 선 아래 수직
	//						insertText->setHorizontalMode(AcDb::kTextLeft);
	//						insertText->setVerticalMode(AcDb::kTextVertMid);
	//					}
	//					insertText->setAlignmentPoint(insPt);

	//					AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
	//					acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	//					insertText->setDatabaseDefaults();
	//					bTR->appendAcDbEntity(insertText);
	//					insertText->close();
	//					bTR->close();
	//					/*sumNum = sumNum + _ttof(m_textDist);
	//					loopDist = loopDist + _ttof(m_textDist);*/
	//					// 반대 방향 (DistChk == 0 라면)
	//					if (DistChk < 0.00000001)
	//						if ((txtDist - textDist) < 0) break;
	//					// 정방향 (DistChk == totalDist 라면)
	//					if (fabs(DistChk - totalDist) < 0.00000001)
	//					{
	//						//if (txtDist > totalDist) break;
	//						if ((txtDist + textDist) > totalDist) break;
	//					}
	//				}
	//			}
	//		}
	//		pArc->close();
	//	}


