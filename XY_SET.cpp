// XY_SET.cpp: 구현 파일
//

#include "StdAfx.h"
#include "afxdialogex.h"
#include "XY_SET.h"


// XY_SET 대화 상자

IMPLEMENT_DYNAMIC(XY_SET, CDialogEx)

XY_SET::XY_SET(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XY, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

XY_SET::~XY_SET()
{
}

void XY_SET::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(XY_SET, CDialogEx)
END_MESSAGE_MAP()


// XY_SET 메시지 처리기
