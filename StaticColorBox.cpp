// D:\DCS_CAD_Prg\CTool\CT_Modify\Common\Mfc\StaticColorBox.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "StaticColorBox.h"


// CStaticColorBox

IMPLEMENT_DYNAMIC(CStaticColorBox, CStatic)

CStaticColorBox::CStaticColorBox()
{

}

CStaticColorBox::~CStaticColorBox()
{
}


BEGIN_MESSAGE_MAP(CStaticColorBox, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CStaticColorBox �޽��� ó�����Դϴ�.



void CStaticColorBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CStatic::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rc1;
	this->GetClientRect(rc1);
	CBrush brush1;
	brush1.CreateSolidBrush(m_colorRef);  
	dc.FillRect(rc1, &brush1);
}

void CStaticColorBox::SetColor(Adesk::UInt32 color)
{
	Adesk::UInt32 acirgb = acedGetRGB(color);
	m_colorRef = RGB(GetRValue(acirgb), GetGValue(acirgb), GetBValue(acirgb));
	Invalidate();
}
