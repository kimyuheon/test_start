#pragma once
#include "resource.h"
#include "IniFile.h"

// CCSR 대화 상자입니다.
class CCSR : public CAcUiDialog {
	DECLARE_DYNAMIC(CCSR)

public:
	CCSR(CWnd* pParent = NULL);
	virtual ~CCSR();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CSR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CIniFile iniFile;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();	
	afx_msg void OnBnClickedCancel();
	ErrorStatus es;
	void DrawMain();
	
	//라운딩 거리 지정 라디오버튼
	afx_msg void OnBnClickedCsrRad1();
	int RoundMode;
	CString RoundMode_Str;
	int b_RadBtn;

	//지반선쪽 수평거리 1.0m 라디오버튼
	afx_msg void OnBnClickedCsrRad2();

	//지반선쪽 수평거리 1.0m 이미지버튼
	afx_msg void OnBnClickedCsrButton1();
	CBitmapButton m_btn1;

	//지반선쪽 사거리 1.0m 라디오버튼
	afx_msg void OnBnClickedCsrRad3();

	//지반선쪽 사거리 1.0m 이미지버튼
	afx_msg void OnBnClickedCsrButton2();
	CBitmapButton m_btn2;

	//횡단면도 축척
	afx_msg void OnEnUpdateCsrScale();
	CString m_ScaleStr;
	double m_Scale;

	//라운딩 후 계획선 Trim 체크박스
	afx_msg void OnBnClickedCsrTrim();
	CButton m_ChkTrim_btn;
	CString m_ChkTrimStr;
	bool bool_ChkTrim;

	//라운딩 선 분할 체크박스
	afx_msg void OnBnClickedCsrDivide();
	CButton m_ChkDivide_btn;
	CString m_ChkDivideStr;
	bool bool_ChkDivide;

	//분할 간격
	afx_msg void OnEnUpdateCsrTrimdist();
	CString m_TrimDistStr;
	double m_TrimDist;

	//선으로 폭파 체크박스
	afx_msg void OnBnClickedCsrExplode();
	CButton m_ChkExplode_btn;
	CString m_ChkExplodeStr;
	bool bool_ChkExplode;


	Acad::ErrorStatus getSysVar(LPCTSTR varName, double& val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, AcGePoint2d& val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, int& val);
	Acad::ErrorStatus setVar(LPCTSTR varName, const resbuf* newVal);
	Acad::ErrorStatus CCSR::setSysVar(LPCTSTR varName, int val);
	void PyoSi(const AcGePoint3d pos);
	double getPickBox();
	void Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result);
	void getclosestPoint(AcGePoint3d sp, AcGePoint3d ep, AcGePoint3d givenPnt, AcGePoint3d& closestPnt);
	AcDbObjectId CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer, double Color, double Bulge, int index = 0);
	AcDbArc*dbArcFromGeArc(AcGeCircArc3d Arc);
	AcDbObjectId makeArc(AcGeCircArc3d& arc);
	int m_color;
	double m_ltScale;
	CString m_layer,m_lineType;
	AcDbVoidPtrArray voidarr;
	void SetProperty(AcDbEntity *pEnt);
	TCHAR			m_BlockName[128];
	AcDbDatabase*	pDb;
	AcDbObjectId CCSR::makeArc(const AcGePoint3d& startPnt, const AcGePoint3d& pnt, const AcGePoint3d& endPnt);
	Acad::ErrorStatus appendToBlock(AcDbEntity* pEnt, AcDbObjectId& idEnt, const TCHAR *pcBlockName, AcDbDatabase *pDb);
	void SplitPline(AcGePoint3d pt1, AcGePoint3d pt2, AcDbObjectId Plobj);
	double DTR(double angle);
	double RTD(double radian);
	afx_msg void OnBnClickedCsrHelp();
};
