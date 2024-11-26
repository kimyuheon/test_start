#include "StdAfx.h"
#include "Arx_Lib.h"

CArx_Lib::CArx_Lib(void){
}

CArx_Lib::~CArx_Lib(void){
}

void CArx_Lib::SetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString vvalue){
	HKEY HKey2;
	CString sCompany, sProduct, prjname;
	
	TCHAR *pszBUF=NULL;
	DWORD stype;

	// ������Ʈ�� ���
	stype=REG_SZ;
	sCompany.Format(_T("Software\\%s"),strCompany);      // ȸ���
	sProduct.Format(_T("%s\\%s"),sCompany,strProduct);		  // ��ǰ��
	RegCreateKey(hKey, sProduct, &HKey2);

	// ������Ʈ���� ������Ʈ���� �����Ѵ�.
	pszBUF=(TCHAR*)(LPCTSTR)vvalue; // �� 
	RegSetValueEx(hKey, (LPCTSTR)vname, NULL, REG_SZ, (const BYTE*)pszBUF, (lstrlen(pszBUF)+1)*sizeof(TCHAR));
	RegCloseKey(HKey2);
}

void CArx_Lib::SetRegistry(CString vname, CString vvalue){
	SetRegistry(HKEY_LOCAL_MACHINE, _T("CT_Excel"), _T("1.0"), vname, vvalue);
}

void CArx_Lib::SetRegistry(HKEY hKey, CString vname, CString vvalue){
	SetRegistry(hKey, _T("CT_Excel"), _T("1.0"), vname, vvalue);
}

CString CArx_Lib::GetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString& vvalue){
	HKEY HKey2;
	CString sCompany, sProduct;
	CString strValue;

	// ������Ʈ�� ���
	sCompany.Format(_T("SOFTWARE\\%s"),strCompany);      // ȸ���
	sProduct.Format(_T("%s\\%s"),sCompany,strProduct);		  // ��ǰ��
	RegCreateKey(hKey, sProduct, &HKey2);
	
	if(RegOpenKeyEx(hKey, (LPCTSTR)sProduct, 0, KEY_READ | CROSS_ACCESS , &HKey2)==ERROR_SUCCESS){
		// KEY���� ������Ʈ���κ��� �о�´�.
		DWORD dwType, dwCount;
		LPCTSTR lpszEntry=(LPCTSTR)vname;
		LONG lResult = RegQueryValueEx(HKey2, (LPTSTR)lpszEntry, NULL, &dwType, NULL, &dwCount);
		if (lResult == ERROR_SUCCESS){
			ASSERT(dwType == REG_SZ);
			lResult = RegQueryValueEx(HKey2, (LPTSTR)lpszEntry, NULL, &dwType, (LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
			strValue.ReleaseBuffer();
		}
		RegCloseKey(HKey2);
	}
	vvalue=strValue;  // ��
	return strValue;
}

CString CArx_Lib::GetRegistry(CString vname, CString& vvalue){
	return GetRegistry(HKEY_LOCAL_MACHINE, _T("CT_Excel"), _T("1.0"), vname, vvalue);
}

CString CArx_Lib::GetRegistry(HKEY hKey, CString vname, CString& vvalue){
#ifdef _ZRX_KR
	#ifdef	_2018
		return GetRegistry(hKey, _T("ZWSOFT\\ZWCAD"), _T("2018\\ko-KR"), vname, vvalue);
	#elif	_2019	
		return GetRegistry(hKey, _T("ZWSOFT\\ZWCAD"), _T("2019\\ko-KR"), vname, vvalue);
	#endif
#elif	_ZRX_EN
	return GetRegistry(hKey, _T("ZWSOFT\\ZWCAD"), _T("2018\\en-US"), vname, vvalue);
#else
	return GetRegistry(hKey, _T("CT_Excel"), _T("1.0"), vname, vvalue);
#endif
}

int CArx_Lib::GetFindCharCount(CString param_string, CString param_find_char){
	int length = param_string.GetLength();		//�Է��� ���ڿ� ����
	int find_cnt = 0;							//��� ī��Ʈ

	//�� ã�ƶ�!!!
	for(int i = 0; i < length; i++){
		if(param_string[i] == param_find_char){
			find_cnt++;
		}
	}
	return find_cnt;		//��� �����ٲ�
}

//Ȯ�嵥���� �Է�
void CArx_Lib::gl_AddXdata(AcDbObjectId eId, TCHAR saveName[], TCHAR Str1[]){
	Acad::ErrorStatus retStat;
	AcDbObject* pObj;
	if((retStat = acdbOpenObject(pObj,eId,AcDb::kForWrite)) != Acad::eOk){
		ads_printf(_T("\n������ �߸��Ǿ����ϴ�.\n"));
		return;
	}

	TCHAR appName[100], resStr1[100];
	appName[0] = resStr1[0] =_T('\0');

	_tcscpy(appName,saveName);
	_tcscpy(resStr1,Str1);

	struct  resbuf  *pRb, *pTemp;

	pRb = pObj->xData(appName);

	if(pRb != NULL){
		pTemp = pRb;
		while ((pTemp->rbnext != NULL) && (pTemp->restype != 1000)){
			pTemp = pTemp->rbnext;
		}
		pTemp->resval.rstring = (TCHAR*) malloc((_tcslen(resStr1) + 1)*sizeof(TCHAR));
		_tcscpy(pTemp->resval.rstring, resStr1);
		while (pTemp->rbnext != NULL){
			pTemp = pTemp->rbnext;
		}
	}else{
		ads_regapp(appName);

		pRb = ads_newrb(AcDb::kDxfRegAppName);
		pTemp = pRb;

		pTemp->resval.rstring
			= (TCHAR*) malloc((_tcslen(appName) + 1)*sizeof(TCHAR));
		_tcscpy(pTemp->resval.rstring, appName);
		pTemp->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
		pTemp = pTemp->rbnext;
		pTemp->restype = 1000;
		pTemp->resval.rstring = (TCHAR*) malloc((_tcslen(resStr1) + 1)*sizeof(TCHAR));
		_tcscpy(pTemp->resval.rstring, resStr1);
	}

	pObj->upgradeOpen();
	pObj->setXData(pRb);

	pObj->close();

	if(pRb) ads_relrb(pRb);
}

//Ȯ�嵥���� ��������
CString CArx_Lib::gfenameToXdata(ads_name en,TCHAR* str){
	int rt=0;
	CString temp;
	AcDbObject* pObj;
	AcDbObjectId objId;
	struct resbuf *pRb=NULL, *pHead=NULL;;

	if(acdbGetObjectId(objId,en) != Acad::eOk){
		return _T("NOT");
	}
	if(acdbOpenObject(pObj,objId,AcDb::kForWrite)==Acad::eWasErased){
		return _T("NOT");
	}

	pRb = pObj->xData(str);
	
	temp=_T("");
		
	if(pRb != NULL){
		pRb = pRb->rbnext;
		if(pRb != NULL){
			temp.Format(_T("%s"), pRb->resval.rstring);
		}
	}
	pObj->close();
	return temp;
}

//Ȯ�嵥���� ����
void CArx_Lib::lf_SReturnList(ads_name ename,int type, CString &buffer){
	struct resbuf* pRb=NULL, *pHead=NULL;
    int rt=0;

	buffer=_T("");

	if(ads_name_nil(ename)) return;
	if(type < 1000)	pRb = ads_entget(ename);
	else			lf_Xdata(ename,pRb);
	pHead=pRb;

	while ((pRb != NULL) && (pRb->restype != type))	pRb = pRb->rbnext;	

	if(pRb != NULL)	rt=type_return(pRb->restype);
	
    switch (rt) {
    case RTSTR:
		buffer.Format(_T("%s"), pRb->resval.rstring);
        break;
    }
	if(pHead) ads_relrb(pHead);	
}

void CArx_Lib::lf_Xdata(ads_name ename,resbuf*& pRb){
	AcDbObject* pObj;
	AcDbObjectId objId;

	acdbGetObjectId(objId,ename);
	if ((es = acdbOpenObject(pObj,objId,AcDb::kForRead)) != Acad::eOk)
	{
        ads_printf(_T("\n������ �߸��Ǿ����ϴ�.\n"));
        return;
    }
	pRb=pObj->xData(NULL);
	pObj->close();
}

int CArx_Lib::type_return(int val){
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

//���� ���ϱ�
double CArx_Lib::GetRadian(double degree){
	double rad = 0;
	rad = degree * 3.141592 / 180;
	return rad;
}

//��׸� ���ϱ�
double CArx_Lib::GetDegree(double radian){
	double deg = 0;
	deg = radian / 3.141592 * 180;
	return deg;
}