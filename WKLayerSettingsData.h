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
	// �������� �̸� 
	CString Name;
	//�ڷ� �ʱ�ȭ
	void ReSetContent();
	//������ �߰�
	BOOL Add(CString name,BOOL bIsOff,BOOL bIsFrozen,BOOL bIsLock,int nColorindex,CString strLineType,CString strLineWeight,BOOL bIsPlottable);
	//�ڷ� ã��
	BOOL  Find(CString LayerName,CWKLayerData& data);
	//�ڷ� ����
	BOOL Delete(CString name);

	int GetCount();
	CWKLayerData GetAt(int i);
	
};
