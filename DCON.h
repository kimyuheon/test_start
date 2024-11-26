#pragma once
#include "IniFile.h"
#include "afxwin.h"

// CDCON 대화 상자입니다.

class CDCON : public CAcUiDialog
{
	DECLARE_DYNAMIC(CDCON)

public:
	CDCON(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDCON();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DCON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnUpdateEdtBase();
	afx_msg void OnBnClickedBtnColor();
	afx_msg void OnEnUpdateEdtBright();
	afx_msg void OnBnClickedChkLayercolor();
	afx_msg void OnDeltaposSpnBright(NMHDR *pNMHDR, LRESULT *pResult);

	void SelectColorChange(int colorIndex);
	CString CheckInputNumber(CString str);
	
	CIniFile iniFile;
	Acad::ErrorStatus es;

	short setColorIdx;
	CString redColorStr;
	CString greenColorStr;
	CString blueColorStr;
	COLORREF btnAfterClr;			//색상 정보 변수
	CString LayerStr;
	bool chkLayerColor;

	CString baseStr;			//도면층 분할 기준
	double baseHeight;			//분할 기준
	CString layerHeaderStr;		//도면층 분할 머릿말
	CString layerColorChkStr;	//도면층 색상 지정 체크
	CString colorBrightStr;		//명도 변화 단계

	CEdit m_BaseCtrl;
	CString m_BaseStr;
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedDconHelp();
};

class WKRGB
{
public:
	unsigned char R;
	unsigned char G;
	unsigned char B;

	WKRGB(unsigned char r, unsigned char g, unsigned char b){
		R = r;
		G = g;
		B = b;
	}
	bool Equals(WKRGB rgb){
		return (R == rgb.R) && (G == rgb.G) && (B == rgb.B);
	}
};

class HSL{
public:
	int H;
	float S;
	float L;

	HSL(int h, float s, float l){
		H = h;
		S = s;
		L = l;
	}

	bool Equals(HSL hsl){
		return (H == hsl.H) && (S == hsl.S) && (L == hsl.L);
	}
};

static float HueToRGB(float v1, float v2, float vH){
	if(vH < 0)
		vH += 1;

	if(vH > 1)
		vH -= 1;

	if((6 * vH) < 1)
		return (v1 + (v2 - v1) * 6 * vH);

	if((2 * vH) < 1)
		return v2;

	if((3 * vH) < 2)
		return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

	return v1;
}

static WKRGB HSLToRGB(HSL hsl){
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	if(hsl.S == 0){
		r = g = b = (unsigned char)(hsl.L * 255);
	}
	else{
		float v1, v2;
		float hue = (float)hsl.H / 360;

		v2 = (hsl.L < 0.5) ? (hsl.L * (1 + hsl.S)) : ((hsl.L + hsl.S) - (hsl.L * hsl.S));
		v1 = 2 * hsl.L - v2;

		r = (unsigned char)(255 * HueToRGB(v1, v2, hue + (1.0f / 3)));
		g = (unsigned char)(255 * HueToRGB(v1, v2, hue));
		b = (unsigned char)(255 * HueToRGB(v1, v2, hue - (1.0f / 3)));
	}

	return WKRGB(r, g, b);
}


static float Min(float a, float b){
	return a <= b ? a : b;
}

static float Max(float a, float b){
	return a >= b ? a : b;
}

static HSL RGBToHSL(WKRGB rgb){
	HSL hsl = HSL(0, 0, 0);

	float r = (rgb.R / 255.0f);
	float g = (rgb.G / 255.0f);
	float b = (rgb.B / 255.0f);

	float min = Min(Min(r, g), b);
	float max = Max(Max(r, g), b);
	float delta = max - min;

	hsl.L = (max + min) / 2;

	if(delta == 0){
		hsl.H = 0;
		hsl.S = 0.0f;
	}
	else{
		hsl.S = (hsl.L <= 0.5) ? (delta / (max + min)) : (delta / (2 - max - min));

		float hue;

		if(r == max){
			hue = ((g - b) / 6) / delta;
		}
		else if (g == max){
			hue = (1.0f / 3) + ((b - r) / 6) / delta;
		}
		else{
			hue = (2.0f / 3) + ((r - g) / 6) / delta;
		}

		if (hue < 0)
			hue += 1;
		if (hue > 1)
			hue -= 1;

		hsl.H = (int)(hue * 360);
	}
	return hsl;
}