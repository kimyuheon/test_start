#pragma once
#include "IniFile.h"
#include "afxwin.h"
// CGST ��ȭ �����Դϴ�.

class CGST : public CAcUiDialog
{
	DECLARE_DYNAMIC(CGST)

public:
	CGST(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGST();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CAcUiSelectButton m_bSelect;
	CComboBox m_cbxDot;
	CIniFile iniFile;

	int JobMode, b_RadBtn, cbxInt, cbxInt2;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	//���� ǥ�� STA
	afx_msg void OnBnClickedGstRadstation();
	CString JobMode_Str;
	//���� ǥ�� NO
	afx_msg void OnBnClickedGstRadnumber();
	//üũ�ڽ� ���鿡�� ����
	afx_msg void OnBnClickedGstCheck1();
	CString Chk_STLengthStr;
	CButton m_Chk1_btn;
	bool b_Chk1;
	//���۰Ÿ�
	afx_msg void OnEnUpdateGstEdtstlength();
	CString m_STLengthStr;
	double m_STLength;
	//�޺��ڽ� �Ҽ���
	afx_msg void OnCbnSelchangeGstCbxdot();
	CString cbxInt_Str;
	//��ô
	afx_msg void OnEnUpdateGstEdtscale();
	CString m_ScaleStr;
	double m_Scale;
	//���� ����
	afx_msg void OnEnUpdateGstEdtdist();
	CString m_DistStr;
	double m_Dist;
	//üũ�ڽ� ���� ���鿡 ����
	afx_msg void OnBnClickedGstCheck2();
	CString Chk_DrawSurveyStr;
	CButton m_Chk2_btn;
	bool b_Chk2;
	//���� ũ��
	afx_msg void OnEnUpdateGstEdtheight();
	afx_msg void OnBnClickedGstBselect();//����ũ���ư
	CString m_HeightStr;
	double m_Height;
	//�Ӹ���
	afx_msg void OnEnUpdateGstEdtpreface();
	CString m_PrefaceStr;
	//üũ�ڽ� ����0+000���� ����
	afx_msg void OnBnClickedGstCheck3();
	CString Chk_FormStr;
	CButton m_Chk3_btn;
	bool b_Chk3;

	double DTR(double angle);
	bool CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, int mode);
	afx_msg void OnBnClickedGstHelp();
};
