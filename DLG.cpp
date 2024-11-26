// DLG.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "DLG.h"
#include "afxdialogex.h"

// CDLG 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDLG, CDialogEx)
	CDLG::CDLG(CWnd* pParent)
	: CAcUiDialog(CDLG::IDD, pParent), m_GridColumnDistStr(_T(""))
	, m_VScaleStr(_T("")){
	#ifdef _ACAD
		LinetypeStr = L"acad.lin";
	#elif _ZCAD
		LinetypeStr = L"zwcad.lin";
	#elif _ZXYCAD
		LinetypeStr = L"ZYXcad.lin";
	#endif
}

CDLG::~CDLG(){
}

void CDLG::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_GRIDDIST, m_GridColumnDistCtr);
	DDX_Text(pDX, IDC_EDT_GRIDDIST, m_GridColumnDistStr);
	DDX_Control(pDX, IDC_EDT_VSCALE, m_VScaleCtr);
	DDX_Control(pDX, IDC_EDT_LINESCALE, m_LineScaleCtr);
	DDX_Text(pDX, IDC_EDT_LINESCALE, m_LineScaleStr);
	DDX_Text(pDX, IDC_EDT_VSCALE, m_VScaleStr);
	DDX_Control(pDX, IDC_CBX_LAYER, m_layerCbx);
}

BEGIN_MESSAGE_MAP(CDLG, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CDLG::OnBnClickedOk)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BTN_COLOR, &CDLG::OnBnClickedBtnColor)
	ON_EN_UPDATE(IDC_EDT_VSCALE, &CDLG::OnEnUpdateEdtVscale)
	ON_EN_UPDATE(IDC_EDT_GRIDDIST, &CDLG::OnEnUpdateEdtGriddist)
	ON_BN_CLICKED(IDC_CHK_STANDARD, &CDLG::OnBnClickedChkStandard)
	ON_BN_CLICKED(IDC_CHK_CONTINUOUS, &CDLG::OnBnClickedChkContinuous)
	ON_CBN_SELCHANGE(IDC_CBX_LAYER, &CDLG::OnCbnSelchangeCbxLayer)
	
	ON_EN_UPDATE(IDC_EDT_LINESCALE, &CDLG::OnEnUpdateEdtLinescale)
	ON_BN_CLICKED(ID_DLG_HELP, &CDLG::OnBnClickedDlgHelp)
END_MESSAGE_MAP()

BOOL CDLG::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	m_VScaleStr = iniFile.readINI(_T("config.ini"), _T("DLG"), _T("VSCALE"));
	if(m_VScaleStr.CompareNoCase(_T("")) == 0)	m_VScaleStr = _T("1000");
	m_GridColumnDistStr = iniFile.readINI(_T("config.ini"), _T("DLG"), _T("GRIDDIST"));
	if(m_GridColumnDistStr.CompareNoCase(_T("")) == 0)	m_GridColumnDistStr = _T("1");
	m_LineScaleStr = iniFile.readINI(_T("config.ini"), _T("DLG"), _T("LINESCALE"));
	if(m_LineScaleStr.CompareNoCase(_T("")) == 0)	m_LineScaleStr = _T("1.0");
	CString m_DwgStandardStr = iniFile.readINI(_T("config.ini"), _T("DLG"), _T("DWGSTANDARD"));
	if(m_DwgStandardStr.CompareNoCase(_T("")) == 0 || (m_DwgStandardStr.CompareNoCase(_T("TRUE")) == 0)){
		dwgStandard = true;
		CheckDlgButton(IDC_CHK_STANDARD, true);
	}
	else{
		dwgStandard = false;
		CheckDlgButton(IDC_CHK_STANDARD, false);
	}
	CString m_ContinueLineStr = iniFile.readINI(_T("config.ini"), _T("DLG"), _T("CONTINUELINE"));
	if(m_ContinueLineStr.CompareNoCase(_T("")) == 0 || (m_ContinueLineStr.CompareNoCase(_T("TRUE")) == 0)){
		continueLine = false;
		CheckDlgButton(IDC_CHK_CONTINUOUS, false);
	}
	else{
		continueLine = true;
		CheckDlgButton(IDC_CHK_CONTINUOUS, true);
	}
	OnBnClickedChkStandard();
	UpdateData(FALSE);

	//도면층 불러오기
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);

	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;		
		pLayerTblRcd->getName(name);
		m_layerCbx.AddString(name);		
	}
	delete pLayerTblIter;
	m_layerCbx.SetCurSel(0);

	OnCbnSelchangeCbxLayer();

	//라인타임 로드
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	//acdbLoadLineTypeFile(_T("HIDDEN"), _T("zwcad.lin"), pDb);
	acdbLoadLineTypeFile(_T("HIDDEN"), LinetypeStr, pDb);
	return TRUE;
}

void CDLG::OnBnClickedOk(){
	if(m_VScaleStr.IsEmpty() || m_GridColumnDistStr.IsEmpty())
		return;

	CAcUiDialog::OnOK();
	UpdateData(TRUE);

	iniFile.writeINI(_T("config.ini"), _T("DLG"), _T("VSCALE"), m_VScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("DLG"), _T("GRIDDIST"), m_GridColumnDistStr);
	iniFile.writeINI(_T("config.ini"), _T("DLG"), _T("LINESCALE"), m_LineScaleStr);

	ads_point sp, ep, hp;
	CString strMsg;
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	strMsg.LoadStringW(IDS_VERT_START_SEL); // "종단 최하단 시작점 지정: "
	if(acedGetPoint(NULL, strMsg, sp) == RTNORM){
		strMsg.LoadStringW(IDS_VERT_END_SEL); // "종단 최하단 끝점 지정: "
		if(acedGetPoint(sp, strMsg, ep) == RTNORM){
			strMsg.LoadStringW(IDS_GRID_TOP_SEL); // "Grid를 그릴 종단 상단점 지정: "
			if(acedGetPoint(sp, strMsg, hp) == RTNORM){
				AcGePoint3d basePoint3d(sp[0], sp[1], 0);
				AcGePoint3d xPoint3d(ep[0], sp[1], 0);
				AcGePoint3d yPoint3d(sp[0], hp[1], 0);

				double totalGridColumnDist = basePoint3d.distanceTo(yPoint3d);
				double distanceAt = (_ttof(m_GridColumnDistStr) * 1000) / _ttof(m_VScaleStr);
				int cnt = totalGridColumnDist / distanceAt;
				//acutPrintf(_T("\n %f\n %f\n %d"), totalGridColumnDist, distanceAt, cnt);

				AcGePoint3d lineSp, lineEp;
				lineSp = basePoint3d;
				lineSp.y = lineSp.y + distanceAt;
				lineEp = xPoint3d;
				lineEp.y = lineEp.y + distanceAt;

				for(int i = 0; i < cnt; i++)
				{
					AcDbLine* drawLine = new AcDbLine(lineSp, lineEp);
					drawLine->setDatabaseDefaults();					
					if(!dwgStandard)
					{
						drawLine->setLayer(layername);
						if(colorIndex != m_curColorIndex)
						{
							drawLine->setColorIndex(m_curColorIndex);
						}
						drawLine->setLinetypeScale(_ttof(m_LineScaleStr));
						if(continueLine){
							drawLine->setLinetype(_T("CONTINUOUS"));
						}
					}
					else
					{
						drawLine->setLinetype(_T("HIDDEN"));
						drawLine->setColorIndex(2);
					}

					AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
					acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);					
					bTR->appendAcDbEntity(drawLine);
					drawLine->close();
					bTR->close();

					lineSp.y = lineSp.y + distanceAt;
					lineEp.y = lineEp.y + distanceAt;
				}
			}
		}
	}
	CompleteEditorCommand();
}

void CDLG::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct){
	if((nIDCtl == IDC_BTN_COLOR)){
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct -> hDC);						//버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;						//버튼영역 구하기
		dc.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect, btnAfterClr);					//버튼색상
		UINT state = lpDrawItemStruct->itemState;				//버튼상태구하기
		if((state &ODS_SELECTED))
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		else
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);

		TCHAR buffer[MAX_PATH];												//버튼의 text를 얻기위한 임시버퍼
		ZeroMemory(buffer,MAX_PATH);										//버퍼초기화
		::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);		//버튼의 text얻기
		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);	//버튼의 text넣기
		dc.Detach();														//버튼의 dc 풀어주기
	}
	CAcUiDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDLG::OnBnClickedBtnColor(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();
	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();
	acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor);
	SelectColorChange(selectColor);
}

//색상 변경
void CDLG::SelectColorChange(int colorIndex){
	CString colorIndexStr;
	colorIndexStr.Format(_T("%d"), colorIndex);
	if(colorIndexStr.CompareNoCase(_T("0")) == 0)
		colorIndexStr.Format(_T("By Block"));
	else if(colorIndexStr.CompareNoCase(_T("256")) == 0)
		colorIndexStr.Format(_T("By Layer"));

	SetDlgItemTextW(IDC_STT_COLOR, colorIndexStr);
	btnAfterClr = acedGetRGB(colorIndex);
	m_curColorIndex = colorIndex;
	Invalidate();
}

//숫자 및 점(.) 입력 체크
CString CDLG::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	for(int i = 0; i < str.GetLength(); i++){
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i,1);
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46))
			str.Remove(*getAtChar);
	}
	return str;
}

void CDLG::OnEnUpdateEdtVscale(){
	UpdateData(TRUE);	
	m_VScaleStr = CheckInputNumber(m_VScaleStr);
	UpdateData(FALSE);
	m_VScaleCtr.SetSel(0, -1);
	m_VScaleCtr.SetSel(-1, -1);
	UpdateData(TRUE);
}

void CDLG::OnEnUpdateEdtGriddist(){
	UpdateData(TRUE);
	m_GridColumnDistStr = CheckInputNumber(m_GridColumnDistStr);
	UpdateData(FALSE);
	m_GridColumnDistCtr.SetSel(0, -1);
	m_GridColumnDistCtr.SetSel(-1, -1);
	UpdateData(TRUE);
}

void CDLG::OnBnClickedChkStandard(){
	if(IsDlgButtonChecked(IDC_CHK_STANDARD)){
		((CButton*)GetDlgItem(IDC_CBX_LAYER))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BTN_COLOR))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_STT_COLOR))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_CONTINUOUS))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDT_LINESCALE))->EnableWindow(FALSE);
		dwgStandard = true;
	}
	else{
		((CButton*)GetDlgItem(IDC_CBX_LAYER))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BTN_COLOR))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_STT_COLOR))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_CONTINUOUS))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDT_LINESCALE))->EnableWindow(TRUE);
		dwgStandard = false;
	}
}

void CDLG::OnBnClickedChkContinuous(){
	if(IsDlgButtonChecked(IDC_CHK_CONTINUOUS)){
		continueLine = true;
	}
	else{
		continueLine = false;
	}
}

void CDLG::OnCbnSelchangeCbxLayer(){
	CString layerName;
	m_layerCbx.GetLBText(m_layerCbx.GetCurSel(), layerName);
	int colorIndex = LayerChangeGetColor(layerName);
	SelectColorChange(colorIndex);
}

//도면층별 색상 가져오기
int CDLG::LayerChangeGetColor(CString layerName){
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);

	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);		
		AcCmColor color;
		
		pLayerTblRcd->getName(layername);
		color = pLayerTblRcd->color();
		colorIndex = color.colorIndex();
		if(layerName.CompareNoCase(layername) == 0){
			delete pLayerTblIter;
			delete pLayerTbl;
			return colorIndex;
		}
	}
	delete pLayerTblIter;
	delete pLayerTbl;
	return 0;
}

void CDLG::OnEnUpdateEdtLinescale()
{
	UpdateData(TRUE);
	m_LineScaleStr = CheckInputNumber(m_LineScaleStr);
	UpdateData(FALSE);
	m_LineScaleCtr.SetSel(0, -1);
	m_LineScaleCtr.SetSel(-1, -1);
	UpdateData(TRUE);
}


void CDLG::OnBnClickedDlgHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/DLG.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
