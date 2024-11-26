// SLT.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLT.h"
#include "afxdialogex.h"



// CSLT ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSLT, CAcUiDialog)

CSLT::CSLT(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CSLT::IDD, pParent)
	, b_RadBtn(0)
{

}

CSLT::~CSLT()
{
}

void CSLT::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);//���������� ��Ʈ�� ������� ����
	//��� ǥ�� ����
	DDX_Control(pDX, IDC_SLT_BTN1, m_btn1);
	DDX_Control(pDX, IDC_SLT_BTN2, m_btn2);
	DDX_Radio(pDX, IDC_SLT_RADPERCENT, b_RadBtn);
	//üũ�ڽ�
	DDX_Control(pDX, IDC_SLT_CHECK1, m_Chk1_btn);
	DDX_Control(pDX, IDC_SLT_CHECK2, m_Chk2_btn);
	DDX_Control(pDX, IDC_SLT_CHECK3, m_Chk3_btn);
	//Static
	DDX_Text(pDX, IDC_SLT_ARROWWIDTH, m_ArrowWidthStr);
	DDX_Text(pDX, IDC_SLT_ARROWHEIGHT, m_ArrowHeightStr);
	DDX_Text(pDX, IDC_SLT_LEADER, m_LeaderStr);
	//�޺��ڽ�, ��ư
	DDX_Control(pDX, IDC_SLT_CBXTEXTTYPE, m_cbxTextType);
	DDX_Control(pDX, IDC_SLT_BSELECT1, m_bSelect1);
	//Static
	DDX_Text(pDX, IDC_SLT_EDTHEIGHT, m_HeightStr);
	DDX_Text(pDX, IDC_SLT_EDTDOT, m_DotStr);
	//�޺��ڽ�
	DDX_Control(pDX, IDC_SLT_CBXLAYER, m_cbxLayer);
	//��ư
	DDX_Control(pDX, IDC_SLT_BTN3, m_btn3);
	DDX_Control(pDX, IDC_SLT_BSELECT2, m_bSelect2);
	DDX_Control(pDX, IDC_SLT_PRINCOLOR, m_btn_Color);
	//Static
	DDX_Text(pDX, IDC_SLT_EDTHORSCALE, m_HScaleStr);
	DDX_Text(pDX, IDC_SLT_EDTVERSCALE, m_VScaleStr);
	DDX_Control(pDX, IDC_SLT_BSELECT3, m_bSelect3);
	//üũ�ڽ�
	DDX_Control(pDX, IDC_SLT_CHECK4, m_Chk4_btn);
	//Static
	DDX_Text(pDX, IDC_SLT_EDTDISTDOT, m_DistDotStr);
	DDX_Text(pDX, IDC_SLT_EDTHEIGHTDOT, m_HeightDotStr);

	DDX_Control(pDX, IDC_SLT_WRITE, m_Write_btn);

}


BEGIN_MESSAGE_MAP(CSLT, CAcUiDialog)
	//ON_BN_CLICKED(IDOK, &CSLT::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSLT::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SLT_RADPERCENT, &CSLT::OnBnClickedSltRadpercent)
	ON_BN_CLICKED(IDC_SLT_RADRATIO, &CSLT::OnBnClickedSltRadratio)
	ON_BN_CLICKED(IDC_SLT_BTN1, &CSLT::OnBnClickedSltBtn1)
	ON_BN_CLICKED(IDC_SLT_BTN2, &CSLT::OnBnClickedSltBtn2)
	ON_BN_CLICKED(IDC_SLT_CHECK1, &CSLT::OnBnClickedSltCheck1)
	ON_BN_CLICKED(IDC_SLT_CHECK2, &CSLT::OnBnClickedSltCheck2)
	ON_BN_CLICKED(IDC_SLT_CHECK3, &CSLT::OnBnClickedSltCheck3)
	ON_EN_UPDATE(IDC_SLT_ARROWWIDTH, &CSLT::OnEnUpdateSltArrowwidth)
	ON_EN_UPDATE(IDC_SLT_ARROWHEIGHT, &CSLT::OnEnUpdateSltArrowheight)
	ON_EN_UPDATE(IDC_SLT_LEADER, &CSLT::OnEnUpdateSltLeader)
	ON_CBN_SELCHANGE(IDC_SLT_CBXTEXTTYPE, &CSLT::OnCbnSelchangeSltCbxtexttype)
	ON_BN_CLICKED(IDC_SLT_BSELECT1, &CSLT::OnBnClickedSltBselect1)
	ON_EN_UPDATE(IDC_SLT_EDTHEIGHT, &CSLT::OnEnUpdateSltEdtheight)
	ON_EN_UPDATE(IDC_SLT_EDTDOT, &CSLT::OnEnUpdateSltEdtdot)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SLT_SPIN1, &CSLT::OnDeltaposSltSpin1)
	ON_CBN_SELCHANGE(IDC_SLT_CBXLAYER, &CSLT::OnCbnSelchangeSltCbxlayer)
	ON_BN_CLICKED(IDC_SLT_BTN3, &CSLT::OnBnClickedSltBtn3)
	ON_BN_CLICKED(IDC_SLT_BSELECT2, &CSLT::OnBnClickedSltBselect2)
	ON_WM_PAINT()
	ON_EN_UPDATE(IDC_SLT_EDTHORSCALE, &CSLT::OnEnUpdateSltEdthorscale)
	ON_EN_UPDATE(IDC_SLT_EDTVERSCALE, &CSLT::OnEnUpdateSltEdtverscale)
	ON_BN_CLICKED(IDC_SLT_BSELECT3, &CSLT::OnBnClickedSltBselect3)
	ON_COMMAND(ID_H_SLT1, &CSLT::OnHSlt1)
	ON_COMMAND(ID_H_SLT2, &CSLT::OnHSlt2)
	ON_COMMAND(ID_H_SLT3, &CSLT::OnHSlt3)
	ON_COMMAND(ID_H_SLT4, &CSLT::OnHSlt4)
	ON_COMMAND(ID_H_SLT5, &CSLT::OnHSlt5)
	ON_COMMAND(ID_H_SLT6, &CSLT::OnHSlt6)
	ON_COMMAND(ID_H_SLT7, &CSLT::OnHSlt7)
	ON_BN_CLICKED(IDC_SLT_CHECK4, &CSLT::OnBnClickedSltCheck4)
	ON_EN_UPDATE(IDC_SLT_EDTDISTDOT, &CSLT::OnEnUpdateSltEdtdistdot)
	ON_EN_UPDATE(IDC_SLT_EDTHEIGHTDOT, &CSLT::OnEnUpdateSltEdtheightdot)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SLT_SPIN2, &CSLT::OnDeltaposSltSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SLT_SPIN3, &CSLT::OnDeltaposSltSpin3)
	ON_BN_CLICKED(IDC_SLT_BTN4, &CSLT::OnBnClickedSltBtn4)
	ON_BN_CLICKED(IDC_SLT_BTN5, &CSLT::OnBnClickedSltBtn5)
	ON_BN_CLICKED(IDC_SLT_WRITE, &CSLT::OnBnClickedSltWrite)
	
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSLT �޽��� ó�����Դϴ�.


BOOL CSLT::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	//��� ǥ�� ���� ��Ʈ��
	m_btn1.LoadBitmaps(IDB_SLT1,IDB_SLT1,NULL,NULL);
	m_btn1.SizeToContent();
	m_btn2.LoadBitmaps(IDB_SLT2,IDB_SLT2,NULL,NULL);
	m_btn2.SizeToContent();
	//������
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	//�۲� ���� ��ô ��ư
	m_bSelect1.AutoLoad();
	m_bSelect2.AutoLoad();
	m_bSelect3.AutoLoad();

	//�۲� �ҷ�����;
	AcDbTextStyleTable *pTextTbl=NULL;
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

	//���̾� �ҷ�����
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
	}
	delete pLayerTblIter;
	delete pLayerTbl;

	
	//��� ǥ�� ����
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("JOBMODE"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		OnBnClickedSltRadpercent();
	else
		OnBnClickedSltRadratio();
	//üũ�ڽ� �տ�"S=" ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Check1"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_Chk1_btn.SetCheck(FALSE);
		bool_Chk1 = false;
		OnBnClickedSltCheck1();
	}else{
		m_Chk1_btn.SetCheck(TRUE);
		bool_Chk1 = true;
		OnBnClickedSltCheck1();
	}
	//üũ�ڽ� ȭ��ǥ �׸���
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Check2"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_Chk2_btn.SetCheck(FALSE);
		bool_Chk2 = false;
		OnBnClickedSltCheck2();
	}else{
		m_Chk2_btn.SetCheck(TRUE);
		bool_Chk2 = true;
		OnBnClickedSltCheck2();
	}
	//üũ�ڽ� +/-ǥ��
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Check3"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_Chk3_btn.SetCheck(FALSE);
		bool_Chk3 = false;
		OnBnClickedSltCheck3();
	}else{
		m_Chk3_btn.SetCheck(TRUE);
		bool_Chk3 = true;
		OnBnClickedSltCheck3();
	}
	//ȭ��ǥ ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("ArrowWidth"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_SLT_ARROWWIDTH, _T("1.000"));
	else
		SetDlgItemText(IDC_SLT_ARROWWIDTH, tempStr);
	//ȭ��ǥ ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("ArrowHeight"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_SLT_ARROWHEIGHT, _T("0.250"));
	else
		SetDlgItemText(IDC_SLT_ARROWHEIGHT, tempStr);
	//���ü� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Leader"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_SLT_LEADER, _T("1.000"));
	else
		SetDlgItemText(IDC_SLT_LEADER, tempStr);
	//�۲� ���ڿ��� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("cmbTextType"));
	if(tempStr.CompareNoCase(_T("")) == 0){
		CmbTextType = _T("0");
	}else{
		CmbTextType = tempStr;
	}
	//�޺��ڽ� �۲�
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("cbxTextType"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_cbxTextType.SetCurSel(0);
		m_cbxTextType.GetLBText(0, CmbTextType);
	}else{
		cbxTextInt_Str = tempStr;
		cbxTextInt = _ttoi(tempStr);
		m_cbxTextType.SetCurSel(cbxTextInt);
		if(m_cbxTextType.FindString(-1, CmbTextType) == -1){
			m_cbxTextType.SetCurSel(0);
			m_cbxTextType.GetLBText(0,CmbTextType);
		}else{
			m_cbxTextType.SetCurSel(cbxTextInt);
			m_cbxTextType.GetLBText(cbxTextInt,CmbTextType);
		}
	}
	//���� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Height"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_SLT_EDTHEIGHT, _T("3.000"));
	else
		SetDlgItemText(IDC_SLT_EDTHEIGHT, tempStr);
	//���� �Ҽ���
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Dot"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_SLT_EDTDOT, _T("3.000"));
	else
		SetDlgItemText(IDC_SLT_EDTDOT, tempStr);
	//���̾� ���ڿ��� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("CmbLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		CmbLayer = _T("0");
	}else{
		CmbLayer = tempStr;
	}
	//�޺��ڽ� ���̾�
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("cbxLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_cbxLayer.SetCurSel(0);
		m_cbxLayer.GetLBText(0, CmbLayer);
	}else{
		cbxLayerInt_Str = tempStr;
		cbxLayerInt = _ttoi(cbxLayerInt_Str);
		m_cbxLayer.SetCurSel(cbxLayerInt);
		if(m_cbxLayer.FindString(-1, CmbLayer) == -1){
			m_cbxLayer.SetCurSel(0);
			m_cbxLayer.GetLBText(0, CmbLayer);
		}else{
			m_cbxLayer.SetCurSel(cbxLayerInt);
			m_cbxLayer.GetLBText(cbxLayerInt,CmbLayer);
		}
	}	
	//��ư ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Color"));
	if(tempStr.CompareNoCase(_T("")) == 0){
		m_Color = 255;
		//ColorSetting(m_Color);
	}else{
		m_Color = _ttoi(tempStr);
		ColorSetting(m_Color);
	}	
	//H��ô
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("HScale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_SLT_EDTHORSCALE, _T("1000"));
	else
		SetDlgItemText(IDC_SLT_EDTHORSCALE, tempStr);
	//V��ô
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("VScale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_SLT_EDTVERSCALE, _T("1000"));
	else
		SetDlgItemText(IDC_SLT_EDTVERSCALE, tempStr);
	//üũ�ڽ� �Ÿ�/���� ���� �Ҽ��� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Check4"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_Chk4_btn.SetCheck(FALSE);
		bool_Chk4 = false;
		OnBnClickedSltCheck4();
	}else{
		m_Chk4_btn.SetCheck(TRUE);
		bool_Chk4 = true;
		OnBnClickedSltCheck4();
	}
	//�Ÿ��� ����� �Ҽ���
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("DistDot"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_SLT_EDTDISTDOT, _T("2"));
	else
		SetDlgItemText(IDC_SLT_EDTDISTDOT, tempStr);
	//���̸� ����� �Ҽ���
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("HeightDot"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_SLT_EDTHEIGHTDOT, _T("2"));
	else
		SetDlgItemText(IDC_SLT_EDTHEIGHTDOT, tempStr);
	

	return TRUE;  
}
void CSLT::MainDraw(){
		CIniFile iniFile;
		AcGePoint3d spt, ept, Midpt, Txtpt;
		CString F_RatioResult, F_PerResult, XresultStr, YresultStr, AddPercent,Re_RatioResult, F_DimDot;
		double Xresult = 0, Yresult = 0;
		CString PerResultStr, RatioResultStr;
		double RatioResult,PerResult, TextDist=0, TextHeight=0;
		AcGePoint3d MinPt,MaxPt;
		//��� ǥ�� ����
		CString tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("JOBMODE"));
		if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
			JobMode = 1;
		else
			JobMode = 2;
		//üũ�ڽ� �տ�"S=" ����
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Check1"));
		if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
			bool_Chk1 = false;
		}else{
			bool_Chk1 = true;	
		}
		//üũ�ڽ� ȭ��ǥ �׸���
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Check2"));
		if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
			bool_Chk2 = false;
		}else{
			bool_Chk2 = true;	
		}
		//üũ�ڽ� +/-ǥ��
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Check3"));
		if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
			bool_Chk3 = false;
		}else{
			bool_Chk3 = true;	
		}
		//ȭ��ǥ ����
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("ArrowWidth"));
		if(tempStr.CompareNoCase(_T("")) == 0)
			m_ArrowWidth = 1.000;
		else
			m_ArrowWidth = _tstof(tempStr);
		//ȭ��ǥ ����
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("ArrowHeight"));
		if(tempStr.CompareNoCase(_T("")) == 0)
			m_ArrowHeight = 0.25;
		else
			m_ArrowHeight = _tstof(tempStr);
		//���ü� ����
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Leader"));
		if(tempStr.CompareNoCase(_T("")) == 0)
			m_Leader = 1.000;
		else
			m_Leader = _tstof(tempStr);
		//�޺��ڽ� �۲�
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("cbxTextType"));
		if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0)
			cbxTextInt = 0;
		else
			cbxTextInt = _ttoi(tempStr);
		//�޺��ڽ� �۲� ���ڿ�
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("CmbTextType"));
		if(tempStr.CompareNoCase(_T("")) == 0)
			CmbTextType = _T("0");
		else
			CmbTextType = tempStr;
		//���� ����
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Height"));
		if(tempStr.CompareNoCase(_T("")) == 0)
			m_Height = 3.000;
		else
			m_Height = _tstof(tempStr);
		//���� �Ҽ���
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Dot"));
		if(tempStr.CompareNoCase(_T("")) == 0)
			m_Dot = 3.000;
		else
			m_Dot = _ttoi(tempStr);
		//�޺��ڽ� ���̾�
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("cbxLayer"));
		if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0)
			cbxLayerInt = 0;
		else{
			cbxLayerInt = _ttoi(tempStr);
		}
		//�޺��ڽ� ���̾� ���ڿ�
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("cmbLayer"));
		if(tempStr.CompareNoCase(_T("")) == 0)
			CmbLayer = _T("0");
		else{
			CmbLayer = tempStr;
		}
		//��ư ����
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Color"));
		if(tempStr.CompareNoCase(_T("")) == 0){
			m_Color = 255;
			//ColorSetting(m_Color);
		}else{
			m_Color = _ttoi(tempStr);
			//SLT.ColorSetting(m_Color);
		}	
		//H��ô
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("HScale"));
		if(tempStr.CompareNoCase(_T("")) == 0)
			m_HScale = 1000;
		else
			m_HScale = _tstof(tempStr);
		//V��ô
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("VScale"));
		if(tempStr.CompareNoCase(_T("")) == 0)
			m_VScale = 1000;
		else
			m_VScale = _tstof(tempStr);
		//üũ�ڽ� �Ÿ�/���� ���� �Ҽ��� ����
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("Check4"));
		if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
			bool_Chk4 = false;
		}else{
			bool_Chk4 = true;
		}
		//�Ÿ��� ����� �Ҽ���
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("DistDot"));
		if(tempStr.CompareNoCase(_T("")) == 0)
			m_DistDot = 2;
		else
			m_DistDot = _tstof(tempStr);
		//���̸� ����� �Ҽ���
		tempStr = iniFile.readINI(_T("config.ini"), _T("SLT"), _T("HeightDot"));
		if(tempStr.CompareNoCase(_T("")) == 0)
			m_HeightDot = 2;
		else
			m_HeightDot = _tstof(tempStr);
		
		::SetFocus(adsw_acadDocWnd());
		if(acedGetPoint(0, _T("\nù��° �� ���� : "), asDblArray(spt)) !=RTNORM){
			return;
		}
		if(acedGetPoint(asDblArray(spt), _T("\n�ι�° �� ���� : "), asDblArray(ept)) !=RTNORM){
			return;
		}

		//�߾��� ã��
		
		acutPolar(asDblArray(spt),ads_angle(asDblArray(spt),asDblArray(ept)),ads_distance(asDblArray(spt),asDblArray(ept))/2, asDblArray(Midpt));
		//m_cbxLayer.GetLBText(m_cbxLayer.GetCurSel(), cbxLayerInt_Str);//�޺��ڽ� ���� �ؽ�Ʈ��������
		double m_Ang = ads_angle(asDblArray(spt), asDblArray(ept));//����
		double RTD_Ang = (m_Ang * 180.0 / PI), TextDistance;
		ACHAR* DimDec;
		struct resbuf rb;
		int rt;
		rt = acedGetVar(_T("DIMDEC"),&rb);

		if(JobMode == 1){//0%�����϶�
			if(bool_Chk3 == true){// +/-ǥ��
				if(bool_Chk4 == true){//�Ÿ�/���� ���� �Ҽ��� ���� üũ��
					//PerResult = (spt.y - ept.y) * (m_VScale/1000) / (spt.x - ept.x) * (m_HScale/1000) * 100;//�ۼ�Ʈ��
					//PerResultStr.Format(_T("%%.%df"),m_HeightDot);
					//YresultStr.Format(PerResultStr, spt.y - ept.y); Yresult = _tstof(YresultStr);
					//PerResultStr.Format(_T("%%.%df"),m_DistDot);
					//XresultStr.Format(PerResultStr, spt.x - ept.x); Xresult = _tstof(XresultStr);
					YresultStr.Format(_T("%f"), spt.y - ept.y); Yresult = _tstof(YresultStr);
					XresultStr.Format(_T("%f"), spt.x - ept.x); Xresult = _tstof(XresultStr);
					if(RTD_Ang >= 0 && RTD_Ang <=180)
						PerResult = abs(Yresult * (m_VScale/1000) / Xresult * (m_HScale/1000)) * 100;
					else
						PerResult = abs(Yresult * (m_VScale/1000) / Xresult * (m_HScale/1000)) * -100;
					
				}else{
					//PerResultStr.Format(_T("%%.%df"),m_Dot);
					//YresultStr.Format(PerResultStr, spt.y - ept.y); Yresult = _tstof(YresultStr);
					//PerResultStr.Format(_T("%%.%df"),m_Dot);
					//XresultStr.Format(PerResultStr, spt.x - ept.x); Xresult = _tstof(XresultStr);
					YresultStr.Format(_T("%f"), spt.y - ept.y); Yresult = _tstof(YresultStr);
					XresultStr.Format(_T("%f"), spt.x - ept.x); Xresult = _tstof(XresultStr);
					if(RTD_Ang >= 0 && RTD_Ang <=180)
						PerResult = abs(Yresult * (m_VScale/1000) / Xresult * (m_HScale/1000)) * 100;
					else
						PerResult = abs(Yresult * (m_VScale/1000) / Xresult * (m_HScale/1000)) * -100;
				}
			}else{// +/-vǥ�� �ƴҽ� ���밪���� ���
				if(bool_Chk4 == true){//�Ÿ�/���� ���� �Ҽ��� ���� üũ��
					//PerResult = (spt.y - ept.y) * (m_VScale/1000) / (spt.x - ept.x) * (m_HScale/1000) * 100;//�ۼ�Ʈ��
					//PerResultStr.Format(_T("%%.%df"),m_HeightDot);
					//YresultStr.Format(PerResultStr, spt.y - ept.y); Yresult = _tstof(YresultStr);
					//PerResultStr.Format(_T("%%.%df"),m_DistDot);
					//XresultStr.Format(PerResultStr, spt.x - ept.x); Xresult = _tstof(XresultStr);
					YresultStr.Format(_T("%f"), spt.y - ept.y); Yresult = _tstof(YresultStr);
					XresultStr.Format(_T("%f"), spt.x - ept.x); Xresult = _tstof(XresultStr);
					PerResult = (abs(Yresult) * (m_VScale/1000) / abs(Xresult) * (m_HScale/1000)) * 100;
				}else{
					//PerResultStr.Format(_T("%%.%df"),m_Dot);
					//YresultStr.Format(PerResultStr, spt.y - ept.y); Yresult = _tstof(YresultStr);
					//PerResultStr.Format(_T("%%.%df"),m_Dot);
					//XresultStr.Format(PerResultStr, spt.x - ept.x); Xresult = _tstof(XresultStr);
					YresultStr.Format(_T("%f"), spt.y - ept.y); Yresult = _tstof(YresultStr);
					XresultStr.Format(_T("%f"), spt.x - ept.x); Xresult = _tstof(XresultStr);
					PerResult = (abs(Yresult) * (m_VScale/1000) / abs(Xresult) * (m_HScale/1000)) * 100;
				}
			}
			if(bool_Chk1 == true){//�տ� S=���� üũ��
				F_PerResult.Format(_T("S=%%.%df"),m_Dot);
				AddPercent.Format(F_PerResult, PerResult);
				PerResultStr = AddPercent + _T("%");
				
			}else{
				F_PerResult.Format(_T("%%.%df"),m_Dot);
				AddPercent.Format(F_PerResult, PerResult);
				PerResultStr = AddPercent + _T("%");
			}
			if(bool_Chk2 == true){//ȭ��ǥ�׸��� üũ��
				AcGePoint3d Arrowpt1, Arrowpt2, H_Leaderpt, LeaderSpt, LeaderEpt, re_LeaderSpt, re_LeaderEpt;
				AcGePoint3dArray ptArray;
				
				if(abs(RTD_Ang - 90.0) < 0.00001 || abs(RTD_Ang - 270.0) < 0.00001){
					return;
				}else if(RTD_Ang >= 0 && RTD_Ang <= 89.0 || RTD_Ang > 270.0 && RTD_Ang < 360.0){
					acutPolar(asDblArray(Midpt), m_Ang + DTR(90.0), m_Height, asDblArray(Txtpt));//�ؽ�Ʈ �߽���
					TextDistance = CreateText(Txtpt, m_Height, m_Height, m_Ang, PerResultStr,CmbLayer, CmbTextType, m_Color, 1);//�ؽ�Ʈ ����

					acutPolar(asDblArray(Midpt), m_Ang + DTR(90.0), m_Height * 0.5, asDblArray(H_Leaderpt));//������ �߽���
					acutPolar(asDblArray(H_Leaderpt),m_Ang,(TextDistance*0.5) + (m_Height*m_Leader), asDblArray(LeaderEpt));
					acutPolar(asDblArray(H_Leaderpt), m_Ang + DTR(180.0),(TextDistance*0.5) + (m_Height*m_Leader), asDblArray(LeaderSpt));
					
					acutPolar(asDblArray(LeaderEpt), m_Ang, m_Height * m_ArrowWidth, asDblArray(Arrowpt1));
					acutPolar(asDblArray(LeaderEpt), m_Ang + DTR(90.0), m_Height * m_ArrowHeight, asDblArray(Arrowpt2));
					CreateSolid(LeaderEpt, Arrowpt1, Arrowpt2, CmbLayer, m_Color);//ȭ��ǥ����
					

				}else{//180~270�� ����
					acutPolar(asDblArray(Midpt), m_Ang - DTR(90.0), m_Height, asDblArray(Txtpt));//�ؽ�Ʈ �߽���
					TextDistance = CreateText(Txtpt, m_Height, m_Height, m_Ang - DTR(180.0), PerResultStr, CmbLayer,CmbTextType, m_Color, 1);//�ؽ�Ʈ ����

					acutPolar(asDblArray(Midpt), m_Ang - DTR(90.0),m_Height * 0.5, asDblArray(H_Leaderpt));//������ �߽���
					acutPolar(asDblArray(H_Leaderpt),m_Ang,(TextDistance*0.5) + (m_Height*m_Leader), asDblArray(LeaderEpt));
					acutPolar(asDblArray(H_Leaderpt), m_Ang + DTR(180.0),(TextDistance*0.5) + (m_Height*m_Leader), asDblArray(LeaderSpt));

					acutPolar(asDblArray(LeaderEpt), m_Ang, m_Height * m_ArrowWidth, asDblArray(Arrowpt1));
					acutPolar(asDblArray(LeaderEpt), m_Ang - DTR(90.0), m_Height * m_ArrowHeight, asDblArray(Arrowpt2));
					CreateSolid(LeaderEpt, Arrowpt1, Arrowpt2, CmbLayer, m_Color);//ȭ��ǥ����
					
				}
				
				
				ptArray.append(LeaderEpt); ptArray.append(LeaderSpt);
				CreatePLine(ptArray, 0, _T("BYLAYER"), 1, CmbLayer, m_Color);//������ ����
			}else{
				if(abs(RTD_Ang - 90.0) < 0.00001 || abs(RTD_Ang - 270.0) < 0.00001)
					return;
				else if(RTD_Ang >= 0 && RTD_Ang <= 89.0 || RTD_Ang > 270.0 && RTD_Ang < 360.0){
					acutPolar(asDblArray(Midpt), m_Ang + (90.0 / 180.0 * PI), 0.5 * m_Height, asDblArray(Txtpt));
					CreateText(Txtpt, m_Height, m_Height,m_Ang, PerResultStr, CmbLayer, CmbTextType, m_Color, 1);//�ؽ�Ʈ
				}else{
					acutPolar(asDblArray(Midpt), m_Ang - (90.0 / 180.0 * PI), 0.5 * m_Height, asDblArray(Txtpt));
					CreateText(Txtpt, m_Height, m_Height,m_Ang - (180.0 / 180.0 * PI), PerResultStr, CmbLayer, CmbTextType, m_Color, 1);//�ؽ�Ʈ
				}
				
				
			}
		}else if(JobMode == 2){// 1:S�����ϋ�
			//RatioResult = abs(spt.x - ept.x) * (m_HScale/1000) / abs(spt.y - ept.y) * (m_VScale/1000); //������
			AcGePoint3d Txtpt;
			F_RatioResult.Format(_T("%%.%df"), m_Dot);
			F_DimDot.Format(_T("%%.%df"), rt);
			XresultStr.Format(F_DimDot, abs(spt.x - ept.x)); Xresult = _tstof(XresultStr);
			YresultStr.Format(F_DimDot, abs(spt.y - ept.y)); Yresult = _tstof(YresultStr);

			if(abs(RTD_Ang - 90.0) < 0.00001 || abs(RTD_Ang - 270.0) < 0.00001){
				RatioResultStr = _T("0 : 1");
				if(abs(RTD_Ang - 90.0) < 0.00001){
					acutPolar(asDblArray(Midpt), ads_angle(asDblArray(spt), asDblArray(ept)) + (90.0 / 180.0 * PI), 1 * m_Height, asDblArray(Txtpt));//�ؽ�Ʈ������
					CreateText(Txtpt, m_Height, m_Height, m_Ang, RatioResultStr, CmbLayer,CmbTextType, m_Color, 1);
				}else{
					acutPolar(asDblArray(Midpt), ads_angle(asDblArray(spt), asDblArray(ept)) - (90.0 / 180.0 * PI), 1 * m_Height, asDblArray(Txtpt));//�ؽ�Ʈ������
					CreateText(Txtpt, m_Height, m_Height, m_Ang + (90.0 / 180.0 * PI), RatioResultStr, CmbLayer, CmbTextType, m_Color, 1);
				}
			}else if(abs(RTD_Ang - 0.0) < 0.00001 || abs(RTD_Ang - 180.0) < 0.00001){
				RatioResultStr = _T("1 : 0");
				if(abs(RTD_Ang - 0.0) < 0.00001){
					acutPolar(asDblArray(Midpt), ads_angle(asDblArray(spt), asDblArray(ept)) + (90.0 / 180.0 * PI), 1 * m_Height, asDblArray(Txtpt));//�ؽ�Ʈ������
					CreateText(Txtpt, m_Height, m_Height, m_Ang, RatioResultStr, CmbLayer, CmbTextType, m_Color, 1);
				}else{
					acutPolar(asDblArray(Midpt), ads_angle(asDblArray(spt), asDblArray(ept)) - (90.0 / 180.0 * PI), 1 * m_Height, asDblArray(Txtpt));//�ؽ�Ʈ������
					CreateText(Txtpt, m_Height, m_Height, m_Ang + (180.0 / 180.0 * PI), RatioResultStr, CmbLayer, CmbTextType, m_Color, 1);
				}
			}else if(RTD_Ang > 0 && RTD_Ang <= 89.0 || RTD_Ang > 270.0 && RTD_Ang < 360.0){
				acutPolar(asDblArray(Midpt), ads_angle(asDblArray(spt), asDblArray(ept)) + (90.0 / 180.0 * PI), 1 * m_Height, asDblArray(Txtpt));//�ؽ�Ʈ������
				RatioResult = (Xresult * (m_HScale/1000)) / (Yresult * (m_VScale/1000));
				Re_RatioResult.Format(F_RatioResult, RatioResult);
				RatioResultStr.Format(_T("1 : %s"),Re_RatioResult);
				CreateText(Txtpt, m_Height, m_Height, m_Ang, RatioResultStr, CmbLayer, CmbTextType, m_Color, 1);
			}else{
				acutPolar(asDblArray(Midpt), ads_angle(asDblArray(spt), asDblArray(ept)) - (90.0 / 180.0 * PI), 1 * m_Height, asDblArray(Txtpt));//�ؽ�Ʈ������
				RatioResult = (Xresult * (m_HScale/1000)) / (Yresult * (m_VScale/1000));
				Re_RatioResult.Format(F_RatioResult, RatioResult);
				RatioResultStr.Format(_T("1 : %s"),Re_RatioResult);
				CreateText(Txtpt, m_Height, m_Height, m_Ang + (180.0 / 180.0 * PI), RatioResultStr, CmbLayer, CmbTextType, m_Color, 1);
			}
			
			
		}
}
void CSLT::OnBnClickedSltWrite()
{
	
	UpdateData(TRUE);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("JobMode"), JobMode_Str); 
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Check1"), Chk1_Str); 
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Check2"), Chk2_Str); 
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Check3"), Chk3_Str); 
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Check4"), Chk4_Str); 
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("ArrowWidth"), m_ArrowWidthStr); 
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("ArrowHeight"), m_ArrowHeightStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Leader"), m_LeaderStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Height"), m_HeightStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Dot"), m_DotStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Color"), m_ColorStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("HScale"), m_HScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("VScale"), m_VScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("HeightDot"), m_HeightDotStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("DistDot"), m_DistDotStr);
	cbxLayerInt = m_cbxLayer.GetCurSel();
	cbxLayerInt_Str.Format(_T("%d"), cbxLayerInt);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("cbxLayer"), cbxLayerInt_Str);
	cbxTextInt = m_cbxTextType.GetCurSel();
	cbxTextInt_Str.Format(_T("%d"), cbxTextInt);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("cbxTextType"), cbxTextInt_Str);

	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("CmbLayer"), CmbLayer);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("CmbTextType"), CmbTextType);

	acDocManager->sendStringToExecute(acDocManager->curDocument(), _T("(SlopeLineText)\n"), true, false, false);

	//CompleteEditorCommand();
	UpdateData(FALSE);
}
void CSLT::OnBnClickedCancel(){
	DestroyWindow();
}
//���� ��ư ���ǥ�ü��� 0%
void CSLT::OnBnClickedSltRadpercent()
{
	UpdateData(TRUE);
	JobMode = 1;
	JobMode_Str.Format(_T("%d"), JobMode);
	b_RadBtn = 0;
	GetDlgItem(IDC_SLT_CHECK1)->EnableWindow(TRUE);GetDlgItem(IDC_SLT_CHECK2)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLT_CHECK3)->EnableWindow(TRUE);GetDlgItem(IDC_SLT_CHECK4)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLT_STATIC13)->EnableWindow(TRUE);GetDlgItem(IDC_SLT_STATIC14)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLT_EDTDISTDOT)->EnableWindow(TRUE);GetDlgItem(IDC_SLT_EDTHEIGHTDOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLT_SPIN2)->EnableWindow(TRUE);GetDlgItem(IDC_SLT_SPIN3)->EnableWindow(TRUE);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("JOBMODE"), JobMode_Str);
	UpdateData(FALSE);
}
//���� ��ư ���ǥ�ü��� 1:S
void CSLT::OnBnClickedSltRadratio()
{
	UpdateData(TRUE);
	JobMode = 2;
	JobMode_Str.Format(_T("%d"), JobMode);
	b_RadBtn = 1;
	GetDlgItem(IDC_SLT_CHECK1)->EnableWindow(FALSE);GetDlgItem(IDC_SLT_CHECK2)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLT_CHECK3)->EnableWindow(FALSE);GetDlgItem(IDC_SLT_CHECK4)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLT_STATIC13)->EnableWindow(FALSE);GetDlgItem(IDC_SLT_STATIC14)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLT_EDTDISTDOT)->EnableWindow(FALSE);GetDlgItem(IDC_SLT_EDTHEIGHTDOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLT_SPIN2)->EnableWindow(FALSE);GetDlgItem(IDC_SLT_SPIN3)->EnableWindow(FALSE);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("JOBMODE"), JobMode_Str);
	UpdateData(FALSE);
}
//��Ʈ�� ���ǥ�ü��� 0%
void CSLT::OnBnClickedSltBtn1()
{
	UpdateData(TRUE);
	//m_btn1.SetCheck(BST_CHECKED);m_btn2.SetCheck(BST_UNCHECKED);
	OnBnClickedSltRadpercent();
	UpdateData(FALSE);
}
//��Ʈ�� ���ǥ�ü��� 1:S
void CSLT::OnBnClickedSltBtn2()
{
	UpdateData(TRUE);
	//m_btn1.SetCheck(BST_UNCHECKED);m_btn2.SetCheck(BST_CHECKED);
	OnBnClickedSltRadratio();
	UpdateData(FALSE);
}
//üũ�ڽ� �տ�"S="����
void CSLT::OnBnClickedSltCheck1()
{
	bool_Chk1 = IsDlgButtonChecked(IDC_SLT_CHECK1);
	if(bool_Chk1 == 0){
		m_Chk1_btn.SetCheck(FALSE);
		Chk1_Str = _T("0");
	}else{
		m_Chk1_btn.SetCheck(TRUE);
		Chk1_Str = _T("1");
	}
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Check1"), Chk1_Str);
}
//üũ�ڽ� ȭ��ǥ �׸���
void CSLT::OnBnClickedSltCheck2()
{
	bool_Chk2 = IsDlgButtonChecked(IDC_SLT_CHECK2);
	if(bool_Chk2 == 1){
		m_Chk2_btn.SetCheck(TRUE);
		Chk2_Str = _T("1");
		GetDlgItem(IDC_SLT_STATIC1)->EnableWindow(TRUE);GetDlgItem(IDC_SLT_STATIC2)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLT_STATIC4)->EnableWindow(TRUE);GetDlgItem(IDC_SLT_STATIC5)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLT_ARROWHEIGHT)->EnableWindow(TRUE);GetDlgItem(IDC_SLT_ARROWWIDTH)->EnableWindow(TRUE);
	}else{
		m_Chk2_btn.SetCheck(FALSE);
		Chk2_Str = _T("0");
		GetDlgItem(IDC_SLT_STATIC1)->EnableWindow(FALSE);GetDlgItem(IDC_SLT_STATIC2)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLT_STATIC4)->EnableWindow(FALSE);GetDlgItem(IDC_SLT_STATIC5)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLT_ARROWHEIGHT)->EnableWindow(FALSE);GetDlgItem(IDC_SLT_ARROWWIDTH)->EnableWindow(FALSE);
	}
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Check2"), Chk2_Str);
}
//üũ�ڽ� +/-ǥ��
void CSLT::OnBnClickedSltCheck3()
{
	bool_Chk3 = IsDlgButtonChecked(IDC_SLT_CHECK3);
	if(bool_Chk3 == 0){
		m_Chk3_btn.SetCheck(FALSE);
		Chk3_Str = _T("0");
	}else{
		m_Chk3_btn.SetCheck(TRUE);
		Chk3_Str = _T("1");
	}
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Check3"), Chk3_Str);
}
//ȭ��ǥ ����
void CSLT::OnEnUpdateSltArrowwidth()
{
	UpdateData(TRUE);
	m_ArrowWidth = _tstof(m_ArrowWidthStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("ArrowWidth"), m_ArrowWidthStr);
	UpdateData(FALSE);
}
//ȭ��ǥ ����
void CSLT::OnEnUpdateSltArrowheight()
{
	UpdateData(TRUE);
	m_ArrowHeight = _tstof(m_ArrowHeightStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("ArrowHeight"), m_ArrowHeightStr);
	UpdateData(FALSE);
}
//���ü� ����
void CSLT::OnEnUpdateSltLeader()
{
	UpdateData(TRUE);
	m_Leader = _tstof(m_LeaderStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Leader"), m_LeaderStr);
	UpdateData(FALSE);
}
//�޺��ڽ� �۲�
void CSLT::OnCbnSelchangeSltCbxtexttype()
{
	UpdateData(TRUE);
	cbxTextInt = m_cbxTextType.GetCurSel();
	if(cbxTextInt == -1)cbxTextInt = 0;
	m_cbxTextType.SetCurSel(cbxTextInt);
	cbxTextInt_Str.Format(_T("%d"), cbxTextInt);
	m_cbxTextType.GetLBText(cbxTextInt, CmbTextType);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("cbxTextType"), cbxTextInt_Str);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("CmbTextType"), CmbTextType);
	UpdateData(FALSE);
}
//�۲� �����ϱ� ��ư
void CSLT::OnBnClickedSltBselect1()
{
	UpdateData(TRUE);
	ads_name en;
	ads_point pt;
	CString strMsg;

	//BeginEditorCommand();
	while(true){
		strMsg.LoadStringW(IDS_SELECT_TEXT); // "\n���� ���� : "
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
		}else if(stat == RTNORM){
			AcDbObjectId objId;
			TCHAR* Name;
			int cnt_AddStr;
			acdbGetObjectId(objId, en);
			 AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			 if(pEnt.openStatus() == Acad::eOk){
			 	if(pEnt->isKindOf(AcDbText::desc())){
			 		AcDbText* pText = AcDbText::cast(pEnt);
					m_Height = pText->height();//�ؽ�Ʈ ����
					
					objId = pText->textStyle();
					AcDbTextStyleTableRecord *pTSTR = NULL;
					acdbOpenObject(pTSTR, objId, AcDb::kForRead);
					pTSTR->getName(Name);
					//m_cbxTextType.AddString(Name);
					pTSTR->close();
			 		pText->close();
					for(int i=0; i < m_cbxTextType.GetCount(); i++){
						if(m_cbxTextType.FindString(i, Name) == -1){//�޺��ڽ��� ������ �۲� �߰�
							m_cbxTextType.AddString(Name);
						}
					}
					cbxTextInt = m_cbxTextType.FindString(0,Name);//������ �۲� index ���ϱ�
					m_cbxTextType.SetCurSel(cbxTextInt);//�޺��ڽ��� �߰��� �۲÷� ����

					CmbTextType.Format(_T("%s"),Name);
					m_HeightStr.Format(_T("%.3f"),m_Height);
					cbxTextInt_Str.Format(_T("%d"),cbxTextInt);
					SetDlgItemText(IDC_SLT_EDTHEIGHT, m_HeightStr);//������ �ؽ�Ʈ ���̰����� ����
					iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Height"), m_HeightStr);
					iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("cbxTextType"), cbxTextInt_Str);
					iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("CmbTextType"), CmbTextType);
					break;
				}else{
					strMsg.LoadStringW(IDS_CHECK_STRING); // "**������ ��ü�� ���ڰ� �ƴմϴ�.**"
					acutPrintf(strMsg);
					continue;
				}
			 }
			 
			pEnt->close();
			acedSSFree(en);	
			
		}
	}
	//CompleteEditorCommand();
	UpdateData(FALSE);
}
//����
void CSLT::OnEnUpdateSltEdtheight()
{
	UpdateData(TRUE);
	m_Height = _tstof(m_HeightStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Height"), m_HeightStr);
	UpdateData(FALSE);
}
//�Ҽ���
void CSLT::OnEnUpdateSltEdtdot()
{
	UpdateData(TRUE);
	m_Dot = _ttoi(m_DotStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Dot"), m_DotStr);
	UpdateData(FALSE);
}
//�Ҽ��� Spin
void CSLT::OnDeltaposSltSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0)
		m_Dot++;
	else
		m_Dot--;
	SetDlgItemInt(IDC_SLT_EDTDOT, m_Dot);
	*pResult = 0;
}
//�޺��ڽ� ���̾�
void CSLT::OnCbnSelchangeSltCbxlayer()
{
	UpdateData(TRUE);
	cbxLayerInt = m_cbxLayer.GetCurSel();
	if(cbxLayerInt == -1)cbxLayerInt = 0;
	m_cbxLayer.SetCurSel(cbxLayerInt);
	cbxLayerInt_Str.Format(_T("%d"), cbxLayerInt);
	m_cbxLayer.GetLBText(cbxLayerInt, CmbLayer);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("cbxLayer"), cbxLayerInt_Str);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("CmbLayer"), CmbLayer);
	UpdateData(FALSE);
}
//���� ��ư
void CSLT::OnBnClickedSltBtn3()
{
	UpdateData(TRUE);
	acedSetColorDialog(m_Color,true,256);
	//m_ColorStr.Format(_T("%d"),m_Color);
	//iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Color"), m_ColorStr);
	ColorSetting(m_Color);
	UpdateData(FALSE);
}
//���̾�,���̾� ���� �����ư
void CSLT::OnBnClickedSltBselect2()
{
	UpdateData(TRUE);
	ads_name en;
	ads_point pt;
	AcCmColor Color;
	CString strMsg;

	//BeginEditorCommand();
	while(true){
		strMsg.LoadStringW(IDS_SELECT_TEXT); // "\n���� ���� : "
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
		}else if(stat == RTNORM){
			int NewIdx;
			AcDbObjectId objId, LayerId;
			AcCmColor Color;
			TCHAR* LayerChar;
			acdbGetObjectId(objId, en);
			 AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			 if(pEnt.openStatus() == Acad::eOk){
				 if(pEnt->isKindOf(AcDbText::desc())){
			 		AcDbText* pText = AcDbText::cast(pEnt);
					LayerId = pText->layerId();
					CmbLayer = pText->layer();

					AcDbLayerTableRecordPointer pLayer(LayerId, AcDb::kForRead);
					Color = pLayer->color();
					m_Color = Color.colorIndex();

			 		pLayer->close();
					pText->close();
					for(int i=0; i < m_cbxLayer.GetCount(); i++){
						if(m_cbxLayer.FindString(i, CmbLayer) == -1){//�޺��ڽ��� ������ ���̾� �߰�
							m_cbxLayer.AddString(CmbLayer);
						}
					}
					cbxLayerInt = m_cbxLayer.FindString(0,CmbLayer);//������ ���̾� index ���ϱ�
					m_cbxLayer.SetCurSel(cbxLayerInt);//�޺��ڽ��� �߰��� ���̾�� ����
					cbxLayerInt_Str.Format(_T("%d"), cbxLayerInt);
					m_ColorStr.Format(_T("%d"),m_Color);
					iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Color"), m_ColorStr);
					iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("cbxLayer"), cbxLayerInt_Str);
					iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("CmbLayer"), CmbLayer);
					ColorSetting(m_Color);
					break;
				}else{
					strMsg.LoadStringW(IDS_CHECK_STRING); // "**������ ��ü�� ���ڰ� �ƴմϴ�.**"
					acutPrintf(strMsg);

					continue;
				}
			 }
			pEnt->close();
			acedSSFree(en);	
			
		}
	}
	
	//CompleteEditorCommand();
	UpdateData(FALSE);
}
//���� ���
void CSLT::ColorSetting(int newColorIndex)
{
	UpdateData(TRUE);
	CString ColorName;
	CRect prect;
	m_btn_Color.GetClientRect (& prect); // get the zone

	//	COLORREF cref = acedGetRGB(nNewColor);

	if(newColorIndex ==256)
		ColorName = _T("By Layer");
	else if(newColorIndex ==0)
		ColorName = _T("By Block");
	else
		ColorName.Format(_T("%d"),newColorIndex);

	unsigned long rgb = acedGetRGB(newColorIndex); 
	int r = GetRValue(rgb);
	int g = GetGValue(rgb);
	int b = GetBValue(rgb);
	FillRect (m_btn_Color.GetDC () -> GetSafeHdc (), & prect, CBrush (RGB (r,g,b))); // fill color 

	((CButton*) GetDlgItem(IDC_SLT_STATIC_COLOR))->SetWindowText(ColorName);
	m_ColorStr.Format(_T("%d"),newColorIndex);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Color"), m_ColorStr);
	UpdateData(FALSE);

}
//��ȭ���� �ű涧 ��»����� �ʱ�ȭ�ǹǷ� OnPaint�� �׸�
void CSLT::OnPaint()
{
	UpdateData(TRUE);
	CPaintDC dc(this); // device context for painting
	m_Color = _ttoi(m_ColorStr);
	ColorSetting(m_Color);
	UpdateData(FALSE);
}

//Horizontal ��ô
void CSLT::OnEnUpdateSltEdthorscale()
{
	m_HScale = _tstof(m_HScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("HScale"), m_HScaleStr);
}
//Vertical ��ô
void CSLT::OnEnUpdateSltEdtverscale()
{
	m_VScale = _tstof(m_VScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("VScale"), m_VScaleStr);
}
//��ư Ŭ�� �˾�â����
void CSLT::OnBnClickedSltBselect3()
{
	CRect rect;
	CMenu mnTemp,*pContextMenu;
	GetDlgItem(IDC_SLT_BSELECT3)->GetWindowRect(&rect);
	mnTemp.LoadMenuW(IDR_MENU1);
	pContextMenu = mnTemp.GetSubMenu(0);
	pContextMenu->TrackPopupMenu(TPM_LEFTBUTTON, rect.left, rect.top+22,this);
	
}
//H=1000,V=1000
void CSLT::OnHSlt1()
{
	UpdateData(TRUE);
	m_HScaleStr = _T("1000");
	OnEnUpdateSltEdthorscale();
	m_VScaleStr = _T("1000");
	OnEnUpdateSltEdtverscale();
	UpdateData(FALSE);
}
//H=1000,V=200
void CSLT::OnHSlt2()
{
	UpdateData(TRUE);
	m_HScaleStr = _T("1000");
	OnEnUpdateSltEdthorscale();
	m_VScaleStr = _T("200");
	OnEnUpdateSltEdtverscale();
	UpdateData(FALSE);
}
//H=1000,V=400
void CSLT::OnHSlt3()
{
	UpdateData(TRUE);
	m_HScaleStr = _T("1000");
	OnEnUpdateSltEdthorscale();
	m_VScaleStr = _T("400");
	OnEnUpdateSltEdtverscale();
	UpdateData(FALSE);
}
//H=1000,V=166.6666667
void CSLT::OnHSlt4()
{
	UpdateData(TRUE);
	m_HScaleStr = _T("1000");
	OnEnUpdateSltEdthorscale();
	m_VScaleStr = _T("166.6666667");
	OnEnUpdateSltEdtverscale();
	UpdateData(FALSE);
}
//H=1200,V=200
void CSLT::OnHSlt5()
{
	UpdateData(TRUE);
	m_HScaleStr = _T("1200");
	OnEnUpdateSltEdthorscale();
	m_VScaleStr = _T("200");
	OnEnUpdateSltEdtverscale();
	UpdateData(FALSE);
}
//H=1200,V=400
void CSLT::OnHSlt6()
{
	UpdateData(TRUE);
	m_HScaleStr = _T("1200");
	OnEnUpdateSltEdthorscale();
	m_VScaleStr = _T("400");
	OnEnUpdateSltEdtverscale();
	UpdateData(FALSE);
}
//H=600,V=200
void CSLT::OnHSlt7()
{
	UpdateData(TRUE);
	m_HScaleStr = _T("600");
	OnEnUpdateSltEdthorscale();
	m_VScaleStr = _T("200");
	OnEnUpdateSltEdtverscale();
	UpdateData(FALSE);
}
//üũ�ڽ� �Ÿ�/���� ���� �Ҽ��� ����
void CSLT::OnBnClickedSltCheck4()
{
	bool_Chk4 = IsDlgButtonChecked(IDC_SLT_CHECK4);
	if(bool_Chk4 == 1){
		m_Chk4_btn.SetCheck(TRUE);
		Chk4_Str= _T("1");
		GetDlgItem(IDC_SLT_STATIC13)->EnableWindow(TRUE);GetDlgItem(IDC_SLT_STATIC14)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLT_EDTDISTDOT)->EnableWindow(TRUE);GetDlgItem(IDC_SLT_EDTHEIGHTDOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLT_SPIN2)->EnableWindow(TRUE);GetDlgItem(IDC_SLT_SPIN3)->EnableWindow(TRUE);
	}else{
		m_Chk4_btn.SetCheck(FALSE);
		Chk4_Str= _T("0");
		GetDlgItem(IDC_SLT_STATIC13)->EnableWindow(FALSE);GetDlgItem(IDC_SLT_STATIC14)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLT_EDTDISTDOT)->EnableWindow(FALSE);GetDlgItem(IDC_SLT_EDTHEIGHTDOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLT_SPIN2)->EnableWindow(FALSE);GetDlgItem(IDC_SLT_SPIN3)->EnableWindow(FALSE);
	}
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("Check4"), Chk4_Str);
}
//�Ÿ� ����� �Ҽ���
void CSLT::OnEnUpdateSltEdtdistdot()
{
	UpdateData(TRUE);
	m_DistDot = _tstof(m_DistDotStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("DistDot"), m_DistDotStr);
	UpdateData(FALSE);
}
//�Ÿ� ����� �Ҽ��� Spin
void CSLT::OnDeltaposSltSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0)
		m_DistDot++;
	else
		m_DistDot--;
	SetDlgItemInt(IDC_SLT_EDTDISTDOT, m_DistDot);
	*pResult = 0;
}
//���� ����� �Ҽ���
void CSLT::OnEnUpdateSltEdtheightdot()
{
	UpdateData(TRUE);
	m_HeightDot = _tstof(m_HeightDotStr);
	iniFile.writeINI(_T("config.ini"), _T("SLT"), _T("HeightDot"), m_HeightDotStr);
	UpdateData(FALSE);
}
//���� ����� �Ҽ��� Spin
void CSLT::OnDeltaposSltSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0)
		m_HeightDot++;
	else
		m_HeightDot--;
	SetDlgItemInt(IDC_SLT_EDTHEIGHTDOT, m_HeightDot);
	*pResult = 0;
}
//�� ����
void CSLT::OnBnClickedSltBtn4()
{
	UpdateData(TRUE);
	AcGePoint3d LinePt, spt, ept;
	ads_name ens;
	CString strMsg;

	while(true){
		strMsg.LoadStringW(IDS_SLOPE_SEARCH_SEL); // "��縦 ��ȸ�� ��, ������ ���� : "
		int stat = acedEntSel(strMsg, ens, asDblArray(LinePt));

		if(stat == RTCAN){
			acedSSFree(ens);
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
					pLine->getStartPoint(spt);
					pLine->getEndPoint(ept);
			 		pLine->close();
				 }else if(pEnt->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline* pPLine = AcDbPolyline::cast(pEnt);
					pPLine->getStartPoint(spt);
					pPLine->getEndPoint(ept);
					pPLine->close();
				 }else{
					strMsg.LoadStringW(IDS_SELECT_CHK_LINE); // "\n**������ ��ü�� ������ �ƴմϴ�.**"
					acutPrintf(strMsg);
					continue;
				}
			 }
			pEnt->close();
			acedSSFree(ens);	
			break;
		}
	}

	CString F_RatioResult, F_PerResult, AddPercent, XresultStr, YresultStr, L_RatioResultStr;
	double Xresult = 0, Yresult = 0;
	double m_Ang = ads_angle(asDblArray(spt), asDblArray(ept));//����
	double RTD_Ang = RTD(m_Ang);
	GetDlgItemText(IDC_SLT_EDTHORSCALE,m_HScaleStr);
	GetDlgItemText(IDC_SLT_EDTVERSCALE,m_VScaleStr);
	m_HScale = _tstof(m_HScaleStr);
	m_VScale = _tstof(m_VScaleStr);

	//�ۼ�Ʈ
	if(bool_Chk4 == true){//�Ÿ�/���� ���� �Ҽ��� ���� üũ��
		//PerResult = (spt.y - ept.y) * (m_VScale/1000) / (spt.x - ept.x) * (m_HScale/1000) * 100;//�ۼ�Ʈ��
		PerResultStr.Format(_T("%%.%df"),m_HeightDot);
		YresultStr.Format(PerResultStr, abs(spt.y - ept.y)); Yresult = _tstof(YresultStr);
		PerResultStr.Format(_T("%%.%df"),m_DistDot);
		XresultStr.Format(PerResultStr, abs(spt.x - ept.x)); Xresult = _tstof(XresultStr);
		if(abs(90.0 - RTD_Ang) < 0.000001 || abs(270.0 - RTD_Ang) < 0.000001)
			PerResult = 0;
		else if(abs(180.0 - RTD_Ang) < 0.000001 || abs(0.0 - RTD_Ang) < 0.000001)
			PerResult = 0;
		else if(RTD_Ang >= 0 && RTD_Ang <=180)
			PerResult = Yresult * (m_VScale/1000) / Xresult * (m_HScale/1000) * 100;
		else
			PerResult = Yresult * (m_VScale/1000) / Xresult * (m_HScale/1000) * 100;
		
	}else{
		PerResultStr.Format(_T("%%.%df"),m_Dot);
		YresultStr.Format(PerResultStr, abs(spt.y - ept.y)); Yresult = _tstof(YresultStr);
		PerResultStr.Format(_T("%%.%df"),m_Dot);
		XresultStr.Format(PerResultStr, abs(spt.x - ept.x)); Xresult = _tstof(XresultStr);
		if(abs(90.0 - RTD_Ang) < 0.000001 || abs(270.0 - RTD_Ang) < 0.000001)
			PerResult = 0;
		else if(abs(180.0 - RTD_Ang) < 0.000001 || abs(0.0 - RTD_Ang) < 0.000001)
			PerResult = 0;
		else if(RTD_Ang >= 0 && RTD_Ang <=180)
			PerResult = Yresult * (m_VScale/1000) / Xresult * (m_HScale/1000) * 100;
		else
			PerResult = Yresult * (m_VScale/1000) / Xresult * (m_HScale/1000) * 100;
	}
	strMsg.LoadStringW(IDS_PERCENT_RESULT); // "�ۼ�Ʈ =  %%.%df"
	F_PerResult.Format(strMsg, m_Dot);
	AddPercent.Format(F_PerResult, PerResult);
	PerResultStr = AddPercent + _T("%");
	
	//����	
	F_RatioResult.Format(_T("%%.%df"), m_Dot);
	XresultStr.Format(F_RatioResult, abs(spt.x - ept.x)); Xresult = _tstof(XresultStr);
	YresultStr.Format(F_RatioResult, abs(spt.y - ept.y)); Yresult = _tstof(YresultStr);

	
	if(abs(90.0 - RTD_Ang) < 0.000001 || abs(270.0 - RTD_Ang) < 0.000001){
		RatioResult = 1;
		RatioResultStr.Format(F_RatioResult, RatioResult);
		L_RatioResultStr.Format(_T(" 1  :  S = 0 : 1"));
	}else if(abs(180.0 - RTD_Ang) < 0.000001 || abs(0.0 - RTD_Ang) < 0.000001){
		RatioResult = 0;
		RatioResultStr.Format(F_RatioResult, RatioResult);
		L_RatioResultStr.Format(_T(" 1  :  S = %s"), RatioResultStr);
	}else{
		RatioResult = Xresult * (m_HScale/1000) / Yresult * (m_VScale/1000);//������
		RatioResultStr.Format(F_RatioResult, RatioResult);
		L_RatioResultStr.Format(_T(" 1  :  S = %s"), RatioResultStr);
	}
		

	

	((CButton*) GetDlgItem(IDC_SLT_STATIC_PERCENT))->SetWindowText(PerResultStr);
	((CButton*) GetDlgItem(IDC_SLT_STATIC_RATIO))->SetWindowText(L_RatioResultStr);
	UpdateData(FALSE);
}
//�� �� ����
void CSLT::OnBnClickedSltBtn5()
{
	UpdateData(TRUE);
	AcGePoint3d spt, ept;
	CString strMsg;
	GetDlgItemText(IDC_SLT_EDTHORSCALE,m_HScaleStr);
	GetDlgItemText(IDC_SLT_EDTVERSCALE,m_VScaleStr);
	m_HScale = _tstof(m_HScaleStr);
	m_VScale = _tstof(m_VScaleStr);

	strMsg.LoadStringW(IDS_FIRST_SEL_CMD); // "\nù ��° �� ���� : "
	if(acedGetPoint(0, strMsg, asDblArray(spt)) != RTNORM)
		return;
	strMsg.LoadStringW(IDS_SECOND_SEL_CMD); // "\n�� ��° �� ���� : "
	if(acedGetPoint(asDblArray(spt), strMsg, asDblArray(ept)) != RTNORM)
		return;

	CString F_RatioResult, F_PerResult, AddPercent, XresultStr, YresultStr, L_RatioResultStr;
	double Xresult = 0, Yresult = 0;
	double m_Ang = ads_angle(asDblArray(spt), asDblArray(ept));//����
	double RTD_Ang = RTD(m_Ang);
	//����	
	F_RatioResult.Format(_T("%%.%df"), m_Dot);
	XresultStr.Format(F_RatioResult, abs(spt.x - ept.x)); Xresult = _tstof(XresultStr);
	YresultStr.Format(F_RatioResult, abs(spt.y - ept.y)); Yresult = _tstof(YresultStr);

	RatioResult = Xresult * (m_HScale/1000) / Yresult * (m_VScale/1000);//������

	if(abs(90.0 - RTD_Ang) < 0.000001 || abs(270.0 - RTD_Ang) < 0.000001){
		RatioResult = 1;
		RatioResultStr.Format(F_RatioResult, RatioResult);
		L_RatioResultStr.Format(_T(" 1  :  S = 0 : 1"));
	}else if(abs(180.0 - RTD_Ang) < 0.000001 || abs(0.0 - RTD_Ang) < 0.000001){
		RatioResult = 0;
		RatioResultStr.Format(F_RatioResult, RatioResult);
		L_RatioResultStr.Format(_T(" 1  :  S = %s"), RatioResultStr);
	}else{
		RatioResult = Xresult * (m_HScale/1000) / Yresult * (m_VScale/1000);//������
		RatioResultStr.Format(F_RatioResult, RatioResult);
		L_RatioResultStr.Format(_T(" 1  :  S = %s"), RatioResultStr);
	}
	
	//�ۼ�Ʈ
	F_PerResult.Format(_T("%%.%df"),m_Dot);
	YresultStr.Format(F_PerResult, abs(spt.y - ept.y)); Yresult = _tstof(YresultStr);
	XresultStr.Format(F_PerResult, abs(spt.x - ept.x)); Xresult = _tstof(XresultStr);

	if(abs(90.0 - RTD_Ang) < 0.000001 || abs(270.0 - RTD_Ang) < 0.000001)
		PerResult = 0;
	else if(abs(180.0 - RTD_Ang) < 0.000001 || abs(0.0 - RTD_Ang) < 0.000001)
		PerResult = 0;
	else if(RTD_Ang >= 0 && RTD_Ang <=180)
		PerResult = Yresult * (m_VScale/1000) / Xresult * (m_HScale/1000) * 100;
	else
		PerResult = Yresult * (m_VScale/1000) / Xresult * (m_HScale/1000) * 100;

	//PerResult = Yresult * (m_VScale/1000) / Xresult * (m_HScale/1000) * 100;
	PerResultStr.Format(F_PerResult, PerResult);

	strMsg.LoadStringW(IDS_PERCENT_DISPLAY); // "�ۼ�Ʈ =  "
	AddPercent = strMsg + PerResultStr + _T("%");
	
	((CButton*) GetDlgItem(IDC_SLT_STATIC_PERCENT))->SetWindowText(AddPercent);
	((CButton*) GetDlgItem(IDC_SLT_STATIC_RATIO))->SetWindowText(L_RatioResultStr);
	UpdateData(FALSE);
}

bool CSLT::CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer, double Color){
	//UpdateData(TRUE);
	Acad::ErrorStatus es;
	AcDbObjectId plineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	//AcDbBlockTableRecord* blkRec = openCurrentSpaceBlock(AcDb::kForWrite, db);
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();

	if(pDb == NULL)return false;

	AcDbPolyline* pPLine = new AcDbPolyline();
	for(int i=0; i<ptArray.length(); i++){
		pPLine->addVertexAt(i, AcGePoint2d(ptArray[i].x, ptArray[i].y));
	}
	pPLine->setConstantWidth(LineWidth);
	pPLine->setLinetype(LineType);
	pPLine->setLinetypeScale(LineTypeScale);
	pPLine->setLayer(Layer);
	pPLine->setColorIndex(Color);
	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(plineObjId, pPLine);
			pBTR->close();
			pPLine->close();
		}else
			return false;
		pBT->close();
	}else
		return false;
	
	pPLine->close();
	//UpdateData(FALSE);
	return true;
}

//�ؽ�Ʈ���� 
double CSLT::CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer,CString Style,double Color, int mode){
	//UpdateData(TRUE);
		Acad::ErrorStatus es;
		AcDbObjectId textStyleId, pTextObjId;
		double XScale = 0,obliquingAngle = 0;
		
		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
		if(pDb == NULL)return false;

		AcDbText* pText = new AcDbText();
		textStyleId = pDb->textstyle();
		pText->setTextStyle(findTextStyleName(Style));
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
			es = pText->setHorizontalMode(AcDb::kTextCenter);
		}else{
			es = pText->setVerticalMode(AcDb::kTextBase);
			es = pText->setHorizontalMode(AcDb::kTextMid);
		}
		pText->setAlignmentPoint(pt);
		pText->setHeight(Height);
		pText->setRotation(Rotate);
		pText->setTextString((LPCTSTR)TxtStr);
		pText->setColorIndex(Color);

		AcDbExtents mExtText;
		pText->getGeomExtents(mExtText);
		TextDist = mExtText.maxPoint().x - mExtText.minPoint().x;//ĳ��� �ؽ�Ʈ ����
		TextHeight = mExtText.maxPoint().y - mExtText.minPoint().y;//ĳ��� �ؽ�Ʈ ����
		MaxPt = mExtText.maxPoint();
		MinPt = mExtText.minPoint();
		//MaxPt.y = MaxPt.y - abs(MaxPt.y - MinPt.y);
		if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
			if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
				es = pBTR->appendAcDbEntity(pTextObjId, pText);
				pBTR->close();
				pText->close();
			}
			pBT->close();

		}else{
			pText->close();
			pBT->close();
		}

		//UpdateData(FALSE);
		return TextDist;
}

//�ָ��� ����
bool CSLT::CreateSolid(AcGePoint3d Solpt1, AcGePoint3d Solpt2, AcGePoint3d Solpt3, CString Layer, double Color){
	//UpdateData(TRUE);
	Acad::ErrorStatus es;
	AcDbObjectId SolidId;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)return false;

	AcDbBlockTable* pBT;
	AcDbBlockTableRecord* pBTR=NULL;

	AcDbSolid* Arrow = new AcDbSolid(Solpt1, Solpt2, Solpt3,Solpt3);
	Arrow->setLayer(Layer);
	Arrow->setColorIndex(Color);

	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(SolidId, Arrow);
			pBTR->close();
			Arrow->close();
		}else{
			pBTR->close();
			Arrow->close();
		}
		pBT->close();
		
	}else{
		pBTR->close();
		Arrow->close();
		pBT->close();
	}
	//UpdateData(FALSE);
	return true;

}
////������ ���� ������ ��ȯ�Ͽ� �����Ѵ�.
double CSLT::DTR(double angle)
{//ads_printf(_T("\n DTR() angle=%f \n"),angle);
	return (angle / 180.0 * PI);
}
////���� ���� ������ ��ȯ�Ͽ� �����Ѵ�.
double CSLT::RTD(double radian)
{//ads_printf(_T("\n RTD() radian=%f \n"),radian);
	return (radian * 180.0 / PI);
}
AcDbObjectId CSLT::findTextStyleName(CString name){
	AcDbTextStyleTable* pTextTbl = NULL;
	AcDbTextStyleTablePointer pTextPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbTextStyleTableIterator* pTextTblIter;
	pTextPoint->newIterator(pTextTblIter);
	
	AcDbObjectId TextId;
	//�ؽ�Ʈ ��Ÿ�� ���̺��� �ҷ��� �Է��� �ؽ�Ʈ ��Ÿ���� ���� ��ġ�ϸ� AcDbObjectId ���� ��ȯ�Ѵ�.
	for(pTextTblIter->start(); !pTextTblIter->done(); pTextTblIter->step()){
		pTextTblIter->getRecordId(TextId);
		AcDbTextStyleTableRecordPointer pTextTblRcd(TextId, AcDb::kForRead);
		
		TCHAR* name2;
		pTextTblRcd->getName(name2);		
		if(wcscmp(name, name2)==0)
			return TextId;
	}
	
	delete pTextTblIter;
	delete pTextTbl;

	return NULL;
}

extern CSLT *pSLT;
void CSLT::PostNcDestroy(){
	delete this;
	pSLT = NULL;
}