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

	//레지스트리 읽고 쓰기
	static void				SetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString vvalue);
	static void				SetRegistry(HKEY hKey, CString vname, CString vvalue);
	static void				SetRegistry(CString vname, CString vvalue);
	static CString			GetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString& vvalue);
	static CString			GetRegistry(HKEY hKey, CString vname, CString& vvalue);
	static CString			GetRegistry(CString vname, CString& vvalue);

	//문자열 갯수 파악
	int						GetFindCharCount(CString param_string, CString param_find_char);

	//확장데이터 읽기 쓰기 / 가져오기
	void gl_AddXdata(AcDbObjectId eId, TCHAR saveName[], TCHAR Str1[]);
	void lf_SReturnList(ads_name ename,int type, CString &buffer);
	void lf_Xdata(ads_name ename,resbuf*& pRb);
	int type_return(int val);
	CString gfenameToXdata(ads_name en,TCHAR* str);

	//각도 계산
	double GetDegree(double radian);
	double GetRadian(double degree);


	Acad::ErrorStatus es;
};
