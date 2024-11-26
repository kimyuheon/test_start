#pragma once
#include "StdAfx.h"
#include "resource.h"
#include "afxcmn.h"

// CDlgAttrubuteView 대화 상자입니다.

class CDlgAttrubuteView : public CDialog
{
	DECLARE_DYNAMIC(CDlgAttrubuteView)

public:
	CDlgAttrubuteView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAttrubuteView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ATTRIBUTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strMSG;
	CString m_strTAG;
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
	CArray<AttBlock,AttBlock>* arr;
	void SetTagList(CArray<AttBlock,AttBlock>& arr);
	afx_msg void OnLvnItemchangedListAtt(NMHDR *pNMHDR, LRESULT *pResult);
};
