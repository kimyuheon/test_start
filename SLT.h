#pragma once
#include "afxwin.h"
#include "IniFile.h"

// CSLT 대화 상자입니다.

class CSLT : public CAcUiDialog
{
	DECLARE_DYNAMIC(CSLT)

public:
	CSLT(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSLT();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SLT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CIniFile iniFile;
	//afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CBitmapButton m_btn1;
	CBitmapButton m_btn2;

	//경사표시 선택 라디오
	afx_msg void OnBnClickedSltRadpercent();
	int JobMode;
	CString JobMode_Str;
	int b_RadBtn;
	//경사표시 선택 라디오
	afx_msg void OnBnClickedSltRadratio();
	//경사표시 비트맵
	afx_msg void OnBnClickedSltBtn1();
	afx_msg void OnBnClickedSltBtn2();
	//체크박스 앞에"S="붙임
	afx_msg void OnBnClickedSltCheck1();
	CButton m_Chk1_btn;
	CString Chk1_Str;
	bool bool_Chk1;
	//체크박스 화살표 그리기
	afx_msg void OnBnClickedSltCheck2();
	CButton m_Chk2_btn;
	CString Chk2_Str;
	bool bool_Chk2;
	//체크박스 +/-표시
	afx_msg void OnBnClickedSltCheck3();
	CButton m_Chk3_btn;
	CString Chk3_Str;
	bool bool_Chk3;
	//화살표 길이
	afx_msg void OnEnUpdateSltArrowwidth();
	CString m_ArrowWidthStr;
	double m_ArrowWidth;
	//화살표 높이
	afx_msg void OnEnUpdateSltArrowheight();
	CString m_ArrowHeightStr;
	double m_ArrowHeight;
	//지시선 여유
	afx_msg void OnEnUpdateSltLeader();
	CString m_LeaderStr;
	double m_Leader;
	//콤보박스 글꼴
	afx_msg void OnCbnSelchangeSltCbxtexttype();
	int cbxTextInt;
	CComboBox m_cbxTextType;
	CString cbxTextInt_Str, CmbTextType;
	//글꼴 추출하기 버튼
	afx_msg void OnBnClickedSltBselect1();
	CString m_TxtStyleStr;
	CAcUiSelectButton m_bSelect1;
	//높이
	afx_msg void OnEnUpdateSltEdtheight();
	CString m_HeightStr;
	double m_Height;
	//소수점
	afx_msg void OnEnUpdateSltEdtdot();
	CString m_DotStr;
	int m_Dot;
	//소수점 Spin
	afx_msg void OnDeltaposSltSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	//콤보박스 레이어
	afx_msg void OnCbnSelchangeSltCbxlayer();
	int cbxLayerInt;
	CComboBox m_cbxLayer;
	CString cbxLayerInt_Str, CmbLayer;
	//색상 버튼
	afx_msg void OnBnClickedSltBtn3();
	int m_Color;
	CString m_ColorStr;
	CButton m_btn3;
	//색상 추출 버튼
	afx_msg void OnBnClickedSltBselect2();
	CString m_ExtColorStr;
	CAcUiSelectButton m_bSelect2;
	//색상 컬러,스태틱 출력
	void ColorSetting(int newColorIndex);
	CStatic m_btn_Color;
	afx_msg void OnPaint();
	//Horizontal 축척
	afx_msg void OnEnUpdateSltEdthorscale();
	CString m_HScaleStr;
	double m_HScale;
	//Vertical 축척
	afx_msg void OnEnUpdateSltEdtverscale();
	CString m_VScaleStr;
	double m_VScale;
	//축척 리스트
	afx_msg void OnBnClickedSltBselect3();
	CButton m_btn4;
	//CAcUiSelectButton m_bSelect3;	
	CAcUiPickButton m_bSelect3;
	//팝업 리스트
	afx_msg void OnHSlt1();
	afx_msg void OnHSlt2();
	afx_msg void OnHSlt3();
	afx_msg void OnHSlt4();
	afx_msg void OnHSlt5();
	afx_msg void OnHSlt6();
	afx_msg void OnHSlt7();
	//체크박스 거리/높이 계산시 소수점 적용
	afx_msg void OnBnClickedSltCheck4();
	CButton m_Chk4_btn;
	CString Chk4_Str;
	bool bool_Chk4;
	//거리 계산할 소수점
	afx_msg void OnEnUpdateSltEdtdistdot();
	CString m_DistDotStr;
	double m_DistDot;
	//거리 계산할 소수점 Spin
	afx_msg void OnDeltaposSltSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	//높이 계산할 소수점
	afx_msg void OnEnUpdateSltEdtheightdot();
	CString m_HeightDotStr;
	double m_HeightDot;
	//높이 계산할 소수점 Spin
	afx_msg void OnDeltaposSltSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	//버튼 선 선택, 두 점 선택
	afx_msg void OnBnClickedSltBtn4();
	afx_msg void OnBnClickedSltBtn5();
	CString PerResultStr, RatioResultStr;
	double PerResult, RatioResult;
	//virtual void PostNcDestroy();
	//폴리선생성
	bool CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer, double Color);
	//텍스트생성
	double CSLT::CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer,CString Style, double Color, int mode);
	//솔리드생성
	bool CSLT::CreateSolid(AcGePoint3d Solpt1, AcGePoint3d Solpt2, AcGePoint3d Solpt3, CString Layer, double Color);
	afx_msg void OnBnClickedSltWrite();
	CButton m_Write_btn;
	afx_msg void OnBnClickedOk();
	double DTR(double angle);//라디안값으로 변환
	double RTD(double radian);//각도로 변환
	AcDbObjectId findTextStyleName(CString name);//글꼴 ID로 변환
	double TextDist, TextHeight;
	AcGePoint3d MaxPt,MinPt;
	void MainDraw();
	
	virtual void PostNcDestroy();
};
