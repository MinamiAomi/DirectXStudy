#pragma once
#ifndef WINAPP_H_
#define WINAPP_H_

#include <Windows.h>
#include <string>

class WinApp final
{
public: // �ÓI�����o�֐�
	/// <summary>
	/// �V���O���g���C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X�̃|�C���^</returns>
	static WinApp* GetInstance();
	
	/// <summary>
	/// �E�B���h�E�v���V�[�W��
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // �����o�֐�
	/// <summary>
	/// �E�B���h�E�n���h���̎擾
	/// </summary>
	/// <returns></returns>
	inline HWND GetHwnd() const { return hwnd_; }
	
	/// <summary>
	/// �E�B���h�E�N���X�̃C���X�^���X�n���h�����擾
	/// </summary>
	/// <returns></returns>
	inline HINSTANCE GetWindowClassHInstance() const { return wnd_class_.hInstance; }
	
	/// <summary>
	/// �E�B���h�E�N���X�̖��O���擾
	/// </summary>
	/// <returns></returns>
	inline const wchar_t* GetWindowClassName() const { return wnd_class_.lpszClassName; }
	
	/// <summary>
	/// �E�B���h�E�^�C�g�����擾
	/// </summary>
	/// <returns></returns>
	inline const std::string& GetWindowTitle() const { return window_title_; }
	
	/// <summary>
	/// �E�B���h�E�����擾
	/// </summary>
	/// <returns></returns>
	inline int GetWindowWidth() const { return window_width_; }
	
	/// <summary>
	/// �E�B���h�E�����擾
	/// </summary>
	/// <returns></returns>
	inline int GetWindowHeight() const { return window_height_; }

	/// <summary>
	/// �Q�[���E�B���h�E���쐬����
	/// </summary>
	/// <param name="widnow_title">�E�B���h�E�^�C�g��</param>
	/// <param name="window_width">�E�B���h�E��</param>
	/// <param name="window_height">�E�B���h�E��</param>
	void CreateGameWindow(const std::string& window_title = "DirectX", int window_width = 1280, int window_height = 720);
	
	/// <summary>
	/// �E�B���h�E������ꂽ��
	/// </summary>
	/// <returns>������</returns>
	bool WindowQuit();
	
	/// <summary>
	/// �Q�[���E�B���h�E��j��
	/// </summary>
	void TerminateGameWindow();

private: // �����o�֐�
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

private: // �����o�ϐ�
	WNDCLASSEX wnd_class_ = {};
	HWND hwnd_ = nullptr;
	std::string window_title_ = "DirectX";
	int window_width_ = 1280;
	int window_height_ = 720;

};

#endif // WINAPP_H_
