#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <afxtempl.h>
#include "PrintFormData.h"
#include "WKUtils.h"


// CCALS 대화 상자입니다.
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
	CCALS(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCALS();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CALS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	//내보내기
	afx_msg void OnBnClickedCalsBtnExport();

	//라디오 속성 정보 엑셀로 내보내기
	afx_msg void OnBnClickedCalsRadexcel();
	int m_RadProperty_Btn;

	//라디오 속성  정보 캐드로 가져오기
	afx_msg void OnBnClickedCalsRadcad();

	//라디오 모형
	afx_msg void OnBnClickedCalsRadmodel();
	int m_RadSearch_Btn;
	
	//Edt 특정배치
	afx_msg void OnEnUpdateCalsEdtcertain();
	CString m_Certain_Str;
	
	//검색할 Form 이름
	afx_msg void OnEnUpdateCalsEdtsearch();
	CString m_SearchForm_Str;
	CArray<StrInfo, StrInfo> m_StrList;

	//검색할 Form 이름 추출버튼
	afx_msg void OnBnClickedCalsBtnextract();
	CAcUiSelectButton m_bSelect;
	Acad::ErrorStatus es;

	//라디오 정렬순서 Z
	afx_msg void OnBnClickedCalsRadzsort();
	int m_RadSort_Btn;
	int m_sorttype;

	//비트맵 정렬순서 Z
	afx_msg void OnBnClickedCalsButton1();	
	CBitmapButton m_btn1;
	
	//라디오 정렬순서 N
	afx_msg void OnBnClickedCalsRadnsort();
	afx_msg void OnBnClickedCalsButton2();
	CBitmapButton m_btn2;

	//라디오 정렬순서 NM
	afx_msg void OnBnClickedCalsRadnmsort();
	afx_msg void OnBnClickedCalsButton3();
	CBitmapButton m_btn3;

	//라디오 정렬순서 ZM
	afx_msg void OnBnClickedCalsRadzmsort();
	afx_msg void OnBnClickedCalsButton4();
	CBitmapButton m_btn4;
	CAcUiSelectButton m_btnExtract;

	//Edt 정렬오차
	afx_msg void OnEnUpdateCalsEdterror();
	CString m_SortError_Str;
	int m_SortError;

	//리스트박스	
	CListCtrl m_FileView;
	CArray<FileInfos, FileInfos> m_FileList;	
	CString m_FilePath, m_FileName, m_blkFilePath;
	int m_LastIdx, m_ListIndex;

	//버튼 도면추가
	afx_msg void OnBnClickedCalsBtnAdddwg();

	//버튼 폴더추가
	afx_msg void OnBnClickedCalsBtnAddfolder();
	void FileSearch(CString path);

	//버튼 활성파일
	afx_msg void OnBnClickedCalsBtnActivedwg();

	//버튼 열린파일
	afx_msg void OnBnClickedCalsBtnOpendwg();

	//버튼 선택열기
	afx_msg void OnBnClickedCalsSelOpen();

	//버튼 선택삭제
	afx_msg void OnBnClickedCalsSelDel();

	//버튼 전체삭제
	afx_msg void OnBnClickedCalsAllDel();

	//버튼 위로
	afx_msg void OnBnClickedCalsBtnUp();

	//버튼 아래로
	afx_msg void OnBnClickedCalsBtnDown();

	//버튼 맨 아래
	afx_msg void OnBnClickedCalsBtnBottom();

	//버튼 맨 위
	afx_msg void OnBnClickedCalsBtnTop();

	//라디오 정렬 위
	afx_msg void OnBnClickedCalsSort1();
	int m_ListSort_Btn;

	//라디오 정렬 아래
	afx_msg void OnBnClickedCalsSort2();
	
	//배열 위치 교환
	void RowSwap(int a, int b);

	//리스트 파일에 배열 업데이트
	void FileListReIndexing();
	BOOL CreateDir(CString dir);

	//파일 리스트뷰에 데이터 올리기
	void FileViewRefresh();

	//활성화된 도면 가져오기
	void GetActiveDWG();

	//도면이 현재 열려있는지 확인하기
	BOOL IsOpenDocument(CString dwg);
	BOOL IsOpenDocument(CString dwg, AcApDocument *doc);

	//열려있는 모든 도면 가져오기
	void GetOpenDWG();

	//경로 내의 지정한 확장자 파일 구하기
	bool FindFile(CString path, CString extends);
				
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void GetLayout(AcDbDatabase* pDb, CArray <AcDbObjectId,AcDbObjectId>& arrLayout);	
	void FindBlock(AcDbObjectId Layout_id,CPrintFormDataArray& m_blockRefIds,CString strName);
	void GetBlockPoint(AcDbObjectId& blockId, double& minpoint_X, double& minpoint_Y,double& maxpoint_X,double& maxpoint_Y, AcGeScale3d& scale,double& angle);
	BOOL CharacterExtraction(CString str);

	CPrintFormDataArray m_blockRefIds, m_PrintRefIds;		//사용자가 선택한 도곽으로 dwg에서 전부 모은 배열
	void ZSort(double tol);							//Z정렬
	void NSort(double tol);							//N정렬
	void ZMSort(double tol);							//역Z정렬
	void NMSort(double tol);							//역N정렬
	bool FindExcel();								//엑셀실행 유무
	bool WriteToExcel();												//엑셀 실행 및 데이타 입력	
	bool LoadtoExcel(CString DocName, CString SheetName);	
	BOOL PlotBlockInitExport(AcDbObjectId Layout_id);
	bool RunningExcelData();
	bool UpdateAttBlockData();	//dwg->layout->UpdateBlock
	void UpdateBlock(AcDbObjectId Layout_id,AttBlockArray& m_AttblkArr,AcDbObjectId m_blkID);	//블럭 업데이트
	void RemoveDuplicates(CStringArray& lst, CStringArray &result);		//문자열 배열 중복삭제	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	//텍스트 색상변경
	afx_msg void OnLvnItemchangedCalsListfiles2(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_ExcelView;//가져오기 리스트	
	CArray<ExcelInfos, ExcelInfos> m_ExcelList;	
	CString m_DocPath, m_SheetName;
	CArray<ExcelValue, ExcelValue> m_ExcelValueList;	
	
	afx_msg void OnBnClickedCalsBtnImport();				//가져오기 버튼	
	BOOL PlotBlockInit(AcDbObjectId Layout_id);	
	AcApDocument* GetOepndDocument(CString strPath);		//경로로 도큐먼트 가져오기
	CWKUtils utils;
	//CArray<AttBlock,AttBlock> arr;
	AttBlockArray arr,compareArr;						//블럭 데이타 배열		
	AttBlockArrayArray arrarr;				//블럭 데이타 배열의 배열
	CStringArray reArr;
	int arrarrcnt;
	CString printBookName;
	CRect initrect, Boxrect;							// 초기 대화상자 rect값 저장
	int m_Fold_Width;
	CRect rc;

	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);
	afx_msg void OnBnClickedCalsHelp();
	virtual BOOL DestroyWindow();
};
