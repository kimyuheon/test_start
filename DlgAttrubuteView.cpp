// DlgAttrubuteView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgAttrubuteView.h"
#include "afxdialogex.h"


// CDlgAttrubuteView ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgAttrubuteView, CDialog)

CDlgAttrubuteView::CDlgAttrubuteView(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAttrubuteView::IDD, pParent)
	, m_strMSG(_T(""))
	, m_strTAG(_T(""))
	
{

}

CDlgAttrubuteView::~CDlgAttrubuteView()
{
}

void CDlgAttrubuteView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_MSG, m_strMSG);
	DDX_Text(pDX, IDC_STATIC_SELECT_TAG, m_strTAG);
	DDX_Control(pDX, IDC_LIST_ATT, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDlgAttrubuteView, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ATT, &CDlgAttrubuteView::OnLvnItemchangedListAtt)
END_MESSAGE_MAP()


// CDlgAttrubuteView �޽��� ó�����Դϴ�.


BOOL CDlgAttrubuteView::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rc;
	m_ListCtrl.GetClientRect(&rc);
	//ScreenToClient(&rc);
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0,TEXT("�±�"),LVCFMT_LEFT,rc.Width()/2);
	m_ListCtrl.InsertColumn(1,TEXT("��"),LVCFMT_LEFT,rc.Width()/2);

	for(int i=0; i< arr->GetCount(); i++)
	{
		m_ListCtrl.InsertItem(i,arr->GetAt(i).AttributeTag);
		m_ListCtrl.SetItem(i,1,LVIF_TEXT,arr->GetAt(i).strValue,0,0,0,NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDlgAttrubuteView::SetTagList(CArray<AttBlock,AttBlock>& arr)
{
	this->arr = &arr;
}

void CDlgAttrubuteView::OnLvnItemchangedListAtt(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData();
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (!(pNMLV->uChanged == LVIF_STATE && pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED)))
		return;

	INT nSelectItem = pNMLV->iItem;
	if (-1 == nSelectItem)
		return;


		CString sLogName = _T("");
		sLogName = m_ListCtrl.GetItemText(nSelectItem, 0);
		m_strTAG = sLogName;
		UpdateData(FALSE);

	*pResult = 0;
}
