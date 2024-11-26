#pragma once
#include "afxwin.h"
#include "IniFile.h"

// CSLT ��ȭ �����Դϴ�.

class CSLT : public CAcUiDialog
{
	DECLARE_DYNAMIC(CSLT)

public:
	CSLT(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSLT();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SLT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CIniFile iniFile;
	//afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CBitmapButton m_btn1;
	CBitmapButton m_btn2;

	//���ǥ�� ���� ����
	afx_msg void OnBnClickedSltRadpercent();
	int JobMode;
	CString JobMode_Str;
	int b_RadBtn;
	//���ǥ�� ���� ����
	afx_msg void OnBnClickedSltRadratio();
	//���ǥ�� ��Ʈ��
	afx_msg void OnBnClickedSltBtn1();
	afx_msg void OnBnClickedSltBtn2();
	//üũ�ڽ� �տ�"S="����
	afx_msg void OnBnClickedSltCheck1();
	CButton m_Chk1_btn;
	CString Chk1_Str;
	bool bool_Chk1;
	//üũ�ڽ� ȭ��ǥ �׸���
	afx_msg void OnBnClickedSltCheck2();
	CButton m_Chk2_btn;
	CString Chk2_Str;
	bool bool_Chk2;
	//üũ�ڽ� +/-ǥ��
	afx_msg void OnBnClickedSltCheck3();
	CButton m_Chk3_btn;
	CString Chk3_Str;
	bool bool_Chk3;
	//ȭ��ǥ ����
	afx_msg void OnEnUpdateSltArrowwidth();
	CString m_ArrowWidthStr;
	double m_ArrowWidth;
	//ȭ��ǥ ����
	afx_msg void OnEnUpdateSltArrowheight();
	CString m_ArrowHeightStr;
	double m_ArrowHeight;
	//���ü� ����
	afx_msg void OnEnUpdateSltLeader();
	CString m_LeaderStr;
	double m_Leader;
	//�޺��ڽ� �۲�
	afx_msg void OnCbnSelchangeSltCbxtexttype();
	int cbxTextInt;
	CComboBox m_cbxTextType;
	CString cbxTextInt_Str, CmbTextType;
	//�۲� �����ϱ� ��ư
	afx_msg void OnBnClickedSltBselect1();
	CString m_TxtStyleStr;
	CAcUiSelectButton m_bSelect1;
	//����
	afx_msg void OnEnUpdateSltEdtheight();
	CString m_HeightStr;
	double m_Height;
	//�Ҽ���
	afx_msg void OnEnUpdateSltEdtdot();
	CString m_DotStr;
	int m_Dot;
	//�Ҽ��� Spin
	afx_msg void OnDeltaposSltSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	//�޺��ڽ� ���̾�
	afx_msg void OnCbnSelchangeSltCbxlayer();
	int cbxLayerInt;
	CComboBox m_cbxLayer;
	CString cbxLayerInt_Str, CmbLayer;
	//���� ��ư
	afx_msg void OnBnClickedSltBtn3();
	int m_Color;
	CString m_ColorStr;
	CButton m_btn3;
	//���� ���� ��ư
	afx_msg void OnBnClickedSltBselect2();
	CString m_ExtColorStr;
	CAcUiSelectButton m_bSelect2;
	//���� �÷�,����ƽ ���
	void ColorSetting(int newColorIndex);
	CStatic m_btn_Color;
	afx_msg void OnPaint();
	//Horizontal ��ô
	afx_msg void OnEnUpdateSltEdthorscale();
	CString m_HScaleStr;
	double m_HScale;
	//Vertical ��ô
	afx_msg void OnEnUpdateSltEdtverscale();
	CString m_VScaleStr;
	double m_VScale;
	//��ô ����Ʈ
	afx_msg void OnBnClickedSltBselect3();
	CButton m_btn4;
	//CAcUiSelectButton m_bSelect3;	
	CAcUiPickButton m_bSelect3;
	//�˾� ����Ʈ
	afx_msg void OnHSlt1();
	afx_msg void OnHSlt2();
	afx_msg void OnHSlt3();
	afx_msg void OnHSlt4();
	afx_msg void OnHSlt5();
	afx_msg void OnHSlt6();
	afx_msg void OnHSlt7();
	//üũ�ڽ� �Ÿ�/���� ���� �Ҽ��� ����
	afx_msg void OnBnClickedSltCheck4();
	CButton m_Chk4_btn;
	CString Chk4_Str;
	bool bool_Chk4;
	//�Ÿ� ����� �Ҽ���
	afx_msg void OnEnUpdateSltEdtdistdot();
	CString m_DistDotStr;
	double m_DistDot;
	//�Ÿ� ����� �Ҽ��� Spin
	afx_msg void OnDeltaposSltSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	//���� ����� �Ҽ���
	afx_msg void OnEnUpdateSltEdtheightdot();
	CString m_HeightDotStr;
	double m_HeightDot;
	//���� ����� �Ҽ��� Spin
	afx_msg void OnDeltaposSltSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	//��ư �� ����, �� �� ����
	afx_msg void OnBnClickedSltBtn4();
	afx_msg void OnBnClickedSltBtn5();
	CString PerResultStr, RatioResultStr;
	double PerResult, RatioResult;
	//virtual void PostNcDestroy();
	//����������
	bool CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer, double Color);
	//�ؽ�Ʈ����
	double CSLT::CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer,CString Style, double Color, int mode);
	//�ָ������
	bool CSLT::CreateSolid(AcGePoint3d Solpt1, AcGePoint3d Solpt2, AcGePoint3d Solpt3, CString Layer, double Color);
	afx_msg void OnBnClickedSltWrite();
	CButton m_Write_btn;
	afx_msg void OnBnClickedOk();
	double DTR(double angle);//���Ȱ����� ��ȯ
	double RTD(double radian);//������ ��ȯ
	AcDbObjectId findTextStyleName(CString name);//�۲� ID�� ��ȯ
	double TextDist, TextHeight;
	AcGePoint3d MaxPt,MinPt;
	void MainDraw();
	
	virtual void PostNcDestroy();
};
