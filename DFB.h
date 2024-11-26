#pragma once
#include "StdAfx.h"
#include "resource.h"
#include "afxwin.h"
#include "IniFile.h"

// CDSLO 대화 상자입니다.
class CDFB : public CAcUiDialog{
	DECLARE_DYNAMIC(CDFB)
public:
	CDFB(CWnd* pParent = NULL);
	virtual ~CDFB();
	enum {IDD = IDD_DFB };
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
	
	int m_nEditSubDraw;
	int m_nEditBaseDraw;
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);

	 
	AcDbObjectId GetLayer(CString& LayerName,CString msg);
	int GetlayerColor(AcDbObjectId LyId);
	AcDbObjectId CreateAcDbLine(AcGePoint3d& sp, AcGePoint3d& ep, CString strLayerName,int color_index );
	Acad::ErrorStatus postToDatabase (AcDbEntity *pEnt, AcDbObjectId& EntID);
	BOOL SSGET(AcDbObjectIdArray& ids,CString strfilter);
	void ss2Array(ads_name ss, AcDbObjectIdArray& ids);
	BOOL FindLayer(CString name,AcDbObjectId& LyId);
	BOOL createLayer(CString name,int ColorIndex,AcDbObjectId& LyId);
	afx_msg void OnBnClickedDfbHelp();
};