#include "WinApp.h"

WinApp* WinApp::GetInstance() {
	static WinApp instance;
	return &instance;
}

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	// ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
		// �E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		// OS�ɑ΂��āA�A�v���I����`����
		PostQuitMessage(0);
		return 0;
	}

	// �W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::CreateGameWindow(const std::string& window_title, int window_width, int window_height) {
	window_width_ = window_width;
	window_height_ = window_height;

	wchar_t wtitle[256] = {};
	MultiByteToWideChar(CP_ACP, 0, window_title.c_str(), -1, wtitle, _countof(wtitle));

	wnd_class_.cbSize = sizeof(WNDCLASSEX);
	wnd_class_.lpfnWndProc = (WNDPROC)WindowProc;		// �E�B���h�E�v���V�[�W����ݒ�
	wnd_class_.lpszClassName = wtitle;					// �E�B���h�E�N���X��
	wnd_class_.hInstance = GetModuleHandle(nullptr);		// �E�B���h�E�n���h��
	wnd_class_.hCursor = LoadCursor(NULL, IDC_ARROW);	// �J�[�\���w��


	RegisterClassEx(&wnd_class_); // �E�B���h�E�N���X��OS�ɓo�^����
	RECT wrc = { 0, 0, window_width_, window_height_ }; // �E�B���h�E�T�C�Y{ X���W, Y���W, ����, �c�� }
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y��␳����

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd_ = CreateWindow(
		wnd_class_.lpszClassName,		// �N���X��
		wtitle,					// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	// �^�C�g���o�[�Ƌ��E��������E�B���h�E
		CW_USEDEFAULT,			// �\�������W�� OS �ɂ��C��
		CW_USEDEFAULT,			// �\�������W�� OS �ɂ��C��
		wrc.right - wrc.left,	// �E�B���h�E��
		wrc.bottom - wrc.top,	// �E�B���h�E��
		nullptr,				// �e�E�B���h�E�n���h��
		nullptr,				// ���j���[�n���h��
		wnd_class_.hInstance,			// �Ăяo���A�v���P�[�V�����n���h��,
		nullptr);				// �ǉ��p�����[�^�[


	// �E�B���h�E��\����Ԃɂ���
	ShowWindow(hwnd_, SW_SHOW);
}

bool WinApp::WindowQuit() {
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// �A�v���P�[�V�������I���Ƃ��� message �� WM_QUIT �ɂȂ�
	return msg.message == WM_QUIT;
}

void WinApp::TerminateGameWindow() {
	UnregisterClass(wnd_class_.lpszClassName, wnd_class_.hInstance);
}
