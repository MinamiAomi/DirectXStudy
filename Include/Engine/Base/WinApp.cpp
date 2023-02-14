#include "WinApp.h"

WinApp* WinApp::GetInstance() {
	static WinApp instance;
	return &instance;
}

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリ終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::CreateGameWindow(const std::string& window_title, int window_width, int window_height) {
	window_width_ = window_width;
	window_height_ = window_height;

	wchar_t wtitle[256] = {};
	MultiByteToWideChar(CP_ACP, 0, window_title.c_str(), -1, wtitle, _countof(wtitle));

	wnd_class_.cbSize = sizeof(WNDCLASSEX);
	wnd_class_.lpfnWndProc = (WNDPROC)WindowProc;		// ウィンドウプロシージャを設定
	wnd_class_.lpszClassName = wtitle;					// ウィンドウクラス名
	wnd_class_.hInstance = GetModuleHandle(nullptr);		// ウィンドウハンドル
	wnd_class_.hCursor = LoadCursor(NULL, IDC_ARROW);	// カーソル指定


	RegisterClassEx(&wnd_class_); // ウィンドウクラスをOSに登録する
	RECT wrc = { 0, 0, window_width_, window_height_ }; // ウィンドウサイズ{ X座標, Y座標, 横幅, 縦幅 }
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズを補正する

	// ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow(
		wnd_class_.lpszClassName,		// クラス名
		wtitle,					// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,			// 表示ｘ座標は OS にお任せ
		CW_USEDEFAULT,			// 表示ｙ座標は OS にお任せ
		wrc.right - wrc.left,	// ウィンドウ幅
		wrc.bottom - wrc.top,	// ウィンドウ高
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		wnd_class_.hInstance,			// 呼び出しアプリケーションハンドル,
		nullptr);				// 追加パラメーター


	// ウィンドウを表示状態にする
	ShowWindow(hwnd_, SW_SHOW);
}

bool WinApp::WindowQuit() {
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// アプリケーションが終わるときに message が WM_QUIT になる
	return msg.message == WM_QUIT;
}

void WinApp::TerminateGameWindow() {
	UnregisterClass(wnd_class_.lpszClassName, wnd_class_.hInstance);
}
