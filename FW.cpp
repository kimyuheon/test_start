// FW.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FW.h"
#include "afxdialogex.h"


// CFW ��ȭ �����Դϴ�.

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


// CFW �޽��� ó�����Դϴ�.

BOOL CFW::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	//��ư �̹���
	m_bSelect.AutoLoad();

	CBitmap Form_B1, Form_B2, Form_B3, Form_B4, Form_B5;
	//����-1 = 1,����-2 = 2,����-3 = 3,����-4 = 4,����-5 = 5
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
	CStatic* p_image = (CStatic*)GetDlgItem(IDC_PICFORM);// Picture Control �̹��� ���
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
	//�� ���̾�
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("LineLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWLINELAYER, _T("CC-LEAL"));
	else
		SetDlgItemText(IDC_FWLINELAYER, tempStr);
	//���� ���̾�
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("TextLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWTXTLAYER, _T("CC-LEAT"));
	else
		SetDlgItemText(IDC_FWTXTLAYER, tempStr);
	//���� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("TextHeight"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWTXTHEIGHT, _T("0.300"));
	else
		SetDlgItemText(IDC_FWTXTHEIGHT, tempStr);
	//���� ��
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("TextWidth"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWTXTWIDTH, _T("1.000"));
	else
		SetDlgItemText(IDC_FWTXTWIDTH, tempStr);
	//���ü� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("Length"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWLDLENGTH, _T("1.000"));
	else
		SetDlgItemText(IDC_FWLDLENGTH, tempStr);
	//�Ҽ���
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("DotCnt"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_FWDECIMAL, _T("2"));
	else
		SetDlgItemText(IDC_FWDECIMAL, tempStr);
	//�Ӹ���
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
	//��ȭ���� ������ ����
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
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("Scale"));//��ô �ҷ���
	if(tempStr.CompareNoCase(_T("")) == 0){
		m_Scale = 1000;
	}else{
		m_Scale = _tstof(tempStr);
	}
	tempStr = iniFile.readINI(_T("config.ini"), _T("FW"), _T("ELValue"));//EL �� �ҷ�����
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
		EntStr.Format(_T("������ EL ���� �Ǵ� �� �Է�(%.3f)<��ô ����(C)>: "), m_ELValue);
		Scale_Print.Format(_T("\n< ���缳�� : ��ô=%0.2f >"), m_Scale);
		Scale_Print2.Format(_T("< ���缳�� : ��ô=%0.2f >"), m_Scale);//�� �ٲ� ���ڷ����� CompareNoCase�� �νĸ��Ͽ� ���θ���
		acutPrintf(Scale_Print);
		stat = acedEntSel(EntStr, ens, asDblArray(ELpt));
		if(stat == RTCAN || stat == RTNONE){
			b_TF = false;
		}else if(stat == RTKWORD){
			resbuf zValueStr,zValueStr2;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
			RTERROR_str.Replace(EntStr, _T(""));
			ads_getvar(_T("ERRNO"), &zValueStr2);//acedEntSel NULL�� ���� �� ���
			
			if(RTERROR_str.CompareNoCase(_T("")) == 0)
				continue;
			else if(RTERROR_str.CompareNoCase(_T("C")) == 0 || RTERROR_str.CompareNoCase(_T("c")) == 0){
				S_ScaleStr.Format(_T("\n��ô �Է�<%.2f> : "),m_Scale);

				if(acedGetReal(S_ScaleStr,&m_Scale)==RTNORM){
					m_ScaleStr.Format(_T("%0.2f"),m_Scale);
					iniFile.writeINI(_T("config.ini"), _T("FW"), _T("Scale"),m_ScaleStr);//��ô ����
				}	
			}else{
				if(RTERROR_str.GetLength() > 0){//���콺 ��Ŭ�� �Ǵ� ���ͽ� ���ڿ����� 0�̹Ƿ� ����EL������ ���
					m_ELValue_Str.Format(_T("%s"),RTERROR_str);
					iniFile.writeINI(_T("config.ini"), _T("FW"), _T("ELValue"),m_ELValue_Str);//EL �� ����
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
			{	// ���� �������� �ʰų� ��ü�� �������� ���� ���, EL���� �߰����� ����
				m_ELValue_Str.Format(_T("%f"),m_ELValue);	// �輺�� �븮 ����
				break;
			}
			if(RTERROR_str.CompareNoCase(Scale_Print2) == 0){//_T("< ���缳�� : ��ô=%0.f >")�� ��ȯ�Ǹ� �ݺ�
				continue;
			}else if(RTERROR_str.CompareNoCase(_T("**������ ��ü�� ���ڰ� �ƴմϴ�.**")) == 0){
				continue;
			}else{
				if(RTERROR_str.GetLength() > 0){//���콺 ��Ŭ�� �Ǵ� ���ͽ� ���ڿ����� 0�̹Ƿ� ����EL������ ���
					m_ELValue_Str.Format(_T("%s"),RTERROR_str);
					iniFile.writeINI(_T("config.ini"), _T("FW"), _T("ELValue"),m_ELValue_Str);//EL �� ����
					break;
				}else{
					m_ELValue_Str.Format(_T("%f"),m_ELValue);
					break;
				}
					
			}
		}else if(stat == RTNORM){//�ؽ�Ʈ ���ý� ���ڸ� ����
			 AcGePoint3d insertPoint;
			 AcDbObjectId objId;
			 acdbGetObjectId(objId, ens);
			 AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			 if(pEnt.openStatus() == Acad::eOk){
			 	if(pEnt->isKindOf(AcDbText::desc())){
			 		AcDbText* pText = AcDbText::cast(pEnt);
			 		inString.Format(_T("%s"), pText->textString());
			 		inString = CheckInputNumber(inString);//�ؽ�Ʈ���� ����,�� ����
			 		pText->close();
				}else{
					acutPrintf(_T("\n**������ ��ü�� ���ڰ� �ƴմϴ�.**\n"));
					continue;
				}
			 }
			pEnt->close();
			acedSSFree(ens);	
			m_ELValue_Str = inString;
			iniFile.writeINI(_T("config.ini"), _T("FW"), _T("ELValue"),m_ELValue_Str);//EL �� ����
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
	//���̾� =  m_LineLayerStr,�ؽ�Ʈ ���̾� = m_TxtLayerStr, �Ӹ��� = m_PreFaceStr
	double L_Length = _tstof(m_LeaderLengthStr);//���ü�
	double L_TxtHeight = _tstof(m_TxtHeightStr);//���� ����
	double L_TxtWidth = _tstof(m_TxtWidthStr);//���� ��
	double L_DotNum = _tstof(m_DecimalStr);// �Ҽ���
	getptStr.Format(_T("\n������(%.3f)���� : "), m_ELValue);
	if(acedGetPoint(NULL, getptStr, asDblArray(STspt)) != RTNORM)return;
	tmp_STspt = STspt;
	acdbUcs2Wcs(asDblArray(STspt),asDblArray(STspt),Adesk::kFalse); // �輺�� �븮 �߰�
	
	while(true){
		if(acedGetPoint(asDblArray(tmp_STspt),_T("\nEL ǥ���� ���� : "), asDblArray(STept)) != RTNORM)return;
		acdbUcs2Wcs(asDblArray(STept),asDblArray(STept),Adesk::kFalse); // �輺�� �븮 �߰�

		STang = ads_angle(asDblArray(STspt), asDblArray(STept));//�������� �¿����
		if((STang >=DTR(0.0) &&STang <=DTR(90.0)) || (STang >DTR(270.0) && STang <DTR(360)))
			Mode = 1;//������ ����
		else
			Mode = 2;//������ ����
		
		if(FormStat == 1){/////////////////����-1/////////////////////
			AcGePoint3d H_Txtpt;
			
			//�ؽ�Ʈ ����
			//NewELValue = m_ELValue + (STept.y - STspt.y);
			NewELValue = m_ELValue + ((STept.y - STspt.y) * m_Scale / 1000) ; // �輺�� �븮 ����
			CString Txtstr1, Txt1;//���� �������� �Ҽ��� �����ϱ�
			int H_DecimalNum = L_DotNum;
			Txt1.Format(_T("%s%%.%df"),m_PreFaceStr, H_DecimalNum);
			Txtstr1.Format(Txt1, NewELValue);//�ؽ�Ʈ���� ��ô�� ���� �����ϰ� ��ü�� ������

			if(L_DotNum == 0)//��ô1:1 -> ��ȣ,����,���� = 0.2331, �� = 0.1164, �������κ� = 0.2  �������� ���α���
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
			CreateLayer(m_TxtLayerStr, 3);//�ؽ�Ʈ ���̾� ����
			bool m_Text = CreateText(H_Txtpt, 1000/m_Scale*L_TxtHeight,L_TxtWidth, 0.0, Txtstr1, m_TxtLayerStr, 0);//�ؽ�Ʈ ����
				if(m_Text == false)break;

			//��������
			ads_polar(asDblArray(Formpt), 0.0, 1000/m_Scale * UnderLength, asDblArray(Formpt_R));//���� ����Ʈ
			ads_polar(asDblArray(Formpt), DTR(180.0), 1000/m_Scale * UnderLength, asDblArray(Formpt_L));//���� ����Ʈ
			if(Mode == 1){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_R);
			}else if(Mode == 2){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_L);
			}
			CreateLayer(m_LineLayerStr, 1);//���� ���̾� ����
			bool b_PLine = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//������ ����
			ptArray.removeAll();//�迭�ʱ�ȭ
			if(b_PLine == false)break;

		}else if(FormStat == 2){//////////����-2////////////////////
			AcGePoint3d H_Txtpt;
			
			//�ؽ�Ʈ ����
			//NewELValue = m_ELValue + (STept.y - STspt.y);
			NewELValue = m_ELValue + ((STept.y - STspt.y) * m_Scale / 1000) ; // �輺�� �븮 ����
			CString Txtstr1, Txt1;//���� �������� �Ҽ��� �����ϱ�
			int H_DecimalNum = L_DotNum;
			Txt1.Format(_T("%s%%.%df"),m_PreFaceStr, H_DecimalNum);
			Txtstr1.Format(Txt1, NewELValue);

			if(L_DotNum == 0)//��ô1000 -> ��ȣ,����,���� = 0.2331, �� = 0.1164, �������κ� = 0.2  �������� ���α���
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
			CreateLayer(m_TxtLayerStr, 3);//�ؽ�Ʈ ���̾� ����
			bool m_Text = CreateText(H_Txtpt, 1000/m_Scale*L_TxtHeight,L_TxtWidth, 0.0, Txtstr1, m_TxtLayerStr, 0);//�ؽ�Ʈ ����
				if(m_Text == false)break;

			//��������
			ads_polar(asDblArray(Formpt), 0.0, 1000/m_Scale * UnderLength, asDblArray(Formpt_R));//���� ����Ʈ
			ads_polar(asDblArray(Formpt), DTR(180.0), 1000/m_Scale * UnderLength, asDblArray(Formpt_L));//���� ����Ʈ
			if(Mode == 1){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_R);
			}else if(Mode == 2){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_L);
			}
			CreateLayer(m_LineLayerStr, 1);//���� ���̾� ����
			bool b_PLine = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//������ ����
			ptArray.removeAll();//�迭�ʱ�ȭ
			if(b_PLine == false)break;

		}else if(FormStat == 3){//////////����-3////////////////////
			AcGePoint3d H_Txtpt,Formpt2,W_Formpt,W_Formpt2,H_Formpt,H_Formpt2;
			
			//�ؽ�Ʈ ����
			//NewELValue = m_ELValue + (STept.y - STspt.y);
			NewELValue = m_ELValue + ((STept.y - STspt.y) * m_Scale / 1000) ; // �輺�� �븮 ����
			CString Txtstr1, Txt1;//���� �������� �Ҽ��� �����ϱ�
			int H_DecimalNum = L_DotNum;
			Txt1.Format(_T("%s%%.%df"),m_PreFaceStr, H_DecimalNum);
			Txtstr1.Format(Txt1, NewELValue);

			if(L_DotNum == 0)//��ô1:1 -> ��ȣ,����,���� = 0.2331, �� = 0.1164, �������κ� = 0.2  �������� ���α���
				UnderLength = (Txtstr1.GetLength()*0.2331 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);
			else if(L_DotNum > 0)
				UnderLength = ((Txtstr1.GetLength()-1)*0.2331 + 0.1164 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);

			ads_polar(asDblArray(STept), 0.0, 1000/m_Scale * L_Length / 2, asDblArray(W_Formpt));
			ads_polar(asDblArray(W_Formpt), DTR(90.0), 1000/m_Scale * L_Length, asDblArray(H_Formpt));
			ads_polar(asDblArray(STept), DTR(180.0), 1000/m_Scale * L_Length/2, asDblArray(W_Formpt2));
			ads_polar(asDblArray(W_Formpt2), DTR(90.0), 1000/m_Scale * L_Length, asDblArray(H_Formpt2));

			if(Mode == 1){
				H_Txtpt.y = H_Formpt.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;//0.15 -> ���ü��� �ؽ�Ʈ ����
				H_Txtpt.x = H_Formpt.x + (0.05 * L_TxtWidth)  * (L_TxtHeight / 0.3)* 1000/m_Scale;
			}else if(Mode == 2){
				H_Txtpt.y = H_Formpt2.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;//0.15 -> ���ü��� �ؽ�Ʈ ����
				H_Txtpt.x = H_Formpt2.x - (UnderLength - (0.05 * L_TxtWidth) * (L_TxtHeight / 0.3)) * 1000/m_Scale;
			}
			CreateLayer(m_TxtLayerStr, 3);//�ؽ�Ʈ ���̾� ����
			bool m_Text = CreateText(H_Txtpt, 1000/m_Scale*L_TxtHeight,L_TxtWidth, 0.0, Txtstr1, m_TxtLayerStr, 0);//�ؽ�Ʈ ����
				if(m_Text == false)break;

			//��������
			ads_polar(asDblArray(H_Formpt), 0.0, 1000/m_Scale * UnderLength, asDblArray(Formpt_R));//���� ����Ʈ
			ads_polar(asDblArray(H_Formpt2), DTR(180.0), 1000/m_Scale * UnderLength, asDblArray(Formpt_L));//���� ����Ʈ
			if(Mode == 1){
				ptArray.append(STept); ptArray.append(H_Formpt); ptArray.append(Formpt_R);
			}else if(Mode == 2){
				ptArray.append(STept); ptArray.append(H_Formpt2); ptArray.append(Formpt_L);
			}
			CreateLayer(m_LineLayerStr, 1);//���� ���̾� ����
			bool b_PLine = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//������ ����
			ptArray.removeAll();//�迭�ʱ�ȭ
			if(b_PLine == false)break;

		}else if(FormStat == 4){//////////����-4////////////////////
			AcGePoint3d H_Txtpt,Formpt2,W_Formpt,W_Formpt2,H_Formpt,H_Formpt2;
			
			//�ؽ�Ʈ ����
			//NewELValue = m_ELValue + (STept.y - STspt.y);
			NewELValue = m_ELValue + ((STept.y - STspt.y) * m_Scale / 1000) ; // �輺�� �븮 ����
			CString Txtstr1, Txt1;//���� �������� �Ҽ��� �����ϱ�
			int H_DecimalNum = L_DotNum;
			Txt1.Format(_T("%s%%.%df"),m_PreFaceStr, H_DecimalNum);
			Txtstr1.Format(Txt1, NewELValue);

			if(L_DotNum == 0)//��ô1:1 -> ��ȣ,����,���� = 0.2331, �� = 0.1164, �������κ� = 0.2  �������� ���α���
				UnderLength = (Txtstr1.GetLength()*0.2331 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);
			else if(L_DotNum > 0)
				UnderLength = ((Txtstr1.GetLength()-1)*0.2331 + 0.1164 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);

			ads_polar(asDblArray(STept), 0.0, 1000/m_Scale * L_Length / 2, asDblArray(W_Formpt));
			ads_polar(asDblArray(W_Formpt), DTR(270.0), 1000/m_Scale * L_Length, asDblArray(H_Formpt));
			ads_polar(asDblArray(STept), DTR(180.0), 1000/m_Scale * L_Length/2, asDblArray(W_Formpt2));
			ads_polar(asDblArray(W_Formpt2), DTR(270.0), 1000/m_Scale * L_Length, asDblArray(H_Formpt2));

			if(Mode == 1){
				H_Txtpt.y = H_Formpt.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;//0.15 -> ���ü��� �ؽ�Ʈ ����
				H_Txtpt.x = H_Formpt.x + (0.05 * L_TxtWidth) * (L_TxtHeight / 0.3) * 1000/m_Scale;
			}else if(Mode == 2){
				H_Txtpt.y = H_Formpt2.y + (0.15 * (L_TxtHeight / 0.3)) * 1000/m_Scale;//0.15 -> ���ü��� �ؽ�Ʈ ����
				H_Txtpt.x = H_Formpt2.x - ((UnderLength - (0.05 * L_TxtWidth) * (L_TxtHeight / 0.3)) * 1000/m_Scale);
			}
			CreateLayer(m_TxtLayerStr, 3);//�ؽ�Ʈ ���̾� ����
			bool m_Text = CreateText(H_Txtpt, 1000/m_Scale*L_TxtHeight,L_TxtWidth, 0.0, Txtstr1, m_TxtLayerStr, 0);//�ؽ�Ʈ ����
				if(m_Text == false)break;

			//��������
			ads_polar(asDblArray(H_Formpt), 0.0, 1000/m_Scale * UnderLength, asDblArray(Formpt_R));//���� ����Ʈ
			ads_polar(asDblArray(H_Formpt2), DTR(180.0), 1000/m_Scale * UnderLength, asDblArray(Formpt_L));//���� ����Ʈ

			if(Mode == 1){
				ptArray.append(STept); ptArray.append(H_Formpt); ptArray.append(Formpt_R);
			}else if(Mode == 2){
				ptArray.append(STept); ptArray.append(H_Formpt2); ptArray.append(Formpt_L);
			}
			CreateLayer(m_LineLayerStr, 1);//���� ���̾� ����
			bool b_PLine = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//������ ����
			ptArray.removeAll();//�迭�ʱ�ȭ
			if(b_PLine == false)break;

		}else if(FormStat == 5){//////////����-5////////////////////
			AcGePoint3d H_Txtpt;

			//�ؽ�Ʈ ����
			//NewELValue = m_ELValue + (STept.y - STspt.y);
			NewELValue = m_ELValue + ((STept.y - STspt.y) * m_Scale / 1000) ; // �輺�� �븮 ����
			CString Txtstr1, Txt1;//���� �������� �Ҽ��� �����ϱ�
			int H_DecimalNum = L_DotNum;
			Txt1.Format(_T("%s%%.%df"),m_PreFaceStr, H_DecimalNum);
			Txtstr1.Format(Txt1, NewELValue);

			if(L_DotNum == 0)//��ô1:1 -> ��ȣ,����,���� = 0.2331, �� = 0.1164, �������κ� = 0.2  �������� ���α���
				UnderLength = (Txtstr1.GetLength()*0.2331 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);
			else if(L_DotNum > 0)
				UnderLength = ((Txtstr1.GetLength()-1)*0.2331 + 0.1164 + 0.2) * L_TxtWidth * (L_TxtHeight / 0.3);//-1�� �Ҽ����� �� ��

			ads_polar(asDblArray(STept), DTR(90.0), 1000/m_Scale * L_Length, asDblArray(Formpt));

			if(Mode == 1){
				H_Txtpt.y = Formpt.y + 0.15 * (L_TxtHeight / 0.3) * 1000/m_Scale;
				H_Txtpt.x = Formpt.x + (0.05 * L_TxtWidth) * (L_TxtHeight / 0.3) * 1000/m_Scale;
			}else if(Mode == 2){
				H_Txtpt.y = Formpt.y + 0.15 * (L_TxtHeight / 0.3) * 1000/m_Scale;
				H_Txtpt.x = Formpt.x - (UnderLength - (0.05 * L_TxtWidth) * (L_TxtHeight / 0.3)) * 1000/m_Scale ;
			}

			CreateLayer(m_TxtLayerStr, 3);//�ؽ�Ʈ ���̾� ����
			bool m_Text = CreateText(H_Txtpt, 1000/m_Scale*L_TxtHeight,L_TxtWidth, 0.0, Txtstr1, m_TxtLayerStr, 0);//�ؽ�Ʈ ����
				if(m_Text == false)break;

			//��������
			ads_polar(asDblArray(Formpt), 0.0, 1000/m_Scale * UnderLength, asDblArray(Formpt_R));//���� ����Ʈ
			ads_polar(asDblArray(Formpt), DTR(180.0), 1000/m_Scale * UnderLength, asDblArray(Formpt_L));//���� ����Ʈ
			if(Mode == 1){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_R);
			}else if(Mode == 2){
				ptArray.append(STept); ptArray.append(Formpt); ptArray.append(Formpt_L);
			}
			CreateLayer(m_LineLayerStr, 1);//���� ���̾� ����
			bool b_PLine = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//������ ����
			ptArray.removeAll();//�迭�ʱ�ȭ
			if(b_PLine == false)break;
			//ȭ��ǥ ��������
			ads_polar(asDblArray(STept), DTR(90.0), 1000/m_Scale * 0.3, asDblArray(Formpt));
			ads_polar(asDblArray(Formpt), 0.0, 1000/m_Scale * 0.15, asDblArray(Formpt_R));
			ptArray.append(Formpt); ptArray.append(Formpt_R); ptArray.append(STept); 
			bool b_Arrow = CreatePLine(ptArray, 0, _T("BYLAYER"), 1, m_LineLayerStr);//������ ����
			ptArray.removeAll();//�迭 �ʱ�ȭ
			if(b_Arrow == false)break;

			//ȭ��ǥ �ָ���
			ads_polar(asDblArray(STept), DTR(90.0),1000/m_Scale * 0.3, asDblArray(Formpt));
			ads_polar(asDblArray(Formpt), DTR(180.0),1000/m_Scale * 0.15, asDblArray(Formpt_L));
			bool b_Solid = CreateSolid(STept, Formpt, Formpt_L, m_LineLayerStr);//ȭ��ǥ �ָ��� ����
			if(b_Solid == false)break;

		}
		

	}//while end
	CompleteEditorCommand();

}

void CFW::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

//������ư 1~5
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

//�� ���̾�
void CFW::OnEnUpdateFwlinelayer()
{
	UpdateData(TRUE);
	m_LineLayerStr;
	UpdateData(FALSE);
}
//���� ���̾�
void CFW::OnEnUpdateFwtxtlayer()
{
	UpdateData(TRUE);
	m_TxtLayerStr;
	UpdateData(FALSE);
}
//���� ����
void CFW::OnEnUpdateFwtxtheight()
{
	UpdateData(TRUE);
	m_TxtHeightStr = CheckInputNumber(m_TxtHeightStr);
	UpdateData(FALSE);

	m_TextHeight_Ctrl.SetSel(0, -1);
	m_TextHeight_Ctrl.SetSel(-1, -1);
	UpdateData(FALSE);
}
//���� ��
void CFW::OnEnUpdateFwtxtwidth()
{
	UpdateData(TRUE);
	Width = _ttoi(m_TxtWidthStr);
	UpdateData(FALSE);
}
//���ü� ����
void CFW::OnEnUpdateFwldlength()
{
	UpdateData(TRUE);
	Length = _ttoi(m_LeaderLengthStr);
	UpdateData(FALSE);
}
//�Ҽ���
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
//�Ӹ���
void CFW::OnEnUpdateFwpreface()
{
	UpdateData(TRUE);
	m_PreFaceStr;
	UpdateData(FALSE);
}
//���� �� ��(.) �Է� üũ
CString CFW::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	CString strNum, strRe = _T("");
	int k = 0;
	for(int i = 0; i < str.GetLength(); i++){
		strNum = str.Mid(i,1);	//for���� ���� ��������ڶ����� �������ڸ� ã������ Mid���� 0��°�� ã�ƾ� ��
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		if(((ascii >= 48 && ascii <= 57) || ascii == 46 || ascii == 46)){
			strRe.Append(getAtChar);
		}
	}
	return strRe;
} 

////������ ���� ������ ��ȯ�Ͽ� �����Ѵ�.
double CFW::DTR(double angle)
{//ads_printf(_T("\n DTR() angle=%f \n"),angle);
	return (angle / 180.0 * PI);
}

////���� ���� ������ ��ȯ�Ͽ� �����Ѵ�.
double CFW::RTD(double radian)
{//ads_printf(_T("\n RTD() radian=%f \n"),radian);
	return (radian * 180.0 / PI);
}

//�ؽ�Ʈ ����
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

//������ ����
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
//���̾� ����
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

//�ָ��� ����
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