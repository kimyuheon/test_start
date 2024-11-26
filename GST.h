#pragma once
#include "IniFile.h"
#include "afxwin.h"
// CGST 대화 상자입니다.

class CGST : public CAcUiDialog
{
	DECLARE_DYNAMIC(CGST)

public:
	CGST(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGST();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CAcUiSelectButton m_bSelect;
	CComboBox m_cbxDot;
	CIniFile iniFile;

	int JobMode, b_RadBtn, cbxInt, cbxInt2;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	//측점 표기 STA
	afx_msg void OnBnClickedGstRadstation();
	CString JobMode_Str;
	//측점 표기 NO
	afx_msg void OnBnClickedGstRadnumber();
	//체크박스 도면에서 지정
	afx_msg void OnBnClickedGstCheck1();
	CString Chk_STLengthStr;
	CButton m_Chk1_btn;
	bool b_Chk1;
	//시작거리
	afx_msg void OnEnUpdateGstEdtstlength();
	CString m_STLengthStr;
	double m_STLength;
	//콤보박스 소수점
	afx_msg void OnCbnSelchangeGstCbxdot();
	CString cbxInt_Str;
	//축척
	afx_msg void OnEnUpdateGstEdtscale();
	CString m_ScaleStr;
	double m_Scale;
	//측점 간격
	afx_msg void OnEnUpdateGstEdtdist();
	CString m_DistStr;
	double m_Dist;
	//체크박스 측점 도면에 쓰기
	afx_msg void OnBnClickedGstCheck2();
	CString Chk_DrawSurveyStr;
	CButton m_Chk2_btn;
	bool b_Chk2;
	//문자 크기
	afx_msg void OnEnUpdateGstEdtheight();
	afx_msg void OnBnClickedGstBselect();//문자크기버튼
	CString m_HeightStr;
	double m_Height;
	//머리말
	afx_msg void OnEnUpdateGstEdtpreface();
	CString m_PrefaceStr;
	//체크박스 측점0+000형식 유지
	afx_msg void OnBnClickedGstCheck3();
	CString Chk_FormStr;
	CButton m_Chk3_btn;
	bool b_Chk3;

	double DTR(double angle);
	bool CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, int mode);
	afx_msg void OnBnClickedGstHelp();
};
