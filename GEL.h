#pragma once
#include "resource.h"
#include "IniFile.h"
#include "afxwin.h"
#include "Arx_Math.h"

// CGEL ��ȭ �����Դϴ�.

class CGEL : public CAcUiDialog
{
	DECLARE_DYNAMIC(CGEL)

public:
	CGEL(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGEL();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBcolor();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedChkextend();
	afx_msg void OnBnClickedChkaddpoint();
	afx_msg void OnCbnSelchangeCbxlayer();
	afx_msg void OnBnClickedChkdrawarrow();
	afx_msg void OnBnClickedBlayerselect();
	afx_msg void OnEnUpdateEdttextheight();
	afx_msg void OnDeltaposSpindot(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnEnUpdateEdtscale();
	afx_msg void OnBnClickedHelp();
	afx_msg void OnBnClickedChkptzvalue();


	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	virtual void PostNcDestroy();

	LRESULT onAcadKeepFocus(WPARAM, LPARAM);
	BOOL CheckInsertValue(void);
	void SelectColorChange(int colorIndex);
	void LayerChangeGetColor(CString layerName);
	CString ReturnNumber(double value);
	CString LReturnNumber(double value);
	bool CheckCStringDigit(CString str);
	CString CheckCStringNumber(CString str);
	CString CheckInputNumber(CString str);
	double DTR(double angle);
	
	bool chkAddArrow, chkAddPoint, chkAddPtZvalue;	
	int dotCnt;
	int m_curColorIndex;	//���� �ε���
	int scale;
	double textSize;
	bool loopJob = false;
	char m_selected = 0;	// 0:�� �� ���� 1:������ ����

	CEdit m_TextHeight_Ctrl;
	CString m_TextHeight_Str, m_dotCnt_Str;	
	CString spValue, epValue;
	CString markText, markExtText;
	CString selectCurLayerName;
	CString m_Scale_Str;	// �輺�� �븮 �߰�(������)
	CString m_before_cmd, m_cur_cmd;
	COLORREF btnAfterClr;	//���� ���� ����
	CIniFile iniFile;

	AcDbObjectId vLineId;
	CAcUiComboBox m_CbxLayer;
	CAcUiSelectButton m_SelectBn;		
	
	afx_msg void OnBnClickedChkElContinue();

	void test();
};
