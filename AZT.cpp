#include "stdafx.h"
#include "AZT.h"
#include "afxdialogex.h"

// CAZT 대화 상자입니다.
IMPLEMENT_DYNAMIC(CAZT, CDialogEx)
	CAZT::CAZT(CWnd* pParent)
	: CDialogEx(CAZT::IDD, pParent), JobMode(0), b_RadBtn(0), JobMode_Str(_T("")), Chk1_Str(_T("")), Chk2_Str(_T("")), Chk3_Str(_T("")), cbxLayerInt_Str(_T("")), m_LayerStr(_T("")), cbxTextInt_Str(_T("")), m_TxtStyleStr(_T("")), m_HeightStr(_T("")), m_WidthStr(_T("")), m_DotStr(_T("")), cbxTextLayerInt_Str(_T("")), m_TextLayerStr(_T("")), cbxLayerInt(0), cbxTextInt(0), cbxTextLayerInt(0), m_Height(0), m_Width(0), m_Dot(0), TextDist(0), TextHeight(0){
};

CAZT::~CAZT(){
}

void CAZT::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_AZT_FORM1, b_RadBtn);
	DDX_Control(pDX, IDC_AZT_CHECK1, m_Chk1_btn);
	DDX_Control(pDX, IDC_AZT_CHECK2, m_Chk2_btn);
	DDX_Control(pDX, IDC_AZT_CHECK3, m_Chk3_btn);
	DDX_Control(pDX, IDC_AZT_CBXLAYER1, m_cbxLayer);
	DDX_Control(pDX, IDC_AZT_BSELECT1, m_bSelect1);
	DDX_Control(pDX, IDC_AZT_CBXTEXTSTYLE, m_cbxTextType);
	DDX_Control(pDX, IDC_AZT_BSELECT2, m_bSelect2);
	DDX_Text(pDX, IDC_AZT_EDTHEIGHT, m_HeightStr);
	DDX_Text(pDX, IDC_AZT_EDTWIDTH, m_WidthStr);
	DDX_Text(pDX, IDC_AZT_DOT, m_DotStr);
	DDX_Control(pDX, IDC_AZT_CBXLAYER2, m_cbxTextLayer);
	DDX_Control(pDX, IDC_AZT_BSELECT3, m_bSelect3);
}

BEGIN_MESSAGE_MAP(CAZT, CDialogEx)
	ON_BN_CLICKED(IDC_AZT_OK, &CAZT::OnBnClickedAztOk)
	ON_BN_CLICKED(IDCANCEL, &CAZT::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_AZT_FORM1, &CAZT::OnBnClickedAztForm1)
	ON_BN_CLICKED(IDC_AZT_FORM2, &CAZT::OnBnClickedAztForm2)
	ON_BN_CLICKED(IDC_AZT_FORM3, &CAZT::OnBnClickedAztForm3)
	ON_BN_CLICKED(IDC_AZT_CHECK1, &CAZT::OnBnClickedAztCheck1)
	ON_BN_CLICKED(IDC_AZT_CHECK2, &CAZT::OnBnClickedAztCheck2)
	ON_BN_CLICKED(IDC_AZT_CHECK3, &CAZT::OnBnClickedAztCheck3)
	ON_CBN_SELCHANGE(IDC_AZT_CBXLAYER1, &CAZT::OnCbnSelchangeAztCbxlayer1)
	ON_BN_CLICKED(IDC_AZT_BSELECT1, &CAZT::OnBnClickedAztBselect1)
	ON_CBN_SELCHANGE(IDC_AZT_CBXTEXTSTYLE, &CAZT::OnCbnSelchangeAztCbxtextstyle)
	ON_BN_CLICKED(IDC_AZT_BSELECT2, &CAZT::OnBnClickedAztBselect2)
	ON_EN_UPDATE(IDC_AZT_EDTHEIGHT, &CAZT::OnEnUpdateAztEdtheight)
	ON_EN_UPDATE(IDC_AZT_EDTWIDTH, &CAZT::OnEnUpdateAztEdtwidth)
	ON_EN_UPDATE(IDC_AZT_DOT, &CAZT::OnEnUpdateAztDot)
	ON_NOTIFY(UDN_DELTAPOS, IDC_AZT_SPIN, &CAZT::OnDeltaposAztSpin)
	ON_CBN_SELCHANGE(IDC_AZT_CBXLAYER2, &CAZT::OnCbnSelchangeAztCbxlayer2)
	ON_BN_CLICKED(IDC_AZT_BSELECT3, &CAZT::OnBnClickedAztBselect3)
	ON_BN_CLICKED(ID_AZT_HELP, &CAZT::OnBnClickedAztHelp)
END_MESSAGE_MAP()

BOOL CAZT::OnInitDialog(){
	CDialogEx::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	m_bSelect1.AutoLoad();
	m_bSelect2.AutoLoad();
	m_bSelect3.AutoLoad();

	//글꼴 불러오기
	AcDbTextStyleTable *pTextTbl = NULL;
	AcDbTextStyleTablePointer pTextPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbTextStyleTableIterator *pTextTblIter;
	pTextPoint->newIterator(pTextTblIter);
	AcDbObjectId TextId;
	for(pTextTblIter->start(); !pTextTblIter->done(); pTextTblIter->step()){
		pTextTblIter->getRecordId(TextId);
		AcDbTextStyleTableRecordPointer pTextTblRcd(TextId, AcDb::kForRead);
		TCHAR *TextName;
		pTextTblRcd->getName(TextName);
		m_cbxTextType.AddString(TextName);
	}
	delete pTextTblIter;
	delete pTextTbl;

	//레이어 불러오기
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
		m_cbxLayer.AddString(name);
		m_cbxTextLayer.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;

	//방위각 표시 형식
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("JOBMODE"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		OnBnClickedAztForm1();
	else if(tempStr.CompareNoCase(_T("2")) == 0)
		OnBnClickedAztForm2();
	else if(tempStr.CompareNoCase(_T("3")) == 0)
		OnBnClickedAztForm3();

	//체크박스 선택한 두 점에 Line 그리기
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Check1"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_Chk1_btn.SetCheck(FALSE);
		bool_Chk1 = false;
		OnBnClickedAztCheck1();
	}
	else{
		m_Chk1_btn.SetCheck(TRUE);
		bool_Chk1 = true;
		OnBnClickedAztCheck1();
	}
	//체크박스 방향 표시 화살표 그리기
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Check2"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_Chk2_btn.SetCheck(FALSE);
		bool_Chk2 = false;
		OnBnClickedAztCheck2();
	}
	else{
		m_Chk2_btn.SetCheck(TRUE);
		bool_Chk2 = true;
		OnBnClickedAztCheck2();
	}
	//체크박스 역 방위각 표기
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Check3"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_Chk3_btn.SetCheck(FALSE);
		bool_Chk3 = false;
		OnBnClickedAztCheck3();
	}
	else{
		m_Chk3_btn.SetCheck(TRUE);
		bool_Chk3 = true;
		OnBnClickedAztCheck3();
	}
	//표기 레이어 문자열로 저장
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("CmbLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0){
		CmbLayer = _T("0");
	}
	else{
		CmbLayer = tempStr;
	}
	//콤보박스 표기 레이어
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("cbxLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_cbxLayer.SetCurSel(0);
		m_cbxLayer.GetLBText(0, CmbLayer);
	}
	else{
		cbxLayerInt_Str = tempStr;
		cbxLayerInt = _ttoi(tempStr);
		m_cbxLayer.SetCurSel(cbxLayerInt);
		if(m_cbxLayer.FindStringExact(-1, CmbLayer) == -1){//레이어가 존재하지 않을 경우
			m_cbxLayer.SetCurSel(0);
			m_cbxLayer.GetLBText(0, CmbLayer);
		}
		else{
			m_cbxLayer.SetCurSel(cbxLayerInt);
			m_cbxLayer.GetLBText(cbxLayerInt, CmbLayer);
		}
		SetDlgItemText(IDC_AZT_CBXLAYER1, CmbLayer);
	}
	//글꼴 문자열로 저장
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("CmbTextStyle"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		CmbTextStyle = _T("Annotative");
	}
	else{
		CmbTextStyle = tempStr;
	}
	//콤보박스 글꼴
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("cbxTextType"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_cbxTextType.SetCurSel(0);
		m_cbxTextType.GetLBText(0, CmbTextStyle);
	}
	else{
		cbxTextInt = _ttoi(tempStr);
		if(m_cbxTextType.FindStringExact(-1, CmbTextStyle) == -1){//글꼴이 존재하지 않을 경우
			m_cbxTextType.SetCurSel(0);
			m_cbxTextType.GetLBText(0, CmbTextStyle);
		}
		else{
			m_cbxTextType.SetCurSel(cbxTextInt);
			m_cbxTextType.GetLBText(cbxTextInt, CmbTextStyle);
		}
		SetDlgItemText(IDC_AZT_CBXTEXTSTYLE, CmbTextStyle);
	}
	//문자 높이
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Height"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_AZT_EDTHEIGHT, _T("3.000"));
	else
		SetDlgItemText(IDC_AZT_EDTHEIGHT, tempStr);
	//문자 폭
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Width"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_AZT_EDTWIDTH, _T("1.000"));
	else
		SetDlgItemText(IDC_AZT_EDTWIDTH, tempStr);
	//초단위 표기 소수점
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Dot"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_AZT_DOT, _T("2"));
	else
		SetDlgItemText(IDC_AZT_DOT, tempStr);
	//방위각 문자 레이어 문자열로 저장
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("CmbTextLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		CmbTextLayer = _T("0");
	}
	else{
		CmbTextLayer = tempStr;
	}
	//콤보박스 방위각 문자 레이어
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("cbxTextLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_cbxTextLayer.SetCurSel(0);
		m_cbxTextLayer.GetLBText(0, CmbTextLayer);
	}
	else{
		cbxTextLayerInt = _ttoi(tempStr);
		if(m_cbxTextLayer.FindStringExact(-1, CmbTextLayer) == -1){//레이어가 존재하지 않을 경우
			m_cbxTextLayer.SetCurSel(0);
			m_cbxTextLayer.GetLBText(0, CmbTextLayer);
		}
		else{
			m_cbxTextLayer.SetCurSel(cbxTextLayerInt);
			m_cbxTextLayer.GetLBText(cbxTextLayerInt, CmbTextLayer);
		}
		SetDlgItemText(IDC_AZT_CBXLAYER2, CmbTextLayer);
	}
	return TRUE;
}

void CAZT::MainDraw(){
	AcGePoint3d spt, ept;
	double val, Sec, ftemp,ftmp, xx,yy;
	int Degree, Min, RE_Degree;
	CString AzimuthStr, SecDot, SecDot_Str, RE_AzimuthStr, strMsg;

	//방위각 표시 형식
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("JOBMODE"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		JobMode = 1;
	else if(tempStr.CompareNoCase(_T("2")) == 0)
		JobMode = 2;
	else if(tempStr.CompareNoCase(_T("3")) == 0)
		JobMode = 3;

	//체크박스 선택한 두 점에 Line 그리기
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Check1"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0)
		bool_Chk1 = false;
	else
		bool_Chk1 = true;

	//체크박스 방향 표시 화살표 그리기
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Check2"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0)
		bool_Chk2 = false;
	else
		bool_Chk2 = true;

	//체크박스 역 방위각 표기
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Check3"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0)
		bool_Chk3 = false;
	else
		bool_Chk3 = true;

	//콤보박스 표기 레이어
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("cbxLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0)
		cbxLayerInt = 0;
	else
		cbxLayerInt = _ttoi(tempStr);

	//표기 레이어 문자
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("CmbLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		CmbLayer = _T("0");
	else
		CmbLayer = tempStr;

	//콤보박스 글꼴
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("cbxTextType"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0)
		cbxTextInt = 0;
	else
		cbxTextInt = _ttoi(tempStr);

	//글꼴 문자
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("CmbTextStyle"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		CmbTextStyle = _T("Standard");
	else
		CmbTextStyle = tempStr;

	//문자 높이
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Height"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_Height = 3.000;
	else
		m_Height = _tstof(tempStr);

	//문자 폭
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Width"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_Width = 1.000;
	else
		m_Width = _tstof(tempStr);

	//초단위 표기 소수점
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("Dot"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_Dot = 2;
	else
		m_Dot = _tstof(tempStr);

	//콤보박스 방위각 문자 레이어
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("cbxTextLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0)
		cbxTextLayerInt = 0;
	else
		cbxTextLayerInt = _ttoi(tempStr);

	//방위각 문자레이어 문자열
	tempStr = iniFile.readINI(_T("config.ini"), _T("AZT"), _T("CmbTextLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		CmbTextLayer = _T("0");
	else
		CmbTextLayer = tempStr;

	::SetFocus(adsw_acadDocWnd());
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	strMsg.LoadStringW(IDS_FIRST_SEL_CMD); // "\n첫 번째 점 지정 : "
	if(acedGetPoint(0, strMsg, asDblArray(spt)) != RTNORM)
		return;
	strMsg.LoadStringW(IDS_SECOND_SEL_CMD); // "\n두 번째 점 지정 : "
	if(acedGetPoint(asDblArray(spt), strMsg, asDblArray(ept)) != RTNORM)
		return;

	//시작-끝각,시작-끝 길이
	double Ang = ads_angle(asDblArray(spt), asDblArray(ept)), Dist = ads_distance(asDblArray(spt), asDblArray(ept));

	//방위각 구하기
	xx = abs(spt.x - ept.x);
	yy = abs(spt.y - ept.y);
	val = atan(xx / yy) * 180 / PI;
	if(RTD(Ang) > 90 && RTD(Ang) < 180 || RTD(Ang) > 270 && RTD(Ang) < 360)
		val = 180 - val;
	
	Degree = ftemp = val + 0.0000001;
	Min = ftmp = (ftemp += - Degree) * 60;
	Sec = (ftmp - Min) * 60;

	SecDot.Format(_T("%%.%df"), m_Dot);
	SecDot_Str.Format(SecDot, Sec);
	if(RTD(Ang) > 90 && RTD(Ang) < 270)
		Degree = Degree + 180;

	//역방위각 
	RE_Degree = Degree + 180;
	if(RE_Degree >= 360)
		RE_Degree = RE_Degree - 360;

	if(JobMode == 1){
		AzimuthStr.Format(_T("%d-%d-%s"), Degree, Min, SecDot_Str);
		RE_AzimuthStr.Format(_T("%d-%d-%s"), RE_Degree, Min, SecDot_Str);
	}
	else if(JobMode == 2){
		AzimuthStr.Format(_T("%d˚ %d' %s\""), Degree, Min, SecDot_Str);
		RE_AzimuthStr.Format(_T("%d˚ %d' %s\""), RE_Degree, Min, SecDot_Str);
	}
	else if(JobMode == 3){
		AzimuthStr.Format(_T("%d도 %d분 %s초"), Degree, Min, SecDot_Str);
		RE_AzimuthStr.Format(_T("%d도 %d분 %s초"), RE_Degree, Min, SecDot_Str);
	}

	AcGePoint3d Midpt, LeaderMidpt, Leaderpt1, Leaderpt2, Txtpt, Arrowpt1, Arrowpt2;
	acutPolar(asDblArray(spt), Ang, Dist / 2, asDblArray(Midpt));

	if(bool_Chk1 == false && bool_Chk2 == false && bool_Chk3 == false)
	{
		acutPolar(asDblArray(Midpt), Ang + DTR(90.0), m_Height * 1.0, asDblArray(Txtpt));
		CreateText(Txtpt, m_Height, m_Width, Ang, AzimuthStr, CmbTextLayer, CmbTextStyle, 1);
	}
	if(bool_Chk1 == true){
		if(RTD(Ang) >= 0 && RTD(Ang) <= 90 || RTD(Ang) > 270 && RTD(Ang) < 360){
			acutPolar(asDblArray(Midpt), Ang + DTR(90.0), m_Height * 1.0, asDblArray(Txtpt));
			CreateText(Txtpt, m_Height, m_Width, Ang, AzimuthStr, CmbTextLayer, CmbTextStyle, 1);
			CreateLine(spt, ept, CmbLayer);
		}
		else{
			acutPolar(asDblArray(Midpt), Ang - DTR(90.0), m_Height * 1.0, asDblArray(Txtpt));
			CreateText(Txtpt, m_Height, m_Width, Ang + DTR(180.0) , AzimuthStr, CmbTextLayer, CmbTextStyle, 1);
			CreateLine(spt, ept, CmbLayer);
		}
	}
	
	if(bool_Chk2 == true){
		if(RTD(Ang) >= 0 && RTD(Ang) <= 90 || RTD(Ang) > 270 && RTD(Ang) < 360){
			acutPolar(asDblArray(Midpt), Ang + DTR(90.0), m_Height * 1.0, asDblArray(Txtpt));
			CreateText(Txtpt, m_Height, m_Width, Ang, AzimuthStr, CmbTextLayer, CmbTextStyle, 1);
			acutPolar(asDblArray(Midpt), Ang + DTR(90.0), m_Height * 0.5, asDblArray(LeaderMidpt));
			acutPolar(asDblArray(LeaderMidpt), Ang, (TextDist / 2) + (m_Height * 2), asDblArray(Leaderpt1));
			acutPolar(asDblArray(LeaderMidpt), Ang + DTR(180.0), (TextDist / 2) + (m_Height * 2), asDblArray(Leaderpt2));
			CreateLine(Leaderpt1, Leaderpt2, CmbLayer);
			acutPolar(asDblArray(Leaderpt1), Ang + DTR(180.0), m_Height, asDblArray(Arrowpt1));
			acutPolar(asDblArray(Arrowpt1), Ang + DTR(90.0), 0.3 * m_Height, asDblArray(Arrowpt2));
			CreateSolid(Leaderpt1, Arrowpt1, Arrowpt2, CmbLayer);
		}
		else{
			acutPolar(asDblArray(Midpt), Ang - DTR(90.0), m_Height * 1.0, asDblArray(Txtpt));
			CreateText(Txtpt, m_Height, m_Width, Ang + DTR(180.0) , AzimuthStr, CmbTextLayer, CmbTextStyle, 1);
			acutPolar(asDblArray(Midpt), Ang - DTR(90.0), m_Height * 0.5, asDblArray(LeaderMidpt));
			acutPolar(asDblArray(LeaderMidpt), Ang, (TextDist / 2) + (m_Height * 2), asDblArray(Leaderpt1));
			acutPolar(asDblArray(LeaderMidpt), Ang - DTR(180.0), (TextDist / 2) + (m_Height * 2), asDblArray(Leaderpt2));
			CreateLine(Leaderpt1, Leaderpt2, CmbLayer);
			acutPolar(asDblArray(Leaderpt1), Ang - DTR(180.0), m_Height, asDblArray(Arrowpt1));
			acutPolar(asDblArray(Arrowpt1), Ang - DTR(90.0), 0.3 * m_Height, asDblArray(Arrowpt2));
			CreateSolid(Leaderpt1, Arrowpt1, Arrowpt2, CmbLayer);
		}
	}
	if(bool_Chk3 == true){//역 방위각 표시
		if(RTD(Ang) >= 0 && RTD(Ang) <= 90 || RTD(Ang) > 270 && RTD(Ang) < 360){
			acutPolar(asDblArray(Midpt), Ang - DTR(90.0), m_Height * 1.0, asDblArray(Txtpt));
			CreateText(Txtpt, m_Height, m_Width, Ang, RE_AzimuthStr, CmbTextLayer, CmbTextStyle, 2);
			acutPolar(asDblArray(Midpt), Ang - DTR(90.0), m_Height * 0.5, asDblArray(LeaderMidpt));
			acutPolar(asDblArray(LeaderMidpt), Ang, (TextDist / 2) + (m_Height * 2), asDblArray(Leaderpt2));
			acutPolar(asDblArray(LeaderMidpt), Ang - DTR(180.0), (TextDist / 2) + (m_Height * 2), asDblArray(Leaderpt1));
			CreateLine(Leaderpt1, Leaderpt2, CmbLayer);
			acutPolar(asDblArray(Leaderpt1), Ang, m_Height, asDblArray(Arrowpt1));
			acutPolar(asDblArray(Arrowpt1), Ang - DTR(90.0), 0.3 * m_Height, asDblArray(Arrowpt2));
			CreateSolid(Leaderpt1, Arrowpt1, Arrowpt2, CmbLayer);
		}
		else{
			acutPolar(asDblArray(Midpt), Ang + DTR(90.0), m_Height * 1.0, asDblArray(Txtpt));
			CreateText(Txtpt, m_Height, m_Width, Ang + DTR(180.0) , RE_AzimuthStr, CmbTextLayer, CmbTextStyle, 2);
			acutPolar(asDblArray(Midpt), Ang + DTR(90.0), m_Height * 0.5, asDblArray(LeaderMidpt));
			acutPolar(asDblArray(LeaderMidpt), Ang, (TextDist / 2) + (m_Height * 2), asDblArray(Leaderpt2));
			acutPolar(asDblArray(LeaderMidpt), Ang - DTR(180.0), (TextDist / 2) + (m_Height * 2), asDblArray(Leaderpt1));
			CreateLine(Leaderpt1, Leaderpt2, CmbLayer);
			acutPolar(asDblArray(Leaderpt1), Ang, m_Height, asDblArray(Arrowpt1));
			acutPolar(asDblArray(Arrowpt1), Ang + DTR(90.0), 0.3 * m_Height, asDblArray(Arrowpt2));
			CreateSolid(Leaderpt1, Arrowpt1, Arrowpt2, CmbLayer);
		}
	}
}

//OK버튼
void CAZT::OnBnClickedAztOk(){
	UpdateData(TRUE);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("JOBMODE"), JobMode_Str);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Check1"), Chk1_Str);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Check2"), Chk2_Str);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Check3"), Chk3_Str);
	cbxLayerInt = m_cbxLayer.GetCurSel();
	cbxLayerInt_Str.Format(_T("%d"), cbxLayerInt);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("cbxLayer"), cbxLayerInt_Str);
	cbxTextInt = m_cbxTextType.GetCurSel();
	cbxTextInt_Str.Format(_T("%d"), cbxTextInt);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("cbxTextType"), cbxTextInt_Str);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Height"), m_HeightStr);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Width"), m_WidthStr);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Dot"), m_DotStr);
	cbxTextLayerInt = m_cbxTextLayer.GetCurSel();
	cbxTextInt_Str.Format(_T("%d"), cbxTextLayerInt);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("cbxTextLayer"), cbxTextLayerInt_Str);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("CmbLayer"), CmbLayer);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("CmbTextStyle"), CmbTextStyle);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("CmbTextLayer"), CmbTextLayer);
	acDocManager->sendStringToExecute(acDocManager->curDocument(), _T("(AZimuthText)\n"), false, true);
	UpdateData(FALSE);
}

void CAZT::OnBnClickedCancel(){
	DestroyWindow();
}

//라디오 형식1
void CAZT::OnBnClickedAztForm1(){
	UpdateData(TRUE);
	JobMode = 1;
	JobMode_Str.Format(_T("%d"), JobMode);
	b_RadBtn = 0;
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("JOBMODE"), JobMode_Str);
	UpdateData(FALSE);
}

//라디오 형식2
void CAZT::OnBnClickedAztForm2(){
	UpdateData(TRUE);
	JobMode = 2;
	JobMode_Str.Format(_T("%d"), JobMode);
	b_RadBtn = 1;
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("JOBMODE"), JobMode_Str);
	UpdateData(FALSE);
}

//라디오 형식3
void CAZT::OnBnClickedAztForm3(){
	UpdateData(TRUE);
	JobMode = 3;
	JobMode_Str.Format(_T("%d"), JobMode);
	b_RadBtn = 2;
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("JOBMODE"), JobMode_Str);
	UpdateData(FALSE);
}

//체크박스 선택한 두 점에 Line 그리기
void CAZT::OnBnClickedAztCheck1(){
	bool_Chk1 = IsDlgButtonChecked(IDC_AZT_CHECK1);
	if(bool_Chk1 == 0)
		Chk1_Str = _T("0");
	else
		Chk1_Str = _T("1");

	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Check1"), Chk1_Str);
}

//체크박스 방향 표시 화살표 그리기
void CAZT::OnBnClickedAztCheck2(){
	bool_Chk2 = IsDlgButtonChecked(IDC_AZT_CHECK2);
	if(bool_Chk2 == 0)
		Chk2_Str = _T("0");
	else
		Chk2_Str = _T("1");

	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Check2"), Chk2_Str);
}

//체크박스 역 방위각 표기
void CAZT::OnBnClickedAztCheck3(){
	bool_Chk3 = IsDlgButtonChecked(IDC_AZT_CHECK3);
	if(bool_Chk3 == 0)
		Chk3_Str = _T("0");
	else
		Chk3_Str = _T("1");

	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Check3"), Chk3_Str);
}

//콤보박스 표기 레이어
void CAZT::OnCbnSelchangeAztCbxlayer1(){
	UpdateData(TRUE);
	cbxLayerInt = m_cbxLayer.GetCurSel();
	if(cbxLayerInt == -1)
		cbxLayerInt = 0;
	m_cbxLayer.SetCurSel(cbxLayerInt);
	cbxLayerInt_Str.Format(_T("%d"), cbxLayerInt);
	m_cbxLayer.GetLBText(cbxLayerInt, CmbLayer);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("cbxLayer"), cbxLayerInt_Str);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("CmbLayer"), CmbLayer);
	UpdateData(FALSE);
}

//표기 레이어 추출버튼
void CAZT::OnBnClickedAztBselect1(){
	UpdateData(TRUE);
	ads_name en;
	ads_point pt;
	AcCmColor Color;
	int cnt_AddStr;
	CString strMsg;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	CWnd* pCad = ModelessDlgHide(this);
	while(true){
		strMsg.LoadStringW(IDS_LAYER_SELECT); // _T("\n레이어를 가져올 객체 선택 : ")
		int stat = acedEntSel(strMsg, en, pt);
		if (stat == RTCAN || stat == RTNONE) {
			ModelessDlgShow(pCad, this);
			break;
		}
		else if(stat == RTERROR){
			resbuf zValueStr;
			ads_getvar(_T("ERRNO"), &zValueStr);
			if (zValueStr.resval.rint == 52) {
				ModelessDlgShow(pCad, this);
				break;
			}
			else
				continue;
		}
		else if(stat == RTNORM){
			AcDbObjectId objId;
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbEntity::desc())){
					AcDbEntity* pEntity = AcDbEntity::cast(pEnt);
					m_LayerStr = pEntity->layer();
					pEntity->close();
					for(int i = 0; i < m_cbxLayer.GetCount() + 1; i++){
						if(m_cbxLayer.FindString(i, m_LayerStr) == -1)
							m_cbxLayer.AddString(m_LayerStr);
					}
					cbxLayerInt = m_cbxLayer.FindString(0, m_LayerStr);
					m_cbxLayer.SetCurSel(cbxLayerInt);
					CmbLayer.Format(_T("%s"), m_LayerStr);
					cbxLayerInt_Str.Format(_T("%d"), cbxLayerInt);
					iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("cbxLayer"), cbxLayerInt_Str);
					iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("CmbLayer"), CmbLayer);
					ModelessDlgShow(pCad, this);
					break;
				}
				else{
					strMsg.LoadStringW(IDS_SELECT_TEXT); // _T("**객체가 아닙니다.**")
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();
			acedSSFree(en);
			ModelessDlgShow(pCad, this);
		}
	}
	UpdateData(FALSE);
}

//콤보박스 글꼴
void CAZT::OnCbnSelchangeAztCbxtextstyle(){
	UpdateData(TRUE);
	cbxTextInt = m_cbxTextType.GetCurSel();
	if(cbxTextInt == -1)
		cbxTextInt = 0;

	m_cbxTextType.SetCurSel(cbxTextInt);
	cbxTextInt_Str.Format(_T("%d"), cbxTextInt);
	m_cbxTextType.GetLBText(cbxTextInt, CmbTextStyle);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("cbxTextType"), cbxTextInt_Str);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("CmbTextStyle"), CmbTextStyle);
	UpdateData(FALSE);
}

//글꼴, 높이, 폭 추출버튼
void CAZT::OnBnClickedAztBselect2(){
	UpdateData(TRUE);
	ads_name en;
	ads_point pt;
	CString strMsg;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		strMsg.LoadStringW(IDS_SELECT_TEXT); // _T("\n문자 선택 : "
		int stat = acedEntSel(strMsg, en, pt);
		if(stat == RTCAN || stat == RTNONE)
			break;
		else if(stat == RTERROR){
			resbuf zValueStr;
			ads_getvar(_T("ERRNO"), &zValueStr);
			if(zValueStr.resval.rint == 52)
				break;
			else
				continue;
		}
		else if(stat == RTNORM){
			AcDbObjectId objId;
			TCHAR* Name;
			int cnt_AddStr;
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbText::desc())){
					AcDbText* pText = AcDbText::cast(pEnt);
					objId = pText->textStyle();
					m_Height = pText->height();
					m_HeightStr.Format(_T("%.3f"), m_Height);
					SetDlgItemText(IDC_AZT_EDTHEIGHT, m_HeightStr);
					iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Height"), m_HeightStr);
					m_Width = pText->widthFactor();
					m_WidthStr.Format(_T("%.3f"), m_Width);
					SetDlgItemText(IDC_AZT_EDTWIDTH, m_WidthStr);
					iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Width"), m_WidthStr);
					AcDbTextStyleTableRecord *pTSTR = NULL;
					acdbOpenObject(pTSTR, objId, AcDb::kForRead);
					pTSTR->getName(Name);
					pTSTR->close();
					pText->close();
					for(int i = 0; i < m_cbxTextType.GetCount(); i++){
						if(m_cbxTextType.FindString(i, Name) == -1)
							m_cbxTextType.AddString(Name);
					}
					cbxTextInt = m_cbxTextType.FindString(0, Name);
					m_cbxTextType.SetCurSel(cbxTextInt);
					CmbTextStyle.Format(_T("%s"), Name);
					cbxTextInt_Str.Format(_T("%d"), cbxTextInt);
					iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("cbxTextType"), cbxTextInt_Str);
					iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("CmbTextStyle"), CmbTextStyle);
					break;
				}
				else{
					strMsg.LoadStringW(IDS_CHECK_STRING); // _T("**선택한 객체는 문자가 아닙니다.**")
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();
			acedSSFree(en);
		}
	}
	UpdateData(FALSE);
}

//문자 높이
void CAZT::OnEnUpdateAztEdtheight(){
	UpdateData(TRUE);
	m_Height = _tstof(m_HeightStr);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Height"), m_HeightStr);
	UpdateData(FALSE);
}

//문자 폭
void CAZT::OnEnUpdateAztEdtwidth(){
	UpdateData(TRUE);
	m_Width = _tstof(m_WidthStr);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Width"), m_WidthStr);
	UpdateData(FALSE);
}

//초단위 표기 소수점
void CAZT::OnEnUpdateAztDot(){
	UpdateData(TRUE);
	m_Dot = _ttoi(m_DotStr);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("Dot"), m_DotStr);
	UpdateData(FALSE);
}

//초단위 표기 소수점 Spin
void CAZT::OnDeltaposAztSpin(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0)
		m_Dot++;
	else
		m_Dot--;
	SetDlgItemInt(IDC_AZT_DOT, m_Dot);
	*pResult = 0;
}

//콤보박스 방위각 문자 레이어
void CAZT::OnCbnSelchangeAztCbxlayer2(){
	UpdateData(TRUE);
	cbxTextLayerInt = m_cbxTextLayer.GetCurSel();
	if(cbxTextLayerInt == -1)cbxTextLayerInt = 0;
	m_cbxTextLayer.SetCurSel(cbxTextLayerInt);
	cbxTextLayerInt_Str.Format(_T("%d"), cbxTextLayerInt);
	m_cbxTextLayer.GetLBText(cbxTextLayerInt, CmbTextLayer);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("cbxTextLayer"), cbxTextLayerInt_Str);
	iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("CmbTextLayer"), CmbTextLayer);
	UpdateData(FALSE);
}

//방위각 문자 레이어 추출버튼
void CAZT::OnBnClickedAztBselect3(){
	UpdateData(TRUE);
	ads_name en;
	ads_point pt;
	AcCmColor Color;
	int cnt_AddStr;
	CString Name, strMsg;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		strMsg.LoadStringW(IDS_LAYER_SELECT); // _T("\n레이어를 가져올 객체 선택 : ")
		int stat = acedEntSel(strMsg, en, pt);
		if(stat == RTCAN || stat == RTNONE)
			break;
		else if(stat == RTERROR){
			resbuf zValueStr;
			ads_getvar(_T("ERRNO"), &zValueStr);
			if(zValueStr.resval.rint == 52)
				break;
			else
				continue;
		}
		else if(stat == RTNORM){
			AcDbObjectId objId;
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbEntity::desc())){
					AcDbEntity* pEntity = AcDbEntity::cast(pEnt);
					Name = pEntity->layer();
					pEntity->close();
					for(int i = 0; i < m_cbxTextLayer.GetCount(); i++){
						if(m_cbxTextLayer.FindString(i, Name) == -1)
							m_cbxTextLayer.AddString(Name);
					}
					cbxTextLayerInt = m_cbxTextLayer.FindString(0, Name);
					m_cbxTextLayer.SetCurSel(cbxTextLayerInt);
					CmbTextLayer.Format(_T("%s"), Name);
					cbxTextLayerInt_Str.Format(_T("%d"), cbxTextLayerInt);
					iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("cbxTextLayer"), cbxTextLayerInt_Str);
					iniFile.writeINI(_T("config.ini"), _T("AZT"), _T("CmbTextLayer"), CmbTextLayer);
					break;
				}
				else{
					strMsg.LoadStringW(IDS_SELECT_TEXT); // _T("**객체가 아닙니다.**")
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();
			acedSSFree(en);	
		}
	}
	UpdateData(FALSE);
}

////각도를 라디안 값으로 변환하여 리턴한다.
double CAZT::DTR(double angle){
	return (angle / 180.0 * PI);
}

////라디안 값을 각도로 변환하여 리턴한다.
double CAZT::RTD(double radian){
	return (radian * 180.0 / PI);
}

bool CAZT::CreateLine(AcGePoint3d spt, AcGePoint3d ept, CString Layer){
	Acad::ErrorStatus es;
	AcDbObjectId pLineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();

	if(pDb == NULL)
		return false;

	AcDbLine* pLine = new AcDbLine(spt,ept);
	pLine->setLayer(Layer);
	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(pLineObjId, pLine);
			pBTR->close();
			pLine->close();
		}
		else
			return false;
		pBT->close();
	}
	else
		return false;
	pLine->close();
	return true;
}

//텍스트생성 
bool CAZT::CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer,CString Style, int mode){
	Acad::ErrorStatus es;
	AcDbObjectId textStyleId, pTextObjId;
	double XScale = 0, obliquingAngle = 0;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return false;

	AcDbText* pText = new AcDbText();
	textStyleId = pDb->textstyle();
	pText->setTextStyle(findTextStyleName(Style));
	AcDbTextStyleTableRecord *pTSTR = NULL;
	AcDbBlockTable* pBT;
	AcDbBlockTableRecord* pBTR;
	acdbOpenObject(pTSTR, textStyleId, AcDb::kForRead);
	if(pTSTR){
		XScale=pTSTR->xScale();
		obliquingAngle = pTSTR->obliquingAngle();
		pTSTR->close();
		pText->setWidthFactor(XScale);
	}
	pText->setOblique(obliquingAngle);
	pText->setLayer(Layer);
	pText->setPosition(pt);
	if(mode == 1){
		es = pText->setVerticalMode(AcDb::kTextBottom);
		es = pText->setHorizontalMode(AcDb::kTextCenter);
	}
	else{
		es = pText->setVerticalMode(AcDb::kTextTop);
		es = pText->setHorizontalMode(AcDb::kTextCenter);
	}
	pText->setAlignmentPoint(pt);
	pText->setHeight(Height);
	pText->setRotation(Rotate);
	pText->setTextString((LPCTSTR)TxtStr);
	AcDbExtents mExtText;
	pText->getGeomExtents(mExtText);
	TextDist = mExtText.maxPoint().x - mExtText.minPoint().x;
	TextHeight = mExtText.maxPoint().y - mExtText.minPoint().y;
	MaxPt = mExtText.maxPoint();
	MinPt = mExtText.minPoint();
	MaxPt.y = MaxPt.y - abs(MaxPt.y - MinPt.y);
	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		resbuf rb;
		acedGetVar(_T("tilemode"), &rb);
		if (rb.resval.rint == 1) {
			if (pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk) {
				es = pBTR->appendAcDbEntity(pTextObjId, pText);
				pBTR->close();
				pText->close();
			}
		} else {
			if (pBT->getAt(ACDB_PAPER_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk) {
				es = pBTR->appendAcDbEntity(pTextObjId, pText);
				pBTR->close();
				pText->close();
			}
		}
		pBT->close();
	}
	return true;
}

//솔리드 생성
bool CAZT::CreateSolid(AcGePoint3d Solpt1, AcGePoint3d Solpt2, AcGePoint3d Solpt3, CString Layer){
	Acad::ErrorStatus es;
	AcDbObjectId SolidId;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return false;

	AcDbBlockTable* pBT;
	AcDbBlockTableRecord* pBTR=NULL;
	AcDbSolid* Arrow = new AcDbSolid(Solpt1, Solpt2, Solpt3, Solpt3);
	Arrow->setLayer(Layer);
	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		resbuf rb;
		acedGetVar(_T("tilemode"), &rb);
		if (rb.resval.rint == 1) {
			if (pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk) {
				es = pBTR->appendAcDbEntity(SolidId, Arrow);
				pBTR->close();
				Arrow->close();
			}
		} else {
			if (pBT->getAt(ACDB_PAPER_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk) {
				es = pBTR->appendAcDbEntity(SolidId, Arrow);
				pBTR->close();
				Arrow->close();
			}
		}
		pBT->close();
	}
	return true;
}

AcDbObjectId CAZT::findTextStyleName(CString name){
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
		if(wcscmp(name, name2) == 0)
			return TextId;
	}
	delete pTextTblIter;
	delete pTextTbl;
	return NULL;
}

extern CAZT *pAZT;
void CAZT::PostNcDestroy(){
	delete this;
	pAZT = NULL;
}

//floor함수를 이용한 반올림 함수
int CAZT::round(double x){
	double d = 0;
	d = floor(x + 0.00005);
	return d;
}

void CAZT::OnBnClickedAztHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/AZT.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
