// DSLO.cpp : ���� �����Դϴ�.
#include "stdafx.h"
#include "DSLO.h"
#include "afxdialogex.h"

// CDSLO ��ȭ �����Դϴ�.
IMPLEMENT_DYNAMIC(CDSLO, CAcUiDialog)
	CDSLO::CDSLO(CWnd* pParent)
	: CAcUiDialog(CDSLO::IDD, pParent), m_EditBaseDraw_Str(_T(""))
	, m_EditSubDraw_Str(_T("")), bClayer(FALSE)
{
}

CDSLO::~CDSLO(){
}

void CDSLO::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBXLAYER, m_cbxLayer);
	DDX_Control(pDX, IDC_BLAYERSELECT, m_selectBn);
	DDX_Control(pDX, IDC_EDIT_1, m_EditBaseDraw_Ctrl);
	DDX_Text(pDX, IDC_EDIT_1, m_EditBaseDraw_Str);
	DDX_Text(pDX, IDC_EDIT_2, m_EditSubDraw_Str);
	DDX_Check(pDX, IDC_CHECK_CLAYER, bClayer);
}

BEGIN_MESSAGE_MAP(CDSLO, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CDSLO::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDSLO::OnBnClickedCancel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_1, &CDSLO::OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BCOLOR, &CDSLO::OnBnClickedBcolor)
	ON_WM_DRAWITEM()
	ON_EN_UPDATE(IDC_EDIT_1, &CDSLO::OnEnUpdateEdit1)
	ON_CBN_SELCHANGE(IDC_CBXLAYER, &CDSLO::OnCbnSelchangeCbxlayer)
	ON_BN_CLICKED(IDC_BLAYERSELECT, &CDSLO::OnBnClickedBlayerselect)
	ON_BN_CLICKED(IDC_CHECK_CLAYER, &CDSLO::OnBnClickedClayer)
	ON_BN_CLICKED(ID_DSLO_HELP, &CDSLO::OnBnClickedDsloHelp)
END_MESSAGE_MAP()

// CDSLO �޽��� ó�����Դϴ�.

BOOL CDSLO::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	
	//������ �ҷ�����
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);

	AcDbObjectId LayerId;
	for (pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()) {
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		pLayerTblRcd->getName(name);
		m_cbxLayer.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;
	
	//������ �б�
	jobStat = 1;
	CString baseShowSpaceStr = iniFile.readINI(_T("config.ini"), _T("DSLO"), _T("BASESHOWSAPCE"));
	if(baseShowSpaceStr.CompareNoCase(_T("")) == 0)	baseShowSpaceStr = _T("10");
	SetDlgItemText(IDC_EDIT_1, baseShowSpaceStr);
	CString subShowCnteStr = iniFile.readINI(_T("config.ini"), _T("DSLO"), _T("SUBSHOWCOUNT"));
	if(subShowCnteStr.CompareNoCase(_T("")) == 0)	subShowCnteStr = _T("3");
	SetDlgItemText(IDC_EDIT_2, subShowCnteStr);
	CString baseShowCnteStr = iniFile.readINI(_T("config.ini"), _T("DSLO"), _T("BASESHOWCOUNT"));
	if(baseShowCnteStr.CompareNoCase(_T("")) == 0)	baseShowCnteStr = _T("1");
	SetDlgItemText(IDC_EDIT_3, baseShowCnteStr);
	
	CString tempStr;
	//���� ������ ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("DSLO"), _T("ChkCLayer"));
	//���� �������� �ƴ� ��
	if (tempStr.CompareNoCase(_T("TRUE")) != 0) {
		//���̾� ����
		tempStr = iniFile.readINI(_T("config.ini"), _T("DSLO"), _T("CmbLayer"));
		if (tempStr.CompareNoCase(_T("")) == 0) {
			CmbLayer = _T("0");
		}
		else {
			CmbLayer = tempStr;
		}
		//�޺��ڽ� ǥ�� ���̾�
		tempStr = iniFile.readINI(_T("config.ini"), _T("DSLO"), _T("cbxLayer"));
		if (tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0) {
			m_cbxLayer.SetCurSel(0);
			m_cbxLayer.GetLBText(0, CmbLayer);
		}
		else {
			cbxLayerInt_Str = tempStr;
			cbxLayerInt = _ttoi(cbxLayerInt_Str);
			//m_cbxLayer.SetCurSel(cbxLayerInt);
			if (m_cbxLayer.FindStringExact(-1, CmbLayer) == -1) {//���̾ �������� ���� ���
				m_cbxLayer.SetCurSel(0);
				m_cbxLayer.GetLBText(0, CmbLayer);
			}
			else {
				m_cbxLayer.SetCurSel(cbxLayerInt);
				m_cbxLayer.GetLBText(cbxLayerInt, CmbLayer);
			}
			SetDlgItemText(IDC_CBXLAYER, CmbLayer);
		}
		OnCbnSelchangeCbxlayer();
	} 
	// ���� ������ ���� ��
	else {
		((CButton*)GetDlgItem(IDC_CHECK_CLAYER))->SetCheck(1);
		AcDbObjectId CLayID;

		CLayID = acdbHostApplicationServices()->workingDatabase()->clayer();
		AcDbLayerTableRecordPointer pClayerTblRcd(CLayID, AcDb::kForRead);
		TCHAR* name;
		pClayerTblRcd->getName(name);
		LayerChangeGetColor(name);
		m_cbxLayer.SetCurSel(m_cbxLayer.FindString(0, name));
	}
	
	//m_cbxLayer.SetCurSel(0);
	m_selectBn.AutoLoad();
	return TRUE;
}

void CDSLO::OnBnClickedOk(){
	CString baseShowSpaceStr;
	GetDlgItemText(IDC_EDIT_1, baseShowSpaceStr);	
	iniFile.writeINI(_T("config.ini"), _T("DSLO"), _T("BASESHOWSAPCE"), baseShowSpaceStr);
	CString subShowCnteStr;
	GetDlgItemText(IDC_EDIT_2, subShowCnteStr);
	iniFile.writeINI(_T("config.ini"), _T("DSLO"), _T("SUBSHOWCOUNT"), subShowCnteStr);
	CString baseShowCnteStr;
	GetDlgItemText(IDC_EDIT_3, baseShowCnteStr);
	iniFile.writeINI(_T("config.ini"), _T("DSLO"), _T("BASESHOWCOUNT"), baseShowCnteStr);
	
	if (IsDlgButtonChecked(IDC_CHECK_CLAYER)) {
		iniFile.writeINI(_T("config.ini"), _T("DSLO"), _T("ChkCLayer"), L"TRUE");
	}
	else {
		iniFile.writeINI(_T("config.ini"), _T("DSLO"), _T("ChkCLayer"), L"FALSE");
		cbxLayerInt = m_cbxLayer.GetCurSel();
		cbxLayerInt_Str.Format(_T("%d"), cbxLayerInt);
		iniFile.writeINI(_T("config.ini"), _T("DSLO"), _T("cbxLayer"), cbxLayerInt_Str);
		CString layerStr;
		m_cbxLayer.GetLBText(m_cbxLayer.GetCurSel(), layerStr);
		iniFile.writeINI(_T("config.ini"), _T("DSLO"), _T("CmbLayer"), layerStr);
	}
	UpdateData();
	if(m_EditBaseDraw_Str.IsEmpty() || m_EditBaseDraw_Str.CompareNoCase(_T("0")) == 0)
		return;

	UpdateData(FALSE);
	OnOK();
	CString layerStr;
	m_cbxLayer.GetLBText(m_cbxLayer.GetCurSel(), layerStr);
	ads_point sp, ep;
	ads_name startEn, endEn;
	AcDbObjectId firstLineId , secondLineId;
	CString curLayer, strMsg;
	m_cbxLayer.GetLBText(m_cbxLayer.GetCurSel(), curLayer);
	setLayer = (LPCTSTR)curLayer;
	bool selectionCheck = false;
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	strMsg.LoadStringW(IDS_SLOPE_START); // "\n��� ���ۼ� ����: "
	if(ads_entsel(strMsg, startEn, sp) == RTNORM){
		strMsg.LoadStringW(IDS_SLOPE_END); // "\n��� ���� ����: "
		if (ads_entsel(strMsg, endEn, ep) == RTNORM) {
			selectionCheck = true;
		}
	}
	if(!selectionCheck)
		return;
	CompleteEditorCommand();
	if(acdbGetObjectId(firstLineId, startEn) == Acad::eOk)
	{
		AcDbEntityPointer pEnt(firstLineId, AcDb::kForRead);
		if(pEnt.openStatus() == Acad::eOk)
		{
			if(pEnt->isKindOf(AcDbLine::desc()))	// ó�� ������ ��ü�� ������ ���
			{
				AcDbLine* baseLine = new AcDbLine();
				baseLine = AcDbLine::cast(pEnt);
				AcGePoint3d baseLineSp, baseLineEp;
				baseLineSp.set(baseLine->startPoint().x, baseLine->startPoint().y, baseLine->startPoint().z);	// �輺�� �븮 ����, Z�� 0�ΰ��� Z�� ���������� ���� (06.25)
				// ù��° ������ start ����Ʈ�� ������
				baseLineEp.set(baseLine->endPoint().x, baseLine->endPoint().y, baseLine->endPoint().z);	// �輺�� �븮 ����, Z�� 0�ΰ��� Z�� ���������� ���� (06.25)
				// ù��° ������ end ����Ʈ�� ������

				if(acdbGetObjectId(secondLineId, endEn) == Acad::eOk)
				{
					AcDbEntityPointer pSecondEnt(secondLineId, AcDb::kForRead);
					if(pSecondEnt.openStatus() == Acad::eOk)
					{
						if(pSecondEnt->isKindOf(AcDbLine::desc()))	// �ι�° ������ ��ü�� ������ ���
						{
							AcDbLine* endLine = new AcDbLine();
							endLine = AcDbLine::cast(pSecondEnt); // �ι�° ������ ��ü�� endLine�� ����
							AcGeVector3d vec, distVec;

							AcGePoint3d startPoint ,nextPoint;
							double nextdist = 0.0;
							CString userDistStr;
							GetDlgItemText(IDC_EDIT_1, userDistStr);
							nextdist = _ttof(userDistStr);
							baseLine->getStartPoint(startPoint);
							baseLine->getPointAtDist(nextdist, nextPoint);

							vec = (startPoint - nextPoint).normal();	// ù��° ������ ���� ���� ����
							distVec = (startPoint - nextPoint).normal(); // ù��° ������ ���� ���� ����
							vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis); // ���� ����Ʈ ����

							AcGePoint3d tmpPoint, tmpPoint2;
							double maxDist;
							int loopCnt;

							maxDist = baseLineSp.distanceTo(baseLineEp); // ù��° ���� ������ ���� ����
							loopCnt = maxDist / _ttof(userDistStr); // �� ǥ�ð��� �� 
							AcGePoint3d basePoint = baseLineSp;	// basePoint = ù��° ��ü�� start ����Ʈ
							AcGePoint3d tmp_basePoint = baseLineSp; // ù��° ��ü�� start ����Ʈ ���(07.03 �߰�)

							AcGeLineSeg3d* testLine = new AcGeLineSeg3d(endLine->startPoint(), endLine->endPoint()); // 3D���� ����(�ι�° ��ü)
							AcGeLineSeg3d* testSubLine = new AcGeLineSeg3d(baseLine->startPoint(), baseLine->endPoint()); // �ι�° line�� �������� ���� Ȯ��
							AcGePoint3dArray pts, subPts;
							for(int i = 0; i < loopCnt+1; i++)
							{
								pts.removeAll(); // pts �ʱ�ȭ
								tmpPoint = basePoint - vec * 100;
								basePoint.z = tmpPoint.z = endLine->startPoint().z;  // �������� �Ϻ� ����(basePoint.z �߰�)
								AcDbLine* tmpLine = new AcDbLine(basePoint, tmpPoint);
								tmpLine->intersectWith(endLine, AcDb::kExtendThis, pts); // z���� ���� �������� ã�� ����(Ȯ���ؾߵǴ� ����)
								tmpLine->close();

								int chk = pts.length();
								if(chk != 0)
								{
									tmp_basePoint.set(basePoint.x,basePoint.y,baseLineSp.z); // �輺�� �븮 �߰�
									AcDbLine* drawLine = new AcDbLine(tmp_basePoint, pts[0]); // basePoint���� tmp_basePoint�� ����
									AcDbLine* tmp_drawLine = new AcDbLine(basePoint, pts[0]); // vector�� �����ҷ��� ���� ����
									drawLine->setLayer(setLayer);
									
									AcGeVector3d subVec;
									subVec = (tmp_drawLine->startPoint() - tmp_drawLine->endPoint()).normal();	// �� ǥ�ö����� �������͸� ������?
									tmp_drawLine->close();
								
									AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
									acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
									drawLine->setDatabaseDefaults();
									bTR->appendAcDbEntity(drawLine);
									drawLine->close();
									bTR->close();
								}
								AcGePoint3d subStartPoint, subEndPoint, tmpsubStartPoint, tmpsubEndPoint; // tmpsubStartPoint, tmpsubEndPoint �߰�;
								subStartPoint = basePoint;
								double subDist = _ttof(userDistStr) / (_ttoi(m_EditSubDraw_Str) + 1);
								for(int j = 0; j < _ttoi(m_EditSubDraw_Str); j++)
								{
									subPts.removeAll();
									subStartPoint = subStartPoint - distVec * subDist;
									tmpPoint2 = subStartPoint - vec * 100;									
									AcDbLine* tmpLine2 = new AcDbLine(subStartPoint, tmpPoint2);
									tmpLine2->intersectWith(endLine, AcDb::kExtendThis, subPts);
									int subchk = subPts.length();
									if(subchk != 0)
									{
										double subLineDist = tmpLine2->startPoint().distanceTo(subPts[0]);
										tmpLine2->close();

										tmpsubStartPoint.set(subStartPoint.x,subStartPoint.y,baseLine->startPoint().z);
										tmpsubEndPoint.set(subPts[0].x,subPts[0].y,(baseLine->startPoint().z + subEndPoint.z)/2);

										AcDbLine* tmpsubLine = new AcDbLine(tmpsubStartPoint, tmpsubEndPoint);
										AcGePoint3d halfEndPoint;
										double subDistance;
										tmpsubLine->getDistAtPoint(tmpsubLine->endPoint(),subDistance);
										subDistance = subDistance * 0.5;
										tmpsubLine->getPointAtDist(subDistance,halfEndPoint);
										tmpsubLine->close();

										AcDbLine* subLien = new AcDbLine(tmpsubStartPoint, halfEndPoint);
										subLien->setLayer(setLayer);
										AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
										acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
										subLien->setDatabaseDefaults();
										
										if(testSubLine->isOn(tmpsubStartPoint) == Adesk::kTrue)
										{
											bTR->appendAcDbEntity(subLien);
										}										
										subLien->close();
										bTR->close();
									}
									tmpLine2->close();
								}
								
								basePoint = basePoint - distVec * _ttof(userDistStr);
								startPoint = nextPoint;
								nextdist += _ttof(userDistStr);
								if(baseLine->getPointAtDist(nextdist, nextPoint) != Acad::eOk)
								{
									baseLine->getEndPoint(nextPoint);
								}
							}
							delete testSubLine;
							delete testLine;
							baseLine->close();							
							endLine->close();
						}
						else if(pSecondEnt->isKindOf(AcDbPolyline::desc()))
						{
							AcDbPolyline* endPLine = new AcDbPolyline();
							endPLine = AcDbPolyline::cast(pSecondEnt);							

							AcGePoint3d startPoint ,nextPoint;
							double nextdist = 0.0;
							CString userDistStr;
							GetDlgItemText(IDC_EDIT_1, userDistStr);
							nextdist = _ttof(userDistStr);
							baseLine->getStartPoint(startPoint);
							baseLine->getPointAtDist(nextdist, nextPoint);

							for(int bi = 0; bi < endPLine->numVerts()-1; bi++){
								AcDbLine* endLine = new AcDbLine();
								AcGePoint3d subLineSp, subLineEp;
								endPLine->getPointAt(bi, subLineSp);
								endLine->setStartPoint(subLineSp);
								endPLine->getPointAt(bi+1, subLineEp);
								endLine->setEndPoint(subLineEp);
								AcGeVector3d vec, distVec;
								vec = (startPoint - nextPoint).normal();
								distVec = (startPoint - nextPoint).normal();
								vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
								AcGePoint3d tmpPoint, tmpPoint2;
								double maxDist;
								int loopCnt;
								maxDist = baseLineSp.distanceTo(baseLineEp);								
								loopCnt = maxDist / _ttof(userDistStr);
								AcGePoint3d basePoint = baseLineSp;
								AcGeLineSeg3d* testLine = new AcGeLineSeg3d(endLine->startPoint(), endLine->endPoint());
								AcGeLineSeg3d* testSubLine = new AcGeLineSeg3d(baseLine->startPoint(), baseLine->endPoint()); // �ι�° line�� �������� ���� Ȯ��
								AcGePoint3dArray pts, subPts;
								AcGePoint3d tmpbaseLineSp;
								for(int i = 0; i < loopCnt+1; i++)
								{
									pts.removeAll();
									tmpPoint = basePoint - vec * 100;
									basePoint.z = tmpPoint.z = endLine->startPoint().z;  // �������� �Ϻ� ����(basePoint.z �߰�)
									AcDbLine* tmpLine = new AcDbLine(basePoint, tmpPoint);
									tmpLine->intersectWith(endPLine, AcDb::kExtendThis, pts);
									tmpLine->close();
									if(pts.length() != 0)
									{
										if(testLine->isOn(pts[0]) == Adesk::kFalse)
										{
											basePoint = basePoint - distVec * _ttof(userDistStr);
											continue;
										}
										tmpbaseLineSp.set(basePoint.x,basePoint.y,baseLineSp.z);
										AcDbLine* drawLine = new AcDbLine(tmpbaseLineSp, pts[0]);
										drawLine->setLayer(setLayer);
										AcGePoint3d subStartPoint, subEndPoint, tmpsubStartPoint, tmpsubEndPoint; // tmpsubStartPoint, tmpsubEndPoint �߰�
										AcGeVector3d subVec;
										subVec = (drawLine->startPoint() - drawLine->endPoint()).normal();
										subStartPoint = basePoint;
										double subDist = _ttof(userDistStr) / (_ttoi(m_EditSubDraw_Str) + 1);
										for(int j = 0; j < _ttoi(m_EditSubDraw_Str); j++){
											subPts.removeAll();
											subStartPoint = subStartPoint - distVec * subDist;
											tmpPoint2 = subStartPoint - vec * 100;
											AcDbLine* tmpLine2 = new AcDbLine(subStartPoint, tmpPoint2);
											tmpLine2->intersectWith(endLine, AcDb::kExtendThis, subPts);
											double subLineDist = tmpLine2->startPoint().distanceTo(subPts[0]);
											tmpLine2->close();
											subEndPoint = subStartPoint - vec * (subLineDist / 2);

											tmpsubStartPoint.set(subStartPoint.x,subStartPoint.y,baseLine->startPoint().z);
											tmpsubEndPoint.set(subEndPoint.x,subEndPoint.y,(baseLine->startPoint().z + subEndPoint.z)/2);

											AcDbLine* subLien = new AcDbLine(tmpsubStartPoint, tmpsubEndPoint);
											subLien->setLayer(setLayer);
											AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
											acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
											subLien->setDatabaseDefaults();
											if(testSubLine->isOn(tmpsubStartPoint) == Adesk::kTrue)
											{
												bTR->appendAcDbEntity(subLien);
											}									
											subLien->close();
											bTR->close();
										}
										AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
										acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
										drawLine->setDatabaseDefaults();
										bTR->appendAcDbEntity(drawLine);
										drawLine->close();
										bTR->close();
									}
									basePoint = basePoint - distVec * _ttof(userDistStr);
									startPoint = nextPoint;
									nextdist += _ttof(userDistStr);
									if(baseLine->getPointAtDist(nextdist, nextPoint) != Acad::eOk)
									{
										baseLine->getEndPoint(nextPoint);
									}
								}
								delete testLine;
								delete testSubLine;
								baseLine->close();
								endLine->close();
							}
							endPLine->close();
						}
						else{
							baseLine->close();
							pSecondEnt->close();
							pEnt->close();
							return;
						}
					}
					pSecondEnt->close();
				}
				baseLine->close();
			}
			else if(pEnt->isKindOf(AcDbPolyline::desc())) // ù��° ��ü�� ���������� ���
			{
				AcDbPolyline* basePLine = new AcDbPolyline();
				basePLine = AcDbPolyline::cast(pEnt);

				if(acdbGetObjectId(secondLineId, endEn) == Acad::eOk)
				{
					AcDbEntityPointer pSecondEnt(secondLineId, AcDb::kForWrite);
					if(pSecondEnt.openStatus() == Acad::eOk)
					{
						if(pSecondEnt->isKindOf(AcDbLine::desc()))
						{
							AcDbLine* endLine = new AcDbLine();
							endLine = AcDbLine::cast(pSecondEnt); // ���� �ι�° ��(z���� ����������)

							AcGePoint3d tmpStartPoint, tmpEndPoint;
							endLine->getStartPoint(tmpStartPoint);
							endLine->getEndPoint(tmpEndPoint);

							tmpStartPoint.z=0.0;
							tmpEndPoint.z=0.0;

							AcDbLine* tmpSubLine = new AcDbLine();
							tmpSubLine->setStartPoint(tmpStartPoint);
							tmpSubLine->setEndPoint(tmpEndPoint);
							
							double endParam, Pdist, checkParam, checkParam2, SubcheckParam;
							AcGePoint3d startPoint ,nextPoint;

							CString userDistStr;							
							GetDlgItemText(IDC_EDIT_1, userDistStr);

							double nextdist = 0.0;
							nextdist = _ttof(userDistStr);

							basePLine->getEndParam(endParam);
							basePLine->getDistAtParam(endParam,Pdist);

							int loopCnt;
							loopCnt = ceil(Pdist / _ttof(userDistStr)); // ��ǥ�� ����

							basePLine->getStartPoint(startPoint);
							basePLine->getPointAtDist(nextdist, nextPoint);

							AcGePoint3dArray pts, subPts;
							AcGePoint3d tmpPoint, tmpPoint2, subtmpPoint, subtmpPoint2; // �ӽ� ����Ʈ
							AcGePoint3d LineSp, LineEp, SubLineSp, SubLineEp; // ���μ� SP,EP & ���꼱�� SP,EP
							AcGeLineSeg3d* testLine = new AcGeLineSeg3d(endLine->startPoint(), endLine->endPoint());

							for(int i =0; i < loopCnt+1; i++)
							{
								basePLine->getParamAtPoint(startPoint,checkParam);
								basePLine->getParamAtPoint(nextPoint,checkParam2);

								AcGeVector3d vec, distVec;
								if(floor(checkParam) == floor(checkParam2))
								{
									vec = (startPoint - nextPoint).normal();
									vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);								
									distVec = (startPoint - nextPoint).normal();
								}
								else
								{
									AcGePoint3d tmp_nextPoint;
									basePLine->getPointAtParam(floor(checkParam)+1, tmp_nextPoint);
									vec = (tmp_nextPoint - startPoint).normal();
									vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
									distVec = (startPoint - tmp_nextPoint).normal();
								}
								pts.removeAll();
								tmpPoint.set(startPoint.x,startPoint.y,0);
								tmpPoint2 = tmpPoint - vec * 100;
								AcDbLine* tmpLine = new AcDbLine(tmpPoint, tmpPoint2);
								tmpLine->intersectWith(tmpSubLine, AcDb::kExtendThis, pts);
								tmpLine->close();
								if(pts.length() != 0)
								{
									LineSp.set(startPoint.x,startPoint.y,startPoint.z);
									LineEp.set(pts[0].x,pts[0].y,endLine->startPoint().z);

									AcDbLine* drawLine = new AcDbLine(LineSp,LineEp);
									drawLine->setLayer(setLayer);
									drawLine->setDatabaseDefaults();

									AcGePoint3d subStartPoint, subNextPoint, subNextdist, subEndPoint;								
									double subDist = _ttof(userDistStr) / (_ttoi(m_EditSubDraw_Str) + 1);
									double tmpdist = 0.0;
									basePLine->getDistAtPoint(startPoint,tmpdist);
									for(int j = 0; j < _ttoi(m_EditSubDraw_Str); j++)
									{
										subPts.removeAll();
										tmpdist += subDist;
										basePLine->getPointAtDist(tmpdist,subStartPoint);
										basePLine->getPointAtDist(tmpdist + subDist,subNextPoint);

										basePLine->getParamAtPoint(subStartPoint,SubcheckParam);
									
										if(floor(checkParam) != floor(SubcheckParam))
										{
											AcGePoint3d tmp_nextPoint;
											basePLine->getPointAtParam(floor(checkParam)+1, tmp_nextPoint);
											vec = (tmp_nextPoint - subStartPoint).normal();
											vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);										
										}
										subtmpPoint.set(subStartPoint.x, subStartPoint.y, 0);
										subtmpPoint2 = subtmpPoint - vec * 100;
										AcDbLine* tmpLine2 = new AcDbLine(subtmpPoint, subtmpPoint2);
										tmpLine2->intersectWith(tmpSubLine, AcDb::kExtendThis, subPts);
										double subLineDist = tmpLine2->startPoint().distanceTo(subPts[0]);
										tmpLine2->close();
										subEndPoint = subStartPoint + vec * (subLineDist / 2);
										SubLineSp.set(subStartPoint.x,subStartPoint.y,startPoint.z);
										SubLineEp.set(subEndPoint.x,subEndPoint.y,(startPoint.z + endLine->startPoint().z) / 2);

										AcDbLine* subLien = new AcDbLine(SubLineSp, SubLineEp);
										subLien->setLayer(setLayer);
										subLien->setDatabaseDefaults();

										AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
										acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);									
										bTR->appendAcDbEntity(subLien);
										subLien->close();
										bTR->close();
									}
									AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
									acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);								
									bTR->appendAcDbEntity(drawLine);
									drawLine->close();
									bTR->close();
								}
								startPoint = nextPoint;
								nextdist += _ttof(userDistStr);								
								if(basePLine->getPointAtDist(nextdist, nextPoint) != Acad::eOk)
								{
									basePLine->getEndPoint(nextPoint);
								}
							}							
							tmpSubLine->close();
							endLine->close();
						}
						else if(pSecondEnt->isKindOf(AcDbPolyline::desc()))	// �ι�° ���� ���������� ���
						{
							AcDbPolyline* endPLine = new AcDbPolyline();
							endPLine = AcDbPolyline::cast(pSecondEnt); // ���� �ι�° ��(z���� ����������)

							AcDbPolyline* tmpPLine = new AcDbPolyline();
							tmpPLine->copyFrom(endPLine);

							double endParam, Pdist, checkParam, checkParam2, SubcheckParam;
							AcGePoint3d startPoint ,nextPoint;

							CString userDistStr;							
							GetDlgItemText(IDC_EDIT_1, userDistStr);

							double nextdist = 0.0;
							nextdist = _ttof(userDistStr);

							basePLine->getEndParam(endParam);
							basePLine->getDistAtParam(endParam,Pdist);

							int loopCnt;
							loopCnt = ceil(Pdist / _ttof(userDistStr)); // ��ǥ�� ����

							basePLine->getStartPoint(startPoint);
							basePLine->getPointAtDist(nextdist, nextPoint);

							tmpPLine->setElevation(0.0);

							AcGePoint3dArray pts, subPts;
							AcGePoint3d tmpPoint, tmpPoint2, subtmpPoint, subtmpPoint2; // �ӽ� ����Ʈ
							AcGePoint3d LineSp, LineEp, SubLineSp, SubLineEp; // ���μ� SP,EP & ���꼱�� SP,EP

							for(int i =0; i < loopCnt+1; i++)
							{
								
								basePLine->getParamAtPoint(startPoint,checkParam);
								basePLine->getParamAtPoint(nextPoint,checkParam2);

								AcGeVector3d vec, distVec;
								if(floor(checkParam) == floor(checkParam2))
								{
									vec = (startPoint - nextPoint).normal();
									vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);								
									distVec = (startPoint - nextPoint).normal();
								}
								else
								{
									AcGePoint3d tmp_nextPoint;
									basePLine->getPointAtParam(floor(checkParam)+1, tmp_nextPoint);
									vec = (tmp_nextPoint - startPoint).normal();
									vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
									distVec = (startPoint - tmp_nextPoint).normal();
								}
								pts.removeAll();
								tmpPoint.set(startPoint.x,startPoint.y,0);								
								tmpPoint2 = tmpPoint - vec * 100;
								AcDbLine* tmpLine = new AcDbLine(tmpPoint, tmpPoint2);
								Acad::ErrorStatus chk;
								chk = tmpLine->intersectWith(tmpPLine, AcDb::kExtendThis, pts);
								/////////////////////// �߰�
								int closePoint = 0;
								closePoint = pts.length();
								double tmp_len1, tmp_len2;
								int index;
								if(closePoint != 0)
								{
									if(closePoint > 1)
									{
										for(int k=0; k < pts.length(); k++)
										{
											if(k == 0)
											{
												tmp_len1 = tmpPoint.distanceTo(pts[k]);
												index = k;
											}
											else
											{
												tmp_len2 = tmpPoint.distanceTo(pts[k]);
												if(tmp_len1 > tmp_len2)
												{
													tmp_len1 = tmp_len2;
													index = k;
												}
											}
										}
									}
									else
									{
										index = 0;
									}
									////////////////////
									tmpLine->close();
									LineSp.set(startPoint.x,startPoint.y,startPoint.z);
									LineEp.set(pts[index].x,pts[index].y,endPLine->elevation());
								
									AcDbLine* drawLine = new AcDbLine(LineSp,LineEp);
									drawLine->setLayer(setLayer);
									drawLine->setDatabaseDefaults();
									AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
									acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);								
									bTR->appendAcDbEntity(drawLine);
									drawLine->close();
									bTR->close();
								}
								AcGePoint3d subStartPoint, subNextPoint, subNextdist, subEndPoint;
								
								double subDist = _ttof(userDistStr) / (_ttoi(m_EditSubDraw_Str) + 1);
								double tmpdist = 0.0;
								basePLine->getDistAtPoint(startPoint,tmpdist);
								for(int j = 0; j < _ttoi(m_EditSubDraw_Str); j++)
								{
									subPts.removeAll();
									tmpdist += subDist;
									basePLine->getPointAtDist(tmpdist,subStartPoint);
									basePLine->getPointAtDist(tmpdist + subDist,subNextPoint);

									basePLine->getParamAtPoint(subStartPoint,SubcheckParam);
									
									if(floor(checkParam) != floor(SubcheckParam))
									{
										AcGePoint3d tmp_nextPoint;
										basePLine->getPointAtParam(floor(checkParam)+1, tmp_nextPoint);
										vec = (tmp_nextPoint - subStartPoint).normal();
										vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);										
									}
									double vec_ang = vec.angleTo(AcGeVector3d::kXAxis);
									subtmpPoint.set(subStartPoint.x, subStartPoint.y, 0);
									subtmpPoint2 = subtmpPoint - vec * 100;
									AcDbLine* tmpLine2 = new AcDbLine(subtmpPoint, subtmpPoint2);
									tmpLine2->intersectWith(tmpPLine, AcDb::kExtendThis, subPts);
									int sub_closePoint = 0;
									sub_closePoint = subPts.length();
									double subtmp_len1, subtmp_len2;
									int subindex;
									AcGePoint3d tmpEndPoint;
									if(sub_closePoint != 0) // �������� ������� �ȱ׸�
									{
										if(sub_closePoint > 1)
										{
											for(int h=0; h < subPts.length(); h++)
											{
												if(h == 0)
												{
													subtmp_len1 = subtmpPoint.distanceTo(subPts[h]);													
													subindex = h;
													tmpEndPoint = subPts[h];
												}
												else
												{
													subtmp_len2 = subtmpPoint.distanceTo(subPts[h]);
													if(subtmp_len1 > subtmp_len2)
													{
														subtmp_len1 = subtmp_len2;
														subindex = h;
														tmpEndPoint = subPts[h];
													}
												}
											}
										}
										else
										{
											subindex = 0;
										}

										SubLineSp.set(subStartPoint.x,subStartPoint.y,startPoint.z);
										SubLineEp.set(subPts[subindex].x,subPts[subindex].y,(startPoint.z + endPLine->elevation()) / 2);

										AcDbLine* tmpsubLine = new AcDbLine(SubLineSp, SubLineEp);
										AcGePoint3d halfEndPoint;
										double subDistance;
										tmpsubLine->getDistAtPoint(tmpsubLine->endPoint(),subDistance);
										subDistance = subDistance * 0.5;
										tmpsubLine->getPointAtDist(subDistance,halfEndPoint);
										tmpsubLine->close();
										
										AcDbLine* subLien = new AcDbLine(SubLineSp, halfEndPoint);
										subLien->setLayer(setLayer);
										subLien->setDatabaseDefaults();

										AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
										acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);									
										bTR->appendAcDbEntity(subLien);
										subLien->close();
										bTR->close();
									}
								}								
								startPoint = nextPoint;
								nextdist += _ttof(userDistStr);								
								if(basePLine->getPointAtDist(nextdist, nextPoint) != Acad::eOk)
								{
									basePLine->getEndPoint(nextPoint);
								}
							}							
							tmpPLine->close();
							endPLine->close();	
						}
						else
						{
							basePLine->close();
							pSecondEnt->close();
							pEnt->close();
							return;
						}						
					}
				}
				basePLine->close();
			}
			else{

			}
		}
		pEnt->close();
	}
}

double CDSLO::GetRadian(double degree){
	double rad = 0;
	rad = degree * 3.141592 / 180;
	return rad;
}

void CDSLO::OnBnClickedCancel(){
	OnCancel();
}

void CDSLO::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;
}

void CDSLO::OnBnClickedBcolor(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();
	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();
	acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor);
	SelectColorChange(selectColor);
}

//���� ����
void CDSLO::SelectColorChange(int colorIndex){
	CString colorIndexStr;
	colorIndexStr.Format(_T("%d"), colorIndex);
	if(colorIndexStr.CompareNoCase(_T("0")) == 0)
		colorIndexStr.Format(_T("By Block"));
	else if(colorIndexStr.CompareNoCase(_T("256")) == 0)
		colorIndexStr.Format(_T("By Layer"));

	btnAfterClr = acedGetRGB(colorIndex);
	m_curColorIndex = colorIndex;

	Invalidate();
}

void CDSLO::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct){
	if((nIDCtl == IDC_BCOLOR)){
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct -> hDC);					//��ư�� dc���ϱ�
		rect = lpDrawItemStruct->rcItem;					//��ư���� ���ϱ�
		dc.Draw3dRect(&rect,RGB(255,255,255),RGB(0,0,0));	//��ư�� �ܰ��� �׸���
		dc.FillSolidRect(&rect, btnAfterClr);				//��ư����
		UINT state = lpDrawItemStruct-> itemState;			//��ư���±��ϱ�
		if((state &ODS_SELECTED))
			dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
		else
			dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);		

		TCHAR buffer[MAX_PATH];								//��ư�� text�� ������� �ӽù���
		ZeroMemory(buffer,MAX_PATH);						//�����ʱ�ȭ
		::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //��ư�� text���
		dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE); //��ư�� text�ֱ�
		dc.Detach();                                                  //��ư�� dc Ǯ���ֱ�
	}
	CAcUiDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDSLO::OnEnUpdateEdit1(){
	UpdateData(TRUE);
	m_EditBaseDraw_Str = CheckInputNumber(m_EditBaseDraw_Str);
	UpdateData(FALSE);
	m_EditBaseDraw_Ctrl.SetSel(0, -1);
	m_EditBaseDraw_Ctrl.SetSel(-1, -1);
	UpdateData(TRUE);
}

//���� �� ��(.) �Է� üũ
CString CDSLO::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	for(int i = 0; i < str.GetLength(); i++){
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i,1);
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46))
			str.Remove(*getAtChar);
	}
	return str;
}

void CDSLO::OnCbnSelchangeCbxlayer(){
	int curLayer = m_cbxLayer.GetCurSel();
	CString layerName;
	m_cbxLayer.GetLBText(m_cbxLayer.GetCurSel(), layerName);
	LayerChangeGetColor(layerName);
}

//�������� ���� ��������
void CDSLO::LayerChangeGetColor(CString layerName){
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);

	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		AcCmColor color;
		int colorIndex;
		pLayerTblRcd->getName(name);
		color = pLayerTblRcd->color();
		colorIndex = color.colorIndex();
		if(layerName.CompareNoCase(name) == 0)
			SelectColorChange(colorIndex);
	}
	delete pLayerTblIter;
	delete pLayerTbl;
}

void CDSLO::OnBnClickedBlayerselect(){
	ads_name en;
	ads_point pt;
	CString strMsg;

	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	strMsg.LoadStringW(IDS_LAYER_SELECT); // "\n���̾ ������ ��ü ����: 
	if (acedEntSel(strMsg, en, pt) == RTNORM) {
		CompleteEditorCommand();

		AcDbObjectId entityId, selectId;
		AcDbEntity* eObj;
		entityId.setFromOldId(en[0]);
		acdbOpenObject(eObj, entityId, AcDb::kForRead);

		selectId = eObj->layerId();
		AcDbLayerTableRecordPointer pSelectLayer(selectId, AcDb::kForRead);

		TCHAR* selectLayerName;
		pSelectLayer->getName(selectLayerName);

		//������ �ҷ�����
		AcDbLayerTable* pLayerTbl = NULL;
		AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
		AcDbLayerTableIterator* pLayerTblIter;
		pLayerPoint->newIterator(pLayerTblIter);

		int cbxIndex = 0;
		AcDbObjectId LayerId;
		for (pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()) {
			pLayerTblIter->getRecordId(LayerId);
			AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
			TCHAR* name;
			pLayerTblRcd->getName(name);
			if (0 == _tcscmp(selectLayerName, name)) {
				m_cbxLayer.SetCurSel(cbxIndex);
				LayerChangeGetColor(selectLayerName);
				break;
			}
			cbxIndex++;
		}
		delete pLayerTblIter;
		delete pLayerTbl;
		OnCbnSelchangeCbxlayer();
	}
	else	CancelEditorCommand();
}

void CDSLO::OnBnClickedClayer()
{
	bool bClayer = IsDlgButtonChecked(IDC_CHECK_CLAYER);
	AcDbObjectId CLayID;

	if (bClayer) {
		CLayID = acdbHostApplicationServices()->workingDatabase()->clayer();
		AcDbLayerTableRecordPointer pClayerTblRcd(CLayID, AcDb::kForRead);
		TCHAR* name;
		pClayerTblRcd->getName(name);
		LayerChangeGetColor(name);
		m_cbxLayer.SetCurSel(m_cbxLayer.FindString(0, name));
	}
}

void CDSLO::OnBnClickedDsloHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/DSLO.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
