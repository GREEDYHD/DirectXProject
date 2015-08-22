//
// Main.cpp -
//

#include "pch.h"
#include "Simulation.h"

using namespace DirectX;

namespace
{
	std::unique_ptr<Simulation> g_Simulation;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// Entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		return 1;

	g_Simulation.reset(new Simulation());

	// Register class and create window
	{
		// Register class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, L"IDI_ICON");
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"DirectX_ProjectWindowClass";
		wcex.hIconSm = LoadIcon(wcex.hInstance, L"IDI_ICON");
		if (!RegisterClassEx(&wcex))
			return 1;

		// Create window
		size_t w, h;
		g_Simulation->GetDefaultSize(w, h);
		RECT rc;
		rc.top = 0;
		rc.left = 0;
		rc.right = static_cast<LONG>(w);
		rc.bottom = static_cast<LONG>(h);
		HWND hwnd = CreateWindow(L"DirectX_ProjectWindowClass", L"DirectX Project", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
			nullptr);
		if (!hwnd)
			return 1;

		ShowWindow(hwnd, nCmdShow);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_Simulation.get()));

		g_Simulation->Initialize(hwnd);
	}

	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_Simulation->Tick();
		}
	}

	g_Simulation.reset();

	CoUninitialize();
	return (int)msg.wParam;
}

// Windows procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;

	auto simulation = reinterpret_cast<Simulation*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

//	if (TwEventWin(hWnd, message, wParam, lParam))
		//return 0; // Event has been handled by AntTweakBar

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			if (!s_minimized)
			{
				s_minimized = true;
				if (!s_in_suspend && simulation)
					simulation->OnSuspending();
				s_in_suspend = true;
			}
		}
		else if (s_minimized)
		{
			s_minimized = false;
			if (s_in_suspend && simulation)
				simulation->OnResuming();
			s_in_suspend = false;
		}
		else if (!s_in_sizemove && simulation)
			simulation->OnWindowSizeChanged();
		break;

	case WM_ENTERSIZEMOVE:
		s_in_sizemove = true;
		break;

	case WM_EXITSIZEMOVE:
		s_in_sizemove = false;
		if (simulation)
			simulation->OnWindowSizeChanged();
		break;

	case WM_GETMINMAXINFO:
	{
		auto info = reinterpret_cast<MINMAXINFO*>(lParam);
		info->ptMinTrackSize.x = 320;
		info->ptMinTrackSize.y = 200;
	}
	break;

	case WM_ACTIVATEAPP:
		if (simulation)
		{
			if (wParam)
			{
				simulation->OnActivated();
			}
			else
			{
				simulation->OnDeactivated();
			}
		}
		break;

	case WM_POWERBROADCAST:
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:
			if (!s_in_suspend && simulation)
				simulation->OnSuspending();
			s_in_suspend = true;
			return true;

		case PBT_APMRESUMESUSPEND:
			if (!s_minimized)
			{
				if (s_in_suspend && simulation)
					simulation->OnResuming();
				s_in_suspend = false;
			}
			return true;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	if (AllocConsole())
	{
		freopen("CONIN$", "w", stdin);
		freopen("CONOUT$", "w", stdout);
		//Might be required? Might not work. Who knows.
		freopen("CONOUY$", "w", stderr);
		SetConsoleTitle(L"Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}