// NSLO.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "NSLO.h"
#include "afxdialogex.h"


// CNSLO 대화 상자입니다.
IMPLEMENT_DYNAMIC(CNSLO, CAcUiDialog)
	CNSLO::CNSLO(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CNSLO::IDD, pParent), m_LineDraw(FALSE)
	, m_ChangeLayer(FALSE)
	, m_AutoDraw(FALSE)
	, m_EditMaxDist_Str(_T("100"))
	, m_EditAutoDraw_Str(_T("100")){
}

CNSLO::~CNSLO(){
}

void CNSLO::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHKDRAWLINE, m_LineDraw);
	DDX_Check(pDX, IDC_CHKCHANGELAYER, m_ChangeLayer);
	DDX_Check(pDX, IDC_CHKAUTODRAW, m_AutoDraw);
	DDX_Control(pDX, IDC_CBXLAYER, m_CbxLayerCtr);
	DDX_Control(pDX, IDC_BSELECT, m_BtnSelect);
	DDX_Control(pDX, IDC_EDTMAXDIST, m_EditMaxDist_Ctrl);
	DDX_Text(pDX, IDC_EDTMAXDIST, m_EditMaxDist_Str);
	DDX_Control(pDX, IDC_EDTAUTODIST, m_EditAutoDraw_Ctrl);
	DDX_Text(pDX, IDC_EDTAUTODIST, m_EditAutoDraw_Str);
}

BEGIN_MESSAGE_MAP(CNSLO, CAcUiDialog)
	ON_STN_CLICKED(IDC_STTBIT1, &CNSLO::OnStnClickedSttbit1)
	ON_STN_CLICKED(IDC_STTBIT2, &CNSLO::OnStnClickedSttbit2)
	ON_STN_CLICKED(IDC_STTBIT3, &CNSLO::OnStnClickedSttbit3)
	ON_BN_CLICKED(IDC_RADIO1, &CNSLO::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CNSLO::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CNSLO::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_CHKDRAWLINE, &CNSLO::OnBnClickedChkdrawline)
	ON_BN_CLICKED(IDC_CHKCHANGELAYER, &CNSLO::OnBnClickedChkchangelayer)
	ON_BN_CLICKED(IDC_CHKAUTODRAW, &CNSLO::OnBnClickedChkautodraw)
	ON_CBN_SELCHANGE(IDC_CBXLAYER, &CNSLO::OnCbnSelchangeCbxlayer)
	ON_BN_CLICKED(IDC_BCOLOR, &CNSLO::OnBnClickedBcolor)
	ON_BN_CLICKED(IDC_BSELECT, &CNSLO::OnBnClickedBselect)
	ON_WM_DRAWITEM()
	ON_EN_UPDATE(IDC_EDTMAXDIST, &CNSLO::OnEnUpdateEdtmaxdist)
	ON_EN_UPDATE(IDC_EDTAUTODIST, &CNSLO::OnEnUpdateEdtautodist)
	ON_BN_CLICKED(IDOK, &CNSLO::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNSLO::OnBnClickedCancel)
END_MESSAGE_MAP()

// CNSLO 메시지 처리기입니다.
BOOL CNSLO::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	//도면층 불러오기
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
		m_CbxLayerCtr.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;

	//변수 및 리소스 초기화
	m_CbxLayerCtr.SetCurSel(0);
	CString curLayerStr;
	m_CbxLayerCtr.GetLBString(m_CbxLayerCtr.GetCurSel(), curLayerStr);
	LayerChangeGetColor(curLayerStr);
	OnCbnSelchangeCbxlayer();
	setLayer = _T("0");
	setColorIdx = 7;
	m_BtnSelect.AutoLoad();

	CString jobStr = iniFile.readINI(_T("config.ini"), _T("NSLO"), _T("JOB"));
	if(jobStr.CompareNoCase(_T("")) == 0 || jobStr.CompareNoCase(_T("0")) == 0){
		CheckDlgButton(IDC_RADIO1, TRUE);
		OnStnClickedSttbit1();
		jobStat = 0;
	}
	else if(jobStr.CompareNoCase(_T("1")) == 0){
		CheckDlgButton(IDC_RADIO2, TRUE);
		OnStnClickedSttbit2();
		jobStat = 1;
	}
	else{
		CheckDlgButton(IDC_RADIO3, TRUE);
		OnStnClickedSttbit3();
		jobStat = 2;
	}

	CString layerChange = iniFile.readINI(_T("config.ini"), _T("NSLO"), _T("LAYERCHANGE"));
	if(layerChange.CompareNoCase(_T("")) == 0 || layerChange.CompareNoCase(_T("TRUE")) == 0){
		CheckDlgButton(IDC_CHKCHANGELAYER, true);
		OnBnClickedChkchangelayer();
	}
	
	CString autoDrawing = iniFile.readINI(_T("config.ini"), _T("NSLO"), _T("AUTODRAW"));
	if(autoDrawing.CompareNoCase(_T("")) == 0 || autoDrawing.CompareNoCase(_T("TRUE")) == 0){
		CheckDlgButton(IDC_CHKAUTODRAW, true);
		OnBnClickedChkautodraw();
	}
	
	CString m_EditAutoDraw_Str = iniFile.readINI(_T("config.ini"), _T("NSLO"), _T("AUTODRAWDISTANCE"));
	if(m_EditAutoDraw_Str.CompareNoCase(_T("")) == 0){
		m_EditAutoDraw_Str.Format(_T("100"));
	}
	SetDlgItemText(IDC_EDTAUTODIST,m_EditAutoDraw_Str);
	UpdateData(FALSE);
	return TRUE; 
}

//블럭 찾기	이미 존재하면 리턴 true
bool CNSLO::FindBlock(CString blkName){
	AcDbBlockTable* pblkTbl;
	if((es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pblkTbl, AcDb::kForRead)) != Acad::eOk)
		return false;

	if(pblkTbl->has(blkName) == Adesk::kTrue){
		AcDbBlockTableIterator* pBTIter;
		if(pblkTbl->newIterator(pBTIter) == Acad::eOk){
			while(!pBTIter->done()){
				AcDbBlockTableRecord *pRecord;
				if(pBTIter->getRecord(pRecord, AcDb::kForRead) == Acad::eOk){				
					ACHAR* temp;
					pRecord->getName(temp);
					if(blkName.CompareNoCase(temp) == 0)
						blkId = pRecord->objectId();					
					pRecord->close();
				}
				pBTIter->step();
			}
		}
		pblkTbl->close();
		return true;
	}
	pblkTbl->close();
	return false;
}

//블록 그리기
void CNSLO::DrawBlock(CString blkName, AcGePoint3d insertPt, double ang, AcGeScale3d blkScale){
	if(!FindBlock(blkName))	MakeBlock(blkName);

	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTRec = NULL;
	AcDbBlockReference *pInsertBlk = new AcDbBlockReference();

	if((es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBT, AcDb::kForWrite)) != Acad::eOk)
	{
		pBT->close();
		pBTRec->close();
		CancelEditorCommand();
	}
	if(pBT->has(blkName) == Adesk::kTrue)
	{
		pInsertBlk->setBlockTableRecord(blkId);
		pInsertBlk->setPosition(insertPt);
		pInsertBlk->setRotation(ang);
		pInsertBlk->setScaleFactors(blkScale);
		pInsertBlk->setLayer(setLayer);
		pInsertBlk->setColorIndex(setColorIdx);
	}
	pBT->getAt(ACDB_MODEL_SPACE, pBTRec, AcDb::kForWrite);
	pBTRec ->appendAcDbEntity(pInsertBlk);
	pBT->close();
	pBTRec ->close();
	pInsertBlk->close();
}

//실행
void CNSLO::OnBnClickedOk(){
	UpdateData(TRUE);
	UpdateData(FALSE);
	ads_point pt1, pt2;
	ads_name en1, en2;
	AcGeVector3d drawVec;
	AcGeVector3d loopVec;
	int rtKey, rtKey2;

	int roopCnt = 0;

	if(jobStat == 0)		iniFile.writeINI(_T("config.ini"), _T("NSLO"), _T("JOB"), _T("0"));
	else if(jobStat == 1)	iniFile.writeINI(_T("config.ini"), _T("NSLO"), _T("JOB"), _T("1"));
	else if(jobStat == 2)	iniFile.writeINI(_T("config.ini"), _T("NSLO"), _T("JOB"), _T("2"));

	if(m_ChangeLayer)	iniFile.writeINI(_T("config.ini"), _T("NSLO"), _T("LAYERCHANGE"), _T("TRUE"));
	else				iniFile.writeINI(_T("config.ini"), _T("NSLO"), _T("LAYERCHANGE"), _T("FALSE"));
	if(m_AutoDraw)		iniFile.writeINI(_T("config.ini"), _T("NSLO"), _T("AUTODRAW"), _T("TRUE"));
	else				iniFile.writeINI(_T("config.ini"), _T("NSLO"), _T("AUTODRAW"), _T("FALSE"));

	iniFile.writeINI(_T("config.ini"), _T("NSLO"), _T("AUTODRAWDISTANCE"), m_EditAutoDraw_Str);

	BeginEditorCommand();
	struct resbuf *userOsnap;
	userOsnap = NULL;
	userOsnap = ads_newrb(RTSHORT);
	ads_getvar(_T("OSMODE"), userOsnap);

	CString autoDistStr;
	GetDlgItemText(IDC_EDTAUTODIST, autoDistStr);
	if(!m_AutoDraw){
		//근처점 스냅 설정
		setSysVar((LPCTSTR)_T("OSMODE"), 512);
		if(RTNORM == acedGetPoint(NULL, _T("사면 하단점 지정 : "), pt1))
		{
			//직교 스냅 설정
			setSysVar((LPCTSTR)_T("OSMODE"), 128);
			if(RTNORM == acedGetPoint(pt1, _T("사면 상단점 지정 : "), pt2))
			{
				AcGePoint3d tmp_pt1, tmp_pt2;
				tmp_pt1.set(asPnt3d(pt1).x, asPnt3d(pt1).y, 0);
				tmp_pt2.set(asPnt3d(pt2).x, asPnt3d(pt2).y, 0);
				drawVec = (tmp_pt1 - tmp_pt2).normal();
				drawVec.rotateBy(GetRadian(0), AcGeVector3d::kZAxis);
				AcGePoint3d p2 = tmp_pt2 + drawVec * 2.0;
				double ang = ads_angle(pt2, asDblArray(p2));

				double inputDist = tmp_pt2.distanceTo(tmp_pt1);
				double inputScale = inputDist / 1000.0;
				AcGeScale3d blkScale(inputScale, inputScale, inputScale);

				if(m_ChangeLayer)
				{
					CString curLayer;
					m_CbxLayerCtr.GetLBString(m_CbxLayerCtr.GetCurSel(), curLayer);
					setLayer = (LPCTSTR)curLayer;
				}

				//Block Name 0: SLOPE, 1: CESLOPE, 2: CESLOPNads_setvar(_T("OSMODE"), userOsnap);
				switch(jobStat)
				{
					ads_setvar(_T("OSMODE"), userOsnap);
				case 0:
					DrawBlock(_T("ZD_SLOPE"), asPnt3d(pt2), ang, blkScale);
					break;
				case 1:
					DrawBlock(_T("ZD_CESLOPE"), asPnt3d(pt2), ang, blkScale);
					break;
				case 2:					
					DrawBlock(_T("ZD_CESLOPN"), asPnt3d(pt2), ang, blkScale);
					break;
				default:
					return;
				}
				CompleteEditorCommand();
				ads_setvar(_T("OSMODE"), userOsnap);
				return;
			}
		}
		else{
			ads_setvar(_T("OSMODE"), userOsnap);
			CancelEditorCommand();
			return;
		}
	}
	else{
		if(m_EditAutoDraw_Str.IsEmpty() || m_EditAutoDraw_Str.CompareNoCase(_T("0")) == 0)
		{
			AfxMessageBox(_T("지정거리가 입력되지 않았습니다."));
			ads_setvar(_T("OSMODE"), userOsnap);
			CancelEditorCommand();
			return;
		}
		if(RTNORM == acedEntSel(_T("\n사면 하단 선 선택: "), en1, pt1))
		{
			if(RTNORM == acedEntSel(_T("\n사면 상단 선 선택: "), en2, pt2))
			{
				AcDbEntity* firstEnt = NULL;
				AcDbEntity* secondEnt = NULL;
				AcDbObjectId firstEntId, secondEntId;
				acdbGetObjectId(firstEntId, en1);
				acdbGetObjectId(secondEntId, en2);
				if(acdbOpenAcDbEntity(firstEnt, firstEntId, AcDb::kForRead) != Acad::eOk)
				{
					firstEnt->close();
					secondEnt->close();
					ads_setvar(_T("OSMODE"), userOsnap);
					return;
				}
				if(acdbOpenAcDbEntity(secondEnt, secondEntId, AcDb::kForRead) != Acad::eOk)
				{
					firstEnt->close();
					secondEnt->close();
					ads_setvar(_T("OSMODE"), userOsnap);
					return;
				}

				AcDbLine* otherLine = new AcDbLine();
				AcDbLine* baseLine = new AcDbLine();
				AcDbPolyline* otherPolyline = new AcDbPolyline();
				AcDbPolyline* basePolyline = new AcDbPolyline();
				AcDb3dPolyline* other3dPolyline = new AcDb3dPolyline();
				AcDb3dPolyline* base3dPolyline = new AcDb3dPolyline();

				//반대 선
				if(firstEnt->isKindOf(AcDbLine::desc()))
				{
					otherLine = AcDbLine::cast(firstEnt);
				}
				else if(firstEnt->isKindOf(AcDbPolyline::desc()))
				{
					otherPolyline = AcDbPolyline::cast(firstEnt);
				}
				else if(firstEnt->isKindOf(AcDb3dPolyline::desc()))
				{
					other3dPolyline = AcDb3dPolyline::cast(firstEnt);
				}
				else
					AfxMessageBox(_T("작업 할 수 없습니다."));
				//기준 선
				if(secondEnt->isKindOf(AcDbLine::desc())){
					baseLine = AcDbLine::cast(secondEnt);
					baseLineStat = 1;
				}
				else if(secondEnt->isKindOf(AcDbPolyline::desc())){
					basePolyline = AcDbPolyline::cast(secondEnt);
					baseLineStat = 2;
				}
				else if(secondEnt->isKindOf(AcDb3dPolyline::desc())){
					base3dPolyline = AcDb3dPolyline::cast(secondEnt);
					baseLineStat = 3;
				}
				else{
					AfxMessageBox(_T("작업 할 수 없습니다."));
					baseLineStat = 4;
				}

				int loopValue;
				// 상단 기준으로 객체 생성(간격도 상단기준)
				if(baseLineStat == 1) // 두번째 선택한 선이 line인 경우
				{
					if(firstEnt->isKindOf(AcDbLine::desc()))
					{
						AcDbLine* tmpFirstLine = new AcDbLine();
						AcGePoint3d lineSp, lineEp;
						AcGePoint3d tmpStartPoint, tmpEndPoint;
						tmpStartPoint.set(otherLine->startPoint().x, otherLine->startPoint().y, 0);
						tmpEndPoint.set(otherLine->endPoint().x, otherLine->endPoint().y, 0);
						tmpFirstLine->setStartPoint(tmpStartPoint);
						tmpFirstLine->setEndPoint(tmpEndPoint);

					
						AcGeVector3d lineVec, drawVec;
						lineSp = baseLine->startPoint();
						lineEp = baseLine->endPoint();
						lineSp.z = lineEp.z = 0;
						double lineDist = lineSp.distanceTo(lineEp);
						lineVec = (lineEp - lineSp).normal();

						loopValue = lineDist / _ttof(m_EditAutoDraw_Str);
						AcGePoint3d loopSp, loopEp;
						AcGePoint3dArray pts;
						for(int i = 0; i < loopValue+1; i++)
						{
							pts.removeAll();
							if(i != 0)	loopSp = lineSp + lineVec * (_ttof(m_EditAutoDraw_Str) * i);
							else		loopSp = lineSp;

							AcGePoint3d tmpPoint;
							drawVec = (lineSp - lineEp).normal();
							drawVec.rotateBy(GetRadian(90), AcGeVector3d::kZAxis);
							tmpPoint = loopSp - drawVec * 100;
						
							AcDbLine* tmpLine = new AcDbLine(loopSp, tmpPoint);
							if(Acad::eOk == tmpLine->intersectWith(tmpFirstLine, AcDb::kExtendBoth, pts))
							{
								loopEp = pts[0];
							}
							else
							{
								tmpPoint = loopSp + drawVec * 100;
								tmpLine->intersectWith(otherLine, AcDb::kExtendBoth, pts);
								loopEp = pts[0];
							}
							
							AcGeLineSeg3d* testLine = new AcGeLineSeg3d(tmpFirstLine->startPoint(), tmpFirstLine->endPoint());
							
							if(testLine->isOn(loopEp) == Adesk::kFalse)
							{
								tmpPoint = loopSp + drawVec * 1000;
								if(testLine->isOn(loopEp) == Adesk::kFalse)
								{
									tmpLine->close();
									delete testLine;
									continue;
								}
							}
							tmpLine->close();
							tmpFirstLine->close();
							delete testLine;
							double ang = ads_angle(asDblArray(loopSp), asDblArray(loopEp));
							double inputDist = loopEp.distanceTo(loopSp);
							double inputScale = inputDist / 1000.0;
							AcGeScale3d blkScale(inputScale, inputScale, inputScale);
							//Block Name 0: SLOPE, 1: CESLOPE, 2: CESLOPN
							switch(jobStat)
							{
								ads_setvar(_T("OSMODE"), userOsnap);
							case 0:
								DrawBlock(_T("ZD_SLOPE"), loopSp, ang, blkScale);
								break;
							case 1:
								DrawBlock(_T("ZD_CESLOPE"), loopSp, ang, blkScale);
								break;
							case 2:
								DrawBlock(_T("ZD_CESLOPN"), loopSp, ang, blkScale);
								break;
							default:
								return;
							}
						}
					}
					else if(firstEnt->isKindOf(AcDbPolyline::desc()))
					{
						// otherPolyline 첫번째 객체 
						// baseLine 두번째 객체

						AcDbPolyline* tmpPolyline = new AcDbPolyline();
						tmpPolyline->copyFrom(otherPolyline);
						tmpPolyline->setElevation(0.0);

						AcGePoint3d lineSp, lineEp;

						AcGeVector3d lineVec, drawVec;
						lineSp = baseLine->startPoint();
						lineEp = baseLine->endPoint();
						lineSp.z = lineEp.z = 0;
						double lineDist = lineSp.distanceTo(lineEp);
						lineVec = (lineEp - lineSp).normal();

						loopValue = lineDist / _ttof(m_EditAutoDraw_Str);
						AcGePoint3d loopSp, loopEp;
						AcGePoint3dArray pts;
						for(int i = 0; i < loopValue+1; i++)
						{
							pts.removeAll();
							if(i != 0)	loopSp = lineSp + lineVec * (_ttof(m_EditAutoDraw_Str) * i);
							else		loopSp = lineSp;

							AcGePoint3d tmpPoint;
							drawVec = (lineSp - lineEp).normal();
							drawVec.rotateBy(GetRadian(90), AcGeVector3d::kZAxis);
							tmpPoint = loopSp - drawVec * 100;
						
							AcDbLine* tmpLine = new AcDbLine(loopSp, tmpPoint);
							if(Acad::eOk == tmpLine->intersectWith(tmpPolyline, AcDb::kExtendBoth, pts))
							{
								loopEp = pts[0];
							}
							else
							{
								tmpPoint = loopSp + drawVec * 100;
								tmpLine->intersectWith(tmpPolyline, AcDb::kExtendBoth, pts);
								loopEp = pts[0];
							}
							// tmpPolyline
							//AcGeLineSeg3d* testLine = new AcGeLineSeg3d(tmpFirstLine->startPoint(), tmpFirstLine->endPoint());
							
							double chk;
							if(tmpPolyline->getParamAtPoint(loopEp,chk) != Acad::eOk)
							{
								tmpPoint = loopSp + drawVec * 1000;
								tmpLine->close();								
								continue;
							}

							tmpLine->close();
							tmpPolyline->close();							
							double ang = ads_angle(asDblArray(loopSp), asDblArray(loopEp));
							double inputDist = loopEp.distanceTo(loopSp);
							double inputScale = inputDist / 1000.0;
							AcGeScale3d blkScale(inputScale, inputScale, inputScale);
							//Block Name 0: SLOPE, 1: CESLOPE, 2: CESLOPN
							switch(jobStat)
							{
								ads_setvar(_T("OSMODE"), userOsnap);
							case 0:
								DrawBlock(_T("ZD_SLOPE"), loopSp, ang, blkScale);
								break;
							case 1:
								DrawBlock(_T("ZD_CESLOPE"), loopSp, ang, blkScale);
								break;
							case 2:
								DrawBlock(_T("ZD_CESLOPN"), loopSp, ang, blkScale);
								break;
							default:
								return;
							}
						}
					}
				}
				else if(baseLineStat == 2) // 두번째 선택한 선이 polyline인 경우
				{
					if(firstEnt->isKindOf(AcDbLine::desc()))
					{
						// otherLine 첫번째 객체가 라인
						// basePolyline 두번째 객체가 폴리라인
						AcDbLine* tmpFirstLine = new AcDbLine();
						AcGePoint3d lineSp, lineEp;
						AcGePoint3d tmpStartPoint, tmpEndPoint;
						tmpStartPoint.set(otherLine->startPoint().x, otherLine->startPoint().y, 0);
						tmpEndPoint.set(otherLine->endPoint().x, otherLine->endPoint().y, 0);
						tmpFirstLine->setStartPoint(tmpStartPoint);
						tmpFirstLine->setEndPoint(tmpEndPoint);
																		
						AcDbPolyline* tmpSPline = new AcDbPolyline();						
						tmpSPline->copyFrom(basePolyline);						
						tmpSPline->setElevation(0.0);

						AcGePoint3d tmpEndpoint;
						tmpSPline->getEndPoint(tmpEndpoint);

						double nextdist = 0.0;
						nextdist = _ttof(m_EditAutoDraw_Str);

						double lineDist;
						tmpSPline->getDistAtPoint(tmpEndpoint, lineDist);
						loopValue = lineDist / _ttof(m_EditAutoDraw_Str);

						AcGePoint3d startPoint ,nextPoint;										
						tmpSPline->getStartPoint(startPoint);
						tmpSPline->getPointAtDist(nextdist, nextPoint);
						AcGePoint3dArray pts;

						AcGePoint3d tmpPoint, tmpPoint2, subtmpPoint, subtmpPoint2; // 임시 포인트
						AcGePoint3d LineSp, LineEp, SubLineSp, SubLineEp; // 메인선 SP,EP & 서브선의 SP,EP
						double checkParam, checkParam2, SubcheckParam;

						for(int i =0; i < loopValue+1; i++)
						{
							tmpSPline->getParamAtPoint(startPoint,checkParam);
							tmpSPline->getParamAtPoint(nextPoint,checkParam2);

							AcGeVector3d vec, distVec;
							if(floor(checkParam) == floor(checkParam2))
							{
								vec = (startPoint - nextPoint).normal();
								vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
							}
							else
							{
								AcGePoint3d tmp_nextPoint;
								tmpSPline->getPointAtParam(floor(checkParam2), tmp_nextPoint);
								vec = (startPoint - tmp_nextPoint).normal();
								vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
							}
							pts.removeAll();
							tmpPoint.set(startPoint.x,startPoint.y,startPoint.z);
							tmpPoint2 = tmpPoint - vec * 100;
							AcDbLine* tmpLine = new AcDbLine(tmpPoint, tmpPoint2);
							if(tmpLine->intersectWith(tmpFirstLine, AcDb::kExtendThis, pts) == Acad::eOk)
							
							if(pts.length() != 0)
							{
								LineSp.set(startPoint.x,startPoint.y,0);
								LineEp.set(pts[0].x,pts[0].y,0);
						
								double ang = ads_angle(asDblArray(LineSp), asDblArray(LineEp));
								double inputDist = LineEp.distanceTo(LineSp);
								double inputScale = inputDist / 1000.0;
								AcGeScale3d blkScale(inputScale, inputScale, inputScale);
								//Block Name 0: SLOPE, 1: CESLOPE, 2: CESLOPN
								switch(jobStat)
								{
									ads_setvar(_T("OSMODE"), userOsnap);
								case 0:
									DrawBlock(_T("ZD_SLOPE"), LineSp, ang, blkScale);
									break;
								case 1:
									DrawBlock(_T("ZD_CESLOPE"), LineSp, ang, blkScale);
									break;
								case 2:
									DrawBlock(_T("ZD_CESLOPN"), LineSp, ang, blkScale);
									break;
								default:
									return;
								}
							}
							tmpLine->close();
							startPoint = nextPoint;
							nextdist += _ttof(m_EditAutoDraw_Str);							
							if(tmpSPline->getPointAtDist(nextdist, nextPoint) != Acad::eOk)
							{
								tmpSPline->getEndPoint(nextPoint);
							}
						}
						tmpFirstLine->close();
						tmpSPline->close();
					}
					else if(firstEnt->isKindOf(AcDbPolyline::desc()))
					{
						// otherPolyline 첫번째 객체가 폴리라인
						// basePolyline 두번째 객체가 폴리라인
						AcDbPolyline* tmpFPline = new AcDbPolyline();
						AcDbPolyline* tmpSPline = new AcDbPolyline();
						tmpFPline->copyFrom(otherPolyline);
						tmpSPline->copyFrom(basePolyline);

						tmpFPline->setElevation(0.0);
						tmpSPline->setElevation(0.0);

						AcGePoint3d tmpEndpoint;
						tmpSPline->getEndPoint(tmpEndpoint);

						double nextdist = 0.0;
						nextdist = _ttof(m_EditAutoDraw_Str);

						double lineDist;
						tmpSPline->getDistAtPoint(tmpEndpoint, lineDist);
						loopValue = lineDist / _ttof(m_EditAutoDraw_Str);

						AcGePoint3d startPoint ,nextPoint;										
						tmpSPline->getStartPoint(startPoint);
						tmpSPline->getPointAtDist(nextdist, nextPoint);
						AcGePoint3dArray pts;

						AcGePoint3d tmpPoint, tmpPoint2, subtmpPoint, subtmpPoint2; // 임시 포인트
						AcGePoint3d LineSp, LineEp, SubLineSp, SubLineEp; // 메인선 SP,EP & 서브선의 SP,EP
						double checkParam, checkParam2, SubcheckParam;

						for(int i =0; i < loopValue+1; i++)
						{
							tmpSPline->getParamAtPoint(startPoint,checkParam);
							tmpSPline->getParamAtPoint(nextPoint,checkParam2);
							
							AcGeVector3d vec, distVec;
							if(floor(checkParam) == floor(checkParam2))
							{
								vec = (startPoint - nextPoint).normal();
								vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
							}
							else
							{
								AcGePoint3d tmp_nextPoint;
								tmpSPline->getPointAtParam(floor(checkParam)+1, tmp_nextPoint);
								vec = (tmp_nextPoint - startPoint).normal();
								vec.rotateBy(90 * 3.141592 / 180, AcGeVector3d::kZAxis);
							}
							
							pts.removeAll();
							tmpPoint.set(startPoint.x,startPoint.y,startPoint.z);
							tmpPoint2 = tmpPoint - vec * 100;
							AcDbLine* tmpLine = new AcDbLine(tmpPoint, tmpPoint2);
							tmpLine->intersectWith(tmpFPline, AcDb::kExtendThis, pts);
							tmpLine->close();

							int closePoint = 0;
							closePoint = pts.length();
							double subtmp_len1, subtmp_len2;
							int index;
							if(closePoint != 0) // 교차점이 없을경우 안그림
							{
								if(closePoint > 1)
								{
									for(int h=0; h < pts.length(); h++)
									{
										if(h == 0)
										{
											subtmp_len1 = tmpPoint.distanceTo(pts[h]);													
											index = h;
										}
										else
										{
											subtmp_len2 = tmpPoint.distanceTo(pts[h]);
											if(subtmp_len1 > subtmp_len2)
											{
												subtmp_len1 = subtmp_len2;
												index = h;
											}
										}
									}
								}
								else
								{
									index = 0;
								}
							
								
								LineSp.set(startPoint.x,startPoint.y,0);
								LineEp.set(pts[index].x,pts[index].y,0);
								
								double ang = acutAngle(asDblArray(LineSp), asDblArray(LineEp));
								double inputDist = LineEp.distanceTo(LineSp);
								double inputScale = inputDist / 1000.0;
								AcGeScale3d blkScale(inputScale, inputScale, inputScale);
								//Block Name 0: SLOPE, 1: CESLOPE, 2: CESLOPN
								switch(jobStat)
								{
									ads_setvar(_T("OSMODE"), userOsnap);
								case 0:
									DrawBlock(_T("ZD_SLOPE"), LineSp, ang, blkScale);
									break;
								case 1:
									DrawBlock(_T("ZD_CESLOPE"), LineSp, ang, blkScale);
									break;
								case 2:
									DrawBlock(_T("ZD_CESLOPN"), LineSp, ang, blkScale);
									break;
								default:
									return;
								}
							}
							startPoint = nextPoint;
							nextdist += _ttof(m_EditAutoDraw_Str);							
							if(tmpSPline->getPointAtDist(nextdist, nextPoint) != Acad::eOk)
							{
								tmpSPline->getEndPoint(nextPoint);
							}
						}
						tmpFPline->close();
						tmpSPline->close();
					}
				}
				else if(baseLineStat == 3) // 두번째 선이 3dPolyline인 경우
				{
					AcGePoint3d lineSp, lineEp;
					AcGeVector3d lineVec, drawVec;
					base3dPolyline->getStartPoint(lineSp);
					base3dPolyline->getEndPoint(lineEp);

					basePolyline->getStartPoint(lineSp);
					basePolyline->getEndPoint(lineEp);

					double lineDist;
					basePolyline->getDistAtPoint(lineEp, lineDist);

					loopValue = lineDist / _ttof(m_EditAutoDraw_Str);
					AcGePoint3d loopSp, loopEp;
					AcGePoint3dArray pts;

					int curVerts = 0;
					double curLineDist, sumDist;
					AcGePoint3d otherLineSp, otherLineEp;
					for(int i = 0; i < loopValue+1; i++){
						if(i == 0){
							basePolyline->getPointAt(curVerts, lineSp);
						}
						else{
							basePolyline->getPointAtDist((_ttof(m_EditAutoDraw_Str) * i), lineSp);
						}

						if(Acad::eOk != basePolyline->getPointAt(curVerts + 1, lineEp)){
							AfxMessageBox(_T("작업 불가능 객체"));
							break;
						}

						otherPolyline->getPointAt(curVerts, otherLineSp);
						otherPolyline->getPointAt(curVerts + 1, otherLineEp);

						basePolyline->getDistAtPoint(lineEp, curLineDist);
						lineVec = (lineEp - lineSp).normal();

						if(i == 0){
							loopSp = lineSp;
							sumDist = curLineDist;
						}
						else{
							if(sumDist < _ttof(m_EditAutoDraw_Str) * i){
								curVerts++;
								sumDist = sumDist + curLineDist;
								continue;
							}
							loopSp = lineSp;
						}

						pts.removeAll();
						AcGePoint3d tmpPoint;
						drawVec = (lineSp - lineEp).normal();
						drawVec.rotateBy(GetRadian(90), AcGeVector3d::kZAxis);
						tmpPoint = loopSp - drawVec * 100;

						AcDbLine* tmpLine = new AcDbLine(loopSp, tmpPoint);
						tmpLine->setColorIndex(4);
						if(Acad::eOk == tmpLine->intersectWith(otherPolyline, AcDb::kExtendBoth, pts)){
							loopEp = pts[0];
						}
						else{
							tmpPoint = loopSp + drawVec * 100;
							tmpLine->intersectWith(otherPolyline, AcDb::kExtendBoth, pts);
							loopEp = pts[0];
						}

						AcGeLineSeg3d* testLine = new AcGeLineSeg3d(otherLineSp, otherLineEp);
						if(testLine->isOn(loopEp) == Adesk::kFalse){
							tmpPoint = loopSp + drawVec * 1000;
							if(testLine->isOn(loopEp) == Adesk::kFalse){
								tmpLine->close();
								delete testLine;
								continue;
							}
						}
						tmpLine->close();
						delete testLine;
						double ang = ads_angle(asDblArray(loopSp), asDblArray(loopEp));
						double inputDist = loopEp.distanceTo(loopSp);
						double inputScale = inputDist / 1000.0;
						AcGeScale3d blkScale(inputScale, inputScale, inputScale);
						switch(jobStat){
							ads_setvar(_T("OSMODE"), userOsnap);
						case 0:							
							DrawBlock(_T("ZD_SLOPE"), loopSp, ang, blkScale);
							break;
						case 1:							
							DrawBlock(_T("ZD_CESLOPE"), loopSp, ang, blkScale);
							break;
						case 2:
							DrawBlock(_T("ZD_CESLOPN"), loopSp, ang, blkScale);
							break;
						default:
							return;
						}
					}
				}
				otherLine->close();
				otherPolyline->close();
				other3dPolyline->close();
				baseLine->close();
				basePolyline->close();
				base3dPolyline->close();
				firstEnt->close();
				secondEnt->close();
				ads_setvar(_T("OSMODE"), userOsnap);
			}
		}	
	}
	CompleteEditorCommand();
	ads_setvar(_T("OSMODE"), userOsnap);
	CAcUiDialog::OnOK();
}

//종료
void CNSLO::OnBnClickedCancel(){
	//CAcUiDialog::OnCancel();
	OnCancel();
}

//블록이 없으면 만들기
Acad::ErrorStatus CNSLO::MakeBlock(CString blkName){
	//Block Name 1: SLOPE, 2: CESLOPE, 3: CESLOPN
	AcDbBlockTable *pBT;
	AcDbObjectId entityId;

	if((es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBT, AcDb::kForWrite)) != Acad::eOk){
		pBT->close() ;
		AfxMessageBox(_T("오픈 에러"));
		return es;
	}

	if(pBT->has(blkName) == Adesk::kTrue){
		pBT->close();
		return Acad::eNotInBlock;
	}

	AcDbBlockTableRecord* insBlkRec = new AcDbBlockTableRecord;
	insBlkRec->setName(blkName);
	insBlkRec->setOrigin(AcGePoint3d::kOrigin);

	if((es = pBT->add(insBlkRec)) != Acad::eOk){
		pBT->close();
		delete insBlkRec;
		return es;
	}
	pBT->close();

	if(blkName.CompareNoCase(_T("ZD_SLOPE")) == 0){
		DrawSlopeLine(insBlkRec, AcGePoint3d(FLDIST, 500, 0), AcGePoint3d(FLDIST, -500, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(FLDIST * 2, 500 - FLCNT, 0), AcGePoint3d(FLDIST * 2, -500 + FLCNT, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(FLDIST * 3, 500 - FLCNT * 2, 0), AcGePoint3d(FLDIST * 3, -500 + FLCNT * 2, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(FLDIST * 4, 500 - FLCNT * 3, 0), AcGePoint3d(FLDIST * 4, -500 + FLCNT * 3, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(FLDIST * 5, 500 - FLCNT * 3 - FLLAST, 0), AcGePoint3d(FLDIST * 5, -500 + FLCNT * 3 + FLLAST, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(FLDIST * 5, 0, 0), AcGePoint3d(1000, 0, 0));
	}
	else if(blkName.CompareNoCase(_T("ZD_CESLOPE")) == 0){
		DrawSlopeLine(insBlkRec, AcGePoint3d(21.9, -476.7, 0), AcGePoint3d(30.9, 412.6, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(30.9, 412.6, 0), AcGePoint3d(32.7, 449, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(32.7, 449, 0), AcGePoint3d(48.9, 458.6, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(48.9, 458.6, 0), AcGePoint3d(56.9, 445.9, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(56.9, 445.9, 0), AcGePoint3d(58.1, -355.8, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(58.1, -355.8, 0), AcGePoint3d(62.3, -372.3, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(62.3, -372.3, 0), AcGePoint3d(72.9, -378, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(72.9, -378, 0), AcGePoint3d(80.7, -368.8, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(80.7, -368.8, 0), AcGePoint3d(88.6, 316.7, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(88.6, 316.7, 0), AcGePoint3d(97.4, 338.9, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(97.4, 338.9, 0), AcGePoint3d(101.9, 338.9, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(101.9, 338.9, 0), AcGePoint3d(115.2, 321.2, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(115.2, 321.2, 0), AcGePoint3d(117.4, -234.4, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(117.4, -234.4, 0), AcGePoint3d(128.5, -245.5, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(128.5, -245.5, 0), AcGePoint3d(141.3, -237.6, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(141.3, -237.6, 0), AcGePoint3d(149.7, 184.9, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(149.7, 184.9, 0), AcGePoint3d(161.4, 204.3, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(161.4, 204.3, 0), AcGePoint3d(171.8, 195.3, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(171.8, 195.3, 0), AcGePoint3d(179.4, -36.5, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(179.4, -36.5, 0), AcGePoint3d(184.5, -61.8, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(184.5, -61.8, 0), AcGePoint3d(198.7, -67.6, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(198.7, -67.6, 0), AcGePoint3d(203, -58.5, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(203, -58.5, 0), AcGePoint3d(216.5, 55.4, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(216.5, 55.4, 0), AcGePoint3d(228.4, 65.4, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(228.4, 65.4, 0), AcGePoint3d(237.3, 56.6, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(237.3, 56.6, 0), AcGePoint3d(248.1, -15.8, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(248.1, -15.8, 0), AcGePoint3d(253, -32.3, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(253, -32.3, 0), AcGePoint3d(265.7, -26.5, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(265.7, -26.5, 0), AcGePoint3d(275.5, 5, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(275.5, 5, 0), AcGePoint3d(296.2, 10.6, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(296.2, 10.6, 0), AcGePoint3d(315.6, -2, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(315.6, -2, 0), AcGePoint3d(351.9, -7.3, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(351.9, -7.3, 0), AcGePoint3d(400.7, 0, 0));
		DrawSlopeLine(insBlkRec, AcGePoint3d(400.7, 0, 0), AcGePoint3d(1000, 0, 0));
	}
	else if(blkName.CompareNoCase(_T("ZD_CESLOPN")) == 0){
		DrawSlopePLine(insBlkRec, AcGePoint3d(FLDIST, 500, 0), AcGePoint3d(FLDIST, -500, 0));
		DrawSlopePLine(insBlkRec, AcGePoint3d(FLDIST * 2, 500 - FLCNT, 0), AcGePoint3d(FLDIST * 2, -500 + FLCNT, 0));
		DrawSlopePLine(insBlkRec, AcGePoint3d(FLDIST * 3, 500 - FLCNT * 2, 0), AcGePoint3d(FLDIST * 3, -500 + FLCNT * 2, 0));
		DrawSlopePLine(insBlkRec, AcGePoint3d(FLDIST * 4, 500 - FLCNT * 3, 0), AcGePoint3d(FLDIST * 4, -500 + FLCNT * 3, 0));
		DrawSlopePLine(insBlkRec, AcGePoint3d(FLDIST * 5, 500 - FLCNT * 3 - FLLAST, 0), AcGePoint3d(FLDIST * 5, -500 + FLCNT * 3 + FLLAST, 0));
		DrawSlopePLine(insBlkRec, AcGePoint3d(FLDIST * 5, 0, 0), AcGePoint3d(1000, 0, 0));
	}	

	blkId = insBlkRec->id();
	insBlkRec->close();

	return Acad::eOk;
}

//라인 그리기
void CNSLO::DrawSlopeLine(AcDbBlockTableRecord* pBlockTableRecord, AcGePoint3d sp, AcGePoint3d ep){
	AcDbLine* drawingLine = new AcDbLine(sp, ep);

	drawingLine->setLayer(0);
	drawingLine->setColorIndex(0);

	if((es = pBlockTableRecord->appendAcDbEntity(drawingLine)) != Acad::eOk){
		delete drawingLine;
		pBlockTableRecord->erase();
		pBlockTableRecord->close();
		return;
	}
	drawingLine->close();
}

//폴리라인 그리기
void CNSLO::DrawSlopePLine(AcDbBlockTableRecord* pBlockTableRecord, AcGePoint3d sp, AcGePoint3d ep){
	AcDbPolyline* drawingpLine = new AcDbPolyline();

	drawingpLine->setLayer(0);
	drawingpLine->addVertexAt(0, AcGePoint2d(sp.x, sp.y), 0, FLDIST, FLDIST);
	drawingpLine->addVertexAt(1, AcGePoint2d(ep.x, ep.y), 0, FLDIST, FLDIST);
	drawingpLine->setColorIndex(0);

	if((es = pBlockTableRecord->appendAcDbEntity(drawingpLine)) != Acad::eOk){
		delete drawingpLine;
		pBlockTableRecord->erase();
		pBlockTableRecord->close();
		return;
	}
	drawingpLine->close();
}

//이미지 1 클릭시
void CNSLO::OnStnClickedSttbit1(){
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1);
	pButton->SetCheck(true);
	pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(false);
	pButton = (CButton*)GetDlgItem(IDC_RADIO3);
	pButton->SetCheck(false);
	jobStat = 0;
}

void CNSLO::OnBnClickedRadio1(){
	jobStat = 0;
}

//이미지 2 클릭시
void CNSLO::OnStnClickedSttbit2(){
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(true);
	pButton = (CButton*)GetDlgItem(IDC_RADIO1);
	pButton->SetCheck(false);
	pButton = (CButton*)GetDlgItem(IDC_RADIO3);
	pButton->SetCheck(false);
	jobStat = 1;
}

void CNSLO::OnBnClickedRadio2(){
	jobStat = 1;
}

//이미지 3 클릭시
void CNSLO::OnStnClickedSttbit3(){
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO3);
	pButton->SetCheck(true);
	pButton = (CButton*)GetDlgItem(IDC_RADIO1);
	pButton->SetCheck(false);
	pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(false);
	jobStat = 2;
}

void CNSLO::OnBnClickedRadio3(){
	jobStat = 2;
}

//선객체로 그리기 체크시
void CNSLO::OnBnClickedChkdrawline(){
	m_LineDraw = IsDlgButtonChecked(IDC_CHKDRAWLINE);

	if(m_LineDraw){
		GetDlgItem(IDC_STTWIDTH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDTMAXDIST)->ShowWindow(SW_SHOW);
	}
	else{		
		GetDlgItem(IDC_STTWIDTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDTMAXDIST)->ShowWindow(SW_HIDE);
	}
}

//도면층 변경 체크시
void CNSLO::OnBnClickedChkchangelayer(){
	m_ChangeLayer = IsDlgButtonChecked(IDC_CHKCHANGELAYER);

	if(m_ChangeLayer){
		GetDlgItem(IDC_BSELECT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CBXLAYER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BCOLOR)->ShowWindow(SW_SHOW);
	}
	else{
		GetDlgItem(IDC_BSELECT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CBXLAYER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BCOLOR)->ShowWindow(SW_HIDE);
	}
}

//지정거리 자동그리기 체크시
void CNSLO::OnBnClickedChkautodraw(){
	m_AutoDraw = IsDlgButtonChecked(IDC_CHKAUTODRAW);
	if(m_AutoDraw)	GetDlgItem(IDC_EDTAUTODIST)->ShowWindow(SW_SHOW);
	else			GetDlgItem(IDC_EDTAUTODIST)->ShowWindow(SW_HIDE);
}

//도면층 변경시
void CNSLO::OnCbnSelchangeCbxlayer(){
	int curLayer = m_CbxLayerCtr.GetCurSel();
	CString layerName;

	m_CbxLayerCtr.GetLBText(m_CbxLayerCtr.GetCurSel(), layerName);
	LayerChangeGetColor(layerName);
}

//색상 변경
void CNSLO::SelectColorChange(int colorIndex){
	CString colorIndexStr;
	colorIndexStr.Format(_T("%d"), colorIndex);
	if(colorIndexStr.CompareNoCase(_T("0")) == 0)			colorIndexStr.Format(_T("블록별"));	
	else if(colorIndexStr.CompareNoCase(_T("256")) == 0)	colorIndexStr.Format(_T("도면층별"));	

	btnAfterClr = acedGetRGB(colorIndex);
	setColorIdx = colorIndex;
	Invalidate();
}

//도면층별 색상 가져오기
void CNSLO::LayerChangeGetColor(CString layerName){
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
		if(layerName.CompareNoCase(name) == 0){
			SelectColorChange(colorIndex);
		}
	}
	delete pLayerTblIter;
	delete pLayerTbl;
}

//색상 클릭시
void CNSLO::OnBnClickedBcolor(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();

	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();

	acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor);
	SelectColorChange(selectColor);
}

//버튼 색상 입히기
void CNSLO::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct){
	if((nIDCtl == IDC_BCOLOR)){
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct -> hDC);					//버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;					//버튼영역 구하기
		dc.Draw3dRect(&rect,RGB(255,255,255),RGB(0,0,0));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect,btnAfterClr);				//버튼색상
		UINT state = lpDrawItemStruct-> itemState;			//버튼상태구하기
		if((state &ODS_SELECTED))	dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
		else						dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);

		TCHAR buffer[MAX_PATH];                            //버튼의 text를 얻기위한 임시버퍼
		ZeroMemory(buffer,MAX_PATH);                       //버퍼초기화
		::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //버튼의 text얻기
		dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE); //버튼의 text넣기
		dc.Detach();                                                  //버튼의 dc 풀어주기
	}
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//객체 선택시
void CNSLO::OnBnClickedBselect(){
	ads_name en;
	ads_point pt;

	BeginEditorCommand();
	if(acedEntSel(_T("\n도면층을 가져올 객체 선택: "), en, pt) == RTNORM){
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
		for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
			pLayerTblIter->getRecordId(LayerId);
			AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
			TCHAR* name;
			pLayerTblRcd->getName(name);
			if(0 == _tcscmp(selectLayerName, name)){
				m_CbxLayerCtr.SetCurSel(cbxIndex);
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

//사면 폭 제한 입력시
void CNSLO::OnEnUpdateEdtmaxdist(){
	UpdateData(TRUE);
	m_EditMaxDist_Str = CheckInputNumber(m_EditMaxDist_Str);
	UpdateData(FALSE);

	m_EditMaxDist_Ctrl.SetSel(0, -1);
	m_EditMaxDist_Ctrl.SetSel(-1, -1);
	UpdateData(TRUE);
}

//지정거리 입력시
void CNSLO::OnEnUpdateEdtautodist(){
	UpdateData(TRUE);
	m_EditAutoDraw_Str = CheckInputNumber(m_EditAutoDraw_Str);
	UpdateData(FALSE);

	m_EditAutoDraw_Ctrl.SetSel(0, -1);
	m_EditAutoDraw_Ctrl.SetSel(-1, -1);
	UpdateData(TRUE);
}

//숫자 및 점(.) 입력 체크
CString CNSLO::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	for(int i = 0; i < str.GetLength(); i++){
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i,1);
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46))	str.Remove(*getAtChar);
	}
	return str;
}

Acad::ErrorStatus CNSLO::setSysVar(LPCTSTR varName, int val){
	ASSERT(varName != NULL);

	resbuf rb;
	rb.restype = RTSHORT;
	rb.resval.rint = (short)val;

	return(setVar(varName, &rb));
}

Acad::ErrorStatus CNSLO::setVar(LPCTSTR varName, const resbuf* newVal){
	CString strResource;
	CString str;
	if(ads_setvar(varName, newVal) != RTNORM) {
		strResource.LoadStringW(IDS_STRING116);		// "Could not set system variable \"%s\"."
		str.Format(strResource, varName);//ArxDbgUtils::stopAlertBox(str);
		return(Acad::eInvalidInput);
	}else{
		return(Acad::eOk);
	}
}

//라디안 구하기
double CNSLO::GetRadian(double degree){
	double rad = 0;
	rad = degree * 3.141592 / 180;
	return rad;
}