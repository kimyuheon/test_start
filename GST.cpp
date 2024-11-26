// GST.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GST.h"
#include "afxdialogex.h"


// CGST 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGST, CAcUiDialog)

CGST::CGST(CWnd* pParent /*=NULL*/)
: CAcUiDialog(CGST::IDD, pParent), b_RadBtn(0), m_STLengthStr(_T("")), m_ScaleStr(_T("")), m_DistStr(_T("")), m_HeightStr(_T("")), /*m_PrefaceStr(_T("")),*/ cbxInt_Str(_T("")), Chk_DrawSurveyStr(_T("")), Chk_STLengthStr(_T("")), Chk_FormStr(_T(""))
{

}

CGST::~CGST()
{
}

void CGST::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	//라디오
	DDX_Radio(pDX, IDC_GST_RADSTATION, b_RadBtn);
	//체크박스
	DDX_Control(pDX, IDC_GST_CHECK1, m_Chk1_btn);
	DDX_Control(pDX, IDC_GST_CHECK2, m_Chk2_btn);
	DDX_Control(pDX, IDC_GST_CHECK3, m_Chk3_btn);
	//콤보박스
	DDX_Control(pDX, IDC_GST_CBXDOT, m_cbxDot);
	//스태틱
	DDX_Text(pDX, IDC_GST_EDTSTLENGTH, m_STLengthStr);
	DDX_Text(pDX, IDC_GST_EDTSCALE, m_ScaleStr);
	DDX_Text(pDX, IDC_GST_EDTDIST, m_DistStr);
	DDX_Text(pDX, IDC_GST_EDTHEIGHT, m_HeightStr);
	DDX_Text(pDX, IDC_GST_EDTPREFACE, m_PrefaceStr);
	//텍스트 높이 출력버튼
	DDX_Control(pDX, IDC_GST_BSELECT, m_bSelect);

}


BEGIN_MESSAGE_MAP(CGST, CAcUiDialog)
	ON_BN_CLICKED(IDC_GST_RADSTATION, &CGST::OnBnClickedGstRadstation)
	ON_BN_CLICKED(IDC_GST_RADNUMBER, &CGST::OnBnClickedGstRadnumber)
	ON_BN_CLICKED(IDC_GST_CHECK1, &CGST::OnBnClickedGstCheck1)
	ON_EN_UPDATE(IDC_GST_EDTSTLENGTH, &CGST::OnEnUpdateGstEdtstlength)
	ON_EN_UPDATE(IDC_GST_EDTSCALE, &CGST::OnEnUpdateGstEdtscale)
	ON_EN_UPDATE(IDC_GST_EDTDIST, &CGST::OnEnUpdateGstEdtdist)
	ON_EN_UPDATE(IDC_GST_EDTHEIGHT, &CGST::OnEnUpdateGstEdtheight)
	ON_EN_UPDATE(IDC_GST_EDTPREFACE, &CGST::OnEnUpdateGstEdtpreface)
	ON_CBN_SELCHANGE(IDC_GST_CBXDOT, &CGST::OnCbnSelchangeGstCbxdot)
	ON_BN_CLICKED(IDC_GST_CHECK2, &CGST::OnBnClickedGstCheck2)
	ON_BN_CLICKED(IDC_GST_BSELECT, &CGST::OnBnClickedGstBselect)
	ON_BN_CLICKED(IDOK, &CGST::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGST::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_GST_CHECK3, &CGST::OnBnClickedGstCheck3)
	ON_BN_CLICKED(ID_GST_HELP, &CGST::OnBnClickedGstHelp)
END_MESSAGE_MAP()


// CGST 메시지 처리기입니다.


BOOL CGST::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	m_bSelect.AutoLoad();//문자크기 텍스트에서 가져오기 버튼;

	//소수점
	m_cbxDot.AddString(_T("0"));m_cbxDot.AddString(_T("1"));m_cbxDot.AddString(_T("2"));
	m_cbxDot.AddString(_T("3"));m_cbxDot.AddString(_T("4"));

	//측점 표기 STA = 1, NO = 2
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("GST"), _T("JOBMODE"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		OnBnClickedGstRadstation();
	else
		OnBnClickedGstRadnumber();
	//시작거리
	tempStr = iniFile.readINI(_T("config.ini"), _T("GST"), _T("STLength"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_GST_EDTSTLENGTH, _T("0"));
	else
		SetDlgItemText(IDC_GST_EDTSTLENGTH, tempStr);
	//체크박스  도면에서 지정
	tempStr = iniFile.readINI(_T("config.ini"), _T("GST"), _T("CHK_STLenght"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("FALSE")) == 0){
		b_Chk1 = false;
		m_Chk1_btn.SetCheck(FALSE);
		OnBnClickedGstCheck1();
	}else{
		b_Chk1 = true;
		m_Chk1_btn.SetCheck(TRUE);
		OnBnClickedGstCheck1();
	}
	//콤보박스 소수점
	tempStr = iniFile.readINI(_T("config.ini"), _T("GST"), _T("cbxDot"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_cbxDot.SetCurSel(0);
	}else{
		SetDlgItemText(IDC_GST_CBXDOT, tempStr);
		cbxInt = _ttoi(tempStr);
		m_cbxDot.SetCurSel(cbxInt);
	}
	//축척
	tempStr = iniFile.readINI(_T("config.ini"), _T("GST"), _T("Scale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_GST_EDTSCALE, _T("1000"));
	else
		SetDlgItemText(IDC_GST_EDTSCALE, tempStr);
	//측점 간격
	tempStr = iniFile.readINI(_T("config.ini"), _T("STT"), _T("Dist"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_GST_EDTDIST, _T("20.00"));
	else
		SetDlgItemText(IDC_GST_EDTDIST, tempStr);
	//측점 도면에 쓰기
	tempStr = iniFile.readINI(_T("config.ini"), _T("GST"), _T("CHK_DrawSurvey"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("FALSE")) == 0){
		b_Chk2 = false;
		m_Chk2_btn.SetCheck(FALSE);
		OnBnClickedGstCheck2();
	}else{
		b_Chk2 = true;
		m_Chk2_btn.SetCheck(TRUE);
		OnBnClickedGstCheck2();
	}
	//문자 크기
	tempStr = iniFile.readINI(_T("config.ini"), _T("GST"), _T("Height"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_GST_EDTHEIGHT, _T("1.000"));
	else
		SetDlgItemText(IDC_GST_EDTHEIGHT, tempStr);
	//머리말
	
	tempStr = iniFile.readINI(_T("config.ini"), _T("GST"), _T("Preface"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_GST_EDTPREFACE, _T(""));
	else
		SetDlgItemText(IDC_GST_EDTPREFACE, tempStr);
	//체크박스 측점 0+000 형식 유지
	tempStr = iniFile.readINI(_T("config.ini"), _T("GST"), _T("CHK_Form"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("FALSE")) == 0){
		b_Chk3 = false;
		m_Chk3_btn.SetCheck(FALSE);
		OnBnClickedGstCheck3();
	}else{
		b_Chk3 = true;
		m_Chk3_btn.SetCheck(TRUE);
		OnBnClickedGstCheck3();
	}


	return TRUE;
}
void CGST::OnBnClickedOk()
{
	UpdateData();
	CAcUiDialog::OnOK();
	if(!(JobMode == 1 || JobMode == 2))JobMode = 1;
	JobMode_Str.Format(_T("%d"), JobMode);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("JOBMODE"), JobMode_Str);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("CHK_STLenght"), Chk_STLengthStr);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("STLenght"), m_STLengthStr);
	//iniFile.writeINI(_T("config.ini"), _T("GST"), _T("cbxDot"), cbxInt_Str);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("Scale"), m_ScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("Dist"), m_DistStr);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("CHK_DrawSurvey"), Chk_DrawSurveyStr);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("Height"), m_HeightStr);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("Preface"), m_PrefaceStr);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("CHK_Form"), Chk_FormStr);

	AcGePoint3d LinePt, ClosePt, AnglePt, AnglePt1, AnglePt2;
	double ClosePt_dst, dist, YNDirect_f;
	ads_name ens;
	CString RTERROR_str, YNDirect, tempKword, DirYN_str, strMsg;
	TCHAR kWord[20];

	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		strMsg.LoadStringW(IDS_LINE_SELECT); // "선형 선택 : "
		int stat = acedEntSel(strMsg, ens,asDblArray(LinePt));

		if(stat == RTCAN){
			acedSSFree(ens);
			CancelEditorCommand();
			return;
		}else if(stat == RTERROR){
			resbuf zValueStr;
			ads_getvar(_T("ERRNO"), &zValueStr);
			if(zValueStr.resval.rint == 52){
				acedSSFree(ens);
				return;
			}else
				continue;
		}else if(stat == RTKWORD){
			continue;
		}else if(stat == RTNORM){
			AcDbObjectId objId;
			acdbGetObjectId(objId, ens);
			
			 AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			 if(pEnt.openStatus() == Acad::eOk){
				 if(pEnt->isKindOf(AcDbLine::desc())){
					AcDbLine* pLine = AcDbLine::cast(pEnt);
					pLine->getClosestPointTo(LinePt, ClosePt);
					pLine->getDistAtPoint(ClosePt,ClosePt_dst);
					pLine->getPointAtDist(ClosePt_dst+0.1, AnglePt);
			 		pLine->close();
				 }else if(pEnt->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline* pPLine = AcDbPolyline::cast(pEnt);
					pPLine->getClosestPointTo(LinePt, ClosePt);
					pPLine->getDistAtPoint(ClosePt,ClosePt_dst);
					pPLine->getPointAtDist(ClosePt_dst+0.1, AnglePt);
					pPLine->close();
				 }else if(pEnt->isKindOf(AcDbArc::desc())){
					AcDbArc* pArc = AcDbArc::cast(pEnt);
					pArc->getClosestPointTo(LinePt, ClosePt);
					pArc->getDistAtPoint(ClosePt,ClosePt_dst);
					pArc->getPointAtDist(ClosePt_dst+0.1, AnglePt);
					pArc->close();
				 }else if(pEnt->isKindOf(AcDbSpline::desc())){
					AcDbSpline* pSPline = AcDbSpline::cast(pEnt);
					pSPline->getClosestPointTo(LinePt, ClosePt);
					pSPline->getDistAtPoint(ClosePt,ClosePt_dst);
					pSPline->getPointAtDist(ClosePt_dst+0.1, AnglePt);
					pSPline->close();
				 }else{
					strMsg.LoadStringW(IDS_SELECT_CHK_LINE); // "**선택한 객체는 선형이 아닙니다.**"
					acutPrintf(strMsg);
					continue;
				}
			 }
			pEnt->close();
			//acedSSFree(ens);	
			break;
		}
			
	}//while end
	

	//가상 방향화살표
	struct resbuf* sys1 = NULL,*sys2 = NULL;
	ads_real do1=0.0, vs=0.0, ss=0.0;
	sys1=ads_newrb(RTREAL);    
	sys2=ads_newrb(RT3DPOINT);
	ads_getvar(_T("VIEWSIZE"), sys1); 
    ads_getvar(_T("SCREENSIZE"), sys2);
	vs=sys1->resval.rreal;
	ss = sys2->resval.rpoint[Y];
	do1=vs/ss;

	ads_polar(asDblArray(ClosePt), ads_angle(asDblArray(ClosePt), asDblArray(AnglePt))+DTR(160.0),50 * do1,asDblArray(AnglePt1));
	ads_polar(asDblArray(ClosePt), ads_angle(asDblArray(ClosePt), asDblArray(AnglePt))-DTR(160.0),50 * do1,asDblArray(AnglePt2));
	acedGrDraw(asDblArray(AnglePt1),asDblArray(ClosePt),1,0);
	acedGrDraw(asDblArray(AnglePt2),asDblArray(ClosePt),1,0);


	while(true){//선형 진행방향 Yes or No
		CString tempStr = iniFile.readINI(_T("config.ini"), _T("GST"), _T("YNDirect"));
		if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("Y")) == 0)
			tempStr.Format(_T("Y"));
		else
			tempStr;

		if(tempStr == _T("Y"))
			YNDirect.Format(_T("Yes(Y)"));
		else if(tempStr == _T("N"))
			YNDirect.Format(_T("No(N)"));

		strMsg.LoadStringW(IDS_LINE_DIRECT_CHK); // "\n선형 진행 방향이 맞습니까? [예(Y)/아니오(N)] <%s> : "
		tempKword.Format(strMsg, YNDirect);
		acedInitGet(0, _T("Y y N n"));
		int stat = acedGetReal(tempKword, &YNDirect_f/*(TCHAR*)(LPCTSTR)DirYN_str*/);
		
		if(stat == RTCAN){
			acedSSFree(ens);
			return;
		}else if(stat == RTNONE){
			DirYN_str = tempStr;
			break;
		}else if(stat == RTNORM){
			continue;
		}else if(stat == RTKWORD){
			if(acedGetInput(kWord) != RTNORM)return;
			if(!_tcscmp(_T("Y"),kWord) || !_tcscmp(_T("y"),kWord))
				DirYN_str.Format(kWord);
			else if(!_tcscmp(_T("N"),kWord) || !_tcscmp(_T("n"),kWord))
				DirYN_str.Format(kWord);
			
			iniFile.writeINI(_T("config.ini"), _T("GST"), _T("YNDirect"), DirYN_str);	
			break;
		}
		
	}//while end

	CString DualFormat_str, Survey_Str, STAdot_Str, NOdot_Str, str_Dot;
	int STA_divvy, NO_divvy;
	AcGePoint3d StaPt, referPt, Close_StaPt, Close_referPt, Spt, Ept;
	double StaReal, CloseStaPt_dst, CloseReferPt_dst, Dist, f_Length,STA_Remainder, NO_Remainder;
	
	m_cbxDot.GetLBText(this->m_cbxDot.GetCurSel(),str_Dot);//콤보박스 선택 텍스트가져오기
	int H_DotNum = _ttoi(str_Dot);

	if(b_Chk1 == true){//도면에서 지정 체크시
		strMsg.LoadStringW(IDS_CIVIL_BASEPT); // "\n측점 기준이 될 점 선택 : "
		if(acedGetPoint(0, strMsg, asDblArray(StaPt))!=RTNORM){
			acedSSFree(ens);
			return;
		}
		strMsg.LoadStringW(IDS_POINT_INPUT); // "\n선택한 점의 측점 입력 : "
		if(acedGetReal(strMsg, &StaReal) != RTNORM){
			acedSSFree(ens);
			return;
		}
	}

	while(true){//반복 실행
		strMsg.LoadStringW(IDS_CIVIL_CHECK_PT); // "\n측점을 조회할 점 선택 : "
		int stat = acedGetPoint(0, strMsg, asDblArray(referPt));
		if(stat == RTCAN || stat == RTNONE){
			acedSSFree(ens);
			return;
		}else if(stat == RTNORM){
			AcDbObjectId objId;
			acdbGetObjectId(objId, ens);
			
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				 if(pEnt->isKindOf(AcDbLine::desc())){
					AcDbLine* pLine = AcDbLine::cast(pEnt);
					if(b_Chk1 == true){//도면에서 지정 체크시 기준 점
						pLine->getClosestPointTo(StaPt, Close_StaPt);
						pLine->getDistAtPoint(Close_StaPt,CloseStaPt_dst);
					}
					pLine->getClosestPointTo(referPt, Close_referPt);
					pLine->getDistAtPoint(Close_referPt, CloseReferPt_dst);
					pLine->getStartPoint(Spt);
					pLine->getEndPoint(Ept);
					pLine->getDistAtPoint(Ept,Dist);//전체 길이
					pLine->close();
				 }else if(pEnt->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline* pPLine = AcDbPolyline::cast(pEnt);
					if(b_Chk1 == true){//도면에서 지정 체크시 기준 점
						pPLine->getClosestPointTo(StaPt, Close_StaPt);
						pPLine->getDistAtPoint(Close_StaPt,CloseStaPt_dst);
					}
					pPLine->getClosestPointTo(referPt, Close_referPt);
					pPLine->getDistAtPoint(Close_referPt, CloseReferPt_dst);
					pPLine->getStartPoint(Spt);
					pPLine->getEndPoint(Ept);
					pPLine->getDistAtPoint(Ept,Dist);//전체 길이
					pPLine->close();
				 }else if(pEnt->isKindOf(AcDbArc::desc())){
					AcDbArc* pArc = AcDbArc::cast(pEnt);
					if(b_Chk1 == true){//도면에서 지정 체크시 기준 점
						pArc->getClosestPointTo(StaPt, Close_StaPt);
						pArc->getDistAtPoint(Close_StaPt,CloseStaPt_dst);
					}
					pArc->getClosestPointTo(referPt, Close_referPt);
					pArc->getDistAtPoint(Close_referPt, CloseReferPt_dst);
					pArc->getStartPoint(Spt);
					pArc->getEndPoint(Ept);
					pArc->getDistAtPoint(Ept,Dist);//전체 길이
					pArc->close();
				 }else if(pEnt->isKindOf(AcDbSpline::desc())){
					AcDbSpline* pSPline = AcDbSpline::cast(pEnt);
					if(b_Chk1 == true){//도면에서 지정 체크시 기준 점
						pSPline->getClosestPointTo(StaPt, Close_StaPt);
						pSPline->getDistAtPoint(Close_StaPt,CloseStaPt_dst);
					}
					pSPline->getClosestPointTo(referPt, Close_referPt);
					pSPline->getDistAtPoint(Close_referPt, CloseReferPt_dst);
					pSPline->getStartPoint(Spt);
					pSPline->getEndPoint(Ept);
					pSPline->getDistAtPoint(Ept,Dist);//전체 길이
					pSPline->close();
				 }else{
					strMsg.LoadStringW(IDS_SELECT_CHK_LINE); // "**선택한 객체는 선형이 아닙니다.**"
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();
		}

		double Survey_YESf, Survey_NOf;
		if(DirYN_str == _T("Y") || DirYN_str == _T("y")){
			if(b_Chk1 == true)
				Survey_YESf = CloseReferPt_dst - CloseStaPt_dst + StaReal;
			else
				Survey_NOf = CloseReferPt_dst + m_STLength;
		}else{
			if(b_Chk1 == true)
				Survey_YESf = (Dist - CloseReferPt_dst) - (Dist - CloseStaPt_dst) + StaReal;
			else
				Survey_NOf = (Dist - CloseReferPt_dst) + m_STLength;
		}

		if(DirYN_str == _T("Y") || DirYN_str == _T("y")){//진행방향 YES
			if(b_Chk1 == true){//도면에서 지정 체크시
				DualFormat_str.Format(_T("%%.%df"), H_DotNum);
				Survey_Str.Format(DualFormat_str, Survey_YESf*1000/m_Scale);
				f_Length = _tstof(Survey_Str);
				if(JobMode == 1){//STA
					STA_divvy = f_Length/1000;
					STA_Remainder = fmod(f_Length, 1000);
					if(STA_divvy < 0 || STA_Remainder < 0){//음수
						STAdot_Str.Format(_T("%s%d%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
						DualFormat_str.Format(STAdot_Str, STA_Remainder);
						if(b_Chk2 == true){//측점 도면에 표기 체크시에만
							if(b_Chk3 == true){//0+000형식유지
								int STA_Int = -STA_Remainder;
								CString STA_Int_str;
								STA_Int_str.Format(_T("%d"),STA_Int);
								if(STA_Int_str.GetLength() == 1){
									STAdot_Str.Format(_T("%s%d-00%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
									DualFormat_str.Format(STAdot_Str, -STA_Remainder);
								}else if(STA_Int_str.GetLength() == 2){
									STAdot_Str.Format(_T("%s%d-0%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
									DualFormat_str.Format(STAdot_Str, -STA_Remainder);
								}
							}
						}
					}else if(STA_divvy >= 0 || STA_Remainder >= 0){//양수
						STAdot_Str.Format(_T("%s%d+%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
						DualFormat_str.Format(STAdot_Str, STA_Remainder);
						if(b_Chk2 == true){//측점 도면에 표기 체크시에만
							if(b_Chk3 == true){//0+000형식유지
								int STA_Int = STA_Remainder;
								CString STA_Int_str;
								STA_Int_str.Format(_T("%d"),STA_Int);
								if(STA_Int_str.GetLength() == 1){
									STAdot_Str.Format(_T("%s%d+00%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
									DualFormat_str.Format(STAdot_Str, STA_Remainder);
								}else if(STA_Int_str.GetLength() == 2){
									STAdot_Str.Format(_T("%s%d+0%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
									DualFormat_str.Format(STAdot_Str, STA_Remainder);
								}
							}
						}
					}
				}else if(JobMode == 2){//NO
					NO_divvy = f_Length/m_Dist;
					NO_Remainder = fmod(f_Length, m_Dist);
					if(NO_divvy < 0 || NO_Remainder < 0){//음수
						NOdot_Str.Format(_T("%s%d+%%.%df"), m_PrefaceStr, NO_divvy, H_DotNum);
						DualFormat_str.Format(NOdot_Str, -NO_Remainder);
					}else if(NO_divvy >= 0 || NO_Remainder >= 0){//양수
						NOdot_Str.Format(_T("%s%d+%%.%df"), m_PrefaceStr, NO_divvy, H_DotNum);
						DualFormat_str.Format(NOdot_Str, NO_Remainder);
					}
				}
			}else if(b_Chk1 == false){//도면에서 지정 X
				DualFormat_str.Format(_T("%%.%df"), H_DotNum);
				Survey_Str.Format(DualFormat_str, Survey_NOf*1000/m_Scale);
				f_Length = _tstof(Survey_Str);
				if(JobMode == 1){//STA
					STA_divvy = f_Length/1000;
					STA_Remainder = fmod(f_Length,1000);
					STAdot_Str.Format(_T("%s%d+%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
					DualFormat_str.Format(STAdot_Str, STA_Remainder);
					if(b_Chk2 == true){//측점 도면에 표기 체크시에만
						if(b_Chk3 == true){//0+000형식유지
							int STA_Int = STA_Remainder;
							CString STA_Int_str;
							STA_Int_str.Format(_T("%d"),STA_Int);
							if(STA_Int_str.GetLength() == 1){
								STAdot_Str.Format(_T("%s%d+00%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
								DualFormat_str.Format(STAdot_Str, STA_Remainder);
							}else if(STA_Int_str.GetLength() == 2){
								STAdot_Str.Format(_T("%s%d+0%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
								DualFormat_str.Format(STAdot_Str, STA_Remainder);
							}
						}
					}
				}else if(JobMode == 2){//NO
					NO_divvy = f_Length/m_Dist;
					NO_Remainder = fmod(f_Length,m_Dist);
					NOdot_Str.Format(_T("%s%d+%%.%df"),m_PrefaceStr, NO_divvy, H_DotNum);
					DualFormat_str.Format(NOdot_Str, NO_Remainder);
				}
			}

			if(b_Chk2 == true)
				CreateText(Close_referPt, m_Height, 1, 0.0, DualFormat_str, _T("0"), 1);
			else {
				strMsg.LoadStringW(IDS_CIVIL_POINT); // " 측점 = %s\n"
				acutPrintf(strMsg, DualFormat_str);
			}

		}else if(DirYN_str == _T("N") || DirYN_str == _T("n")){//진행방향 NO
			if(b_Chk1 == true){//도면에서 지정 체크시
				DualFormat_str.Format(_T("%%.%df"), H_DotNum);
				Survey_Str.Format(DualFormat_str, Survey_YESf*1000/m_Scale);
				f_Length = _tstof(Survey_Str);
				if(JobMode == 1){//STA
					STA_divvy = f_Length/1000;
					STA_Remainder = fmod(f_Length, 1000);
					if(STA_divvy < 0 || STA_Remainder < 0){//음수
						STAdot_Str.Format(_T("%s%d%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
						DualFormat_str.Format(STAdot_Str, STA_Remainder);
						if(b_Chk2 == true){//측점 도면에 표기 체크시에만
							if(b_Chk3 == true){//0+000형식유지
								int STA_Int = -STA_Remainder;
								CString STA_Int_str;
								STA_Int_str.Format(_T("%d"),STA_Int);
								if(STA_Int_str.GetLength() == 1){
									STAdot_Str.Format(_T("%s%d-00%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
									DualFormat_str.Format(STAdot_Str, -STA_Remainder);
								}else if(STA_Int_str.GetLength() == 2){
									STAdot_Str.Format(_T("%s%d-0%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
									DualFormat_str.Format(STAdot_Str, -STA_Remainder);
								}
							}
						}
					}else if(STA_divvy >= 0 || STA_Remainder >= 0){//양수
						STAdot_Str.Format(_T("%s%d+%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
						DualFormat_str.Format(STAdot_Str, STA_Remainder);
						if(b_Chk3 == true){//0+000형식유지
							int STA_Int = STA_Remainder;
							CString STA_Int_str;
							STA_Int_str.Format(_T("%d"),STA_Int);
							if(STA_Int_str.GetLength() == 1){
								STAdot_Str.Format(_T("%s%d+00%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
								DualFormat_str.Format(STAdot_Str, STA_Remainder);
							}else if(STA_Int_str.GetLength() == 2){
								STAdot_Str.Format(_T("%s%d+0%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
								DualFormat_str.Format(STAdot_Str, STA_Remainder);
							}
						}
					}

				}else if(JobMode == 2){//NO
					NO_divvy = f_Length/m_Dist;
					NO_Remainder = fmod(f_Length, m_Dist);
					if(NO_divvy < 0 || NO_Remainder < 0){
						NOdot_Str.Format(_T("%s%d+%%.%df"), m_PrefaceStr, NO_divvy, H_DotNum);
						DualFormat_str.Format(NOdot_Str, -NO_Remainder);
					}else if(NO_divvy >= 0 || NO_Remainder >= 0){
						NOdot_Str.Format(_T("%s%d+%%.%df"), m_PrefaceStr, NO_divvy, H_DotNum);
						DualFormat_str.Format(NOdot_Str, NO_Remainder);
					}
				}
			}else if(b_Chk1 == false){//도면에서 지정 X
				DualFormat_str.Format(_T("%%.%df"), H_DotNum);
				Survey_Str.Format(DualFormat_str, Survey_NOf*1000/m_Scale);
				f_Length = _tstof(Survey_Str);
				if(JobMode == 1){
					STA_divvy = f_Length/1000;
					STA_Remainder = fmod(f_Length,1000);
					STAdot_Str.Format(_T("%s%d+%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
					DualFormat_str.Format(STAdot_Str, STA_Remainder);
					if(b_Chk2 == true){//측점 도면에 표기 체크시에만
						if(b_Chk3 == true){//0+000형식유지
							int STA_Int = STA_Remainder;
							CString STA_Int_str;
							STA_Int_str.Format(_T("%d"),STA_Int);
							if(STA_Int_str.GetLength() == 1){
								STAdot_Str.Format(_T("%s%d+00%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
								DualFormat_str.Format(STAdot_Str, STA_Remainder);
							}else if(STA_Int_str.GetLength() == 2){
								STAdot_Str.Format(_T("%s%d+0%%.%df"),m_PrefaceStr, STA_divvy, H_DotNum);
								DualFormat_str.Format(STAdot_Str, STA_Remainder);
							}
						}
					}
				}else if(JobMode == 2){
					NO_divvy = f_Length/m_Dist;
					NO_Remainder = fmod(f_Length,m_Dist);
					NOdot_Str.Format(_T("%s%d+%%.%df"),m_PrefaceStr, NO_divvy, H_DotNum);
					DualFormat_str.Format(NOdot_Str, NO_Remainder);
				}
			}

			if(b_Chk2 == true)
				CreateText(Close_referPt, m_Height, 1, 0.0, DualFormat_str, _T("0"), 1);
			else {
				strMsg.LoadStringW(IDS_CIVIL_POINT); // " 측점 = %s\n"
				acutPrintf(strMsg, DualFormat_str);
			}
			

		}

	}//while end
	CompleteEditorCommand();
	acedSSFree(ens);
	iniFile.writeINI(_T("config.ini"), _T("STT"), _T("YNDirect"), DirYN_str);
	
}


void CGST::OnBnClickedCancel()
{
	CAcUiDialog::OnCancel();
}
//측점표기 라디오 STA
void CGST::OnBnClickedGstRadstation()
{
	UpdateData(TRUE);
	JobMode = 1;
	JobMode_Str.Format(_T("%d"), JobMode);
	b_RadBtn = 0;
	GetDlgItem(IDC_GST_STATIC_DIST)->EnableWindow(FALSE);//측점 간격 비활성화
	GetDlgItem(IDC_GST_EDTDIST)->EnableWindow(FALSE);
	GetDlgItem(IDC_GST_CHECK3)->EnableWindow(TRUE);//측점 0+000 형식 유지 활성화
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("JOBMODE"), JobMode_Str);
	UpdateData(FALSE);
}

//측점표기 라디오 NO
void CGST::OnBnClickedGstRadnumber()
{
	UpdateData(TRUE);
	JobMode = 2;
	JobMode_Str.Format(_T("%d"), JobMode);
	b_RadBtn = 1;
	GetDlgItem(IDC_GST_STATIC_DIST)->EnableWindow(TRUE);//측점 간격 활성화
	GetDlgItem(IDC_GST_EDTDIST)->EnableWindow(TRUE);
	GetDlgItem(IDC_GST_CHECK3)->EnableWindow(FALSE);//측점 0+000 형식 유지 비활성화
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("JOBMODE"), JobMode_Str);
	UpdateData(FALSE);
}
//시작거리 도면에서 지정 체크박스
void CGST::OnBnClickedGstCheck1()
{
	b_Chk1 = IsDlgButtonChecked(IDC_GST_CHECK1);
	if(b_Chk1 == 0){
		m_Chk1_btn.SetCheck(FALSE);
		Chk_STLengthStr = _T("FALSE");
		GetDlgItem(IDC_GST_EDTSTLENGTH)->EnableWindow(TRUE);
	}else{
		m_Chk1_btn.SetCheck(TRUE);
		Chk_STLengthStr = _T("TRUE");
		GetDlgItem(IDC_GST_EDTSTLENGTH)->EnableWindow(FALSE);
	}
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("CHK_STLenght"), Chk_STLengthStr);
}
//시작 거리
void CGST::OnEnUpdateGstEdtstlength()
{
	UpdateData(TRUE);
	m_STLength = _tstof(m_STLengthStr);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("STLenght"), m_STLengthStr);
	UpdateData(FALSE);
}
//콤보박스 소수점
void CGST::OnCbnSelchangeGstCbxdot()
{
	UpdateData(TRUE);
	cbxInt = m_cbxDot.GetCurSel();
	if(cbxInt == -1)cbxInt = 0;
	m_cbxDot.SetCurSel(cbxInt);
	cbxInt_Str.Format(_T("%d"),cbxInt);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("cbxDot"), cbxInt_Str);
	UpdateData(FALSE);	
}
//축척
void CGST::OnEnUpdateGstEdtscale()
{
	UpdateData(TRUE);
	m_Scale = _tstof(m_ScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("Scale"), m_ScaleStr);
	UpdateData(FALSE);
}
//측점 간격
void CGST::OnEnUpdateGstEdtdist()
{
	UpdateData(TRUE);
	m_Dist = _tstof(m_DistStr);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("Dist"), m_DistStr);
	UpdateData(FALSE);
}
//체크박스 측점 도면에 쓰기
void CGST::OnBnClickedGstCheck2()
{
	b_Chk2 = IsDlgButtonChecked(IDC_GST_CHECK2);
	if(b_Chk2 == 0){
		m_Chk2_btn.SetCheck(FALSE);
		Chk_DrawSurveyStr = _T("FALSE");
		GetDlgItem(IDC_GST_STATIC_TXTHEIGHT)->EnableWindow(FALSE);GetDlgItem(IDC_GST_STATIC_PREFACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_GST_EDTHEIGHT)->EnableWindow(FALSE);GetDlgItem(IDC_GST_EDTPREFACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_GST_BSELECT)->EnableWindow(FALSE);GetDlgItem(IDC_GST_CHECK3)->EnableWindow(FALSE);
	}else{
		m_Chk2_btn.SetCheck(TRUE);
		Chk_DrawSurveyStr = _T("TRUE");
		GetDlgItem(IDC_GST_STATIC_TXTHEIGHT)->EnableWindow(TRUE);GetDlgItem(IDC_GST_STATIC_PREFACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_GST_EDTHEIGHT)->EnableWindow(TRUE);GetDlgItem(IDC_GST_EDTPREFACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_GST_BSELECT)->EnableWindow(TRUE);GetDlgItem(IDC_GST_CHECK3)->EnableWindow(TRUE);
	}
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("CHK_DrawSurvey"), Chk_DrawSurveyStr);
}
//문자 크기
void CGST::OnEnUpdateGstEdtheight()
{
	UpdateData(TRUE);
	m_Height = _tstof(m_HeightStr);
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("Height"), m_HeightStr);
	UpdateData(FALSE);
}
//문자 크기 버튼
void CGST::OnBnClickedGstBselect()
{
	UpdateData(TRUE);
	ads_name en;
	ads_point pt;
	CGST GST;
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		CString strMsg;
		strMsg.LoadStringW(IDS_SELECT_TEXT); // "\n문자 선택 : "
		int stat = acedEntSel(strMsg, en, pt);

		if(stat == RTCAN || stat == RTNONE)
			break;
		else if(stat == RTERROR){
			resbuf zValueStr;
			ads_getvar(_T("ERRNO"), &zValueStr);// aced
			if(zValueStr.resval.rint == 52)//52  = NULL(ERRNO) Enter시
				GST.DoModal();
			else
				continue;
		
		}else if(stat == RTNORM){
			 AcGePoint3d insertPoint;
			 AcDbObjectId objId;
			 acdbGetObjectId(objId, en);
			 AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			 if(pEnt.openStatus() == Acad::eOk){
			 	if(pEnt->isKindOf(AcDbText::desc())){
			 		AcDbText* pText = AcDbText::cast(pEnt);
					double TxtHeight = pText->height();
					m_HeightStr.Format(_T("%.3f"), TxtHeight);
			 		pText->close();
					break;
				}else{
					strMsg.LoadStringW(IDS_CHECK_STRING); // "**선택한 객체는 문자가 아닙니다.**"
					acutPrintf(strMsg);
					continue;
				}
			 }
			pEnt->close();
			acedSSFree(en);	
		}
	}//while end	
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("Height"), m_HeightStr);
	CompleteEditorCommand();
	UpdateData(FALSE);
}
//머리말
void CGST::OnEnUpdateGstEdtpreface()
{
	UpdateData(TRUE);
	m_PrefaceStr;
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("Preface"), m_PrefaceStr);
	UpdateData(FALSE);
}
//체크박스 측점 0+000형식 유지
void CGST::OnBnClickedGstCheck3()
{
	b_Chk3 = IsDlgButtonChecked(IDC_GST_CHECK3);
	if(b_Chk3 == 0){
		m_Chk3_btn.SetCheck(FALSE);
		Chk_FormStr = _T("FALSE");
	}else{
		m_Chk3_btn.SetCheck(TRUE);
		Chk_FormStr = _T("TRUE");
	}
	iniFile.writeINI(_T("config.ini"), _T("GST"), _T("CHK_Form"), Chk_FormStr);
}
////각도를 라디안 값으로 변환하여 리턴한다.
double CGST::DTR(double angle)
{//ads_printf(_T("\n DTR() angle=%f \n"),angle);
	return (angle / 180.0 * PI);
}
//텍스트 생성
bool CGST::CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, int mode){
		Acad::ErrorStatus es;
		AcDbObjectId textStyleId, pTextObjId;
		double XScale = 0,obliquingAngle = 0;
		
		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
		if(pDb == NULL)return false;

		AcDbText* pText = new AcDbText();
		textStyleId = pDb->textstyle();
		pText->setTextStyle(textStyleId);
		AcDbTextStyleTableRecord *pTSTR=NULL;
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
			es = pText->setVerticalMode(AcDb::kTextBase);
			es = pText->setHorizontalMode(AcDb::kTextLeft);
		}else{
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

void CGST::OnBnClickedGstHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/GST.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
