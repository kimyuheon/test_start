#pragma once
#include "IniFile.h"
#include "afxwin.h"
// CSTT 대화 상자입니다.

class CSTT : public CAcUiDialog
{
	DECLARE_DYNAMIC(CSTT)

public:
	CSTT(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSTT();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_STT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CRect Dialog_rc;

	CComboBox m_cbxLineLayer, m_cbxTextLayer, m_cbxDotLayer, m_cbxDot, m_cbxTextType;
	//CString m_cbxTextTypeStr, m_cbxDotStr, m_cbxLineLayerStr, m_cbxTextLayerStr, m_cbxDotLayerStr;
	int b_RadBtn, JobMode;
	CIniFile iniFile;
	//측점표기 라디오버튼
	afx_msg void OnBnClickedSttRadstation();
	afx_msg void OnBnClickedSttRadnumber();
	//축척
	afx_msg void OnEnUpdateSttEdtscale();
	CString m_ScaleStr;
	double m_Scale;
	//시작거리
	afx_msg void OnEnUpdateSttEdtstlength();
	CString m_STLengthStr;
	double m_STLength;
    //측점간격
	afx_msg void OnEnUpdateSttEdtdist();
	CString m_DistStr;
	double m_Dist;
	//크기
	afx_msg void OnEnUpdateSttEdtheight();
	CString m_HeightStr;
	double m_Height;
	//폭
	afx_msg void OnEnUpdateSttEdtwidth();
	CString m_WidthStr;
	double m_Width;
	//DOt 표기 직경
	afx_msg void OnEnUpdateSttEdtdot();
	CString m_DotStr;
	double m_Dot;
	//중심~인출설 길이
	afx_msg void OnEnUpdateSttEdtdist2();
	CString m_Dist2Str;
	double m_Dist2;
	//중심~인출선 Offset
	afx_msg void OnEnUpdateSttEdtoffset();
	CString m_OffsetStr;
	double m_Offset;

	int cbxInt,cbxInt2;
	afx_msg void OnCbnSelchangeSttCbxline();
	afx_msg void OnCbnSelchangeSttCbxtext();
	afx_msg void OnCbnSelchangeSttCbxlayerdot();
	afx_msg void OnCbnSelchangeSttCbxtexttype();
	afx_msg void OnCbnSelchangeSttCbxdot();
	CAcUiSelectButton m_bSelect1, m_bSelect2, m_bSelect3, m_bSelect4;
	//레이어 문자 버튼
	afx_msg void OnBnClickedSttBselect1();
	afx_msg void OnBnClickedSttBselect2();
	afx_msg void OnBnClickedSttBselect3();
	afx_msg void OnBnClickedSttBselect4();
	bool TakeInLayer(int Mode);
	
	bool b_Chk1, b_Chk2, b_Chk3, b_Chk4;
	CButton m_Chk1_btn, m_Chk2_btn, m_Chk3_btn, m_Chk4_btn;
	afx_msg void OnBnClickedSttCheck1();
	afx_msg void OnBnClickedSttCheck2();
	afx_msg void OnBnClickedSttCheck3();
	afx_msg void OnBnClickedSttCheck4();
	
	bool m_bFlag;
	CString cbxText_Str, cbxLine_Str, cbxDot_Str;
	CString m_StandLayer;
	double DTR(double angle);
	void getclosestPoint(AcGePoint3d sp, AcGePoint3d ep, AcGePoint3d givenPnt, AcGePoint3d& closestPnt);
	bool CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, int mode);
	bool CreateLine(AcGePoint3d spt,AcGePoint3d ept, CString LineType, double LineTypeScale, CString Layer);
	bool CreateLayer(CString Layer, int color);
	bool CreatePLine(AcGePoint3d pt1, AcGePoint3d pt2, double LineWidth, CString LineType, double LineTypeScale, CString Layer);
	void CreatePoint(AcGePoint3d pos);
	CString CheckInputNumber(CString str, int Mode);
	
	afx_msg void OnBnClickedSttHelp();
};
