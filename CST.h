#pragma once
#include "IniFile.h"
#include "afxwin.h"

// CCST 대화 상자입니다.
class CCST : public CAcUiDialog{
	DECLARE_DYNAMIC(CCST)
public:
	CCST(CWnd* pParent = NULL);
	virtual ~CCST();
	enum {IDD = IDD_CST};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnUpdateCstEdtdist();
	afx_msg void OnBnClickedCstBselect();
	afx_msg void OnBnClickedCstRadedit();
	afx_msg void OnBnClickedCstRadwrite();
	afx_msg void OnEnUpdateCstEdtheight();
	afx_msg void OnEnUpdateCstEdtfooter();
	afx_msg void OnEnUpdateCstEdtpreface();
	afx_msg void OnBnClickedCstRadnumber();
	afx_msg void OnCbnSelchangeCstCbxdot();
	afx_msg void OnBnClickedCstRadstation();
	afx_msg void OnBnClickedCstHelp();

	CString CheckInputNumber(CString str);
	CString CheckNumber(CString str, int Mode);
	bool CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, int mode);
	
	CIniFile iniFile;
	CComboBox m_cbxDot;
	CAcUiSelectButton m_bSelect;

	int cbxInt;
	int JobMode;
	int OperMode;
	int b_RadBtn1;
	int b_RadBtn2;
	
	double m_Dist;
	double m_Height;
	
	CString Text1_Str;
	CString Text2_Str;
	CString m_DistStr;
	CString cbxInt_Str;
	CString JobMode_Str;
	CString m_HeightStr;
	CString m_FooterStr;
	CString OperMode_Str;
	CString EditText_Str;
	CString m_PrefaceStr;	
};