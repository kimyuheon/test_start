#pragma once
#include "resource.h"
#include "afxwin.h"
#include "IniFile.h"

// CDCW 대화 상자입니다.

class CDCW : public CAcUiDialog
{
	DECLARE_DYNAMIC(CDCW)

public:
	CDCW(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDCW();

	void initLayer();
	void makeDCW();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DCW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
