#pragma once
#include "afxdialogex.h"
#include "afxwin.h"
#include "PrintFormData.h"
#include "WKUtils.h"

// CEMTT 대화 상자
struct FileInfosEMTT {
	int index;
	CString result;
	CString full_path;
	CString filename;
	//CString plotstyle;
};
struct ExcelValueEMTT {
	CString dwgPath;
	CString dwgLayout;
	CString dwgBlkTitle;
};
struct ExcelInfosEMTT {
	int index;
	CString Sheet_Name;
	CString Doc_Name;
	//CString plotstyle;
};
struct StrInfoEMTT {
	CString FileForm;
};
struct attrEMTT {
	CString EntityName;
	CString tag;
	CString value;
};
struct BlocksEMTT {
	AcDbObjectId blkId;
	CString BlockName;
	CString AttributeName;
};
struct TxTEMTT {
	CString EntityName;
	CString Txt_Tag;
	CString Pos_Value;
	AcGePoint3d AreaMin;		// 문자 범위
	AcGePoint3d AreaMax;		// 문자 범위
	bool FormBlk;
	CString BlockName;
	AcGePoint3d MinVector;		// 폼 - 문자 벡터
	AcGePoint3d MaxVector;		// 폼 - 문자 벡터
};
struct TxTEXCEL {
	CString EntityName;
	CString Txt_Tag;
	CString Pos_Value;
};
struct BlockEMTT {
	CString BlockName;
	double Half_height;		//블록의 높이의 반절 
	AcGePoint3d MinVector;
	AcGePoint3d MaxVector;
	bool b_Att;
	std::vector<CString> v_tag;
	std::vector<CString> v_value;
	//int SaveNum[2];
};

class CEMTT : public CDialogEx
{
	DECLARE_DYNAMIC(CEMTT)

public:
	CEMTT(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEMTT();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMTT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_btn1;
	CBitmapButton m_btn2;
	CBitmapButton m_btn3;
	CBitmapButton m_btn4;

	CButton m_btn_Clear;

	CRect initrect, Boxrect, rc, rt;						// 초기 대화상자 rect값 저장
	int m_Fold_Width;
	int m_RadProperty_Btn;
	int m_RadSearch_Btn;
	int m_RadSort_Btn;
	int m_ListSort_Btn;
	int m_sorttype;
	int m_SortError;
	int m_LastIdx, m_ListIndex;
	int arrarrcnt;

	double viewTime;

	AcGePoint3d m_topleft;
	AcGePoint3d m_rightbottom;
	AcGePoint3d FormArea_minpt;
	AcGePoint3d FormArea_maxpt;
	AcGePoint3d SelectArea_minpt;
	AcGePoint3d SelectArea_maxpt;

	CListCtrl m_FileView;
	CListCtrl m_ExcelView;
	CListCtrl m_TXTView;
	
	CString m_SearchForm_Str;
	CString m_SortError_Str;
	CString m_FilePath, m_FileName, m_blkFilePath;
	CString printBookName;
	CString m_Certain_Str;
	CStringArray reArr;

	CAcUiSelectButton m_bSelect;
	Acad::ErrorStatus es;

	CArray<FileInfosEMTT, FileInfosEMTT> m_FileList;
	CArray<ExcelInfosEMTT, ExcelInfosEMTT> m_ExcelList;
	CArray<ExcelValueEMTT, ExcelValueEMTT> m_ExcelValueList;
	CArray<StrInfoEMTT, StrInfoEMTT> m_StrList;

	CPrintFormDataArray m_blockRefIds, m_PrintRefIds;		//사용자가 선택한 도곽으로 dwg에서 전부 모은 배열
	AttBlockArray arr, compareArr;							//블럭 데이타 배열		
	AttBlockArrayArray arrarr;								//블럭 데이타 배열의 배열

	typedef std::vector<std::vector<TxTEXCEL>> TxTEXCELlst;
	std::vector<TxTEMTT> m_TxtList;
	std::vector<TxTEXCEL> m_TxtExcel;
	TxTEXCELlst m_txtExcelLst;
	std::vector<CString> m_key;
	std::vector<CString> m_value;
	
	CWKUtils utils;

	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	bool RunningExcelData();
	void FileListReIndexing();
	void RowSwap(int a, int b);
	BOOL CreateDir(CString dir);
	void FileViewRefresh();
	void GetLayout(AcDbDatabase* pDb, CArray <AcDbObjectId, AcDbObjectId>& arrLayout);
	void GetBlockPoint(AcDbObjectId& blockId, double& minpoint_X, double& minpoint_Y, double& maxpoint_X, double& maxpoint_Y, AcGeScale3d& scale, double& angle);
	bool FindDWGFile(int index, CString path, bool bcheck);
	void RemoveDuplicates(CStringArray& lst, CStringArray& result);
	AcApDocument* GetOpendDocument(CString strPath);
	BOOL IsOpenDocument(CString dwg);
	BOOL IsOpenDocument(CString dwg, AcApDocument* doc);
	void GetActiveDWG();
	void GetOpenDWG();
	void FindBlock(AcDbObjectId Layout_id, CPrintFormDataArray& m_blockRefIds, CString strName);
	BOOL PlotBlockInitExport(AcDbObjectId Layout_id);
	bool LoadtoExcel(CString DocName, CString SheetName);
	bool WriteToExcel();
	bool FindExcel();
	bool UpdateAttBlockData();
	void UpdateBlock(AcDbObjectId Layout_id, AttBlockArray& m_AttblkArr, AcDbObjectId m_blkID);
	BOOL PlotBlockInit(AcDbObjectId Layout_id);
	BOOL CharacterExtraction(CString str);
	BOOL GetWindow();
	bool getTarget_ViewDir(AcGePoint3d& Tartget, AcGeVector3d& Viewdir);
	void AddBlock(AcDbObjectId& blockid);
	void asdkzoomObj(AcGePoint3d min, AcGePoint3d max);
	void InitZoom(AcGePoint3d min, AcGePoint3d max);
	void GetBlockReferenceTxtList(AcDbObjectId blkid, std::vector<TxTEXCEL>& TxtExcel);

	void ZSort(double tol);								//Z정렬
	void NSort(double tol);								//N정렬
	void ZMSort(double tol);							//역Z정렬
	void NMSort(double tol);							//역N정렬

	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedEmttRadexcel();
	afx_msg void OnBnClickedEmttRadcad();
	afx_msg void OnBnClickedEmttBtnextract();
	afx_msg void OnBnClickedEmttRadmodel();
	afx_msg void OnEnUpdateCalsEdtcertain();
	afx_msg void OnBnClickedEmttRadzsort();
	afx_msg void OnBnClickedEmttRadnsort();
	afx_msg void OnBnClickedEmttRadnmsort();
	afx_msg void OnBnClickedEmttRadzmsort();
	afx_msg void OnBnClickedEmttButton1();
	afx_msg void OnBnClickedEmttButton2();
	afx_msg void OnBnClickedEmttButton3();
	afx_msg void OnBnClickedEmttButton4();
	afx_msg void OnEnUpdateEmttEdterror();
	afx_msg void OnBnClickedEmttBtnAdddwg();
	afx_msg void OnBnClickedEmttBtnAddfolder();
	afx_msg void OnBnClickedEmttBtnActivedwg();
	afx_msg void OnBnClickedEmttBtnOpendwg();
	afx_msg void OnNMDblclkEmttListfiles(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedEmttSelDel();
	afx_msg void OnBnClickedEmttAllDel();
	afx_msg void OnBnClickedEmttSort1();
	afx_msg void OnBnClickedEmttSort2();
	afx_msg void OnBnClickedEmttBtnUp();
	afx_msg void OnBnClickedEmttBtnDown();
	afx_msg void OnBnClickedEmttBtnTop();
	afx_msg void OnBnClickedEmttBtnBottom();
	afx_msg void OnBnClickedEmttBtnImport();
	afx_msg void OnBnClickedEmttTxtAdd();
	afx_msg void OnBnClickedEmttBtnExport();
};
