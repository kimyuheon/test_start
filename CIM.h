#pragma once

#include "StdAfx.h"

#include "resource.h"
#include "afxwin.h"
#include "afxdialogex.h"
#include "StaticColorBox.h"
#include "IniFile.h"

//#include "e:\zrx\zrxsdk2017\inc\zacuibutton.h"
//#include "zacuiButton.h"

class CCIM : public CAcUiDialog
{
	DECLARE_DYNAMIC(CCIM)

public:
	CCIM(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCIM();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CIM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();	
	afx_msg void OnBnClickedTextload();
	afx_msg void OnBnClickedExcel();
	afx_msg void OnBnClickedCirclebtn();
	afx_msg void OnBnClickedPointbtn();
	afx_msg void OnBnClickedBlockbtn();
	afx_msg void OnBnClickedSet();
	afx_msg void OnBnClickedSpace();
	afx_msg void OnBnClickedColon();
	afx_msg void OnBnClickedPathbtn();
	afx_msg void OnClickedPathbtn();
	afx_msg void OnBnClickedTextbtn();
	afx_msg void OnBnClickedZvalcolorbtn();
	afx_msg void OnBnClickedPointcolorbtn();
	afx_msg void OnBnClickedConlinecolorbtn();
	afx_msg void OnBnClickedText();
	afx_msg void OnBnClickedConline();
	afx_msg void OnBnClickedCutpoint();
	afx_msg void OnBnClickedZvalue();
	afx_msg void OnCbnSelchangeTextlayer();
	afx_msg void OnCbnSelchangeConlinelayer();
	afx_msg void OnBnClickedMinus();
	afx_msg void OnBnClickedPlus();
	afx_msg void OnCbnSelchangeZvaluelayer();
	afx_msg void OnCbnSelchangeCutpointlayer();
	afx_msg void OnBnClickedSelconline();
	afx_msg void OnBnClickedSeltext();
	afx_msg void OnBnClickedSelzvalue();
	afx_msg void OnBnClickedSelpoint();
	
	void initLayer();
	void initBlock();
	void ImpressExcel();
	void ImpressPolyline();
	void ImpressText();	
	void appendText(CString text, AcGePoint3d p3d);	
	void appendCircle(AcGePoint3d p3d);
	void appendBlock(AcGePoint3d p3d);
	void SelectTextColorChange(int colorIndex);
	void SelectZvalColorChange(int colorIndex);
	void SelectConlineColorChange(int colorIndex);
	void SelectPointColorChange(int colorIndex);
	void TextLayerChangeGetColor(CString layerName);
	void ZValLayerChangeGetColor(CString layerName);
	void ConlineLayerChangeGetColor(CString layerName);
	void PointLayerChangeGetColor(CString layerName);
	void appendPoint(AcGePoint3d p3d);
	void append2dPoly(AcDb2dPolyline *p2dPoly);
	void append3dPoly(AcDb3dPolyline *p3dPoly);
	void appendZVal(AcGePoint3d p3d, CString cnt);
	BOOL CALLBACK EnumWindowCallBack(HWND hwnd, LPARAM lParam);
	double getVARIANT(VARIANT& val);

	int ZvalColorIndex;
	int ConlineColIndex;
	int PointColIndex;
	int TextColIndex;
		
	CComboBox CTEXTLAYER;
	CComboBox CUTPNTLAYER;
	CComboBox CCLINELAYER;
	CComboBox ZVALLAYER;
	CComboBox CBName;

	CString TextLayer;
	CString ConLineLayer;
	CString ZValLayer;
	CString PointLayer;

	CButton BCutpoint;
	CButton CConline;
	CButton CText;
	CButton BZIns;
	CButton BZChk;

	CStaticColorBox PointPicColor;
	CStaticColorBox ConPicColor;
	CStaticColorBox ZValPicColor;
	CStaticColorBox TextPicColor;
	
	double PointThickness;	// �� �β�
	double CRad;			// �� ������
	double BScale;			// ��� ��ô
	double TextSize;		// ���� ũ��
	double TextXOff;		// ���� XOffset ũ��
	double TextYOff;		// ���� YOffset ũ��
	double ZTextSize;		// Z���� ũ��
	double ZTextXOff;		// Z���� XOffset ũ��
	double ZTextYOff;		// Z���� YOffset ũ��
	int ZTextPnt;		// Z���� �Ҽ���

	LPDISPATCH saveLP;
	CIniFile iniFile;
	CAcUiSelectButton m_PointSelectButton;
	CAcUiSelectButton m_TextSelectButton;
	CAcUiSelectButton m_LineSelectButton;
	CAcUiSelectButton m_ZValueSelectButton;
	afx_msg void OnBnClickedCimHelp();
};
