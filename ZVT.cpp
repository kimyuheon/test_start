#include "stdafx.h"
#include "ZVT.h"
#include "afxdialogex.h"

// CZVT ��ȭ �����Դϴ�.
IMPLEMENT_DYNAMIC(CZVT, CAcUiDialog)
	CZVT::CZVT(CWnd* pParent)
	: CAcUiDialog(CZVT::IDD, pParent), m_textHeightStr(_T(""))
	, m_textWidthStr(_T(""))
	, m_textDistStr(_T("")){
}

CZVT::~CZVT(){
}

void CZVT::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBXTEXTLAYER, m_cbxTextLayer);
	DDX_Control(pDX, IDC_CBXTEXTSTYLE, m_cbxTextStyle);
	DDX_Control(pDX, IDC_CBXPOINTLAYER, m_cbxPointLayer);
	DDX_Control(pDX, IDC_BPOINT, m_PointSelectButton);
	DDX_Control(pDX, IDC_BTEXT, m_TextSelectButton);
	DDX_Control(pDX, IDC_EDTTEXTHEIGHT, m_textHeightCtrl);
	DDX_Text(pDX, IDC_EDTTEXTHEIGHT, m_textHeightStr);
	DDX_Control(pDX, IDC_EDTTEXTWIDHT, m_textWidthCtrl);
	DDX_Text(pDX, IDC_EDTTEXTWIDHT, m_textWidthStr);
	DDX_Control(pDX, IDC_EDTDIST, m_textDistCtrl);
	DDX_Text(pDX, IDC_EDTDIST, m_textDistStr);
}

BEGIN_MESSAGE_MAP(CZVT, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CZVT::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CZVT::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTEXT, &CZVT::OnBnClickedBtext)
	ON_BN_CLICKED(IDC_BPOINT, &CZVT::OnBnClickedBpoint)
	ON_EN_UPDATE(IDC_EDTTEXTHEIGHT, &CZVT::OnEnUpdateEdttextheight)
	ON_EN_UPDATE(IDC_EDTTEXTWIDHT, &CZVT::OnEnUpdateEdttextwidht)
	ON_EN_UPDATE(IDC_EDTDIST, &CZVT::OnEnUpdateEdtdist)
	ON_BN_CLICKED(ID_ZVT_HELP, &CZVT::OnBnClickedZvtHelp)
END_MESSAGE_MAP()

//��ȭ���� �ʱ�ȭ
BOOL CZVT::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	//������ ���
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	//��ư ������ �ε�
	m_PointSelectButton.AutoLoad();
	m_TextSelectButton.AutoLoad();

	//������ �ʱ�ȭ
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
		m_cbxTextLayer.AddString(name);
		m_cbxPointLayer.AddString(name);
	}	
	delete pLayerTblIter;
	delete pLayerTbl;

	m_cbxTextLayer.SetCurSel(0);
	m_cbxPointLayer.SetCurSel(0);

	//���� ��Ÿ�� �ʱ�ȭ
	/*
	AcDbTextStyleTable* pTextStyleTbl = NULL;
	AcDbTextStyleTablePointer pTextStylePoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbTextStyleTableIterator* pTextStyleTblIter;
	pTextStylePoint->newIterator(pTextStyleTblIter);

	AcDbObjectId textStyleId;
	for(pTextStyleTblIter->start(); !pTextStyleTblIter->done(); pTextStyleTblIter->step()){
		pTextStyleTblIter->getRecordId(textStyleId);
		AcDbTextStyleTableRecordPointer pTextStyleTblRcd(textStyleId, AcDb::kForRead);
		TCHAR* name;
		pTextStyleTblRcd->getName(name);
		m_cbxTextStyle.AddString(name);
	}
	delete pTextStyleTblIter;
	delete pTextStyleTbl;
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

	m_cbxTextStyle.SetCurSel(0);

	//�ʱ�ȭ �ɼ� �ҷ�����
	CString pointCreateStr = iniFile.readINI(_T("config.ini"), _T("ZVT"), _T("POINTCREATE"));
	if(pointCreateStr.CompareNoCase(_T("")) == 0 || pointCreateStr.CompareNoCase(_T("FALSE")) == 0)
		CheckDlgButton(IDC_CHKCP, FALSE);
	else
		CheckDlgButton(IDC_CHKCP, TRUE);

	CString textZValueStr = iniFile.readINI(_T("config.ini"), _T("ZVT"), _T("TEXTZVALUE"));
	if(textZValueStr.CompareNoCase(_T("")) == 0 || textZValueStr.CompareNoCase(_T("FALSE")) == 0)
		CheckDlgButton(IDC_CHKZV, FALSE);
	else
		CheckDlgButton(IDC_CHKZV, TRUE);

	CString textHeightStr = iniFile.readINI(_T("config.ini"), _T("ZVT"), _T("TEXTHEIGHT"));
	if(textHeightStr.IsEmpty())
		textHeightStr = _T("2.5");
	SetDlgItemText(IDC_EDTTEXTHEIGHT, textHeightStr);

	CString textWidthStr = iniFile.readINI(_T("config.ini"), _T("ZVT"), _T("TEXTWIDTH"));
	if(textWidthStr.IsEmpty())
		textWidthStr = _T("10");
	SetDlgItemText(IDC_EDTTEXTWIDHT, textWidthStr);

	CString textPositionStr = iniFile.readINI(_T("config.ini"), _T("ZVT"), _T("TEXTPOSITION"));
	if(textPositionStr.CompareNoCase(_T("")) == 0 || textPositionStr.CompareNoCase(_T("UP")) == 0)
		CheckDlgButton(IDC_RADUP, TRUE);
	else if(textPositionStr.CompareNoCase(_T("DOWN")) == 0)
		CheckDlgButton(IDC_RADDOWN, TRUE);
	else if(textPositionStr.CompareNoCase(_T("LEFT")) == 0)
		CheckDlgButton(IDC_RADLEFT, TRUE);
	else if(textPositionStr.CompareNoCase(_T("RIGHT")) == 0)
		CheckDlgButton(IDC_RADRIGHT, TRUE);

	CString textPointDistStr = iniFile.readINI(_T("config.ini"), _T("ZVT"), _T("TEXTDIST"));
	if(textPointDistStr.IsEmpty())
		textPointDistStr = _T("1.0");
	SetDlgItemText(IDC_EDTDIST, textPointDistStr);

	return TRUE;
}

//����
void CZVT::OnBnClickedOk(){
	CAcUiDialog::OnOK();

	ads_point pt1, pt2;
	ads_name en;
	ads_real inputValue;

	int result;
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		CString insertValue, strMsg;
		strMsg.LoadStringW(IDS_INSERT_PT_SET); // "\n���� �� ����: "
		result = acedGetPoint(NULL, strMsg, pt1);
		if(result == RTNORM){
			strMsg.LoadStringW(IDS_CMD_SEL_Z_INPUT); // "\n���� ���� ���� �Ǵ� Z�� �Է�: "
			result = acedEntSel(strMsg, en, pt2);
			if(result == RTNORM){
				AcDbObjectId objId;
				if(acdbGetObjectId(objId, en) == Acad::eOk){
					AcDbEntityPointer pEnt(objId, AcDb::kForRead);
					if(pEnt.openStatus() == Acad::eOk){
						if(pEnt->isKindOf(AcDbText::desc())){
							AcDbText* pText = AcDbText::cast(pEnt);
							insertValue = pText->textString();
							pText->close();
						}
					}
					pEnt->close();
				}
			}else if(result == RTERROR){
				resbuf zValueStr;
				ads_getvar(_T("LASTPROMPT"), &zValueStr);
				insertValue.Format(_T("\n%s"), zValueStr.resval.rstring);
				strMsg.LoadStringW(IDS_CMD_SEL_Z_INPUT); // "\n���� ���� ���� �Ǵ� Z�� �Է�: "
				insertValue.Replace(strMsg, _T(""));
				strMsg.LoadStringW(IDS_INSERT_PT_SET); // "\n���� �� ����: "
				if(insertValue.CompareNoCase(strMsg) == 0)	continue;
			}
		}
		if(result == RTCAN || result == RTNONE)	break;

		CString tempStr;
		double textDist;
		GetDlgItemText(IDC_EDTDIST, tempStr);
		textDist = _ttof(tempStr);

		AcDbText* pText = new AcDbText();
		pText->setTextString(insertValue);

		AcGePoint3d textP;
		textP = asPnt3d(pt1);
		if(IsDlgButtonChecked(IDC_RADUP)){
			textP.y += textDist;
			pText->setHorizontalMode(AcDb::kTextCenter);
			pText->setVerticalMode(AcDb::kTextBottom);
		}
		else if(IsDlgButtonChecked(IDC_RADDOWN)){
			textP.y -= textDist;
			pText->setHorizontalMode(AcDb::kTextCenter);
			pText->setVerticalMode(AcDb::kTextTop);
		}
		else if(IsDlgButtonChecked(IDC_RADLEFT)){
			textP.x -= textDist;
			pText->setHorizontalMode(AcDb::kTextRight);
			pText->setVerticalMode(AcDb::kTextVertMid);
		}
		else if(IsDlgButtonChecked(IDC_RADRIGHT)){
			textP.x += textDist;
			pText->setHorizontalMode(AcDb::kTextLeft);
			pText->setVerticalMode(AcDb::kTextVertMid);
		}

		if(IsDlgButtonChecked(IDC_CHKZV))
			textP.z = _ttof(insertValue);

		pText->setAlignmentPoint(textP);

		CString curTextLayer;
		m_cbxTextLayer.GetLBText(m_cbxTextLayer.GetCurSel(), curTextLayer);
		pText->setLayer(curTextLayer);

		AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
		acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);		

		pText->setDatabaseDefaults();
		bTR->appendAcDbEntity(pText);
		pText->close();

		if(IsDlgButtonChecked(IDC_CHKCP)){
			CString curPointLayer;
			m_cbxPointLayer.GetLBText(m_cbxPointLayer.GetCurSel(), curPointLayer);
			AcDbPoint* pPoint = new AcDbPoint();
			pPoint->setPosition(AcGePoint3d(pt1[0], pt1[1], _ttof(insertValue)));			
			pPoint->setLayer(curPointLayer);
			pPoint->setDatabaseDefaults();
			bTR->appendAcDbEntity(pPoint);
			pPoint->close();
		}
		bTR->close();

		result = 1;
	}
	CompleteEditorCommand();

	//�ʱ�ȭ �ɼ� ����
	CString iniFileTextStr;

	if(IsDlgButtonChecked(IDC_CHKCP))	iniFile.writeINI(_T("config.ini"), _T("ZVT"), _T("POINTCREATE"), _T("TRUE"));
	else	iniFile.writeINI(_T("config.ini"), _T("ZVT"), _T("POINTCREATE"), _T("FALSE"));

	if(IsDlgButtonChecked(IDC_CHKZV))	iniFile.writeINI(_T("config.ini"), _T("ZVT"), _T("TEXTZVALUE"), _T("TRUE"));
	else	iniFile.writeINI(_T("config.ini"), _T("ZVT"), _T("TEXTZVALUE"), _T("FALSE"));

	GetDlgItemText(IDC_EDTTEXTHEIGHT, iniFileTextStr);
	iniFile.writeINI(_T("config.ini"), _T("ZVT"), _T("TEXTHEIGHT"), iniFileTextStr);

	GetDlgItemText(IDC_EDTTEXTWIDHT, iniFileTextStr);
	iniFile.writeINI(_T("config.ini"), _T("ZVT"), _T("TEXTWIDTH"), iniFileTextStr);

	if(IsDlgButtonChecked(IDC_RADUP))	iniFile.writeINI(_T("config.ini"), _T("ZVT"), _T("TEXTPOSITION"), _T("UP"));
	else if(IsDlgButtonChecked(IDC_RADDOWN))	iniFile.writeINI(_T("config.ini"), _T("ZVT"), _T("TEXTPOSITION"), _T("DOWN"));
	else if(IsDlgButtonChecked(IDC_RADLEFT))	iniFile.writeINI(_T("config.ini"), _T("ZVT"), _T("TEXTPOSITION"), _T("LEFT"));
	else if(IsDlgButtonChecked(IDC_RADRIGHT))	iniFile.writeINI(_T("config.ini"), _T("ZVT"), _T("TEXTPOSITION"), _T("RIGHT"));

	GetDlgItemText(IDC_EDTDIST, iniFileTextStr);
	iniFile.writeINI(_T("config.ini"), _T("ZVT"), _T("TEXTDIST"), iniFileTextStr);
}

//����
void CZVT::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

//�ؽ�Ʈ ���� ��ü ���ý�
void CZVT::OnBnClickedBtext(){	
	ads_name txt;
	ads_point pt;
	AcDbObjectId TextId;	

	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	CString strMsg;
	strMsg.LoadStringW(IDS_GET_TEXT_ENTITY); // "Ư���� ������ ���� ��ü ����: "
	if(acedEntSel(strMsg, txt, pt) == RTNORM)	CompleteEditorCommand();
	else{
		CancelEditorCommand();
		return;
	}

	if(acdbGetObjectId(TextId, txt) == Acad::eOk){
		AcDbEntityPointer pEnt(TextId, AcDb::kForRead);
		if(pEnt.openStatus()==Acad::eOk){			
			if(pEnt != NULL){
				if(pEnt->isKindOf(AcDbText::desc())){
					AcDbText *pText = AcDbText::cast(pEnt);
					double height, width;
					height = pText->height();
					width = pText->widthFactor();

					// ������ �ؽ�Ʈ�� ����
					CString heightC;
					heightC.Format(_T("%.2f"), height);

					// ������ �ؽ�Ʈ�� �ʺ�
					CString widthC;
					widthC.Format(_T("%.2f"), width);

					// ������ �ؽ�Ʈ�� ���̿� �ʺ� ��ȭ������ �ؽ�Ʈ ������ ����Ʈ �ڽ��� �Է��Ѵ�.
					SetDlgItemText(IDC_EDTTEXTHEIGHT, heightC);
					SetDlgItemText(IDC_EDTTEXTWIDHT, widthC);
					CString layer = pText->layer();
					// �ؽ�Ʈ�� �������� For������ ã�´�.
					for(int i = 0; i < m_cbxTextLayer.GetCount(); i++){
						CString curLayer;
						m_cbxTextLayer.GetLBText(i, curLayer);
						if(wcscmp(layer, curLayer) == 0){
							m_cbxTextLayer.SetCurSel(i);
							break;
						}
					}
					// �ؽ�Ʈ�� ��Ÿ���� For������ ã�´�.
					for(int i = 0; i < m_cbxTextStyle.GetCount(); i++){
						CString curStyle;
						m_cbxTextStyle.GetLBText(i, curStyle);
						if(findTextStyle(curStyle) == pText->textStyle()){
							m_cbxTextStyle.SetCurSel(i);
							break;
						}
					}
					pText->close();
				}
			}
		}		
	}
}

// �ؽ�Ʈ�� ��Ÿ���� �����ϴ� �ڵ�.
AcDbObjectId CZVT::findTextStyle(CString name){	
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

//�� ��ü ���ý�
void CZVT::OnBnClickedBpoint(){
	ads_name en;
	ads_point pt;
	AcDbObjectId objId;	

	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	CString strMsg;
	strMsg.LoadStringW(IDS_GET_LAYER_ENTITY); // "\n�������� ������ ��ü ����: "
	if(acedEntSel(strMsg, en, pt) == RTNORM)	CompleteEditorCommand();
	else{
		CancelEditorCommand();
		return;
	}

	if(acdbGetObjectId(objId, en) == Acad::eOk){
		AcDbEntityPointer pEnt(objId, AcDb::kForRead);
		if(pEnt.openStatus()==Acad::eOk){			
			if(pEnt != NULL){
				AcDbEntity* pObj = AcDbEntity::cast(pEnt);

				CString layer = pObj->layer();
				// �ؽ�Ʈ�� �������� For������ ã�´�.
				for(int i = 0; i < m_cbxPointLayer.GetCount(); i++){
					CString curLayer;
					m_cbxPointLayer.GetLBText(i, curLayer);
					if(wcscmp(layer, curLayer) == 0){
						m_cbxPointLayer.SetCurSel(i);
						break;
					}
				}
				pObj->close();
			}
		}
	}			
}

//���� ���� �Է½�
void CZVT::OnEnUpdateEdttextheight(){
	UpdateData(TRUE);	
	m_textHeightStr = CheckInputNumber(m_textHeightStr);	
	UpdateData(FALSE);

	m_textHeightCtrl.SetSel(0, -1);
	m_textHeightCtrl.SetSel(-1, -1);

	UpdateData(TRUE);
}

//���� �� �Է½�
void CZVT::OnEnUpdateEdttextwidht(){
	UpdateData(TRUE);	
	m_textWidthStr = CheckInputNumber(m_textWidthStr);	
	UpdateData(FALSE);

	m_textWidthCtrl.SetSel(0, -1);
	m_textWidthCtrl.SetSel(-1, -1);

	UpdateData(TRUE);
}

//���� �Ÿ� �Է½�
void CZVT::OnEnUpdateEdtdist(){
	UpdateData(TRUE);	
	m_textDistStr = CheckInputNumber(m_textDistStr);
	UpdateData(FALSE);

	m_textDistCtrl.SetSel(0, -1);
	m_textDistCtrl.SetSel(-1, -1);

	UpdateData(TRUE);
}

//���� �� ��(.) �Է� üũ
CString CZVT::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	for(int i = 0; i < str.GetLength(); i++){
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i,1);
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46)){
			str.Remove(*getAtChar);
		}
	}
	return str;
}

void CZVT::OnBnClickedZvtHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/ZVT.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
