#ifdef _DEBUG
# pragma comment(lib, "comsuppwd.lib")
#else
# pragma comment(lib, "comsuppw.lib")
#endif
# pragma comment(lib, "wbemuuid.lib")

#include <windows.h>
#include <atlstr.h>
#include <sphelper.h>
#include <sapi.h>
#include<comutil.h>
#include<string.h>
#include<stdlib.h>

#pragma comment(lib,"sapi.lib")
#pragma comment(lib, "comsupp.lib") 

#define GID_CMD_GR 333333
#define WM_RECOEVENT WM_USER+1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

char szAppName[] = "TsinghuaYincheng";
BOOL b_initSR;
BOOL b_Cmd_Grammar;
CComPtr<ISpRecoContext>m_cpRecoCtxt;  //语音识别程序接口
CComPtr<ISpRecoGrammar>m_cpCmdGramma; //识别语法
CComPtr<ISpRecognizer>m_cpRecoEngine; //语音识别引擎


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	HWND        hwnd;
	MSG         msg;
	WNDCLASS    wndclass;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,
		TEXT("C/C++学院语音识别教程"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC           hdc;
	PAINTSTRUCT   ps;

	switch (message)
	{
	case WM_CREATE:
	{
		//初始化COM端口
		::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		//创建识别引擎COM实例为共享型
		HRESULT hr = m_cpRecoEngine.CoCreateInstance(CLSID_SpSharedRecognizer);
		//创建识别上下文接口
		if (SUCCEEDED(hr))
		{
			hr = m_cpRecoEngine->CreateRecoContext(&m_cpRecoCtxt);
		}
		else MessageBox(hwnd, TEXT("error1"), TEXT("error"), S_OK);
		//设置识别消息,使计算机时刻监听语音消息
		if (SUCCEEDED(hr))
		{
			hr = m_cpRecoCtxt->SetNotifyWindowMessage(hwnd, WM_RECOEVENT, 0, 0);
		}
		else MessageBox(hwnd, TEXT("error2"), TEXT("error"), S_OK);
		//设置我们感兴趣的事件
		if (SUCCEEDED(hr))
		{
			ULONGLONG ullMyEvents = SPFEI(SPEI_SOUND_START) | SPFEI(SPEI_RECOGNITION) | SPFEI(SPEI_SOUND_END);
			hr = m_cpRecoCtxt->SetInterest(ullMyEvents, ullMyEvents);
		}
		else MessageBox(hwnd, TEXT("error3"), TEXT("error"), S_OK);
		//创建语法规则
		b_Cmd_Grammar = TRUE;
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("error4"), TEXT("error"), S_OK);
		}
		hr = m_cpRecoCtxt->CreateGrammar(GID_CMD_GR, &m_cpCmdGramma);
		WCHAR wszXMLFile[20] = L"er.xml";
		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)"er.xml", -1, wszXMLFile, 256);
		hr = m_cpCmdGramma->LoadCmdFromFile(wszXMLFile, SPLO_DYNAMIC);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("error5"), TEXT("error"), S_OK);
		}
		b_initSR = TRUE;
		//在开始识别时，激活语法进行识别
		hr = m_cpCmdGramma->SetRuleState(NULL, NULL, SPRS_ACTIVE);
		return 0;
	}
	case WM_RECOEVENT:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		hdc = GetDC(hwnd);
		USES_CONVERSION;
		CSpEvent event;
		while (event.GetFrom(m_cpRecoCtxt) == S_OK)
		{
			switch (event.eEventId)
			{
			case SPEI_RECOGNITION:
			{
				static const WCHAR wszUnrecognized[] = L"<Unrecognized>";
				CSpDynamicString dstrText;
				//取得识别结果
				if (FAILED(event.RecoResult()->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL)))
				{
					dstrText = wszUnrecognized;
				}
				BSTR SRout;
				dstrText.CopyToBSTR(&SRout);
				char* lpszText2 = _com_util::ConvertBSTRToString(SRout);

				if (b_Cmd_Grammar)
				{
					if (strstr("左移", lpszText2) != NULL)
					{
						keybd_event(VK_LEFT,0,0,0);
						Sleep(500);
						keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
						DrawText(hdc, TEXT("左移"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					}
					
					if (strstr("右移", lpszText2) != NULL)
					{
						keybd_event(VK_RIGHT, 0, 0, 0);
						Sleep(500);
						keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
						DrawText(hdc, TEXT("右移"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					}

					if (strstr("蹲下", lpszText2) != NULL)
					{
						keybd_event(VK_DOWN, 0, 0, 0);
						Sleep(500);
						keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
						DrawText(hdc, TEXT("蹲下"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					}

					if (strstr("跳", lpszText2) != NULL)
					{
						keybd_event(VK_UP, 0, 0, 0);
						Sleep(100);
						keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
						DrawText(hdc, TEXT("跳"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					}

					if (strstr("右上", lpszText2) != NULL)
					{
						keybd_event(VK_RIGHT, 0, 0, 0);
						keybd_event(VK_UP, 0, 0, 0);
						Sleep(500);
						keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
						keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
						DrawText(hdc, TEXT("右上"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					}
					
					if (strstr("左上", lpszText2) != NULL)
					{
						keybd_event(VK_LEFT, 0, 0, 0);
						keybd_event(VK_UP, 0, 0, 0);
						Sleep(500);
						keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
						keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
						DrawText(hdc, TEXT("左上"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					}

					if (strstr("攻击", lpszText2) != NULL)
					{
						keybd_event(VK_SPACE, 0, 0, 0);
						keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
						DrawText(hdc, TEXT("攻击"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					}
				}
			}
			}
		}
		return TRUE;
	}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
