// MyComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "WkcomboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWKComboBox

CWKComboBox::CWKComboBox()
{
}

CWKComboBox::~CWKComboBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

void CWKComboBox::RecalcDropWidth()
{
    // Reset the dropped width
    int nNumEntries = GetCount();
    int nWidth = 0;
    CString str;

    CClientDC dc(this);
    int nSave = dc.SaveDC();
    dc.SelectObject(GetFont());

    int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
    for (int i = 0; i < nNumEntries; i++)
    {
        GetLBText(i, str);
        int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
        nWidth = max(nWidth, nLength);
    }
    
    // Add margin space to the calculations
    nWidth += dc.GetTextExtent("0").cx;

    dc.RestoreDC(nSave);
    SetDroppedWidth(nWidth);
}

BEGIN_MESSAGE_MAP(CWKComboBox, CComboBox)
	//{{AFX_MSG_MAP(CWKComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWKComboBox message handlers

void CWKComboBox::OnDropdown() 
{
    RecalcDropWidth();
}
