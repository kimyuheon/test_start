// DSLO.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "DSLO.h"
#include "afxdialogex.h"

// CDSLO 대화 상자입니다.
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

// CDSLO 메시지 처리기입니다.

BOOL CDSLO::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	
	//도면층 불러오기
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
	
	//설정값 읽기
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
	//현재 도면층 선택
	tempStr = iniFile.readINI(_T("config.ini"), _T("DSLO"), _T("ChkCLayer"));
	//현재 도면층이 아닐 시
	if (tempStr.CompareNoCase(_T("TRUE")) != 0) {
		//레이어 저장
		tempStr = iniFile.readINI(_T("config.ini"), _T("DSLO"), _T("CmbLayer"));
		if (tempStr.CompareNoCase(_T("")) == 0) {
			CmbLayer = _T("0");
		}
		else {
			CmbLayer = tempStr;
		}
		//콤보박스 표기 레이어
		tempStr = iniFile.readINI(_T("config.ini"), _T("DSLO"), _T("cbxLayer"));
		if (tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("-1")) == 0) {
			m_cbxLayer.SetCurSel(0);
			m_cbxLayer.GetLBText(0, CmbLayer);
		}
		else {
			cbxLayerInt_Str = tempStr;
			cbxLayerInt = _ttoi(cbxLayerInt_Str);
			//m_cbxLayer.SetCurSel(cbxLayerInt);
			if (m_cbxLayer.FindStringExact(-1, CmbLayer) == -1) {//레이어가 존재하지 않을 경우
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
	// 현재 도면층 선택 시
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

	strMsg.LoadStringW(IDS_SLOPE_START); // "\n사면 시작선 선택: "
	if(ads_entsel(strMsg, startEn, sp) == RTNORM){
		strMsg.LoadStringW(IDS_SLOPE_END); // "\n사면 끝선 선택: "
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
			if(pEnt->isKindOf(AcDbLine::desc()))	// 처음 선택한 객체가 라인인 경우
			{
				AcDbLine* baseLine = new AcDbLine();
				baseLine = AcDbLine::cast(pEnt);
				AcGePoint3d baseLineSp, baseLineEp;
				baseLineSp.set(baseLine->startPoint().x, baseLine->startPoint().y, baseLine->startPoint().z);	// 김성곤 대리 수정, Z값 0인것을 Z값 가져오도록 수정 (06.25)
				// 첫번째 라인의 start 포인트를 가져옴
				baseLineEp.set(baseLine->endPoint().x, baseLine->endPoint().y, baseLine->endPoint().z);	// 김성곤 대리 수정, Z값 0인것을 Z값 가져오도록 수정 (06.25)
				// 첫번째 라인의 end 포인트를 가져옴

				if(acdbGetObjectId(secondLineId, endEn) == Acad::eOk)
				{
					AcDbEntityPointer pSecondEnt(secondLineId, AcDb::kForRead);
					if(pSecondEnt.openStatus() == Acad::eOk)
					{
						if(pSecondEnt->isKindOf(AcDbLine::desc()))	// 두번째 선택한 객체도 라인일 경우
						{
							AcDbLine* endLine = new AcDbLine();
							endLine = AcDbLine::cast(pSecondEnt); // 두번째 선택한 객체를 endLine에 삽입
							AcGeVector3d vec, distVec;

							AcGePoint3d startPoint ,nextPoint;
							double nextdist = 0.0;
							CString userDistStr;
							GetDlgItemText(IDC_EDIT_1, userDistStr);
							nextdist = _ttof(userDistStr);
							baseLine->getStartPoint(startPoint);
							baseLine->getPointAtDist(nextdist, nextPoint);

							vec = (startPoint - nextPoint).normal();	// 첫번째 라인의 수직 벡터 구함
							distVec = (startPoint - nextPoint).normal(); // 첫번째 라인의 수직 벡터 구함
							vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis); // 수직 포인트 구함

							AcGePoint3d tmpPoint, tmpPoint2;
							double maxDist;
							int loopCnt;

							maxDist = baseLineSp.distanceTo(baseLineEp); // 첫번째 선택 라인의 길이 구함
							loopCnt = maxDist / _ttof(userDistStr); // 주 표시간의 수 
							AcGePoint3d basePoint = baseLineSp;	// basePoint = 첫번째 객체의 start 포인트
							AcGePoint3d tmp_basePoint = baseLineSp; // 첫번째 객체의 start 포인트 백업(07.03 추가)

							AcGeLineSeg3d* testLine = new AcGeLineSeg3d(endLine->startPoint(), endLine->endPoint()); // 3D선분 생성(두번째 객체)
							AcGeLineSeg3d* testSubLine = new AcGeLineSeg3d(baseLine->startPoint(), baseLine->endPoint()); // 두번째 line의 생성할지 유무 확인
							AcGePoint3dArray pts, subPts;
							for(int i = 0; i < loopCnt+1; i++)
							{
								pts.removeAll(); // pts 초기화
								tmpPoint = basePoint - vec * 100;
								basePoint.z = tmpPoint.z = endLine->startPoint().z;  // 알폰씨가 일부 수정(basePoint.z 추가)
								AcDbLine* tmpLine = new AcDbLine(basePoint, tmpPoint);
								tmpLine->intersectWith(endLine, AcDb::kExtendThis, pts); // z값에 따른 교차점을 찾지 못함(확인해야되는 구간)
								tmpLine->close();

								int chk = pts.length();
								if(chk != 0)
								{
									tmp_basePoint.set(basePoint.x,basePoint.y,baseLineSp.z); // 김성곤 대리 추가
									AcDbLine* drawLine = new AcDbLine(tmp_basePoint, pts[0]); // basePoint에서 tmp_basePoint로 수정
									AcDbLine* tmp_drawLine = new AcDbLine(basePoint, pts[0]); // vector를 생성할려고 만든 라인
									drawLine->setLayer(setLayer);
									
									AcGeVector3d subVec;
									subVec = (tmp_drawLine->startPoint() - tmp_drawLine->endPoint()).normal();	// 주 표시라인의 수직벡터를 구하지?
									tmp_drawLine->close();
								
									AcDbBlockTableRecord* bTR = new AcDbBlockTableRecord();
									acdbOpenObject(bTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
									drawLine->setDatabaseDefaults();
									bTR->appendAcDbEntity(drawLine);
									drawLine->close();
									bTR->close();
								}
								AcGePoint3d subStartPoint, subEndPoint, tmpsubStartPoint, tmpsubEndPoint; // tmpsubStartPoint, tmpsubEndPoint 추가;
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
								AcGeLineSeg3d* testSubLine = new AcGeLineSeg3d(baseLine->startPoint(), baseLine->endPoint()); // 두번째 line의 생성할지 유무 확인
								AcGePoint3dArray pts, subPts;
								AcGePoint3d tmpbaseLineSp;
								for(int i = 0; i < loopCnt+1; i++)
								{
									pts.removeAll();
									tmpPoint = basePoint - vec * 100;
									basePoint.z = tmpPoint.z = endLine->startPoint().z;  // 알폰씨가 일부 수정(basePoint.z 추가)
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
										AcGePoint3d subStartPoint, subEndPoint, tmpsubStartPoint, tmpsubEndPoint; // tmpsubStartPoint, tmpsubEndPoint 추가
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
			else if(pEnt->isKindOf(AcDbPolyline::desc())) // 첫번째 객체가 폴리라인인 경우
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
							endLine = AcDbLine::cast(pSecondEnt); // 실제 두번째 선(z값을 가지고있음)

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
							loopCnt = ceil(Pdist / _ttof(userDistStr)); // 주표시 수량

							basePLine->getStartPoint(startPoint);
							basePLine->getPointAtDist(nextdist, nextPoint);

							AcGePoint3dArray pts, subPts;
							AcGePoint3d tmpPoint, tmpPoint2, subtmpPoint, subtmpPoint2; // 임시 포인트
							AcGePoint3d LineSp, LineEp, SubLineSp, SubLineEp; // 메인선 SP,EP & 서브선의 SP,EP
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
						else if(pSecondEnt->isKindOf(AcDbPolyline::desc()))	// 두번째 선이 폴리라인인 경우
						{
							AcDbPolyline* endPLine = new AcDbPolyline();
							endPLine = AcDbPolyline::cast(pSecondEnt); // 실제 두번째 선(z값을 가지고있음)

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
							loopCnt = ceil(Pdist / _ttof(userDistStr)); // 주표시 수량

							basePLine->getStartPoint(startPoint);
							basePLine->getPointAtDist(nextdist, nextPoint);

							tmpPLine->setElevation(0.0);

							AcGePoint3dArray pts, subPts;
							AcGePoint3d tmpPoint, tmpPoint2, subtmpPoint, subtmpPoint2; // 임시 포인트
							AcGePoint3d LineSp, LineEp, SubLineSp, SubLineEp; // 메인선 SP,EP & 서브선의 SP,EP

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
								/////////////////////// 추가
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
									if(sub_closePoint != 0) // 교차점이 없을경우 안그림
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

//색상 변경
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
		dc.Attach(lpDrawItemStruct -> hDC);					//버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;					//버튼영역 구하기
		dc.Draw3dRect(&rect,RGB(255,255,255),RGB(0,0,0));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect, btnAfterClr);				//버튼색상
		UINT state = lpDrawItemStruct-> itemState;			//버튼상태구하기
		if((state &ODS_SELECTED))
			dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
		else
			dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);		

		TCHAR buffer[MAX_PATH];								//버튼의 text를 얻기위한 임시버퍼
		ZeroMemory(buffer,MAX_PATH);						//버퍼초기화
		::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //버튼의 text얻기
		dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE); //버튼의 text넣기
		dc.Detach();                                                  //버튼의 dc 풀어주기
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

//숫자 및 점(.) 입력 체크
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

//도면층별 색상 가져오기
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

	strMsg.LoadStringW(IDS_LAYER_SELECT); // "\n레이어를 가져올 객체 선택: 
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

		//도면층 불러오기
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
