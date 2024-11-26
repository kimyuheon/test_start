#pragma once

#if defined(_X86_)
#define CROSS_ACCESS KEY_WOW64_32KEY
#elif defined(_WIN64)
#define CROSS_ACCESS KEY_WOW64_64KEY
#endif

class CArx_Lib
{
public:
	CArx_Lib(void);
	~CArx_Lib(void);

	//������Ʈ�� �а� ����
	static void				SetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString vvalue);
	static void				SetRegistry(HKEY hKey, CString vname, CString vvalue);
	static void				SetRegistry(CString vname, CString vvalue);
	static CString			GetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString& vvalue);
	static CString			GetRegistry(HKEY hKey, CString vname, CString& vvalue);
	static CString			GetRegistry(CString vname, CString& vvalue);

	//���ڿ� ���� �ľ�
	int						GetFindCharCount(CString param_string, CString param_find_char);

	//Ȯ�嵥���� �б� ���� / ��������
	void gl_AddXdata(AcDbObjectId eId, TCHAR saveName[], TCHAR Str1[]);
	void lf_SReturnList(ads_name ename,int type, CString &buffer);
	void lf_Xdata(ads_name ename,resbuf*& pRb);
	int type_return(int val);
	CString gfenameToXdata(ads_name en,TCHAR* str);

	//���� ���
	double GetDegree(double radian);
	double GetRadian(double degree);


	Acad::ErrorStatus es;
};
