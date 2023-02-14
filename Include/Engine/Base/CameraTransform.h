#pragma once
#ifndef CAMERATRANSFORM_H_
#define CAMERATRANSFORM_H_

#include <d3d12.h>
#include <wrl.h>

#include "MathUtility.h"

class Camera3D
{
private:
	struct ConstDataCameraTransform {
		Matrix44 view;
		Matrix44 proj;
		Vector3 eye;
	};

public:
	/// <summary>
	/// �J�����̍��W���擾
	/// </summary>
	/// <returns></returns>
	inline const Vector3& GetEye()const { return eye_; }
	/// <summary>
	/// �J�����̍��W���Z�b�g
	/// </summary>
	/// <param name="eye"></param>
	inline void SetEye(const Vector3& eye) { eye_ = eye; }
	/// <summary>
	/// �����_���擾
	/// </summary>
	/// <returns></returns>
	inline const Vector3& GetTarget()const { return target_; }
	/// <summary>
	/// �����_���Z�b�g
	/// </summary>
	/// <param name="target"></param>
	inline void SetTarget(const Vector3& target) { target_ = target; }
	/// <summary>
	/// ��x�N�g�����擾
	/// </summary>
	/// <returns></returns>
	inline const Vector3& GetUp()const { return up_; }
	/// <summary>
	/// ��x�N�g�����Z�b�g
	/// </summary>
	/// <param name="up"></param>
	inline void SetUp(const Vector3& up) { up_ = up; }
	/// <summary>
	/// �r���[�s����擾
	/// </summary>
	/// <returns></returns>
	inline const Matrix44& GetViewMatrix() const { return viewMatrix_; }

	/// <summary>
	/// �㉺��p���擾
	/// </summary>
	/// <returns></returns>
	inline float GetFovAngleY() const { return fovAngleY_; }
	/// <summary>
	/// �㉺��p���Z�b�g
	/// </summary>
	/// <param name="fovAngleY"></param>
	inline void SetFovAngleY(float fovAngleY) { fovAngleY_ = fovAngleY; }
	/// <summary>
	/// �A�X�y�N�g����擾
	/// </summary>
	/// <returns></returns>
	inline float GetAspectRatio() const { return aspectRatio_; }
	/// <summary>
	/// �A�X�y�N�g����Z�b�g
	/// </summary>
	/// <param name="aspectRatio"></param>
	inline void SetAspectRatio(float aspectRatio) { aspectRatio_ = aspectRatio; }
	/// <summary>
	/// �O�[���擾
	/// </summary>
	/// <returns></returns>
	inline float GetNearZ() const { return nearZ_; }
	/// <summary>
	/// �O�[���Z�b�g
	/// </summary>
	/// <param name="nearZ"></param>
	inline void SetNearZ(float nearZ) { nearZ_ = nearZ; }
	/// <summary>
	/// ���[���擾
	/// </summary>
	/// <returns></returns>
	inline float GetFarZ() const { return farZ_; }
	/// <summary>
	/// ���[���Z�b�g
	/// </summary>
	/// <param name="farZ"></param>
	inline void SetFarZ(float farZ) { farZ_ = farZ; }
	/// <summary>
	/// �ˉe�s����擾
	/// </summary>
	/// <returns></returns>
	inline const Matrix44& GetProjMatrix() const { return projMatrix_; }

	/// <summary>
	/// �O���x�N�g�����擾
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetForward() const { return target_ - eye_; }
	/// <summary>
	/// �O���x�N�g�����Z�b�g
	/// </summary>
	/// <param name="ray"></param>
	inline void SetForward(const Vector3& ray) { target_ = eye_ + ray; }
	/// <summary>
	/// �E�x�N�g�����擾
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetRight() const { return Cross(GetForward(), up_).Normalized(); }
	
	/// <summary>
	/// �����_�ړ�
	/// </summary>
	/// <param name="move"></param>
	inline void MoveVector(const Vector3& move) { eye_ += move; target_ += move; }
	/// <summary>
	/// ���_�ړ�
	/// </summary>
	/// <param name="move"></param>
	inline void MoveEyeVector(const Vector3& move) { eye_ += move; }

	/// <summary>
	/// �o�b�t�@�𐶐�
	/// </summary>
	void CreateBuffer();
	/// <summary>
	/// �s����X�V
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// �s���]��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="rootParameterIndexWorldTransform">���[�g�p�����[�^�̃C���f�b�N�X</param>
	void SetGraphicsCommand(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

private:
	// ���_���W
	Vector3 eye_ = Vector3(0.0f, 0.0f, -1.0f);
	// �����_���W
	Vector3 target_ = Vector3(0.0f, 0.0f, 0.0f);
	// ��x�N�g��
	Vector3 up_ = Vector3(0.0f, 1.0f, 0.0f);
	// �r���[�s��
	Matrix44 viewMatrix_;

	// �㉺��p
	float fovAngleY_ = Math::ToRadians(45.0f);
	// �A�X�y�N�g��
	float aspectRatio_ = 1280.0f / 720.0f;
	// �O�[
	float nearZ_ = 0.1f;
	// ���[
	float farZ_ = 1000.0f;
	// �ˉe�s��
	Matrix44 projMatrix_;

	//�@�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	// �}�b�v�p
	ConstDataCameraTransform* map_ = nullptr;

};

class Camera2D {
public:
	/// <summary>
	/// �J�����̍��W���擾
	/// </summary>
	/// <returns></returns>
	const Vector2& GetPosition() const { return position_; }
	/// <summary>
	/// �J�����̍��W���Z�b�g
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector2& position) { position_ = position; }
	/// <summary>
	/// �J�����̔{�����擾
	/// </summary>
	/// <returns></returns>
	const Vector2& GetZoom() const { return zoom_; }
	/// <summary>
	/// �J�����̔{�����Z�b�g
	/// </summary>
	/// <param name="zoom"></param>
	void SetZoom(const Vector2& zoom) { zoom_ = zoom; }
	/// <summary>
	/// �r���[�s����擾
	/// </summary>
	/// <returns></returns>
	const Matrix44& GetViewMatrix() const { return viewMatrix_; }

	/// <summary>
	/// �J�������W�̍��[���擾
	/// </summary>
	/// <returns></returns>
	float GetLeft() const { return left_; }
	/// <summary>
	/// �J�������W�̍��[���Z�b�g
	/// </summary>
	/// <param name="left"></param>
	void SetLeft(float left) { left_ = left; }
	/// <summary>
	/// �J�������W�̉E�[���擾
	/// </summary>
	/// <returns></returns>
	float GetRight() const { return right_; }
	/// <summary>
	/// �J�������W�̉E�[���Z�b�g
	/// </summary>
	/// <param name="right"></param>
	void SetRight(float right) { right_ = right; }
	/// <summary>
	/// �J�������W�̏�[���擾
	/// </summary>
	/// <returns></returns>
	float GetTop() const { return top_; }
	/// <summary>
	/// �J�������W�̏�[���Z�b�g
	/// </summary>
	/// <param name="top"></param>
	void SetTop(float top) { top_ = top; }
	/// <summary>
	/// �J�������W�̉��[���擾
	/// </summary>
	/// <returns></returns>
	float GetBottom() const { return bottom_; }
	/// <summary>
	/// �J�������W�̉��[���Z�b�g
	/// </summary>
	/// <param name="bottom"></param>
	void SetBottom(float bottom) { bottom_ = bottom; }
	/// <summary>
	/// �ˉe�s����擾
	/// </summary>
	/// <returns></returns>
	const Matrix44& GetProjMatrix() const { return projMatrix_; }

	/// <summary>
	/// �r���[�ˉe�s����擾
	/// </summary>
	/// <returns></returns>
	const Matrix44& GetViewProjMatrix() const { return viewProjMatrix_; }

	/// <summary>
	/// �s����X�V
	/// </summary>
	void UpdateMatrix();

private:
	// �J�����̃��[���h���W
	Vector2 position_ = {};
	// �Y�[��
	Vector2 zoom_ = Vector2(1.0f, 1.0f);
	// �r���[�s��
	Matrix44 viewMatrix_;

	// ��
	float left_ = -640.0f;
	// �E
	float right_ = 640.0f;
	// ��
	float top_ = 360;
	// ��
	float bottom_ = -360;
	// �O�[
	float nearZ_ = 0.1f;
	// ���[
	float farZ_ = 1000.0f;
	// �ˉe�s��
	Matrix44 projMatrix_;
	// �r���[�ˉe�s��
	Matrix44 viewProjMatrix_;

};

#endif
