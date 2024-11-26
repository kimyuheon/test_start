// DCW.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DCW.h"
#include "afxdialogex.h"


// CDCW ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDCW, CAcUiDialog)

CDCW::CDCW(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CDCW::IDD, pParent){
}

CDCW::~CDCW(){
}

void CDCW::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAYER, CLayer);
	DDX_Control(pDX, IDC_SELLAYER, m_selectBtn);
}

BEGIN_MESSAGE_MAP(CDCW, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CDCW::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHKARROW, &CDCW::OnBnClickedChkarrow)
	ON_BN_CLICKED(IDC_SELLAYER, &CDCW::OnBnClickedSellayer)
	ON_BN_CLICKED(ID_DCW_HELP, &CDCW::OnBnClickedDcwHelp)
END_MESSAGE_MAP()

// CDCW �޽��� ó�����Դϴ�.

BOOL CDCW::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
    SetIcon(hIcon, FALSE);

	m_selectBtn.AutoLoad();

	// �޺��ڽ��� ���̾� �̸� �߰�
	initLayer();

	//����Ʈ �ڽ� �ʱ�ȭ
	CString factorStr = iniFile.readINI(_T("config.ini"), _T("DCW"), _T("FACTOR"));
	if(factorStr.CompareNoCase(_T("")) == 0)	factorStr.Format(_T("8.00"));
	SetDlgItemText(IDC_FACTOR, factorStr);

	CString thicknessStr = iniFile.readINI(_T("config.ini"), _T("DCW"), _T("THICKNESS"));
	if(thicknessStr.CompareNoCase(_T("")) == 0)	thicknessStr.Format(_T("0.50"));
	SetDlgItemText(IDC_THICKNESS, thicknessStr);
	
	CString colorFactorStr = iniFile.readINI(_T("config.ini"), _T("DCW"), _T("COLORFACTOR"));
	if(colorFactorStr.CompareNoCase(_T("")) == 0)	colorFactorStr.Format(_T("1.50"));
	SetDlgItemText(IDC_COLORFACTOR, colorFactorStr);
	
	CString factorSpaceStr = iniFile.readINI(_T("config.ini"), _T("DCW"), _T("FACTORSPACE"));
	if(factorSpaceStr.CompareNoCase(_T("")) == 0)	factorSpaceStr.Format(_T("0.25"));
	SetDlgItemText(IDC_FACTORSPACE, factorSpaceStr);
	
	CString divisionStr = iniFile.readINI(_T("config.ini"), _T("DCW"), _T("DIVISION"));
	if(divisionStr.CompareNoCase(_T("")) == 0)	divisionStr.Format(_T("6.00"));
	SetDlgItemText(IDC_DIVISION, divisionStr);
	
	//üũ�ڽ� �ʱ�ȭ
	CString arrowChkStr = iniFile.readINI(_T("config.ini"), _T("DCW"), _T("ARROWCHECK"));
	if(arrowChkStr.CompareNoCase(_T("")) == 0 || arrowChkStr.CompareNoCase(_T("TRUE")) == 0)	CheckDlgButton(IDC_CHKARROW, TRUE);
	else	CheckDlgButton(IDC_CHKARROW, FALSE);
	
	CString rangeChkStr = iniFile.readINI(_T("config.ini"), _T("DCW"), _T("RANGE"));
	if(rangeChkStr.CompareNoCase(_T("")) == 0 || rangeChkStr.CompareNoCase(_T("TRUE")) == 0)	CheckDlgButton(IDC_RANGLE, TRUE);
	else	CheckDlgButton(IDC_RECTANGLE, TRUE);

	//���̾� �޺��ڽ� �ʱ�ȭ
	CLayer.SetCurSel(0);

	return TRUE; 
}

static double getRadian(double degree){
	double rad = 0;
	rad = degree * 3.141592 / 180;
	return rad;
}

void CDCW::initLayer(){
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
		CLayer.AddString(name);
	}	
	delete pLayerTblIter;
	delete pLayerTbl;
}

void CDCW::OnBnClickedOk(){
	BeginEditorCommand();
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	makeDCW();
	CompleteEditorCommand();
}

void CDCW::makeDCW(){
	ads_point p1, p2;
	CString strMsg;

	strMsg.LoadStringW(IDS_FIRST_SELECT); // "ù��° ���� �����ϼ���."
	if(acedGetPoint(NULL, strMsg, p1) != RTNORM)
		return;
	strMsg.LoadStringW(IDS_SECOND_SELECT); // "�ι�° ���� �����ϼ���."
	if(acedGetPoint(p1, strMsg, p2) != RTNORM)
		return;

	AcGePoint3d start, end;
	start = asPnt3d(p1);
	end = asPnt3d(p2);

	// Ⱦ�ܺ��� �� ��ȯ
	CString CW;
	GetDlgItemText(IDC_FACTOR, CW);
	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("FACTOR"), CW);

	// ���� �� ��ȯ
	CString Thk;
	GetDlgItemText(IDC_THICKNESS, Thk);
	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("THICKNESS"), Thk);
	
	// ���� ���� ����
	CString sp;
	GetDlgItemText(IDC_COLORFACTOR, sp);
	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("COLORFACTOR"), sp);

	// ����� ���� ����
	CString fa;
	GetDlgItemText(IDC_FACTORSPACE, fa);
	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("FACTORSPACE"), fa);

	// ����� ����
	CString Div;
	GetDlgItemText(IDC_DIVISION, Div);
	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("DIVISION"), Div);

	// ���̾� �̸� �ҷ�����
	CString Layer;
	CLayer.GetLBText(CLayer.GetCurSel(), Layer);

	// CString���� ���� ��ġ���� double������ ��ȯ
	double division = _wtof(Div);
	double DCWWid = _wtof(CW);
	double thickNess = _wtof(Thk);
	double btwColorWid = _wtof(sp) * thickNess;
	double btwDCWWid = _wtof(fa);

	//�������� �簢���� bool���� ����
	bool chkRangle = IsDlgButtonChecked(IDC_RANGLE);
	if(chkRangle)	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("RANGE"), _T("TRUE"));
	bool chkRectangle = IsDlgButtonChecked(IDC_RECTANGLE);
	if(chkRectangle)	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("RANGE"), _T("FALSE"));
	
	//Ⱦ�ܺ����� Ʋ�� �����ϴ� �ܰ����� 4���� ���Ͱ� ����
	AcGeVector3d up, down, left, right;

	up = (end - start).normal();
	down = (start - end).normal();

	// ������ ��� �� ����, �簢�� ��� �� �ϳ��� �� �߰��� �¿� ���Ͱ� ����
	if(chkRangle){
		up.rotateBy(getRadian(90), AcGeVector3d::kZAxis);
		AcGePoint3d pVec;

		pVec = start + up * 40;
		up.rotateBy(getRadian(270), AcGeVector3d::kZAxis);

		left = (pVec - start).normal();
		right = (start - pVec).normal();
	}
	else if(chkRectangle){
		ads_point p3;
		strMsg.LoadStringW(IDS_THIRD_SELECT); // "����° ���� �����ϼ���."
		if(acedGetPoint(p2, strMsg, p3) != RTNORM)
			return;
		AcGePoint3d stand;
		stand = asPnt3d(p3);
		left = (stand - end).normal();
		right = (end - stand).normal();
	}
	
	// Ⱦ�ܺ����� ��ü �ܰ� ������ ����
	AcGePoint3d pt1, pt2, pt3, pt4;
	pt1 = start + left * DCWWid/2;
	pt2 = start + right * DCWWid/2;
	pt3 = end + left * DCWWid/2;
	pt4 = end + right * DCWWid/2;
		
	// ����� ������ Ⱦ�ܺ��� ������ ���� ���
	if(DCWWid >= division){
		// ���� ������ Ⱦ�ܺ��� ���� ���Ѵ�.
		double DCWDist = DCWWid/2 - btwDCWWid/2;

		// �ϳ��� Ⱦ�ܺ��� ���� ����Ʈ���� ���Ѵ�.
		AcGePoint3d LU, LD, RU, RD;
		RD = pt2;
		LD = pt2 + left * DCWDist;
		RU = pt2 + up * thickNess;
		LU = RU + left * DCWDist;

		//ó�� �׷��� Ⱦ�ܺ��� ���� ����� ���鿡 �ٷ� �׸���.
		AcDbSolid *pStartSolid = new AcDbSolid(LU, RU, LD, RD);

		// Ⱦ�ܺ��� ������ �׸� �� �������� ã�� ���� objectIdArray�� ���� �׸��� �ִ´�.
		AcDbObjectIdArray solidArr;
		AcDbBlockTableRecord *pBTR = NULL;
		if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite) != Acad::eOk)
			return;
		
		pStartSolid->setLayer(Layer);
		pBTR->appendAcDbEntity(pStartSolid);		
		AcDbObjectId startS = pStartSolid->objectId();
		solidArr.append(startS);	
		pStartSolid->close();

		// �׷��� Ⱦ�ܺ��� ���� �������� ���������� �����Ѵ�. ������ �����ʺ��� �Ѵ�.
		bool Lf = true;
		bool Rl = false;

		// Ⱦ�ܺ����� �ܰ��� ������ �׷��������� �ݺ��Ѵ�.
		while(true){
			// �׷��� Ⱦ�ܺ��� ���� ������ ���
			if(Lf && !Rl){
				// Ⱦ�ܺ��� ���� �ܰ����� �׸���.
				RD = LU + up * btwColorWid/3;
				RD = RD + left * btwDCWWid;
				LD = RD + left * DCWDist;
				RU = RD + up * thickNess;
				LU = RU + left * DCWDist;
				
				// �׷����� Ⱦ�ܺ����� ���̰� ���� �ܰ����� ����� ��� �ݺ����� �ߴ��Ѵ�.
				if(LD.distanceTo(pt1) >= pt1.distanceTo(pt3) || LU.distanceTo(pt1) >= pt1.distanceTo(pt3))	break;
		
				// �ش� Ⱦ�ܺ��� ���� �׸��� ���鿡 �ۼ��Ѵ�.
				AcDbSolid *pSolid = new AcDbSolid(LU, RU, LD, RD);
				pSolid->setLayer(Layer);
				pBTR->appendAcDbEntity(pSolid);	
				AcDbObjectId solid = pSolid->objectId();
				// Ⱦ�ܺ��� ObjectIdArray�� �ش� Ⱦ�ܺ��� ID�� �߰��Ѵ�.
				solidArr.append(solid);					
				pSolid->close();

				// bool ���� �ٲ۴�.
				Lf = false;
				Rl = true;
		
			}
			// �׷��� ���� �������� ���
			else if(!Lf && Rl){
				// Ⱦ�ܺ��� ���� �ܰ����� �׸���.
				LD = RU + up * btwColorWid/3;
				LD = LD + right * btwDCWWid;
				RD = LD + right * DCWDist;
				RU = RD + up * thickNess;
				LU = RU + left * DCWDist;			
		
				// �׷����� Ⱦ�ܺ����� ���̰� ������ �ܰ����� ����� ��� �ݺ����� �ߴ��Ѵ�.
				if(RD.distanceTo(pt2) >= pt2.distanceTo(pt4) || RU.distanceTo(pt2) >= pt2.distanceTo(pt4))	break;

				// �ش� Ⱦ�ܺ��� ���� �׸��� ���鿡 �ۼ��Ѵ�.
				AcDbSolid *pSolid = new AcDbSolid(LU, RU, LD, RD);
				pSolid->setLayer(Layer);
				pBTR->appendAcDbEntity(pSolid);
				AcDbObjectId solid = pSolid->objectId();

				// Ⱦ�ܺ��� ObjectIdArray�� �ش� Ⱦ�ܺ��� ID�� �߰��Ѵ�.
				solidArr.append(solid);			
				pSolid->close();
				
				// bool ���� �ٲ۴�.
				Lf = true;
				Rl = false;
			}
		}		
		
		// ȭ��ǥ�� �׸��� ���θ� ���� üũ�ڽ��� �����´�.
		bool chkArrow = IsDlgButtonChecked(IDC_CHKARROW);
		if(chkArrow){		
			iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("ARROWCHECK"), _T("TRUE"));
			//ó�� �׷��� Ⱦ�ܺ��� ��(������ ù��° ��)�� �ҷ��� �����Ѵ�.
			AcDbSolid *delSolid = NULL;
			if(acdbOpenObject(delSolid, solidArr[solidArr.find(solidArr.first())], AcDb::kForWrite) != Acad::eOk)
				return;
			delSolid->erase();
			delSolid->close();

			// �ش� ���� ID���� objectIdArray���� �����.
			solidArr.remove(solidArr.first());

			// ȭ��ǥ�� �׷��� �Ÿ����� ���Ѵ�.
			double arrowEnd = DCWDist/4;
			
			// ���ۺκ� 2���� ȭ��ǥ�� �׸� Point3d���� ���Ѵ�.
			AcGePoint3d arEnd, arLeft, arRight, LeftUp, LeftDown, RightUp, RightDown;
			AcGePoint3d arEnd2, arLeft2, arRight2, LeftUp2, LeftDown2, RightUp2, RightDown2;
			
			// ù��° Ⱦ�ܺ��� ���� ����� ���� ����° Ⱦ�ܺ��� ���� �Ʒ� �� ���� �ҷ��´�.
			AcDbSolid *standSolid = NULL;
			if(acdbOpenObject(standSolid, solidArr[1], AcDb::kForRead) != Acad::eOk)
				return;
			standSolid->getPointAt(2, arEnd);
			standSolid->getPointAt(3, arEnd2);
			standSolid->close();
					
			// �ΰ��� ȭ��ǥ�� �׸� ������ �����Ѵ�.
			arEnd = arEnd + right * arrowEnd;
			arEnd = arEnd + down * 0.1;		
			arLeft = arEnd + down * 0.4;
			arLeft = arLeft + left * 0.225;
			arRight = arLeft + right * 0.45;
			
			LeftUp = arLeft + right * 0.15;
			RightUp = arRight + left * 0.15;
			LeftDown = LeftUp + down * 0.4;
			RightDown = RightUp + down * 0.4;
			
			arEnd2 = arEnd2 + left * arrowEnd;
			arEnd2 = arEnd2 + down * 0.1;		
			arLeft2 = arEnd2 + down * 0.4;
			arLeft2 = arLeft2 + left * 0.225;
			arRight2 = arLeft2 + right * 0.45;
			
			LeftUp2 = arLeft2 + right * 0.15;
			RightUp2 = arRight2 + left * 0.15;
			LeftDown2 = LeftUp2 + down * 0.4;
			RightDown2 = RightUp2 + down * 0.4;
			
			// 4���� Solid�� �̷���� ȭ��ǥ�� �����ϰ� ���鿡 �׸���.
			AcDbSolid *Arrow = new AcDbSolid(arEnd, arLeft, arRight);
			AcDbSolid *Arrow2 = new AcDbSolid(LeftUp, RightUp, LeftDown, RightDown);
			AcDbSolid *Arrow3 = new AcDbSolid(arEnd2, arLeft2, arRight2);
			AcDbSolid *Arrow4 = new AcDbSolid(LeftUp2, RightUp2, LeftDown2, RightDown2);
			
			Arrow->setLayer(Layer);
			Arrow2->setLayer(Layer);
			Arrow3->setLayer(Layer);
			Arrow4->setLayer(Layer);

			pBTR->appendAcDbEntity(Arrow);
			pBTR->appendAcDbEntity(Arrow2);
			pBTR->appendAcDbEntity(Arrow3);
			pBTR->appendAcDbEntity(Arrow4);

			Arrow->close();
			Arrow2->close();
			Arrow3->close();
			Arrow4->close();
			
			// ��ü Ⱦ�ܺ��� ���� ������ ������ �������� 1���� 0���� �����Ѵ�.
			int chkLast = solidArr.length() % 2;

			// �������� 1�� ��� ���� ���� Ⱦ�ܺ��� ���� �����ϰ� ȭ��ǥ�� �׸���.
			if(chkLast == 1){

				//������ Ⱦ�ܺ��� ���� ã�Ƽ� �����Ѵ�.
				AcDbSolid *pLastSolid = NULL;
				if(acdbOpenObject(pLastSolid, solidArr[solidArr.find(solidArr.last())], AcDb::kForWrite) != Acad::eOk)
					return;
				pLastSolid->erase();
				pLastSolid->close();

				// �ش� Ⱦ�ܺ��� ���� ID���� ObjectIdArray���� �����.
				solidArr.remove(solidArr.last());

				// ���� ������ Ⱦ�ܺ��� ������ ����° Ⱦ�ܺ��� ���� �� ������ �ҷ��´�.
				AcDbSolid *pLStandSolid = NULL;
				if(acdbOpenObject(pLStandSolid, solidArr[solidArr.find(solidArr.last())-1], AcDb::kForRead) != Acad::eOk)
					return;

				AcGePoint3d arEnd3, arLeft3, arRight3, LeftUp3, LeftDown3, RightUp3, RightDown3;
				AcGePoint3d arEnd4, arLeft4, arRight4, LeftUp4, LeftDown4, RightUp4, RightDown4;

				pLStandSolid->getPointAt(0, arEnd3);
				pLStandSolid->getPointAt(1, arEnd4);
				pLStandSolid->close();

				// �ΰ��� ȭ��ǥ�� �׸� ������ �����Ѵ�.
				arEnd3 = arEnd3 + right * arrowEnd;
				arEnd3 = arEnd3 + up * 0.1;		
				arLeft3 = arEnd3 + up * 0.4;
				arLeft3 = arLeft3 + left * 0.225;
				arRight3 = arLeft3 + right * 0.45;
				
				LeftDown3 = arLeft3 + right * 0.15;
				RightDown3 = arRight3 + left * 0.15;
				LeftUp3 = LeftDown3 + up * 0.4;
				RightUp3 = RightDown3 + up * 0.4;
				
				arEnd4 = arEnd4 + left * arrowEnd;
				arEnd4 = arEnd4 + up * 0.1;		
				arLeft4 = arEnd4 + up * 0.4;
				arLeft4 = arLeft4 + left * 0.225;
				arRight4 = arLeft4 + right * 0.45;
				
				LeftDown4 = arLeft4 + right * 0.15;
				RightDown4 = arRight4 + left * 0.15;
				LeftUp4 = LeftDown4 + up * 0.4;
				RightUp4 = RightDown4 + up * 0.4;

				// 4���� Solid�� �̷���� ȭ��ǥ�� �����ϰ� ���鿡 �׸���.
				AcDbSolid *Arrow5 = new AcDbSolid(arLeft3, arRight3, arEnd3);
				AcDbSolid *Arrow6 = new AcDbSolid(LeftUp3, RightUp3, LeftDown3, RightDown3);
				AcDbSolid *Arrow7 = new AcDbSolid(arLeft4, arRight4, arEnd4);
				AcDbSolid *Arrow8 = new AcDbSolid(LeftUp4, RightUp4, LeftDown4, RightDown4);
				
				Arrow5->setLayer(Layer);
				Arrow6->setLayer(Layer);
				Arrow7->setLayer(Layer);
				Arrow8->setLayer(Layer);
				
				pBTR->appendAcDbEntity(Arrow5);
				pBTR->appendAcDbEntity(Arrow6);
				pBTR->appendAcDbEntity(Arrow7);
				pBTR->appendAcDbEntity(Arrow8);
				
				Arrow5->close();
				Arrow6->close();
				Arrow7->close();
				Arrow8->close();
			}		
			// ���������� ���� ��� �ٷ� ���������� �ι�° Ⱦ�ܺ��� ���� �� �� �� ���� �ҷ��´�.
			else if(chkLast == 0){				

				// ���������� �ι�° Ⱦ�ܺ��� ���� �ҷ��´�.
				AcDbSolid *pLStandSolid = NULL;
				if(acdbOpenObject(pLStandSolid, solidArr[solidArr.find(solidArr.last())-1], AcDb::kForRead) != Acad::eOk)
					return;

				AcGePoint3d arEnd3, arLeft3, arRight3, LeftUp3, LeftDown3, RightUp3, RightDown3;
				AcGePoint3d arEnd4, arLeft4, arRight4, LeftUp4, LeftDown4, RightUp4, RightDown4;

				pLStandSolid->getPointAt(0, arEnd3);
				pLStandSolid->getPointAt(1, arEnd4);
				pLStandSolid->close();

				// �ΰ��� ȭ��ǥ�� �׸� ������ �����Ѵ�.
				arEnd3 = arEnd3 + right * arrowEnd;
				arEnd3 = arEnd3 + up * 0.1;		
				arLeft3 = arEnd3 + up * 0.4;
				arLeft3 = arLeft3 + left * 0.225;
				arRight3 = arLeft3 + right * 0.45;
				
				LeftDown3 = arLeft3 + right * 0.15;
				RightDown3 = arRight3 + left * 0.15;
				LeftUp3 = LeftDown3 + up * 0.4;
				RightUp3 = RightDown3 + up * 0.4;
				
				arEnd4 = arEnd4 + left * arrowEnd;
				arEnd4 = arEnd4 + up * 0.1;		
				arLeft4 = arEnd4 + up * 0.4;
				arLeft4 = arLeft4 + left * 0.225;
				arRight4 = arLeft4 + right * 0.45;
				
				LeftDown4 = arLeft4 + right * 0.15;
				RightDown4 = arRight4 + left * 0.15;
				LeftUp4 = LeftDown4 + up * 0.4;
				RightUp4 = RightDown4 + up * 0.4;


				// 4���� Solid�� �̷���� ȭ��ǥ�� �����ϰ� ���鿡 �׸���.
				AcDbSolid *Arrow5 = new AcDbSolid(arLeft3, arRight3, arEnd3);
				AcDbSolid *Arrow6 = new AcDbSolid(LeftUp3, RightUp3, LeftDown3, RightDown3);
				AcDbSolid *Arrow7 = new AcDbSolid(arLeft4, arRight4, arEnd4);
				AcDbSolid *Arrow8 = new AcDbSolid(LeftUp4, RightUp4, LeftDown4, RightDown4);
				
				Arrow5->setLayer(Layer);
				Arrow6->setLayer(Layer);
				Arrow7->setLayer(Layer);
				Arrow8->setLayer(Layer);
				
				pBTR->appendAcDbEntity(Arrow5);
				pBTR->appendAcDbEntity(Arrow6);
				pBTR->appendAcDbEntity(Arrow7);
				pBTR->appendAcDbEntity(Arrow8);
				
				Arrow5->close();
				Arrow6->close();
				Arrow7->close();
				Arrow8->close();
			}
		}
		else	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("ARROWCHECK"), _T("FALSE"));		
		pBTR->close();
	}
	// ����� ������ Ⱦ�ܺ��� ������ Ŭ ���
	else if(DCWWid < division){
		// ��ü Ⱦ�ܺ��� ���� ���Ѵ�.
		AcGePoint3d LU, LD, RU, RD;
		RD = pt2;
		LD = pt2 + left * DCWWid;
		RU = pt2 + up * thickNess;
		LU = RU + left * DCWWid;
		
		// ù Ⱦ�ܺ��� ���� �׸��� ���鿡 �ۼ��Ѵ�.
		AcDbSolid *pStartSolid = new AcDbSolid(LU, RU, LD, RD);		

		AcDbObjectIdArray solidArr;
		AcDbBlockTableRecord *pBTR = NULL;
		if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite))
			return;
		
		pStartSolid->setLayer(Layer);
		pBTR->appendAcDbEntity(pStartSolid);
		
		AcDbObjectId startS = pStartSolid->objectId();
		// ù Ⱦ�ܺ��� ���� ID ���� ObjectIdArray�� �ִ´�.
		solidArr.append(startS);	
		pStartSolid->close();
		
		// Ⱦ�ܺ����� �ܰ��� ������ �׷��������� �ݺ��Ѵ�.
		while(true){
			// Ⱦ�ܺ��� ���� �׷��� ���� �����Ѵ�.
			RD = RD + up * (btwColorWid + thickNess);			
			LD = LD + up * (btwColorWid + thickNess);
			RU = RD + up * thickNess;
			LU = RU + left * DCWWid;

			// �ش� Ⱦ�ܺ��� ���� ������ ��ü �ܰ��� ���ʺ��� �� ��� �ݺ��� �ߴ��Ѵ�.
			if(LD.distanceTo(pt1) >= pt1.distanceTo(pt3) || LU.distanceTo(pt1) >= pt1.distanceTo(pt3))
				break;

			// �ش� Ⱦ�ܺ��� ���� �׸��� ���鿡 �ۼ��Ѵ�.							
			AcDbSolid *pSolid = new AcDbSolid(LU, RU, LD, RD);
			pSolid->setLayer(Layer);
			pBTR->appendAcDbEntity(pSolid);	
			AcDbObjectId solid = pSolid->objectId();
			// �ش� Ⱦ�ܺ��� ���� ID ���� ObjectIdArray�� �ִ´�.
			solidArr.append(solid);					
			pSolid->close();
		}			
		
		// �Ϲ��� Ⱦ�ܺ����� ȭ��ǥ�� ������ ���ϴ� üũ�ڽ��� ���� �����´�.
		bool chkArrow = IsDlgButtonChecked(IDC_CHKONEWAY);

		// ȭ��ǥ�� üũ�ڽ��� üũ�Ǿ����� ��� ȭ��ǥ�� �ִ´�.
		if(chkArrow){
			// ȭ��ǥ�� �׷��� ��ġ�� �Ÿ����� ù��°�� ������ Ⱦ�ܺ��� ���� ���̸� ������ ���δ�.
			double halfdist = DCWWid/2;		
			double arrowEnd = halfdist/2;

			// ù ��° Ⱦ�ܺ��� ���� ���̸� ������ ���̰� �ش� ���� ���� �� ���� ���� �����´�.
			AcDbSolid *FirSolid = NULL;		
			if(acdbOpenObject(FirSolid, solidArr[0], AcDb::kForWrite) != Acad::eOk)
				return;		
			AcGePoint3d halfup, halfdown;
			FirSolid->getPointAt(1, halfup);
			FirSolid->getPointAt(3, halfdown);
			
			halfup = halfup + left * halfdist;
			halfdown = halfdown + left * halfdist;

			FirSolid->setPointAt(1, halfup);
			FirSolid->setPointAt(3, halfdown);
			FirSolid->close();		

			AcGePoint3d arEnd, arLeft, arRight, LeftUp, LeftDown, RightUp, RightDown;
					
			arEnd = halfup;

			// ȭ��ǥ�� �׸� ���� �����Ѵ�.
			arEnd = arEnd + right * arrowEnd;
			arEnd = arEnd + up * 0.4;		
			arLeft = arEnd + down * 0.4;
			arLeft = arLeft + left * 0.225;
			arRight = arLeft + right * 0.45;
			
			LeftUp = arLeft + right * 0.15;
			RightUp = arRight + left * 0.15;
			LeftDown = LeftUp + down * 0.4;
			RightDown = RightUp + down * 0.4;
			
			// ȭ��ǥ�� �׸��� ���鿡 �ִ´�.
			AcDbSolid *Arrow = new AcDbSolid(arEnd, arLeft, arRight);
			AcDbSolid *Arrow2 = new AcDbSolid(LeftUp, RightUp, LeftDown, RightDown);					
			Arrow->setLayer(Layer);
			Arrow2->setLayer(Layer);
			pBTR->appendAcDbEntity(Arrow);
			pBTR->appendAcDbEntity(Arrow2);	
			Arrow->close();
			Arrow2->close();
			
			// ������ Ⱦ�ܺ��� ���� ���̸� ������ ���̰� �ش� ���� ������ �Ʒ� ���� ���� �����´�.
			AcDbSolid *pLastSolid = NULL;
			if(acdbOpenObject(pLastSolid, solidArr[solidArr.find(solidArr.last())], AcDb::kForWrite) != Acad::eOk)
				return;		

			AcGePoint3d halfup2, halfdown2;
						
			pLastSolid->getPointAt(0, halfup2);
			pLastSolid->getPointAt(2, halfdown2);
			
			halfup2 = halfup2 + right * halfdist;
			halfdown2 = halfdown2 + right * halfdist;

			pLastSolid->setPointAt(0, halfup2);
			pLastSolid->setPointAt(2, halfdown2);
			pLastSolid->close();

			AcGePoint3d arEnd2, arLeft2, arRight2, LeftUp2, LeftDown2, RightUp2, RightDown2;
			arEnd2 = halfdown2;

			// ȭ��ǥ�� �׸� ���� �����Ѵ�.
			arEnd2 = arEnd2 + left * arrowEnd;
			arEnd2 = arEnd2 + down * 0.4;		
			arLeft2 = arEnd2 + up * 0.4;
			arLeft2 = arLeft2 + left * 0.225;
			arRight2 = arLeft2 + right * 0.45;
			
			LeftUp2 = arLeft2 + right * 0.15;
			RightUp2 = arRight2 + left * 0.15;
			LeftDown2 = LeftUp2 + up * 0.4;
			RightDown2 = RightUp2 + up * 0.4;
			
			// ȭ��ǥ�� �׸��� ���鿡 �ִ´�.
			AcDbSolid *Arrow5 = new AcDbSolid(arLeft2, arRight2, arEnd2);
			AcDbSolid *Arrow6 = new AcDbSolid(LeftUp2, RightUp2, LeftDown2, RightDown2);			
			Arrow5->setLayer(Layer);
			Arrow6->setLayer(Layer);
			pBTR->appendAcDbEntity(Arrow5);
			pBTR->appendAcDbEntity(Arrow6);	
			Arrow5->close();
			Arrow6->close();		
		}
		//�� ���ڵ� ���̺��� �ݴ´�.
		pBTR->close();
	}
}

void CDCW::OnBnClickedChkarrow(){
	bool chkRA = IsDlgButtonChecked(IDC_CHKARROW);
	if(chkRA)
		GetDlgItem(IDC_CHKONEWAY)->EnableWindow(TRUE);
	else if(!chkRA)
		GetDlgItem(IDC_CHKONEWAY)->EnableWindow(FALSE);
}

void CDCW::OnBnClickedSellayer(){
	BeginEditorCommand();
	ads_point p1;
	ads_name n1;
	AcDbObjectId EntId;
	CString strMsg;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	strMsg.LoadStringW(IDS_SELECT_ENTITY); // "��ü ����: "
	if(acedEntSel(strMsg, n1, p1) != RTNORM)
		CompleteEditorCommand();

	if(acdbGetObjectId(EntId, n1) == Acad::eOk){	
		AcDbEntityPointer pEnt(EntId, AcDb::kForRead);
		CLayer.SetCurSel(CLayer.FindString(0, pEnt->layer()));
	}
	CompleteEditorCommand();
}


void CDCW::OnBnClickedDcwHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/DCW.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
