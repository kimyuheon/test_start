#pragma once
#include "IniFile.h"

// CARI ��ȭ �����Դϴ�.

class CARI : public CAcUiDialog
{
	DECLARE_DYNAMIC(CARI)

public:
	CARI(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CARI();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ARI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CIniFile iniFile;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//üũ�ڽ� X,Y � �߽� ��ǥ 
	afx_msg void OnBnClickedAriCheck1();
	CButton m_ChkXY_btn;
	CString m_ChkXYStr;
	bool bool_ChkXY;
	
	// Update X,Y � �߽� ��ǥ 
	afx_msg void OnEnUpdateAriDotxy();
	CString m_DotXYStr;
	int m_DotXY;

	// X,Y � �߽� ��ǥ SPIN
	afx_msg void OnDeltaposAriSpinxy(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinXY;

	// X,Y � �߽� ��ǥ ���
	afx_msg void OnBnClickedAriRadio1();
	int XYMode;
	CString XYMode_Str;
	int b_RadBtn_XY;

	// X,Y � �߽� ��ǥ ����
	afx_msg void OnBnClickedAriRadio2();

	//üũ�ڽ� IA ����
	afx_msg void OnBnClickedAriCheck2();
	CButton m_ChkIA_btn;
	CString m_ChkIAStr;
	bool bool_ChkIA;

	//Update IA ����
	afx_msg void OnEnUpdateAriDotia();
	CString m_DotIAStr;
	int m_DotIA;

	// IA ���� SPIN
	afx_msg void OnDeltaposAriSpinia(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinIA;

	// ���� IA ���� ��
	afx_msg void OnBnClickedAriRadio3();
	int IAMode;
	CString IAMode_Str;
	int b_RadBtn_IA;

	// ���� IA ���� ��-��-��
	afx_msg void OnBnClickedAriRadio4();

	//üũ�ڽ� R � ������
	afx_msg void OnBnClickedAriCheck3();
	CButton m_ChkR_btn;
	CString m_ChkRStr;
	bool bool_ChkR;

	//Update R � ������
	afx_msg void OnEnUpdateAriDotr();
	CString m_DotRStr;
	int m_DotR;
		
	// R � ������ SPIN
	afx_msg void OnDeltaposAriSpinr(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinR;

	//üũ�ڽ� TL ���� ����
	afx_msg void OnBnClickedAriCheck4();
	CButton m_ChkTL_btn;
	CString m_ChkTLStr;
	bool bool_ChkTL;

	//Update TL ���� ����
	afx_msg void OnEnUpdateAriDottl();
	CString m_DotTLStr;
	int m_DotTL;

	// TL ���� ���� SPIN
	afx_msg void OnDeltaposAriSpintl(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinTL;

	// üũ�ڽ� CL � ����
	afx_msg void OnBnClickedAriCheck5();
	CButton m_ChkCL_btn;
	CString m_ChkCLStr;
	bool bool_ChkCL;

	// Update CL � ����
	afx_msg void OnEnUpdateAriDotcl();
	CString m_DotCLStr;
	int m_DotCL;

	// CL � ���� SPIN 
	afx_msg void OnDeltaposAriSpincl(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinCL;

	// üũ�ڽ� SL ����
	afx_msg void OnBnClickedAriCheck6();
	CButton m_ChkSL_btn;
	CString m_ChkSLStr;
	bool bool_ChkSL;

	//Update SL ����
	afx_msg void OnEnUpdateAriDotsl();
	CString m_DotSLStr;
	int m_DotSL;

	// SL ���� SPIN
	afx_msg void OnDeltaposAriSpinsl(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinSL;

	//üũ�ڽ� ���� ��Ī ���� ����.
	afx_msg void OnBnClickedAriNoname();
	CButton m_ChkNoname_btn;
	CString m_ChkNonameStr;
	bool bool_ChkNoname;

	// Update ���� ����
	afx_msg void OnEnUpdateAriEdtheight();
	CString m_TxtHeightStr;
	double m_TxtHeight;

	// ���� ���� �����ư
	afx_msg void OnBnClickedAriBselect1();
	CAcUiSelectButton m_bSelect1;
	CString m_bSelect1_Str;

	// �޺��ڽ� ���̾� 
	afx_msg void OnCbnSelchangeAriCmblayer();
	int m_CbxLayerInt;
	CComboBox m_CbxLayer;
	CString m_CbxLayerIntStr,m_CbxLayerStr;
	
	// �޺��ڽ� ���̾� �����ư
	afx_msg void OnBnClickedAriBselect2();
	CAcUiSelectButton m_bSelect2;
	CString m_bSelect2_Str;

	// �޺��ڽ� �۲� 
	afx_msg void OnCbnSelchangeAriCmbstyle();
	int m_CbxTextStyleInt;
	CComboBox m_CbxTextStyle;
	CString m_CbxTextStyleIntStr,m_CbxTextStyleStr;

	// �޺��ڽ� �۲� ���� ��ư
	afx_msg void OnBnClickedAriBselect3();
	CAcUiSelectButton m_bSelect3;
	CString m_bSelect3_Str;
	

	void PyoSi(const AcGePoint3d pos);
	double getPickBox();
	Acad::ErrorStatus getSysVar(LPCTSTR varName, double& val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, int& val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, AcGePoint2d& val);
	void Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result);
	double DTR(double angle);
	double RTD(double radian);
	double CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer,CString Style,double Color, int mode);
	AcDbObjectId findTextStyleName(CString name);
	
	afx_msg void OnBnClickedAriHelp();
};
