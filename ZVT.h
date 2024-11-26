#pragma once
#include "afxwin.h"
#include "IniFile.h"

// CZVT 대화 상자입니다.
class CZVT : public CAcUiDialog{
	DECLARE_DYNAMIC(CZVT)
public:
	CZVT(CWnd* pParent = NULL);
	virtual ~CZVT();
	enum {IDD = IDD_ZVT};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtext();
	afx_msg void OnBnClickedBpoint();
	afx_msg void OnEnUpdateEdtdist();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnUpdateEdttextwidht();
	afx_msg void OnEnUpdateEdttextheight();

	CString CheckInputNumber(CString str);
	AcDbObjectId findTextStyle(CString name);
	
	CIniFile iniFile;
	CComboBox m_cbxTextLayer;
	CComboBox m_cbxTextStyle;
	CComboBox m_cbxPointLayer;
	CAcUiSelectButton m_TextSelectButton;
	CAcUiSelectButton m_PointSelectButton;
	
	CEdit m_textDistCtrl;
	CEdit m_textWidthCtrl;
	CEdit m_textHeightCtrl;

	CString m_textDistStr;
	CString m_textWidthStr;
	CString m_textHeightStr;
	afx_msg void OnBnClickedZvtHelp();
};