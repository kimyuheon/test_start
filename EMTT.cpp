// EMTT.cpp: 구현 파일
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "EMTT.h"
#include <WinBase.h>

#include "CRange.h"
#include "CRanges.h"
#include "CApplication.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "WkcomboBox.h"
#include <tlhelp32.h>

#define MAX_BUFF	4096

template<class T>
void QSortCArray(T& t, int(__cdecl* compare)(const void* elem1, const void* elem2)) {
	if (t.GetSize() <= 0)	return;
	qsort(t.GetData(), t.GetSize(), sizeof(t[0]), compare);
}

//내림차순 정렬
int CompareStringDescEMTT(const void* elem1, const void* elem2) {
	FileInfosEMTT* p1 = (FileInfosEMTT*)elem1;
	FileInfosEMTT* p2 = (FileInfosEMTT*)elem2;
	if (p1->filename > p2->filename)	return +1;
	if (p1->filename < p2->filename)	return -1;

	return 0;
}

// 오름차순 정렬
int CompareStringAscEMTT(const void* elem1, const void* elem2) {
	FileInfosEMTT* p1 = (FileInfosEMTT*)elem1;
	FileInfosEMTT* p2 = (FileInfosEMTT*)elem2;
	if (p1->filename > p2->filename) return -1;
	if (p1->filename < p2->filename) return +1;

	return 0;
}

//X좌표를 기준으로 정렬오름차순
int EMTTComparePntX(const void* elem1, const void* elem2) {
	CPrintFormData* ent1 = (CPrintFormData*)elem1;
	CPrintFormData* ent2 = (CPrintFormData*)elem2;
	if (ent1->minpoint_X > ent2->minpoint_X)	return +1;
	if (ent1->minpoint_X < ent2->minpoint_X)	return -1;

	return 0;
}

//Y좌표를 기준으로 정렬오름차순
int EMTTComparePntY(const void* elem1, const void* elem2) {
	CPrintFormData* ent1 = (CPrintFormData*)elem1;
	CPrintFormData* ent2 = (CPrintFormData*)elem2;
	if (ent1->minpoint_Y > ent2->minpoint_Y)	return +1;
	if (ent1->minpoint_Y < ent2->minpoint_Y)	return -1;

	return 0;
}

//X좌표를 기준으로 정렬내림차순 
int EMTTComparePntX_De(const void* elem1, const void* elem2) {
	CPrintFormData* ent1 = (CPrintFormData*)elem1;
	CPrintFormData* ent2 = (CPrintFormData*)elem2;
	if (ent1->minpoint_X > ent2->minpoint_X)	return -1;
	if (ent1->minpoint_X < ent2->minpoint_X)	return +1;

	return 0;
}

//Y좌표를 기준으로 정렬내림차순 
int EMTTComparePntY_De(const void* elem1, const void* elem2) {
	CPrintFormData* ent1 = (CPrintFormData*)elem1;
	CPrintFormData* ent2 = (CPrintFormData*)elem2;
	if (ent1->minpoint_Y > ent2->minpoint_Y)	return -1;
	if (ent1->minpoint_Y < ent2->minpoint_Y)	return +1;

	return 0;
}

// CEMTT 대화 상자

IMPLEMENT_DYNAMIC(CEMTT, CDialogEx)

CEMTT::CEMTT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EMTT, pParent)
	, m_RadProperty_Btn(0)
	, m_RadSearch_Btn(0)
	, m_RadSort_Btn(0)
	, m_ListSort_Btn(0)
	, m_SearchForm_Str(_T(""))
	, m_SortError_Str(_T(""))
{

}

CEMTT::~CEMTT()
{
}

void CEMTT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EMTT_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_EMTT_BUTTON2, m_btn2);
	DDX_Control(pDX, IDC_EMTT_BUTTON3, m_btn3);
	DDX_Control(pDX, IDC_EMTT_BUTTON4, m_btn4);
	DDX_Control(pDX, IDC_EMTT_TXT_CLEAR, m_btn_Clear);
	DDX_Radio(pDX, IDC_EMTT_RADEXCEL, m_RadProperty_Btn);
	DDX_Radio(pDX, IDC_EMTT_RADMODEL, m_RadSearch_Btn);
	DDX_Radio(pDX, IDC_EMTT_RADZSORT, m_RadSort_Btn);
	DDX_Control(pDX, IDC_EMTT_LISTFILES, m_FileView);
	DDX_Radio(pDX, IDC_EMTT_SORT1, m_ListSort_Btn);
	DDX_Control(pDX, IDC_EMTT_BTNEXTRACT, m_bSelect);
	DDX_Control(pDX, IDC_EMTT_LISTFILES2, m_ExcelView);
	DDX_Control(pDX, IDC_EMTT_TXT_LIST, m_TXTView);
	DDX_Text(pDX, IDC_EMTT_EDTSEARCH, m_SearchForm_Str);
	DDX_Text(pDX, IDC_EMTT_EDTERROR, m_SortError_Str);
	DDX_Text(pDX, IDC_EMTT_EDTCERTAIN, m_Certain_Str);
}


BEGIN_MESSAGE_MAP(CEMTT, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDCANCEL, &CEMTT::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_EMTT_RADEXCEL, &CEMTT::OnBnClickedEmttRadexcel)
	ON_BN_CLICKED(IDC_EMTT_RADCAD, &CEMTT::OnBnClickedEmttRadcad)
	ON_BN_CLICKED(IDC_EMTT_BTNEXTRACT, &CEMTT::OnBnClickedEmttBtnextract)
	ON_BN_CLICKED(IDC_EMTT_RADMODEL, &CEMTT::OnBnClickedEmttRadmodel)
	ON_BN_CLICKED(IDC_EMTT_RADLAYOUT, &CEMTT::OnBnClickedEmttRadmodel)
	ON_BN_CLICKED(IDC_EMTT_RADMODELLAY, &CEMTT::OnBnClickedEmttRadmodel)
	ON_BN_CLICKED(IDC_EMTT_RADCERTAIN, &CEMTT::OnBnClickedEmttRadmodel)
	ON_BN_CLICKED(IDC_EMTT_RADACTIVE, &CEMTT::OnBnClickedEmttRadmodel)
	ON_EN_UPDATE(IDC_EMTT_EDTCERTAIN, &CEMTT::OnEnUpdateCalsEdtcertain)
	ON_BN_CLICKED(IDC_EMTT_RADZSORT, &CEMTT::OnBnClickedEmttRadzsort)
	ON_BN_CLICKED(IDC_EMTT_RADNSORT, &CEMTT::OnBnClickedEmttRadnsort)
	ON_BN_CLICKED(IDC_EMTT_RADNMSORT, &CEMTT::OnBnClickedEmttRadnmsort)
	ON_BN_CLICKED(IDC_EMTT_RADZMSORT, &CEMTT::OnBnClickedEmttRadzmsort)
	ON_BN_CLICKED(IDC_EMTT_BUTTON1, &CEMTT::OnBnClickedEmttButton1)
	ON_BN_CLICKED(IDC_EMTT_BUTTON2, &CEMTT::OnBnClickedEmttButton2)
	ON_BN_CLICKED(IDC_EMTT_BUTTON3, &CEMTT::OnBnClickedEmttButton3)
	ON_BN_CLICKED(IDC_EMTT_BUTTON4, &CEMTT::OnBnClickedEmttButton4)
	ON_EN_UPDATE(IDC_EMTT_EDTERROR, &CEMTT::OnEnUpdateEmttEdterror)
	ON_BN_CLICKED(IDC_EMTT_BTN_ADDDWG, &CEMTT::OnBnClickedEmttBtnAdddwg)
	ON_BN_CLICKED(IDC_EMTT_BTN_ADDFOLDER, &CEMTT::OnBnClickedEmttBtnAddfolder)
	ON_BN_CLICKED(IDC_EMTT_BTN_ACTIVEDWG, &CEMTT::OnBnClickedEmttBtnActivedwg)
	ON_BN_CLICKED(IDC_EMTT_BTN_OPENDWG, &CEMTT::OnBnClickedEmttBtnOpendwg)
	ON_NOTIFY(NM_DBLCLK, IDC_EMTT_LISTFILES, &CEMTT::OnNMDblclkEmttListfiles)
	ON_BN_CLICKED(IDC_EMTT_SEL_DEL, &CEMTT::OnBnClickedEmttSelDel)
	ON_BN_CLICKED(IDC_EMTT_ALL_DEL, &CEMTT::OnBnClickedEmttAllDel)
	ON_BN_CLICKED(IDC_EMTT_SORT1, &CEMTT::OnBnClickedEmttSort1)
	ON_BN_CLICKED(IDC_EMTT_SORT2, &CEMTT::OnBnClickedEmttSort2)
	ON_BN_CLICKED(IDC_EMTT_BTN_UP, &CEMTT::OnBnClickedEmttBtnUp)
	ON_BN_CLICKED(IDC_EMTT_BTN_DOWN, &CEMTT::OnBnClickedEmttBtnDown)
	ON_BN_CLICKED(IDC_EMTT_BTN_TOP, &CEMTT::OnBnClickedEmttBtnTop)
	ON_BN_CLICKED(IDC_EMTT_BTN_BOTTOM, &CEMTT::OnBnClickedEmttBtnBottom)
	ON_BN_CLICKED(IDC_EMTT_BTN_IMPORT, &CEMTT::OnBnClickedEmttBtnImport)
	ON_BN_CLICKED(IDC_EMTT_TXT_ADD, &CEMTT::OnBnClickedEmttTxtAdd)
	ON_BN_CLICKED(IDC_EMTT_BTN_EXPORT, &CEMTT::OnBnClickedEmttBtnExport)
END_MESSAGE_MAP()


// CEMTT 메시지 처리기

LRESULT CEMTT::onAcadKeepFocus(WPARAM, LPARAM) {
	return TRUE;
}

BOOL CEMTT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	m_bSelect.AutoLoad();

	this->GetWindowRect(&initrect);
	GetDlgItem(IDC_EXCEL_BOX)->GetWindowRect(&Boxrect);
	m_Fold_Width = Boxrect.left - initrect.left;
	this->SetWindowPos(NULL, 0, 0, m_Fold_Width, initrect.bottom - initrect.top, SWP_NOMOVE);
	//Edt 컨트롤 처음에 보이지 않게 함
	GetDlgItem(IDC_EMTT_EDTCERTAIN)->ShowWindow(SW_HIDE);

	//정렬순서 비트맵
	m_btn1.LoadBitmaps(IDB_BITMAP_ORDER1, IDB_BITMAP_ORDER1, NULL, NULL);
	m_btn1.SizeToContent();
	m_btn2.LoadBitmaps(IDB_BITMAP_ORDER2, IDB_BITMAP_ORDER2, NULL, NULL);
	m_btn2.SizeToContent();
	m_btn3.LoadBitmaps(IDB_BITMAP_ORDER3, IDB_BITMAP_ORDER3, NULL, NULL);
	m_btn3.SizeToContent();
	m_btn4.LoadBitmaps(IDB_BITMAP_ORDER4, IDB_BITMAP_ORDER4, NULL, NULL);
	m_btn4.SizeToContent();


	//리스트 컨트롤 초기화
	m_FileView.GetWindowRect(&rt);
	m_FileView.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);// 리스트 컨트롤에 선표시 및 ITEM 선택시 한행 선택
	m_FileView.DeleteAllItems();
	m_FileView.InsertColumn(0, TEXT("파일명"), LVCFMT_LEFT, rt.Width() * 0.7, -1);
	m_FileView.InsertColumn(1, TEXT("처리결과"), LVCFMT_LEFT, rt.Width() * 0.3, -1);
	m_FileView.GetHeaderCtrl()->ModifyStyle(0, WS_BORDER, SWP_DRAWFRAME);
	//FileViewRefresh();

	m_ExcelView.GetWindowRect(&rt);
	m_ExcelView.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);// 리스트 컨트롤에 선표시 및 ITEM 선택시 한행 선택

	m_ExcelView.InsertColumn(0, TEXT("문서명"), LVCFMT_LEFT, rt.Width() * 0.5);
	m_ExcelView.InsertColumn(1, TEXT("시트명"), LVCFMT_LEFT, rt.Width() * 0.5);
	m_ExcelView.GetHeaderCtrl()->ModifyStyle(0, WS_BORDER, SWP_DRAWFRAME);

	m_TXTView.GetWindowRect(&rt);
	m_TXTView.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);// 리스트 컨트롤에 선표시 및 ITEM 선택시 한행 선택
	m_TXTView.DeleteAllItems();
	m_TXTView.InsertColumn(0, TEXT("객체명"), LVCFMT_LEFT, rt.Width() * 0.2, -1);
	m_TXTView.InsertColumn(1, TEXT("내용/태그"), LVCFMT_LEFT, rt.Width() * 0.25, -1);
	m_TXTView.InsertColumn(2, TEXT("좌표/값"), LVCFMT_LEFT, rt.Width() * 0.55, -1);
	m_TXTView.GetHeaderCtrl()->ModifyStyle(0, WS_BORDER, SWP_DRAWFRAME);

	m_TxtList.erase(m_TxtList.begin(), m_TxtList.end());

	//::ReleaseDC(this->GetSafeHwnd(), dc);

	GetDlgItem(IDC_EMTT_RADCERTAIN)->GetWindowRect(rc);
	ScreenToClient(&rc);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEMTT::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DestroyWindow();
	//CDialogEx::OnCancel();
}

extern CEMTT* pEMTT;
void CEMTT::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	pEMTT = NULL;
	//CDialogEx::PostNcDestroy();
}

//라디오 속성 정보 Excel로 내보내기
void CEMTT::OnBnClickedEmttRadexcel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_RadProperty_Btn = 0;
	this->SetWindowPos(NULL, 0, 0, m_Fold_Width, initrect.bottom - initrect.top, SWP_NOMOVE);
	UpdateData(FALSE);
}


void CEMTT::OnBnClickedEmttRadcad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_RadProperty_Btn = 1;
	this->SetWindowPos(NULL, 0, 0,
		initrect.right - initrect.left, initrect.bottom - initrect.top, SWP_NOMOVE);

	m_ExcelList.RemoveAll();
	m_ExcelView.DeleteAllItems();
	//엑셀 문서,시트명 뽑아오기
	RunningExcelData();

	for (int i = 0; i < m_ExcelList.GetCount(); i++) {
		m_ExcelView.InsertItem(i, m_ExcelList[i].Doc_Name);
		m_ExcelView.SetItem(i, 1, LVIF_TEXT, m_ExcelList[i].Sheet_Name, 0, 0, 0, NULL);
	}

	UpdateData(FALSE);
}


void CEMTT::OnBnClickedEmttBtnextract()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.UpdateData(TRUE);
	//BeginEditorCommand();
	::SetFocus(adsw_acadDocWnd());
	ads_name en;
	ads_point pt;
	StrInfoEMTT si;

	struct resbuf fromrb, toWCS, toDCS;
	fromrb.restype = RTSHORT;
	fromrb.resval.rint = 1; // UCS
	toWCS.restype = RTSHORT;
	toWCS.resval.rint = 0; // WCS 
	toDCS.restype = RTSHORT;
	toDCS.resval.rint = 2; // DCS

	AcGePoint3d minUCSPt, maxUCSPt, minWCSPt, maxWCSPt, minDCSPt, maxDCSPt;

	while (true) {
		int stat = acedEntSel(_T("\n블록 선택 : \n"), en, pt);

		if (stat == RTCAN || stat == RTNONE)
			break;
		else if (stat == RTERROR) {
			resbuf zValueStr;
			ads_getvar(_T("ERRNO"), &zValueStr);
			if (zValueStr.resval.rint == 52)
				break;
			else
				continue;
		}
		else if (stat == RTNORM) {
			AcDbObjectId objId;
			acdbGetObjectId(objId, en);
			//AcDbEntity *pEnt=NULL;

			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if (pEnt.openStatus() == Acad::eOk) {
				if (pEnt->isKindOf(AcDbBlockReference::desc())) {
					AcDbBlockReference* pInsert = AcDbBlockReference::cast(pEnt);
					if (!pInsert) {
						acutPrintf(_T("\n선택된 엔티티가 블록이 아닙니다.\n"));
						pEnt->close();
						break;
					}
					else if (pInsert) {
						AcDbExtents FormArea1;
						
						// Target/ VIEWDIR 시스템 변수 조사 후 0 이 아니면 변수 저장 후 0으로 초기화
						AcGePoint3d TartgetPt;
						AcGeVector3d vViewdir;
						bool isZero = getTarget_ViewDir(TartgetPt, vViewdir);

						pInsert->getGeomExtents(FormArea1);
						//Utils utils;
						AcDbExtents extentsDCS, tmpExtents;
						AcGePoint3d FormMin, FormMax;

						tmpExtents = FormArea1;

						AcGeMatrix3d mat;
						mat.setToTranslation((TartgetPt.asVector()));

						minUCSPt = tmpExtents.minPoint();
						maxUCSPt = tmpExtents.maxPoint();

						minUCSPt.transformBy(mat);
						maxUCSPt.transformBy(mat);

						//// WCS to DCS
						acedTrans(asDblArray(minUCSPt), &toWCS, &toDCS, FALSE, asDblArray(FormMin));
						acedTrans(asDblArray(maxUCSPt), &toWCS, &toDCS, FALSE, asDblArray(FormMax));

						extentsDCS.addPoint(FormMin);
						extentsDCS.addPoint(FormMax);

						FormArea_minpt = AcGePoint3d(extentsDCS.minPoint().x, extentsDCS.minPoint().y, extentsDCS.minPoint().z);
						FormArea_maxpt = AcGePoint3d(extentsDCS.maxPoint().x, extentsDCS.maxPoint().y, extentsDCS.maxPoint().z);
					}
					AcDbObjectId blockDefID = pInsert->blockTableRecord();
					AcGeScale3d scale = pInsert->scaleFactors();

					pInsert->close();

					AcDbBlockTableRecord* pBTR;
					if (Acad::eOk != (es = acdbOpenObject(pBTR, blockDefID, AcDb::kForRead))) {
						acutPrintf(_T("\n블록 정의 엑세스 할 수 없습니다..\n"));
						break;
					}

					const TCHAR* pBlkName;
					es = pBTR->getName(pBlkName);
					pBTR->close();
					m_SearchForm_Str.Replace(_T(" "), NULL);
					if (m_SearchForm_Str.CompareNoCase(_T("")) == 0) {
						m_SearchForm_Str = m_SearchForm_Str + pBlkName;
					}
					else {
						if (m_SearchForm_Str.Right(1) == _T(","))
							m_SearchForm_Str = m_SearchForm_Str + _T(" ") + pBlkName;
						else {
							m_SearchForm_Str = m_SearchForm_Str + _T(", ") + pBlkName;
						}
					}


					//si.FileForm = pBlkName;
					//
					//m_StrList.Add(si);

					if ((Acad::eOk != es) || !pBlkName) {
						acutPrintf(_T("블록 이름을 추출 할 수 없습니다.\n"));
						break;
					}
					pEnt->close();

					CString blockname = pBlkName;
					if (blockname.CompareNoCase(_T("")) != 0)	AddBlock(objId);
					//acutPrintf(_T("\n 블록 이름 : '%s'\n"),pBlkName);
					break;
				}
			}
			
		}
	}
	//CompleteEditorCommand();
	UpdateData(FALSE);
}

bool CEMTT::RunningExcelData()
{
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	COleVariant VTrue((short)TRUE);
	COleVariant VFalse((short)FALSE);
	CApplication app;
	CRange startrng;
	LPDISPATCH lpdisp;
	CWorksheet sheet;
	CWorkbook book;
	CWorkbooks books;
	CWorksheets sheets;
	int column1 = 0;
	int column2 = 1;
	int pointcnt;
	// app 엑셀 오브젝트	
	::CoInitialize(NULL);

	IDispatch* pdispExcelApp = NULL;
	IUnknown* pUnk = NULL;
	CLSID clsExcelApp;

	if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsExcelApp)))
	{
		AfxMessageBox(_T("엑셀이 설치되어 있지 않습니다."));
	}

	HRESULT hr = GetActiveObject(clsExcelApp, NULL, (IUnknown**)&pUnk);
	if (SUCCEEDED(hr)) {
		hr = pUnk->QueryInterface(IID_IDispatch, (void**)&pdispExcelApp);
	}
	else
	{
		AfxMessageBox(_T("엑셀을 찾을 수 없습니다."));
	}

	if (NULL == pdispExcelApp) {
		return false;
	}

	app.AttachDispatch(pdispExcelApp);
	lpdisp = app.get_Workbooks();
	ASSERT(lpdisp);
	books.AttachDispatch(lpdisp);

	//Books
	ExcelInfosEMTT exif;
	CString bookName;
	int bookscnt = books.get_Count();
	for (int i = 1; i <= books.get_Count(); i++) {
		lpdisp = books.get_Item(COleVariant((short)i));
		ASSERT(lpdisp);
		book.AttachDispatch(lpdisp);
		CString bb = book.get_Name();
		//acutPrintf(_T("\n book : %s"),bb);
		bookName = book.get_Name();

		//Sheets
		lpdisp = book.get_Sheets();
		ASSERT(lpdisp);
		sheets.AttachDispatch(lpdisp);
		//Sheet
		for (int i = 1; i <= sheets.get_Count(); i++) {
			lpdisp = sheets.get_Item(COleVariant((short)i));
			ASSERT(lpdisp);
			sheet.AttachDispatch(lpdisp);
			CString aa = sheet.get_Name();
			//acutPrintf(_T("\nsheet : %s"),aa);
			exif.Sheet_Name = sheet.get_Name();
			exif.Doc_Name = bookName;
			m_ExcelList.Add(exif);
		}
	}
	::CoUninitialize();
	return true;
}


void CEMTT::OnBnClickedEmttRadmodel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_RadSearch_Btn == 3) {
		GetDlgItem(IDC_EMTT_EDTCERTAIN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EMTT_RADCERTAIN)->MoveWindow(rc.left, rc.top, 15, rc.Height());
	}
	else {
		GetDlgItem(IDC_EMTT_EDTCERTAIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EMTT_RADCERTAIN)->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height());
	}

	UpdateData(FALSE);
}

//Edt 특정배치 
void CEMTT::OnEnUpdateCalsEdtcertain()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}

void CEMTT::OnBnClickedEmttRadzsort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_RadSort_Btn = 0;
	m_sorttype = 0;
	UpdateData(FALSE);
}


void CEMTT::OnBnClickedEmttRadnsort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_RadSort_Btn = 1;
	m_sorttype = 1;
	UpdateData(FALSE);
}


void CEMTT::OnBnClickedEmttRadnmsort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_RadSort_Btn = 2;
	m_sorttype = 2;
	UpdateData(FALSE);
}


void CEMTT::OnBnClickedEmttRadzmsort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_RadSort_Btn = 3;
	m_sorttype = 3;
	UpdateData(FALSE);
}


void CEMTT::OnBnClickedEmttButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	OnBnClickedEmttRadzsort();
	UpdateData(FALSE);
}


void CEMTT::OnBnClickedEmttButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	OnBnClickedEmttRadnsort();
	UpdateData(FALSE);
}


void CEMTT::OnBnClickedEmttButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	OnBnClickedEmttRadnmsort();
	UpdateData(FALSE);
}


void CEMTT::OnBnClickedEmttButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	OnBnClickedEmttRadzmsort();
	UpdateData(FALSE);
}


void CEMTT::OnEnUpdateEmttEdterror()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_SortError = _tstof(m_SortError_Str);
	UpdateData(FALSE);
}

void CEMTT::RowSwap(int a, int b) {
	FileInfosEMTT files;
	files = m_FileList[a];
	m_FileList[a] = m_FileList[b];
	m_FileList[b] = files;
}

void CEMTT::FileListReIndexing()
{
	for (int i = 0; i < m_FileList.GetCount(); i++)	m_FileList[i].index = i;
	m_FileView.DeleteAllItems();
	for (int i = 0; i < m_FileList.GetCount(); i++) {
		CString seq_string;
		seq_string.Format(_T("%d"), m_FileList[i].index + 1);
		m_FileView.InsertItem(i, m_FileList[i].filename);
		m_FileView.SetItem(i, 1, LVIF_TEXT, m_FileList[i].result, 0, 0, 0, NULL);
		//m_FileView.SetItem(i,3,LVIF_TEXT,m_FileList[i].plotstyle,0,0,0,NULL);
	}
}

BOOL CEMTT::CreateDir(CString dir) {
	CFileFind file;
	CString strFile = _T("*.*");
	BOOL bResult = file.FindFile(dir + strFile);
	if (!bResult)	bResult = CreateDirectory(dir, NULL);

	return bResult;
}

//파일 리스트뷰에 데이터 올리기
void CEMTT::FileViewRefresh() {
	m_FileView.DeleteAllItems();
	for (int i = 0; i < m_FileList.GetCount(); i++) {
		CString seq_string;
		seq_string.Format(_T("%d"), i + 1);
		m_FileView.InsertItem(i, m_FileList[i].filename);
		m_FileView.SetItem(i, 1, LVIF_TEXT, m_FileList[i].result, 0, 0, 0, NULL);
		//		m_FileView.SetItem(i,3,LVIF_TEXT,m_FileList[i].plotstyle,0,0,0,NULL);
	}
}

// 모형 배치 모으기
void CEMTT::GetLayout(AcDbDatabase* pDb, CArray <AcDbObjectId, AcDbObjectId>& arrLayout) {
	AcDbDictionary* pDict = NULL;
	AcDbObject* pObj;
	if (pDb->getLayoutDictionary(pDict, AcDb::kForRead) == Acad::eOk)
	{
		AcDbDictionaryIterator* pIter = pDict->newIterator();
		for (; !pIter->done(); pIter->next())
		{
			ACHAR* pLayName = NULL;
			Acad::ErrorStatus es = pIter->getObject(pObj, AcDb::kForRead);
			if (es != Acad::eOk) continue;
			AcDbLayout* pLayout = AcDbLayout::cast(pObj);
			if (pLayout)
			{
				arrLayout.Add(pLayout->objectId());
				pLayout->close();
			}
			else
			{
				pObj->close();
			}
		}
		delete pIter;
		pDict->close();
	}
}

// 블럭 좌표 가져오기
void CEMTT::GetBlockPoint(AcDbObjectId& blockId, double& minpoint_X, double& minpoint_Y, double& maxpoint_X, double& maxpoint_Y, AcGeScale3d& scale, double& angle)
{
	AcDbEntityPointer pEnt(blockId, AcDb::kForRead);
	if (pEnt.openStatus() == Acad::eOk)
	{

		AcDbBlockReference* pBlockRef = AcDbBlockReference::cast(pEnt);
		if (pBlockRef) {
			AcDbExtents extents;
			pBlockRef->geomExtentsBestFit(extents);
			//pBlockRef->getGeomExtents(extents);

			minpoint_X = extents.minPoint().x;
			minpoint_Y = extents.minPoint().y;

			maxpoint_X = extents.maxPoint().x;
			maxpoint_Y = extents.maxPoint().y;
			scale = pBlockRef->scaleFactors();
			angle = pBlockRef->rotation();
		}
		pEnt->close();
	}
}

bool CEMTT::FindDWGFile(int index, CString path, bool bcheck)
{
	CFileFind finder;
	BOOL working = finder.FindFile(path + "\\" + L"*.DWG");
	//int i = 0;
	while (working) {
		working = finder.FindNextFile();
		if (finder.IsDots())	continue;
		if (finder.IsDirectory()) {
		}
		else {
			bool addedCheck = false;

			for (int i = 0; i < m_FileList.GetCount(); i++) {
				if (m_FileList[i].full_path.CompareNoCase(finder.GetFilePath()) == 0) {
					addedCheck = true;
				}
			}

			if (!addedCheck) {
				FileInfosEMTT fi;
				fi.index = index;
				fi.filename = finder.GetFileName();
				fi.full_path = finder.GetFilePath();
				m_FileList.Add(fi);
				index++;
			}
		}
	}
	return true;
}

//블럭 중복 제거
void CEMTT::RemoveDuplicates(CStringArray& lst, CStringArray& result)
{
	result.RemoveAll();

	for (int i = 0; i < lst.GetCount(); i++)
	{
		bool bisfind = false;
		for (int j = 0; j < result.GetCount(); j++)
		{
			if (result[j].CompareNoCase(lst[i]) == 0)
			{
				bisfind = true;
				break;
			}
		}
		if (!bisfind)
			result.Add(lst[i]);
	}
}

//경로로 도큐먼트 가져오기
AcApDocument* CEMTT::GetOpendDocument(CString strPath) {
	AcApDocument* doc = NULL;
	//여기서 부터는 열린 도면들
	AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
	for (; !pIter->done(); pIter->step()) {
		const ACHAR* name = NULL;
		name = pIter->document()->fileName();
		if (strPath.CompareNoCase(name) == 0) {
			doc = pIter->document();
			break;
		}
	}
	delete pIter;
	return doc;
}

//도면이 현재 열려있는지 확인하기
BOOL CEMTT::IsOpenDocument(CString dwg) {
	BOOL bIsOpen = FALSE;
	//여기서 부터는 열린 도면들
	AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
	for (; !pIter->done(); pIter->step()) {
		if (dwg.CompareNoCase(pIter->document()->fileName()) == 0) {
			bIsOpen = TRUE;
			break;
		}
	}
	delete pIter;
	return bIsOpen;
}

//도면이 현재 열려있는지 확인하기
BOOL CEMTT::IsOpenDocument(CString dwg, AcApDocument* doc) {
	BOOL bIsOpen = FALSE;
	//여기서 부터는 열린 도면들
	AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
	for (; !pIter->done(); pIter->step()) {
		if (dwg.CompareNoCase(pIter->document()->fileName()) == 0) {
			bIsOpen = TRUE;
			doc = pIter->document();
			break;
		}
	}
	delete pIter;
	return bIsOpen;
}

//활성 도면 탐색
void CEMTT::GetActiveDWG()
{
	AcApDocument* activeDoc = NULL;
	activeDoc = acDocManager->curDocument();
	bool bisExist = false;
	for (int i = 0; i < m_FileList.GetCount(); i++) {
		if (m_FileList[i].full_path.CompareNoCase((CString)activeDoc->fileName()) == 0) {
			bisExist = true;
			break;
		}
	}
	if (!bisExist) {
		if (IsOpenDocument(activeDoc->fileName())) {
			FileInfosEMTT fileinfo;
			fileinfo.index = m_LastIdx;
			fileinfo.full_path = activeDoc->fileName();
			CString str, res;
			str = activeDoc->fileName();
			int NameIdx = str.ReverseFind(_T('\\'));//끝에서 \\ 인덱스 찾음
			res = str.Mid(NameIdx + 1, str.GetLength());//찾은 idx에서 끝까지 도면이름
			fileinfo.filename = res;

			if (fileinfo.full_path.Find(_T("\\")) != -1)	m_FileList.Add(fileinfo); 
			else m_FileList.Add(fileinfo);
			m_LastIdx++;
		}
	}
}

//열린 도면 탐색
void CEMTT::GetOpenDWG()
{
	AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
	while (!pIter->done()) {
		if (pIter->document() == acDocManager->curDocument()) {
			pIter->step();
			continue;
		}
		bool bisExist = false;
		const ACHAR* pDocName = NULL;
		for (int i = 0; i < m_FileList.GetCount(); i++) {
			pDocName = pIter->document()->fileName();
			if (m_FileList[i].full_path.CompareNoCase(pDocName) == 0) {
				bisExist = true;
				break;
			}
		}
		if (bisExist == false) {
			FileInfosEMTT fileinfo;
			fileinfo.filename = pIter->document()->docTitle();
			ACHAR* pNewDocName = NULL;
			acutNewString(pDocName, pNewDocName);
			if (pNewDocName)	fileinfo.full_path = pNewDocName;
			if (fileinfo.full_path.Find(_T("\\")) != -1)	m_FileList.Add(fileinfo);
		}
		pIter->step();
	}
	delete pIter;
}

//도면 추가
void CEMTT::OnBnClickedEmttBtnAdddwg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	int index = m_FileList.GetCount();

	CString File, FilePath;

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_HIDEREADONLY,
		_T("도면(*.dwg) |*.dwg| DXF(*.dxf)|*.dxf| All Files(*.*)|*.*|"));

	fileDlg.GetOFN().lpstrFile = File.GetBuffer(MAX_BUFF);
	fileDlg.GetOFN().nMaxFile = MAX_BUFF;

	if (fileDlg.DoModal() == IDOK)
	{
		for (POSITION pos = fileDlg.GetStartPosition(); pos != NULL;)
		{
			bool addedCheck = false;
			FilePath = fileDlg.GetNextPathName(pos);

			for (int i = 0; i < m_FileList.GetCount(); i++) {
				if (m_FileList[i].full_path.CompareNoCase(FilePath) == 0) {
					addedCheck = true;
				}
			}

			if (!addedCheck) {
				FileInfosEMTT insertfileinfo;
				insertfileinfo.index = index;
				insertfileinfo.full_path = FilePath;
				insertfileinfo.filename = FilePath.Mid(FilePath.ReverseFind('\\') + 1);
				m_FileList.Add(insertfileinfo);
			}
		}
	}

	// 리스트 뒤부터 파일 추가
	for (int i = index; i < m_FileList.GetCount(); i++) {
		CString seq_string;
		seq_string.Format(_T("%d"), i + 1);
		m_FileView.InsertItem(i, m_FileList[i].filename);
		m_FileView.SetItem(i, 1, LVIF_TEXT, m_FileList[i].result, 0, 0, 0, NULL);
	}

	UpdateData(FALSE);
}

//폴더 추가
void CEMTT::OnBnClickedEmttBtnAddfolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = m_FileList.GetCount();
	CString FilePath = L"";
	CString strInitPath = L"";

	CFolderPickerDialog Picker(strInitPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		bool addedCheck = false;
		FilePath = Picker.GetFolderPath();

		FindDWGFile(index, FilePath, true);
	}

	// 리스트 뒤부터 파일 추가
	for (int i = index; i < m_FileList.GetCount(); i++) {
		CString seq_string;
		seq_string.Format(_T("%d"), i + 1);
		m_FileView.InsertItem(i, m_FileList[i].filename);
		m_FileView.SetItem(i, 1, LVIF_TEXT, m_FileList[i].result, 0, 0, 0, NULL);
	}
}

//활성 도면
void CEMTT::OnBnClickedEmttBtnActivedwg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	GetActiveDWG();
	FileListReIndexing();
	UpdateData(FALSE);
}

//열린 도면
void CEMTT::OnBnClickedEmttBtnOpendwg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	OnBnClickedEmttBtnActivedwg(); //열린 도면 열기를 하면 활성 도면도 같이 추가
	GetOpenDWG();
	FileListReIndexing();
	UpdateData(FALSE);
}

//더블클릭으로 도면 열기
void CEMTT::OnNMDblclkEmttListfiles(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iItem < 0) return;
	CString path = m_FileList[pNMLV->iItem].full_path;
	AcApDocument* doc = NULL;
	bool bIsopen = IsOpenDocument(path, doc);
	if (bIsopen) acDocManager->setCurDocument(doc);
	else {
		Acad::ErrorStatus es = Acad::eOk;
		es = acDocManager->appContextOpenDocument(path);
	}
	*pResult = 0;
}

//선택한 리스트 삭제
void CEMTT::OnBnClickedEmttSelDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_FileView.GetItemCount() <= 0)	return;
	if (m_FileList.GetCount() <= 0)	return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedItem == m_FileList.GetCount())	nSelectedItem = nSelectedItem - 1;
	if (nSelectedItem < 0)	return;

	POSITION pos = m_FileView.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		while (pos) {
			int nItem = m_FileView.GetNextSelectedItem(pos);
			for (int i = 0; i < m_FileList.GetCount(); i++) {
				if (m_FileList[i].filename.CompareNoCase(m_FileView.GetItemText(nItem, 0)) == 0) {
					m_FileList.RemoveAt(i);
					break;
				}
			}
		}
	}
	FileListReIndexing();
	m_FileView.SetItemState(nSelectedItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_FileView.EnsureVisible(nSelectedItem, TRUE);
	m_FileView.SetFocus(); //선택된 항목들 포커스 유지
	UpdateData(FALSE);
}

//리스트 모두 삭제
void CEMTT::OnBnClickedEmttAllDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_FileList.GetCount() < 0)	return;
	m_FileList.RemoveAll();
	FileListReIndexing();
	FileViewRefresh();
	UpdateData(FALSE);
}

// 라디오 정렬 위
void CEMTT::OnBnClickedEmttSort1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_ListSort_Btn = 0;

	if (m_FileView.GetItemCount() <= 0)return;
	QSortCArray(m_FileList, CompareStringAscEMTT);
	FileListReIndexing();
	FileViewRefresh();
	UpdateData(FALSE);
}

// 라디오 정렬 아래
void CEMTT::OnBnClickedEmttSort2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_ListSort_Btn = 1;
	if (m_FileView.GetItemCount() <= 0)return;
	QSortCArray(m_FileList, CompareStringDescEMTT);
	FileListReIndexing();
	FileViewRefresh();
	UpdateData(FALSE);
}

//버튼 위로
void CEMTT::OnBnClickedEmttBtnUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_FileView.GetItemCount() <= 0) return;
	if (m_FileList.GetCount() <= 0) return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedItem < 0) return;
	if (nSelectedItem == 0) return;
	RowSwap(nSelectedItem, nSelectedItem - 1);
	FileListReIndexing();
	m_FileView.SetItemState(nSelectedItem - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_FileView.EnsureVisible(nSelectedItem - 1, TRUE);
	m_FileView.SetFocus();// 선택된 항목들 포커스 유지

	UpdateData(FALSE);
}


void CEMTT::OnBnClickedEmttBtnDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_FileView.GetItemCount() <= 0)return;
	if (m_FileList.GetCount() <= 0)return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedItem < 0)return;
	if (nSelectedItem == m_FileList.GetCount() - 1)return;
	RowSwap(nSelectedItem, nSelectedItem + 1);
	FileListReIndexing();
	m_FileView.SetItemState(nSelectedItem + 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_FileView.EnsureVisible(nSelectedItem + 1, TRUE);
	m_FileView.SetFocus(); // 선택된 항목들 포커스 유지
	UpdateData(FALSE);
}

//맨 위
void CEMTT::OnBnClickedEmttBtnTop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_FileView.GetItemCount() <= 0)return;
	if (m_FileList.GetCount() <= 0)return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedItem < 0)return;
	m_FileList.InsertAt(0, m_FileList[nSelectedItem]);
	m_FileList.RemoveAt(nSelectedItem + 1);
	FileListReIndexing();
	m_FileView.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_FileView.EnsureVisible(0, TRUE);
	m_FileView.SetFocus(); // 선택된 항목들 포커스 유지
	UpdateData(FALSE);
}

//맨 아래
void CEMTT::OnBnClickedEmttBtnBottom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_FileView.GetItemCount() <= 0)return;
	if (m_FileList.GetCount() <= 0)return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedItem < 0)return;
	if (nSelectedItem == m_FileList.GetCount() - 1)return;
	m_FileList.InsertAt(m_FileView.GetItemCount(), m_FileList[nSelectedItem]);
	m_FileList.RemoveAt(nSelectedItem);
	//RowSwap(nSelectedItem, m_FileList.GetCount() - 1);
	FileListReIndexing();
	m_FileView.SetItemState(m_FileView.GetItemCount(), LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_FileView.EnsureVisible(m_FileView.GetItemCount(), TRUE);
	m_FileView.SetFocus(); // 선택된 항목들 포커스 유지
	UpdateData(FALSE);
}


void CEMTT::OnBnClickedEmttBtnImport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ExcelValueList.RemoveAll();

	int nSelectedItem = m_ExcelView.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedItem < 0)return;
	//if(nSelectedItem == m_ExcelList.GetCount() -1)return;
	bool b_Excel = LoadtoExcel(m_ExcelList[nSelectedItem].Doc_Name, m_ExcelList[nSelectedItem].Sheet_Name);
	UpdateAttBlockData();
	AfxMessageBox(_T("CAD로 가져오기가 완료되었습니다."));
}

void CEMTT::FindBlock(AcDbObjectId Layout_id, CPrintFormDataArray& m_blockRefIds, CString strName)
{

	AcDbObjectId blktbl_id = AcDbObjectId::kNull;
	ACHAR* pLayName = NULL;
	AcDbLayout* pLayout = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pLayout, Layout_id, AcDb::kForRead);
	if (pLayout)
	{
		blktbl_id = pLayout->getBlockTableRecordId();
		pLayout->getLayoutName(pLayName);
		pLayout->close();
	}

	//m_blockRefIds.RemoveAll();
	AcDbObjectIdArray ids;

	AcDbBlockTableRecordPointer blockTable(blktbl_id, AcDb::kForRead);
	if (blockTable.openStatus() == Acad::eOk)
	{
		//m_blockRefIds.RemoveAll();
		AcDbBlockTableRecordIterator* pBtblrIter;
		es = blockTable->newIterator(pBtblrIter);
		if (es != Acad::eOk)
		{
			blockTable->close();
			return;
		}
		int cc = 0;
		// iterate the entities within the layout
		for (; !pBtblrIter->done(); pBtblrIter->step())
		{
			AcDbEntity* pEnt;
			es = pBtblrIter->getEntity(pEnt, AcDb::kForRead);
			if (es != Acad::eOk)
			{
				blockTable->close();
				delete pBtblrIter;
				return;
			}

			// if this is a block reference
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pSelBlockRef = AcDbBlockReference::cast(pEnt);
				if (pSelBlockRef) {
					AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
					if (pBTR.openStatus() == Acad::eOk) {
						ACHAR* Name = NULL;
						pBTR->getName(Name);
						pBTR->close();
						//AfxMessageBox(Name);
						if (strName.CompareNoCase(Name) == 0)
						{
							//m_blockRefIds.append(pSelBlockRef->id());							
							//GetBlockReferenceAttributeList(pSelBlockRef->id(), arr);
							//if (arr.length() > 0) {//블럭속성이 존재할 경우에만 블럭추가.
							ids.append(pSelBlockRef->id());
							CPrintFormData data;
							//data.FormId = pSelBlockRef->id();
							GetBlockPoint(
								pSelBlockRef->id(), 
								data.minpoint_X, data.minpoint_Y, data.maxpoint_X, data.maxpoint_Y, 
								data.scale, data.angle);
							data.FormId = pSelBlockRef->id();
							data.BlockName = Name;
							data.BlockPath = m_blkFilePath;
							data.LayoutName = pLayName;
							m_blockRefIds.Add(data);
						}
					}
				}
			}
			es = pEnt->close();
			if (es != Acad::eOk)
			{
				blockTable->close();
				delete pBtblrIter;
				return;
			}
			cc++;
		}
		CString ccstr;
		ccstr.Format(_T("\n %s의 블록수 : %d"), pLayName, cc);
		//AfxMessageBox(ccstr);
		delete pBtblrIter;
		blockTable->close();
	}
}

BOOL CEMTT::PlotBlockInitExport(AcDbObjectId Layout_id)
{
	//블럭 타이틀 중복 제거하고 array에 저장
	CStringArray m_blkTitleArr, res_m_blkTitleArr;
	for (int i = 0; i < m_ExcelValueList.GetCount(); i++) {
		CString m_blkTitle = m_ExcelValueList[i].dwgBlkTitle;
		m_blkTitleArr.Add(m_blkTitle);
	}
	RemoveDuplicates(m_blkTitleArr, res_m_blkTitleArr);

	m_blockRefIds.RemoveAll();
	for (int j = 0; j < res_m_blkTitleArr.GetCount(); j++) {
		//m_blockRefIds.RemoveAll();
		FindBlock(Layout_id, m_blockRefIds, res_m_blkTitleArr[j]);
	}

	if (m_blockRefIds.GetCount() > 0) {
		if (m_RadSort_Btn == 0)
			ZSort(m_SortError);
		else if (m_RadSort_Btn == 1)
			ZMSort(m_SortError);
		else if (m_RadSort_Btn == 2)
			NSort(m_SortError);
		else if (m_RadSort_Btn == 3)
			NMSort(m_SortError);
	}
	//if(m_blockRefIds.GetCount() > 0)
	//	m_PrintRefIds.Append(m_blockRefIds);

	for (int k = 0; k < m_blockRefIds.GetCount(); k++) {
		UpdateBlock(Layout_id, arrarr[arrarrcnt], m_blockRefIds[k].FormId);
		arrarrcnt++;
		//acutPrintf(_T("\n%d"),arrarrcnt);
	}

	return true;
}

//엑셀 값들 배열에 추출
bool CEMTT::LoadtoExcel(CString DocName, CString SheetName) {
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	COleVariant VTrue((short)TRUE);
	COleVariant VFalse((short)FALSE);
	CApplication app;
	CRange startrng;
	LPDISPATCH lpdisp;
	CWorksheet sheet;
	CWorkbook book;
	CWorkbooks books;
	CWorksheets sheets;
	int column1 = 0;
	int column2 = 1;
	int pointcnt;
	// app 엑셀 오브젝트	
	::CoInitialize(NULL);

	IDispatch* pdispExcelApp = NULL;
	IUnknown* pUnk = NULL;
	CLSID clsExcelApp;


	if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsExcelApp)))
	{
		AfxMessageBox(_T("엑셀이 설치되어 있지 않습니다."));
	}



	HRESULT hr = GetActiveObject(clsExcelApp, NULL, (IUnknown**)&pUnk);
	if (SUCCEEDED(hr)) {
		hr = pUnk->QueryInterface(IID_IDispatch, (void**)&pdispExcelApp);
	}
	else
	{
		AfxMessageBox(_T("엑셀을 찾을 수 없습니다."));
	}

	//App 찾기 실패시 리턴
	if (NULL == pdispExcelApp) {
		return false;
	}

	//Application

	app.AttachDispatch(pdispExcelApp);

	//Workbooks	

	lpdisp = app.get_Workbooks();
	ASSERT(lpdisp);
	books.AttachDispatch(lpdisp);
	//Books
	ExcelInfosEMTT exif;
	CString bookName;
	int booksCnt = books.get_Count();
	for (int i = 1; i <= books.get_Count(); i++) {//선택 문서 찾으면 for문 나옴
		lpdisp = books.get_Item(COleVariant((short)i));
		ASSERT(lpdisp);
		//book
		book.AttachDispatch(lpdisp);
		if (book.get_Name() == DocName)
			break;

	}
	//sheets
	lpdisp = book.get_Sheets();
	ASSERT(lpdisp);
	sheets.AttachDispatch(lpdisp);

	//m_ExcelList[i-1].Sheet_Name = sheet.get_Name();
	for (int i = 1; i <= sheets.get_Count(); i++) {//선택 시트 찾으면 for문 나옴
		//sheet
		lpdisp = sheets.get_Item(COleVariant((short)i));
		ASSERT(lpdisp);
		sheet.AttachDispatch(lpdisp);
		if (sheet.get_Name() == SheetName)
			break;
	}

	//startrng = sheet.get_Range(COleVariant(_T("A1")),COleVariant(_T("Z100")));
	startrng = sheet.get_Cells();
	CString SelectXIsSheet;
	int SelectXIsIndex = 0;
	lpdisp = startrng.get_Parent();
	sheet.AttachDispatch(lpdisp);
	SelectXIsSheet = sheet.get_Name();
	SelectXIsIndex = sheet.get_Index();

	int iRows, iCols;
	CRange rowRang = startrng.get_Rows();
	iRows = rowRang.get_Count();
	CRange colRang = startrng.get_Columns();
	iCols = colRang.get_Count();

	CRange curRange;
	curRange = startrng.get_Item(COleVariant((long)1), COleVariant((long)1)).pdispVal;
	CString str = curRange.get_Value2();
	if (str.CompareNoCase(_T("번호")) != 0)
		return false;
	curRange = startrng.get_Item(COleVariant((long)1), COleVariant((long)2)).pdispVal;
	str = curRange.get_Value2();
	if (str.CompareNoCase(_T("경로")) != 0)
		return false;
	curRange = startrng.get_Item(COleVariant((long)1), COleVariant((long)3)).pdispVal;
	str = curRange.get_Value2();
	if (str.CompareNoCase(_T("Layout")) != 0)
		return false;
	curRange = startrng.get_Item(COleVariant((long)1), COleVariant((long)4)).pdispVal;
	str = curRange.get_Value2();
	if (str.CompareNoCase(_T("블록명")) != 0)
		return false;

	//curRange = sheet.get_Range(COleVariant(_T("A1")), VOptional);
	//curRange = curRange.get_Resize(COleVariant((short)50), COleVariant((short)100));

	//열 개수.
	int m_MaxColIdx = 1, m_MaxRowIdx = 1;
	/*
	for(long j = 1; j<=iRows; j++){
		curRange = startrng.get_Item(COleVariant((long)j), COleVariant((long)1)).pdispVal;
		CString aa = curRange.get_Value2();
		if(aa.CompareNoCase(_T("")) == 0)
			break;
		//acutPrintf(_T("\n Row : %s"),aa);
		m_MaxRowIdx++;
	}
	*/
	int j = 1;
	while (true) {
		curRange = startrng.get_Item(COleVariant((long)j), COleVariant((long)1)).pdispVal;
		CString aa = curRange.get_Value2();
		if (aa.CompareNoCase(_T("")) == 0)
			break;
		//acutPrintf(_T("\n Row : %s"),aa);
		m_MaxRowIdx++;
		j++;
	}

	//행 개수
	/*
	for(long k=1; k<=iCols; k++){
		curRange = startrng.get_Item(COleVariant((long)1), COleVariant((long)k)).pdispVal;
		CString aa = curRange.get_Value2();
		if(aa.CompareNoCase(_T("")) == 0)
			break;
		//acutPrintf(_T("\n Col : %s"),aa);
		m_MaxColIdx++;
	}
	*/
	int k = 1;
	while (true) {
		curRange = startrng.get_Item(COleVariant((long)1), COleVariant((long)k)).pdispVal;
		CString aa = curRange.get_Value2();
		if (aa.CompareNoCase(_T("")) == 0)
			break;
		//acutPrintf(_T("\n Col : %s"),aa);
		m_MaxColIdx++;
		k++;
	}

	startrng = sheet.get_Range(COleVariant(_T("A1")), VOptional);
	startrng = startrng.get_Resize(COleVariant((long)m_MaxRowIdx), COleVariant((long)m_MaxColIdx));

	rowRang = startrng.get_Rows();
	iRows = rowRang.get_Count();
	colRang = startrng.get_Columns();
	iCols = colRang.get_Count();

	//acutPrintf(_T("\n Row : %d"),iRows-1);
	//acutPrintf(_T("\n Col : %d"),iCols-1);
	m_txtExcelLst.erase(m_txtExcelLst.begin(), m_txtExcelLst.end());
	arrarr.removeAll();
	arr.removeAll();
	ExcelValueEMTT Exv;
	AttBlock att;


	if (iRows - 1 < 1)return false;	//열이 1개 이하이면 종료
	//엑셀 값들 배열에 넣음.
	for (int i = 2; i <= iRows - 1; i++) {

		for (int j = 2; j <= iCols - 1; j++) {
			if (j == 2) {
				curRange = startrng.get_Item(COleVariant((long)i), COleVariant((long)j)).pdispVal;
				Exv.dwgPath = curRange.get_Value2();
			}
			else if (j == 3) {
				curRange = startrng.get_Item(COleVariant((long)i), COleVariant((long)j)).pdispVal;
				Exv.dwgLayout = curRange.get_Value2();
			}
			else if (j == 4) {
				curRange = startrng.get_Item(COleVariant((long)i), COleVariant((long)j)).pdispVal;
				Exv.dwgBlkTitle = curRange.get_Value2();
			}
			else {// j >= 5
				curRange = startrng.get_Item(COleVariant((long)i), COleVariant((long)j)).pdispVal;
				CString m_ColValueStr = curRange.get_Value2();
				//if(m_ColValueStr.CompareNoCase(_T(""))!=0){
				att.strValue = curRange.get_Value2();
				curRange = startrng.get_Item(COleVariant((long)1), COleVariant((long)j)).pdispVal;
				att.AttributeTag = curRange.get_Value2();
				arr.append(att);
				//}

			}
		}
		//acutPrintf(_T("\n count : %d"),arr.length());

		m_ExcelValueList.Add(Exv);
		arrarr.append(arr);
		arr.removeAll();
	}

	/*
	acutPrintf(_T("\n arrarr[0] count : %d"), arrarr[0].length());

	for (int i = 0; i < arrarr[0].length(); i++) {
		acutPrintf(_T("\n%s = %s\n"), arrarr[0][i].AttributeTag, arrarr[0][i].strValue);
	}
	*/
	//acutPrintf(_T("\n count : %d"),m_ExcelValueList.GetCount());
	//acutPrintf(_T("\n count : %d"),arrarr.length());

	return true;
}

//속성 정보 캐드로 dwg정보 = m_ExcelValueList, 속성정보 = arrarr,arr
bool CEMTT::UpdateAttBlockData() {
	//엑셀의 블록속성배열의 수를 UpdateBlock에서 중복된 값을 가져오지 않게 하기 위해
	arrarrcnt = 0;

	for (int i = 0; i < m_ExcelValueList.GetCount(); i++)
	{
		//일단 해당 파일이 열려있는지 확인하자.
		//열려있을경우 새로 파일을 열지 말자		
		//AfxMessageBox(m_FileList[i].full_path);
		if (i > 0) {//이전 블로경로,레이아웃이 같으면 넘어감. PlotBlockInitExport->Findblock 함수에서 중복부분 처리
			if (m_ExcelValueList[i].dwgPath == m_ExcelValueList[i - 1].dwgPath) {
				if (m_ExcelValueList[i].dwgLayout == m_ExcelValueList[i - 1].dwgLayout) {
					continue;
				}
			}
		}
		m_blkFilePath = m_ExcelValueList[i].dwgPath;	//m_blkFilePath를 전역으로 저장해둠 Findblock에서 쓰기위해

		AcApDocument* doc;
		BOOL bIsopen = IsOpenDocument(m_ExcelValueList[i].dwgPath);
		if (bIsopen) {
			doc = GetOpendDocument(m_ExcelValueList[i].dwgPath);
			if (doc != NULL)	acDocManager->setCurDocument(doc);
		}
		else
		{
			Acad::ErrorStatus es = acDocManager->appContextOpenDocument(m_ExcelValueList[i].dwgPath);
		}

		AcApDocument* activeDoc = acDocManager->curDocument();
		AcApLayoutManager* pLayoutManager = (AcApLayoutManager*)acdbHostApplicationServices()->layoutManager();
		const ACHAR* ActiveLayoutName = pLayoutManager->findActiveLayout(Adesk::kTrue);
		AcDbDatabase* pDb = activeDoc->database();

		acDocManager->lockDocument(acDocManager->curDocument());

		CArray <AcDbObjectId, AcDbObjectId> arrLayout;
		GetLayout(pDb, arrLayout);//레이아웃 목록뽑아옴

		AcDbObject* pObj;


		//도면의 레이아웃들중 처음 보이는 레이아웃을 저장해 놓았다가 파일을 닫기전에 원래 상태로 만들고 끝내자
		const ACHAR* OriginLayout = NULL;
		/*
		함수 변경
		AcApLayoutManager Class
		virtual const ACHAR * findActiveTab();
		virtual Acad::ErrorStatus getActiveTab(AcString & sName) = 0;
		OriginLayout = pLayoutManager->findActiveTab();
		*/

#ifdef _ZCAD || _ZXYCAD
		OriginLayout = pLayoutManager->findActiveTab();
#elif _ACAD
		AcString ActiveName;
		OriginLayout = (ACHAR*)pLayoutManager->getActiveTab(ActiveName);
#endif

		const ACHAR* csModel = acdbGetReservedString(AcDb::kModel);
		CString strModel(csModel);


		AcDbObjectId layout_id = AcDbObjectId::kNull;
		for (int k = 0; k < arrLayout.GetCount(); k++)
		{
			ACHAR* pLayName = NULL;
			AcDbLayout* pLayout = NULL;
			Acad::ErrorStatus es = acdbOpenObject(pLayout, arrLayout[k], AcDb::kForWrite);
			if (pLayout)
			{
				pLayout->getLayoutName(pLayName);
				//레이아웃이 엑셀값과 같을때 실행
				if (((CString)pLayName).CompareNoCase(m_ExcelValueList[i].dwgLayout) == 0)
				{
					layout_id = pLayout->id();
					pLayoutManager->setCurrentLayoutId(pLayout->id());
					pLayoutManager->setCurrentLayout(pLayName);
					pLayoutManager->updateLayoutTabs();

					pLayout->close();
					//UpdateBlock(layout_id,arrarr[i],m_ExcelValueList[i].dwgBlkTitle);
					PlotBlockInitExport(layout_id);
					break;

				}
				pLayout->close();

			}
		}


		//도면의 레이아웃들중 처음 보이는 레이아웃을 저장해 놓았다가 파일을 닫기전에 원래 상태로 만들고 끝내자
		pLayoutManager->setCurrentLayout(OriginLayout);
		pLayoutManager->updateLayoutTabs();


		acDocManager->unlockDocument(acDocManager->curDocument());
		if (!bIsopen) {
			acDocManager->unlockDocument(activeDoc);
			acDocManager->activateDocument(activeDoc);
			acDocManager->lockDocument(activeDoc); // Lock
			activeDoc->database()->saveAs(acDocManager->curDocument()->fileName());
			acdbSetDbmod(activeDoc->database(), 0); // clear changes flag
			acDocManager->unlockDocument(activeDoc);//unlock
			acDocManager->closeDocument(activeDoc);
			activeDoc = NULL;

		}
		else {
			if (i > 0) {
				if (m_ExcelValueList[i].dwgPath == m_ExcelValueList[i - 1].dwgPath) {
					acDocManager->unlockDocument(activeDoc);
					acDocManager->activateDocument(activeDoc);
					acDocManager->lockDocument(activeDoc); // Lock
					activeDoc->database()->saveAs(acDocManager->curDocument()->fileName());
					acdbSetDbmod(activeDoc->database(), 0); // clear changes flag
					acDocManager->unlockDocument(activeDoc);//unlock
					acDocManager->closeDocument(activeDoc);
					activeDoc = NULL;
				}
				else
					acDocManager->unlockDocument(activeDoc);
			}

		}

	}
	return true;
}

//속성블럭 ID검색하여 엑셀값으로 변경
void CEMTT::UpdateBlock(AcDbObjectId Layout_id, AttBlockArray& m_AttblkArr, AcDbObjectId m_blkID)
{

	AcDbObjectId blktbl_id = AcDbObjectId::kNull;
	ACHAR* pLayName = NULL;
	AcDbLayout* pLayout = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pLayout, Layout_id, AcDb::kForRead);
	if (pLayout)
	{
		blktbl_id = pLayout->getBlockTableRecordId();
		pLayout->getLayoutName(pLayName);
		pLayout->close();
	}

	//m_blockRefIds.RemoveAll();
	AcDbObjectIdArray ids;

	AcDbBlockTableRecordPointer blockTable(blktbl_id, AcDb::kForRead);
	if (blockTable.openStatus() == Acad::eOk)
	{
		//m_blockRefIds.RemoveAll();
		AcDbBlockTableRecordIterator* pBtblrIter;
		es = blockTable->newIterator(pBtblrIter);
		if (es != Acad::eOk)
		{
			blockTable->close();
			return;
		}
		int cc = 0;
		// iterate the entities within the layout
		for (; !pBtblrIter->done(); pBtblrIter->step())
		{
			AcDbEntity* pEnt;
			es = pBtblrIter->getEntity(pEnt, AcDb::kForRead);
			if (es != Acad::eOk)
			{
				blockTable->close();
				delete pBtblrIter;
				return;
			}

			// if this is a block reference
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pSelBlockRef = AcDbBlockReference::cast(pEnt);
				if (pSelBlockRef) {
					AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
					if (pBTR.openStatus() == Acad::eOk) {
						ACHAR* Name = NULL;
						pBTR->getName(Name);
						//AfxMessageBox(Name);
						if (m_blkID == pSelBlockRef->id())//if(strName.CompareNoCase(Name) ==0)
						{
							//m_blockRefIds.append(pSelBlockRef->id());							
							utils.GetBlockReferenceAttributeList(pSelBlockRef->id(), compareArr);

							//속성의 개수가 0 또는 가져온 속성개수와 같지 않을때 반복
							if (/*compareArr.length() != m_AttblkArr.length() ||*/ compareArr.length() == 0)continue;
							//AfxMessageBox(_T("ok"));
							for (int i = 0; i < m_AttblkArr.length(); i++) {
								CString m_AttTag, m_AttValue;
								m_AttTag = m_AttblkArr[i].AttributeTag;
								m_AttValue = m_AttblkArr[i].strValue;
								utils.SetBlockReferenceAttribute(pSelBlockRef->id(), m_AttTag, m_AttValue);
							}
							break;
						}
						pBTR->close();
					}
				}
			}

			es = pEnt->close();
			if (es != Acad::eOk)
			{
				blockTable->close();
				delete pBtblrIter;
				return;
			}
			cc++;
		}

		delete pBtblrIter;
		blockTable->close();
	}

}

#pragma  region 정렬
//z->방향 소트 (완료)
void CEMTT::ZSort(double tol) {
	int blockCnt = m_blockRefIds.GetCount();
	if (blockCnt == 1)
		return;

	QSortCArray(m_blockRefIds, EMTTComparePntY_De);
	for (int i = 0; i < blockCnt; i++) {
		for (int k = 0; k < blockCnt - 1; k++) {
			double a = m_blockRefIds[k].minpoint_Y;
			double b = m_blockRefIds[k + 1].minpoint_Y;
			if (abs(m_blockRefIds[k].minpoint_Y - m_blockRefIds[k + 1].minpoint_Y) < tol) {
				if (m_blockRefIds[k].minpoint_X > m_blockRefIds[k + 1].minpoint_X) {
					CPrintFormData TmpId;
					TmpId = m_blockRefIds[k];
					m_blockRefIds[k] = m_blockRefIds[k + 1];
					m_blockRefIds[k + 1] = TmpId;
				}
			}
		}
	}
}

//N↑방향 소트 (완료)
void CEMTT::NSort(double tol) {
	int blockCnt = m_blockRefIds.GetCount();
	if (blockCnt == 1)	return;

	QSortCArray(m_blockRefIds, EMTTComparePntX);
	for (int i = 0; i < blockCnt; i++) {
		for (int k = 0; k < blockCnt - 1; k++) {
			if (abs(m_blockRefIds[k].minpoint_X - m_blockRefIds[k + 1].minpoint_X) < tol) {
				if (m_blockRefIds[k].minpoint_Y < m_blockRefIds[k + 1].minpoint_Y) {
					CPrintFormData TmpId;
					TmpId = m_blockRefIds[k];
					m_blockRefIds[k] = m_blockRefIds[k + 1];
					m_blockRefIds[k + 1] = TmpId;
				}
			}
		}
	}
}

//z 반전 방향 소트 (완료)
void CEMTT::ZMSort(double tol) {
	int blockCnt = m_blockRefIds.GetCount();
	if (blockCnt == 1)	return;
	QSortCArray(m_blockRefIds, EMTTComparePntY);

	for (int i = 0; i < blockCnt; i++) {
		for (int k = 0; k < blockCnt - 1; k++) {
			if (abs(m_blockRefIds[k].minpoint_Y - m_blockRefIds[k + 1].minpoint_Y) < tol) {
				if (m_blockRefIds[k].minpoint_X > m_blockRefIds[k + 1].minpoint_X) {
					CPrintFormData TmpId;
					TmpId = m_blockRefIds[k];
					m_blockRefIds[k] = m_blockRefIds[k + 1];
					m_blockRefIds[k + 1] = TmpId;
				}
			}
		}
	}
}

//N 반전 방향 소트
void CEMTT::NMSort(double tol) {
	int blockCnt = m_blockRefIds.GetCount();
	if (blockCnt == 1)	return;
	QSortCArray(m_blockRefIds, EMTTComparePntX);

	for (int i = 0; i < blockCnt; i++) {
		for (int k = 0; k < blockCnt - 1; k++) {
			if (abs(m_blockRefIds[k].minpoint_X - m_blockRefIds[k + 1].minpoint_X) < tol) {
				if (m_blockRefIds[k].minpoint_Y > m_blockRefIds[k + 1].minpoint_Y) {
					CPrintFormData TmpId;
					TmpId = m_blockRefIds[k];
					m_blockRefIds[k] = m_blockRefIds[k + 1];
					m_blockRefIds[k + 1] = TmpId;
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////
//문자 리스트 컨트롤
////////////////////////////////////////////////////////////////////////

// 객체 추가
void CEMTT::OnBnClickedEmttTxtAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->ShowWindow(SW_HIDE);
	//acDocManager->lockDocument(acDocManager->curDocument());
	UpdateData(true);
	m_TXTView.DeleteAllItems();

	ads_name entName, FormEnt;
	ads_point entPt, FormPt;
	Acad::ErrorStatus es, BlockES;
	bool bBlock = false;

	// 좌표 변환에 필요한 요소
	struct resbuf fromrb, toWCS, toDCS;
	fromrb.restype = RTSHORT;
	fromrb.resval.rint = 1; // UCS
	toWCS.restype = RTSHORT;
	toWCS.resval.rint = 0; // WCS 
	toDCS.restype = RTSHORT;
	toDCS.resval.rint = 2; // DCS

	AcGePoint3d minUCSPt, maxUCSPt, minWCSPt, maxWCSPt, minDCSPt, maxDCSPt;

	// 문자 또는 속성 객체 선택
	if (acedEntSel(_T("\n문자 또는 속성 선택:"), entName, entPt) != RTNORM) {
		this->ShowWindow(SW_SHOW);
		return;
	}

	AcDbObjectId objId;
	acdbGetObjectId(objId, entName);
	double angle = 0;
	AcGeScale3d scale = 0.0;
	AcDbObjectIdArray ids;
	ACHAR* BlockName = NULL;
	bool bInsert = false, bAtt = false;
	TxTEMTT ptxtlst;
	attrEMTT pattrlst;

	AcDbEntityPointer pEnt(objId, AcDb::kForRead);
	if (pEnt.openStatus() == Acad::eOk) {
		//단일행 문자
		if (pEnt->isKindOf(AcDbText::desc())) {
			AcDbText* pText = AcDbText::cast(pEnt);
			for (int i = 0; i < m_TxtList.size(); i++) {
				if (m_TxtList[i].Txt_Tag.CompareNoCase(pText->textString()) == 0) {
					bInsert = true;
					break;
				}
			}

			if (!bInsert) {
				// 문자 범위 선택 
				// SelectArea_minpt SelectArea_maxpt
				GetWindow();

				// 문자 속성 대입
				ptxtlst.EntityName = L"단일문자";
				ptxtlst.Txt_Tag = pText->textString();
				ptxtlst.Pos_Value.Format(L"X:%f,Y:%f,Z:%f",
					pText->position().x, pText->position().y, pText->position().z);
				ptxtlst.AreaMin = SelectArea_minpt;
				ptxtlst.AreaMax = SelectArea_maxpt;
			}

		}
		//다중행 문자
		else if (pEnt->isKindOf(AcDbMText::desc())) {
			AcDbMText* pMText = AcDbMText::cast(pEnt);
			for (int i = 0; i < m_TxtList.size(); i++) {
				if (m_TxtList[i].Txt_Tag.CompareNoCase(pMText->contents()) == 0) {
					bInsert = true;
					break;
				}
			}

			if (!bInsert) {
				// 문자 범위 선택 
				// SelectArea_minpt SelectArea_maxpt
				GetWindow();

				// 문자 속성 대입
				ptxtlst.EntityName = L"다중문자";
				ptxtlst.Txt_Tag = pMText->contents();
				ptxtlst.Pos_Value.Format(L"X:%f,Y:%f,Z:%f",
					pMText->location().x, pMText->location().y, pMText->location().z);
				ptxtlst.AreaMin = SelectArea_minpt;
				ptxtlst.AreaMax = SelectArea_maxpt;
			}
		}
		//속성 - 대화상자
		else if (pEnt->isKindOf(AcDbBlockReference::desc())) {
			AcDbBlockReference* pSelBlockRef = AcDbBlockReference::cast(pEnt);
			if (pSelBlockRef) {
				AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
				if (pBTR.openStatus() == Acad::eOk) {
					bAtt = true;
					pBTR->getName(BlockName);
					// m_SearchForm_Str(도곽폼)에 추가
					pBTR->getBlockReferenceIds(ids);
					if (pBTR->hasAttributeDefinitions()) {
						es = acdbOpenObject(pSelBlockRef, ids[0], AcDb::kForRead);
						AcDbObjectIterator* Itr = pSelBlockRef->attributeIterator();
						int i = 0;
						for (Itr->start(); !Itr->done(); Itr->step()) {
							AcDbAttribute* pAttr;
							AcDbObjectId attributeId = Itr->objectId();
							es = pSelBlockRef->openAttribute(pAttr, attributeId, AcDb::kForRead);
							//if (pAttr != NULL && !pAttr->isConstant()) {}
							// 태그 / 속성 모으기
							ptxtlst.EntityName = L"고급속성";
							ptxtlst.Txt_Tag = pAttr->tag();
							ptxtlst.Pos_Value = pAttr->textString();

							// 대입한 요소의 구조체 추가
							m_TxtList.emplace_back(ptxtlst);

							i++;
						}
					}
				}
			}
		}
	}

	// 도각 블럭 선택
	if (!bAtt) {	// 선택한 객체가 속성이 아닐 경우에만 진행
		int state = acedEntSel(_T("\n블럭 선택(선택안할 시 엔터):"), FormEnt, FormPt);

		// 선택한 객체가 블럭인지 확인
		if (state == RTNORM) { // 도곽 순서대로 인식
			AcDbObjectId RefId = AcDbObjectId::kNull;
			AcDbObjectId RefTblId = AcDbObjectId::kNull;
			acdbGetObjectId(RefId, FormEnt);
			AcDbEntityPointer pREfEnt(RefId, AcDb::kForRead);
			if (pREfEnt.openStatus() == Acad::eOk)	{
				if (pREfEnt->isA() == AcDbBlockReference::desc()) {
					bBlock = true;
					ptxtlst.FormBlk = true;

					// 블럭 영역 구하기
					AcDbBlockReference* pSelFormRef = AcDbBlockReference::cast(pREfEnt);
					if (pSelFormRef) {
						AcDbExtents FormArea;

						AcGePoint3d TartgetPt;
						AcGeVector3d vViewdir;
						bool isZero = getTarget_ViewDir(TartgetPt, vViewdir);

						pSelFormRef->getGeomExtents(FormArea);
						AcDbExtents extentsDCS, tmpExtents;
						AcGePoint3d FormMin, FormMax;

						tmpExtents = FormArea;

						AcGeMatrix3d mat;
						mat.setToTranslation((TartgetPt.asVector()));

						minUCSPt = tmpExtents.minPoint();
						maxUCSPt = tmpExtents.maxPoint();

						minUCSPt.transformBy(mat);
						maxUCSPt.transformBy(mat);

						// WCS to DCS
						acedTrans(asDblArray(minUCSPt), &toWCS, &toDCS, FALSE, asDblArray(FormMin));
						acedTrans(asDblArray(maxUCSPt), &toWCS, &toDCS, FALSE, asDblArray(FormMax));

						extentsDCS.addPoint(FormMin);
						extentsDCS.addPoint(FormMax);

						FormArea_minpt = AcGePoint3d(extentsDCS.minPoint().x, extentsDCS.minPoint().y, extentsDCS.minPoint().z);
						FormArea_maxpt = AcGePoint3d(extentsDCS.maxPoint().x, extentsDCS.maxPoint().y, extentsDCS.maxPoint().z);

						scale = pSelFormRef->scaleFactors();
						RefTblId = pSelFormRef->blockTableRecord();
						//pREfEnt->close();

						// 도곽 블럭 이름 추가
						AcDbBlockTableRecord* pBTR;
						es = acdbOpenObject(pBTR, RefTblId, AcDb::kForRead);
						if (es == Acad::eOk) {
							const TCHAR* pBlkName;
							es = pBTR->getName(pBlkName);
							pBTR->close();
							m_SearchForm_Str.Replace(_T(" "), NULL);
							if (m_SearchForm_Str.CompareNoCase(_T("")) == 0) {
								m_SearchForm_Str = m_SearchForm_Str + pBlkName;
							}
							else {
								if (m_SearchForm_Str.Right(1) == _T(","))
									m_SearchForm_Str = m_SearchForm_Str + _T(" ") + pBlkName;
								else {
									m_SearchForm_Str = m_SearchForm_Str + _T(", ") + pBlkName;
								}
							}

							// 구조체 대입 - 블럭 이름
							ptxtlst.BlockName = pBlkName;
						}
					}

					// 벡터 구한 후 구조체에 대입
					AcDbExtents FormArea_Tmp, SelectArea_Tmp;
					AcGeMatrix3d mat1;

					// [보류] Z값 0 으로 지정 
					// Z축을 기준으로 돌리는데 Z축의 깊이가 다를경우 벡터의 크기가 달라진다.
					FormArea_minpt.z = 0;
					FormArea_maxpt.z = 0;
					SelectArea_minpt.z = 0;
					SelectArea_maxpt.z = 0;

					// 임시의 AcDbExtents 에 대입
					// AcDbExtents 로 돌리지 않으면 이상하게 됬다.
					FormArea_Tmp.addPoint(FormArea_minpt);
					FormArea_Tmp.addPoint(FormArea_maxpt);
					SelectArea_Tmp.addPoint(SelectArea_minpt);
					SelectArea_Tmp.addPoint(SelectArea_maxpt);
					mat1.setToRotation(-angle, AcGeVector3d::kZAxis);
					FormArea_Tmp.transformBy(mat1);
					SelectArea_Tmp.transformBy(mat1);

					// 벡터 = 선택 영역 - 도곽 영역
					AcGeVector3d minvec = (SelectArea_Tmp.minPoint() - FormArea_Tmp.minPoint());
					AcGeVector3d maxvec = (SelectArea_Tmp.maxPoint() - FormArea_Tmp.maxPoint());

					// 벡터 = 벡터/스케일 (1기준)
					minvec.x = (minvec.x / scale.sx);
					minvec.y = (minvec.y / scale.sy);
					minvec.z = 0;
					maxvec.x = (maxvec.x / scale.sx);
					maxvec.y = (maxvec.y / scale.sy);
					maxvec.z = 0;

					// 구조체 대입 - 벡터
					ptxtlst.MinVector = AcGePoint3d(minvec.x, minvec.y, minvec.z);
					ptxtlst.MaxVector = AcGePoint3d(maxvec.x, maxvec.y, maxvec.z);
				}
				else {
					bBlock = false;
					ptxtlst.FormBlk = false;
					acutPrintf(_T("\n선택된 엔티티가 블록이 아닙니다.\n"));
				}
				pREfEnt->close();
			}
			// 대입한 요소의 구조체 추가
			m_TxtList.emplace_back(ptxtlst);
		}
		else {// 특정 문자열 순서대로 인식
			bBlock = false;
			ptxtlst.FormBlk = false;

			// 대입한 요소의 구조체 추가(
			m_TxtList.emplace_back(ptxtlst);
		}
	}

	// 리스트 추가
	for (int i = 0; i < m_TxtList.size(); i++) {
		m_TXTView.InsertItem(i, m_TxtList[i].EntityName);
		m_TXTView.SetItem(i, 1, LVIF_TEXT, m_TxtList[i].Txt_Tag, 0, 0, 0, NULL);
		m_TXTView.SetItem(i, 2, LVIF_TEXT, m_TxtList[i].Pos_Value, 0, 0, 0, NULL);
	}
	UpdateData(FALSE);
	// 설정값 저장

	//acDocManager->unlockDocument(acDocManager->curDocument());
	this->ShowWindow(SW_SHOW);
}


void CEMTT::OnBnClickedEmttBtnExport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	this->ShowWindow(SW_HIDE);
	m_blockRefIds.RemoveAll();
	m_StrList.RemoveAll();
	m_txtExcelLst.erase(m_txtExcelLst.begin(), m_txtExcelLst.end());
	m_TxtExcel.erase(m_TxtExcel.begin(), m_TxtExcel.end());
	arrarr.removeAll();
	arr.removeAll();
	m_PrintRefIds.RemoveAll();
	//검색할 Form 이름 
	CharacterExtraction(m_SearchForm_Str);
	//AfxMessageBox(_T("1"));
	for (int i = 0; i < m_FileList.GetCount(); i++)
	{
		//일단 해당 파일이 열려있는지 확인하자.
		//열려있을경우 새로 파일을 열지 말자
		m_blkFilePath = m_FileList[i].full_path;
		//AfxMessageBox(m_FileList[i].full_path);
		AcApDocument* doc;
		BOOL bIsopen = IsOpenDocument(m_blkFilePath);
		if (bIsopen) {
			doc = GetOpendDocument(m_blkFilePath);
			if (doc != NULL)	acDocManager->setCurDocument(doc);
		}
		else
		{
			Acad::ErrorStatus es = acDocManager->appContextOpenDocument(m_blkFilePath);
		}

		AcApDocument* activeDoc = acDocManager->curDocument();
		AcApLayoutManager* pLayoutManager = (AcApLayoutManager*)acdbHostApplicationServices()->layoutManager();
		const ACHAR* ActiveLayoutName = pLayoutManager->findActiveLayout(Adesk::kTrue);
		AcDbDatabase* pDb = activeDoc->database();

		acDocManager->lockDocument(acDocManager->curDocument());

		CArray <AcDbObjectId, AcDbObjectId> arrLayout;
		GetLayout(pDb, arrLayout);

		AcDbObject* pObj;


		//도면의 레이아웃들중 처음 보이는 레이아웃을 저장해 놓았다가 파일을 닫기전에 원래 상태로 만들고 끝내자
		const ACHAR* OriginLayout = NULL;
		/*
		함수 변경
		AcApLayoutManager Class
		virtual const ACHAR * findActiveTab();
		virtual Acad::ErrorStatus getActiveTab(AcString & sName) = 0;
		OriginLayout = pLayoutManager->findActiveTab();
		*/
#if _ZCAD || _ZXYCAD
		OriginLayout = pLayoutManager->findActiveTab();
#elif _ACAD
		AcString ActiveName;
		OriginLayout = (ACHAR*)pLayoutManager->getActiveTab(ActiveName);
#endif

		const ACHAR* csModel = acdbGetReservedString(AcDb::kModel);
		CString strModel(csModel);


		//모형
		if (m_RadSearch_Btn == 0)
		{

			AcDbObjectId layout_id = AcDbObjectId::kNull;
			for (int k = 0; k < arrLayout.GetCount(); k++)
			{
				ACHAR* pLayName = NULL;
				AcDbLayout* pLayout = NULL;
				Acad::ErrorStatus es = acdbOpenObject(pLayout, arrLayout[k], AcDb::kForWrite);
				if (pLayout)
				{
					pLayout->getLayoutName(pLayName);
					if (((CString)pLayName).CompareNoCase(_T("Model")) == 0 || ((CString)pLayName).CompareNoCase(_T("모형")) == 0)
					{
						layout_id = pLayout->id();
						pLayoutManager->setCurrentLayoutId(pLayout->id());
						pLayoutManager->setCurrentLayout(pLayName);
						pLayoutManager->updateLayoutTabs();

						pLayout->close();
						PlotBlockInit(layout_id);
						break;

					}
					pLayout->close();

				}
			}
			//start

		//ExportExcel(layout_id);

		}
		else if (m_RadSearch_Btn == 1)//배치
		{

			AcDbObjectId layout_id = AcDbObjectId::kNull;
			for (int k = 0; k < arrLayout.GetCount(); k++)
			{
				ACHAR* pLayName = NULL;
				AcDbLayout* pLayout = NULL;
				Acad::ErrorStatus es = acdbOpenObject(pLayout, arrLayout[k], AcDb::kForWrite);
				if (pLayout)
				{
					pLayout->getLayoutName(pLayName);
					if (((CString)pLayName).CompareNoCase(_T("Model")) != 0 && ((CString)pLayName).CompareNoCase(_T("모형")) != 0)
					{
						layout_id = pLayout->id();
						//pLayoutManager->setCurrentLayoutId(pLayout->id());
						pLayoutManager->setCurrentLayout(pLayName);
						pLayoutManager->updateLayoutTabs();

						pLayout->close();
						PlotBlockInit(layout_id);
					}
					else
						pLayout->close();

				}

			}
			//ExportExcel(layout_id);
		}
		else if (m_RadSearch_Btn == 2) {	//모형 + 배치
			AcDbObjectId layout_id = AcDbObjectId::kNull;
			for (int k = 0; k < arrLayout.GetCount(); k++)
			{
				ACHAR* pLayName = NULL;
				AcDbLayout* pLayout = NULL;
				Acad::ErrorStatus es = acdbOpenObject(pLayout, arrLayout[k], AcDb::kForWrite);
				if (pLayout)
				{
					layout_id = pLayout->id();
					pLayout->getLayoutName(pLayName);
					pLayout->close();
					//	pLayoutManager->setCurrentLayoutId(pLayout->id());
					pLayoutManager->setCurrentLayout(pLayName);
					pLayoutManager->updateLayoutTabs();

					PlotBlockInit(layout_id);
				}

			}

		}
		else if (m_RadSearch_Btn == 4) {
			AcDbObjectId layout_id = AcDbObjectId::kNull;
			for (int k = 0; k < arrLayout.GetCount(); k++)
			{
				ACHAR* pLayName = NULL;
				AcDbLayout* pLayout = NULL;
				Acad::ErrorStatus es = acdbOpenObject(pLayout, arrLayout[k], AcDb::kForWrite);
				if (pLayout)
				{
					pLayout->getLayoutName(pLayName);
					if (((CString)pLayName).CompareNoCase(ActiveLayoutName) == 0)
					{
						layout_id = pLayout->id();
						pLayoutManager->setCurrentLayoutId(pLayout->id());
						pLayoutManager->setCurrentLayout(pLayName);
						pLayoutManager->updateLayoutTabs();

						pLayout->close();
						PlotBlockInit(layout_id);
						break;
					}
					pLayout->close();
				}
			}
		}
		else
		{//특정 배치만 
			AcDbObjectId layout_id = AcDbObjectId::kNull;
			for (int k = 0; k < arrLayout.GetCount(); k++)
			{

				ACHAR* pLayName = NULL;
				AcDbLayout* pLayout = NULL;
				Acad::ErrorStatus es = acdbOpenObject(pLayout, arrLayout[k], AcDb::kForWrite);
				if (pLayout)
				{
					pLayout->getLayoutName(pLayName);
					pLayout->close();
					CString strName(pLayName);
					if (strName.CompareNoCase(m_Certain_Str) == 0)
					{
						pLayoutManager->setCurrentLayout(pLayName);
						pLayoutManager->updateLayoutTabs();
						layout_id = pLayout->id();
					}
				}

			}
			PlotBlockInit(layout_id);

		}
		//도면의 레이아웃들중 처음 보이는 레이아웃을 저장해 놓았다가 파일을 닫기전에 원래 상태로 만들고 끝내자
		pLayoutManager->setCurrentLayout(OriginLayout);
		pLayoutManager->updateLayoutTabs();


		acDocManager->unlockDocument(acDocManager->curDocument());
		if (!bIsopen) {
			acDocManager->unlockDocument(activeDoc);
			acDocManager->activateDocument(activeDoc);
			acDocManager->lockDocument(activeDoc); // Lock
			activeDoc->database()->saveAs(acDocManager->curDocument()->fileName());
			acdbSetDbmod(activeDoc->database(), 0); // clear changes flag
			acDocManager->unlockDocument(activeDoc);//unlock
			acDocManager->closeDocument(activeDoc);
			activeDoc = NULL;

		}
		else	acDocManager->unlockDocument(activeDoc);

		if (m_blockRefIds.GetCount() > 0)
			m_FileList[i].result = _T("처리완료");
		else
			m_FileList[i].result = _T("Form 없음");

	}

	//acutPrintf(_T("\n 총갯수 : %d"),m_PrintRefIds.GetCount());
	//PlotBlockInit 함수에서 블럭 ID 정렬 = m_PrintRefIds
	m_txtExcelLst.erase(m_txtExcelLst.begin(), m_txtExcelLst.end());
	for (int j = 0; j < m_PrintRefIds.GetCount(); j++) {
		/*
		utils.GetBlockReferenceAttributeList(m_PrintRefIds[j].FormId, arr);
		if (arr.length() == 0)
			continue;

		arrarr.append(arr);
		*/
		m_TxtExcel.erase(m_TxtExcel.begin(), m_TxtExcel.end());
		GetBlockReferenceTxtList(m_PrintRefIds[j].FormId, m_TxtExcel);
		if (m_TxtExcel.size() == 0)
			continue;
		m_txtExcelLst.emplace_back(m_TxtExcel);
	}

	WriteToExcel();
	FileListReIndexing();

	/////////////////////가져오기 리스트 Update///////////////////////
	m_ExcelList.RemoveAll();
	m_ExcelView.DeleteAllItems();
	//엑셀 문서,시트명 뽑아오기
	//RunningExcelData();

	for (int i = 0; i < m_ExcelList.GetCount(); i++) {
		m_ExcelView.InsertItem(i, m_ExcelList[i].Doc_Name);
		m_ExcelView.SetItem(i, 1, LVIF_TEXT, m_ExcelList[i].Sheet_Name, 0, 0, 0, NULL);
	}
	/////////////////////가져오기 리스트 Update///////////////////////
	this->ShowWindow(SW_SHOW);

	bool b_formres = true;
	for (int i = 0; i < m_FileList.GetCount(); i++) {
		if (m_FileList[i].result.CompareNoCase(_T("Form 없음")) == 0)
			b_formres = false;
	}

	CString m_resMessage, m_printHead, m_printMid, m_printBot;
	m_printHead.Format(_T("\n%s 시트로 내보내기가 완료되었습니다."), printBookName);
	m_printMid.Format(_T("\n\n내보낸 시트의 1행은 수정하면 안됩니다."));
	m_printBot.Format(_T("\n\n검색할 Form 블록이 없어 처리되지 못한 도면이 있으니 확인해 주십시오."));
	if (b_formres == true)
		m_resMessage.Format(_T("%s%s"), m_printHead, m_printMid);
	else
		m_resMessage.Format(_T("%s%s%s"), m_printHead, m_printMid, m_printBot);

	AfxMessageBox(m_resMessage);

	UpdateData(FALSE);
}

bool CEMTT::WriteToExcel()
{
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	COleVariant VTrue((short)TRUE);
	COleVariant VFalse((short)FALSE);
	CApplication app;
	CRange startrng;
	LPDISPATCH lpdisp;
	CWorksheet sheet;
	CWorkbook book;
	CWorkbooks books;
	CWorksheets sheets;
	::CoInitialize(NULL);

	IDispatch* pdispExcelApp = NULL;
	IUnknown* pUnk = NULL;
	CLSID clsExcelApp;

	UpdateData(TRUE);
	bool b_Proces = FindExcel();
	// app 엑셀 오브젝트
	if (!b_Proces) {//실행중인 엑셀존재X
		if (!app.CreateDispatch(L"Excel.Application")) {
			AfxMessageBox(_T("엑셀 프로그램이 없습니다."));
			app.Quit();
		}
	}
	else {//실행중인 엑셀존재
		if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsExcelApp)))
		{
			AfxMessageBox(_T("엑셀이 설치되어 있지 않습니다."));
		}
		else {
			HRESULT hr = GetActiveObject(clsExcelApp, NULL, (IUnknown**)&pUnk);
			if (SUCCEEDED(hr)) {
				hr = pUnk->QueryInterface(IID_IDispatch, (void**)&pdispExcelApp);
			}
			else
			{
				AfxMessageBox(_T("엑셀을 찾을 수 없습니다."));
			}

			if (NULL == pdispExcelApp) {
				return false;
			}
			app.AttachDispatch(pdispExcelApp);
		}
	}



	//if(!app.CreateDispatch(L"Excel.Application")){// 엑셀 실행
	//	AfxMessageBox(L"엑셀을 열 수 없습니다.");
	//	app.Quit();
	//}
	app.put_Visible(TRUE);
	app.put_UserControl(TRUE);

	//Application

	//app.AttachDispatch(pdispExcelApp);

	//Workbooks

	lpdisp = app.get_Workbooks();
	ASSERT(lpdisp);
	books.AttachDispatch(lpdisp);

	//Books
	//lpdisp=books.Add(VOptional);
	lpdisp = books.Add(VOptional);
	ASSERT(lpdisp);
	book.AttachDispatch(lpdisp);


	//Sheets
	lpdisp = book.get_Sheets();
	ASSERT(lpdisp);
	sheets.AttachDispatch(lpdisp);
	printBookName = book.get_Name();


	//Sheet
	lpdisp = sheets.get_Item(COleVariant((short)1));
	ASSERT(lpdisp);
	sheet.AttachDispatch(lpdisp);

	//Sheet
	//lpdisp = sheets.Add(VOptional, VOptional, COleVariant((short)(1)), VOptional); // sheet 3개 추가
	//ASSERT(lpdisp);
	//lpdisp = sheets.get_Item(COleVariant((short)1));
	//ASSERT(lpdisp);
	//sheet.AttachDispatch(lpdisp);
	//sheet.put_Name(_T("내보내기"));

	startrng = sheet.get_Range(COleVariant(_T("A1")), COleVariant(_T("A1")));
	startrng.put_ColumnWidth(COleVariant((long)5));
	startrng.put_Value2(COleVariant(_T("번호")));
	//startrng.put_Item(COleVariant((long)1), COleVariant((long)1), COleVariant(_T("번호")));

	startrng = sheet.get_Range(COleVariant(_T("B1")), COleVariant(_T("B1")));
	startrng.put_ColumnWidth(COleVariant((long)m_FileList[0].full_path.GetLength() + 10));
	startrng.put_Value2(COleVariant(_T("경로")));

	startrng = sheet.get_Range(COleVariant(_T("C1")), COleVariant(_T("C1")));
	startrng.put_ColumnWidth(COleVariant((long)7));
	startrng.put_Value2(COleVariant(_T("Layout")));

	startrng = sheet.get_Range(COleVariant(_T("D1")), COleVariant(_T("D1")));
	startrng.put_ColumnWidth(COleVariant((long)8));
	startrng.put_Value2(COleVariant(_T("블록명")));

	CRange Cells = sheet.get_Cells();
	CRange Cell;


	reArr.RemoveAll();
	CStringArray ResArr;
	//속성블록 목록 문자배열에 저장
	for (int i = 0; i < arrarr.length(); i++) {
		for (int j = 0; j < arrarr[i].length(); j++) {
			ResArr.Add(arrarr[i][j].AttributeTag);
		}
	}

	//블록 타이틀 중복되는 것 제거.
	RemoveDuplicates(ResArr, reArr);

	//속성의 title을 1열에 입력	
	/*for (int i = 0; i < reArr.GetCount(); i++) {
		Cell = Cells.get_Item(COleVariant((long)1), COleVariant((long)i + 5)).pdispVal;
		Cell.put_ColumnWidth(COleVariant((long)reArr[i].GetLength() + 2));
		Cell.put_Value2(COleVariant(reArr[i]));
	}*/

	//문자 및 속성 타이틀을 1열에 입력
	for (int i = 0; i < m_TxtList.size(); i++) {
		Cell = Cells.get_Item(COleVariant((long)1), COleVariant((long)i + 5)).pdispVal;
		Cell.put_ColumnWidth(COleVariant((long)m_TxtList[i].Txt_Tag.GetLength() + 2));
		Cell.put_Value2(COleVariant(m_TxtList[i].Txt_Tag));
	}

	//속성블럭 수만큼 엑셀에 내보냄
	for (int i = 0; i < m_PrintRefIds.GetCount(); i++) {

		Cell = Cells.get_Item(COleVariant((long)i + 2), COleVariant((long)1)).pdispVal;
		Cell.put_NumberFormatLocal(COleVariant(_T("@")));
		Cell.put_Value2(COleVariant((long)i + 1));

		Cell = Cells.get_Item(COleVariant((long)i + 2), COleVariant((long)2)).pdispVal;
		Cell.put_NumberFormatLocal(COleVariant(_T("@")));
		if (COleVariant(Cell.get_ColumnWidth()) < COleVariant((long)m_PrintRefIds[i].BlockPath.GetLength() + 20)) {
			Cell.put_ColumnWidth(COleVariant((long)m_PrintRefIds[i].BlockPath.GetLength() + 2));

		}
		Cell.put_Value2(COleVariant(m_PrintRefIds[i].BlockPath));


		Cell = Cells.get_Item(COleVariant((long)i + 2), COleVariant((long)3)).pdispVal;
		Cell.put_NumberFormatLocal(COleVariant(_T("@")));
		if (COleVariant(Cell.get_ColumnWidth()) < COleVariant((long)m_PrintRefIds[i].LayoutName.GetLength() + 2)) {
			Cell.put_ColumnWidth(COleVariant((long)m_PrintRefIds[i].LayoutName.GetLength() + 2));
		}
		Cell.put_Value2(COleVariant(m_PrintRefIds[i].LayoutName));

		Cell = Cells.get_Item(COleVariant((long)i + 2), COleVariant((long)4)).pdispVal;
		Cell.put_NumberFormatLocal(COleVariant(_T("@")));
		if (COleVariant(Cell.get_ColumnWidth()) < COleVariant((long)m_PrintRefIds[i].BlockName.GetLength() + 2)) {
			Cell.put_ColumnWidth(COleVariant((long)m_PrintRefIds[i].BlockName.GetLength() + 2));
		}
		Cell.put_Value2(COleVariant(m_PrintRefIds[i].BlockName));

		/*
		//속성블록 목록
		for (int k = 0; k < reArr.GetCount(); k++) {//속성 리스트
			for (int j = 0; j < arrarr[i].length(); j++) {//속성리스트값
				if (reArr[k] == arrarr[i][j].AttributeTag) {
					CRange Cell = Cells.get_Item(COleVariant((long)i + 2), COleVariant((long)k + 5)).pdispVal;
					Cell.put_NumberFormatLocal(COleVariant(_T("@")));
					Cell.put_Value2(COleVariant(arrarr[i][j].strValue));
					//CString aa = arrarr[i][j].strValue;
					//acutPrintf(_T("\n%s : %s"),arrarr[i][j].AttributeTag, aa);
				}
			}
		}
		*/

		//문자 내용 및 속성값 목록
		//for (int k = 0; k < m_txtExcelLst.size(); k++) {
			for (int j = 0; j < m_txtExcelLst[i].size(); j++) {
				CRange Cell = Cells.get_Item(COleVariant((long)i + 2), COleVariant((long)j + 5)).pdispVal;
				Cell.put_NumberFormatLocal(COleVariant(_T("@")));
				if (COleVariant(Cell.get_ColumnWidth()) < COleVariant((long)m_txtExcelLst[i][j].Txt_Tag.GetLength() + 2))
					Cell.put_ColumnWidth(COleVariant((long)m_txtExcelLst[i][j].Txt_Tag.GetLength() + 2));
				Cell.put_Value2(COleVariant(m_txtExcelLst[i][j].Txt_Tag));
			}
		//}
	}
	//acutPrintf(_T("\n reArr : %d"),reArr.GetCount());
	return true;
}

bool CEMTT::FindExcel()
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hProcess;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	CString strFile;
	bool bExistExcel = false;
	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			strFile = entry.szExeFile;
			if (strFile.CompareNoCase(_T("excel.exe")) == 0) {
				ULONG idProc;
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ParentProcessID);

				bExistExcel = true;
				break;
			}
		}
	}
	CloseHandle(snapshot);
	return bExistExcel;
}

BOOL CEMTT::PlotBlockInit(AcDbObjectId Layout_id)
{

	BOOL bSuccess = TRUE;
	//	FindBlock(Layout_id, m_blockRefIds, newBlock.BlockName);
	m_blockRefIds.RemoveAll();
	// 검색할 Form  = m_StrList
	for (int j = 0; j < m_StrList.GetCount(); j++) {
		FindBlock(Layout_id, m_blockRefIds, m_StrList[j].FileForm);
	}
	if (m_blockRefIds.GetCount() > 0) {
		if (m_RadSort_Btn == 0)
			ZSort(m_SortError);
		else if (m_RadSort_Btn == 1)
			ZMSort(m_SortError);
		else if (m_RadSort_Btn == 2)
			NSort(m_SortError);
		else if (m_RadSort_Btn == 3)
			NMSort(m_SortError);
	}
	if (m_blockRefIds.GetCount() > 0)
		m_PrintRefIds.Append(m_blockRefIds);

	if (m_blockRefIds.GetCount() == 0)
		return false;
	//acutPrintf(_T("\n 갯수 : %d"),m_blockRefIds.GetCount());
	return true;
}

//숫자, 점, (-) 추출
BOOL CEMTT::CharacterExtraction(CString str) {
	TCHAR* getAtChar;
	CString getstr, reStr;
	StrInfoEMTT si;

	str.Replace(_T(" "), NULL);
	int roop = 0;
	for (int i = 0; i < str.GetLength(); i++) {
		getstr = str.Mid(i, 1);
		getAtChar = (TCHAR*)(LPCTSTR)getstr;
		int ascii = __toascii(*getAtChar);
		if ((ascii == 44)) {
			si.FileForm = str.Mid(roop, i - roop);
			m_StrList.Add(si);
			roop = i + 1;
		}
	}
	si.FileForm = str.Mid(roop, str.GetLength());
	m_StrList.Add(si);
	return true;
}

BOOL CEMTT::GetWindow() {
	struct resbuf fromrb, toWCS, toDCS;
	fromrb.restype = RTSHORT;
	fromrb.resval.rint = 1; // UCS
	toWCS.restype = RTSHORT;
	toWCS.resval.rint = 0; // WCS 
	toDCS.restype = RTSHORT;
	toDCS.resval.rint = 2; // DCS

	if (acedGetPoint(NULL, _T("\n첫 번째 점을 선택하세요！"), asDblArray(m_topleft)) == RTNORM) {
		if (acedGetCorner(asDblArray(m_topleft), _T("\n두번째점을 선택하세요！\n"), asDblArray(m_rightbottom)) == RTNORM) {
			AcDbExtents pt;
			AcGePoint3d minPt, maxPt, minWCSPt, maxWCSPt, minDCSPt, maxDCSPt;

			m_topleft.z = 0;
			m_rightbottom.z = 0;

			// Target/ VIEWDIR 시스템 변수 조사 후 0 이 아니면 변수 저장 후 0으로 초기화
			AcGePoint3d TartgetPt;
			AcGeVector3d vViewdir;
			bool isZero = getTarget_ViewDir(TartgetPt, vViewdir);

			acedTrans(asDblArray(m_topleft), &fromrb, &toWCS, FALSE, asDblArray(m_topleft));
			acedTrans(asDblArray(m_rightbottom), &fromrb, &toWCS, FALSE, asDblArray(m_rightbottom));

			AcGeMatrix3d mat;
			mat.setToTranslation((TartgetPt.asVector()));  //反向平移
			AcGePoint3d maxPnt, minPnt;
			maxPnt[X] = m_rightbottom[0];
			maxPnt[Y] = m_rightbottom[1];
			maxPnt[Z] = m_rightbottom[2];
			minPnt[X] = m_topleft[0];
			minPnt[Y] = m_topleft[1];
			minPnt[Z] = m_topleft[2];
			maxPnt.transformBy(mat);
			minPnt.transformBy(mat);

			// WCS to DCS
			acedTrans(asDblArray(minPnt), &toWCS, &toDCS, FALSE, asDblArray(minPnt));
			acedTrans(asDblArray(maxPnt), &toWCS, &toDCS, FALSE, asDblArray(maxPnt));

			pt.addPoint(minPnt);
			pt.addPoint(maxPnt);

			SelectArea_minpt = AcGePoint3d(pt.minPoint().x, pt.minPoint().y, pt.minPoint().z);
			SelectArea_maxpt = AcGePoint3d(pt.maxPoint().x, pt.maxPoint().y, pt.maxPoint().z);
		}
		else {
			AfxMessageBox(_T("\n두 번째 점을 선택하지 않았습니다. 블록을 다시 등록 하십시요"));
			return FALSE;
		}
	}
	else {
		AfxMessageBox(_T("\n첫 번째 점을 선택하지 않았습니다. 블록을 다시 등록 하십시요"));
		return FALSE;
	}
	return TRUE;
}

bool CEMTT::getTarget_ViewDir(AcGePoint3d& Tartget, AcGeVector3d& Viewdir)
{
	bool Zerochk = false;

	struct resbuf rb;
	acedGetVar(L"TARGET", &rb);
	Tartget.x = rb.resval.rpoint[X];
	Tartget.y = rb.resval.rpoint[Y];
	Tartget.z = rb.resval.rpoint[Z];

	struct resbuf rb1;
	acedGetVar(L"VIEWDIR", &rb1);
	Viewdir.x = rb1.resval.rpoint[X];
	Viewdir.y = rb1.resval.rpoint[Y];
	Viewdir.z = rb1.resval.rpoint[Z];

	struct resbuf rb2;
	AcGePoint3d UcsOrg;
	acedGetVar(L"UCSORG", &rb2);
	UcsOrg.x = rb2.resval.rpoint[X];
	UcsOrg.y = rb2.resval.rpoint[Y];
	UcsOrg.z = rb2.resval.rpoint[Z];

	AcGePoint3d SysTarPt;
	AcGeVector3d SysDirPt;

	// target값과 ucsorg값을 더할 때 0이면 target값 0으로 정의
	if ((Tartget.x + UcsOrg.x) < 0.000001 &&
		(Tartget.y + UcsOrg.y) < 0.000001 &&
		(Tartget.z + UcsOrg.z) < 0.000001) {
		Tartget = AcGePoint3d(0, 0, 0);
	}

	SysTarPt = Tartget;
	SysDirPt = Viewdir;

	if ((abs(SysTarPt.x) < 0.000001) && (abs(SysTarPt.y) < 0.000001) && (abs(SysTarPt.z) < 0.000001)) {
		if ((abs(SysDirPt.x) < 0.000001) && (abs(SysDirPt.y) < 0.000001) && (abs(SysDirPt.z) < 1.000001)) {
			Zerochk = true;
		}
	}
	return Zerochk;
}

void CEMTT::AddBlock(AcDbObjectId& blockid) {
	Acad::ErrorStatus es;
	ACHAR* BlockName = NULL;
	double angle = 0;
	AcGeScale3d scale = 0.0;
	AcDbObjectIdArray ids;
	AcDbBlockReference* pSelBlockRef;
	AcDbBlockTableRecordIterator* pItr = NULL;
	bool b_Att_chk = false;

	// [각도] / [축척] / [고급속성] 값 알기 위해 블럭 객체 정보 접근
	AcDbEntityPointer pEnt(blockid, AcDb::kForRead);
	if (pEnt.openStatus() == Acad::eOk) {
		pSelBlockRef = AcDbBlockReference::cast(pEnt);
		if (pSelBlockRef) {
			scale = pSelBlockRef->scaleFactors();
			angle = pSelBlockRef->rotation();
			AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
			if (pBTR.openStatus() == Acad::eOk) {
				pBTR->getName(BlockName);
				pBTR->getBlockReferenceIds(ids);
				if (pBTR->hasAttributeDefinitions())
				{
					//for (int i = 0; i < ids.length(); i++){
					es = acdbOpenObject(pSelBlockRef, ids[0], AcDb::kForRead);
					AcDbObjectIterator* Itr = pSelBlockRef->attributeIterator();
					m_key.clear();
					m_value.clear();
					for (Itr->start(); !Itr->done(); Itr->step()) {
						AcDbAttribute* pAttr;
						AcDbObjectId attributeId = Itr->objectId();
						pSelBlockRef->openAttribute(pAttr, attributeId, AcDb::kForRead);
						if (pAttr != NULL && !pAttr->isConstant())
						{
							b_Att_chk = true;
							attrEMTT row;
							row.tag = (CStringA)pAttr->tag();
							m_key.push_back(pAttr->tag());
							row.value = (CStringA)pAttr->textString();
							m_value.push_back(pAttr->textString());

							//m_attr.Add(row);
							//m_key_val.push_back(row);

							pAttr->close();
						}
					}
					//}
				}
				else {
					m_key.clear();
					m_value.clear();
				}
				pBTR->close();
			}//end-if
		}
		pEnt->close();
	}

	AcDbExtents FormArea_Tmp, PrintArea_Tmp;
	AcGeScale3d Calc_Scale = scale/* * 0.01*/;
	AcGePoint3d FormArea_MinPoint = FormArea_minpt;//FormArea.minPoint();
	AcGePoint3d FormArea_MaxPoint = FormArea_maxpt;//FormArea.maxPoint();
	AcGePoint3d PrintArea_MinPoint = SelectArea_minpt;
	AcGePoint3d PrintArea_MaxPoint = SelectArea_maxpt;
	//Z값을 일단 0으로 한다 Z축을 기준으로 돌리는데 Z축의 깊이가 다를경우 벡터의 크기가 달라진다.
	FormArea_MinPoint.z = 0;
	FormArea_MaxPoint.z = 0;
	PrintArea_MinPoint.z = 0;
	PrintArea_MaxPoint.z = 0;
	//임시의 AcDbExtents 에 넣는다. AcDbExtents 로 돌리지 않으면 이상하게 됬다.
	FormArea_Tmp.addPoint(FormArea_MinPoint);
	FormArea_Tmp.addPoint(FormArea_MaxPoint);
	PrintArea_Tmp.addPoint(PrintArea_MinPoint);
	PrintArea_Tmp.addPoint(PrintArea_MaxPoint);
	AcGeMatrix3d mat;
	mat.setToRotation(-angle, AcGeVector3d::kZAxis);
	FormArea_Tmp.transformBy(mat);
	PrintArea_Tmp.transformBy(mat);
	//벡터를 계산한다.
	AcGeVector3d minvec = (PrintArea_Tmp.minPoint() - FormArea_Tmp.minPoint());// * Calc_Scale;
	AcGeVector3d maxvec = (PrintArea_Tmp.maxPoint() - FormArea_Tmp.maxPoint());// * Calc_Scale;

	if (abs(PrintArea_Tmp.minPoint().x - FormArea_Tmp.minPoint().x) <= 0.000001 &&
		abs(PrintArea_Tmp.minPoint().y - FormArea_Tmp.minPoint().y) <= 0.000001 &&
		abs(PrintArea_Tmp.minPoint().z - FormArea_Tmp.minPoint().z) <= 0.000001) {
		minvec.x = 0;
		minvec.y = 0;
		minvec.z = 0;
	}
	if (abs(PrintArea_Tmp.maxPoint().x - FormArea_Tmp.maxPoint().x) <= 0.000001 &&
		abs(PrintArea_Tmp.maxPoint().y - FormArea_Tmp.maxPoint().y) <= 0.000001 &&
		abs(PrintArea_Tmp.maxPoint().z - FormArea_Tmp.maxPoint().z) <= 0.000001) {
		maxvec.x = 0;
		maxvec.y = 0;
		maxvec.z = 0;
	}
	//벡터를 스케일로 나눈다. 왜냐면 스케일이 1일때로 만들기 위해서 

	minvec.x = (minvec.x / Calc_Scale.sx);
	minvec.y = (minvec.y / Calc_Scale.sy);
	minvec.z = 0;
	maxvec.x = (maxvec.x / Calc_Scale.sx);
	maxvec.y = (maxvec.y / Calc_Scale.sy);
	maxvec.z = 0;
	// 	acutPrintf(_T("최종 벡터 MIN %f,%f,%f \n"),minvec.x,minvec.y,minvec.z);
	// 	acutPrintf(_T("최종 벡터 MAX %f,%f,%f \n"),maxvec.x,maxvec.y,maxvec.z);
	//계산된 벡터와 이름을 저장한다.
	BlockEMTT newBlock;
	newBlock.BlockName = BlockName;
	newBlock.Half_height = abs(FormArea_maxpt.y - FormArea_minpt.y);
	newBlock.MinVector = AcGePoint3d(minvec.x, minvec.y, minvec.z);
	newBlock.MaxVector = AcGePoint3d(maxvec.x, maxvec.y, maxvec.z);
	newBlock.b_Att = b_Att_chk;
	newBlock.v_tag.erase(newBlock.v_tag.begin(), newBlock.v_tag.end());
	newBlock.v_value.erase(newBlock.v_value.begin(), newBlock.v_value.end());
	for (int i = 0; i < m_key.size(); i++) {
		newBlock.v_tag.push_back(m_key[i]);
		newBlock.v_value.push_back(m_value[i]);
	}
	//global_BlockList.Add(newBlock);
	//UserData.XMLDataCreat();
}

void CEMTT::asdkzoomObj(AcGePoint3d min, AcGePoint3d max) {
	// 	CString minPtstr;
	// 	CString maxPtStr;
	// 	minPtstr.Format(L"%.3f,%.3f",min.x,min.y);
	// 	maxPtStr.Format(L"%.3f,%.3f",max.x,max.y);
	// 	acutPrintf(minPtstr);
	// 	acutPrintf(_T("\n"));
	// 	acutPrintf(maxPtStr);
	// 	acedCommand(RTSTR,L"_ZOOM",RTSTR,L"_W",RTSTR,minPtstr,RTSTR,maxPtStr,RTNONE);
	// get the extents of the drawing

	resbuf wcs, ucs, dcs;

	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	/*struct resbuf rb;
	AcGePoint3d Tartget;
	acedGetVar(L"TARGET", &rb);
	Tartget.x = rb.resval.rpoint[X];
	Tartget.y = rb.resval.rpoint[Y];
	Tartget.z = rb.resval.rpoint[Z];

	struct resbuf rb1;
	AcGeVector3d Viewdir;
	acedGetVar(L"VIEWDIR", &rb1);
	Viewdir.x = rb1.resval.rpoint[X];
	Viewdir.y = rb1.resval.rpoint[Y];
	Viewdir.z = rb1.resval.rpoint[Z];

	struct resbuf rb2;
	AcGePoint3d UcsOrg;
	acedGetVar(L"UCSORG", &rb2);
	UcsOrg.x = rb2.resval.rpoint[X];
	UcsOrg.y = rb2.resval.rpoint[Y];
	UcsOrg.z = rb2.resval.rpoint[Z];

	struct resbuf rb3;
	double TWangle;
	acedGetVar(L"VIEWTWIST", &rb3);
	TWangle = rb3.resval.rreal;*/

	AcDbViewTableRecord view;
	AcGePoint2d max_2d(max.x, max.y);
	AcGePoint2d min_2d(min.x, min.y);
	// now set the view centre point
	view.setCenterPoint(min_2d + (max_2d - min_2d) / 2.0);
	// now height and width of view
	view.setHeight(max_2d.y - min_2d.y);
	view.setWidth(max_2d.x - min_2d.x);
	// set the view
	acedSetCurrentView(&view, NULL);
	// updates the extents
	acdbHostApplicationServices()->workingDatabase()->updateExt(TRUE);

	/*AcGePoint3d Tartget1, UcsOrg1;
	AcGeVector3d Viewdir1;

	acedTrans(asDblArray(Tartget), &ucs, &wcs, false, asDblArray(Tartget1));
	view.setTarget(Tartget1);
	acedTrans(asDblArray(Viewdir), &ucs, &wcs, 1, asDblArray(Viewdir1));
	view.setViewDirection(Viewdir1);
	view.setViewTwist(TWangle);
	acedTrans(asDblArray(UcsOrg), &ucs, &wcs, false, asDblArray(UcsOrg1));
	view.setUcs(UcsOrg1, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis);

	acedSetCurrentView(&view, NULL);*/

	struct resbuf rb;

	acedGetVar(TEXT("VIEWDIR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 1, rb.resval.rpoint);
	view.setViewDirection({ rb.resval.rpoint[X],rb.resval.rpoint[Y],rb.resval.rpoint[Z] });

	acedGetVar(TEXT("TARGET"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);
	view.setTarget({ rb.resval.rpoint[X],rb.resval.rpoint[Y],rb.resval.rpoint[Z] });

	acedGetVar(TEXT("VIEWTWIST"), &rb);
	view.setViewTwist(rb.resval.rreal);

	acedGetVar(TEXT("UCSORG"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);
	view.setUcs({ rb.resval.rpoint[X],rb.resval.rpoint[Y],rb.resval.rpoint[Z] }, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis);

	AcGePoint3d DCSminPt;
	AcGePoint3d DCSmaxPt;
	acedTrans(asDblArray(min), &wcs, &dcs, Adesk::kFalse, asDblArray(DCSminPt));
	acedTrans(asDblArray(max), &wcs, &dcs, Adesk::kFalse, asDblArray(DCSmaxPt));
	view.setCenterPoint({ (DCSminPt.x + DCSmaxPt.x) / 2, (DCSminPt.y + DCSmaxPt.y) / 2 });
	view.setHeight(fabs(DCSmaxPt.y - DCSminPt.y));
	view.setWidth(fabs(DCSmaxPt.x - DCSminPt.x));
	acedSetCurrentView(&view, NULL);
}

void CEMTT::InitZoom(AcGePoint3d min, AcGePoint3d max)
{
	resbuf wcs, ucs, dcs;

	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	struct resbuf rb;
	AcGePoint3d Target(0, 0, 0), UcsOrg(0, 0, 0), ViewCtr(0, 0, 0);
	AcGePoint2d ScreenSize(0, 0);
	AcGeVector3d viewDir(0, 0, 1);
	double TWangle = 0.0f;

	acedGetVar(TEXT("VIEWDIR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 1, rb.resval.rpoint);
	Target.set(rb.resval.rpoint[0], rb.resval.rpoint[1], rb.resval.rpoint[2]);

	acedGetVar(TEXT("TARGET"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);

	acedGetVar(TEXT("VIEWTWIST"), &rb);
	rb.resval.rreal;

	acedGetVar(TEXT("UCSORG"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);

	acedGetVar(TEXT("SCREENSIZE"), &rb);
	//acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);
	ScreenSize.set(rb.resval.rpoint[0], rb.resval.rpoint[1]);

	acedGetVar(TEXT("VIEWCTR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);
	ViewCtr.set(rb.resval.rpoint[0], rb.resval.rpoint[1], rb.resval.rpoint[2]);
}

void CEMTT::GetBlockReferenceTxtList(AcDbObjectId blkid, std::vector<TxTEXCEL>& TxtExcel)
{
	//sAttributeTag.removeAll();
	// 좌표 변환에 필요한 요소
	struct resbuf fromrb, toWCS, toDCS;
	fromrb.restype = RTSHORT;
	fromrb.resval.rint = 1; // UCS
	toWCS.restype = RTSHORT;
	toWCS.resval.rint = 0; // WCS 
	toDCS.restype = RTSHORT;
	toDCS.resval.rint = 2; // DCS

	AcDbEntityPointer pEnt(blkid, AcDb::kForRead);
	if (pEnt.openStatus() == Acad::eOk) {
		AcDbBlockReference* pBlockRef = AcDbBlockReference::cast(pEnt);
		if (pBlockRef) {
			AcDbBlockTableRecordPointer pBTR(pBlockRef->blockTableRecord(), AcDb::kForRead);
			if (pBTR.openStatus() == Acad::eOk) {
				ACHAR* Name = NULL;
				pBTR->getName(Name);
				//AfxMessageBox(Name);

				////////////////////////////////////////
				// 벡터에 대한 영역 구하기
				AcGePoint3d Tartget;
				AcGeVector3d Viewdir;
				AcDbExtents extents, Origin, extentsWCS, extentsDCS, textExt;
				
				getTarget_ViewDir(Tartget, Viewdir);

				pBlockRef->getGeomExtents(extents);
				AcGeScale3d scale = pBlockRef->scaleFactors();
				double angle = pBlockRef->rotation();

				struct resbuf rb3;
				acedGetVar(L"VIEWTWIST", &rb3);
				double TWangle = rb3.resval.rreal;

				AcGeMatrix3d transmat;
				transmat.setToTranslation((Tartget.asVector()));
				transmat.setToRotation(-TWangle, AcGeVector3d::kZAxis);

				AcGePoint3d minPt, maxPt, minWCSPt, maxWCSPt, minDCSPt, maxDCSPt;

				minPt = extents.minPoint();
				maxPt = extents.maxPoint();

				minPt.transformBy(transmat);
				maxPt.transformBy(transmat);
				//acedTrans(asDblArray(minPt), &toWCS, &toDCS, FALSE, asDblArray(minDCSPt));
				//acedTrans(asDblArray(maxPt), &toWCS, &toDCS, FALSE, asDblArray(maxDCSPt));

				extentsDCS.addPoint(minPt);
				extentsDCS.addPoint(maxPt);

				extents = extentsDCS;

				AcGeScale3d Calc_Scale = scale;
				AcGeMatrix3d mat;
				mat.setToRotation(-angle, AcGeVector3d::kZAxis);
				extents.transformBy(mat);

				asdkzoomObj(extentsDCS.minPoint(), extentsDCS.maxPoint());
				//acedCommand(RTSTR, L"_ZOOM", RTSTR, L"_W", RTSTR, extentsDCS.minPoint(), RTSTR, extentsDCS.maxPoint(), RTNONE);

				for (int i = 0; i < m_TxtList.size(); i++) {
					if (m_TxtList[i].BlockName.CompareNoCase(Name) == 0) {
						AcDbExtents Tmpextents;
						TxTEMTT tmp_TXT;
						tmp_TXT = m_TxtList[i];
						double minx = extents.minPoint().x + (tmp_TXT.MinVector.x * Calc_Scale.sx);
						double miny = extents.minPoint().y + (tmp_TXT.MinVector.y * Calc_Scale.sy);
						double maxx = extents.maxPoint().x + (tmp_TXT.MaxVector.x * Calc_Scale.sx);
						double maxy = extents.maxPoint().y + (tmp_TXT.MaxVector.y * Calc_Scale.sy);

						Tmpextents.addPoint(AcGePoint3d(minx, miny, 0));
						Tmpextents.addPoint(AcGePoint3d(maxx, maxy, 0));
						mat.setToRotation(angle, AcGeVector3d::kZAxis);
						Tmpextents.transformBy(mat);

						// SelectArea
						AcGePoint2d MinSelPt(0, 0), MaxSelPt(0, 0);
						ads_point dMinPt, dMaxPt;

						if (TWangle > 0.000001) {
							acedTrans(asDblArray(Tmpextents.minPoint()), &toWCS, &toDCS, FALSE, dMinPt);
							acedTrans(asDblArray(Tmpextents.maxPoint()), &toWCS, &toDCS, FALSE, dMaxPt);

							MinSelPt.x = dMinPt[0];
							MinSelPt.y = dMinPt[1];
							MaxSelPt.x = dMaxPt[0];
							MaxSelPt.y = dMaxPt[1];
						}
						else {
							MinSelPt.x = Tmpextents.minPoint().x;
							MinSelPt.y = Tmpextents.minPoint().y;
							MaxSelPt.x = Tmpextents.maxPoint().x;
							MaxSelPt.y = Tmpextents.maxPoint().y;
						}

						// [+] 선택하기 MinSelPt MaxSelPt
						ads_name ss;
						//AcDbObjectId txtId = AcDbObjectId::kNull;
						AcDbEntity* pTextEntity;
						Adesk::Int32 len;
						AcDbObjectIdArray text_ids;

						//test
						//acedGrDraw(asDblArray(MinSelPt), asDblArray(MaxSelPt), 1, 0);
						//acutPrintf(L"\nX=%f Y=%f\n", MinSelPt.x, MinSelPt.y);
						//acutPrintf(L"\nX=%f Y=%f\n", MaxSelPt.x, MaxSelPt.y);

						int rc = acedSSGet(_T("C"), asDblArray(MinSelPt), asDblArray(MaxSelPt), NULL, ss);
						//acdbGetObjectId(txtId, ss);
						acedSSLength(ss, &len);

						for (int i = 0; i < len; i++) {
							ads_name ent;
							AcDbObjectId objId;
							acedSSName(ss, i, ent);
							acdbGetObjectId(objId, ent);
							text_ids.append(objId);
						}

						for (int i = 0; i < text_ids.length(); i++) {
							AcDbEntityPointer pTxtEnt(text_ids[i], AcDb::kForRead);
							if (pTxtEnt.openStatus() == Acad::eOk) {
								if (pTxtEnt->isKindOf(AcDbText::desc())) {
									AcDbText* pTxt = AcDbText::cast(pTxtEnt);
									CString SelTxt = pTxt->textString();
									TxTEXCEL tmpExcel;
									tmpExcel.Txt_Tag = SelTxt;
									TxtExcel.emplace_back(tmpExcel);
									acutPrintf(L"\ntext=%s\n", SelTxt);
								}
								else if (pTxtEnt->isKindOf(AcDbMText::desc())) {
									AcDbMText* pMTxt = AcDbMText::cast(pTxtEnt);
									CString SelTxt = pMTxt->contents();
									TxTEXCEL tmpExcel;
									tmpExcel.Txt_Tag = SelTxt;
									TxtExcel.emplace_back(tmpExcel);
								}
							}
						}
						// [-] 선택하기 MinSelPt MaxSelPt
					}
				}
				////////////////////////////////////////
				/*
				AcDbObjectId idBlkDef;
				AcDbBlockTableRecord* pBTR;
				AcDbAttribute* pAttr;
				AcDbObjectIterator* pAttrIterator;
				idBlkDef = pBlockRef->blockTableRecord();
				acdbOpenObject((AcDbObject*&)pBTR, idBlkDef, AcDb::kForRead);
				if (pBTR->hasAttributeDefinitions()) {
					pAttrIterator = pBlockRef->attributeIterator();
					for (; !pAttrIterator->done(); pAttrIterator->step()) {
						AcDbObjectId attrObjId;
						attrObjId = pAttrIterator->objectId();
						pBlockRef->openAttribute((AcDbAttribute*&)pAttr, attrObjId, AcDb::kForRead);

						AttBlock att;
						att.AttributeTag = pAttr->tag();
						att.strValue = pAttr->textString();

						//sAttributeTag.append(att);
						pAttr->close();
					}
					delete pAttrIterator;
				}
				pBTR->close();
				pBlockRef->close();
				*/
				////////////////////////////////////////
			}
		}
		pEnt->close();
	}

}