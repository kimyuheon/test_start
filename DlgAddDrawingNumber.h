#pragma once
#include "resource.h"
#include "StdAfx.h"
#include "afxwin.h"

#include "CRange.h"
#include "CRanges.h"
#include "CApplication.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "WkcomboBox.h"

// CDlgAddDrawingNumber 대화 상자입니다.
struct FileInfos_{
	int index;
	CString full_path;
	CString filename;
	CString result;
};

struct Block
{
	AcDbObjectId blkId;
	AcGeScale3d scale;
	double angle;
	AcDbExtents FileNamArea;
	AcDbExtents FormArea;
	CString BlockName;
	AcGePoint3d File_MinVector;
	AcGePoint3d File_MaxVector;
	AcGePoint3d File_NamePoint;
	AcGeVector3d File_NameVector;
	CString AttributeName;

};
class CPrintFormData_
{
public:
	CPrintFormData_(void){};
	virtual ~CPrintFormData_(void){};
	AcDbObjectId FormId;
	double minpoint_X;
	double minpoint_Y;
	double maxpoint_X;
	double maxpoint_Y;

	double file_minpoint_X;
	double file_minpoint_Y;
	double file_maxpoint_X;
	double file_maxpoint_Y;
	AcGeScale3d scale;
	double angle;

};
class CPrintFormDataArray_ : public CArray<CPrintFormData_, CPrintFormData_&> {
public:
	CPrintFormDataArray_() : CArray<CPrintFormData_, CPrintFormData_&>() {};

	CPrintFormDataArray_(const CPrintFormDataArray_ &PrintFormDataArray) {
		RemoveAll();
		Append(PrintFormDataArray);	
	}

	void operator= (const CPrintFormDataArray_ &PrintFormDataArray) {
		RemoveAll();
		Append(PrintFormDataArray);	
	}
};

class CDlgAddDrawingNumber : public CAcUiDialog
{
	DECLARE_DYNAMIC(CDlgAddDrawingNumber)

public:
	CDlgAddDrawingNumber(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAddDrawingNumber();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	LRESULT onAcadKeepFocus(WPARAM, LPARAM);
	// 쓰기할 대상 
	int m_WriteTarget;
	// 폼에 번호 쓸 대상
	CWKComboBox m_FormTarget;
	
	// 폼 이름
	CString m_FormOption_FormName;
	// 폼 옵션 삽입 위치
	CString m_FormOption_FormLocation;
	// 도면 번호, 삽입 문자, 파일 이름 라디오 버튼 
	int m_InsertText_Target;
	// 도면번호 접두사
	CString m_strDwgName_Prefix;
	// 도면번호 시작숫자
	int m_iDwgNumber_Start;
	// 삽입문자
	CString m_strDwgInstartText;
	// 삽입문자 숫자 증가
	BOOL m_bInsertTextIncrease;
	// 삽입문자 숫자 증가 시작 숫자
	int m_InsertText_StartNumber;
	// 삽입문자 자릿수
	int m_iInsertTextDigit;
	// 미리보기
	CString m_preview;
	// 도면 파일 이름 쓰기
	BOOL m_bDwgNameWrite;
	int m_iSort;
	// 정렬오차
	double m_dblSortTolerance;
	// 문자옵션 글꼴
	CWKComboBox m_cbTextOption_Font;
	// 문자옵션 레이어
	CWKComboBox m_cbTextOption_Layer;
	// 문자 옵션 높이
	double m_strTextOption_Height;
	// 문자옵션 폭
	double m_strTextOption_Width;
	// 문자옵션 각도
	CWKComboBox m_cbTextOption_Angle;
	// 문자옵션 기존 문자 삭제
	BOOL m_bOldTextRemove;
	// 도면파일이름쓰기 삽입점
	CString m_strDwgFileName_Location;
	// 도면파일이름쓰기 글꼴
	CWKComboBox m_cbDwgFileName_Font;
	// 도면파일이름쓰기 레이어
	CWKComboBox m_cbDwgFileName_Layer;
	// 도면파일이름쓰기 높이
	double m_dblDwgFileName_Height;
	// 도면파일이름 쓰기 폭
	double m_dblDwgFileName_Width;
	// 도면파일이름쓰기 정렬
	CWKComboBox m_cbDwgFileName_Sort;
	// 도면파일이름쓰기 각도
	CWKComboBox m_cbDwgFileName_Angle;
	// 도면파일이름쓰기 전체경로로 쓰기
	BOOL m_bDwgFileName_Fullpath;
	// 도면파일이름쓰기 확장자포함
	BOOL m_bDwgFileName_IncExt;
	// 도면파일이름쓰기 필드로 삽입
	BOOL m_bDwgFileName_UseField;
	afx_msg void OnBnClickedRadioFormOption();
	CListCtrl m_FileView;													//루트파일시스템 트리뷰
	// 폼 옵션 Xref, 속성블록 중에 택일
	int m_formOption;
	//칼라
	int m_Color;
	int m_Color1;
	CRect MainForm_Size;

	void ColorSetting(int newColorIndex,CStatic& ColorBox,int colorNameCrtl);
	CStatic m_Btn_Color;
	afx_msg void OnBnClickedButton5();
	CStatic m_Btn_Color2;
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedRadioWriteType();
	afx_msg void OnDeltaposSpinDigit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckWriteDwgName();
	void DlgChangeSize();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioTarget();
	afx_msg void OnBnClickedButtonTextOptionFont();
	afx_msg void OnBnClickedButtonTextOptionLayer();
	afx_msg void OnBnClickedButtonWriteDwgNameFont();
	afx_msg void OnBnClickedButtonWriteDwgNameLayer();
	afx_msg void OnEnChangeEditDwgNumberPrefix();
	afx_msg void OnEnChangeEditDwgStartNumber();
	void makeDwgNumber();
	afx_msg void OnEnChangeEditIncreaseDigit();
	afx_msg void OnEnChangeEditIncreaseStartNumber();
	afx_msg void OnEnChangeEditInsertText();
	afx_msg void OnBnClickedButtonFormoptionFormName();
	CArray<FileInfos_, FileInfos_> m_FileList;								//파일리스트 저장;
	//도면 번호 저장 위치 
	AcDbExtents m_FormNumberLocation;
	//파일명 쓰기 위치
	AcGeVector3d m_DwgFileNameWriteLocation;
	void AddBlock(AcDbObjectId& blockid);
	Block newBlock;
	
	afx_msg void OnBnClickedButtonFormoptionInsertPoint();
	void GetLayout(AcDbDatabase* pDb, CArray <AcDbObjectId,AcDbObjectId>& arrLayout);
	void  gogo();
	AcApDocument* GetOepndDocument(CString strPath);
	BOOL PlotBlockInit(AcDbObjectId Layout_id);

	BOOL m_bIsUserSelectForm;
	void ZSort(double tol);													//Z 방향 소트
	void NSort(double tol);													//N 방향 소트
	void ZMSort(double tol);												//Z 반전 방향 소트
	void NMSort(double tol);												//N 반전 방향 소트
	CPrintFormDataArray_ m_blockRefIds;										//사용자가 선택한 도곽으로 dwg에서 전부 모은 배열
	BOOL UserSelectBlock(CPrintFormDataArray_& m_blockRefIds);
	void ss2Array(ads_name ss, CPrintFormDataArray_& objIds);
	BOOL enameToObjId(ads_name ent, AcDbObjectId& objId);
	void  FindBlock(AcDbObjectId Layout_id,CPrintFormDataArray_& m_blockRefIds,CString strName);
	void GetLayoutBrefs(AcDbObjectId idLayout,CString strBlkName, AcDbObjectIdArray& ids);
	void GetBlockPoint(AcDbObjectId& blockId, double& minpoint_X, double& minpoint_Y,double& maxpoint_X,double& maxpoint_Y, AcGeScale3d& scale,double& angle);
	afx_msg void OnBnClickedButtonGo1();
	afx_msg void OnBnClickedButtonGo2();
	void AddBlock();
	afx_msg void OnBnClickedButtonWriteDwgNamePoint();
	void etoc(CStringArray& arr);
	void WriteTextModule(AcDbObjectId Layout_id);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonAddActiveDwg();

	void GetActiveDWG();
	void GetOpenDWG();
	void FileListReIndexing();
	BOOL IsOpenDocument(CString dwg, AcApDocument *doc);
	BOOL IsOpenDocument(CString dwg);
	void RowSwap(int a, int b);
	void FileViewRefresh();
	void FileSearch(CString path);
	bool FindFile(CString path, CString extends);
	void InitLitView();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonAddOpenDwg();
	afx_msg void OnBnClickedButtontTop();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonBottom();
	afx_msg void OnBnClickedButtonSelRemove();
	afx_msg void OnBnClickedButtonAllRemove();
	
	afx_msg void OnBnClickedButtonAddDwg();
	afx_msg void OnBnClickedButtonAddFolder();
	afx_msg void OnBnClickedButtonOpen();
	
	afx_msg void OnBnClickedButtonListSave();
	afx_msg void OnBnClickedButtonListOpen();
	afx_msg void OnBnClickedButtonListSort();
	afx_msg void OnBnClickedButtonExit();
	
	afx_msg void OnSortClick1();
	afx_msg void OnSortClick2();
	afx_msg void OnSortClick3();
	afx_msg void OnSortClick4();

	// sort button
	CButton* BtSrot1;
	CButton* BtSrot2;
	CButton* BtSrot3;
	CButton* BtSrot4;
	CRect CADrect, Boxrect;							// CAD rect / 접기 대상 rect
	int m_Fold_Width;
	virtual BOOL DestroyWindow();

	//virtual void PostNcDestroy();
	//afx_msg void OnDestroy();
	//afx_msg void OnClose();

	

	
};
