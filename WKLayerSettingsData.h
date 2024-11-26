#pragma once

class CWKLayerData
{
public:
	CString name;
	BOOL bIsOff;
	BOOL bIsFrozen;
	BOOL bIsLock;
	int nColorindex;
	CString strLineType;
	CString strLineWeight;
	BOOL bIsPlottable;


};


class CWKLayerSettingsData
{
public:
	CWKLayerSettingsData(void);
	~CWKLayerSettingsData(void);
private:
	CArray <CWKLayerData,CWKLayerData> m_data;

public:
	// 데이터의 이름 
	CString Name;
	//자료 초기화
	void ReSetContent();
	//데이터 추가
	BOOL Add(CString name,BOOL bIsOff,BOOL bIsFrozen,BOOL bIsLock,int nColorindex,CString strLineType,CString strLineWeight,BOOL bIsPlottable);
	//자료 찾기
	BOOL  Find(CString LayerName,CWKLayerData& data);
	//자료 삭제
	BOOL Delete(CString name);

	int GetCount();
	CWKLayerData GetAt(int i);
	
};
