#pragma once
#include "IniFile.h"

// CAZT 대화 상자입니다.
class CAZT : public CDialogEx{
	DECLARE_DYNAMIC(CAZT)
public:
	CAZT(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAZT();
	enum {IDD = IDD_AZT};
	CModalDialog *Modaless;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedAztOk();
	afx_msg void OnEnUpdateAztDot();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAztForm1();
	afx_msg void OnBnClickedAztForm2();
	afx_msg void OnBnClickedAztForm3();
	afx_msg void OnBnClickedAztCheck1();
	afx_msg void OnBnClickedAztCheck2();
	afx_msg void OnBnClickedAztCheck3();
	afx_msg void OnBnClickedAztBselect1();
	afx_msg void OnBnClickedAztBselect2();
	afx_msg void OnBnClickedAztBselect3();
	afx_msg void OnEnUpdateAztEdtwidth();
	afx_msg void OnEnUpdateAztEdtheight();
	afx_msg void OnCbnSelchangeAztCbxlayer1();
	afx_msg void OnCbnSelchangeAztCbxlayer2();
	afx_msg void OnCbnSelchangeAztCbxtextstyle();
	afx_msg void OnDeltaposAztSpin(NMHDR *pNMHDR, LRESULT *pResult);
	
	void MainDraw();

	int round(double x);		//반올림

	double DTR(double angle);	//라디안값으로 변환
	double RTD(double radian);	//각도로 변환

	bool CAZT::CreateLine(AcGePoint3d spt, AcGePoint3d ept, CString Layer);
	bool CreateSolid(AcGePoint3d Solpt1, AcGePoint3d Solpt2, AcGePoint3d Solpt3, CString Layer);
	bool CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, CString Style, int mode);
		
	AcDbObjectId findTextStyleName(CString name);

	CAZT *AZT;
	CIniFile iniFile;
	CComboBox m_cbxLayer;
	CComboBox m_cbxTextType;
	CComboBox m_cbxTextLayer;

	CButton m_Chk1_btn;
	CButton m_Chk2_btn;
	CButton m_Chk3_btn;
	CAcUiSelectButton m_bSelect1;
	CAcUiSelectButton m_bSelect2;
	CAcUiSelectButton m_bSelect3;
	
	CString Chk1_Str;
	CString Chk2_Str;
	CString Chk3_Str;
	CString m_DotStr;
	CString JobMode_Str;
	CString m_WidthStr;
	CString m_HeightStr;
	CString m_LayerStr;
	CString m_TxtStyleStr;
	CString m_TextLayerStr;
	CString CmbLayer;
	CString CmbTextStyle;
	CString CmbTextLayer;
	CString cbxTextInt_Str;
	CString cbxLayerInt_Str;
	CString cbxTextLayerInt_Str;
	
	int m_Dot;
	int JobMode;
	int b_RadBtn;
	int cbxTextInt;
	int cbxLayerInt;
	int cbxTextLayerInt;
	
	double m_Width;
	double m_Height;
	double TextDist;
	double TextHeight;

	bool bool_Chk1;
	bool bool_Chk2;
	bool bool_Chk3;
	
	AcGePoint3d MinPt;
	AcGePoint3d MaxPt;
	afx_msg void OnBnClickedAztHelp();
};