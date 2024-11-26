#include "StdAfx.h"
#include "DLT.h"

CDLT::CDLT(void){
	#ifdef _ACAD
		CADLinetypeStr = L"\\CADLT.LIN";
	#elif _ZCAD
		CADLinetypeStr = L"\\ZWCADLT.LIN";
	#elif _ZXYCAD
		CADLinetypeStr = L"\\CADLT.LIN";
	#endif
}

CDLT::~CDLT(void){
}

void CDLT::DrainLineType(){
	int up_cnt = 0 , k = 0,osmode = 0,OldColor=0, state;
	CString LineType;
	ads_name pcs,ename1,up_pcs;
	resbuf rb;
	//AcGePoint3d spt,ept;
	AcDbObjectId objID;
	AcDbObjectIdArray IdArray;
	TCHAR Type[30];
	Adesk::Int32 sslen = 0;
	CString LT, str_LT, strResource, LTString, vvalue, CODE, SCstring, SCset, TextStyle, UserInput, strMsg;
	double LTS = 0.0, Mirror = 0.0;

	CString strPath;
	strPath = g_modulePath;
	int findNUM = strPath.ReverseFind('\\');
	strPath.Delete(findNUM, strPath.GetLength() - 1);

	//CString strLINFile = strPath + _T("\\ZWCADLT.LIN");
	CString strLINFile = strPath + CADLinetypeStr;

	//ads_command(RTSTR, _T("UNDO"), RTSTR, _T("BE"), RTNONE);
	getSysVar(_T("TEXTSTYLE"), TextStyle);
	getSysVar(_T("OSMODE"), osmode);
	setSysVar(_T("OSMODE"), 0);
	
	strMsg.LoadStringW(IDS_LTYPE_CHANGE_SEL); // "\n라인 타입을 변경할 객체 선택 : "
	acutPrintf(strMsg);
	strMsg.LoadStringW(IDS_SELECT_ENTITY);
	const wchar_t* prompts[2] = { strMsg, L"", };
	//if(acedSSGet(NULL, NULL, NULL, NULL, pcs) != RTNORM){
	if(acedSSGet(L":$", prompts, NULL, NULL, pcs) != RTNORM){
		acedSSFree(pcs);
		setSysVar(_T("TEXTSTYLE"), TextStyle);
		setSysVar(_T("OSMODE"), osmode);		
		return;
	}
	if(acedSSLength(pcs, &sslen) != RTNORM){
		acedSSFree(pcs);
		setSysVar(_T("TEXTSTYLE"), TextStyle);
		setSysVar(_T("OSMODE"), osmode);		
		return;
	}

	for(int i = 0; i < sslen; i++){
		if(acedSSName(pcs, i ,ename1) == RTNORM) {
			arx_setObList(ename1, -1, true);
			up_cnt++;
			acedSSFree(ename1);
		}
	}

	acedInitGet(0, _T("c C"));
	strMsg.LoadStringW(IDS_DLT_TYPE); // "\n형식입력 = 토사(T)/L~L15/U~U15/V~V15/J~J5/다이크(D~D5)/산마루(B~B5)/맹암거(M~M10)/종배수(LO~LO5)/소단(G~G5)/용수로(Y~Y18)/플륨관(F~F5): "
	state = acedGetString(TRUE, strMsg, Type);
	if(state != RTNORM){
		setSysVar(_T("TEXTSTYLE"), TextStyle);
		setSysVar(_T("OSMODE"), osmode);		
		return;
	} 
	//else if (state == RTNORM) {
	//	CString	strKeyWord;
	//	strKeyWord.Format(L"%s", Type);
	//	bUserInput = false;
	//	//acedGetInput(strKeyWord.GetBuffer(512), 512);
	//	if (strKeyWord.CompareNoCase(_T("C")) == 0 || strKeyWord.CompareNoCase(_T("c")) == 0) { // 사용자 입력
	//		strMsg.LoadStringW(IDS_USER_DLT_TYPE);
	//		CString UserStr = iniFile.readINI(_T("config.ini"), _T("DLT"), _T("USER"));
	//		if (UserStr.CompareNoCase(_T("")) == 0) {
	//			strMsg = strMsg + L" : "; // "\n사용자 측구 형식 입력" + L" : ";
	//		} else {
	//			strMsg = strMsg + L"(" + UserStr + L")" + L" : "; // "\n사용자 측구 형식 입력(값) :"
	//		}
	//		state = acedGetString(TRUE, strMsg, Type);
	//		bUserInput = true;
	//		CString strUser;
	//		strUser.Format(L"%s", Type);
	//		if (!strUser.IsEmpty())
	//			iniFile.writeINI(_T("config.ini"), _T("DLT"), _T("USER"), strUser);
	//	}
	//}

	CString scaleStr = iniFile.readINI(_T("config.ini"), _T("DLT"), _T("SCALE"));
	if(scaleStr.CompareNoCase(_T("")) == 0){	
		vvalue = _T("1.000");
	}
	else{
		vvalue = scaleStr;
	}

	TCHAR *Kword;
	Kword = (TCHAR*)(LPCTSTR)_T("n");
	CODE = LineTypeLoad(Type, Kword);//코드 목록
	TCHAR *LTcode = (TCHAR*)(LPCTSTR)CODE;

	for(int i = 0; i < up_cnt; i++)
	{
		arx_getObList(up_pcs, i, true);
// 		spt = lf_3DReturnList(up_pcs, 10);
// 		ept = lf_3DReturnList(up_pcs, 11);

		Acad::ErrorStatus es;
		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
		TCHAR szLtFile[MAX_PATH], *str;

		//CString strPath;
		//iniFile.GetRegistry(HKEY_CURRENT_USER, _T("RoamableRootFolder"), strPath);

		//iniFile.GetRegistry(HKEY_CURRENT_USER, _T("Path"), strPath);

		//iniFile.GetRegistry(HKEY_CURRENT_USER, _T("Path"), strPath);

		//iniFile.GetRegistry(HKEY_CURRENT_USER, _T("Path"), strPath);
		/*strPath = g_modulePath;
		int findNUM = strPath.ReverseFind('\\');
		strPath.Delete(findNUM, strPath.GetLength() - 1);*/
		

		//CString strLINFile = strPath + _T("\\ZWCADLT.LIN");
		//CString strLINFile = strPath + CADLinetypeStr;
		//acutPrintf(L"\n%s\n", strLINFile);

		if(RTNORM != acedFindFile(strLINFile, szLtFile)){
			setSysVar(_T("TEXTSTYLE"), TextStyle);
			setSysVar(_T("OSMODE"), osmode);
			return ;
		}

		AcDbLinetypeTable *pLtTable = NULL;
		es = pDb->getLinetypeTable(pLtTable,AcDb::kForRead);
		ACHAR *szLtype = LTcode;
		bool isLinetypeLoaded = pLtTable->has(szLtype);
		es = pLtTable->close();

		AcDbDatabase *pTempDatabase = new  AcDbDatabase(true,false);
		es = pTempDatabase->loadLineTypeFile(szLtype, szLtFile);
		if(Acad::eOk == es){
			AcDbLinetypeTable *pTempLtTable;
			AcDbLinetypeTableRecord *pTempLtRec=NULL;

			es = pTempDatabase->getLinetypeTable(pTempLtTable,AcDb::kForRead);
			AcDbObjectId ltRecId = AcDbObjectId::kNull;
			es = pTempLtTable->getAt(szLtype, ltRecId);
			pTempLtTable->close();

			AcDbObjectIdArray objIdArray;
			objIdArray.append(ltRecId);

			AcDbIdMapping idMap;
			es = pDb->wblockCloneObjects(objIdArray, pDb->linetypeTableId(), idMap, AcDb::kDrcReplace);	
		}
		delete pTempDatabase;

		ads_name ads_LT;
		AcDbObjectId Newid;
		acdbGetObjectId(Newid, up_pcs);


		AcDbCurve* pLine;
		acdbOpenObject(pLine,Newid,AcDb::kForWrite);
		if(pLine !=NULL){
			pLine->setLinetype(LTcode);
			pLine->close();
		}


	}//for end

	//축척
	strMsg.LoadStringW(IDS_INPUT_SCALE); // "\n축척입력<%s> : "
	strResource.Format(strMsg, vvalue);
	if(acedGetReal(strResource, &LTS) ==RTCAN){
		setSysVar(_T("TEXTSTYLE"),TextStyle);
		setSysVar(_T("OSMODE"), osmode);
		return;
	}

	if(LTS == 0.0)	LTS = _wtof(vvalue);
	SCset.Format(_T("%.1f"), LTS);
	iniFile.writeINI(_T("config.ini"), _T("DLT"), _T("SCALE"), SCset);

	for(int i = 0; i < up_cnt; i++){
		arx_getObList(up_pcs,i,true);
// 		spt = lf_3DReturnList(up_pcs, 10);
// 		ept = lf_3DReturnList(up_pcs, 11);

		ads_name ads_LT;
		AcDbObjectId Newid;
		acdbGetObjectId(Newid, up_pcs);

		
		AcDbCurve* pLine;
		acdbOpenObject(pLine,Newid,AcDb::kForWrite);
		if(pLine !=NULL){
			pLine->setLinetypeScale(LTS);
			pLine->close();
		}
	}//for end

	//반전
	acedInitGet(RSG_NOZERO, _T("Y N"));
	strMsg.LoadStringW(IDS_CHOICE_REVERSE);  // "\n반전?(Y/N)<N>: "
	if(acedGetReal(strMsg, &Mirror) == RTCAN){
		setSysVar(_T("TEXTSTYLE"), TextStyle);
		setSysVar(_T("OSMODE"), osmode);
		return;
	}
	TCHAR RKword[20];
	CString YKey;
	acedGetInput(RKword);
	YKey.Format(_T("%s"), RKword);

	if(YKey == _T("y") || YKey == _T("Y")){
		CODE = LineTypeLoad(Type, RKword);//코드 목록
		TCHAR *LTcode = (TCHAR*)(LPCTSTR)CODE;
		for(int i = 0; i < up_cnt; i++){
			arx_getObList(up_pcs, i, true);
// 			spt = lf_3DReturnList(up_pcs, 10);
// 			ept = lf_3DReturnList(up_pcs, 11);

			Acad::ErrorStatus es;

			AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
			TCHAR szLtFile[MAX_PATH], *str;

			//CString strPath;
			//iniFile.GetRegistry(HKEY_CURRENT_USER, _T("Path"), strPath);
			//CString strLINFile = strPath+_T("\\ZWCADLT.LIN");
			CString strLINFile = strPath + CADLinetypeStr;

			if(RTNORM != acedFindFile(strLINFile, szLtFile)){
				setSysVar(_T("TEXTSTYLE"), TextStyle);
				setSysVar(_T("OSMODE"), osmode);
				return ;
			}

			AcDbLinetypeTable *pLtTable = NULL;
			es = pDb->getLinetypeTable(pLtTable, AcDb::kForRead);
			ACHAR *szLtype = LTcode;
			bool isLinetypeLoaded = pLtTable->has(szLtype);
			es = pLtTable->close();

			AcDbDatabase *pTempDatabase = new  AcDbDatabase(true,false);
			es = pTempDatabase->loadLineTypeFile(szLtype, szLtFile);
			if (Acad::eOk == es){
				AcDbLinetypeTable *pTempLtTable;
				AcDbLinetypeTableRecord *pTempLtRec=NULL;

				es = pTempDatabase->getLinetypeTable(pTempLtTable, AcDb::kForRead);
				AcDbObjectId ltRecId = AcDbObjectId::kNull;
				es = pTempLtTable->getAt(szLtype, ltRecId);
				pTempLtTable->close();

				AcDbObjectIdArray objIdArray;
				objIdArray.append(ltRecId);

				AcDbIdMapping idMap;
				es = pDb->wblockCloneObjects(objIdArray, pDb->linetypeTableId(), idMap, AcDb::kDrcReplace);	
				pTempLtRec->close();
			}
			/*
			TCHAR* Font;
			int bold = 0, italic = 0, charset = 0, pitchfamily = 0;

			AcDbTextStyleTable* pTST;
			AcDbTextStyleTableRecord *pTSTR = NULL;
			es = pTempDatabase->getTextStyleTable(pTST, AcDb::kForRead);
			es = pTSTR->font(Font, bold, italic, charset, pitchfamily);
			pTSTR->close();
			pTST->close();
			*/
			delete pTempDatabase;

			ads_name ads_LT;
			AcDbObjectId Newid;
			acdbGetObjectId(Newid, up_pcs);

			AcDbCurve* pLine;
			acdbOpenObject(pLine,Newid, AcDb::kForWrite);
			pLine->setLinetypeScale(LTS);
			pLine->setLinetype(LTcode);
			pLine->close();
		}//for end
	}

	acedSSFree(up_pcs);//메모리해제
	//ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	setSysVar(_T("TEXTSTYLE"), TextStyle);	
	setSysVar(_T("OSMODE"), osmode);
}

//기존 객체 환경(ent:객체명, Mode:오배수관(3,4) 구별을 위한 변수)
CString CDLT::LineTypeLoad(TCHAR *Type, TCHAR *Mirror){
	CString str_code, str_mirror,str_type;

	str_type = (LPCTSTR)Type;
	str_mirror = (LPCTSTR)Mirror;

	// [개발 중]
	/*
	if (bUserInput) { // 사용자 측구 형식 입력
		if (str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")) {
			str_code = str_type + _T("00");
		} else if (str_mirror == _T("Y") || str_mirror == _T("y")) {
			str_code = str_type + _T("00R");
		}
		return str_code; // 바로 리턴
	}
	*/

	//토사측구 L형측구
	if(str_type == _T("T") || str_type == _T("t") || str_type == _T("")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCST00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCST00R");
		}
	}else if(str_type == _T("L") || str_type == _T("l")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL00R");
		}
	}else if(str_type == _T("L1") || str_type == _T("l1")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL01");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL01R");
		}
	}else if(str_type == _T("L2") || str_type == _T("l2")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL02");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL02R");
		}
	}else if(str_type == _T("L3") || str_type == _T("l3")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL03");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL03R");
		}
	}else if(str_type == _T("L4") || str_type == _T("l4")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL04");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL04R");
		}
	}else if(str_type == _T("L5") || str_type == _T("l5")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL05");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL05R");
		}
	}else if(str_type == _T("L6") || str_type == _T("l6")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL06");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL06R");
		}
	}else if(str_type == _T("L7") || str_type == _T("l7")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL07");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL07R");
		}
	}else if(str_type == _T("L8") || str_type == _T("l8")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL08");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL08R");
		}
	}else if(str_type == _T("L9") || str_type == _T("l9")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL09");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL09R");
		}
	}else if(str_type == _T("L10") || str_type == _T("l10")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL10");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL10R");
		}
	}else if(str_type == _T("L11") || str_type == _T("l11")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL11");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL11R");
		}
	}else if(str_type == _T("L12") || str_type == _T("l12")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL12");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL12R");
		}
	}else if(str_type == _T("L13") || str_type == _T("l13")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL13");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL13R");
		}
	}else if(str_type == _T("L14") || str_type == _T("l14")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL14");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL14R");
		}
	}else if(str_type == _T("L15") || str_type == _T("l15")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSL15");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSL15R");
		}
	}
	//U형측구
	if(str_type == _T("U") || str_type == _T("u")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU00R");
		}
	}else if(str_type == _T("U1") || str_type == _T("u1")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU01");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU01R");
		}
	}else if(str_type == _T("U2") || str_type == _T("u2")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU02");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU02R");
		}
	}else if(str_type == _T("U3") || str_type == _T("u3")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU03");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU03R");
		}
	}else if(str_type == _T("U4") || str_type == _T("u4")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU04");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU04R");
		}
	}else if(str_type == _T("U5") || str_type == _T("u5")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU05");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU05R");
		}
	}else if(str_type == _T("U6") || str_type == _T("u6")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU06");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU06R");
		}
	}else if(str_type == _T("U7") || str_type == _T("u7")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU07");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU07R");
		}
	}else if(str_type == _T("U8") || str_type == _T("u8")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU08");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU08R");
		}
	}else if(str_type == _T("U9") || str_type == _T("u9")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU09");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU09R");
		}
	}else if(str_type == _T("U10") || str_type == _T("u10")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU10");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU10R");
		}
	}else if(str_type == _T("U11") || str_type == _T("u11")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU11");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU11R");
		}
	}else if(str_type == _T("U12") || str_type == _T("u12")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU12");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU12R");
		}
	}else if(str_type == _T("U13") || str_type == _T("u13")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU13");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU13R");
		}
	}else if(str_type == _T("U14") || str_type == _T("u14")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU14");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU14R");
		}
	}else if(str_type == _T("U15") || str_type == _T("u15")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSU15");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSU15R");
		}
	}
	//V형측구
	if(str_type == _T("V") || str_type == _T("v")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV00R");
		}
	}else if(str_type == _T("V1") || str_type == _T("v1")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV01");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV01R");
		}
	}else if(str_type == _T("V2") || str_type == _T("v2")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV02");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV02R");
		}
	}else if(str_type == _T("V3") || str_type == _T("v3")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV03");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV03R");
		}
	}else if(str_type == _T("V4") || str_type == _T("v4")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV04");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV04R");
		}
	}else if(str_type == _T("V5") || str_type == _T("v5")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV05");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV05R");
		}
	}else if(str_type == _T("V6") || str_type == _T("v6")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV06");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV06R");
		}
	}else if(str_type == _T("V7") || str_type == _T("v7")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV07");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV07R");
		}
	}else if(str_type == _T("V8") || str_type == _T("v8")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV08");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV08R");
		}
	}else if(str_type == _T("V9") || str_type == _T("v9")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV09");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV09R");
		}
	}else if(str_type == _T("V10") || str_type == _T("v10")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV10");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV10R");
		}
	}else if(str_type == _T("V11") || str_type == _T("v11")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV11");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV11R");
		}
	}else if(str_type == _T("V12") || str_type == _T("v12")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV12");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV12R");
		}
	}else if(str_type == _T("V13") || str_type == _T("v13")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV13");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV13R");
		}
	}else if(str_type == _T("V14") || str_type == _T("v14")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV14");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV14R");
		}
	}else if(str_type == _T("V15") || str_type == _T("v15")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSV15");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSV15R");
		}
	}
	//J형측구
	if(str_type == _T("J") || str_type == _T("j")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSJ00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSJ00R");
		}
	}else if(str_type == _T("J1") || str_type == _T("j1")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSJ01");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSJ01R");
		}
	}else if(str_type == _T("J2") || str_type == _T("j2")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSJ02");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSJ02R");
		}
	}else if(str_type == _T("J3") || str_type == _T("j3")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSJ03");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSJ03R");
		}
	}else if(str_type == _T("J4") || str_type == _T("j4")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSJ04");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSJ04R");
		}
	}else if(str_type == _T("J5") || str_type == _T("j5")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSJ05");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSJ05R");
		}
	}
	//다이크
	if(str_type == _T("D") || str_type == _T("d")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSD00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSD00R");
		}
	}else if(str_type == _T("D1") || str_type == _T("d1")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSD01");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSD01R");
		}
	}else if(str_type == _T("D2") || str_type == _T("d2")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSD02");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSD02R");
		}
	}else if(str_type == _T("D3") || str_type == _T("d3")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSD03");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSD03R");
		}
	}else if(str_type == _T("D4") || str_type == _T("d4")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSD04");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSD04R");
		}
	}else if(str_type == _T("D5") || str_type == _T("d5")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSD05");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSD05R");
		}
	}
	//산마루
	if(str_type == _T("B") || str_type == _T("b")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSB00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSB00R");
		}
	}else if(str_type == _T("B1") || str_type == _T("b1")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSB01");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSB01R");
		}
	}else if(str_type == _T("B2") || str_type == _T("b2")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSB02");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSB02R");
		}
	}else if(str_type == _T("B3") || str_type == _T("b3")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSB03");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSB03R");
		}
	}else if(str_type == _T("B4") || str_type == _T("b4")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSB04");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSB04R");
		}
	}else if(str_type == _T("B5") || str_type == _T("b5")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSB05");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSB05R");
		}
	}
	//맹암거
	if(str_type == _T("M") || str_type == _T("m")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSM00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSM00R");
		}
	}else if(str_type == _T("M1") || str_type == _T("m1")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSM01");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSM01R");
		}
	}else if(str_type == _T("M2") || str_type == _T("m2")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSM02");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSM02R");
		}
	}else if(str_type == _T("M3") || str_type == _T("m3")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSM03");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSM03R");
		}
	}else if(str_type == _T("M4") || str_type == _T("m4")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSM04");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSM04R");
		}
	}else if(str_type == _T("M5") || str_type == _T("m5")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSM05");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSM05R");
		}
	}else if(str_type == _T("M6") || str_type == _T("m6")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSM06");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSM06R");
		}
	}else if(str_type == _T("M7") || str_type == _T("m7")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSM07");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSM07R");
		}
	}else if(str_type == _T("M8") || str_type == _T("m8")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSM08");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSM08R");
		}
	}else if(str_type == _T("M9") || str_type == _T("m9")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSM09");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSM09R");
		}
	}else if(str_type == _T("M10") || str_type == _T("m10")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSM10");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSM10R");
		}
	}
	//종배수관
	if(str_type == _T("LO") || str_type == _T("lo") || str_type == _T("Lo") || str_type == _T("lO")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSO00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSO00R");
		}
	}else if(str_type == _T("LO1") || str_type == _T("lo1") || str_type == _T("Lo1") || str_type == _T("lO1")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSO01");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSO01R");
		}
	}else if(str_type == _T("LO2") || str_type == _T("lo2") || str_type == _T("Lo2") || str_type == _T("lO2")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSO02");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSO02R");
		}
	}else if(str_type == _T("LO3") || str_type == _T("lo3") || str_type == _T("Lo3") || str_type == _T("lO3")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSO03");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSO03R");
		}
	}else if(str_type == _T("LO4") || str_type == _T("lo4") || str_type == _T("Lo4") || str_type == _T("lO4")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSO04");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSO04R");
		}
	}else if(str_type == _T("LO5") || str_type == _T("lo5") || str_type == _T("Lo5") || str_type == _T("lO5")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSO05");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSO05R");
		}
	}
	//소단측구
	if(str_type == _T("G") || str_type == _T("g")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSG00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSG00R");
		}
	}else if(str_type == _T("G1") || str_type == _T("g1")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSG01");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSG01R");
		}
	}else if(str_type == _T("G2") || str_type == _T("g2")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSG02");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSG02R");
		}
	}else if(str_type == _T("G3") || str_type == _T("g3")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSG03");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSG03R");
		}
	}else if(str_type == _T("G4") || str_type == _T("g4")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSG04");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSG04R");
		}
	}else if(str_type == _T("G5") || str_type == _T("g5")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSG05");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSG05R");
		}
	}
	//용수로
	if(str_type == _T("Y") || str_type == _T("y")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY00R");
		}
	}else if(str_type == _T("Y1") || str_type == _T("y1")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY01");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY01R");
		}
	}else if(str_type == _T("Y2") || str_type == _T("y2")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY02");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY02R");
		}
	}else if(str_type == _T("Y3") || str_type == _T("y3")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY03");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY03R");
		}
	}else if(str_type == _T("Y4") || str_type == _T("y4")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY04");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY04R");
		}
	}else if(str_type == _T("Y5") || str_type == _T("y5")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY05");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY05R");
		}
	}else if(str_type == _T("Y6") || str_type == _T("y6")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY06");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY06R");
		}
	}else if(str_type == _T("Y7") || str_type == _T("y7")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY07");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY07R");
		}
	}else if(str_type == _T("Y8") || str_type == _T("y8")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY08");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY08R");
		}
	}else if(str_type == _T("Y9") || str_type == _T("y9")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY09");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY09R");
		}
	}else if(str_type == _T("Y10") || str_type == _T("y10")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY10");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY10R");
		}
	}else if(str_type == _T("Y11") || str_type == _T("y11")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY11");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY11R");
		}
	}else if(str_type == _T("Y12") || str_type == _T("y12")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY12");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY12R");
		}
	}else if(str_type == _T("Y13") || str_type == _T("y13")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY13");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY13R");
		}
	}else if(str_type == _T("Y14") || str_type == _T("y14")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY14");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY14R");
		}
	}else if(str_type == _T("Y15") || str_type == _T("y15")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY15");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY15R");
		}
	}else if(str_type == _T("Y16") || str_type == _T("y16")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY16");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY16R");
		}
	}else if(str_type == _T("Y17") || str_type == _T("y17")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY17");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY17R");
		}
	}else if(str_type == _T("Y18") || str_type == _T("y18")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSY18");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSY18R");
		}
	}
	//플륨관
	if(str_type == _T("F") || str_type == _T("f")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSF00");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSF00R");
		}
	}else if(str_type == _T("F1") || str_type == _T("f1")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSF01");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSF01R");
		}
	}else if(str_type == _T("F2") || str_type == _T("f2")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSF02");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSF02R");
		}
	}else if(str_type == _T("F3") || str_type == _T("f3")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSF03");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSF03R");
		}
	}else if(str_type == _T("F4") || str_type == _T("f4")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSF04");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSF04R");
		}
	}else if(str_type == _T("F5") || str_type == _T("f5")){
		if(str_mirror == _T("N") || str_mirror == _T("n") || str_mirror == _T("")){
			str_code = _T("KDCSF05");
		}else if(str_mirror == _T("Y") || str_mirror == _T("y")){
			str_code = _T("KDCSF05R");
		}
	}	

	return str_code;
}

Acad::ErrorStatus CDLT::setVar(LPCTSTR varName, const resbuf* newVal){
	CString str;
	if (ads_setvar(varName, newVal) != RTNORM) {
		str.Format(_T("Could not set system variable \"%s\"."), varName);
		return(Acad::eInvalidInput);
	}
	else {
		return(Acad::eOk);
	}
}

Acad::ErrorStatus CDLT::getSysVar(LPCTSTR varName, int& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTSHORT);
		val = rb.resval.rint;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

Acad::ErrorStatus CDLT::getSysVar(LPCTSTR varName, double& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTREAL);
		val = rb.resval.rreal;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

Acad::ErrorStatus CDLT::getSysVar(LPCTSTR varName, CString& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTSTR);
		val = rb.resval.rstring;
		free(rb.resval.rstring);
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

Acad::ErrorStatus CDLT::getSysVar(LPCTSTR varName, AcGePoint2d& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTPOINT);
		val.set(rb.resval.rpoint[X], rb.resval.rpoint[Y]);
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

Acad::ErrorStatus CDLT::getSysVar(LPCTSTR varName, AcGePoint3d& val){
	resbuf rb;
	if (ads_getvar(varName, &rb) == RTNORM) {
		ASSERT(rb.restype == RT3DPOINT);
		val.set(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]);
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

Acad::ErrorStatus CDLT::setSysVar(LPCTSTR varName, int val){
	ASSERT(varName != NULL);

	resbuf rb;
	rb.restype = RTSHORT;
	rb.resval.rint = (short)val;

	return(setVar(varName, &rb));
}

Acad::ErrorStatus CDLT::setSysVar(LPCTSTR varName, double val){
	ASSERT(varName != NULL);

	resbuf rb;
	rb.restype = RTREAL;
	rb.resval.rreal = val;

	return(setVar(varName, &rb));
}

Acad::ErrorStatus CDLT::setSysVar(LPCTSTR varName, const TCHAR* val){
	ASSERT(varName != NULL);

	resbuf rb;
	rb.restype = RTSTR;
	rb.resval.rstring = const_cast<TCHAR*>(val);

	return(setVar(varName, &rb));
}

void CDLT::arx_setObList(ads_name enn, int index, bool chk){
	AcDbObjectId objId;

	if(acdbGetObjectId(objId, enn) != Acad::eOk)
		return;
	if(chk && index == -1){
		uparry.append(objId);
	}else if(!chk && index == -1){
		downarry.append(objId);
	}else if(chk && index != -1){
		uparry.setAt(index,objId);
	}else if(!chk && index != -1){
		downarry.setAt(index,objId);
	}
}


void CDLT::arx_getObList(ads_name& enn, int index, bool chk){
	AcDbObjectId objId;

	if(chk && index == -1){
		while(!uparry.isEmpty())
			uparry.removeLast();
		return;
	}else if(!chk && index == -1){
		while(!downarry.isEmpty())
			downarry.removeLast();
		return;
	}
	if(chk){
		objId = uparry.at(index);
	}else{
		objId = downarry.at(index);
	}
	acdbGetAdsName(enn, objId);

	return;
}

AcGePoint3d CDLT::lf_3DReturnList(ads_name ename, int type){
	AcGePoint3d retpt;
	struct resbuf* pRb = NULL, *pHead = NULL;
	int rt;

	if(ads_name_nil(ename))
		return retpt;
	if(type < 1000)
		pHead = pRb = ads_entget(ename);
	else
		lf_Xdata(ename, pRb);

	pHead=pRb;
	while ((pRb != NULL) && (pRb->restype != type)){
		pRb = pRb->rbnext;
	}
	if(pRb != NULL)
		rt=type_return(pRb->restype);

	switch(rt){
	case RT3DPOINT:
		retpt[0]=pRb->resval.rpoint[X];
		retpt[1]=pRb->resval.rpoint[Y];
		retpt[2]=pRb->resval.rpoint[Z];
		break;
	default:
		break;
	}

	if(pHead)
		ads_relrb(pHead);
	return retpt;
}

void CDLT::lf_Xdata(ads_name ename, resbuf*& pRb){
	AcDbObject* pObj;
	AcDbObjectId objId;

	acdbGetObjectId(objId, ename);
	if((es = acdbOpenObject(pObj, objId, AcDb::kForRead)) != Acad::eOk){
		ads_printf(_T("\n선택이 잘못되었습니다.\n"));
		return;
	}
	pRb = pObj->xData(NULL);
	pObj->close();
}

int CDLT::type_return(int val){
	if (val < 10) {
		return RTSTR;
	} else if (val < 38) {
		return RT3DPOINT;
	}else if (val < 60) {
		return RTREAL;
	}else if (val < 1010) {
		return RTSTR;
	} else if (val < 1040) {
		return RT3DPOINT;
	} else if (val < 1060) {
		return RTREAL;
	} else if (val < 1071) {
		return RTSHORT;
	} else if (val == 1071) {
		return RTLONG;
	}
	return -100;
}