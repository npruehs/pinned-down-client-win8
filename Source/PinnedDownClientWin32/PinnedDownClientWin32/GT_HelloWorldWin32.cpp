#if FALSE

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <strsafe.h>

// Main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// Text that appears in the title bar.
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

// Window-procedure function that handles the many messages that an application receives from the operating system.
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");

	switch (uMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Print out "Hello, World!" in the top left corner.
		TextOut(hdc, 5, 5, greeting, _tcslen(greeting));

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

// User-provided entry point for a graphical Windows-based application.
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// Set up information about the window class, i.e. the application icon, the background color of the window, the name to display in the title bar and the name of the window procedure function.
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	// Register new window class.
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	// Create window.
	HWND hWnd = CreateWindow(
		szWindowClass,					// Name of the application.
		szTitle,						// Text that appears in the title bar.
		WS_OVERLAPPEDWINDOW,			// Type of window to create.
		CW_USEDEFAULT, CW_USEDEFAULT,	// Initial position (x, y).
		500, 100,						// Initial size (width, length).
		NULL,							// Parent of this window.
		NULL,							// No menu bar.
		hInstance,						// First parameter from WinMain.
		NULL							// Not used in this application.
		);
	if (!hWnd)
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		// Display the error message and exit the process

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)TEXT("CreateWindow")) + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			TEXT("CreateWindow"), dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);

		return 1;
	}

	// Show window.
	ShowWindow(
		hWnd,		// Handle to window to show.
		nCmdShow);	// Fourth parameter from WinMain.

	UpdateWindow(hWnd);

	// Message loop to listen for the messages that the operating system sends.
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

#endif