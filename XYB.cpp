#include "stdafx.h"
#include "XYB.h"
#include "afxdialogex.h"

// CXYB 대화 상자입니다.
IMPLEMENT_DYNAMIC(CXYB, CDialogEx)
	CXYB::CXYB(CWnd* pParent)
	: CAcUiDialog(CXYB::IDD, pParent)
	, m_xScaleStr(_T(""))
	, m_yScaleStr(_T("")){
}

CXYB::~CXYB(){
}

void CXYB::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDTX, m_xScaleStr);
	DDX_Text(pDX, IDC_EDTY, m_yScaleStr);
	DDX_Control(pDX, IDC_EDTX, m_xScaleCtrl);
	DDX_Control(pDX, IDC_EDTY, m_yScaleCtrl);
}

BEGIN_MESSAGE_MAP(CXYB, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CXYB::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CXYB::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHKEXPLODE, &CXYB::OnBnClickedChkexplode)
END_MESSAGE_MAP()

//초기화
BOOL CXYB::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	CString scaleXStr = iniFile.readINI(_T("config.ini"), _T("XYB"), _T("XSCALE"));
	if(scaleXStr.CompareNoCase(_T("")) == 0)
		scaleXStr = _T("1");
	SetDlgItemText(IDC_EDTX, scaleXStr);

	CString scaleYStr = iniFile.readINI(_T("config.ini"), _T("XYB"), _T("YSCALE"));
	if(scaleYStr.CompareNoCase(_T("")) == 0)
		scaleYStr = _T("1");
	SetDlgItemText(IDC_EDTY, scaleYStr);

	CString explodeStr = iniFile.readINI(_T("config.ini"), _T("XYB"), _T("EXPLODE"));

	if(explodeStr.CompareNoCase(_T("")) == 0 || explodeStr.CompareNoCase(_T("TRUE")) == 0)
		CheckDlgButton(IDC_CHKEXPLODE, TRUE);
	else
		CheckDlgButton(IDC_CHKEXPLODE, FALSE);

	return TRUE;
}

void CXYB::OnBnClickedOk(){
	UpdateData(TRUE);
	UpdateData(FALSE);
	BeginEditorCommand();

	CTime curTime = CTime::GetCurrentTime();
	blkName.Format(_T("%d%d%d_%d%d%d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());

	ads_name en;
	// [+] SUP-29742
	// long	len;
	// [-] END
	Adesk::Int32 len;
	if(acedSSGet(NULL, NULL, NULL, NULL, en) != RTNORM){
		CompleteEditorCommand();
		return;
	}
	if(acedSSLength(en, &len) != RTNORM){
		acedSSFree(en);
		CompleteEditorCommand();
		return;
	}

	ads_point basePoint;
	if(acedGetPoint(NULL, _T("\n기준점 지정: "), basePoint) != RTNORM){
		acedSSFree(en);
		CompleteEditorCommand();
	}

	AcDbBlockTable *pBlockTable = NULL;
	if(acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead) != Acad::eOk){
		pBlockTable->close();
		AfxMessageBox(_T("오픈 에러"));
		return;
	}

	AcDbBlockTableRecord* pBlockRecord = new AcDbBlockTableRecord;
	pBlockRecord->setName(blkName);	
	pBlockRecord->setOrigin(asPnt3d(basePoint));
	pBlockRecord->setBlockInsertUnits(AcDb::kUnitsMillimeters);

	if(pBlockTable->upgradeOpen() != Acad::eOk){
		delete pBlockRecord;
		pBlockTable->close();
		return;
	}	
	if(pBlockTable->add(pBlockRecord) != Acad::eOk){
		pBlockTable->close();
		delete pBlockRecord;
		return;
	}
	pBlockTable->close();

	AcDbEntity* pEnt;
	for(int i = 0; i < len; i++){
		ads_name getEn;
		acedSSName(en, i, getEn);
		AcDbObjectId objId;
		AcDbObjectId newEntId;
		acdbGetObjectId(objId, getEn);
		ErrorStatus es;
		if(acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite) == Acad::eOk){
			if(pEnt->isKindOf(AcDbText::desc())){
				AcDbText* pText = new AcDbText();
				pText->copyFrom(pEnt);
				pText->setDatabaseDefaults();
				pBlockRecord->appendAcDbEntity(pText);
				pText->close();
			}
			if(pEnt->isKindOf(AcDbMText::desc())){
				AcDbMText* pMtext = new AcDbMText();
				pMtext->copyFrom(pEnt);
				pMtext->setDatabaseDefaults();
				pBlockRecord->appendAcDbEntity(pMtext);	
				pMtext->close();
			}
			if(pEnt->isKindOf(AcDbLine::desc())){
				AcDbLine* pLine = new AcDbLine();
				pLine->copyFrom(pEnt);
				pLine->setDatabaseDefaults();
				pBlockRecord->appendAcDbEntity(pLine);
				pLine->close();
			}
			if(pEnt->isKindOf(AcDbPolyline::desc())){
				AcDbPolyline* pPoly = new AcDbPolyline();
				pPoly->copyFrom(pEnt);
				pPoly->setDatabaseDefaults();
				pBlockRecord->appendAcDbEntity(pPoly);
				pPoly->close();
			}
			if(pEnt->isKindOf(AcDbCircle::desc())){
				AcDbCircle* pCircle = new AcDbCircle();
				pCircle->copyFrom(pEnt);
				pCircle->setDatabaseDefaults();
				pBlockRecord->appendAcDbEntity(pCircle);
				pCircle->close();
			}
			if(pEnt->isKindOf(AcDbArc::desc())){
				AcDbArc* pArc = new AcDbArc();
				pArc->copyFrom(pEnt);
				pArc->setDatabaseDefaults();
				pBlockRecord->appendAcDbEntity(pArc);
				pArc->close();
			}
			if(pEnt->isKindOf(AcDbRegion::desc())){
				AcDbRegion* pReg = new AcDbRegion();
				pReg->copyFrom(pEnt);
				pReg->setDatabaseDefaults();
				pBlockRecord->appendAcDbEntity(pReg);
				pReg->close();
			}
			if(pEnt->isKindOf(AcDbBlockReference::desc())){
				AcDbBlockReference* pBlk = new AcDbBlockReference();
				pBlk->copyFrom(pEnt);
				pBlk->setDatabaseDefaults();
				pBlockRecord->appendAcDbEntity(pBlk);
				pBlk->close();
			}
			pEnt->erase();
		}
		pEnt->close();
	}
	pBlockRecord->close();
	InsertBlock(basePoint);
	acedSSFree(en);	
	CAcUiDialog::OnOK();

	iniFile.writeINI(_T("config.ini"), _T("XYB"), _T("XSCALE"), m_xScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("XYB"), _T("YSCALE"), m_yScaleStr);

	if(IsDlgButtonChecked(IDC_CHKEXPLODE) == 1)
		iniFile.writeINI(_T("config.ini"), _T("XYB"), _T("EXPLODE"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("XYB"), _T("EXPLODE"), _T("FAlSE"));
}

void CXYB::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

void CXYB::OnBnClickedChkexplode(){
}

void CXYB::InsertBlock(ads_point bp){	
	AcGeScale3d scale(_ttof(m_xScaleStr), _ttof(m_yScaleStr), 1);
	AcDbBlockTable* pblkTbl;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pblkTbl, AcDb::kForWrite);
	AcDbObjectId blkId;
	if(pblkTbl->has(blkName) == Adesk::kTrue){
		AcDbBlockTableIterator* pBTIter;
		if(pblkTbl->newIterator(pBTIter) == Acad::eOk){
			while(!pBTIter->done()){
				AcDbBlockTableRecord *pRecord;
				if(pBTIter->getRecord(pRecord, AcDb::kForRead) == Acad::eOk){
					ACHAR* temp;
					pRecord->getName(temp);
					if(blkName.CompareNoCase(temp) == 0){
						blkId = pRecord->objectId();
					}
					pRecord->close();
				}
				pBTIter->step();
			}
		}
		delete pBTIter;
	}
	pblkTbl->close();

	AcDbBlockTable* pTable;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pTable, AcDb::kForWrite);
	AcDbBlockReference* insBlk = new AcDbBlockReference();
	insBlk->setBlockTableRecord(blkId);
	insBlk->setPosition(asPnt3d(bp));
	insBlk->setScaleFactors(scale);
	insBlk->setRotation(0);
	insBlk->setDatabaseDefaults();

	AcDbBlockTableRecord *pBTRec = NULL;
	pTable->getAt(ACDB_MODEL_SPACE, pBTRec, AcDb::kForWrite);
	pBTRec ->appendAcDbEntity(insBlk);
	pTable->close();
	pBTRec ->close();
	insBlk->close();

	if(IsDlgButtonChecked(IDC_CHKEXPLODE) == FALSE){
		ads_name lastEn;
		ads_entlast(lastEn);
		AcDbObjectId entId;
		acdbGetObjectId(entId, lastEn);
		AcDbEntity *pEnt = NULL;
		acdbOpenObject(pEnt, entId, AcDb::kForWrite);
		AcDbVoidPtrArray pExps;
		if(pEnt->explode(pExps) == Acad::eOk){
			for(int i = 0; i < pExps.length(); i++){
				AcDbEntity *pExpEnt = (AcDbEntity*)pExps[i];
				AppendEntity(pExpEnt);
			}
			pEnt->erase(true);
		}
		pEnt->close();	

		AcDbBlockTable* pTable;
		Acad::ErrorStatus es = acdbCurDwg()->getBlockTable(pTable, AcDb::kForRead);
		if(es == Acad::eOk){
			AcDbBlockTableRecord* pBtr = NULL;
			if(pTable->has(blkName)){
				if(pTable->getAt(blkName, pBtr, AcDb::kForWrite) == Acad::eOk){
					pBtr->erase();
					pBtr->close();
				}
			}
			pTable->close();
		}
	}
}

AcDbObjectId CXYB::AppendEntity(AcDbEntity* pEnt){
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable *pBt;
	pDb->getBlockTable(pBt, AcDb::kForRead);
	AcDbBlockTableRecord *pBtr;
	pBt->getAt(ACDB_MODEL_SPACE, pBtr, AcDb::kForWrite);
	AcDbObjectId entId;
	pBtr->appendAcDbEntity(entId, pEnt);
	pBtr->close();
	pBt->close();
	pEnt->close();
	return entId;
}