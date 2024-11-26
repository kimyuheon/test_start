#include "StdAfx.h"
#include "WKLayerHashTable.h"



CWKLayerHashTable::CWKLayerHashTable(void)
{
}

CWKLayerHashTable::~CWKLayerHashTable(void)
{
}


void CWKLayerHashTable::ReSetContent()
{
	m_data.RemoveAll();
}
BOOL CWKLayerHashTable::Add(AcDbObjectId LayerId,int coloridx,CString name,BOOL bisoff,BOOL bisfrozen,BOOL bislock,AcCmColor color,AcDbObjectId lineid,AcDb::LineWeight lineweight,BOOL bisplottable)
{
	if(name.CompareNoCase(_T(""))==0)return FALSE;
	CWKLayerHashData data;
	data.LayerId = LayerId;
	data.ColorIndex = coloridx;
	data.LayerName = name;
	data.bIsFrozen = bisfrozen;
	data.bIsLock = bislock;
	data.bIsOff = bisoff;
	data.Plottable = bisplottable;
	data.Color = color;
	data.LineTypeId = lineid;
	data.LineWeight = lineweight;
	m_data.Add(data);
	return TRUE;
}
BOOL  CWKLayerHashTable::Find(CString LayerName,CWKLayerHashData& data)
{
	CString name;
	if(!AfxExtractSubString(name,LayerName,1,'|'))
		name = LayerName;

	BOOL bRC =FALSE;
	for(int i = 0; i< m_data.GetCount(); i++)
	{
		CString strName = m_data[i].LayerName;
		if(strName.CompareNoCase(name)==0)
		{
			data = m_data[i];
			bRC = TRUE;
			break;
		}
	}
	return bRC;
	
	
}
BOOL CWKLayerHashTable::Delete(CString name)
{
	if(name.CompareNoCase(_T(""))==0)return FALSE;
	for(int i = 0; i< m_data.GetCount(); i++)
	{
		CString strName = m_data[i].LayerName;
		if(strName.CompareNoCase(name)==0)
		{
			m_data.RemoveAt(i);
			break;
		}
	}
	return TRUE;
}