#pragma once
#include "resource.h"
#include "afxwin.h"
#include "IniFile.h"

// CDCW ��ȭ �����Դϴ�.

class CDCW : public CAcUiDialog
{
	DECLARE_DYNAMIC(CDCW)

public:
	CDCW(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDCW();

	void initLayer();
	void makeDCW();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DCW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedChkarrow();
	afx_msg void OnBnClickedSellayer();

	CComboBox CLayer;
	CIniFile iniFile;
	CAcUiSelectButton m_selectBtn;
	afx_msg void OnBnClickedDcwHelp();
};
