#include "stdafx.h"
#include "FELCmd.h"

CFELCmd::CFELCmd(void) {
}

CFELCmd::~CFELCmd(void) {
}

void CFELCmd::LoadSetting()
{
	//모드 1.가로선  2.지시선	
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("JOBMODE"));
	if (tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0) {
		m_lineStat = 1;
		//OnBnClickedRadline();
	} else if (tempStr.CompareNoCase(_T("2")) == 0) {
		m_lineStat = 2;
		//OnBnClickedRadarrow();
	}

	//가로선길이
	tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("Length"));
	if (tempStr.CompareNoCase(_T("")) == 0) {
		m_Length = 100;
		//SetDlgItemText(IDC_EDTLENGTH, _T("100"));
	} else {
		m_Length = _wtoi(tempStr);
		//SetDlgItemText(IDC_EDTLENGTH, tempStr);
	}		

	//축척
	tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("Scale"));
	if (tempStr.CompareNoCase(_T("")) == 0) {
		m_Scale = 1000;
		//SetDlgItemText(IDC_EDTSCALE, _T("1000"));
	} else {
		m_Scale = _wtof(tempStr);
		//SetDlgItemText(IDC_EDTSCALE, tempStr);
	}

	//소수점
	tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("DotCnt"));
	if (tempStr.CompareNoCase(_T("")) == 0) {
		m_DotCnt = 3;
		//SetDlgItemText(IDC_EDTPOINT, _T("3"));
	} else {
		m_DotCnt = _wtoi(tempStr);
		//SetDlgItemText(IDC_EDTPOINT, tempStr);
	}
	//머리말
	tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("PreFace"));
	if (tempStr.CompareNoCase(_T("")) == 0) {
		m_PreFaceStr = L"EL=";
		//SetDlgItemText(IDC_EDTHEADER, _T("EL="));
	} else {
		m_PreFaceStr = tempStr;
		//SetDlgItemText(IDC_EDTHEADER, tempStr);
	}
	//글자높이
	tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("TextHeight"));
	if (tempStr.CompareNoCase(_T("")) == 0) {
		m_TextHeight_Str = L"0.300";
		//SetDlgItemText(IDC_EDTHEIGHT, _T("0.300"));
	} else {
		m_TextHeight_Str = tempStr;
		//SetDlgItemText(IDC_EDTHEIGHT, tempStr);
	}

	//도면층 보류
	//m_LayerStr = curLayerStyle(acdbHostApplicationServices()->workingDatabase()->clayer());
}

void CFELCmd::SaveSetting()
{
	// 모드 1.가로선  2.지시선	
	CString TempStr;
	TempStr.Format(L"%d", m_lineStat);
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("JOBMODE"), TempStr);
	// 축척
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("Scale"), m_ScaleStr);
	// 길이
	TempStr.Format(L"%d", m_Length);
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("Length"), TempStr);
	// 소수점
	TempStr.Format(L"%d", m_DotCnt);
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("DotCnt"), TempStr);
	// 머릿말
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("PreFace"), m_PreFaceStr);
	// 문자 높이
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("TextHeight"), m_TextHeight_Str);
}

CString CFELCmd::curLayerStyle(AcDbObjectId a_layId)
{
	CString str;
	AcDbLayerTableRecordPointer pLayerRcd(a_layId, AcDb::kForRead);
	TCHAR* TxtStyleStr;
	pLayerRcd->getName(TxtStyleStr);

	str.Format(L"%s", TxtStyleStr);

	return str;
}

void CFELCmd::command() 
{
	LoadSetting();
	// EL 높이
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("ELValue"));
	if (tempStr.CompareNoCase(_T("")) == 0)
		m_ELValue_str = _T("0.000");
	else
		m_ELValue_str = tempStr;

	double m_ELValue = _tstof(tempStr);
	AcDbLayerTable* pLayerTable;
	AcCmColor color;
	color.setColorIndex(2);
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL)
		return;
	pDb->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (pLayerTable->has(m_LayerStr) != Adesk::kTrue) {
		AcDbLayerTableRecord* pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(m_LayerStr);
		pLayerTableRecord->setColor(color);

		AcDbLinetypeTable* pLinetypeTable;
		pDb->getLinetypeTable(pLinetypeTable, AcDb::kForRead);

		AcDbObjectId linetypeId;
		pLinetypeTable->getAt(_T("BYLAYER"), linetypeId);
		pLayerTableRecord->setLinetypeObjectId(linetypeId);
		pLayerTable->add(pLayerTableRecord);
		pLayerTable->close();
		pLayerTableRecord->close();
	}
	else
		pLayerTable->close();

	int stat;
	TCHAR kWord[133];
	ads_name ens;
	AcGePoint3d ELpt;
	CString inString, EntStr, Scale_Print, Scale_Print2, RTERROR_str, S_ScaleStr, m_EL, strMsg;
	bool b_TF = true;

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while (b_TF) {
		acedInitGet(0, _T("c C s S"));
		strMsg.LoadStringW(IDS_FEL_BASEPT_SEL); // "기준점 EL 선택 또는 값 입력(%.3f) [축척 변경(C) 설정 변경(S)]: "
		EntStr.Format(strMsg, m_ELValue);
		strMsg.LoadStringW(IDS_SET_CMD_SCALE); // "\n< 현재설정 : 축척=%0.f >"

		// 표시 설정
		CString setStr;
		if (m_lineStat == 1) {
			setStr.LoadStringW(IDS_HORIZON_DRAW);
		} else if (m_lineStat = 2) {
			setStr.LoadStringW(IDS_LEADER_DRAW);
		}

		Scale_Print.Format(strMsg, setStr, m_ELValue, m_Scale, m_TextHeight_Str, m_DotCnt);
		strMsg.LoadStringW(IDS_SET_SCALE); // "< 현재설정 : 축척=%0.f >"
		Scale_Print2.Format(strMsg, m_Scale);
		acutPrintf(Scale_Print);
		/*ads_point TF_xform[4];
		struct resbuf* TF_ref;
		stat = acedNEntSel(EntStr, ens, asDblArray(ELpt), TF_xform, &TF_ref);*/
		stat = acedEntSel(EntStr, ens, asDblArray(ELpt));
		if (stat == RTCAN || stat == RTNONE) {
			b_TF = false;
			return;
		}
		else if (stat == RTKWORD) {
			CString	strKeyWord;
			acedGetInput(strKeyWord.GetBuffer(512), 512);

			if (strKeyWord.CompareNoCase(_T("S")) == 0 || strKeyWord.CompareNoCase(_T("s")) == 0) {
				if (IDOK == dlg.DoModal()) {
					m_lineStat = dlg.lineStat;
					m_Scale = dlg.Scale;
					m_Length = dlg.Length;
					m_DotCnt = dlg.DotCnt;
					m_LayerStr = dlg.m_LayerStr;
					m_PreFaceStr = dlg.m_PreFaceStr;
					m_TextHeight_Str = dlg.m_TextHeight_Str;
					//m_ELValue_str = dlg.m_ELValue_str;
				}
			} else if (strKeyWord.CompareNoCase(_T("C")) == 0 || strKeyWord.CompareNoCase(_T("c")) == 0) {
				strMsg.LoadStringW(IDS_FEL_INPUT_SCALE); // "\n축척 입력<%.0f> : "
				S_ScaleStr.Format(strMsg, m_Scale);
				if (acedGetReal(S_ScaleStr, &m_Scale) == RTNORM) {
					m_ScaleStr.Format(_T("%0.f"), m_Scale);
					iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("Scale"), m_ScaleStr);
				}
			} else {
				resbuf zValueStr;
				ads_getvar(_T("LASTPROMPT"), &zValueStr);
				RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
				RTERROR_str.Replace(EntStr, _T(""));
				if (RTERROR_str.CompareNoCase(_T("")) == 0) {
					break;
				} else {
					if (RTERROR_str.GetLength() > 0) {
						m_ELValue_str.Format(_T("%s"), RTERROR_str);
						iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("ELValue"), m_ELValue_str);
						break;
					} else {
						m_ELValue_str.Format(_T("%f"), m_ELValue);
						break;
					}
				}
			}
		} else if (stat == RTERROR) {
			resbuf zValueStr, zValueStr2;
			//strMsg.LoadStringW(IDS_CHECK_STRING); //"**선택한 객체는 문자가 아닙니다.**"
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
			//acedPrompt(RTERROR_str);
			RTERROR_str.Replace(EntStr, _T(""));
			ads_getvar(_T("ERRNO"), &zValueStr2);
			if (zValueStr2.resval.rint == 52)
				break;
			if (RTERROR_str.CompareNoCase(Scale_Print2) == 0)
				continue;
			else if (RTERROR_str.CompareNoCase(strMsg) == 0)
				continue;
			else {
				if (RTERROR_str.GetLength() > 0) {
					m_ELValue_str.Format(_T("%s"), RTERROR_str);
					m_ELValue_str = CheckInputNumber(m_ELValue_str);
					iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("ELValue"), m_ELValue_str);
					break;
				} else {
					m_ELValue_str.Format(_T("%f"), m_ELValue);
					break;
				}
			}
		} else if (stat == RTNORM) {
			AcGePoint3d insertPoint;
			AcDbObjectId objId;
			acdbGetObjectId(objId, ens);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if (pEnt.openStatus() == Acad::eOk) {
				if (pEnt->isKindOf(AcDbText::desc())) {
					AcDbText* pText = AcDbText::cast(pEnt);
					inString.Format(_T("%s"), pText->textString());
					inString = CheckInputNumber(inString);
					pText->close();
				} else if (pEnt->isKindOf(AcDbMText::desc())) {
					AcDbMText* pMtext = AcDbMText::cast(pEnt);
					inString.Format(_T("%s"), pMtext->contents());
					inString = CheckInputNumber(inString);
					pMtext->close();
				} else {
					strMsg.LoadStringW(IDS_CHECK_STRING_ENTER); // "\n**선택한 객체는 문자가 아닙니다.**\n"
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();
			acedSSFree(ens);
			m_ELValue_str = inString;
			iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("ELValue"), m_ELValue_str);
			b_TF = false;
		}
	}
	AcGePoint3d STpt, spt, ept;
	double StrNum = _tstof(m_ELValue_str);
	CString getptStr;
	strMsg.LoadStringW(IDS_FEL_BASE_SEL); // "\n기준점(%.3f)선택 : "
	getptStr.Format(strMsg, StrNum);
	if (acedGetPoint(NULL, getptStr, asDblArray(STpt)) != RTNORM)
		return;

	bool b_EL = true;
	bool H_Lead = true;
	double EL_Value, H_Scale, H_Height, H_Length, H_DotNum;
	H_Scale = m_Scale;			//축척
	H_Length = m_Length;			//길이
	H_DotNum = m_DotCnt;			//소수점
	H_Height = _tstof(m_TextHeight_Str);	//문자높이

	ads_name GetEn;
	while (b_EL) {
		strMsg.LoadStringW(IDS_FEL_EL_INPUT); // "\n표시할 EL 입력 : "
		stat = acedEntSel(strMsg, GetEn, asDblArray(ELpt));
		strMsg.Replace(L"\n", L"");
		if (stat == RTCAN || stat == RTNONE)
			b_EL = false;
		/*if (acedGetReal(strMsg, &EL_Value) != RTNORM)
			b_EL = false;*/
		else {
			if (stat == RTKWORD) {
				resbuf zValueStr;
				ads_getvar(_T("LASTPROMPT"), &zValueStr);
				RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
				RTERROR_str.Replace(EntStr, _T(""));
				if (RTERROR_str.CompareNoCase(_T("")) == 0)
					break;
				else {
					if (RTERROR_str.GetLength() > 0) {
						m_ELValue_str.Format(_T("%s"), RTERROR_str);
						//iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("ELValue"), m_ELValue_str);
						//break;
					} else {
						m_ELValue_str.Format(_T("%f"), m_ELValue);
						//break;
					}
				}
			} else if (stat == RTERROR) {
				resbuf zValueStr, zValueStr2;
				//strMsg.LoadStringW(IDS_CHECK_STRING); //"**선택한 객체는 문자가 아닙니다.**"
				ads_getvar(_T("LASTPROMPT"), &zValueStr);
				RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
				RTERROR_str.Replace(strMsg, _T(""));
				ads_getvar(_T("ERRNO"), &zValueStr2);
				if (zValueStr2.resval.rint == 52)
					break;
				if (RTERROR_str.CompareNoCase(Scale_Print2) == 0)
					continue;
				else if (RTERROR_str.CompareNoCase(strMsg) == 0)
					continue;
				else {
					if (RTERROR_str.GetLength() > 0) {
						m_ELValue_str.Format(_T("%s"), RTERROR_str);
						//iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("ELValue"), m_ELValue_str);
						EL_Value = _wtof(m_ELValue_str);
						//break;
					}
					else {
						m_ELValue_str.Format(_T("%f"), m_ELValue);
						EL_Value = m_ELValue;
						//break;
					}
				}
			} else if (stat == RTNORM) {
				AcGePoint3d insertPoint;
				AcDbObjectId objId;
				acdbGetObjectId(objId, GetEn);
				AcDbEntityPointer pEnt(objId, AcDb::kForRead);
				if (pEnt.openStatus() == Acad::eOk) {
					if (pEnt->isKindOf(AcDbText::desc())) {
						AcDbText* pText = AcDbText::cast(pEnt);
						inString.Format(_T("%s"), pText->textString());
						inString = CheckInputNumber(inString);
						pText->close();
					} else if (pEnt->isKindOf(AcDbMText::desc())) {
						AcDbMText* pMtext = AcDbMText::cast(pEnt);
						inString.Format(_T("%s"), pMtext->contents());
						inString = CheckInputNumber(inString);
						pMtext->close();
					} else {
						strMsg.LoadStringW(IDS_CHECK_STRING_ENTER); // "\n**선택한 객체는 문자가 아닙니다.**\n"
						acutPrintf(strMsg);
						continue;
					}
				}
				pEnt->close();
				acedSSFree(ens);
				//m_ELValue_str = inString;
				EL_Value = _wtof(inString);
				//iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("ELValue"), m_ELValue_str);
				//b_EL = false;
			}

			if (m_lineStat == 1) {
				AcGePoint3d Npt, pPt;
				AcGePoint3dArray ptArray;
				pPt.y = STpt.y + (EL_Value - StrNum) * 1000 / H_Scale;
				pPt.x = STpt.x;
				ads_polar(asDblArray(pPt), 0.0, 1000 / H_Scale * (H_Length / 2), asDblArray(spt));
				ads_polar(asDblArray(pPt), ads_angle(asDblArray(spt), asDblArray(pPt)), 1000 / H_Scale * (H_Length / 2), asDblArray(ept));
				ptArray.append(spt); ptArray.append(ept);
				bool m_pLine = CreatePLine(ptArray, 0, _T("BYBLOCK"), 1, m_LayerStr);
				ptArray.removeAll();
				if (m_pLine == false)
					b_EL = false;

				CString HTxtStr, last;
				int H_DecimalNum = H_DotNum;
				last.Format(_T("%s%%.%df"), m_PreFaceStr, H_DecimalNum);
				HTxtStr.Format(last, EL_Value/* * m_Scale / 1000*/);
				Npt.y = STpt.y + 1000 / H_Scale * (EL_Value - StrNum + 0.1);
				Npt.x = STpt.x;

				bool m_Text = CreateText(Npt, 1000 / H_Scale * H_Height, 0.0, HTxtStr, m_LayerStr, 1);
				if (m_Text == false)
					b_EL = false;
			}
			else if (m_lineStat == 2) {
				AcGePoint3d Lpt, ArrowPt1, ArrowPt2, LinePt1, LinePt2;
				AcGePoint3dArray LptArray;
				double H_DotLength = 0;

				CString LTxtstr, Ltxt;
				int H_DecimalNum = H_DotNum;
				Ltxt.Format(_T("%s%%.%df"), m_PreFaceStr, H_DecimalNum);
				LTxtstr.Format(Ltxt, EL_Value);
				Lpt.y = STpt.y + 1000 / H_Scale * (EL_Value - StrNum + 1.1) + 1000 / H_Scale * (H_Length - 1);
				Lpt.x = STpt.x;

				bool m_LText = CreateText(Lpt, 1000 / H_Scale * H_Height, 0.0, LTxtstr, m_LayerStr, 2);
				if (m_LText == false)
					b_EL = false;

				Lpt.y = STpt.y + (EL_Value - StrNum) * 1000 / H_Scale;
				Lpt.x = STpt.x;
				acutPolar(asDblArray(Lpt), DTR(90.0), 1000 / H_Scale * 0.3, asDblArray(ArrowPt1));
				acutPolar(asDblArray(ArrowPt1), DTR(180.0), 1000 / H_Scale * 0.1, asDblArray(ArrowPt2));
				bool m_Solid = CreateSolid(Lpt, ArrowPt1, ArrowPt2, m_LayerStr);
				if (m_Solid == false)
					b_EL = false;

				Lpt.y = STpt.y + (EL_Value - StrNum) * 1000 / H_Scale;
				Lpt.x = STpt.x;
				if (H_DotNum == 0)
					H_DotLength = (LTxtstr.GetLength() * 0.2331 + 0.2) * 1000 / H_Scale;
				else if (H_DotNum > 0)
					H_DotLength = ((LTxtstr.GetLength() - 1) * 0.2331 + 0.1164 + 0.2) * 1000 / H_Scale;

				acutPolar(asDblArray(Lpt), DTR(90.0), 1000 / H_Scale * H_Length, asDblArray(LinePt1));
				acutPolar(asDblArray(LinePt1), 0.0, H_DotLength, asDblArray(LinePt2));
				LptArray.append(Lpt); LptArray.append(LinePt1); LptArray.append(LinePt2);
				bool m_LPline = CreatePLine(LptArray, 0, _T("BYBLOCK"), 1, m_LayerStr);
				LptArray.removeAll();
				if (m_LPline == false)
					b_EL = false;
			}
		}
	}
	SaveSetting();
}

//숫자 및 점(.) 입력 체크
CString CFELCmd::CheckInputNumber(CString str) {
	TCHAR* getAtChar;
	CString strNum, strRe = _T("");
	int k = 0, isNum = -1;
	for (int i = 0; i < str.GetLength(); i++) {
		strNum = str.Mid(i, 1);
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		if (((ascii >= 48 && ascii <= 57) || ascii == 45 || ascii == 46)) {
			if (isNum == -1 || isNum == 1) {
				isNum = 1;
				strRe.Append(getAtChar);
			} else {
				if (strRe.IsEmpty()) {
					isNum = 1;
					strRe.Append(getAtChar);
				} else break;
			}
		} else {
			isNum = 2;
		}
	}

	if (strRe.GetAt(0) == '.') // 맨앞에 .이면 삭제
		strRe = strRe.Mid(1, strRe.GetLength() - 1);

	return strRe;
}

double CFELCmd::DTR(double angle) {
	return (angle / 180.0 * PI);
}

////라디안 값을 각도로 변환하여 리턴한다.
double CFELCmd::RTD(double radian) {
	return (radian * 180.0 / PI);
}

//폴리선 생성
bool CFELCmd::CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer) {
	Acad::ErrorStatus es;
	AcDbObjectId plineObjId;
	AcDbBlockTable* pBT;
	AcDbBlockTableRecord* pBTR;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();

	if (pDb == NULL)
		return false;

	AcDbPolyline* pPLine = new AcDbPolyline();
	for (int i = 0; i < ptArray.length(); i++) {
		pPLine->addVertexAt(i, AcGePoint2d(ptArray[i].x, ptArray[i].y));
	}

	pPLine->setConstantWidth(LineWidth);
	pPLine->setLinetype(LineType);
	pPLine->setLinetypeScale(LineTypeScale);
	pPLine->setLayer(Layer);

	if (pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk) {
		// [kyh] 현재 작업창
		if (acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite) == Acad::eOk) {
			//if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
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

//텍스트생성 mode = 1 (가로선) mode = 2 (지시선)
bool CFELCmd::CreateText(AcGePoint3d pt, double Height, double Rotate, CString TxtStr, CString Layer, int mode) {
	Acad::ErrorStatus es;
	AcDbObjectId textStyleId, pTextObjId;
	double XScale = 0, obliquingAngle = 0;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL)
		return false;

	AcDbText* pText = new AcDbText();
	textStyleId = pDb->textstyle();
	pText->setTextStyle(textStyleId);
	AcDbTextStyleTableRecord* pTSTR = NULL;
	AcDbBlockTable* pBT;
	AcDbBlockTableRecord* pBTR;
	acdbOpenObject(pTSTR, textStyleId, AcDb::kForRead);
	if (pTSTR) {
		XScale = pTSTR->xScale();
		obliquingAngle = pTSTR->obliquingAngle();
		pTSTR->close();
		pText->setWidthFactor(XScale);
	}

	pText->setOblique(obliquingAngle);
	pText->setLayer(Layer);
	pText->setPosition(pt);
	if (mode == 1) {
		es = pText->setVerticalMode(AcDb::kTextBottom);
		es = pText->setHorizontalMode(AcDb::kTextCenter);
	}
	else {
		es = pText->setVerticalMode(AcDb::kTextBase);
		es = pText->setHorizontalMode(AcDb::kTextLeft);
	}
	pText->setAlignmentPoint(pt);
	pText->setHeight(Height);
	pText->setRotation(Rotate);
	pText->setTextString((LPCTSTR)TxtStr);

	if (pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk) {
		// [kyh] 현재 작업창
		if (acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite) == Acad::eOk) {
			//if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(pTextObjId, pText);
			pBTR->close();
			pText->close();
		}
		pBT->close();
	}
	return true;
}

//솔리드 생성
bool CFELCmd::CreateSolid(AcGePoint3d Solpt1, AcGePoint3d Solpt2, AcGePoint3d Solpt3, CString Layer) {
	Acad::ErrorStatus es;
	AcDbObjectId SolidId;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (pDb == NULL)
		return false;

	AcDbBlockTable* pBT;
	AcDbBlockTableRecord* pBTR = NULL;

	AcDbSolid* Arrow = new AcDbSolid(Solpt1, Solpt2, Solpt3, Solpt3);
	Arrow->setLayer(Layer);

	if (pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk) {
		if (acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite) == Acad::eOk) {
			es = pBTR->appendAcDbEntity(SolidId, Arrow);
			pBTR->close();
			Arrow->close();
		}
		/*if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(SolidId, Arrow);
			pBTR->close();
			Arrow->close();
		}*/
		pBT->close();
	}
	return true;
}
