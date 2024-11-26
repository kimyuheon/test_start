// CALS.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CALS.h"
#include "afxdialogex.h"
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


#define PUMP_MSG(hWnd)do {static MSG msg; if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)==TRUE) TranslateMessage(&msg);DispatchMessage(&msg);}} while(0)   

template<class T>
void QSortCArray(T& t, int(__cdecl *compare)(const void *elem1, const void *elem2)){
	if(t.GetSize() <= 0)	return;
	qsort(t.GetData(), t.GetSize(), sizeof(t[0]), compare);
}

//내림차순 정렬
int CompareStringDescending(const void *elem1, const void *elem2){
	FileInfos* p1 = (FileInfos*)elem1;
	FileInfos* p2 = (FileInfos*)elem2;
	if(p1->filename > p2->filename)	return +1;
	if(p1->filename < p2->filename)	return -1;

	return 0;
}

// 오름차순 정렬
int CompareStringAscending(const void *elem1, const void *elem2){
	FileInfos* p1 = (FileInfos*)elem1;
	FileInfos* p2 = (FileInfos*)elem2;
	if(p1->filename > p2->filename) return -1;
	if(p1->filename < p2->filename) return +1;

	return 0;
}
 
 //X좌표를 기준으로 정렬오름차순
 int ComparePointX(const void *elem1, const void *elem2){
 	CPrintFormData *ent1 = (CPrintFormData*)elem1;
 	CPrintFormData *ent2 = (CPrintFormData*)elem2;
 	if(ent1->minpoint_X > ent2->minpoint_X)	return +1;
 	if(ent1->minpoint_X < ent2->minpoint_X)	return -1;
 
 	return 0;
 }
 
 //Y좌표를 기준으로 정렬오름차순
 int ComparePointY(const void *elem1, const void *elem2){
 	CPrintFormData *ent1 = (CPrintFormData*)elem1;
 	CPrintFormData *ent2 = (CPrintFormData*)elem2;
 	if(ent1->minpoint_Y > ent2->minpoint_Y)	return +1;
 	if(ent1->minpoint_Y < ent2->minpoint_Y)	return -1;
 
 	return 0;
 }
 
 //X좌표를 기준으로 정렬내림차순 
 int ComparePointX_De(const void *elem1, const void *elem2){
 	CPrintFormData *ent1 = (CPrintFormData*)elem1;
 	CPrintFormData *ent2 = (CPrintFormData*)elem2;
 	if(ent1->minpoint_X > ent2->minpoint_X)	return -1;
 	if(ent1->minpoint_X < ent2->minpoint_X)	return +1;
 
 	return 0;
 }

//Y좌표를 기준으로 정렬내림차순 
 int ComparePointY_De(const void *elem1, const void *elem2){
 	CPrintFormData *ent1 = (CPrintFormData*)elem1;
 	CPrintFormData *ent2 = (CPrintFormData*)elem2;
 	if(ent1->minpoint_Y > ent2->minpoint_Y)	return -1;
 	if(ent1->minpoint_Y < ent2->minpoint_Y)	return +1;
 
 	return 0;
 }

// CCALS 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCALS, CAcUiDialog)

CCALS::CCALS(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CCALS::IDD, pParent)
	, m_RadProperty_Btn(0)
	, m_RadSearch_Btn(0)
	, m_RadSort_Btn(0)
	, m_LastIdx(0)
	,m_ListIndex(0)
	, m_ListSort_Btn(0)
{

}


CCALS::~CCALS()
{
}

void CCALS::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);	
	DDX_Radio(pDX, IDC_CALS_RADEXCEL, m_RadProperty_Btn);
	DDX_Radio(pDX, IDC_CALS_RADMODEL, m_RadSearch_Btn);
	DDX_Radio(pDX, IDC_CALS_RADZSORT, m_RadSort_Btn);
	DDX_Control(pDX, IDC_CALS_LISTFILES, m_FileView);
	DDX_Radio(pDX, IDC_CALS_SORT1, m_ListSort_Btn);
	DDX_Control(pDX, IDC_CALS_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_CALS_BUTTON2, m_btn2);
	DDX_Control(pDX, IDC_CALS_BUTTON3, m_btn3);
	DDX_Control(pDX, IDC_CALS_BUTTON4, m_btn4);
	DDX_Control(pDX, IDC_CALS_BTNEXTRACT, m_bSelect);
	DDX_Text(pDX, IDC_CALS_EDTCERTAIN, m_Certain_Str);
	DDX_Text(pDX, IDC_CALS_EDTSEARCH, m_SearchForm_Str);
	DDX_Text(pDX, IDC_CALS_EDTERROR, m_SortError_Str);
	DDX_Control(pDX, IDC_CALS_LISTFILES2, m_ExcelView);
}


BEGIN_MESSAGE_MAP(CCALS, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDCANCEL, &CCALS::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CALS_BTN_EXPORT, &CCALS::OnBnClickedCalsBtnExport)
	ON_BN_CLICKED(IDC_CALS_RADEXCEL, &CCALS::OnBnClickedCalsRadexcel)
	ON_BN_CLICKED(IDC_CALS_RADCAD, &CCALS::OnBnClickedCalsRadcad)
	ON_BN_CLICKED(IDC_CALS_RADMODEL, &CCALS::OnBnClickedCalsRadmodel)
	ON_BN_CLICKED(IDC_CALS_RADLAYOUT, &CCALS::OnBnClickedCalsRadmodel)
	ON_BN_CLICKED(IDC_CALS_RADMODELLAY, &CCALS::OnBnClickedCalsRadmodel)
	ON_BN_CLICKED(IDC_CALS_RADCERTAIN, &CCALS::OnBnClickedCalsRadmodel)
	ON_BN_CLICKED(IDC_CALS_RADACTIVE, &CCALS::OnBnClickedCalsRadmodel)
	ON_BN_CLICKED(IDC_CALS_BTNEXTRACT, &CCALS::OnBnClickedCalsBtnextract)
	ON_BN_CLICKED(IDC_CALS_RADZSORT, &CCALS::OnBnClickedCalsRadzsort)
	ON_BN_CLICKED(IDC_CALS_BUTTON1, &CCALS::OnBnClickedCalsButton1)
	ON_BN_CLICKED(IDC_CALS_RADNSORT, &CCALS::OnBnClickedCalsRadnsort)
	ON_BN_CLICKED(IDC_CALS_BUTTON2, &CCALS::OnBnClickedCalsButton2)
	ON_BN_CLICKED(IDC_CALS_RADNMSORT, &CCALS::OnBnClickedCalsRadnmsort)
	ON_BN_CLICKED(IDC_CALS_BUTTON3, &CCALS::OnBnClickedCalsButton3)
	ON_BN_CLICKED(IDC_CALS_RADZMSORT, &CCALS::OnBnClickedCalsRadzmsort)
	ON_BN_CLICKED(IDC_CALS_BUTTON4, &CCALS::OnBnClickedCalsButton4)
	ON_EN_UPDATE(IDC_CALS_EDTERROR, &CCALS::OnEnUpdateCalsEdterror)	
	ON_BN_CLICKED(IDC_CALS_BTN_ADDDWG, &CCALS::OnBnClickedCalsBtnAdddwg)
	ON_BN_CLICKED(IDC_CALS_BTN_ADDFOLDER, &CCALS::OnBnClickedCalsBtnAddfolder)
	ON_BN_CLICKED(IDC_CALS_BTN_ACTIVEDWG, &CCALS::OnBnClickedCalsBtnActivedwg)
	ON_BN_CLICKED(IDC_CALS_BTN_OPENDWG, &CCALS::OnBnClickedCalsBtnOpendwg)
	ON_BN_CLICKED(IDC_CALS_SEL_OPEN, &CCALS::OnBnClickedCalsSelOpen)
	ON_BN_CLICKED(IDC_CALS_SEL_DEL, &CCALS::OnBnClickedCalsSelDel)
	ON_BN_CLICKED(IDC_CALS_ALL_DEL, &CCALS::OnBnClickedCalsAllDel)
	ON_BN_CLICKED(IDC_CALS_BTN_UP, &CCALS::OnBnClickedCalsBtnUp)
	ON_BN_CLICKED(IDC_CALS_BTN_DOWN, &CCALS::OnBnClickedCalsBtnDown)
	ON_BN_CLICKED(IDC_CALS_BTN_BOTTOM, &CCALS::OnBnClickedCalsBtnBottom)
	ON_BN_CLICKED(IDC_CALS_BTN_TOP, &CCALS::OnBnClickedCalsBtnTop)
	ON_BN_CLICKED(IDC_CALS_SORT1, &CCALS::OnBnClickedCalsSort1)
	ON_BN_CLICKED(IDC_CALS_SORT2, &CCALS::OnBnClickedCalsSort2)			
	ON_EN_UPDATE(IDC_CALS_EDTCERTAIN, &CCALS::OnEnUpdateCalsEdtcertain)
	ON_EN_UPDATE(IDC_CALS_EDTSEARCH, &CCALS::OnEnUpdateCalsEdtsearch)
	ON_WM_CTLCOLOR()
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_CALS_LISTFILES2, &CCALS::OnLvnItemchangedCalsListfiles2)	
	ON_BN_CLICKED(IDC_CALS_BTN_IMPORT, &CCALS::OnBnClickedCalsBtnImport)
	ON_BN_CLICKED(ID_CALS_HELP, &CCALS::OnBnClickedCalsHelp)
END_MESSAGE_MAP()


// CCALS 메시지 처리기입니다.

LRESULT CCALS::onAcadKeepFocus(WPARAM, LPARAM) {
	return TRUE;
}

BOOL CCALS::OnInitDialog()
{
	
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	m_bSelect.AutoLoad();

	//정렬 오차 BASE
	SetDlgItemText(IDC_CALS_EDTERROR, _T("10.0"));

	// 해상도 값을 부여하기 위해 dc 선언
	/*CDC dc;
	HDC hdc = ::GetDC(this->m_hWnd);
	dc.Attach(hdc);*/
	
	this->GetWindowRect(&initrect);

	GetDlgItem(IDC_EXCEL_BOX)->GetWindowRect(&Boxrect);
	m_Fold_Width = Boxrect.left - initrect.left;
	this->SetWindowPos(NULL, 0, 0, m_Fold_Width, initrect.bottom - initrect.top, SWP_NOMOVE);
	//Edt 컨트롤 처음에 보이지 않게 함
	GetDlgItem(IDC_CALS_EDTCERTAIN)->ShowWindow(SW_HIDE);
	//정렬순서 비트맵
	m_btn1.LoadBitmaps(IDB_BITMAP_ORDER1,IDB_BITMAP_ORDER1,NULL,NULL);
	m_btn1.SizeToContent();
	m_btn2.LoadBitmaps(IDB_BITMAP_ORDER2,IDB_BITMAP_ORDER2,NULL,NULL);
	m_btn2.SizeToContent();
	m_btn3.LoadBitmaps(IDB_BITMAP_ORDER3,IDB_BITMAP_ORDER3,NULL,NULL);
	m_btn3.SizeToContent();
	m_btn4.LoadBitmaps(IDB_BITMAP_ORDER4,IDB_BITMAP_ORDER4,NULL,NULL);
	m_btn4.SizeToContent();
	//리스트 컨트롤 초기화
	CRect rt;
	m_FileView.GetWindowRect(&rt);
	m_FileView.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);// 리스트 컨트롤에 선표시 및 ITEM 선택시 한행 선택
	m_FileView.DeleteAllItems();

	auto IDStoCString = [](int a_ID) -> CString {
		CString IDS;
		IDS.LoadStringW(a_ID);
		return IDS;
		};

	/*bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);*/

	m_FileView.InsertColumn(0, IDStoCString(IDS_FILENAME), LVCFMT_LEFT, rt.Width() * 0.7, -1);		//"파일명"
	m_FileView.InsertColumn(1, IDStoCString(IDS_PROC_RESULT), LVCFMT_LEFT, rt.Width()*0.3,-1);		//처리결과
	FileViewRefresh();

	m_ExcelView.GetWindowRect(&rt);
	m_ExcelView.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);// 리스트 컨트롤에 선표시 및 ITEM 선택시 한행 선택

	m_ExcelView.InsertColumn(0, IDStoCString(IDS_DOC_NAME), LVCFMT_LEFT, rt.Width()*0.5);		//"문서명"
	m_ExcelView.InsertColumn(1, IDStoCString(IDS_SHEET), LVCFMT_LEFT, rt.Width()*0.5);		//"시트명"
	
	//::ReleaseDC(this->GetSafeHwnd(), dc);

	GetDlgItem(IDC_CALS_RADCERTAIN)->GetWindowRect(rc);
	ScreenToClient(&rc);

	return TRUE;  
}

void CCALS::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DestroyWindow();
}

//엑셀 프로세스 실행 유무
bool CCALS::FindExcel(){
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hProcess;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	CString strFile;
	bool bExistExcel = false;
	if(Process32First(snapshot, &entry) == TRUE){
		while(Process32Next(snapshot, &entry) == TRUE){
			strFile=entry.szExeFile;
			if(strFile.CompareNoCase(_T("excel.exe")) == 0){
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
//엑셀 생성 및 쓰기.
bool CCALS::WriteToExcel(){

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

	IDispatch* pdispExcelApp =NULL;
	IUnknown *pUnk=NULL;
	CLSID clsExcelApp;
	CString strMsg;

	UpdateData(TRUE);

	/*bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);*/

	bool b_Proces = FindExcel();
	// app 엑셀 오브젝트
	if(!b_Proces){//실행중인 엑셀존재X
		if(!app.CreateDispatch(L"Excel.Application")){
			strMsg.LoadStringW(IDS_CHECK_EXCEL); // "엑셀 프로그램이 없습니다."
			AfxMessageBox(strMsg);
			app.Quit();
		}
	}else{//실행중인 엑셀존재
		if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsExcelApp))) 
		{
			strMsg.LoadStringW(IDS_SETUP_EXCEL); // "엑셀이 설치되어 있지 않습니다."
			AfxMessageBox(strMsg);
		}else{
			HRESULT hr = GetActiveObject(clsExcelApp, NULL, (IUnknown**)&pUnk);
			if(SUCCEEDED(hr)){
				hr = pUnk->QueryInterface(IID_IDispatch, (void **)&pdispExcelApp);
			}
			else
			{
				strMsg.LoadStringW(IDS_FIND_EXCEL); // "엑셀을 찾을 수 없습니다."
				AfxMessageBox(strMsg);
			}

			if(NULL == pdispExcelApp){		
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

	lpdisp=app.get_Workbooks();
	ASSERT(lpdisp);
	books.AttachDispatch(lpdisp);

	//Books
	//lpdisp=books.Add(VOptional);
	lpdisp=books.Add(VOptional);
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
	strMsg.LoadStringW(IDS_NUMBER);	// 번호
	startrng.put_Value2(COleVariant(strMsg));
	//startrng.put_Item(COleVariant((long)1), COleVariant((long)1), COleVariant(_T("번호")));

	startrng = sheet.get_Range(COleVariant(_T("B1")), COleVariant(_T("B1")));
	startrng.put_ColumnWidth(COleVariant((long)m_FileList[0].full_path.GetLength()+10));
	strMsg.LoadStringW(IDS_PATH);	// 경로
	startrng.put_Value2(COleVariant(strMsg));

	startrng = sheet.get_Range(COleVariant(_T("C1")), COleVariant(_T("C1")));
	startrng.put_ColumnWidth(COleVariant((long)7));
	startrng.put_Value2(COleVariant(_T("Layout")));

	startrng = sheet.get_Range(COleVariant(_T("D1")), COleVariant(_T("D1")));
	startrng.put_ColumnWidth(COleVariant((long)8));
	strMsg.LoadStringW(IDS_BLOCKNAME);	// 블록명
	startrng.put_Value2(COleVariant(strMsg));

	CRange Cells = sheet.get_Cells();
	CRange Cell;
	
	
	reArr.RemoveAll();	
	CStringArray ResArr;
	//속성블록 목록 문자배열에 저장
	for(int i=0; i<arrarr.length(); i++){
		for(int j=0; j<arrarr[i].length(); j++){

			ResArr.Add(arrarr[i][j].AttributeTag);
		}

	}
	
	//블록 타이틀 중복되는 것 제거.
	RemoveDuplicates(ResArr,reArr);

	//속성의 title을 1열에 입력	
	for(int i=0; i<reArr.GetCount(); i++){
		Cell = Cells.get_Item(COleVariant((long)1), COleVariant((long)i+5)).pdispVal;		
		Cell.put_ColumnWidth(COleVariant((long)reArr[i].GetLength()+2));
		Cell.put_Value2(COleVariant(reArr[i])); 
	}
	
	//속성블럭 수만큼 엑셀에 내보냄
	for(int i = 0; i<m_PrintRefIds.GetCount(); i++){
	
		Cell = Cells.get_Item(COleVariant((long)i+2), COleVariant((long)1)).pdispVal;
		Cell.put_NumberFormatLocal(COleVariant(_T("@")));
		Cell.put_Value2(COleVariant((long)i+1));
	
		Cell = Cells.get_Item(COleVariant((long)i+2), COleVariant((long)2)).pdispVal;
		Cell.put_NumberFormatLocal(COleVariant(_T("@")));
		if(COleVariant(Cell.get_ColumnWidth()) < COleVariant((long)m_PrintRefIds[i].BlockPath.GetLength()+20)){
			Cell.put_ColumnWidth(COleVariant((long)m_PrintRefIds[i].BlockPath.GetLength()+2));
			
		}
		Cell.put_Value2(COleVariant(m_PrintRefIds[i].BlockPath));
		
	
		Cell = Cells.get_Item(COleVariant((long)i+2), COleVariant((long)3)).pdispVal;
		Cell.put_NumberFormatLocal(COleVariant(_T("@")));
		if(COleVariant(Cell.get_ColumnWidth()) < COleVariant((long)m_PrintRefIds[i].LayoutName.GetLength()+2)){
			Cell.put_ColumnWidth(COleVariant((long)m_PrintRefIds[i].LayoutName.GetLength()+2));
		}
		Cell.put_Value2(COleVariant(m_PrintRefIds[i].LayoutName));
	
		Cell = Cells.get_Item(COleVariant((long)i+2), COleVariant((long)4)).pdispVal;
		Cell.put_NumberFormatLocal(COleVariant(_T("@")));
		if(COleVariant(Cell.get_ColumnWidth()) < COleVariant((long)m_PrintRefIds[i].BlockName.GetLength()+2)){
			Cell.put_ColumnWidth(COleVariant((long)m_PrintRefIds[i].BlockName.GetLength()+2));
		}
		Cell.put_Value2(COleVariant(m_PrintRefIds[i].BlockName));
	
	
		//속성블록 목록
		for(int k=0; k<reArr.GetCount(); k++){//속성 리스트
			for(int j=0; j<arrarr[i].length(); j++){//속성리스트값
				if(reArr[k] == arrarr[i][j].AttributeTag){					
					CRange Cell = Cells.get_Item(COleVariant((long)i+2), COleVariant((long)k+5)).pdispVal;
					Cell.put_NumberFormatLocal(COleVariant(_T("@")));
					Cell.put_Value2(COleVariant(arrarr[i][j].strValue)); 					
					//CString aa = arrarr[i][j].strValue;
					//acutPrintf(_T("\n%s : %s"),arrarr[i][j].AttributeTag, aa);
					
				}

			}
			
		}
	
	
	}
	//acutPrintf(_T("\n reArr : %d"),reArr.GetCount());
	return true;
}

void CCALS::RemoveDuplicates(CStringArray& lst, CStringArray &result)
{
	result.RemoveAll();

	for(int i = 0 ; i < lst.GetCount(); i++)
	{
		bool bisfind =false;
		for(int j = 0 ; j < result.GetCount(); j++)
		{
			if(result[j].CompareNoCase(lst[i])==0)
			{
				bisfind = true;
				break;
			}
		}
		if(!bisfind)
			result.Add(lst[i]);
	}
}

//경로로 도큐먼트 가져오기
AcApDocument* CCALS::GetOepndDocument(CString strPath){
	AcApDocument *doc = NULL;
	//여기서 부터는 열린 도면들
	AcApDocumentIterator *pIter = acDocManager->newAcApDocumentIterator();
	for(; !pIter->done(); pIter->step()){
		const ACHAR *name = NULL;
		name = pIter->document()->fileName();
		if(strPath.CompareNoCase(name) == 0){
			doc = pIter->document();
			break;
		}
	}
	delete pIter;
	return doc;
}

//내보내기
void CCALS::OnBnClickedCalsBtnExport()
{
	UpdateData(TRUE);
	this->ShowWindow(SW_HIDE);
	CString strMsg;
	m_blockRefIds.RemoveAll();
	m_StrList.RemoveAll();
	arrarr.removeAll();
	arr.removeAll();
	m_PrintRefIds.RemoveAll();
	//검색할 Form 이름 
	CharacterExtraction(m_SearchForm_Str);
	//AfxMessageBox(_T("1"));
	for(int i=0; i<m_FileList.GetCount(); i++)
	{
		//일단 해당 파일이 열려있는지 확인하자.
		//열려있을경우 새로 파일을 열지 말자
		m_blkFilePath = m_FileList[i].full_path;
		//AfxMessageBox(m_FileList[i].full_path);
		AcApDocument* doc;
		BOOL bIsopen = IsOpenDocument(m_blkFilePath);
		if(bIsopen){
			doc = GetOepndDocument(m_blkFilePath);
			if(doc!=NULL)	acDocManager->setCurDocument(doc);
		}
		else
		{
			Acad::ErrorStatus es =acDocManager->appContextOpenDocument(m_blkFilePath);
		}
		
		AcApDocument * activeDoc =acDocManager->curDocument();
		AcApLayoutManager *pLayoutManager = (AcApLayoutManager *)acdbHostApplicationServices()->layoutManager(); 
		const ACHAR* ActiveLayoutName = pLayoutManager->findActiveLayout(Adesk::kTrue);
		AcDbDatabase * pDb = activeDoc->database();

		acDocManager->lockDocument(acDocManager->curDocument());

		CArray <AcDbObjectId,AcDbObjectId> arrLayout;
		GetLayout(pDb, arrLayout);

		AcDbObject *pObj;


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
	#ifndef _2025
			OriginLayout = pLayoutManager->findActiveTab();
	#else // z2025부터 함수 변경
			AcString ActiveName;
			OriginLayout = (ACHAR*)pLayoutManager->getActiveTab(ActiveName);
	#endif
#elif _ACAD
		AcString ActiveName;
		OriginLayout = (ACHAR*)pLayoutManager->getActiveTab(ActiveName);
#endif

		const ACHAR *csModel=acdbGetReservedString( AcDb::kModel );
		CString strModel(csModel);


		//모형
	 if(m_RadSearch_Btn==0)
		{

			AcDbObjectId layout_id = AcDbObjectId::kNull;
			for(int k = 0; k < arrLayout.GetCount() ;k++)
			{
				ACHAR *pLayName = NULL;
				AcDbLayout* pLayout = NULL;
				Acad::ErrorStatus es =	acdbOpenObject(pLayout,arrLayout[k],AcDb::kForWrite); 
				if(pLayout)
				{
					pLayout->getLayoutName(pLayName);
					if( ((CString)pLayName).CompareNoCase(_T("Model")) == 0  || ((CString)pLayName).CompareNoCase(_T("모형")) == 0 )
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
		else if(m_RadSearch_Btn ==1)//배치
		{

			AcDbObjectId layout_id = AcDbObjectId::kNull;
			for(int k = 0; k < arrLayout.GetCount() ;k++)
			{
				ACHAR *pLayName = NULL;
				AcDbLayout* pLayout = NULL;
				Acad::ErrorStatus es =	acdbOpenObject(pLayout,arrLayout[k],AcDb::kForWrite); 
				if(pLayout)
				{
					pLayout->getLayoutName(pLayName);
					if( ((CString)pLayName).CompareNoCase(_T("Model")) != 0  && ((CString)pLayName).CompareNoCase(_T("모형")) != 0 )
					{
						layout_id = pLayout->id();
						//pLayoutManager->setCurrentLayoutId(pLayout->id());
						pLayoutManager->setCurrentLayout(pLayName); 
						pLayoutManager->updateLayoutTabs();
						
						pLayout->close();
						PlotBlockInit(layout_id);
					}else
						pLayout->close();

				}
				
			}
			//ExportExcel(layout_id);
		}
		else if(m_RadSearch_Btn ==2){	//모형 + 배치
			AcDbObjectId layout_id = AcDbObjectId::kNull;
			for(int k = 0; k < arrLayout.GetCount() ;k++)
			{
				ACHAR *pLayName = NULL;
				AcDbLayout* pLayout = NULL;
				Acad::ErrorStatus es =	acdbOpenObject(pLayout,arrLayout[k],AcDb::kForWrite); 
				if(pLayout)
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
			for(int k = 0; k < arrLayout.GetCount() ;k++)
			{
				
				ACHAR *pLayName = NULL;
				AcDbLayout* pLayout = NULL;
				Acad::ErrorStatus es =	acdbOpenObject(pLayout,arrLayout[k],AcDb::kForWrite); 
				if(pLayout)
				{
					pLayout->getLayoutName(pLayName);
					pLayout->close();
					CString strName(pLayName);
					if(strName.CompareNoCase(m_Certain_Str)==0)
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
		if(!bIsopen){
			acDocManager->unlockDocument(activeDoc);
			acDocManager->activateDocument(activeDoc);
			acDocManager->lockDocument(activeDoc); // Lock
			activeDoc->database()->saveAs(acDocManager->curDocument()->fileName());
			acdbSetDbmod(activeDoc->database(), 0); // clear changes flag
			acDocManager->unlockDocument(activeDoc);//unlock
			acDocManager->closeDocument(activeDoc);
			activeDoc=NULL;

		}
		else	acDocManager->unlockDocument(activeDoc);

		bool dllchk = false;
		dllchk = loadResourcedllChk();
		if (!dllchk)
			g_hCurrResource = NULL;
		CAcModuleResourceOverride resOverride(g_hCurrResource);

		if (m_blockRefIds.GetCount() > 0) {
			strMsg.LoadStringW(IDS_COMPLETE); // 처리완료
			m_FileList[i].result = strMsg;
		} else {
			strMsg.LoadStringW(IDS_NOFORM); // Form 없음
			m_FileList[i].result = strMsg;
		}

	}
	
	//acutPrintf(_T("\n 총갯수 : %d"),m_PrintRefIds.GetCount());
	for(int j=0; j<m_PrintRefIds.GetCount(); j++){
		utils.GetBlockReferenceAttributeList(m_PrintRefIds[j].FormId, arr);
		if(arr.length() == 0)
			continue;

		arrarr.append(arr);
	}

	WriteToExcel();
	FileListReIndexing();

	/////////////////////가져오기 리스트 Update///////////////////////
	m_ExcelList.RemoveAll();
	m_ExcelView.DeleteAllItems();
	//엑셀 문서,시트명 뽑아오기
	RunningExcelData();

	for(int i=0; i<m_ExcelList.GetCount(); i++){
		m_ExcelView.InsertItem(i, m_ExcelList[i].Doc_Name);
		m_ExcelView.SetItem(i,1, LVIF_TEXT, m_ExcelList[i].Sheet_Name, 0, 0, 0, NULL);
	}
	/////////////////////가져오기 리스트 Update///////////////////////
	this->ShowWindow(SW_SHOW);

	bool b_formres = true;
	for(int i=0; i<m_FileList.GetCount(); i++){
		strMsg.LoadStringW(IDS_NOFORM); // Form 없음
		if(m_FileList[i].result.CompareNoCase(strMsg)==0)
			b_formres = false;
	}

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	CString m_resMessage, m_printHead, m_printMid, m_printBot;
	strMsg.LoadStringW(IDS_COMPLETE_EXPORT); // 시트로 내보내기가 완료되었습니다.
	m_printHead.Format(_T("\n%s %s"),printBookName, strMsg);
	strMsg.LoadStringW(IDS_MODIFY_CHECK); // \n\n내보낸 시트의 1행은 수정하면 안됩니다.
	m_printMid.Format(_T("%s"), strMsg);
	strMsg.LoadStringW(IDS_CHECK_BLOCK); // \n\n검색할 Form 블록이 없어 처리되지 못한 도면이 있으니 확인해 주십시오.
	m_printBot.Format(_T("%s"), strMsg);
	if(b_formres == true)
		m_resMessage.Format(_T("%s%s"),m_printHead,m_printMid);
	else
		m_resMessage.Format(_T("%s%s%s"),m_printHead,m_printMid,m_printBot);
	
	AfxMessageBox(m_resMessage);

	UpdateData(FALSE);
}

BOOL CCALS::PlotBlockInit(AcDbObjectId Layout_id)
{

	BOOL bSuccess = TRUE;
	//	FindBlock(Layout_id, m_blockRefIds, newBlock.BlockName);
	m_blockRefIds.RemoveAll();
	for(int j=0; j<m_StrList.GetCount(); j++){		
		FindBlock(Layout_id, m_blockRefIds, m_StrList[j].FileForm);
	}
	if(m_blockRefIds.GetCount() > 0){
		if(m_RadSort_Btn == 0)
			ZSort(m_SortError);
		else if(m_RadSort_Btn == 1)
			ZMSort(m_SortError);
		else if(m_RadSort_Btn == 2)
			NSort(m_SortError);
		else if(m_RadSort_Btn == 3)
			NMSort(m_SortError);
	}
	if(m_blockRefIds.GetCount() > 0)
		m_PrintRefIds.Append(m_blockRefIds);

	if(m_blockRefIds.GetCount() == 0)
		return false;
	//acutPrintf(_T("\n 갯수 : %d"),m_blockRefIds.GetCount());
	return true;
}

void CCALS::FindBlock(AcDbObjectId Layout_id,CPrintFormDataArray& m_blockRefIds,CString strName)
{

	AcDbObjectId blktbl_id = AcDbObjectId::kNull;
	ACHAR *pLayName = NULL;
	AcDbLayout* pLayout = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pLayout,Layout_id,AcDb::kForRead); 
	if(pLayout)
	{
		blktbl_id = pLayout->getBlockTableRecordId();
		pLayout->getLayoutName(pLayName);
		pLayout->close();
	}

	//m_blockRefIds.RemoveAll();
	AcDbObjectIdArray ids;

	AcDbBlockTableRecordPointer blockTable (blktbl_id, AcDb::kForRead);
	if (blockTable.openStatus () == Acad::eOk)
	{
		//m_blockRefIds.RemoveAll();
		AcDbBlockTableRecordIterator* pBtblrIter;
		es=blockTable->newIterator(pBtblrIter);
		if (es!= Acad::eOk)
		{
			blockTable->close();
			return;
		}
		int cc = 0;
		// iterate the entities within the layout
		for(;!pBtblrIter->done();pBtblrIter->step())
		{
			AcDbEntity* pEnt;
			es=pBtblrIter->getEntity(pEnt, AcDb::kForRead);
			if (es!= Acad::eOk)
			{
				blockTable->close();
				delete pBtblrIter;
				return ;
			}

			// if this is a block reference
			if(pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pSelBlockRef = AcDbBlockReference::cast(pEnt);
				if(pSelBlockRef){
					AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
					if(pBTR.openStatus() == Acad::eOk){
						ACHAR* Name=NULL;
						pBTR->getName(Name);
						//AfxMessageBox(Name);
						if(strName.CompareNoCase(Name) ==0)
						{
							//m_blockRefIds.append(pSelBlockRef->id());							
							utils.GetBlockReferenceAttributeList(pSelBlockRef->id(), arr);
							if(arr.length() > 0){//블럭속성이 존재할 경우에만 블럭추가.
								ids.append(pSelBlockRef->id());
								CPrintFormData data;
								//data.FormId = pSelBlockRef->id();
								GetBlockPoint(pSelBlockRef->id(),data.minpoint_X,data.minpoint_Y,data.maxpoint_X,data.maxpoint_Y,data.scale,data.angle);
								data.FormId = pSelBlockRef->id();
								data.BlockName = Name;
								data.BlockPath =m_blkFilePath;
								data.LayoutName = pLayName;
								m_blockRefIds.Add(data);
								//AfxMessageBox(Name);
								arr.removeAll();
							}					
						}
						pBTR->close();
					}
				}
			}

			es=pEnt->close();
			if (es!= Acad::eOk)
			{
				blockTable->close();
				delete pBtblrIter;
				return ;
			}
			cc++;
		}
		CString ccstr;
		ccstr.Format(_T("\n %s의 블록수 : %d"),pLayName,cc);
		//AfxMessageBox(ccstr);
		delete pBtblrIter;
		blockTable->close();
	}

}

//라디오 속성 정보 Excel로 내보내기
void CCALS::OnBnClickedCalsRadexcel()
{
	UpdateData(TRUE);
	m_RadProperty_Btn = 0;	
	this->SetWindowPos(NULL, 0, 0, m_Fold_Width, initrect.bottom - initrect.top, SWP_NOMOVE);
	UpdateData(FALSE);
}

//디오 속성 정보 CAD로 가져오기
void CCALS::OnBnClickedCalsRadcad()
{
	UpdateData(TRUE);
	m_RadProperty_Btn = 1;	
	this->SetWindowPos(NULL, 0, 0,
		initrect.right - initrect.left, initrect.bottom - initrect.top, SWP_NOMOVE);

	m_ExcelList.RemoveAll();
	m_ExcelView.DeleteAllItems();
	//엑셀 문서,시트명 뽑아오기
	RunningExcelData();

	for(int i=0; i<m_ExcelList.GetCount(); i++){
		m_ExcelView.InsertItem(i, m_ExcelList[i].Doc_Name);
		m_ExcelView.SetItem(i,1, LVIF_TEXT, m_ExcelList[i].Sheet_Name, 0, 0, 0, NULL);
	}
		
	UpdateData(FALSE);
}
///////////////////////////////////////검색 및 수정 대상//////////////////////////////////
//라디오 모형
void CCALS::OnBnClickedCalsRadmodel()
{
	UpdateData(TRUE);

	if(m_RadSearch_Btn ==3){
		GetDlgItem(IDC_CALS_EDTCERTAIN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CALS_RADCERTAIN)->MoveWindow(rc.left,rc.top,15,rc.Height());

		
	}else{
		GetDlgItem(IDC_CALS_EDTCERTAIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CALS_RADCERTAIN)->MoveWindow(rc.left,rc.top,rc.Width(),rc.Height());
	}
	
	UpdateData(FALSE);
}

//Edt 특정배치 
void CCALS::OnEnUpdateCalsEdtcertain()
{
	UpdateData(TRUE);
	
	UpdateData(FALSE);
}

//Edt 검색할 Form 이름
void CCALS::OnEnUpdateCalsEdtsearch()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}

//버튼 추출버튼
void CCALS::OnBnClickedCalsBtnextract()
{
	UpdateData(TRUE);
	BeginEditorCommand();
	ads_name en;
	ads_point pt;
	StrInfo si;
	CString strMsg;
	while(true){
		strMsg.LoadStringW(IDS_SELECT_BLOCK); // "\n블록 선택 : \n"
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
			acdbGetObjectId(objId, en);
			//AcDbEntity *pEnt=NULL;

			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){				
				if(pEnt->isKindOf(AcDbBlockReference::desc())){
					AcDbBlockReference* pInsert = AcDbBlockReference::cast(pEnt);
					if(!pInsert){
						strMsg.LoadStringW(IDS_CHECK_ENTITY_BLOCK); // "\n선택된 엔티티가 블록이 아닙니다.\n"
						acutPrintf(strMsg);
						pEnt->close();
						break;
					}
					AcDbObjectId blockDefID = pInsert->blockTableRecord();

					pInsert->close();

					AcDbBlockTableRecord *pBTR;
					if(Acad::eOk != (es= acdbOpenObject(pBTR, blockDefID, AcDb::kForRead))){
						strMsg.LoadStringW(IDS_ACCESS_BLOCK); // \n블록 정의 엑세스 할 수 없습니다..\n
						acutPrintf(strMsg);
						break;
					}

					const TCHAR* pBlkName;
					es = pBTR->getName(pBlkName);
					pBTR->close();
					//m_SearchForm_Str.Replace(_T(" "),NULL);
					if(m_SearchForm_Str.CompareNoCase(_T(""))==0){
						m_SearchForm_Str = m_SearchForm_Str + pBlkName;
					}else{
						if(m_SearchForm_Str.Right(1)==_T(","))
							m_SearchForm_Str = m_SearchForm_Str + _T(" ") + pBlkName;
						else{
							m_SearchForm_Str = m_SearchForm_Str + _T(", ") + pBlkName;
						}
					}
					

					//si.FileForm = pBlkName;
					//
					//m_StrList.Add(si);

					if((Acad::eOk != es) || !pBlkName){
						strMsg.LoadStringW(IDS_EXTRACT_BLOCK); // 블록 이름을 추출 할 수 없습니다.\n
						acutPrintf(strMsg);
						break;
					}
					pEnt->close();

					//acutPrintf(_T("\n 블록 이름 : '%s'\n"),pBlkName);
					break;	
				}
			}

		}
	}
	CompleteEditorCommand();
	UpdateData(FALSE);
}
////////////////////////////////한 도면에 Form이 여러개인 경우/////////////////////////////////////
//라디오 정렬순서 Z
void CCALS::OnBnClickedCalsRadzsort()
{
	UpdateData(TRUE);
	m_RadSort_Btn = 0;
	m_sorttype = 0;
	UpdateData(FALSE);
}

//비트맵 정령순서 Z
void CCALS::OnBnClickedCalsButton1()
{
	UpdateData(TRUE);
	OnBnClickedCalsRadzsort();
	UpdateData(FALSE);
}

//라디오 정렬순서 역Z
void CCALS::OnBnClickedCalsRadnsort()
{
	UpdateData(TRUE);
	m_RadSort_Btn = 1;
	m_sorttype = 1;
	UpdateData(FALSE);
}

//비트맵 정령순서 역Z
void CCALS::OnBnClickedCalsButton2()
{
	UpdateData(TRUE);
	OnBnClickedCalsRadnsort();
	UpdateData(FALSE);
}

//라디오 정렬순서 N
void CCALS::OnBnClickedCalsRadnmsort()
{
	UpdateData(TRUE);
	m_RadSort_Btn = 2;
	m_sorttype = 2;
	UpdateData(FALSE);
}

//비트맵 정령순서 N
void CCALS::OnBnClickedCalsButton3()
{
	UpdateData(TRUE);
	OnBnClickedCalsRadnmsort();
	UpdateData(FALSE);
}

//라디오 정렬순서 역N
void CCALS::OnBnClickedCalsRadzmsort()
{
	UpdateData(TRUE);
	m_RadSort_Btn = 3;
	m_sorttype = 3;
	UpdateData(FALSE);
}

//비트맵 정령순서 역N
void CCALS::OnBnClickedCalsButton4()
{
	UpdateData(TRUE);
	OnBnClickedCalsRadzmsort();
	UpdateData(FALSE);
}

//Edt 정렬오차 
void CCALS::OnEnUpdateCalsEdterror()
{
	UpdateData(TRUE);
	m_SortError = _tstof(m_SortError_Str);
	UpdateData(FALSE);
}
//////////////////////////////////리스트 박스/////////////////////////////////////////////
//리스트 박스

//리스트 배열 위치 교환
void CCALS::RowSwap(int a, int b){
	FileInfos files;
	files = m_FileList[a];
	m_FileList[a] = m_FileList[b];
	m_FileList[b] = files;
}
//리스트 파일에 배열 업데이트
void CCALS::FileListReIndexing(){
	for(int i = 0; i < m_FileList.GetCount(); i++)	m_FileList[i].index = i;
	m_FileView.DeleteAllItems();
	for(int i = 0; i < m_FileList.GetCount(); i++){
		CString seq_string; 
		seq_string.Format(_T("%d"), m_FileList[i].index + 1); 
		m_FileView.InsertItem(i, m_FileList[i].filename);		
		m_FileView.SetItem(i, 1, LVIF_TEXT, m_FileList[i].result, 0, 0, 0, NULL);
		//m_FileView.SetItem(i,3,LVIF_TEXT,m_FileList[i].plotstyle,0,0,0,NULL);
	}
}

BOOL CCALS::CreateDir(CString dir){
	CFileFind file;
	CString strFile = _T("*.*");
	BOOL bResult = file.FindFile(dir + strFile);
	if(!bResult)	bResult = CreateDirectory(dir, NULL);

	return bResult;
}

//파일 리스트뷰에 데이터 올리기
void CCALS::FileViewRefresh(){
	m_FileView.DeleteAllItems();
	for(int i = 0; i< m_FileList.GetCount(); i++){
		CString seq_string; 
		seq_string.Format(_T("%d"), i + 1); 
		m_FileView.InsertItem(i,m_FileList[i].filename);		
		m_FileView.SetItem(i, 1, LVIF_TEXT, m_FileList[i].result, 0, 0, 0, NULL);
		//		m_FileView.SetItem(i,3,LVIF_TEXT,m_FileList[i].plotstyle,0,0,0,NULL);
	}
}

//버튼 도면 추가
void CCALS::OnBnClickedCalsBtnAdddwg()
{
	UpdateData(TRUE);
	CFileFind finder;
	static TCHAR BASED_CODE szFilter[] = _T("dwg (*.dwg)|*.dwg||");
	CFileDialog dlg(TRUE, _T("dwg"),NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,szFilter);
	if(IDOK == dlg.DoModal()){
		m_FilePath = dlg.GetPathName();
		//m_FileName = dlg.GetFileName();
		AcApDocument *activeDoc = NULL;
		activeDoc = acDocManager->curDocument();
		bool bisExist = false;
		for(int i=0; i<m_FileList.GetCount(); i++){
			if(m_FileList[i].full_path.CompareNoCase((CString)m_FilePath) == 0){
				bisExist=true;
				break;
			}
		}
		if(!bisExist){
			FileInfos fi; 

			//fi.index = m_LastIdx;		
			fi.filename = dlg.GetFileName();
			fi.full_path = m_FilePath;
			if(fi.full_path.Find(_T("\\")) != -1) m_FileList.Add(fi);
			//m_FileList.Add(fi);
			//m_FileView.InsertItem(m_LastIdx+1, m_FileName);
			m_LastIdx++;
		}

	}
	FileListReIndexing();

	UpdateData(FALSE);
}

//버튼 폴더 추가
void CCALS::OnBnClickedCalsBtnAddfolder()
{
	UpdateData(TRUE);
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];		//경로 저장 버퍼

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);	//파일 경로

	// 경로 가져와 사용할 경우 
	CString str;
	str.Format(_T("%s"), szBuffer);

	FileSearch(szBuffer);
	FileListReIndexing();	
	UpdateData(FALSE);
}

void CCALS::FileSearch(CString path){

	FindFile(path,_T("*.dwg"));

	FileViewRefresh();
}

//경로 내의 지정한 확장자 파일 구하기
bool CCALS::FindFile(CString path, CString extends){
	CFileFind finder;
	BOOL working = finder.FindFile(path + L"\\" + extends);
	int i = 0;
	while(working){
		working = finder.FindNextFile();
		if(finder.IsDots())	continue;
		if(finder.IsDirectory()){
		}   
		else{    
			FileInfos fi; 
			fi.index = i;			
			fi.filename = finder.GetFileName();     
			fi.full_path = finder.GetFilePath();
			m_FileList.Add(fi);
			//i++;
			i++;
		}
	}
	return true;
}
//버튼 활성파일
void CCALS::OnBnClickedCalsBtnActivedwg()
{
	UpdateData(TRUE);
	GetActiveDWG();
	FileListReIndexing();
	UpdateData(FALSE);
}

//버튼 열린파일
void CCALS::OnBnClickedCalsBtnOpendwg()
{
	UpdateData(TRUE);
	OnBnClickedCalsBtnActivedwg(); //열린 도면 열기를 하면 활성 도면도 같이 추가
	GetOpenDWG();
	FileListReIndexing();
	UpdateData(FALSE);
}

//버튼 선택열기
void CCALS::OnBnClickedCalsSelOpen()
{
	UpdateData(TRUE);	
	POSITION pos = m_FileView.GetFirstSelectedItemPosition();
	int selected = -1;
	if(pos != NULL){
		while(pos){
			int nItem = m_FileView.GetNextSelectedItem(pos);
			selected = nItem +1;
		}
	}

	if(selected <0)	return;
	CString path = m_FileList[selected-1].full_path;
	ShowWindow(SW_HIDE);
	AcApDocument *doc=NULL;
	BOOL bIsopen = IsOpenDocument(path, doc);
	if(bIsopen)	acDocManager->setCurDocument(doc);
	else		Acad::ErrorStatus es = acDocManager->appContextOpenDocument(path);	
	ShowWindow(SW_SHOW);
	UpdateData(FALSE);	
}

//버튼 선택삭제
void CCALS::OnBnClickedCalsSelDel()
{
	UpdateData(TRUE);
	if(m_FileView.GetItemCount() <= 0)	return;
	if(m_FileList.GetCount() <= 0)	return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if(nSelectedItem == m_FileList.GetCount())	nSelectedItem = nSelectedItem - 1;
	if(nSelectedItem < 0)	return;

	POSITION pos = m_FileView.GetFirstSelectedItemPosition();
	if(pos != NULL){
		while (pos){
			int nItem = m_FileView.GetNextSelectedItem(pos);
			for(int i = 0; i < m_FileList.GetCount(); i++){
				if(m_FileList[i].filename.CompareNoCase(m_FileView.GetItemText(nItem, 0)) == 0){
					m_FileList.RemoveAt(i);
					break;
				}
			}
		}
	}
	FileListReIndexing();
	m_FileView.SetItemState(nSelectedItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_FileView.EnsureVisible(nSelectedItem, TRUE);
	m_FileView.SetFocus(); //선택된 항목들 포커스 유지
	UpdateData(FALSE);
}

//버튼 전체삭제
void CCALS::OnBnClickedCalsAllDel()
{
	UpdateData(TRUE);
	if(m_FileList.GetCount() < 0)	return;
	m_FileList.RemoveAll();
	FileListReIndexing();
	FileViewRefresh();
	UpdateData(FALSE);
}

//버튼 위로
void CCALS::OnBnClickedCalsBtnUp()
{
	UpdateData(TRUE);
	if(m_FileView.GetItemCount() <= 0) return;
	if(m_FileList.GetCount() <= 0) return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if(nSelectedItem < 0) return;
	if(nSelectedItem == 0) return;
	RowSwap(nSelectedItem, nSelectedItem-1);
	FileListReIndexing();
	m_FileView.SetItemState(nSelectedItem -1 , LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_FileView.EnsureVisible(nSelectedItem -1 , TRUE);
	m_FileView.SetFocus();// 선택된 항목들 포커스 유지
	
	UpdateData(FALSE);
}

//버튼 아래로
void CCALS::OnBnClickedCalsBtnDown()
{
	UpdateData(TRUE);
	if(m_FileView.GetItemCount() <= 0)return;
	if(m_FileList.GetCount() <= 0)return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if(nSelectedItem < 0)return;
	if(nSelectedItem == m_FileList.GetCount() -1)return;
	RowSwap(nSelectedItem, nSelectedItem +1);
	FileListReIndexing();
	m_FileView.SetItemState(nSelectedItem + 1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_FileView.EnsureVisible(nSelectedItem + 1, TRUE);
	m_FileView.SetFocus(); // 선택된 항목들 포커스 유지
	UpdateData(FALSE);
}

//버튼 맨 위
void CCALS::OnBnClickedCalsBtnTop()
{
	UpdateData(TRUE);
	if(m_FileView.GetItemCount() <= 0)return;
	if(m_FileList.GetCount() <= 0)return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if(nSelectedItem < 0)return;
	m_FileList.InsertAt(0, m_FileList[nSelectedItem]);	
	m_FileList.RemoveAt(nSelectedItem+1);	
	FileListReIndexing();
	m_FileView.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_FileView.EnsureVisible(0, TRUE);
	m_FileView.SetFocus(); // 선택된 항목들 포커스 유지
	//for(int i=0; i<m_FileList.GetCount(); i++){		
	//	acutPrintf(_T("\n%s\n"), m_FileList[i].full_path);
	//	
	//}
	UpdateData(FALSE);
}

//버튼 맨 아래
void CCALS::OnBnClickedCalsBtnBottom()
{
	UpdateData(TRUE);
	if(m_FileView.GetItemCount() <= 0)return;
	if(m_FileList.GetCount() <= 0)return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if(nSelectedItem < 0)return;
	if(nSelectedItem == m_FileList.GetCount() -1)return;
	RowSwap(nSelectedItem, m_FileList.GetCount()-1);
	FileListReIndexing();
	m_FileView.SetItemState(m_FileList.GetCount()-1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_FileView.EnsureVisible(m_FileList.GetCount()-1, TRUE);
	m_FileView.SetFocus(); // 선택된 항목들 포커스 유지
	//for(int i=0; i<m_FileList.GetCount(); i++){
	//	acutPrintf(_T("\n%s\n"), m_FileList[i].full_path);
	//}
	UpdateData(FALSE);
}

//라디오 정렬 위
void CCALS::OnBnClickedCalsSort1()
{
	UpdateData(TRUE);
	m_ListSort_Btn = 0;
	
	if(m_FileView.GetItemCount() <= 0)return;
	QSortCArray(m_FileList, CompareStringAscending);
	FileListReIndexing();
	FileViewRefresh();	
	UpdateData(FALSE);
}

//라디오 정렬 아래
void CCALS::OnBnClickedCalsSort2()
{
	UpdateData(TRUE);
	m_ListSort_Btn = 1;
	if(m_FileView.GetItemCount() <= 0)return;
	QSortCArray(m_FileList, CompareStringDescending);
	FileListReIndexing();
	FileViewRefresh();
	UpdateData(FALSE);
}
/////////////////////////////////속성 정보 캐드로 가져오기////////////////////////////////////
//가져오기
void CCALS::OnBnClickedCalsBtnImport()
{
	m_ExcelValueList.RemoveAll();
	CString strMsg;

	int nSelectedItem = m_ExcelView.GetNextItem(-1, LVNI_SELECTED);
	if(nSelectedItem < 0)return;
	//if(nSelectedItem == m_ExcelList.GetCount() -1)return;
	bool b_Excel = LoadtoExcel(m_ExcelList[nSelectedItem].Doc_Name,m_ExcelList[nSelectedItem].Sheet_Name);
	UpdateAttBlockData();
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	strMsg.LoadStringW(IDS_IMPORT_CAD); // CAD로 가져오기가 완료되었습니다.
	AfxMessageBox(strMsg);
}

BOOL CCALS::PlotBlockInitExport(AcDbObjectId Layout_id)
{
	//블럭 타이틀 중복 제거하고 array에 저장
	CStringArray m_blkTitleArr, res_m_blkTitleArr;
	for(int i=0; i<m_ExcelValueList.GetCount(); i++){
		CString m_blkTitle = m_ExcelValueList[i].dwgBlkTitle;
		m_blkTitleArr.Add(m_blkTitle);
	}
	RemoveDuplicates(m_blkTitleArr,res_m_blkTitleArr);

	m_blockRefIds.RemoveAll();
	for(int j=0; j<res_m_blkTitleArr.GetCount(); j++){
		//m_blockRefIds.RemoveAll();
		FindBlock(Layout_id, m_blockRefIds, res_m_blkTitleArr[j]);
	}
	
	if(m_blockRefIds.GetCount() > 0){
		if(m_RadSort_Btn == 0)
			ZSort(m_SortError);
		else if(m_RadSort_Btn == 1)
			ZMSort(m_SortError);
		else if(m_RadSort_Btn == 2)
			NSort(m_SortError);
		else if(m_RadSort_Btn == 3)
			NMSort(m_SortError);
	}
	//if(m_blockRefIds.GetCount() > 0)
	//	m_PrintRefIds.Append(m_blockRefIds);
	
	for(int k=0; k<m_blockRefIds.GetCount(); k++){
		UpdateBlock(Layout_id, arrarr[arrarrcnt],m_blockRefIds[k].FormId);
		arrarrcnt++;
		//acutPrintf(_T("\n%d"),arrarrcnt);
	}

	return true;
}

//엑셀 값들 배열에 추출
bool CCALS::LoadtoExcel(CString DocName, CString SheetName){
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

	IDispatch* pdispExcelApp =NULL;
	IUnknown *pUnk=NULL;
	CLSID clsExcelApp;
	CString strMsg;
	
	if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsExcelApp))) 
	{
		strMsg.LoadStringW(IDS_SETUP_EXCEL); // 엑셀이 설치되어 있지 않습니다.
		AfxMessageBox(strMsg);
	}



	HRESULT hr = GetActiveObject(clsExcelApp, NULL, (IUnknown**)&pUnk);
	if(SUCCEEDED(hr)){
		hr = pUnk->QueryInterface(IID_IDispatch, (void **)&pdispExcelApp);
	}
	else
	{
		strMsg.LoadStringW(IDS_FIND_EXCEL); // "엑셀을 찾을 수 없습니다."
		AfxMessageBox(strMsg);
	}

	//App 찾기 실패시 리턴
	if(NULL == pdispExcelApp){		
		return false;
	}					
	
	//Application

	app.AttachDispatch(pdispExcelApp);

	//Workbooks	
	
	lpdisp=app.get_Workbooks();
	ASSERT(lpdisp);
	books.AttachDispatch(lpdisp);
	//Books
	ExcelInfos exif;
	CString bookName;
	int booksCnt = books.get_Count();
	for(int i=1; i<=books.get_Count(); i++){//선택 문서 찾으면 for문 나옴
		lpdisp=books.get_Item(COleVariant((short)i));
		ASSERT(lpdisp);
		//book
		book.AttachDispatch(lpdisp);
		if(book.get_Name() == DocName)
			break;
		
	}
	//sheets
	lpdisp = book.get_Sheets();
	ASSERT(lpdisp); 
	sheets.AttachDispatch(lpdisp); 

	//m_ExcelList[i-1].Sheet_Name = sheet.get_Name();
	for(int i=1; i<=sheets.get_Count(); i++){//선택 시트 찾으면 for문 나옴
		//sheet
		lpdisp = sheets.get_Item(COleVariant((short)i));
		ASSERT(lpdisp);
		sheet.AttachDispatch(lpdisp);
		if(sheet.get_Name() == SheetName)
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
	strMsg.LoadStringW(IDS_NUMBER);
	if(str.CompareNoCase(strMsg)!=0)
		return false;
	curRange = startrng.get_Item(COleVariant((long)1), COleVariant((long)2)).pdispVal;
	str = curRange.get_Value2();
	strMsg.LoadStringW(IDS_PATH);	// 경로
	if(str.CompareNoCase(strMsg)!=0)
		return false;	
	curRange = startrng.get_Item(COleVariant((long)1), COleVariant((long)3)).pdispVal;
	str = curRange.get_Value2();
	if(str.CompareNoCase(_T("Layout"))!=0)			
		return false;
	curRange = startrng.get_Item(COleVariant((long)1), COleVariant((long)4)).pdispVal;
	str = curRange.get_Value2();
	strMsg.LoadStringW(IDS_BLOCKNAME);	// 블록명
	if(str.CompareNoCase(strMsg)!=0)
		return false;

	//curRange = sheet.get_Range(COleVariant(_T("A1")), VOptional);
	//curRange = curRange.get_Resize(COleVariant((short)50), COleVariant((short)100));

	//열 개수.
	int m_MaxColIdx=1, m_MaxRowIdx=1;
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
	arrarr.removeAll();
	arr.removeAll();
	ExcelValue Exv;
	AttBlock att;
	

	if(iRows-1 < 1)return false;	//열이 1개 이하이면 종료
	//엑셀 값들 배열에 넣음.
	for(int i=2; i<=iRows-1; i++){
		
		for(int j=2; j<=iCols-1; j++){
			if(j==2){
				curRange = startrng.get_Item(COleVariant((long)i), COleVariant((long)j)).pdispVal;
				Exv.dwgPath = curRange.get_Value2();
			}else if(j==3){
				curRange = startrng.get_Item(COleVariant((long)i), COleVariant((long)j)).pdispVal;
				Exv.dwgLayout = curRange.get_Value2();
			}else if(j==4){
				curRange = startrng.get_Item(COleVariant((long)i), COleVariant((long)j)).pdispVal;
				Exv.dwgBlkTitle = curRange.get_Value2();
			}else{// j >= 5
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
//속성정보Cad로 가져오기 리스트 추출 함수
bool CCALS::RunningExcelData(){
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

	IDispatch* pdispExcelApp =NULL;
	IUnknown *pUnk=NULL;
	CLSID clsExcelApp;
	CString strMsg;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsExcelApp))) 
	{
		strMsg.LoadStringW(IDS_SETUP_EXCEL); // "엑셀이 설치되어 있지 않습니다."
		AfxMessageBox(strMsg);
	}


	HRESULT hr = GetActiveObject(clsExcelApp, NULL, (IUnknown**)&pUnk);
	if(SUCCEEDED(hr)){
		hr = pUnk->QueryInterface(IID_IDispatch, (void **)&pdispExcelApp);
	}
	else
	{
		strMsg.LoadStringW(IDS_FIND_EXCEL); // "엑셀을 찾을 수 없습니다."
		AfxMessageBox(strMsg);
	}

	if(NULL == pdispExcelApp){		
		return false;
	}					

	app.AttachDispatch(pdispExcelApp);	
	lpdisp=app.get_Workbooks();	
	ASSERT(lpdisp);
	books.AttachDispatch(lpdisp);

	//Books
	ExcelInfos exif;
	CString bookName;
	int bookscnt = books.get_Count();
	for(int i=1; i<=books.get_Count(); i++){
		lpdisp=books.get_Item(COleVariant((short)i));
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
		for(int i=1; i<=sheets.get_Count(); i++){
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
//속성 정보 캐드로 dwg정보 = m_ExcelValueList, 속성정보 = arrarr,arr
bool CCALS::UpdateAttBlockData(){
	//엑셀의 블록속성배열의 수를 UpdateBlock에서 중복된 값을 가져오지 않게 하기 위해
	arrarrcnt=0;

	for(int i=0; i<m_ExcelValueList.GetCount(); i++)
	{
		//일단 해당 파일이 열려있는지 확인하자.
		//열려있을경우 새로 파일을 열지 말자		
		//AfxMessageBox(m_FileList[i].full_path);
		if(i>0){//이전 블로경로,레이아웃이 같으면 넘어감. PlotBlockInitExport->Findblock 함수에서 중복부분 처리
			if(m_ExcelValueList[i].dwgPath == m_ExcelValueList[i-1].dwgPath){
				if(m_ExcelValueList[i].dwgLayout == m_ExcelValueList[i-1].dwgLayout){
					continue;	
				}
			}
		}
		m_blkFilePath = m_ExcelValueList[i].dwgPath;	//m_blkFilePath를 전역으로 저장해둠 Findblock에서 쓰기위해

		AcApDocument* doc;
		BOOL bIsopen = IsOpenDocument(m_ExcelValueList[i].dwgPath);
		if(bIsopen){
			doc = GetOepndDocument(m_ExcelValueList[i].dwgPath);
			if(doc!=NULL)	acDocManager->setCurDocument(doc);
		}
		else
		{
			Acad::ErrorStatus es =acDocManager->appContextOpenDocument(m_ExcelValueList[i].dwgPath);
		}

		AcApDocument * activeDoc =acDocManager->curDocument();
		AcApLayoutManager *pLayoutManager = (AcApLayoutManager *)acdbHostApplicationServices()->layoutManager(); 
		const ACHAR* ActiveLayoutName = pLayoutManager->findActiveLayout(Adesk::kTrue);
		AcDbDatabase * pDb = activeDoc->database();

		acDocManager->lockDocument(acDocManager->curDocument());

		CArray <AcDbObjectId,AcDbObjectId> arrLayout;
		GetLayout(pDb, arrLayout);//레이아웃 목록뽑아옴

		AcDbObject *pObj;


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
			#ifndef _2025
				OriginLayout = pLayoutManager->findActiveTab();
			#else // z2025부터 함수 변경
				AcString ActiveName;
				OriginLayout = (ACHAR*)pLayoutManager->getActiveTab(ActiveName);
			#endif
		#elif _ACAD
			AcString ActiveName;
			OriginLayout = (ACHAR*)pLayoutManager->getActiveTab(ActiveName);
		#endif

		const ACHAR *csModel=acdbGetReservedString( AcDb::kModel );
		CString strModel(csModel);


		AcDbObjectId layout_id = AcDbObjectId::kNull;
		for(int k = 0; k < arrLayout.GetCount() ;k++)
		{
			ACHAR *pLayName = NULL;
			AcDbLayout* pLayout = NULL;
			Acad::ErrorStatus es =	acdbOpenObject(pLayout,arrLayout[k],AcDb::kForWrite); 
			if(pLayout)
			{
				pLayout->getLayoutName(pLayName);
				//레이아웃이 엑셀값과 같을때 실행
				if( ((CString)pLayName).CompareNoCase(m_ExcelValueList[i].dwgLayout) == 0)
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
		if(!bIsopen){
			acDocManager->unlockDocument(activeDoc);
			acDocManager->activateDocument(activeDoc);
			acDocManager->lockDocument(activeDoc); // Lock
			activeDoc->database()->saveAs(acDocManager->curDocument()->fileName());
			acdbSetDbmod(activeDoc->database(), 0); // clear changes flag
			acDocManager->unlockDocument(activeDoc);//unlock
			acDocManager->closeDocument(activeDoc);
			activeDoc=NULL;

		}
		else{
			if(i > 0){
				if(m_ExcelValueList[i].dwgPath == m_ExcelValueList[i-1].dwgPath){
					acDocManager->unlockDocument(activeDoc);
					acDocManager->activateDocument(activeDoc);
					acDocManager->lockDocument(activeDoc); // Lock
					activeDoc->database()->saveAs(acDocManager->curDocument()->fileName());
					acdbSetDbmod(activeDoc->database(), 0); // clear changes flag
					acDocManager->unlockDocument(activeDoc);//unlock
					acDocManager->closeDocument(activeDoc);
					activeDoc=NULL;
				}else
					acDocManager->unlockDocument(activeDoc);
			}

		}	

	}
	return true;
}

//속성블럭 ID검색하여 엑셀값으로 변경
void CCALS::UpdateBlock(AcDbObjectId Layout_id,AttBlockArray& m_AttblkArr,AcDbObjectId m_blkID)
{

	AcDbObjectId blktbl_id = AcDbObjectId::kNull;
	ACHAR *pLayName = NULL;
	AcDbLayout* pLayout = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pLayout,Layout_id,AcDb::kForRead); 
	if(pLayout)
	{
		blktbl_id = pLayout->getBlockTableRecordId();
		pLayout->getLayoutName(pLayName);
		pLayout->close();
	}

	//m_blockRefIds.RemoveAll();
	AcDbObjectIdArray ids;

	AcDbBlockTableRecordPointer blockTable (blktbl_id, AcDb::kForRead);
	if (blockTable.openStatus () == Acad::eOk)
	{
		//m_blockRefIds.RemoveAll();
		AcDbBlockTableRecordIterator* pBtblrIter;
		es=blockTable->newIterator(pBtblrIter);
		if (es!= Acad::eOk)
		{
			blockTable->close();
			return;
		}
		int cc = 0;
		// iterate the entities within the layout
		for(;!pBtblrIter->done();pBtblrIter->step())
		{
			AcDbEntity* pEnt;
			es=pBtblrIter->getEntity(pEnt, AcDb::kForRead);
			if (es!= Acad::eOk)
			{
				blockTable->close();
				delete pBtblrIter;
				return ;
			}

			// if this is a block reference
			if(pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pSelBlockRef = AcDbBlockReference::cast(pEnt);
				if(pSelBlockRef){
					AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
					if(pBTR.openStatus() == Acad::eOk){
						ACHAR* Name=NULL;
						pBTR->getName(Name);
						//AfxMessageBox(Name);
						if(m_blkID == pSelBlockRef->id())//if(strName.CompareNoCase(Name) ==0)
						{
							//m_blockRefIds.append(pSelBlockRef->id());							
							utils.GetBlockReferenceAttributeList(pSelBlockRef->id(), compareArr);
							
							//속성의 개수가 0 또는 가져온 속성개수와 같지 않을때 반복
							if(/*compareArr.length() != m_AttblkArr.length() ||*/ compareArr.length() == 0)continue;
							//AfxMessageBox(_T("ok"));
							for(int i=0; i<m_AttblkArr.length(); i++){
								CString m_AttTag,m_AttValue;
								m_AttTag = m_AttblkArr[i].AttributeTag;
								m_AttValue = m_AttblkArr[i].strValue;
								utils.SetBlockReferenceAttribute(pSelBlockRef->id(),m_AttTag,m_AttValue);
							}
							break;
						}
						pBTR->close();
					}
				}
			}

			es=pEnt->close();
			if (es!= Acad::eOk)
			{
				blockTable->close();
				delete pBtblrIter;
				return ;
			}
			cc++;
		}
		
		delete pBtblrIter;
		blockTable->close();
	}

}

//활성화된 도면 가져오기
void CCALS::GetActiveDWG(){	
	
	//현재 선택된 도면
	AcApDocument *activeDoc = NULL;
	activeDoc = acDocManager->curDocument();
	bool bisExist = false;
	for(int i = 0; i < m_FileList.GetCount(); i++){
		if(m_FileList[i].full_path.CompareNoCase((CString)activeDoc->fileName()) == 0){
			bisExist = true;
			break;
		}
	}
	if(!bisExist){
		if(IsOpenDocument(activeDoc->fileName())){
			FileInfos fileinfo;			
			fileinfo.index = m_LastIdx;			
			fileinfo.full_path = activeDoc->fileName();
			CString str, res;
			str = activeDoc->fileName();
			int NameIdx = str.ReverseFind(_T('\\'));//끝에서 \\ 인덱스 찾음
			res = str.Mid(NameIdx+1,str.GetLength());//찾은 idx에서 끝까지 도면이름
			fileinfo.filename = res;
			
			//fileinfo.plotstyle = _T("monochrome.ctb");	
			//fileinfo.plotstyle = global_UserSetting.PlotStyleName;
			if(fileinfo.full_path.Find(_T("\\")) != -1)	m_FileList.Add(fileinfo);
			else m_FileList.Add(fileinfo);
			m_LastIdx++;
		}
	}
}

//도면이 현재 열려있는지 확인하기
BOOL CCALS::IsOpenDocument(CString dwg){
	BOOL bIsOpen = FALSE;
	AcApDocument* pDoc;
	//여기서 부터는 열린 도면들
	AcApDocumentIterator *pIter = acDocManager->newAcApDocumentIterator();
	for(; !pIter->done(); pIter->step()){
		AcApDocument* pDoc = pIter->document();
		CString dwgTitle = pDoc->fileName();
		if(dwg.CompareNoCase(dwgTitle) == 0){
			bIsOpen = TRUE;
			break;
		}
	}
	delete pIter;
	return bIsOpen;
}
//도면이 현재 열려있는지 확인하기
BOOL CCALS::IsOpenDocument(CString dwg, AcApDocument *doc){
	BOOL bIsOpen = FALSE;
	//여기서 부터는 열린 도면들
	AcApDocumentIterator *pIter = acDocManager->newAcApDocumentIterator();
	for(; !pIter->done(); pIter->step()){
		if(dwg.CompareNoCase(pIter->document()->fileName()) == 0){
			bIsOpen = TRUE;
			doc = pIter->document();
			break;
		}
	}
	delete pIter;
	return bIsOpen;
}

//열려있는 모든 도면 가져오기
void CCALS::GetOpenDWG(){
	//여기서 부터는 열린 도면들
	AcApDocumentIterator *pIter = acDocManager->newAcApDocumentIterator();
	while(!pIter->done()){
		if(pIter->document() == acDocManager->curDocument()){
			pIter->step();
			continue;
		}
		bool bisExist = false;
		const ACHAR *pDocName = NULL;
		for(int i = 0; i < m_FileList.GetCount(); i++){	
			pDocName=pIter->document()->fileName();
			if(m_FileList[i].full_path.CompareNoCase(pDocName) == 0){
				bisExist = true;
				break;
			}
		}
		if(bisExist == false){
			FileInfos fileinfo;			
			fileinfo.filename = pIter->document()->docTitle();
			ACHAR *pNewDocName = NULL;
			acutNewString(pDocName, pNewDocName);
			if(pNewDocName)	fileinfo.full_path=pNewDocName;
			//fileinfo.plotstyle = _T("monochrome.ctb");
			//fileinfo.plotstyle = global_UserSetting.PlotStyleName;
			if(fileinfo.full_path.Find(_T("\\")) != -1)	m_FileList.Add(fileinfo);
		}
		pIter->step();
	}
	delete pIter;
}

extern CCALS *pCALS;
void CCALS::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (g_hCurrResource)
		FreeLibrary(g_hCurrResource);

	delete this;
	pCALS = NULL;
	//CAcUiDialog::PostNcDestroy();
}

//Enter ESC 키 모달영향X
BOOL CCALS::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	return CAcUiDialog::PreTranslateMessage(pMsg);
}

void CCALS::GetLayout(AcDbDatabase* pDb, CArray <AcDbObjectId,AcDbObjectId>& arrLayout){
	AcDbDictionary *pDict = NULL;
	AcDbObject *pObj;
	if( pDb->getLayoutDictionary( pDict, AcDb::kForRead ) == Acad::eOk)
	{ 
		AcDbDictionaryIterator *pIter = pDict->newIterator();
		for(;!pIter->done();pIter->next())
		{

			ACHAR *pLayName = NULL;
			Acad::ErrorStatus es= pIter->getObject(pObj,AcDb::kForRead);
			if(es!=Acad::eOk) continue;
			AcDbLayout *pLayout = AcDbLayout::cast(pObj);
			if(pLayout)
			{
				int iLay = pLayout->getTabOrder();
				pLayout->getLayoutName(pLayName);
				arrLayout.SetAtGrow(iLay, pLayout->objectId());
				//arrLayout.Add(pLayout->objectId());
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

void CCALS::GetBlockPoint(AcDbObjectId& blockId, double& minpoint_X, double& minpoint_Y,double& maxpoint_X,double& maxpoint_Y, AcGeScale3d& scale,double& angle)
{
	AcDbEntityPointer pEnt(blockId, AcDb::kForRead);
	if(pEnt.openStatus()==Acad::eOk)
	{

		AcDbBlockReference* pBlockRef=AcDbBlockReference::cast(pEnt);
		if(pBlockRef){
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

//숫자, 점, (-) 추출
BOOL CCALS::CharacterExtraction(CString str){
	TCHAR* getAtChar;
	CString getstr,reStr;
	StrInfo si;
	
	
	str.Replace(_T(", "), _T(","));
	int roop=0;
	for(int i = 0; i < str.GetLength(); i++){
		getstr = str.Mid(i,1);
		getAtChar = (TCHAR*)(LPCTSTR)getstr;
		int ascii = __toascii(*getAtChar);
		if((ascii == 44)){
			si.FileForm = str.Mid(roop,i - roop);
			m_StrList.Add(si);
			roop = i+1;
		}
	}
	si.FileForm = str.Mid(roop, str.GetLength());
	m_StrList.Add(si);
	return true;
}

#pragma  region 정렬
//z->방향 소트 (완료)
void CCALS::ZSort(double tol){
	int blockCnt = m_blockRefIds.GetCount();
	if(blockCnt == 1)
		return;

	QSortCArray(m_blockRefIds, ComparePointY_De);
	for(int i = 0; i < blockCnt; i++){
		for(int k = 0; k < blockCnt - 1; k++){
			double a = m_blockRefIds[k].minpoint_Y;
			double b = m_blockRefIds[k + 1].minpoint_Y;
			if(abs(m_blockRefIds[k].minpoint_Y - m_blockRefIds[k + 1].minpoint_Y) < tol){
				if(m_blockRefIds[k].minpoint_X > m_blockRefIds[k + 1].minpoint_X){
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
void CCALS::NSort(double tol){
	int blockCnt = m_blockRefIds.GetCount();
	if(blockCnt == 1)	return;

	QSortCArray(m_blockRefIds, ComparePointX);
	for(int i = 0; i < blockCnt; i++){
		for(int k = 0; k < blockCnt - 1; k++){
			if(abs(m_blockRefIds[k].minpoint_X - m_blockRefIds[k + 1].minpoint_X) < tol){
				if(m_blockRefIds[k].minpoint_Y < m_blockRefIds[k + 1].minpoint_Y){
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
void CCALS::ZMSort(double tol){
	int blockCnt = m_blockRefIds.GetCount();
	if(blockCnt == 1)	return;
	QSortCArray(m_blockRefIds, ComparePointY);

	for(int i = 0; i < blockCnt; i++){
		for(int k = 0; k < blockCnt - 1; k++){
			if(abs(m_blockRefIds[k].minpoint_Y - m_blockRefIds[k + 1].minpoint_Y) < tol){
				if(m_blockRefIds[k].minpoint_X > m_blockRefIds[k + 1].minpoint_X){
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
void CCALS::NMSort(double tol){
	int blockCnt = m_blockRefIds.GetCount();
	if(blockCnt == 1)	return;
	QSortCArray(m_blockRefIds, ComparePointX);

	for(int i = 0; i< blockCnt; i++){
		for(int k = 0; k<blockCnt - 1; k++){
			if(abs(m_blockRefIds[k].minpoint_X - m_blockRefIds[k + 1].minpoint_X) < tol){
				if(m_blockRefIds[k].minpoint_Y > m_blockRefIds[k + 1].minpoint_Y){
					CPrintFormData TmpId;
					TmpId = m_blockRefIds[k];
					m_blockRefIds[k] = m_blockRefIds[k + 1];
					m_blockRefIds[k + 1] = TmpId;
				}
			}
		}
	}
}
//텍스트 색상 변경
HBRUSH CCALS::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CAcUiDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetDlgCtrlID() == IDC_STATIC1){
		pDC->SetTextColor(RGB(153,0,0));
	}
	return hbr;
}


//void CCALS::OnLvnItemchangedCalsListfiles2(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//}


void CCALS::OnBnClickedCalsHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/CALS.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}


BOOL CCALS::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (g_hCurrResource)
		FreeLibrary(g_hCurrResource);

	delete this;
	pCALS = NULL;

	return CAcUiDialog::DestroyWindow();
}
