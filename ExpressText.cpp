#include "stdafx.h"
#include "CEX.h"
#include "afxdialogex.h"

void CCEX::makeText(AcDbObjectIdArray arr)
{

	int pointcnt;
	pointcnt = GetDlgItemInt(IDC_POINTEDIT);
	CString cnt;
	cnt.Format(_T("%%.%df"), pointcnt);	

	CStdioFile TextFile;
	CString TextPath;
	CString Set;
	if(IsDlgButtonChecked(IDC_SET))
		GetDlgItem(IDC_SET)->GetWindowTextW(Set);
	else if(IsDlgButtonChecked(IDC_Colon))
		Set = ",";
	else if(IsDlgButtonChecked(IDC_SPACE))
		Set = " ";
	GetDlgItem(IDC_PATHINPUT)->GetWindowText(TextPath);
	AcGePoint3dArray chkArr;
	
	if(TextFile.Open(TextPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText)){
		for(int i = 0; i<arr.length(); i++){
			AcDbEntityPointer pEnt(arr[i], AcDb::kForRead);
			Acad::ErrorStatus es=pEnt.openStatus();
			if(es!=Acad::eOk){
				AfxMessageBox(L"블럭 열기 오류!");			
				return;
			}
			
			if(pEnt->isKindOf(AcDbArc::desc())){
				AcDbArc *pArc(AcDbArc::cast(pEnt));
				double startX, startY, startZ, endX, endY, endZ, centerX, centerY, centerZ, sumStr;
				AcGePoint3d start, end;
				pArc->getStartPoint(start);
				
				pArc->getEndPoint(end);			
				pArc->close();
				startX = start.x;
				startY = start.y;
				startZ = start.z;
		
				endX = end.x;
				endY = end.y;
				endZ = end.z;
		
				centerX = pArc->center().x;
				centerY = pArc->center().y;
				centerZ = pArc->center().z;
				
				CString startXStr;
				startXStr.Format(cnt, startX);			
				CString startYStr;
				startYStr.Format(cnt, startY);
				CString startZStr;
				startZStr.Format(cnt, startZ);
				CString cenXStr;
				cenXStr.Format(cnt, centerX);
				CString cenYStr;
				cenYStr.Format(cnt, centerY);
				CString cenZStr;
				cenZStr.Format(cnt, centerZ);
				CString endXStr;
				endXStr.Format(cnt, endX);
				CString endYStr;
				endYStr.Format(cnt, endY);
				CString endZStr;
				endZStr.Format(cnt, endZ);
		
				if(CExcept.GetCheck()){
					if(chkArr.length() == 0){
						chkArr.append(start);
						chkArr.append(end);
						chkArr.append(pArc->center());						
						if(CZAxis.GetCheck()){
							TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");
							TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");
							TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");
						}
						else if(!CZAxis.GetCheck()){
							TextFile.WriteString(startXStr + Set + startYStr + L"\n");
							TextFile.WriteString(endXStr + Set + endYStr + L"\n");
							TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
						}					
					}
					else{
						if(chkArr.find(start) == -1){
							chkArr.append(start);							
							if(CZAxis.GetCheck())
								TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");						
							else if(!CZAxis.GetCheck())
								TextFile.WriteString(startXStr + Set + startYStr + L"\n");								
						}
						if(chkArr.find(end) == -1){
							chkArr.append(end);
							if(CZAxis.GetCheck())
								TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");						
							else if(!CZAxis.GetCheck())
								TextFile.WriteString(endXStr + Set + endYStr + L"\n");
						}				
						if(chkArr.find(pArc->center()) == -1){
							chkArr.append(pArc->center());
							if(CZAxis.GetCheck())
								TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");						
							else if(!CZAxis.GetCheck())
								TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
						}
					}
				}
				else{
					if(CZAxis.GetCheck()){
						TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");
						TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");
						TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");
					}
					else if(!CZAxis.GetCheck()){
						TextFile.WriteString(startXStr + Set + startYStr + L"\n");
						TextFile.WriteString(endXStr + Set + endYStr + L"\n");
						TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
					}
				}
			}	
			else if(pEnt->isKindOf(AcDbLine::desc())){
				AcDbLine *Line(AcDbLine::cast(pEnt));
				double startX, startY, startZ, endX, endY, endZ;
				AcGePoint3d start, end;
		
				Line->getStartPoint(start);
				Line->getEndPoint(end);
				startX = start.x;
				startY = start.y;
				startZ = start.z;
				endX = end.x;
				endY = end.y;
				endZ = end.z;
				Line->close();
		
				CString startXStr;
				startXStr.Format(cnt, startX);
				CString startYStr;
				startYStr.Format(cnt, startY);
				CString startZStr;
				startZStr.Format(cnt, startZ);
				CString endXStr;
				endXStr.Format(cnt, endX);
				CString endYStr;
				endYStr.Format(cnt, endY);
				CString endZStr;
				endZStr.Format(cnt, endZ);
				
				if(CExcept.GetCheck()){
					if(chkArr.length() == 0){
						chkArr.append(start);
						chkArr.append(end);
						if(CZAxis.GetCheck()){
							TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");
							TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");
						}
						else if(!CZAxis.GetCheck()){
							TextFile.WriteString(startXStr + Set + startYStr + L"\n");
							TextFile.WriteString(endXStr + Set + endYStr + L"\n");
						}
					}
					else{
						if(chkArr.find(start) == -1){
							chkArr.append(start);
							if(CZAxis.GetCheck())
								TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");
							else if(!CZAxis.GetCheck())
								TextFile.WriteString(startXStr + Set + startYStr + L"\n");
						}
						if(chkArr.find(end) == -1){
							chkArr.append(end);
							if(CZAxis.GetCheck())
								TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");
							else if(!CZAxis.GetCheck())
								TextFile.WriteString(endXStr + Set + endYStr + L"\n");
						}
					}
				}
				else{
					if(CZAxis.GetCheck()){
						TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");
						TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");
					}
					else if(!CZAxis.GetCheck()){
						TextFile.WriteString(startXStr + Set + startYStr + L"\n");
						TextFile.WriteString(endXStr + Set + endYStr + L"\n");
					}
				}
			}
			else if(pEnt->isKindOf(AcDbCircle::desc())){
				AcDbCircle *Circle(AcDbCircle::cast(pEnt));
				AcGePoint3d center;
				double cenX, cenY, cenZ;
				center = Circle->center();
		
				cenX = center.x;
				cenY = center.y;
				cenZ = center.z;		
		
				CString cenXStr;
				cenXStr.Format(cnt, cenX);
				CString cenYStr;
				cenYStr.Format(cnt, cenY);
				CString cenZStr;
				cenZStr.Format(cnt, cenZ);
				if(CExcept.GetCheck()){
					if(chkArr.length() == 0){
						chkArr.append(center);
						if(CZAxis.GetCheck())							
							TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");						
						else if(!CZAxis.GetCheck())							
							TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");						
					}
					else if(chkArr.length() != 0){
						if(chkArr.find(center) == -1){
							chkArr.append(center);
							if(CZAxis.GetCheck())							
								TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");
							else if(!CZAxis.GetCheck())							
								TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
						}
					}				
				}
				else{
					if(CZAxis.GetCheck())							
						TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");
					else if(!CZAxis.GetCheck())							
						TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
				}
				Circle->close();
			}
			else if(pEnt->isKindOf(AcDbPolyline::desc())){
				AcDbPolyline *pline(AcDbPolyline::cast(pEnt));
				double startX, startY, startZ, cenX, cenY, cenZ, endX, endY, endZ;
				AcGePoint3d start, ver, cen;
				pline->getStartPoint(start);
		
				if(pline->hasBulges() && pline->numVerts() == 2 && CDonut.GetCheck()){							
					double bulge, bulge2;
					pline->getBulgeAt(0, bulge);
					pline->getBulgeAt(1, bulge2);
					if(bulge == bulge2 == 1.0){
						pline->getPointAt(1, ver);
						AcGeVector3d v1;
						v1 = (ver - start).normal();
						double dist = ver.distanceTo(start)/2;
						cen = start + v1 * dist;
						if(CExcept.GetCheck()){
							if(chkArr.length() == 0){
								chkArr.append(cen);
								CString cenXStr;
								cenXStr.Format(cnt, cen.x);
								CString cenYStr;
								cenYStr.Format(cnt, cen.y);
								CString cenZStr;
								cenZStr.Format(cnt, cen.z);
								if(CZAxis.GetCheck())							
									TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");						
								else if(!CZAxis.GetCheck())							
									TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
							}
							else if(chkArr.length() != 0){
								if(chkArr.find(cen) == -1){
									chkArr.append(cen);
									CString cenXStr;
									cenXStr.Format(cnt, cen.x);
									CString cenYStr;
									cenYStr.Format(cnt, cen.y);
									CString cenZStr;
									cenZStr.Format(cnt, cen.z);
									if(CZAxis.GetCheck())							
										TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");	
									else if(!CZAxis.GetCheck())							
										TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");					
								}
							}
						}
						else if(!CExcept.GetCheck()){						
							CString cenXStr;
							cenXStr.Format(cnt, cen.x);
							CString cenYStr;
							cenYStr.Format(cnt, cen.y);
							CString cenZStr;
							cenZStr.Format(cnt, cen.z);
							if(CZAxis.GetCheck())							
								TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");	
							else if(!CZAxis.GetCheck())							
								TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");					
						}
					}
					else if(CPolyLine.GetCheck()){					
						int vertcnt = pline->numVerts();
						for(int i=0; i<vertcnt; i++){						
							pline->getPointAt(i, ver);
							if(CExcept.GetCheck()){
								if(chkArr.length() == 0){
									chkArr.append(ver);
									CString verX;
									verX.Format(cnt, ver.x);
									CString verY;
									verY.Format(cnt, ver.y);
									CString verZ;
									verZ.Format(cnt, ver.z);
									if(CZAxis.GetCheck())							
										TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
									else if(!CZAxis.GetCheck())							
										TextFile.WriteString(verX + Set + verY + L"\n");					
								}
								else if(chkArr.length() != 0){
									if(chkArr.find(ver) == -1){
										chkArr.append(ver);
										CString verX;
										verX.Format(cnt, ver.x);
										CString verY;
										verY.Format(cnt, ver.y);
										CString verZ;
										verZ.Format(cnt, ver.z);
										if(CZAxis.GetCheck())							
											TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
										else if(!CZAxis.GetCheck())							
											TextFile.WriteString(verX + Set + verY + L"\n");
									}
								}
							}
							else{
								CString verX;
								verX.Format(cnt, ver.x);
								CString verY;
								verY.Format(cnt, ver.y);
								CString verZ;
								verZ.Format(cnt, ver.z);
								if(CZAxis.GetCheck())							
									TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
								else if(!CZAxis.GetCheck())							
									TextFile.WriteString(verX + Set + verY + L"\n");
							}
						}					
					}
				}
				else if(CPolyLine.GetCheck()){
					int vertcnt = pline->numVerts();
					for(int i=0; i<vertcnt; i++){
						int vertcnt = pline->numVerts();
						for(int i=0; i<vertcnt; i++){						
							pline->getPointAt(i, ver);
							if(CExcept.GetCheck()){
								if(chkArr.length() == 0){
									chkArr.append(ver);
									CString verX;
									verX.Format(cnt, ver.x);
									CString verY;
									verY.Format(cnt, ver.y);
									CString verZ;
									verZ.Format(cnt, ver.z);
									if(CZAxis.GetCheck())							
										TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
									else if(!CZAxis.GetCheck())							
										TextFile.WriteString(verX + Set + verY + L"\n");
								}
								else if(chkArr.length() != 0){
									if(chkArr.find(ver) == -1){
										chkArr.append(ver);
										CString verX;
										verX.Format(cnt, ver.x);
										CString verY;
										verY.Format(cnt, ver.y);
										CString verZ;
										verZ.Format(cnt, ver.z);
										if(CZAxis.GetCheck())							
											TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
										else if(!CZAxis.GetCheck())							
											TextFile.WriteString(verX + Set + verY + L"\n");
									}
								}
							}
							else{
								CString verX;
								verX.Format(cnt, ver.x);
								CString verY;
								verY.Format(cnt, ver.y);
								CString verZ;
								verZ.Format(cnt, ver.z);
								if(CZAxis.GetCheck())							
									TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
								else if(!CZAxis.GetCheck())							
									TextFile.WriteString(verX + Set + verY + L"\n");
							}
						}
					}
				}
			}
			else if(pEnt->isKindOf(AcDbPoint::desc())){
				AcDbPoint *Point(AcDbPoint::cast(pEnt));
				double pointX, pointY, pointZ;
				pointX = Point->position().x;
				pointY = Point->position().y;
				pointZ = Point->position().z;
				CString pX;
				pX.Format(cnt, pointX);
				CString pY;
				pX.Format(cnt, pointX);
				CString pZ;
				pX.Format(cnt, pointX);
				if(chkArr.length() == 0){
					chkArr.append(Point->position());
					if(CZAxis.GetCheck())							
						TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
					else if(!CZAxis.GetCheck())							
						TextFile.WriteString(pX + Set + pY + L"\n");
				}
				else{
					if(chkArr.find(Point->position()) == -1){
						chkArr.append(Point->position());
						if(CZAxis.GetCheck())							
							TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
						else if(!CZAxis.GetCheck())							
							TextFile.WriteString(pX + Set + pY + L"\n");
					}
				}
				Point->close();
			}
			else if(pEnt->isKindOf(AcDbBlockReference::desc())){
				AcDbBlockReference *pBR(AcDbBlockReference::cast(pEnt));			
				double pointX, pointY, pointZ;
				pointX = pBR->position().x;
				pointY = pBR->position().y;
				pointZ = pBR->position().z;
				CString pX;
				pX.Format(cnt, pointX);
				CString pY;
				pX.Format(cnt, pointX);
				CString pZ;
				pX.Format(cnt, pointX);
				
				if(CExcept.GetCheck()){
					if(chkArr.length() == 0){
						chkArr.append(pBR->position());
						if(CZAxis.GetCheck())							
							TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
						else if(!CZAxis.GetCheck())							
							TextFile.WriteString(pX + Set + pY + L"\n");
					}
					else{
						if(chkArr.find(pBR->position()) == -1){
							chkArr.append(pBR->position());
							if(CZAxis.GetCheck())							
								TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
							else if(!CZAxis.GetCheck())							
								TextFile.WriteString(pX + Set + pY + L"\n");
						}
					}
				}
				else {
					if(CZAxis.GetCheck())							
						TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
					else if(!CZAxis.GetCheck())							
						TextFile.WriteString(pX + Set + pY + L"\n");
				}
				pBR->close();
			}
			else if(pEnt->isKindOf(AcDbText::desc())){
				AcDbText *Text(AcDbText::cast(pEnt));
				double pointX, pointY, pointZ;
				pointX = Text->position().x;
				pointY = Text->position().y;
				pointZ = Text->position().z;
				CString pX;
				pX.Format(cnt, pointX);
				CString pY;
				pX.Format(cnt, pointX);
				CString pZ;
				pX.Format(cnt, pointX);
				CString conText = Text->textString();
				
				if(CExcept.GetCheck()){
					if(chkArr.length() == 0){
						chkArr.append(Text->position());
						if(CZAxis.GetCheck())
							if(CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + pZ + conText + L"\n");
							else if(!CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
						else if(!CZAxis.GetCheck())							
							if(CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + conText + L"\n");
							else if(!CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + L"\n");
					}
					else{
						if(chkArr.find(Text->position()) == -1){
							chkArr.append(Text->position());
							if(CZAxis.GetCheck())
							if(CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + pZ + conText + L"\n");
							else if(!CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
							else if(!CZAxis.GetCheck())							
							if(CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + conText + L"\n");
							else if(!CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + L"\n");
						}
					}
				}
				else if(CExcept.GetCheck()){
					if(CZAxis.GetCheck())
						if(CContext.GetCheck())
							TextFile.WriteString(pX + Set + pY + Set + pZ + conText + L"\n");
						else if(!CContext.GetCheck())
							TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
						else if(!CZAxis.GetCheck())							
						if(CContext.GetCheck())
							TextFile.WriteString(pX + Set + pY + Set + conText + L"\n");
						else if(!CContext.GetCheck())
							TextFile.WriteString(pX + Set + pY + Set + L"\n");
				}
				Text->close();
			}				
		}		
	}
	TextFile.Close();
}

void CCEX::makeText(AcDbObjectIdArray arr, int coordinate){
	int pointcnt;
	pointcnt = GetDlgItemInt(IDC_POINTEDIT);
	CString cnt;
	cnt.Format(_T("%%.%df"), pointcnt);	

	CStdioFile TextFile;
	CString TextPath;
	CString Set;
	if(IsDlgButtonChecked(IDC_SET))
		GetDlgItem(IDC_SET)->GetWindowTextW(Set);
	else if(IsDlgButtonChecked(IDC_Colon))
		Set = ",";
	else if(IsDlgButtonChecked(IDC_SPACE))
		Set = " ";
	GetDlgItem(IDC_PATHINPUT)->GetWindowText(TextPath);
	AcGePoint3dArray chkArr;
	
	if(TextFile.Open(TextPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText)){
		for(int i = 0; i<arr.length(); i++){
			AcDbEntityPointer pEnt(arr[i], AcDb::kForRead);
			Acad::ErrorStatus es=pEnt.openStatus();
			if(es!=Acad::eOk){
				AfxMessageBox(L"블럭 열기 오류!");			
				return;
			}
			
			if(pEnt->isKindOf(AcDbArc::desc())){
				AcDbArc *pArc(AcDbArc::cast(pEnt));
				double startX, startY, startZ, endX, endY, endZ, centerX, centerY, centerZ;
				AcGePoint3d start, end;
				pArc->getStartPoint(start);
				
				pArc->getEndPoint(end);			
				pArc->close();
				if(coordinate == 1){
					startX = start.y;
					startY = start.x;
					startZ = start.z;

					endX = end.y;
					endY = end.x;
					endZ = end.z;

					centerX = pArc->center().y;
					centerY = pArc->center().x;
					centerZ = pArc->center().z;
				}
				else{
					startX = start.x;
					startY = start.y;
					startZ = start.z;

					endX = end.x;
					endY = end.y;
					endZ = end.z;

					centerX = pArc->center().x;
					centerY = pArc->center().y;
					centerZ = pArc->center().z;
				}

				CString startXStr, startYStr, startZStr, cenXStr, cenYStr, cenZStr, endXStr, endYStr, endZStr;
				startXStr.Format(cnt, startX);
				startYStr.Format(cnt, startY);
				startZStr.Format(cnt, startZ);
				cenXStr.Format(cnt, centerX);
				cenYStr.Format(cnt, centerY);
				cenZStr.Format(cnt, centerZ);
				endXStr.Format(cnt, endX);
				endYStr.Format(cnt, endY);
				endZStr.Format(cnt, endZ);
		
				if(CExcept.GetCheck()){
					if(chkArr.length() == 0){
						chkArr.append(start);
						chkArr.append(end);
						chkArr.append(pArc->center());						
						if(CZAxis.GetCheck()){
							TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");
							TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");
							TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");
						}
						else if(!CZAxis.GetCheck()){
							TextFile.WriteString(startXStr + Set + startYStr + L"\n");
							TextFile.WriteString(endXStr + Set + endYStr + L"\n");
							TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
						}					
					}
					else{
						if(chkArr.find(start) == -1){
							chkArr.append(start);							
							if(CZAxis.GetCheck())
								TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");						
							else if(!CZAxis.GetCheck())
								TextFile.WriteString(startXStr + Set + startYStr + L"\n");								
						}
						if(chkArr.find(end) == -1){
							chkArr.append(end);
							if(CZAxis.GetCheck())
								TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");						
							else if(!CZAxis.GetCheck())
								TextFile.WriteString(endXStr + Set + endYStr + L"\n");
						}				
						if(chkArr.find(pArc->center()) == -1){
							chkArr.append(pArc->center());
							if(CZAxis.GetCheck())
								TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");						
							else if(!CZAxis.GetCheck())
								TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
						}
					}
				}
				else{
					if(CZAxis.GetCheck()){
						TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");
						TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");
						TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");
					}
					else if(!CZAxis.GetCheck()){
						TextFile.WriteString(startXStr + Set + startYStr + L"\n");
						TextFile.WriteString(endXStr + Set + endYStr + L"\n");
						TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
					}
				}
			}	
			else if(pEnt->isKindOf(AcDbLine::desc())){
				AcDbLine *Line(AcDbLine::cast(pEnt));
				double startX, startY, startZ, endX, endY, endZ;
				AcGePoint3d start, end;
		
				Line->getStartPoint(start);
				Line->getEndPoint(end);
				if(coordinate == 1){
					startX = start.y;
					startY = start.x;
					startZ = start.z;
					endX = end.y;
					endY = end.x;
					endZ = end.z;
				}
				else{
					startX = start.x;
					startY = start.y;
					startZ = start.z;
					endX = end.x;
					endY = end.y;
					endZ = end.z;
				}
				Line->close();
		
				CString startXStr, startYStr, startZStr, endXStr, endYStr, endZStr, sumStr;
				startXStr.Format(cnt, startX);
				startYStr.Format(cnt, startY);
				startZStr.Format(cnt, startZ);
				endXStr.Format(cnt, endX);
				endYStr.Format(cnt, endY);
				endZStr.Format(cnt, endZ);
				
				if(CExcept.GetCheck()){
					if(chkArr.length() == 0){
						chkArr.append(start);
						chkArr.append(end);
						if(CZAxis.GetCheck()){
							TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");
							TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");
						} else if(!CZAxis.GetCheck()){
							TextFile.WriteString(startXStr + Set + startYStr + L"\n");
							TextFile.WriteString(endXStr + Set + endYStr + L"\n");
						}
					}
					else{
						if(chkArr.find(start) == -1){
							chkArr.append(start);
							if (CZAxis.GetCheck()) {
								TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");
							} else if (!CZAxis.GetCheck()) {
								TextFile.WriteString(startXStr + Set + startYStr + L"\n");
							}
						}
						if(chkArr.find(end) == -1){
							chkArr.append(end);
							if (CZAxis.GetCheck()) {
								TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");
							} else if (!CZAxis.GetCheck()) {
								TextFile.WriteString(endXStr + Set + endYStr + L"\n");
							}
						}
					}
				}
				else{
					if(CZAxis.GetCheck()){
						TextFile.WriteString(startXStr + Set + startYStr + Set + startZStr + L"\n");
						TextFile.WriteString(endXStr + Set + endYStr + Set + endZStr + L"\n");
					} else if(!CZAxis.GetCheck()){
						TextFile.WriteString(startXStr + Set + startYStr + L"\n");
						TextFile.WriteString(endXStr + Set + endYStr + L"\n");
					}
				}
			}
			else if(pEnt->isKindOf(AcDbCircle::desc())){
				AcDbCircle *Circle(AcDbCircle::cast(pEnt));
				AcGePoint3d center;
				double cenX, cenY, cenZ;
				center = Circle->center();
		
				if(coordinate == 1){
					cenX = center.y;
					cenY = center.x;
					cenZ = center.z;
				}
				else{
					cenX = center.x;
					cenY = center.y;
					cenZ = center.z;
				}
		
				CString cenXStr, cenYStr, cenZStr;
				cenXStr.Format(cnt, cenX);
				cenYStr.Format(cnt, cenY);
				cenZStr.Format(cnt, cenZ);
				if(CExcept.GetCheck()){
					if(chkArr.length() == 0){
						chkArr.append(center);
						if(CZAxis.GetCheck())							
							TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");						
						else if(!CZAxis.GetCheck())							
							TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");						
					}
					else if(chkArr.length() != 0){
						if(chkArr.find(center) == -1){
							chkArr.append(center);
							if(CZAxis.GetCheck())							
								TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");
							else if(!CZAxis.GetCheck())							
								TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
						}
					}				
				}
				else{
					if(CZAxis.GetCheck())							
						TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");
					else if(!CZAxis.GetCheck())							
						TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
				}
				Circle->close();
			}
			else if(pEnt->isKindOf(AcDbPolyline::desc())){
				AcDbPolyline *pline(AcDbPolyline::cast(pEnt));
				double startX, startY, startZ, cenX, cenY, cenZ, endX, endY, endZ;
				AcGePoint3d start, ver, cen;
				pline->getStartPoint(start);
		
				if(pline->hasBulges() && pline->numVerts() == 2 && CDonut.GetCheck()){							
					double bulge, bulge2;
					pline->getBulgeAt(0, bulge);
					pline->getBulgeAt(1, bulge2);
					if(bulge == bulge2 == 1.0){
						pline->getPointAt(1, ver);
						AcGeVector3d v1;
						v1 = (ver - start).normal();
						double dist = ver.distanceTo(start)/2;
						cen = start + v1 * dist;
						if(CExcept.GetCheck()){
							if(chkArr.length() == 0){
								chkArr.append(cen);
								CString cenXStr, cenYStr, cenZStr;
								if(coordinate == 1){
									cenXStr.Format(cnt, cen.y);
									cenYStr.Format(cnt, cen.x);
									cenZStr.Format(cnt, cen.z);
								}
								else{
									cenXStr.Format(cnt, cen.x);
									cenYStr.Format(cnt, cen.y);
									cenZStr.Format(cnt, cen.z);
								}
								if(CZAxis.GetCheck())							
									TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");						
								else if(!CZAxis.GetCheck())							
									TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");
							}
							else if(chkArr.length() != 0){
								if(chkArr.find(cen) == -1){
									chkArr.append(cen);
									CString cenXStr, cenYStr, cenZStr;
									if(coordinate == 1){
										cenXStr.Format(cnt, cen.y);
										cenYStr.Format(cnt, cen.x);
										cenZStr.Format(cnt, cen.z);
									}
									else{
										cenXStr.Format(cnt, cen.x);
										cenYStr.Format(cnt, cen.y);
										cenZStr.Format(cnt, cen.z);
									}
									if(CZAxis.GetCheck())							
										TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");	
									else if(!CZAxis.GetCheck())							
										TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");					
								}
							}
						}
						else if(!CExcept.GetCheck()){
							CString cenXStr, cenYStr, cenZStr;
							if(coordinate == 1){
								cenXStr.Format(cnt, cen.y);
								cenYStr.Format(cnt, cen.x);
								cenZStr.Format(cnt, cen.z);
							}
							else{
								cenXStr.Format(cnt, cen.x);
								cenYStr.Format(cnt, cen.y);
								cenZStr.Format(cnt, cen.z);
							}
							if(CZAxis.GetCheck())							
								TextFile.WriteString(cenXStr + Set + cenYStr + Set + cenZStr + L"\n");	
							else if(!CZAxis.GetCheck())							
								TextFile.WriteString(cenXStr + Set + cenYStr + L"\n");					
						}
					}
					else if(CPolyLine.GetCheck()){					
						int vertcnt = pline->numVerts();
						for(int i=0; i<vertcnt; i++){						
							pline->getPointAt(i, ver);
							if(CExcept.GetCheck()){
								if(chkArr.length() == 0){
									chkArr.append(ver);
									CString verX, verY, verZ;
									if(coordinate == 1){
										verX.Format(cnt, ver.y);
										verY.Format(cnt, ver.x);
										verZ.Format(cnt, ver.z);
									}
									else{
										verX.Format(cnt, ver.x);
										verY.Format(cnt, ver.y);
										verZ.Format(cnt, ver.z);
									}
									if(CZAxis.GetCheck())							
										TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
									else if(!CZAxis.GetCheck())
										TextFile.WriteString(verX + Set + verY + L"\n");
								}
								else if(chkArr.length() != 0){
									if(chkArr.find(ver) == -1){
										chkArr.append(ver);
										CString verX, verY, verZ;
										if(coordinate == 1){
											verX.Format(cnt, ver.y);
											verY.Format(cnt, ver.x);
											verZ.Format(cnt, ver.z);
										}
										else{
											verX.Format(cnt, ver.x);
											verY.Format(cnt, ver.y);
											verZ.Format(cnt, ver.z);
										}
										if(CZAxis.GetCheck())							
											TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
										else if(!CZAxis.GetCheck())							
											TextFile.WriteString(verX + Set + verY + L"\n");
									}
								}
							}
							else{
								CString verX, verY, verZ;
								if(coordinate == 1){
									verX.Format(cnt, ver.y);
									verY.Format(cnt, ver.x);
									verZ.Format(cnt, ver.z);
								}
								else{
									verX.Format(cnt, ver.x);
									verY.Format(cnt, ver.y);
									verZ.Format(cnt, ver.z);
								}
								if(CZAxis.GetCheck())							
									TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
								else if(!CZAxis.GetCheck())							
									TextFile.WriteString(verX + Set + verY + L"\n");
							}
						}					
					}
				}
				else if(CPolyLine.GetCheck()){
					int vertcnt = pline->numVerts();
					for(int i=0; i<vertcnt; i++){
						int vertcnt = pline->numVerts();
						for(int i=0; i<vertcnt; i++){						
							pline->getPointAt(i, ver);
							if(CExcept.GetCheck()){
								if(chkArr.length() == 0){
									chkArr.append(ver);
									CString verX, verY, verZ;
									if(coordinate == 1){
										verX.Format(cnt, ver.y);
										verY.Format(cnt, ver.x);
										verZ.Format(cnt, ver.z);
									}
									else{
										verX.Format(cnt, ver.x);
										verY.Format(cnt, ver.y);
										verZ.Format(cnt, ver.z);
									}
									if(CZAxis.GetCheck())							
										TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
									else if(!CZAxis.GetCheck())							
										TextFile.WriteString(verX + Set + verY + L"\n");
								}
								else if(chkArr.length() != 0){
									if(chkArr.find(ver) == -1){
										chkArr.append(ver);
										CString verX, verY, verZ;
										if(coordinate == 1){
											verX.Format(cnt, ver.y);
											verY.Format(cnt, ver.x);
											verZ.Format(cnt, ver.z);
										}
										else{
											verX.Format(cnt, ver.x);
											verY.Format(cnt, ver.y);
											verZ.Format(cnt, ver.z);
										}
										if(CZAxis.GetCheck())							
											TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
										else if(!CZAxis.GetCheck())							
											TextFile.WriteString(verX + Set + verY + L"\n");
									}
								}
							}
							else{
								CString verX, verY, verZ;
								if(coordinate == 1){
									verX.Format(cnt, ver.y);
									verY.Format(cnt, ver.x);
									verZ.Format(cnt, ver.z);
								}
								else{
									verX.Format(cnt, ver.x);
									verY.Format(cnt, ver.y);
									verZ.Format(cnt, ver.z);
								}
								if(CZAxis.GetCheck())							
									TextFile.WriteString(verX + Set + verY + Set + verZ + L"\n");	
								else if(!CZAxis.GetCheck())							
									TextFile.WriteString(verX + Set + verY + L"\n");
							}
						}
					}
				}
			}
			else if(pEnt->isKindOf(AcDbPoint::desc())){
				AcDbPoint *Point(AcDbPoint::cast(pEnt));
				double pointX, pointY, pointZ;
				if(coordinate == 1){
					pointX = Point->position().y;
					pointY = Point->position().x;
					pointZ = Point->position().z;
				}
				else{
					pointX = Point->position().x;
					pointY = Point->position().y;
					pointZ = Point->position().z;
				}
				CString pX, pY, pZ;
				pX.Format(cnt, pointX);
				pX.Format(cnt, pointX);
				pX.Format(cnt, pointX);
				if(chkArr.length() == 0){
					chkArr.append(Point->position());
					if(CZAxis.GetCheck())							
						TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
					else if(!CZAxis.GetCheck())							
						TextFile.WriteString(pX + Set + pY + L"\n");
				}
				else{
					if(chkArr.find(Point->position()) == -1){
						chkArr.append(Point->position());
						if(CZAxis.GetCheck())							
							TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
						else if(!CZAxis.GetCheck())							
							TextFile.WriteString(pX + Set + pY + L"\n");
					}
				}
				Point->close();
			}
			else if(pEnt->isKindOf(AcDbBlockReference::desc())){
				AcDbBlockReference *pBR(AcDbBlockReference::cast(pEnt));			
				double pointX, pointY, pointZ;
				if(coordinate == 1){
					pointX = pBR->position().y;
					pointY = pBR->position().x;
					pointZ = pBR->position().z;
				}
				else{
					pointX = pBR->position().x;
					pointY = pBR->position().y;
					pointZ = pBR->position().z;
				}
				CString pX, pY, pZ;
				pX.Format(cnt, pointX);
				pX.Format(cnt, pointX);
				pX.Format(cnt, pointX);
				
				if(CExcept.GetCheck()){
					if(chkArr.length() == 0){
						chkArr.append(pBR->position());
						if(CZAxis.GetCheck())							
							TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
						else if(!CZAxis.GetCheck())							
							TextFile.WriteString(pX + Set + pY + L"\n");
					}
					else{
						if(chkArr.find(pBR->position()) == -1){
							chkArr.append(pBR->position());
							if(CZAxis.GetCheck())							
								TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
							else if(!CZAxis.GetCheck())							
								TextFile.WriteString(pX + Set + pY + L"\n");
						}
					}
				}
				else {
					if(CZAxis.GetCheck())
						TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
					else if(!CZAxis.GetCheck())
						TextFile.WriteString(pX + Set + pY + L"\n");
				}
				pBR->close();
			}
			else if(pEnt->isKindOf(AcDbText::desc())){
				AcDbText *Text(AcDbText::cast(pEnt));
				double pointX, pointY, pointZ;
				if(coordinate == 1){
					pointX = Text->position().y;
					pointY = Text->position().x;
					pointZ = Text->position().z;
				}
				else{
					pointX = Text->position().x;
					pointY = Text->position().y;
					pointZ = Text->position().z;
				}
				CString pX, pY, pZ;
				pX.Format(cnt, pointX);
				pX.Format(cnt, pointX);
				pX.Format(cnt, pointX);
				CString conText = Text->textString();
				
				if(CExcept.GetCheck()){
					if(chkArr.length() == 0){
						chkArr.append(Text->position());
						if(CZAxis.GetCheck())
							if(CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + pZ + conText + L"\n");
							else if(!CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
						else if(!CZAxis.GetCheck())
							if(CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + conText + L"\n");
							else if(!CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + L"\n");
					}
					else{
						if(chkArr.find(Text->position()) == -1){
							chkArr.append(Text->position());
							if(CZAxis.GetCheck())
							if(CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + pZ + conText + L"\n");
							else if(!CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
							else if(!CZAxis.GetCheck())
							if(CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + conText + L"\n");
							else if(!CContext.GetCheck())
								TextFile.WriteString(pX + Set + pY + Set + L"\n");
						}
					}
				}
				else if(CExcept.GetCheck()){
					if(CZAxis.GetCheck())
						if(CContext.GetCheck())
							TextFile.WriteString(pX + Set + pY + Set + pZ + conText + L"\n");
						else if(!CContext.GetCheck())
							TextFile.WriteString(pX + Set + pY + Set + pZ + L"\n");
						else if(!CZAxis.GetCheck())
						if(CContext.GetCheck())
							TextFile.WriteString(pX + Set + pY + Set + conText + L"\n");
						else if(!CContext.GetCheck())
							TextFile.WriteString(pX + Set + pY + Set + L"\n");
				}
				Text->close();
			}
		}
	}
	TextFile.Close();
}