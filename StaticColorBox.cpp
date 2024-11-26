// D:\DCS_CAD_Prg\CTool\CT_Modify\Common\Mfc\StaticColorBox.cpp : 구현 파일입니다.
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



// CStaticColorBox 메시지 처리기입니다.



void CStaticColorBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.
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
