#include "stdafx.h"
#include "CST.h"
#include "afxdialogex.h"

// CCST 대화 상자입니다.
IMPLEMENT_DYNAMIC(CCST, CAcUiDialog)
	CCST::CCST(CWnd* pParent)
	: CAcUiDialog(CCST::IDD, pParent), b_RadBtn1(0), b_RadBtn2(0), m_DistStr(_T("")), m_HeightStr(_T("")), m_PrefaceStr(_T("")), m_FooterStr(_T("")){
}

CCST::~CCST(){
}

void CCST::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_CST_RADSTATION, b_RadBtn1);
	DDX_Control(pDX, IDC_CST_CBXDOT, m_cbxDot);
	DDX_Text(pDX, IDC_CST_EDTDIST, m_DistStr);
	DDX_Radio(pDX, IDC_CST_RADEDIT, b_RadBtn2);
	DDX_Text(pDX, IDC_CST_EDTHEIGHT, m_HeightStr);
	DDX_Text(pDX, IDC_CST_EDTPREFACE, m_PrefaceStr);
	DDX_Text(pDX, IDC_CST_EDTFOOTER, m_FooterStr);
	DDX_Control(pDX, IDC_CST_BSELECT, m_bSelect);
}

BEGIN_MESSAGE_MAP(CCST, CAcUiDialog)
	ON_BN_CLICKED(IDC_CST_RADSTATION, &CCST::OnBnClickedCstRadstation)
	ON_BN_CLICKED(IDC_CST_RADNUMBER, &CCST::OnBnClickedCstRadnumber)
	ON_CBN_SELCHANGE(IDC_CST_CBXDOT, &CCST::OnCbnSelchangeCstCbxdot)
	ON_EN_UPDATE(IDC_CST_EDTDIST, &CCST::OnEnUpdateCstEdtdist)
	ON_BN_CLICKED(IDC_CST_RADWRITE, &CCST::OnBnClickedCstRadwrite)
	ON_BN_CLICKED(IDC_CST_RADEDIT, &CCST::OnBnClickedCstRadedit)
	ON_EN_UPDATE(IDC_CST_EDTHEIGHT, &CCST::OnEnUpdateCstEdtheight)
	ON_EN_UPDATE(IDC_CST_EDTPREFACE, &CCST::OnEnUpdateCstEdtpreface)
	ON_EN_UPDATE(IDC_CST_EDTFOOTER, &CCST::OnEnUpdateCstEdtfooter)
	ON_BN_CLICKED(IDC_CST_BSELECT, &CCST::OnBnClickedCstBselect)
	ON_BN_CLICKED(IDOK, &CCST::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCST::OnBnClickedCancel)
	ON_BN_CLICKED(ID_CST_HELP, &CCST::OnBnClickedCstHelp)
END_MESSAGE_MAP()

BOOL CCST::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	m_bSelect.AutoLoad();

	//소수점
	m_cbxDot.AddString(_T("0"));
	m_cbxDot.AddString(_T("1"));
	m_cbxDot.AddString(_T("2"));
	m_cbxDot.AddString(_T("3"));
	m_cbxDot.AddString(_T("4"));

	//측점 표기 STA = 1, NO = 2
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("CST"), _T("JOBMODE"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		OnBnClickedCstRadstation();
	else
		OnBnClickedCstRadnumber();

	//콤보박스 소수점
	tempStr = iniFile.readINI(_T("config.ini"), _T("CST"), _T("cbxDot"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0)
		m_cbxDot.SetCurSel(0);
	else{
		SetDlgItemText(IDC_CST_CBXDOT, tempStr);
		cbxInt = _ttoi(tempStr);
		m_cbxDot.SetCurSel(cbxInt);
	}

	//측점 간격
	tempStr = iniFile.readINI(_T("config.ini"), _T("CST"), _T("Dist"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_CST_EDTDIST, _T("20"));
	else
		SetDlgItemText(IDC_CST_EDTDIST, tempStr);

	//기존 문자 수정 = 1 도면에 쓰기 = 2
	tempStr = iniFile.readINI(_T("config.ini"), _T("CST"), _T("OPERMODE"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		OnBnClickedCstRadedit();
	else
		OnBnClickedCstRadwrite();

	//문자 크기
	tempStr = iniFile.readINI(_T("config.ini"), _T("CST"), _T("Height"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_CST_EDTHEIGHT, _T("3.000"));
	else
		SetDlgItemText(IDC_CST_EDTHEIGHT, tempStr);

	//머리말
	tempStr = iniFile.readINI(_T("config.ini"), _T("CST"), _T("Preface"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_CST_EDTPREFACE, _T("L="));
	else
		SetDlgItemText(IDC_CST_EDTPREFACE, tempStr);

	//꼬리말
	tempStr = iniFile.readINI(_T("config.ini"), _T("CST"), _T("Footer"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_CST_EDTFOOTER, _T("m"));
	else
		SetDlgItemText(IDC_CST_EDTFOOTER, tempStr);

	return TRUE;  
}

void CCST::OnBnClickedOk(){
	UpdateData();
	CAcUiDialog::OnOK();
	AcGePoint3d TxtPt1, TxtPt2, EditTextPt;
	CString Text1Num_Str, Text2Num_Str, TextSum_Str, TextSum_Format_Str, SurNum_Int1_Str, SurNum_Tail1_Str, SurNum_Int2_Str, SurNum_Tail2_Str, SurNumSum_Format_Str, SurNumSum_Str;
	double Text_Cnt, Text1Num, Text2Num, SurNum_Int1, SurNum_Tail1, SurNum_Int2, SurNum_Tail2, SurNum1, SurNum2, SurNum_cnt;
	ads_name Text1, Text2, EditText;
	CString strMsg;

	BeginEditorCommand();
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	while(true){//반복구간
		while(true){
			strMsg.LoadStringW(IDS_FIRST_STA_SELECT); // "\n첫번째 측점 문자 선택 : "
			int stat = acedEntSel(strMsg, Text1, asDblArray(TxtPt1));
			if(stat == RTCAN || stat == RTNONE){
				acedSSFree(Text1);
				return;
			}
			else if(stat == RTERROR){
				resbuf zValueStr;
				ads_getvar(_T("ERRNO"), &zValueStr);
				if(zValueStr.resval.rint == 52){
					acedSSFree(Text1);
					return;
				}
				else
					continue;
			}
			else if(stat == RTKWORD)
				continue;
			else if(stat == RTNORM){
				AcDbObjectId objId;
				acdbGetObjectId(objId, Text1);
				AcDbEntityPointer pEnt(objId, AcDb::kForRead);
				if(pEnt.openStatus() == Acad::eOk){
					if(pEnt->isKindOf(AcDbText::desc())){
						AcDbText* pText = AcDbText::cast(pEnt);
						Text1_Str = pText->textString();
						Text1Num_Str = CheckInputNumber(Text1_Str);
						SurNum_Int1_Str = CheckNumber(Text1_Str, 1);
						SurNum_Tail1_Str = CheckNumber(Text1_Str, 2);
						pText->close();
					}
					else{
						strMsg.LoadStringW(IDS_CHECK_STRING); // "\n**선택한 객체는 문자가 아닙니다.**"
						acutPrintf(strMsg);
						continue;
					}
				}
				pEnt->close();
				acedSSFree(Text1);
				break;
			}
		}
		while(true){
			strMsg.LoadStringW(IDS_SECOND_STA_SELECT); // "\n두번째 측점 문자 선택 : "
			int stat = acedEntSel(strMsg, Text2, asDblArray(TxtPt2));
			if(stat == RTCAN || stat == RTNONE){
				acedSSFree(Text2);
				return;
			}
			else if(stat == RTERROR){
				resbuf zValueStr;
				ads_getvar(_T("ERRNO"), &zValueStr);
				if(zValueStr.resval.rint == 52){
					acedSSFree(Text2);
					return;
				}
				else
					continue;
			}
			else if(stat == RTKWORD)
				continue;
			else if(stat == RTNORM){
				AcDbObjectId objId;
				acdbGetObjectId(objId, Text2);
				AcDbEntityPointer pEnt(objId, AcDb::kForRead);
				if(pEnt.openStatus() == Acad::eOk){
					if(pEnt->isKindOf(AcDbText::desc())){
						AcDbText* pText = AcDbText::cast(pEnt);
						Text2_Str = pText->textString();
						Text2Num_Str = CheckInputNumber(Text2_Str);
						SurNum_Int2_Str = CheckNumber(Text2_Str, 1);
						SurNum_Tail2_Str = CheckNumber(Text2_Str, 2);
						pText->close();
					}
					else{
						strMsg.LoadStringW(IDS_CHECK_STRING); // "\n**선택한 객체는 문자가 아닙니다.**"
						acutPrintf(strMsg);
						continue;
					}
				}
				pEnt->close();
				acedSSFree(Text2);
				break;
			}
		}
		CString Str_Dot;
		m_cbxDot.GetLBText(this->m_cbxDot.GetCurSel(), Str_Dot);//콤보박스 선택 텍스트가져오기
		int H_Dotcnt = _ttoi(Str_Dot);
		if(OperMode == 1){
			while(true){
				strMsg.LoadStringW(IDS_MODIFY_TEXT_SEL); // "\n내용을 수정할 문자 선택 : "
				int stat = acedEntSel(strMsg, EditText, asDblArray(EditTextPt));
				if(stat == RTCAN || stat == RTNONE){
					acedSSFree(EditText);
					return;
				}
				else if(stat == RTERROR){
					resbuf zValueStr;
					ads_getvar(_T("ERRNO"), &zValueStr);
					if(zValueStr.resval.rint == 52){
						acedSSFree(EditText);
						return;
					}
					else
						continue;
				}
				else if(stat == RTKWORD)
					continue;
				else if(stat == RTNORM){
					AcDbObjectId objId;
					acdbGetObjectId(objId, EditText);
					AcDbEntityPointer pEnt(objId, AcDb::kForWrite);
					if(pEnt.openStatus() == Acad::eOk){
						if(pEnt->isKindOf(AcDbText::desc())){
							AcDbText* pText = AcDbText::cast(pEnt);
							EditText_Str = pText->textString();
							if(JobMode == 1){//측점 표기 STA
								Text1Num = _tstof(Text1Num_Str);
								Text2Num = _tstof(Text2Num_Str);
								if(Text1Num < 0 && Text2Num >= 0 || Text1Num >= 0 && Text2Num < 0){
									if(Text1Num > Text2Num)
										Text_Cnt = abs(Text1Num - Text2Num);
									else
										Text_Cnt = abs(Text2Num - Text1Num);
								}
								else
									Text_Cnt = abs(Text1Num - Text2Num);

								TextSum_Format_Str.Format(_T("%s%%.%df%s"),m_PrefaceStr, H_Dotcnt, m_FooterStr);
								TextSum_Str.Format(TextSum_Format_Str, Text_Cnt);
								pText->setTextString(TextSum_Str);
							}
							else{//측점 표기 NO
								SurNum_Int1 = _tstof(SurNum_Int1_Str);
								SurNum_Tail1 = _tstof(SurNum_Tail1_Str);
								SurNum_Int2 = _tstof(SurNum_Int2_Str);
								SurNum_Tail2 = _tstof(SurNum_Tail2_Str);

								if(SurNum_Int1 < 0)
									SurNum1 = SurNum_Int1 * m_Dist - SurNum_Tail1;
								else
									SurNum1 = SurNum_Int1 * m_Dist + SurNum_Tail1;

								if(SurNum_Int2 < 0)
									SurNum2 = SurNum_Int2 * m_Dist - SurNum_Tail2;
								else
									SurNum2 = SurNum_Int2 * m_Dist + SurNum_Tail2;

								if(SurNum1 < 0 && SurNum2 >= 0 || SurNum1 >= 0 && SurNum2 < 0){
									if(SurNum1 > SurNum2)
										SurNum_cnt = abs(SurNum1 - SurNum2);
									else
										SurNum_cnt = abs(SurNum2 - SurNum1);
								}
								else
									SurNum_cnt = abs(SurNum1 - SurNum2);

								SurNumSum_Format_Str.Format(_T("%s%%.%df%s"),m_PrefaceStr, H_Dotcnt, m_FooterStr);
								SurNumSum_Str.Format(SurNumSum_Format_Str, SurNum_cnt);
								pText->setTextString(SurNumSum_Str);
							}
							pText->close();
						}
						else{
							strMsg.LoadStringW(IDS_CHECK_STRING); // "\n**선택한 객체는 문자가 아닙니다.**"
							acutPrintf(strMsg);
							continue;
						}
					}
					pEnt->close();
					acedSSFree(EditText);
					break;
				}
			}
		}
		else{//OperMode == 2
			while(true){
				strMsg.LoadStringW(IDS_CAL_TXT_INTPT_SEL); // "\n계산된 문자를 삽입할 점 선택 : "
				int stat = acedGetPoint(0, strMsg, asDblArray(EditTextPt));
				if(stat == RTCAN || stat == RTNONE){
					acedSSFree(EditText);
					return;
				}
				else if(stat == RTERROR){
					resbuf zValueStr;
					ads_getvar(_T("ERRNO"), &zValueStr);
					if(zValueStr.resval.rint == 52){
						acedSSFree(EditText);
						return;
					}
					else
						continue;
				}
				else if(stat == RTNORM){
					if(JobMode == 1){//측점 표기 STA
						Text1Num = _tstof(Text1Num_Str);
						Text2Num = _tstof(Text2Num_Str);
						if(Text1Num < 0 && Text2Num >= 0 || Text1Num >= 0 && Text2Num < 0){
							if(Text1Num > Text2Num)
								Text_Cnt = abs(Text1Num - Text2Num);
							else
								Text_Cnt = abs(Text2Num - Text1Num);
						}
						else
							Text_Cnt = abs(Text1Num - Text2Num);

						TextSum_Format_Str.Format(_T("%s%%.%df%s"),m_PrefaceStr, H_Dotcnt, m_FooterStr);
						TextSum_Str.Format(TextSum_Format_Str, Text_Cnt);
						CreateText(EditTextPt, m_Height, 1.0, 0.0, TextSum_Str, _T("0"), 1);
					}
					else{//측점 표기 NO
						SurNum_Int1 = _tstof(SurNum_Int1_Str);
						SurNum_Tail1 = _tstof(SurNum_Tail1_Str);
						SurNum_Int2 = _tstof(SurNum_Int2_Str);
						SurNum_Tail2 = _tstof(SurNum_Tail2_Str);

						if(SurNum_Int1 < 0)
							SurNum1 = SurNum_Int1 * m_Dist - SurNum_Tail1;
						else
							SurNum1 = SurNum_Int1 * m_Dist + SurNum_Tail1;

						if(SurNum_Int2 < 0)
							SurNum2 = SurNum_Int2 * m_Dist - SurNum_Tail2;
						else
							SurNum2 = SurNum_Int2 * m_Dist + SurNum_Tail2;

						if(SurNum1 < 0 && SurNum2 >= 0 || SurNum1 >= 0 && SurNum2 < 0){
							if(SurNum1 > SurNum2)
								SurNum_cnt = abs(SurNum1 - SurNum2);
							else
								SurNum_cnt = abs(SurNum2 - SurNum1);
						}
						else
							SurNum_cnt = abs(SurNum1 - SurNum2);

						SurNumSum_Format_Str.Format(_T("%s%%.%df%s"), m_PrefaceStr, H_Dotcnt, m_FooterStr);
						SurNumSum_Str.Format(SurNumSum_Format_Str, SurNum_cnt);
						CreateText(EditTextPt, m_Height, 1.0, 0.0, SurNumSum_Str, _T("0"), 1);
					}
					break;
				}
			}
		}
	}
	CompleteEditorCommand();
}

void CCST::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

//측점 표기 STA
void CCST::OnBnClickedCstRadstation(){
	UpdateData(TRUE);
	JobMode = 1;
	JobMode_Str.Format(_T("%d"), JobMode);
	b_RadBtn1 = 0;
	GetDlgItem(IDC_CST_STATIC_DIST)->EnableWindow(FALSE);
	GetDlgItem(IDC_CST_EDTDIST)->EnableWindow(FALSE);
	iniFile.writeINI(_T("config.ini"), _T("CST"), _T("JOBMODE"), JobMode_Str);
	UpdateData(FALSE);
}

//측점 표기 NO
void CCST::OnBnClickedCstRadnumber(){
	UpdateData(TRUE);
	JobMode = 2;
	JobMode_Str.Format(_T("%d"), JobMode);
	b_RadBtn1 = 1;
	GetDlgItem(IDC_CST_STATIC_DIST)->EnableWindow(TRUE);
	GetDlgItem(IDC_CST_EDTDIST)->EnableWindow(TRUE);
	iniFile.writeINI(_T("config.ini"), _T("CST"), _T("JOBMODE"), JobMode_Str);
	UpdateData(FALSE);
}

//콤보박스 소수점
void CCST::OnCbnSelchangeCstCbxdot(){
	UpdateData(TRUE);
	cbxInt = m_cbxDot.GetCurSel();
	if(cbxInt == -1)cbxInt = 0;
	m_cbxDot.SetCurSel(cbxInt);
	cbxInt_Str.Format(_T("%d"), cbxInt);
	iniFile.writeINI(_T("config.ini"), _T("CST"), _T("cbxDot"), cbxInt_Str);
	UpdateData(FALSE);
}

//측점 간격
void CCST::OnEnUpdateCstEdtdist(){
	UpdateData(TRUE);
	m_Dist = _tstof(m_DistStr);
	iniFile.writeINI(_T("config.ini"), _T("CST"), _T("Dist"), m_DistStr);
	UpdateData(FALSE);
}

//기존 문자 수정
void CCST::OnBnClickedCstRadedit(){
	UpdateData(TRUE);
	OperMode = 1;
	OperMode_Str.Format(_T("%d"), OperMode);
	b_RadBtn2 = 0;
	GetDlgItem(IDC_CST_STATIC_TXTHEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CST_EDTHEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CST_BSELECT)->EnableWindow(FALSE);
	iniFile.writeINI(_T("config.ini"), _T("CST"), _T("OPERMODE"), OperMode_Str);
	UpdateData(FALSE);
}

//도면에 계산 값 쓰기
void CCST::OnBnClickedCstRadwrite(){
	UpdateData(TRUE);
	OperMode = 2;
	OperMode_Str.Format(_T("%d"), OperMode);
	b_RadBtn2 = 1;
	GetDlgItem(IDC_CST_STATIC_TXTHEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CST_EDTHEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CST_BSELECT)->EnableWindow(TRUE);
	iniFile.writeINI(_T("config.ini"), _T("CST"), _T("OPERMODE"), OperMode_Str);
	UpdateData(FALSE);
}

//문자 크기
void CCST::OnEnUpdateCstEdtheight(){
	UpdateData(TRUE);
	m_Height = _tstof(m_HeightStr);
	iniFile.writeINI(_T("config.ini"), _T("CST"), _T("Height"), m_HeightStr);
	UpdateData(FALSE);
}

//문자 크기 가져오기 버튼
void CCST::OnBnClickedCstBselect(){
	UpdateData(TRUE);
	ads_name en;
	ads_point pt;
	CString strMsg;
	CCST CST;
	BeginEditorCommand();
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	while(true){
		strMsg.LoadStringW(IDS_SELECT_TEXT); // _T("문자 선택: ")
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
					m_HeightStr.Format(_T("%.3f"), TxtHeight);
					pText->close();
					break;
				}
				else{
					strMsg.LoadStringW(IDS_CHECK_STRING); // "\n**선택한 객체는 문자가 아닙니다.**"
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();
			acedSSFree(en);
		}
	}
	iniFile.writeINI(_T("config.ini"), _T("CST"), _T("Height"), m_HeightStr);
	CompleteEditorCommand();
	UpdateData(FALSE);
}

//머리맣
void CCST::OnEnUpdateCstEdtpreface(){
	UpdateData(TRUE);
	m_PrefaceStr;
	iniFile.writeINI(_T("config.ini"), _T("CST"), _T("Preface"), m_PrefaceStr);
	UpdateData(FALSE);
}

//꼬리말
void CCST::OnEnUpdateCstEdtfooter(){
	UpdateData(TRUE);
	m_FooterStr;
	iniFile.writeINI(_T("config.ini"), _T("CST"), _T("Footer"), m_FooterStr);
	UpdateData(FALSE);
}

//숫자 및 점(.) 추출
CString CCST::CheckInputNumber(CString str){
	TCHAR* getAtChar, *getDotChar;
	CString strNum, strRe = _T(""), strFDot = _T("");
	int k = 0;
	for(int i = 0; i < str.GetLength(); i++){
		strNum = str.Mid(i, 1);	
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		if(((ascii >= 48 && ascii <= 57) || ascii == 46 || ascii == 45)){
			strRe.Append(getAtChar);
			if(strRe.Mid(0, 1) == _T(".") || strRe.Mid(0, 1) == _T("0"))
				strRe.Delete(0, 1);
		}
	}
	return strRe;
} 

//숫자 및 점(.) (+) (-) 추출
CString CCST::CheckNumber(CString str, int Mode){
	CString strNum, strRe = _T("");
	int DotNum, PM_Num;
	bool bDot, bPMnumber = true;
	for(int i = 0; i < str.GetLength(); i++){
		strNum = str.Mid(i, 1);	
		if (strNum == _T(".")) {
			DotNum = i;
			bDot = true;
			break;
		}
		else
			bDot = false;
	}
	for(int i = 0; i < str.GetLength(); i++){
		strNum = str.Mid(i, 1);	
		if(strNum == _T("+")){
			PM_Num = i;
			bPMnumber = true;
			break;
		}
		else
			bPMnumber = false;
	}

	if (bDot == false && bPMnumber == false)
		return strRe;
	else if(Mode == 1)
		strRe = str.Mid(DotNum + 1, (PM_Num - DotNum - 1));
	else
		strRe = str.Mid(PM_Num + 1, str.GetLength() - 1);

	return strRe;
}

bool CCST::CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, int mode){
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

void CCST::OnBnClickedCstHelp()
{
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

	Help_Path.Format(_T("%s\\%s::/1. html/CST.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}