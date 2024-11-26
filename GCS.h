#pragma once
#include "IniFile.h"
#include "afxwin.h"

// CGCS ��ȭ �����Դϴ�.

class CGCS : public CAcUiDialog
{
	DECLARE_DYNAMIC(CGCS)

public:
	CGCS(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGCS();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GCS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CIniFile iniFile;

	int JobMode;
	CString JobMode_Str;
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//���� ǥ�� STA
	afx_msg void OnBnClickedGcsRadstation();
	int b_RadBtn;
	//���� ǥ�� NO
	afx_msg void OnBnClickedGcsRadnumber();

	//�޺��ڽ� �Ҽ���
	afx_msg void OnCbnSelchangeGcsCbxdot();
	CComboBox m_cbxDot;
	CString cbxInt_Str, m_DotStr;
	int cbxInt, m_Dot;
	
	//���� ����
	afx_msg void OnEnUpdateGcsEdtdist();
	CString m_DistStr;
	double m_Dist;
	//üũ�ڽ� ���� ���� ���鿡�� ����
	afx_msg void OnBnClickedGcsCheck1();
	bool b_Chk1;
	CButton m_Chk1_btn;
	CString Chk_ApptStr;
	//���� ����
	afx_msg void OnEnUpdateGcsEdtrefroad();
	CString m_RefroadStr;
	double m_Refroad;
	//���� ����
	afx_msg void OnEnUpdateGcsEdtcorroad();
	CString m_CorroadStr;
	double m_Corroad;
	//üũ�ڽ� �̰� �Ÿ� ǥ��
	afx_msg void OnBnClickedGcsCheck2();
	bool b_Chk2;
	CButton m_Chk2_btn;
	CString Chk_NotationStr;
	//������ ���� ���� ����
	double DTR(double angle);
	void OpenExcel(AcDbObjectIdArray arr);
	CString CheckInputNumber(CString str, int Mode);
	AcDbObjectId CreateLine(AcGePoint3d spt,AcGePoint3d ept);
	void EraseEntity(AcDbObjectId objId);
	bool FindExcel();
	void ImpressExcel();

	HANDLE hProcess;
	AcDbObjectId objId_ref, objId_cor;
	ads_name RefEns,CorEns;
	AcGePoint3d CorSpt,CorEpt,RefSpt,RefEpt;
	AcDbObjectIdArray ObjIdArray;
	CString DirYN_Str_cor, DirYN_Str_ref;//�������
	double DirSurvey_corF, DirSurvey_refF;//���鿡�� ���� ����
	AcGePoint3d DirPt_cor,DirPt_ref;//���鿡�� ���� ������


	typedef struct CellAtt {
		bool IsMerge;
		double IsWidht;
		double IsHeight;
		double IsMergeWidth;
		double isMergeHeight;
		long IsRow;
		long IsCol;
		CString IsRange;
		CString IsAddress;
		CString IsString;
		int IsLeftLine;
		int IsUpLine;
		int IsRightLine;
		int IsDownLine;
		double IsStrSize;
		int IsVertical;
		int IsHorizon;
	}CellAtt;

	typedef std::vector<CellAtt> CellAttArray;
	typedef std::vector<CellAttArray> CellAttTable;

	afx_msg void OnBnClickedGcsHelp();
};
