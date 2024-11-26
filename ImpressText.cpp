
#include "StdAfx.h"
#include "CIM.h"
#include <tlhelp32.h>
#include "afxdialogex.h"

int GetFindCharCount(CString parm_string, char parm_find_char){
	int length = parm_string.GetLength(), find_count = 0; 
	for(int i = 0; i < length; i++){ 
		if(parm_string[i] == parm_find_char){
			find_count++;
		}
	} 
	return find_count; 
}

void CCIM::ImpressText(){
	CStdioFile txt_file;
	CString path, strMsg;
	GetDlgItem(IDC_PATHINPUT)->GetWindowText(path);
	if(path.IsEmpty()){
		strMsg.LoadStringW(IDS_NOTSET_PATH); // "경로를 지정하지 않았습니다"
		AfxMessageBox(strMsg);
		return;
	}
	// 현재 경로에서 지정한 텍스트 파일을 연다.
	// CFile::modeRead  ->  파일을 읽기 모드로 연다. ( 파일이 없다면 실패한다.)
	// CFile::typeText  ->  파일의 속성을 텍스트 형식으로 지정한다. ( 생략 가능)
	if(txt_file.Open(path, CFile::modeRead | CFile::typeText)){

		CString display_str, str;
		bool changeXY = IsDlgButtonChecked(IDC_XYCHANGE);
		bool chk2dPoly = IsDlgButtonChecked(IDC_2DPOLY);
		bool chk3dPoly = IsDlgButtonChecked(IDC_3DPOLY);

		AcDb2dPolyline *P2dline = new AcDb2dPolyline;
		AcDb3dPolyline *P3dline = new AcDb3dPolyline;

		int pointcnt;
		pointcnt = GetDlgItemInt(IDC_PCNT);
		CString cnt;
		cnt.Format(_T("%%.%df"), pointcnt);

		// 파일에 더이상 텍스트 내용이 없을때까지 한줄씩 반복해서 읽어서 str 변수에 저장한다.
		while(txt_file.ReadString(str)){
			CString A1, A2, A3, A4;
			// 읽어들인 한줄의 텍스트를 display_str 변수에 추가한다.
			display_str += str;
			double x = 0.0, y = 0.0, z = 0.0;
			bool chkColon = IsDlgButtonChecked(IDC_Colon);
			bool chkSpace = IsDlgButtonChecked(IDC_SPACE);
			bool chkSet = IsDlgButtonChecked(IDC_SET);
			int count;

			if(chkColon)
				count = GetFindCharCount(str, ',');
			else if(chkSpace)
				count = GetFindCharCount(str, ' ');
			else if(chkSet){
				CString set;
				GetDlgItemText(IDC_SETSPACE, set);
				char* ss = LPSTR(LPCTSTR(set));
				count = GetFindCharCount(str, *ss);
			}

			if(count == 1){
				if(chkColon){
					AfxExtractSubString(A1, str, 0, ',');
					AfxExtractSubString(A2, str, 1, ',');
					x = _wtof(A1);
					y = _wtof(A2);
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
				}
				else if(chkSet){
					CString set;
					GetDlgItemText(IDC_SETSPACE, set);
					char* ss = LPSTR(LPCTSTR(set));

					AfxExtractSubString(A1, str, 0, *ss);
					AfxExtractSubString(A2, str, 1, *ss);
					x = _wtof(A1);
					y = _wtof(A2);
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
				}
				else if(chkSpace){
					AfxExtractSubString(A1, str, 0, ' ');
					AfxExtractSubString(A2, str, 1, ' ');
					x = _wtof(A1);
					y = _wtof(A2);
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
				}
			}
			else if(count == 2){
				if(chkColon){
					CString Text;
					AfxExtractSubString(A1, str, 0, ',');
					AfxExtractSubString(A2, str, 1, ',');
					AfxExtractSubString(A3, str, 2, ',');
					x = _wtof(A1);
					y = _wtof(A2);
					if(BZChk)
						z = _wtof(A3);
					if(CText.GetCheck())
						if(z == 0.0)
							if(isalpha(str.GetAt(0)))
								Text = A3;
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){				
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
					if(CText.GetCheck())
						appendText(Text, p3d);
				}
				if(chkSpace){
					CString Text;
					AfxExtractSubString(A1, str, 0, ' ');
					AfxExtractSubString(A2, str, 1, ' ');
					AfxExtractSubString(A3, str, 2, ' ');
					x = _wtof(A1);
					y = _wtof(A2);
					if(BZChk)
						z = _wtof(A3);
					if(CText.GetCheck())
						if(z == 0.0)
							if(isalpha(str.GetAt(0)))
								Text = A3;					
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){				
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
					if(CText.GetCheck())
						appendText(Text, p3d);
				}
			}
			else if(count == 3){
				if(chkColon){
					CString Text;
					AfxExtractSubString(A1, str, 0, ',');
					AfxExtractSubString(A2, str, 1, ',');
					AfxExtractSubString(A3, str, 2, ',');
					AfxExtractSubString(A4, str, 3, ',');
					x = _wtof(A1);
					y = _wtof(A2);
					if(BZChk)
						z = _wtof(A3);
					if(CText.GetCheck())
						if(z == 0.0)
							Text = A4;					
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){				
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
					if(CText.GetCheck())
						appendText(Text, p3d);
				}
			}
			// 멀티라인 속성을 가진 에디트 컨트롤은 줄의 끝에 "\r\n" 가 있어야지 줄바꿈을 해준다.
			// ReadString 함수가 한줄의 텍스트를 읽을때 해당줄의 끝에있는 "\n" 은 읽어서 버리기
			// 때문에 텍스트의 한줄끝에 "\n" 이 있다고 할지라도 반드시 "\n" 을 붙여야 합니다.
			//display_str += "\r\n";
		}
		if(CConline.GetCheck()){
			if(chk2dPoly){
				append2dPoly(P2dline);
			}
			else if(chk3dPoly){
				append3dPoly(P3dline);
			}
		}
		// display_str 변수에 저장되어 있는 내용을 IDC_MY_TEXT 에디트 컨트롤에 출력한다.
		//SetDlgItemText(IDC_MY_TEXT, display_str);

		// 작업을 마친후에 파일을 닫는다.
		txt_file.Close();
	}
}