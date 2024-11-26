#pragma once
#include "resource.h"
#include "WKUtils.h"

class CTextProcess
{
public:
	CTextProcess(void);
	~CTextProcess(void);

	//bisArea =  면적 계산을 할것인지 말것인지 여부 
	
	
	AcDbObjectId insertTextCMD(AcGePoint3d insPt, double textHeight,CString StyleName,double widthFactor,double angle,CString Lyname,int colorindex,CString strText,AcDb::TextVertMode& TextVPos,AcDb::TextHorzMode& TextHPos);
	
};
