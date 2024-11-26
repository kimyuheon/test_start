#pragma once
#include "resource.h"
#include "IniFile.h"
#include "afxwin.h"
#include "Arx_Math.h"

// CGEL 대화 상자입니다.

class CGEL : public CAcUiDialog
{
	DECLARE_DYNAMIC(CGEL)

public:
	CGEL(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGEL();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	int m_curColorIndex;	//색상 인덱스
	int scale;
	double textSize;
	bool loopJob = false;
	char m_selected = 0;	// 0:두 점 선택 1:폴리선 선택

	CEdit m_TextHeight_Ctrl;
	CString m_TextHeight_Str, m_dotCnt_Str;	
	CString spValue, epValue;
	CString markText, markExtText;
	CString selectCurLayerName;
	CString m_Scale_Str;	// 김성곤 대리 추가(스케일)
	CString m_before_cmd, m_cur_cmd;
	COLORREF btnAfterClr;	//색상 정보 변수
	CIniFile iniFile;

	AcDbObjectId vLineId;
	CAcUiComboBox m_CbxLayer;
	CAcUiSelectButton m_SelectBn;		
	
	afx_msg void OnBnClickedChkElContinue();

	void test();
};
