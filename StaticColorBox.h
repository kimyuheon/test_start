#pragma once

#include "StdAfx.h"

// CStaticColorBox

class CStaticColorBox : public CStatic
{
	DECLARE_DYNAMIC(CStaticColorBox)

public:
	CStaticColorBox();
	virtual ~CStaticColorBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	COLORREF m_colorRef;
	void SetColor(Adesk::UInt32 color);
};


