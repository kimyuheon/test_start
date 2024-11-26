#include "stdafx.h"
#include "STT.h"
#include "afxdialogex.h"

// CSTT 대화 상자입니다.
IMPLEMENT_DYNAMIC(CSTT, CAcUiDialog)
	CSTT::CSTT(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CSTT::IDD, pParent), b_RadBtn(0), m_ScaleStr(_T("")), m_STLengthStr(_T("")), m_DistStr(_T("")), m_HeightStr(_T("")), m_WidthStr(_T("")), m_DotStr(_T("")), m_Dist2Str(_T("")), m_OffsetStr(_T("")), cbxLine_Str(_T("")), cbxText_Str(_T("")), cbxDot_Str(_T("")){
}

CSTT::~CSTT(){
}

void CSTT::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_STT_RADSTATION, b_RadBtn);
	DDX_Control(pDX, IDC_STT_CBXTEXTTYPE, m_cbxTextType);
	DDX_Control(pDX, IDC_STT_CBXDOT, m_cbxDot);
	DDX_Control(pDX, IDC_STT_CBXLINE, m_cbxLineLayer);
	DDX_Control(pDX, IDC_STT_CBXTEXT, m_cbxTextLayer);
	DDX_Control(pDX, IDC_STT_CBXLAYERDOT, m_cbxDotLayer);
	DDX_Text(pDX, IDC_STT_EDTSCALE, m_ScaleStr);
	DDX_Text(pDX, IDC_STT_EDTSTLENGTH, m_STLengthStr);
	DDX_Text(pDX, IDC_STT_EDTDIST, m_DistStr);
	DDX_Text(pDX, IDC_STT_EDTHEIGHT, m_HeightStr);
	DDX_Text(pDX, IDC_STT_EDTWIDTH, m_WidthStr);
	DDX_Text(pDX, IDC_STT_EDTDOT, m_DotStr);
	DDX_Text(pDX, IDC_STT_EDTDIST2, m_Dist2Str);
	DDX_Text(pDX, IDC_STT_EDTOFFSET, m_OffsetStr);
	DDX_Control(pDX, IDC_STT_BSELECT1, m_bSelect1);
	DDX_Control(pDX, IDC_STT_BSELECT2, m_bSelect2);
	DDX_Control(pDX, IDC_STT_BSELECT3, m_bSelect3);
	DDX_Control(pDX, IDC_STT_BSELECT4, m_bSelect4);
	DDX_Control(pDX, IDC_STT_CHECK1, m_Chk1_btn);
	DDX_Control(pDX, IDC_STT_CHECK2, m_Chk2_btn);
	DDX_Control(pDX, IDC_STT_CHECK3, m_Chk3_btn);
	DDX_Control(pDX, IDC_STT_CHECK4, m_Chk4_btn);
}

BEGIN_MESSAGE_MAP(CSTT, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CSTT::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSTT::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_STT_RADSTATION, &CSTT::OnBnClickedSttRadstation)
	ON_BN_CLICKED(IDC_STT_RADNUMBER, &CSTT::OnBnClickedSttRadnumber)
	ON_EN_UPDATE(IDC_STT_EDTSCALE, &CSTT::OnEnUpdateSttEdtscale)
	ON_EN_UPDATE(IDC_STT_EDTSTLENGTH, &CSTT::OnEnUpdateSttEdtstlength)
	ON_EN_UPDATE(IDC_STT_EDTDIST, &CSTT::OnEnUpdateSttEdtdist)
	ON_EN_UPDATE(IDC_STT_EDTHEIGHT, &CSTT::OnEnUpdateSttEdtheight)
	ON_EN_UPDATE(IDC_STT_EDTWIDTH, &CSTT::OnEnUpdateSttEdtwidth)
	ON_EN_UPDATE(IDC_STT_EDTDOT, &CSTT::OnEnUpdateSttEdtdot)
	ON_EN_UPDATE(IDC_STT_EDTDIST2, &CSTT::OnEnUpdateSttEdtdist2)
	ON_EN_UPDATE(IDC_STT_EDTOFFSET, &CSTT::OnEnUpdateSttEdtoffset)
	ON_CBN_SELCHANGE(IDC_STT_CBXLINE, &CSTT::OnCbnSelchangeSttCbxline)
	ON_CBN_SELCHANGE(IDC_STT_CBXTEXT, &CSTT::OnCbnSelchangeSttCbxtext)
	ON_CBN_SELCHANGE(IDC_STT_CBXLAYERDOT, &CSTT::OnCbnSelchangeSttCbxlayerdot)
	ON_CBN_SELCHANGE(IDC_STT_CBXTEXTTYPE, &CSTT::OnCbnSelchangeSttCbxtexttype)
	ON_CBN_SELCHANGE(IDC_STT_CBXDOT, &CSTT::OnCbnSelchangeSttCbxdot)
	ON_BN_CLICKED(IDC_STT_BSELECT1, &CSTT::OnBnClickedSttBselect1)
	ON_BN_CLICKED(IDC_STT_BSELECT2, &CSTT::OnBnClickedSttBselect2)
	ON_BN_CLICKED(IDC_STT_BSELECT3, &CSTT::OnBnClickedSttBselect3)
	ON_BN_CLICKED(IDC_STT_BSELECT4, &CSTT::OnBnClickedSttBselect4)
	ON_BN_CLICKED(IDC_STT_CHECK1, &CSTT::OnBnClickedSttCheck1)
	ON_BN_CLICKED(IDC_STT_CHECK2, &CSTT::OnBnClickedSttCheck2)
	ON_BN_CLICKED(IDC_STT_CHECK3, &CSTT::OnBnClickedSttCheck3)
	ON_BN_CLICKED(IDC_STT_CHECK4, &CSTT::OnBnClickedSttCheck4)
	ON_BN_CLICKED(ID_STT_HELP, &CSTT::OnBnClickedSttHelp)
END_MESSAGE_MAP()

BOOL CSTT::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	CreateLayer(_T("CD-LEAL"), 1);
	CreateLayer(_T("CD-LEAT"), 3);
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	m_bSelect1.AutoLoad();
	m_bSelect2.AutoLoad();
	m_bSelect3.AutoLoad();
	m_bSelect4.AutoLoad();

	//글꼴 불러오기
	AcDbTextStyleTable* pTextTbl = NULL;
	AcDbTextStyleTablePointer pTextPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbTextStyleTableIterator* pTextTblIter;
	pTextPoint->newIterator(pTextTblIter);

	AcDbObjectId TextId;
	for(pTextTblIter->start(); !pTextTblIter->done(); pTextTblIter->step()){
		pTextTblIter->getRecordId(TextId);
		AcDbTextStyleTableRecordPointer pTextTblRcd(TextId, AcDb::kForRead);
		TCHAR* TextName;
		pTextTblRcd->getName(TextName);
		m_cbxTextType.AddString(TextName);
	}
	delete pTextTblIter;
	delete pTextTbl;

	//소수점
	m_cbxDot.AddString(_T("0"));
	m_cbxDot.AddString(_T("1"));
	m_cbxDot.AddString(_T("2"));
	m_cbxDot.AddString(_T("3"));
	m_cbxDot.AddString(_T("4"));

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
		m_cbxLineLayer.AddString(name);
		m_cbxTextLayer.AddString(name);
		m_cbxDotLayer.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;

	//콤보박스 글꼴
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("cbxTextType"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_cbxTextType.SetCurSel(0);
	}
	else{
		SetDlgItemText(IDC_STT_CBXTEXTTYPE, tempStr);
		cbxInt = _ttoi(tempStr);
		m_cbxTextType.SetCurSel(cbxInt);
	}
	//콤보박스 소수점
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("cbxDot"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_cbxDot.SetCurSel(0);
	}
	else{
		SetDlgItemText(IDC_STT_CBXDOT, tempStr);
		cbxInt = _ttoi(tempStr);
		m_cbxDot.SetCurSel(cbxInt);
	}
	//콤보박스 선 레이어
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("cbxLineLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_cbxLineLayer.SetCurSel(0);
	}
	else{
		SetDlgItemText(IDC_STT_CBXLINE, tempStr);
		cbxInt = _ttoi(tempStr);
		m_cbxLineLayer.SetCurSel(cbxInt);
	}
	//콤보박스 문자 레이어
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("cbxTextLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_cbxTextLayer.SetCurSel(0);
	}
	else{
		SetDlgItemText(IDC_STT_CBXTEXT, tempStr);
		cbxInt = _ttoi(tempStr);
		m_cbxTextLayer.SetCurSel(cbxInt);
	}
	//콤보박스 Dot 레이어
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("cbxDotLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_cbxDotLayer.SetCurSel(0);
	}
	else{
		SetDlgItemText(IDC_STT_CBXLAYERDOT, tempStr);
		cbxInt = _tstoi(tempStr);
		m_cbxDotLayer.SetCurSel(cbxInt);
	}
	//측점 표기 STA = 1	 NO = 2
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("JOBMODE"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){
		OnBnClickedSttRadstation();
	}
	else{
		OnBnClickedSttRadnumber();
	}
	//축척
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Scale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_STT_EDTSCALE, _T("1000"));
	else
		SetDlgItemText(IDC_STT_EDTSCALE, tempStr);
	//시작 거리
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("STLength"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_STT_EDTSTLENGTH, _T("0"));
	else
		SetDlgItemText(IDC_STT_EDTSTLENGTH, tempStr);
	//체크박스 시작 거리
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Chk_STLength"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("FALSE")) == 0){
		b_Chk1 = false;
		m_Chk1_btn.SetCheck(FALSE);
		OnBnClickedSttCheck1();
	}
	else{
		b_Chk1 = true;
		m_Chk1_btn.SetCheck(TRUE);
		OnBnClickedSttCheck1();
	}
	//측점 간격
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Dist"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_STT_EDTDIST, _T("20.00"));
	else
		SetDlgItemText(IDC_STT_EDTDIST, tempStr);
	//크기
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Height"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_STT_EDTHEIGHT, _T("3.000"));
	else
		SetDlgItemText(IDC_STT_EDTHEIGHT, tempStr);
	//폭
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Width"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_STT_EDTWIDTH, _T("1.000"));
	else
		SetDlgItemText(IDC_STT_EDTWIDTH, tempStr);
	//Dot 표기 직경
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Dot"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_STT_EDTDOT, _T("1.000"));
	else
		SetDlgItemText(IDC_STT_EDTDOT, tempStr);
	//Dot 표기 직경 체크박스
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Chk_Dot"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){
		b_Chk2 = true;
		m_Chk2_btn.SetCheck(TRUE);
		OnBnClickedSttCheck2();
	}
	else{
		b_Chk2 = false;
		m_Chk2_btn.SetCheck(FALSE);
		OnBnClickedSttCheck2();
	}
	//중심~인출선 길이
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Dist2"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_STT_EDTDIST2, _T("60.00"));
	else
		SetDlgItemText(IDC_STT_EDTDIST2, tempStr);
	//중심~인출선 길이,offset 체크박스
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Chk_Dst2Offset"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("TRUE")) == 0 ){
		b_Chk3 = true;
		m_Chk3_btn.SetCheck(TRUE);
		OnBnClickedSttCheck3();
	}
	else{
		b_Chk3 = false;
		m_Chk3_btn.SetCheck(FALSE);
		OnBnClickedSttCheck3();
	}
	//중심~인출선 Offset
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Offset"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_STT_EDTOFFSET, _T("20.00"));
	else
		SetDlgItemText(IDC_STT_EDTOFFSET, tempStr);

	//표준 레이어 체크박스
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Chk_StandLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){
		b_Chk4 = true;
		m_Chk4_btn.SetCheck(TRUE);
		OnBnClickedSttCheck4();
	}
	else{
		b_Chk4 = false;
		m_Chk4_btn.SetCheck(FALSE);
		OnBnClickedSttCheck4();
	}
	return TRUE;
}

void CSTT::OnBnClickedOk(){
	UpdateData();
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	CSTT STT;
	CString modeStr, strMsg;
	if(!(JobMode == 1 || JobMode == 2))
		JobMode = 1;

	modeStr.Format(_T("%d"), JobMode);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("JOBMODE"), modeStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Scale"), m_ScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("STLength"), m_STLengthStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Dist"), m_DistStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Height"), m_HeightStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Width"), m_WidthStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Dot"), m_DotStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Dist2"), m_Dist2Str);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Offset"), m_OffsetStr);

	AcGePoint3d LinePt, Spt, Ept, ClosePt, AnglePt;
	double ClosePt_dst, dist, CloseStaPt_dst = 0;//선형 길이
	ads_name ens;
	while(TRUE){
		acedInitGet(0, _T(""));
		CString RTERROR_str;
		strMsg.LoadStringW(IDS_LINE_SELECT); // "선형 선택 : "
		int stat = acedEntSel(strMsg, ens, asDblArray(LinePt));
		if(stat == RTCAN){
			CompleteEditorCommand();
			return;
		}
		else if(stat == RTERROR){
			resbuf zValueStr;
			ads_getvar(_T("ERRNO"), &zValueStr);// aced
			if(zValueStr.resval.rint == 52){//52  = NULL(ERRNO) Enter시
				CompleteEditorCommand();
				return;
			}
			else 
				continue;
		}
		else if(stat == RTKWORD){
			continue;
		}
		else if(stat == RTNORM){//진행방향 화살표때문에 작성(곡선, 스플라인 등에서 EntSel로 지정한 점을 기준으로 만들어야함)
			AcDbObjectId objId;
			acdbGetObjectId(objId, ens);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbLine::desc())){
					AcDbLine* pLine = AcDbLine::cast(pEnt);
					pLine->getClosestPointTo(LinePt, ClosePt);
					pLine->getDistAtPoint(ClosePt, ClosePt_dst);
					pLine->getPointAtDist(ClosePt_dst + 0.1, AnglePt);
					pLine->close();
				}
				else if(pEnt->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline* pPLine = AcDbPolyline::cast(pEnt);
					pPLine->getClosestPointTo(LinePt, ClosePt);
					pPLine->getDistAtPoint(ClosePt, ClosePt_dst);
					pPLine->getPointAtDist(ClosePt_dst + 0.1, AnglePt);
					pPLine->close();
				}
				else if(pEnt->isKindOf(AcDbArc::desc())){
					AcDbArc* pArc = AcDbArc::cast(pEnt);
					pArc->getClosestPointTo(LinePt, ClosePt);
					pArc->getDistAtPoint(ClosePt, ClosePt_dst);
					pArc->getPointAtDist(ClosePt_dst + 0.1, AnglePt);
					pArc->close();
				}
				else if(pEnt->isKindOf(AcDbSpline::desc())){
					AcDbSpline* pSPline = AcDbSpline::cast(pEnt);
					pSPline->getClosestPointTo(LinePt, ClosePt);
					pSPline->getDistAtPoint(ClosePt, ClosePt_dst);
					pSPline->getPointAtDist(ClosePt_dst + 0.1, AnglePt);
					pSPline->close();
				}
				else{
					strMsg.LoadStringW(IDS_SELECT_CHK_LINE); // "**선택한 객체는 선형이 아닙니다.**"
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();
			break;
		}
	}//while end
	acedInitGet(0, _T("Y y N n"));
	CString Dir_str, RTERROR_str, DirYN_str, YNDirect, tempKword, NoTDirect_str;
	AcGePoint3d Stapt, Close_Stapt, LWpt, AnglePt1, AnglePt2, L_LWspt, L_LWept, R_LWspt, R_LWept, Close_LWpt, L_DotPt1, L_DotPt2, R_DotPt1, R_DotPt2;
	AcGePoint3d L_Textpt, R_Textpt, L_Textpt1, R_Textpt1;
	double StaReal = 0.0, YNDirect_f = 0.0, NoTDirect_f = 0.0;
	TCHAR kWord[30], SurptKword[30];

	//가상 방향화살표
	struct resbuf* sys1 = NULL,*sys2 = NULL;
	ads_real do1 = 0.0, vs = 0.0, ss = 0.0;
	sys1 = ads_newrb(RTREAL);    
	sys2 = ads_newrb(RT3DPOINT);
	ads_getvar(_T("VIEWSIZE"), sys1); 
	ads_getvar(_T("SCREENSIZE"), sys2);
	vs = sys1->resval.rreal;
	ss = sys2->resval.rpoint[Y];
	do1 = vs / ss;
	ads_polar(asDblArray(ClosePt), ads_angle(asDblArray(ClosePt), asDblArray(AnglePt)) + DTR(160.0), 50 * do1, asDblArray(AnglePt1));
	ads_polar(asDblArray(ClosePt), ads_angle(asDblArray(ClosePt), asDblArray(AnglePt)) - DTR(160.0), 50 * do1, asDblArray(AnglePt2));
	acedGrDraw(asDblArray(AnglePt1), asDblArray(ClosePt), 1, 0);
	acedGrDraw(asDblArray(AnglePt2), asDblArray(ClosePt), 1, 0);

	while(true){//선형 진행방향 Yes or No
		CString tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("YNDirect"));
		if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("Y")) == 0)
			tempStr.Format(_T("Y"));
		else
			tempStr;

		if(tempStr == _T("Y"))
			YNDirect.Format(_T("Yes(Y)"));
		else if(tempStr == _T("N"))
			YNDirect.Format(_T("No(N)"));

		strMsg.LoadStringW(IDS_LINE_DIRECT_CHK); // "\n선형 진행 방향이 맞습니까? [Yes(Y)/No(N)] <%s> : "
		tempKword.Format(strMsg, YNDirect);
		acedInitGet(0, _T("Y y N n"));
		int stat = acedGetReal(tempKword, &YNDirect_f);
		if(stat == RTCAN){
			acedSSFree(ens);
			CompleteEditorCommand();
			return;
		}
		else if(stat == RTNONE){
			DirYN_str = tempStr;
			break;
		}
		else if(stat == RTNORM){
			continue;
		}
		else if(stat == RTKWORD){
			if(acedGetInput(kWord) != RTNORM)
				STT.DoModal();
			if(!_tcscmp(_T("Y"),kWord) || !_tcscmp(_T("y"),kWord))
				DirYN_str.Format(kWord);
			else if(!_tcscmp(_T("N"),kWord) || !_tcscmp(_T("n"),kWord))
				DirYN_str.Format(kWord);

			iniFile.writeINI(_T("config.ini"), _T("STT"), _T("YNDirect"), DirYN_str);	
			break;
		}
	}
	if(b_Chk1 == true){//도면에서 지정 체크시
		strMsg.LoadStringW(IDS_CIVIL_BASEPT); // "\n측점 기준이 될 점 선택 : "
		if(acedGetPoint(0, strMsg, asDblArray(Stapt)) != RTNORM){
			acedSSFree(ens);
			CompleteEditorCommand();
			return;
		}
		strMsg.LoadStringW(IDS_POINT_INPUT); // "\n선택한 점의 측점 입력 : ")
		if(acedGetReal(strMsg, &StaReal) != RTNORM){
			acedSSFree(ens);
			CompleteEditorCommand();
			return;
		}
	}
	int stat;
	AcGePoint3d InputSurNumPt;
	double InputSurNum;
	while(true){//반복작업
		strMsg.LoadStringW(IDS_CIVIL_LEADER_INPUT); // "인출선을 표기할 점 선택 또는 측점 입력 : "
		int InputStat = acedGetPoint(0, strMsg, asDblArray(LWpt));
		if(InputStat == RTCAN || InputStat == RTNONE){
			acedSSFree(ens);
			CompleteEditorCommand();
			return;
		}
		else if(InputStat == RTNORM){
			if(acedGetInput(SurptKword) != RTNORM)
				return;
			ClosePt_dst = _tstof(SurptKword);
		}
		while(true){//인출선 표기 방향 L or R or A
			CString tempStr2 = iniFile.readINI(_T("config.ini"), _T("STT"), _T("NotDirect"));
			if(tempStr2.CompareNoCase(_T("")) == 0 || tempStr2.CompareNoCase(_T("L")) == 0)
				tempStr2.Format(_T("L"));
			else
				tempStr2;

			if (tempStr2 == _T("L")) {
				strMsg.LoadStringW(IDS_LEFT); // "좌(L)"
				NoTDirect_str.Format(strMsg);
			} else if (tempStr2 == _T("R")) {
				strMsg.LoadStringW(IDS_RIGHT); // "우(R)"
				NoTDirect_str.Format(strMsg);
			} else if (tempStr2 == _T("A")) {
				strMsg.LoadStringW(IDS_EVERYTHING); // "모두(A)"
				NoTDirect_str.Format(strMsg);
			}

			acedInitGet(0, _T("L l R r A a"));
			strMsg.LoadStringW(IDS_CIVIL_LEADER_DIRECT); // "\n인출선 표기 방향 [좌(L)/우(R)/모두(A)] <%s> : "
			tempKword.Format(strMsg, NoTDirect_str);
			stat = acedGetReal(tempKword, &NoTDirect_f);
			if(stat == RTCAN){
				acedSSFree(ens);
				CompleteEditorCommand();
				return;
			}
			else if(stat == RTNONE){
				Dir_str = tempStr2;
				iniFile.writeINI(_T("config.ini"), _T("STT"), _T("NotDirect"), Dir_str);
				break;
			}
			else if(stat == RTNORM){
				continue;
			}
			else if(stat == RTKWORD){
				if(acedGetInput(kWord) != RTNORM)STT.DoModal();
				if(!_tcscmp(_T("L"),kWord) || !_tcscmp(_T("l"),kWord))
					Dir_str.Format(kWord);
				else if(!_tcscmp(_T("R"),kWord) || !_tcscmp(_T("r"),kWord))
					Dir_str.Format(kWord);
				else if(!_tcscmp(_T("A"),kWord) || !_tcscmp(_T("a"),kWord))
					Dir_str.Format(kWord);

				iniFile.writeINI(_T("config.ini"), _T("STT"), _T("NotDirect"), Dir_str);
				break;
			}
		}//while end
		AcDbObjectId objId;
		acdbGetObjectId(objId, ens);
		AcDbEntityPointer pEnt(objId, AcDb::kForRead);
		if(pEnt.openStatus() == Acad::eOk){
			if(pEnt->isKindOf(AcDbLine::desc())){
				AcDbLine* pLine = AcDbLine::cast(pEnt);
				if(b_Chk1 == true){//도면에서 지정 체크시 기준 점
					pLine->getClosestPointTo(Stapt, Close_Stapt);
					pLine->getDistAtPoint(Close_Stapt, CloseStaPt_dst);
				}
				if(InputStat == RTNORM){
					if(0 != _tcscmp(_T(""), SurptKword)){//사용자가 측점값 입력시
						ClosePt_dst = ClosePt_dst + CloseStaPt_dst;
						pLine->getPointAtDist(ClosePt_dst, Close_LWpt);
					}
					else{
						pLine->getClosestPointTo(LWpt, Close_LWpt);
						pLine->getDistAtPoint(Close_LWpt, ClosePt_dst);
					}
				}
				pLine->getStartPoint(Spt);
				pLine->getEndPoint(Ept);
				pLine->getPointAtDist(ClosePt_dst + 0.1, AnglePt);
				pLine->getDistAtPoint(Ept, dist);//전체 길이
				pLine->close();
			}
			else if(pEnt->isKindOf(AcDbPolyline::desc())){
				AcDbPolyline* pPLine = AcDbPolyline::cast(pEnt);
				if(b_Chk1 == true){//도면에서 지정 체크시 기준 점
					pPLine->getClosestPointTo(Stapt, Close_Stapt);
					pPLine->getDistAtPoint(Close_Stapt, CloseStaPt_dst);
				}
				if(InputStat == RTNORM){
					if(0 != _tcscmp(_T(""), SurptKword)){//사용자가 측점값 입력시
						ClosePt_dst = ClosePt_dst + CloseStaPt_dst;
						pPLine->getPointAtDist(ClosePt_dst, Close_LWpt);
					}
					else{
						pPLine->getClosestPointTo(LWpt, Close_LWpt);
						pPLine->getDistAtPoint(Close_LWpt, ClosePt_dst);
					}
				}
				pPLine->getStartPoint(Spt);
				pPLine->getEndPoint(Ept);
				pPLine->getPointAtDist(ClosePt_dst + 0.1, AnglePt);
				pPLine->getDistAtPoint(Ept, dist);
				pPLine->close();
			}
			else if(pEnt->isKindOf(AcDbArc::desc())){
				AcDbArc* pArc = AcDbArc::cast(pEnt);
				if(b_Chk1 == true){//도면에서 지정 체크시 기준 점
					pArc->getClosestPointTo(Stapt, Close_Stapt);
					pArc->getDistAtPoint(Close_Stapt, CloseStaPt_dst);
				}
				if(InputStat == RTNORM){
					if(0 != _tcscmp(_T(""), SurptKword)){//사용자가 측점값 입력시
						ClosePt_dst = ClosePt_dst + CloseStaPt_dst;
						pArc->getPointAtDist(ClosePt_dst, Close_LWpt);
					}
					else{
						pArc->getClosestPointTo(LWpt, Close_LWpt);
						pArc->getDistAtPoint(Close_LWpt, ClosePt_dst);
					}
				}
				pArc->getStartPoint(Spt);
				pArc->getEndPoint(Ept);
				pArc->getPointAtDist(ClosePt_dst + 0.1, AnglePt);
				pArc->getDistAtPoint(Ept, dist);
				pArc->close();
			}
			else if(pEnt->isKindOf(AcDbSpline::desc())){
				AcDbSpline* pSPline = AcDbSpline::cast(pEnt);
				if(b_Chk1 == true){//도면에서 지정 체크시 기준 점
					pSPline->getClosestPointTo(Stapt, Close_Stapt);
					pSPline->getDistAtPoint(Close_Stapt, CloseStaPt_dst);
				}
				if(InputStat == RTNORM){
					if(0 != _tcscmp(_T(""), SurptKword)){//사용자가 측점값 입력시
						ClosePt_dst = ClosePt_dst + CloseStaPt_dst;
						pSPline->getPointAtDist(ClosePt_dst, Close_LWpt);
					}
					else{
						pSPline->getClosestPointTo(LWpt, Close_LWpt);
						pSPline->getDistAtPoint(Close_LWpt, ClosePt_dst);
					}
				}
				pSPline->getStartPoint(Spt);
				pSPline->getEndPoint(Ept);
				pSPline->getPointAtDist(ClosePt_dst + 0.1, AnglePt);
				pSPline->getDistAtPoint(Ept, dist);
				pSPline->close();
			}
			else{
				strMsg.LoadStringW(IDS_SELECT_CHK_LINE); // "\n**선택한 객체는 선형이 아닙니다.**"
				acutPrintf(strMsg);
				continue;
			}
		}
		pEnt->close();

		CString str_Dotlayer, str_LineLayer, str_Textlayer;
		if(m_StandLayer == _T("0")){
			m_cbxDotLayer.GetLBText(this->m_cbxDotLayer.GetCurSel(), str_Dotlayer);		//Dot 콤보박스 선택 텍스트가져오기
			m_cbxLineLayer.GetLBText(this->m_cbxLineLayer.GetCurSel(), str_LineLayer);	//선 콤보박스 선택 텍스트가져오기
			m_cbxTextLayer.GetLBText(this->m_cbxTextLayer.GetCurSel(), str_Textlayer);	//문자 콤보박스 선택 텍스트가져오기
		}
		else if(m_StandLayer == _T("1")){
			str_Dotlayer = _T("CD-LEAL");
			str_LineLayer = _T("CD-LEAL");
			str_Textlayer = _T("CD-LEAT");
		}

		double STAang = ads_angle(asDblArray(Close_LWpt), asDblArray(AnglePt));//LWpt근처점 각도
		ads_polar(asDblArray(Close_LWpt), STAang + DTR(90.0), m_Offset, asDblArray(L_LWspt));
		ads_polar(asDblArray(Close_LWpt), STAang + DTR(90.0), m_Dist2, asDblArray(L_LWept)); 
		ads_polar(asDblArray(Close_LWpt), STAang - DTR(90.0), m_Offset, asDblArray(R_LWspt));
		ads_polar(asDblArray(Close_LWpt), STAang - DTR(90.0), m_Dist2, asDblArray(R_LWept));
		ads_polar(asDblArray(Close_LWpt), STAang + DTR(90.0), (m_Dist2 - 1), asDblArray(L_Textpt1));
		ads_polar(asDblArray(Close_LWpt), STAang - DTR(90.0), (m_Dist2 - 1), asDblArray(R_Textpt1));
		ads_polar(asDblArray(L_LWept), STAang + DTR(90.0), m_Dot / 4, asDblArray(L_DotPt1));
		ads_polar(asDblArray(L_LWept), STAang - DTR(90.0), m_Dot / 4, asDblArray(L_DotPt2));
		ads_polar(asDblArray(R_LWept), STAang - DTR(90.0), m_Dot / 4, asDblArray(R_DotPt1));
		ads_polar(asDblArray(R_LWept), STAang + DTR(90.0), m_Dot / 4, asDblArray(R_DotPt2));
		if(DirYN_str == _T("Y") || DirYN_str == _T("y")){
			ads_polar(asDblArray(L_Textpt1), STAang, -1, asDblArray(L_Textpt));
			ads_polar(asDblArray(R_Textpt1), STAang, -1, asDblArray(R_Textpt));
		}
		else{
			ads_polar(asDblArray(L_Textpt1), STAang, +1, asDblArray(L_Textpt));
			ads_polar(asDblArray(R_Textpt1), STAang, +1, asDblArray(R_Textpt));
		}

		if(b_Chk3 == true){//인출선 표기 체크시
			if(DirYN_str == _T("Y") || DirYN_str == _T("y")){
				if(Dir_str == _T("L") || Dir_str == _T("l"))
					CreateLine(L_LWspt, L_LWept, _T("BYLAYER"), 1, str_LineLayer);
				else if(Dir_str == _T("R") || Dir_str == _T("r"))
					CreateLine(R_LWspt, R_LWept, _T("BYLAYER"), 1, str_LineLayer);
			}
			else{
				if(Dir_str == _T("L") || Dir_str == _T("l"))
					CreateLine(R_LWspt, R_LWept, _T("BYLAYER"), 1, str_LineLayer);
				else if(Dir_str == _T("R") || Dir_str == _T("r"))
					CreateLine(L_LWspt, L_LWept, _T("BYLAYER"), 1, str_LineLayer);
			}
			if(Dir_str == _T("A") || Dir_str == _T("a")){
				CreateLine(R_LWspt, R_LWept, _T("BYLAYER"), 1, str_LineLayer);
				CreateLine(L_LWspt, L_LWept, _T("BYLAYER"), 1, str_LineLayer);
			}
		}
		if(b_Chk2 == true){//Dot 표기직경 체크시
			if(DirYN_str == _T("Y") || DirYN_str == _T("y")){
				if(Dir_str == _T("L") || Dir_str == _T("l"))
					CreatePLine(L_DotPt1, L_DotPt2, m_Dot,_T("bylayer"), 1, str_Dotlayer );
				else if(Dir_str == _T("R") || Dir_str == _T("r"))
					CreatePLine(R_DotPt1, R_DotPt2, m_Dot,_T("bylayer"), 1, str_Dotlayer);
			}
			else{
				if(Dir_str == _T("L") || Dir_str == _T("l"))
					CreatePLine(R_DotPt1, R_DotPt2, m_Dot,_T("bylayer"), 1, str_Dotlayer);
				else if(Dir_str == _T("R") || Dir_str == _T("r"))
					CreatePLine(L_DotPt1, L_DotPt2, m_Dot,_T("bylayer"), 1, str_Dotlayer);
			}
			if(Dir_str == _T("A") || Dir_str == _T("a")){
				CreatePLine(L_DotPt1, L_DotPt2, m_Dot,_T("bylayer"), 1, str_Dotlayer);
				CreatePLine(R_DotPt1, R_DotPt2, m_Dot,_T("bylayer"), 1, str_Dotlayer);
			}
		}
		//텍스트 
		CString str_Dot, Survey_Str, DualFormat_str, FirstNum, SecondNum,Num, STAdot_str, NOdot_str;
		m_cbxDot.GetLBText(this->m_cbxDot.GetCurSel(), str_Dot);//콤보박스 선택 텍스트가져오기
		int H_DotNum = _tstof(str_Dot), NO_divvy, STA_divvy;
		double f_Length, STA_Remainder, NO_Remainder, Survey_YESf, Survey_NOf;

		if(DirYN_str == _T("Y") || DirYN_str == _T("y")){
			if(b_Chk1 == true)
				Survey_YESf = ClosePt_dst - CloseStaPt_dst + StaReal;
			else
				Survey_NOf = ClosePt_dst + m_STLength;
		}
		else{
			if(b_Chk1 == true)
				Survey_YESf = (dist - ClosePt_dst) - (dist - CloseStaPt_dst) + StaReal;
			else
				Survey_NOf = (dist - ClosePt_dst) + m_STLength;
		}
		if(DirYN_str == _T("Y") || DirYN_str == _T("y")){
			if(b_Chk1 == true){//도면에서 지정 체크시
				DualFormat_str.Format(_T("%%.%df"), H_DotNum);
				Survey_Str.Format(DualFormat_str, Survey_YESf * 1000 / m_Scale);
				f_Length = _tstof(Survey_Str);
				if(JobMode == 1){
					STA_divvy = f_Length / 1000;
					STA_Remainder = fmod(f_Length, 1000);
					if(STA_divvy < 0 || STA_Remainder < 0){
						STAdot_str.Format(_T("STA.%d%%.%df"), STA_divvy, H_DotNum);
						DualFormat_str.Format(STAdot_str, STA_Remainder);
					}
					else if(STA_Remainder >= 0 || STA_divvy >= 0){
						STAdot_str.Format(_T("STA.%d+%%.%df"), STA_divvy, H_DotNum);
						DualFormat_str.Format(STAdot_str, STA_Remainder);
					}
				}
				else if(JobMode == 2){
					NO_divvy = f_Length / m_Dist;
					NO_Remainder = fmod(f_Length, m_Dist);
					if(NO_divvy < 0 || NO_Remainder < 0){
						NOdot_str.Format(_T("NO.%d+%%.%df"), NO_divvy, H_DotNum);
						DualFormat_str.Format(NOdot_str, -NO_Remainder);
					}
					else if(NO_Remainder >= 0 || NO_divvy >= 0){
						NOdot_str.Format(_T("NO.%d+%%.%df"), NO_divvy, H_DotNum);
						DualFormat_str.Format(NOdot_str, NO_Remainder);
					}
				}
			}
			else{//도면에서 지정 체크X
				DualFormat_str.Format(_T("%%.%df"), H_DotNum);
				Survey_Str.Format(DualFormat_str, Survey_NOf * 1000 / m_Scale);
				f_Length = _tstof(Survey_Str);
				if(JobMode == 1){
					STA_divvy = f_Length / 1000;
					STA_Remainder = fmod(f_Length, 1000);
					STAdot_str.Format(_T("STA.%d+%%.%df"), STA_divvy, H_DotNum);
					DualFormat_str.Format(STAdot_str, STA_Remainder);
				}
				else if(JobMode == 2){
					NO_divvy = f_Length / m_Dist;
					NO_Remainder = fmod(f_Length, m_Dist);
					NOdot_str.Format(_T("NO.%d+%%.%df"), NO_divvy, H_DotNum);
					DualFormat_str.Format(NOdot_str, NO_Remainder);
				}
			}
			if(Dir_str == _T("L") || Dir_str == _T("l")){
				CreateText(L_Textpt, m_Height, m_Width, STAang + DTR(90.0), DualFormat_str, str_Textlayer, 2);
			}
			else if(Dir_str == _T("R") || Dir_str == _T("r")){
				CreateText(R_Textpt, m_Height, m_Width, STAang + DTR(90.0), DualFormat_str, str_Textlayer, 1);
			}
			else if(Dir_str == _T("A") || Dir_str == _T("a")){
				CreateText(L_Textpt, m_Height, m_Width, STAang + DTR(90.0), DualFormat_str, str_Textlayer, 2);
				CreateText(R_Textpt, m_Height, m_Width, STAang + DTR(90.0), DualFormat_str, str_Textlayer, 1);
			}
		}
		else{//NO
			if(b_Chk1 == true){//도면에서 지정 체크시
				DualFormat_str.Format(_T("%%.%df"), H_DotNum);
				Survey_Str.Format(DualFormat_str, Survey_YESf * 1000 / m_Scale);
				f_Length = _tstof(Survey_Str);
				if(JobMode == 1){
					STA_divvy = f_Length / 1000;
					STA_Remainder = fmod(f_Length, 1000);
					if(STA_divvy < 0 || STA_Remainder < 0){
						STAdot_str.Format(_T("STA.%d%%.%df"), STA_divvy, H_DotNum);
						DualFormat_str.Format(STAdot_str, STA_Remainder);
					}
					else if(STA_Remainder >= 0 || STA_divvy >= 0){
						STAdot_str.Format(_T("STA.%d+%%.%df"), STA_divvy, H_DotNum);
						DualFormat_str.Format(STAdot_str, STA_Remainder);
					}
				}
				else if(JobMode == 2){
					NO_divvy = f_Length / m_Dist;
					NO_Remainder = fmod(f_Length, m_Dist);
					if(NO_divvy < 0 || NO_Remainder < 0){
						NOdot_str.Format(_T("STA.%d%%.%df"), NO_divvy, H_DotNum);
						DualFormat_str.Format(NOdot_str, NO_Remainder);
					}
					else if(NO_Remainder >= 0 || NO_divvy >= 0){
						NOdot_str.Format(_T("NO.%d+%%.%df"), NO_divvy, H_DotNum);
						DualFormat_str.Format(NOdot_str, NO_Remainder);
					}
				}
			}
			else{//도면에서 지정 체크X
				DualFormat_str.Format(_T("%%.%df"), H_DotNum);
				Survey_Str.Format(DualFormat_str, Survey_NOf * 1000 / m_Scale);
				f_Length = _tstof(Survey_Str);
				if(JobMode == 1){
					STA_divvy = f_Length / 1000;
					STA_Remainder = fmod(f_Length, 1000);
					STAdot_str.Format(_T("STA.%d+%%.%df"), STA_divvy, H_DotNum);
					DualFormat_str.Format(STAdot_str, STA_Remainder);
				}
				else if(JobMode == 2){
					NO_divvy = f_Length / m_Dist;
					NO_Remainder = fmod(f_Length, m_Dist);
					NOdot_str.Format(_T("NO.%d+%%.%df"), NO_divvy, H_DotNum);
					DualFormat_str.Format(NOdot_str, NO_Remainder);
				}
			}
			if(Dir_str == _T("L") || Dir_str == _T("l")){
				CreateText(R_Textpt, m_Height, m_Width, STAang - DTR(90.0), DualFormat_str, str_Textlayer, 2);
			}
			else if(Dir_str == _T("R") || Dir_str == _T("r")){
				CreateText(L_Textpt, m_Height, m_Width, STAang - DTR(90.0), DualFormat_str, str_Textlayer, 1);
			}
			else if(Dir_str == _T("A") || Dir_str == _T("a")){
				CreateText(L_Textpt, m_Height, m_Width, STAang - DTR(90.0), DualFormat_str, str_Textlayer, 1);
				CreateText(R_Textpt, m_Height, m_Width, STAang - DTR(90.0), DualFormat_str, str_Textlayer, 2);
			}
		}
	}
	acedSSFree(ens);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("YNDirect"), DirYN_str);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("NotDirect"), Dir_str);
	CompleteEditorCommand();
}

void CSTT::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

//라디오 STATION
void CSTT::OnBnClickedSttRadstation(){
	UpdateData(TRUE);
	JobMode = 1;
	b_RadBtn = 0;
	GetDlgItem(IDC_STT_STATIC_PTWIDTH)->EnableWindow(FALSE);//측점간격 비활성화
	GetDlgItem(IDC_STT_EDTDIST)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

//라디오 NUMBER
void CSTT::OnBnClickedSttRadnumber(){
	UpdateData(TRUE);
	JobMode = 2;
	b_RadBtn = 1;
	GetDlgItem(IDC_STT_STATIC_PTWIDTH)->EnableWindow(TRUE);//측점간격 활성화
	GetDlgItem(IDC_STT_EDTDIST)->EnableWindow(TRUE);
	UpdateData(FALSE);
}

//축척
void CSTT::OnEnUpdateSttEdtscale(){
	UpdateData(TRUE);
	m_Scale = _tstof(m_ScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Scale"), m_ScaleStr);
	UpdateData(FALSE);
}

//시작거리
void CSTT::OnEnUpdateSttEdtstlength(){
	UpdateData(TRUE);
	m_STLength = _tstof(m_STLengthStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("STLength"), m_STLengthStr);
	UpdateData(FALSE);
}

//측점 간격
void CSTT::OnEnUpdateSttEdtdist(){
	UpdateData(TRUE);
	m_Dist = _tstof(m_DistStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Dist"), m_DistStr);
	UpdateData(FALSE);
}

//크기
void CSTT::OnEnUpdateSttEdtheight(){
	UpdateData(TRUE);
	m_Height = _tstof(m_HeightStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Height"), m_HeightStr);
	UpdateData(FALSE);
}

//폭
void CSTT::OnEnUpdateSttEdtwidth(){
	UpdateData(TRUE);
	m_Width = _tstof(m_WidthStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Width"), m_WidthStr);
	UpdateData(FALSE);
}

//Dot 표기 직경
void CSTT::OnEnUpdateSttEdtdot(){
	UpdateData(TRUE);
	m_Dot = _tstof(m_DotStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Dot"), m_DotStr);
	UpdateData(FALSE);
}

//중심~인출선 길이
void CSTT::OnEnUpdateSttEdtdist2(){
	UpdateData(TRUE);
	m_Dist2 = _tstof(m_Dist2Str);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Dist2"), m_Dist2Str);
	UpdateData(FALSE);
}

//중심~인출선 offset
void CSTT::OnEnUpdateSttEdtoffset(){
	UpdateData(TRUE);
	m_Offset = _tstof(m_OffsetStr);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Offset"), m_OffsetStr);
	UpdateData(FALSE);
}

//콤보박스 글꼴
void CSTT::OnCbnSelchangeSttCbxtexttype(){
	UpdateData(TRUE);
	cbxInt = m_cbxTextType.GetCurSel();
	if(cbxInt == -1)
		cbxInt = 0;
	m_cbxTextType.SetCurSel(cbxInt);
	CString cbxInt_Str;
	cbxInt_Str.Format(_T("%d"), cbxInt);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("cbxTextType"), cbxInt_Str);
	UpdateData(FALSE);
}

//소수점
void CSTT::OnCbnSelchangeSttCbxdot(){
	UpdateData(TRUE);
	cbxInt = m_cbxDot.GetCurSel();
	if(cbxInt == -1)
		cbxInt = 0;
	m_cbxDot.SetCurSel(cbxInt);
	CString cbxInt_Str;
	cbxInt_Str.Format(_T("%d"), cbxInt);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("cbxDot"), cbxInt_Str);
	UpdateData(FALSE);	
}

//선
void CSTT::OnCbnSelchangeSttCbxline(){
	UpdateData(TRUE);
	cbxInt2 = m_cbxLineLayer.GetCurSel();
	if(cbxInt2 == -1)
		cbxInt2 = 0;
	m_cbxLineLayer.SetCurSel(cbxInt2);
	cbxLine_Str.Format(_T("%d"), cbxInt2);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("cbxLineLayer"), cbxLine_Str);
	UpdateData(FALSE);
}

//문자
void CSTT::OnCbnSelchangeSttCbxtext(){
	UpdateData(TRUE);
	cbxInt = m_cbxTextLayer.GetCurSel();
	if(cbxInt == -1)
		cbxInt = 0;
	m_cbxTextLayer.SetCurSel(cbxInt);
	cbxText_Str.Format(_T("%d"), cbxInt);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("cbxTextLayer"), cbxText_Str);
	UpdateData(FALSE);
}

//Dot
void CSTT::OnCbnSelchangeSttCbxlayerdot(){
	UpdateData(TRUE);
	cbxInt = m_cbxDotLayer.GetCurSel();
	if(cbxInt == -1)
		cbxInt = 0;
	m_cbxDotLayer.SetCurSel(cbxInt);
	cbxDot_Str.Format(_T("%d"), cbxInt);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("cbxDotLayer"), cbxDot_Str);
	UpdateData(FALSE);
}

//폭 버튼
void CSTT::OnBnClickedSttBselect1(){
	UpdateData(TRUE);
	ads_name en;
	ads_point pt;
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		CString strMsg;
		strMsg.LoadStringW(IDS_SELECT_TEXT); // "문자 선택: "
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
			AcGePoint3d insertPoint;
			AcDbObjectId objId;
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbText::desc())){
					AcDbText* pText = AcDbText::cast(pEnt);
					double TxtHeight = pText->height();
					double TxtWidth = pText->widthFactor();
					m_HeightStr.Format(_T("%.3f"), TxtHeight);
					m_WidthStr.Format(_T("%.3f"), TxtWidth);
					pText->close();
					break;
				}
				else{
					strMsg.LoadStringW(IDS_CHECK_STRING); // "**선택한 객체는 문자가 아닙니다.**"
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();
			acedSSFree(en);	
		}
	}
	CompleteEditorCommand();
	UpdateData(FALSE);
}

//선 레이어 버튼
void CSTT::OnBnClickedSttBselect2(){
	UpdateData(TRUE);
	TakeInLayer(1);
	UpdateData(FALSE);
}

//문자 레이어 버튼
void CSTT::OnBnClickedSttBselect3(){
	UpdateData(TRUE);
	TakeInLayer(2);
	UpdateData(FALSE);
}

//DOt 레이어 버튼
void CSTT::OnBnClickedSttBselect4(){
	UpdateData(TRUE);
	TakeInLayer(3);
	UpdateData(FALSE);
}

bool CSTT::TakeInLayer(int Mode){
	ads_name en;
	ads_point pt;
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	CString strMsg;
	strMsg.LoadStringW(IDS_GET_LAYER_ENTITY); // "\n도면층을 가져올 객체 선택: "
	if(acedEntSel(strMsg, en, pt) != RTNORM){
		strMsg.LoadStringW(IDS_CHECK_ENTITY); // "\n객체가 선택되지 않았습니다.\n"
		acutPrintf(strMsg);
		acedSSFree(en);
		CompleteEditorCommand();
		return false;
	}
	if(en[0] == 0 || en[0] == NULL)
		return false;

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
	CString cbxIndexStr;
	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		pLayerTblRcd->getName(name);
		if(0 == _tcscmp(selectLayerName, name)){
			if(Mode == 1){
				m_cbxLineLayer.SetCurSel(cbxIndex);
				cbxIndexStr.Format(_T("%d"), cbxIndex);
				iniFile.writeINI(_T("config.ini"), _T("STT"), _T("cbxLineLayer"), cbxIndexStr);
			}
			else if(Mode == 2){
				m_cbxTextLayer.SetCurSel(cbxIndex);
				cbxIndexStr.Format(_T("%d"), cbxIndex);
				iniFile.writeINI(_T("config.ini"), _T("STT"), _T("cbxTextLayer"), cbxIndexStr);
			}
			else if(Mode == 3){
				m_cbxDotLayer.SetCurSel(cbxIndex);
				cbxIndexStr.Format(_T("%d"), cbxIndex);
				iniFile.writeINI(_T("config.ini"), _T("STT"), _T("cbxDotLayer"), cbxIndexStr);
			}
			break;
		}
		cbxIndex++;
	}
	acedSSFree(en);
	delete pLayerTblIter;
	delete pLayerTbl;
	CompleteEditorCommand();
	return true;
}

//시작거리 도면에서 지정 체크박스
void CSTT::OnBnClickedSttCheck1(){
	b_Chk1 = IsDlgButtonChecked(IDC_STT_CHECK1);
	if(b_Chk1 == 0){
		m_Chk1_btn.SetCheck(FALSE);
		m_STLengthStr = _T("FALSE");
		GetDlgItem(IDC_STT_EDTSTLENGTH)->EnableWindow(TRUE);
	}else{
		m_Chk1_btn.SetCheck(TRUE);
		m_STLengthStr = _T("TRUE");
		GetDlgItem(IDC_STT_EDTSTLENGTH)->EnableWindow(FALSE);
	}
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Chk_STLength"), m_STLengthStr);
}

//Dot 표기 직경 체크박스
void CSTT::OnBnClickedSttCheck2(){
	b_Chk2 = IsDlgButtonChecked(IDC_STT_CHECK2);
	if(b_Chk2 == 0){
		m_Chk2_btn.SetCheck(FALSE);
		m_DotStr = _T("0");
		GetDlgItem(IDC_STT_EDTDOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STT_STATIC_M1)->EnableWindow(FALSE);
	}else{
		m_Chk2_btn.SetCheck(TRUE);
		m_DotStr = _T("1");
		GetDlgItem(IDC_STT_EDTDOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STT_STATIC_M1)->EnableWindow(TRUE);
	}
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Chk_Dot"), m_DotStr);
}

//인출선 표기 체크박스
void CSTT::OnBnClickedSttCheck3(){
	b_Chk3 = IsDlgButtonChecked(IDC_STT_CHECK3);
	if(b_Chk3 == 0){
		m_Chk3_btn.SetCheck(FALSE);
		m_Dist2Str = _T("FALSE");
		GetDlgItem(IDC_STT_EDTDIST2)->EnableWindow(FALSE);
		GetDlgItem(IDC_STT_STATIC2)->EnableWindow(FALSE);
		GetDlgItem(IDC_STT_STATIC_M2)->EnableWindow(FALSE);
		GetDlgItem(IDC_STT_EDTOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_STT_STATIC_OFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_STT_STATIC_M3)->EnableWindow(FALSE);
	}else{
		m_Chk3_btn.SetCheck(TRUE);
		m_Dist2Str = _T("TRUE");
		GetDlgItem(IDC_STT_EDTDIST2)->EnableWindow(TRUE);
		GetDlgItem(IDC_STT_STATIC2)->EnableWindow(TRUE);
		GetDlgItem(IDC_STT_STATIC_M2)->EnableWindow(TRUE);
		GetDlgItem(IDC_STT_EDTOFFSET)->EnableWindow(TRUE);
		GetDlgItem(IDC_STT_STATIC_OFFSET)->EnableWindow(TRUE);
		GetDlgItem(IDC_STT_STATIC_M3)->EnableWindow(TRUE);
	}
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Chk_Dst2Offset"), m_Dist2Str);
}

//표준 레이어 사용 체크박스
void CSTT::OnBnClickedSttCheck4(){
	b_Chk4 = IsDlgButtonChecked(IDC_STT_CHECK4);
	if(b_Chk4 == 0){
		m_StandLayer = _T("0");
		m_Chk4_btn.SetCheck(FALSE);
		GetDlgItem(IDC_STT_STATIC_LAYERLINE)->EnableWindow(TRUE);GetDlgItem(IDC_STT_STATIC_LAYERTXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STT_STATIC_LAYERDOT)->EnableWindow(TRUE);GetDlgItem(IDC_STT_CBXLINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STT_CBXTEXT)->EnableWindow(TRUE);GetDlgItem(IDC_STT_CBXLAYERDOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STT_BSELECT2)->EnableWindow(TRUE);GetDlgItem(IDC_STT_BSELECT3)->EnableWindow(TRUE);
		GetDlgItem(IDC_STT_BSELECT4)->EnableWindow(TRUE);
	}else{
		m_StandLayer = _T("1");
		m_Chk4_btn.SetCheck(TRUE);
		GetDlgItem(IDC_STT_STATIC_LAYERLINE)->EnableWindow(FALSE);GetDlgItem(IDC_STT_STATIC_LAYERTXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STT_STATIC_LAYERDOT)->EnableWindow(FALSE);GetDlgItem(IDC_STT_CBXLINE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STT_CBXTEXT)->EnableWindow(FALSE);GetDlgItem(IDC_STT_CBXLAYERDOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STT_BSELECT2)->EnableWindow(FALSE);GetDlgItem(IDC_STT_BSELECT3)->EnableWindow(FALSE);
		GetDlgItem(IDC_STT_BSELECT4)->EnableWindow(FALSE);
	}
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("Chk_StandLayer"), m_StandLayer);
}

//각도를 라디안 값으로 변환하여 리턴한다.
double CSTT::DTR(double angle){
	return (angle / 180.0 * PI);
}

void CSTT::getclosestPoint(AcGePoint3d sp, AcGePoint3d ep, AcGePoint3d givenPnt, AcGePoint3d& closestPnt){
	AcGeVector3d v, v1, v2;
	AcGeMatrix3d mat;
	double l = (double)0, ang = (double)0, dst = (double)0;

	v = ep - sp;
	v.normalize();
	l = sp.distanceTo(givenPnt);
	v1 = givenPnt - sp;
	v2 = ep - sp;
	ang = v1.angleTo(v2);
	dst = l * cos(ang);
	v = v * dst;
	mat.setToTranslation(v);
	closestPnt = sp;
	closestPnt.transformBy(mat);
}

//텍스트 생성
bool CSTT::CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, int mode){
	Acad::ErrorStatus es;
	AcDbObjectId textStyleId, pTextObjId;
	double XScale = 0, obliquingAngle = 0;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return false;

	AcDbText* pText = new AcDbText();
	textStyleId = pDb->textstyle();
	pText->setTextStyle(textStyleId);
	AcDbTextStyleTableRecord *pTSTR = NULL;
	AcDbBlockTable* pBT;
	AcDbBlockTableRecord* pBTR;
	acdbOpenObject(pTSTR, textStyleId, AcDb::kForRead);
	if(pTSTR){
		XScale = pTSTR->xScale();
		obliquingAngle = pTSTR->obliquingAngle();
		pTSTR->close();
		pText->setWidthFactor(XScale);
	}

	pText->setOblique(obliquingAngle);
	pText->setLayer(Layer);
	pText->setPosition(pt);
	if(mode == 1){
		es = pText->setVerticalMode(AcDb::kTextBase);
		es = pText->setHorizontalMode(AcDb::kTextLeft);
	}
	else{
		es = pText->setVerticalMode(AcDb::kTextBottom);
		es = pText->setHorizontalMode(AcDb::kTextRight);
	}
	pText->setAlignmentPoint(pt);
	pText->setHeight(Height);
	pText->setWidthFactor(Width);
	pText->setRotation(Rotate);
	pText->setTextString((LPCTSTR)TxtStr);
	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(pTextObjId, pText);
			pBTR->close();
			pText->close();
		}
		pBT->close();
	}
	return true;
}

//라인 생성
bool CSTT::CreateLine(AcGePoint3d spt,AcGePoint3d ept, CString LineType, double LineTypeScale, CString Layer){
	Acad::ErrorStatus es;
	AcDbObjectId pLineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return false;

	AcDbLine* pLine = new AcDbLine(spt,ept);
	pLine->setLinetype(LineType);
	pLine->setLinetypeScale(LineTypeScale);
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

//레이어 생성
bool CSTT::CreateLayer(CString Layer, int color){
	AcDbLayerTable* pLayerTable;
	AcCmColor Layercolor;
	Layercolor.setColorIndex(color);
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return false;

	pDb->getLayerTable(pLayerTable, AcDb::kForWrite);
	if(pLayerTable->has(Layer) != Adesk::kTrue){
		AcDbLayerTableRecord* pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(Layer);
		pLayerTableRecord->setColor(Layercolor);
		AcDbLinetypeTable* pLinetypeTable;
		pDb->getLinetypeTable(pLinetypeTable, AcDb::kForRead);
		AcDbObjectId LinetypeId;
		pLinetypeTable->getAt(_T("BYLAYER"), LinetypeId);
		pLayerTableRecord->setLinetypeObjectId(LinetypeId);
		pLayerTable->add(pLayerTableRecord);
		pLinetypeTable->close();
		pLayerTableRecord->close();
		pLayerTable->close();	
	}
	else{
		pLayerTable->close();
		return false;
	}
	return true;
}

//고체 폴리선
bool CSTT::CreatePLine(AcGePoint3d pt1, AcGePoint3d pt2, double LineWidth, CString LineType, double LineTypeScale, CString Layer){
	Acad::ErrorStatus es;
	AcDbObjectId plineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return false;

	AcDbPolyline* pPLine = new AcDbPolyline();
	pPLine->addVertexAt(0, AcGePoint2d(pt1.x, pt1.y));//시작점
	pPLine->addVertexAt(0, AcGePoint2d(pt2.x, pt2.y), 1, LineWidth, LineWidth);//bulge 값 시작,끝 두께 값 주면 호로 작도
	pPLine->addVertexAt(0, AcGePoint2d(pt1.x, pt1.y), 1, LineWidth, LineWidth);
	pPLine->setConstantWidth(LineWidth);
	pPLine->setLinetype(LineType);
	pPLine->setLinetypeScale(LineTypeScale);
	pPLine->setLayer(Layer);

	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(plineObjId, pPLine);
			pBTR->close();
			pPLine->close();
		}
		else
			return false;

		pBT->close();
	}
	else
		return false;

	pPLine->close();
	return true;
}

//점 생성
void CSTT::CreatePoint(AcGePoint3d pos){
	double ofsetX,ofsetY;
	acutPrintf(_T("\n%g, %g, %g"), pos.x, pos.y, 0);
	AcDbPoint* pPoint = new AcDbPoint;
	pPoint->setPosition(AcGePoint3d(pos.x, pos.y, 0));
	AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
	acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	pPoint->setDatabaseDefaults();
	blkRec->appendAcDbEntity(pPoint);
	pPoint->close();
	blkRec->close();
}

//Mode=0 점 숫자추출 Mode=1 두번째 정수부터 점 숫자추출 Mode=2 양수 첫째자리추출
CString CSTT::CheckInputNumber(CString str, int Mode){
	TCHAR* getAtChar;
	CString strNum, strRe = _T("");
	int k = 0;
	if(Mode == 0){
		for(int i = 0; i < str.GetLength(); i++){
			strNum = str.Mid(i, 1);			//for문을 돌면 사라진문자때문에 다음문자를 찾으려면 Mid에서 0번째를 찾아야 함
			getAtChar = (TCHAR*)(LPCTSTR)strNum;
			int ascii = __toascii(*getAtChar);
			if(((ascii >= 48 && ascii <= 57) || ascii == 46)){
				strRe.Append(getAtChar);
			}
		}
	}
	else if(Mode == 1){
		for(int i = 0; i < str.GetLength(); i++){
			strNum = str.Mid(i + 1, 1);		//for문을 돌면 사라진문자때문에 다음문자를 찾으려면 Mid에서 0번째를 찾아야 함
			getAtChar = (TCHAR*)(LPCTSTR)strNum;
			int ascii = __toascii(*getAtChar);
			if(((ascii >= 48 && ascii <= 57) || ascii == 46)){
				strRe.Append(getAtChar);
			}
		}
	}
	else if(Mode == 2){
		strNum = str.Mid(0, 1);				//for문을 돌면 사라진문자때문에 다음문자를 찾으려면 Mid에서 0번째를 찾아야 함
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		strRe.Append(getAtChar);
	}
	return strRe;
}

void CSTT::OnBnClickedSttHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/STT.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
