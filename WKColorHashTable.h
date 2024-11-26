#pragma once

class CWKColorHashData
{
public:
	AcDbObjectId LayerId;
	int ColorIndex;
	CString LayerName;
};
class CWKColorHashTable
{
public:
	CWKColorHashTable(void);
	~CWKColorHashTable(void);
private:
	CArray <CWKColorHashData,CWKColorHashData> m_data;

public:
	//자료 초기화
	void ReSetContent();
	//데이터 추가
	BOOL Add(AcDbObjectId LayerId,int coloridx,CString name);
	//자료 찾기
	AcDbObjectId  Find(int coloridx,CString& name);
	//자료 삭제
	BOOL Delete(CString name);
};
