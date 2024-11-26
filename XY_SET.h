#pragma once
#include "afxdialogex.h"


// XY_SET 대화 상자

class XY_SET : public CDialogEx
{
	DECLARE_DYNAMIC(XY_SET)

public:
	XY_SET(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~XY_SET();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
