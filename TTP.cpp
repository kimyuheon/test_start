#include "stdafx.h"
#include "TTP.h"
#include "afxdialogex.h"

// CTTP 대화 상자입니다.
IMPLEMENT_DYNAMIC(CTTP, CAcUiDialog)
	CTTP::CTTP(CWnd* pParent)
	: CAcUiDialog(CTTP::IDD, pParent), m_xValueStr(_T(""))
	, m_yValueStr(_T("")){
}

CTTP::~CTTP(){
}

void CTTP::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDTXOFFSET, m_xValueCtrl);
	DDX_Control(pDX, IDC_EDTYOFFSET, m_yValueCtrl);
	DDX_Text(pDX, IDC_EDTXOFFSET, m_xValueStr);
	DDX_Text(pDX, IDC_EDTYOFFSET, m_yValueStr);
	DDX_Control(pDX, IDC_CBXLAYER, m_cbxLayer);
	DDX_Control(pDX, IDC_BSELECT, m_bSelect);
}

BEGIN_MESSAGE_MAP(CTTP, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CTTP::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTTP::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADINSERTPOINT, &CTTP::OnBnClickedRadinsertpoint)
	ON_BN_CLICKED(IDC_RADALIGNPOINT, &CTTP::OnBnClickedRadalignpoint)
	ON_EN_UPDATE(IDC_EDTXOFFSET, &CTTP::OnEnUpdateEdtxoffset)
	ON_EN_UPDATE(IDC_EDTYOFFSET, &CTTP::OnEnUpdateEdtyoffset)
	ON_BN_CLICKED(IDC_BSELECT, &CTTP::OnBnClickedBselect)
	ON_BN_CLICKED(ID_TTP_HELP, &CTTP::OnBnClickedTtpHelp)
END_MESSAGE_MAP()

//초기화
BOOL CTTP::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	//아이콘
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	//버튼 이미지
	m_bSelect.AutoLoad();

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
		m_cbxLayer.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;

	m_cbxLayer.SetCurSel(0);

	//옵션 초기화
	CString opStr;
	opStr = iniFile.readINI(_T("config.ini"), _T("TTP"), _T("MODE"));	
	if(opStr.CompareNoCase(_T("")) == 0 || opStr.CompareNoCase(_T("1")) == 0)
		CheckDlgButton(IDC_RADINSERTPOINT, TRUE);
	else
		CheckDlgButton(IDC_RADALIGNPOINT, TRUE);

	mode = _wtoi(opStr);//모드값 지정
	opStr = iniFile.readINI(_T("config.ini"), _T("TTP"), _T("OFFSETX"));
	if(opStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_EDTXOFFSET, _T("0"));
	else
		SetDlgItemText(IDC_EDTXOFFSET, opStr);

	opStr = iniFile.readINI(_T("config.ini"), _T("TTP"), _T("OFFSETY"));
	if(opStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_EDTYOFFSET, _T("0"));
	else
		SetDlgItemText(IDC_EDTYOFFSET, opStr);

	UpdateData(FALSE);
	return TRUE;	
}

void CTTP::OnBnClickedOk(){	
	UpdateData();
	CAcUiDialog::OnOK();

	ads_name ens;
	// [+] SUP-29742
	// long	len;
	// [-] END
	Adesk::Int32 len;
	resbuf *filter = NULL;
	filter = ads_buildlist(RTDXF0, _T("TEXT"), RTNONE);

	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	CString strMsg;
	strMsg.LoadStringW(IDS_SELECT_ENTITY);
	const wchar_t* prompts[2] = { strMsg, L"", };
	//if(acedSSGet(NULL, NULL, NULL, filter, ens) != RTNORM){
	if(acedSSGet(L":$", prompts, NULL, filter, ens) != RTNORM){
		acedSSFree(ens);
		acutRelRb(filter);
		CancelEditorCommand();
		return;
	}
	if(acedSSLength(ens, &len) != RTNORM){
		acedSSFree(ens);
		acutRelRb(filter);
		CancelEditorCommand();
		return;
	}
	acutRelRb(filter);
	CompleteEditorCommand();

	for(int i = 0; i < len; i++){
		AcGePoint3d insertPoint;
		ads_name en;
		acedSSName(ens, i, en);
		AcDbObjectId objId;
		acdbGetObjectId(objId, en);
		AcDbEntityPointer pEnt(objId, AcDb::kForRead);
		if(pEnt.openStatus() == Acad::eOk){
			if(pEnt->isKindOf(AcDbText::desc())){
				AcDbText* pText = AcDbText::cast(pEnt);
				CString inString;
				inString.Format(_T("%s"), pText->textString());
				inString = CheckInputNumber(inString);
				if(mode == 1){
					insertPoint.x = pText->position().x + _wtof(m_xValueStr);
					insertPoint.y = pText->position().y + _wtof(m_yValueStr);
					insertPoint.z = pText->position().z;
				}
				else if(mode == 2){
					insertPoint.x = pText->alignmentPoint().x + _wtof(m_xValueStr);
					insertPoint.y = pText->alignmentPoint().y + _wtof(m_yValueStr);
					insertPoint.z = pText->alignmentPoint().z;
				}
				//문자 정렬점으로 자리맞추기 하지않고 일반적으로 작성한 텍스트는
				//원점에 점이 찍히기 때문에 x,y,z가 원점이면 문자 삽입점에 점 생성을 함.
				if(mode == 2 && pText->alignmentPoint().x == 0 && pText->alignmentPoint().y == 0 && pText->alignmentPoint().z == 0){
					insertPoint.x = pText->position().x;
					insertPoint.y = pText->position().y;
					insertPoint.z = pText->position().z;
				}
				CreatePoint(inString, insertPoint);
				pText->close();
			}
		}
		pEnt->close();
	}

	acedSSFree(ens);
	CString modeStr;
	modeStr.Format(_T("%d"), mode);
	iniFile.writeINI(_T("config.ini"), _T("TTP"), _T("MODE"), modeStr);
	iniFile.writeINI(_T("config.ini"), _T("TTP"), _T("OFFSETX"), m_xValueStr);
	iniFile.writeINI(_T("config.ini"), _T("TTP"), _T("OFFSETY"), m_yValueStr);
}

void CTTP::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

void CTTP::OnBnClickedRadinsertpoint(){
	mode = 1;
	iniFile.writeINI(_T("config.ini"), _T("TTP"), _T("MODE"), _T("1"));
}

void CTTP::OnBnClickedRadalignpoint(){
	mode = 2;
	iniFile.writeINI(_T("config.ini"), _T("TTP"), _T("MODE"), _T("2"));
}

void CTTP::OnEnUpdateEdtxoffset(){
	UpdateData(TRUE);
	//m_xValueStr = CheckInputNumber(m_xValueStr);
	UpdateData(FALSE);

	//// [ 1. initialize value ]
	CString cstrText;
	m_xValueCtrl.GetWindowTextW(cstrText);
	char chText[100] = { 0, };
	strcpy(chText, LPSTR(LPCTSTR(cstrText)));
	//chText = LPSTR(LPCTSTR(cstrText));
	int count = strlen(chText);
	
	//// [ 2. remove second point ]
	bool pointChecker = false;
	for (int i = 0; i < count; i++) {
		if (!pointChecker && chText[i] == '.') pointChecker = true;
		else if (pointChecker && chText[i] == '.') chText[i] = NULL;
	}
	
	//// [ 3. remove middle of minuse ]
	int startIdx = (true) ? 1 : 0;
	for (int i = startIdx; i < count; i++) {
		if (chText[i] == '-') chText[i] = NULL;
	}

	//// [ 4. remove character ]
	for (int i = 0; i < count; i++) {
		if (!(chText[i] >= 48 && chText[i] <= 57)) {
			if (chText[i] != '.' && chText[i] != '-') chText[i] = NULL;
		}
	}

	m_xValueCtrl.SetSel(0, -1);
	m_xValueCtrl.SetSel(-1, -1);

	UpdateData(TRUE);
}

void CTTP::OnEnUpdateEdtyoffset(){
	UpdateData(TRUE);
	//m_yValueStr = CheckInputNumber(m_yValueStr);
	UpdateData(FALSE);

	//// [ 1. initialize value ]
	CString cstrText;
	m_yValueCtrl.GetWindowTextW(cstrText);
	char chText[100] = { 0, };
	strcpy(chText, LPSTR(LPCTSTR(cstrText)));
	//chText = LPSTR(LPCTSTR(cstrText));
	int count = strlen(chText);
	
	//// [ 2. remove second point ]
	bool pointChecker = false;
	for (int i = 0; i < count; i++) {
		if (!pointChecker && chText[i] == '.') pointChecker = true;
		else if (pointChecker && chText[i] == '.') chText[i] = NULL;
	}
	
	//// [ 3. remove middle of minuse ]
	int startIdx = (true) ? 1 : 0;
	for (int i = startIdx; i < count; i++) {
		if (chText[i] == '-') chText[i] = NULL;
	}

	//// [ 4. remove character ]
	for (int i = 0; i < count; i++) {
		if (!(chText[i] >= 48 && chText[i] <= 57)) {
			if (chText[i] != '.' && chText[i] != '-') chText[i] = NULL;
		}
	}

	m_yValueCtrl.SetSel(0, -1);
	m_yValueCtrl.SetSel(-1, -1);

	UpdateData(TRUE);
}

//숫자 및 점(.) 입력 체크
CString CTTP::CheckInputNumber(CString str){
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

//점 생성
void CTTP::CreatePoint(CString zValue, AcGePoint3d pos){
	acutPrintf(_T("\n%g, %g, %g"), pos.x, pos.y, _ttof(zValue));
	CString curLayer;
	m_cbxLayer.GetLBString(m_cbxLayer.GetCurSel(), curLayer);	
	AcDbPoint* pPoint = new AcDbPoint;
	pPoint->setPosition(AcGePoint3d(pos.x, pos.y, _ttof(zValue)));
	pPoint->setLayer((LPCTSTR)curLayer);

	AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
	acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	pPoint->setDatabaseDefaults();
	blkRec->appendAcDbEntity(pPoint);
	pPoint->close();
	blkRec->close();
}

void CTTP::OnBnClickedBselect(){
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
	acedEntSel(strMsg, en, pt);
	CompleteEditorCommand();

	if(en[0] == 0 || en[0] == NULL)
		return;

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
	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		pLayerTblRcd->getName(name);
		if(0 == _tcscmp(selectLayerName, name)){
			m_cbxLayer.SetCurSel(cbxIndex);
			break;
		}
		cbxIndex++;
	}
	delete pLayerTblIter;
	delete pLayerTbl;
}

void CTTP::OnBnClickedTtpHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/TTP.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
