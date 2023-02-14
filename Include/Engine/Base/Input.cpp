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
	// DirectInputの初期化
	result = DirectInput8Create(
		winApp_->GetWindowClassHInstance(), DIRECTINPUT_HEADER_VERSION,
		IID_IDirectInput8, static_cast<void**>(&directInput_), nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));
	// 入力データー形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	// マウスデバイスの生成
	result = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));
	// 入力データー形式のセット
	result = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = mouse_->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update() {
	memcpy(preKeys_, curkeys_, kKeyDataNum);
	// キーボード情報の取得開始
	keyboard_->Acquire();
	// 全キーの入力状態を取得する
	keyboard_->GetDeviceState(sizeof(curkeys_), curkeys_);
	
	preMouseState_ = curMouseState_;
	// マウス情報の取得開始
	mouse_->Acquire();
	// マウスの状態を取得する
	mouse_->GetDeviceState(sizeof(DIMOUSESTATE),&curMouseState_);

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(winApp_->GetHwnd(), &p);
	mousePos_ = Vector2(static_cast<float>(p.x), static_cast<float>(p.y));

	
	// 現在の状態を前回の状態に保存
	for (auto& it : gamePads_) {
		it.preState = it.curState;
	}
	XINPUT_STATE tmpState = {};
	for (size_t i = 0; i < kXInputConnectMaxCount; i++) {
		XInputGetState((DWORD)i, &gamePads_[i].curState);
	}
}

