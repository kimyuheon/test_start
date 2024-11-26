// DlgAddDrawingNumber.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgAddDrawingNumber.h"
#include "afxdialogex.h"
#include "WKUtils.h"
#include "TextProcess.h"
#include "DlgAttrubuteView.h"

extern CDlgAddDrawingNumber* Maindlg;

// CDlgAddDrawingNumber 대화 상자입니다.
template<class T>
void QSortCArray( T& t, int (__cdecl *compare )(const void *elem1, const void *elem2 ) )
{
	if( t.GetSize() <= 0 ) return;

	qsort( t.GetData(), t.GetSize(), sizeof( t[0] ), compare );
}	

// 내림차순 정렬
int CompareStringDescending_(const void *elem1, const void *elem2){
	FileInfos_* p1 = (FileInfos_*)elem1;
	FileInfos_* p2 = (FileInfos_*)elem2;
	if(p1->filename > p2->filename)	return +1;
	if(p1->filename < p2->filename)	return -1;

	return 0;
}

// 오름차순 정렬
int CompareStringAscending_(const void *elem1, const void *elem2){
	FileInfos_* p1 = (FileInfos_*)elem1;
	FileInfos_* p2 = (FileInfos_*)elem2;
	if(p1->filename > p2->filename) return -1;
	if(p1->filename < p2->filename) return +1;

	return 0;
}

//X좌표를 기준으로 정렬오름차순 
int ComparePointX_( const void *elem1, const void *elem2 )
{

	CPrintFormData_ * ent1 = (CPrintFormData_ *)elem1;
	CPrintFormData_* ent2 = (CPrintFormData_ *)elem2;

	if( ent1->minpoint_X > ent2->minpoint_X ) return +1;
	if( ent1->minpoint_X < ent2->minpoint_X ) return -1;

	return 0;
}
//Y좌표를 기준으로 정렬오름차순
int ComparePointY_( const void *elem1, const void *elem2 )
{

	CPrintFormData_ * ent1 = (CPrintFormData_ *)elem1;
	CPrintFormData_* ent2 = (CPrintFormData_ *)elem2;

	if( ent1->minpoint_Y > ent2->minpoint_Y ) return +1;
	if( ent1->minpoint_Y < ent2->minpoint_Y ) return -1;

	return 0;
}
//X좌표를 기준으로 정렬내림차순 
int ComparePointX_De_( const void *elem1, const void *elem2 )
{

	CPrintFormData_ * ent1 = (CPrintFormData_ *)elem1;
	CPrintFormData_* ent2 = (CPrintFormData_ *)elem2;

	if( ent1->minpoint_X > ent2->minpoint_X ) return -1;
	if( ent1->minpoint_X < ent2->minpoint_X ) return +1;

	return 0;
}
//Y좌표를 기준으로 정렬내림차순 
int ComparePointY_De_( const void *elem1, const void *elem2 )
{

	CPrintFormData_ * ent1 = (CPrintFormData_ *)elem1;
	CPrintFormData_* ent2 = (CPrintFormData_ *)elem2;

	if( ent1->minpoint_Y > ent2->minpoint_Y ) return -1;
	if( ent1->minpoint_Y < ent2->minpoint_Y ) return +1;

	return 0;
}
IMPLEMENT_DYNAMIC(CDlgAddDrawingNumber, CAcUiDialog)

CDlgAddDrawingNumber::CDlgAddDrawingNumber(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CDlgAddDrawingNumber::IDD, pParent)
	, m_WriteTarget(0)
	, m_FormOption_FormName(_T(""))
	, m_FormOption_FormLocation(_T(""))
	, m_InsertText_Target(1)
	, m_strDwgName_Prefix(_T(""))
	, m_iDwgNumber_Start(1)
	, m_strDwgInstartText(_T(""))
	, m_bInsertTextIncrease(TRUE)
	, m_InsertText_StartNumber(1)
	, m_iInsertTextDigit(1)
	, m_preview(_T(""))
	, m_bDwgNameWrite(FALSE)
	, m_iSort(0)
	, m_dblSortTolerance(10)
	, m_strTextOption_Height(5.000)
	, m_strTextOption_Width(1.000)
	, m_bOldTextRemove(TRUE)
	, m_strDwgFileName_Location(_T(""))
	, m_dblDwgFileName_Height(5.000)
	, m_dblDwgFileName_Width(1.000)
	, m_bDwgFileName_Fullpath(FALSE)
	, m_bDwgFileName_IncExt(FALSE)
	, m_bDwgFileName_UseField(FALSE)
	, m_formOption(0)
	, m_Color(256)
	, m_Color1(256)

{

}
LRESULT CDlgAddDrawingNumber::onAcadKeepFocus(WPARAM, LPARAM){
	return TRUE;
}

CDlgAddDrawingNumber::~CDlgAddDrawingNumber()
{
}

void CDlgAddDrawingNumber::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WRITE_TARGET, m_FormTarget);
	DDX_Text(pDX, IDC_EDIT_FORMOPTION_FORM_NAME, m_FormOption_FormName);
	DDX_Text(pDX, IDC_EDIT_FORMOPTION_INSERT_POINT, m_FormOption_FormLocation);
	DDX_Text(pDX, IDC_EDIT_DWG_NUMBER_PREFIX, m_strDwgName_Prefix);
	DDX_Text(pDX, IDC_EDIT_DWG_START_NUMBER, m_iDwgNumber_Start);
	DDX_Text(pDX, IDC_EDIT_INSERT_TEXT, m_strDwgInstartText);
	DDX_Check(pDX, IDC_CHECK_INCREASE, m_bInsertTextIncrease);
	DDX_Text(pDX, IDC_EDIT_INCREASE_START_NUMBER, m_InsertText_StartNumber);
	DDX_Text(pDX, IDC_EDIT_INCREASE_DIGIT, m_iInsertTextDigit);
	DDX_Text(pDX, IDC_STATIC_PREVIEW, m_preview);
	DDX_Check(pDX, IDC_CHECK_WRITE_DWG_NAME, m_bDwgNameWrite);
	DDX_Radio(pDX, IDC_RADIO_ZSORT, m_iSort);
	DDX_Text(pDX, IDC_EDIT_TOLERANCE, m_dblSortTolerance);
	DDX_Control(pDX, IDC_COMBO_TEXT_OPTION_FONT, m_cbTextOption_Font);
	DDX_Control(pDX, IDC_COMBO_TEXT_OPTION_LAYER, m_cbTextOption_Layer);
	DDX_Text(pDX, IDC_EDIT_TEXT_OPTION_TEXT_HEIGHT, m_strTextOption_Height);
	DDX_Text(pDX, IDC_EDIT_TEXT_OPTION_TEXT_WIDTH, m_strTextOption_Width);
	DDX_Control(pDX, IDC_COMBO_TEXT_OPTION_ANGLE, m_cbTextOption_Angle);
	DDX_Check(pDX, IDC_CHECK_TEXT_OPTION_OLD_REMOVE, m_bOldTextRemove);
	DDX_Text(pDX, IDC_EDIT_WRITE_DWG_NAME_POINT, m_strDwgFileName_Location);
	DDX_Control(pDX, IDC_COMBO_WRITE_DWG_NAME_FONT, m_cbDwgFileName_Font);
	DDX_Control(pDX, IDC_COMBO_WRITE_DWG_NAME_LAYER, m_cbDwgFileName_Layer);
	DDX_Text(pDX, IDC_EDIT_WRITE_DWG_NAME_TEXT_HEIGHT, m_dblDwgFileName_Height);
	DDX_Text(pDX, IDC_EDIT_WRITE_DWG_NAME_TEXT_WIDTH, m_dblDwgFileName_Width);
	DDX_Control(pDX, IDC_COMBO_WRITE_DWG_NAME_ALIGN, m_cbDwgFileName_Sort);
	DDX_Control(pDX, IDC_COMBO_WRITE_DWG_NAME_TEXT_ANGLE, m_cbDwgFileName_Angle);
	DDX_Check(pDX, IDC_CHECK_WRITE_DWG_NAME_FULLPATH, m_bDwgFileName_Fullpath);
	DDX_Check(pDX, IDC_CHECK_WRITE_DWG_NAME_INC_EXT, m_bDwgFileName_IncExt);
	DDX_Check(pDX, IDC_CHECK_WRITE_DWG_NAME_FIELD, m_bDwgFileName_UseField);
	DDX_Radio(pDX, IDC_RADIO_FORM_OPTION1, m_formOption);
	DDX_Radio(pDX, IDC_RADIO_TARGET1, m_WriteTarget);
	DDX_Radio(pDX, IDC_RADIO_WRITE_TYPE1, m_InsertText_Target);
	DDX_Control(pDX, IDC_STATIC_COLOR_ATTR, m_Btn_Color);
	DDX_Control(pDX, IDC_STATIC_COLOR_ATTR2, m_Btn_Color2);
	DDX_Control(pDX, IDC_LIST_DWG, m_FileView);
}


BEGIN_MESSAGE_MAP(CDlgAddDrawingNumber, CAcUiDialog)
		ON_MESSAGE (WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_RADIO_FORM_OPTION2, &CDlgAddDrawingNumber::OnBnClickedRadioFormOption)
	ON_BN_CLICKED(IDC_RADIO_FORM_OPTION1, &CDlgAddDrawingNumber::OnBnClickedRadioFormOption)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgAddDrawingNumber::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlgAddDrawingNumber::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_RADIO_WRITE_TYPE1, &CDlgAddDrawingNumber::OnBnClickedRadioWriteType)
	ON_BN_CLICKED(IDC_RADIO_WRITE_TYPE2, &CDlgAddDrawingNumber::OnBnClickedRadioWriteType)
	ON_BN_CLICKED(IDC_RADIO_WRITE_TYPE3, &CDlgAddDrawingNumber::OnBnClickedRadioWriteType)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DIGIT, &CDlgAddDrawingNumber::OnDeltaposSpinDigit)
	ON_BN_CLICKED(IDC_CHECK_WRITE_DWG_NAME, &CDlgAddDrawingNumber::OnBnClickedCheckWriteDwgName)
	ON_BN_CLICKED(IDC_RADIO_TARGET1, &CDlgAddDrawingNumber::OnBnClickedRadioTarget)
	ON_BN_CLICKED(IDC_RADIO_TARGET2, &CDlgAddDrawingNumber::OnBnClickedRadioTarget)
	ON_BN_CLICKED(IDC_BUTTON_TEXT_OPTION_FONT, &CDlgAddDrawingNumber::OnBnClickedButtonTextOptionFont)
	ON_BN_CLICKED(IDC_BUTTON_TEXT_OPTION_LAYER, &CDlgAddDrawingNumber::OnBnClickedButtonTextOptionLayer)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_DWG_NAME_FONT, &CDlgAddDrawingNumber::OnBnClickedButtonWriteDwgNameFont)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_DWG_NAME_LAYER, &CDlgAddDrawingNumber::OnBnClickedButtonWriteDwgNameLayer)
	ON_EN_CHANGE(IDC_EDIT_DWG_NUMBER_PREFIX, &CDlgAddDrawingNumber::OnEnChangeEditDwgNumberPrefix)
	ON_EN_CHANGE(IDC_EDIT_DWG_START_NUMBER, &CDlgAddDrawingNumber::OnEnChangeEditDwgStartNumber)
	ON_EN_CHANGE(IDC_EDIT_INCREASE_DIGIT, &CDlgAddDrawingNumber::OnEnChangeEditIncreaseDigit)
	ON_EN_CHANGE(IDC_EDIT_INCREASE_START_NUMBER, &CDlgAddDrawingNumber::OnEnChangeEditIncreaseStartNumber)
	ON_EN_CHANGE(IDC_EDIT_INSERT_TEXT, &CDlgAddDrawingNumber::OnEnChangeEditInsertText)
	ON_BN_CLICKED(IDC_BUTTON_FORMOPTION_FORM_NAME, &CDlgAddDrawingNumber::OnBnClickedButtonFormoptionFormName)
	ON_BN_CLICKED(IDC_BUTTON_FORMOPTION_INSERT_POINT, &CDlgAddDrawingNumber::OnBnClickedButtonFormoptionInsertPoint)
	ON_BN_CLICKED(IDC_BUTTON_GO1, &CDlgAddDrawingNumber::OnBnClickedButtonGo1)
	ON_BN_CLICKED(IDC_BUTTON_GO2, &CDlgAddDrawingNumber::OnBnClickedButtonGo2)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_DWG_NAME_POINT, &CDlgAddDrawingNumber::OnBnClickedButtonWriteDwgNamePoint)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_ADD_ACTIVE_DWG, &CDlgAddDrawingNumber::OnBnClickedButtonAddActiveDwg)
	ON_BN_CLICKED(IDC_BUTTON_ADD_OPEN_DWG, &CDlgAddDrawingNumber::OnBnClickedButtonAddOpenDwg)
	ON_BN_CLICKED(IDC_BUTTONT_TOP, &CDlgAddDrawingNumber::OnBnClickedButtontTop)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CDlgAddDrawingNumber::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CDlgAddDrawingNumber::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_BOTTOM, &CDlgAddDrawingNumber::OnBnClickedButtonBottom)
	ON_BN_CLICKED(IDC_BUTTON_SEL_REMOVE, &CDlgAddDrawingNumber::OnBnClickedButtonSelRemove)
	ON_BN_CLICKED(IDC_BUTTON_ALL_REMOVE, &CDlgAddDrawingNumber::OnBnClickedButtonAllRemove)
	ON_BN_CLICKED(IDC_BUTTON_ADD_DWG, &CDlgAddDrawingNumber::OnBnClickedButtonAddDwg)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FOLDER, &CDlgAddDrawingNumber::OnBnClickedButtonAddFolder)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CDlgAddDrawingNumber::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_LIST_SAVE, &CDlgAddDrawingNumber::OnBnClickedButtonListSave)
	ON_BN_CLICKED(IDC_BUTTON_LIST_OPEN, &CDlgAddDrawingNumber::OnBnClickedButtonListOpen)
	ON_BN_CLICKED(IDC_BUTTON_LIST_SORT, &CDlgAddDrawingNumber::OnBnClickedButtonListSort)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CDlgAddDrawingNumber::OnBnClickedButtonExit)
	ON_STN_CLICKED(IDC_SORT1, &CDlgAddDrawingNumber::OnSortClick1)
	ON_STN_CLICKED(IDC_SORT2, &CDlgAddDrawingNumber::OnSortClick2)
	ON_STN_CLICKED(IDC_SORT3, &CDlgAddDrawingNumber::OnSortClick3)
	ON_STN_CLICKED(IDC_SORT4, &CDlgAddDrawingNumber::OnSortClick4)
	//ON_WM_DESTROY()
	//ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgAddDrawingNumber 메시지 처리기입니다.
void CDlgAddDrawingNumber::OnBnClickedRadioFormOption()
{
	UpdateData();
	if(m_formOption ==0){
		GetDlgItem(IDC_STATIC_INSERT_LOCATION)->SetWindowText(_T("삽입 위치 : "));
		GetDlgItem(IDC_BUTTON_FORMOPTION_INSERT_POINT)->SetWindowText(_T("위치선택"));

		GetDlgItem(IDC_STATIC_TEXT_OPTION_FONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TEXT_OPTION_FONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_TEXT_OPTION_FONT)->EnableWindow(TRUE);

		GetDlgItem(IDC_STATIC_TEXT_OPTION_LAYER)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TEXT_OPTION_LAYER)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_TEXT_OPTION_LAYER)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_COLOR_ATTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_COL)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_TEXT_OPTION_HEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEXT_OPTION_TEXT_HEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_TEXT_OPTION_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEXT_OPTION_TEXT_WIDTH)->EnableWindow(TRUE);

		GetDlgItem(IDC_STATIC_TEXT_OPTION_ANGLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TEXT_OPTION_ANGLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_TEXT_OPTION_OLD_REMOVE)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDC_STATIC_INSERT_LOCATION)->SetWindowText(_T("속성 이름 : "));
		GetDlgItem(IDC_BUTTON_FORMOPTION_INSERT_POINT)->SetWindowText(_T("속성선택"));

		GetDlgItem(IDC_STATIC_TEXT_OPTION_FONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TEXT_OPTION_FONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TEXT_OPTION_FONT)->EnableWindow(FALSE);

		GetDlgItem(IDC_STATIC_TEXT_OPTION_LAYER)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TEXT_OPTION_LAYER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TEXT_OPTION_LAYER)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_COLOR_ATTR)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_COL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TEXT_OPTION_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEXT_OPTION_TEXT_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TEXT_OPTION_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEXT_OPTION_TEXT_WIDTH)->EnableWindow(FALSE);

		GetDlgItem(IDC_STATIC_TEXT_OPTION_ANGLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TEXT_OPTION_ANGLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_TEXT_OPTION_OLD_REMOVE)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}


void CDlgAddDrawingNumber::ColorSetting(int newColorIndex,CStatic& ColorBox,int colorNameCrtl)
{
	CString ColorName;
	CRect prect;
	ColorBox.GetClientRect (& prect); // get the zone

	if(newColorIndex ==256)
		ColorName = _T("By Layer");
	else if(newColorIndex ==0)
		ColorName = _T("By Block");
	else
		ColorName.Format(_T("%d"),newColorIndex);

	unsigned long rgb = acedGetRGB(newColorIndex); 
	int r = GetRValue(rgb);
	int g = GetGValue(rgb);
	int b = GetBValue(rgb);

	FillRect (ColorBox.GetDC () -> GetSafeHdc (), & prect, CBrush (RGB (r,g,b))); // fill color 

	((CButton*) GetDlgItem(colorNameCrtl))->SetWindowText(ColorName);
}

void CDlgAddDrawingNumber::OnBnClickedButton5()
{
	UpdateData();
	CWKUtils Utils;
	Utils.GetColorPicker(m_Color);

	UpdateData(FALSE);
	ColorSetting(m_Color,m_Btn_Color,IDC_STATIC_COL);
}


void CDlgAddDrawingNumber::OnBnClickedButton7()
{
	UpdateData();
	CWKUtils Utils;
	Utils.GetColorPicker(m_Color1);

	UpdateData(FALSE);
	ColorSetting(m_Color1,m_Btn_Color2,IDC_STATIC_COL2);
}


void CDlgAddDrawingNumber::OnBnClickedRadioWriteType()
{
	UpdateData();
	
	if(m_InsertText_Target ==0)
	{

		GetDlgItem(IDC_EDIT_DWG_NUMBER_PREFIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DWG_START_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INSERT_TEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_INCREASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_INCREASE_START_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INCREASE_START_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_INCREASE_DIGIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INCREASE_DIGIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_DIGIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PREVIEW)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PREVIEW_LABEL)->EnableWindow(FALSE);
			
	}
	else if(m_InsertText_Target ==1)
	{
		GetDlgItem(IDC_EDIT_DWG_NUMBER_PREFIX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DWG_START_NUMBER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_INSERT_TEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_INCREASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_INCREASE_START_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INCREASE_START_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_INCREASE_DIGIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_INCREASE_DIGIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_DIGIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PREVIEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PREVIEW_LABEL)->EnableWindow(TRUE);
	}
	else if(m_InsertText_Target ==2)
	{
		GetDlgItem(IDC_EDIT_DWG_NUMBER_PREFIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DWG_START_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INSERT_TEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_INCREASE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_INCREASE_START_NUMBER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_INCREASE_START_NUMBER)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_INCREASE_DIGIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_INCREASE_DIGIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_DIGIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PREVIEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PREVIEW_LABEL)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}


void CDlgAddDrawingNumber::OnDeltaposSpinDigit(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta <0){
		if(m_iInsertTextDigit >=10)return;
		m_iInsertTextDigit++;
	}else{
		if(m_iInsertTextDigit <=1)return;
		m_iInsertTextDigit--;
	}
	UpdateData(FALSE);
	makeDwgNumber();
	
	*pResult = 0;
}


void CDlgAddDrawingNumber::OnBnClickedCheckWriteDwgName()
{
	UpdateData();

	if(m_bDwgNameWrite){
		GetDlgItem(IDC_BUTTON_WRITE_DWG_NAME_POINT			)->EnableWindow(TRUE);
		GetDlgItem(	IDC_EDIT_WRITE_DWG_NAME_POINT			)->EnableWindow(TRUE);
		GetDlgItem(	IDC_CHECK_WRITE_DWG_NAME_FULLPATH		)->EnableWindow(TRUE);
		GetDlgItem(	IDC_CHECK_WRITE_DWG_NAME_INC_EXT		)->EnableWindow(TRUE);
		GetDlgItem(	IDC_CHECK_WRITE_DWG_NAME_FIELD			)->EnableWindow(TRUE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_FONT			)->EnableWindow(TRUE);
		GetDlgItem(	IDC_COMBO_WRITE_DWG_NAME_FONT			)->EnableWindow(TRUE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_LAYER			)->EnableWindow(TRUE);
		GetDlgItem(	IDC_BUTTON7								)->EnableWindow(TRUE);
		GetDlgItem(	IDC_STATIC_COLOR_ATTR2					)->EnableWindow(TRUE);
		GetDlgItem(	IDC_STATIC_COL2							)->EnableWindow(TRUE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_TEXT_HEIGHT	)->EnableWindow(TRUE);
		GetDlgItem(	IDC_EDIT_WRITE_DWG_NAME_TEXT_HEIGHT		)->EnableWindow(TRUE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_TEXT_WIDTH	)->EnableWindow(TRUE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_ALIGN			)->EnableWindow(TRUE);
		GetDlgItem(	IDC_COMBO_WRITE_DWG_NAME_ALIGN			)->EnableWindow(TRUE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_TEXT_ANGLE	)->EnableWindow(TRUE);
		GetDlgItem(	IDC_COMBO_WRITE_DWG_NAME_TEXT_ANGLE		)->EnableWindow(TRUE);
		GetDlgItem(	IDC_EDIT_WRITE_DWG_NAME_TEXT_WIDTH		)->EnableWindow(TRUE);
		GetDlgItem(	IDC_BUTTON_WRITE_DWG_NAME_FONT		)->EnableWindow(TRUE);
		GetDlgItem(	IDC_BUTTON_WRITE_DWG_NAME_LAYER		)->EnableWindow(TRUE);
		
			
	}else{
		GetDlgItem(IDC_BUTTON_WRITE_DWG_NAME_POINT			)->EnableWindow(FALSE);
		GetDlgItem(	IDC_EDIT_WRITE_DWG_NAME_POINT			)->EnableWindow(FALSE);
		GetDlgItem(	IDC_CHECK_WRITE_DWG_NAME_FULLPATH		)->EnableWindow(FALSE);
		GetDlgItem(	IDC_CHECK_WRITE_DWG_NAME_INC_EXT		)->EnableWindow(FALSE);
		GetDlgItem(	IDC_CHECK_WRITE_DWG_NAME_FIELD			)->EnableWindow(FALSE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_FONT			)->EnableWindow(FALSE);
		GetDlgItem(	IDC_COMBO_WRITE_DWG_NAME_FONT			)->EnableWindow(FALSE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_LAYER			)->EnableWindow(FALSE);
		GetDlgItem(	IDC_BUTTON7								)->EnableWindow(FALSE);
		GetDlgItem(	IDC_STATIC_COLOR_ATTR2					)->EnableWindow(FALSE);
		GetDlgItem(	IDC_STATIC_COL2							)->EnableWindow(FALSE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_TEXT_HEIGHT	)->EnableWindow(FALSE);
		GetDlgItem(	IDC_EDIT_WRITE_DWG_NAME_TEXT_HEIGHT		)->EnableWindow(FALSE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_TEXT_WIDTH	)->EnableWindow(FALSE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_ALIGN			)->EnableWindow(FALSE);
		GetDlgItem(	IDC_COMBO_WRITE_DWG_NAME_ALIGN			)->EnableWindow(FALSE);
		GetDlgItem(	IDC_STATIC_WRITE_DWG_NAME_TEXT_ANGLE	)->EnableWindow(FALSE);
		GetDlgItem(	IDC_COMBO_WRITE_DWG_NAME_TEXT_ANGLE		)->EnableWindow(FALSE);
		GetDlgItem(	IDC_EDIT_WRITE_DWG_NAME_TEXT_WIDTH		)->EnableWindow(FALSE);
		GetDlgItem(	IDC_BUTTON_WRITE_DWG_NAME_FONT		)->EnableWindow(FALSE);
		GetDlgItem(	IDC_BUTTON_WRITE_DWG_NAME_LAYER		)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

//다이얼로그의 사이즈를 늘였다 줄였다 하기 
void CDlgAddDrawingNumber::DlgChangeSize(){
	UpdateData();
	HWND hwndBox = ::GetDlgItem(this->m_hWnd, IDD_ADN);
	//MONITORINFO mi;
		
	//RECT rc3;
	//GetWindowRect(&rc3);

	// 접히는 너비 구하기
	GetDlgItem(IDC_LIST_DWG)->GetWindowRect(&Boxrect);
	m_Fold_Width = MainForm_Size.right - Boxrect.left;

	//double sidesize = 391;
	double sidesize = (double)m_Fold_Width;
	if(m_WriteTarget ==0){
		GetDlgItem(IDC_BUTTON_GO1)->SetWindowTextW(_T("전체 Form"));
		GetDlgItem(IDC_BUTTON_GO2)->EnableWindow(TRUE);
		
		this->SetWindowPos(NULL, (int)MainForm_Size.left, (int)MainForm_Size.top, MainForm_Size.Width() - (int)sidesize, MainForm_Size.Height(), 0);
		/*HMONITOR monitor = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
		if(monitor){
			mi.cbSize = sizeof(mi);
			GetMonitorInfo(monitor, &mi);
			double dlgx = (((MainForm_Size.right - MainForm_Size.left) - sidesize)) ;
			double x = ((((int)mi.rcMonitor.right - (int)mi.rcMonitor.left ) / 2) + (int)mi.rcMonitor.left) - (int)(dlgx / 2);
			MoveWindow((int)x, (int)rc3.top, (int)dlgx, MainForm_Size.Height());
		}*/
	}
	else{
		GetDlgItem(IDC_BUTTON_GO1)->SetWindowTextW(_T("실행"));
		GetDlgItem(IDC_BUTTON_GO2)->EnableWindow(FALSE);
		this->SetWindowPos(NULL, (int)MainForm_Size.left, (int)MainForm_Size.top, MainForm_Size.Width()/* + (int)sidesize*/, MainForm_Size.Height(), 0);
		/*HMONITOR monitor = MonitorFromWindow(this->m_hWnd, MONITOR_DEFAULTTONEAREST);
		if(monitor){
			mi.cbSize = sizeof(mi);
			GetMonitorInfo(monitor, &mi);
			double dlgx = (((MainForm_Size.right - MainForm_Size.left))) ;
			double x = (((mi.rcMonitor.right - mi.rcMonitor.left) / 2) + mi.rcMonitor.left) - (dlgx / 2);
			MoveWindow((int)x, (int)rc3.top,(int)dlgx, MainForm_Size.Height());
		}*/
	}
}

BOOL CDlgAddDrawingNumber::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	UpdateData();
	InitLitView();
	this->GetWindowRect(MainForm_Size);
	DlgChangeSize();
	OnBnClickedRadioFormOption();
	OnBnClickedRadioWriteType();
	OnBnClickedCheckWriteDwgName();

	BtSrot1 = (CButton*)GetDlgItem(IDC_RADIO_ZSORT);
	BtSrot2 = (CButton*)GetDlgItem(IDC_RADIO_NSORT);
	BtSrot3 = (CButton*)GetDlgItem(IDC_RADIO_NMSORT);
	BtSrot4 = (CButton*)GetDlgItem(IDB_BITMAP_ORDER4);

	CStringArray txtStylelst, layerlst;
	CWKUtils utils;
	utils.getTextStyleList(txtStylelst);
	for(int i=0; i<txtStylelst.GetCount();i++){
		m_cbTextOption_Font.AddString(txtStylelst[i]);
		m_cbDwgFileName_Font.AddString(txtStylelst[i]);
	}

	utils.GetAllLayers(layerlst);
	for(int i=0; i<layerlst.GetCount();i++){
		m_cbTextOption_Layer.AddString(layerlst[i]);
		m_cbDwgFileName_Layer.AddString(layerlst[i]);
	}
	m_cbDwgFileName_Font.SetCurSel(0);
	m_cbDwgFileName_Layer.SetCurSel(0);
	m_cbTextOption_Font.SetCurSel(0);
	m_cbTextOption_Layer.SetCurSel(0);
	m_cbTextOption_Angle.SetCurSel(0);
	m_cbDwgFileName_Angle.SetCurSel(0);
	m_cbDwgFileName_Sort.SetCurSel(0);
	m_FormTarget.SetCurSel(0);

	

	UpdateData(FALSE);


	return TRUE;
}


void CDlgAddDrawingNumber::OnBnClickedRadioTarget()
{
	DlgChangeSize();
}




void CDlgAddDrawingNumber::OnBnClickedButtonTextOptionFont()
{
	BeginEditorCommand();
	UpdateData();
	CWKUtils utils;
	AcDbObjectIdArray ids;
	utils.SSGET(ids,_T("TEXT"));
	if(ids.length()==0){CompleteEditorCommand(); return;}
	CString strStyleName;
 	utils.GetTextStyleName(ids.first(),strStyleName);

	int idx = m_cbTextOption_Font.FindString(0,strStyleName);
	m_cbTextOption_Font.SetCurSel(idx);
	UpdateData(FALSE);
	CompleteEditorCommand();

}


void CDlgAddDrawingNumber::OnBnClickedButtonTextOptionLayer()
{
	BeginEditorCommand();
	UpdateData();
	CWKUtils utils;
	CString layerName;
	utils.GetLayer(layerName,_T("\n레이어를 가져올 객체 선택:"));

	int idx = m_cbTextOption_Layer.FindString(0,layerName);
	m_cbTextOption_Layer.SetCurSel(idx);
	UpdateData(FALSE);
	CompleteEditorCommand();
}


void CDlgAddDrawingNumber::OnBnClickedButtonWriteDwgNameFont()
{
	BeginEditorCommand();
	UpdateData();
	CWKUtils utils;
	AcDbObjectIdArray ids;
	utils.SSGET(ids,_T("TEXT"));
	if(ids.length()==0){CompleteEditorCommand(); return;}
	CString strStyleName;
	utils.GetTextStyleName(ids.first(),strStyleName);

	int idx = m_cbDwgFileName_Font.FindString(0,strStyleName);
	m_cbDwgFileName_Font.SetCurSel(idx);
	UpdateData(FALSE);
	CompleteEditorCommand();
}


void CDlgAddDrawingNumber::OnBnClickedButtonWriteDwgNameLayer()
{
	BeginEditorCommand();
	UpdateData();
	CWKUtils utils;
	CString layerName;
	utils.GetLayer(layerName,_T("\n레이어를 가져올 객체 선택:"));

	int idx = m_cbDwgFileName_Layer.FindString(0,layerName);
	m_cbDwgFileName_Layer.SetCurSel(idx);
	UpdateData(FALSE);
	CompleteEditorCommand();
}
void CDlgAddDrawingNumber::OnEnChangeEditDwgNumberPrefix(){makeDwgNumber();}
void CDlgAddDrawingNumber::OnEnChangeEditDwgStartNumber(){makeDwgNumber();}
void CDlgAddDrawingNumber::OnEnChangeEditIncreaseDigit(){makeDwgNumber();}
void CDlgAddDrawingNumber::OnEnChangeEditIncreaseStartNumber(){makeDwgNumber();}
void CDlgAddDrawingNumber::OnEnChangeEditInsertText(){makeDwgNumber();}
void CDlgAddDrawingNumber::makeDwgNumber()
{	
	UpdateData();
	if(m_InsertText_Target==1){
		CString strdigit;
		strdigit.Format(_T("%d"),m_iInsertTextDigit);
		CString strTxt(_T(""));       
		CString crossVLineFmt("%0");
		crossVLineFmt=crossVLineFmt+strdigit+L"d";
		strTxt.Format(crossVLineFmt,m_iDwgNumber_Start);
		m_preview=m_strDwgName_Prefix+strTxt;
	}else if(m_InsertText_Target==2){
		if(m_bInsertTextIncrease){
			CString strdigit;
			strdigit.Format(_T("%d"),m_iInsertTextDigit);
			CString strTxt(_T(""));       
			CString crossVLineFmt("%0");
			crossVLineFmt=crossVLineFmt+strdigit+L"d";
			strTxt.Format(crossVLineFmt,m_InsertText_StartNumber);
			m_preview=m_strDwgInstartText+strTxt;
		}else{
			m_preview=m_strDwgInstartText;
		}}
	UpdateData(FALSE);
}



void CDlgAddDrawingNumber::OnBnClickedButtonFormoptionFormName()
{
	UpdateData();
	BeginEditorCommand();
	CWKUtils utils;
	AcDbObjectIdArray ids;
	utils.BlockSSGET(ids,_T("\n도곽 선택 :"));
	if(ids.length()==0){CompleteEditorCommand();return;}

	

	if(m_formOption==1)
	{
		CArray<AttBlock,AttBlock> arr;
		utils.GetBlockReferenceAttributeList(ids.first(),arr);
		if(arr.GetCount() ==0)
		{
			newBlock = Block();
			AfxMessageBox(_T("선택한 폼에 속성값이 없습니다."));
			CompleteEditorCommand();
			return;
		}
		
		
	}
	
	
		

	CString name; AcGePoint3d pos;AcDbExtents Extents;
	utils.AsdkGetBlockAttribute(ids.first(),name,pos,Extents);
	AddBlock(ids.first());
	m_FormOption_FormName = name;
	CompleteEditorCommand();
	UpdateData(FALSE);
}

void CDlgAddDrawingNumber::AddBlock(AcDbObjectId& blockid){

	ACHAR* BlockName=NULL;
	double angle = 0;
	AcGeScale3d scale = 0.0;

	AcDbEntityPointer pEnt(blockid, AcDb::kForRead);
	if(pEnt.openStatus()==Acad::eOk)
	{
		AcDbBlockReference* pSelBlockRef=AcDbBlockReference::cast(pEnt);
		if(pSelBlockRef){
			scale = pSelBlockRef->scaleFactors();
			angle = pSelBlockRef->rotation();
			AcDbExtents FormArea1;
			pSelBlockRef->geomExtentsBestFit(FormArea1);
			newBlock.FormArea = FormArea1;
			newBlock.blkId = pSelBlockRef->id();
			newBlock.scale = scale;
			newBlock.angle =angle;
			
			AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
			if(pBTR.openStatus()==Acad::eOk){		
				pBTR->getName(BlockName) ;
				newBlock.BlockName = BlockName;
				pBTR->close() ;
			}//end-if
		}
		pEnt->close();
	}

}

void CDlgAddDrawingNumber::OnBnClickedButtonFormoptionInsertPoint()
{
	if(newBlock.BlockName.CompareNoCase(_T("")) ==0){AfxMessageBox(_T("도곽을 먼저 선택하십시요!!"));return;}
	BeginEditorCommand();
	UpdateData();

	if(m_formOption==0)
	{

		AcGePoint3d m_topleft,m_rightbottom;
		//////////////////////////////////////////////////////////////////////////
		//텍스트 박스 지정
		if (acedGetPoint(NULL,_T("\n문자가 삽입될 위치의 첫번째 모서리 점 선택:"),asDblArray(m_topleft))==RTNORM)
		{
			if (acedGetCorner(asDblArray(m_topleft),_T("\n문자가 삽입될 위치의 두번째 모서리 점 선택:"),asDblArray(m_rightbottom))==RTNORM)
			{
				AcDbExtents pt,tmp;
				m_topleft.z = 0;
				m_rightbottom.z = 0;

				pt.addPoint(m_topleft);
				pt.addPoint(m_rightbottom);
				newBlock.FileNamArea = pt;
				AddBlock();
				m_FormOption_FormLocation.Format(_T("%f,%f"),newBlock.File_MinVector.x,newBlock.File_MinVector.y);
			}
			else
			{
				AcDbExtents pt;
				pt.addPoint(AcGePoint3d(0,0,0));
				pt.addPoint(AcGePoint3d(0,0,0));
				newBlock.FileNamArea = pt;
			}
		}
	}
	else
	{
		CWKUtils utils;	
		CArray<AttBlock,AttBlock> arr;
		utils.GetBlockReferenceAttributeList(newBlock.blkId,arr);
		if(arr.GetCount() ==0)
		{
			AfxMessageBox(_T("선택한 폼에 속성값이 없습니다."));
			CompleteEditorCommand();
			return;
		}
		for(int i=0; i < arr.GetCount(); i++)
		{
			acutPrintf(_T("\ntag : %s, value : %s"),arr[i].AttributeTag,arr[i].strValue);
		}

		CDlgAttrubuteView dlg;
		dlg.SetTagList(arr);
		if(dlg.DoModal() == IDOK)
		{
			newBlock.AttributeName =m_FormOption_FormLocation =   dlg.m_strTAG;
			
		}
	}
	
	UpdateData(FALSE);
	CompleteEditorCommand();

}
void CDlgAddDrawingNumber::AddBlock()
{
	AcDbExtents FormArea_Tmp;
	AcGeScale3d Calc_Scale = newBlock.scale;
	AcGePoint3d FormArea_MinPoint = newBlock.FormArea.minPoint();
	AcGePoint3d FormArea_MaxPoint = newBlock.FormArea.maxPoint();
	
	FormArea_MinPoint.z = 0;
	FormArea_MaxPoint.z = 0;

	FormArea_Tmp.addPoint(FormArea_MinPoint);
	FormArea_Tmp.addPoint(FormArea_MaxPoint);

	AcGeMatrix3d mat;
	mat.setToRotation(-newBlock.angle,AcGeVector3d::kZAxis);
	FormArea_Tmp.transformBy(mat);


	//벡터를 계산한다.
	AcGeVector3d  filename_minvec;
	AcGeVector3d  filename_maxvec;
	if(newBlock.FileNamArea.minPoint().x == 0 && newBlock.FileNamArea.maxPoint().y ==0)
	{
		if(newBlock.FileNamArea.minPoint() == FormArea_Tmp.minPoint()){
			filename_minvec.x = 0;filename_minvec.y= 0;filename_minvec.z= 0;

		}
		if(newBlock.FileNamArea.maxPoint() == FormArea_Tmp.maxPoint()){
			filename_maxvec.x = 0;filename_maxvec.y = 0;filename_maxvec.z = 0;
		}

	}else
	{
		filename_minvec= (newBlock.FileNamArea.minPoint()-FormArea_Tmp.minPoint()  );// * Calc_Scale;
		filename_maxvec= (newBlock.FileNamArea.maxPoint()-FormArea_Tmp.maxPoint()  );// * Calc_Scale;
	}


	if(newBlock.FileNamArea.minPoint() == FormArea_Tmp.minPoint()){
		filename_minvec.x = 0;filename_minvec.y= 0;filename_minvec.z= 0;

	}
	if(newBlock.FileNamArea.maxPoint() == FormArea_Tmp.maxPoint()){
		filename_maxvec.x = 0;filename_maxvec.y = 0;filename_maxvec.z = 0;
	}

	filename_minvec.x = filename_minvec.x / Calc_Scale.sx;
	filename_minvec.y = filename_minvec.y / Calc_Scale.sy;
	filename_minvec.z = 0;

	filename_maxvec.x = filename_maxvec.x / Calc_Scale.sx;
	filename_maxvec.y = filename_maxvec.y / Calc_Scale.sy;
	filename_maxvec.z = 0;


	newBlock.File_MinVector = AcGePoint3d(filename_minvec.x,filename_minvec.y,filename_minvec.z);
	newBlock.File_MaxVector = AcGePoint3d(filename_maxvec.x,filename_maxvec.y,filename_maxvec.z);

}


void CDlgAddDrawingNumber::GetLayout(AcDbDatabase* pDb, CArray <AcDbObjectId,AcDbObjectId>& arrLayout)
{
	AcDbDictionary *pDict = NULL;
	AcDbObject *pObj;
	if( pDb->getLayoutDictionary( pDict, AcDb::kForRead ) == Acad::eOk)
	{ 
		AcDbDictionaryIterator *pIter = pDict->newIterator();
		for(;!pIter->done();pIter->next())
		{

			ACHAR *pLayName = NULL;
			Acad::ErrorStatus es= pIter->getObject(pObj,AcDb::kForRead);
			if(es!=Acad::eOk) continue;
			AcDbLayout *pLayout = AcDbLayout::cast(pObj);
			if(pLayout)
			{

				arrLayout.Add(pLayout->objectId());
				pLayout->close();



			}
			else
			{
				pObj->close();

			}
		}
		delete pIter;
		pDict->close();
	}


}


#pragma  region 정렬
//z->방향 소트 (완료)
void CDlgAddDrawingNumber::ZSort(double tol)
{
	int blockCnt = m_blockRefIds.GetCount();
	if(blockCnt ==1)return;

	QSortCArray( m_blockRefIds, ComparePointY_De_ ); 

	for(int i=0; i < blockCnt; i++)
	{
		for(int k=0; k < blockCnt-1; k++)
		{
			double a = m_blockRefIds[k].minpoint_Y;
			double b = m_blockRefIds[k+1].minpoint_Y;
			if(abs(m_blockRefIds[k].minpoint_Y - m_blockRefIds[k+1].minpoint_Y) <tol)
			{
				if(m_blockRefIds[k].minpoint_X > m_blockRefIds[k+1].minpoint_X)
				{
					CPrintFormData_ TmpId;
					TmpId = m_blockRefIds[k];
					m_blockRefIds[k] = m_blockRefIds[k+1];
					m_blockRefIds[k+1] = TmpId;

				}

			}
		}
	}
}
//N↑방향 소트 (완료)
void CDlgAddDrawingNumber::NSort(double tol)
{
	int blockCnt = m_blockRefIds.GetCount();
	if(blockCnt ==1)return;

	QSortCArray( m_blockRefIds, ComparePointX_ ); 


	for(int i=0; i< blockCnt; i++)
	{
		for(int k=0; k<blockCnt-1; k++)
		{
			if(abs(m_blockRefIds[k].minpoint_X - m_blockRefIds[k+1].minpoint_X) <tol)
			{
				if(m_blockRefIds[k].minpoint_Y < m_blockRefIds[k+1].minpoint_Y)
				{
					CPrintFormData_ TmpId;
					TmpId = m_blockRefIds[k];
					m_blockRefIds[k] = m_blockRefIds[k+1];
					m_blockRefIds[k+1] = TmpId;
				}
			}
		}
	}
}
//z 반전 방향 소트 (완료)
void CDlgAddDrawingNumber::ZMSort(double tol)
{

	int blockCnt = m_blockRefIds.GetCount();
	if(blockCnt ==1)return;

	// 	acutPrintf(_T("정렬전\n"));
	// 	for(int i=0; i < blockCnt; i++)
	// 	{
	// 		acutPrintf(_T("x : %f,y : %f, z: %f \n"),m_blockRefIds[i].minpoint_X,m_blockRefIds[i].minpoint_Y);
	// 	}

	QSortCArray( m_blockRefIds, ComparePointY_ ); 

	for(int i=0; i < blockCnt; i++)
	{
		for(int k=0; k < blockCnt-1; k++)
		{
			double a = abs(m_blockRefIds[k].minpoint_Y - m_blockRefIds[k+1].minpoint_Y);
			double b = m_blockRefIds[k+1].minpoint_Y;

			if(abs(m_blockRefIds[k].minpoint_Y - m_blockRefIds[k+1].minpoint_Y) <tol)
			{
				if(m_blockRefIds[k].minpoint_X > m_blockRefIds[k+1].minpoint_X)
				{
					CPrintFormData_ TmpId;
					TmpId = m_blockRefIds[k];
					m_blockRefIds[k] = m_blockRefIds[k+1];
					m_blockRefIds[k+1] = TmpId;

				}

			}
		}
	}
	// 	acutPrintf(_T("정렬후\n"));
	// 	for(int i=0; i < blockCnt; i++)
	// 	{
	// 		acutPrintf(_T("x : %f,y : %f, z: %f \n"),m_blockRefIds[i].minpoint_X,m_blockRefIds[i].minpoint_Y);
	// 	}

}
//N 반전 방향 소트
void CDlgAddDrawingNumber::NMSort(double tol)
{
	int blockCnt = m_blockRefIds.GetCount();
	if(blockCnt ==1)return;

	QSortCArray( m_blockRefIds, ComparePointX_ ); 

	for(int i=0; i< blockCnt; i++)
	{
		for(int k=0; k<blockCnt-1; k++)
		{

			if(abs(m_blockRefIds[k].minpoint_X - m_blockRefIds[k+1].minpoint_X) <tol)
			{
				if(m_blockRefIds[k].minpoint_Y > m_blockRefIds[k+1].minpoint_Y)
				{
					CPrintFormData_ TmpId;
					TmpId = m_blockRefIds[k];
					m_blockRefIds[k] = m_blockRefIds[k+1];
					m_blockRefIds[k+1] = TmpId;
				}
			}
		}
	}
}
#pragma endregion


void CDlgAddDrawingNumber::ss2Array(ads_name ss, CPrintFormDataArray_& objIds)
{
	Adesk::Int32	len;
	int		ret;
	AcDbObjectId objId;
	ads_name ent;

	acedSSLength(ss, &len);
	for (long i=0L; i<len; i++) {
		ret = acedSSName(ss, i, ent);
		ASSERT(ret == RTNORM);
		if (enameToObjId(ent, objId) == TRUE){
			CPrintFormData_ data;
			GetBlockPoint(objId,data.minpoint_X,data.minpoint_Y,data.maxpoint_X,data.maxpoint_Y,data.scale,data.angle);
			data.FormId = objId;

			objIds.Add(data);
		}
	}//end-for
	acedSSFree(ss);
}


//ads_name 을 objectid 로 바꾸고 오브젝트를 열어서 리스트에 있는 블록인지 확인하고 리스트에 있을경우에만 추가하도록 한다.
BOOL CDlgAddDrawingNumber::enameToObjId(ads_name ent, AcDbObjectId& objId)
{
	BOOL bISExistBlock = FALSE;

	Acad::ErrorStatus es = acdbGetObjectId(objId, ent);
	if (es == Acad::eOk)
	{
		AcDbEntityPointer pEnt(objId, AcDb::kForRead);
		if(pEnt.openStatus()==Acad::eOk)
		{
			if (pEnt->isA()==AcDbBlockReference::desc())
			{
				AcDbBlockReference* pSelBlockRef=AcDbBlockReference::cast(pEnt);
				if(pSelBlockRef){
					//pSelBlockRef->geomExtentsBestFit(FormArea);
					AcDbObjectId blockid = pSelBlockRef->blockTableRecord();
					AcDbBlockTableRecordPointer pBTR(blockid, AcDb::kForRead);

					if(pBTR.openStatus()==Acad::eOk){		
						ACHAR *name ;
						pBTR->getName(name) ;
						if(newBlock.BlockName.CompareNoCase(name)==0)bISExistBlock = TRUE;
						pBTR->close() ;
					}//end-if					
				}			
			}
			pEnt->close();
		}


	}

	return bISExistBlock;
}

BOOL CDlgAddDrawingNumber::UserSelectBlock(CPrintFormDataArray_& m_blockRefIds)
{
	BOOL bSuccess = FALSE;
	
	struct resbuf * filter = acutBuildList(-4,_T("<OR"),0);   
	struct resbuf * nextList = filter;   
	nextList->rbnext = acutBuildList(2,newBlock.BlockName,0);   
	nextList = nextList->rbnext;   
	nextList->rbnext = acutBuildList(-4,_T("OR>"),0);   


	ads_name ss;
	int ssss = acedSSGet(NULL, NULL, NULL ,filter, ss);
	if (RTNORM == ssss){					//드래그로 객체를 가져온다.
		m_blockRefIds.RemoveAll();												//블록 배열 초기화
		ss2Array(ss, m_blockRefIds);											//사용자가 지정한 블록 가져오기
		bSuccess = TRUE;

	}
	else
	{
		bSuccess = FALSE;
	}
	return bSuccess;

}


void CDlgAddDrawingNumber::GetLayoutBrefs(AcDbObjectId idLayout,CString strBlkName, AcDbObjectIdArray& ids)
{
	Acad::ErrorStatus es;
	AcDbLayout *pLayout = 0;

	// Open the layout object
	if ( (es=acdbOpenObject(pLayout, idLayout, AcDb::kForRead)) == Acad::eOk )
	{
		AcDbObjectId idBlock = pLayout->getBlockTableRecordId();
		AcDbBlockTableRecord *pBTR = 0;

		// Open its blocktable record
		if ( (es=acdbOpenObject(pBTR, idBlock, AcDb::kForRead)) == Acad::eOk )
		{
			AcDbBlockTableRecordIterator *pIt = 0;
			es = pBTR->newIterator(pIt);
			if (es == Acad::eOk)
			{ 
				// Iterate over its entities
				for (pIt->start(); !pIt->done(); pIt->step())
				{
					AcDbEntity *pEnt = 0;
					es = pIt->getEntity(pEnt, AcDb::kForRead);
					if (es == Acad::eOk)
					{ 
						AcDbBlockReference *pBref = AcDbBlockReference::cast(pEnt);
						if (pBref) // If it is a BREF...
						{

							AcDbBlockTableRecord *pRefdBlock = 0;
							// Open BREF's blocktable record
							if ( (es=acdbOpenObject(pRefdBlock, pBref->blockTableRecord(), AcDb::kForRead)) == Acad::eOk )
							{

								const ACHAR *pName = 0;
								pRefdBlock->getName(pName); // get the blockname
								if(wcscmp(strBlkName,pName)==0)
								{
									ids.append(pEnt->objectId());
								}
								//blocks[pEnt->objectId()] = CString(pName); // store it.
								pRefdBlock->close();		
							}
						}
						pEnt->close();
					}
				}
				delete pIt;
			}
			pBTR->close();		
		}
		pLayout->close();		
	}
}

void CDlgAddDrawingNumber::GetBlockPoint(AcDbObjectId& blockId, double& minpoint_X, double& minpoint_Y,double& maxpoint_X,double& maxpoint_Y, AcGeScale3d& scale,double& angle)
{
	AcDbEntityPointer pEnt(blockId, AcDb::kForRead);
	if(pEnt.openStatus()==Acad::eOk)
	{

		AcDbBlockReference* pBlockRef=AcDbBlockReference::cast(pEnt);
		if(pBlockRef){
			AcDbExtents extents;
			pBlockRef->geomExtentsBestFit(extents);
			//pBlockRef->getGeomExtents(extents);

			minpoint_X = extents.minPoint().x;
			minpoint_Y = extents.minPoint().y;

			maxpoint_X = extents.maxPoint().x;
			maxpoint_Y = extents.maxPoint().y;
			scale = pBlockRef->scaleFactors();
			angle = pBlockRef->rotation();

		}
		pEnt->close();
	}

}


void CDlgAddDrawingNumber::FindBlock(AcDbObjectId Layout_id,CPrintFormDataArray_& m_blockRefIds,CString strName)
{

	AcDbObjectId blktbl_id = AcDbObjectId::kNull;
	ACHAR *pLayName = NULL;
	AcDbLayout* pLayout = NULL;
	Acad::ErrorStatus es =	acdbOpenObject(pLayout,Layout_id,AcDb::kForRead); 
	if(pLayout)
	{
		blktbl_id = pLayout->getBlockTableRecordId();
		pLayout->close();
	}

	m_blockRefIds.RemoveAll();
	AcDbObjectIdArray ids;
		
		AcDbBlockTableRecordPointer blockTable (blktbl_id, AcDb::kForRead);
		if (blockTable.openStatus () == Acad::eOk)
		{
			m_blockRefIds.RemoveAll();
			AcDbBlockTableRecordIterator* pBtblrIter;
			es=blockTable->newIterator(pBtblrIter);
			if (es!= Acad::eOk)
			{
				blockTable->close();
				return;
			}
			// iterate the entities within the layout
			for(;!pBtblrIter->done();pBtblrIter->step())
			{
				AcDbEntity* pEnt;
				es=pBtblrIter->getEntity(pEnt,	AcDb::kForRead);
				if (es!= Acad::eOk)
				{
					blockTable->close();
					delete pBtblrIter;
					return ;
				}

				// if this is a block reference
				if(pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference* pSelBlockRef = AcDbBlockReference::cast(pEnt);
					if(pSelBlockRef){
						AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
						if(pBTR.openStatus() == Acad::eOk){
							ACHAR* Name=NULL;
							pBTR->getName(Name);
							if(strName.CompareNoCase(Name) ==0)
							{
								ids.append(pSelBlockRef->id());
							}
							pBTR->close();
						}
					}
				}

				es=pEnt->close();
				if (es!= Acad::eOk)
				{
					blockTable->close();
					delete pBtblrIter;
					return ;
				}
			}
			delete pBtblrIter;
			blockTable->close();
		}


	for(int i = 0 ; i< ids.length(); i++)
	{
		CPrintFormData_ data;
		GetBlockPoint(ids[i],data.minpoint_X,data.minpoint_Y,data.maxpoint_X,data.maxpoint_Y,data.scale,data.angle);
		data.FormId = ids[i];
		m_blockRefIds.Add(data);
	}

}
BOOL CDlgAddDrawingNumber::PlotBlockInit(AcDbObjectId Layout_id)
{
	BOOL bSuccess = FALSE;
	if(m_bIsUserSelectForm){
		bSuccess = UserSelectBlock(m_blockRefIds);
	}

	else
	{
		FindBlock(Layout_id,m_blockRefIds,newBlock.BlockName);
		bSuccess = TRUE;
	}

	if(m_blockRefIds.GetCount() >0){
		if(m_iSort ==0)
		{
			ZSort(m_dblSortTolerance);
		}
		else if(m_iSort ==1)
		{
			ZMSort(m_dblSortTolerance);
		}
		else if(m_iSort ==2)
		{
			NSort(m_dblSortTolerance);
		}
		else if(m_iSort ==3)
		{
			NMSort(m_dblSortTolerance);
		}


	}
	return bSuccess;

}

//경로로 도큐먼트 가져오기
AcApDocument* CDlgAddDrawingNumber::GetOepndDocument(CString strPath){
	AcApDocument *doc = NULL;
	//여기서 부터는 열린 도면들
	AcApDocumentIterator *pIter = acDocManager->newAcApDocumentIterator();
	for(; !pIter->done(); pIter->step()){
		const ACHAR *name = NULL;
		name = pIter->document()->fileName();
		if(strPath.CompareNoCase(name) == 0){
			doc = pIter->document();
			break;
		}
	}
	delete pIter;
	return doc;
}

void  CDlgAddDrawingNumber::gogo()
{
	if(acDocManager->documentCount() <=0 ){ return;}
		if(m_WriteTarget==0)
		{
			AcApDocument * activeDoc =acDocManager->curDocument();
			AcApLayoutManager *pLayoutManager = (AcApLayoutManager *)acdbHostApplicationServices()->layoutManager(); 
			const ACHAR* ActiveLayoutName = pLayoutManager->findActiveLayout(Adesk::kTrue);
			AcDbDatabase * pDb = activeDoc->database();

			acDocManager->lockDocument(acDocManager->curDocument());


			CArray <AcDbObjectId,AcDbObjectId> arrLayout;
			GetLayout(pDb, arrLayout);

			AcDbObject *pObj;


			//도면의 레이아웃들중 처음 보이는 레이아웃을 저장해 놓았다가 파일을 닫기전에 원래 상태로 만들고 끝내자
			const ACHAR* OriginLayout = NULL;
			/*
			함수 변경
			AcApLayoutManager Class
			virtual const ACHAR * findActiveTab();
			virtual Acad::ErrorStatus getActiveTab(AcString & sName) = 0;
			OriginLayout = pLayoutManager->findActiveTab();
			*/
			#if _ZCAD || _ZXYCAD
				#ifndef _2025
						OriginLayout = pLayoutManager->findActiveTab();
				#else // z2025부터 함수 변경
						AcString ActiveName;
						OriginLayout = (ACHAR*)pLayoutManager->getActiveTab(ActiveName);
				#endif
			#elif _ACAD
					AcString ActiveName;
					OriginLayout = (ACHAR*)pLayoutManager->getActiveTab(ActiveName);
			#endif

			const ACHAR *csModel=acdbGetReservedString( AcDb::kModel );
			CString strModel(csModel);

			int nCursel = m_FormTarget.GetCurSel();
			CString strTargetLayout;
			m_FormTarget.GetLBText(nCursel,strTargetLayout);


			if(strTargetLayout.CompareNoCase(_T("활성된 도면"))==0)
			{
				// [+] findLayoutNamed is return AcdbObjectid
				// Use acdbOpenAcDbObject
				//AcDbLayout* pLayout = pLayoutManager->findLayoutNamed(ActiveLayoutName);
				AcDbLayout* pLayout = nullptr;
				AcDbObjectId id = pLayoutManager->findLayoutNamed(ActiveLayoutName);
				acdbOpenAcDbObject((AcDbObject*&)pLayout, id, AcDb::kForRead);
				ACHAR *pLayName = NULL;
				if(pLayout)
				{
					pLayout->getLayoutName(pLayName);
					//	pLayoutManager->setCurrentLayoutId(pLayout->id());
					pLayoutManager->setCurrentLayout(pLayName); 
					pLayoutManager->updateLayoutTabs();
					WriteTextModule(pLayout->id());
				}
				pLayout->close();
				// [-] END 
			}
			else if(strTargetLayout.CompareNoCase(_T("모형만 검색"))==0)
			{

				AcDbObjectId layout_id = AcDbObjectId::kNull;
				for(int k = 0; k < arrLayout.GetCount() ;k++)
				{
					ACHAR *pLayName = NULL;
					AcDbLayout* pLayout = NULL;
					Acad::ErrorStatus es =	acdbOpenObject(pLayout,arrLayout[k],AcDb::kForWrite); 
					if(pLayout)
					{
						pLayout->getLayoutName(pLayName);
						if( ((CString)pLayName).CompareNoCase(_T("Model")) == 0  || ((CString)pLayName).CompareNoCase(_T("모형")) == 0 )
						{
							layout_id = pLayout->id();
							//pLayoutManager->setCurrentLayoutId(pLayout->id());
							pLayoutManager->setCurrentLayout(pLayName); 
							pLayoutManager->updateLayoutTabs();
							pLayout->close();
							break;

						}
						pLayout->close();

					}
				}
				//start
				WriteTextModule(layout_id);
			}
			else if(strTargetLayout.CompareNoCase(_T("배치만 검색"))==0)
			{

				AcDbObjectId layout_id = AcDbObjectId::kNull;
				for(int k = 0; k < arrLayout.GetCount() ;k++)
				{
					ACHAR *pLayName = NULL;
					AcDbLayout* pLayout = NULL;
					Acad::ErrorStatus es =	acdbOpenObject(pLayout,arrLayout[k],AcDb::kForWrite); 
					if(pLayout)
					{
						pLayout->getLayoutName(pLayName);
						if( ((CString)pLayName).CompareNoCase(_T("Model")) != 0  && ((CString)pLayName).CompareNoCase(_T("모형")) != 0 )
						{
							layout_id = pLayout->id();
							//pLayoutManager->setCurrentLayoutId(pLayout->id());
							pLayoutManager->setCurrentLayout(pLayName); 
							pLayoutManager->updateLayoutTabs();
							pLayout->close();

							WriteTextModule(layout_id);

						}
						pLayout->close();

					}
				}

			}
			else{
				for(int k = 0; k < arrLayout.GetCount() ;k++)
				{
					ACHAR *pLayName = NULL;
					AcDbLayout* pLayout = NULL;
					Acad::ErrorStatus es =	acdbOpenObject(pLayout,arrLayout[k],AcDb::kForWrite); 
					if(pLayout)
					{
						pLayout->getLayoutName(pLayName);
						pLayout->close();
						//	pLayoutManager->setCurrentLayoutId(pLayout->id());
						pLayoutManager->setCurrentLayout(pLayName); 
						pLayoutManager->updateLayoutTabs();

						WriteTextModule(pLayout->id());
					}
				}
			}
			//도면의 레이아웃들중 처음 보이는 레이아웃을 저장해 놓았다가 파일을 닫기전에 원래 상태로 만들고 끝내자
			pLayoutManager->setCurrentLayout(OriginLayout);
			pLayoutManager->updateLayoutTabs();


			acDocManager->unlockDocument(acDocManager->curDocument());
		}
		else
		{
			for(int i=0; i<m_FileList.GetCount(); i++)
			{

				//일단 해당 파일이 열려있는지 확인하자.
				//열려있을경우 새로 파일을 열지 말자
				AcApDocument* doc;
				BOOL bIsopen = IsOpenDocument(m_FileList[i].full_path);
				if(bIsopen){
					doc = GetOepndDocument(m_FileList[i].full_path);
					if(doc!=NULL)	acDocManager->setCurDocument(doc);
				}
				else
				{
					Acad::ErrorStatus es =acDocManager->appContextOpenDocument(m_FileList[i].full_path);
				}

				acDocManager->lockDocument(acDocManager->curDocument());


				AcApDocument * activeDoc =acDocManager->curDocument();
				AcApLayoutManager *pLayoutManager = (AcApLayoutManager *)acdbHostApplicationServices()->layoutManager(); 
				const ACHAR* ActiveLayoutName = pLayoutManager->findActiveLayout(Adesk::kTrue);
				AcDbDatabase * pDb = activeDoc->database();



			
				CArray <AcDbObjectId,AcDbObjectId> arrLayout;
				GetLayout(pDb, arrLayout);

				AcDbObject *pObj;


				//도면의 레이아웃들중 처음 보이는 레이아웃을 저장해 놓았다가 파일을 닫기전에 원래 상태로 만들고 끝내자
				const ACHAR* OriginLayout = NULL;
				/*
				함수 변경
				AcApLayoutManager Class
				virtual const ACHAR * findActiveTab();
				virtual Acad::ErrorStatus getActiveTab(AcString & sName) = 0;
				OriginLayout = pLayoutManager->findActiveTab();
				*/

		#if _ZCAD || _ZXYCAD
			#ifndef _2025
						OriginLayout = pLayoutManager->findActiveTab();
			#else // z2025부터 함수 변경
						AcString ActiveName;
						OriginLayout = (ACHAR*)pLayoutManager->getActiveTab(ActiveName);
			#endif
		#elif _ACAD
					AcString ActiveName;
					OriginLayout = (ACHAR*)pLayoutManager->getActiveTab(ActiveName);
		#endif

				const ACHAR *csModel=acdbGetReservedString( AcDb::kModel );
				CString strModel(csModel);

				int nCursel = m_FormTarget.GetCurSel();
				CString strTargetLayout;
				m_FormTarget.GetLBText(nCursel,strTargetLayout);


				if(strTargetLayout.CompareNoCase(_T("활성된 도면"))==0)
				{

					// [+] findLayoutNamed is return AcdbObjectid
					// Use acdbOpenAcDbObject
					//AcDbLayout* pLayout = pLayoutManager->findLayoutNamed(ActiveLayoutName);
					AcDbLayout* pLayout = nullptr;
					AcDbObjectId id = pLayoutManager->findLayoutNamed(ActiveLayoutName);
					acdbOpenAcDbObject((AcDbObject*&)pLayout, id, AcDb::kForRead);
					ACHAR *pLayName = NULL;
					if(pLayout)
					{
						pLayout->getLayoutName(pLayName);
						//	pLayoutManager->setCurrentLayoutId(pLayout->id());
						pLayoutManager->setCurrentLayout(pLayName); 
						pLayoutManager->updateLayoutTabs();
						WriteTextModule(pLayout->id());
					}
					pLayout->close();
					// [-] END 
				}
				else if(strTargetLayout.CompareNoCase(_T("모형만 검색"))==0)
				{

					AcDbObjectId layout_id = AcDbObjectId::kNull;
					for(int k = 0; k < arrLayout.GetCount() ;k++)
					{
						ACHAR *pLayName = NULL;
						AcDbLayout* pLayout = NULL;
						Acad::ErrorStatus es =	acdbOpenObject(pLayout,arrLayout[k],AcDb::kForWrite); 
						if(pLayout)
						{
							pLayout->getLayoutName(pLayName);
							if( ((CString)pLayName).CompareNoCase(_T("Model")) == 0  || ((CString)pLayName).CompareNoCase(_T("모형")) == 0 )
							{
								layout_id = pLayout->id();
								//pLayoutManager->setCurrentLayoutId(pLayout->id());
								pLayoutManager->setCurrentLayout(pLayName); 
								pLayoutManager->updateLayoutTabs();
								pLayout->close();
								break;

							}
							pLayout->close();

						}
					}
					//start
					WriteTextModule(layout_id);
				}
				else if(strTargetLayout.CompareNoCase(_T("배치만 검색"))==0)
				{

					AcDbObjectId layout_id = AcDbObjectId::kNull;
					for(int k = 0; k < arrLayout.GetCount() ;k++)
					{
						ACHAR *pLayName = NULL;
						AcDbLayout* pLayout = NULL;
						Acad::ErrorStatus es =	acdbOpenObject(pLayout,arrLayout[k],AcDb::kForWrite); 
						if(pLayout)
						{
							pLayout->getLayoutName(pLayName);
							if( ((CString)pLayName).CompareNoCase(_T("Model")) != 0  && ((CString)pLayName).CompareNoCase(_T("모형")) != 0 )
							{
								layout_id = pLayout->id();
								//pLayoutManager->setCurrentLayoutId(pLayout->id());
								pLayoutManager->setCurrentLayout(pLayName); 
								pLayoutManager->updateLayoutTabs();
								pLayout->close();

								WriteTextModule(layout_id);

							}
							pLayout->close();

						}
					}

				}
				else{
					for(int k = 0; k < arrLayout.GetCount() ;k++)
					{
						ACHAR *pLayName = NULL;
						AcDbLayout* pLayout = NULL;
						Acad::ErrorStatus es =	acdbOpenObject(pLayout,arrLayout[k],AcDb::kForWrite); 
						if(pLayout)
						{
							pLayout->getLayoutName(pLayName);
							pLayout->close();
							//	pLayoutManager->setCurrentLayoutId(pLayout->id());
							pLayoutManager->setCurrentLayout(pLayName); 
							pLayoutManager->updateLayoutTabs();

							WriteTextModule(pLayout->id());
						}
					}
				}
				//도면의 레이아웃들중 처음 보이는 레이아웃을 저장해 놓았다가 파일을 닫기전에 원래 상태로 만들고 끝내자
				pLayoutManager->setCurrentLayout(OriginLayout);
				pLayoutManager->updateLayoutTabs();


				acDocManager->unlockDocument(acDocManager->curDocument());
				if(!bIsopen){
					acDocManager->unlockDocument(activeDoc);
					acDocManager->activateDocument(activeDoc);
					acDocManager->lockDocument(activeDoc); // Lock
					activeDoc->database()->saveAs(acDocManager->curDocument()->fileName());
					acdbSetDbmod(activeDoc->database(), 0); // clear changes flag
					acDocManager->unlockDocument(activeDoc);//unlock
					acDocManager->closeDocument(activeDoc);
					activeDoc=NULL;

				}
				else	acDocManager->unlockDocument(activeDoc);


				m_FileList[i].result = _T("처리완료");
			}

		}
		
	}
	void CDlgAddDrawingNumber::WriteTextModule(AcDbObjectId Layout_id)
	{
		CStringArray strArr;

		if(m_InsertText_Target ==0){etoc(strArr);}
		PlotBlockInit(Layout_id);
		CWKUtils ut;
		CTextProcess txt;
		for(int i=0; i< m_blockRefIds.GetCount(); i++)
		{

			CString strMsg;
			//텍스트 만들기 
			if(m_InsertText_Target==0){
				if(strArr.GetCount() -1 < i )
				{
					strMsg = "";
				}
				else
				{
					strMsg = strArr[i];

				}
			}else if(m_InsertText_Target==1){
				CString strdigit;
				strdigit.Format(_T("%d"),m_iInsertTextDigit);
				CString strTxt(_T(""));       
				CString crossVLineFmt("%0");
				crossVLineFmt=crossVLineFmt+strdigit+L"d";
				strTxt.Format(crossVLineFmt,m_iDwgNumber_Start+i);
				strMsg=m_strDwgName_Prefix+strTxt;
			}else if(m_InsertText_Target==2){
				if(m_bInsertTextIncrease){
					CString strdigit;
					strdigit.Format(_T("%d"),m_iInsertTextDigit);
					CString strTxt(_T(""));       
					CString crossVLineFmt("%0");
					crossVLineFmt=crossVLineFmt+strdigit+L"d";
					strTxt.Format(crossVLineFmt,m_InsertText_StartNumber+i);
					strMsg=m_strDwgInstartText+strTxt;
				}else{
					strMsg=m_strDwgInstartText;
				}}


			//텍스트 위치
			double File_minx = m_blockRefIds[i].minpoint_X + (newBlock.File_MinVector.x * m_blockRefIds[i].scale.sx);
			double File_miny = m_blockRefIds[i].minpoint_Y + (newBlock.File_MinVector.y * m_blockRefIds[i].scale.sy);
			double File_maxx = m_blockRefIds[i].maxpoint_X + (newBlock.File_MaxVector.x * m_blockRefIds[i].scale.sx);
			double File_maxy = m_blockRefIds[i].maxpoint_Y + (newBlock.File_MaxVector.y * m_blockRefIds[i].scale.sy);

			AcGePoint3d p1(File_minx,File_miny,0);
			AcGePoint3d p2(File_maxx,File_maxy,0);

			//기존 텍스트 삭제 
			if(m_bOldTextRemove)
			{
				AcDbObjectIdArray text_ids;
				ut.SSGET(text_ids,p1,p2,_T("TEXT"));
				for(int i = 0; i < text_ids.length(); i++){
					AcDbEntity *pE;
					if(Acad::eOk == acdbOpenObject(pE, text_ids[i], AcDb::kForWrite)){
						pE->erase();
						pE->close();
					}
				}
				text_ids.removeAll();
				ut.SSGET(text_ids,p1,p2,_T("MTEXT"));
				for(int i = 0; i < text_ids.length(); i++){
					AcDbEntity *pE;
					if(Acad::eOk == acdbOpenObject(pE, text_ids[i], AcDb::kForWrite)){
						pE->erase();
						pE->close();
					}
				}
			}

			//텍스트
			CString styleName,strAngle, strlayer;
			m_cbTextOption_Font.GetLBText(m_cbTextOption_Font.GetCurSel(),styleName);

			double angle =0;
			m_cbTextOption_Angle.GetLBText(m_cbTextOption_Angle.GetCurSel(),strAngle);
			if(strAngle.CompareNoCase(_T("0도"))==0)
			{
				angle =ut.DTR(0);
			}
			else if(strAngle.CompareNoCase(_T("90도"))==0)
			{
				angle =ut.DTR(90);
			}
			else if(strAngle.CompareNoCase(_T("180도"))==0)
			{
				angle =ut.DTR(180);
			}
			else if(strAngle.CompareNoCase(_T("270도"))==0)
			{
				angle =ut.DTR(270);
			}

			m_cbTextOption_Layer.GetLBText(m_cbTextOption_Layer.GetCurSel(),strlayer);
			TextVertMode  align_Center ;
			TextHorzMode  align_M ;
			align_Center = kTextVertMid;
			align_M =kTextCenter;

			AcDbExtents tt;
			tt.addPoint(AcGePoint3d(File_minx,File_miny,0));
			tt.addPoint(AcGePoint3d(File_maxx,File_maxy,0));

			AcGePoint3d targetpt = tt.minPoint() + ((tt.maxPoint() - tt.minPoint()) /2);
			if(m_formOption ==0)
				txt.insertTextCMD(targetpt,m_strTextOption_Height,styleName,m_strTextOption_Width,angle,strlayer,m_Color,strMsg,align_Center,align_M);
			else
				ut.SetBlockReferenceAttribute(m_blockRefIds[i].FormId,newBlock.AttributeName,strMsg);

			if(m_bDwgNameWrite)
			{

				CString writeStr;
				if(!m_bDwgFileName_Fullpath && !m_bDwgFileName_IncExt)
				{
					writeStr = _T("%<\\AcVar Filename \\f \"%fn2\">%");
				}
				else if(m_bDwgFileName_Fullpath && !m_bDwgFileName_IncExt)
				{
					writeStr  =_T("%<\\AcVar Filename \\f \"%fn3\">%");
				}
				else if(!m_bDwgFileName_Fullpath && m_bDwgFileName_IncExt)
				{
					writeStr  = _T("%<\\AcVar Filename \\f \"%fn6\">%");
				}
				else if(m_bDwgFileName_Fullpath && m_bDwgFileName_IncExt)
				{
					writeStr  = _T("%<\\AcVar Filename \\f \"%fn7\">%");
					
				}

				m_cbDwgFileName_Font.GetLBText(m_cbDwgFileName_Font.GetCurSel(),styleName);

				double angle =0;
				m_cbDwgFileName_Angle.GetLBText(m_cbDwgFileName_Angle.GetCurSel(),strAngle);
				if(strAngle.CompareNoCase(_T("0도"))==0)
				{
					angle =ut.DTR(0);
				}
				else if(strAngle.CompareNoCase(_T("90도"))==0)
				{
					angle =ut.DTR(90);
				}
				else if(strAngle.CompareNoCase(_T("180도"))==0)
				{
					angle =ut.DTR(180);
				}
				else if(strAngle.CompareNoCase(_T("270도"))==0)
				{
					angle =ut.DTR(270);
				}

				m_cbDwgFileName_Layer.GetLBText(m_cbDwgFileName_Layer.GetCurSel(),strlayer);
				TextVertMode  align_virt  = kTextBase;
				TextHorzMode  align_horz  = kTextLeft;
				CString strsort;
				m_cbDwgFileName_Sort.GetLBText(m_cbDwgFileName_Sort.GetCurSel(),strsort);

				if(strsort.CompareNoCase(_T("왼쪽"))==0){
					align_horz = kTextLeft;
					align_virt = kTextBase;
				}else if(strsort.CompareNoCase(_T("중심"))==0){
					align_horz = kTextCenter;
					align_virt = kTextBase;
				}else if(strsort.CompareNoCase(_T("오른쪽"))==0){
					align_horz = kTextRight;
					align_virt = kTextBase;
				}else if(strsort.CompareNoCase(_T("정렬"))==0){
					align_horz = kTextAlign;
					align_virt = kTextBase;
				}else if(strsort.CompareNoCase(_T("맞춤"))==0){
					align_horz = kTextFit;
					align_virt = kTextVertMid;
				}else if(strsort.CompareNoCase(_T("맨 위 왼쪽"))==0){
					align_horz = kTextLeft;
					align_virt = kTextTop;
				}else if(strsort.CompareNoCase(_T("맨 위 중심"))==0){
					align_horz = kTextCenter;
					align_virt = kTextTop;
				}else if(strsort.CompareNoCase(_T("맨 위 오른쪽"))==0){
					align_horz = kTextRight;
					align_virt = kTextTop;
				}else if(strsort.CompareNoCase(_T("중간 왼쪽"))==0){
					align_horz = kTextLeft;
					align_virt = kTextVertMid;
				}else if(strsort.CompareNoCase(_T("중간 중심"))==0){
					align_horz = kTextCenter;
					align_virt = kTextVertMid;
				}else if(strsort.CompareNoCase(_T("중간 오른쪽"))==0){
					align_horz = kTextRight;
					align_virt = kTextVertMid;
				}else if(strsort.CompareNoCase(_T("맨 아래 왼쪽"))==0){
					align_horz = kTextLeft;
					align_virt = kTextBottom;
				}else if(strsort.CompareNoCase(_T("맨 아래 중심"))==0){
					align_horz = kTextCenter;
					align_virt = kTextBottom;
				}else if(strsort.CompareNoCase(_T("맨 아래 오른쪽"))==0){
					align_horz = kTextRight;
					align_virt = kTextBottom;
				}

				newBlock.File_NameVector.z = 0;

				AcGePoint3d target_ = AcGePoint3d(m_blockRefIds[i].minpoint_X,m_blockRefIds[i].minpoint_Y,0) + (newBlock.File_NameVector * m_blockRefIds[i].scale.sx);
				txt.insertTextCMD(target_,m_dblDwgFileName_Height,styleName,m_dblDwgFileName_Width,angle,strlayer,m_Color1,writeStr,align_virt,align_horz);
			}

		}
	}
void CDlgAddDrawingNumber::OnBnClickedButtonGo1(){UpdateData();ShowWindow(SW_HIDE);   m_bIsUserSelectForm = FALSE; gogo(); FileListReIndexing();ShowWindow(SW_SHOW);}
void CDlgAddDrawingNumber::OnBnClickedButtonGo2(){UpdateData();ShowWindow(SW_HIDE);  m_bIsUserSelectForm = TRUE;gogo();ShowWindow(SW_SHOW);}


void CDlgAddDrawingNumber::OnBnClickedButtonWriteDwgNamePoint()
{
	if(newBlock.BlockName.CompareNoCase(_T("")) ==0){AfxMessageBox(_T("도곽을 먼저 선택하십시요!!"));return;}
	BeginEditorCommand();
	UpdateData();
	
	//////////////////////////////////////////////////////////////////////////
	//텍스트 박스 지정
	if (acedGetPoint(NULL,_T("\n파일 이름이 삽입될 점 선택:"),asDblArray(newBlock.File_NamePoint))==RTNORM)
	{
		newBlock.File_NameVector = (newBlock.File_NamePoint-newBlock.FormArea.minPoint() ) / newBlock.scale.sx;
		m_strDwgFileName_Location.Format(_T("%f,%f"),newBlock.File_NamePoint.x,newBlock.File_NamePoint.y);
	}
	UpdateData(FALSE);
	CompleteEditorCommand();
}

void CDlgAddDrawingNumber::etoc(CStringArray& arr)
{
	arr.RemoveAll();
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	COleVariant VTrue((short)TRUE);
	COleVariant VFalse((short)FALSE);
	CApplication app;
	CRange startrng;
	LPDISPATCH lpdisp;
	CWorksheet sheet;
	CWorkbook book;
	CWorkbooks books;
	CWorksheets sheets;

	
	//엑셀 실행가능 여부
	CLSID clsid;
	LPDISPATCH lpDisp; 
	HRESULT hr = ::CLSIDFromProgID(_T("Excel.Application"), &clsid);
	if(FAILED(hr)){
		AfxMessageBox(_T("엑셀이 설치되어 있지 않습니다.."));
		return;
	}
	IDispatch *pDisp = NULL;
	IUnknown *pUnk = NULL;
	hr = GetActiveObject(clsid, NULL, (IUnknown**)&pUnk);
	//이미 실행중인 Excel 이 있는지 확인
	if(SUCCEEDED(hr)) 
	{
		hr = pUnk->QueryInterface(IID_IDispatch, (void **)&pDisp);
		app.AttachDispatch(pDisp);
	}
	else
	{
		AfxMessageBox(_T("엑셀이 실행되지 않았습니다."));
		return;
	}

	//엑셀 보여주고
//	app.put_Visible(true);

	//워크북 열고
	lpDisp = app.get_Workbooks();
	ASSERT(lpDisp);
	books.AttachDispatch(lpDisp);
	lpDisp = app.get_ActiveSheet();
	ASSERT(lpDisp);
	sheet.AttachDispatch(lpDisp);

	//범위선택 대화상자 열기
	COleVariant getCell, selectSheet;	//범위 변수
	getCell.pdispVal = NULL;
	if(getCell.pdispVal == NULL){
		getCell = app.InputBox(_T("범위 선택"), COleVariant(TEXT("표를 캐드로 가져오기")), selectSheet, VOptional, VOptional, VOptional, VOptional, COleVariant(short(8)));

		//범위 미지정 또는 종료
		if(getCell.pdispVal == NULL){
			AfxMessageBox(_T("범위를 지정하지 않았습니다."));
			return;
		}
	}

	//범위 대입
	startrng.AttachDispatch(getCell.pdispVal);
	


	//시트 가져오기
	CString SelectXlsSheet;				//선택 시트
	int SelectXlsIndex = 0;
	lpDisp = startrng.get_Parent();		//레인지의 부모격인 시트를 가져온다.
	sheet.AttachDispatch(lpDisp);		//매칭
	SelectXlsSheet = sheet.get_Name();	//부모님 이름이 뭐야~?
	SelectXlsIndex = sheet.get_Index();


	int iRows, iCols;	//행열 갯수 카운트
	CRange rowRang = startrng.get_Rows();		//전체 가로 열
	iRows = rowRang.get_Count();			//전체 중 가로 열 갯수
	CRange colRang = startrng.get_Columns();	//전체 세로 열
	iCols = colRang.get_Count();			//전체 중 세로 열 갯수


	if(iCols >1){AfxMessageBox(_T("하나의 열만 선택 해야 합니다."));return;}


	CRange curRange;	//현재 선택된 셀
	long ReferenceStyle = 0;	//???
	VARIANT varMerge;		//병합 체크

	
	//데이터 가져오기
	for(long icurRow = 1; icurRow <= iRows; icurRow++){
		for(long icurCol = 1; icurCol <= iCols; icurCol++){
			//현재 선택된 셀 대입	
			curRange.AttachDispatch(startrng.get_Item(COleVariant(icurRow), COleVariant(icurCol)).pdispVal);
			
			arr.Add(curRange.get_Text().bstrVal);

			curRange.ReleaseDispatch();
		}
	}


}


void CDlgAddDrawingNumber::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	ColorSetting(m_Color,m_Btn_Color,IDC_STATIC_COL);
	ColorSetting(m_Color1,m_Btn_Color2,IDC_STATIC_COL2);
}


void CDlgAddDrawingNumber::OnBnClickedButtonAddActiveDwg()
{
	GetActiveDWG();
	FileListReIndexing();
}

//도면이 현재 열려있는지 확인하기
BOOL CDlgAddDrawingNumber::IsOpenDocument(CString dwg, AcApDocument *doc){
	BOOL bIsOpen = FALSE;
	//여기서 부터는 열린 도면들
	AcApDocumentIterator *pIter = acDocManager->newAcApDocumentIterator();
	for(; !pIter->done(); pIter->step()){
		if(dwg.CompareNoCase(pIter->document()->fileName()) == 0){
			bIsOpen = TRUE;
			doc = pIter->document();
			break;
		}
	}
	delete pIter;
	return bIsOpen;
}

//도면이 현재 열려있는지 확인하기
BOOL CDlgAddDrawingNumber::IsOpenDocument(CString dwg){
	BOOL bIsOpen = FALSE;
	//여기서 부터는 열린 도면들
	AcApDocumentIterator *pIter = acDocManager->newAcApDocumentIterator();
	for(; !pIter->done(); pIter->step()){
		if(dwg.CompareNoCase(pIter->document()->fileName()) == 0){
			bIsOpen = TRUE;
			break;
		}
	}
	delete pIter;
	return bIsOpen;
}


//활성화된 도면 가져오기
void CDlgAddDrawingNumber::GetActiveDWG(){	
	//현재 선택된 도면
	AcApDocument *activeDoc = NULL;
	activeDoc = acDocManager->curDocument();
	bool bisExist = false;
	for(int i = 0; i < m_FileList.GetCount(); i++){
		if(m_FileList[i].full_path.CompareNoCase((CString)activeDoc->fileName()) == 0){
			bisExist = true;
			break;
		}
	}
	if(!bisExist){
		if(IsOpenDocument(activeDoc->fileName())){
			FileInfos_ fileinfo;
			fileinfo.filename = activeDoc->docTitle();
			fileinfo.full_path = activeDoc->fileName();			
			if(fileinfo.full_path.Find(_T("\\")) != -1)	m_FileList.Add(fileinfo);
		}
	}
}

void CDlgAddDrawingNumber::OnBnClickedButtonAddOpenDwg()
{
	GetOpenDWG();
	FileListReIndexing();
}
void CDlgAddDrawingNumber::GetOpenDWG(){
	AcApDocumentIterator *pIter = acDocManager->newAcApDocumentIterator();
	while(!pIter->done()){
		bool bisExist = false;
		const ACHAR *pDocName = pIter->document()->fileName();
		for(int i = 0; i < m_FileList.GetCount(); i++){	
			if(m_FileList[i].full_path.CompareNoCase(pDocName) == 0){
				bisExist = true;
				break;
			}
		}
		if(bisExist == false){
			FileInfos_ fileinfo;
			fileinfo.filename = pIter->document()->docTitle();
			ACHAR *pNewDocName = NULL;
			acutNewString(pDocName, pNewDocName);
			if(pNewDocName)	fileinfo.full_path=pNewDocName;
			if(fileinfo.full_path.Find(_T("\\")) != -1)	m_FileList.Add(fileinfo);
		}
		pIter->step();
	}
	delete pIter;
}


void CDlgAddDrawingNumber::OnBnClickedButtontTop()
{
	if(m_FileView.GetItemCount() <= 0)	return;
	if(m_FileList.GetCount() <= 0)	return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if(nSelectedItem < 0)	return;
	RowSwap(0,nSelectedItem);
	FileListReIndexing();
	m_FileView.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_FileView.EnsureVisible(0, TRUE);
	m_FileView.SetFocus(); //선택된 항목들 포커스 유지
}


void CDlgAddDrawingNumber::OnBnClickedButtonUp()
{
	if(m_FileView.GetItemCount() <= 0)	return;
	if(m_FileList.GetCount() <= 0)	return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if(nSelectedItem < 0)	return;
	if(nSelectedItem == 0)	return;
	RowSwap(nSelectedItem,nSelectedItem - 1);
	FileListReIndexing();
	m_FileView.SetItemState(nSelectedItem - 1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_FileView.EnsureVisible(nSelectedItem - 1, TRUE);
	m_FileView.SetFocus(); //선택된 항목들 포커스 유지
}


void CDlgAddDrawingNumber::OnBnClickedButtonDown()
{
	if(m_FileView.GetItemCount() <= 0)	return;
	if(m_FileList.GetCount() <= 0)	return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if(nSelectedItem < 0)	return;
	if(nSelectedItem == m_FileList.GetCount() - 1)	return;
	RowSwap(nSelectedItem, nSelectedItem + 1);
	FileListReIndexing();
	m_FileView.SetItemState(nSelectedItem + 1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_FileView.EnsureVisible(nSelectedItem + 1, TRUE);
	m_FileView.SetFocus(); //선택된 항목들 포커스 유지
}


void CDlgAddDrawingNumber::OnBnClickedButtonBottom()
{
	if(m_FileView.GetItemCount() <= 0)	return;
	if(m_FileList.GetCount() < 0)	return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if(nSelectedItem < 0)	return;

	RowSwap(m_FileView.GetItemCount() - 1,nSelectedItem);
	FileListReIndexing();
	m_FileView.SetItemState(m_FileView.GetItemCount() - 1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_FileView.EnsureVisible(m_FileView.GetItemCount() - 1, TRUE);
	m_FileView.SetFocus(); //선택된 항목들 포커스 유지
}


void CDlgAddDrawingNumber::OnBnClickedButtonSelRemove()
{
	if(m_FileView.GetItemCount() <= 0)	return;
	if(m_FileList.GetCount() <= 0)	return;
	int nSelectedItem = m_FileView.GetNextItem(-1, LVNI_SELECTED);
	if(nSelectedItem == m_FileList.GetCount())	nSelectedItem = nSelectedItem - 1;
	if(nSelectedItem < 0)	return;

	POSITION pos = m_FileView.GetFirstSelectedItemPosition();
	if(pos != NULL){
		while (pos){
			int nItem = m_FileView.GetNextSelectedItem(pos);
			for(int i = 0; i < m_FileList.GetCount(); i++){
				if(m_FileList[i].filename.CompareNoCase(m_FileView.GetItemText(nItem, 2)) == 0){
					m_FileList.RemoveAt(i);
					break;
				}
			}
		}
	}
	FileListReIndexing();
	m_FileView.SetItemState(nSelectedItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_FileView.EnsureVisible(nSelectedItem, TRUE);
	m_FileView.SetFocus(); //선택된 항목들 포커스 유지
}


void CDlgAddDrawingNumber::OnBnClickedButtonAllRemove()
{
	if(m_FileList.GetCount() < 0)	return;
	m_FileList.RemoveAll();
	FileListReIndexing();
	FileViewRefresh();
}

void CDlgAddDrawingNumber::FileListReIndexing(){
 	for(int i = 0; i < m_FileList.GetCount(); i++)	m_FileList[i].index = i;
 	m_FileView.DeleteAllItems();
 	for(int i = 0; i < m_FileList.GetCount(); i++){
 		CString seq_string; 
 		seq_string.Format(_T("%d"), m_FileList[i].index + 1); 
 		m_FileView.InsertItem(i, m_FileList[i].filename);
 		m_FileView.SetItem(i, 1, LVIF_TEXT,m_FileList[i].result, 0, 0, 0, NULL);
 		
 	}
}

void CDlgAddDrawingNumber::RowSwap(int a, int b){
	FileInfos_ files;
	files = m_FileList[a];
	m_FileList[a] = m_FileList[b];
	m_FileList[b] = files;
}

//파일 리스트뷰에 데이터 올리기
void CDlgAddDrawingNumber::FileViewRefresh(){
	m_FileView.DeleteAllItems();
	for(int i = 0; i< m_FileList.GetCount(); i++){
		CString seq_string; 
		seq_string.Format(_T("%d"), i + 1); 
		m_FileView.InsertItem(i,m_FileList[i].filename);
		m_FileView.SetItem(i, 1, LVIF_TEXT, m_FileList[i].result, 0, 0, 0, NULL);
	}
}


void CDlgAddDrawingNumber::OnBnClickedButtonAddDwg()
{
	CString str = _T("All files(*.dwg)|*.dwg|"); // 모든 파일 표시
	CFileDialog dlg(TRUE, _T("*.dwg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();

		AcApDocument *activeDoc = NULL;
		activeDoc = acDocManager->curDocument();
		bool bisExist = false;
		for(int i = 0; i < m_FileList.GetCount(); i++){
			if(m_FileList[i].full_path.CompareNoCase((CString)strPathName) == 0){
				bisExist = true;
				break;
			}
		}
		if(!bisExist){
				FileInfos_ fileinfo;
				fileinfo.filename = dlg.GetFileName();
				fileinfo.full_path = strPathName;			
				if(fileinfo.full_path.Find(_T("\\")) != -1)	m_FileList.Add(fileinfo);
		}
	}
	FileListReIndexing();

}

void CDlgAddDrawingNumber::InitLitView(){


	CRect rc;
	m_FileView.GetClientRect(&rc);
	m_FileView.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);	//체크박스 레이아웃 설정
	m_FileView.DeleteAllItems();	//초기화
	//컬럼명 설정
	m_FileView.InsertColumn(0, _T("파일명"), LVCFMT_CENTER, rc.Width()*0.7, -1); 
	m_FileView.InsertColumn(1, _T("처리결과"), LVCFMT_CENTER, rc.Width()*0.3, -1); 
}


void CDlgAddDrawingNumber::OnBnClickedButtonAddFolder()
{
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	//BrInfo.lpszTitle = _T("파일이 저장될 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // 파일경로 읽어오기

	// 경로를 가져와 사용할 경우, Edit Control 에 값 저장
	CString str;
	str.Format(_T("%s"), szBuffer);
	
	FileSearch(szBuffer);
	FileListReIndexing();
}


void CDlgAddDrawingNumber::OnBnClickedButtonOpen()
{

	POSITION pos = m_FileView.GetFirstSelectedItemPosition();
	int selected=-1;
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = m_FileView.GetNextSelectedItem(pos);
			selected = nItem+1;
		}
	}

	
	if(selected <0)	return;
	CString path = m_FileList[selected-1].full_path;
	ShowWindow(SW_HIDE);
	//	BeginEditorCommand();
	AcApDocument *doc=NULL;
	BOOL bIsopen = IsOpenDocument(path, doc);
	if(bIsopen)	acDocManager->setCurDocument(doc);
	else		Acad::ErrorStatus es = acDocManager->appContextOpenDocument(path);
	ShowWindow(SW_SHOW);
	//CompleteEditorCommand();
}


void CDlgAddDrawingNumber::FileSearch(CString path){
	
	FindFile(path,_T("*.dwg"));

	FileViewRefresh();
}

bool CDlgAddDrawingNumber::FindFile(CString path, CString extends){
	CFileFind finder;
	BOOL working = finder.FindFile(path + L"\\" + extends);
	int i = 0;
	while(working){
		working = finder.FindNextFile();
		if(finder.IsDots())	continue;
		if(finder.IsDirectory()){
		}   
		else{    
			FileInfos_ fi; 
			fi.index = i;
			fi.filename = finder.GetFileName();     
			fi.full_path = finder.GetFilePath();
			m_FileList.Add(fi);
			i++;
		}
	}
	return true;
}


void CDlgAddDrawingNumber::OnBnClickedButtonListSave()
{
	CString str = _T("All files(*.txt)|*.txt|"); 
	CFileDialog dlg(TRUE, _T("*.txt"), NULL, OFN_HIDEREADONLY , str, this);

	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();


		CString strRes = _T("CT_ADN\r\n");
		CFile file;
		if(!(file.Open(strPathName,CFile::modeCreate | CFile::modeWrite)))
			return ;

		TCHAR szTmp[2];
		WORD wd = 0xfeff; // 유니코드 BOM
		memcpy(szTmp,&wd,2);
		file.Write(szTmp,2);
		
		file.Write(strRes,strRes.GetLength()*sizeof(TCHAR)); // 
		for(int i=0; i<m_FileList.GetCount(); i++)
		{
			strRes.Format(_T("%s\r\n"),m_FileList[i].full_path);
			file.Write(strRes,strRes.GetLength()*sizeof(TCHAR)); // 
		}
		file.Close();
	}
	
}


void CDlgAddDrawingNumber::OnBnClickedButtonListOpen()
{
	m_FileList.RemoveAll();
	CString str = _T("All files(*.txt)|*.txt|"); 
	CFileDialog dlg(FALSE, _T("*.txt"), NULL, OFN_HIDEREADONLY , str, this);

	if (dlg.DoModal() == IDOK)
	{
		CFile file;  

		CString strPathName = dlg.GetPathName();
		file.Open(strPathName, CFile::modeRead); //  fName 의 파일을 열고
		CArchive ar(&file, CArchive::load);
		int i=0;
		CString path;
		while (ar.ReadString(path))  //한줄씩 파일을 읽어와서 cList저장
		{
				if(i ==0)
				{
					if(!path.CompareNoCase(_T("﻿CT_ADN"))==0)
					{
						AfxMessageBox(_T("파일형식이 맞지 않습니다."));
						ar.Close();
						file.Close();
						return;
					}
					i++;
					continue;
				}

				TCHAR drive[_MAX_DRIVE+1], dir[_MAX_DIR+1], fname[_MAX_FNAME+_MAX_EXT+1], ext[_MAX_EXT+1];
				errno_t err = _tsplitpath_s(path, drive, dir, fname, ext);

				FileInfos_ fi; 
				fi.index = i;
				fi.filename.Format(_T("%s%s"),fname,ext);
				fi.full_path = path;
				m_FileList.Add(fi);
				i++;
			

			
		}
		ar.Close();
		file.Close();
		FileListReIndexing();
		

	}
}


void CDlgAddDrawingNumber::OnBnClickedButtonListSort()
{
	QSortCArray( m_FileList, CompareStringAscending_);
	FileListReIndexing();

}

void CDlgAddDrawingNumber::OnBnClickedButtonExit()
{
	DestroyWindow(); 
}

BOOL CDlgAddDrawingNumber::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	Maindlg = NULL;

	return CAcUiDialog::DestroyWindow();
}

void CDlgAddDrawingNumber::OnCancel()
{
	// TODO: Add extra cleanup here
	DestroyWindow();

	//CAcUiDialog::OnCancel();
}

void CDlgAddDrawingNumber::OnSortClick1()
{
	UpdateData(true);
	m_iSort = 0;
	UpdateData(false);
}

void CDlgAddDrawingNumber::OnSortClick2()
{
	UpdateData(true);
	m_iSort = 1;
	UpdateData(false);
}

void CDlgAddDrawingNumber::OnSortClick3()
{
	UpdateData(true);
	m_iSort = 2;
	UpdateData(false);
}

void CDlgAddDrawingNumber::OnSortClick4()
{
	UpdateData(true);
	m_iSort = 3;
	UpdateData(false);
}

//void CDlgAddDrawingNumber::OnClose()
//{
//	Maindlg = NULL;
//	CAcUiDialog::OnClose();
//}

//void CDlgAddDrawingNumber::PostNcDestroy()
//{
//	delete this;
//	Maindlg = NULL;
//	//CAcUiDialog::PostNcDestroy();
//}

//void CDlgAddDrawingNumber::OnDestroy()
//{
//	CAcUiDialog::OnDestroy();
//	Maindlg = NULL;
//}


