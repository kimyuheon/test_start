#pragma once
#include "afxwin.h"
#include "IniFile.h"

// CTTP 대화 상자입니다.
class CTTP : public CAcUiDialog{
	DECLARE_DYNAMIC(CTTP)
public:
	CTTP(CWnd* pParent = NULL);
	virtual ~CTTP();
	enum {IDD = IDD_TTP};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBselect();
	afx_msg void OnEnUpdateEdtxoffset();
	afx_msg void OnEnUpdateEdtyoffset();
	afx_msg void OnBnClickedRadalignpoint();
	afx_msg void OnBnClickedRadinsertpoint();
	
	void CreatePoint(CString zValue, AcGePoint3d pos);
	CString CheckInputNumber(CString str);
	
	CIniFile iniFile;
	CEdit m_xValueCtrl;
	CEdit m_yValueCtrl;
	CAcUiComboBox m_cbxLayer;
	CAcUiSelectButton m_bSelect;

	int mode;
	
	CString m_xValueStr;
	CString m_yValueStr;	
	afx_msg void OnBnClickedTtpHelp();
};