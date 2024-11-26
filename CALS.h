#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <afxtempl.h>
#include "PrintFormData.h"
#include "WKUtils.h"


// CCALS ��ȭ �����Դϴ�.
struct FileInfos{
	int index;
	CString result;
	CString full_path;
	CString filename;
	//CString plotstyle;
};
struct ExcelValue{
	CString dwgPath;
	CString dwgLayout;
	CString dwgBlkTitle;
};
struct ExcelInfos{
	int index;
	CString Sheet_Name;
	CString Doc_Name;	
	//CString plotstyle;
};
struct StrInfo{	
	CString FileForm;	
};

struct Blocks
{
	AcDbObjectId blkId;	
	CString BlockName;	
	CString AttributeName;
};

class CCALS : public CAcUiDialog
{
	DECLARE_DYNAMIC(CCALS)

public:
	CCALS(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCALS();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CALS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	//��������
	afx_msg void OnBnClickedCalsBtnExport();

	//���� �Ӽ� ���� ������ ��������
	afx_msg void OnBnClickedCalsRadexcel();
	int m_RadProperty_Btn;

	//���� �Ӽ�  ���� ĳ��� ��������
	afx_msg void OnBnClickedCalsRadcad();

	//���� ����
	afx_msg void OnBnClickedCalsRadmodel();
	int m_RadSearch_Btn;
	
	//Edt Ư����ġ
	afx_msg void OnEnUpdateCalsEdtcertain();
	CString m_Certain_Str;
	
	//�˻��� Form �̸�
	afx_msg void OnEnUpdateCalsEdtsearch();
	CString m_SearchForm_Str;
	CArray<StrInfo, StrInfo> m_StrList;

	//�˻��� Form �̸� �����ư
	afx_msg void OnBnClickedCalsBtnextract();
	CAcUiSelectButton m_bSelect;
	Acad::ErrorStatus es;

	//���� ���ļ��� Z
	afx_msg void OnBnClickedCalsRadzsort();
	int m_RadSort_Btn;
	int m_sorttype;

	//��Ʈ�� ���ļ��� Z
	afx_msg void OnBnClickedCalsButton1();	
	CBitmapButton m_btn1;
	
	//���� ���ļ��� N
	afx_msg void OnBnClickedCalsRadnsort();
	afx_msg void OnBnClickedCalsButton2();
	CBitmapButton m_btn2;

	//���� ���ļ��� NM
	afx_msg void OnBnClickedCalsRadnmsort();
	afx_msg void OnBnClickedCalsButton3();
	CBitmapButton m_btn3;

	//���� ���ļ��� ZM
	afx_msg void OnBnClickedCalsRadzmsort();
	afx_msg void OnBnClickedCalsButton4();
	CBitmapButton m_btn4;
	CAcUiSelectButton m_btnExtract;

	//Edt ���Ŀ���
	afx_msg void OnEnUpdateCalsEdterror();
	CString m_SortError_Str;
	int m_SortError;

	//����Ʈ�ڽ�	
	CListCtrl m_FileView;
	CArray<FileInfos, FileInfos> m_FileList;	
	CString m_FilePath, m_FileName, m_blkFilePath;
	int m_LastIdx, m_ListIndex;

	//��ư �����߰�
	afx_msg void OnBnClickedCalsBtnAdddwg();

	//��ư �����߰�
	afx_msg void OnBnClickedCalsBtnAddfolder();
	void FileSearch(CString path);

	//��ư Ȱ������
	afx_msg void OnBnClickedCalsBtnActivedwg();

	//��ư ��������
	afx_msg void OnBnClickedCalsBtnOpendwg();

	//��ư ���ÿ���
	afx_msg void OnBnClickedCalsSelOpen();

	//��ư ���û���
	afx_msg void OnBnClickedCalsSelDel();

	//��ư ��ü����
	afx_msg void OnBnClickedCalsAllDel();

	//��ư ����
	afx_msg void OnBnClickedCalsBtnUp();

	//��ư �Ʒ���
	afx_msg void OnBnClickedCalsBtnDown();

	//��ư �� �Ʒ�
	afx_msg void OnBnClickedCalsBtnBottom();

	//��ư �� ��
	afx_msg void OnBnClickedCalsBtnTop();

	//���� ���� ��
	afx_msg void OnBnClickedCalsSort1();
	int m_ListSort_Btn;

	//���� ���� �Ʒ�
	afx_msg void OnBnClickedCalsSort2();
	
	//�迭 ��ġ ��ȯ
	void RowSwap(int a, int b);

	//����Ʈ ���Ͽ� �迭 ������Ʈ
	void FileListReIndexing();
	BOOL CreateDir(CString dir);

	//���� ����Ʈ�信 ������ �ø���
	void FileViewRefresh();

	//Ȱ��ȭ�� ���� ��������
	void GetActiveDWG();

	//������ ���� �����ִ��� Ȯ���ϱ�
	BOOL IsOpenDocument(CString dwg);
	BOOL IsOpenDocument(CString dwg, AcApDocument *doc);

	//�����ִ� ��� ���� ��������
	void GetOpenDWG();

	//��� ���� ������ Ȯ���� ���� ���ϱ�
	bool FindFile(CString path, CString extends);
				
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void GetLayout(AcDbDatabase* pDb, CArray <AcDbObjectId,AcDbObjectId>& arrLayout);	
	void FindBlock(AcDbObjectId Layout_id,CPrintFormDataArray& m_blockRefIds,CString strName);
	void GetBlockPoint(AcDbObjectId& blockId, double& minpoint_X, double& minpoint_Y,double& maxpoint_X,double& maxpoint_Y, AcGeScale3d& scale,double& angle);
	BOOL CharacterExtraction(CString str);

	CPrintFormDataArray m_blockRefIds, m_PrintRefIds;		//����ڰ� ������ �������� dwg���� ���� ���� �迭
	void ZSort(double tol);							//Z����
	void NSort(double tol);							//N����
	void ZMSort(double tol);							//��Z����
	void NMSort(double tol);							//��N����
	bool FindExcel();								//�������� ����
	bool WriteToExcel();												//���� ���� �� ����Ÿ �Է�	
	bool LoadtoExcel(CString DocName, CString SheetName);	
	BOOL PlotBlockInitExport(AcDbObjectId Layout_id);
	bool RunningExcelData();
	bool UpdateAttBlockData();	//dwg->layout->UpdateBlock
	void UpdateBlock(AcDbObjectId Layout_id,AttBlockArray& m_AttblkArr,AcDbObjectId m_blkID);	//�� ������Ʈ
	void RemoveDuplicates(CStringArray& lst, CStringArray &result);		//���ڿ� �迭 �ߺ�����	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	//�ؽ�Ʈ ���󺯰�
	afx_msg void OnLvnItemchangedCalsListfiles2(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_ExcelView;//�������� ����Ʈ	
	CArray<ExcelInfos, ExcelInfos> m_ExcelList;	
	CString m_DocPath, m_SheetName;
	CArray<ExcelValue, ExcelValue> m_ExcelValueList;	
	
	afx_msg void OnBnClickedCalsBtnImport();				//�������� ��ư	
	BOOL PlotBlockInit(AcDbObjectId Layout_id);	
	AcApDocument* GetOepndDocument(CString strPath);		//��η� ��ť��Ʈ ��������
	CWKUtils utils;
	//CArray<AttBlock,AttBlock> arr;
	AttBlockArray arr,compareArr;						//�� ����Ÿ �迭		
	AttBlockArrayArray arrarr;				//�� ����Ÿ �迭�� �迭
	CStringArray reArr;
	int arrarrcnt;
	CString printBookName;
	CRect initrect, Boxrect;							// �ʱ� ��ȭ���� rect�� ����
	int m_Fold_Width;
	CRect rc;

	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);
	afx_msg void OnBnClickedCalsHelp();
	virtual BOOL DestroyWindow();
};
