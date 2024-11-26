#pragma once
#include "resource.h"
#include "IniFile.h"
#include "afxwin.h"
#include "IniFile.h"

// CDSTA ��ȭ �����Դϴ�.
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
	int gcd(int a, int b);	// �ִ�����
	int lcm(int a, int b);	// �ִ�����
	// ���⿡ ���� �� �� ���ϱ�(����)
	AcGePoint3d vecTmpPt(AcDbCurve* pObj, double totalDist, AcGePoint3d Pt, AcGePoint3d tmpPt);
	// ���� �� �׸���
	void BothLineDraw(AcGePoint3d spt, AcGePoint3d ept, AcGePoint3d stmpPt, AcGePoint3d etmpPt, CString curLayerStr);
	// ���� �� �׸���
	void BothMakeCircle(AcGePoint3d spt, AcGePoint3d ept, CString curLayerStr);
	// ���� ���ϱ�
	double getStartOffset(double& Dist, double& Point, double& Result);
	void Draw(AcDbCurve* pObj, AcGePoint3d spt, AcGePoint3d ept, AcDbObjectId curTextStyleId);
	double getRadian(double degree);
	AcDbPolyline* CovertToPLine(AcDbCurve* pObj, AcDbPolyline* pPLine);

	virtual void PostNcDestroy();

	CIniFile iniFile;
	CEdit m_staDistCtrl;		//���� ǥ�� ���� ��Ʈ��
	CEdit m_textDistCtrl;		//���� ǥ�� ���� ��Ʈ��
	CEdit m_textSizeCtrl;		//���� ũ�� ��Ʈ��
	CEdit m_textOffsetCtrl;		//���� �̰� �Ÿ� ��Ʈ��
	CEdit m_staLineLengthCtrl;	//���� ���� ���� ��Ʈ��
	CEdit m_staStartPointCtrl;	//���� ���� ��Ʈ��
	CEdit m_staCircleDistCtrl;	//�� ǥ�� ���� ��Ʈ��
	CAcUiComboBox m_cbxStaLayer;
	CAcUiComboBox m_cbxTextStyle;
	CAcUiComboBox m_cbxTextLayer;
	CAcUiComboBox m_cbxTextPosition;
	CAcUiSelectButton m_staSelectBtn;
	CAcUiSelectButton m_textSelectBtn;
	
	COLORREF m_staColorBtn;		//���� ���� ����
	COLORREF m_textColorBtn;	//���� ���� ����
	
	bool makeText;		//���� ǥ��
	bool makeCircle;	//�� ǥ�� ����
	bool selectStart;	//���� �������� ǥ�� ����
	bool textVisible;	//���� ǥ�� ����
	bool ChangeTextColor; //�ؽ�Ʈ ���� ����
	bool ChangeLayerColor; // ���̾� ���� ����

	int jobMode;		//0: STA,	1:NO
	int m_selectLineColorIndex;
	int m_selectTextColorIndex;

	CString m_staDist;			//���� ǥ�� ����
	CString m_textDist;			//���� ǥ�� ����
	CString m_textSize;			//���� ũ��
	CString m_textOffset;		//���� �̰� �Ÿ�
	CString m_staLineLength;	//���� ���� ����
	CString m_staStartPoint;	//���� ����
	CString m_staCircleDist;	//�� ǥ�� ����
	CString m_decimalPosition;	//���� �Ҽ��� ��ġ
	CString curLayerStr;
	CString curTextStr; 
	CString curTextStyleStr;
	virtual BOOL DestroyWindow();

	AcGePoint3d spt, ept;
	double txt;
	AcGePoint3d as_pt;
	
	afx_msg void OnBnClickedDstaHelp();
};