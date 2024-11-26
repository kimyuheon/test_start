#include "StdAfx.h"
#include "WKLayerSettingsData.h"

CWKLayerSettingsData::CWKLayerSettingsData(void)
{
}

CWKLayerSettingsData::~CWKLayerSettingsData(void)
{
}

void CWKLayerSettingsData::ReSetContent()
{
	m_data.RemoveAll();
}
BOOL CWKLayerSettingsData::Add(CString name,BOOL bIsOff,BOOL bIsFrozen,BOOL bIsLock,int nColorindex,CString strLineType,CString strLineWeight,BOOL bIsPlottable)
{
	if(name.CompareNoCase(_T(""))==0)return FALSE;
	CWKLayerData data;

	data.name = name;
	data.bIsFrozen = bIsFrozen;
	data.bIsLock = bIsLock;
	data.bIsOff = bIsOff;
	data.bIsPlottable = bIsPlottable;
	data.nColorindex = nColorindex;
	data.strLineType= strLineType;
	data.strLineWeight = strLineWeight;

	m_data.Add(data);
	return TRUE;
}
BOOL  CWKLayerSettingsData::Find(CString LayerName,CWKLayerData& data)
{

	BOOL bRC =FALSE;
	for(int i = 0; i< m_data.GetCount(); i++)
	{
		CString strName = m_data[i].name;
		if(strName.CompareNoCase(LayerName)==0)
		{
			data = m_data[i];
			bRC = TRUE;
			break;
		}
	}
	return bRC;


}
BOOL CWKLayerSettingsData::Delete(CString name)
{
	if(name.CompareNoCase(_T(""))==0)return FALSE;
	for(int i = 0; i< m_data.GetCount(); i++)
	{
		CString strName = m_data[i].name;
		if(strName.CompareNoCase(name)==0)
		{
			m_data.RemoveAt(i);
			break;
		}
	}
	return TRUE;
}

int CWKLayerSettingsData::GetCount()
{
	return m_data.GetCount();
}

CWKLayerData CWKLayerSettingsData::GetAt(int i)
{
	return m_data.GetAt(i);
}