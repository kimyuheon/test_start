#pragma once
#include "afxwin.h"
#include "resource.h"
#include "inetstrc.h"
#include "IniFile.h"

// WCZ 대화 상자입니다.
class CWCZ : public CAcUiDialog{
	DECLARE_DYNAMIC(CWCZ)
public:
	CWCZ(CWnd* pParent = NULL);
	virtual ~CWCZ();
	enum {IDD = IDD_WCZ};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedTextsel();

	void initLayer();
	void initStyle();
	CString findTextStyle(AcDbObjectId StyleId);
	AcDbObjectId findTextStyleName(CString StyleId);
	
	CIniFile iniFile;
	CAcUiSelectButton m_selectBtn;

	CComboBox CLayer;
	CComboBox CStyle;

	afx_msg void OnBnClickedWczHelp();
};