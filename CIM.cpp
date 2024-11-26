#include "StdAfx.h"
#include "CIM.h"

// CCIM 대화 상자입니다.
IMPLEMENT_DYNAMIC(CCIM, CAcUiDialog)
CCIM::CCIM(CWnd* pParent)
	: CAcUiDialog(CCIM::IDD, pParent){
}

CCIM::~CCIM(){
}

void CCIM::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTLAYER, CTEXTLAYER);
	DDX_Control(pDX, IDC_CUTPOINTLAYER, CUTPNTLAYER);
	DDX_Control(pDX, IDC_CONLINELAYER, CCLINELAYER);
	DDX_Control(pDX, IDC_ZVALUELAYER, ZVALLAYER);
	DDX_Control(pDX, IDC_BNAME, CBName);
	DDX_Control(pDX, IDC_TEXT, CText);
	DDX_Control(pDX, IDC_ZVALUE, BZIns);
	DDX_Control(pDX, IDC_CUTPOINT, BCutpoint);
	DDX_Control(pDX, IDC_CONLINE, CConline);
	DDX_Control(pDX, IDC_POINTPICCOLOR, PointPicColor);
	DDX_Control(pDX, IDC_ZVALPICCOLOR, ZValPicColor);
	DDX_Control(pDX, IDC_ZCHECK, BZChk);
	DDX_Control(pDX, IDC_CONLINEPICCOLOR, ConPicColor);
	DDX_Control(pDX, IDC_TCPICCOLOR, TextPicColor);
	DDX_Control(pDX, IDC_SELPOINT, m_PointSelectButton);
	DDX_Control(pDX, IDC_SELTEXT, m_TextSelectButton);
	DDX_Control(pDX, IDC_SELCONLINE, m_LineSelectButton);
	DDX_Control(pDX, IDC_SELZVALUE, m_ZValueSelectButton);
}

BEGIN_MESSAGE_MAP(CCIM, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CCIM::OnBnClickedOk)
	ON_BN_CLICKED(IDC_TEXTLOAD, &CCIM::OnBnClickedTextload)
	ON_BN_CLICKED(IDC_EXCEL, &CCIM::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_CIRCLEBTN, &CCIM::OnBnClickedCirclebtn)
	ON_BN_CLICKED(IDC_POINTBTN, &CCIM::OnBnClickedPointbtn)
	ON_BN_CLICKED(IDC_BLOCKBTN, &CCIM::OnBnClickedBlockbtn)
	ON_BN_CLICKED(IDC_SET, &CCIM::OnBnClickedSet)
	ON_BN_CLICKED(IDC_SPACE, &CCIM::OnBnClickedSpace)
	ON_BN_CLICKED(IDC_Colon, &CCIM::OnBnClickedColon)
	ON_BN_CLICKED(IDC_PATHBTN, &CCIM::OnClickedPathbtn)
	ON_BN_CLICKED(IDC_TEXTBTN, &CCIM::OnBnClickedTextbtn)
	ON_BN_CLICKED(IDC_ZVALCOLORBTN, &CCIM::OnBnClickedZvalcolorbtn)
	ON_BN_CLICKED(IDC_POINTCOLORBTN, &CCIM::OnBnClickedPointcolorbtn)
	ON_BN_CLICKED(IDC_CONLINECOLORBTN, &CCIM::OnBnClickedConlinecolorbtn)
	ON_BN_CLICKED(IDC_TEXT, &CCIM::OnBnClickedText)
	ON_BN_CLICKED(IDC_CONLINE, &CCIM::OnBnClickedConline)
	ON_BN_CLICKED(IDC_CUTPOINT, &CCIM::OnBnClickedCutpoint)
	ON_BN_CLICKED(IDC_ZVALUE, &CCIM::OnBnClickedZvalue)
	ON_CBN_SELCHANGE(IDC_TEXTLAYER, &CCIM::OnCbnSelchangeTextlayer)
	ON_CBN_SELCHANGE(IDC_CONLINELAYER, &CCIM::OnCbnSelchangeConlinelayer)
	ON_BN_CLICKED(IDC_MINUS, &CCIM::OnBnClickedMinus)
	ON_BN_CLICKED(IDC_PLUS, &CCIM::OnBnClickedPlus)
	ON_CBN_SELCHANGE(IDC_ZVALUELAYER, &CCIM::OnCbnSelchangeZvaluelayer)
	ON_CBN_SELCHANGE(IDC_CUTPOINTLAYER, &CCIM::OnCbnSelchangeCutpointlayer)
	ON_BN_CLICKED(IDC_SELCONLINE, &CCIM::OnBnClickedSelconline)
	ON_BN_CLICKED(IDC_SELTEXT, &CCIM::OnBnClickedSeltext)
	ON_BN_CLICKED(IDC_SELZVALUE, &CCIM::OnBnClickedSelzvalue)
	ON_BN_CLICKED(IDC_SELPOINT, &CCIM::OnBnClickedSelpoint)
	ON_BN_CLICKED(ID_CIM_HELP, &CCIM::OnBnClickedCimHelp)
END_MESSAGE_MAP()

// CCIM 메시지 처리기입니다.
void CCIM::OnBnClickedOk(){
	bool chkExc = IsDlgButtonChecked(IDC_EXCEL);
	bool chkTxt = IsDlgButtonChecked(IDC_TEXTLOAD);	
	//ads_command(RTSTR, _T("PDMODE"), RTSTR, _T("0"), RTNONE);

	//DIALOG 창에서 설정한 값 저장
	CString iniStr;
	//엑셀 화면
	if(IsDlgButtonChecked(IDC_EXCEL))
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("JOBMODE"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("JOBMODE"), _T("FALSE"));
	// X,Y 좌표 바꾸기
	if(IsDlgButtonChecked(IDC_XYCHANGE))
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("XYCHANGE"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("XYCHANGE"), _T("FALSE"));
	// 1번째 열을 문자로 인식
	if (IsDlgButtonChecked(IDC_FIRST_TXT))
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("FIRST_TXT"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("FIRST_TXT"), _T("FALSE"));
	// 절점 종류
	if(IsDlgButtonChecked(IDC_Colon))
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("DIVISION"), _T("1"));
	if(IsDlgButtonChecked(IDC_SPACE))
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("DIVISION"), _T("2"));
	if(IsDlgButtonChecked(IDC_SET))
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("DIVISION"), _T("3"));
	// 텍스트 간격	
	GetDlgItemText(IDC_SETSPACE, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("DIVISIONSTRING"), iniStr);
	// 연결선 종류
	if(IsDlgButtonChecked(IDC_2DPOLY))
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("DIVISION"), _T("1"));
	if(IsDlgButtonChecked(IDC_SPACE))
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("DIVISION"), _T("2"));
	// 점 두께
	GetDlgItemText(IDC_POINTSIZE, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("PTHIKNESS"), iniStr);
	// 원 반지름
	GetDlgItemText(IDC_DIAMETER, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("CIRCLERAD"), iniStr);
	// 블록 축척
	GetDlgItemText(IDC_SCALE, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("BLOCKSCALE"), iniStr);
	// Z값 넣기
	if(IsDlgButtonChecked(IDC_ZCHECK))
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("INSERTZ"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("INSERTZ"), _T("FALSE"));
	// 문자 크기
	GetDlgItemText(IDC_TEXTSIZE, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("TEXTSIZE"), iniStr);
	// 문자 XOffset
	GetDlgItemText(IDC_XOFF, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("TEXTXOFFSET"), iniStr);
	// 문자 YOffset
	GetDlgItemText(IDC_YOFF, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("TEXTYOFFSET"), iniStr);
	// Z문자 크기
	GetDlgItemText(IDC_TEXTSIZE4, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("ZTEXTSIZE"), iniStr);
	// Z문자 XOffset
	GetDlgItemText(IDC_XOFF4, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("ZTEXTXOFFSET"), iniStr);
	// Z문자 YOffset
	GetDlgItemText(IDC_YOFF4, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("ZTEXTYOFFSET"), iniStr);
	// Z문자 소수점
	GetDlgItemText(IDC_PCNT, iniStr);
	//iniStr.Format(_T("%d"), ZTextPnt);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("ZTEXTYPNT"), iniStr);
	// 텍스트 레이어
	CTEXTLAYER.GetLBText(CTEXTLAYER.GetCurSel(),iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("TEXTLAYER"), iniStr);
	// 라인 레이어
	CCLINELAYER.GetLBText(CCLINELAYER.GetCurSel(),iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("LINELAYER"), iniStr);
	// Z값 레이어
	ZVALLAYER.GetLBText(ZVALLAYER.GetCurSel(),iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("ZVALUELAYER"), iniStr);
	// 점 레이어
	CUTPNTLAYER.GetLBText(CUTPNTLAYER.GetCurSel(),iniStr);
	iniFile.writeINI(_T("config.ini"), _T("CIM"), _T("POINTLAYER"), iniStr);


	if(chkExc){
		ImpressExcel();
	}
	else if(chkTxt)
		ImpressText();
	
	//CAcUiDialog::OnOK();
}

void CCIM::initLayer(){
	AcDbLayerTable* pLayerTbl = NULL;
	acdbCurDwg()->getSymbolTable(pLayerTbl, AcDb::kForWrite);	
	AcCmColor color;

	if(!pLayerTbl->has(_T("EL"))){
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord();
		color.setColorIndex(2);
		pLayerTblRcd->setColor(color);
		pLayerTblRcd->setName(_T("EL"));		
		pLayerTbl->add(pLayerTblRcd);
		pLayerTblRcd->close();
	}
	if(!pLayerTbl->has(_T("Line"))){
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord();
		color.setColorIndex(7);
		pLayerTblRcd->setColor(color);
		pLayerTblRcd->setName(_T("Line"));		
		pLayerTbl->add(pLayerTblRcd);
		pLayerTblRcd->close();
	}
	if(!pLayerTbl->has(_T("No"))){
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord();
		color.setColorIndex(3);
		pLayerTblRcd->setColor(color);
		pLayerTblRcd->setName(_T("No"));		
		pLayerTbl->add(pLayerTblRcd);
		pLayerTblRcd->close();
	}
	if(!pLayerTbl->has(_T("Point"))){
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord();
		color.setColorIndex(1);
		pLayerTblRcd->setColor(color);
		pLayerTblRcd->setName(_T("Point"));		
		pLayerTbl->add(pLayerTblRcd);
		pLayerTblRcd->close();
	}
	if(!pLayerTbl->has(_T("Text"))){
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord();
		color.setColorIndex(7);
		pLayerTblRcd->setColor(color);
		pLayerTblRcd->setName(_T("Text"));		
		pLayerTbl->add(pLayerTblRcd);
		pLayerTblRcd->close();
	}
	pLayerTbl->close();
}

void CCIM::initBlock(){
	AcDbBlockTable* pBlockTbl = NULL;
	AcDbBlockTablePointer pBlockPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbBlockTableIterator* pBlockTblIter;
	pBlockPoint->newIterator(pBlockTblIter);

	AcDbObjectId blockId;
	for(pBlockTblIter->start(); !pBlockTblIter->done(); pBlockTblIter->step()){
		pBlockTblIter->getRecordId(blockId);
		AcDbBlockTableRecordPointer pBlockTblRcd(blockId, AcDb::kForRead);
		TCHAR* name = NULL;
		pBlockTblRcd->getName(name);
		if(_tcscmp(name, _T("*Model_Space")) != 0 && _tcscmp(name, _T("*Paper_Space")) != 0 && _tcscmp(name, _T("*Paper_Space0")) != 0)
			CBName.AddString(name);
	}
	delete pBlockTblIter;
	delete pBlockTbl;
}

BOOL CCIM::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
    SetIcon(hIcon, FALSE);
	m_TextSelectButton.AutoLoad();
	m_LineSelectButton.AutoLoad();
	m_PointSelectButton.AutoLoad();
	m_ZValueSelectButton.AutoLoad();

	initLayer();
	initBlock();

	//각각 콤보박스에 도면층 이름 설정
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
		CTEXTLAYER.AddString(name);
		CUTPNTLAYER.AddString(name);
		CCLINELAYER.AddString(name);
		ZVALLAYER.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;

	// 엑셀 화면
	CString jobLineStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("JOBMODE"));
	if(jobLineStr.CompareNoCase(_T("")) == 0 || jobLineStr.CompareNoCase(_T("EXCEL")) == 0)
		CheckDlgButton(IDC_EXCEL, TRUE);
	else if(jobLineStr.CompareNoCase(_T("TEXT")) == 0)
		CheckDlgButton(IDC_TEXTLOAD, TRUE);
	// x,y 좌표 바꾸기
	CString xyChangeStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("XYCHANGE"));
	if(xyChangeStr.CompareNoCase(_T("")) == 0 || xyChangeStr.CompareNoCase(_T("TRUE")) == 0)
		CheckDlgButton(IDC_XYCHANGE, TRUE);
	// 1번째 열을 문자로 인식
	CString firtTxTStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("FIRST_TXT"));
	if (firtTxTStr.CompareNoCase(_T("")) == 0 || firtTxTStr.CompareNoCase(_T("TRUE")) == 0)
		CheckDlgButton(IDC_FIRST_TXT, TRUE);
	// 절점 종류
	CString divisionStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("DIVISION"));
	if(divisionStr.CompareNoCase(_T("")) == 0 || divisionStr.CompareNoCase(_T("1")) == 0)
		CheckDlgButton(IDC_Colon, TRUE);
	else if(divisionStr.CompareNoCase(_T("2")) == 0)
		CheckDlgButton(IDC_SPACE, TRUE);
	else if(divisionStr.CompareNoCase(_T("3")) == 0)
		CheckDlgButton(IDC_SET, TRUE);
	// 텍스트 간격
	CString divisionValue = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("DIVISIONSTRING"));
	SetDlgItemText(IDC_SETSPACE, divisionValue);
	// 연결선 종류
	CString ConnectStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("CONNECT"));
	if(ConnectStr.CompareNoCase(_T("1")) == 0)
		CheckDlgButton(IDC_2DPOLY, TRUE);
	else if(ConnectStr.CompareNoCase(_T("")) == 0 || ConnectStr.CompareNoCase(_T("2")) == 0)
		CheckDlgButton(IDC_3DPOLY, TRUE);
	// 점 두께
	CString PthicknessStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("PTHIKNESS"));
	if(PthicknessStr.CompareNoCase(_T("")) == 0){
		PointThickness = 0.5;
		PthicknessStr.Format(_T("%g"), PointThickness);
	}
	SetDlgItemText(IDC_POINTSIZE, PthicknessStr);
	// 원 반지름
	CString CradStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("CIRCLERAD"));
	if(CradStr.CompareNoCase(_T("")) == 0){
		CRad = 1.0;
		CradStr.Format(_T("%g"), CRad);
	}
	SetDlgItemText(IDC_DIAMETER, CradStr);
	// 블록 축척
	CString BScaleStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("BLOCKSCALE"));
	if(BScaleStr.CompareNoCase(_T("")) == 0){
		BScale = 1.0;
		BScaleStr.Format(_T("%g"), BScale);
	}
	SetDlgItemText(IDC_SCALE, BScaleStr);
	// Z값 넣기
	CString InsertZValueStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("INSERTZ"));
	if(InsertZValueStr.CompareNoCase(_T("")) == 0 || InsertZValueStr.CompareNoCase(_T("TRUE")) == 0)
		CheckDlgButton(IDC_ZCHECK, TRUE);
	// 문자 크기 IDC_TEXTSIZE
	CString TextSizeStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("TEXTSIZE"));
	if(TextSizeStr.CompareNoCase(_T("")) == 0){
		TextSize = 2.5;
		TextSizeStr.Format(_T("%g"), TextSize);
	}
	SetDlgItemText(IDC_TEXTSIZE, TextSizeStr);
	// 문자 XOffset IDC_XOFF
	CString TextXOffStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("TEXTXOFFSET"));
	if(TextXOffStr.CompareNoCase(_T("")) == 0){
		TextXOff = 0.0;
		TextXOffStr.Format(_T("%g"), TextXOff);
	}
	SetDlgItemText(IDC_XOFF, TextXOffStr);
	// 문자 YOffset IDC_YOFF
	CString TextYOffStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("TEXTYOFFSET"));
	if(TextYOffStr.CompareNoCase(_T("")) == 0){
		TextYOff = 1.0;
		TextYOffStr.Format(_T("%g"), TextYOff);
	}
	SetDlgItemText(IDC_YOFF, TextYOffStr);
	// Z문자 크기 IDC_TEXTSIZE4
	CString ZTextSizeStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("ZTEXTSIZE"));
	if(ZTextSizeStr.CompareNoCase(_T("")) == 0){
		ZTextSize = 2.5;
		ZTextSizeStr.Format(_T("%g"), ZTextSize);
	}
	SetDlgItemText(IDC_TEXTSIZE4, ZTextSizeStr);
	// Z문자 XOffset IDC_XOFF4
	CString ZTextXOffStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("ZTEXTXOFFSET"));
	if(ZTextXOffStr.CompareNoCase(_T("")) == 0){
		ZTextXOff = 0.0;
		ZTextXOffStr.Format(_T("%g"), ZTextXOff);
	}
	SetDlgItemText(IDC_XOFF4, ZTextXOffStr);
	// Z문자 Yoffset IDC_YOFF4
	CString ZTextYOffStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("ZTEXTYOFFSET"));
	if(ZTextYOffStr.CompareNoCase(_T("")) == 0){
		ZTextYOff = 0.0;
		ZTextYOffStr.Format(_T("%g"), ZTextYOff);
	}
	SetDlgItemText(IDC_YOFF4, ZTextYOffStr);
	// Z문자 소수점 IDC_PCNT
	CString ZTextPntStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("ZTEXTYPNT"));
	if(ZTextPntStr.CompareNoCase(_T("")) == 0){
		ZTextPnt = 3;
		ZTextPntStr.Format(_T("%d"), ZTextPnt);
	}
	if (_ttoi(ZTextPntStr) < 0) {
		ZTextPnt = 1;
		ZTextPntStr.Format(_T("%d"), ZTextPnt);
	}
	SetDlgItemText(IDC_PCNT, ZTextPntStr);
	// 텍스트 레이어
	CString TextLayerStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("TEXTLAYER"));
	if(TextLayerStr.CompareNoCase(_T("")) == 0){
		TextLayerStr = "No";
	}
	// 라인 레이어
	CString LineLayerStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("LINELAYER"));
	if(LineLayerStr.CompareNoCase(_T("")) == 0){
		LineLayerStr = "Line";
	}
	// Z값 레이어
	CString ZValueLayerStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("ZVALUELAYER"));
	if(ZValueLayerStr.CompareNoCase(_T("")) == 0){
		ZValueLayerStr = "EL";
	}
	// 점 레이어
	CString PointLayerStr = iniFile.readINI(_T("config.ini"), _T("CIM"), _T("POINTLAYER"));
	if(PointLayerStr.CompareNoCase(_T("")) == 0){
		PointLayerStr = "Point";
	}
	
	int index1 = CTEXTLAYER.SelectString(-1,TextLayerStr);
	int index2 = CCLINELAYER.SelectString(-1,LineLayerStr);
	int index3 = ZVALLAYER.SelectString(-1,ZValueLayerStr);
	int index4 = CUTPNTLAYER.SelectString(-1, PointLayerStr);

	if(index1 != -1) // 텍스트 레이어
	{
		CTEXTLAYER.SetCurSel(index1);
	}
	else
	{
		CTEXTLAYER.SetCurSel(0);
	}
	if(index2 != -1) // 라인 레이어
	{
		CCLINELAYER.SetCurSel(index2);
	}
	else
	{
		CCLINELAYER.SetCurSel(0);
	}
	if(index3 != -1) // Z값 레이어
	{
		ZVALLAYER.SetCurSel(index3);
	}
	else
	{
		ZVALLAYER.SetCurSel(0);
	}
	if(index4 != -1) // 정점 레이어
	{
		CUTPNTLAYER.SetCurSel(index4);
	}
	else
	{
		CUTPNTLAYER.SetCurSel(0);
	}
	
	// 레이어 설정
	//CTEXTLAYER.SetCurSel(CTEXTLAYER.FindString(0, _T("No")));
	//CCLINELAYER.SetCurSel(CCLINELAYER.FindString(0, _T("Line")));
	//ZVALLAYER.SetCurSel(ZVALLAYER.FindString(0, _T("EL")));
	//CUTPNTLAYER.SetCurSel(CUTPNTLAYER.FindString(0, _T("Point")));
	// 엑셀 화면
	CheckDlgButton(IDC_EXCEL, TRUE);
	CWnd *pWnd2 = GetDlgItem(IDC_TEXT);
	CButton *pText = NULL;
	if(pWnd2){
		pText = (CButton*)pWnd2;
		pText->SetCheck(true);
	}
	// X,Y좌표
	CWnd *pWnd3 = GetDlgItem(IDC_XYCHANGE);
	CButton *pXYChange = NULL;
	if(pWnd3){
		pXYChange = (CButton*)pWnd3;
		pXYChange->SetCheck(true);
	}
	// 1번째 열을 문자로 인식
	CWnd* pWnd33 = GetDlgItem(IDC_FIRST_TXT);
	CButton* pFirstTXT = NULL;
	if (pWnd33) {
		pFirstTXT = (CButton*)pWnd33;
		pFirstTXT->SetCheck(true);
	}
	//쉼표
	CWnd *pWnd4 = GetDlgItem(IDC_Colon);
	CButton *pColon = NULL;
	if(pWnd4){
		pColon = (CButton*)pWnd4;
		pColon->SetCheck(true);
	}
	//3DPOLYLINE
	CWnd *pWnd5 = GetDlgItem(IDC_3DPOLY);
	CButton *p3DPoly = NULL;
	if(pWnd5){
		p3DPoly = (CButton*)pWnd5;
		p3DPoly->SetCheck(true);
	}
	//점
	CWnd *pWnd6 = GetDlgItem(IDC_POINTBTN);
	CButton *pPoint = NULL;
	if(pWnd6){
		pPoint = (CButton*)pWnd6;
		pPoint->SetCheck(true);
	}
	//연결선
	CWnd *pWnd7 = GetDlgItem(IDC_CONLINE);
	CButton *pConline = NULL;
	if(pWnd7){
		pConline = (CButton*)pWnd7;
		pConline->SetCheck(true);
	}
	//Z값
	CWnd *pWnd8 = GetDlgItem(IDC_ZVALUE);
	CButton *pZValue = NULL;
	if(pWnd8){
		pZValue = (CButton*)pWnd8;
		pZValue->SetCheck(true);
	}
	//절점
	CWnd *pWnd9 = GetDlgItem(IDC_CUTPOINT);
	CButton *pCutpoint = NULL;
	if(pWnd9){
		pCutpoint = (CButton*)pWnd9;
		pCutpoint->SetCheck(true);
	}

	CTEXTLAYER.GetLBText(CTEXTLAYER.GetCurSel(), TextLayer);
	TextLayerChangeGetColor(TextLayer);
	CCLINELAYER.GetLBText(CCLINELAYER.GetCurSel(), ConLineLayer);
	ConlineLayerChangeGetColor(ConLineLayer);
	ZVALLAYER.GetLBText(ZVALLAYER.GetCurSel(), ZValLayer);
	ZValLayerChangeGetColor(ZValLayer);
	CUTPNTLAYER.GetLBText(CUTPNTLAYER.GetCurSel(), PointLayer);
	PointLayerChangeGetColor(PointLayer);
	CBName.SetCurSel(1);

	//GetDlgItem(IDC_TEXTSIZE)->SetWindowTextW(_T("2.50")); // 문자크기
	//GetDlgItem(IDC_XOFF)->SetWindowTextW(_T("0.00")); // 문자 X_OFFSET
	//GetDlgItem(IDC_YOFF)->SetWindowTextW(_T("1.00")); // 문자 Y_OFFSET
		
	//GetDlgItem(IDC_TEXTSIZE4)->SetWindowTextW(_T("2.50")); // Z값 문자 크기
	//GetDlgItem(IDC_XOFF4)->SetWindowTextW(_T("0.00")); // Z값 문자 X_OFFSET
	//GetDlgItem(IDC_YOFF4)->SetWindowTextW(_T("-1.00")); // Z값 문자 Y_OFFSET
	//GetDlgItem(IDC_PCNT)->SetWindowTextW(_T("3")); // Z값 소수점
	
	//GetDlgItem(IDC_POINTSIZE)->SetWindowTextW(_T("0.50")); // 점 두께
	//GetDlgItem(IDC_DIAMETER)->SetWindowTextW(_T("1.000")); // 원 반지름
	//GetDlgItem(IDC_SCALE)->SetWindowTextW(_T("1.000")); // 블록 축척
	
	GetDlgItem(IDC_PATHBTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_PATHINPUT)->EnableWindow(TRUE);
	
	GetDlgItem(IDC_Colon)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPACE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SET)->EnableWindow(FALSE);
	GetDlgItem(IDC_DIAMETER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_SCALE)->EnableWindow(FALSE);

	Invalidate(TRUE);
	return TRUE;
}

void CCIM::OnBnClickedTextload(){	
	bool chkBtn = IsDlgButtonChecked(IDC_TEXTLOAD);
	if(chkBtn){
		GetDlgItem(IDC_PATHBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_PATHINPUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_Colon)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SET)->EnableWindow(TRUE);
		GetDlgItem(IDC_SETSPACE)->EnableWindow(TRUE);
	}
}

void CCIM::OnBnClickedExcel(){
	bool chkBtn = IsDlgButtonChecked(IDC_EXCEL);
	if(chkBtn){
		GetDlgItem(IDC_PATHBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_PATHINPUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_Colon)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETSPACE)->EnableWindow(FALSE);
	}
}

void CCIM::OnBnClickedCirclebtn(){
	GetDlgItem(IDC_POINTSIZE)->EnableWindow(FALSE);
	GetDlgItem(IDC_DIAMETER)->EnableWindow(TRUE);
	GetDlgItem(IDC_BNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_SCALE)->EnableWindow(FALSE);
}

void CCIM::OnBnClickedPointbtn(){
	GetDlgItem(IDC_POINTSIZE)->EnableWindow(TRUE);
	GetDlgItem(IDC_DIAMETER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_SCALE)->EnableWindow(FALSE);
}

void CCIM::OnBnClickedBlockbtn(){	
	if(CBName.GetCount() != 0){
		GetDlgItem(IDC_POINTSIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_SCALE)->EnableWindow(TRUE);
	}
	else if(CBName.GetCount() == 0){
		CString strMsg;
		strMsg.LoadStringW(IDS_BLOCK_IN_CAD); // "도면상에 블럭이 없습니다."
		AfxMessageBox(strMsg);
		CheckDlgButton(IDC_POINTBTN, true);
		CheckDlgButton(IDC_BLOCKBTN, false);
		GetDlgItem(IDC_POINTSIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCALE)->EnableWindow(FALSE);
	}
}

void CCIM::OnBnClickedSet(){
	GetDlgItem(IDC_SETSPACE)->ShowWindow(TRUE);
}

void CCIM::OnBnClickedSpace(){
	GetDlgItem(IDC_SETSPACE)->ShowWindow(FALSE);
}

void CCIM::OnBnClickedColon(){
	GetDlgItem(IDC_SETSPACE)->ShowWindow(FALSE);
}

void CCIM::OnClickedPathbtn(){
	bool chkExc = IsDlgButtonChecked(IDC_EXCEL);
	bool chkTxt = IsDlgButtonChecked(IDC_TEXTLOAD);
	if(chkExc){
		CFileDialog dlg(true, _T("xlsx"), NULL, OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT, _T("xlsx (*.xlsx)|*.xlsx|"), NULL);
		if(dlg.DoModal() == IDOK)
			SetDlgItemText(IDC_PATHINPUT, dlg.GetPathName());		
	}
	else if(chkTxt){
		CFileDialog dlg(true, _T("txt"), NULL, OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT, _T("txt (*.txt)|*.txt|"), NULL);
		if(dlg.DoModal() == IDOK)
			SetDlgItemText(IDC_PATHINPUT, dlg.GetPathName());
	}	
}

void CCIM::SelectTextColorChange(int colorIndex){
	CString colorIndexStr;
	colorIndexStr.Format(_T("%d"), colorIndex);
	SetDlgItemTextW(IDC_TCOLOR, colorIndexStr);
	TextPicColor.SetColor(colorIndex);
}

void CCIM::SelectZvalColorChange(int colorIndex){
	CString colorIndexStr;
	colorIndexStr.Format(_T("%d"), colorIndex);
	SetDlgItemTextW(IDC_ZVALCOLOR, colorIndexStr);
	ZValPicColor.SetColor(colorIndex);
}

void CCIM::SelectConlineColorChange(int colorIndex){
	CString colorIndexStr;
	colorIndexStr.Format(_T("%d"), colorIndex);
	SetDlgItemTextW(IDC_CONCOLOR, colorIndexStr);
	ConPicColor.SetColor(colorIndex);
}

void CCIM::SelectPointColorChange(int colorIndex){
	CString colorIndexStr;
	colorIndexStr.Format(_T("%d"), colorIndex);
	SetDlgItemTextW(IDC_POINTCOLOR, colorIndexStr);
	PointPicColor.SetColor(colorIndex);
}

void CCIM::TextLayerChangeGetColor(CString layerName){
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);

	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		
		int colorIndex;
		AcCmColor color;
		pLayerTblRcd->getName(name);
		color = pLayerTblRcd->color();
		colorIndex = color.colorIndex();		
		if(layerName.CompareNoCase(name) == 0)
			SelectTextColorChange(colorIndex);
	}
	delete pLayerTblIter;
	delete pLayerTbl;
}

void CCIM::ZValLayerChangeGetColor(CString layerName){
	AcDbLayerTable* pLayerTbl = NULL;
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
		if(layerName.CompareNoCase(name) == 0)
			SelectZvalColorChange(colorIndex);
	}
	delete pLayerTblIter;
	delete pLayerTbl;
}

void CCIM::ConlineLayerChangeGetColor(CString layerName){
	AcDbLayerTable* pLayerTbl = NULL;
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
		if(layerName.CompareNoCase(name) == 0)
			SelectConlineColorChange(colorIndex);
	}
	delete pLayerTblIter;
	delete pLayerTbl;
}

void CCIM::PointLayerChangeGetColor(CString layerName){
	AcDbLayerTable* pLayerTbl = NULL;
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
		if(layerName.CompareNoCase(name) == 0)
			SelectPointColorChange(colorIndex);
	}
	delete pLayerTblIter;
	delete pLayerTbl;
}

void CCIM::OnBnClickedTextbtn(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();
	
	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();
	
	if(acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor) != Acad::eOk){
		SelectTextColorChange(selectColor);
		TextColIndex = selectColor;
	}
}

void CCIM::OnBnClickedZvalcolorbtn(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();
	
	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();
	
	if(acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor) != Acad::eOk){
		SelectZvalColorChange(selectColor);
		ZvalColorIndex = selectColor;
	}
}

void CCIM::OnBnClickedPointcolorbtn(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();
	
	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();
	
	if(acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor) != Acad::eOk){
		SelectPointColorChange(selectColor);
		PointColIndex = selectColor;
	}	
}

void CCIM::OnBnClickedConlinecolorbtn(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();
	
	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();
	
	if(acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor) != Acad::eOk){
		SelectConlineColorChange(selectColor);
		ConlineColIndex = selectColor;
	}
}

void CCIM::OnBnClickedText(){
	bool chk = CText.GetCheck();
	if(chk){
		GetDlgItem(IDC_TEXTLAYER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SELTEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_TEXTSIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_XOFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_YOFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_TEXTBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_TCPICCOLOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_TCOLOR)->EnableWindow(TRUE);
	}
	else if(!chk){
		GetDlgItem(IDC_TEXTLAYER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SELTEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_TEXTSIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_XOFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_YOFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_TEXTBTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_TCPICCOLOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_TCOLOR)->EnableWindow(FALSE);
	}
}

void CCIM::OnBnClickedConline(){
	bool chk = CConline.GetCheck();
	if(chk){
		GetDlgItem(IDC_CONLINELAYER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SELCONLINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_2DPOLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_3DPOLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CONLINECOLORBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CONLINEPICCOLOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_CONCOLOR)->EnableWindow(TRUE);		
	}
	else if(!chk){
		GetDlgItem(IDC_CONLINELAYER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SELCONLINE)->EnableWindow(FALSE);
		GetDlgItem(IDC_2DPOLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_3DPOLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_CONLINECOLORBTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CONLINEPICCOLOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CONCOLOR)->EnableWindow(FALSE);		
	}	
}

void CCIM::OnBnClickedCutpoint(){
	bool chk = BCutpoint.GetCheck();
	if(chk){
		GetDlgItem(IDC_CUTPOINTLAYER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SELPOINT)->EnableWindow(TRUE);
		GetDlgItem(IDC_POINTBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DIAMETER)->EnableWindow(TRUE);
		GetDlgItem(IDC_BNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_SCALE)->EnableWindow(TRUE);		
		GetDlgItem(IDC_CIRCLEBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BLOCKBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_POINTCOLORBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_POINTPICCOLOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_POINTCOLOR)->EnableWindow(TRUE);
	}
	else if(!chk){
		GetDlgItem(IDC_CUTPOINTLAYER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SELPOINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_POINTBTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCALE)->EnableWindow(FALSE);		
		GetDlgItem(IDC_CIRCLEBTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BLOCKBTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_POINTCOLORBTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_POINTPICCOLOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_POINTCOLOR)->EnableWindow(FALSE);
	}
}

void CCIM::OnBnClickedZvalue(){
	bool chk = BZIns.GetCheck();
	if(chk){
		GetDlgItem(IDC_ZVALUELAYER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SELZVALUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_TEXTSIZE4)->EnableWindow(TRUE);
		GetDlgItem(IDC_PCNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MINUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_PLUS)->EnableWindow(TRUE);		
		GetDlgItem(IDC_XOFF4)->EnableWindow(TRUE);
		GetDlgItem(IDC_YOFF4)->EnableWindow(TRUE);
		GetDlgItem(IDC_ZVALCOLORBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_ZVALPICCOLOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_ZVALCOLOR)->EnableWindow(TRUE);
	}
	else if(!chk){
		GetDlgItem(IDC_ZVALUELAYER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SELZVALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_TEXTSIZE4)->EnableWindow(FALSE);
		GetDlgItem(IDC_PCNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_MINUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_PLUS)->EnableWindow(FALSE);		
		GetDlgItem(IDC_XOFF4)->EnableWindow(FALSE);
		GetDlgItem(IDC_YOFF4)->EnableWindow(FALSE);
		GetDlgItem(IDC_ZVALCOLORBTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_ZVALPICCOLOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_ZVALCOLOR)->EnableWindow(FALSE);
	}
}

void CCIM::OnCbnSelchangeTextlayer(){
	CTEXTLAYER.GetLBText(CTEXTLAYER.GetCurSel(), TextLayer);
	TextLayerChangeGetColor(TextLayer);
}

void CCIM::OnCbnSelchangeConlinelayer(){
	CCLINELAYER.GetLBText(CCLINELAYER.GetCurSel(), ConLineLayer);
	ConlineLayerChangeGetColor(ConLineLayer);
}

void CCIM::OnBnClickedMinus(){
	int Pcnt = GetDlgItemInt(IDC_PCNT);
	if(Pcnt != 0)
		Pcnt--;

	SetDlgItemInt(IDC_PCNT, Pcnt);
}

void CCIM::OnBnClickedPlus(){
	int Pcnt = GetDlgItemInt(IDC_PCNT);
	Pcnt++;
	SetDlgItemInt(IDC_PCNT, Pcnt);
}

void CCIM::OnCbnSelchangeZvaluelayer(){
	ZVALLAYER.GetLBText(ZVALLAYER.GetCurSel(), ZValLayer);
	ZValLayerChangeGetColor(ZValLayer);
}

void CCIM::OnCbnSelchangeCutpointlayer(){
	CUTPNTLAYER.GetLBText(CUTPNTLAYER.GetCurSel(), PointLayer);
	PointLayerChangeGetColor(PointLayer);
}

void CCIM::OnBnClickedSelconline(){
	BeginEditorCommand();
	ads_point p1;
	ads_name n1;
	AcDbObjectId EntId;
	CString strMsg;

	strMsg.LoadStringW(IDS_SELECT_ENTITY); // "객체 선택: "
	if(acedEntSel(strMsg, n1, p1) != RTNORM)
		CompleteEditorCommand();

	if(acdbGetObjectId(EntId, n1) == Acad::eOk){
		AcDbEntityPointer pEnt(EntId, AcDb::kForRead);
		ConLineLayer = pEnt->layer();
		ConlineColIndex = pEnt->colorIndex();
		CCLINELAYER.SetCurSel(CCLINELAYER.FindString(1, ConLineLayer));
		if(ConlineColIndex == 256)
			ConlineLayerChangeGetColor(ConLineLayer);
		else
			SelectConlineColorChange(ConlineColIndex);
	}
	CompleteEditorCommand();
}

void CCIM::OnBnClickedSeltext(){
	BeginEditorCommand();
	ads_point p1;
	ads_name n1;
	AcDbObjectId EntId;
	CString strMsg;

	strMsg.LoadStringW(IDS_SELECT_ENTITY); // "객체 선택: "
	if(acedEntSel(strMsg, n1, p1) != RTNORM)
		CompleteEditorCommand();
	
	if(acdbGetObjectId(EntId, n1) == Acad::eOk){
		AcDbEntityPointer pEnt(EntId, AcDb::kForRead);
		TextLayer = pEnt->layer();
		TextColIndex = pEnt->colorIndex();
		CTEXTLAYER.SetCurSel(CTEXTLAYER.FindString(0, TextLayer));
		if(TextColIndex == 256)
			TextLayerChangeGetColor(TextLayer);
		else
			SelectTextColorChange(TextColIndex);
	}
	CompleteEditorCommand();
}

void CCIM::OnBnClickedSelzvalue(){
	BeginEditorCommand();
	ads_point p1;
	ads_name n1;
	AcDbObjectId EntId;
	CString strMsg;

	strMsg.LoadStringW(IDS_SELECT_ENTITY); // "객체 선택: "
	if(acedEntSel(strMsg, n1, p1) != RTNORM)
		CompleteEditorCommand();

	if(acdbGetObjectId(EntId, n1) == Acad::eOk){
		AcDbEntityPointer pEnt(EntId, AcDb::kForRead);
		ZValLayer = pEnt->layer();
		ZvalColorIndex = pEnt->colorIndex();
		ZVALLAYER.SetCurSel(ZVALLAYER.FindString(0, ZValLayer));
		if(ZvalColorIndex == 256)
			ZValLayerChangeGetColor(ZValLayer);
		else
			SelectZvalColorChange(ZvalColorIndex);
	}
	CompleteEditorCommand();
}

void CCIM::OnBnClickedSelpoint(){
	BeginEditorCommand();
	ads_point p1;
	ads_name n1;
	AcDbObjectId EntId;
	CString strMsg;

	strMsg.LoadStringW(IDS_SELECT_ENTITY); // "객체 선택: "
	if(acedEntSel(strMsg, n1, p1) != RTNORM)
		CompleteEditorCommand();

	if(acdbGetObjectId(EntId, n1) == Acad::eOk){
		AcDbEntityPointer pEnt(EntId, AcDb::kForRead);
		PointLayer = pEnt->layer();
		PointColIndex = pEnt->colorIndex();
		CUTPNTLAYER.SetCurSel(CUTPNTLAYER.FindString(0, PointLayer));
		if(PointColIndex == 256)
			PointLayerChangeGetColor(PointLayer);
		else
			SelectPointColorChange(PointColIndex);
	}
	CompleteEditorCommand();
}

void CCIM::OnBnClickedCimHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/CIM.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
