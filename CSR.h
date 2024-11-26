#pragma once
#include "resource.h"
#include "IniFile.h"

// CCSR ��ȭ �����Դϴ�.
class CCSR : public CAcUiDialog {
	DECLARE_DYNAMIC(CCSR)

public:
	CCSR(CWnd* pParent = NULL);
	virtual ~CCSR();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CSR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CIniFile iniFile;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();	
	afx_msg void OnBnClickedCancel();
	ErrorStatus es;
	void DrawMain();
	
	//���� �Ÿ� ���� ������ư
	afx_msg void OnBnClickedCsrRad1();
	int RoundMode;
	CString RoundMode_Str;
	int b_RadBtn;

	//���ݼ��� ����Ÿ� 1.0m ������ư
	afx_msg void OnBnClickedCsrRad2();

	//���ݼ��� ����Ÿ� 1.0m �̹�����ư
	afx_msg void OnBnClickedCsrButton1();
	CBitmapButton m_btn1;

	//���ݼ��� ��Ÿ� 1.0m ������ư
	afx_msg void OnBnClickedCsrRad3();

	//���ݼ��� ��Ÿ� 1.0m �̹�����ư
	afx_msg void OnBnClickedCsrButton2();
	CBitmapButton m_btn2;

	//Ⱦ�ܸ鵵 ��ô
	afx_msg void OnEnUpdateCsrScale();
	CString m_ScaleStr;
	double m_Scale;

	//���� �� ��ȹ�� Trim üũ�ڽ�
	afx_msg void OnBnClickedCsrTrim();
	CButton m_ChkTrim_btn;
	CString m_ChkTrimStr;
	bool bool_ChkTrim;

	//���� �� ���� üũ�ڽ�
	afx_msg void OnBnClickedCsrDivide();
	CButton m_ChkDivide_btn;
	CString m_ChkDivideStr;
	bool bool_ChkDivide;

	//���� ����
	afx_msg void OnEnUpdateCsrTrimdist();
	CString m_TrimDistStr;
	double m_TrimDist;

	//������ ���� üũ�ڽ�
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
