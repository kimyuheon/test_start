#pragma once
#include "resource.h"
#include "IniFile.h"
#include "afxwin.h"
#include "IniFile.h"

// CDSTA 대화 상자입니다.
class CDSTA : public CAcUiDialog{
	DECLARE_DYNAMIC(CDSTA)
public:
	CDSTA(CWnd* pParent = NULL);
	virtual ~CDSTA();
	enum {IDD = IDD_DSTA};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadno();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadsta();
	afx_msg void OnBnClickedChktext();
	afx_msg void OnEnUpdateEdtstadist();
	afx_msg void OnEnUpdateEdtstaline();
	afx_msg void OnEnUpdateEdtstastart();
	afx_msg void OnEnUpdateEdttextdist();
	afx_msg void OnEnUpdateEdttextsize();
	afx_msg void OnEnUpdateEdtstacircle();
	afx_msg void OnBnClickedChkstastart();
	afx_msg void OnBnClickedBtnstacolor();
	afx_msg void OnEnUpdateEdttextoffset();
	afx_msg void OnBnClickedBtnstaselect();
	afx_msg void OnBnClickedChkstacircle();
	afx_msg void OnBnClickedBtntextcolor();
	afx_msg void OnBnClickedBtntextselect();
	afx_msg void OnCbnSelchangeCbxstalayer();
	afx_msg void OnBnClickedChktextvisible();
	afx_msg void OnCbnSelchangeCbxtextstyle();
	afx_msg void OnCbnSelchangeCbxtextlayer();
	afx_msg void OnDeltaposSpndecimalpoint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	LRESULT onAcadKeepFocus(WPARAM, LPARAM);

	int LayerChangeGetColor(CString layerName);
	void SelectColorChange(int colorIndex, int stat);
	AcDbObjectId findObjectLayer(CString name);
	AcDbObjectId findTextStyle(CString name);
	int gcd(int a, int b);	// 최대공약수
	int lcm(int a, int b);	// 최대공배수
	// 방향에 따른 두 점 구하기(벡터)
	AcGePoint3d vecTmpPt(AcDbCurve* pObj, double totalDist, AcGePoint3d Pt, AcGePoint3d tmpPt);
	// 양쪽 선 그리기
	void BothLineDraw(AcGePoint3d spt, AcGePoint3d ept, AcGePoint3d stmpPt, AcGePoint3d etmpPt, CString curLayerStr);
	// 양쪽 원 그리기
	void BothMakeCircle(AcGePoint3d spt, AcGePoint3d ept, CString curLayerStr);
	// 간격 구하기
	double getStartOffset(double& Dist, double& Point, double& Result);
	void Draw(AcDbCurve* pObj, AcGePoint3d spt, AcGePoint3d ept, AcDbObjectId curTextStyleId);
	double getRadian(double degree);
	AcDbPolyline* CovertToPLine(AcDbCurve* pObj, AcDbPolyline* pPLine);

	virtual void PostNcDestroy();

	CIniFile iniFile;
	CEdit m_staDistCtrl;		//측점 표시 간격 컨트롤
	CEdit m_textDistCtrl;		//문자 표기 간격 컨트롤
	CEdit m_textSizeCtrl;		//문자 크기 컨트롤
	CEdit m_textOffsetCtrl;		//문자 이격 거리 컨트롤
	CEdit m_staLineLengthCtrl;	//측점 선의 길이 컨트롤
	CEdit m_staStartPointCtrl;	//시작 측점 컨트롤
	CEdit m_staCircleDistCtrl;	//원 표시 간격 컨트롤
	CAcUiComboBox m_cbxStaLayer;
	CAcUiComboBox m_cbxTextStyle;
	CAcUiComboBox m_cbxTextLayer;
	CAcUiComboBox m_cbxTextPosition;
	CAcUiSelectButton m_staSelectBtn;
	CAcUiSelectButton m_textSelectBtn;
	
	COLORREF m_staColorBtn;		//색상 정보 변수
	COLORREF m_textColorBtn;	//색상 정보 변수
	
	bool makeText;		//문자 표시
	bool makeCircle;	//원 표기 간격
	bool selectStart;	//선택 지점부터 표기 시작
	bool textVisible;	//문자 표기 생략
	bool ChangeTextColor; //텍스트 색상 변경
	bool ChangeLayerColor; // 레이어 색상 변경

	int jobMode;		//0: STA,	1:NO
	int m_selectLineColorIndex;
	int m_selectTextColorIndex;

	CString m_staDist;			//측점 표시 간격
	CString m_textDist;			//문자 표기 간격
	CString m_textSize;			//문자 크기
	CString m_textOffset;		//문자 이격 거리
	CString m_staLineLength;	//측점 선의 길이
	CString m_staStartPoint;	//시작 측점
	CString m_staCircleDist;	//원 표시 간격
	CString m_decimalPosition;	//문자 소수점 위치
	CString curLayerStr;
	CString curTextStr; 
	CString curTextStyleStr;
	virtual BOOL DestroyWindow();

	AcGePoint3d spt, ept;
	double txt;
	AcGePoint3d as_pt;
	
	afx_msg void OnBnClickedDstaHelp();
};