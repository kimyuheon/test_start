// CUT.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CUT.h"
#include "afxdialogex.h"

#ifdef _ACAD
	CString Linetype = L"acadiso.lin";
#elif _ZCAD
	CString Linetype = L"ZWCADiso.lin";
#elif _ZXYCAD
	CString Linetype = L"ZYXCADiso.lin";
#endif
// CCUT ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCUT, CAcUiDialog)

	CCUT::CCUT(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CCUT::IDD, pParent),m_CutExceptStr(_T("")), m_CutHeightStr(_T("")), m_CutLscaleStr(_T("")), m_CutMinHeightStr(_T("")), m_CutScaleStr(_T("")),m_CutSlopeStr(_T(""))
{

}

CCUT::~CCUT()
{
}

void CCUT::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);

	//Static
	DDX_Text(pDX, IDC_CUT_HEIGHT,m_CutHeightStr);
	DDX_Text(pDX, IDC_CUT_SLOPE,m_CutSlopeStr);
	DDX_Text(pDX, IDC_CUT_SCALE,m_CutScaleStr);
	DDX_Text(pDX, IDC_CUT_MINHEIGHT,m_CutMinHeightStr);
	DDX_Text(pDX, IDC_CUT_EXCEPT,m_CutExceptStr);
	DDX_Text(pDX, IDC_CUT_LSCALE,m_CutLscaleStr);
	//üũ�ڽ�
	DDX_Control(pDX, IDC_CUT_CHECK1,m_Chk1_btn);
	DDX_Control(pDX, IDC_CUT_CHECK2,m_Chk2_btn);
	//��ư
	DDX_Control(pDX, IDC_CUT_BSELECT1,m_bSelect1);
	DDX_Control(pDX, IDC_CUT_BSELECT2,m_bSelect2);
	DDX_Control(pDX, IDC_CUT_COLORBTN,m_Colorbtn);
	DDX_Control(pDX, IDC_CUT_PRINCOLOR, m_btn_Color);
	//�޺��ڽ�
	DDX_Control(pDX, IDC_CUT_CBXLAYER,m_CbxLayer);

}


BEGIN_MESSAGE_MAP(CCUT, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_CUT_WRITE, &CCUT::OnBnClickedCutWrite)
	ON_BN_CLICKED(IDCANCEL, &CCUT::OnBnClickedCancel)
	ON_EN_UPDATE(IDC_CUT_HEIGHT, &CCUT::OnEnUpdateCutHeight)
	ON_EN_UPDATE(IDC_CUT_SLOPE, &CCUT::OnEnUpdateCutSlope)
	ON_EN_UPDATE(IDC_CUT_SCALE, &CCUT::OnEnUpdateCutScale)
	ON_EN_UPDATE(IDC_CUT_MINHEIGHT, &CCUT::OnEnUpdateCutMinheight)
	ON_EN_UPDATE(IDC_CUT_EXCEPT, &CCUT::OnEnUpdateCutExcept)
	ON_EN_UPDATE(IDC_CUT_LSCALE, &CCUT::OnEnUpdateCutLscale)
	ON_BN_CLICKED(IDC_CUT_CHECK1, &CCUT::OnBnClickedCutCheck1)
	ON_BN_CLICKED(IDC_CUT_CHECK2, &CCUT::OnBnClickedCutCheck2)
	ON_BN_CLICKED(IDC_CUT_BSELECT1, &CCUT::OnBnClickedCutBselect1)
	ON_CBN_SELCHANGE(IDC_CUT_CBXLAYER, &CCUT::OnCbnSelchangeCutCbxlayer)
	ON_BN_CLICKED(IDC_CUT_COLORBTN, &CCUT::OnBnClickedCutColorbtn)
	ON_BN_CLICKED(IDC_CUT_BSELECT2, &CCUT::OnBnClickedCutBselect2)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_CUT_HELP, &CCUT::OnBnClickedCutHelp)
END_MESSAGE_MAP()


// CCUT �޽��� ó�����Դϴ�.

LRESULT CCUT::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

BOOL CCUT::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	//hidden ���� �߰�
	AcDbObjectId lineTypeId;
	acdbLoadLineTypeFile(_T("HIDDEN"), Linetype, acdbHostApplicationServices()->workingDatabase());

	AcDbLinetypeTable *pLinetypeTable; 
	ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLinetypeTable, AcDb::kForRead); 
	if(es == Acad::eOk)
		pLinetypeTable->getAt(_T("HIDDEN"), lineTypeId, Adesk::kTrue); 	

	pLinetypeTable->close(); 


	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	TCHAR szLtFile[MAX_PATH], *str;


	CString strLINFile = Linetype, strMsg;

	if(RTNORM != acedFindFile(strLINFile, szLtFile)){
		strMsg.LoadStringW(IDS_EXIST_FILE); // "%s ������ �����ϴ�."
		acutPrintf(strMsg, Linetype);
	}

	AcDbLinetypeTable *pLtTable = NULL;
	es = pDb->getLinetypeTable(pLtTable,AcDb::kForRead);
	ACHAR *szLtype = _T("HIDDEN");
	bool isLinetypeLoaded = pLtTable->has(szLtype);

	es = pLtTable->close();

	if(isLinetypeLoaded){
		AcDbDatabase *pTempDatabase = new  AcDbDatabase(true,false);
		es = pTempDatabase->loadLineTypeFile(szLtype, szLtFile);
		if(Acad::eOk == es){
			AcDbLinetypeTable *pTempLtTable;
			AcDbLinetypeTableRecord *pTempLtRec=NULL;

			es = pTempDatabase->getLinetypeTable(pTempLtTable,AcDb::kForRead);
			AcDbObjectId ltRecId = AcDbObjectId::kNull;
			es = pTempLtTable->getAt(szLtype, ltRecId);			
			pTempLtTable->close();
			if(Acad::eOk == es){
				acutPrintf(_T("\nLintype reloaded!"));
			}else{
				acutPrintf(_T("\nCouldn't reload Lintype"));
			}
		}else{
			acutPrintf(_T("\nError loading linetype from file!"));
		}
		delete pTempDatabase;
	}else{
		if(Acad::eOk == pDb->loadLineTypeFile(szLtype,szLtFile)){
			acutPrintf(_T("\nLinetype loaded from file!"));
		}else{
			acutPrintf(_T("\nError loading linetype from file!"));
		}		
	}

	//�⺻ ���̾� ����
	bool b_Layer= CreateLayer(_T("CC-BNCH"),3);

	//������
	HICON hIcon = LoadIcon(_hdllInstance,MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon,FALSE);

	m_bSelect1.AutoLoad();m_bSelect2.AutoLoad();

	//���̾� �ҷ�����
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);
	
	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		pLayerTblRcd->getName(name);
		m_CbxLayer.AddString(name);
		pLayerTblRcd->close();
	}
	
	delete pLayerTblIter;
	pLayerPoint->close();
	//pLayerTbl->close();	
	

	//������ ����
	CString tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Height"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_CUT_HEIGHT,_T("1.000"));
	else
		SetDlgItemText(IDC_CUT_HEIGHT,tempStr);

	//����� ���
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Slope"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_CUT_SLOPE,_T("4"));
	else
		SetDlgItemText(IDC_CUT_SLOPE,tempStr);

	//���� ��ô
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Scale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_CUT_SCALE,_T("1000"));
	else
		SetDlgItemText(IDC_CUT_SCALE,tempStr);

	//�ּ� ����
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("MinHeight"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_CUT_MINHEIGHT,_T("0.50"));
	else
		SetDlgItemText(IDC_CUT_MINHEIGHT,tempStr);

	//������ ���� ��� 1:S
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Except"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_CUT_EXCEPT,_T("4"));
	else
		SetDlgItemText(IDC_CUT_EXCEPT,tempStr);

	//hidden  �� ��� üũ�ڽ�
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Check1"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){
		m_Chk1_btn.SetCheck(TRUE);
		bool_Chk1 = true;
		OnBnClickedCutCheck1();		
	}		
	else{
		m_Chk1_btn.SetCheck(FALSE);
		bool_Chk1 = false;
		OnBnClickedCutCheck1();
	}	

	//hidden  �� ��ô
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("LScale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_CUT_LSCALE,_T("4"));
	else
		SetDlgItemText(IDC_CUT_LSCALE,tempStr);

	//���̾� ���ڿ��� ����
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("CbxLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_CbxLayerStr = _T("0");
	else
		m_CbxLayerStr = tempStr;

	//�޺��ڽ� ���̾�
	tempStr = iniFile.readINI(_T("config.ini"), _T("CUT"), _T("CbxLayerInt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_CbxLayer.SetCurSel(0);
		m_CbxLayer.GetLBText(0, m_CbxLayerStr);
	}else{
		m_CbxLayerIntStr = tempStr;
		m_CbxLayerInt = _ttoi(m_CbxLayerIntStr);
		m_CbxLayer.SetCurSel(m_CbxLayerInt);
		if(m_CbxLayer.FindString(-1, m_CbxLayerStr) == -1){
			m_CbxLayer.SetCurSel(0);
			m_CbxLayer.GetLBText(0, m_CbxLayerStr);
		}else{
			m_CbxLayer.SetCurSel(m_CbxLayerInt);
			m_CbxLayer.GetLBText(m_CbxLayerInt,m_CbxLayerStr);
		}
	}	

	//ǥ�� ���̾� ��� üũ�ڽ�
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Check2"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){
		m_Chk2_btn.SetCheck(TRUE);
		bool_Chk2 = true;
		m_CbxLayer.SetCurSel(m_CbxLayerInt);
		OnBnClickedCutCheck2();
	}		
	else{		
		m_Chk2_btn.SetCheck(FALSE);
		bool_Chk2 = false;
		OnBnClickedCutCheck2();
	}	



	//���� ��ư	
	tempStr = iniFile.readINI(_T("config.ini"), _T("CUT"), _T("Color"));
	if(tempStr.CompareNoCase(_T("")) == 0){
		m_Color = 255;
		//ColorSetting(m_Color);
	}else{
		m_Color = _ttoi(tempStr);
		ColorSetting(m_Color);
	}	


	return TRUE;  
}

void CCUT::CUTMain()
{
	int old_Osmode;
	double m_Slope, m_Width;
	AcGePoint3d sp,ep,Entpt, pt1,pt2,pt3;
	AcGePoint3dArray ptarr;
	ads_name en;
	CString strMsg;

	getSysVar(_T("OSMODE"),old_Osmode);//���� ������
	setSysVar(_T("OSMODE"),33);//������ ���� ������ ����
	ptarr.removeAll();

	//������ ����
	CString tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Height"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_CutHeight = 1.000;
	else
		m_CutHeight = _tstof(tempStr);

	//����� ���
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Slope"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_CutSlope = 4;
	else
		m_CutSlope = _ttoi(tempStr);

	//���� ��ô
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Scale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_CutScale = 1000;
	else
		m_CutScale = _ttoi(tempStr);

	//�ּ� ����
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("MinHeight"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_CutMinHeight = 0.50;
	else
		m_CutMinHeight = _tstof(tempStr);

	//������ ���� ��� 1:S
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Except"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_CutExcept = 4;
	else
		m_CutExcept = _ttoi(tempStr);

	//hidden  �� ��� üũ�ڽ�
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Check1"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){		
		bool_Chk1 = true;		
	}		
	else{		
		bool_Chk1 = false;		
	}	

	//hidden  �� ��ô
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("LScale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_CutLscale = 4;
	else
		m_CutLscale = _tstof(tempStr);

	//ǥ�� ���̾� ��� üũ�ڽ�
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("Check2"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0){		
		bool_Chk2 = true;
	}		
	else{	
		bool_Chk2 = false;
	}	

	//���̾� ���ڿ��� ����
	tempStr = iniFile.readINI(_T("config.ini"),_T("CUT"),_T("CbxLayer"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_CbxLayerStr = _T("0");
	else
		m_CbxLayerStr = tempStr;

	//�޺��ڽ� ���̾�
	tempStr = iniFile.readINI(_T("config.ini"), _T("CUT"), _T("CbxLayerInt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0){
		m_CbxLayerInt = 0;
	}else{
		m_CbxLayerInt = _ttoi(tempStr);
	}	

	//���� ��ư	
	tempStr = iniFile.readINI(_T("config.ini"), _T("CUT"), _T("Color"));
	if(tempStr.CompareNoCase(_T("")) == 0){
		m_Color = 255;
		//ColorSetting(m_Color);
	}else{
		m_Color = _ttoi(tempStr);
		//ColorSetting(m_Color);
	}	

	::SetFocus(adsw_acadDocWnd());
	strMsg.LoadStringW(IDS_CUT_FIRST_SEL); // "\n������ ���� �� ���� : "
	if(acedGetPoint(0, strMsg,asDblArray(sp))!=RTNORM){
		setSysVar(_T("OSMODE"),old_Osmode);
		return;
	}
	strMsg.LoadStringW(IDS_CUT_SECOND_SEL); // "\n������ �� �� ���� : "
	if(acedGetPoint(asDblArray(sp), strMsg,asDblArray(ep))!=RTNORM){
		setSysVar(_T("OSMODE"),old_Osmode);
		return;
	}

	while(true){
		strMsg.LoadStringW(IDS_GROUND_SELECT); // "\n���ݼ� ���� : "
		int stat = acedEntSel(strMsg,en,asDblArray(Entpt));

		if(stat == RTCAN  || stat == RTNONE){
			setSysVar(_T("OSMODE"),old_Osmode);
			return;
		}else if(stat == RTERROR){
			resbuf zValueStr;
			ads_getvar(_T("ERRNO"), &zValueStr);
			if(zValueStr.resval.rint == 52)
				break;
			else
				continue;

		}else if(stat == RTNORM){
			AcDbObjectId objId;
			AcGePoint3d AccordPt, npt, SlpPt, Stdpt, ClosePt,VirtualPt,Cutpt;

			acdbGetObjectId(objId,en);
			AcDbEntityPointer pEnt(objId,AcDb::kForRead);


			if(sp.y > ep.y){
				npt = ep;
				ep = sp;
				sp = npt;
			}

			AcGePoint3d Polyspt,Polyept,Ppt, pt1,pt2,pt3, IntrSpt,IntrEpt ,InterPt, SlopePt, Fixpt1,Fixpt2,Fixpt3, PolarPt,RemainPt3,RemainSlpPt3;
			AcGePoint3dArray polyArr,remainsArr;
			double m_SubSlope, m_MainSlope, m_MainWidth,m_SubWidth,m_MainHeight,m_SubHeight, m_Cutcnt,m_Remains, m_MainSlpDist, m_SubSlpDist;
			double m_RemainSlpDist, m_RemainWidth, m_RemainHeight;

			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbLine::desc())){								
				}else if(pEnt->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline *pPline = AcDbPolyline::cast(pEnt);
					pPline->upgradeOpen();//������
					pPline->getStartPoint(Polyspt);
					pPline->getEndPoint(Polyept);

					//pPline->getClosestPointTo(sp,sp);
					//pPline->getClosestPointTo(ep,ep);
					double Ptdist, EndDist, PtParam;
					pPline->getDistAtPoint(ep, EndDist);
					int cnt = 0;
					while(true){
						pPline->getPointAt(cnt,Ppt);
						pPline->getDistAtPoint(Ppt, Ptdist);
						//pPline->getParamAtDist(Ptdist, PtParam);
						if (EndDist <= Ptdist)
						{
							polyArr.append(ep);
							break;
						}
						polyArr.append(Ppt);
						/*if(Ppt == Polyept)
							break;*/
						cnt++;
					}

					pPline->close();

					if(polyArr.first().y > polyArr.last().y)//������ ������ y���� ���� y�ຸ��  Ŭ�� ��ġ����
						polyArr.reverse();

					polyArr.removeFirst();//��������  sp�� ��ü
					polyArr.insertAt(0, sp);//�������� �������� ����������

					int findcnt =  polyArr.find(ep);

					AcGePoint3d upPt1,upPt2;
					//////////////////////////////////////////////////////////////////////////////////////////////////////					
					if(polyArr.length() == 2){//������ �� 2���� ��
						Fixpt1 = polyArr[0];
						//Fixpt2 = polyArr[1];
						Fixpt2 = ep;
						Fixpt3.x = Fixpt2.x; Fixpt3.y = Fixpt1.y;
						pt1 = Fixpt1;
						pt2 = Fixpt2;
						pt3 = Fixpt3;

						m_MainWidth = pt1.distanceTo(pt3);//���� ����
						m_MainHeight = pt2.distanceTo(pt3);//���� ����
						m_MainSlpDist = pt1.distanceTo(pt2);//���� ����
						if(m_CutExcept != 0){
							if(m_MainWidth / m_MainHeight >= m_CutExcept ){//���� ����� �� ����X
								setSysVar(_T("OSMODE"), old_Osmode);
								pEnt->close();
								return;
							}
						}
						

						m_MainSlope = m_MainHeight / m_MainWidth * 100;//���� ���
						m_SubSlope = m_CutSlope;// ����� ���
						double CutHeightScale = m_CutHeight*(1000.0/(m_CutScale*1.0));
						
						while(true){
							Polar(pt1,ads_angle(asDblArray(Fixpt3),asDblArray(Fixpt2)),CutHeightScale,upPt1);
							Polar(upPt1,ads_angle(asDblArray(Fixpt1),asDblArray(Fixpt3)),m_CutHeight*1000,upPt2);
							m_SubHeight = m_SubSlope * upPt1.distanceTo(upPt2) / 100; //����� ������
							upPt2.y = upPt2.y + m_SubHeight;							
							
							if(acdbInters(asDblArray(upPt1),asDblArray(upPt2),asDblArray(Fixpt1),asDblArray(Fixpt2),1,asDblArray(InterPt))!=RTNORM)
								break;
							
							Polar(InterPt,ads_angle(asDblArray(Fixpt2),asDblArray(Fixpt3)),CutHeightScale,pt3);
							ptarr.append(pt1);ptarr.append(pt3);ptarr.append(InterPt);
							if(bool_Chk1){//hidden �� ��� üũ��
								if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
									CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,_T("CC-BNCH"),0);
								else
									CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,m_CbxLayerStr,0);	
							}else{
								if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
									CreatePLine(ptarr,0,_T("bylayer"),1,_T("CC-BNCH"),0);
								else
									CreatePLine(ptarr,0,_T("bylayer"),1,m_CbxLayerStr,0);	
							}
							
							ptarr.removeAll();

							pt1 = InterPt;
							
						}

						//������ ���� ���ϱ�
						//pt2 = polyArr[1];
						pt2 = ep;
						pt3.x = pt2.x; pt3.y = pt1.y;
						m_MainWidth = pt1.distanceTo(pt3);//���� ����
						m_MainHeight = pt2.distanceTo(pt3);//���� ����
						m_MainSlpDist = pt1.distanceTo(pt2);//���� ����

						m_SubHeight = m_SubSlope * pt1.distanceTo(pt3) / 100; //����� ������
						pt3.y = pt3.y+m_SubHeight;//pt3�� y�� ����������̸�ŭ ����

						if(pt2.distanceTo(pt3) > m_CutMinHeight*(1000.0/(m_CutScale*1.0))){//�ּҳ��̺��� Ŭ ���
							ptarr.append(pt1);ptarr.append(pt3);ptarr.append(pt2);
							if(bool_Chk1){//hidden �� ��� üũ��
								if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
									CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,_T("CC-BNCH"),0);
								else
									CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,m_CbxLayerStr,0);	
							}else{
								if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
									CreatePLine(ptarr,0,_T("bylayer"),1,_T("CC-BNCH"),0);
								else
									CreatePLine(ptarr,0,_T("bylayer"),1,m_CbxLayerStr,0);	
							}
							ptarr.removeAll();
						}else{//�ּҳ��̺��� ���� ���
							ads_name e_name;
							AcDbObjectId objId;
							AcGePoint3d PolyStpt;
							//������ ���� ������ ����
							acdbEntLast(e_name);
							acdbGetObjectId(objId, e_name);
							AcDbEntityPointer pEnt(objId,AcDb::kForRead);

							if(pEnt.openStatus() == Acad::eOk){
								if(pEnt->isKindOf(AcDbPolyline::desc())){		
									AcDbPolyline *pPline = AcDbPolyline::cast(pEnt);
									pPline->getStartPoint(PolyStpt);//������ �� �޾ƿ��� ��ü ����
									pPline->upgradeOpen();
									pPline->erase();
									pPline->close();
								}
								pEnt->close();
							}
							//PyoSi(PolyStpt);
							pt1 = PolyStpt;
							pt2 = ep;
							//pt2 = polyArr[1];
							pt3.x = pt2.x; pt3.y = pt1.y;

							m_SubHeight = m_SubSlope * pt1.distanceTo(pt3) / 100; //����� ������
							pt3.y = pt3.y+m_SubHeight;//pt3�� y�� ����������̸�ŭ ����

							ptarr.append(pt1);ptarr.append(pt3);ptarr.append(pt2);
							if(bool_Chk1){//hidden �� ��� üũ��
								if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
									CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,_T("CC-BNCH"),0);
								else
									CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,m_CbxLayerStr,0);	
							}else{
								if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
									CreatePLine(ptarr,0,_T("bylayer"),1,_T("CC-BNCH"),0);
								else
									CreatePLine(ptarr,0,_T("bylayer"),1,m_CbxLayerStr,0);	
							}
							ptarr.removeAll();

						}

						/////////////////////////////////////////////////////////////////////////////////////////////////////
					}else if(polyArr.length() > 2){//������ �� 2�� �̻��� ��
						for(int i = 1; i<polyArr.length(); i++){
						//for (int i = 1; i < findcnt; i++) {
							Fixpt1 = polyArr[i-1];
							Fixpt2 = polyArr[i];
							Fixpt3.x = Fixpt2.x; Fixpt3.y = Fixpt1.y;
							pt1 = Fixpt1;
							pt2 = Fixpt2;
							pt3 = Fixpt3;

							if(Fixpt2 == Fixpt3)continue;
						
							if(i+1 < polyArr.length()){
							//if (i + 1 < findcnt) {
								AcGePoint3d Nextpt1,Nextpt2,Nextpt3;
								Nextpt1 = polyArr[i];
								Nextpt2 = polyArr[i+1];
								//Nextpt2 = ep;
								Nextpt3.x = Nextpt2.x; Nextpt3.y = Nextpt1.y;
								
							}
							
							m_MainWidth = pt1.distanceTo(pt3);//���� ����
							m_MainHeight = pt2.distanceTo(pt3);//���� ����
							m_MainSlpDist = pt1.distanceTo(pt2);//���� ����
							
							if(m_CutExcept != 0){
								if(m_MainWidth / m_MainHeight >= m_CutExcept ){//���� ����� �� ����X								
									continue;
								}
							}
							
							m_MainSlope = m_MainHeight / m_MainWidth * 100;//���� ���
							m_SubSlope = m_CutSlope;// ����� ���
							
							while(true){
								Polar(pt1,ads_angle(asDblArray(Fixpt3),asDblArray(Fixpt2)),m_CutHeight*(1000.0/(m_CutScale*1.0)),upPt1);
								Polar(upPt1,ads_angle(asDblArray(Fixpt1),asDblArray(Fixpt3)),m_CutHeight*1000,upPt2);
								m_SubHeight = m_SubSlope * upPt1.distanceTo(upPt2) / 100; //����� ������
								upPt2.y = upPt2.y + m_SubHeight;
								//������ ���� �� �������� ������ ã��.
								if(acdbInters(asDblArray(upPt1),asDblArray(upPt2),asDblArray(Fixpt1),asDblArray(Fixpt2),1,asDblArray(InterPt))!=RTNORM){

									Fixpt1 = polyArr[i];
									Fixpt2 = polyArr[i+1];
									if(Fixpt2.x == 0 && Fixpt2.y == 0)break;
									Fixpt3.x = Fixpt2.x; Fixpt3.y = Fixpt1.y;

									m_MainWidth = Fixpt1.distanceTo(Fixpt3);//���� ����
									m_MainHeight = Fixpt2.distanceTo(Fixpt3);//���� ����
									m_MainSlpDist = Fixpt1.distanceTo(Fixpt2);//���� ����

									if(m_CutExcept != 0 && m_MainWidth / m_MainHeight >= m_CutExcept){//���� ����� �� ����X								
										//������ ���� ���ϱ�
										pt2 = polyArr[i];
										pt3.x = pt2.x; pt3.y = pt1.y;
										m_MainWidth = pt1.distanceTo(pt3);//���� ����
										m_MainHeight = pt2.distanceTo(pt3);//���� ����
										m_MainSlpDist = pt1.distanceTo(pt2);//���� ����
										//PyoSi(pt1);PyoSi(pt2);

										m_SubHeight = m_SubSlope * pt1.distanceTo(pt3) / 100; //����� ������
										pt3.y = pt3.y+m_SubHeight;//pt3�� y�� ����������̸�ŭ ����
										if(pt2.distanceTo(pt3) > m_CutMinHeight*(1000.0/(m_CutScale*1.0))){//�ּҳ��̺��� Ŭ ���
											ptarr.append(pt1);ptarr.append(pt3);ptarr.append(pt2);
											if(bool_Chk1){//hidden �� ��� üũ��
													if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
														CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,_T("CC-BNCH"),0);
													else
														CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,m_CbxLayerStr,0);	
												}else{
													if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
														CreatePLine(ptarr,0,_T("bylayer"),1,_T("CC-BNCH"),0);
													else
														CreatePLine(ptarr,0,_T("bylayer"),1,m_CbxLayerStr,0);	
												}
											ptarr.removeAll();
											break;
										}else{//�ּҳ��̺��� ���� ���
											ads_name e_name;
											AcDbObjectId objId;
											AcGePoint3d PolyStpt;
											//������ ���� ������ ����
											acdbEntLast(e_name);
											acdbGetObjectId(objId, e_name);
											AcDbEntityPointer pEnt(objId,AcDb::kForRead);

											if(pEnt.openStatus() == Acad::eOk){
												if(pEnt->isKindOf(AcDbPolyline::desc())){		
													AcDbPolyline *pPline = AcDbPolyline::cast(pEnt);
													pPline->getStartPoint(PolyStpt);//������ �� �޾ƿ��� ��ü ����
													pPline->upgradeOpen();
													pPline->erase();
													pPline->close();
												}
												pEnt->close();
											}
											//PyoSi(PolyStpt);PyoSi(polyArr[polyArr.length()-2]);
											pt1 = PolyStpt;
											pt2 = polyArr[polyArr.length()-2];
											pt3.x = pt2.x; pt3.y = pt1.y;

											m_SubHeight = m_SubSlope * pt1.distanceTo(pt3) / 100; //����� ������
											pt3.y = pt3.y+m_SubHeight;//pt3�� y�� ����������̸�ŭ ����

											ptarr.append(pt1);ptarr.append(pt3);ptarr.append(pt2);
											if(bool_Chk1){//hidden �� ��� üũ��
												if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
													CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,_T("CC-BNCH"),0);
												else
													CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,m_CbxLayerStr,0);	
											}else{
												if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
													CreatePLine(ptarr,0,_T("bylayer"),1,_T("CC-BNCH"),0);
												else
													CreatePLine(ptarr,0,_T("bylayer"),1,m_CbxLayerStr,0);	
											}
											ptarr.removeAll();
											break;
										}
									}else{
										if(Fixpt2 != Fixpt3){//�������� ��簡 0�� �ƴ� ���
											acdbInters(asDblArray(upPt1),asDblArray(upPt2),asDblArray(Fixpt1),asDblArray(Fixpt2),0,asDblArray(InterPt));
											//PyoSi(InterPt);
											Polar(InterPt, ads_angle(asDblArray(Fixpt2),asDblArray(Fixpt3)), m_CutHeight*(1000.0/(m_CutScale*1.0)),pt3);
											ptarr.append(pt1);ptarr.append(pt3);ptarr.append(InterPt);
											if(bool_Chk1){//hidden �� ��� üũ��
												if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
													CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,_T("CC-BNCH"),0);
												else
													CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,m_CbxLayerStr,0);	
											}else{
												if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
													CreatePLine(ptarr,0,_T("bylayer"),1,_T("CC-BNCH"),0);
												else
													CreatePLine(ptarr,0,_T("bylayer"),1,m_CbxLayerStr,0);	
											}
											ptarr.removeAll();
											polyArr.removeAt(i);
											polyArr.insertAt(i,InterPt);
											break;
										}else{//���� �� ��簡 0�� ���
											//������ ���� ���ϱ�
											pt2 = polyArr[i];
											pt3.x = pt2.x; pt3.y = pt1.y;
											m_MainWidth = pt1.distanceTo(pt3);//���� ����
											m_MainHeight = pt2.distanceTo(pt3);//���� ����
											m_MainSlpDist = pt1.distanceTo(pt2);//���� ����
											//PyoSi(pt1);PyoSi(pt2);

											m_SubHeight = m_SubSlope * pt1.distanceTo(pt3) / 100; //����� ������
											pt3.y = pt3.y+m_SubHeight;//pt3�� y�� ����������̸�ŭ ����
											if(pt2.distanceTo(pt3) > m_CutMinHeight*(1000.0/(m_CutScale*1.0))){//�ּҳ��̺��� Ŭ ���
												ptarr.append(pt1);ptarr.append(pt3);ptarr.append(pt2);
												if(bool_Chk1){//hidden �� ��� üũ��
													if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
														CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,_T("CC-BNCH"),0);
													else
														CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,m_CbxLayerStr,0);	
												}else{
													if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
														CreatePLine(ptarr,0,_T("bylayer"),1,_T("CC-BNCH"),0);
													else
														CreatePLine(ptarr,0,_T("bylayer"),1,m_CbxLayerStr,0);	
												}
												ptarr.removeAll();
												break;
											}else{//�ּҳ��̺��� ���� ���
												ads_name e_name;
												AcDbObjectId objId;
												AcGePoint3d PolyStpt;
												//������ ���� ������ ����
												acdbEntLast(e_name);
												acdbGetObjectId(objId, e_name);
												AcDbEntityPointer pEnt(objId,AcDb::kForRead);

												if(pEnt.openStatus() == Acad::eOk){
													if(pEnt->isKindOf(AcDbPolyline::desc())){		
														AcDbPolyline *pPline = AcDbPolyline::cast(pEnt);
														pPline->getStartPoint(PolyStpt);//������ �� �޾ƿ��� ��ü ����
														pPline->upgradeOpen();
														pPline->erase();
														pPline->close();
													}
													pEnt->close();
												}
												//PyoSi(PolyStpt);PyoSi(polyArr[polyArr.length()-2]);
												pt1 = PolyStpt;
												pt2 = polyArr[polyArr.length()-2];
												pt3.x = pt2.x; pt3.y = pt1.y;

												m_SubHeight = m_SubSlope * pt1.distanceTo(pt3) / 100; //����� ������
												pt3.y = pt3.y+m_SubHeight;//pt3�� y�� ����������̸�ŭ ����

												ptarr.append(pt1);ptarr.append(pt3);ptarr.append(pt2);
												if(bool_Chk1){//hidden �� ��� üũ��
													if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
														CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,_T("CC-BNCH"),0);
													else
														CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,m_CbxLayerStr,0);	
												}else{
													if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
														CreatePLine(ptarr,0,_T("bylayer"),1,_T("CC-BNCH"),0);
													else
														CreatePLine(ptarr,0,_T("bylayer"),1,m_CbxLayerStr,0);	
												}
												ptarr.removeAll();
												break;
											}
										}
										
									}									
								}


								Polar(InterPt,ads_angle(asDblArray(Fixpt2),asDblArray(Fixpt3)),m_CutHeight*(1000.0/(m_CutScale*1.0)),pt3);

								ptarr.append(pt1);ptarr.append(pt3);ptarr.append(InterPt);
								if(bool_Chk1){//hidden �� ��� üũ��
									if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
										CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,_T("CC-BNCH"),0);
									else
										CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,m_CbxLayerStr,0);	
								}else{
									if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
										CreatePLine(ptarr,0,_T("bylayer"),1,_T("CC-BNCH"),0);
									else
										CreatePLine(ptarr,0,_T("bylayer"),1,m_CbxLayerStr,0);	
								}
								ptarr.removeAll();

								pt1 = InterPt;
							}//while end

						}//for i end ������ �� ����

						//������ ���� ���ϱ�
						pt2 = polyArr.last();
						//pt2 = ep;
						pt3.x = pt2.x; pt3.y = pt1.y;
						m_MainWidth = pt1.distanceTo(pt3);//���� ����
						m_MainHeight = pt2.distanceTo(pt3);//���� ����
						m_MainSlpDist = pt1.distanceTo(pt2);//���� ����

						if(m_CutExcept != 0 && m_MainWidth / m_MainHeight >= m_CutExcept ){//���� ����� �� ����X	
							pt2=polyArr[polyArr.length()-1];
							pt3.x = pt2.x; pt3.y = pt1.y;
						}else{
							m_SubHeight = m_SubSlope * pt1.distanceTo(pt3) / 100; //����� ������
							pt3.y = pt3.y+m_SubHeight;//pt3�� y�� ����������̸�ŭ ����
							//PyoSi(pt1);PyoSi(pt2);
							if(pt2.distanceTo(pt3) > m_CutMinHeight*(1000.0/(m_CutScale*1.0))){//�ּҳ��̺��� Ŭ ���
								ptarr.append(pt1);ptarr.append(pt3);ptarr.append(pt2);
								if(bool_Chk1){//hidden �� ��� üũ��
									if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
										CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,_T("CC-BNCH"),0);
									else
										CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,m_CbxLayerStr,0);	
								}else{
									if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
										CreatePLine(ptarr,0,_T("bylayer"),1,_T("CC-BNCH"),0);
									else
										CreatePLine(ptarr,0,_T("bylayer"),1,m_CbxLayerStr,0);	
								}
								ptarr.removeAll();
							}else{//�ּҳ��̺��� ���� ���
								ads_name e_name;
								AcDbObjectId objId;
								AcGePoint3d PolyStpt;
								//������ ���� ������ ����
								acdbEntLast(e_name);
								acdbGetObjectId(objId, e_name);
								AcDbEntityPointer pEnt(objId,AcDb::kForRead);

								if(pEnt.openStatus() == Acad::eOk){
									if(pEnt->isKindOf(AcDbPolyline::desc())){		
										AcDbPolyline *pPline = AcDbPolyline::cast(pEnt);
										pPline->getStartPoint(PolyStpt);//������ �� �޾ƿ��� ��ü ����
										pPline->upgradeOpen();
										pPline->erase();
										pPline->close();
									}
									pEnt->close();
								}
								//PyoSi(PolyStpt);
								pt1 = PolyStpt;
								pt2 = polyArr.last();
								//pt2 = ep;
								pt3.x = pt2.x; pt3.y = pt1.y;
								m_MainWidth = pt1.distanceTo(pt3);//���� ����
								m_MainHeight = pt2.distanceTo(pt3);//���� ����
								m_MainSlpDist = pt1.distanceTo(pt2);//���� ����

								if(m_CutExcept != 0 && m_MainWidth / m_MainHeight >= m_CutExcept ){//���� ����� �� ����X	
									pt2=polyArr[polyArr.length()-1];
									pt3.x = pt2.x; pt3.y = pt1.y;
								}

								m_SubHeight = m_SubSlope * pt1.distanceTo(pt3) / 100; //����� ������
								pt3.y = pt3.y+m_SubHeight;//pt3�� y�� ����������̸�ŭ ����

								ptarr.append(pt1);ptarr.append(pt3);ptarr.append(pt2);
								if(bool_Chk1){//hidden �� ��� üũ��
									if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
										CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,_T("CC-BNCH"),0);
									else
										CreatePLine(ptarr,0,_T("HIDDEN"),m_CutLscale,m_CbxLayerStr,0);	
								}else{
									if(bool_Chk2)//ǥ�� ���̾� ��� üũ��
										CreatePLine(ptarr,0,_T("bylayer"),1,_T("CC-BNCH"),0);
									else
										CreatePLine(ptarr,0,_T("bylayer"),1,m_CbxLayerStr,0);	
								}
								ptarr.removeAll();

							}
						}

					}//������ 2���̻��϶� end

				}else{					
					strMsg.LoadStringW(IDS_CHECK_PLINE); // "\n** �������� �ƴմϴ�. �ٽ� �����Ͻʽÿ�. **"
					acutPrintf(strMsg);
					continue;
				}
				pEnt->close();
			}

			break;
		}
	}//while ���ݼ����� end


	setSysVar(_T("OSMODE"),old_Osmode);//����� ������ ��ȯ��
}


void CCUT::OnBnClickedCutWrite()
{
	UpdateData(TRUE);

	iniFile.writeINI(_T("config.ini"),_T("CUT"),_T("Height"), m_CutHeightStr);
	iniFile.writeINI(_T("config.ini"),_T("CUT"),_T("Slope"), m_CutSlopeStr);
	iniFile.writeINI(_T("config.ini"),_T("CUT"),_T("Scale"), m_CutScaleStr);
	iniFile.writeINI(_T("config.ini"),_T("CUT"),_T("MinHeight"), m_CutMinHeightStr);
	iniFile.writeINI(_T("config.ini"),_T("CUT"),_T("Except"), m_CutExceptStr);
	iniFile.writeINI(_T("config.ini"),_T("CUT"),_T("Check1"), Chk1_Str);
	iniFile.writeINI(_T("config.ini"),_T("CUT"),_T("LScale"), m_CutLscaleStr);
	iniFile.writeINI(_T("config.ini"),_T("CUT"),_T("Check2"), Chk2_Str);	
	iniFile.writeINI(_T("config.ini"),_T("CUT"),_T("Color"), m_ColorStr);

	m_CbxLayerInt = m_CbxLayer.GetCurSel();
	m_CbxLayerIntStr.Format(_T("%d"),m_CbxLayerInt);
	m_CbxLayer.GetLBText(m_CbxLayerInt,m_CbxLayerStr);
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("CbxLayerInt"), m_CbxLayerIntStr);
	iniFile.writeINI(_T("config.ini"),_T("CUT"),_T("CbxLayer"), m_CbxLayerStr);	

	acDocManager->sendStringToExecute(acDocManager->curDocument(), _T("(FloorPicking)\n"), false, true);

	UpdateData(FALSE);
}

void CCUT::OnBnClickedCancel()
{	
	PostNcDestroy();
	//DestroyWindow();
	//CAcUiDialog::OnCancel();	
}

//������ ����
void CCUT::OnEnUpdateCutHeight()
{
	UpdateData(TRUE);
	m_CutHeight = _tstof(m_CutHeightStr);
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("Height"),m_CutHeightStr);
	UpdateData(FALSE);
}

//����� ���
void CCUT::OnEnUpdateCutSlope()
{
	UpdateData(TRUE);
	m_CutSlope = _tstof(m_CutSlopeStr);
	if (m_CutSlope > 10) {
		CString strMsg;
		strMsg.LoadStringW(IDS_SLOPE_CHECK); // "\n������ �� ��簡 �ʹ� Ů�ϴ�.\nȮ�� �� �ٽ� �Է����ֽʽÿ�.(���尪=0 �Ǵ� 4)"
		AfxMessageBox(strMsg);
	}
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("Slope"),m_CutSlopeStr);
	UpdateData(FALSE);

}

//���� ��ô
void CCUT::OnEnUpdateCutScale()
{
	UpdateData(TRUE);
	m_CutScale = _tstof(m_CutScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("Scale"),m_CutScaleStr);
	UpdateData(FALSE);
}

//�ּ� ����
void CCUT::OnEnUpdateCutMinheight()
{
	UpdateData(TRUE);
	m_CutMinHeight = _tstof(m_CutMinHeightStr);
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("MinHeight"),m_CutMinHeightStr);
	UpdateData(FALSE);
}

//������ ���� ��� 1: S
void CCUT::OnEnUpdateCutExcept()
{
	UpdateData(TRUE);
	m_CutExcept = _tstof(m_CutExceptStr);
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("Except"),m_CutExceptStr);
	UpdateData(FALSE);
}

///////////////////////////[Hidden �� �ɼ�]/////////////////////////////////////////////////////

//Hidden �� ��� üũ�ڽ�
void CCUT::OnBnClickedCutCheck1()
{
	bool_Chk1 = IsDlgButtonChecked(IDC_CUT_CHECK1);
	if(bool_Chk1 == 0){
		m_Chk1_btn.SetCheck(FALSE);
		Chk1_Str = _T("0");
		GetDlgItem(IDC_CUT_BSELECT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CUT_STATIC1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CUT_LSCALE)->EnableWindow(FALSE);
	}else{
		m_Chk1_btn.SetCheck(TRUE);
		Chk1_Str = _T("1");
		GetDlgItem(IDC_CUT_BSELECT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CUT_STATIC1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CUT_LSCALE)->EnableWindow(TRUE);
	}
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("Check1"), Chk1_Str);
}
//�� ��ô �����ư
void CCUT::OnBnClickedCutBselect1()
{
	UpdateData(TRUE);
	ads_name en;
	ads_point pt;
	AcCmColor Color;
	CString strMsg;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		::SetFocus(adsw_acadDocWnd());
		strMsg.LoadStringW(IDS_SELECT_ENTITY); // "\n��ü ���� : \n"
		int stat = acedEntSel(strMsg, en, pt);

		if(stat == RTCAN || stat == RTNONE)
			break;
		else if(stat == RTERROR){
			resbuf zValueStr;
			ads_getvar(_T("ERRNO"), &zValueStr);
			if(zValueStr.resval.rint == 52)
				break;
			else
				continue;
		}else if(stat == RTNORM){
			int NewIdx;
			AcDbObjectId objId;
			AcCmColor Color;
			TCHAR* LayerChar;
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbEntity::desc())){			 		
					AcDbEntity* pEntity = AcDbEntity::cast(pEnt);
					m_CutLscale = pEntity->linetypeScale();
					TCHAR* aa = pEntity->linetype();

					pEntity->close();

					m_CutLscaleStr.Format(_T("%.1f"),m_CutLscale);
					iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("LScale"), m_CutLscaleStr);
					//pCUT->SetDlgItemTextW(IDC_CUT_LSCALE,m_CutLscaleStr);
					//OnEnUpdateCutLscale();
					pEnt->close();
					//acedSSFree(en);	
					break;
				}else{
					strMsg.LoadStringW(IDS_CHECK_SCALE); // "**��ô�� ������ �� �����ϴ�..**"
					acutPrintf(strMsg);
					pEnt->close();
					//acedSSFree(en);	
					continue;
				}
			}

		}
	}

	UpdateData(FALSE);
}

//Hidden �� ��ô
void CCUT::OnEnUpdateCutLscale()
{
	UpdateData(TRUE);
	m_CutLscale = _tstof(m_CutLscaleStr);
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("LScale"),m_CutLscaleStr);
	UpdateData(FALSE);
}

///////////////////////////[���̾� �� ���� ����]/////////////////////////////////////////////////////

//ǥ�� ���̾� ��� üũ�ڽ�
void CCUT::OnBnClickedCutCheck2()
{
	bool_Chk2 = IsDlgButtonChecked(IDC_CUT_CHECK2);
	if(bool_Chk2 == 0){
		m_Chk2_btn.SetCheck(FALSE);
		Chk2_Str = _T("0");
		GetDlgItem(IDC_CUT_CBXLAYER)->EnableWindow(TRUE);
		GetDlgItem(IDC_CUT_COLORBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CUT_BSELECT2)->EnableWindow(TRUE);
	}else{
		m_Chk2_btn.SetCheck(TRUE);
		Chk2_Str = _T("1");	
		StdLayer_idx = m_CbxLayer.FindString(-1, _T("CC-BNCH"));
		m_CbxLayer.SetCurSel(StdLayer_idx);
		ColorSetting(3);
		GetDlgItem(IDC_CUT_CBXLAYER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CUT_COLORBTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CUT_BSELECT2)->EnableWindow(FALSE);
		
	}
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("Check2"), Chk2_Str);
}

//���̾� ���� �޺��ڽ�
void CCUT::OnCbnSelchangeCutCbxlayer()
{
	UpdateData(TRUE);
	m_CbxLayerInt = m_CbxLayer.GetCurSel();
	if(m_CbxLayerInt == -1)m_CbxLayerInt = 0;
	m_CbxLayer.SetCurSel(m_CbxLayerInt);
	m_CbxLayerIntStr.Format(_T("%d"),m_CbxLayerInt);
	m_CbxLayer.GetLBText(m_CbxLayerInt, m_CbxLayerStr);
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("CbxLayer"), m_CbxLayerStr);
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("CbxLayerInt"), m_CbxLayerIntStr);
	int lastSel = m_CbxLayer.FindString(-1,m_CbxLayerStr);

	int layerColor = getLayerColor(m_CbxLayerStr);
	ColorSetting(layerColor);

	UpdateData(FALSE);
}

//���� ���� ��ư
void CCUT::OnBnClickedCutColorbtn()
{
	UpdateData(TRUE);
	acedSetColorDialog(m_Color,true,256);
	ColorSetting(m_Color);
	UpdateData(FALSE);
}

//���� ���� ��ư
void CCUT::OnBnClickedCutBselect2()
{
	UpdateData(TRUE);
	ads_name en;
	ads_point pt;
	AcCmColor Color;
	CString strMsg;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(true){
		::SetFocus(adsw_acadDocWnd());
		strMsg.LoadStringW(IDS_SELECT_ENTITY); // "\n��ü ���� : \n"
		int stat = acedEntSel(strMsg, en, pt);

		if(stat == RTCAN || stat == RTNONE)
			break;
		else if(stat == RTERROR){
			resbuf zValueStr;
			ads_getvar(_T("ERRNO"), &zValueStr);
			if(zValueStr.resval.rint == 52)
				break;
			else
				continue;
		}else if(stat == RTNORM){
			int NewIdx;
			AcDbObjectId objId, LayerId;
			AcCmColor Color;
			TCHAR* LayerChar;
			acdbGetObjectId(objId, en);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbEntity::desc())){
					//AcDbText* pText = AcDbText::cast(pEnt);
					//LayerId = pText->layerId();
					//m_CbxLayerStr = pText->layer();
					AcDbEntity* pEntity = AcDbEntity::cast(pEnt);
					LayerId = pEntity->layerId();
					m_CbxLayerStr = pEntity->layer();

					AcDbLayerTableRecordPointer pLayer(LayerId, AcDb::kForRead);
					Color = pLayer->color();
					m_Color = Color.colorIndex();

					pLayer->close();
					pEntity->close();

					for(int i=0; i < m_CbxLayer.GetCount(); i++){
						if(m_CbxLayer.FindString(i, m_CbxLayerStr) == -1){//�޺��ڽ��� ������ ���̾� �߰�
							m_CbxLayer.AddString(m_CbxLayerStr);
						}
					}
					m_CbxLayerInt = m_CbxLayer.FindString(0,m_CbxLayerStr);//������ ���̾� index ���ϱ�
					m_CbxLayer.SetCurSel(m_CbxLayerInt);//�޺��ڽ��� �߰��� ���̾�� ����
					m_CbxLayerIntStr.Format(_T("%d"), m_CbxLayerInt);

					m_ColorStr.Format(_T("%d"),m_Color);
					iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("Color"), m_ColorStr);
					iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("CbxLayerInt"), m_CbxLayerIntStr);
					iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("CbxLayer"), m_CbxLayerStr);
					ColorSetting(m_Color);
					pEnt->close();
					acedSSFree(en);	
					break;
				}else{
					strMsg.LoadStringW(IDS_NOT_CHECK_COLOR); // "**������ ������ �� �����ϴ�..**"
					acutPrintf(strMsg);
					pEnt->close();
					acedSSFree(en);	
					continue;
				}
			}

		}
	}


	UpdateData(FALSE);
}

//���� ���
void CCUT::ColorSetting(int newColorIndex)
{
	UpdateData(TRUE);
	CString ColorName;
	CRect prect;
	m_btn_Color.GetClientRect (& prect); // get the zone

	//	COLORREF cref = acedGetRGB(nNewColor);

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
	FillRect (m_btn_Color.GetDC () -> GetSafeHdc (), & prect, CBrush (RGB (r,g,b))); // fill color 

	((CButton*) GetDlgItem(IDC_CUT_STATIC_COLOR))->SetWindowText(ColorName);
	m_ColorStr.Format(_T("%d"),newColorIndex);
	iniFile.writeINI(_T("config.ini"), _T("CUT"), _T("Color"), m_ColorStr);
	UpdateData(FALSE);

}

void CCUT::OnPaint()
{
	UpdateData(TRUE);
	CPaintDC dc(this); // device context for painting
	m_Color = _ttoi(m_ColorStr);
	ColorSetting(m_Color);
	UpdateData(FALSE);
}

extern CCUT *pCUT;
void CCUT::PostNcDestroy()
{
	delete this;
	pCUT = NULL;
	//CAcUiDialog::PostNcDestroy();
}

//���̾� ����
bool CCUT::CreateLayer(CString Layer, int color){
	AcDbLayerTable* pLayerTable;
	AcCmColor Layercolor;
	Layercolor.setColorIndex(color);
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return false;

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
		pLayerTable->add(pLayerTableRecord);
		pLinetypeTable->close();
		pLayerTableRecord->close();
		pLayerTable->close();		
	}
	else{
		pLayerTable->close();


		return false;
	}
	return true;
}

int CCUT::getLayerColor(CString LayerName){
	//���̾� �ҷ�����
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);
	int colorindex=-1;

	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		pLayerTblRcd->getName(name);
		if(LayerName == name){//���̾ ���� ��� ����ã��

			AcDbLayerTableRecordPointer layerTableRec(LayerId,  AcDb::kForRead);

			if (layerTableRec.openStatus() == Acad::eOk)
			{

				AcCmColor color = layerTableRec->color();
				colorindex = color.colorIndex();
				break;
			}
			layerTableRec->close();
		}
	}
	delete pLayerTblIter;
	delete pLayerTbl;

	return colorindex;
}

bool CCUT::CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer, double Color){
	Acad::ErrorStatus es;
	AcDbObjectId pLineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	acDocManager->curDocument()->database();
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

Acad::ErrorStatus CCUT::setVar(LPCTSTR varName, const resbuf* newVal){
	CString str;
	if (ads_setvar(varName, newVal) != RTNORM) {
		str.Format(_T("Could not set system variable \"%s\"."), varName);
		return(Acad::eInvalidInput);
	}
	else {
		return(Acad::eOk);
	}
}
Acad::ErrorStatus CCUT::setSysVar(LPCTSTR varName, int val){
	ASSERT(varName != NULL);

	resbuf rb;
	rb.restype = RTSHORT;
	rb.resval.rint = (short)val;

	return(setVar(varName, &rb));
}

Acad::ErrorStatus CCUT::getSysVar(LPCTSTR varName, int& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTSHORT);
		val = rb.resval.rint;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

void CCUT::Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result)
{
	result.set(base.x + dist*cos(angle), base.y + dist*sin(angle), base.z);
}

void CCUT::PyoSi(const AcGePoint3d pos)
{
	AcGePoint3d	pt1, pt2, pt3;
	double		ang[] = { 0.25, 0.75 };
	double		ps = getPickBox();		// real size of pickbox in screen 
	double kPi = 3.14159265358979323846;


	for (int dur = 0; dur < 2; dur++) {
		ads_polar(asDblArray(pos), ang[dur] * kPi, ps, asDblArray(pt2));
		ads_polar(asDblArray(pos), (ang[dur] + 1.0) * kPi, ps, asDblArray(pt3));
		acedGrDraw(asDblArray(pt2), asDblArray(pt3), 1, 0);
	}//end-for
}
double CCUT::getPickBox()
{
	int			iPickBox;
	double		viewsize;
	AcGePoint2d screensize;

	getSysVar(_T("VIEWSIZE"), viewsize);
	getSysVar(_T("SCREENSIZE"), screensize);
	getSysVar(_T("PICKBOX"), iPickBox);

	return ((viewsize / screensize.y)*(double)iPickBox);
}

Acad::ErrorStatus CCUT::getSysVar(LPCTSTR varName, double& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTREAL);
		val = rb.resval.rreal;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

Acad::ErrorStatus CCUT::getSysVar(LPCTSTR varName, AcGePoint2d& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTPOINT);
		val.set(rb.resval.rpoint[X], rb.resval.rpoint[Y]);
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

void CCUT::getclosestPoint(AcGePoint3d sp, AcGePoint3d ep, AcGePoint3d givenPnt, AcGePoint3d& closestPnt)
{
	AcGeVector3d v,v1,v2;
	AcGeMatrix3d mat;
	double  l=(double)0, ang=(double)0, dst=(double)0;

	v=ep-sp;
	v.normalize();
	l=sp.distanceTo(givenPnt);	
	v1 = givenPnt-sp;
	v2 = ep - sp;
	ang = v1.angleTo(v2);
	dst=l*cos(ang);
	v=v*dst;
	mat.setToTranslation(v);
	closestPnt=sp;
	closestPnt.transformBy(mat);		
}

void CCUT::OnBnClickedCutHelp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString Help_Path = g_modulePath;
	int findNUM = Help_Path.ReverseFind('\\');
	Help_Path.Delete(findNUM, Help_Path.GetLength() - 1);

	CString cadlang;
	cadlang.Format(L"%s", g_CADLang);

	CString lang =
		ReadStringValueInRegistry(HKEY_CURRENT_USER, L"SOFTWARE\\WORKS", L"Language");

	CString helpFile = L"ZDREAM.chm";
#ifdef _ZYXCAD
	helpFile.Replace(L"ZDREAM.chm", L"WORKS.chm");
#endif
	helpFile = GetHelpPath(cadlang, lang, helpFile);

	Help_Path.Format(_T("%s\\%s::/1. html/CUT.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
