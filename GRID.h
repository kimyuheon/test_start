#pragma once
#include "IniFile.h"


// CDCG 대화 상자입니다.

class CDCG : public CDialogEx
{
	DECLARE_DYNAMIC(CDCG)

public:
	CDCG(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDCG();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DCG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	LRESULT onAcadKeepFocus(WPARAM, LPARAM);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CIniFile iniFile;
	//라디오 Grid 작성방법
	afx_msg void OnBnClickedDcgRad1();
	afx_msg void OnBnClickedDcgRad2();
	afx_msg void OnBnClickedDcgRad3();	
	int m_Radbtn, JobMode;
	CString JobMode_Str;
	//Edt 상
	afx_msg void OnEnUpdateDcgEdtUp();
	CString m_GridUpStr;
	int m_GridUp; 
	//Edt 하
	afx_msg void OnEnUpdateDcgEdtDown();
	CString m_GridDownStr;
	int m_GridDown; 
	//Edt 좌
	afx_msg void OnEnUpdateDcgEdtLeft();
	CString m_GridLeftStr;
	int m_GridLeft; 
	//Edt 우
	afx_msg void OnEnUpdateDcgEdtRight();
	CString m_GridRightStr;
	int m_GridRight; 
	//체크박스 모든 설정 표준화에 맞춤
	afx_msg void OnBnClickedDcgChkStandard();
	CButton m_ChkStd_btn;
	CString m_ChkStd_Str;
	bool bool_ChkStd;
	//Edt 횡단면도 축척
	afx_msg void OnEnUpdateDcgEdtScale();
	CString m_ScaleStr;
	double m_Scale;
	//Edt 주Grid 간격
	afx_msg void OnEnUpdateDcgEdtMainwidth();
	CString m_MainWidthStr;
	double m_MainWidth;
	//Edt 주Grid 선 축척
	afx_msg void OnEnUpdateDcgEdtMainlinescale();
	CString m_MainLScaleStr;
	double m_MainLScale;
	//체크박스 주Grid Continuous 선으로 그리기
	afx_msg void OnBnClickedDcgChkMaincon();
	CButton m_ChkMainCon_btn;
	CString m_ChkMainCon_Str;
	bool bool_ChkMaincon;
	//콤보박스 주Grid 레이어
	afx_msg void OnCbnSelchangeDcgComboMainlayer();
	int m_CbxMainLayerInt;
	CComboBox m_CbxMainLayer;
	CString m_CbxMainLayer_Str,m_CbxMainLayerInt_Str;
	//버튼 주Grid 색상버튼 
	afx_msg void OnBnClickedDcgBtnMaincolor();
	int m_MainColor;
	CString m_MainColorStr;
	CButton m_MainColor_btn;
	//추출버튼 주Grid 색상 버튼
	afx_msg void OnBnClickedDcgMainextract();
	CString m_ExtMainColorStr;
	CAcUiSelectButton m_ExtMainSelect;
	CStatic m_btn_MainColor;
	//Edt 보조Grid 간격
	afx_msg void OnEnUpdateDcgEdtSubwidth();
	CString m_SubWidthStr;
	double m_SubWidth;
	//Edt 보조Grid 선 축척
	afx_msg void OnEnUpdateDcgEdtSublinescale();
	CString m_SubLScaleStr;
	double m_SubLScale;
	//체크박스 보조Grid Continuous 선으로 그리기
	afx_msg void OnBnClickedDcgChkSubcon();
	CButton m_ChkSubCon_btn;
	CString m_ChkSubCon_Str;
	bool bool_ChkSubCon;
	//콤보박스 보조Grid 레이어
	afx_msg void OnCbnSelchangeDcgComboSublayer();
	int m_CbxSubLayerInt;
	CComboBox m_CbxSubLayer;
	CString m_CbxSubLayer_Str,m_CbxSubLayerInt_Str;
	//버튼 보조Grid 색상
	afx_msg void OnBnClickedDcgBtnSubcolor();
	int m_SubColor;
	CString m_SubColorStr;
	CButton m_SubColor_btn;
	//추출버튼 보조Grid 색상
	afx_msg void OnBnClickedDcgSubextract();	
	CString m_ExtSubColorStr;
	CAcUiSelectButton m_ExtSubSelect;
	CStatic m_btn_SubColor;
	//체크박스 문자 영역 보조 Grid 삭제
	afx_msg void OnBnClickedDcgChkDelgrid();
	CButton m_ChkDelGrid_btn;
	CString m_ChkDelGrid_Str;
	bool bool_ChkDelGrid;
	//체크박스 EL 표기 좌
	afx_msg void OnBnClickedDcgChkElleft();
	CButton m_ChkELLeft_btn;
	CString m_ChkELLeft_Str;
	bool bool_ChkELLeft;
	//체크박스 EL 표기 우
	afx_msg void OnBnClickedDcgChkElright();
	CButton m_ChkELRight_btn;
	CString m_ChkELRight_Str;
	bool bool_ChkELRight;
	//Edt 문자크기
	afx_msg void OnEnUpdateDcgEdtTextheight();
	CString m_TextHeightStr;
	double m_TextHeight;
	//Edt 문자 폭
	afx_msg void OnEnUpdateDcgEdtTextwidth();
	CString m_TextWidthStr;
	double m_TextWidth;
	//추출버튼 문자 크기 버튼
	afx_msg void OnBnClickedDcgTextextract();
	CString m_ExtTextSizeStr;
	CAcUiSelectButton m_ExtTextSizeSelect;
	//콤보박스 글꼴 
	afx_msg void OnCbnSelchangeDcgComboTextstyle();
	int m_CbxTextStyleInt;
	CComboBox m_CbxTextStyle;
	CString m_CbxTextStyle_Str,m_CbxTextStyleInt_Str;
	//콤보박스 문자 레이어
	afx_msg void OnCbnSelchangeDcgComboTextlayer();
	int m_CbxTextLayerInt;
	CComboBox m_CbxTextLayer;
	CString m_CbxTextLayer_Str,m_CbxTextLayerInt_Str;
	//추출버튼 문자 레이어 버튼
	afx_msg void OnBnClickedDcgTextlayerextract();
	CString m_ExtTextLayerStr;
	CAcUiSelectButton m_ExtTextLayerSelect;
	//버튼 문자 색상
	afx_msg void OnBnClickedDcgBtnTextcolor2();
	int m_TextColor;
	CString m_TextColorStr;
	CButton m_TextColor_btn;
	CStatic m_btn_TextColor;

	double m_ELValue;
	CString m_ELValueStr;

	bool CreateLayer(CString Layer, int color);
	int getLayerColor(CString LayerName);
	void ColorSetting(int newColorIndex, int Mode);
	afx_msg void OnPaint();	
	virtual void PostNcDestroy();
	void DrawMain();
	AcDbObjectId m_PolyObjID;
	AcDbObjectId CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer, double Color);
	CString CheckInputNumber(CString str);
	void Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result);
	double DTR(double angle);
	double RTD(double angle);
	void PyoSi(const AcGePoint3d pos);
	double getPickBox();
	Acad::ErrorStatus getSysVar(LPCTSTR varName, double& val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, AcGePoint2d& val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, int& val);
	AcGePoint3dArray PolyptArr(AcGePoint3d pt1, AcGePoint3d pt2, AcGePoint3d pt3, AcGePoint3d pt4, double Ang);
	double CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer,CString Style,double Color, int mode);
	AcDbObjectId findTextStyleName(CString name);
	void CreateGrid(AcGePoint3dArray polyarr, AcGePoint3d Cntrpt, double ELValue);
	void CreateObjGrid(AcDbObjectId objID, AcGePoint3d Cntrpt, double ELValue);
	AcGePoint3dArray Obj_getInterpt(AcDbObjectId PolygonID , AcGePoint3d Cntrpt, int Mode);
	AcGePoint3d XMaxpt, YMaxpt, XMinpt, YMinpt;
	void LoadLineType(ACHAR* LineType, ACHAR* LineTypeFile);
	afx_msg void OnBnClickedDcgWrite();
	afx_msg void OnBnClickedDcgCancel();
	afx_msg void OnBnClickedDcgHelp();
};
