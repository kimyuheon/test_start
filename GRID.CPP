// DCG.cpp : ±¸Çö ÆÄÀÏÀÔ´Ï´Ù.
//

#include "stdafx.h"
#include "DCG.h"
#include "afxdialogex.h"

#ifdef _ACAD
	CString LinetypeStr = L"acad.lin";
#elif _ZCAD
	CString LinetypeStr = L"zwcad.lin";
#elif _ZXYCAD
	CString LinetypeStr = L"ZYXcad.lin";
#endif

// CDCG ´ëÈ­ »óÀÚÀÔ´Ï´Ù.

IMPLEMENT_DYNAMIC(CDCG, CDialogEx)

CDCG::CDCG(CWnd* pParent /*=NULL*/)
: CDialogEx(CDCG::IDD, pParent),m_Radbtn(0)
{

}

CDCG::~CDCG()
{
}

void CDCG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DCG_RAD1, m_Radbtn);
	//Static
	DDX_Text(pDX, IDC_DCG_EDT_UP, m_GridUpStr);
	DDX_Text(pDX, IDC_DCG_EDT_DOWN, m_GridDownStr);
	DDX_Text(pDX, IDC_DCG_EDT_LEFT, m_GridLeftStr);
	DDX_Text(pDX, IDC_DCG_EDT_RIGHT, m_GridRightStr);
	DDX_Text(pDX, IDC_DCG_EDT_SCALE, m_ScaleStr);
	DDX_Text(pDX, IDC_DCG_EDT_MAINWIDTH, m_MainWidthStr);
	DDX_Text(pDX, IDC_DCG_EDT_MAINLINESCALE, m_MainLScaleStr);
	DDX_Text(pDX, IDC_DCG_EDT_SUBWIDTH, m_SubWidthStr);
	DDX_Text(pDX, IDC_DCG_EDT_SUBLINESCALE, m_SubLScaleStr);
	DDX_Text(pDX, IDC_DCG_EDT_TEXTHEIGHT, m_TextHeightStr);
	DDX_Text(pDX, IDC_DCG_EDT_TEXTWIDTH, m_TextWidthStr);
	//Ã¼Å©¹Ú½º
	DDX_Control(pDX, IDC_DCG_CHK_STANDARD, m_ChkStd_btn);
	DDX_Control(pDX, IDC_DCG_CHK_MAINCON, m_ChkMainCon_btn);
	//DDX_Control(pDX, IDC_DCG_CHK_SUBCON, m_ChkSubCon_btn);
	DDX_Control(pDX, IDC_DCG_CHK_DELGRID, m_ChkDelGrid_btn);
	DDX_Control(pDX, IDC_DCG_CHK_ELLEFT, m_ChkELLeft_btn);
	DDX_Control(pDX, IDC_DCG_CHK_ELRIGHT, m_ChkELRight_btn);
	//¹öÆ°,ÃßÃâ¹öÆ°
	DDX_Control(pDX, IDC_DCG_BTN_MAINCOLOR, m_MainColor_btn);
	DDX_Control(pDX, IDC_DCG_BTN_SUBCOLOR, m_SubColor_btn);
	DDX_Control(pDX, IDC_DCG_MAINEXTRACT, m_ExtMainSelect);
	DDX_Control(pDX, IDC_DCG_SUBEXTRACT, m_ExtSubSelect);
	DDX_Control(pDX, IDC_DCG_TEXTEXTRACT, m_ExtTextSizeSelect);
	DDX_Control(pDX, IDC_DCG_TEXTLAYEREXTRACT, m_ExtTextLayerSelect);
	DDX_Control(pDX, IDC_DCG_MAINPRINCOLOR, m_btn_MainColor);
	DDX_Control(pDX, IDC_DCG_SUBPRINCOLOR, m_btn_SubColor);
	DDX_Control(pDX, IDC_DCG_TEXTPRINCOLOR, m_btn_TextColor);
	//ÄÞº¸¹Ú½º
	DDX_Control(pDX, IDC_DCG_COMBO_MAINLAYER, m_CbxMainLayer);
	DDX_Control(pDX, IDC_DCG_COMBO_SUBLAYER, m_CbxSubLayer);
	DDX_Control(pDX, IDC_DCG_COMBO_TEXTSTYLE, m_CbxTextStyle);
	DDX_Control(pDX, IDC_DCG_COMBO_TEXTLAYER, m_CbxTextLayer);
	

}


BEGIN_MESSAGE_MAP(CDCG, CDialogEx)	
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_DCG_RAD1, &CDCG::OnBnClickedDcgRad1)
	ON_EN_UPDATE(IDC_DCG_EDT_UP, &CDCG::OnEnUpdateDcgEdtUp)
	ON_EN_UPDATE(IDC_DCG_EDT_DOWN, &CDCG::OnEnUpdateDcgEdtDown)
	ON_EN_UPDATE(IDC_DCG_EDT_LEFT, &CDCG::OnEnUpdateDcgEdtLeft)
	ON_EN_UPDATE(IDC_DCG_EDT_RIGHT, &CDCG::OnEnUpdateDcgEdtRight)
	ON_BN_CLICKED(IDC_DCG_CHK_STANDARD, &CDCG::OnBnClickedDcgChkStandard)
	ON_EN_UPDATE(IDC_DCG_EDT_SCALE, &CDCG::OnEnUpdateDcgEdtScale)
	ON_BN_CLICKED(IDOK, &CDCG::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDCG::OnBnClickedCancel)
	ON_EN_UPDATE(IDC_DCG_EDT_MAINWIDTH, &CDCG::OnEnUpdateDcgEdtMainwidth)
	ON_EN_UPDATE(IDC_DCG_EDT_MAINLINESCALE, &CDCG::OnEnUpdateDcgEdtMainlinescale)
	ON_BN_CLICKED(IDC_DCG_CHK_MAINCON, &CDCG::OnBnClickedDcgChkMaincon)
	ON_CBN_SELCHANGE(IDC_DCG_COMBO_MAINLAYER, &CDCG::OnCbnSelchangeDcgComboMainlayer)
	ON_BN_CLICKED(IDC_DCG_BTN_MAINCOLOR, &CDCG::OnBnClickedDcgBtnMaincolor)
	ON_BN_CLICKED(IDC_DCG_MAINEXTRACT, &CDCG::OnBnClickedDcgMainextract)
	ON_EN_UPDATE(IDC_DCG_EDT_SUBWIDTH, &CDCG::OnEnUpdateDcgEdtSubwidth)
	ON_EN_UPDATE(IDC_DCG_EDT_SUBLINESCALE, &CDCG::OnEnUpdateDcgEdtSublinescale)
//	ON_BN_CLICKED(IDC_DCG_CHK_SUBCON, &CDCG::OnBnClickedDcgChkSubcon)
	ON_CBN_SELCHANGE(IDC_DCG_COMBO_SUBLAYER, &CDCG::OnCbnSelchangeDcgComboSublayer)
	ON_BN_CLICKED(IDC_DCG_BTN_SUBCOLOR, &CDCG::OnBnClickedDcgBtnSubcolor)
	ON_BN_CLICKED(IDC_DCG_SUBEXTRACT, &CDCG::OnBnClickedDcgSubextract)
	ON_BN_CLICKED(IDC_DCG_CHK_DELGRID, &CDCG::OnBnClickedDcgChkDelgrid)
	ON_BN_CLICKED(IDC_DCG_CHK_ELLEFT, &CDCG::OnBnClickedDcgChkElleft)
	ON_BN_CLICKED(IDC_DCG_CHK_ELRIGHT, &CDCG::OnBnClickedDcgChkElright)
	ON_EN_UPDATE(IDC_DCG_EDT_TEXTHEIGHT, &CDCG::OnEnUpdateDcgEdtTextheight)
	ON_EN_UPDATE(IDC_DCG_EDT_TEXTWIDTH, &CDCG::OnEnUpdateDcgEdtTextwidth)
	ON_BN_CLICKED(IDC_DCG_TEXTEXTRACT, &CDCG::OnBnClickedDcgTextextract)
	ON_CBN_SELCHANGE(IDC_DCG_COMBO_TEXTSTYLE, &CDCG::OnCbnSelchangeDcgComboTextstyle)
	ON_CBN_SELCHANGE(IDC_DCG_COMBO_TEXTLAYER, &CDCG::OnCbnSelchangeDcgComboTextlayer)
	ON_BN_CLICKED(IDC_DCG_TEXTLAYEREXTRACT, &CDCG::OnBnClickedDcgTextlayerextract)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DCG_RAD2, &CDCG::OnBnClickedDcgRad2)
	ON_BN_CLICKED(IDC_DCG_RAD3, &CDCG::OnBnClickedDcgRad3)
	ON_BN_CLICKED(IDC_DCG_BTN_TEXTCOLOR2, &CDCG::OnBnClickedDcgBtnTextcolor2)
	//ON_BN_CLICKED(IDC_DCG_WRITE, &CDCG::OnBnClickedDcgWrite)
	//ON_BN_CLICKED(IDC_DCG_CANCEL, &CDCG::OnBnClickedDcgCancel)
	ON_BN_CLICKED(ID_DCG_HELP, &CDCG::OnBnClickedDcgHelp)
END_MESSAGE_MAP()


// CDCG ¸Þ½ÃÁö Ã³¸®±âÀÔ´Ï´Ù.
LRESULT CDCG::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

BOOL CDCG::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//AfxMessageBox(_T("1"));
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon,FALSE);

	//LoadLineType(_T("Dot2"), _T("zwcad.lin"));

	//·¹ÀÌ¾î »ý¼º
	CreateLayer(_T("CC-GRID-MAJR"),3);
	CreateLayer(_T("CC-GRID-MINR"),1);
	CreateLayer(_T("CC-GRID-TEXT"),2);

	//ÃßÃâ¹öÆ°
	m_ExtMainSelect.AutoLoad();
	m_ExtSubSelect.AutoLoad();
	m_ExtTextLayerSelect.AutoLoad();
	m_ExtTextSizeSelect.AutoLoad();

	//±Û²Ã ºÒ·¯¿À±â
	AcDbTextStyleTable *pTextTbl = NULL;
	AcDbTextStyleTablePointer pTextPt(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbTextStyleTableIterator *pTextIter;
	pTextPt->newIterator(pTextIter);

	AcDbObjectId TextId;
	for(pTextIter->start(); !pTextIter->done(); pTextIter->step())
	{
		pTextIter->getRecordId(TextId);
		AcDbTextStyleTableRecordPointer pTextRcd(TextId, AcDb::kForRead);
		TCHAR *TextName;
		pTextRcd->getName(TextName);
		m_CbxTextStyle.AddString(TextName);
	}
	delete pTextIter;
	//pTextTbl->close();
	if (pTextTbl != NULL) pTextTbl->close();

	//·¹ÀÌ¾î ºÒ·¯¿À±â
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPt(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerIter;
	pLayerPt->newIterator(pLayerIter);

	AcDbObjectId LayerId;
	for(pLayerIter->start(); !pLayerIter->done(); pLayerIter->step())
	{
		pLayerIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerRcd(LayerId, AcDb::kForRead);
		TCHAR *Layername;
		pLayerRcd->getName(Layername);
		m_CbxMainLayer.AddString(Layername);
		m_CbxSubLayer.AddString(Layername);
		m_CbxTextLayer.AddString(Layername);
	}
	delete pLayerIter;
	//pLayerTbl->close();
	if (pLayerTbl != NULL) pLayerTbl->close();

	//Grid ÀÛ¼º ¹æ¹ý ¶óµð¿À¹öÆ°
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("JobMode"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		OnBnClickedDcgRad1();
	else if(tempStr.CompareNoCase(_T("2")) == 0)
		OnBnClickedDcgRad2();
	else if(tempStr.CompareNoCase(_T("3")) == 0)
		OnBnClickedDcgRad3();

	//Edt »ó
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("GridUp"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_DCG_EDT_UP, _T("20"));
	else
		SetDlgItemText(IDC_DCG_EDT_UP, tempStr);
	//Edt ÇÏ
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("GridDown"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_DCG_EDT_DOWN, _T("20"));
	else
		SetDlgItemText(IDC_DCG_EDT_DOWN, tempStr);
	//Edt ÁÂ
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("GridLeft"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_DCG_EDT_LEFT, _T("50"));
	else
		SetDlgItemText(IDC_DCG_EDT_LEFT, tempStr);
	//Edt ¿ì
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("GridRight"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_DCG_EDT_RIGHT, _T("50"));
	else
		SetDlgItemText(IDC_DCG_EDT_RIGHT, tempStr);

	//Ã¼Å©¹Ú½º ¸ðµç¼³Á¤ Ç¥ÁØÈ­¿¡ ¸ÂÃã
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("ChkStd"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){
		m_ChkStd_btn.SetCheck(TRUE);
		bool_ChkStd = TRUE;
		OnBnClickedDcgChkStandard();
	}else{
		m_ChkStd_btn.SetCheck(FALSE);
		bool_ChkStd = false;
		OnBnClickedDcgChkStandard();
	}

	//Edt È¾´Ü¸éµµ ÃàÃ´
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("Scale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_DCG_EDT_SCALE, _T("1000"));
	else
		SetDlgItemText(IDC_DCG_EDT_SCALE, tempStr);



	///////////////////////////////////////////ÁÖ Grid ¿É¼Ç////////////////////////////////////////////

	//Edt ÁÖ Grid °£°Ý
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("MainWidth"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_DCG_EDT_MAINWIDTH, _T("5.000"));
	else
		SetDlgItemText(IDC_DCG_EDT_MAINWIDTH, tempStr);

	//Edt ÁÖ Grid ¼± ÃàÃ´
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("MainLScale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_DCG_EDT_MAINLINESCALE, _T("1.000"));
	else
		SetDlgItemText(IDC_DCG_EDT_MAINLINESCALE, tempStr);

	//Ã¼Å©¹Ú½º Continuous ¼±À¸·Î ±×¸®±â
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("ChkMaincon"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_ChkMainCon_btn.SetCheck(FALSE);
		bool_ChkMaincon = false;
		OnBnClickedDcgChkMaincon();
	}else{
		m_ChkMainCon_btn.SetCheck(TRUE);
		bool_ChkMaincon = true;
		OnBnClickedDcgChkMaincon();
	}

	//¹öÆ° ÁÖ Grid »ö»ó ¹öÆ°
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("MainColor"));
	if(tempStr.CompareNoCase(_T("")) == 0){
		m_MainColor = 255;
		//ColorSetting(m_Color);
	}else{
		m_MainColor = _ttoi(tempStr);
		ColorSetting(m_MainColor,1);
	}	

	//ÄÞº¸¹Ú½º ÁÖ Grid ·¹ÀÌ¾î string
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxMainLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_CbxMainLayer_Str = _T("CC-GRID-MAJR");
	}else{
		m_CbxMainLayer_Str = tempStr;
	}
	//ÄÞº¸¹Ú½º ÁÖ Grid ·¹ÀÌ¾î int
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxMainLayerInt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_CbxMainLayerInt = m_CbxMainLayer.FindString(-1, m_CbxMainLayer_Str);
		m_CbxMainLayer.SetCurSel(m_CbxMainLayerInt);		
		m_MainColor = getLayerColor(m_CbxMainLayer_Str);
		ColorSetting(m_MainColor, 1);
	}else{
		m_CbxMainLayerInt = m_CbxMainLayer.FindString(-1, m_CbxMainLayer_Str);
		if(m_CbxMainLayerInt == -1)m_CbxMainLayerInt = 0;
		//m_CbxMainLayerInt = _ttoi(m_CbxMainLayerInt_Str);
		m_CbxMainLayer.SetCurSel(m_CbxMainLayerInt);
		if(m_CbxMainLayer.FindString(-1, m_CbxMainLayer_Str) == -1){
			m_CbxMainLayer.SetCurSel(0);
			m_CbxMainLayerInt = 0;
			m_CbxMainLayer.GetLBText(0, m_CbxMainLayer_Str);
			m_MainColor = getLayerColor(m_CbxMainLayer_Str);
			ColorSetting(m_MainColor, 1);
		}else{
			m_CbxMainLayer.SetCurSel(m_CbxMainLayerInt);
			m_CbxMainLayer.GetLBText(m_CbxMainLayerInt,m_CbxMainLayer_Str);
			m_MainColor = getLayerColor(m_CbxMainLayer_Str);
			ColorSetting(m_MainColor, 1);
		}
	}	

	
	///////////////////////////////////////////º¸Á¶ Grid ¿É¼Ç////////////////////////////////////////////

	//Edt º¸Á¶ Grid °£°Ý
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("SubWidth"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_DCG_EDT_SUBWIDTH, _T("1.000"));
	else
		SetDlgItemText(IDC_DCG_EDT_SUBWIDTH, tempStr);

	//Edt º¸Á¶ Grid ¼± ÃàÃ´
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("SubLScale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_DCG_EDT_SUBLINESCALE, _T("1.000"));
	else
		SetDlgItemText(IDC_DCG_EDT_SUBLINESCALE, tempStr);

	//ÄÞº¸¹Ú½º º¸Á¶ Grid ·¹ÀÌ¾î string
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxSubLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_CbxSubLayer_Str = _T("CC-GRID-MINR");
	}else{
		m_CbxSubLayer_Str = tempStr;
	}
	//ÄÞº¸¹Ú½º º¸Á¶Grid ·¹ÀÌ¾î int
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxSubLayerInt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_CbxSubLayerInt = m_CbxMainLayer.FindString(-1, m_CbxSubLayer_Str);
		m_CbxSubLayer.SetCurSel(m_CbxSubLayerInt);			
		m_SubColor = getLayerColor(m_CbxSubLayer_Str);
		ColorSetting(m_SubColor, 2);
	}else{
		m_CbxSubLayerInt = m_CbxMainLayer.FindString(-1, m_CbxSubLayer_Str);
		if(m_CbxSubLayerInt == -1)m_CbxSubLayerInt = 0;
		//m_CbxSubLayerInt = _ttoi(m_CbxSubLayerInt_Str);
		m_CbxSubLayer.SetCurSel(m_CbxSubLayerInt);
		if(m_CbxSubLayer.FindString(-1, m_CbxSubLayer_Str) == -1){
			m_CbxSubLayer.SetCurSel(0);
			m_CbxSubLayerInt = 0;
			m_CbxSubLayer.GetLBText(0, m_CbxSubLayer_Str);
			m_SubColor = getLayerColor(m_CbxSubLayer_Str);
			ColorSetting(m_SubColor, 2);
		}else{
			m_CbxSubLayer.SetCurSel(m_CbxSubLayerInt);
			m_CbxSubLayer.GetLBText(m_CbxSubLayerInt,m_CbxSubLayer_Str);
			m_SubColor = getLayerColor(m_CbxSubLayer_Str);
			ColorSetting(m_SubColor, 2);
		}
	}	

	//¹öÆ° º¸Á¶Grid »ö»ó ¹öÆ°	
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("SubColor"));
	if(tempStr.CompareNoCase(_T("")) == 0){
		m_SubColor = 255;
		//ColorSetting(m_Color);
	}else{
		m_SubColor = _ttoi(tempStr);
		ColorSetting(m_SubColor,2);
	}	

	///////////////////////////////////////////¹®ÀÚ ¿É¼Ç////////////////////////////////////////////

	//Ã¼Å©¹Ú½º ¹®ÀÚ ¿µ¿ª º¸Á¶ Gird »èÁ¦
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("ChkDelGrid"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){
		m_ChkDelGrid_btn.SetCheck(TRUE);
		bool_ChkDelGrid = true;
		OnBnClickedDcgChkDelgrid();
	}else{
		m_ChkDelGrid_btn.SetCheck(FALSE);
		bool_ChkDelGrid = false;
		OnBnClickedDcgChkDelgrid();
	}

	//Ã¼Å©¹Ú½º ELÇ¥±â ÁÂ
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("ChkELLeft"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){
		m_ChkELLeft_btn.SetCheck(TRUE);
		bool_ChkELLeft = true;
		OnBnClickedDcgChkElleft();
	}else{
		m_ChkELLeft_btn.SetCheck(FALSE);
		bool_ChkELLeft = false;
		OnBnClickedDcgChkElleft();
	}

	//Ã¼Å©¹Ú½º ELÇ¥±â ¿ì
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("ChkELRight"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){
		m_ChkELRight_btn.SetCheck(TRUE);
		bool_ChkELRight = true;
		OnBnClickedDcgChkElright();
	}else{
		m_ChkELRight_btn.SetCheck(FALSE);
		bool_ChkELRight = false;
		OnBnClickedDcgChkElright();
	}

	//Edt ¹®ÀÚÅ©±â
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("TextHeight"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_DCG_EDT_TEXTHEIGHT, _T("0.500"));
	else
		SetDlgItemText(IDC_DCG_EDT_TEXTHEIGHT, tempStr);

	//Edt Æø
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("TextWidth"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_DCG_EDT_TEXTWIDTH, _T("1.000"));
	else
		SetDlgItemText(IDC_DCG_EDT_TEXTWIDTH, tempStr);

	//ÄÞº¸¹Ú½º ±Û²Ã string
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxTextStyle"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_CbxTextStyle_Str = _T("Standard");
	}else{
		m_CbxTextStyle_Str = tempStr;
	}

	//ÄÞº¸¹Ú½º ±Û²Ã int
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxTextStyleInt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_CbxTextStyleInt = m_CbxTextStyle.FindString(-1, m_CbxTextStyle_Str);
		if(m_CbxTextStyleInt == -1)m_CbxTextStyleInt = 0;
		m_CbxTextStyle.SetCurSel(m_CbxTextStyleInt);
		//m_CbxTextStyle.SetCurSel(0);
		//m_CbxTextStyleInt = _ttoi(tempStr);
		//m_CbxTextStyle.GetLBText(0, m_CbxTextStyle_Str);		
	}else{
		//m_CbxTextStyleInt_Str = tempStr;
		//m_CbxTextStyleInt = _ttoi(m_CbxTextStyleInt_Str);
		m_CbxTextStyleInt = m_CbxTextStyle.FindString(-1, m_CbxTextStyle_Str);
		if(m_CbxTextStyleInt == -1)m_CbxTextStyleInt = 0;
		m_CbxTextStyle.SetCurSel(m_CbxTextStyleInt);
		if(m_CbxTextStyle.FindString(-1, m_CbxTextStyle_Str) == -1){
			m_CbxTextStyle.SetCurSel(0);
			m_CbxTextStyleInt = 0;
			m_CbxTextStyle.GetLBText(0, m_CbxTextStyle_Str);
		}else{
			m_CbxTextStyle.SetCurSel(m_CbxTextStyleInt);
			m_CbxTextStyle.GetLBText(m_CbxTextStyleInt,m_CbxTextStyle_Str);
		}
	}	

	//ÄÞº¸¹Ú½º ¹®ÀÚ ·¹ÀÌ¾î string
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxTextLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_CbxTextLayer_Str = _T("CC-GRID-TEXT");
	}else{
		m_CbxTextLayer_Str = tempStr;
	}
	//ÄÞº¸¹Ú½º ¹®ÀÚ ·¹ÀÌ¾î int
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxTextLayerInt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_CbxTextLayerInt = m_CbxTextLayer.FindString(-1, m_CbxTextLayer_Str);
		m_CbxTextLayer.SetCurSel(m_CbxTextLayerInt);		
		m_TextColor = getLayerColor(m_CbxTextLayer_Str);
		ColorSetting(m_TextColor, 3);
	}else{
		m_CbxTextLayerInt = m_CbxTextLayer.FindString(-1, m_CbxTextLayer_Str);
		if(m_CbxTextLayerInt == -1)m_CbxTextLayerInt = 0;
		//m_CbxTextLayerInt = _ttoi(m_CbxTextLayerInt_Str);
		m_CbxTextLayer.SetCurSel(m_CbxTextLayerInt);
		if(m_CbxTextLayer.FindString(-1, m_CbxTextLayer_Str) == -1){
			m_CbxTextLayer.SetCurSel(0);
			m_CbxTextLayerInt = 0;
			m_CbxTextLayer.GetLBText(0, m_CbxTextLayer_Str);
			m_TextColor = getLayerColor(m_CbxTextLayer_Str);
			ColorSetting(m_TextColor, 3);
		}else{
			m_CbxTextLayer.SetCurSel(m_CbxTextLayerInt);
			m_CbxTextLayer.GetLBText(m_CbxTextLayerInt,m_CbxTextLayer_Str);
			m_TextColor = getLayerColor(m_CbxTextLayer_Str);
			ColorSetting(m_TextColor, 3);
		}
	}	

	//¹öÆ° º¸Á¶Grid »ö»ó ¹öÆ°	
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("TextColor"));
	if(tempStr.CompareNoCase(_T("")) == 0){
		m_TextColor = 255;
		//ColorSetting(m_Color);
	}else{
		m_TextColor = _ttoi(tempStr);
		ColorSetting(m_TextColor,3);
	}	

	return TRUE;  
}


//
//void CDCG::OnBnClickedDcgWrite()
void CDCG::OnBnClickedOk()
{
	UpdateData(true);	

	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("JobMode"), JobMode_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("GridUp"), m_GridUpStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("GridDown"), m_GridDownStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("GridLeft"), m_GridLeftStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("GridRight"), m_GridRightStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ChkStd"), m_ChkStd_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("Scale"), m_ScaleStr);

	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("MainWidth"), m_MainWidthStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("MainLScale"), m_MainLScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ChkMainCon"), m_ChkMainCon_Str);
	m_CbxMainLayerInt = m_CbxMainLayer.GetCurSel();
	m_CbxMainLayerInt_Str.Format(_T("%d"), m_CbxMainLayerInt);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxMainLayer"), m_CbxMainLayer_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxMainLayerInt"), m_CbxMainLayerInt_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("MainColor"), m_MainColorStr);

	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("SubWidth"), m_SubWidthStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("SubLScale"), m_SubLScaleStr);
	m_CbxSubLayerInt = m_CbxSubLayer.GetCurSel();
	m_CbxSubLayerInt_Str.Format(_T("%d"), m_CbxSubLayerInt);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxSubLayer"), m_CbxSubLayer_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxSubLayerInt"), m_CbxSubLayerInt_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("SubColor"), m_SubColorStr);

	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ChkDelGrid"), m_ChkDelGrid_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ChkELLeft"), m_ChkELLeft_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ChkELRight"), m_ChkELRight_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("TextHeight"), m_TextHeightStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("TextWidth"), m_TextWidthStr);
	m_CbxTextStyleInt = m_CbxTextStyle.GetCurSel();
	m_CbxTextStyleInt_Str.Format(_T("%d"), m_CbxTextStyleInt);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextStyle"), m_CbxTextStyle_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextStyleInt"), m_CbxTextStyleInt_Str);
	m_CbxTextLayerInt = m_CbxTextLayer.GetCurSel();
	m_CbxTextLayerInt_Str.Format(_T("%d"), m_CbxTextLayerInt);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextLayer"), m_CbxTextLayer_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextLayerInt"), m_CbxTextLayerInt_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("TextColor"), m_TextColorStr);

	acDocManager->sendStringToExecute(acDocManager->curDocument(), _T("(DrawCrossGrid)\n"), false, true);

	UpdateData(false);
}

void CDCG::OnBnClickedCancel()
{
	//AfxMessageBox(_T("1"));
	DestroyWindow();
}
//¸í·É¾î ½ÇÇà
void CDCG::DrawMain()
{
	acDocManager->lockDocument(acDocManager->curDocument());

	LoadLineType(_T("Dot2"), (ACHAR*)(LPCTSTR)LinetypeStr);
	//Grid ÀÛ¼º ¹æ¹ý ¶óµð¿À¹öÆ°
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("JobMode"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		JobMode = 1;
	else if(tempStr.CompareNoCase(_T("2")) == 0)
		JobMode = 2;
	else if(tempStr.CompareNoCase(_T("3")) == 0)
		JobMode = 3;

	//Edt »ó
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("GridUp"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_GridUp = 20;
	else
		m_GridUp = _ttoi(tempStr);
	//Edt ÇÏ
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("GridDown"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_GridDown = 20;
	else
		m_GridDown = _ttoi(tempStr);
	//Edt ÁÂ
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("GridLeft"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_GridLeft = 50;
	else
		m_GridLeft = _ttoi(tempStr);
	//Edt ¿ì
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("GridRight"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_GridRight = 50;
	else
		m_GridRight = _ttoi(tempStr);

	//Ã¼Å©¹Ú½º ¸ðµç¼³Á¤ Ç¥ÁØÈ­¿¡ ¸ÂÃã
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("ChkStd"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){		
		bool_ChkStd = true;
	}else{		
		bool_ChkStd = false;		
	}

	//Edt È¾´Ü¸éµµ ÃàÃ´
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("Scale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_Scale = 1000;
	else
		m_Scale = _tstof(tempStr);

	///////////////////////////////////////////ÁÖ Grid ¿É¼Ç////////////////////////////////////////////

	//Edt ÁÖ Grid °£°Ý
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("MainWidth"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_MainWidth = 5.000;
	else
		m_MainWidth = _tstof(tempStr);

	//Edt ÁÖ Grid ¼± ÃàÃ´
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("MainLScale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_MainLScale = 1.000;
	else
		m_MainLScale = _tstof(tempStr);

	//Ã¼Å©¹Ú½º Continuous ¼±À¸·Î ±×¸®±â
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("ChkMaincon"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){	
		bool_ChkMaincon = false;		
	}else{		
		bool_ChkMaincon = true;
	}

	//ÄÞº¸¹Ú½º ÁÖ Grid ·¹ÀÌ¾î string
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxMainLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_CbxMainLayer_Str = _T("CC-GRID-MAJR");
	}else{
		m_CbxMainLayer_Str = tempStr;
	}
	//ÄÞº¸¹Ú½º ÁÖ Grid ·¹ÀÌ¾î int
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxMainLayerInt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_CbxMainLayerInt = m_CbxMainLayer.FindString(-1, m_CbxMainLayer_Str);		
		m_MainColor = getLayerColor(m_CbxMainLayer_Str);		
	}else{
		m_CbxMainLayerInt_Str = tempStr;
		m_CbxMainLayerInt = _ttoi(m_CbxMainLayerInt_Str);		
		if(m_CbxMainLayer.FindString(-1, m_CbxMainLayer_Str) == -1){
			m_CbxMainLayer.SetCurSel(0);
			m_CbxMainLayerInt = 0;			
			m_MainColor = getLayerColor(m_CbxMainLayer_Str);			
		}else{	
			m_CbxMainLayerInt = m_CbxMainLayer.FindString(-1, m_CbxMainLayer_Str);
			m_MainColor = getLayerColor(m_CbxMainLayer_Str);			
		}
	}	
	//¹öÆ° ÁÖ Grid ·¹ÀÌ¾î »ö»ó
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("MainColor"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_MainColor = 3;
	}else{
		m_MainColor = _ttoi(tempStr);
	}

	///////////////////////////////////////////º¸Á¶ Grid ¿É¼Ç////////////////////////////////////////////

	//Edt º¸Á¶ Grid °£°Ý
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("SubWidth"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_SubWidth = 1.000;
	else
		m_SubWidth = _tstof(tempStr);

	//Edt º¸Á¶ Grid ¼± ÃàÃ´
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("SubLScale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_SubLScale = 1.000;
	else
		m_SubLScale = _tstof(tempStr);

	//ÄÞº¸¹Ú½º º¸Á¶ Grid ·¹ÀÌ¾î string
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxSubLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_CbxSubLayer_Str = _T("CC-GRID-MINR");
	}else{
		m_CbxSubLayer_Str = tempStr;
	}
	//ÄÞº¸¹Ú½º º¸Á¶Grid ·¹ÀÌ¾î int
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxSubLayerInt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_CbxSubLayerInt = m_CbxMainLayer.FindString(-1, m_CbxSubLayer_Str);		
		m_SubColor = getLayerColor(m_CbxSubLayer_Str);		
	}else{
		m_CbxSubLayerInt_Str = tempStr;
		m_CbxSubLayerInt = _ttoi(m_CbxSubLayerInt_Str);		
		if(m_CbxSubLayer.FindString(-1, m_CbxSubLayer_Str) == -1){	
			m_CbxSubLayerInt = 0;					
			m_SubColor = getLayerColor(m_CbxSubLayer_Str);			
		}else{	
			m_CbxSubLayerInt = m_CbxSubLayer.FindString(-1, m_CbxSubLayer_Str);
			m_SubColor = getLayerColor(m_CbxSubLayer_Str);			
		}
	}	
	//¹öÆ° º¸Á¶ Grid ·¹ÀÌ¾î »ö»ó
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("SubColor"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_SubColor = 1;
	}else{
		m_SubColor = _ttoi(tempStr);
	}

	///////////////////////////////////////////¹®ÀÚ ¿É¼Ç////////////////////////////////////////////

	//Ã¼Å©¹Ú½º ¹®ÀÚ ¿µ¿ª º¸Á¶ Gird »èÁ¦
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("ChkDelGrid"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){		
		bool_ChkDelGrid = true;
	}else{
		bool_ChkDelGrid = false;
	}

	//Ã¼Å©¹Ú½º ELÇ¥±â ÁÂ
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("ChkELLeft"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){
		bool_ChkELLeft = true;
	}else{
		bool_ChkELLeft = false;
	}

	//Ã¼Å©¹Ú½º ELÇ¥±â ¿ì
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("ChkELRight"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){
		bool_ChkELRight = true;
	}else{
		bool_ChkELRight = false;
	}

	//Edt ¹®ÀÚÅ©±â
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("TextHeight"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_TextHeight = 0.500;
	else
		m_TextHeight = _tstof(tempStr);

	//Edt Æø
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("TextWidth"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_TextWidth = 1.000;
	else
		m_TextWidth = _tstof(tempStr);

	//ÄÞº¸¹Ú½º ±Û²Ã string
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxTextStyle"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_CbxTextStyle_Str = _T("Standard");
	}else{
		m_CbxTextStyle_Str = tempStr;
	}

	//ÄÞº¸¹Ú½º ¹®ÀÚ ·¹ÀÌ¾î string
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxTextLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_CbxTextLayer_Str = _T("CC-GRID-TEXT");
	}else{
		m_CbxTextLayer_Str = tempStr;
	}
	//ÄÞº¸¹Ú½º ¹®ÀÚ ·¹ÀÌ¾î int
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("CbxTextLayerInt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_CbxTextLayerInt = m_CbxTextLayer.FindString(-1, m_CbxTextLayer_Str);
		m_TextColor = getLayerColor(m_CbxTextLayer_Str);
	}else{
		m_CbxTextLayerInt_Str = tempStr;
		m_CbxTextLayerInt = _ttoi(m_CbxTextLayerInt_Str);
		if(m_CbxTextLayer.FindString(-1, m_CbxTextLayer_Str) == -1){
			m_CbxTextLayerInt = 0;
			m_TextColor = getLayerColor(m_CbxTextLayer_Str);
		}else{
			m_TextColor = getLayerColor(m_CbxTextLayer_Str);
		}
	}	
	//¹öÆ° ¹®ÀÚ ·¹ÀÌ¾î »ö»ó
	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("TextColor"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_TextColor = 2;
	}else{
		m_TextColor = _ttoi(tempStr);
	}

	tempStr = iniFile.readINI(_T("config.ini"), _T("DCG"), _T("ELValue"));
	if(tempStr.CompareNoCase(_T("")) == 0 ){
		m_ELValue = 0;
	}else{
		m_ELValue = _tstof(tempStr);
	}

	::SetFocus(adsw_acadDocWnd());

	AcGePoint3d Polypt1,Polypt2,Polypt3,Polypt4,pPt,Polyept,Polyspt,Cntrpt,ELpt, PriorCntrpt;
	AcGePoint3dArray Recarr,polyArr,DrawArr, polyObjArr;	
	ads_name en,ads_rec;
	ads_point pt;
	AcDbObjectId recId;
	CString strMsg;

	if(JobMode == 1){//ÀÛ¼º¹æ¹ý ¿µ¿ª
		strMsg.LoadStringW(IDS_CROSS_CORNER_SEL); // "\nÈ¾´Ü Grid ÇÑÂÊ ¸ð¼­¸® ¼±ÅÃ : "
		if(acedGetPoint(0, strMsg,asDblArray(Polypt1)) != RTNORM)
			return;
		strMsg.LoadStringW(IDS_CROSS_OPPOSITE_SEL); // "\nÈ¾´Ü Grid ¹Ý´ëÂÊ ¸ð¼­¸® ¼±ÅÃ : "
		if(acedGetCorner(asDblArray(Polypt1), strMsg, asDblArray(Polypt3)) != RTNORM)
			return;

		if(Polypt1.y < Polypt3.y){
			Polypt2.x = Polypt3.x; Polypt2.y = Polypt1.y;
			Polypt4.x = Polypt1.x; Polypt4.y = Polypt3.y;
		}else{
			Polypt2.x = Polypt1.x; Polypt2.y = Polypt3.y;
			Polypt4.x = Polypt3.x; Polypt4.y = Polypt1.y;
		}
		
		Recarr = PolyptArr(Polypt1,Polypt2,Polypt3,Polypt4,ads_angle(asDblArray(Polypt1),asDblArray(Polypt3)));
		Polypt1 = Recarr[0];Polypt2 = Recarr[1];Polypt3 = Recarr[2];Polypt4 = Recarr[3];
		//Recarr.append(Polypt1);
		//Recarr.append(Polypt1);Recarr.append(Polypt2);Recarr.append(Polypt3);Recarr.append(Polypt4);Recarr.append(Polypt1);

		CreatePLine(Recarr,0,_T("bylayer"),1,_T("0"),0);		
		acdbEntLast(ads_rec);
		acdbGetObjectId(recId,ads_rec);

		strMsg.LoadStringW(IDS_CROSS_CENTER_SEL); // "\nÈ¾´Ü Áß½ÉÁ¡ ¼±ÅÃ : "
		if(acedGetPoint(0, strMsg, asDblArray(Cntrpt))!=RTNORM){
			acdbEntDel(ads_rec);
			return;
		}
	}else if(JobMode == 2){//ÀÛ¼º¹æ¹ý °´Ã¼ ¼±ÅÃ
		while(true){
			strMsg.LoadStringW(IDS_CLOSED_PLINE_SEL); // "\nÆóÇÕµÈ Æú¸®¼± ¼±ÅÃ : "
			int stat = acedEntSel(strMsg, en, pt);
			if(stat == RTCAN || stat == RTNONE){
				return;
			}else if(stat == RTERROR){
				resbuf zValueStr;
				ads_getvar(_T("ERRNO"), &zValueStr);
				if(zValueStr.resval.rint == 52)
					return;
			}else if(stat == RTNORM){
				AcGePoint3dArray interptArr;
				AcDbObjectId objId, LayerId;
				TCHAR* LayerChar;
				acdbGetObjectId(objId, en);
				AcDbEntityPointer pEnt(objId, AcDb::kForRead);
				if(pEnt.openStatus() == Acad::eOk){
					if(pEnt->isKindOf(AcDbPolyline::desc())){
						AcDbPolyline *pPline = AcDbPolyline::cast(pEnt);
						pPline->getEndPoint(Polyept);
						pPline->getStartPoint(Polyspt);
						pPline->upgradeOpen();
						int m_VertNum = pPline->numVerts();										
						//pPline->intersectWith(pPline,AcDb::kOnBothOperands, interptArr);
						AcGePoint3d pt;
						for(int i=0; i<m_VertNum; i++){
							pPline->getPointAt(i, pt);
							polyObjArr.append(pt);
						}
						if(Polyspt!=Polyept){
							polyObjArr.append(Polyspt);
						}
						if(m_VertNum == 4 && Polyspt == Polyept)
							polyObjArr.append(Polyspt);

						m_PolyObjID = CreatePLine(polyObjArr,0,_T("BYLAYER"), m_MainLScale, _T("0"), 0);
						
						pPline->erase();
						pPline->close();
						break;
						
					}
				}
			}
		}
		strMsg.LoadStringW(IDS_CROSS_CENTER_SEL); // "\nÈ¾´Ü Áß½ÉÁ¡ ¼±ÅÃ : "
		if(acedGetPoint(0, strMsg,asDblArray(Cntrpt))!=RTNORM)
			return;

	}else if(JobMode == 3){// ÀÛ¼º¹æ¹ý °íÁ¤Å©±â
		strMsg.LoadStringW(IDS_CROSS_CENTER_SEL); // "\nÈ¾´Ü Áß½ÉÁ¡ ¼±ÅÃ : "
		if(acedGetPoint(0, strMsg,asDblArray(Cntrpt))!=RTNORM)
			return;
		PriorCntrpt = Cntrpt;//±âÁ¸ È¾´Ü Áß½ÉÁ¡
	}
	TCHAR kWord[20];
	CString ELstr, RTERROR_str, inString;
	ads_name ens;
	while(true){
		//acedInitGet(0, _T("0 1 2 3 4 5 6 7 8 9 -"));
		strMsg.LoadStringW(IDS_EL_INPUT); // "±âÁØÁ¡ EL ¼±ÅÃ ¶Ç´Â °ª ÀÔ·Â(%.6f) : "
		ELstr.Format(strMsg, m_ELValue);
		
		int stat2 = acedEntSel(ELstr, ens, asDblArray(ELpt));
		if(stat2 == RTCAN || stat2 == RTNONE){
			acdbEntDel(ads_rec);
			return;
		}else if(stat2 == RTKWORD){
			resbuf zValueStr,zValueStr2;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
			RTERROR_str.Replace(ELstr, _T(""));
			ads_getvar(_T("ERRNO"), &zValueStr2);//acedEntSel NULLÀ» ±¸ÇÒ ¶§ »ç¿ë
			if(acedGetInput(kWord) ==RTNORM){
				m_ELValue = _tstof(kWord);
				m_ELValueStr.Format(_T("%s"), kWord);
				iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ELValue"), m_ELValueStr);
			}			
			break;
			
		}else if(stat2 == RTERROR){
			resbuf zValueStr, zValueStr2;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			ads_getvar(_T("ERRNO"), &zValueStr2);
			RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
			RTERROR_str.Replace(ELstr, _T(""));
			if(zValueStr2.resval.rint == 52)//52  = NULL(ERRNO)
			{	// °ªÀ» ±âÀÔÇÏÁö ¾Ê°Å³ª °´Ã¼¸¦ ¼±ÅÃÇÏÁö ¾ÊÀ» °æ¿ì, EL°ªÀ» Ãß°¡ÇÏÁö ¾ÊÀ½
				m_ELValueStr.Format(_T("%f"),m_ELValue);	
				iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ELValue"), m_ELValueStr);				
				break;
			}
			m_ELValue = _tstof(RTERROR_str);
			iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ELValue"), RTERROR_str);			
			break;
		}else if(stat2 == RTNORM){
			if(acedGetInput(kWord)!=RTNORM)
				return;
			CString kWord_Str;
			kWord_Str.Format(_T("%s"),kWord);
			if(kWord_Str.CompareNoCase(_T("")) != 0){
				m_ELValue = _tstof(kWord_Str);
				m_ELValueStr = kWord_Str;
				iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ELValue"), m_ELValueStr);
				break;
			}
				
			AcDbObjectId objId;
			acdbGetObjectId(objId, ens);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				AcDbText* pText = AcDbText::cast(pEnt);
				//inString = pText->textString();
				inString = CheckInputNumber(pText->textString());
				pText->close();
			}else{
				continue;
			}
			pEnt->close();
			m_ELValue = _tstof(inString);
			m_ELValueStr = inString;
			iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ELValue"), m_ELValueStr);
			//acdbEntDel(ads_rec);
			break;
		}
		
	}//while end
	
	if(abs(m_ELValue) < m_MainWidth){
		Cntrpt.y = Cntrpt.y - m_ELValue;
		m_ELValue = 0;
	}else if(abs(m_ELValue) >= m_MainWidth){
		Cntrpt.y = Cntrpt.y - (fmod(m_ELValue, m_MainWidth) * (1000/m_Scale));
		m_ELValue = m_ELValue - fmod(m_ELValue, m_MainWidth);
	}
	
	/////////////////////////////////////////////////////////////////////////
	ads_name ads_gridName;
	if(JobMode == 1){
		//CreateGrid(Recarr,Cntrpt, m_ELValue);
		CreateObjGrid(recId,Cntrpt,m_ELValue);
		acdbGetAdsName(ads_gridName, recId);
		acdbEntDel(ads_gridName);
	}else if(JobMode == 2){//°´Ã¼ ¼±ÅÃ
		CreateObjGrid(m_PolyObjID,Cntrpt,m_ELValue);
	}else if(JobMode == 3){//°íÁ¤Å©±â
		Polypt1.x = PriorCntrpt.x - m_GridLeft; Polypt1.y = PriorCntrpt.y - m_GridDown;
		Polypt2.x = PriorCntrpt.x + m_GridRight; Polypt2.y = PriorCntrpt.y - m_GridDown;
		Polypt3.x = PriorCntrpt.x + m_GridRight; Polypt3.y = PriorCntrpt.y + m_GridDown;
		Polypt4.x = PriorCntrpt.x - m_GridLeft; Polypt4.y = PriorCntrpt.y + m_GridDown;
		Recarr.append(Polypt1);Recarr.append(Polypt2);Recarr.append(Polypt3);Recarr.append(Polypt4);Recarr.append(Polypt1);
		AcDbObjectId objgridID = CreatePLine(Recarr,0,_T("bylayer"),1,_T("0"),7);
		CreateObjGrid(objgridID, Cntrpt, m_ELValue);

		acdbGetAdsName(ads_gridName, objgridID);
		acdbEntDel(ads_gridName);
		//CreateGrid(Recarr,Cntrpt, m_ELValue);
		//CreateGrid()
	}
	Recarr.removeAll();
	acDocManager->unlockDocument(acDocManager->curDocument());	//µµ¸é Àá±Ý
}

//¶óµð¿À¹öÆ° Grid ÀÛ¼º¹æ¹ý
void CDCG::OnBnClickedDcgRad1()
{
	UpdateData(true);
	JobMode = 1;
	JobMode_Str.Format(_T("%d"),JobMode);
	m_Radbtn = 0;
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("JobMode"), JobMode_Str);
	GetDlgItem(IDC_DCG_EDT_UP)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_EDT_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DCG_EDT_LEFT)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_EDT_RIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC1)->EnableWindow(FALSE);GetDlgItem(IDC_STATIC2)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC3)->EnableWindow(FALSE);GetDlgItem(IDC_STATIC4)->EnableWindow(FALSE);

	UpdateData(false);
}

void CDCG::OnBnClickedDcgRad2()
{
	UpdateData(true);
	JobMode = 2;
	JobMode_Str.Format(_T("%d"),JobMode);
	m_Radbtn = 1;
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("JobMode"), JobMode_Str);
	GetDlgItem(IDC_DCG_EDT_UP)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_EDT_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DCG_EDT_LEFT)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_EDT_RIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC1)->EnableWindow(FALSE);GetDlgItem(IDC_STATIC2)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC3)->EnableWindow(FALSE);GetDlgItem(IDC_STATIC4)->EnableWindow(FALSE);
	UpdateData(false);
}

void CDCG::OnBnClickedDcgRad3()
{
	UpdateData(true);
	JobMode = 3;
	JobMode_Str.Format(_T("%d"),JobMode);
	m_Radbtn = 2;
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("JobMode"), JobMode_Str);
	GetDlgItem(IDC_DCG_EDT_UP)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_EDT_DOWN)->EnableWindow(TRUE);
	GetDlgItem(IDC_DCG_EDT_LEFT)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_EDT_RIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC1)->EnableWindow(TRUE);GetDlgItem(IDC_STATIC2)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC3)->EnableWindow(TRUE);GetDlgItem(IDC_STATIC4)->EnableWindow(TRUE);
	UpdateData(false);
}

//Edt »ó
void CDCG::OnEnUpdateDcgEdtUp()
{
	UpdateData(true);
	m_GridUp = _ttoi(m_GridUpStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("GridUp"), m_GridUpStr);
	UpdateData(false);
}

//Edt ÇÏ
void CDCG::OnEnUpdateDcgEdtDown()
{
	UpdateData(true);
	m_GridDown = _ttoi(m_GridDownStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("GridDown"), m_GridDownStr);
	UpdateData(false);
}

//Edt ÁÂ
void CDCG::OnEnUpdateDcgEdtLeft()
{
	UpdateData(true);
	m_GridLeft = _ttoi(m_GridLeftStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("GridLeft"), m_GridLeftStr);
	UpdateData(false);
}

//Edt ¿ì
void CDCG::OnEnUpdateDcgEdtRight()
{
	UpdateData(true);
	m_GridRight = _ttoi(m_GridRightStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("GridRight"), m_GridRightStr);
	UpdateData(false);
}

//Ã¼Å©¹Ú½º ¸ðµç ¼³Á¤ Ç¥ÁØÈ­¿¡ ¸ÂÃã
void CDCG::OnBnClickedDcgChkStandard()
{
	UpdateData(true);
	bool_ChkStd = IsDlgButtonChecked(IDC_DCG_CHK_STANDARD);
	if(bool_ChkStd == 1){
		m_ChkStd_btn.SetCheck(true);
		m_ChkStd_Str = _T("1");
		SetDlgItemText(IDC_DCG_EDT_MAINWIDTH, _T("5.000"));SetDlgItemText(IDC_DCG_EDT_MAINLINESCALE, _T("1.000"));
		m_ChkMainCon_btn.SetCheck(false);
		m_CbxMainLayerInt = m_CbxMainLayer.FindString(-1, _T("CC-GRID-MAJR"));
		m_CbxMainLayer.SetCurSel(m_CbxMainLayerInt);
		m_MainColor = getLayerColor(_T("CC-GRID-MAJR"));
		ColorSetting(m_MainColor, 1);
		SetDlgItemText(IDC_DCG_EDT_SUBWIDTH, _T("1.000"));SetDlgItemText(IDC_DCG_EDT_SUBLINESCALE, _T("1.000"));
		m_CbxSubLayerInt = m_CbxSubLayer.FindString(-1, _T("CC-GRID-MINR"));
		m_CbxSubLayer.SetCurSel(m_CbxSubLayerInt);
		m_SubColor = getLayerColor(_T("CC-GRID-MINR"));
		ColorSetting(m_SubColor, 2);
		m_ChkDelGrid_btn.SetCheck(true); m_ChkELLeft_btn.SetCheck(true); m_ChkELRight_btn.SetCheck(true);
		SetDlgItemText(IDC_DCG_EDT_TEXTHEIGHT, _T("0.500"));SetDlgItemText(IDC_DCG_EDT_TEXTWIDTH, _T("1.000"));
		m_CbxTextLayerInt = m_CbxTextLayer.FindString(-1, _T("CC-GRID-TEXT"));
		m_CbxTextLayer.SetCurSel(m_CbxTextLayerInt);
		m_TextColor = getLayerColor(_T("CC-GRID-TEXT"));
		ColorSetting(m_TextColor, 3);
		GetDlgItem(IDC_DCG_EDT_MAINWIDTH)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_EDT_MAINLINESCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCG_CHK_MAINCON)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_COMBO_MAINLAYER)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCG_BTN_MAINCOLOR)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_MAINEXTRACT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCG_EDT_SUBWIDTH)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_EDT_SUBLINESCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCG_COMBO_SUBLAYER)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCG_BTN_SUBCOLOR)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_SUBEXTRACT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCG_CHK_DELGRID)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_CHK_ELLEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCG_CHK_ELRIGHT)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_EDT_TEXTHEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCG_EDT_TEXTWIDTH)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_TEXTEXTRACT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCG_COMBO_TEXTLAYER)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_COMBO_TEXTSTYLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCG_TEXTLAYEREXTRACT)->EnableWindow(FALSE);GetDlgItem(IDC_DCG_BTN_TEXTCOLOR2)->EnableWindow(FALSE);

	}else{
		m_ChkStd_btn.SetCheck(false);
		m_ChkStd_Str = _T("0");
		GetDlgItem(IDC_DCG_EDT_MAINWIDTH)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_EDT_MAINLINESCALE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCG_CHK_MAINCON)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_COMBO_MAINLAYER)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCG_BTN_MAINCOLOR)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_MAINEXTRACT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCG_EDT_SUBWIDTH)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_EDT_SUBLINESCALE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCG_COMBO_SUBLAYER)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCG_BTN_SUBCOLOR)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_SUBEXTRACT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCG_CHK_DELGRID)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_CHK_ELLEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCG_CHK_ELRIGHT)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_EDT_TEXTHEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCG_EDT_TEXTWIDTH)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_TEXTEXTRACT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCG_COMBO_TEXTLAYER)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_COMBO_TEXTSTYLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCG_TEXTLAYEREXTRACT)->EnableWindow(TRUE);GetDlgItem(IDC_DCG_BTN_TEXTCOLOR2)->EnableWindow(TRUE);
	}
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ChkStd"), m_ChkStd_Str);
	UpdateData(false);
}

//Edt È¾´Ü¸éµµ ÃàÃ´
void CDCG::OnEnUpdateDcgEdtScale()
{
	UpdateData(true);
	m_Scale = _tstof(m_ScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("Scale"), m_ScaleStr);
	UpdateData(false);
}

//Edt ÁÖ Grid °£°Ý
void CDCG::OnEnUpdateDcgEdtMainwidth()
{
	UpdateData(true);
	m_MainWidth = _tstof(m_MainWidthStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("MainWidth"), m_MainWidthStr);
	UpdateData(false);
}

//Edt ÁÖ Grid ¼± ÃàÃ´
void CDCG::OnEnUpdateDcgEdtMainlinescale()
{
	UpdateData(true);
	m_MainLScale = _tstof(m_MainLScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("MainLScale"), m_MainLScaleStr);
	UpdateData(false);
}

//Ã¼Å©¹Ú½º ÁÖ Grid Continuous ¼±À¸·Î ±×¸®±â
void CDCG::OnBnClickedDcgChkMaincon()
{
	UpdateData(true);
	bool_ChkMaincon = IsDlgButtonChecked(IDC_DCG_CHK_MAINCON);
	if(bool_ChkMaincon == 1){
		m_ChkMainCon_btn.SetCheck(true);
		m_ChkMainCon_Str = _T("1");
	}else{
		m_ChkMainCon_btn.SetCheck(false);
		m_ChkMainCon_Str = _T("0");
	}
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ChkMainCon"), m_ChkMainCon_Str);
	UpdateData(false);
}

//ÄÞº¸¹Ú½º ÁÖ Grid ·¹ÀÌ¾î
void CDCG::OnCbnSelchangeDcgComboMainlayer()
{
	UpdateData(true);	
	m_CbxMainLayerInt = m_CbxMainLayer.GetCurSel();		
	m_CbxMainLayer.SetCurSel(m_CbxMainLayerInt);
	m_CbxMainLayerInt_Str.Format(_T("%d"), m_CbxMainLayerInt);
	m_CbxMainLayer.GetLBText(m_CbxMainLayerInt, m_CbxMainLayer_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxMainLayer"), m_CbxMainLayer_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxMainLayerInt"), m_CbxMainLayerInt_Str);
	int color = getLayerColor(m_CbxMainLayer_Str);
	ColorSetting(color, 1);
	UpdateData(false);
}

//¹öÆ° ÁÖ Grid »ö»ó
void CDCG::OnBnClickedDcgBtnMaincolor()
{
	UpdateData(true);
	acedSetColorDialog(m_MainColor, true, 256);
	ColorSetting(m_MainColor, 1);
	m_MainColorStr.Format(_T("%d"),m_MainColor);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("MainColor"), m_MainColorStr);
	UpdateData(false);
}

//ÃßÃâ¹öÆ° ÁÖ Grid »ö»ó
void CDCG::OnBnClickedDcgMainextract()
{
	UpdateData(true);
	ads_name en;
	ads_point pt;
	AcCmColor Color;
	CString strMsg;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		::SetFocus(adsw_acadDocWnd());
		strMsg.LoadStringW(IDS_SELECT_ENTITY); // "\n°´Ã¼ ¼±ÅÃ : \n"
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
			TCHAR* LayerChar;
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbEntity::desc())){
					AcDbEntity* pEntity = AcDbEntity::cast(pEnt);				
					LayerId = pEntity->layerId();
					m_CbxMainLayer_Str = pEntity->layer();

					AcDbLayerTableRecordPointer pLayer(LayerId,AcDb::kForRead);
					Color = pLayer->color();
					m_MainColor = Color.colorIndex();
					pLayer->close();
					pEntity->close();

					for(int i =0; i < m_CbxMainLayer.GetCount(); i++){
						if(m_CbxMainLayer.FindString(i, m_CbxMainLayer_Str) == -1)
							m_CbxMainLayer.AddString(m_CbxMainLayer_Str);
					}
					m_CbxMainLayerInt = m_CbxMainLayer.FindString(0, m_CbxMainLayer_Str);
					m_CbxMainLayer.SetCurSel(m_CbxMainLayerInt);
					m_CbxMainLayerInt_Str.Format(_T("%d"), m_CbxMainLayerInt);

					m_MainColorStr.Format(_T("%d"), m_MainColor);
					iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("MainColor"), m_MainColorStr);
					iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxMainLayer"), m_CbxMainLayer_Str);
					iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxMainLayerInt"), m_CbxMainLayerInt_Str);
					ColorSetting(m_MainColor,1);
					pEnt->close();
					break;
				}else{
					strMsg.LoadStringW(IDS_CHECK_LAYER_COLOR); // "\n**·¹ÀÌ¾î ¹×, »ö»óÀ» °¡Á®¿Ã ¼ö ¾ø½À´Ï´Ù.**"
					acutPrintf(strMsg);
					pEnt->close();
					continue;
				}
			}
		}
	}

	UpdateData(false);
}

//Edt º¸Á¶ Grid °£°Ý
void CDCG::OnEnUpdateDcgEdtSubwidth()
{
	UpdateData(true);
	m_SubWidth = _tstof(m_SubWidthStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("SubWidth"), m_SubWidthStr);
	UpdateData(false);
}

//Edt º¸Á¶ Grid ¼± ÃàÃ´
void CDCG::OnEnUpdateDcgEdtSublinescale()
{
	UpdateData(true);
	m_SubLScale = _tstof(m_SubLScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("SubLScale"), m_SubLScaleStr);
	UpdateData(false);
}

//ÄÞº¸¹Ú½º º¸Á¶ Grid ·¹ÀÌ¾î
void CDCG::OnCbnSelchangeDcgComboSublayer()
{
	UpdateData(true);
	m_CbxSubLayerInt = m_CbxSubLayer.GetCurSel();	
	m_CbxSubLayer.SetCurSel(m_CbxSubLayerInt);
	m_CbxSubLayerInt_Str.Format(_T("%d"), m_CbxSubLayerInt);
	m_CbxSubLayer.GetLBText(m_CbxSubLayerInt, m_CbxSubLayer_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxSubLayer"), m_CbxSubLayer_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxSubLayerInt"), m_CbxSubLayerInt_Str);
	int color = getLayerColor(m_CbxSubLayer_Str);
	ColorSetting(color, 2);
	UpdateData(false);
}

//¹öÆ° º¸Á¶ Grid »ö»ó
void CDCG::OnBnClickedDcgBtnSubcolor()
{
	UpdateData(true);
	acedSetColorDialog(m_SubColor, true, 256);
	ColorSetting(m_SubColor, 2);
	UpdateData(false);
}

//Ãß­„¹öÆ° º¸Á¶ Grid »ö»ó
void CDCG::OnBnClickedDcgSubextract()
{
	UpdateData(true);
	ads_name en;
	ads_point pt;
	AcCmColor Color;
	CString strMsg;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		::SetFocus(adsw_acadDocWnd());
		strMsg.LoadStringW(IDS_SELECT_ENTITY); // "\n°´Ã¼ ¼±ÅÃ : \n"
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
			TCHAR* LayerChar;
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbEntity::desc())){
					AcDbEntity* pEntity = AcDbEntity::cast(pEnt);				
					LayerId = pEntity->layerId();
					m_CbxSubLayer_Str = pEntity->layer();

					AcDbLayerTableRecordPointer pLayer(LayerId,AcDb::kForRead);
					Color = pLayer->color();
					m_SubColor = Color.colorIndex();
					pLayer->close();
					pEntity->close();

					for(int i =0; i < m_CbxMainLayer.GetCount(); i++){
						if(m_CbxMainLayer.FindString(i, m_CbxMainLayer_Str) == -1)
							m_CbxMainLayer.AddString(m_CbxMainLayer_Str);
					}
					m_CbxSubLayerInt = m_CbxSubLayer.FindString(0, m_CbxSubLayer_Str);
					m_CbxSubLayer.SetCurSel(m_CbxSubLayerInt);
					m_CbxSubLayerInt_Str.Format(_T("%d"), m_CbxSubLayerInt);

					m_SubColorStr.Format(_T("%d"), m_SubColor);
					iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("SubColor"), m_SubColorStr);
					iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxSubLayer"), m_CbxSubLayer_Str);
					iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxSubLayerInt"), m_CbxSubLayerInt_Str);
					ColorSetting(m_SubColor,2);
					pEnt->close();
					break;
				}else{
					strMsg.LoadStringW(IDS_CHECK_LAYER_COLOR); // "\n**·¹ÀÌ¾î ¹×, »ö»óÀ» °¡Á®¿Ã ¼ö ¾ø½À´Ï´Ù.**"
					acutPrintf(strMsg);
					pEnt->close();
					continue;
				}
			}
		}
	}

	UpdateData(false);
}

//Ã¼Å©¹Ú½º ¹®ÀÚ ¿µ¿ª º¸Á¶ Grid »èÁ¦
void CDCG::OnBnClickedDcgChkDelgrid()
{
	UpdateData(true);
	bool_ChkDelGrid = IsDlgButtonChecked(IDC_DCG_CHK_DELGRID);
	if(bool_ChkDelGrid == 1){
		m_ChkDelGrid_btn.SetCheck(true);
		m_ChkDelGrid_Str = _T("1");
	}else{
		m_ChkDelGrid_btn.SetCheck(false);
		m_ChkDelGrid_Str = _T("0");
	}
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ChkDelGrid"), m_ChkDelGrid_Str);
	UpdateData(false);
}

//Ã¼Å©¹Ú½º EL Ç¥±â ÁÂ
void CDCG::OnBnClickedDcgChkElleft()
{
	UpdateData(true);
	bool_ChkELLeft = IsDlgButtonChecked(IDC_DCG_CHK_ELLEFT);
	if(bool_ChkELLeft == 1){
		m_ChkELLeft_btn.SetCheck(true);
		m_ChkELLeft_Str = _T("1");
	}else{
		if(bool_ChkELRight == false){
			CString strMsg;
			strMsg.LoadStringW(IDS_EL_CHECK_MSG); // "EL Ç¥±â´Â 1°¡Áö ÀÌ»ó ¼±ÅÃµÇ¾î¾ß ÇÕ´Ï´Ù."
			AfxMessageBox(strMsg);
			m_ChkELLeft_btn.SetCheck(true);
			m_ChkELLeft_Str = _T("1");
		}else{
			m_ChkELLeft_btn.SetCheck(false);
			m_ChkELLeft_Str = _T("0");
		}
		
	}
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ChkELLeft"), m_ChkELLeft_Str);
	UpdateData(false);
}

//Ã¼Å©¹Ú½º EL Ç¥±â ¿ì
void CDCG::OnBnClickedDcgChkElright()
{
	UpdateData(true);
	bool_ChkELRight = IsDlgButtonChecked(IDC_DCG_CHK_ELRIGHT);
	if(bool_ChkELRight == 1){
		m_ChkELRight_btn.SetCheck(true);
		m_ChkELRight_Str = _T("1");
	}else{
		if(bool_ChkELLeft == false){
			CString strMsg;
			strMsg.LoadStringW(IDS_EL_CHECK_MSG); // "EL Ç¥±â´Â 1°¡Áö ÀÌ»ó ¼±ÅÃµÇ¾î¾ß ÇÕ´Ï´Ù."
			AfxMessageBox(strMsg);
			m_ChkELRight_btn.SetCheck(true);
			m_ChkELRight_Str = _T("1");
		}else{
			m_ChkELRight_btn.SetCheck(false);
			m_ChkELRight_Str = _T("0");
		}
		
	}
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("ChkELRight"), m_ChkELRight_Str);
	UpdateData(false);
}

//Edt ¹®ÀÚÅ©±â
void CDCG::OnEnUpdateDcgEdtTextheight()
{
	UpdateData(true);
	m_TextHeight = _tstof(m_TextHeightStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("TextHeight"), m_TextHeightStr);
	UpdateData(false);
}

//Edt ¹®ÀÚ Æø
void CDCG::OnEnUpdateDcgEdtTextwidth()
{
	UpdateData(true);
	m_TextWidth = _tstof(m_TextWidthStr);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("TextWidth"), m_TextWidthStr);
	UpdateData(false);
}

//ÃßÃâ¹öÆ° ¹®ÀÚ
void CDCG::OnBnClickedDcgTextextract()
{
	UpdateData(true);
	ads_name en;
	ads_point pt;
	AcCmColor Color;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		CString strMsg;
		::SetFocus(adsw_acadDocWnd());
		strMsg.LoadStringW(IDS_STYLE_HEIGHT_TEXT); // "\n±Û²Ã°ú ³ôÀÌ¸¦ °¡Á®¿Ã ¹®ÀÚ ¼±ÅÃ : \n"
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
			AcDbObjectId objId, TextId;
			TCHAR* Name;
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbText::desc())){
					AcDbText* pText = AcDbText::cast(pEnt);				
					m_TextHeight = pText->height();
					m_TextHeightStr.Format(_T("%.3f"),m_TextHeight);
					SetDlgItemText(IDC_DCG_EDT_TEXTHEIGHT, m_TextHeightStr);
					m_TextWidth = pText->widthFactor();
					m_TextWidthStr.Format(_T("%.3f"),m_TextWidth);
					SetDlgItemText(IDC_DCG_EDT_TEXTWIDTH, m_TextWidthStr);
					TextId = pText->textStyle();
					AcDbTextStyleTableRecord *pTSTR = NULL;
					acdbOpenObject(pTSTR, TextId, AcDb::kForRead);
					pTSTR->getName(Name);
					pTSTR->close();
					pText->close();

					for(int i =0; i < m_CbxTextStyle.GetCount(); i++){
						if(m_CbxTextStyle.FindString(i, Name) == -1)
							m_CbxTextStyle.AddString(Name);
					}
					m_CbxTextStyleInt = m_CbxTextStyle.FindString(0, Name);
					m_CbxTextStyle.SetCurSel(m_CbxTextStyleInt);

					m_CbxTextStyle_Str.Format(_T("%s"), Name);
					m_CbxTextStyleInt_Str.Format(_T("%d"), m_CbxTextStyleInt);
				
					iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextStyle"), m_CbxTextStyle_Str);
					iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextStyleInt"), m_CbxTextStyleInt_Str);					
					pEnt->close();
					break;
				}else{
					strMsg.LoadStringW(IDS_CHECK_LAYER_COLOR); // "\n**·¹ÀÌ¾î ¹×, »ö»óÀ» °¡Á®¿Ã ¼ö ¾ø½À´Ï´Ù.**"
					acutPrintf(strMsg);
					pEnt->close();
					continue;
				}
			}
		}
	}
	UpdateData(false);
}

//ÄÞº¸¹Ú½º ±Û²Ã ¹öÆ°
void CDCG::OnCbnSelchangeDcgComboTextstyle()
{
	UpdateData(true);
	m_CbxTextStyleInt = m_CbxTextStyle.GetCurSel();
	if(m_CbxTextStyleInt == -1)m_CbxTextStyleInt = 0;
	m_CbxTextStyle.SetCurSel(m_CbxTextStyleInt);
	m_CbxTextStyleInt_Str.Format(_T("%d"), m_CbxTextStyleInt);
	m_CbxTextStyle.GetLBText(m_CbxTextStyleInt, m_CbxTextStyle_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextStyle"), m_CbxTextStyle_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextStyleInt"), m_CbxTextStyleInt_Str);
	UpdateData(false);
}

//ÄÞº¸¹Ú½º ¹®ÀÚ ·¹ÀÌ¾î
void CDCG::OnCbnSelchangeDcgComboTextlayer()
{
	UpdateData(true);
	
	m_CbxTextLayerInt = m_CbxTextLayer.GetCurSel();	
	m_CbxTextLayer.SetCurSel(m_CbxTextLayerInt);
	m_CbxTextLayerInt_Str.Format(_T("%d"), m_CbxTextLayerInt);
	m_CbxTextLayer.GetLBText(m_CbxTextLayerInt, m_CbxTextLayer_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextLayer"), m_CbxTextLayer_Str);
	iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextLayerInt"), m_CbxTextLayerInt_Str);
	int color = getLayerColor(m_CbxTextLayer_Str);
	ColorSetting(color, 3);
	UpdateData(false);
}

//ÃßÃâ¹öÆ° ·¹ÀÌ¾î ¹öÆ°
void CDCG::OnBnClickedDcgTextlayerextract()
{
	UpdateData(true);
	ads_name en;
	ads_point pt;
	AcCmColor Color;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		CString strMsg;
		::SetFocus(adsw_acadDocWnd());
		strMsg.LoadStringW(IDS_SELECT_ENTITY); // _T("\n°´Ã¼ ¼±ÅÃ : \n")
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
			TCHAR* LayerChar;
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbEntity::desc())){
					AcDbEntity* pEntity = AcDbEntity::cast(pEnt);				
					LayerId = pEntity->layerId();
					m_CbxTextLayer_Str = pEntity->layer();

					AcDbLayerTableRecordPointer pLayer(LayerId,AcDb::kForRead);
					Color = pLayer->color();
					m_TextColor = Color.colorIndex();
					pLayer->close();
					pEntity->close();

					for(int i =0; i < m_CbxTextLayer.GetCount(); i++){
						if(m_CbxTextLayer.FindString(i, m_CbxTextLayer_Str) == -1)
							m_CbxTextLayer.AddString(m_CbxTextLayer_Str);
					}
					m_CbxTextLayerInt = m_CbxTextLayer.FindString(0, m_CbxTextLayer_Str);
					m_CbxTextLayer.SetCurSel(m_CbxTextLayerInt);
					m_CbxTextLayerInt_Str.Format(_T("%d"), m_CbxTextLayerInt);

					m_TextColorStr.Format(_T("%d"), m_TextColor);
					iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("TextColor"), m_TextColorStr);
					iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextLayer"), m_CbxTextLayer_Str);
					iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("CbxTextLayerInt"), m_CbxTextLayerInt_Str);
					ColorSetting(m_TextColor,1);
					pEnt->close();
					break;
				}else{
					strMsg.LoadStringW(IDS_CHECK_LAYER_COLOR); // "\n**·¹ÀÌ¾î ¹×, »ö»óÀ» °¡Á®¿Ã ¼ö ¾ø½À´Ï´Ù.**"
					acutPrintf(strMsg);
					pEnt->close();
					continue;
				}
			}
		}
	}

	UpdateData(false);
}

//¹öÆ° ÅØ½ºÆ®·¹ÀÌ¾î »ö»ó¹öÆ°
void CDCG::OnBnClickedDcgBtnTextcolor2()
{
	UpdateData(true);
	acedSetColorDialog(m_TextColor, true, 256);
	ColorSetting(m_TextColor, 3);
	UpdateData(false);
}

//·¹ÀÌ¾î »ý¼º
bool CDCG::CreateLayer(CString Layer, int color){
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

int CDCG::getLayerColor(CString LayerName){
	//·¹ÀÌ¾î ºÒ·¯¿À±â
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);
	int colorindex=-1;

	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		pLayerTblRcd->getName(name);
		if(LayerName == name){//·¹ÀÌ¾î°¡ ÀÖÀ» °æ¿ì »ö»óÃ£±â

			AcDbLayerTableRecordPointer layerTableRec(LayerId,  AcDb::kForRead);

			if (layerTableRec.openStatus() == Acad::eOk)
			{

				AcCmColor color = layerTableRec->color();
				colorindex = color.colorIndex();
				break;
			}
			layerTableRec->close();
		}
	}
	delete pLayerTblIter;
	if (pLayerTbl != NULL) delete pLayerTbl;

	return colorindex;
}

//»ö»ó Ãâ·Â
void CDCG::ColorSetting(int newColorIndex, int Mode)//Mode = 1 ÁÖ ±×¸®µå »ö»ó , Mode = 2 º¸Á¶ ±×¸®µå »ö»ó, Mode = 3 ÅØ½ºÆ® ·¹ÀÌ¾î »ö»ó
{
	UpdateData(TRUE);
	CString ColorName;
	CRect prect;
	if(Mode == 1)
		m_btn_MainColor.GetClientRect (& prect); // get the zone
	else if(Mode == 2)
		m_btn_SubColor.GetClientRect (& prect); // get the zone
	else if(Mode == 3)
		m_btn_TextColor.GetClientRect (& prect); // get the zone
	

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
	if(Mode == 1){
		FillRect (m_btn_MainColor.GetDC () -> GetSafeHdc (), & prect, CBrush (RGB (r,g,b))); // fill color 
		((CButton*) GetDlgItem(IDC_DCG_MAINCOLORSTATIC))->SetWindowText(ColorName);
		m_MainColorStr.Format(_T("%d"),newColorIndex);		
		iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("MainColor"), m_MainColorStr);
	}else if(Mode == 2){
		FillRect (m_btn_SubColor.GetDC () -> GetSafeHdc (), & prect, CBrush (RGB (r,g,b))); // fill color 
		((CButton*) GetDlgItem(IDC_DCG_SUBCOLORSTATIC))->SetWindowText(ColorName);
		m_SubColorStr.Format(_T("%d"),newColorIndex);
		iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("SubColor"), m_SubColorStr);
	}else if(Mode == 3){
		FillRect (m_btn_TextColor.GetDC () -> GetSafeHdc (), & prect, CBrush (RGB (r,g,b))); // fill color 
		((CButton*) GetDlgItem(IDC_DCG_TEXTCOLORSTATIC))->SetWindowText(ColorName);
		m_TextColorStr.Format(_T("%d"),newColorIndex);
		iniFile.writeINI(_T("config.ini"), _T("DCG"), _T("TextColor"), m_TextColorStr);
	}

	
	UpdateData(FALSE);

}

void CDCG::OnPaint()
{
	UpdateData(TRUE);
	CPaintDC dc(this); // device context for painting
	m_MainColor = _ttoi(m_MainColorStr);
	ColorSetting(m_MainColor,1);
	m_SubColor = _ttoi(m_SubColorStr);
	ColorSetting(m_SubColor,2);
	m_TextColor = _ttoi(m_TextColorStr);
	ColorSetting(m_TextColor,3);
	UpdateData(FALSE);
}

extern CDCG *pDCG;
void CDCG::PostNcDestroy()
{
	delete this;
	pDCG = NULL;

	//CDialogEx::PostNcDestroy();
}

AcDbObjectId CDCG::CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer, double Color){

	Acad::ErrorStatus es;
	AcDbObjectId pLineObjId, returnId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	acDocManager->curDocument()->database();
	if(pDb == NULL)return NULL;

	

	AcDbPolyline* pPline = new AcDbPolyline();
	for(int i =0; i<ptArray.length(); i++){
		pPline->addVertexAt(i, AcGePoint2d(ptArray[i].x, ptArray[i].y));
	}
	pPline->setConstantWidth(LineWidth);
	pPline->setLinetype(LineType);
	pPline->setLinetypeScale(LineTypeScale);
	pPline->setLayer(Layer);
	pPline->setColorIndex(Color);
	
	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(pLineObjId, pPline);			
			pBTR->close();
			pPline->close();
		}else
			return NULL;
		pBT->close();
	}else
		return NULL;
	pPline->close();

	return pLineObjId;
}

//¼ýÀÚ ¹× Á¡(.) ÀÔ·Â Ã¼Å©
CString CDCG::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	CString strNum, strRe = _T("");
	int k = 0;
	for(int i = 0; i < str.GetLength(); i++){
		strNum = str.Mid(i,1);	//for¹®À» µ¹¸é »ç¶óÁø¹®ÀÚ¶§¹®¿¡ ´ÙÀ½¹®ÀÚ¸¦ Ã£À¸·Á¸é Mid¿¡¼­ 0¹øÂ°¸¦ Ã£¾Æ¾ß ÇÔ
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		if(((ascii >= 48 && ascii <= 57) || ascii == 46 || ascii == 46)){
			strRe.Append(getAtChar);
		}
	}
	return strRe;

} 

void CDCG::Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result)
{
	result.set(base.x + dist*cos(angle), base.y + dist*sin(angle), base.z);
}

////°¢µµ¸¦ ¶óµð¾È °ªÀ¸·Î º¯È¯ÇÏ¿© ¸®ÅÏÇÑ´Ù.
double CDCG::DTR(double angle)
{
	return (angle / 180.0 * PI);
}
////¶óµð¾È °ªÀ» °¢µµ·Î º¯È¯ÇÏ¿© ¸®ÅÏÇÑ´Ù.
double CDCG::RTD(double radian)
{
	return (radian * 180.0 / PI);
}

void CDCG::PyoSi(const AcGePoint3d pos)
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
double CDCG::getPickBox()
{
	int			iPickBox;
	double		viewsize;
	AcGePoint2d screensize;

	getSysVar(_T("VIEWSIZE"), viewsize);
	getSysVar(_T("SCREENSIZE"), screensize);
	getSysVar(_T("PICKBOX"), iPickBox);

	return ((viewsize / screensize.y)*(double)iPickBox);
}
Acad::ErrorStatus CDCG::getSysVar(LPCTSTR varName, double& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTREAL);
		val = rb.resval.rreal;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

Acad::ErrorStatus CDCG::getSysVar(LPCTSTR varName, AcGePoint2d& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTPOINT);
		val.set(rb.resval.rpoint[X], rb.resval.rpoint[Y]);
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}
Acad::ErrorStatus CDCG::getSysVar(LPCTSTR varName, int& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTSHORT);
		val = rb.resval.rint;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

AcGePoint3dArray CDCG::PolyptArr(AcGePoint3d pt1, AcGePoint3d pt2, AcGePoint3d pt3, AcGePoint3d pt4, double Ang)
{
	AcGePoint3dArray arr;
	
	if(RTD(Ang) >=90 && RTD(Ang) < 180){
		arr.append(pt2);arr.append(pt1);arr.append(pt4);arr.append(pt3);arr.append(pt2);
	}else if(RTD(Ang) >=180 && RTD(Ang) < 270){
		arr.append(pt3);arr.append(pt2);arr.append(pt1);arr.append(pt4);arr.append(pt3);
	}else if(RTD(Ang) >=270 && RTD(Ang) < 360){
		arr.append(pt2);arr.append(pt3);arr.append(pt4);arr.append(pt1);arr.append(pt2);
	}else if(RTD(Ang) >=0 && RTD(Ang) < 90){
		arr.append(pt1);arr.append(pt2);arr.append(pt3);arr.append(pt4);arr.append(pt1);
	}
	return arr;
}

//ÅØ½ºÆ®»ý¼º 
double CDCG::CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer,CString Style,double Color, int mode){
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
		es = pText->setHorizontalMode(AcDb::kTextRight);
	}else{
		es = pText->setVerticalMode(AcDb::kTextBottom);
		es = pText->setHorizontalMode(AcDb::kTextLeft);
	}
	pText->setAlignmentPoint(pt);
	pText->setHeight(Height);
	pText->setRotation(Rotate);
	pText->setWidthFactor(Width);
	pText->setTextString((LPCTSTR)TxtStr);
	pText->setColorIndex(Color);

	AcDbExtents mExtText;
	pText->getGeomExtents(mExtText);
	//mExtText.maxPoint().x - mExtText.minPoint().x;//Ä³µå»ó ÅØ½ºÆ® ±æÀÌ
	//mExtText.maxPoint().y - mExtText.minPoint().y;//Ä³µå»ó ÅØ½ºÆ® ³ôÀÌ
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

AcDbObjectId CDCG::findTextStyleName(CString name){
	AcDbTextStyleTable* pTextTbl = NULL;
	AcDbTextStyleTablePointer pTextPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbTextStyleTableIterator* pTextTblIter;
	pTextPoint->newIterator(pTextTblIter);

	AcDbObjectId TextId;
	//ÅØ½ºÆ® ½ºÅ¸ÀÏ Å×ÀÌºíÀ» ºÒ·¯¿Í ÀÔ·ÂÇÑ ÅØ½ºÆ® ½ºÅ¸ÀÏÀÇ °ª°ú ÀÏÄ¡ÇÏ¸é AcDbObjectId °ªÀ» ¹ÝÈ¯ÇÑ´Ù.
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


//°´Ã¼ ¼±ÅÃ ±×¸®µå ±×¸®±â
void CDCG::CreateObjGrid(AcDbObjectId PolygonID, AcGePoint3d Cntrpt, double ELValue){
	
	AcGePoint3dArray polyArr;
	AcGePoint3d MainLWidpt,MainRWidpt, MainUHgtpt, MainDHgtpt, m_StdCntrpt;
	AcGePoint3d SubLWidpt,SubRWidpt, SubUHgtpt, SubDHgtpt, TextLpt, TextRpt, TextDpt, TextUpt;
	double m_MainLWidRemain,m_MainRWidRemain, m_MainUHgtRemain, m_MainDHgtRemain;
	double m_SubLWidRemain,m_SubRWidRemain, m_SubUHgtRemain, m_SubDHgtRemain;
	double m_CalScale = (1000/m_Scale);
	
	CString textcnt_str, ELV_str;
	ELV_str.Format(_T("%.0f"),ELValue);
	
	///////////////////¼¼·Î Áß½É¼±//////////////////////////
	
	polyArr = Obj_getInterpt(PolygonID, Cntrpt, 2);
	MainUHgtpt = polyArr[0]; MainDHgtpt = polyArr[1];
	m_MainUHgtRemain = fmod(abs(MainUHgtpt.y - Cntrpt.y), m_SubWidth * m_CalScale);//°¡·Î À§ÂÊºÎºÐ while¹® Á¦ÇÑµÎ±âÀ§ÇØ ¹Ì¸® À§ÂÊ³ª¸ÓÁö ±¸ÇÔ
	if(m_MainUHgtRemain < 0.001)m_MainUHgtRemain = m_MainUHgtRemain + (m_SubWidth * m_CalScale);//³ª¸ÓÁö°¡ 0.01º¸´Ù ÀÛ´Ù¸é º¸Á¶°£°Ý¸¸Å­ Ãß°¡
	Polar(MainUHgtpt, DTR(270), m_MainUHgtRemain, MainUHgtpt);//À§ÂÊ Á¡ ³ª¸ÓÁö¸¸Å­ ÀÌµ¿

	m_MainDHgtRemain = fmod(abs(MainDHgtpt.y - Cntrpt.y), m_SubWidth * m_CalScale);//°¡·Î ¾Æ·¡ÂÊºÎºÐ while¹® Á¦ÇÑµÎ±âÀ§ÇØ ¹Ì¸® À§ÂÊ³ª¸ÓÁö ±¸ÇÔ
	if(m_MainDHgtRemain < 0.001)m_MainDHgtRemain = m_MainDHgtRemain + (m_SubWidth * m_CalScale);//³ª¸ÓÁö°¡ 0.01º¸´Ù ÀÛ´Ù¸é º¸Á¶°£°Ý¸¸Å­ Ãß°¡
	Polar(MainDHgtpt, DTR(90), m_MainDHgtRemain + ((m_TextHeight*4)*m_CalScale), MainDHgtpt);//¾Æ·¡ÂÊ Á¡ ³ª¸ÓÁö¸¸Å­ ÀÌµ¿

	///////////////////¸ÞÀÎ °¡·Î Áß½É¼± + ¾Æ·¡ºÎºÐ//////////////////////////
	int textcnt=ELValue;//ÅØ½ºÆ® ¼ýÀÚ
	m_StdCntrpt = Cntrpt;
	while(true){///////////////////¸ÞÀÎ °¡·Î ¾Æ·¡ ºÎºÐ//////////////////////////
		polyArr = Obj_getInterpt(PolygonID,m_StdCntrpt,1);//Æú¸®°ï °´Ã¼¿ÍÀÇ ±³Â÷Á¡
		MainLWidpt = polyArr[0]; MainRWidpt = polyArr[1];

		//¸ÞÀÎ ±×¸®µå´Â ELÇ¥±â ¿µÇâ X
		m_MainLWidRemain = fmod(abs(MainLWidpt.x-m_StdCntrpt.x), m_SubWidth*m_CalScale);
		if(m_MainLWidRemain < 0.001)m_MainLWidRemain = m_MainLWidRemain+(m_SubWidth*m_CalScale);
		
		m_MainRWidRemain = fmod(abs(MainRWidpt.x-m_StdCntrpt.x), m_SubWidth*m_CalScale);
		if(m_MainRWidRemain < 0.001)m_MainRWidRemain = m_MainRWidRemain+(m_SubWidth*m_CalScale);
		
		Polar(MainLWidpt, ads_angle(asDblArray(MainLWidpt),asDblArray(MainRWidpt)),m_MainLWidRemain,MainLWidpt);
		Polar(MainRWidpt, ads_angle(asDblArray(MainRWidpt),asDblArray(MainLWidpt)),m_MainLWidRemain,MainRWidpt);
		//PyoSi(MainLWidpt);PyoSi(MainRWidpt);
		polyArr.append(MainLWidpt);polyArr.append(MainRWidpt);

		textcnt_str.Format(_T("%d"),textcnt);

		//ELÇ¥±â¿¡ µû¸¥ ÅØ½ºÆ®
		if(bool_ChkELLeft==true){
			TextLpt = MainLWidpt;
			TextLpt.x = MainLWidpt.x + ((m_TextHeight*4)*m_CalScale) - ((m_TextHeight/2)*m_CalScale);  TextLpt.y = MainLWidpt.y + ((m_TextHeight/2)*m_CalScale);
			CreateText(TextLpt, m_TextHeight*m_CalScale, m_TextWidth, 0, textcnt_str, m_CbxTextLayer_Str, m_CbxTextStyle_Str, m_TextColor, 1);
		}
		if(bool_ChkELRight==true){
			TextRpt = MainRWidpt;
			TextRpt.x = MainRWidpt.x - ((m_TextHeight*4)*m_CalScale) + ((m_TextHeight/2)*m_CalScale);  TextRpt.y = MainRWidpt.y + ((m_TextHeight/2)*m_CalScale);
			CreateText(TextRpt, m_TextHeight*m_CalScale, m_TextWidth, 0, textcnt_str, m_CbxTextLayer_Str, m_CbxTextStyle_Str, m_TextColor, 2);
		}
		//Continuous ¼±À¸·Î ±×¸®±â Ã¼Å©½Ã ¿¬¼Ó¼±À¸·Î ÀÛµµ ¾Æ´Ò½Ã DotÀ¸·Î ÀÛµµ
		if(bool_ChkMaincon==true)
			CreatePLine(polyArr,0,_T("Continuous"), m_MainLScale, m_CbxMainLayer_Str, m_MainColor);
		else
			CreatePLine(polyArr,0,_T("Dot2"), m_MainLScale, m_CbxMainLayer_Str, m_MainColor);
		polyArr.removeAll();
		if(fmod(m_MainWidth,m_SubWidth) == 0)
			m_StdCntrpt.y = m_StdCntrpt.y - (m_MainWidth*m_CalScale);//Áß½ÉÁ¡¿¡¼­ ¸ÞÀÎ±×¸®µå °£°Ý¸¸Å­ (-)ÇÏ¿© ¹Ýº¹
		else
			m_StdCntrpt.y = m_StdCntrpt.y - (m_MainWidth*m_SubWidth*m_CalScale);//Áß½ÉÁ¡¿¡¼­ ¸ÞÀÎ±×¸®µå °£°Ý¸¸Å­ (-)ÇÏ¿© ¹Ýº¹

		if(YMinpt.y + m_MainDHgtRemain + ((m_TextHeight*4)*m_CalScale) > m_StdCntrpt.y)break;//
		if(fmod(m_MainWidth,m_SubWidth) == 0)
			textcnt = textcnt - m_MainWidth;
		else
			textcnt = textcnt - (m_MainWidth*m_SubWidth);
	}

	///////////////////¸ÞÀÎ °¡·Î À§ºÎºÐ//////////////////////////
	textcnt=ELValue + m_MainWidth;
	m_StdCntrpt = Cntrpt;
	if(fmod(m_MainWidth,m_SubWidth) == 0)
		m_StdCntrpt.y = m_StdCntrpt.y + (m_MainWidth*m_CalScale);
	else
		m_StdCntrpt.y = m_StdCntrpt.y + (m_MainWidth*m_SubWidth*m_CalScale);
	while(true){
		polyArr = Obj_getInterpt(PolygonID,m_StdCntrpt,1);//Æú¸®°ï °´Ã¼¿ÍÀÇ ±³Â÷Á¡
		MainLWidpt = polyArr[0]; MainRWidpt = polyArr[1];

		//¸ÞÀÎ ±×¸®µå´Â ELÇ¥±â ¿µÇâ X
		m_MainLWidRemain = fmod(abs(MainLWidpt.x-m_StdCntrpt.x), m_SubWidth*m_CalScale);
		if(m_MainLWidRemain < 0.001)m_MainLWidRemain = m_MainLWidRemain+(m_SubWidth*m_CalScale);
		
		m_MainRWidRemain = fmod(abs(MainRWidpt.x-m_StdCntrpt.x), m_SubWidth*m_CalScale);
		if(m_MainRWidRemain < 0.001)m_MainRWidRemain = m_MainRWidRemain+(m_SubWidth*m_CalScale);
		
		Polar(MainLWidpt, 0,m_MainLWidRemain,MainLWidpt);
		Polar(MainRWidpt, DTR(180),m_MainLWidRemain,MainRWidpt);
		//PyoSi(MainLWidpt);PyoSi(MainRWidpt);
		polyArr.append(MainLWidpt);polyArr.append(MainRWidpt);
		
		textcnt_str.Format(_T("%d"),textcnt);

		//ELÇ¥±â¿¡ µû¸¥ ÅØ½ºÆ®
		if(bool_ChkELLeft==true){
			TextLpt = MainLWidpt;
			TextLpt.x = MainLWidpt.x + ((m_TextHeight*4) * m_CalScale) - ((m_TextHeight/2) * m_CalScale);  TextLpt.y = MainLWidpt.y + ((m_TextHeight/2) * m_CalScale);
			CreateText(TextLpt, m_TextHeight * m_CalScale, m_TextWidth, 0, textcnt_str, m_CbxTextLayer_Str, m_CbxTextStyle_Str, m_TextColor, 1);
		}
		if(bool_ChkELRight==true){
			TextRpt = MainRWidpt;
			TextRpt.x = MainRWidpt.x - ((m_TextHeight*4) * m_CalScale) + ((m_TextHeight/2) * m_CalScale);  TextRpt.y = MainRWidpt.y + ((m_TextHeight/2) * m_CalScale);
			CreateText(TextRpt, m_TextHeight * m_CalScale, m_TextWidth, 0, textcnt_str, m_CbxTextLayer_Str, m_CbxTextStyle_Str, m_TextColor, 2);
		}

		//Continuous ¼±À¸·Î ±×¸®±â Ã¼Å©½Ã ¿¬¼Ó¼±À¸·Î ÀÛµµ ¾Æ´Ò½Ã DotÀ¸·Î ÀÛµµ
		if(bool_ChkMaincon==true)
			CreatePLine(polyArr,0,_T("Continuous"), m_MainLScale, m_CbxMainLayer_Str, m_MainColor);
		else
			CreatePLine(polyArr,0,_T("Dot2"), m_MainLScale, m_CbxMainLayer_Str, m_MainColor);
		//CreatePLine(polyArr, 0,_T("bylayer"), 1, _T("0"), 0);
		polyArr.removeAll();
		if(fmod(m_MainWidth,m_SubWidth) == 0)
			m_StdCntrpt.y = m_StdCntrpt.y + (m_MainWidth*m_CalScale);//Áß½ÉÁ¡¿¡¼­ ¸ÞÀÎ±×¸®µå °£°Ý¸¸Å­ (-)ÇÏ¿© ¹Ýº¹
		else
			m_StdCntrpt.y = m_StdCntrpt.y + (m_MainWidth*m_SubWidth*m_CalScale);//Áß½ÉÁ¡¿¡¼­ ¸ÞÀÎ±×¸®µå °£°Ý¸¸Å­ (-)ÇÏ¿© ¹Ýº¹

		//m_StdCntrpt.y = m_StdCntrpt.y + (m_MainWidth * m_CalScale);
		if(YMaxpt.y - m_MainUHgtRemain < m_StdCntrpt.y)break;
		if(fmod(m_MainWidth,m_SubWidth) == 0)
			textcnt = textcnt + m_MainWidth;
		else
			textcnt = textcnt + (m_MainWidth*m_SubWidth);
	}
	
	///////////////////¸ÞÀÎ ¼¼·Î ¿ÞÂÊºÎºÐ//////////////////////////
	m_StdCntrpt = Cntrpt;
	m_StdCntrpt.x = m_StdCntrpt.x - (m_MainWidth * m_SubWidth * m_CalScale);
	double VertiNum = -(m_MainWidth), VertiMulti=1;
	CString VertiNumStr;
	while(true){
		polyArr = Obj_getInterpt(PolygonID,m_StdCntrpt,2);//Æú¸®°ï °´Ã¼¿ÍÀÇ ±³Â÷Á¡
		MainUHgtpt = polyArr[0]; MainDHgtpt = polyArr[1];
		
		m_MainUHgtRemain = fmod(abs(MainUHgtpt.y-m_StdCntrpt.y), m_SubWidth * m_CalScale);
		if(m_MainUHgtRemain < 0.001)m_MainUHgtRemain = m_MainUHgtRemain+(m_SubWidth * m_CalScale);

		m_MainDHgtRemain = fmod(abs(abs(MainDHgtpt.y-m_StdCntrpt.y)+((m_TextHeight*4) * m_CalScale)), m_SubWidth * m_CalScale);
		if(m_MainDHgtRemain < 0.001)m_MainDHgtRemain = m_MainDHgtRemain+(m_SubWidth * m_CalScale);
		
		Polar(MainUHgtpt, DTR(270) ,m_MainUHgtRemain,MainUHgtpt);
		Polar(MainDHgtpt, DTR(90) ,m_MainDHgtRemain,MainDHgtpt);
		//PyoSi(MainLWidpt);PyoSi(MainRWidpt);
		polyArr.append(MainUHgtpt);polyArr.append(MainDHgtpt);

		VertiNum = m_MainWidth * VertiMulti * m_SubWidth;//
		VertiNumStr.Format(_T("-%.0f"),VertiNum);
		TextDpt = MainDHgtpt;
		TextDpt.y = MainDHgtpt.y + ((m_TextHeight*4) * m_CalScale) - ((m_TextHeight/2) * m_CalScale); TextDpt.x = MainDHgtpt.x - ((m_TextHeight/2) * m_CalScale);
		CreateText(TextDpt, m_TextHeight * m_CalScale, m_TextWidth, DTR(90), VertiNumStr, m_CbxTextLayer_Str, m_CbxTextStyle_Str, m_TextColor, 1);

		//Continuous ¼±À¸·Î ±×¸®±â Ã¼Å©½Ã ¿¬¼Ó¼±À¸·Î ÀÛµµ ¾Æ´Ò½Ã DotÀ¸·Î ÀÛµµ
		if(bool_ChkMaincon==true)
			CreatePLine(polyArr,0,_T("Continuous"), m_MainLScale, m_CbxMainLayer_Str, m_MainColor);
		else
			CreatePLine(polyArr,0,_T("Dot2"), m_MainLScale, m_CbxMainLayer_Str, m_MainColor);
		polyArr.removeAll();


		m_StdCntrpt.x = m_StdCntrpt.x - (m_MainWidth * m_SubWidth * m_CalScale);
		//if(YMinpt.y + (m_TextHeight * 4) + m_MainDHgtRemain > m_StdCntrpt.y)
		//¹®ÀÚ ¿µ¿ª º¸Á¶ Grid »èÁ¦ Ã¼Å©½Ã
		if(bool_ChkDelGrid==true){
			if(bool_ChkELLeft==false){
				if(XMinpt.x + m_MainLWidRemain > m_StdCntrpt.x)break;
			}else{
				if(XMinpt.x + m_MainLWidRemain + ((m_TextHeight*4) * m_CalScale) > m_StdCntrpt.x)break;
			}
		}else{			
			if(XMinpt.x + m_MainLWidRemain > m_StdCntrpt.x)break;
		}
		
		VertiMulti++;
	}

	///////////////////¸ÞÀÎ ¼¼·Î ¿À¸¥ÂÊºÎºÐ//////////////////////////
	m_StdCntrpt = Cntrpt;
	VertiNum = -(m_MainWidth), VertiMulti=0;
	while(true){
		polyArr = Obj_getInterpt(PolygonID,m_StdCntrpt,2);//Æú¸®°ï °´Ã¼¿ÍÀÇ ±³Â÷Á¡
		MainUHgtpt = polyArr[0]; MainDHgtpt = polyArr[1];
		
		m_MainUHgtRemain = fmod(abs(MainUHgtpt.y-m_StdCntrpt.y), m_SubWidth * m_CalScale);
		if(m_MainUHgtRemain < 0.001)m_MainUHgtRemain = m_MainUHgtRemain+(m_SubWidth * m_CalScale);

		m_MainDHgtRemain = fmod(abs(abs(MainDHgtpt.y-m_StdCntrpt.y)+((m_TextHeight*4) * m_CalScale)), m_SubWidth * m_CalScale);
		if(m_MainDHgtRemain < 0.001)m_MainDHgtRemain = m_MainDHgtRemain+(m_SubWidth * m_CalScale);
		
		Polar(MainUHgtpt, DTR(270) ,m_MainUHgtRemain,MainUHgtpt);
		Polar(MainDHgtpt, DTR(90) ,m_MainDHgtRemain,MainDHgtpt);
		//PyoSi(MainLWidpt);PyoSi(MainRWidpt);
		polyArr.append(MainUHgtpt);polyArr.append(MainDHgtpt);

		VertiNum = m_MainWidth * VertiMulti * m_SubWidth;
		VertiNumStr.Format(_T("%.0f"),VertiNum);
		TextDpt = MainDHgtpt;
		TextDpt.y = MainDHgtpt.y + ((m_TextHeight*4) * m_CalScale) - ((m_TextHeight/2) * m_CalScale); TextDpt.x = MainDHgtpt.x - ((m_TextHeight/2) * m_CalScale);
		CreateText(TextDpt, m_TextHeight * m_CalScale, m_TextWidth, DTR(90), VertiNumStr, m_CbxTextLayer_Str, m_CbxTextStyle_Str, m_TextColor, 1);

		//Continuous ¼±À¸·Î ±×¸®±â Ã¼Å©½Ã ¿¬¼Ó¼±À¸·Î ÀÛµµ ¾Æ´Ò½Ã DotÀ¸·Î ÀÛµµ
		if(bool_ChkMaincon==true)
			CreatePLine(polyArr,0,_T("Continuous"), m_MainLScale, m_CbxMainLayer_Str, m_MainColor);
		else
			CreatePLine(polyArr,0,_T("Dot2"), m_MainLScale, m_CbxMainLayer_Str, m_MainColor);
		polyArr.removeAll();
		m_StdCntrpt.x = m_StdCntrpt.x + (m_MainWidth * m_SubWidth * m_CalScale);
		//if(YMinpt.y + (m_TextHeight * 4) + m_MainDHgtRemain > m_StdCntrpt.y)
		//¹®ÀÚ ¿µ¿ª º¸Á¶ Grid »èÁ¦ Ã¼Å©½Ã
		if(bool_ChkDelGrid==true){
			if(bool_ChkELRight==false){
				if(XMaxpt.x - m_MainRWidRemain < m_StdCntrpt.x)break;
			}else{
				if(XMaxpt.x - m_MainRWidRemain - ((m_TextHeight*4) * m_CalScale) < m_StdCntrpt.x)break;
			}			
		}else{
			if(XMaxpt.x - m_MainRWidRemain < m_StdCntrpt.x)break;
		}
		
		VertiMulti++;
	}

	///////////////////º¸Á¶ °¡·Î  ¾Æ·¡ºÎºÐ//////////////////////////
	
	m_StdCntrpt = Cntrpt;
	int count=0, maincnt = 0;
	while(true){
		polyArr = Obj_getInterpt(PolygonID,m_StdCntrpt,1);//Æú¸®°ï °´Ã¼¿ÍÀÇ ±³Â÷Á¡
		MainLWidpt = polyArr[0]; MainRWidpt = polyArr[1];

		if(bool_ChkELLeft==true){//EL Ç¥±â ÁÂ Ã¼Å©½Ã ¹®ÀÚÅ©±â*4¸¸Å­ ³ª¸ÓÁö Ãß°¡
			//m_MainLWidRemain = fmod(abs(abs(MainLWidpt.x-m_StdCntrpt.x)-((m_TextHeight*4) * m_CalScale)),m_SubWidth * m_CalScale);
			m_MainLWidRemain = fmod(abs(abs(MainLWidpt.x-m_StdCntrpt.x)),m_SubWidth * m_CalScale);
			if(m_MainLWidRemain < 0.001)m_MainLWidRemain = m_MainLWidRemain+(m_SubWidth * m_CalScale);
			m_MainLWidRemain = m_MainLWidRemain+((m_TextHeight*4) * m_CalScale);
			if(bool_ChkDelGrid==false)// ¹®ÀÚ ¿µ¿ª º¸Á¶ Grid »èÁ¦ Ã¼Å©XÀÏ ¶§ ¹®ÀÚÅ©±â*4 Ãß°¡ºÎºÐ Á¦°Å
				m_MainLWidRemain = m_MainLWidRemain-((m_TextHeight*4) * m_CalScale);
		}else{
			m_MainLWidRemain = fmod(abs(MainLWidpt.x-m_StdCntrpt.x), m_SubWidth * m_CalScale);
			if(m_MainLWidRemain < 0.001)m_MainLWidRemain = m_MainLWidRemain+(m_SubWidth * m_CalScale);
		}
		if(bool_ChkELRight==true){//EL Ç¥±â ¿ì Ã¼Å©½Ã ¹®ÀÚÅ©±â*4¸¸Å­ ³ª¸ÓÁö Ãß°¡
			//m_MainRWidRemain = fmod(abs(abs(MainRWidpt.x-m_StdCntrpt.x)-((m_TextHeight*4) * m_CalScale)), m_SubWidth * m_CalScale);
			m_MainRWidRemain = fmod(abs(abs(MainRWidpt.x-m_StdCntrpt.x)), m_SubWidth * m_CalScale);
			if(m_MainRWidRemain < 0.001)m_MainRWidRemain = m_MainRWidRemain+(m_SubWidth * m_CalScale);
			m_MainRWidRemain = m_MainRWidRemain+((m_TextHeight*4) * m_CalScale);
			if(bool_ChkDelGrid==false)// ¹®ÀÚ ¿µ¿ª º¸Á¶ Grid »èÁ¦ Ã¼Å©XÀÏ ¶§ ¹®ÀÚÅ©±â*4 Ãß°¡ºÎºÐ Á¦°Å
				m_MainRWidRemain = m_MainRWidRemain-((m_TextHeight*4) * m_CalScale);
		}else{
			m_MainRWidRemain = fmod(abs(MainRWidpt.x-m_StdCntrpt.x), m_SubWidth * m_CalScale);
			if(m_MainRWidRemain < 0.001)m_MainRWidRemain = m_MainRWidRemain+(m_SubWidth * m_CalScale);
		}	

		Polar(MainLWidpt, 0,m_MainLWidRemain,MainLWidpt);
		Polar(MainRWidpt, DTR(180),m_MainRWidRemain,MainRWidpt);
		//PyoSi(MainLWidpt);PyoSi(MainRWidpt);
		polyArr.append(MainLWidpt);polyArr.append(MainRWidpt);
		//PyoSi(MainLWidpt);
		//Continuous ¼±À¸·Î ±×¸®±â Ã¼Å©½Ã ¿¬¼Ó¼±À¸·Î ÀÛµµ ¾Æ´Ò½Ã DotÀ¸·Î ÀÛµµ
		if(bool_ChkMaincon==true)
			CreatePLine(polyArr,0,_T("Continuous"), m_SubLScale, m_CbxSubLayer_Str, m_SubColor);
		else
			CreatePLine(polyArr,0,_T("Dot2"), m_SubLScale, m_CbxSubLayer_Str, m_SubColor);

		ads_name ens;
		if(fmod(m_MainWidth,m_SubWidth) == 0){
			if(count == maincnt*m_MainWidth/m_SubWidth){//¸ÞÀÎ ±×¸®µå¿Í °ãÄ¡´Â ºÎºÐ »èÁ¦
				acdbEntLast(ens);
				acdbEntDel(ens);
				maincnt++;
			}
		}else{
			if(count == maincnt*m_MainWidth){//¸ÞÀÎ ±×¸®µå¿Í °ãÄ¡´Â ºÎºÐ »èÁ¦
				acdbEntLast(ens);
				acdbEntDel(ens);
				maincnt++;
			}
		}
		
		polyArr.removeAll();

		m_StdCntrpt.y = m_StdCntrpt.y - (m_SubWidth * m_CalScale);
		//if(YMinpt.y + (m_TextHeight * 4) + m_MainDHgtRemain > m_StdCntrpt.y)
		
		//¹®ÀÚ ¿µ¿ª º¸Á¶ Grid »èÁ¦½Ã ¾Æ·¡ ÅØ½ºÆ® ºÎºÐ±îÁö º¸Á¶±×¸®µå ÀÛµµ
		if(bool_ChkDelGrid==true){
			if(YMinpt.y + m_MainDHgtRemain + ((m_TextHeight * 4) * m_CalScale) >= m_StdCntrpt.y)break;
		}else{
			if(YMinpt.y + m_MainDHgtRemain >= m_StdCntrpt.y)break;
		}
		
		count++;
	}

	///////////////////º¸Á¶ °¡·Î  À§ºÎºÐ//////////////////////////
	m_StdCntrpt = Cntrpt;
	count=0, maincnt = 0;
	while(true){
		polyArr = Obj_getInterpt(PolygonID,m_StdCntrpt,1);//Æú¸®°ï °´Ã¼¿ÍÀÇ ±³Â÷Á¡
		MainLWidpt = polyArr[0]; MainRWidpt = polyArr[1];

		if(bool_ChkELLeft==true){//EL Ç¥±â ÁÂ Ã¼Å©½Ã ¹®ÀÚÅ©±â*4¸¸Å­ ³ª¸ÓÁö Ãß°¡
			//m_MainLWidRemain = fmod(abs(abs(MainLWidpt.x-m_StdCntrpt.x)-((m_TextHeight*4) * m_CalScale)),m_SubWidth * m_CalScale);
			m_MainLWidRemain = fmod(abs(abs(MainLWidpt.x-m_StdCntrpt.x)),m_SubWidth * m_CalScale);
			if(m_MainLWidRemain < 0.001)m_MainLWidRemain = m_MainLWidRemain+(m_SubWidth * m_CalScale);
			m_MainLWidRemain = m_MainLWidRemain+((m_TextHeight*4) * m_CalScale);
			if(bool_ChkDelGrid==false)// ¹®ÀÚ ¿µ¿ª º¸Á¶ Grid »èÁ¦ Ã¼Å©XÀÏ ¶§ ¹®ÀÚÅ©±â*4 Ãß°¡ºÎºÐ Á¦°Å
				m_MainLWidRemain = m_MainLWidRemain-((m_TextHeight*4) * m_CalScale);
		}else{
			m_MainLWidRemain = fmod(abs(MainLWidpt.x-m_StdCntrpt.x), m_SubWidth * m_CalScale);
			if(m_MainLWidRemain < 0.001)m_MainLWidRemain = m_MainLWidRemain+(m_SubWidth * m_CalScale);
		}
		if(bool_ChkELRight==true){//EL Ç¥±â ¿ì Ã¼Å©½Ã ¹®ÀÚÅ©±â*4¸¸Å­ ³ª¸ÓÁö Ãß°¡
			//m_MainRWidRemain = fmod(abs(abs(MainRWidpt.x-m_StdCntrpt.x)-((m_TextHeight*4) * m_CalScale)), m_SubWidth * m_CalScale);
			m_MainRWidRemain = fmod(abs(abs(MainRWidpt.x-m_StdCntrpt.x)), m_SubWidth * m_CalScale);
			if(m_MainRWidRemain < 0.001)m_MainRWidRemain = m_MainRWidRemain+(m_SubWidth * m_CalScale);
			m_MainRWidRemain = m_MainRWidRemain+((m_TextHeight*4) * m_CalScale);
			if(bool_ChkDelGrid==false)// ¹®ÀÚ ¿µ¿ª º¸Á¶ Grid »èÁ¦ Ã¼Å©XÀÏ ¶§ ¹®ÀÚÅ©±â*4 Ãß°¡ºÎºÐ Á¦°Å
				m_MainRWidRemain = m_MainRWidRemain-((m_TextHeight*4) * m_CalScale);
		}else{
			m_MainRWidRemain = fmod(abs(MainRWidpt.x-m_StdCntrpt.x), m_SubWidth * m_CalScale);
			if(m_MainRWidRemain < 0.001)m_MainRWidRemain = m_MainRWidRemain+(m_SubWidth * m_CalScale);
		}	

		Polar(MainLWidpt, 0,m_MainLWidRemain,MainLWidpt);
		Polar(MainRWidpt, DTR(180),m_MainRWidRemain,MainRWidpt);
		//PyoSi(MainLWidpt);PyoSi(MainRWidpt);
		polyArr.append(MainLWidpt);polyArr.append(MainRWidpt);

		//Continuous ¼±À¸·Î ±×¸®±â Ã¼Å©½Ã ¿¬¼Ó¼±À¸·Î ÀÛµµ ¾Æ´Ò½Ã DotÀ¸·Î ÀÛµµ
		if(bool_ChkMaincon==true)
			CreatePLine(polyArr,0,_T("Continuous"), m_SubLScale, m_CbxSubLayer_Str, m_SubColor);
		else
			CreatePLine(polyArr,0,_T("Dot2"), m_SubLScale, m_CbxSubLayer_Str, m_SubColor);

		ads_name ens;
		//if(count == maincnt*m_MainWidth/m_SubWidth){//¸ÞÀÎ ±×¸®µå¿Í °ãÄ¡´Â ºÎºÐ »èÁ¦
		//	acdbEntLast(ens);
		//	acdbEntDel(ens);
		//	maincnt++;
		//}
		if(fmod(m_MainWidth,m_SubWidth) == 0){
			if(count == maincnt*m_MainWidth/m_SubWidth){//¸ÞÀÎ ±×¸®µå¿Í °ãÄ¡´Â ºÎºÐ »èÁ¦
				acdbEntLast(ens);
				acdbEntDel(ens);
				maincnt++;
			}
		}else{
			if(count == maincnt*m_MainWidth){//¸ÞÀÎ ±×¸®µå¿Í °ãÄ¡´Â ºÎºÐ »èÁ¦
				acdbEntLast(ens);
				acdbEntDel(ens);
				maincnt++;
			}
		}

		polyArr.removeAll();
		m_StdCntrpt.y = m_StdCntrpt.y + (m_SubWidth * m_CalScale);
		//if(YMinpt.y + (m_TextHeight * 4) + m_MainDHgtRemain > m_StdCntrpt.y)
		if(YMaxpt.y - m_MainDHgtRemain  <= m_StdCntrpt.y)break;
		count++;
	}

	///////////////////º¸Á¶ ¼¼·Î ¿ÞÂÊºÎºÐ//////////////////////////
	m_StdCntrpt = Cntrpt;
	count=0, maincnt = 0;
	while(true){
		polyArr = Obj_getInterpt(PolygonID,m_StdCntrpt,2);//Æú¸®°ï °´Ã¼¿ÍÀÇ ±³Â÷Á¡
		MainUHgtpt = polyArr[0]; MainDHgtpt = polyArr[1];
		
		m_MainUHgtRemain = fmod(abs(MainUHgtpt.y-m_StdCntrpt.y), m_SubWidth * m_CalScale);
		if(m_MainUHgtRemain < 0.001)m_MainUHgtRemain = m_MainUHgtRemain+(m_SubWidth * m_CalScale);
		//¾Æ·¡ÂÊ¿¡ ¹®ÀÚ´Â ¹«Á¶°Ç ÀÛµµµÇ±â ¶§¹®¿¡  ¹®ÀÚÅ©±â*4¸¸Å­ ³ª¸ÓÁö ºÎºÐ Ãß°¡
		m_MainDHgtRemain = fmod(abs(abs(MainDHgtpt.y-m_StdCntrpt.y)+((m_TextHeight*4) * m_CalScale)), m_SubWidth * m_CalScale);
		if(m_MainDHgtRemain < 0.001)m_MainDHgtRemain = m_MainDHgtRemain+(m_SubWidth * m_CalScale);
		m_MainDHgtRemain = m_MainDHgtRemain + ((m_TextHeight*4) * m_CalScale);
		if(bool_ChkDelGrid==false)//¹®ÀÚ ¿µ¿ª º¸Á¶ Grid »èÁ¦ Ã¼Å©X ½Ã ¹®ÀÚÅ©±â*4Ãß°¡ ºÎºÐ »èÁ¦
			m_MainDHgtRemain = m_MainDHgtRemain - ((m_TextHeight*4) * m_CalScale);
		
		Polar(MainUHgtpt, DTR(270) ,m_MainUHgtRemain,MainUHgtpt);
		Polar(MainDHgtpt, DTR(90) ,m_MainDHgtRemain,MainDHgtpt);
		//PyoSi(MainLWidpt);PyoSi(MainRWidpt);
		polyArr.append(MainUHgtpt);polyArr.append(MainDHgtpt);

		//Continuous ¼±À¸·Î ±×¸®±â Ã¼Å©½Ã ¿¬¼Ó¼±À¸·Î ÀÛµµ ¾Æ´Ò½Ã DotÀ¸·Î ÀÛµµ
		if(bool_ChkMaincon==true)
			CreatePLine(polyArr,0,_T("Continuous"), m_SubLScale, m_CbxSubLayer_Str, m_SubColor);
		else
			CreatePLine(polyArr,0,_T("Dot2"), m_SubLScale, m_CbxSubLayer_Str, m_SubColor);

		ads_name ens;
		if(count == maincnt*m_MainWidth){//¸ÞÀÎ ±×¸®µå¿Í °ãÄ¡´Â ºÎºÐ »èÁ¦
			acdbEntLast(ens);
			acdbEntDel(ens);
			maincnt++;
		}

		polyArr.removeAll();
		m_StdCntrpt.x = m_StdCntrpt.x - (m_SubWidth * m_CalScale);
		//if(YMinpt.y + (m_TextHeight * 4) + m_MainDHgtRemain > m_StdCntrpt.y)		
		if(XMinpt.x + m_MainLWidRemain > m_StdCntrpt.x)break;
		
		count++;
	}

	///////////////////º¸Á¶ ¼¼·Î ¿À¸¥ÂÊºÎºÐ//////////////////////////
	m_StdCntrpt = Cntrpt;
	count=0, maincnt = 0;
	while(true){
		polyArr = Obj_getInterpt(PolygonID,m_StdCntrpt,2);//Æú¸®°ï °´Ã¼¿ÍÀÇ ±³Â÷Á¡
		MainUHgtpt = polyArr[0]; MainDHgtpt = polyArr[1];
		
		m_MainUHgtRemain = fmod(abs(MainUHgtpt.y-m_StdCntrpt.y), m_SubWidth * m_CalScale);
		if(m_MainUHgtRemain < 0.001)m_MainUHgtRemain = m_MainUHgtRemain+(m_SubWidth * m_CalScale);
		//¾Æ·¡ÂÊ¿¡ ¹®ÀÚ´Â ¹«Á¶°Ç ÀÛµµµÇ±â ¶§¹®¿¡  ¹®ÀÚÅ©±â*4¸¸Å­ ³ª¸ÓÁö ºÎºÐ Ãß°¡
		m_MainDHgtRemain = fmod(abs(abs(MainDHgtpt.y-m_StdCntrpt.y)+((m_TextHeight*4) * m_CalScale)), m_SubWidth * m_CalScale);
		if(m_MainDHgtRemain < 0.001)m_MainDHgtRemain = m_MainDHgtRemain+(m_SubWidth * m_CalScale);
		m_MainDHgtRemain = m_MainDHgtRemain + ((m_TextHeight*4) * m_CalScale);
		if(bool_ChkDelGrid==false)//¹®ÀÚ ¿µ¿ª º¸Á¶ Grid »èÁ¦ Ã¼Å©X ½Ã ¹®ÀÚÅ©±â*4Ãß°¡ ºÎºÐ »èÁ¦
			m_MainDHgtRemain = m_MainDHgtRemain - ((m_TextHeight*4) * m_CalScale);
		
		Polar(MainUHgtpt, DTR(270) ,m_MainUHgtRemain,MainUHgtpt);
		Polar(MainDHgtpt, DTR(90) ,m_MainDHgtRemain,MainDHgtpt);
		//PyoSi(MainLWidpt);PyoSi(MainRWidpt);
		polyArr.append(MainUHgtpt);polyArr.append(MainDHgtpt);

		//Continuous ¼±À¸·Î ±×¸®±â Ã¼Å©½Ã ¿¬¼Ó¼±À¸·Î ÀÛµµ ¾Æ´Ò½Ã DotÀ¸·Î ÀÛµµ
		if(bool_ChkMaincon==true)
			CreatePLine(polyArr,0,_T("Continuous"), m_SubLScale, m_CbxSubLayer_Str, m_SubColor);
		else
			CreatePLine(polyArr,0,_T("Dot2"), m_SubLScale, m_CbxSubLayer_Str, m_SubColor);

		ads_name ens;
		if(count == maincnt*m_MainWidth){//¸ÞÀÎ ±×¸®µå¿Í °ãÄ¡´Â ºÎºÐ »èÁ¦
			acdbEntLast(ens);
			acdbEntDel(ens);
			maincnt++;
		}
		polyArr.removeAll();
		m_StdCntrpt.x = m_StdCntrpt.x + (m_SubWidth * m_CalScale);
		//if(YMinpt.y + (m_TextHeight * 4) + m_MainDHgtRemain > m_StdCntrpt.y)
		
		if(XMaxpt.x - m_MainRWidRemain < m_StdCntrpt.x)break;	
		
		count++;
	}
}

//±³Â÷Á¡ ±¸ÇÏ´Â ÇÔ¼ö
AcGePoint3dArray CDCG::Obj_getInterpt(AcDbObjectId PolygonID ,AcGePoint3d Cntrpt, int Mode){//Mode=1 °¡·Î Mode=2 ¼¼·Î
	AcGePoint3dArray InterptArr, MMArr, CntrArr, polyArr;
	AcGePoint3d pPt, MainLWidpt, MainRWidpt, MainUHgtpt, MainDHgtpt, MMpt;	

	AcDbEntityPointer pEnt(PolygonID, AcDb::kForRead);
	if(pEnt.openStatus() == Acad::eOk){
		if(pEnt->isKindOf(AcDbPolyline::desc())){
			AcDbPolyline* pPline = AcDbPolyline::cast(pEnt);

			for(int i=0; i<pPline->numVerts(); i++){
				pPline->getPointAt(i,pPt);
				MMArr.append(pPt);
			}

			//°´Ã¼ÀÇ X,YÃÖ´ë/ÃÖ¼Ò°ª 
			double m_XMax, m_XMin, m_YMax, m_YMin, num;
			m_XMax = MMArr[0].x;m_XMin = MMArr[0].x;m_YMax = MMArr[0].y;m_YMin = MMArr[0].y;
			XMaxpt = MMArr[0];XMinpt = MMArr[0];YMaxpt = MMArr[0];YMinpt = MMArr[0];
			for(int j=0; j<MMArr.length(); j++){//Æú¸®°ïÀÇ ÃÖ´ë ÃÖ¼Ò±æÀÌ ±¸ÇÏ±â À§ÇØ
				MMpt = MMArr[j];
				if(MMpt.x > m_XMax){
					m_XMax = MMArr[j].x;
					XMaxpt = MMArr[j];
				}
				if(MMpt.x < m_XMin){
					m_XMin = MMArr[j].x;
					XMinpt = MMArr[j];
				}
				if(MMpt.y > m_YMax){
					m_YMax = MMArr[j].y;
					YMaxpt = MMArr[j];
				}
				if(MMpt.y < m_YMin){
					m_YMin = MMArr[j].y;
					YMinpt = MMArr[j];
				}
			}
			if(Mode==1){//°¡·Î
				Polar(Cntrpt, 0, abs(XMaxpt.x - Cntrpt.x),MainLWidpt);
				Polar(Cntrpt, DTR(180),abs(Cntrpt.x - XMinpt.x),MainRWidpt);
				CntrArr.append(MainLWidpt);CntrArr.append(MainRWidpt);
			}else if(Mode==2){//¼¼·Î
				Polar(Cntrpt, DTR(90), abs(YMaxpt.y - Cntrpt.y),MainUHgtpt);
				Polar(Cntrpt, DTR(270), abs(Cntrpt.y - YMinpt.y),MainDHgtpt);
				CntrArr.append(MainUHgtpt);CntrArr.append(MainDHgtpt);
			}
			

			AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
			acDocManager->curDocument()->database();
			if(pDb == NULL)return InterptArr;
			Acad::ErrorStatus es;
			AcDbObjectId pPLineObjId;
			AcDbBlockTable *pBT;
			AcDbBlockTableRecord *pBTR;
			//±³Â÷Á¡ ±¸ÇÏ±â À§ÇØ °¡·Î/¼¼·Î ¶óÀÎ »ý¼º
			AcDbPolyline* pPline2 = new AcDbPolyline();
			for(int i =0; i<CntrArr.length(); i++){
				pPline2->addVertexAt(i, AcGePoint2d(CntrArr[i].x, CntrArr[i].y));
			}
			pPline2->setConstantWidth(0);
			pPline2->setLinetype(_T("bylayer"));
			pPline2->setLinetypeScale(1);
			pPline2->setLayer(_T("0"));
			pPline2->setColorIndex(0);

			if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
				if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
					es = pBTR->appendAcDbEntity(pPLineObjId, pPline2);			
					pBTR->close();
					pPline2->close();
				}else
					return InterptArr;
				pBT->close();
			}else
				return InterptArr;
			//PolygonID¿Í »ý¼ºÇÑ ¶óÀÎÀÇ ±³Â÷Á¡
			if(pPline->intersectWith(pPline2,AcDb::kOnBothOperands,InterptArr) != Acad::eOk)
				return InterptArr;
			if(Mode==1){//°¡·Î ±³Â÷Á¡
				double LAng = RTD(ads_angle(asDblArray(Cntrpt),asDblArray(InterptArr[1])));
				double RAng = RTD(ads_angle(asDblArray(Cntrpt),asDblArray(InterptArr[0])));
				if(LAng > RAng){
					MainLWidpt = InterptArr[1]; MainRWidpt = InterptArr[0];
				}else{
					MainLWidpt = InterptArr[0]; MainRWidpt = InterptArr[1];
				}
				
			}else if(Mode==2){//¼¼·Î ±³Â÷Á¡
				double UAng = RTD(ads_angle(asDblArray(Cntrpt),asDblArray(InterptArr[1])));
				double DAng = RTD(ads_angle(asDblArray(Cntrpt),asDblArray(InterptArr[0])));
				if(UAng < DAng){
					MainUHgtpt = InterptArr[1]; MainDHgtpt = InterptArr[0];
				}else{
					MainUHgtpt = InterptArr[0]; MainDHgtpt = InterptArr[1];
				}
			}
		
			pPline2->close();
			pPline->close();
			CntrArr.removeAll();	
			//±³Â÷Á¡ ±¸ÇÏ±â À§ÇØ »ý¼ºÇÑ ¶óÀÎ »èÁ¦
			ads_name lastent;
			acdbEntLast(lastent);
			acdbEntDel(lastent);
			if(Mode==1){
				polyArr.append(MainLWidpt);polyArr.append(MainRWidpt);
				polyArr.removeAll();
			}else if(Mode==2){
				polyArr.append(MainUHgtpt);polyArr.append(MainDHgtpt);
				polyArr.removeAll();
			}
			
		}
	}

	return polyArr;
}

void CDCG::LoadLineType(ACHAR* LineType, ACHAR* LineTypeFile){
	//hidden ¶óÀÎ Ãß°¡
	AcDbObjectId lineTypeId;
	AcDbDatabase *pWorkDB = acdbHostApplicationServices()->workingDatabase();
	pWorkDB->loadLineTypeFile(LineType, LineTypeFile);
	acutPrintf(_T("\n %s , %s"),LineType, LineTypeFile);
	return;

	//acdbLoadLineTypeFile(LineType, LineTypeFile, pWorkDB);
	//
	//AcDbLinetypeTable *pLinetypeTable; 
	//ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLinetypeTable, AcDb::kForRead); 
	//if(es == Acad::eOk)
	//	pLinetypeTable->getAt(LineType, lineTypeId, Adesk::kTrue); 	
	//
	//pLinetypeTable->close(); 
	//
	//
	//AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	//TCHAR szLtFile[MAX_PATH], *str;
	//
	//
	//CString strLINFile = LineTypeFile;
	//
	//if(RTNORM != zcedFindFile(strLINFile, szLtFile)){
	//	acutPrintf(_T("ZWCADiso.lin ÆÄÀÏÀÌ ¾ø½À´Ï´Ù."));
	//}
	//
	//AcDbLinetypeTable *pLtTable = NULL;
	//es = pWorkDB->getLinetypeTable(pLtTable,AcDb::kForRead);
	//ACHAR *szLtype = LineType;
	//bool isLinetypeLoaded = pLtTable->has(szLtype);
	//
	//es = pLtTable->close();
	//
	//if(!isLinetypeLoaded){
	//	
	//	es = pWorkDB->loadLineTypeFile(szLtype, szLtFile);
	//	if(Zcad::eOk == es){
	//		AcDbLinetypeTable *pTempLtTable;
	//		AcDbLinetypeTableRecord *pTempLtRec=NULL;
	//
	//		es = pWorkDB->getLinetypeTable(pTempLtTable,AcDb::kForRead);
	//		AcDbObjectId ltRecId = AcDbObjectId::kNull;
	//		es = pTempLtTable->getAt(szLtype, ltRecId);			
	//		pTempLtTable->close();
	//		if(Zcad::eOk == es){
	//			//acutPrintf(_T("\nLintype reloaded!"));
	//		}else{
	//			acutPrintf(_T("\nCouldn't reload Lintype"));
	//		}
	//	}else{
	//		acutPrintf(_T("\nError loading linetype from file!"));
	//	}
	//	//delete pTempDatabase;
	//}else{
	//	if(Zcad::eOk == pWorkDB->loadLineTypeFile(szLtype,szLtFile)){
	//		acutPrintf(_T("\nLinetype loaded from file!"));
	//	}else{
	//		acutPrintf(_T("\nError loading linetype from file!"));
	//	}		
	//}
	
	
}



void CDCG::OnBnClickedDcgHelp()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
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

	Help_Path.Format(_T("%s\\%s::/1. html/DCG.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
