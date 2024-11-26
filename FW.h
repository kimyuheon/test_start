#pragma once
#include "IniFile.h"
#include "afxwin.h"
// CFW 대화 상자입니다.

class CFW : public CAcUiDialog
{
	DECLARE_DYNAMIC(CFW)

public:
	CFW(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFW();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FW };

	CButton m_Radio1, m_Radio2, m_Radio3, m_Radio4, m_Radio5;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFwform1();
	afx_msg void OnBnClickedFwform2();
	afx_msg void OnBnClickedFwform3();
	afx_msg void OnBnClickedFwform4();
	afx_msg void OnBnClickedFwform5();

	CIniFile iniFile;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnEnUpdateFwlinelayer();
	CString m_LineLayerStr;

	afx_msg void OnEnUpdateFwtxtlayer();
	CString m_TxtLayerStr;

	afx_msg void OnEnUpdateFwtxtheight();
	CString m_TxtHeightStr;
	int Height;

	afx_msg void OnEnUpdateFwtxtwidth();
	CString m_TxtWidthStr;
	int Width;

	afx_msg void OnEnUpdateFwldlength();
	CString m_LeaderLengthStr;
	int Length;

	afx_msg void OnEnUpdateFwdecimal();
	CString m_PreFaceStr;
	int DotCnt;

	afx_msg void OnEnUpdateFwpreface();
	CString m_DecimalStr;

	int FormStat;
	CBitmapButton Form1,Form2,Form3,Form4,Form5;
	CEdit m_TextHeight_Ctrl;
	CString CheckInputNumber(CString str);
	int G_Form1;
	CString m_ScaleStr;
	CString m_ELValue_Str;
	CString m_LL_Form1, m_LL_Form2, m_LL_Form3, m_LL_Form4, m_LL_Form5;

	double DTR(double angle);
	double RTD(double radian);
	bool CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer);
	bool CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, int mode);
	bool CreateLayer(CString Layer, int color);
	bool CreateSolid(AcGePoint3d Solpt1, AcGePoint3d Solpt2, AcGePoint3d Solpt3, CString Layer);
	CAcUiSelectButton m_bSelect;
};
