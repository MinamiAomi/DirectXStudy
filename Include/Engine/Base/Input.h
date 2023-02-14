#pragma once
#ifndef INPUT_H_
#define INPUT_H_

#include <array>
#include <dinput.h>
#include <stdint.h>
#include <Xinput.h>
#include <vector>
#include <wrl.h>

#include "MathUtility.h"
#include "WinApp.h"

#define DIRECTINPUT_VERSION 0x0800 // DirectIput�̃o�[�W�����w��
#define MOOUSE_ON_VALUE 0x80

enum MouseButton 
{
	kMouseButtonLeft,
	kMouseButtonRight,
	kMouseButtonMid,
};

// �R���g���[���[�{�^��
enum PadButton {
	kPadButtonUp = 0x0001,			// ��
	kPadButtonDown = 0x0002,			// ��
	kPadButtonLeft = 0x0004,			// ��
	kPadButtonRight = 0x0008,		// �E
	kPadButtonStart = 0x0010,		// ���j���[
	kPadButtonBack = 0x0020,			// �r���[
	kPadButtonLeftThumb = 0x0040,			// L3
	kPadButtonRightThumb = 0x0080,			// R3
	kPadButtonLeftShoulder = 0x0100,		// L1
	kPadButtonRightShoulder = 0x0200,		// R1
	kPadButtonA = 0x1000,			// A
	kPadButtonB = 0x2000,			// B
	kPadButtonX = 0x4000,			// X
	kPadButtonY = 0x8000,			// Y
	kPadLeftTrigger = 0x000F0000,	// L2 (�K���Ȑ��������蓖��)
	kPadRightTrigger = 0x00FF0000,	// R2 (�K���Ȑ��������蓖��)
};

// �X�e�B�b�N�̌���
enum StickDirection {
	kStickDirectionUp,			// LEFT STICK DIRECTION UP
	kStickDirectionDown,		// LEFT STICK DIRECTION DOWN
	kStickDirectionRight,		// LEFT STICK DIRECTION RIGHT
	kStickDirectionLeft,		// LEFT STICK DIRECTION LEFT
};

class Input 
{

private:
	template<class TYPE>
	using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

private:
	struct GamePad
	{
		XINPUT_STATE curState = {};
		XINPUT_STATE preState = {};
		BYTE triggerDeadZone = XINPUT_GAMEPAD_TRIGGER_THRESHOLD; // �g���K�[�̃f�b�h�]�[��
		SHORT leftStickDeadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE; // ���X�e�B�b�N�̃f�b�h�]�[��
		SHORT rightStickDeadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;	// �E�X�e�B�b�N�̃f�b�h�]�[��
	};

private:
	static constexpr int kKeyDataNum = 256;
	static constexpr int kXInputConnectMaxCount = 4;

public: // �ÓI�����o�֐�
	static Input* GetInstance();

public: // �����o�֐�
#pragma region �L�[�{�[�h�֌W

	/// <summary>
	/// ������Ă���
	/// </summary>
	/// <param name="keycode">�L�[�R�[�h</param>
	/// <returns></returns>
	inline bool IsKeyPressed(uint8_t keycode) const;
	/// <summary>
	/// ������Ă��Ȃ�
	/// </summary>
	/// <param name="keycode">�L�[�R�[�h</param>
	/// <returns></returns>
	inline bool IsKeyEmpty(uint8_t keycode) const;
	/// <summary>
	/// �������u��
	/// </summary>
	/// <param name="keycode">�L�[�R�[�h</param>
	/// <returns></returns>
	inline bool IsKeyTrigger(uint8_t keycode) const;
	/// <summary>
	/// �������u��
	/// </summary>
	/// <param name="keycode">�L�[�R�[�h</param>
	/// <returns></returns>
	inline bool IsKeyRelease(uint8_t keycode) const;
	
#pragma endregion

#pragma region �}�E�X�֌W
	
	/// <summary>
	/// �}�E�X�{�^����������Ă���
	/// </summary>
	/// <param name="buttontype">�{�^���R�[�h</param>
	/// <returns></returns>
	inline bool IsMousePressed(MouseButton buttontype) const;
	/// <summary>
	/// �}�E�X�{�^����������Ă��Ȃ�
	/// </summary>
	/// <param name="buttontype">�{�^���R�[�h</param>
	/// <returns></returns>
	inline bool IsMouseEmpty(MouseButton buttontype) const;
	/// <summary>
	/// �}�E�X�{�^���������ꂽ�u��
	/// </summary>
	/// <param name="buttontype">�{�^���R�[�h</param>
	/// <returns></returns>
	inline bool IsMouseTrigger(MouseButton buttontype) const;
	/// <summary>
	/// �}�E�X�{�^���𗣂����u��
	/// </summary>
	/// <param name="buttontype">�{�^���R�[�h</param>
	/// <returns></returns>
	inline bool IsMouseRelease(MouseButton buttontype) const;
	/// <summary>
	/// �}�E�X�̃X�N���[�����W
	/// </summary>
	/// <returns></returns>
	inline const Vector2& GetMousePosition() const;
	/// <summary>
	/// �}�E�X�̈ړ���
	/// </summary>
	/// <returns></returns>
	inline const Vector2 GetMouseMove() const;
	/// <summary>
	/// �}�E�X�z�C�[��
	/// </summary>
	/// <returns></returns>
	inline float GetWheel() const;
	
#pragma endregion

#pragma region �R���g���[���[�֌W
	
	/// <summary>
	/// �g���K�[�̃f�b�h�]�[����ύX����
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="deadzone">�f�b�h�]�[���@�F�@0�`255(0 ~ 0xFF)</param>
	inline void SetTriggerDeadZone(int index, int deadzone);
	/// <summary>
	/// �E�X�e�B�b�N�̃f�b�h�]�[����ύX����
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="deadzone">�f�b�h�]�[���@�F�@0�`32768(0 ~ 0x8000)</param>
	inline void SetRightStickDeadZone(int index, int deadzone);
	/// <summary>
	/// ���X�e�B�b�N�̃f�b�h�]�[����ύX����
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="deadzone">�f�b�h�]�[���@�F�@0�`32768(0 ~ 0x8000)</param>
	inline void SetLeftStickDeadZone(int index, int deadzone);

	/// <summary>
	/// �g���K�[�̃f�b�h�]�[����ύX����
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	inline int GetTriggerDeadZone(int index);
	/// <summary>
	/// �E�X�e�B�b�N�̃f�b�h�]�[����ύX����
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	inline int GetRightStickDeadZone(int index);
	/// <summary>
	/// ���X�e�B�b�N�̃f�b�h�]�[����ύX����
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	inline int GetLeftStickDeadZone(int index);

	/// <summary>
	/// �{�^���������ꂽ��
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="button">�{�^���ԍ�</param>
	/// <returns></returns>
	bool IsPadButtonTrigger(int index, PadButton button) const;
	/// <summary>
	/// �{�^����������Ă��鎞
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="button">�{�^���ԍ�</param>
	/// <returns></returns>
	bool IsPadButtonPressed(int index, PadButton button) const;
	/// <summary>
	/// �{�^���������ꂽ��
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="button">�{�^���ԍ�</param>
	/// <returns></returns>
	bool IsPadButtonRelease(int index, PadButton button) const;
	/// <summary>
	/// �E�X�e�B�b�N���w��̌����ɓ|���ꂽ��
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="direction">�X�e�B�b�N����</param>
	/// <returns></returns>
	bool IsRightStickDirection(int index, StickDirection direction) const;
	/// <summary>
	/// ���X�e�B�b�N���w��̌����ɓ|���ꂽ��
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="direction">�X�e�B�b�N����</param>
	/// <returns></returns>
	bool IsLeftStickDirection(int index, StickDirection direction) const;
	/// <summary>
	/// �E�g���K�[
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="out">0�`255</param>
	/// <returns>�����@or�@���s</returns>
	int GetRightTrigger(int index) const;
	/// <summary>
	/// ���g���K�[
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="out">0�`255</param>
	/// <returns>�����@or�@���s</returns>
	int GetLeftTrigger(int index) const;
	/// <summary>
	/// �E�X�e�B�b�N
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="outx">X��</param>
	/// <param name="outy">Y��</param>
	void GetRightStick(int index, int* outx, int* outy) const;
	/// <summary>
	/// �E�X�e�B�b�N
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	Vector2 GetRightStick(int index) const;
	/// <summary>
	/// ���X�e�B�b�N
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	/// <param name="outx">X��</param>
	/// <param name="outy">Y��</param>
	void GetLeftStick(int index, int* outx, int* outy) const;
	/// <summary>
	/// ���X�e�B�b�N
	/// </summary>
	/// <param name="number">�R���g���[���[�ԍ�</param>
	Vector2 GetLeftStick(int index) const;

#pragma endregion

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

private:
	WinApp* winApp_ = nullptr;

	ComPtr<IDirectInput8> directInput_;
	ComPtr<IDirectInputDevice8> keyboard_;
	ComPtr<IDirectInputDevice8> mouse_;

	BYTE curkeys_[kKeyDataNum] = {}; // ���݂̃L�[�̓��͏��
	BYTE preKeys_[kKeyDataNum] = {}; // �O��̃L�[�̓��͏��

	DIMOUSESTATE2 curMouseState_ = {}; // ���݂̃}�E�X�̓��͏��
	DIMOUSESTATE2 preMouseState_ = {}; // �O��̃}�E�X�̓��͏��
	Vector2 mousePos_;					// �}�E�X�̃X�N���[�����W

	std::array<GamePad, kXInputConnectMaxCount> gamePads_;

};

#pragma region �C�����C����`

inline bool Input::IsKeyPressed(uint8_t keycode) const { return curkeys_[keycode] != 0; }
inline bool Input::IsKeyEmpty(uint8_t keycode) const { return curkeys_[keycode] == 0; }
inline bool Input::IsKeyTrigger(uint8_t keycode) const { return preKeys_[keycode] == 0 && curkeys_[keycode] != 0; }
inline bool Input::IsKeyRelease(uint8_t keycode) const { return preKeys_[keycode] != 0 && curkeys_[keycode] == 0; }
inline bool Input::IsMousePressed(MouseButton buttontype) const {
	return curMouseState_.rgbButtons[static_cast<int>(buttontype)] & MOOUSE_ON_VALUE;
}
inline bool Input::IsMouseEmpty(MouseButton buttontype) const {
	return !(curMouseState_.rgbButtons[static_cast<int>(buttontype)] & MOOUSE_ON_VALUE);
}
inline bool Input::IsMouseTrigger(MouseButton buttontype) const {
	return !(preMouseState_.rgbButtons[static_cast<int>(buttontype)] & MOOUSE_ON_VALUE) &&
		curMouseState_.rgbButtons[static_cast<int>(buttontype)] & MOOUSE_ON_VALUE;
}
inline bool Input::IsMouseRelease(MouseButton buttontype) const {
	return preMouseState_.rgbButtons[static_cast<int>(buttontype)] & MOOUSE_ON_VALUE &&
		!(curMouseState_.rgbButtons[static_cast<int>(buttontype)] & MOOUSE_ON_VALUE);
}
inline const Vector2& Input::GetMousePosition() const { return mousePos_; }
inline const Vector2 Input::GetMouseMove() const { return Vector2(static_cast<float>(curMouseState_.lX), static_cast<float>(curMouseState_.lY)); }
inline float Input::GetWheel() const { return static_cast<float>(curMouseState_.lZ); }

inline void Input::SetTriggerDeadZone(int index, int deadzone)
{
	gamePads_[index].triggerDeadZone = deadzone;
}

inline void Input::SetRightStickDeadZone(int index, int deadzone)
{
	gamePads_[index].rightStickDeadZone = deadzone;
}

inline void Input::SetLeftStickDeadZone(int index, int deadzone)
{
	gamePads_[index].leftStickDeadZone = deadzone;
}

inline int Input::GetTriggerDeadZone(int index)
{
	return gamePads_[index].triggerDeadZone;
}

inline int Input::GetRightStickDeadZone(int index)
{
	return gamePads_[index].rightStickDeadZone;
}

inline int Input::GetLeftStickDeadZone(int index)
{
	return gamePads_[index].leftStickDeadZone;
}

inline bool Input::IsPadButtonTrigger(int index, PadButton button) const
{
	switch (button)
	{
	case kPadLeftTrigger: // ���g���K�[
		return (gamePads_[index].curState.Gamepad.bLeftTrigger > gamePads_[index].triggerDeadZone) &&
			!(gamePads_[index].preState.Gamepad.bLeftTrigger > gamePads_[index].triggerDeadZone);

	case kPadRightTrigger: // �E�g���K�[
		return (gamePads_[index].curState.Gamepad.bRightTrigger > gamePads_[index].triggerDeadZone) &&
			!(gamePads_[index].preState.Gamepad.bRightTrigger > gamePads_[index].triggerDeadZone);

	default: // ���̑��{�^��
		return (gamePads_[index].curState.Gamepad.wButtons & button) &&
			!(gamePads_[index].preState.Gamepad.wButtons & button);

	}
	return false;
}

inline bool Input::IsPadButtonPressed(int index, PadButton button) const
{
	switch (button)
	{
	case kPadLeftTrigger: // ���g���K�[
		return (gamePads_[index].curState.Gamepad.bLeftTrigger > gamePads_[index].triggerDeadZone);

	case kPadRightTrigger: // �E�g���K�[
		return (gamePads_[index].curState.Gamepad.bRightTrigger > gamePads_[index].triggerDeadZone);

	default: // ���̑��{�^��
		return (gamePads_[index].curState.Gamepad.wButtons & button);

	}
	return false;
}

inline bool Input::IsPadButtonRelease(int index, PadButton button) const
{
	switch (button)
	{
	case kPadLeftTrigger: // ���g���K�[
		return !(gamePads_[index].curState.Gamepad.bLeftTrigger > gamePads_[index].triggerDeadZone) &&
			(gamePads_[index].preState.Gamepad.bLeftTrigger > gamePads_[index].triggerDeadZone);

	case kPadRightTrigger: // �E�g���K�[
		return !(gamePads_[index].curState.Gamepad.bRightTrigger > gamePads_[index].triggerDeadZone) &&
			(gamePads_[index].preState.Gamepad.bRightTrigger > gamePads_[index].triggerDeadZone);

	default: // ���̑��{�^��
		return !(gamePads_[index].curState.Gamepad.wButtons & button) &&
			(gamePads_[index].preState.Gamepad.wButtons & button);

	}
	return false;
}

inline bool Input::IsRightStickDirection(int index, StickDirection direction) const
{
	switch (direction)
	{
	case kStickDirectionUp:
		return gamePads_[index].curState.Gamepad.sThumbRY >= gamePads_[index].rightStickDeadZone;

	case kStickDirectionDown:
		return gamePads_[index].curState.Gamepad.sThumbRY <= -gamePads_[index].rightStickDeadZone;

	case kStickDirectionRight:
		return gamePads_[index].curState.Gamepad.sThumbRX >= gamePads_[index].rightStickDeadZone;

	case kStickDirectionLeft:
		return gamePads_[index].curState.Gamepad.sThumbRX <= -gamePads_[index].rightStickDeadZone;
	}
	return false;
}

inline bool Input::IsLeftStickDirection(int index, StickDirection direction) const
{
	switch (direction)
	{
	case kStickDirectionUp:
		return gamePads_[index].curState.Gamepad.sThumbLY >= gamePads_[index].leftStickDeadZone;

	case kStickDirectionDown:
		return gamePads_[index].curState.Gamepad.sThumbLY <= -gamePads_[index].leftStickDeadZone;

	case kStickDirectionRight:
		return gamePads_[index].curState.Gamepad.sThumbLX >= gamePads_[index].leftStickDeadZone;

	case kStickDirectionLeft:
		return gamePads_[index].curState.Gamepad.sThumbLX <= -gamePads_[index].leftStickDeadZone;
	}
	return false;
}

inline int Input::GetRightTrigger(int index) const
{
	if (gamePads_[index].curState.Gamepad.bRightTrigger > gamePads_[index].triggerDeadZone) {
		return gamePads_[index].curState.Gamepad.bRightTrigger;
	}
	return 0;
}

inline int Input::GetLeftTrigger(int index) const
{
	if (gamePads_[index].curState.Gamepad.bLeftTrigger > gamePads_[index].triggerDeadZone) {
		return gamePads_[index].curState.Gamepad.bLeftTrigger;
	}
	return 0;
}

inline void Input::GetRightStick(int index, int* outx, int* outy) const
{
	Vector2 tmp = Vector2(static_cast<float>(gamePads_[index].curState.Gamepad.sThumbRX), static_cast<float>(gamePads_[index].curState.Gamepad.sThumbRY));
	if (tmp.Length() > gamePads_[index].rightStickDeadZone) {
		*outx = gamePads_[index].curState.Gamepad.sThumbRX;
		*outy = gamePads_[index].curState.Gamepad.sThumbRY;
		return;
	}
	*outx = 0;
	*outy = 0;
	return;
}

inline Vector2 Input::GetRightStick(int index) const
{
	Vector2 tmp = Vector2(static_cast<float>(gamePads_[index].curState.Gamepad.sThumbRX), static_cast<float>(gamePads_[index].curState.Gamepad.sThumbRY));
	if (tmp.Length() > gamePads_[index].rightStickDeadZone) {
		return tmp;
	}
	return Vector2::Zero;
}

inline void Input::GetLeftStick(int index, int* outx, int* outy) const
{
	Vector2 tmp = Vector2(static_cast<float>(gamePads_[index].curState.Gamepad.sThumbLX), static_cast<float>(gamePads_[index].curState.Gamepad.sThumbLY));
	if (tmp.Length() > gamePads_[index].leftStickDeadZone) {
		*outx = gamePads_[index].curState.Gamepad.sThumbLX;
		*outy = gamePads_[index].curState.Gamepad.sThumbLY;
		return;
	}
	*outx = 0;
	*outy = 0;
	return;
}

inline Vector2 Input::GetLeftStick(int index) const
{
	Vector2 tmp = Vector2(static_cast<float>(gamePads_[index].curState.Gamepad.sThumbLX), static_cast<float>(gamePads_[index].curState.Gamepad.sThumbLY));
	if (tmp.Length() > gamePads_[index].leftStickDeadZone) {
		return tmp;
	}
	return Vector2::Zero;
}

#pragma endregion

#endif // INPUT_H_
