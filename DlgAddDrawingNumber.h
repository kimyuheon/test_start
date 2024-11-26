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

// CDlgAddDrawingNumber ��ȭ �����Դϴ�.
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
	CDlgAddDrawingNumber(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgAddDrawingNumber();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ADN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	LRESULT onAcadKeepFocus(WPARAM, LPARAM);
	// ������ ��� 
	int m_WriteTarget;
	// ���� ��ȣ �� ���
	CWKComboBox m_FormTarget;
	
	// �� �̸�
	CString m_FormOption_FormName;
	// �� �ɼ� ���� ��ġ
	CString m_FormOption_FormLocation;
	// ���� ��ȣ, ���� ����, ���� �̸� ���� ��ư 
	int m_InsertText_Target;
	// �����ȣ ���λ�
	CString m_strDwgName_Prefix;
	// �����ȣ ���ۼ���
	int m_iDwgNumber_Start;
	// ���Թ���
	CString m_strDwgInstartText;
	// ���Թ��� ���� ����
	BOOL m_bInsertTextIncrease;
	// ���Թ��� ���� ���� ���� ����
	int m_InsertText_StartNumber;
	// ���Թ��� �ڸ���
	int m_iInsertTextDigit;
	// �̸�����
	CString m_preview;
	// ���� ���� �̸� ����
	BOOL m_bDwgNameWrite;
	int m_iSort;
	// ���Ŀ���
	double m_dblSortTolerance;
	// ���ڿɼ� �۲�
	CWKComboBox m_cbTextOption_Font;
	// ���ڿɼ� ���̾�
	CWKComboBox m_cbTextOption_Layer;
	// ���� �ɼ� ����
	double m_strTextOption_Height;
	// ���ڿɼ� ��
	double m_strTextOption_Width;
	// ���ڿɼ� ����
	CWKComboBox m_cbTextOption_Angle;
	// ���ڿɼ� ���� ���� ����
	BOOL m_bOldTextRemove;
	// ���������̸����� ������
	CString m_strDwgFileName_Location;
	// ���������̸����� �۲�
	CWKComboBox m_cbDwgFileName_Font;
	// ���������̸����� ���̾�
	CWKComboBox m_cbDwgFileName_Layer;
	// ���������̸����� ����
	double m_dblDwgFileName_Height;
	// ���������̸� ���� ��
	double m_dblDwgFileName_Width;
	// ���������̸����� ����
	CWKComboBox m_cbDwgFileName_Sort;
	// ���������̸����� ����
	CWKComboBox m_cbDwgFileName_Angle;
	// ���������̸����� ��ü��η� ����
	BOOL m_bDwgFileName_Fullpath;
	// ���������̸����� Ȯ��������
	BOOL m_bDwgFileName_IncExt;
	// ���������̸����� �ʵ�� ����
	BOOL m_bDwgFileName_UseField;
	afx_msg void OnBnClickedRadioFormOption();
	CListCtrl m_FileView;													//��Ʈ���Ͻý��� Ʈ����
	// �� �ɼ� Xref, �Ӽ���� �߿� ����
	int m_formOption;
	//Į��
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
	CArray<FileInfos_, FileInfos_> m_FileList;								//���ϸ���Ʈ ����;
	//���� ��ȣ ���� ��ġ 
	AcDbExtents m_FormNumberLocation;
	//���ϸ� ���� ��ġ
	AcGeVector3d m_DwgFileNameWriteLocation;
	void AddBlock(AcDbObjectId& blockid);
	Block newBlock;
	
	afx_msg void OnBnClickedButtonFormoptionInsertPoint();
	void GetLayout(AcDbDatabase* pDb, CArray <AcDbObjectId,AcDbObjectId>& arrLayout);
	void  gogo();
	AcApDocument* GetOepndDocument(CString strPath);
	BOOL PlotBlockInit(AcDbObjectId Layout_id);

	BOOL m_bIsUserSelectForm;
	void ZSort(double tol);													//Z ���� ��Ʈ
	void NSort(double tol);													//N ���� ��Ʈ
	void ZMSort(double tol);												//Z ���� ���� ��Ʈ
	void NMSort(double tol);												//N ���� ���� ��Ʈ
	CPrintFormDataArray_ m_blockRefIds;										//����ڰ� ������ �������� dwg���� ���� ���� �迭
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
	CRect CADrect, Boxrect;							// CAD rect / ���� ��� rect
	int m_Fold_Width;
	virtual BOOL DestroyWindow();

	//virtual void PostNcDestroy();
	//afx_msg void OnDestroy();
	//afx_msg void OnClose();

	

	
};
