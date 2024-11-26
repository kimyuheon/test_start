#pragma once
#include "afxwin.h"
#include "IniFile.h"
#include <vector>


// CCUT 대화 상자입니다.

class CCUT : public CAcUiDialog
{
	DECLARE_DYNAMIC(CCUT)

public:
	CCUT(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCUT();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void CUTMain();

	CIniFile iniFile;

	afx_msg void OnBnClickedCutWrite();
	afx_msg void OnBnClickedCancel();
	//층따기 높이
	afx_msg void OnEnUpdateCutHeight();
	CString m_CutHeightStr;
	double m_CutHeight;
	//배수층 경사
	afx_msg void OnEnUpdateCutSlope();
	CString m_CutSlopeStr;
	int m_CutSlope;
	//도면 축척
	afx_msg void OnEnUpdateCutScale();
	CString m_CutScaleStr;
	int m_CutScale;
	//최소 높이/
	afx_msg void OnEnUpdateCutMinheight();
	CString m_CutMinHeightStr;
	double m_CutMinHeight;
	//층따기 제외 경사 1: S
	afx_msg void OnEnUpdateCutExcept();
	CString m_CutExceptStr;
	int m_CutExcept;
	//Hidden 선 축척
	afx_msg void OnEnUpdateCutLscale();
	CString m_CutLscaleStr;
	double m_CutLscale;
	//Hidden 선 사용 체크박스
	afx_msg void OnBnClickedCutCheck1();
	CButton m_Chk1_btn;
	CString Chk1_Str;
	bool bool_Chk1;
	//표준 레이어 사용 체크박스
	afx_msg void OnBnClickedCutCheck2();
	CButton m_Chk2_btn;
	CString Chk2_Str;
	bool bool_Chk2;
	//선 축척 추출버튼
	afx_msg void OnBnClickedCutBselect1();
	CString m_Bselect1Str;
	CAcUiSelectButton m_bSelect1;
	//표준 레이어 콤보박스
	afx_msg void OnCbnSelchangeCutCbxlayer();
	int m_CbxLayerInt;
	CComboBox m_CbxLayer;
	CString m_CbxLayerIntStr,m_CbxLayerStr;
	int StdLayer_idx;
	//표준 레이어 색상버튼
	afx_msg void OnBnClickedCutColorbtn();
	int m_Color;
	CString m_ColorStr;
	CButton m_Colorbtn;
	CStatic m_btn_Color;
	//표준 레이어 색상 추출 버튼
	afx_msg void OnBnClickedCutBselect2();
	CString m_Bselect2Str;
	CAcUiSelectButton m_bSelect2;

	LRESULT onAcadKeepFocus(WPARAM, LPARAM);
	void ColorSetting(int newColorIndex);

	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	bool CreateLayer(CString Layer, int color);
	int getLayerColor(CString LayerName);
	bool CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer, double Color);
	Acad::ErrorStatus setVar(LPCTSTR varName, const resbuf* newVal);
	Acad::ErrorStatus setSysVar(LPCTSTR varName, int val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, int& val);
	void Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result);
	void PyoSi(const AcGePoint3d pos);
	double getPickBox();
	Acad::ErrorStatus getSysVar(LPCTSTR varName, double& val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, AcGePoint2d& val);
	void getclosestPoint(AcGePoint3d sp, AcGePoint3d ep, AcGePoint3d givenPnt, AcGePoint3d& closestPnt);
	afx_msg void OnBnClickedCutHelp();
};
