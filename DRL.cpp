#include "StdAfx.h"
#include "DRL.h"

CDRL::CDRL(void){
}

CDRL::~CDRL(void){
}

void CDRL::MainDraw(){
	CString m_ScaleStr, m_WidthStr, OptionStr, OptionScaleStr, OptionWidthStr, strMsg;
	double m_Scale, m_Width, m_NewScale, m_NewWidth;
	AcGePoint3d FirstPt,SecondPt;
	TCHAR Kword[10];

	//축척
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("DRL"), _T("Scale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_ScaleStr = _T("1000");
	else
		m_ScaleStr = tempStr;
	//버림두께
	tempStr = iniFile.readINI(_T("config.ini"), _T("DRL"), _T("Width"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_WidthStr = _T("0.1");
	else
		m_WidthStr = tempStr;

	m_Scale = _tstof(m_ScaleStr);
	m_Width = _tstof(m_WidthStr);
	
	while(true){
		acedInitGet(0, _T("c C"));
		strMsg.LoadStringW(IDS_DRL_FIRST_SELECT); // "\n첫번째 점 지정 : < 현재설정 : 축척=%g, 버림두께=%g, 설정변경(C) > : "
		OptionStr.Format(strMsg,m_Scale,m_Width );
		strMsg.LoadStringW(IDS_DRL_INPUT_SCALE); //"\n축척(%g) : "
		OptionScaleStr.Format(strMsg, m_Scale);
		strMsg.LoadStringW(IDS_LEAN_THK); // "\n버림두께(%g) : "
		OptionWidthStr.Format(strMsg, m_Width);

		int stat = acedGetPoint(0, OptionStr, asDblArray(FirstPt));//첫번째 점 지정

		if(acedGetInput(Kword)!=RTNORM)
			return;

		if(stat == RTCAN || stat == RTNONE)
			return;
		else if(stat == RTKWORD){
			while(true){
				int ScaleStat = acedGetReal(OptionScaleStr, &m_Scale);//축척

				if(ScaleStat == RTCAN)
					return;
				else if(ScaleStat == RTNONE){
					m_Scale = _tstof(m_ScaleStr);
					break;
				}
				else if(ScaleStat == RTNORM){
					break;
				}
				else
					return;
			}
			
			while(true){
				int ThickStat = acedGetReal(OptionWidthStr, &m_Width);//버림두께

				if(ThickStat == RTCAN)
					return;
				else if(ThickStat == RTNONE){
					m_Width = _tstof(m_WidthStr);
					break;
				}
				else if(ThickStat == RTNORM){
					break;
				}
				else
					return;
			}	

			m_ScaleStr.Format(_T("%g"), m_Scale);//소수점 0을 다버리기 위해 %g 사용
			m_WidthStr.Format(_T("%g"), m_Width);
			iniFile.writeINI(_T("config.ini"), _T("DRL"), _T("Scale"), m_ScaleStr); 
			iniFile.writeINI(_T("config.ini"), _T("DRL"), _T("Width"), m_WidthStr); 
		}
		else if(stat == RTNORM){
			double Ang;
			AcGePoint3d FirstPt_1, FirstPt_2, SecondPt_1, SecondPt_2;
			AcGePoint3dArray ptArray;

			strMsg.LoadStringW(IDS_DRL_FIRST_SELECT); // "\n두번째 점 지정 : "
			if(acedGetPoint(asDblArray(FirstPt), strMsg, asDblArray(SecondPt)) != RTNORM)
				return;

			Ang = ads_angle(asDblArray(FirstPt), asDblArray(SecondPt));

			acutPolar(asDblArray(FirstPt), Ang + DTR(180.0), 0.1 * (1000 / m_Scale), asDblArray(FirstPt_1));
			acutPolar(asDblArray(FirstPt_1), Ang - DTR(90.0), m_Width * (1000 / m_Scale), asDblArray(FirstPt_2));
			acutPolar(asDblArray(SecondPt), Ang, 0.1 * (1000 / m_Scale), asDblArray(SecondPt_1));
			acutPolar(asDblArray(SecondPt_1), Ang - DTR(90.0), m_Width * (1000 / m_Scale), asDblArray(SecondPt_2));
			ptArray.append(FirstPt); ptArray.append(FirstPt_1); ptArray.append(FirstPt_2);
			ptArray.append(SecondPt_2);	ptArray.append(SecondPt_1); ptArray.append(SecondPt);
			CreateLayer(_T("CS-CONC-LEAN"), 3);
			CreatePLine(ptArray, _T("CS-CONC-LEAN"));
			break;
		}
		else
			continue;
	}

}

//폴리선 생성
bool CDRL::CreatePLine(AcGePoint3dArray ptArray, CString Layer){
	Acad::ErrorStatus es;
	AcDbObjectId plineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();

	if(pDb == NULL)
		return false;

	AcDbPolyline* pPLine = new AcDbPolyline();
	for(int i=0; i<ptArray.length(); i++){
		pPLine->addVertexAt(i, AcGePoint2d(ptArray[i].x, ptArray[i].y));
	}
	
	pPLine->setLayer(Layer);
	
	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(plineObjId, pPLine);
			pBTR->close();
			pPLine->close();
		}
		else
			return false;
		pBT->close();
	}
	else
		return false;
	
	pPLine->close();

	return true;
}

////각도를 라디안 값으로 변환하여 리턴한다.
double CDRL::DTR(double angle){
	return (angle / 180.0 * PI);
}

//레이어 생성
bool CDRL::CreateLayer(CString Layer, int color){
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