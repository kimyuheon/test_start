// DCW.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DCW.h"
#include "afxdialogex.h"


// CDCW 대화 상자입니다.

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

// CDCW 메시지 처리기입니다.

BOOL CDCW::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
    SetIcon(hIcon, FALSE);

	m_selectBtn.AutoLoad();

	// 콤보박스에 레이어 이름 추가
	initLayer();

	//에디트 박스 초기화
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
	
	//체크박스 초기화
	CString arrowChkStr = iniFile.readINI(_T("config.ini"), _T("DCW"), _T("ARROWCHECK"));
	if(arrowChkStr.CompareNoCase(_T("")) == 0 || arrowChkStr.CompareNoCase(_T("TRUE")) == 0)	CheckDlgButton(IDC_CHKARROW, TRUE);
	else	CheckDlgButton(IDC_CHKARROW, FALSE);
	
	CString rangeChkStr = iniFile.readINI(_T("config.ini"), _T("DCW"), _T("RANGE"));
	if(rangeChkStr.CompareNoCase(_T("")) == 0 || rangeChkStr.CompareNoCase(_T("TRUE")) == 0)	CheckDlgButton(IDC_RANGLE, TRUE);
	else	CheckDlgButton(IDC_RECTANGLE, TRUE);

	//레이어 콤보박스 초기화
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

	strMsg.LoadStringW(IDS_FIRST_SELECT); // "첫번째 점을 지정하세요."
	if(acedGetPoint(NULL, strMsg, p1) != RTNORM)
		return;
	strMsg.LoadStringW(IDS_SECOND_SELECT); // "두번째 점을 지정하세요."
	if(acedGetPoint(p1, strMsg, p2) != RTNORM)
		return;

	AcGePoint3d start, end;
	start = asPnt3d(p1);
	end = asPnt3d(p2);

	// 횡단보도 폭 변환
	CString CW;
	GetDlgItemText(IDC_FACTOR, CW);
	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("FACTOR"), CW);

	// 도색 폭 변환
	CString Thk;
	GetDlgItemText(IDC_THICKNESS, Thk);
	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("THICKNESS"), Thk);
	
	// 도색 사이 간격
	CString sp;
	GetDlgItemText(IDC_COLORFACTOR, sp);
	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("COLORFACTOR"), sp);

	// 양방향 사이 간격
	CString fa;
	GetDlgItemText(IDC_FACTORSPACE, fa);
	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("FACTORSPACE"), fa);

	// 양방향 구분
	CString Div;
	GetDlgItemText(IDC_DIVISION, Div);
	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("DIVISION"), Div);

	// 레이어 이름 불러오기
	CString Layer;
	CLayer.GetLBText(CLayer.GetCurSel(), Layer);

	// CString으로 받은 수치들을 double형으로 변환
	double division = _wtof(Div);
	double DCWWid = _wtof(CW);
	double thickNess = _wtof(Thk);
	double btwColorWid = _wtof(sp) * thickNess;
	double btwDCWWid = _wtof(fa);

	//직각인지 사각인지 bool값을 대입
	bool chkRangle = IsDlgButtonChecked(IDC_RANGLE);
	if(chkRangle)	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("RANGE"), _T("TRUE"));
	bool chkRectangle = IsDlgButtonChecked(IDC_RECTANGLE);
	if(chkRectangle)	iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("RANGE"), _T("FALSE"));
	
	//횡단보도의 틀을 지정하는 외곽선과 4방향 벡터값 설정
	AcGeVector3d up, down, left, right;

	up = (end - start).normal();
	down = (start - end).normal();

	// 직각일 경우 두 점만, 사각일 경우 점 하나를 더 추가해 좌우 벡터값 설정
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
		strMsg.LoadStringW(IDS_THIRD_SELECT); // "세번째 점을 지정하세요."
		if(acedGetPoint(p2, strMsg, p3) != RTNORM)
			return;
		AcGePoint3d stand;
		stand = asPnt3d(p3);
		left = (stand - end).normal();
		right = (end - stand).normal();
	}
	
	// 횡단보도의 전체 외곽 꼭짓점 설정
	AcGePoint3d pt1, pt2, pt3, pt4;
	pt1 = start + left * DCWWid/2;
	pt2 = start + right * DCWWid/2;
	pt3 = end + left * DCWWid/2;
	pt4 = end + right * DCWWid/2;
		
	// 양방향 구분이 횡단보도 폭보다 적을 경우
	if(DCWWid >= division){
		// 한쪽 방향의 횡단보도 폭을 정한다.
		double DCWDist = DCWWid/2 - btwDCWWid/2;

		// 하나의 횡단보도 블럭의 포인트값을 정한다.
		AcGePoint3d LU, LD, RU, RD;
		RD = pt2;
		LD = pt2 + left * DCWDist;
		RU = pt2 + up * thickNess;
		LU = RU + left * DCWDist;

		//처음 그려질 횡단보도 블럭을 만들고 도면에 바로 그린다.
		AcDbSolid *pStartSolid = new AcDbSolid(LU, RU, LD, RD);

		// 횡단보도 방향을 그릴 때 기준점을 찾기 위해 objectIdArray에 블럭을 그리고 넣는다.
		AcDbObjectIdArray solidArr;
		AcDbBlockTableRecord *pBTR = NULL;
		if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite) != Acad::eOk)
			return;
		
		pStartSolid->setLayer(Layer);
		pBTR->appendAcDbEntity(pStartSolid);		
		AcDbObjectId startS = pStartSolid->objectId();
		solidArr.append(startS);	
		pStartSolid->close();

		// 그려질 횡단보도 블럭이 왼쪽인지 오른쪽인지 구분한다. 시작은 오른쪽부터 한다.
		bool Lf = true;
		bool Rl = false;

		// 횡단보도가 외곽점 끝까지 그려질때까지 반복한다.
		while(true){
			// 그려질 횡단보도 블럭이 왼쪽일 경우
			if(Lf && !Rl){
				// 횡단보도 블럭의 외곽선을 그린다.
				RD = LU + up * btwColorWid/3;
				RD = RD + left * btwDCWWid;
				LD = RD + left * DCWDist;
				RU = RD + up * thickNess;
				LU = RU + left * DCWDist;
				
				// 그려지는 횡단보도의 길이가 왼쪽 외곽보다 길어질 경우 반복문을 중단한다.
				if(LD.distanceTo(pt1) >= pt1.distanceTo(pt3) || LU.distanceTo(pt1) >= pt1.distanceTo(pt3))	break;
		
				// 해당 횡단보도 블럭을 그리고 도면에 작성한다.
				AcDbSolid *pSolid = new AcDbSolid(LU, RU, LD, RD);
				pSolid->setLayer(Layer);
				pBTR->appendAcDbEntity(pSolid);	
				AcDbObjectId solid = pSolid->objectId();
				// 횡단보도 ObjectIdArray에 해당 횡단보도 ID를 추가한다.
				solidArr.append(solid);					
				pSolid->close();

				// bool 값을 바꾼다.
				Lf = false;
				Rl = true;
		
			}
			// 그려질 블럭이 오른쪽일 경우
			else if(!Lf && Rl){
				// 횡단보도 블럭의 외곽선을 그린다.
				LD = RU + up * btwColorWid/3;
				LD = LD + right * btwDCWWid;
				RD = LD + right * DCWDist;
				RU = RD + up * thickNess;
				LU = RU + left * DCWDist;			
		
				// 그려지는 횡단보도의 길이가 오른쪽 외곽보다 길어질 경우 반복문을 중단한다.
				if(RD.distanceTo(pt2) >= pt2.distanceTo(pt4) || RU.distanceTo(pt2) >= pt2.distanceTo(pt4))	break;

				// 해당 횡단보도 블럭을 그리고 도면에 작성한다.
				AcDbSolid *pSolid = new AcDbSolid(LU, RU, LD, RD);
				pSolid->setLayer(Layer);
				pBTR->appendAcDbEntity(pSolid);
				AcDbObjectId solid = pSolid->objectId();

				// 횡단보도 ObjectIdArray에 해당 횡단보도 ID를 추가한다.
				solidArr.append(solid);			
				pSolid->close();
				
				// bool 값을 바꾼다.
				Lf = true;
				Rl = false;
			}
		}		
		
		// 화살표를 그릴지 여부를 묻는 체크박스를 가져온다.
		bool chkArrow = IsDlgButtonChecked(IDC_CHKARROW);
		if(chkArrow){		
			iniFile.writeINI(_T("config.ini"), _T("DCW"), _T("ARROWCHECK"), _T("TRUE"));
			//처음 그려진 횡단보도 블럭(오른쪽 첫번째 블럭)을 불러서 삭제한다.
			AcDbSolid *delSolid = NULL;
			if(acdbOpenObject(delSolid, solidArr[solidArr.find(solidArr.first())], AcDb::kForWrite) != Acad::eOk)
				return;
			delSolid->erase();
			delSolid->close();

			// 해당 블럭의 ID값을 objectIdArray에서 지운다.
			solidArr.remove(solidArr.first());

			// 화살표가 그려질 거리값을 정한다.
			double arrowEnd = DCWDist/4;
			
			// 시작부분 2개의 화살표를 그릴 Point3d값을 정한다.
			AcGePoint3d arEnd, arLeft, arRight, LeftUp, LeftDown, RightUp, RightDown;
			AcGePoint3d arEnd2, arLeft2, arRight2, LeftUp2, LeftDown2, RightUp2, RightDown2;
			
			// 첫번째 횡단보도 블럭을 지우고 나서 세번째 횡단보도 블럭의 아래 두 점을 불러온다.
			AcDbSolid *standSolid = NULL;
			if(acdbOpenObject(standSolid, solidArr[1], AcDb::kForRead) != Acad::eOk)
				return;
			standSolid->getPointAt(2, arEnd);
			standSolid->getPointAt(3, arEnd2);
			standSolid->close();
					
			// 두개의 화살표를 그릴 점들을 지정한다.
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
			
			// 4개의 Solid로 이루어진 화살표를 생성하고 도면에 그린다.
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
			
			// 전체 횡단보도 블럭의 개수를 나눠서 나머지가 1인지 0인지 구분한다.
			int chkLast = solidArr.length() % 2;

			// 나머지가 1인 경우 왼쪽 끝의 횡단보도 블럭을 삭제하고 화살표를 그린다.
			if(chkLast == 1){

				//마지막 횡단보도 블럭을 찾아서 삭제한다.
				AcDbSolid *pLastSolid = NULL;
				if(acdbOpenObject(pLastSolid, solidArr[solidArr.find(solidArr.last())], AcDb::kForWrite) != Acad::eOk)
					return;
				pLastSolid->erase();
				pLastSolid->close();

				// 해당 횡단보도 블럭의 ID값을 ObjectIdArray에서 지운다.
				solidArr.remove(solidArr.last());

				// 지운 마지막 횡단보도 블럭에서 세번째 횡단보도 블럭의 위 두점을 불러온다.
				AcDbSolid *pLStandSolid = NULL;
				if(acdbOpenObject(pLStandSolid, solidArr[solidArr.find(solidArr.last())-1], AcDb::kForRead) != Acad::eOk)
					return;

				AcGePoint3d arEnd3, arLeft3, arRight3, LeftUp3, LeftDown3, RightUp3, RightDown3;
				AcGePoint3d arEnd4, arLeft4, arRight4, LeftUp4, LeftDown4, RightUp4, RightDown4;

				pLStandSolid->getPointAt(0, arEnd3);
				pLStandSolid->getPointAt(1, arEnd4);
				pLStandSolid->close();

				// 두개의 화살표를 그릴 점들을 지정한다.
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

				// 4개의 Solid로 이루어진 화살표를 생성하고 도면에 그린다.
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
			// 나머지값이 없을 경우 바로 마지막에서 두번째 횡단보도 블럭의 위 두 점 값을 불러온다.
			else if(chkLast == 0){				

				// 마지막에서 두번째 횡단보도 블럭을 불러온다.
				AcDbSolid *pLStandSolid = NULL;
				if(acdbOpenObject(pLStandSolid, solidArr[solidArr.find(solidArr.last())-1], AcDb::kForRead) != Acad::eOk)
					return;

				AcGePoint3d arEnd3, arLeft3, arRight3, LeftUp3, LeftDown3, RightUp3, RightDown3;
				AcGePoint3d arEnd4, arLeft4, arRight4, LeftUp4, LeftDown4, RightUp4, RightDown4;

				pLStandSolid->getPointAt(0, arEnd3);
				pLStandSolid->getPointAt(1, arEnd4);
				pLStandSolid->close();

				// 두개의 화살표를 그릴 점들을 지정한다.
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


				// 4개의 Solid로 이루어진 화살표를 생성하고 도면에 그린다.
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
	// 양방향 구분이 횡단보도 폭보다 클 경우
	else if(DCWWid < division){
		// 전체 횡단보도 폭을 정한다.
		AcGePoint3d LU, LD, RU, RD;
		RD = pt2;
		LD = pt2 + left * DCWWid;
		RU = pt2 + up * thickNess;
		LU = RU + left * DCWWid;
		
		// 첫 횡단보도 블럭을 그리고 도면에 작성한다.
		AcDbSolid *pStartSolid = new AcDbSolid(LU, RU, LD, RD);		

		AcDbObjectIdArray solidArr;
		AcDbBlockTableRecord *pBTR = NULL;
		if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite))
			return;
		
		pStartSolid->setLayer(Layer);
		pBTR->appendAcDbEntity(pStartSolid);
		
		AcDbObjectId startS = pStartSolid->objectId();
		// 첫 횡단보도 블럭의 ID 값을 ObjectIdArray에 넣는다.
		solidArr.append(startS);	
		pStartSolid->close();
		
		// 횡단보도가 외곽점 끝까지 그려질때까지 반복한다.
		while(true){
			// 횡단보도 블럭이 그려질 점을 지정한다.
			RD = RD + up * (btwColorWid + thickNess);			
			LD = LD + up * (btwColorWid + thickNess);
			RU = RD + up * thickNess;
			LU = RU + left * DCWWid;

			// 해당 횡단보도 블럭의 왼쪽이 전체 외곽의 왼쪽보다 길 경우 반복을 중단한다.
			if(LD.distanceTo(pt1) >= pt1.distanceTo(pt3) || LU.distanceTo(pt1) >= pt1.distanceTo(pt3))
				break;

			// 해당 횡단보도 블럭을 그리고 도면에 작성한다.							
			AcDbSolid *pSolid = new AcDbSolid(LU, RU, LD, RD);
			pSolid->setLayer(Layer);
			pBTR->appendAcDbEntity(pSolid);	
			AcDbObjectId solid = pSolid->objectId();
			// 해당 횡단보도 블럭의 ID 값을 ObjectIdArray에 넣는다.
			solidArr.append(solid);					
			pSolid->close();
		}			
		
		// 일방향 횡단보도의 화살표를 넣을지 정하는 체크박스의 값을 가져온다.
		bool chkArrow = IsDlgButtonChecked(IDC_CHKONEWAY);

		// 화살표의 체크박스가 체크되어있을 경우 화살표를 넣는다.
		if(chkArrow){
			// 화살표가 그려질 위치의 거리값과 첫번째와 마지막 횡단보도 블럭의 길이를 반으로 줄인다.
			double halfdist = DCWWid/2;		
			double arrowEnd = halfdist/2;

			// 첫 번째 횡단보도 블럭의 길이를 반으로 줄이고 해당 블럭의 왼쪽 위 점의 값을 가져온다.
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

			// 화살표를 그릴 점을 지정한다.
			arEnd = arEnd + right * arrowEnd;
			arEnd = arEnd + up * 0.4;		
			arLeft = arEnd + down * 0.4;
			arLeft = arLeft + left * 0.225;
			arRight = arLeft + right * 0.45;
			
			LeftUp = arLeft + right * 0.15;
			RightUp = arRight + left * 0.15;
			LeftDown = LeftUp + down * 0.4;
			RightDown = RightUp + down * 0.4;
			
			// 화살표를 그리고 도면에 넣는다.
			AcDbSolid *Arrow = new AcDbSolid(arEnd, arLeft, arRight);
			AcDbSolid *Arrow2 = new AcDbSolid(LeftUp, RightUp, LeftDown, RightDown);					
			Arrow->setLayer(Layer);
			Arrow2->setLayer(Layer);
			pBTR->appendAcDbEntity(Arrow);
			pBTR->appendAcDbEntity(Arrow2);	
			Arrow->close();
			Arrow2->close();
			
			// 마지막 횡단보도 블럭의 길이를 반으로 줄이고 해당 블럭의 오른쪽 아래 점의 값을 가져온다.
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

			// 화살표를 그릴 점을 지정한다.
			arEnd2 = arEnd2 + left * arrowEnd;
			arEnd2 = arEnd2 + down * 0.4;		
			arLeft2 = arEnd2 + up * 0.4;
			arLeft2 = arLeft2 + left * 0.225;
			arRight2 = arLeft2 + right * 0.45;
			
			LeftUp2 = arLeft2 + right * 0.15;
			RightUp2 = arRight2 + left * 0.15;
			LeftDown2 = LeftUp2 + up * 0.4;
			RightDown2 = RightUp2 + up * 0.4;
			
			// 화살표를 그리고 도면에 넣는다.
			AcDbSolid *Arrow5 = new AcDbSolid(arLeft2, arRight2, arEnd2);
			AcDbSolid *Arrow6 = new AcDbSolid(LeftUp2, RightUp2, LeftDown2, RightDown2);			
			Arrow5->setLayer(Layer);
			Arrow6->setLayer(Layer);
			pBTR->appendAcDbEntity(Arrow5);
			pBTR->appendAcDbEntity(Arrow6);	
			Arrow5->close();
			Arrow6->close();		
		}
		//블럭 레코드 테이블을 닫는다.
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

	strMsg.LoadStringW(IDS_SELECT_ENTITY); // "객체 선택: "
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

	Help_Path.Format(_T("%s\\%s::/1. html/DCW.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
