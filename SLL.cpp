#include "StdAfx.h"
#include "SLL.h"

CSLL::CSLL(void){
}

CSLL::~CSLL(void){
}

void CSLL::Main(){
	//수평축척
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("SLL"), _T("HScale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_HScale_Str = _T("1000");
	else
		m_HScale_Str = tempStr;
	//수직축척
	tempStr = iniFile.readINI(_T("config.ini"), _T("SLL"), _T("VScale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_VScale_Str = _T("1000");
	else
		m_VScale_Str = tempStr;
	
	m_HScale = _ttoi(m_HScale_Str);
	m_VScale = _ttoi(m_VScale_Str);
	AcGePoint3d spt, ept, SlopePt;
	AcGePoint3d ept_x,InterPt;
	int stat, HScale_Int[1], VScale_Int[1];
	TCHAR kWord[30];
	CString Format_HScale, Format_VScale, strMsg;
	double Yresult = 0, Xresult = 0;

	while(true){
		strMsg.LoadStringW(IDS_SET_VHSCALE); // "\n< 현재설정 : 수평축척=%d, 수직축척=%d >"
		acutPrintf(strMsg, m_HScale, m_VScale);
		acedInitGet(0, _T("C D"));
		strMsg.LoadStringW(IDS_START_SEL); // "\n시작점 선택 <축척 변경(C)/기본값(D)> : "
		stat = acedGetPoint(0, strMsg, asDblArray(spt));
		if(stat == RTCAN || stat == RTNONE)
			return;
		else if(stat == RTKWORD){
			if(acedGetInput(kWord) != RTNORM)
				return;
			if(!_tcscmp(_T("C"),kWord)){
				strMsg.LoadStringW(IDS_SET_HSCALE); // "\n수평 축척(%d) : "
				Format_HScale.Format(strMsg, m_HScale);
				if(acedGetInt(Format_HScale, HScale_Int)==RTNONE)
					HScale_Int[0] = m_HScale;
				strMsg.LoadStringW(IDS_SET_VSCALE); // "\n수직 축척(%d) : "
				Format_VScale.Format(strMsg, m_VScale);
				if(acedGetInt(Format_VScale, VScale_Int)==RTNONE)
					VScale_Int[0] = m_VScale;

				m_HScale_Str.Format(_T("%d"), HScale_Int[0]);
				m_VScale_Str.Format(_T("%d"), VScale_Int[0]);
				m_HScale = _ttoi(m_HScale_Str);
				m_VScale = _ttoi(m_VScale_Str);
				iniFile.writeINI(_T("config.ini"), _T("SLL"), _T("HScale"), m_HScale_Str); 
				iniFile.writeINI(_T("config.ini"), _T("SLL"), _T("VScale"), m_VScale_Str); 
			}
			else if(!_tcscmp(_T("D"),kWord)){//기본값 
				m_HScale_Str = _T("1000");
				m_VScale_Str = _T("1000");
				m_HScale = _ttoi(m_HScale_Str);
				m_VScale = _ttoi(m_VScale_Str);
				iniFile.writeINI(_T("config.ini"), _T("SLL"), _T("HScale"), m_HScale_Str); 
				iniFile.writeINI(_T("config.ini"), _T("SLL"), _T("VScale"), m_VScale_Str); 
			}			
		}
		else if(stat == RTNORM){
			strMsg.LoadStringW(IDS_DIRECT_SEL); // "\n다음점 방향 선택 : "
			if(acedGetPoint(asDblArray(spt), strMsg, asDblArray(ept)) != RTNORM)
				break;
			while(true){
				acedInitGet(0, _T("**"));
				strMsg.LoadStringW(IDS_SLOPE_INPUT); // "\n경사 입력(% 입력시 *사용[예:2*]) : "
				int Real = acedGetString(1, strMsg, kWord);
				if(Real == RTCAN || Real == RTNONE)
					return;
				else if(Real == RTNORM){
					CString temp;
					temp.Format(_T("%s"),kWord);
					if(temp == _T(""))//값 입력 없이Enter시 명령어 종료
						return;
					double dist = ads_distance(asDblArray(spt), asDblArray(ept));
					double Xdist = abs(spt.x - ept.x);
					double Ydist = abs(spt.y - ept.y);
					double Ang = ads_angle(asDblArray(spt), asDblArray(ept));
					CString InputString = CheckInputNumber((LPCTSTR)kWord);
					double Rec = 0.0;
					
					if(chkString == true){// (*)존재 %로 계산
						Yresult = abs(spt.y - ept.y);
						Xresult = (Yresult * 100) / _tstof(InputString);
						
						if(m_HScale >= m_VScale)//가로축척/세로축척으로  높이조절
							Yresult = Yresult * (m_HScale / m_VScale);
						else
							Yresult = Yresult / (m_VScale / m_HScale);
						if(Ang >= DTR(0.0) && Ang < DTR(90.0)){
							if(chkMinusString == false){// (-)존재에 따른 작도 변화
								SlopePt.x = spt.x + Xresult;
								SlopePt.y = spt.y + Yresult;
							}
							else{
								SlopePt.x = spt.x + Xresult;
								SlopePt.y = spt.y - Yresult;
							}
						}
						else if(Ang > DTR(90.0) && Ang < DTR(180.0)){
							if(chkMinusString == false){
								SlopePt.x = spt.x - Xresult;
								SlopePt.y = spt.y + Yresult;
							}
							else{
								SlopePt.x = spt.x - Xresult;
								SlopePt.y = spt.y - Yresult;
							}
						}
						else if(Ang > DTR(180.0) && Ang < DTR(270.0)){
							if(chkMinusString == false){
								SlopePt.x = spt.x - Xresult;
								SlopePt.y = spt.y - Yresult;
							}
							else{
								SlopePt.x = spt.x - Xresult;
								SlopePt.y = spt.y + Yresult;
							}
						}
						else if(Ang > DTR(270.0) && Ang < DTR(360.0)){
							if(chkMinusString == false){
								SlopePt.x = spt.x + Xresult;
								SlopePt.y = spt.y - Yresult;
							}
							else{
								SlopePt.x = spt.x + Xresult;
								SlopePt.y = spt.y + Yresult;
							}
						}
						else
							return;
						
						ept_x.x = ept.x;
						acdbInters(asDblArray(ept_x), asDblArray(ept), asDblArray(spt), asDblArray(SlopePt), 0, asDblArray(InterPt));
						CreateLine(spt, InterPt);
						return;
					}
					else{//  (*)존재X 비율 계산
						Yresult = abs(spt.y - ept.y);
						Xresult =_tstof(InputString) * Yresult;
						if(m_HScale >= m_VScale)//가로축척/세로축척으로  높이조절
							Yresult = Yresult * (m_HScale / m_VScale);
						else
							Yresult = Yresult / (m_VScale / m_HScale);

						if(Ang >= DTR(0.0) && Ang < DTR(90.0)){
							if(chkMinusString == false){// (-)존재에 따른 작도 변화
								SlopePt.x = spt.x + Xresult;
								SlopePt.y = spt.y + Yresult;
							}
							else{
								SlopePt.x = spt.x - Xresult;
								SlopePt.y = spt.y + Yresult;
							}
						}
						else if(Ang > DTR(90.0) && Ang < DTR(180.0)){
							if(chkMinusString == false){
								SlopePt.x = spt.x - Xresult;
								SlopePt.y = spt.y + Yresult;
							}
							else{
								SlopePt.x = spt.x + Xresult;
								SlopePt.y = spt.y + Yresult;
							}
						}
						else if(Ang > DTR(180.0) && Ang < DTR(270.0)){
							if(chkMinusString == false){
								SlopePt.x = spt.x - Xresult;
								SlopePt.y = spt.y - Yresult;
							}else{
								SlopePt.x = spt.x + Xresult;
								SlopePt.y = spt.y - Yresult;
							}
						}
						else if(Ang > DTR(270.0) && Ang < DTR(360.0)){
							if(chkMinusString == false){
								SlopePt.x = spt.x + Xresult;
								SlopePt.y = spt.y - Yresult;
							}
							else{
								SlopePt.x = spt.x - Xresult;
								SlopePt.y = spt.y - Yresult;
							}
						}
						else//0도 180도는 y증분이 0이라 구할 수 없으므로 return
							return;
						
						strMsg.LoadStringW(IDS_RECT_INPUT); // "\n사각 입력<0.0> : "
						stat = acedGetReal(strMsg, &Rec);
						if(stat == RTCAN)
							return;
						else if(stat == RTNONE){
							CreateLine(spt,SlopePt);
							break;
						}
						else if(stat == RTNORM){
							double Skew = 0, SkewHeight,SkewComb;
							AcGePoint3d SkewPt;
							SkewHeight = SlopePt.x * tan(DTR(Rec));
							SkewComb = SlopePt.x / cos(DTR(Rec));//빗변
							SlopePt.x = SkewComb;//밑변을 빗변으로 변경
							CreateLine(spt,SlopePt);
							return;
						}
						else
							return;
						return;
					}
				}
				else
					return;
			}
		}
		else
			continue;
	}
}

//숫자 및 점(.) 입력 체크
CString CSLL::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	chkString = false;
	chkMinusString = false;
	CString getstr;

	for(int i = 0; i < str.GetLength(); i++){
		getstr = str.Mid(i, 1);
		getAtChar = (TCHAR*)(LPCTSTR)getstr;
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46)){
			str.Remove(*getAtChar);
			i--;
			if(ascii == 42)
				chkString = true;
			if(ascii == 45)
				chkMinusString = true;
		}
	}
	return str;
}

bool CSLL::CreateLine(AcGePoint3d spt, AcGePoint3d ept){
	Acad::ErrorStatus es;
	AcDbObjectId pLineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();

	if(pDb == NULL)
		return false;

	AcDbLine* pLine = new AcDbLine(spt, ept);	
	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(pLineObjId, pLine);
			pBTR->close();
			pLine->close();
		}
		else
			return false;
		pBT->close();
	}
	else
		return false;
	pLine->close();

	return true;
}

////라디안 값을 각도로 변환하여 리턴한다.
double CSLL::RTD(double radian){
	return (radian * 180.0 / PI);
}

////각도를 라디안 값으로 변환하여 리턴한다.
double CSLL::DTR(double angle){
	return (angle / 180.0 * PI);
}