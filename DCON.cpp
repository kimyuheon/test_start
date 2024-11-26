// DCON.cpp : 구현 파일입니다.

#include "stdafx.h"
#include "DCON.h"
#include "afxdialogex.h"

// CDCON 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDCON, CAcUiDialog)

CDCON::CDCON(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CDCON::IDD, pParent)
	, m_BaseStr(_T("")){
}

CDCON::~CDCON(){
}

void CDCON::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_BASE, m_BaseCtrl);
	DDX_Text(pDX, IDC_EDT_BASE, m_BaseStr);
}

BEGIN_MESSAGE_MAP(CDCON, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CDCON::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_COLOR, &CDCON::OnBnClickedBtnColor)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_BRIGHT, &CDCON::OnDeltaposSpnBright)
	ON_BN_CLICKED(IDC_CHK_LAYERCOLOR, &CDCON::OnBnClickedChkLayercolor)
	ON_EN_UPDATE(IDC_EDT_BASE, &CDCON::OnEnUpdateEdtBase)
	ON_EN_UPDATE(IDC_EDT_BRIGHT, &CDCON::OnEnUpdateEdtBright)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(ID_DCON_HELP, &CDCON::OnBnClickedDconHelp)
END_MESSAGE_MAP()

// CDCON 메시지 처리기입니다.
BOOL CDCON::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	
	baseStr = iniFile.readINI(_T("config.ini"), _T("DCON"), _T("HEIGHTBASE"));
	if(baseStr.CompareNoCase(_T("")) == 0)
		baseStr = _T("25");
	SetDlgItemText(IDC_EDT_BASE, baseStr);

	layerHeaderStr = iniFile.readINI(_T("config.ini"), _T("DCON"), _T("LAYERHEADER"));
	if(layerHeaderStr.CompareNoCase(_T("")) == 0)
		layerHeaderStr = _T("Con-");
	SetDlgItemText(IDC_EDT_LAYERNAME, layerHeaderStr);
	
	layerColorChkStr = iniFile.readINI(_T("config.ini"), _T("DCON"), _T("LAYERCOLORCHANGE"));
	if(layerColorChkStr.CompareNoCase(_T("")) == 0 || layerColorChkStr.CompareNoCase(_T("TRUE")) == 0){
		layerColorChkStr = _T("TRUE");
		chkLayerColor = true;
		((CButton*)GetDlgItem(IDC_CHK_LAYERCOLOR))->SetCheck(1);
		GetDlgItem(IDC_BTN_COLOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_BRIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPN_BRIGHT)->EnableWindow(TRUE);
	}
	else{
		layerColorChkStr = _T("FALSE");
		chkLayerColor = false;
		((CButton*)GetDlgItem(IDC_CHK_LAYERCOLOR))->SetCheck(0);
		GetDlgItem(IDC_BTN_COLOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_BRIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPN_BRIGHT)->EnableWindow(FALSE);
	}

	redColorStr = iniFile.readINI(_T("config.ini"), _T("DCON"), _T("RCOLOR"));
	if(redColorStr.CompareNoCase(_T("")) == 0)
		redColorStr = _T("255");
	greenColorStr = iniFile.readINI(_T("config.ini"), _T("DCON"), _T("GCOLOR"));
	if(greenColorStr.CompareNoCase(_T("")) == 0)
		greenColorStr = _T("255");
	blueColorStr = iniFile.readINI(_T("config.ini"), _T("DCON"), _T("BCOLOR"));
	if(blueColorStr.CompareNoCase(_T("")) == 0)
		blueColorStr = _T("255");
	
	UInt8 URec, UGreen, UBlue, aci;
	URec = _ttoi(redColorStr);
	UGreen = _ttoi(greenColorStr);
	UBlue = _ttoi(blueColorStr);
#ifdef _ZCAD
	aci = AcCmEntityColor::lookUpACI(URec, UGreen, UBlue);
	btnAfterClr = aci;
#elif _ACAD
	btnAfterClr = RGB((BYTE*)(LPCTSTR)redColorStr, (BYTE*)(LPCTSTR)greenColorStr, (BYTE*)(LPCTSTR)blueColorStr);
#endif
	colorBrightStr = iniFile.readINI(_T("config.ini"), _T("DCON"), _T("BRIGHT"));
	if(colorBrightStr.CompareNoCase(_T("")) == 0)
		colorBrightStr = _T("2");

	SetDlgItemInt(IDC_EDT_BRIGHT, _ttoi(colorBrightStr));
	SelectColorChange(btnAfterClr);
	return TRUE;
}

void CDCON::OnBnClickedOk(){
	UpdateData(TRUE);
	GetDlgItemText(IDC_EDT_LAYERNAME, layerHeaderStr);
	iniFile.writeINI(_T("config.ini"), _T("DCON"), _T("HEIGHTBASE"), baseStr);
	iniFile.writeINI(_T("config.ini"), _T("DCON"), _T("LAYERHEADER"), layerHeaderStr);
	iniFile.writeINI(_T("config.ini"), _T("DCON"), _T("LAYERCOLORCHANGE"), layerColorChkStr);
	iniFile.writeINI(_T("config.ini"), _T("DCON"), _T("RCOLOR"), redColorStr);
	iniFile.writeINI(_T("config.ini"), _T("DCON"), _T("GCOLOR"), greenColorStr);
	iniFile.writeINI(_T("config.ini"), _T("DCON"), _T("BCOLOR"), blueColorStr);
	iniFile.writeINI(_T("config.ini"), _T("DCON"), _T("BRIGHT"), colorBrightStr);
	OnOK();
	//트루컬러 불러오기	
	HSL data = RGBToHSL(WKRGB(_ttoi(redColorStr), _ttoi(greenColorStr), _ttoi(blueColorStr)));
	data.L = 0.50f;        //광도 수정 
	WKRGB value = HSLToRGB(data);
	CString strMsg;

	baseHeight = _ttof(baseStr);

	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	ads_name ss;

	resbuf* filter = ads_buildlist(RTDXF0, _T("LWPOLYLINE"), NULL);
	strMsg.LoadStringW(IDS_SELECT_ENTITY);
	const wchar_t* prompts[2] = { strMsg, L"", };
	//if (acedSSGet(NULL, NULL, NULL, filter, ss) != RTNORM) {
	if(acedSSGet(L":$", prompts, NULL, filter, ss) != RTNORM){
		acutRelRb(filter);
		return;
	}
	acutRelRb(filter);
	
	// [+] SUP-29742
	// long	len;
	// [-] END
	Adesk::Int32 len;
	if(acedSSLength(ss, &len) != RTNORM || len == 0){
		strMsg.LoadStringW(IDS_SELECT_CANCEL); // "\n선택 취소!"
		acutPrintf(strMsg);
		CancelEditorCommand();
	}
	else{
		AcDbObjectIdArray ids;
		CStringArray elevations;
		for(int i = 0; i < len; i++){
			ads_name en;
			acedSSName(ss, i, en);
			AcDbObjectId objId;
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);

			if(pEnt->isKindOf(AcDbPolyline::desc())){
				AcDbPolyline* pPline = AcDbPolyline::cast(pEnt);
				double elevation = pPline->elevation();
				ids.append(pPline->id());
				CString temp;
				temp.Format(_T("%g"), elevation);
				elevations.Add(temp);
			}
			pEnt->close();
		}

		//정렬
		int i, j;
		for(i = 1; i < len; i++){
			CString temp = elevations.GetAt(i);
			AcDbObjectId tempId = ids.at(i);
			for(j = i - 1; j >= 0 && _ttof(elevations.GetAt(j)) > _ttof(temp); j--){
				elevations.SetAt(j + 1, elevations.GetAt(j));
				ids.setAt(j + 1, ids.at(j));
			}
			elevations.SetAt(j + 1, temp);
			ids.setAt(j + 1, tempId);
		}
		
		//도면층 생성 카운팅
		//라인타입 로드
		AcDbObjectId lineTypeId;
		acdbLoadLineTypeFile(_T("Continuous"), _T("acadiso.lin"), acdbHostApplicationServices()->workingDatabase());

		AcDbLinetypeTable *pLinetypeTable; 
		es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLinetypeTable, AcDb::kForRead); 
		if(es == Acad::eOk)
			pLinetypeTable->getAt(_T("Continuous"), lineTypeId, Adesk::kTrue); 	

		pLinetypeTable->close(); 
				
		int cr_Layer_cnt = 0;
		AcCmColor cm;
		CString newLayer;
		AcDbObjectId layerId;
		double baseNum = _ttof(elevations.GetAt(0));
		double calcNum = _ttof(baseStr);
		for(int i = 0; i < len; i++){
			if(baseNum <= _ttof(elevations.GetAt(i))){
				AcDbLayerTable* layerTbl;
				acdbHostApplicationServices()->workingDatabase()->getSymbolTable(layerTbl, AcDb::kForWrite);
				AcDbLayerTableRecord *pLayerTableRecord = new AcDbLayerTableRecord;			
				cr_Layer_cnt++;
				newLayer.Format(_T("%s-%g"), layerHeaderStr, baseNum);				
				pLayerTableRecord->setName(newLayer);
				pLayerTableRecord->setIsFrozen(0);
				pLayerTableRecord->setIsOff(0);
				pLayerTableRecord->setVPDFLT(0);
				pLayerTableRecord->setIsLocked(0);
				pLayerTableRecord->setLinetypeObjectId(lineTypeId);
				if(chkLayerColor == true)
				{
					cm.setRGB(value.R, value.G, value.B);
					pLayerTableRecord->setColor(cm);
				}
				layerTbl->add(pLayerTableRecord);
				layerId = pLayerTableRecord->id();
				pLayerTableRecord->close();
				layerTbl->close();
				
				baseNum = baseNum + calcNum;
				data.L = data.L - (_ttof(colorBrightStr) / 100);
				value = HSLToRGB(data);
			}
			AcDbEntityPointer pEnt(ids.at(i), AcDb::kForWrite);
			AcDbPolyline* pPline = AcDbPolyline::cast(pEnt);
			pPline->setLayer(layerId);
			pPline->close();
			pEnt->close();
		}
	}
	CompleteEditorCommand();
}

//색상 버튼 클릭시
void CDCON::OnBnClickedBtnColor(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();

	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();

	acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor);
	SelectColorChange(selectColor);
}

//색상 변경
void CDCON::SelectColorChange(int colorIndex){
	CString colorIndexStr, strMsg;
	colorIndexStr.Format(_T("%d"), colorIndex);
	if(colorIndexStr.CompareNoCase(_T("0")) == 0){
		colorIndexStr.Format(_T("By Block"));
	}
	else if(colorIndexStr.CompareNoCase(_T("256")) == 0){
		colorIndexStr.Format(_T("By Layer"));
	}
	btnAfterClr = acedGetRGB(colorIndex);

	redColorStr.Format(_T("%lu"), (ULONG)GetRValue(acedGetRGB(colorIndex)));
	greenColorStr.Format(_T("%lu"), (ULONG)GetGValue(acedGetRGB(colorIndex)));
	blueColorStr.Format(_T("%lu"), (ULONG)GetBValue(acedGetRGB(colorIndex)));
	setColorIdx = colorIndex;
	Invalidate();
}

//높이별 도면층 구분 체크
void CDCON::OnBnClickedChkLayercolor(){
	if(IsDlgButtonChecked(IDC_CHK_LAYERCOLOR)){
		chkLayerColor = true;
		layerColorChkStr.Format(_T("TRUE"));
		GetDlgItem(IDC_BTN_COLOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_BRIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPN_BRIGHT)->EnableWindow(TRUE);
	}
	else{
		chkLayerColor = false;
		layerColorChkStr.Format(_T("FALSE"));
		GetDlgItem(IDC_BTN_COLOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_BRIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPN_BRIGHT)->EnableWindow(FALSE);
	}
}

void CDCON::OnEnUpdateEdtBase(){
	UpdateData(TRUE);
	baseStr = CheckInputNumber(m_BaseStr);
	UpdateData(FALSE);
	m_BaseCtrl.SetSel(0, -1);
	m_BaseCtrl.SetSel(-1, -1);
	UpdateData(TRUE);
	baseHeight = _ttof(baseStr);
}

//숫자 및 점(.) 입력 체크
CString CDCON::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	for(int i = 0; i < str.GetLength(); i++){
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i, 1);
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46))
			str.Remove(*getAtChar);
	}
	return str;
}

//명도 값 버튼 클릭시
void CDCON::OnDeltaposSpnBright(NMHDR *pNMHDR, LRESULT *pResult){
	GetDlgItemText(IDC_EDT_BRIGHT, colorBrightStr);
	int brightInt = _ttoi(colorBrightStr);
	
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta <= 0){
		brightInt++;
	}
	else{
		brightInt--;
	}
	colorBrightStr.Format(_T("%d"), brightInt);
	SetDlgItemText(IDC_EDT_BRIGHT, colorBrightStr);
	*pResult = 0;
}

void CDCON::OnEnUpdateEdtBright(){
	int value;
	value = GetDlgItemInt(IDC_EDT_BRIGHT);
	if(value <= 0){
		value = 1;
		colorBrightStr.Format(_T("1"));
		SetDlgItemText(IDC_EDT_BRIGHT, colorBrightStr);
	}
	else if(11 <= value){
		value = 10;
		colorBrightStr.Format(_T("10"));
		SetDlgItemText(IDC_EDT_BRIGHT, colorBrightStr);
	}
}

void CDCON::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct){
	if((nIDCtl == IDC_BTN_COLOR)){
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct->hDC);						//버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;						//버튼영역 구하기
		dc.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect, btnAfterClr);					//버튼색상
		UINT state = lpDrawItemStruct->itemState;				//버튼상태구하기
		if((state &ODS_SELECTED))
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		else
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
	}
	CAcUiDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDCON::OnBnClickedDconHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/DCON.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
