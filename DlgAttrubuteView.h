#pragma once
#include "StdAfx.h"
#include "resource.h"
#include "afxcmn.h"

// CDlgAttrubuteView ��ȭ �����Դϴ�.

class CDlgAttrubuteView : public CDialog
{
	DECLARE_DYNAMIC(CDlgAttrubuteView)

public:
	CDlgAttrubuteView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgAttrubuteView();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ATTRIBUTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
