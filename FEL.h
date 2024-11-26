#pragma once
#include "afxwin.h"
#include "IniFile.h"
#include "Arx_Math.h"

// CFEL 대화 상자입니다.
class CFEL : public CAcUiDialog {
	DECLARE_DYNAMIC(CFEL)
public:
	CFEL(CWnd* pParent = NULL);
	virtual ~CFEL();
	enum {IDD = IDD_FEL};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnUpdateEdtlayer();
	afx_msg void OnBnClickedRadline();
	afx_msg void OnEnUpdateEdtscale();
	afx_msg void OnEnUpdateEdtpoint();
	afx_msg void OnEnUpdateEdtheight();
	afx_msg void OnBnClickedRadarrow();
	afx_msg void OnEnUpdateEdtlength();
	afx_msg void OnEnUpdateEdtpreface();
	
	void pyosi(ads_point pt1);
	void ads_arxpt(AcGePoint3d& ps,ads_point pe);
	
	bool CheckCStringDigit(CString str);
	bool CreateSolid(AcGePoint3d Solpt1, AcGePoint3d Solpt2, AcGePoint3d Solpt3, CString Layer);
	bool CreateText(AcGePoint3d pt, double Height, double Rotate, CString TxtStr, CString Layer, int mode);
	bool CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer);

	double DTR(double angle);
	double RTD(double radian);
	
	CString CheckInputNumber(CString str);
	
	CIniFile iniFile;
	CButton m_btn1, m_btn2;
	CEdit m_TextHeight_Ctrl;
	CAcUiSelectButton m_bSelect;

	CComboBox m_CbxLayer;
	
	int Scale;
	int Length;
	int DotCnt;
	int lineStat;

	CString m_LayerStr;
	CString m_ScaleStr;
	CString m_LengthStr;
	CString m_DotCntStr;	
	CString m_PreFaceStr;
	CString m_ELValue_str;
	CString m_TextHeight_Str;
	afx_msg void OnBnClickedFelHelp();
};