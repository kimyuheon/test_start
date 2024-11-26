#pragma once
#include "IniFile.h"
#include "resource.h"

// CZVL 대화 상자입니다.
class CZVL : public CAcUiDialog {
	DECLARE_DYNAMIC(CZVL)
public:
	CZVL(CWnd* pParent = NULL);
	virtual ~CZVL();
	enum {IDD = IDD_ZVL};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadline();
	afx_msg void OnBnClickedRadpline();
	afx_msg void OnBnClickedRad3Dpline();

	CIniFile iniFile;

	double zValue;

	CString m_Mode;
	afx_msg void OnBnClickedZvlHelp();
};