#pragma once
#include "IniFile.h"

// CARI 대화 상자입니다.

class CARI : public CAcUiDialog
{
	DECLARE_DYNAMIC(CARI)

public:
	CARI(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CARI();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ARI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CIniFile iniFile;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//체크박스 X,Y 곡선 중심 좌표 
	afx_msg void OnBnClickedAriCheck1();
	CButton m_ChkXY_btn;
	CString m_ChkXYStr;
	bool bool_ChkXY;
	
	// Update X,Y 곡선 중심 좌표 
	afx_msg void OnEnUpdateAriDotxy();
	CString m_DotXYStr;
	int m_DotXY;

	// X,Y 곡선 중심 좌표 SPIN
	afx_msg void OnDeltaposAriSpinxy(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinXY;

	// X,Y 곡선 중심 좌표 토목
	afx_msg void OnBnClickedAriRadio1();
	int XYMode;
	CString XYMode_Str;
	int b_RadBtn_XY;

	// X,Y 곡선 중심 좌표 수학
	afx_msg void OnBnClickedAriRadio2();

	//체크박스 IA 교각
	afx_msg void OnBnClickedAriCheck2();
	CButton m_ChkIA_btn;
	CString m_ChkIAStr;
	bool bool_ChkIA;

	//Update IA 교각
	afx_msg void OnEnUpdateAriDotia();
	CString m_DotIAStr;
	int m_DotIA;

	// IA 교각 SPIN
	afx_msg void OnDeltaposAriSpinia(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinIA;

	// 라디오 IA 교각 도
	afx_msg void OnBnClickedAriRadio3();
	int IAMode;
	CString IAMode_Str;
	int b_RadBtn_IA;

	// 라디오 IA 교각 도-분-초
	afx_msg void OnBnClickedAriRadio4();

	//체크박스 R 곡선 반지름
	afx_msg void OnBnClickedAriCheck3();
	CButton m_ChkR_btn;
	CString m_ChkRStr;
	bool bool_ChkR;

	//Update R 곡선 반지름
	afx_msg void OnEnUpdateAriDotr();
	CString m_DotRStr;
	int m_DotR;
		
	// R 곡선 반지름 SPIN
	afx_msg void OnDeltaposAriSpinr(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinR;

	//체크박스 TL 접선 길이
	afx_msg void OnBnClickedAriCheck4();
	CButton m_ChkTL_btn;
	CString m_ChkTLStr;
	bool bool_ChkTL;

	//Update TL 접선 길이
	afx_msg void OnEnUpdateAriDottl();
	CString m_DotTLStr;
	int m_DotTL;

	// TL 접선 길이 SPIN
	afx_msg void OnDeltaposAriSpintl(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinTL;

	// 체크박스 CL 곡선 길이
	afx_msg void OnBnClickedAriCheck5();
	CButton m_ChkCL_btn;
	CString m_ChkCLStr;
	bool bool_ChkCL;

	// Update CL 곡선 길이
	afx_msg void OnEnUpdateAriDotcl();
	CString m_DotCLStr;
	int m_DotCL;

	// CL 곡선 길이 SPIN 
	afx_msg void OnDeltaposAriSpincl(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinCL;

	// 체크박스 SL 외할
	afx_msg void OnBnClickedAriCheck6();
	CButton m_ChkSL_btn;
	CString m_ChkSLStr;
	bool bool_ChkSL;

	//Update SL 외할
	afx_msg void OnEnUpdateAriDotsl();
	CString m_DotSLStr;
	int m_DotSL;

	// SL 외할 SPIN
	afx_msg void OnDeltaposAriSpinsl(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SpinSL;

	//체크박스 제원 명칭 쓰지 않음.
	afx_msg void OnBnClickedAriNoname();
	CButton m_ChkNoname_btn;
	CString m_ChkNonameStr;
	bool bool_ChkNoname;

	// Update 문자 높이
	afx_msg void OnEnUpdateAriEdtheight();
	CString m_TxtHeightStr;
	double m_TxtHeight;

	// 문자 높이 추출버튼
	afx_msg void OnBnClickedAriBselect1();
	CAcUiSelectButton m_bSelect1;
	CString m_bSelect1_Str;

	// 콤보박스 레이어 
	afx_msg void OnCbnSelchangeAriCmblayer();
	int m_CbxLayerInt;
	CComboBox m_CbxLayer;
	CString m_CbxLayerIntStr,m_CbxLayerStr;
	
	// 콤보박스 레이어 추출버튼
	afx_msg void OnBnClickedAriBselect2();
	CAcUiSelectButton m_bSelect2;
	CString m_bSelect2_Str;

	// 콤보박스 글꼴 
	afx_msg void OnCbnSelchangeAriCmbstyle();
	int m_CbxTextStyleInt;
	CComboBox m_CbxTextStyle;
	CString m_CbxTextStyleIntStr,m_CbxTextStyleStr;

	// 콤보박스 글꼴 추출 버튼
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
