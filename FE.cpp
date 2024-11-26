#include "stdafx.h"
#include "FE.h"
#include "afxdialogex.h"

// CFE ��ȭ �����Դϴ�.
IMPLEMENT_DYNAMIC(CFE, CAcUiDialog)
	CFE::CFE(CWnd* pParent)
	: CAcUiDialog(CFE::IDD, pParent),m_LayerStr(_T("")), m_DecimalStr(_T("")), m_ScaleStr(_T("")), m_PreFaceStr(_T("")), m_TxtHeightStr(_T("")), R_upNdown(0){
}

CFE::~CFE(){
}

void CFE::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FELAYER, m_LayerStr);
	DDX_Text(pDX, IDC_FESCALE, m_ScaleStr);
	DDX_Radio(pDX, IDC_RADIOFEUP, R_upNdown);
	DDX_Text(pDX, IDC_FEDECIMAL, m_DecimalStr);
	DDX_Text(pDX, IDC_FEPREFACE, m_PreFaceStr);
	DDX_Text(pDX, IDC_FEHEIGHT, m_TxtHeightStr);
	DDX_Control(pDX, IDC_FEHEIGHT, m_TextHeight_Ctrl);
	DDX_Control(pDX, IDC_CHECKFELEADER, m_Leader_btn);
	DDX_Control(pDX, IDC_CHECKFESPOINT, m_Chkpoint_btn);
	DDX_Control(pDX, IDC_CHECKFEHORIZONTAL, m_Horizontal_btn);
	DDX_Control(pDX, IDC_CHECKFEHPOINT, m_HorizontalSTpt_btn);
}

BEGIN_MESSAGE_MAP(CFE, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CFE::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFE::OnBnClickedCancel)
	ON_EN_UPDATE(IDC_FELAYER, &CFE::OnEnUpdateFelayer)
	ON_EN_UPDATE(IDC_FESCALE, &CFE::OnEnUpdateFescale)
	ON_EN_UPDATE(IDC_FEHEIGHT, &CFE::OnEnUpdateFeheight)
	ON_EN_UPDATE(IDC_FEDECIMAL, &CFE::OnEnUpdateFedecimal)
	ON_EN_UPDATE(IDC_FEPREFACE, &CFE::OnEnUpdateFepreface)
	ON_BN_CLICKED(IDC_RADIOFEUP, &CFE::OnBnClickedRadiofeup)
	ON_BN_CLICKED(IDC_RADIOFEDOWN, &CFE::OnBnClickedRadiofedown)
	ON_BN_CLICKED(IDC_CHECKFESPOINT, &CFE::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_CHECKFEHPOINT, &CFE::OnBnClickedCheckfehpoint)
	ON_BN_CLICKED(IDC_CHECKFELEADER, &CFE::OnBnClickedCheckfeleader)
	ON_BN_CLICKED(IDC_CHECKFEHORIZONTAL, &CFE::OnBnClickedCheckfehorizontal)
END_MESSAGE_MAP()

BOOL CFE::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	m_bSelect.AutoLoad();

	//������
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("Layer"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FELAYER, _T("EL"));
	else
		SetDlgItemText(IDC_FELAYER, tempStr);

	//�Ҽ���
	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("Decimal"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FEDECIMAL, _T("3"));
	else
		SetDlgItemText(IDC_FEDECIMAL, tempStr);

	//��ô
	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("Scale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FESCALE, _T("1000"));
	else
		SetDlgItemText(IDC_FESCALE, tempStr);

	//�Ӹ���
	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("PreFace"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FEPREFACE, _T(""));
	else
		SetDlgItemText(IDC_FEPREFACE, tempStr);

	//���� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("TxtHeight"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FEHEIGHT, _T("0.300"));
	else
		SetDlgItemText(IDC_FEHEIGHT, tempStr);

	//���� �� �� �Ʒ�
	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("UPDOWN"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		OnBnClickedRadiofedown();
	else
		OnBnClickedRadiofeup();

	//�������� point ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("ChkPoint"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("FALSE")) == 0){
		b_Chkpoint = false;
		m_Chkpoint_btn.SetCheck(FALSE);
	}
	else if(tempStr.CompareNoCase(_T("TRUE")) == 0){
		b_Chkpoint = true;
		m_Chkpoint_btn.SetCheck(TRUE);
	}

	//����Ÿ� ǥ��
	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("ChkHorizontal"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("FALSE")) == 0){
		b_Horizontal = false;
		m_Horizontal_btn.SetCheck(FALSE);
		m_HorizontalSTpt_btn.EnableWindow(FALSE);
		m_Leader_btn.EnableWindow(FALSE);
	}
	else if(tempStr.CompareNoCase(_T("TRUE")) == 0){
		b_Horizontal = true;
		m_Horizontal_btn.SetCheck(TRUE);
		m_HorizontalSTpt_btn.EnableWindow(TRUE);
		m_Leader_btn.EnableWindow(TRUE);
	}

	//����Ÿ� ������ ���� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("ChkHpoint"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("FALSE")) == 0){
		b_HorizontalSTpt = false;
		m_HorizontalSTpt_btn.SetCheck(FALSE);
	}
	else if(tempStr.CompareNoCase(_T("TRUE")) == 0){
		b_HorizontalSTpt = true;
		m_HorizontalSTpt_btn.SetCheck(TRUE);
	}

	//���ü� ǥ��
	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("ChkLeader"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("FALSE")) == 0){
		b_Leader = false;
		m_Leader_btn.SetCheck(FALSE);
		GetDlgItem(IDC_RADIOFEUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIOFEDOWN)->EnableWindow(TRUE);
	}
	else if(tempStr.CompareNoCase(_T("TRUE")) == 0){
		b_Leader = true;
		m_Leader_btn.SetCheck(TRUE);
		GetDlgItem(IDC_RADIOFEUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIOFEDOWN)->EnableWindow(FALSE);
	}
	return TRUE;
}

void CFE::OnBnClickedOk(){
	UpdateData();
	CAcUiDialog::OnOK();
	CString UPDOWN;
	UPDOWN.Format(_T("%d"), R_upNdown);
	iniFile.writeINI(_T("config.ini"), _T("FE"), _T("Layer"), m_LayerStr);
	iniFile.writeINI(_T("config.ini"), _T("FE"), _T("Decimal"), m_DecimalStr);
	iniFile.writeINI(_T("config.ini"), _T("FE"), _T("Scale"), m_ScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("FE"), _T("PreFace"), m_PreFaceStr);
	iniFile.writeINI(_T("config.ini"), _T("FE"), _T("TxtHeight"), m_TxtHeightStr);
	iniFile.writeINI(_T("config.ini"), _T("FE"), _T("UPDOWN"), UPDOWN);

	double m_ELValue, m_Scale, m_TxtHeight, m_Decimal;
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("ELValue"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_ELValue = 0.00;
	else
		m_ELValue = _tstof(tempStr);

	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("ChkPoint"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_CheckPoint_Str = _T("FALSE");	// �輺�� �븮 �߰�
		//m_CheckPoint_Str = _T("TRUE");
	else
		m_CheckPoint_Str = tempStr;

	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("ChkHorizontal"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_Horizontal_Str = _T("FALSE");	// �輺�� �븮 �߰�
		//m_Horizontal_Str = _T("TRUE");
	else
		m_Horizontal_Str = tempStr;

	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("ChkHpoint"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_HorizontalSTpt_Str = _T("FALSE");	// �輺�� �븮 �߰�
		//m_HorizontalSTpt_Str = _T("TRUE");
	else
		m_HorizontalSTpt_Str = tempStr;

	tempStr = iniFile.readINI(_T("config.ini"), _T("FE"), _T("ChkLeader"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_Leader_Str = _T("FALSE");		// �輺�� �븮 �߰�
		//m_Leader_Str = _T("TRUE");
	else
		m_Leader_Str = tempStr;

	m_Scale = _tstof(m_ScaleStr);
	m_TxtHeight = _tstof(m_TxtHeightStr);
	CString S_ScaleStr, EntStr, inString, RTERROR_str, Scale_Print, Scale_Print2;
	ads_name ens;
	int stat;
	AcGePoint3d ELpt;
	bool b_TF = true;

	BeginEditorCommand();
	while(b_TF){
		acedInitGet(0, _T("c C"));
		EntStr.Format(_T("������ EL ���� �Ǵ� �� �Է�(%.3f)<��ô ����(C)>: "), m_ELValue);
		Scale_Print.Format(_T("\n< ���缳�� : ��ô=%0.2f >"), m_Scale);
		Scale_Print2.Format(_T("< ���缳�� : ��ô=%0.2f >"), m_Scale);
		acutPrintf(Scale_Print);
		stat = acedEntSel(EntStr, ens, asDblArray(ELpt));
		if(stat == RTCAN || stat == RTNONE)
			b_TF = false;
		else if(stat == RTKWORD){
			resbuf zValueStr;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
			RTERROR_str.Replace(EntStr, _T(""));
			if(RTERROR_str.CompareNoCase(_T("")) == 0)
				break;
			else if(RTERROR_str.CompareNoCase(_T("C")) == 0 || RTERROR_str.CompareNoCase(_T("c")) == 0){
				S_ScaleStr.Format(_T("\n��ô �Է�<%.2f> : "),m_Scale);
				if(acedGetReal(S_ScaleStr,&m_Scale) == RTNORM){
					m_ScaleStr.Format(_T("%0.f"), m_Scale);
					iniFile.writeINI(_T("config.ini"), _T("FE"), _T("Scale"), m_ScaleStr);
				}
			}
			else{
				if(RTERROR_str.GetLength() > 0){
					m_ELValue_Str.Format(_T("%s"),RTERROR_str);
					iniFile.writeINI(_T("config.ini"), _T("FW"), _T("ELValue"), m_ELValue_Str);
					break;
				}
				else{
					m_ELValue_Str.Format(_T("%f"), m_ELValue);
					break;
				}
			}
		}
		else if(stat == RTERROR){
			resbuf zValueStr, zValueStr2;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
			RTERROR_str.Replace(EntStr, _T(""));
			ads_getvar(_T("ERRNO"), &zValueStr2);
			if(zValueStr2.resval.rint == 52)
			{	// ���� �������� �ʰų� ��ü�� �������� ���� ���, EL���� �߰����� ����
				m_ELValue_Str.Format(_T("%f"),m_ELValue);	// �輺�� �븮 ����
				break;
			}
			if(RTERROR_str.CompareNoCase(Scale_Print2) == 0)
				continue;			
			else if(RTERROR_str.CompareNoCase(_T("**������ ��ü�� ���ڰ� �ƴմϴ�.**")) == 0)
				continue;
			else{
				if(RTERROR_str.GetLength() > 0){
					m_ELValue_Str.Format(_T("%s"),RTERROR_str);
					iniFile.writeINI(_T("config.ini"), _T("FE"), _T("ELValue"),m_ELValue_Str);
					break;
				}else{
					m_ELValue_Str.Format(_T("%f"),m_ELValue);
					break;
				}
			}
		}
		else if(stat == RTNORM){
			AcGePoint3d insertPoint;
			AcDbObjectId objId;
			acdbGetObjectId(objId, ens);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbText::desc())){
					AcDbText* pText = AcDbText::cast(pEnt);
					inString.Format(_T("%s"), pText->textString());
					inString = CheckInputNumber(inString);
					pText->close();
				}
				else{
					acutPrintf(_T("\n**������ ��ü�� ���ڰ� �ƴմϴ�.**\n"));
					continue;
				}
			}
			pEnt->close();
			acedSSFree(ens);	
			m_ELValue_Str = inString;
			iniFile.writeINI(_T("config.ini"), _T("FE"), _T("ELValue"), m_ELValue_Str);
			b_TF = false;
		}
	}
	m_ELValue = _tstof(m_ELValue_Str);
	CString getELpt_Str, Txtstr1, Txt1, ELtxt, ELtxtstr, Ltxt, Ltxtstr;
	AcGePoint3d STspt, STept, Txtpt, Hztpt, LeaderPt1,LeaderPt2, UnderPt, LeaderELpt, LeaderLpt, tmp_STspt;
	AcGePoint3dArray ptArray;
	int H_DecimalNum = _ttoi(m_DecimalStr);
	double UnderLength;

	getELpt_Str.Format(_T("\n������(%.3f)���� : "), m_ELValue);
	if(acedGetPoint(NULL, getELpt_Str, asDblArray(STspt)) != RTNORM)
		return;
	tmp_STspt = STspt;
	acdbUcs2Wcs(asDblArray(STspt),asDblArray(STspt),Adesk::kFalse); // �輺�� �븮 �߰�
	if(m_Horizontal_Str == _T("TRUE")){
		if(m_HorizontalSTpt_Str == _T("TRUE")){
			int GetptInt = acedGetPoint(0, _T("\n�Ÿ� ǥ�� ������ ���� : "), asDblArray(Hztpt));
			acdbUcs2Wcs(asDblArray(Hztpt),asDblArray(Hztpt),Adesk::kFalse); // �輺�� �븮 �߰�
			if(GetptInt == RTNONE)
				Hztpt = STspt;
			else if(GetptInt == RTCAN)
				return;
			else if(GetptInt != RTNORM)
				return;
		}
	}
	while(true){
		if(acedGetPoint(asDblArray(tmp_STspt),_T("\n��ȹ�� ���� : "), asDblArray(STept)) != RTNORM)
			return;
		acdbUcs2Wcs(asDblArray(STept),asDblArray(STept),Adesk::kFalse); // �輺�� �븮 �߰�
		if(m_Horizontal_Str == _T("TRUE") || m_Leader_Str == _T("TRUE")){
			Txt1.Format(_T("%s%%.%df (L=%%.%df)"), m_PreFaceStr, H_DecimalNum, H_DecimalNum);
			Txtstr1.Format(Txt1, m_ELValue + (STept.y - STspt.y) * m_Scale / 1000, abs(Hztpt.x - STept.x) * m_Scale / 1000);
			ELtxt.Format(_T("EL=%%.%df"), H_DecimalNum);
			ELtxtstr.Format(ELtxt, m_ELValue + (STept.y - STspt.y) * m_Scale / 1000);
			if(m_HorizontalSTpt_Str == _T("TRUE")){
				Ltxt.Format(_T("L=%%.%dfm"), H_DecimalNum);
				Ltxtstr.Format(Ltxt, abs(Hztpt.x - STept.x) * m_Scale / 1000);
			}
			else{
				Ltxt.Format(_T("L=%%.%dfm"), H_DecimalNum);
				Ltxtstr.Format(Ltxt, abs(STspt.x - STept.x) * m_Scale / 1000);
			}
		}
		else{
			Txt1.Format(_T("%s%%.%df"), m_PreFaceStr, H_DecimalNum);
			Txtstr1.Format(Txt1, m_ELValue + (STept.y - STspt.y) * m_Scale / 1000);
		}

		if(m_CheckPoint_Str == _T("TRUE"))
			CreatePoint(STept, _T("0"));
		if(R_upNdown == 0)
			ads_polar(asDblArray(STept), DTR(90.0), 0.3 * 1000 / m_Scale, asDblArray(Txtpt));
		else if(R_upNdown == 1)
			ads_polar(asDblArray(STept), DTR(270.0), 0.3 * 1000 / m_Scale, asDblArray(Txtpt));

		CreateLayer(m_LayerStr, 2);
		if(m_Horizontal_Str == _T("FALSE"))
			//CreateText(Txtpt, 1000 / m_Scale * m_TxtHeight, 1, 0.0, Txtstr1, m_LayerStr, 0);	
			CreateText(Txtpt, m_TxtHeight, 1, 0.0, Txtstr1, m_LayerStr, 3);	// �輺�� �븮 ����
		else{
			if(m_Leader_Str == _T("TRUE"))
			{
				if(acedGetPoint(asDblArray(STept), _T("\n���ü� ���� �� ���� : "), asDblArray(LeaderPt1)) != RTNORM)
					return;
				acdbUcs2Wcs(asDblArray(LeaderPt1),asDblArray(LeaderPt1),Adesk::kFalse); // �輺�� �븮 �߰�
				if(acedGetPoint(asDblArray(LeaderPt1), _T("\n���ü� ���� ���� : "), asDblArray(LeaderPt2)) != RTNORM)
					return;
				acdbUcs2Wcs(asDblArray(LeaderPt2),asDblArray(LeaderPt2),Adesk::kFalse); // �輺�� �븮 �߰�
				double LeaderAng = ads_angle(asDblArray(LeaderPt1), asDblArray(LeaderPt2));
				if(0 <= LeaderAng && LeaderAng <= DTR(90.0) || DTR(270.0) <= LeaderAng && LeaderAng <= DTR(360.0))
					LeaderAng = DTR(0.0);
				else
					LeaderAng = DTR(180.0);

				if(H_DecimalNum == 0)
					UnderLength = (Ltxtstr.GetLength() * 0.2331 + 0.3776 + 0.2) * (m_TxtHeight / 0.3);
				else if(H_DecimalNum > 0)
					UnderLength = ((Ltxtstr.GetLength() - 1) * 0.2331 + 0.3776 + 0.1164 + 0.2) * (m_TxtHeight / 0.3);

				ads_polar(asDblArray(LeaderPt1), LeaderAng, 1000 / m_Scale*UnderLength, asDblArray(UnderPt));
				ptArray.append(STept);
				ptArray.append(UnderPt);
				ptArray.append(LeaderPt1);

				CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LayerStr);
				ptArray.removeAll();
				if(abs(LeaderAng - 0.0) < 0.000001)
				{
					LeaderELpt.x = LeaderPt1.x + (1000 / m_Scale * 0.2);
					LeaderELpt.y = UnderPt.y + (1000 / m_Scale * 0.15);
					LeaderLpt.x = LeaderPt1.x + (1000 / m_Scale * 0.2);
					LeaderLpt.y = UnderPt.y - (1000 / m_Scale * 0.15);
				}
				else if(abs(LeaderAng - DTR(180.0)) < 0.000001)
				{
					LeaderELpt.x = UnderPt.x + (1000 / m_Scale * 0.2);
					LeaderELpt.y = UnderPt.y + (1000 / m_Scale * 0.15);
					LeaderLpt.x = UnderPt.x + (1000 / m_Scale * 0.2);
					LeaderLpt.y = UnderPt.y - (1000 / m_Scale * 0.15);
				}
				CreateText(LeaderELpt, m_TxtHeight, 1, 0, ELtxtstr, m_LayerStr, 1);
				CreateText(LeaderLpt, m_TxtHeight, 1, 0, Ltxtstr, m_LayerStr, 2);
			}
			else if(m_Leader_Str == _T("FALSE") || m_HorizontalSTpt_Str == _T("FALSE"))
				//CreateText(Txtpt, 1000 / m_Scale * m_TxtHeight, 1, 0.0, Txtstr1, m_LayerStr, 0);
				CreateText(Txtpt, m_TxtHeight, 1, 0.0, Txtstr1, m_LayerStr, 3);	// ������ ��带 ����(�輺�� �븮 �߰�)
		}
	}
	CompleteEditorCommand();
}

void CFE::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

//ELǥ�� ���� �� ��
void CFE::OnBnClickedRadiofeup(){
	UpdateData(TRUE);
	R_upNdown = 0;
	UpdateData(FALSE);
}

//ELǥ�� ���� �� �Ʒ�
void CFE::OnBnClickedRadiofedown(){
	UpdateData(TRUE);
	R_upNdown = 1;
	UpdateData(FALSE);
}

//���̾�
void CFE::OnEnUpdateFelayer(){
	UpdateData(TRUE);
	m_LayerStr;
	UpdateData(FALSE);
}

//�Ҽ���
void CFE::OnEnUpdateFedecimal(){
	UpdateData(TRUE);
	DotCnt = _ttoi(m_DecimalStr);
	UpdateData(FALSE);
	if(DotCnt < 0 )
		m_DecimalStr = _T("0");
	else if(DotCnt > 6)
		m_DecimalStr = _T("6");

	UpdateData(FALSE);
}

//��ô
void CFE::OnEnUpdateFescale(){
	UpdateData(TRUE);
	CEdit *pEdit;
	pEdit = (CEdit*)GetDlgItem(IDC_FESCALE);
	CheckInputNumber(pEdit,false);
	UpdateData(FALSE);
}

//�Ӹ���
void CFE::OnEnUpdateFepreface(){
	UpdateData(TRUE);
	m_PreFaceStr;
	UpdateData(FALSE);
}

//���� ����
void CFE::OnEnUpdateFeheight(){
	UpdateData(TRUE);
	m_TxtHeightStr = CheckInputNumber(m_TxtHeightStr);
	UpdateData(FALSE);

	m_TextHeight_Ctrl.SetSel(0, -1);
	m_TextHeight_Ctrl.SetSel(-1, -1);
	UpdateData(FALSE);
}

//���� �� ��(.) �Է� üũ
CString CFE::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	CString strNum, strRe = _T("");
	int k = 0;
	for(int i = 0; i < str.GetLength(); i++){
		strNum = str.Mid(i, 1);
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		if(((ascii >= 48 && ascii <= 57) || ascii == 46))
			strRe.Append(getAtChar);
	}
	return strRe;
}

void CFE::CheckInputNumber(CEdit *cedit, bool minus)
{
	//// [ 1. initialize value ]
	CString cstrText;
	cedit->GetWindowTextW(cstrText);
	char* chText = new char[strlen(CT2A(cstrText))+1];
	strcpy(chText, CT2A(cstrText));
	int count = strlen(chText);

	bool pointChecker = false;
	for (int i = 0; i < count; i++) {
		if (!pointChecker && chText[i] == '.') pointChecker = true;
		else if (pointChecker && chText[i] == '.') chText[i] = NULL;
	}

	int startIdx = (minus) ? 1 : 0;
	for (int i = startIdx; i < count; i++) {
		if (chText[i] == '-') chText[i] = NULL;
	}

	for (int i = 0; i < count; i++) {
		if (!(chText[i] >= 48 && chText[i] <= 57)) {
			if (chText[i] != '.' && chText[i] != '-') chText[i] = NULL;
		}
	}

	m_ScaleStr = CA2T(chText);
	//cedit->SetWindowTextW(CA2T(chText));

	cedit->SetSel(0, -1);
	cedit->SetSel(-1, -1);

	return;
}

////������ ���� ������ ��ȯ�Ͽ� �����Ѵ�.
double CFE::DTR(double angle){
	return (angle / 180.0 * PI);
}

//�ؽ�Ʈ ����
bool CFE::CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, int mode){
	Acad::ErrorStatus es;
	AcDbObjectId textStyleId, pTextObjId;
	double XScale = 0, obliquingAngle = 0;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return false;

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
		es = pText->setVerticalMode(AcDb::kTextBottom);
		es = pText->setHorizontalMode(AcDb::kTextLeft);
	}
	else if(mode == 2){
		es = pText->setVerticalMode(AcDb::kTextTop);
		es = pText->setHorizontalMode(AcDb::kTextLeft);
	}	// �輺�� �븮 �߰�
	else if(mode == 3){		
		es = pText->setHorizontalMode(AcDb::kTextLeft);
	}	// �輺�� �븮 �߰� ��
	else{
		es = pText->setVerticalMode(AcDb::kTextBottom);
		es = pText->setHorizontalMode(AcDb::kTextMid);
	}
	//
	pText->setAlignmentPoint(pt);
	pText->setHeight(Height);
	pText->setWidthFactor(Width);
	pText->setRotation(Rotate);
	pText->setTextString((LPCTSTR)TxtStr);

	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk)
	{
		int cvportindex;
		resbuf zValueStr;
		ads_getvar(_T("CVPORT"), &zValueStr);
		cvportindex = zValueStr.resval.rint;

		int Tileindex;
		resbuf TileStr;
		ads_getvar(_T("TILEMODE"), &TileStr);
		Tileindex = TileStr.resval.rint;
		if(cvportindex == 2)
		{
			if (Tileindex == 0) { 
				if (pBT->getAt(ACDB_PAPER_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk)
				{
					es = pBTR->appendAcDbEntity(pTextObjId, pText);
					pBTR->close();
					pText->close();
				}
			}
			else {
				if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk)
				{
					es = pBTR->appendAcDbEntity(pTextObjId, pText);
					pBTR->close();
					pText->close();
				}
			}
		}
		else
		{
			if (Tileindex == 0) {
				if (pBT->getAt(ACDB_PAPER_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk)
				{
					es = pBTR->appendAcDbEntity(pTextObjId, pText);
					pBTR->close();
					pText->close();
				}
			}
			else {
				if (pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk)
				{
					es = pBTR->appendAcDbEntity(pTextObjId, pText);
					pBTR->close();
					pText->close();
				}
			}
		}
		/*
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk)
		{
			es = pBTR->appendAcDbEntity(pTextObjId, pText);
			pBTR->close();
			pText->close();
		}
		*/
		pBT->close();
	}
	return true;
}

//���̾� ����
bool CFE::CreateLayer(CString Layer, int color){
	AcDbLayerTable* pLayerTable;
	AcCmColor Layercolor;

	Layercolor.setColorIndex(color);
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return false;

	pDb->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (pLayerTable->has(Layer) != Adesk::kTrue) {
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

//������ ����
bool CFE::CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer){
	Acad::ErrorStatus es;
	AcDbObjectId pLineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();

	if(pDb == NULL)
		return false;

	AcDbPolyline* pPline = new AcDbPolyline();
	for(int i = 0; i < ptArray.length(); i++){
		pPline->addVertexAt(i, AcGePoint2d(ptArray[i].x, ptArray[i].y));
	}
	pPline->setConstantWidth(LineWidth);
	pPline->setLinetype(LineType);
	pPline->setLinetypeScale(LineTypeScale);
	pPline->setLayer(Layer);

	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk)
	{
		CString cvportStr;
		resbuf zValueStr;
		ads_getvar(_T("LASTPROMPT"), &zValueStr);
		cvportStr.Format(_T("%s"), zValueStr.resval.rstring);
		if(cvportStr == "2")
		{
			if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk)
			{
				es = pBTR->appendAcDbEntity(pLineObjId, pPline);
				pBTR->close();
				pPline->close();
			}
			else
				return false;
			pBT->close();
		}
		else
		{
			if(pBT->getAt(ACDB_PAPER_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk)
			{
				es = pBTR->appendAcDbEntity(pLineObjId, pPline);
				pBTR->close();
				pPline->close();
			}
			else
				return false;
			pBT->close();
		}
	}
	else
		return false;

	pPline->close();
	return true;
}

//�� ����
void CFE::CreatePoint(AcGePoint3d pos, CString Layer){
	AcDbPoint* pPoint = new AcDbPoint;
	pPoint->setPosition(AcGePoint3d(pos.x, pos.y, 0));
	pPoint->setLayer((LPCTSTR)Layer);

	AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
	acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	pPoint->setDatabaseDefaults();
	blkRec->appendAcDbEntity(pPoint);
	pPoint->close();
	blkRec->close();
}

//üũ�ڽ�
void CFE::OnBnClickedBtnConnect(){
	b_Chkpoint = IsDlgButtonChecked(IDC_CHECKFESPOINT);
	if(b_Chkpoint == 0){
		m_Chkpoint_btn.SetCheck(FALSE);
		m_CheckPoint_Str = _T("FALSE");
	}
	else{
		m_Chkpoint_btn.SetCheck(TRUE);
		m_CheckPoint_Str = _T("TRUE");
	}
	iniFile.writeINI(_T("config.ini"), _T("FE"), _T("ChkPoint"), m_CheckPoint_Str);
}

void CFE::OnBnClickedCheckfehorizontal(){
	b_Horizontal = IsDlgButtonChecked(IDC_CHECKFEHORIZONTAL);
	if(b_Horizontal == 0){
		m_Horizontal_btn.SetCheck(FALSE);
		m_Horizontal_Str = _T("FALSE");
		m_HorizontalSTpt_btn.EnableWindow(FALSE);
		m_Leader_btn.EnableWindow(FALSE);
	}
	else{
		m_Horizontal_btn.SetCheck(TRUE);
		m_Horizontal_Str = _T("TRUE");
		m_HorizontalSTpt_btn.EnableWindow(TRUE);
		m_Leader_btn.EnableWindow(TRUE);
	}
	iniFile.writeINI(_T("config.ini"), _T("FE"), _T("ChkHorizontal"), m_Horizontal_Str);
}

void CFE::OnBnClickedCheckfehpoint(){
	b_HorizontalSTpt = IsDlgButtonChecked(IDC_CHECKFEHPOINT);
	if(b_HorizontalSTpt == 0){
		m_HorizontalSTpt_btn.SetCheck(FALSE);
		m_HorizontalSTpt_Str = _T("FALSE");
	}
	else{
		m_HorizontalSTpt_btn.SetCheck(TRUE);
		m_HorizontalSTpt_Str = _T("TRUE");
	}
	iniFile.writeINI(_T("config.ini"), _T("FE"), _T("ChkHpoint"), m_HorizontalSTpt_Str);
}

void CFE::OnBnClickedCheckfeleader(){
	b_Leader = IsDlgButtonChecked(IDC_CHECKFELEADER);
	if(b_Leader == 0){
		m_Leader_btn.SetCheck(FALSE);
		m_Leader_Str = _T("FALSE");
		GetDlgItem(IDC_RADIOFEUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIOFEDOWN)->EnableWindow(TRUE);
	}else{
		m_Leader_btn.SetCheck(TRUE);
		m_Leader_Str = _T("TRUE");
		GetDlgItem(IDC_RADIOFEUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIOFEDOWN)->EnableWindow(FALSE);
	}
	iniFile.writeINI(_T("config.ini"), _T("FE"), _T("ChkLeader"), m_Leader_Str);
}