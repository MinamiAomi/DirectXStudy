#include "Input.h"

#include <cassert>
#include <Windows.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment (lib, "xinput.lib")

Input* Input::GetInstance() 
{
	static Input instance;
	return &instance;
}

void Input::Initialize()
{
	winApp_ = WinApp::GetInstance();

	HRESULT result = S_FALSE;
	// DirectInput�̏�����
	result = DirectInput8Create(
		winApp_->GetWindowClassHInstance(), DIRECTINPUT_HEADER_VERSION,
		IID_IDirectInput8, static_cast<void**>(&directInput_), nullptr);
	assert(SUCCEEDED(result));

	// �L�[�{�[�h�f�o�C�X�̐���
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));
	// ���̓f�[�^�[�`���̃Z�b�g
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	// �r�����䃌�x���̃Z�b�g
	result = keyboard_->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	// �}�E�X�f�o�C�X�̐���
	result = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));
	// ���̓f�[�^�[�`���̃Z�b�g
	result = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	// �r�����䃌�x���̃Z�b�g
	result = mouse_->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update() {
	memcpy(preKeys_, curkeys_, kKeyDataNum);
	// �L�[�{�[�h���̎擾�J�n
	keyboard_->Acquire();
	// �S�L�[�̓��͏�Ԃ��擾����
	keyboard_->GetDeviceState(sizeof(curkeys_), curkeys_);
	
	preMouseState_ = curMouseState_;
	// �}�E�X���̎擾�J�n
	mouse_->Acquire();
	// �}�E�X�̏�Ԃ��擾����
	mouse_->GetDeviceState(sizeof(DIMOUSESTATE),&curMouseState_);

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(winApp_->GetHwnd(), &p);
	mousePos_ = Vector2(static_cast<float>(p.x), static_cast<float>(p.y));

	
	// ���݂̏�Ԃ�O��̏�Ԃɕۑ�
	for (auto& it : gamePads_) {
		it.preState = it.curState;
	}
	XINPUT_STATE tmpState = {};
	for (size_t i = 0; i < kXInputConnectMaxCount; i++) {
		XInputGetState((DWORD)i, &gamePads_[i].curState);
	}
}

