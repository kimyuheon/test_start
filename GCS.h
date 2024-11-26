#pragma once
#include "IniFile.h"
#include "afxwin.h"

// CGCS 대화 상자입니다.

class CGCS : public CAcUiDialog
{
	DECLARE_DYNAMIC(CGCS)

public:
	CGCS(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGCS();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GCS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CIniFile iniFile;

	int JobMode;
	CString JobMode_Str;
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//측점 표기 STA
	afx_msg void OnBnClickedGcsRadstation();
	int b_RadBtn;
	//측점 표기 NO
	afx_msg void OnBnClickedGcsRadnumber();

	//콤보박스 소수점
	afx_msg void OnCbnSelchangeGcsCbxdot();
	CComboBox m_cbxDot;
	CString cbxInt_Str, m_DotStr;
	int cbxInt, m_Dot;
	
	//측점 간격
	afx_msg void OnEnUpdateGcsEdtdist();
	CString m_DistStr;
	double m_Dist;
	//체크박스 시작 측점 도면에서 지정
	afx_msg void OnBnClickedGcsCheck1();
	bool b_Chk1;
	CButton m_Chk1_btn;
	CString Chk_ApptStr;
	//기준 도로
	afx_msg void OnEnUpdateGcsEdtrefroad();
	CString m_RefroadStr;
	double m_Refroad;
	//대응 도로
	afx_msg void OnEnUpdateGcsEdtcorroad();
	CString m_CorroadStr;
	double m_Corroad;
	//체크박스 이격 거리 표기
	afx_msg void OnBnClickedGcsCheck2();
	bool b_Chk2;
	CButton m_Chk2_btn;
	CString Chk_NotationStr;
	//각도를 라디안 으로 변경
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
	CString DirYN_Str_cor, DirYN_Str_ref;//진행방향
	double DirSurvey_corF, DirSurvey_refF;//도면에서 지정 측점
	AcGePoint3d DirPt_cor,DirPt_ref;//도면에서 지정 시작점


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
