#include "stdafx.h"
#include "CSR.h"
#include "afxdialogex.h"

// CCSR 대화 상자입니다.
IMPLEMENT_DYNAMIC(CCSR, CAcUiDialog)
	CCSR::CCSR(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CCSR::IDD, pParent),b_RadBtn(0){//라디오 버튼 변수 초기화
}

CCSR::~CCSR(){
}

void CCSR::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	//라운딩 거리지정 라디오/이미지 버튼
	DDX_Control(pDX, IDC_CSR_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_CSR_BUTTON2, m_btn2);
	DDX_Radio(pDX, IDC_CSR_RAD1, b_RadBtn);
	//Static
	DDX_Text(pDX,IDC_CSR_SCALE, m_ScaleStr);
	DDX_Text(pDX,IDC_CSR_TRIMDIST, m_TrimDistStr);
	//체크박스
	DDX_Control(pDX, IDC_CSR_TRIM, m_ChkTrim_btn);
	DDX_Control(pDX, IDC_CSR_DIVIDE, m_ChkDivide_btn);
	DDX_Control(pDX, IDC_CSR_EXPLODE, m_ChkExplode_btn);
}

BEGIN_MESSAGE_MAP(CCSR, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CCSR::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCSR::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CSR_RAD1, &CCSR::OnBnClickedCsrRad1)
	ON_BN_CLICKED(IDC_CSR_RAD2, &CCSR::OnBnClickedCsrRad2)
	ON_BN_CLICKED(IDC_CSR_RAD3, &CCSR::OnBnClickedCsrRad3)
	//ON_CONTROL_RANGE(BN_CLICKED,IDC_CSR_Rad1,IDC_CSR_RAD3,)
	ON_BN_CLICKED(IDC_CSR_BUTTON1, &CCSR::OnBnClickedCsrButton1)	
	ON_BN_CLICKED(IDC_CSR_BUTTON2, &CCSR::OnBnClickedCsrButton2)
	ON_EN_UPDATE(IDC_CSR_SCALE, &CCSR::OnEnUpdateCsrScale)
	ON_BN_CLICKED(IDC_CSR_TRIM, &CCSR::OnBnClickedCsrTrim)
	ON_BN_CLICKED(IDC_CSR_DIVIDE, &CCSR::OnBnClickedCsrDivide)
	ON_EN_UPDATE(IDC_CSR_TRIMDIST, &CCSR::OnEnUpdateCsrTrimdist)
	ON_BN_CLICKED(IDC_CSR_EXPLODE, &CCSR::OnBnClickedCsrExplode)
	
	ON_BN_CLICKED(ID_CSR_HELP, &CCSR::OnBnClickedCsrHelp)
END_MESSAGE_MAP()


BOOL CCSR::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	m_btn1.LoadBitmaps(IDB_CSR1,IDB_CSR1,NULL,NULL);
	m_btn1.SizeToContent();
	m_btn2.LoadBitmaps(IDB_CSR2,IDB_CSR2,NULL,NULL);
	m_btn2.SizeToContent();
	
	//라운딩 방식 라디오버튼
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("CSR"), _T("RoundMode"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		OnBnClickedCsrRad1();
	else if(tempStr.CompareNoCase(_T("2")) ==0)
		OnBnClickedCsrRad2();
	else if(tempStr.CompareNoCase(_T("3")) ==0)
		OnBnClickedCsrRad3();

	//횡단면도 축척
	tempStr = iniFile.readINI(_T("config.ini"), _T("CSR"), _T("Scale"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1000")) == 0)
		SetDlgItemText(IDC_CSR_SCALE,_T("1000"));
	else
		SetDlgItemText(IDC_CSR_SCALE,tempStr);

	//체크박스 라운딩 후 계획선 Trim
	tempStr = iniFile.readINI(_T("config.ini"), _T("CSR"), _T("Trim"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_ChkTrim_btn.SetCheck(FALSE);
		bool_ChkTrim = false;
		OnBnClickedCsrTrim();
	}else{
		m_ChkTrim_btn.SetCheck(TRUE);
		bool_ChkTrim = true;
		OnBnClickedCsrTrim();
	}

	//체크박스 라운딩 선 분할
	tempStr = iniFile.readINI(_T("config.ini"), _T("CSR"), _T("Divide"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_ChkDivide_btn.SetCheck(FALSE);
		bool_ChkDivide = false;
		OnBnClickedCsrDivide();
	}else{
		m_ChkDivide_btn.SetCheck(TRUE);
		bool_ChkDivide = true;
		OnBnClickedCsrDivide();
	}

	//분할 간격
	tempStr = iniFile.readINI(_T("config.ini"), _T("CSR"), _T("TrimDist"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0.2")) == 0)
		SetDlgItemText(IDC_CSR_TRIMDIST,_T("0.2"));
	else
		SetDlgItemText(IDC_CSR_TRIMDIST,tempStr);
	
	//체크박스 선으로 폭파
	tempStr = iniFile.readINI(_T("config.ini"), _T("CSR"), _T("Explode"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		m_ChkExplode_btn.SetCheck(FALSE);
		bool_ChkExplode = false;
		OnBnClickedCsrExplode();
	}else{
		m_ChkExplode_btn.SetCheck(TRUE);
		bool_ChkExplode = true;
		OnBnClickedCsrExplode();
	}

	return TRUE;
}

// 명령어 실행
void CCSR::OnBnClickedOk(){
	CAcUiDialog::OnOK();
	ads_name en1,en2;
	AcGePoint3d Planpt, Groundpt, PlanSpt,PlanEpt,GroundSpt,GroundEpt,CloseSpt,CloseEpt,ClosePt,pt, interPt;
	AcGePoint3dArray polyarr1,polyarr2, InterPtArr, PolySpEpArr;
	int Old_osmode;
	AcDbObjectId PlanObjId,GroundObjId;
	CString strMsg;

	getSysVar(_T("OSMODE"),Old_osmode);
	setSysVar(_T("OSMODE"), 0);//근처점 교차점 끝점
	
	BeginEditorCommand();
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	while(true){
		strMsg.LoadStringW(IDS_SLOPE_SELECT); // "\n사면 계획선 선택 : "
		int stat = acedEntSel(strMsg, en1, asDblArray(Planpt));

		if(stat == RTCAN || stat == RTNONE){
			setSysVar(_T("OSMODE"), Old_osmode);
			return;
		}else if(stat == RTNORM){
			acdbGetObjectId(PlanObjId, en1);
			AcDbEntityPointer pEnt(PlanObjId,AcDb::kForRead);

			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbLine::desc())){
					AcDbLine *pLine = AcDbLine::cast(pEnt);
					//사면 걔획선 속성 값 저장.
					m_color = pLine->colorIndex();
					m_layer = pLine->layer();
					m_lineType = pLine->linetype();
					m_ltScale = pLine->linetypeScale();

					pLine->getStartPoint(PlanSpt);
					pLine->getEndPoint(PlanEpt);
					pLine->close();
					polyarr1.append(PlanSpt);polyarr1.append(PlanEpt);
				}else if(pEnt->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline *pPline = AcDbPolyline::cast(pEnt);
					//사면 걔획선 속성 값 저장.
					m_color = pPline->colorIndex();
					m_layer = pPline->layer();
					m_lineType = pPline->linetype();
					m_ltScale = pPline->linetypeScale();

					pPline->getStartPoint(PlanSpt);
					pPline->getEndPoint(PlanEpt);
					int cnt = 0;
					while(true){//폴리선 포인트 array에 저장
						pPline->getPointAt(cnt,pt);
						polyarr1.append(pt);						
						if(pt == PlanEpt)
							break;
						cnt++;
					}
					pPline->close();
				}
				pEnt->close();
			}

			break;
		}else if(stat == RTERROR){
			resbuf zValueStr, zValueStr2;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);			
			ads_getvar(_T("ERRNO"), &zValueStr2);
			if(zValueStr2.resval.rint == 52)//Entsel에서 Enter 시 RTERROR로 들어와 다음과 같이 처리
			{	
				setSysVar(_T("OSMODE"), Old_osmode);
				return;
			}else
				continue;//객체 선택 없을시 다시 반복문
		}else
			continue;
	}//while end

	while(true){
		strMsg.LoadStringW(IDS_GROUND_SELECT);
		int stat = acedEntSel(strMsg, en2, asDblArray(Groundpt));

		if(stat == RTCAN || stat == RTNONE){
			setSysVar(_T("OSMODE"), Old_osmode);
			return;
		}else if(stat == RTNORM){
			acdbGetObjectId(GroundObjId, en2);
			AcDbEntityPointer pEnt(GroundObjId,AcDb::kForRead);

			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbLine::desc())){
					AcDbLine *pLine = AcDbLine::cast(pEnt);
					pLine->getStartPoint(GroundSpt);
					pLine->getEndPoint(GroundEpt);					
					pLine->close();
					polyarr2.append(GroundSpt); polyarr2.append(GroundEpt);
				}else if(pEnt->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline *pPline = AcDbPolyline::cast(pEnt);
					pPline->getStartPoint(GroundSpt);
					pPline->getEndPoint(GroundEpt);
					
					int cnt = 0;
					while(true){//폴리선 포인트 array에 저장
						pPline->getPointAt(cnt,pt);
						polyarr2.append(pt);						
						if(pt == GroundEpt)
							break;
						cnt++;
					}
					pPline->close();
				}
				pEnt->close();
			}
			break;
		}else if(stat == RTERROR){
			resbuf zValueStr, zValueStr2;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);			
			ads_getvar(_T("ERRNO"), &zValueStr2);
			if(zValueStr2.resval.rint == 52)//Entsel에서 Enter 시 RTERROR로 들어와 다음과 같이 처리
			{	
				setSysVar(_T("OSMODE"), Old_osmode);
				return;
			}else
				continue;//객체 선택 없을시 다시 반복문
		}else
			continue;
	}//while end
	
	AcGePoint3d vrtpt1,vrtpt2, Arcpt1,Arcpt2,polySpt,polyEpt,polarpt1,polarpt2,extendpt, Midpt;
	AcGePoint2d Arcpt1_2d,Arcpt2_2d;	
	AcGePoint3dArray divarr;
	int idx1,idx2;
	double m_max_x = polyarr1[0].x, m_max_y = polyarr1[0].y;
	double dist;
	
	// 계획선 정렬 polyarr1
	for(int j=0; j<polyarr1.length(); j++){//계획선이 폴리선일 떄 가장 높은점 = 지반선과 연결될 점
		if(polyarr1[j].y > m_max_y)
			m_max_y = polyarr1[j].y;
		if (polyarr1[j].x > m_max_x)
			m_max_x = polyarr1[j].x;
	}
	if(PlanEpt.y == m_max_y)//끝점이 가장높은 점의 y값일 경우 계획선 array reverse
		polyarr1.reverse();

	m_max_x = polyarr2[0].x, m_max_y = polyarr2[0].y;

	// 지반선 정렬 polyarr2
	for (int j = 0; j < polyarr2.length(); j++) {//지반선과 연결될 점
		if (polyarr2[j].y > m_max_y)
			m_max_y = polyarr2[j].y;
		if (polyarr2[j].x > m_max_x)
			m_max_x = polyarr2[j].x;
	}
	//if (GroundSpt.x == m_max_x && GroundEpt.y == m_max_y)//끝점이 가장높은 점의 x값일 경우 지반선 array reverse
	//	polyarr2.reverse();

	Polar(polyarr1[0],ads_angle(asDblArray(polyarr1[1]),asDblArray(polyarr1[0])),1000,vrtpt1);
	Polar(polyarr1[0],ads_angle(asDblArray(polyarr1[0]),asDblArray(polyarr1[1])),1000,vrtpt2);

	for(int i=1; i<polyarr2.length(); i++){
		if(acdbInters(asDblArray(vrtpt1),asDblArray(vrtpt2),asDblArray(polyarr2[i-1]),asDblArray(polyarr2[i]),1,asDblArray(interPt))==RTNORM){
			InterPtArr.append(interPt);PolySpEpArr.append(polyarr2[i-1]);PolySpEpArr.append(polyarr2[i]);
		}//interPtArr[0] = 교차점 PolySpEpArr[0],PolySpEpArr[1] = 폴리선의 시작 끝점.
	}

	//교차점이 1개를 초과할 경우 메시지 뿌림
	if(InterPtArr.length() != 1){
		strMsg.LoadStringW(IDS_GROUND_SLOPE_CHK);
		// \n계획선과 지반선의 교차점이 너무 많습니다.\n라운딩될 부분에 선이 여러개 교차되는지 확인해 주십시오."
		AfxMessageBox(strMsg);
		setSysVar(_T("OSMODE"), Old_osmode);
		return;
	}
	
	if(PlanSpt.distanceTo(interPt) < PlanEpt.distanceTo(interPt)){//교차점과 계획선의 시작,끝점중 가까운점을 끝점으로 지정
		AcGePoint3d chgpt;
		PlanSpt = chgpt;
		PlanSpt = PlanEpt;
		PlanEpt = chgpt;
	}

	//사이각 큰방향으로 라운딩
	AcGeVector3d vec1,vec2,vec3, vecBulge;
	
	vec1 = interPt - polyarr1[1];
	vec2 = interPt - PolySpEpArr[0];
	vec3 = interPt - PolySpEpArr[1];

	double Dimang1 = vec1.angleTo(vec2);
	double Dimang2 = vec1.angleTo(vec3);
	//PyoSi(interPt);
	//라운딩 방식
	if(RoundMode == 1){//라운딩 거리 지정
		TCHAR kword[30];
		double RoundingDist=0;
		AcGePoint3d Rdistpt1,Rdistpt2;
		while(true){
			setSysVar(_T("OSMODE"),545);
			strMsg.LoadStringW(IDS_SEL_ROUNDING); // "\nRounding 거리 입력 또는 두 점 선택 : "
			int stat = acedGetPoint(0, strMsg, asDblArray(Rdistpt1));

			if(stat == RTNONE || stat == RTCAN){
				setSysVar(_T("OSMODE"),Old_osmode);
				return;
			}else if(stat == RTNORM){ 
				if(acedGetInput(kword)!=RTNORM){
					setSysVar(_T("OSMODE"),Old_osmode);
					return;
				}
				//acutPrintf(_T("\n거리 : %f   점 : %f,%f"),_tstof(kword),Rdistpt1.x,Rdistpt1.y);
				RoundingDist = _tstof(kword);//RoundingDist = 거리
				if(RoundingDist == 0){//점 선택시
					strMsg.LoadStringW(IDS_SEL_SECOND); // "\n두 번째 점 지정 : ""
					if(acedGetPoint(asDblArray(Rdistpt1), strMsg, asDblArray(Rdistpt2))!=RTNORM){
						setSysVar(_T("OSMODE"),Old_osmode);
						return;
					}
					RoundingDist = Rdistpt1.distanceTo(Rdistpt2);// 두 점의 길이
				}else{//값 입력시 
					RoundingDist = _tstof(kword);
				}
				Polar(interPt,ads_angle(asDblArray(interPt),asDblArray(polyarr1[1])),RoundingDist,Arcpt1);
				//사이각 큰쪽으로 작도
				if(Dimang1 > Dimang2)
					Polar(interPt, ads_angle(asDblArray(interPt),asDblArray(PolySpEpArr[0])),RoundingDist,Arcpt2);
				else
					Polar(interPt, ads_angle(asDblArray(interPt),asDblArray(PolySpEpArr[1])),RoundingDist,Arcpt2);


				double ArcDim = 360 - (180 + RTD(acos(((pow(RoundingDist,2) * 2) - pow(Arcpt1.distanceTo(Arcpt2),2))/(2*pow(RoundingDist,2)))));
				double BulgeDist =tan(DTR(ArcDim/4.0));		
				double Bulge = BulgeDist * Arcpt1.distanceTo(Arcpt2) / 2.0;

				getclosestPoint(Arcpt1,Arcpt2,interPt,Midpt);//계획,지반선 교차점의 수직점
				PyoSi(Midpt);
				dist = -(interPt.distanceTo(Midpt) - Bulge);

				Polar(interPt,ads_angle(asDblArray(interPt),asDblArray(polyarr1[1])),RoundingDist*(1000.0/m_Scale),Arcpt1);
				//사이각 큰쪽으로 작도
				if(Dimang1 > Dimang2)
					Polar(interPt, ads_angle(asDblArray(interPt),asDblArray(PolySpEpArr[0])),RoundingDist*(1000.0/m_Scale),Arcpt2);
				else
					Polar(interPt, ads_angle(asDblArray(interPt),asDblArray(PolySpEpArr[1])),RoundingDist*(1000.0/m_Scale),Arcpt2);

				vec1 = interPt - Arcpt1;
				vec2 = interPt - Arcpt2;

				// 두 벡터의 방향 체크
				double vecCCW = vec1.x * vec2.y - vec1.y * vec2.x;

				AcGePoint3dArray exarr;
				if (vecCCW < 0) {
					exarr.append(Arcpt2);
					exarr.append(Arcpt1);
				} else {
					exarr.append(Arcpt1);
					exarr.append(Arcpt2);
				}

				AcDbObjectId Plobj = CreatePLine(exarr,0,m_lineType,m_ltScale,m_layer,m_color, dist);
				exarr.removeAll();		
				//PyoSi(Arcpt1);PyoSi(Arcpt2);
				//acedCommand(RTSTR, _T("_pline"), RT3DPOINT, Arcpt1, RTSTR, _T("A"), RTSTR, _T("D"), RT3DPOINT, interPt, RT3DPOINT, Arcpt2,RTSTR,_T(""), 0);

				AcDbEntityPointer pEnt1(Plobj,AcDb::kForRead);
				if(bool_ChkDivide){//라운딩 선 분할 시
					double div_cnt,m_length,div_remains;
					AcGePoint3d gspt,gept,div3dpt;
					AcGePoint2d div2dpt;

					if(pEnt1.openStatus() == Acad::eOk){
						if(pEnt1->isKindOf(AcDbPolyline::desc())){
							AcDbPolyline* pPline = AcDbPolyline::cast(pEnt1);
							pPline->upgradeOpen();
							pPline->getEndPoint(gept);
							pPline->getStartPoint(gspt);
							pPline->getDistAtPoint(gept,m_length);
							//m_length = 총거리
							div_cnt = floor(m_length/m_TrimDist);
							div_remains = fmod(m_length,m_TrimDist);					
							pPline->getPointAtDist(div_remains,div3dpt);
							//PyoSi(div3dpt);
							div2dpt.x = div3dpt.x; div2dpt.y = div3dpt.y;

							//pPline->addVertexAt(1,div2dpt);
							//pPline->setBulgeAt(0,0);
							for(int j=0; j<div_cnt; j++){
								pPline->getPointAtDist(m_TrimDist*j+div_remains,div3dpt);
								//PyoSi(div3dpt);
								div2dpt.x = div3dpt.x; div2dpt.y = div3dpt.y;
								divarr.append(div3dpt);
								//pPline->setBulgeAt(j,dist);
								//pPline->addVertexAt(j+1,div2dpt);
							}
							divarr.reverse();//정점 추가시 시작점부터 추가하때문에  reverse함.
							pPline->setBulgeAt(0,0);//폴리라인의 돌출 삭제
							for(int k=1; k<div_cnt+1; k++){
								pPline->addVertexAt(1,AcGePoint2d(divarr[k-1].x,divarr[k-1].y));	
							}
							AcDbVoidPtrArray voidptarr;
							pPline->explode(voidptarr);
							pPline->close();
						}
						pEnt1->close();
					}
					if(bool_ChkExplode){//선으로 폭파 체크시
						ads_name ads_Pline;
						acdbGetAdsName(ads_Pline,Plobj);
						acedCommand(RTSTR, _T("_EXPLODE"),RTENAME,ads_Pline,0);
						
					}
				}//bool_ChkDivide end

				acdbGetObjectId(PlanObjId, en1);
				AcDbEntityPointer pEnt(PlanObjId,AcDb::kForRead);
				if(bool_ChkTrim){//라운딩 후 계획선 Trim true일 떄
					if(pEnt.openStatus() == Acad::eOk){
						if(pEnt->isKindOf(AcDbLine::desc())){
							AcDbLine *pLine = AcDbLine::cast(pEnt);
							pLine->getStartPoint(PlanSpt);
							pLine->getEndPoint(PlanEpt);
							pLine->upgradeOpen();

							if(PlanSpt.distanceTo(interPt) < PlanEpt.distanceTo(interPt))
								pLine->setStartPoint(Arcpt1);
							else
								pLine->setEndPoint(Arcpt1);

							pLine->close();
						}else if(pEnt->isKindOf(AcDbPolyline::desc())){
							AcDbPolyline *pPline = AcDbPolyline::cast(pEnt);	
							pPline->upgradeOpen();
							if(PlanSpt.distanceTo(interPt) < PlanEpt.distanceTo(interPt)){
								Arcpt1_2d.x = Arcpt1.x; Arcpt1_2d.y = Arcpt1.y;
								pPline->setPointAt(0,Arcpt1_2d);				
							}else{
								Arcpt1_2d.x = Arcpt1.x; Arcpt1_2d.y = Arcpt1.y;
								pPline->setPointAt(polyarr1.length()-1,Arcpt1_2d);	
							}
							AcDbVoidPtrArray entarr;
							pPline->explode(entarr);

							pPline->close();
						}
						pEnt->close();
					}
				}//bool_ChkTrim end
				break;
			}//RTNORM end
		}//while end

	}
	else if (RoundMode == 2) {//지반선쪽 사거리 1.0m
		Polar(interPt, ads_angle(asDblArray(interPt), asDblArray(polyarr1[1])), 1, Arcpt1);
		//사이각 큰쪽으로 작도
		if (Dimang1 > Dimang2 || fabs(Dimang1 - Dimang2) < 0.000001)
			Polar(interPt, ads_angle(asDblArray(interPt), asDblArray(PolySpEpArr[0])), 1, Arcpt2);
		else
			Polar(interPt, ads_angle(asDblArray(interPt), asDblArray(PolySpEpArr[1])), 1, Arcpt2);

		//double ArcDim = 360 - (180 + RTD(acos((2 - pow(Arcpt1.distanceTo(Arcpt2),2))/2)));
		double a, b, c;
		a = Arcpt1.distanceTo(Arcpt2);
		b = interPt.distanceTo(Arcpt1);
		c = interPt.distanceTo(Arcpt2);
		// 두 직선의 각도 구하기 - 제 2코사인 법칙(삼각형 각도) : A = arccos((b^2 + c^2 - a^2)/(2bc)) 
		double ArcDim = 360 - (180 + RTD(acos((pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c))));
		//double ArcDim = 360 - (180 + RTD(acos((pow(1*(1000.0/m_Scale),2) + pow(1*(1000.0/m_Scale),2) - pow(Arcpt1.distanceTo(Arcpt2),2))/(2*(1*(1000.0/m_Scale))*(1*(1000.0/m_Scale))))));
		// bulge 구하기
		double BulgeDist = tan(DTR(ArcDim / 4.0));
		double Bulge = (BulgeDist * Arcpt1.distanceTo(Arcpt2) / 2.0);

		getclosestPoint(Arcpt1, Arcpt2, interPt, Midpt);//계획,지반선 교차점의 수직점
		vecBulge = interPt - Midpt;
		PyoSi(Midpt);
		dist = -(interPt.distanceTo(Midpt) - Bulge);
		//acutPrintf(_T("%f"), dist);
		//dist = dist *(1000.0/m_Scale);

		Polar(interPt, ads_angle(asDblArray(interPt), asDblArray(polyarr1[1])), 1 * (1000.0 / m_Scale), Arcpt1);
		//사이각 큰쪽으로 작도
		if (Dimang1 > Dimang2 || fabs(Dimang1 - Dimang2) < 0.000001)
			Polar(interPt, ads_angle(asDblArray(interPt), asDblArray(PolySpEpArr[0])), 1 * (1000.0 / m_Scale), Arcpt2);
		else
			Polar(interPt, ads_angle(asDblArray(interPt), asDblArray(PolySpEpArr[1])), 1 * (1000.0 / m_Scale), Arcpt2);

		vec1 = interPt - Arcpt1;
		vec2 = interPt - Arcpt2;

		// 두 벡터의 방향 체크
		double vecCCW = vec1.x * vec2.y - vec1.y * vec2.x;

		AcGePoint3dArray exarr;
		if (vecCCW < 0) {
			exarr.append(Arcpt2);
			exarr.append(Arcpt1);
		} else {
			exarr.append(Arcpt1);
			exarr.append(Arcpt2);
		}
		AcDbObjectId Plobj = CreatePLine(exarr,0,m_lineType,m_ltScale,m_layer,m_color, dist);
		exarr.removeAll();		
		//PyoSi(Arcpt1);PyoSi(Arcpt2);
		//acedCommand(RTSTR, _T("_pline"), RT3DPOINT, Arcpt1, RTSTR, _T("A"), RTSTR, _T("D"), RT3DPOINT, interPt, RT3DPOINT, Arcpt2,RTSTR,_T(""), 0);
		
		AcDbEntityPointer pEnt1(Plobj,AcDb::kForRead);
		if(bool_ChkDivide){//라운딩 선 분할 시
			double div_cnt,m_length,div_remains;
			AcGePoint3d gspt,gept,div3dpt;
			AcGePoint2d div2dpt;
			
			if(pEnt1.openStatus() == Acad::eOk){
				if(pEnt1->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline* pPline = AcDbPolyline::cast(pEnt1);
					pPline->upgradeOpen();
					pPline->getEndPoint(gept);
					pPline->getStartPoint(gspt);
					pPline->getDistAtPoint(gept,m_length);
					//m_length = 총거리
					div_cnt = floor(m_length/m_TrimDist);
					div_remains = fmod(m_length,m_TrimDist);					
					pPline->getPointAtDist(div_remains,div3dpt);
					//PyoSi(div3dpt);
					div2dpt.x = div3dpt.x; div2dpt.y = div3dpt.y;
					
					//pPline->addVertexAt(1,div2dpt);
					//pPline->setBulgeAt(0,0);
					for(int j=0; j<div_cnt; j++){
						pPline->getPointAtDist(m_TrimDist*j+div_remains,div3dpt);
						//PyoSi(div3dpt);
						div2dpt.x = div3dpt.x; div2dpt.y = div3dpt.y;
						divarr.append(div3dpt);
						
					}
					divarr.reverse();//정점 추가시 시작점부터 추가하때문에  reverse함.
					pPline->setBulgeAt(0,0);//폴리라인의 돌출 삭제
					for(int k=1; k<div_cnt+1; k++){
						pPline->addVertexAt(1,AcGePoint2d(divarr[k-1].x,divarr[k-1].y));	
					}					
					pPline->close();
				}
				pEnt1->close();
			}
			if(bool_ChkExplode){//선으로 폭파 체크시
				ads_name ads_Pline;
				acdbGetAdsName(ads_Pline,Plobj);
				acedCommand(RTSTR, _T("_EXPLODE"),RTENAME,ads_Pline,0);
			}
		}//bool_ChkDivide end
	
		acdbGetObjectId(PlanObjId, en1);
		AcDbEntityPointer pEnt(PlanObjId,AcDb::kForRead);
		if(bool_ChkTrim){//라운딩 후 계획선 Trim true일 떄
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbLine::desc())){
					AcDbLine *pLine = AcDbLine::cast(pEnt);
					pLine->getStartPoint(PlanSpt);
					pLine->getEndPoint(PlanEpt);
					pLine->upgradeOpen();

					if(PlanSpt.distanceTo(interPt) < PlanEpt.distanceTo(interPt))
						pLine->setStartPoint(Arcpt1);
					else
						pLine->setEndPoint(Arcpt1);

					pLine->close();
				}else if(pEnt->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline *pPline = AcDbPolyline::cast(pEnt);	
					pPline->upgradeOpen();
					if(PlanSpt.distanceTo(interPt) < PlanEpt.distanceTo(interPt)){
						Arcpt1_2d.x = Arcpt1.x; Arcpt1_2d.y = Arcpt1.y;
						pPline->setPointAt(0,Arcpt1_2d);				
					}else{
						Arcpt1_2d.x = Arcpt1.x; Arcpt1_2d.y = Arcpt1.y;
						pPline->setPointAt(polyarr1.length()-1,Arcpt1_2d);	
					}
					AcDbVoidPtrArray entarr;
					pPline->explode(entarr);
					
					pPline->close();
				}
				pEnt->close();
			}
		}//bool_ChkTrim end
		
	}else if( RoundMode == 3){//지반선쪽 수평거리 1.0m
		Polar(interPt,ads_angle(asDblArray(interPt),asDblArray(polyarr1[1])),1,Arcpt1);
		//사이각 큰쪽으로 작도
		if(Dimang1 > Dimang2 || fabs(Dimang1 - Dimang2) < 0.000001){
			Polar(interPt, ads_angle(asDblArray(interPt),asDblArray(PolySpEpArr[0])),1,Arcpt2);
		} else {			
			Polar(interPt, ads_angle(asDblArray(interPt),asDblArray(PolySpEpArr[1])),1,Arcpt2);
		}		
				
		double ArcDim = 360 - (180 + RTD(acos((2 - pow(Arcpt1.distanceTo(Arcpt2),2))/2)));
		double BulgeDist =tan(DTR(ArcDim/4.0));		
		double Bulge = BulgeDist * Arcpt1.distanceTo(Arcpt2) / 2.0;
		
		getclosestPoint(Arcpt1,Arcpt2,interPt,Midpt);//계획,지반선 교차점의 수직점
		PyoSi(Midpt);
		dist = -(interPt.distanceTo(Midpt) - Bulge);
		
		Polar(interPt,ads_angle(asDblArray(interPt),asDblArray(polyarr1[1])),1*(1000.0/m_Scale),Arcpt1);
		//사이각 큰쪽으로 작도
		if(Dimang1 > Dimang2 || fabs(Dimang1 - Dimang2) < 0.000001){
			Polar(interPt, ads_angle(asDblArray(interPt),asDblArray(PolySpEpArr[0])),1*(1000.0/m_Scale),Arcpt2);
		}else{
			Polar(interPt, ads_angle(asDblArray(interPt),asDblArray(PolySpEpArr[1])),1*(1000.0/m_Scale),Arcpt2);
		}
		
		vec1 = interPt - Arcpt1;
		vec2 = interPt - Arcpt2;

		// 두 벡터의 방향 체크
		double vecCCW = vec1.x * vec2.y - vec1.y * vec2.x;

		AcGePoint3dArray exarr;
		AcDbObjectId Plobj;
		if (vecCCW < 0) {
			Polar(interPt, PI, 1 * (1000.0 / m_Scale), polarpt1);
			Polar(polarpt1, PI / 2, 100, polarpt2);
			Polar(polarpt2, PI / 2 * 3, 100, polarpt1);

			acdbInters(asDblArray(polarpt1), asDblArray(polarpt2), asDblArray(PolySpEpArr[0]), asDblArray(PolySpEpArr[1]), 0, asDblArray(extendpt));

			exarr.append(extendpt);
			exarr.append(Arcpt2);
			exarr.append(Arcpt1);
			Plobj = CreatePLine(exarr, 0, m_lineType, m_ltScale, m_layer, m_color, dist, 1);
		} else {
			exarr.append(Arcpt1);
			exarr.append(Arcpt2);

			Polar(interPt, 0, 1 * (1000.0 / m_Scale), polarpt1);
			Polar(polarpt1, PI / 2, 100, polarpt2);
			Polar(polarpt2, PI / 2 * 3, 100, polarpt1);

			acdbInters(asDblArray(polarpt1), asDblArray(polarpt2), asDblArray(PolySpEpArr[0]), asDblArray(PolySpEpArr[1]), 0, asDblArray(extendpt));

			exarr.append(extendpt);
			Plobj = CreatePLine(exarr, 0, m_lineType, m_ltScale, m_layer, m_color, dist);
		}
		exarr.removeAll();		

		AcDbEntityPointer pEnt1(Plobj,AcDb::kForRead);
		if(bool_ChkDivide){//라운딩 선 분할 시
			double div_cnt,m_length,div_remains;
			AcGePoint3d gspt,gept,div3dpt;
			AcGePoint2d div2dpt;
			
			if(pEnt1.openStatus() == Acad::eOk){
				if(pEnt1->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline* pPline = AcDbPolyline::cast(pEnt1);
					pPline->upgradeOpen();
					
					pPline->getEndPoint(gept);
					pPline->getStartPoint(gspt);
					pPline->getDistAtPoint(gept,m_length);
					//m_length = 총거리
					div_cnt = floor(m_length/m_TrimDist);
					div_remains = fmod(m_length,m_TrimDist);					
					pPline->getPointAtDist(div_remains,div3dpt);
					//PyoSi(div3dpt);
					div2dpt.x = div3dpt.x; div2dpt.y = div3dpt.y;
					
					//pPline->addVertexAt(1,div2dpt);
					//pPline->setBulgeAt(0,0);
					for(int j=0; j<div_cnt; j++){
						//pPline->getPointAtDist(m_TrimDist*j+div_remains,div3dpt);
						pPline->getPointAtDist(m_TrimDist* j, div3dpt);
						//PyoSi(div3dpt);
						div2dpt.x = div3dpt.x; div2dpt.y = div3dpt.y;
						divarr.append(div3dpt);
						//pPline->setBulgeAt(j,dist);
						//pPline->addVertexAt(j+1,div2dpt);
					}

					pPline->getPointAtDist(m_TrimDist* div_cnt, div3dpt);
					divarr.append(div3dpt);

					divarr.reverse();//정점 추가시 시작점부터 추가하때문에  reverse함.l

					if (vecCCW < 0)
						pPline->setBulgeAt(1, 0);//폴리라인의 돌출 삭제
					else 
						pPline->setBulgeAt(0, 0);//폴리라인의 돌출 삭제

					// 첫점부터 그리기 위해 나머지 버텍스 삭제
					pPline->removeVertexAt(1);
					pPline->removeVertexAt(2);

					for(int k=1; k<div_cnt+1; k++){
						//acutPrintf(L"\ndivarr %g,%g\n", divarr[k - 1].x, divarr[k - 1].y);
						pPline->addVertexAt(1,AcGePoint2d(divarr[k-1].x,divarr[k-1].y));					
					}
					//pPline->addVertexAt(div_cnt+2,AcGePoint2d(extendpt.x,extendpt.y));//수평 교차점 끝에 추가.
					
					pPline->close();
				}
				pEnt1->close();
			}
			if(bool_ChkExplode){//선으로 폭파 체크시
				ads_name ads_Pline;
				acdbGetAdsName(ads_Pline,Plobj);
				acedCommand(RTSTR, _T("_EXPLODE"),RTENAME,ads_Pline,0);
			}
		}//bool_ChkDivide end
		

	}//roundmode end
	
	polyarr1.removeAll();polyarr2.removeAll();
	PolySpEpArr.removeAll();
	InterPtArr.removeAll();

	CompleteEditorCommand();

	//PyoSi(interPt);
	setSysVar(_T("OSMODE"), Old_osmode);//사용자 OSNAP 반환
}

void CCSR::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

//라운딩 거리 지정
void CCSR::OnBnClickedCsrRad1()
{
	UpdateData(TRUE);
	RoundMode = 1;
	RoundMode_Str.Format(_T("%d"),RoundMode);
	b_RadBtn = 0;
	iniFile.writeINI(_T("config.ini"), _T("CSR"), _T("RoundMode"), RoundMode_Str);
	UpdateData(FALSE);
}

//지반선쪽 사거리 1.0 m
void CCSR::OnBnClickedCsrRad2()
{
	UpdateData(TRUE);
	RoundMode = 2;
	RoundMode_Str.Format(_T("%d"),RoundMode);
	b_RadBtn = 1;
	iniFile.writeINI(_T("config.ini"), _T("CSR"), _T("RoundMode"), RoundMode_Str);
	UpdateData(FALSE);
}

//지반선쪽 사거리 1.0 m 버튼
void CCSR::OnBnClickedCsrButton1()
{
	UpdateData(TRUE);
	OnBnClickedCsrRad2();	
	UpdateData(FALSE);
}

//지반선쪽 수평거리 1.0 m 
void CCSR::OnBnClickedCsrRad3()
{
	UpdateData(TRUE);
	RoundMode = 3;
	RoundMode_Str.Format(_T("%d"),RoundMode);
	b_RadBtn = 2;
	iniFile.writeINI(_T("config.ini"), _T("CSR"), _T("RoundMode"), RoundMode_Str);
	UpdateData(FALSE);
}

//지반선쪽 수평거리 1.0 m 버튼
void CCSR::OnBnClickedCsrButton2()
{
	UpdateData(TRUE);
	OnBnClickedCsrRad3();
	UpdateData(FALSE);
}

//횡단면도 축척
void CCSR::OnEnUpdateCsrScale()
{
	UpdateData(TRUE);
	m_Scale = _tstof(m_ScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("CSR"), _T("Scale"), m_ScaleStr);
	UpdateData(FALSE);	
}

//라운딩 후 계획선 Trim 체크박스
void CCSR::OnBnClickedCsrTrim()
{
	UpdateData(TRUE);
	bool_ChkTrim = IsDlgButtonChecked(IDC_CSR_TRIM);
	if(bool_ChkTrim == 1){
		m_ChkTrim_btn.SetCheck(true);
		m_ChkTrimStr = _T("1");
	}else{
		m_ChkTrim_btn.SetCheck(false);
		m_ChkTrimStr = _T("0");
	}
	iniFile.writeINI(_T("config.ini"), _T("CSR"), _T("Trim"), m_ChkTrimStr);
	UpdateData(FALSE);
}

//라운딩 선 분할 체크박스
void CCSR::OnBnClickedCsrDivide()
{
	UpdateData(TRUE);
	bool_ChkDivide = IsDlgButtonChecked(IDC_CSR_DIVIDE);
	if(bool_ChkDivide == 1){
		m_ChkDivide_btn.SetCheck(true);
		m_ChkDivideStr = _T("1");
		GetDlgItem(IDC_CSR_TRIMDIST)->EnableWindow(true);
		GetDlgItem(IDC_CSR_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_CSR_EXPLODE)->EnableWindow(true);
	}else{
		m_ChkDivide_btn.SetCheck(false);
		m_ChkDivideStr = _T("0");
		GetDlgItem(IDC_CSR_TRIMDIST)->EnableWindow(false);
		GetDlgItem(IDC_CSR_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_CSR_EXPLODE)->EnableWindow(false);
	}
	iniFile.writeINI(_T("config.ini"), _T("CSR"), _T("Divide"), m_ChkDivideStr);
	UpdateData(FALSE);
}

//분할 간격 
void CCSR::OnEnUpdateCsrTrimdist()
{
	UpdateData(TRUE);
	m_TrimDist = _tstof(m_TrimDistStr);
	iniFile.writeINI(_T("config.ini"), _T("CSR"), _T("TrimDist"), m_TrimDistStr);
	UpdateData(FALSE);	
}

//선으로 폭파
void CCSR::OnBnClickedCsrExplode()
{
	UpdateData(TRUE);
	bool_ChkExplode = IsDlgButtonChecked(IDC_CSR_EXPLODE);
	if(bool_ChkExplode == 1){
		m_ChkExplode_btn.SetCheck(true);
		m_ChkExplodeStr = _T("1");
	}else{
		m_ChkExplode_btn.SetCheck(false);
		m_ChkExplodeStr = _T("0");
	}
	iniFile.writeINI(_T("config.ini"), _T("CSR"), _T("Explode"), m_ChkExplodeStr);
	UpdateData(FALSE);
}

Acad::ErrorStatus CCSR::getSysVar(LPCTSTR varName, double& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTREAL);
		val = rb.resval.rreal;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}
Acad::ErrorStatus CCSR::getSysVar(LPCTSTR varName, AcGePoint2d& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTPOINT);
		val.set(rb.resval.rpoint[X], rb.resval.rpoint[Y]);
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}
Acad::ErrorStatus CCSR::getSysVar(LPCTSTR varName, int& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTSHORT);
		val = rb.resval.rint;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}
Acad::ErrorStatus CCSR::setVar(LPCTSTR varName, const resbuf* newVal){
	CString str;
	if (ads_setvar(varName, newVal) != RTNORM) {
		str.Format(_T("Could not set system variable \"%s\"."), varName);
		return(Acad::eInvalidInput);
	}
	else {
		return(Acad::eOk);
	}
}
Acad::ErrorStatus CCSR::setSysVar(LPCTSTR varName, int val){
	ASSERT(varName != NULL);

	resbuf rb;
	rb.restype = RTSHORT;
	rb.resval.rint = (short)val;

	return(setVar(varName, &rb));
}

void CCSR::PyoSi(const AcGePoint3d pos)
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
double CCSR::getPickBox()
{
	int			iPickBox;
	double		viewsize;
	AcGePoint2d screensize;

	getSysVar(_T("VIEWSIZE"), viewsize);
	getSysVar(_T("SCREENSIZE"), screensize);
	getSysVar(_T("PICKBOX"), iPickBox);

	return ((viewsize / screensize.y)*(double)iPickBox);
}

void CCSR::Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result)
{
	result.set(base.x + dist*cos(angle), base.y + dist*sin(angle), base.z);
}

void CCSR::getclosestPoint(AcGePoint3d sp, AcGePoint3d ep, AcGePoint3d givenPnt, AcGePoint3d& closestPnt)
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

AcDbObjectId CCSR::CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer, double Color, double Bulge, int index)
{
	Acad::ErrorStatus es;
	AcDbObjectId pLineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	acDocManager->curDocument()->database();
	if(pDb == NULL)return 0;

	AcDbPolyline* pPline = new AcDbPolyline();
	SetProperty(pPline);
	for(int i =0; i<ptArray.length(); i++){
		pPline->addVertexAt(i, AcGePoint2d(ptArray[i].x, ptArray[i].y));
	}
	pPline->setConstantWidth(LineWidth);
	pPline->setLinetype(LineType);
	pPline->setLinetypeScale(LineTypeScale);
	pPline->setLayer(Layer);
	// index 기본값 0
	pPline->setBulgeAt(index,Bulge);
	//pPline->setBulgeAt(0,-0.17);
	
	
	

	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(pLineObjId, pPline);
			pBTR->close();
			pPline->close();
		}else
			return 0;
		pBT->close();
	}else
		return 0;
	pPline->close();

	return pLineObjId;
}

void CCSR::SetProperty(AcDbEntity *pEnt)
{
	if (pEnt->setColorIndex(m_color) != Acad::eOk)
	{
		CString strMsg;
		strMsg.LoadStringW(IDS_COLOR_CHECK);
		acutPrintf(strMsg);
	}//end-if

	pEnt->setLayer(m_layer);
	pEnt->setLinetype(m_lineType);	
	//pEnt->setLinetypeScale(m_ltScale);
	pEnt->setLinetypeScale(m_ltScale);	// 로컬 축척은 1.0으로 한다.
}

////각도를 라디안 값으로 변환하여 리턴한다.
double CCSR::DTR(double angle)
{
	return (angle / 180.0 * PI);
}

////라디안 값을 각도로 변환하여 리턴한다.
double CCSR::RTD(double radian)
{
	return (radian * 180.0 / PI);
}

void CCSR::OnBnClickedCsrHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/CSR.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
