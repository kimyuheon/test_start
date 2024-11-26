#pragma once
#include "resource.h"
#include "FileTreeCtrl.h"

struct FileInfos{
	int index;
	BOOL check;
	CString full_path;
	CString filename;
};

// CADN ��ȭ �����Դϴ�.
class CADN : public CDialogEx{
	DECLARE_DYNAMIC(CADN)

public:
	CADN(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CADN();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ADN };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
	CStringArray m_MultiFolder;								//����ڰ� ��Ƽ ���� 
	CArray<FileInfos, FileInfos> m_FileList;				//���ϸ���Ʈ ����
	CListCtrl m_FileView;									//��Ʈ���Ͻý��� Ʈ��
	BOOL m_bIsAllCheck;										//����ڰ� ���ϸ���Ʈ ��� üũ �ڽ� �����ߴ��� ����
	CString m_FilePath;										//����Ʈ�信�� ������ �ϸ� ������ ��θ� �ѷ��ִ� �ؽ�Ʈ �ڽ�


	afx_msg void OnLvnItemchangedListFiles(NMHDR *pNMHDR, LRESULT *pResult);
};
