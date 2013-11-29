#include "../include/common.h"
#include "../include/lguibase.h"
#include "../include/lgui.h"

LRESULT WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#define  ID_BUTTON 100
extern PDeskIconInfo pAppIconClass;
int winmain();
void ArtInit(){
	PDeskIconNode pIconNode;
	pIconNode = (PDeskIconNode)malloc(sizeof(DeskIconNode));
	if(!pIconNode){
		return false;
	}
	strcpy(pIconNode->lpszIconName,"draw.bmp");
	strcpy(pIconNode->lpszName,"����");
	strcpy(pIconNode->lpszComment,"art");
	strcpy(pIconNode->lpszBinName,"art");
	pIconNode->pNext=NULL;
	pIconNode->entry=winmain;
	AddNodeToDeskIconClass(pIconNode,pAppIconClass);
}

int winmain()
{

	int iStatus;
	HANDLE hWnd;
/*	HBRUSH hBrush;*/
	WNDCLASSEX wcex;
	MSG msg;
	SCROLLINFO si;
	if(!InitGUIClient())
		return 0;

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)WndProc2;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= 0;
	wcex.hIcon			= 0;
	wcex.hCursor		= 0;
	wcex.hbrBackground	= CreateSolidBrush(RGB(147,222,252));
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "artwin";
	wcex.hIconSm		= 0;

	RegisterClass(&wcex);

	hWnd = CreateWindow("artwin", "��������", WS_MAIN | WS_VISIBLE | 
			WS_THINBORDER|WS_CAPTION | WS_OKBUTTON |WS_CLOSEBOX,
			 1, 20,240, 200, NULL, NULL, NULL, NULL);	



	if (!hWnd)  return false;
	ShowWindow(hWnd, true);
/*	UpdateWindow(hWnd);*/


	while (GetMessage(&msg,hWnd)){

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	TerminateGUIClient();
}

LRESULT WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hDC;
	static RECT rc;
	static PAINTSTRUCT ps;
	HWND hWndButton;
	int x,y,x1,y1;
	float realMin,realMax,imagMin,imagMax;
	int d;
	
	int i=0;

	switch(message)
	{
		case LMSG_CREATE:
			SetTimer(hWnd,1,100);	
			break;
		case LMSG_COMMAND:
			switch(HIWORD(wParam)){
			case BN_CLICKED:

				switch(LOWORD(wParam)){

				case ID_BUTTON:
					DestroyWindow(hWnd);
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}

			break;
		case LMSG_PENDOWN:/*�����mouse�¼�*/
			CaptureMouse(hWnd,BYCLIENT);
			break;
		case LMSG_PENMOVE:
			hDC=GetDC(hWnd);
			x=(int)wParam;
			y=(int)lParam;
			ScreenToClient(hWnd,&x,&y);
			SetPixel(hDC,x,y ,RGB(255,0,0));
			ReleaseDC(hWnd,hDC);
			break;
		case LMSG_PENUP:
			DisCaptureMouse();
			break;
		case LMSG_TIMER:
			InvalidateRect(hWnd,NULL,true);	
			break;
		case LMSG_PAINT:
		{
			HPEN hPen;
			HBRUSH hBrush;
			POINT point;
			ps.bPaintDirect=false;

			hDC=BeginPaint(hWnd, &ps);
			if(!hDC){
				return true;
			}

			acoral_rand_start();
		//	srand(time(0));
			/*realMin = -0.7940,realMax=-0.785,imagMin=0.162,imagMax=0.165;*/
			/*realMin = -0.78937,realMax=-0.789,imagMin=0.164,imagMax=0.16300;*/
			realMin = -0.78937,realMax=-0.78928,imagMin=0.16434,imagMax=0.16443;
			/*MoveToEx(hDC,-10,-10,&point);*/
			/*LineTo(hDC,20,20);*/
			for (i=0;i<10;i++){
				x= ((int)acoral_rand()) % 234;
				y= ((int)acoral_rand()) % 280;
				x1= ((int)acoral_rand()) % 234;
				y1= ((int)acoral_rand()) % 280;
				
				d= ((int)acoral_rand()) % 65535;
				hPen = CreatePen(1,1,d);
				hBrush = CreateSolidBrush(d);
				SelectObject(hDC, hPen);
				SelectObject(hDC, hBrush);
				Rectangle(hDC,x,y,x1,y1);
				DeleteObject(hPen);
				DeleteObject(hBrush);
				/*SetPixel(hDC,x,y,d);*/
				/*printf("%d\n",d);*/
			}
			
			EndPaint(hWnd, &ps);
			break;
		}
		case LMSG_CLOSE:
			PostQuitMessage(hWnd);
			break;		
		case LMSG_DESTROY:
			KillTimer(hWnd,1);
			PostQuitMessage(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return true;
}
