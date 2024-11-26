#pragma once
#include "resource.h"
#include "afxwin.h"
#include "IniFile.h"

// CDSLO 대화 상자입니다.
class CDSLO : public CAcUiDialog {
	DECLARE_DYNAMIC(CDSLO)
public:
	CDSLO(CWnd* pParent = NULL);
	virtual ~CDSLO();
	enum {IDD = IDD_DSLO};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnUpdateEdit1();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBcolor();
	afx_msg void OnCbnSelchangeCbxlayer();
	afx_msg void OnBnClickedBlayerselect();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedClayer();

	double GetRadian(double degree);
	void SelectColorChange(int colorIndex);
	void LayerChangeGetColor(CString layerName);
	CString CheckInputNumber(CString str);
	
	Acad::ErrorStatus es;

	CIniFile iniFile;
	CComboBox m_cbxLayer;
	CEdit m_EditBaseDraw_Ctrl;
	CAcUiSelectButton m_selectBn;
		
	LPCTSTR setLayer;
	COLORREF btnAfterClr;	//색상 정보 변수

	int jobStat;
	int m_curColorIndex;	//색상 인덱스
	int cbxLayerInt;
	int cbxTextLayerInt;

	CString m_EditSubDraw_Str;
	CString m_EditBaseDraw_Str;
	CString CmbLayer;
	CString cbxLayerInt_Str;
	BOOL bClayer;
	afx_msg void OnBnClickedDsloHelp();
};