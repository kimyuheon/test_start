// ADN.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "ADN.h"
#include "afxdialogex.h"

// CArray 정렬템플릿함수
template<class T>
void QSortCArray(T& t, int(__cdecl *compare)(const void *elem1, const void *elem2)){
	if(t.GetSize() <= 0)
		return;

	qsort( t.GetData(), t.GetSize(), sizeof(t[0]), compare);
}

// 내림차순 정렬
int CompareStringDescending(const void *elem1, const void *elem2){
	FileInfos* p1 = (FileInfos*)elem1;
	FileInfos* p2 = (FileInfos*)elem2;

	if(p1->filename > p2->filename)
		return + 1;
	if(p1->filename < p2->filename)
		return - 1;

	return 0;
}
// 오름차순 정렬
int CompareStringAscending(const void *elem1, const void *elem2){
	FileInfos* p1 = (FileInfos*)elem1;
	FileInfos* p2 = (FileInfos*)elem2;

	if(p1->filename > p2->filename)
		return - 1;
	if(p1->filename < p2->filename)
		return + 1;

	return 0;
}

// CADN 대화 상자입니다.
IMPLEMENT_DYNAMIC(CADN, CDialogEx)

CADN::CADN(CWnd* pParent /*=NULL*/)
	: CDialogEx(CADN::IDD, pParent){
}

CADN::~CADN(){
}

void CADN::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CADN, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_COMBO_EXTENSION, &CADN::OnNMClickComboExtension)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COMBO_EXTENSION, &CADN::OnNMCustomdrawComboExtension)
//	ON_NOTIFY(TVN_SELCHANGED, IDC_COMBO_EXTENSION, &CADN::OnTvnSelchangedComboExtension)
ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FILES, &CADN::OnLvnColumnclickListFiles)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILES, &CADN::OnLvnItemchangedListFiles)
END_MESSAGE_MAP()

// CADN 메시지 처리기입니다.
BOOL CADN::OnInitDialog(){
	CDialogEx::OnInitDialog();
	return TRUE;
}

void CADN::OnNMClickComboExtension(NMHDR *pNMHDR, LRESULT *pResult){
	*pResult = 0;
	CPoint point;
	UINT nFlags = 0;
	GetCursorPos(&point);
	::ScreenToClient(m_TreeCtrl.m_hWnd, &point);
	HTREEITEM hItem = m_TreeCtrl.HitTest(point, &nFlags);
	// 아이템이 존재하고 체크박스에 이벤트가 발생했다면
	if(hItem != NULL && (nFlags & TVHT_ONITEMSTATEICON) != 0){
		// 해당 아이템이 체크되어 있다면 
		if(m_TreeCtrl.GetCheck(hItem)){
			CString path;
			path = m_TreeCtrl.ItemToPath(hItem);

			int index = 0;
			for(int i = 0; i < m_MultiFolder.GetCount(); i++){
				CString dest_Folder = m_MultiFolder[i];
				if(dest_Folder.CompareNoCase(path) == 0){
					index = i;
					break;
				}
			}
			if(m_MultiFolder.GetCount() <= 0)
				return;
			m_MultiFolder.RemoveAt(index);
		}
		// 해당 아이템이 체크되어 있지 않다면
		else{
			CString path;
			path = m_TreeCtrl.ItemToPath(hItem);
			bool bisExsit = false;
			for(int i = 0 ; i < m_MultiFolder.GetCount(); i++){
				CString dest_Folder = m_MultiFolder[i];
				if(dest_Folder.CompareNoCase(path) == 0){
					bisExsit = true;
					break;
				}
			}
			if(!bisExsit)
				m_MultiFolder.Add(path);
		}
		FileSearch();
	}
}

void CADN::FileSearch(){
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_EXTENSION))->GetCurSel();
	if(nSel < 0)
		return;

	CString Extend;
	((CComboBox*)GetDlgItem(IDC_COMBO_EXTENSION))->GetLBText(nSel,Extend);
	m_FileList.RemoveAll();
	m_FileView.DeleteAllItems();
	for(int i = 0; i < m_MultiFolder.GetCount(); i++){
		FindFile(m_MultiFolder[i],Extend);
	}

	//정렬하기
	if(((CButton *)GetDlgItem(IDC_SORT1))->GetCheck()){QSortCArray( m_FileList, CompareStringAscending);}
	else if(((CButton *)GetDlgItem(IDC_SORT2))->GetCheck()){QSortCArray( m_FileList, CompareStringDescending);}
	FileViewRefresh();
}

bool CADN::FindFile(CString path, CString extends){
	CFileFind finder;
	BOOL working = finder.FindFile(path + "\\" + extends);
	int i = 0;
	while(working){
		working = finder.FindNextFile();
		if(finder.IsDots())
			continue;

		if(finder.IsDirectory()){
		}   
		else{
			FileInfos fi;
			fi.index = i;
			fi.check = false;
			fi.filename = finder.GetFileName();
			fi.full_path = finder.GetFilePath();
			m_FileList.Add(fi);
			i++;
		}
	}
	return true;
}

//파일 리스트뷰에 데이터 올리기
void CADN::FileViewRefresh(){
	m_FileView.DeleteAllItems();
	for(int i = 0; i < m_FileList.GetCount(); i++){
		CString seq_string; 
		seq_string.Format(_T("%d"), i + 1);
		m_FileView.InsertItem(i, _T(""));
		m_FileView.SetCheck(i, m_FileList[i].check);
		m_FileView.SetItem(i, 1, LVIF_TEXT, seq_string, 0, 0, 0, NULL);
		m_FileView.SetItem(i, 2, LVIF_TEXT, m_FileList[i].filename, 0, 0, 0, NULL);
	}
}

void CADN::OnLvnColumnclickListFiles(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nCnt = m_FileView.GetItemCount();
	if(nCnt <= 0)
		return;

	if(pNMLV->iSubItem == 0){
		if(m_bIsAllCheck){
			for(int i = 0; i < nCnt ; i++){
				m_FileList[i].check = false;
				m_FileView.SetCheck(i, FALSE);
			}
			m_bIsAllCheck = FALSE;
			SetHeaderCheck(FALSE);
		}
		else{
			for(int i = 0; i < nCnt; i++){
				m_FileList[i].check = true;
				m_FileView.SetCheck(i);
			}
			m_bIsAllCheck = TRUE;
			SetHeaderCheck(TRUE);
		}
	}
	*pResult = 0;
}

void CADN::SetHeaderCheck(BOOL bFlag){
	HDITEM hditem;
	hditem.mask = HDI_IMAGE | HDI_FORMAT;
	m_FileView.GetHeaderCtrl()->GetItem(0, &hditem);
	hditem.fmt = HDF_IMAGE | HDF_LEFT;

	if(bFlag)
		hditem.iImage = 1;
	else
		hditem.iImage = 0;

	m_FileView.GetHeaderCtrl()->SetItem(0, &hditem);
}


void CADN::OnLvnItemchangedListFiles(NMHDR *pNMHDR, LRESULT *pResult){
	UpdateData(TRUE);
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->iItem < 0)
		return;

	m_FilePath = m_FileList[pNMLV->iItem].full_path;
	*pResult = 0;
	UpdateData(FALSE);
}