// ARI.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ARI.h"
#include "afxdialogex.h"


// CARI ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CARI, CAcUiDialog)

	CARI::CARI(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CARI::IDD, pParent),b_RadBtn_IA(0),b_RadBtn_XY(0)
{

}

CARI::~CARI()
{
}

void CARI::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	//üũ�ڽ�
	DDX_Control(pDX, IDC_ARI_CHECK1, m_ChkXY_btn);
	DDX_Control(pDX, IDC_ARI_CHECK2, m_ChkIA_btn);
	DDX_Control(pDX, IDC_ARI_CHECK3, m_ChkR_btn);
	DDX_Control(pDX, IDC_ARI_CHECK4, m_ChkTL_btn);
	DDX_Control(pDX, IDC_ARI_CHECK5, m_ChkCL_btn);
	DDX_Control(pDX, IDC_ARI_CHECK6, m_ChkSL_btn);
	DDX_Control(pDX, IDC_ARI_NONAME, m_ChkNoname_btn);
	//���� ��ư
	DDX_Radio(pDX, IDC_ARI_RADIO1, b_RadBtn_XY);
	DDX_Radio(pDX, IDC_ARI_RADIO3, b_RadBtn_IA);
	//static
	DDX_Text(pDX, IDC_ARI_DOTXY, m_DotXYStr);
	DDX_Text(pDX, IDC_ARI_DOTIA, m_DotIAStr);
	DDX_Text(pDX, IDC_ARI_DOTR, m_DotRStr);
	DDX_Text(pDX, IDC_ARI_DOTTL, m_DotTLStr);
	DDX_Text(pDX, IDC_ARI_DOTCL, m_DotCLStr);
	DDX_Text(pDX, IDC_ARI_DOTSL, m_DotSLStr);
	DDX_Text(pDX, IDC_ARI_EDTHEIGHT, m_TxtHeightStr);
	//�޺��ڽ�
	DDX_Control(pDX, IDC_ARI_CMBLAYER, m_CbxLayer);
	DDX_Control(pDX, IDC_ARI_CMBSTYLE, m_CbxTextStyle);
	//���� ��ư
	DDX_Control(pDX, IDC_ARI_BSELECT1,m_bSelect1);
	DDX_Control(pDX, IDC_ARI_BSELECT2,m_bSelect2);
	DDX_Control(pDX, IDC_ARI_BSELECT3,m_bSelect3);

}


BEGIN_MESSAGE_MAP(CARI, CAcUiDialog)
	ON_BN_CLICKED(IDC_ARI_CHECK1, &CARI::OnBnClickedAriCheck1)	
	ON_NOTIFY(UDN_DELTAPOS, IDC_ARI_SPINXY, &CARI::OnDeltaposAriSpinxy)
	ON_BN_CLICKED(IDC_ARI_RADIO1, &CARI::OnBnClickedAriRadio1)
	ON_BN_CLICKED(IDC_ARI_RADIO2, &CARI::OnBnClickedAriRadio2)
	ON_BN_CLICKED(IDC_ARI_CHECK2, &CARI::OnBnClickedAriCheck2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ARI_SPINIA, &CARI::OnDeltaposAriSpinia)
	ON_BN_CLICKED(IDC_ARI_RADIO3, &CARI::OnBnClickedAriRadio3)
	ON_BN_CLICKED(IDC_ARI_RADIO4, &CARI::OnBnClickedAriRadio4)
	ON_BN_CLICKED(IDC_ARI_CHECK3, &CARI::OnBnClickedAriCheck3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ARI_SPINR, &CARI::OnDeltaposAriSpinr)
	ON_BN_CLICKED(IDC_ARI_CHECK4, &CARI::OnBnClickedAriCheck4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ARI_SPINTL, &CARI::OnDeltaposAriSpintl)
	ON_BN_CLICKED(IDC_ARI_CHECK5, &CARI::OnBnClickedAriCheck5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ARI_SPINCL, &CARI::OnDeltaposAriSpincl)
	ON_BN_CLICKED(IDC_ARI_CHECK6, &CARI::OnBnClickedAriCheck6)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ARI_SPINSL, &CARI::OnDeltaposAriSpinsl)
	ON_BN_CLICKED(IDC_ARI_NONAME, &CARI::OnBnClickedAriNoname)
	ON_EN_UPDATE(IDC_ARI_EDTHEIGHT, &CARI::OnEnUpdateAriEdtheight)
	ON_BN_CLICKED(IDC_ARI_BSELECT1, &CARI::OnBnClickedAriBselect1)
	ON_CBN_SELCHANGE(IDC_ARI_CMBLAYER, &CARI::OnCbnSelchangeAriCmblayer)
	ON_BN_CLICKED(IDC_ARI_BSELECT2, &CARI::OnBnClickedAriBselect2)
	ON_CBN_SELCHANGE(IDC_ARI_CMBSTYLE, &CARI::OnCbnSelchangeAriCmbstyle)
	ON_BN_CLICKED(IDC_ARI_BSELECT3, &CARI::OnBnClickedAriBselect3)
	ON_BN_CLICKED(IDOK, &CARI::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CARI::OnBnClickedCancel)
	ON_EN_UPDATE(IDC_ARI_DOTXY, &CARI::OnEnUpdateAriDotxy)
	ON_EN_UPDATE(IDC_ARI_DOTIA, &CARI::OnEnUpdateAriDotia)
	ON_EN_UPDATE(IDC_ARI_DOTR, &CARI::OnEnUpdateAriDotr)
	ON_EN_UPDATE(IDC_ARI_DOTTL, &CARI::OnEnUpdateAriDottl)
	ON_EN_UPDATE(IDC_ARI_DOTCL, &CARI::OnEnUpdateAriDotcl)
	ON_EN_UPDATE(IDC_ARI_DOTSL, &CARI::OnEnUpdateAriDotsl)
	ON_BN_CLICKED(ID_ARI_HELP, &CARI::OnBnClickedAriHelp)
END_MESSAGE_MAP()


// CARI �޽��� ó�����Դϴ�.

BOOL CARI::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	//������
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	//�۲� ���̾� ���� ��ư
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
		m_CbxTextStyle.AddString(TextName);
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
		m_CbxLayer.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;

	//üũ�ڽ� XY � �߽� ��ǥ
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("CheckXY"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		bool_ChkXY = false;
		m_ChkXY_btn.SetCheck(FALSE);		
		//OnBnClickedAriCheck1();
	}else{
		bool_ChkXY = true;
		m_ChkXY_btn.SetCheck(TRUE);		
		//OnBnClickedAriCheck1();
	}

	// Update X,Y � �߽� ��ǥ
 	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("DotXY"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_ARI_DOTXY, _T("5"));
	else
		SetDlgItemText(IDC_ARI_DOTXY, tempStr);

	// X,Y � �߽� ��ǥ ��� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("XYMode"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		OnBnClickedAriRadio1();
	else
		OnBnClickedAriRadio2();

	//üũ�ڽ� IA ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("CheckIA"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_ChkIA_btn.SetCheck(FALSE);
		bool_ChkIA = false;
		OnBnClickedAriCheck2();
	}else{
		m_ChkIA_btn.SetCheck(TRUE);
		bool_ChkIA = true;
		OnBnClickedAriCheck2();
	}

	// Update IA ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("DotIA"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_ARI_DOTIA, _T("2"));
	else
		SetDlgItemText(IDC_ARI_DOTIA, tempStr);

	// AI ���� �� ��-��-��
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("IAMode"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		OnBnClickedAriRadio3();
	else
		OnBnClickedAriRadio4();

	//üũ�ڽ� R � ������
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("CheckR"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_ChkR_btn.SetCheck(FALSE);
		bool_ChkR = false;
		OnBnClickedAriCheck3();
	}else{
		m_ChkR_btn.SetCheck(TRUE);
		bool_ChkR = true;
		OnBnClickedAriCheck3();
	}

	// Update R � ������
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("DotR"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_ARI_DOTR, _T("2"));
	else
		SetDlgItemText(IDC_ARI_DOTR, tempStr);

	//üũ�ڽ� TL ���� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("CheckTL"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_ChkTL_btn.SetCheck(FALSE);
		bool_ChkTL = false;
		OnBnClickedAriCheck4();
	}else{
		m_ChkTL_btn.SetCheck(TRUE);
		bool_ChkTL = true;
		OnBnClickedAriCheck4();
	}

	// Update TL ���� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("DotTL"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_ARI_DOTTL, _T("5"));
	else
		SetDlgItemText(IDC_ARI_DOTTL, tempStr);

	//üũ�ڽ� CL � ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("CheckCL"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_ChkCL_btn.SetCheck(FALSE);
		bool_ChkCL = false;
		OnBnClickedAriCheck5();
	}else{
		m_ChkCL_btn.SetCheck(TRUE);
		bool_ChkCL = true;
		OnBnClickedAriCheck5();
	}

	// Update CL � ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("DotCL"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_ARI_DOTCL, _T("5"));
	else
		SetDlgItemText(IDC_ARI_DOTCL, tempStr);

	//üũ�ڽ� SL ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("CheckSL"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_ChkSL_btn.SetCheck(FALSE);
		bool_ChkSL = false;
		OnBnClickedAriCheck6();
	}else{
		m_ChkSL_btn.SetCheck(TRUE);
		bool_ChkSL = true;
		OnBnClickedAriCheck6();
	}

	// Update SL ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("DotSL"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_ARI_DOTSL, _T("5"));
	else
		SetDlgItemText(IDC_ARI_DOTSL, tempStr);

	//üũ�ڽ� ���� ��Ī ���� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("CheckNoname"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_ChkNoname_btn.SetCheck(FALSE);
		bool_ChkNoname = false;
		OnBnClickedAriNoname();
	}else{
		m_ChkNoname_btn.SetCheck(TRUE);
		bool_ChkNoname = true;
		OnBnClickedAriNoname();
	}

	//���� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("Height"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_ARI_EDTHEIGHT, _T("3.000"));
	else
		SetDlgItemText(IDC_ARI_EDTHEIGHT, tempStr);

	//���̾� ���ڿ��� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("LayerStr"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_CbxLayerStr = _T("0");
	}else{
		m_CbxLayerStr = tempStr;
	}
	//�޺��ڽ� ���̾�
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("CbxLayerInt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_CbxLayer.SetCurSel(0);
		m_CbxLayerInt = _ttoi(tempStr);
		m_CbxLayer.GetLBText(0, m_CbxLayerStr);
	}else{
		m_CbxLayerIntStr = tempStr;
		m_CbxLayerInt = _ttoi(m_CbxLayerIntStr);
		m_CbxLayer.SetCurSel(m_CbxLayerInt);
		if(m_CbxLayer.FindString(-1, m_CbxLayerStr) == -1){
			m_CbxLayer.SetCurSel(0);
			m_CbxLayerInt = 0;
			m_CbxLayer.GetLBText(0, m_CbxLayerStr);
		}else{
			m_CbxLayer.SetCurSel(m_CbxLayerInt);
			m_CbxLayer.GetLBText(m_CbxLayerInt,m_CbxLayerStr);
		}
	}	

	//�۲� ���ڿ��� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("TextStyleStr"));
	if(tempStr.CompareNoCase(_T("")) == 0){
		m_CbxTextStyleStr = _T("0");
	}else{
		m_CbxTextStyleStr = tempStr;
	}
	//�޺��ڽ� �۲�
	tempStr = iniFile.readINI(_T("config.ini"), _T("ARI"), _T("CbxTextStyleInt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_CbxTextStyle.SetCurSel(0);
		m_CbxTextStyleInt = _ttoi(tempStr);
		m_CbxTextStyle.GetLBText(0, m_CbxTextStyleStr);
	}else{
		m_CbxTextStyleIntStr = tempStr;
		m_CbxTextStyleInt = _ttoi(tempStr);
		m_CbxTextStyle.SetCurSel(m_CbxTextStyleInt);
		if(m_CbxTextStyle.FindString(-1, m_CbxTextStyleStr) == -1){
			m_CbxTextStyle.SetCurSel(0);
			m_CbxTextStyleInt = 0;
			m_CbxTextStyle.GetLBText(0,m_CbxTextStyleStr);
		}else{
			m_CbxTextStyle.SetCurSel(m_CbxTextStyleInt);
			m_CbxTextStyle.GetLBText(m_CbxTextStyleInt,m_CbxTextStyleStr);
		}
	}

	return TRUE;  
}

void CARI::OnBnClickedCancel()
{	
	CAcUiDialog::OnCancel();
}

//��ɾ� ����
void CARI::OnBnClickedOk()
{	
	UpdateData();
	CAcUiDialog::OnOK();




	ads_point pt1,pt2;
	ads_name en, ename1;
	// [+] SUP-29742
	// long	sslen;
	// [-] END
	Adesk::Int32 sslen;
	AcDbObjectId objId, curId;
	AcDbObjectIdArray objarr;
	double Ang_IA,dist_R,dist_TL,dist_CL,dist_SL,XY_x, XY_y;	
	AcGePoint3d spt,ept, spt_1,ept_1,virpt1,virpt2, InterPt, InternalPt, PlrSpt1, PlrSpt2, PlrEpt1, PlrEpt2, CntrPt, Textpt, XYpt,Polypt1,Polypt2;
	double Dimang, m_stLength,m_edLength;
	CString IA_Str, IA_DotStr, TL_Str, TL_DotStr, R_Str, R_DotStr, SL_Str, SL_DotStr, CL_Str, CL_DotStr, X_Str, X_DotStr, Y_Str, Y_DotStr;
	AcGeVector3d vec1,vec2;
	AcGePoint3dArray polyarr;
	CString m_layer,m_TxtStyle, strMsg;
	int m_coloridx;

	BeginEditorCommand();
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	resbuf* filter = ads_buildlist(RTDXF0, _T("LWPOLYLINE,ARC"), RTNONE);
	//if(acedSSGet(NULL, NULL, NULL, filter, en)!=RTNORM)
	strMsg.LoadStringW(IDS_SELECT_ENTITY);
	const wchar_t* prompts[2] = { strMsg, L"", };
	if(acedSSGet(L":$", prompts, NULL, filter, en)!=RTNORM)
		return;	

	if(acedSSLength(en, &sslen) !=RTNORM){
		acedSSFree(en);
		return;
	}

	for(int i=0; i<sslen; i++){
		if(acedSSName(en, i, ename1) == RTNORM){
			acdbGetObjectId(objId, ename1);
			objarr.append(objId);
		}
	}
	acedSSFree(en);

	for(int j=0; j<objarr.length(); j++){
		curId = objarr.at(j);
		AcDbEntityPointer pEnt(curId, AcDb::kForRead);

		if(pEnt.openStatus() == Acad::eOk){
			if(pEnt->isKindOf(AcDbPolyline::desc())){
				AcDbPolyline *pPline = AcDbPolyline::cast(pEnt);
				m_layer = pPline->layer();//�ؽ�Ʈ ���̾� 
				m_coloridx = pPline->colorIndex();//�ؽ�Ʈ ����				
				pPline->getStartPoint(spt);
				pPline->getPointAtDist(0.000001,spt_1);				
				pPline->getEndPoint(ept);
				pPline->getDistAtPoint(ept,dist_CL);//��� ����
				pPline->getPointAtDist(dist_CL-0.000001,ept_1);

				int cnt = 1;
				while(true){//������ ����Ʈ array�� ����
					pPline->getPointAt(cnt-1,Polypt1);
					pPline->getPointAt(cnt,Polypt2);

					//polyarr.append(pt);						
					pPline->getDistAtPoint(Polypt1,m_stLength);//���������� ù��° ������ �Ÿ�
					pPline->getDistAtPoint(Polypt2,m_edLength);//���������� �ι��� ������ �Ÿ�
					dist_CL = m_edLength - m_stLength;// �� ������ ��� ����.

					if(abs(dist_CL - Polypt1.distanceTo(Polypt2)) < 0.001){
						if(Polypt2 == ept)
							break;
						cnt++;						
						continue;	
					}
					PyoSi(Polypt1);PyoSi(Polypt2);
					m_stLength = m_stLength+0.000001;
					m_edLength = m_edLength-0.000001;
					//pPline->highlight();
					pPline->getPointAtDist(m_stLength,spt_1);
					pPline->getPointAtDist(m_edLength,ept_1);

					Polar(Polypt1,ads_angle(asDblArray(Polypt1),asDblArray(spt_1)),1000,virpt1);
					Polar(Polypt2,ads_angle(asDblArray(Polypt2),asDblArray(ept_1)),1000,virpt2);
					acdbInters(asDblArray(Polypt1),asDblArray(virpt1),asDblArray(Polypt2),asDblArray(virpt2),0,asDblArray(InterPt));
					//PyoSi(InterPt);
					pPline->getClosestPointTo(InterPt,InternalPt);

					vec1 = InterPt - virpt1;
					vec2 = InterPt - Polypt2;


					IA_DotStr.Format(_T("%%.%df"),m_DotIA);
					IA_Str.Format(IA_DotStr,RTD(vec1.angleTo(vec2)));
					Ang_IA = _tstof(IA_Str);//����

					R_DotStr.Format(_T("%%.%df"),m_DotR);
					R_Str.Format(R_DotStr,dist_CL * 360 / Ang_IA / (PI*2));
					dist_R = _tstof(R_Str);//������

					TL_DotStr.Format(_T("%%.%df"),m_DotTL);
					TL_Str.Format(TL_DotStr,Polypt1.distanceTo(InterPt));
					dist_TL = _tstof(TL_Str);//��������

					CL_DotStr.Format(_T("%%.%df"),m_DotCL);
					CL_Str.Format(CL_DotStr, dist_CL);
					dist_CL = _tstof(CL_Str);//� ����

					SL_DotStr.Format(_T("%%.%df"),m_DotSL);
					SL_Str.Format(SL_DotStr,InterPt.distanceTo(InternalPt));
					dist_SL = _tstof(SL_Str);//����
					//PyoSi(InterPt);PyoSi(InternalPt);
					Polar(Polypt1,ads_angle(asDblArray(Polypt1),asDblArray(spt_1))+DTR(90.0),dist_CL, PlrSpt1);
					Polar(Polypt1,ads_angle(asDblArray(Polypt1),asDblArray(spt_1))-DTR(90.0),dist_CL, PlrSpt2);
					Polar(Polypt2,ads_angle(asDblArray(Polypt2),asDblArray(ept_1))+DTR(90.0),dist_CL, PlrEpt1);
					Polar(Polypt2,ads_angle(asDblArray(Polypt2),asDblArray(ept_1))-DTR(90.0),dist_CL, PlrEpt2);

					acdbInters(asDblArray(PlrSpt1),asDblArray(PlrSpt2),asDblArray(PlrEpt1),asDblArray(PlrEpt2),0,asDblArray(CntrPt));
					//PyoSi(PlrSpt1);PyoSi(PlrSpt2);PyoSi(PlrEpt1);PyoSi(PlrEpt2);
					XYpt = CntrPt;//� �߽� ��ǥ
					double vs;
					if(XYMode == 1){//� �߽� ��ǥ ����� �� xy ����
						vs = XYpt.x;
						XYpt.x = XYpt.y;
						XYpt.y = vs;
					}

					strMsg.LoadStringW(IDS_SELECT_ENTITY);
					if(acedGetPoint(0, strMsg, asDblArray(Textpt))!=RTNORM)
						return;

					m_CbxLayer.GetLBText(m_CbxLayerInt,m_layer);
					m_CbxTextStyle.GetLBText(m_CbxTextStyleInt,m_TxtStyle);//�޺��ڽ� �۲�

					if(bool_ChkXY){//� �߽� ��ǥ 
						if(bool_ChkNoname){
							X_DotStr.Format(_T("%%.%df"), m_DotXY);
							X_Str.Format(X_DotStr, XYpt.x);
							Y_DotStr.Format(_T("%%.%df"), m_DotXY);
							Y_Str.Format(Y_DotStr, XYpt.y);
						}else{
							X_DotStr.Format(_T("X=%%.%df"), m_DotXY);
							X_Str.Format(X_DotStr, XYpt.x);
							Y_DotStr.Format(_T("Y=%%.%df"), m_DotXY);
							Y_Str.Format(Y_DotStr, XYpt.y);
						}

						CreateText(Textpt,m_TxtHeight,0,0,X_Str, m_layer,m_TxtStyle,m_coloridx,1);
						Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
						CreateText(Textpt,m_TxtHeight,0,0,Y_Str, m_layer,m_TxtStyle,m_coloridx,1);
						Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
					}

					if(bool_ChkIA){//����
						if(IAMode == 1){//��
							if(bool_ChkNoname){
								IA_DotStr.Format(_T("%%.%df"),m_DotIA);
								IA_Str.Format(IA_DotStr,Ang_IA);								
							}else{
								IA_DotStr.Format(_T("IA=%%.%df"),m_DotIA);
								IA_Str.Format(IA_DotStr,Ang_IA);
							}
						}else{//��-��-��
							double d_Degree,d_Minute,d_Second;
							int i_Degree,i_Minute, i_Second;

							IA_DotStr.Format(_T("%%.%df"),m_DotIA);
							IA_Str.Format(IA_DotStr,Ang_IA);	
							Ang_IA = _tstof(IA_Str);

							d_Degree = Ang_IA;
							i_Degree = Ang_IA;
							d_Minute = (d_Degree - i_Degree)*60;
							i_Minute = d_Minute;
							d_Second = (d_Minute - i_Minute)*60;
							i_Second = d_Second;

							if(bool_ChkNoname){
								IA_DotStr.Format(_T("%d-%d-%%.%df"), i_Degree,i_Minute, m_DotIA);
								IA_Str.Format(IA_DotStr,d_Second);	
							}else{
								IA_DotStr.Format(_T("IA=%d-%d-%%.%df"), i_Degree,i_Minute, m_DotIA);
								IA_Str.Format(IA_DotStr,d_Second);	
							}
						}


						CreateText(Textpt,m_TxtHeight,0,0,IA_Str, m_layer,m_TxtStyle,m_coloridx,1);
						Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
					}

					if(bool_ChkR){//� ������
						if(bool_ChkNoname){
							R_DotStr.Format(_T("%%.%df"),m_DotR);
							R_Str.Format(R_DotStr, dist_R);
						}else{
							R_DotStr.Format(_T("R=%%.%df"),m_DotR);
							R_Str.Format(R_DotStr, dist_R);
						}


						CreateText(Textpt,m_TxtHeight,0,0,R_Str, m_layer,m_TxtStyle,m_coloridx,1);
						Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
					}			

					if(bool_ChkTL){//���� ����
						if(bool_ChkNoname){
							TL_DotStr.Format(_T("%%.%df"),m_DotTL);
							TL_Str.Format(TL_DotStr, dist_TL);
						}else{
							TL_DotStr.Format(_T("TL=%%.%df"),m_DotTL);
							TL_Str.Format(TL_DotStr, dist_TL);
						}
						CreateText(Textpt,m_TxtHeight,0,0,TL_Str, m_layer,m_TxtStyle,m_coloridx,1);
						Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
					}	

					if(bool_ChkCL){//� ����
						if(bool_ChkNoname){
							CL_DotStr.Format(_T("%%.%df"),m_DotCL);
							CL_Str.Format(CL_DotStr, dist_CL);
						}else{
							CL_DotStr.Format(_T("CL=%%.%df"),m_DotCL);
							CL_Str.Format(CL_DotStr, dist_CL);
						}
						CreateText(Textpt,m_TxtHeight,0,0,CL_Str, m_layer,m_TxtStyle,m_coloridx,1);
						Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
					}

					if(bool_ChkSL){//����
						if(bool_ChkNoname){
							SL_DotStr.Format(_T("%%.%df"),m_DotSL);
							SL_Str.Format(SL_DotStr, dist_SL);
						}else{
							SL_DotStr.Format(_T("SL=%%.%df"),m_DotSL);
							SL_Str.Format(SL_DotStr, dist_SL);
						}

						CreateText(Textpt,m_TxtHeight,0,0,SL_Str, m_layer,m_TxtStyle,m_coloridx,1);
						Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
					}

					if(Polypt2 == ept)
						break;
					cnt++;
				}
				pPline->close();	
				//acutPrintf(_T("\n AI : %f \n R : %f \n TL : %f \n CL : %f \n SL : %f"),Ang_IA,dist_R,dist_TL,dist_CL, dist_SL);

			}else if(pEnt->isKindOf(AcDbArc::desc())){
				AcDbArc *pArc = AcDbArc::cast(pEnt);
				m_layer = pArc->layer();//�ؽ�Ʈ ���̾� 
				m_coloridx = pArc->colorIndex();//�ؽ�Ʈ ����				
				pArc->getStartPoint(spt);
				pArc->getPointAtDist(0.000001,spt_1);				
				pArc->getEndPoint(ept);
				pArc->getDistAtPoint(ept,dist_CL);//��� ����
				pArc->getPointAtDist(dist_CL-0.000001,ept_1);

				Polar(spt,ads_angle(asDblArray(spt),asDblArray(spt_1)),dist_CL,virpt1);
				Polar(ept,ads_angle(asDblArray(ept),asDblArray(ept_1)),dist_CL,virpt2);
				acdbInters(asDblArray(spt),asDblArray(virpt1),asDblArray(ept),asDblArray(virpt2),1,asDblArray(InterPt));				
				pArc->getClosestPointTo(InterPt,InternalPt);

				vec1 = InterPt - virpt1;
				vec2 = InterPt - ept;


				IA_DotStr.Format(_T("%%.%df"),m_DotIA);
				IA_Str.Format(IA_DotStr,RTD(vec1.angleTo(vec2)));
				Ang_IA = _tstof(IA_Str);//����

				R_DotStr.Format(_T("%%.%df"),m_DotR);
				R_Str.Format(R_DotStr,360 * dist_CL / (PI*2) / Ang_IA);
				dist_R = _tstof(R_Str);//������

				TL_DotStr.Format(_T("%%.%df"),m_DotTL);
				TL_Str.Format(TL_DotStr,spt.distanceTo(InterPt));
				dist_TL = _tstof(TL_Str);//��������

				CL_DotStr.Format(_T("%%.%df"),m_DotCL);
				CL_Str.Format(CL_DotStr, dist_CL);
				dist_CL = _tstof(CL_Str);//� ����

				SL_DotStr.Format(_T("%%.%df"),m_DotSL);
				SL_Str.Format(SL_DotStr,InterPt.distanceTo(InternalPt));
				dist_SL = _tstof(SL_Str);//����
				//acutPrintf(_T("\n AI : %f \n R : %f \n TL : %f \n CL : %f \n SL : %f"),Ang_IA,dist_R,dist_TL,dist_CL, dist_SL);

				Polar(spt,ads_angle(asDblArray(spt),asDblArray(spt_1))+DTR(90.0),dist_CL, PlrSpt1);
				Polar(spt,ads_angle(asDblArray(spt),asDblArray(spt_1))-DTR(90.0),dist_CL, PlrSpt2);
				Polar(ept,ads_angle(asDblArray(ept),asDblArray(ept_1))+DTR(90.0),dist_CL, PlrEpt1);
				Polar(ept,ads_angle(asDblArray(ept),asDblArray(ept_1))-DTR(90.0),dist_CL, PlrEpt2);

				acdbInters(asDblArray(PlrSpt1),asDblArray(PlrSpt2),asDblArray(PlrEpt1),asDblArray(PlrEpt2),1,asDblArray(CntrPt));
				XYpt = CntrPt;//� �߽� ��ǥ
				double vs;
				if(XYMode == 1){//� �߽� ��ǥ ����� �� xy ����
					vs = XYpt.x;
					XYpt.x = XYpt.y;
					XYpt.y = vs;
				}

				pArc->close();	
				strMsg.LoadStringW(IDS_INSERT_TEXT); // _T("\n���� ������ : "
				if(acedGetPoint(0, strMsg, asDblArray(Textpt))!=RTNORM)
					return;

				m_CbxLayer.GetLBText(m_CbxLayerInt,m_layer);//�޺��ڽ� ���̾�
				m_CbxTextStyle.GetLBText(m_CbxTextStyleInt,m_TxtStyle);//�޺��ڽ� �۲�

				if(bool_ChkXY){//� �߽� ��ǥ 
					if(bool_ChkNoname){
						X_DotStr.Format(_T("%%.%df"), m_DotXY);
						X_Str.Format(X_DotStr, XYpt.x);
						Y_DotStr.Format(_T("%%.%df"), m_DotXY);
						Y_Str.Format(Y_DotStr, XYpt.y);
					}else{
						X_DotStr.Format(_T("X=%%.%df"), m_DotXY);
						X_Str.Format(X_DotStr, XYpt.x);
						Y_DotStr.Format(_T("Y=%%.%df"), m_DotXY);
						Y_Str.Format(Y_DotStr, XYpt.y);
					}

					CreateText(Textpt,m_TxtHeight,0,0,X_Str, m_layer,m_TxtStyle,m_coloridx,1);
					Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
					CreateText(Textpt,m_TxtHeight,0,0,Y_Str, m_layer,m_TxtStyle,m_coloridx,1);
					Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
				}

				if(bool_ChkIA){//����
					if(IAMode == 1){//��
						if(bool_ChkNoname){
							IA_DotStr.Format(_T("%%.%df"),m_DotIA);
							IA_Str.Format(IA_DotStr,Ang_IA);								
						}else{
							IA_DotStr.Format(_T("IA=%%.%df"),m_DotIA);
							IA_Str.Format(IA_DotStr,Ang_IA);
						}
					}else{//��-��-��
						double d_Degree,d_Minute,d_Second;
						int i_Degree,i_Minute, i_Second;

						IA_DotStr.Format(_T("%%.%df"),m_DotIA);
						IA_Str.Format(IA_DotStr,Ang_IA);	
						Ang_IA = _tstof(IA_Str);

						d_Degree = Ang_IA;
						i_Degree = Ang_IA;
						d_Minute = (d_Degree - i_Degree)*60;
						i_Minute = d_Minute;
						d_Second = (d_Minute - i_Minute)*60;
						i_Second = d_Second;

						if(bool_ChkNoname){
							IA_DotStr.Format(_T("%d-%d-%%.%df"), i_Degree,i_Minute, m_DotIA);
							IA_Str.Format(IA_DotStr,d_Second);	
						}else{
							IA_DotStr.Format(_T("IA=%d-%d-%%.%df"), i_Degree,i_Minute, m_DotIA);
							IA_Str.Format(IA_DotStr,d_Second);	
						}
					}


					CreateText(Textpt,m_TxtHeight,0,0,IA_Str, m_layer,m_TxtStyle,m_coloridx,1);
					Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
				}

				if(bool_ChkR){//� ������
					if(bool_ChkNoname){
						R_DotStr.Format(_T("%%.%df"),m_DotR);
						R_Str.Format(R_DotStr, dist_R);
					}else{
						R_DotStr.Format(_T("R=%%.%df"),m_DotR);
						R_Str.Format(R_DotStr, dist_R);
					}


					CreateText(Textpt,m_TxtHeight,0,0,R_Str, m_layer,m_TxtStyle,m_coloridx,1);
					Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
				}			

				if(bool_ChkTL){//���� ����
					if(bool_ChkNoname){
						TL_DotStr.Format(_T("%%.%df"),m_DotTL);
						TL_Str.Format(TL_DotStr, dist_TL);
					}else{
						TL_DotStr.Format(_T("TL=%%.%df"),m_DotTL);
						TL_Str.Format(TL_DotStr, dist_TL);
					}
					CreateText(Textpt,m_TxtHeight,0,0,TL_Str, m_layer,m_TxtStyle,m_coloridx,1);
					Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
				}	

				if(bool_ChkCL){//� ����
					if(bool_ChkNoname){
						CL_DotStr.Format(_T("%%.%df"),m_DotCL);
						CL_Str.Format(CL_DotStr, dist_CL);
					}else{
						CL_DotStr.Format(_T("CL=%%.%df"),m_DotCL);
						CL_Str.Format(CL_DotStr, dist_CL);
					}
					CreateText(Textpt,m_TxtHeight,0,0,CL_Str, m_layer,m_TxtStyle,m_coloridx,1);
					Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
				}

				if(bool_ChkSL){//����
					if(bool_ChkNoname){
						SL_DotStr.Format(_T("%%.%df"),m_DotSL);
						SL_Str.Format(SL_DotStr, dist_SL);
					}else{
						SL_DotStr.Format(_T("SL=%%.%df"),m_DotSL);
						SL_Str.Format(SL_DotStr, dist_SL);
					}

					CreateText(Textpt,m_TxtHeight,0,0,SL_Str, m_layer,m_TxtStyle,m_coloridx,1);
					Polar(Textpt,DTR(270.0),m_TxtHeight*1.5, Textpt);
				}
			}
		}

	}

	CompleteEditorCommand();

}

//üũ�ڽ� X,Y � �߽� ��ǥ 
void CARI::OnBnClickedAriCheck1()
{
	UpdateData(TRUE);
	bool_ChkXY = IsDlgButtonChecked(IDC_ARI_CHECK1);
	if(bool_ChkXY == 0){
		m_ChkXY_btn.SetCheck(FALSE);
		m_ChkXYStr = _T("0");
	}else{
		m_ChkXY_btn.SetCheck(TRUE);
		m_ChkXYStr = _T("1");
	}
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("CheckXY"), m_ChkXYStr);
	UpdateData(FALSE);
}

// Update XY � �߽� ��ǥ 
void CARI::OnEnUpdateAriDotxy()
{
	UpdateData(TRUE);
	m_DotXY = _ttoi(m_DotXYStr);
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("DotXY"), m_DotXYStr);
	UpdateData(FALSE);
}

// X,Y � �߽� ��ǥ SPIN
void CARI::OnDeltaposAriSpinxy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0)
		m_DotXY++;
	else
		m_DotXY--;
	SetDlgItemInt(IDC_ARI_DOTXY, m_DotXY);
	*pResult = 0;
}

// ���� X,Y � �߽� ��ǥ ���
void CARI::OnBnClickedAriRadio1()
{
	UpdateData(TRUE);
	XYMode = 1;
	XYMode_Str.Format(_T("%d"),XYMode);
	b_RadBtn_XY = 0;
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("XYMode"), XYMode_Str);
	UpdateData(FALSE);
}

// ���� X,Y � �߽� ��ǥ ����
void CARI::OnBnClickedAriRadio2()
{
	UpdateData(TRUE);
	XYMode = 2;
	XYMode_Str.Format(_T("%d"),XYMode);
	b_RadBtn_XY = 1;
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("XYMode"), XYMode_Str);
	UpdateData(FALSE);
}

//üũ�ڽ� IA ����
void CARI::OnBnClickedAriCheck2()
{
	UpdateData(TRUE);
	bool_ChkIA = IsDlgButtonChecked(IDC_ARI_CHECK2);
	if(bool_ChkIA == 0){
		m_ChkIA_btn.SetCheck(FALSE);
		m_ChkIAStr = _T("0");
	}else{
		m_ChkIA_btn.SetCheck(TRUE);
		m_ChkIAStr = _T("1");
	}
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("CheckIA"), m_ChkIAStr);
	UpdateData(FALSE);
}

// Update IA ����
void CARI::OnEnUpdateAriDotia()
{
	UpdateData(TRUE);
	m_DotIA = _ttoi(m_DotIAStr);
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("DotIA"), m_DotIAStr);
	UpdateData(FALSE);
}

// IA ���� SPIN
void CARI::OnDeltaposAriSpinia(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0)
		m_DotIA++;
	else
		m_DotIA--;
	SetDlgItemInt(IDC_ARI_DOTIA, m_DotIA);
	*pResult = 0;
}

// ���� IA ���� ��
void CARI::OnBnClickedAriRadio3()
{
	UpdateData(TRUE);
	IAMode = 1;
	IAMode_Str.Format(_T("%d"),IAMode);
	b_RadBtn_IA = 0;
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("IAMode"), IAMode_Str);
	UpdateData(FALSE);
}

// ���� IA ���� ��-��-��
void CARI::OnBnClickedAriRadio4()
{
	UpdateData(TRUE);
	IAMode = 2;
	IAMode_Str.Format(_T("%d"),IAMode);
	b_RadBtn_IA = 1;
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("IAMode"), IAMode_Str);
	UpdateData(FALSE);
}

//üũ�ڽ� R � ������
void CARI::OnBnClickedAriCheck3()
{
	UpdateData(TRUE);
	bool_ChkR = IsDlgButtonChecked(IDC_ARI_CHECK3);
	if(bool_ChkR == 0){
		m_ChkR_btn.SetCheck(FALSE);
		m_ChkRStr = _T("0");
	}else{
		m_ChkR_btn.SetCheck(TRUE);
		m_ChkRStr = _T("1");
	}
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("CheckR"), m_ChkRStr);
	UpdateData(FALSE);
}

// Update R � ������
void CARI::OnEnUpdateAriDotr()
{
	UpdateData(TRUE);
	m_DotR = _ttoi(m_DotRStr);
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("DotR"), m_DotRStr);
	UpdateData(FALSE);
}

// R � ������ SPIN
void CARI::OnDeltaposAriSpinr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0)
		m_DotR++;
	else
		m_DotR--;
	SetDlgItemInt(IDC_ARI_DOTR, m_DotR);
	*pResult = 0;
}

//üũ�ڽ� TL ���� ����
void CARI::OnBnClickedAriCheck4()
{
	UpdateData(TRUE);
	bool_ChkTL = IsDlgButtonChecked(IDC_ARI_CHECK4);
	if(bool_ChkTL == 0){
		m_ChkTL_btn.SetCheck(FALSE);
		m_ChkTLStr = _T("0");
	}else{
		m_ChkTL_btn.SetCheck(TRUE);
		m_ChkTLStr = _T("1");
	}
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("CheckTL"), m_ChkTLStr);
	UpdateData(FALSE);
}

// Update TL ���� ����
void CARI::OnEnUpdateAriDottl()
{
	UpdateData(TRUE);
	m_DotTL = _ttoi(m_DotTLStr);
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("DotTL"), m_DotTLStr);
	UpdateData(FALSE);
}

// TL ���� ���� SPIN
void CARI::OnDeltaposAriSpintl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0)
		m_DotTL++;
	else
		m_DotTL--;
	SetDlgItemInt(IDC_ARI_DOTTL, m_DotTL);
	*pResult = 0;
}

// üũ�ڽ� CL � ����
void CARI::OnBnClickedAriCheck5()
{
	UpdateData(TRUE);
	bool_ChkCL = IsDlgButtonChecked(IDC_ARI_CHECK5);
	if(bool_ChkCL == 0){
		m_ChkCL_btn.SetCheck(FALSE);
		m_ChkCLStr = _T("0");
	}else{
		m_ChkCL_btn.SetCheck(TRUE);
		m_ChkCLStr = _T("1");
	}
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("CheckCL"), m_ChkCLStr);
	UpdateData(FALSE);
}

// Update CL � ����
void CARI::OnEnUpdateAriDotcl()
{
	UpdateData(TRUE);
	m_DotCL = _ttoi(m_DotCLStr);
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("DotCL"), m_DotCLStr);
	UpdateData(FALSE);
}

// CL � ���� SPIN 
void CARI::OnDeltaposAriSpincl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0)
		m_DotCL++;
	else
		m_DotCL--;
	SetDlgItemInt(IDC_ARI_DOTCL, m_DotCL);
	*pResult = 0;
}

// üũ�ڽ� SL ����
void CARI::OnBnClickedAriCheck6()
{
	UpdateData(TRUE);
	bool_ChkSL = IsDlgButtonChecked(IDC_ARI_CHECK6);
	if(bool_ChkSL == 0){
		m_ChkSL_btn.SetCheck(FALSE);
		m_ChkSLStr = _T("0");
	}else{
		m_ChkSL_btn.SetCheck(TRUE);
		m_ChkSLStr = _T("1");
	}
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("CheckSL"), m_ChkSLStr);
	UpdateData(FALSE);
}

// Update SL ����
void CARI::OnEnUpdateAriDotsl()
{
	UpdateData(TRUE);
	m_DotSL = _ttoi(m_DotSLStr);
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("DotSL"), m_DotSLStr);
	UpdateData(FALSE);
}

// SL ���� SPIN
void CARI::OnDeltaposAriSpinsl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0)
		m_DotSL++;
	else
		m_DotSL--;
	SetDlgItemInt(IDC_ARI_DOTSL, m_DotSL);
	*pResult = 0;
}

//üũ�ڽ� ���� ��Ī ���� ����.
void CARI::OnBnClickedAriNoname()
{
	UpdateData(TRUE);
	bool_ChkNoname = IsDlgButtonChecked(IDC_ARI_NONAME);
	if(bool_ChkNoname == 0){
		m_ChkNoname_btn.SetCheck(FALSE);
		m_ChkNonameStr = _T("0");
	}else{
		m_ChkNoname_btn.SetCheck(TRUE);
		m_ChkNonameStr = _T("1");
	}
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("CheckNoname"), m_ChkNonameStr);
	UpdateData(FALSE);
}

// Update ���� ����
void CARI::OnEnUpdateAriEdtheight()
{
	UpdateData(TRUE);
	m_TxtHeight = _tstof(m_TxtHeightStr);
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("Height"), m_TxtHeightStr);
	UpdateData(FALSE);
}

// ���� ���� �����ư
void CARI::OnBnClickedAriBselect1()
{
	UpdateData(TRUE);
	BeginEditorCommand();

	ads_name en;
	ads_point pt;
	CString strMsg;
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		strMsg.LoadStringW(IDS_SELECT_TEXT); // _T("\n���� ���� : ")
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
					m_TxtHeight = pText->height();//�ؽ�Ʈ ����	
					pText->close();					

					m_TxtHeightStr.Format(_T("%.3f"),m_TxtHeight);					
					SetDlgItemText(IDC_ARI_EDTHEIGHT, m_TxtHeightStr);//������ �ؽ�Ʈ ���̰����� ����
					iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("Height"), m_TxtHeightStr);					
					break;
				}else{
					strMsg.LoadStringW(IDS_CHECK_STRING); // _T("**������ ��ü�� ���ڰ� �ƴմϴ�.**")
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

// �޺��ڽ� ���̾� 
void CARI::OnCbnSelchangeAriCmblayer()
{
	UpdateData(TRUE);
	m_CbxLayerInt = m_CbxLayer.GetCurSel();
	if(m_CbxLayerInt == -1)m_CbxLayerInt = 0;
	m_CbxLayer.SetCurSel(m_CbxLayerInt);
	m_CbxLayerIntStr.Format(_T("%d"), m_CbxLayerInt);
	m_CbxLayer.GetLBText(m_CbxLayerInt, m_CbxLayerStr);
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("CbxLayerInt"), m_CbxLayerIntStr);
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("LayerStr"), m_CbxLayerStr);
	UpdateData(FALSE);
}

// �޺��ڽ� ���̾� �����ư
void CARI::OnBnClickedAriBselect2()
{
	UpdateData(TRUE);
	BeginEditorCommand();

	ads_name en;
	ads_point pt;
	CString strMsg;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		strMsg.LoadStringW(IDS_SELECT_TEXT); // _T("\n��ü ���� : ")
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
				if(pEnt->isKindOf(AcDbEntity::desc())){
					AcDbEntity* pEntity = AcDbEntity::cast(pEnt);
					m_CbxLayerStr = pEntity->layer();		
					pEntity->close();

					for(int i=0; i < m_CbxLayer.GetCount(); i++){
						if(m_CbxLayer.FindString(i, m_CbxLayerStr) == -1){//�޺��ڽ��� ������ ���̾� �߰�
							m_CbxLayer.AddString(m_CbxLayerStr);
						}
					}
					m_CbxLayerInt = m_CbxLayer.FindString(0,m_CbxLayerStr);//������ ���̾� index ���ϱ�
					m_CbxLayer.SetCurSel(m_CbxLayerInt);//�޺��ڽ��� �߰��� ���̾�� ����

					m_CbxLayerIntStr.Format(_T("%d"), m_CbxLayerInt);	
					iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("CbxLayerInt"), m_CbxLayerIntStr);
					iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("LayerStr"), m_CbxLayerStr);				
					break;
				}else{
					strMsg.LoadStringW(IDS_SELECT_TEXT); // _T("**��ü�� �ƴմϴ�.**")
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

// �޺��ڽ� �۲� 
void CARI::OnCbnSelchangeAriCmbstyle()
{
	UpdateData(TRUE);
	m_CbxTextStyleInt = m_CbxTextStyle.GetCurSel();
	if(m_CbxTextStyleInt == -1)m_CbxTextStyleInt = 0;
	m_CbxTextStyle.SetCurSel(m_CbxTextStyleInt);
	m_CbxTextStyleIntStr.Format(_T("%d"), m_CbxTextStyleInt);
	m_CbxTextStyle.GetLBText(m_CbxTextStyleInt, m_CbxTextStyleStr);
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("CbxTextStyleInt"), m_CbxTextStyleIntStr);
	iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("TextStyleStr"), m_CbxTextStyleStr);	
	UpdateData(FALSE);
}

// �޺��ڽ� �۲� ���� ��ư
void CARI::OnBnClickedAriBselect3()
{
	UpdateData(TRUE);
	BeginEditorCommand();

	ads_name en;
	ads_point pt;
	CString strMsg;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		strMsg.LoadStringW(IDS_SELECT_TEXT); // _T("\n���� ���� : "
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
					objId = pText->textStyle();

					AcDbTextStyleTableRecord *pTSTR = NULL;
					acdbOpenObject(pTSTR, objId, AcDb::kForRead);
					pTSTR->getName(Name);
					pText->close();					

					for(int i=0; i < m_CbxTextStyle.GetCount(); i++){
						if(m_CbxTextStyle.FindString(i, Name) == -1){//�޺��ڽ��� ������ �۲� �߰�
							m_CbxTextStyle.AddString(Name);
						}
					}
					m_CbxTextStyleInt = m_CbxTextStyle.FindString(0,Name);//������ �۲� index ���ϱ�
					m_CbxTextStyle.SetCurSel(m_CbxTextStyleInt);//�޺��ڽ��� �߰��� �۲÷� ����

					m_CbxTextStyleStr.Format(_T("%s"),Name);					
					m_CbxTextStyleIntStr.Format(_T("%d"),m_CbxTextStyleInt);

					iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("CbxTextStyleInt"), m_CbxTextStyleIntStr);
					iniFile.writeINI(_T("config.ini"), _T("ARI"), _T("TextStyleStr"), m_CbxTextStyleStr);				
					break;
				}else{
					strMsg.LoadStringW(IDS_CHECK_STRING); // _T("**������ ��ü�� ���ڰ� �ƴմϴ�.**")
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

void CARI::PyoSi(const AcGePoint3d pos)
{
	AcGePoint3d	pt1, pt2, pt3;
	double		ang[] = { 0.25, 0.75 };
	double		ps = getPickBox();		// real size of pickbox in screen 
	double kPi = 3.14159265358979323846;


	for (int dur = 0; dur < 2; dur++) {
		ads_polar(asDblArray(pos), ang[dur] * kPi, ps, asDblArray(pt2));
		ads_polar(asDblArray(pos), (ang[dur] + 1.0) * kPi, ps, asDblArray(pt3));
		acedGrDraw(asDblArray(pt2), asDblArray(pt3), 1, 0);
	}//end-for
}

double CARI::getPickBox()
{
	int			iPickBox;
	double		viewsize;
	AcGePoint2d screensize;

	getSysVar(_T("VIEWSIZE"), viewsize);
	getSysVar(_T("SCREENSIZE"), screensize);
	getSysVar(_T("PICKBOX"), iPickBox);

	return ((viewsize / screensize.y)*(double)iPickBox);
}

Acad::ErrorStatus CARI::getSysVar(LPCTSTR varName, double& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTREAL);
		val = rb.resval.rreal;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

Acad::ErrorStatus CARI::getSysVar(LPCTSTR varName, int& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTSHORT);
		val = rb.resval.rint;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

Acad::ErrorStatus CARI::getSysVar(LPCTSTR varName, AcGePoint2d& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTPOINT);
		val.set(rb.resval.rpoint[X], rb.resval.rpoint[Y]);
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

void CARI::Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result)
{
	result.set(base.x + dist*cos(angle), base.y + dist*sin(angle), base.z);
}

////������ ���� ������ ��ȯ�Ͽ� �����Ѵ�.
double CARI::DTR(double angle)
{
	return (angle / 180.0 * PI);
}
////���� ���� ������ ��ȯ�Ͽ� �����Ѵ�.
double CARI::RTD(double radian)
{
	return (radian * 180.0 / PI);
}

//�ؽ�Ʈ���� 
double CARI::CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer,CString Style,double Color, int mode){
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
		es = pText->setVerticalMode(AcDb::kTextVertMid);
		es = pText->setHorizontalMode(AcDb::kTextLeft);
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
	mExtText.maxPoint().x - mExtText.minPoint().x;//ĳ��� �ؽ�Ʈ ����
	mExtText.maxPoint().y - mExtText.minPoint().y;//ĳ��� �ؽ�Ʈ ����
	mExtText.maxPoint();
	mExtText.minPoint();
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
	return 0;
}

AcDbObjectId CARI::findTextStyleName(CString name){
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

void CARI::OnBnClickedAriHelp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	Help_Path.Format(_T("%s\\%s::/1. html/ARI.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
