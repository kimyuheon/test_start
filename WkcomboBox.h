#if !defined(AFX_MYCOMBOBOX_H__5E3BB93E_0385_11D4_A2E3_0048543D92F7__INCLUDED_)
#define AFX_MYCOMBOBOX_H__5E3BB93E_0385_11D4_A2E3_0048543D92F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWKComboBox window

class CWKComboBox : public CComboBox
{
// Construction
public:
	CWKComboBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWKComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWKComboBox();

protected:
    void RecalcDropWidth();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWKComboBox)
	afx_msg void OnDropdown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCOMBOBOX_H__5E3BB93E_0385_11D4_A2E3_0048543D92F7__INCLUDED_)
