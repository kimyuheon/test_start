#include "StdAfx.h"
#include "IniFile.h"
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"

CIniFile::CIniFile(void){
}

CIniFile::~CIniFile(void){
}

BOOL CIniFile::OpenINI(CString strFile){
	//������ ������ ���� ���� 
	CFileFind pFind;
	if(!pFind.FindFile(strFile))
	{
		return FALSE;		// ����ã��		
	}
	return TRUE;
}

CString CIniFile::readINI(CString file, CString strSubject, CString strName){
	CString strPath;
	//CString t_strPath;
	CString strFile;
	CString strRec;
	int findNUM = 0;

	strPath = g_modulePath;
	findNUM = strPath.ReverseFind('\\');
	strPath.Delete(findNUM, strPath.GetLength() - 1);

	//acutPrintf(L"\nREAD PATH : %s\n", strPath);

	//������Ʈ�� ��� ���� (*RX ��ο� ini ���� �����ϴ� ������ ����)
#ifdef	_ZRX_KR
	//GetRegistry(HKEY_CURRENT_USER, _T("RoamableRootFolder"), t_strPath);
	//GetRegistry(HKEY_CURRENT_USER, _T("Path"), strPath);
#endif
	if (strPath == "")
	{
		//strPath = getenv("APPDATA");
		//strPath = strPath + _T("\\ZDream\\");
		//CreateDirectory(strPath, NULL);
		//strPath = strPath + CADVer + _T("\\");
		//CreateDirectory(strFile, NULL);
		CreateFolder(strPath + _T("\\"));
		strFile = strPath + file;
		//CreateDirectory(strFile, NULL);
		CStdioFile txtFile(strFile, CFile::modeCreate | CFile::modeReadWrite);
		txtFile.Close();
	}
	else
	{
		strFile = strPath + _T("\\") + file;
	}
	
	//CString strFile = strPath + _T("ZDream\\") + file;
	
	CString strResult = "";
	char cStr[MAX_LENGTH];
	//wchar_t wtext[MAX_LENGTH] = { 0, };
	//mbstowcs(wtext, cStr, strlen(cStr) + 1);
	if(OpenINI(strFile) == TRUE){
		if(GetPrivateProfileStringW(strSubject, strName, _T(""), (LPWSTR)cStr, MAX_LENGTH, LPCTSTR(strFile)))
		{
			strResult.Format(_T("%s"), cStr);
		}
		else
		{
			//acutPrintf(_T("\nopen & writeINI Ȯ��\n"));
			writeINI(file, strSubject, strName, "");
		}
	}
	else
	{
		writeINI(file, strSubject, strName, "");
	}
	return strResult;
}

BOOL CIniFile::writeINI(CString file, CString strSubject, CString strName, CString strProfile){
	CString strPath;
	CString strFile;
	int findNUM = 0;

	strPath = g_modulePath;
	findNUM = strPath.ReverseFind('\\');
	strPath.Delete(findNUM, strPath.GetLength() - 1);

	//acutPrintf(L"\nREAD PATH : %s\n", strPath);

	//������Ʈ�� ��� ���� (*RX ��ο� ini ���� �����ϴ� ������ ����)
	//GetRegistry(HKEY_CURRENT_USER, _T("RoamableRootFolder"), t_strPath);
	//GetRegistry(HKEY_CURRENT_USER, _T("Path"), strPath);
	//CString strFile=strPath+_T("ZDream\\")+file;

	if (strPath == "")
	{
		//strPath = getenv("APPDATA");
		//strPath = strPath + _T("\\ZDream\\");
		//strPath = strPath + CADVer + _T("\\");
		//strFile = strPath + file;
		strFile = strPath + _T("\\") + file;
	}
	else
	{
		strFile = strPath + _T("\\") + file;
	}

	//CString strFile = strPath + _T("\\") + file;
	CString strRec;
	if(OpenINI(strFile) == FALSE){
		TRY{
			//���� ������ ����
			CFileFind fp;
			//if(!fp.FindFile(strPath+_T("ZDream")))
			//acutPrintf(_T("\nWriteINI & OpenINI\n"));
			if (!fp.FindFile(strPath)) 
			{
				//CreateDirectory(strPath+_T("ZDream"), NULL);
				//strPath = getenv("APPDATA");
				//strPath = strPath + _T("\\ZDream\\");
				//CreateDirectory(strPath, NULL);
				//strPath = strPath + CADVer + _T("\\");
				CreateFolder(strPath + _T("\\"));
				strFile = strPath + file;
				//CreateDirectory(strPath + _T("\\ZDream\\"), NULL);
				//strFile = strPath + _T("\\ZDream") + _T("\\") + _T("2021") + _T("\\") + file;
				//SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath);
			}
			fp.Close();
			//acutPrintf(_T("\n fp. close \n"));
			//acutPrintf(_T("\n%s\n"), strFile);
			//���� ����
			CStdioFile txtFile(strFile, CFile::modeCreate | CFile::modeReadWrite);
			//acutPrintf(_T("\n txtFile \n"));
			strRec.Format(_T("[%s]\n%s=%s"), strSubject, strName, strProfile);
			txtFile.WriteString(strRec);
			txtFile.Seek(0, CFile::end);
			txtFile.Close();
		}
		CATCH(CFileException, e){
			e->ReportError();
		}
		END_CATCH			
	}else{
		//�׸��� ������ �׳� ���� ������ ���� �������� ���� 
		if(WritePrivateProfileStringW(strSubject, strName, strProfile, strFile) == FALSE)
			return FALSE;		
	}
	return TRUE;
}

CString CIniFile::GetRegistry(HKEY hKey, CString vname, CString& vvalue){
#ifdef _ZRX_KR
	#ifdef	_2018
		return GetRegistry(hKey, _T("ZWSOFT\\ZWCAD"), _T("2018\\ko-KR"), vname, vvalue);
	#elif	_2019
		return GetRegistry(hKey, _T("ZWSOFT\\ZWCAD"), _T("2019\\ko-KR"), vname, vvalue);
	#elif _2020	
		return GetRegistry(hKey, _T("ZWSOFT\\ZWCAD"), _T("2020\\ko-KR"), vname, vvalue);
	#elif _2021	
		//return GetRegistry(hKey, _T("ZWSOFT\\ZWCAD"), _T("2021\\ko-KR"), vname, vvalue);
		return GetRegistry(hKey, _T("ZDREAM"), _T("2021"), vname, vvalue);
	#elif _2021M	
		return GetRegistry(hKey, _T("ZDREAM"), _T("2021"), vname, vvalue);
	#elif _2022	
		return GetRegistry(hKey, _T("ZDREAM"), _T("2022"), vname, vvalue);
	#endif
#elif	_ZRX_EN
	#ifdef	_2018
		return GetRegistry(hKey, _T("ZWSOFT\\ZWCAD"), _T("2018\\ko-KR"), vname, vvalue);
	#elif	_2019
		return GetRegistry(hKey, _T("ZWSOFT\\ZWCAD"), _T("2019\\ko-KR"), vname, vvalue);
	#elif _2020	
		return GetRegistry(hKey, _T("ZWSOFT\\ZWCAD"), _T("2020\\ko-KR"), vname, vvalue);
	#elif _2021	
		return GetRegistry(hKey, _T("ZDREAM"), _T("2021"), vname, vvalue);
	#elif _2021M	
		return GetRegistry(hKey, _T("ZDREAM"), _T("2021"), vname, vvalue);
	#elif _2022	
		return GetRegistry(hKey, _T("ZDREAM"), _T("2022"), vname, vvalue);
	#endif
	//return GetRegistry(hKey, _T("ZWSOFT\\ZWCAD"), _T("2018\\en-US"), vname, vvalue);
#else
	return GetRegistry(hKey, _T("CT_Excel"), _T("1.0"), vname, vvalue);
#endif
	return L"";
}

CString CIniFile::GetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString& vvalue){
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
		if(lResult == ERROR_SUCCESS){
			ASSERT(dwType == REG_SZ);
			lResult = RegQueryValueEx(HKey2, (LPTSTR)lpszEntry, NULL, &dwType, (LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
			strValue.ReleaseBuffer();
		}
		RegCloseKey(HKey2);
	}

	vvalue=strValue;  // ��
	return strValue;
}

CString CIniFile::GetRegistry(HKEY hKey, LPCTSTR strCompany, CString vname, CString& vvalue) {
	HKEY HKey2;
	CString sCompany, sProduct;
	CString strValue;

	// ������Ʈ�� ���
	//sCompany.Format(_T("Software\\%s"), strCompany);		//ȸ���
	//sProduct.Format(_T("%s\\%s"), sCompany, strProduct);	//��ǰ��

	sCompany.Format(_T("Software\\%s"), strCompany);
	RegCreateKey(hKey, sCompany, &HKey2);

	if (RegOpenKeyEx(hKey, (LPCTSTR)sCompany, 0, KEY_READ | CROSS_ACCESS, &HKey2) == ERROR_SUCCESS) {
		// KEY���� ������Ʈ���κ��� �о�´�.
		DWORD dwType, dwCount;
		LPCTSTR lpszEntry = (LPCTSTR)vname;
		LONG lResult = RegQueryValueEx(HKey2, (LPTSTR)lpszEntry, NULL, &dwType, NULL, &dwCount);

		if (lResult == ERROR_SUCCESS) {
			ASSERT(dwType == REG_SZ);
			lResult = RegQueryValueEx(HKey2, (LPTSTR)lpszEntry, NULL, &dwType, (LPBYTE)strValue.GetBuffer(dwCount / sizeof(TCHAR)), &dwCount);
			strValue.ReleaseBuffer();
		}
		RegCloseKey(HKey2);
	}
	vvalue = strValue;	//��
	return strValue;
}

void CIniFile::CreateFolder(CString csPath)
{
	// UpdateData(TRUE);
	// csPath = m_csTopFolderName + csPath;

	CString csPrefix(_T("")), csToken(_T(""));
	int nStart = 0, nEnd;
	while ((nEnd = csPath.Find('\\', nStart)) >= 0)
	{
		CString csToken = csPath.Mid(nStart, nEnd - nStart);
		CreateDirectory(csPrefix + csToken, NULL);

		csPrefix += csToken;
		csPrefix += _T("\\");
		nStart = nEnd + 1;
	}
	csToken = csPath.Mid(nStart);
	CreateDirectory(csPrefix + csToken, NULL);
}

//���� ���ϱ�
double CIniFile::getRadian(double degree){
	double rad = 0;
	rad = degree * 3.141592 / 180;
	return rad;
}

//���� ���ϱ�
double CIniFile::getDegree(double radian){
	double deg = 0;
	deg = radian / 3.141592 * 180;
	return deg;
}

//���� �� üũ
bool CIniFile::CheckCStringDigit(CString str){
	char atStr;
	int dotflag = 0;
	for(int i = 0 ; i < str.GetLength(); i++){
		if(dotflag == 2){
			return false;
		}
		atStr = str.GetAt(i);
		if(atStr >= '0' && atStr <= '9'){

		}
		else{
			return false;
		}
		if(atStr == '.')	dotflag++;
	}
	return true;
}

//���� �� ��(.) �Է� üũ
CString CIniFile::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	for(int i = 0; i < str.GetLength(); i++){
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i,1);
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46)){
			str.Remove(*getAtChar);
		}
	}
	return str;
} 