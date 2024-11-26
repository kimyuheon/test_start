#pragma once
#include "resource.h"
#include "FileTreeCtrl.h"

struct FileInfos{
	int index;
	BOOL check;
	CString full_path;
	CString filename;
};

// CADN 대화 상자입니다.
class CADN : public CDialogEx{
	DECLARE_DYNAMIC(CADN)

public:
	CADN(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CADN();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADN };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickComboExtension(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListFiles(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	void FileSearch();
	bool FindFile(CString path, CString extends);
	void FileViewRefresh();
	void SetHeaderCheck( BOOL bFlag);
	
	CFileTreeCtrl m_TreeCtrl;
	CStringArray m_MultiFolder;								//사용자가 멀티 폴더 
	CArray<FileInfos, FileInfos> m_FileList;				//파일리스트 저장
	CListCtrl m_FileView;									//루트파일시스템 트리
	BOOL m_bIsAllCheck;										//사용자가 파일리스트 헤더 체크 박스 선택했는지 여부
	CString m_FilePath;										//리스트뷰에서 선택을 하면 파일의 경로를 뿌려주는 텍스트 박스


	afx_msg void OnLvnItemchangedListFiles(NMHDR *pNMHDR, LRESULT *pResult);
};
