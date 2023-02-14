#pragma once
#ifndef WINAPP_H_
#define WINAPP_H_

#include <Windows.h>
#include <string>

class WinApp final
{
public: // 静的メンバ関数
	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns>インスタンスのポインタ</returns>
	static WinApp* GetInstance();
	
	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数
	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns></returns>
	inline HWND GetHwnd() const { return hwnd_; }
	
	/// <summary>
	/// ウィンドウクラスのインスタンスハンドルを取得
	/// </summary>
	/// <returns></returns>
	inline HINSTANCE GetWindowClassHInstance() const { return wnd_class_.hInstance; }
	
	/// <summary>
	/// ウィンドウクラスの名前を取得
	/// </summary>
	/// <returns></returns>
	inline const wchar_t* GetWindowClassName() const { return wnd_class_.lpszClassName; }
	
	/// <summary>
	/// ウィンドウタイトルを取得
	/// </summary>
	/// <returns></returns>
	inline const std::string& GetWindowTitle() const { return window_title_; }
	
	/// <summary>
	/// ウィンドウ幅を取得
	/// </summary>
	/// <returns></returns>
	inline int GetWindowWidth() const { return window_width_; }
	
	/// <summary>
	/// ウィンドウ高を取得
	/// </summary>
	/// <returns></returns>
	inline int GetWindowHeight() const { return window_height_; }

	/// <summary>
	/// ゲームウィンドウを作成する
	/// </summary>
	/// <param name="widnow_title">ウィンドウタイトル</param>
	/// <param name="window_width">ウィンドウ幅</param>
	/// <param name="window_height">ウィンドウ高</param>
	void CreateGameWindow(const std::string& window_title = "DirectX", int window_width = 1280, int window_height = 720);
	
	/// <summary>
	/// ウィンドウが閉じられたか
	/// </summary>
	/// <returns>閉じたか</returns>
	bool WindowQuit();
	
	/// <summary>
	/// ゲームウィンドウを破棄
	/// </summary>
	void TerminateGameWindow();

private: // メンバ関数
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

private: // メンバ変数
	WNDCLASSEX wnd_class_ = {};
	HWND hwnd_ = nullptr;
	std::string window_title_ = "DirectX";
	int window_width_ = 1280;
	int window_height_ = 720;

};

#endif // WINAPP_H_
