
#include "StdAfx.h"
#include "math.h"
#include "XY.h"
#include "XY_SET.h"

CXY::CXY(void){
}

CXY::~CXY(void){
}

void CXY::command(){
	ads_point p1, p2, p3;
	CString modeStr, modePrtStr, sizeStr, dotCntStr;

	while(true){
		modeStr = iniFile.readINI(_T("config.ini"), _T("XY"), _T("MODE"));
		if(modeStr.CompareNoCase(_T("")) == 0)	modeStr = _T("1");
		iniFile.writeINI(_T("config.ini"), _T("XY"), _T("MODE"), modeStr);
		sizeStr = iniFile.readINI(_T("config.ini"), _T("XY"), _T("SIZE"));
		if(sizeStr.CompareNoCase(_T("")) == 0)	sizeStr = _T("2.5");
		iniFile.writeINI(_T("config.ini"), _T("XY"), _T("SIZE"), sizeStr);
		tsize = _ttof(sizeStr);
		dotCntStr = iniFile.readINI(_T("config.ini"), _T("XY"), _T("DOTCNT"));
		if(dotCntStr.CompareNoCase(_T("")) == 0)	dotCntStr = _T("4");
		iniFile.writeINI(_T("config.ini"), _T("XY"), _T("DOTCNT"), dotCntStr);
		pntCnt = _ttoi(dotCntStr);

		if(modeStr.CompareNoCase(_T("1")))		modePrtStr.Format(_T("수학"));
		else if(modeStr.CompareNoCase(_T("2")))	modePrtStr.Format(_T("토목"));
		
		//AfxMessageBox(modePrtStr);

		int inputresult;
		acedInitGet(0, ACRX_T("C"));
		CString putStr = _T("\n좌표점 선택 [ 현재설정 : 좌표계 = ") + modePrtStr + _T(", 문자크기 = ") + sizeStr + _T(", 소수점 = ") + dotCntStr + _T(", 설정변경(C) ] : ");
		inputresult = acedGetPoint(NULL, putStr, p1);
		if(inputresult == RTNORM)	createXY(p1, p2, p3);
		else if(inputresult == RTKWORD){
			TCHAR inputStr[10];
			acedGetInput(inputStr);
			if(_tcscmp(inputStr, _T("C")) == 0){
				//////////////////////////////////
				//XY_SET dlg;
				//dlg.DoModal();
				//Mode : modePrt
				//크기 : tsize
				//소수 : pntCnt
				//////////////////////////////////
				CString prompt;
				prompt.Format(_T("토목=1, 수학=2: "), modeStr);
				acedGetInt(prompt, &modePrt);			
				if(modePrt <= 0 || 3 <= modePrt){
					modePrt = 1;
				}
				modeStr.Format(_T("%d"), modePrt);
				prompt.Format(_T("문자크기 = %s: "), sizeStr);
				acedGetReal(prompt, &tsize);
				sizeStr.Format(_T("%f"), tsize);
				prompt.Format(_T("소수점 개수 = %s: "), dotCntStr);
				acedGetInt(prompt, &pntCnt);
				if(pntCnt < -1 || 9 <= pntCnt){
					pntCnt = 4;
				}
				dotCntStr.Format(_T("%d"), pntCnt);			
		//AfxMessageBox(modeStr);
				iniFile.writeINI(_T("config.ini"), _T("XY"), _T("MODE"), modeStr);	
				iniFile.writeINI(_T("config.ini"), _T("XY"), _T("SIZE"), sizeStr);				
				iniFile.writeINI(_T("config.ini"), _T("XY"), _T("DOTCNT"), dotCntStr);				
			}
		}
		else	break;
	}
}

static double getRadian(double degree){
	double rad = 0;
	rad = degree * 3.141592 / 180;
	return rad;
}

void CXY::createXY(ads_point p1, ads_point p2, ads_point p3){
		if(acedGetPoint(p1, _T("다음 지시선 선택 : "), p2) != RTNORM)	return;
		if(acedGetPoint(p2, _T("방향 선택 : "), p3) != RTNORM)	return;
		AcGePoint3d pt1, pt2, pt3, endL, a1, atop, pt1_inp, pt2_inp, pt3_inp; // pt1_inp, pt2_inp, pt3_inp 추가(김성곤 대리 수정)
		AcDbExtents pExt;
		CString xtext, ytext, x, y;
		CString Cnt;
		Cnt = iniFile.readINI(_T("config.ini"), _T("XY"), _T("DOTCNT"));
		tsize = _ttof(iniFile.readINI(_T("config.ini"), _T("XY"), _T("SIZE")));
		
		pt1 = asPnt3d(p1);
		pt2 = asPnt3d(p2);
		pt3 = asPnt3d(p3);
		
		double xSize = pt1.x, ySize = pt1.y;

		switch(_ttoi(Cnt)){
		case 0:
			xtext.Format(_T("%.0f"), xSize);
			ytext.Format(_T("%.0f"), ySize);
			break;
		case 1:
			xtext.Format(_T("%.1f"), xSize);
			ytext.Format(_T("%.1f"), ySize);			
			break;
		case 2:
			xtext.Format(_T("%.2f"), xSize);
			ytext.Format(_T("%.2f"), ySize);
			break;
		case 3:
			xtext.Format(_T("%.3f"), xSize);
			ytext.Format(_T("%.3f"), ySize);
			break;
		case 4:
			xtext.Format(_T("%.4f"), xSize);
			ytext.Format(_T("%.4f"), ySize);
			break;
		case 5:
			xtext.Format(_T("%.5f"), xSize);
			ytext.Format(_T("%.5f"), ySize);
			break;
		case 6:
			xtext.Format(_T("%.6f"), xSize);
			ytext.Format(_T("%.6f"), ySize);
			break;
		case 7:
			xtext.Format(_T("%.7f"), xSize);
			ytext.Format(_T("%.7f"), ySize);
			break;
		case 8:
			xtext.Format(_T("%.8f"), xSize);
			ytext.Format(_T("%.8f"), ySize);
			break;
		}

		/*CString tempTextTrim;
		double temp = xSize * _Pow_int(10, _ttoi(Cnt));
		temp = floor(temp + 0.5);
		temp /= _Pow_int(10, _ttoi(Cnt));
				
		xtext.Format(_T("%G"), temp);

		temp = ySize * _Pow_int(10, _ttoi(Cnt));
		temp = floor(temp + 0.5);
		temp /= _Pow_int(10, _ttoi(Cnt));

		ytext.Format(_T("%G"), temp);*/
		// 김성곤 대리 수정 UCS가 적용되어 있을때, 좌표 수정
		acdbUcs2Wcs(asDblArray(pt1),asDblArray(pt1),Adesk::kFalse);
		acdbUcs2Wcs(asDblArray(pt2),asDblArray(pt2),Adesk::kFalse);
		acdbUcs2Wcs(asDblArray(pt3),asDblArray(pt3),Adesk::kFalse);	
		// 김성곤 대리 수정

		AcDbLine *Line = new AcDbLine(pt1, pt2);
		AcDbText *xText = new AcDbText;
		AcDbText *yText = new AcDbText;
		
		CString modeStr;
		modeStr = iniFile.readINI(_T("config.ini"), _T("XY"), _T("MODE"));
		if(modeStr.CompareNoCase(_T("1")) == 0){
			x = _T("X = ") + ytext;
			y = _T("Y = ") + xtext;	
		}else{
			x = _T("X = ") + xtext;
			y = _T("Y = ") + ytext;			
		}

		xText->setTextString(x);
		yText->setTextString(y);
		
		int xlen = xtext.GetLength(), ylen = ytext.GetLength();		
		if(pt3.x >= pt2.x)
		{
			xText->setHeight(tsize);
			xText->setPosition(pt2);

			//yText->setVerticalMode(AcDb::kTextTop);
			//yText->setHorizontalMode(AcDb::kTextLeft);
			yText->setHeight(tsize);
			//yText->setAlignmentPoint(pt2);
			yText->setPosition(pt2);
			
			AcGeVector3d a, a2;
			if(xlen >= ylen)		xText->getGeomExtents(pExt);
			else if(ylen > xlen)	yText->getGeomExtents(pExt);
			// 문자 폭 구하기
			double dist;
			double offset = 1;
			dist = pExt.maxPoint().x - pExt.minPoint().x;
			// 벡터 구하기
			a = (pt3 - pt2).normal();

			pt3 = pt2 + (a * dist);

			//pt3.set(pExt.maxPoint().x, pExt.minPoint().y, 0);
			//double dist = pt2.distanceTo(pt3)/6;
			//dist = pt2.distanceTo(pt3);
			//a1.set(pExt.minPoint().x, pExt.maxPoint().y, 0);
			//텍스트 크기 X2			
			//a = (pt2 - pt3).normal();
			//pt3 = pt3 + a * dist;
			AcGePoint3d p1, p2;
			p1.set(pt2.x+tsize, pt2.y+tsize, 0);
			p2.set(pt2.x+tsize, pt2.y-tsize, 0);

			xText->setPosition(p1);
			yText->setAlignmentPoint(p2);
		}
		else if(pt2.x > pt3.x){
			xText->setHorizontalMode(AcDb::kTextRight);
			xText->setHeight(tsize);
			xText->setPosition(pt2);
			xText->setAlignmentPoint(pt2);
			xText->getGeomExtents(pExt);

			yText->setVerticalMode(AcDb::kTextTop);
			yText->setHorizontalMode(AcDb::kTextRight);
			yText->setHeight(tsize);
			yText->setPosition(pt2);
			yText->setAlignmentPoint(pt2);
			AcGePoint3d p1, p2;

			pt3.set(pExt.maxPoint().x, pExt.minPoint().y, 0);			
			ads_point test;
			double dist = pt2.distanceTo(pt3);

			AcGeVector3d a;
			a = (pt2 - pt3).normal();

			pt3 = pt2 + a * dist;
			p1.set(pt3.x, pt3.y+tsize, 0);
			p2.set(pt3.x, pt3.y-tsize, 0);

			xText->setHorizontalMode(AcDb::kTextLeft);
			xText->setPosition(p1);
			yText->setHorizontalMode(AcDb::kTextLeft);
			yText->setAlignmentPoint(p2);
		}		
		AcDbLine *Line2 = new AcDbLine(pt2, pt3);
		AcDbBlockTableRecord *pBTR = NULL;
		if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite) != Acad::eOk)	return;
		pBTR->appendAcDbEntity(Line);
		pBTR->appendAcDbEntity(Line2);
		pBTR->appendAcDbEntity(xText);
		pBTR->appendAcDbEntity(yText);
		Line->close();
		Line2->close();
		xText->close();
		yText->close();
		pBTR->close();
}