// FW.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FW.h"
#include "afxdialogex.h"


// CFW 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFW, CAcUiDialog)

CFW::CFW(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CFW::IDD, pParent),m_LineLayerStr(_T("")), m_TxtLayerStr(_T("")), m_TxtHeightStr(_T("")),
	m_TxtWidthStr(_T("")), m_LeaderLengthStr(_T("")), m_PreFaceStr(_T("")), m_DecimalStr(_T("")), G_Form1(0),m_ELValue_Str(_T(""))
{

}

CFW::~CFW()
{
}

void CFW::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FWTXTHEIGHT, m_TextHeight_Ctrl);
	//DDX_Control(pDX, IDC_FORMBITMAP, Form1);
	DDX_Radio(pDX, IDC_FWFORM1, G_Form1);
	DDX_Text(pDX, IDC_FWLINELAYER, m_LineLayerStr);
	DDX_Text(pDX, IDC_FWTXTLAYER, m_TxtLayerStr);
	DDX_Text(pDX, IDC_FWTXTHEIGHT, m_TxtHeightStr);
	DDX_Text(pDX, IDC_FWTXTWIDTH, m_TxtWidthStr);
	DDX_Text(pDX, IDC_FWLDLENGTH, m_LeaderLengthStr);
	DDX_Text(pDX, IDC_FWDECIMAL, m_DecimalStr);
	DDX_Text(pDX, IDC_FWPREFACE, m_PreFaceStr);
}

BEGIN_MESSAGE_MAP(CFW, CAcUiDialog)
	ON_BN_CLICKED(IDC_FWFORM1, &CFW::OnBnClickedFwform1)
	ON_BN_CLICKED(IDC_FWFORM2, &CFW::OnBnClickedFwform2)
	ON_BN_CLICKED(IDC_FWFORM3, &CFW::OnBnClickedFwform3)
	ON_BN_CLICKED(IDC_FWFORM4, &CFW::OnBnClickedFwform4)
	ON_BN_CLICKED(IDC_FWFORM5, &CFW::OnBnClickedFwform5)
	ON_BN_CLICKED(IDOK, &CFW::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFW::OnBnClickedCancel)
	ON_EN_UPDATE(IDC_FWLINELAYER, &CFW::OnEnUpdateFwlinelayer)
	ON_EN_UPDATE(IDC_FWTXTLAYER, &CFW::OnEnUpdateFwtxtlayer)
	ON_EN_UPDATE(IDC_FWTXTHEIGHT, &CFW::OnEnUpdateFwtxtheight)
	ON_EN_UPDATE(IDC_FWTXTWIDTH, &CFW::OnEnUpdateFwtxtwidth)
	ON_EN_UPDATE(IDC_FWLDLENGTH, &CFW::OnEnUpdateFwldlength)
	ON_EN_UPDATE(IDC_FWDECIMAL, &CFW::OnEnUpdateFwdecimal)
	ON_EN_UPDATE(IDC_FWPREFACE, &CFW::OnEnUpdateFwpreface)
END_MESSAGE_MAP()


// CFW 메시지 처리기입니다.

BOOL CFW::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	//버튼 이미지
	m_bSelect.AutoLoad();

	CBitmap Form_B1, Form_B2, Form_B3, Form_B4, Form_B5;
	//형식-1 = 1,형식-2 = 2,형식-3 = 3,형식-4 = 4,형식-5 = 5
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("JOBMODE"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0 ){
		OnBnClickedFwform1();
	}else if(tempStr.CompareNoCase(_T("2")) == 0){
		OnBnClickedFwform2();
	}else if(tempStr.CompareNoCase(_T("3")) == 0){
		OnBnClickedFwform3();
	}else if(tempStr.CompareNoCase(_T("4")) == 0){
		OnBnClickedFwform4();
	}else if(tempStr.CompareNoCase(_T("5")) == 0){
		OnBnClickedFwform5();
	}
	CStatic* p_image = (CStatic*)GetDlgItem(IDC_PICFORM);// Picture Control 이미지 출력
	CBitmap FormBit;
	if(G_Form1 == 0)
		FormBit.LoadBitmap(IDB_FORM1);
	else if(G_Form1 == 1)
		FormBit.LoadBitmap(IDB_FORM2);
	else if(G_Form1 == 2)
		FormBit.LoadBitmap(IDB_FORM3);
	else if(G_Form1 == 3)
		FormBit.LoadBitmap(IDB_FORM4);
	else if(G_Form1 == 4)
		FormBit.LoadBitmap(IDB_FORM5);
	
	HBITMAP h_old_bitmap = p_image->SetBitmap(FormBit);
	if(h_old_bitmap != NULL)::DeleteObject(h_old_bitmap);
	FormBit.Detach();
	//선 레이어
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("LineLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWLINELAYER, _T("CC-LEAL"));
	else
		SetDlgItemText(IDC_FWLINELAYER, tempStr);
	//문자 레이어
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("TextLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWTXTLAYER, _T("CC-LEAT"));
	else
		SetDlgItemText(IDC_FWTXTLAYER, tempStr);
	//문자 높이
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("TextHeight"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWTXTHEIGHT, _T("0.300"));
	else
		SetDlgItemText(IDC_FWTXTHEIGHT, tempStr);
	//문자 폭
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("TextWidth"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWTXTWIDTH, _T("1.000"));
	else
		SetDlgItemText(IDC_FWTXTWIDTH, tempStr);
	//지시선 길이
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("Length"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWLDLENGTH, _T("1.000"));
	else
		SetDlgItemText(IDC_FWLDLENGTH, tempStr);
	//소수점
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("DotCnt"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWDECIMAL, _T("2"));
	else
		SetDlgItemText(IDC_FWDECIMAL, tempStr);
	//머리말
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("PreFace"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWPREFACE, _T("EL="));
	else
		SetDlgItemText(IDC_FWPREFACE, tempStr);



	return TRUE;
}

void CFW::OnBnClickedOk()
{
	UpdateData();
	CAcUiDialog::OnOK();
	//대화상자 설정값 저장
	CString modeStr;
	if(!(FormStat == 1 || FormStat == 2 || FormStat == 3 || FormStat == 4 || FormStat == 5))FormStat = 1;
	modeStr.Format(_T("%d"), FormStat);
	iniFile.writeINI(_T("config.ini"), _T("FW"), _T("JOBMODE"),modeStr);
	iniFile.writeINI(_T("config.ini"), _T("FW"), _T("LineLayer"),m_LineLayerStr);
	iniFile.writeINI(_T("config.ini"), _T("FW"), _T("TextLayer"),m_TxtLayerStr);
	iniFile.writeINI(_T("config.ini"), _T("FW"), _T("TextHeight"),m_TxtHeightStr);
	iniFile.writeINI(_T("config.ini"), _T("FW"), _T("TextWidth"),m_TxtWidthStr);
	iniFile.writeINI(_T("config.ini"), _T("FW"), _T("Length"),m_LeaderLengthStr);
	iniFile.writeINI(_T("config.ini"), _T("FW"), _T("DotCnt"),m_DecimalStr);
	iniFile.writeINI(_T("config.ini"), _T("FW"), _T("PreFace"),m_PreFaceStr);

	double m_Scale, m_ELValue;
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("Scale"));//축척 불러옴
	if(tempStr.CompareNoCase(_T("")) == 0){
		m_Scale = 1000;
	}else{
		m_Scale = _tstof(tempStr);
	}
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("ELValue"));//EL 값 불러오기
	if(tempStr.CompareNoCase(_T("")) == 0){
		m_ELValue = 0;
	}else{
		m_ELValue = _tstof(tempStr);
	}

	CString S_ScaleStr, EntStr, inString, RTERROR_str,Scale_Print,Scale_Print2;
	ads_name ens;
	int stat;
	AcGePoint3d ELpt;
	bool b_TF=true;

	BeginEditorCommand();
	while(b_TF){
		acedInitGet(0, _T("c C 0 1 2 3 4 5 6 7 8 9 10"));
		EntStr.Format(_T("기준점 EL 선택 또는 값 입력(%.3f)<축척 변경(C)>: "), m_ELValue);
		Scale_Print.Format(_T("\n< 현재설정 : 축척=%0.2f >"), m_Scale);
		Scale_Print2.Format(_T("< 현재설정 : 축척=%0.2f >"), m_Scale);//줄 바꿈 문자로인해 CompareNoCase로 인식못하여 새로만듬
		acutPrintf(Scale_Print);
		stat = acedEntSel(EntStr, ens, asDblArray(ELpt));
		if(stat == RTCAN || stat == RTNONE){
			b_TF = false;
		}else if(stat == RTKWORD){
			resbuf zValueStr,zValueStr2;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
			RTERROR_str.Replace(EntStr, _T(""));
			ads_getvar(_T("ERRNO"), &zValueStr2);//acedEntSel NULL을 구할 때 사용
			
			if(RTERROR_str.CompareNoCase(_T("")) == 0)
				continue;
			else if(RTERROR_str.CompareNoCase(_T("C")) == 0 || RTERROR_str.CompareNoCase(_T("c")) == 0){
				S_ScaleStr.Format(_T("\n축척 입력<%.2f> : "),m_Scale);

				if(acedGetReal(S_ScaleStr,&m_Scale)==RTNORM){
					m_ScaleStr.Format(_T("%0.2f"),m_Scale);
					iniFile.writeINI(_T("config.ini"), _T("FW"), _T("Scale"),m_ScaleStr);//축척 저장
				}	
			}else{
				if(RTERROR_str.GetLength() > 0){//마우스 우클릭 또는 엔터시 문자열수가 0이므로 기존EL값으로 출력
					m_ELValue_Str.Format(_T("%s"),RTERROR_str);
					iniFile.writeINI(_T("config.ini"), _T("FW"), _T("ELValue"),m_ELValue_Str);//EL 값 저장
					break;
				}else{
					m_ELValue_Str.Format(_T("%f"),m_ELValue);
					break;
				}
			}
		}else if(stat == RTERROR){
			resbuf zValueStr, zValueStr2;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			ads_getvar(_T("ERRNO"), &zValueStr2);
			RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
			RTERROR_str.Replace(EntStr, _T(""));
			if(zValueStr2.resval.rint == 52)//52  = NULL(ERRNO)
			{	// 값을 기입하지 않거나 객체를 선택하지 않을 경우, EL값을 추가하지 않음
				m_ELValue_Str.Format(_T("%f"),m_ELValue);	// 김성곤 대리 수정
				break;
			}
			if(RTERROR_str.CompareNoCase(Scale_Print2) == 0){//_T("< 현재설정 : 축척=%0.f >")가 반환되면 반복
				continue;
			}else if(RTERROR_str.CompareNoCase(_T("**선택한 객체는 문자가 아닙니다.**")) == 0){
				continue;
			}else{
				if(RTERROR_str.GetLength() > 0){//마우스 우클릭 또는 엔터시 문자열수가 0이므로 기존EL값으로 출력
					m_ELValue_Str.Format(_T("%s"),RTERROR_str);
					iniFile.writeINI(_T("config.ini"), _T("FW"), _T("ELValue"),m_ELValue_Str);//EL 값 저장
					break;
				}else{
					m_ELValue_Str.Format(_T("%f"),m_ELValue);
					break;
				}
					
			}
		}else if(stat == RTNORM){//텍스트 선택시 숫자만 추출
			 AcGePoint3d insertPoint;
			 AcDbObjectId objId;
			 acdbGetObjectId(objId, ens);
			 AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			 if(pEnt.openStatus() == Acad::eOk){
			 	if(pEnt->isKindOf(AcDbText::desc())){
			 		AcDbText* pText = AcDbText::cast(pEnt);
			 		inString.Format(_T("%s"), pText->textString());
			 		inString = CheckInputNumber(inString);//텍스트에서 숫자,점 추출
			 		pText->close();
				}else{
					acutPrintf(_T("\n**선택한 객체는 문자가 아닙니다.**\n"));
					continue;
				}
			 }
			pEnt->close();
			acedSSFree(ens);	
			m_ELValue_Str = inString;
			iniFile.writeINI(_T("config.ini"), _T("FW"), _T("ELValue"),m_ELValue_Str);//EL 값 저장
			b_TF = false;
		}
		
	}//while end
	AcGePoint3dArray ptArray;
	AcGePoint3d STspt, STept, Formpt,Formpt_L,Formpt_R, tmp_STspt;
	CString getptStr;
	m_ELValue = _tstof(m_ELValue_Str);
	int Mode;
	bool b_Mark = true;
	double STang, UnderLength, NewELValue;
	//레이어 =  m_LineLayerStr,텍스트 레이어 = m_TxtLayerStr, 머리말 = m_PreFaceStr
	double L_Length = _tstof(m_LeaderLengthStr);//지시선
	double L_TxtHeight = _tstof(m_TxtHeightStr);//문자 높이
	double L_TxtWidth = _tstof(m_TxtWidthStr);//문자 폭
	double L_DotNum = _tstof(m_DecimalStr);// 소수점
	getptStr.Format(_T("\n기준점(%.3f)선택 : "), m_ELValue);
	if(acedGetPoint(NULL, getptStr, asDblArray(STspt)) != RTNORM)return;
	tmp_STspt = STspt;
	acdbUcs2Wcs(asDblArray(STspt),asDblArray(STspt),Adesk::kFalse); // 김성곤 대리 추가
	
	while(true){
		if(acedGetPoint(asDblArray(tmp_STspt),_T("\nEL 표기점 선택 : "), asDblArray(STept)) != RTNORM)return;
		acdbUcs2Wcs(asDblArray(STept),asDblArray(STept),Adesk::kFalse); // 김성곤 대리 추가

		STang = ads_angle(asDblArray(STspt), asDblArray(STept));//폴리선의 좌우결정
		if((STang >=DTR(0.0) &&STang <=DTR(90.0)) || (STang >DTR(270.0) && STang <DTR(360)))
			Mode = 1;//폴리선 우측
		else
			Mode = 2;//폴리선 좌측
		
		if(FormStat == 1){/////////////////형식-1/////////////////////
			AcGePoint3d H_Txtpt;
			
			//텍스트 생성
			//NewELValue = m_ELValue + (STept.y - STspt.y);
			NewELValue = m_ELValue + ((STept.y - STspt.y) * m_Scale / 1000) ; // 김성곤 대리 수정
			CString Txtstr1, Txt1;//이중 포멧으로 소수점 지정하기
			int H_DecimalNum = L_DotNum;
			Txt1.Format(_T("%s%%.%df"),m_PreFaceStr, H_DecimalNum);
			Txtstr1.Format(Txt1, NewELValue);//텍스트값은 축척에 따라 감소하고 객체는 증가함

			if(L_DotNum == 0)//축척1:1 -> 등호,숫자,문자 = 0.2331, 점 = 0.1164, 나머지부분 = 0.2  폴리라인 가로길이
				UnderLength = (Txtstr1.GetLength()*0.2331 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);
			else if(L_DotNum > 0)
				UnderLength = ((Txtstr1.GetLength()-1)*0.2331 + 0.1164 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);

			ads_polar(asDblArray(STept), DTR(90.0), 1000/m_Scale * L_Length, asDblArray(Formpt));

			if(Mode == 1){
				H_Txtpt.y = Formpt.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;
				H_Txtpt.x = Formpt.x +(0.05 * L_TxtWidth) * (L_TxtHeight / 0.3) * 1000/m_Scale;
			}else if(Mode == 2){
				H_Txtpt.y = Formpt.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;
				H_Txtpt.x = Formpt.x - (UnderLength - (0.05 * L_TxtWidth) * (L_TxtHeight / 0.3)) * 1000/m_Scale;
			}
			CreateLayer(m_TxtLayerStr, 3);//텍스트 레이어 생성
			bool m_Text = CreateText(H_Txtpt, 1000/m_Scale*L_TxtHeight,L_TxtWidth, 0.0, Txtstr1, m_TxtLayerStr, 0);//텍스트 생성
				if(m_Text == false)break;

			//폴리라인
			ads_polar(asDblArray(Formpt), 0.0, 1000/m_Scale * UnderLength, asDblArray(Formpt_R));//우측 포인트
			ads_polar(asDblArray(Formpt), DTR(180.0), 1000/m_Scale * UnderLength, asDblArray(Formpt_L));//좌측 포인트
			if(Mode == 1){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_R);
			}else if(Mode == 2){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_L);
			}
			CreateLayer(m_LineLayerStr, 1);//라인 레이어 생성
			bool b_PLine = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//폴리선 생성
			ptArray.removeAll();//배열초기화
			if(b_PLine == false)break;

		}else if(FormStat == 2){//////////형식-2////////////////////
			AcGePoint3d H_Txtpt;
			
			//텍스트 생성
			//NewELValue = m_ELValue + (STept.y - STspt.y);
			NewELValue = m_ELValue + ((STept.y - STspt.y) * m_Scale / 1000) ; // 김성곤 대리 수정
			CString Txtstr1, Txt1;//이중 포멧으로 소수점 지정하기
			int H_DecimalNum = L_DotNum;
			Txt1.Format(_T("%s%%.%df"),m_PreFaceStr, H_DecimalNum);
			Txtstr1.Format(Txt1, NewELValue);

			if(L_DotNum == 0)//축척1000 -> 등호,숫자,문자 = 0.2331, 점 = 0.1164, 나머지부분 = 0.2  폴리라인 가로길이
				UnderLength = (Txtstr1.GetLength()*0.2331 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);
			else if(L_DotNum > 0)
				UnderLength = ((Txtstr1.GetLength()-1)*0.2331 + 0.1164 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);

			ads_polar(asDblArray(STept), DTR(270.0), 1000/m_Scale * L_Length, asDblArray(Formpt));

			if(Mode == 1){
				H_Txtpt.y = Formpt.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;
				H_Txtpt.x = Formpt.x + (0.1 * L_TxtWidth) * (L_TxtHeight / 0.3) * 1000/m_Scale;
			}else if(Mode == 2){
				H_Txtpt.y = Formpt.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;
				H_Txtpt.x = Formpt.x - (UnderLength - (0.1 * L_TxtWidth) * (L_TxtHeight / 0.3)) * 1000/m_Scale;
			}
			CreateLayer(m_TxtLayerStr, 3);//텍스트 레이어 생성
			bool m_Text = CreateText(H_Txtpt, 1000/m_Scale*L_TxtHeight,L_TxtWidth, 0.0, Txtstr1, m_TxtLayerStr, 0);//텍스트 생성
				if(m_Text == false)break;

			//폴리라인
			ads_polar(asDblArray(Formpt), 0.0, 1000/m_Scale * UnderLength, asDblArray(Formpt_R));//우측 포인트
			ads_polar(asDblArray(Formpt), DTR(180.0), 1000/m_Scale * UnderLength, asDblArray(Formpt_L));//좌측 포인트
			if(Mode == 1){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_R);
			}else if(Mode == 2){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_L);
			}
			CreateLayer(m_LineLayerStr, 1);//라인 레이어 생성
			bool b_PLine = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//폴리선 생성
			ptArray.removeAll();//배열초기화
			if(b_PLine == false)break;

		}else if(FormStat == 3){//////////형식-3////////////////////
			AcGePoint3d H_Txtpt,Formpt2,W_Formpt,W_Formpt2,H_Formpt,H_Formpt2;
			
			//텍스트 생성
			//NewELValue = m_ELValue + (STept.y - STspt.y);
			NewELValue = m_ELValue + ((STept.y - STspt.y) * m_Scale / 1000) ; // 김성곤 대리 수정
			CString Txtstr1, Txt1;//이중 포멧으로 소수점 지정하기
			int H_DecimalNum = L_DotNum;
			Txt1.Format(_T("%s%%.%df"),m_PreFaceStr, H_DecimalNum);
			Txtstr1.Format(Txt1, NewELValue);

			if(L_DotNum == 0)//축척1:1 -> 등호,숫자,문자 = 0.2331, 점 = 0.1164, 나머지부분 = 0.2  폴리라인 가로길이
				UnderLength = (Txtstr1.GetLength()*0.2331 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);
			else if(L_DotNum > 0)
				UnderLength = ((Txtstr1.GetLength()-1)*0.2331 + 0.1164 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);

			ads_polar(asDblArray(STept), 0.0, 1000/m_Scale * L_Length / 2, asDblArray(W_Formpt));
			ads_polar(asDblArray(W_Formpt), DTR(90.0), 1000/m_Scale * L_Length, asDblArray(H_Formpt));
			ads_polar(asDblArray(STept), DTR(180.0), 1000/m_Scale * L_Length/2, asDblArray(W_Formpt2));
			ads_polar(asDblArray(W_Formpt2), DTR(90.0), 1000/m_Scale * L_Length, asDblArray(H_Formpt2));

			if(Mode == 1){
				H_Txtpt.y = H_Formpt.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;//0.15 -> 지시선과 텍스트 사이
				H_Txtpt.x = H_Formpt.x + (0.05 * L_TxtWidth)  * (L_TxtHeight / 0.3)* 1000/m_Scale;
			}else if(Mode == 2){
				H_Txtpt.y = H_Formpt2.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;//0.15 -> 지시선과 텍스트 사이
				H_Txtpt.x = H_Formpt2.x - (UnderLength - (0.05 * L_TxtWidth) * (L_TxtHeight / 0.3)) * 1000/m_Scale;
			}
			CreateLayer(m_TxtLayerStr, 3);//텍스트 레이어 생성
			bool m_Text = CreateText(H_Txtpt, 1000/m_Scale*L_TxtHeight,L_TxtWidth, 0.0, Txtstr1, m_TxtLayerStr, 0);//텍스트 생성
				if(m_Text == false)break;

			//폴리라인
			ads_polar(asDblArray(H_Formpt), 0.0, 1000/m_Scale * UnderLength, asDblArray(Formpt_R));//우측 포인트
			ads_polar(asDblArray(H_Formpt2), DTR(180.0), 1000/m_Scale * UnderLength, asDblArray(Formpt_L));//좌측 포인트
			if(Mode == 1){
				ptArray.append(STept); ptArray.append(H_Formpt); ptArray.append(Formpt_R);
			}else if(Mode == 2){
				ptArray.append(STept); ptArray.append(H_Formpt2); ptArray.append(Formpt_L);
			}
			CreateLayer(m_LineLayerStr, 1);//라인 레이어 생성
			bool b_PLine = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//폴리선 생성
			ptArray.removeAll();//배열초기화
			if(b_PLine == false)break;

		}else if(FormStat == 4){//////////형식-4////////////////////
			AcGePoint3d H_Txtpt,Formpt2,W_Formpt,W_Formpt2,H_Formpt,H_Formpt2;
			
			//텍스트 생성
			//NewELValue = m_ELValue + (STept.y - STspt.y);
			NewELValue = m_ELValue + ((STept.y - STspt.y) * m_Scale / 1000) ; // 김성곤 대리 수정
			CString Txtstr1, Txt1;//이중 포멧으로 소수점 지정하기
			int H_DecimalNum = L_DotNum;
			Txt1.Format(_T("%s%%.%df"),m_PreFaceStr, H_DecimalNum);
			Txtstr1.Format(Txt1, NewELValue);

			if(L_DotNum == 0)//축척1:1 -> 등호,숫자,문자 = 0.2331, 점 = 0.1164, 나머지부분 = 0.2  폴리라인 가로길이
				UnderLength = (Txtstr1.GetLength()*0.2331 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);
			else if(L_DotNum > 0)
				UnderLength = ((Txtstr1.GetLength()-1)*0.2331 + 0.1164 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);

			ads_polar(asDblArray(STept), 0.0, 1000/m_Scale * L_Length / 2, asDblArray(W_Formpt));
			ads_polar(asDblArray(W_Formpt), DTR(270.0), 1000/m_Scale * L_Length, asDblArray(H_Formpt));
			ads_polar(asDblArray(STept), DTR(180.0), 1000/m_Scale * L_Length/2, asDblArray(W_Formpt2));
			ads_polar(asDblArray(W_Formpt2), DTR(270.0), 1000/m_Scale * L_Length, asDblArray(H_Formpt2));

			if(Mode == 1){
				H_Txtpt.y = H_Formpt.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;//0.15 -> 지시선과 텍스트 사이
				H_Txtpt.x = H_Formpt.x + (0.05 * L_TxtWidth) * (L_TxtHeight / 0.3) * 1000/m_Scale;
			}else if(Mode == 2){
				H_Txtpt.y = H_Formpt2.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;//0.15 -> 지시선과 텍스트 사이
				H_Txtpt.x = H_Formpt2.x - ((UnderLength - (0.05 * L_TxtWidth) * (L_TxtHeight / 0.3)) * 1000/m_Scale);
			}
			CreateLayer(m_TxtLayerStr, 3);//텍스트 레이어 생성
			bool m_Text = CreateText(H_Txtpt, 1000/m_Scale*L_TxtHeight,L_TxtWidth, 0.0, Txtstr1, m_TxtLayerStr, 0);//텍스트 생성
				if(m_Text == false)break;

			//폴리라인
			ads_polar(asDblArray(H_Formpt), 0.0, 1000/m_Scale * UnderLength, asDblArray(Formpt_R));//우측 포인트
			ads_polar(asDblArray(H_Formpt2), DTR(180.0), 1000/m_Scale * UnderLength, asDblArray(Formpt_L));//좌측 포인트

			if(Mode == 1){
				ptArray.append(STept); ptArray.append(H_Formpt); ptArray.append(Formpt_R);
			}else if(Mode == 2){
				ptArray.append(STept); ptArray.append(H_Formpt2); ptArray.append(Formpt_L);
			}
			CreateLayer(m_LineLayerStr, 1);//라인 레이어 생성
			bool b_PLine = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//폴리선 생성
			ptArray.removeAll();//배열초기화
			if(b_PLine == false)break;

		}else if(FormStat == 5){//////////형식-5////////////////////
			AcGePoint3d H_Txtpt;

			//텍스트 생성
			//NewELValue = m_ELValue + (STept.y - STspt.y);
			NewELValue = m_ELValue + ((STept.y - STspt.y) * m_Scale / 1000) ; // 김성곤 대리 수정
			CString Txtstr1, Txt1;//이중 포멧으로 소수점 지정하기
			int H_DecimalNum = L_DotNum;
			Txt1.Format(_T("%s%%.%df"),m_PreFaceStr, H_DecimalNum);
			Txtstr1.Format(Txt1, NewELValue);

			if(L_DotNum == 0)//축척1:1 -> 등호,숫자,문자 = 0.2331, 점 = 0.1164, 나머지부분 = 0.2  폴리라인 가로길이
				UnderLength = (Txtstr1.GetLength()*0.2331 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);
			else if(L_DotNum > 0)
				UnderLength = ((Txtstr1.GetLength()-1)*0.2331 + 0.1164 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);//-1은 소수점을 뺀 것

			ads_polar(asDblArray(STept), DTR(90.0), 1000/m_Scale * L_Length, asDblArray(Formpt));

			if(Mode == 1){
				H_Txtpt.y = Formpt.y + 0.15 * (L_TxtHeight / 0.3) * 1000/m_Scale;
				H_Txtpt.x = Formpt.x + (0.05 * L_TxtWidth) * (L_TxtHeight / 0.3) * 1000/m_Scale;
			}else if(Mode == 2){
				H_Txtpt.y = Formpt.y + 0.15 * (L_TxtHeight / 0.3) * 1000/m_Scale;
				H_Txtpt.x = Formpt.x - (UnderLength - (0.05 * L_TxtWidth) * (L_TxtHeight / 0.3)) * 1000/m_Scale ;
			}

			CreateLayer(m_TxtLayerStr, 3);//텍스트 레이어 생성
			bool m_Text = CreateText(H_Txtpt, 1000/m_Scale*L_TxtHeight,L_TxtWidth, 0.0, Txtstr1, m_TxtLayerStr, 0);//텍스트 생성
				if(m_Text == false)break;

			//폴리라인
			ads_polar(asDblArray(Formpt), 0.0, 1000/m_Scale * UnderLength, asDblArray(Formpt_R));//우측 포인트
			ads_polar(asDblArray(Formpt), DTR(180.0), 1000/m_Scale * UnderLength, asDblArray(Formpt_L));//좌측 포인트
			if(Mode == 1){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_R);
			}else if(Mode == 2){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_L);
			}
			CreateLayer(m_LineLayerStr, 1);//라인 레이어 생성
			bool b_PLine = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//폴리선 생성
			ptArray.removeAll();//배열초기화
			if(b_PLine == false)break;
			//화살표 폴리라인
			ads_polar(asDblArray(STept), DTR(90.0), 1000/m_Scale * 0.3, asDblArray(Formpt));
			ads_polar(asDblArray(Formpt), 0.0, 1000/m_Scale * 0.15, asDblArray(Formpt_R));
			ptArray.append(Formpt); ptArray.append(Formpt_R); ptArray.append(STept); 
			bool b_Arrow = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//폴리선 생성
			ptArray.removeAll();//배열 초기화
			if(b_Arrow == false)break;

			//화살표 솔리드
			ads_polar(asDblArray(STept), DTR(90.0),1000/m_Scale * 0.3, asDblArray(Formpt));
			ads_polar(asDblArray(Formpt), DTR(180.0),1000/m_Scale * 0.15, asDblArray(Formpt_L));
			bool b_Solid = CreateSolid(STept, Formpt, Formpt_L, m_LineLayerStr);//화살표 솔리드 생성
			if(b_Solid == false)break;

		}
		

	}//while end
	CompleteEditorCommand();

}

void CFW::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

//라디오버튼 1~5
void CFW::OnBnClickedFwform1()
{
	UpdateData(TRUE);
	G_Form1=0;
	FormStat = 1;

	CStatic* p_image = (CStatic*)GetDlgItem(IDC_PICFORM);
	CBitmap image;
	image.LoadBitmap(IDB_FORM1);
	HBITMAP h_old_bitmap = p_image->SetBitmap(image);
	if(h_old_bitmap != NULL)::DeleteObject(h_old_bitmap);
	image.Detach();

	UpdateData(FALSE);
}

void CFW::OnBnClickedFwform2()
{
	UpdateData(TRUE);
	G_Form1=1;
	FormStat = 2;

	CStatic* p_image = (CStatic*)GetDlgItem(IDC_PICFORM);
	CBitmap image;
	image.LoadBitmap(IDB_FORM2);
	HBITMAP h_old_bitmap = p_image->SetBitmap(image);
	if(h_old_bitmap != NULL)::DeleteObject(h_old_bitmap);
	image.Detach();
	UpdateData(FALSE);
}


void CFW::OnBnClickedFwform3()
{
	UpdateData(TRUE);
	G_Form1=2;
	FormStat = 3;

	CStatic* p_image = (CStatic*)GetDlgItem(IDC_PICFORM);
	CBitmap image;
	image.LoadBitmap(IDB_FORM3);
	HBITMAP h_old_bitmap = p_image->SetBitmap(image);
	if(h_old_bitmap != NULL)::DeleteObject(h_old_bitmap);
	image.Detach();
	UpdateData(FALSE);
}


void CFW::OnBnClickedFwform4()
{
	UpdateData(TRUE);
	G_Form1=3;
	FormStat = 4;

	CStatic* p_image = (CStatic*)GetDlgItem(IDC_PICFORM);
	CBitmap image;
	image.LoadBitmap(IDB_FORM4);
	HBITMAP h_old_bitmap = p_image->SetBitmap(image);
	if(h_old_bitmap != NULL)::DeleteObject(h_old_bitmap);
	image.Detach();
	UpdateData(FALSE);
}


void CFW::OnBnClickedFwform5()
{
	UpdateData(TRUE);
	G_Form1=4;
	FormStat = 5;

	CStatic* p_image = (CStatic*)GetDlgItem(IDC_PICFORM);
	CBitmap image;
	image.LoadBitmap(IDB_FORM5);
	HBITMAP h_old_bitmap = p_image->SetBitmap(image);
	if(h_old_bitmap != NULL)::DeleteObject(h_old_bitmap);
	image.Detach();
	UpdateData(FALSE);
}

//선 레이어
void CFW::OnEnUpdateFwlinelayer()
{
	UpdateData(TRUE);
	m_LineLayerStr;
	UpdateData(FALSE);
}
//문자 레이어
void CFW::OnEnUpdateFwtxtlayer()
{
	UpdateData(TRUE);
	m_TxtLayerStr;
	UpdateData(FALSE);
}
//문자 높이
void CFW::OnEnUpdateFwtxtheight()
{
	UpdateData(TRUE);
	m_TxtHeightStr = CheckInputNumber(m_TxtHeightStr);
	UpdateData(FALSE);

	m_TextHeight_Ctrl.SetSel(0, -1);
	m_TextHeight_Ctrl.SetSel(-1, -1);
	UpdateData(FALSE);
}
//문자 폭
void CFW::OnEnUpdateFwtxtwidth()
{
	UpdateData(TRUE);
	Width = _ttoi(m_TxtWidthStr);
	UpdateData(FALSE);
}
//지시선 길이
void CFW::OnEnUpdateFwldlength()
{
	UpdateData(TRUE);
	Length = _ttoi(m_LeaderLengthStr);
	UpdateData(FALSE);
}
//소수점
void CFW::OnEnUpdateFwdecimal()
{
	UpdateData(TRUE);
	DotCnt = _ttoi(m_DecimalStr);
	UpdateData(FALSE);
	if(DotCnt < 0 ){
		m_DecimalStr = _T("0");
	}
	else if(DotCnt > 6){
		m_DecimalStr = _T("6");
	}
	UpdateData(FALSE);
}
//머리말
void CFW::OnEnUpdateFwpreface()
{
	UpdateData(TRUE);
	m_PreFaceStr;
	UpdateData(FALSE);
}
//숫자 및 점(.) 입력 체크
CString CFW::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	CString strNum, strRe = _T("");
	int k = 0;
	for(int i = 0; i < str.GetLength(); i++){
		strNum = str.Mid(i,1);	//for문을 돌면 사라진문자때문에 다음문자를 찾으려면 Mid에서 0번째를 찾아야 함
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		if(((ascii >= 48 && ascii <= 57) || ascii == 46 || ascii == 46)){
			strRe.Append(getAtChar);
		}
	}
	return strRe;
} 

////각도를 라디안 값으로 변환하여 리턴한다.
double CFW::DTR(double angle)
{//ads_printf(_T("\n DTR() angle=%f \n"),angle);
	return (angle / 180.0 * PI);
}

////라디안 값을 각도로 변환하여 리턴한다.
double CFW::RTD(double radian)
{//ads_printf(_T("\n RTD() radian=%f \n"),radian);
	return (radian * 180.0 / PI);
}

//텍스트 생성
bool CFW::CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, int mode){
		Acad::ErrorStatus es;
		AcDbObjectId textStyleId, pTextObjId;
		double XScale = 0,obliquingAngle = 0;
		
		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
		if(pDb == NULL)return false;

		AcDbText* pText = new AcDbText();
		textStyleId = pDb->textstyle();
		pText->setTextStyle(textStyleId);
		AcDbTextStyleTableRecord *pTSTR=NULL;
		AcDbBlockTable* pBT;
		AcDbBlockTableRecord* pBTR;
		acdbOpenObject(pTSTR, textStyleId, AcDb::kForRead);
		if(pTSTR){
			XScale=pTSTR->xScale();
			obliquingAngle = pTSTR->obliquingAngle();
			pTSTR->close();
			pText->setWidthFactor(XScale);
		}
		
		pText->setOblique(obliquingAngle);
		pText->setLayer(Layer);
		pText->setPosition(pt);
		if(mode == 1){
			es = pText->setVerticalMode(AcDb::kTextBottom);
			es = pText->setHorizontalMode(AcDb::kTextCenter);
		}else{
			es = pText->setVerticalMode(AcDb::kTextBase);
			es = pText->setHorizontalMode(AcDb::kTextLeft);
		}
		pText->setAlignmentPoint(pt);
		pText->setHeight(Height);
		pText->setWidthFactor(Width);
		pText->setRotation(Rotate);
		pText->setTextString((LPCTSTR)TxtStr);
		
		if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(pTextObjId, pText);
			pBTR->close();
			pText->close();
		}
		pBT->close();
		
	}
		return true;
}

//폴리선 생성
bool CFW::CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer){
	Acad::ErrorStatus es;
	AcDbObjectId pLineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();

	if(pDb == NULL)return false;

	AcDbPolyline* pPline = new AcDbPolyline();
	for(int i =0; i<ptArray.length(); i++){
		pPline->addVertexAt(i, AcGePoint2d(ptArray[i].x, ptArray[i].y));
	}
	pPline->setConstantWidth(LineWidth);
	pPline->setLinetype(LineType);
	pPline->setLinetypeScale(LineTypeScale);
	pPline->setLayer(Layer);

	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(pLineObjId, pPline);
			pBTR->close();
			pPline->close();
		}else
			return false;
		pBT->close();
	}else
		return false;
	pPline->close();

	return true;
}
//레이어 생성
bool CFW::CreateLayer(CString Layer, int color){
	AcDbLayerTable* pLayerTable;
	AcCmColor Layercolor;
	
	Layercolor.setColorIndex(color);
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)return false;

	pDb->getLayerTable(pLayerTable, AcDb::kForWrite);

	if(pLayerTable->has(Layer) != Adesk::kTrue){
		AcDbLayerTableRecord* pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(Layer);
		pLayerTableRecord->setColor(Layercolor);

		AcDbLinetypeTable* pLinetypeTable;
		pDb->getLinetypeTable(pLinetypeTable, AcDb::kForRead);

		AcDbObjectId LinetypeId;
		pLinetypeTable->getAt(_T("BYLAYER"), LinetypeId);
		pLayerTableRecord->setLinetypeObjectId(LinetypeId);
		pLinetypeTable->close();
		pLayerTable->add(pLayerTableRecord);
		pLinetypeTable->close();
		pLayerTableRecord->close();
		pLayerTable->close();
	}else{
		pLayerTable->close();
		return false;
	}

	return true;
}

//솔리드 생성
bool CFW::CreateSolid(AcGePoint3d Solpt1, AcGePoint3d Solpt2, AcGePoint3d Solpt3, CString Layer){
	Acad::ErrorStatus es;
	AcDbObjectId SolidId;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)return false;

	AcDbBlockTable* pBT;
	AcDbBlockTableRecord* pBTR=NULL;

	AcDbSolid* Arrow = new AcDbSolid(Solpt1, Solpt2, Solpt3,Solpt3);
	Arrow->setLayer(Layer);
	

	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(SolidId, Arrow);
			pBTR->close();
			Arrow->close();
		}
		pBT->close();
		
	}
	return true;
}