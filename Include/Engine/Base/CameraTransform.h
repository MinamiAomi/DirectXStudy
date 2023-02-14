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
	/// カメラの座標を取得
	/// </summary>
	/// <returns></returns>
	inline const Vector3& GetEye()const { return eye_; }
	/// <summary>
	/// カメラの座標をセット
	/// </summary>
	/// <param name="eye"></param>
	inline void SetEye(const Vector3& eye) { eye_ = eye; }
	/// <summary>
	/// 注視点を取得
	/// </summary>
	/// <returns></returns>
	inline const Vector3& GetTarget()const { return target_; }
	/// <summary>
	/// 注視点をセット
	/// </summary>
	/// <param name="target"></param>
	inline void SetTarget(const Vector3& target) { target_ = target; }
	/// <summary>
	/// 上ベクトルを取得
	/// </summary>
	/// <returns></returns>
	inline const Vector3& GetUp()const { return up_; }
	/// <summary>
	/// 上ベクトルをセット
	/// </summary>
	/// <param name="up"></param>
	inline void SetUp(const Vector3& up) { up_ = up; }
	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	/// <returns></returns>
	inline const Matrix44& GetViewMatrix() const { return viewMatrix_; }

	/// <summary>
	/// 上下画角を取得
	/// </summary>
	/// <returns></returns>
	inline float GetFovAngleY() const { return fovAngleY_; }
	/// <summary>
	/// 上下画角をセット
	/// </summary>
	/// <param name="fovAngleY"></param>
	inline void SetFovAngleY(float fovAngleY) { fovAngleY_ = fovAngleY; }
	/// <summary>
	/// アスペクト比を取得
	/// </summary>
	/// <returns></returns>
	inline float GetAspectRatio() const { return aspectRatio_; }
	/// <summary>
	/// アスペクト比をセット
	/// </summary>
	/// <param name="aspectRatio"></param>
	inline void SetAspectRatio(float aspectRatio) { aspectRatio_ = aspectRatio; }
	/// <summary>
	/// 前端を取得
	/// </summary>
	/// <returns></returns>
	inline float GetNearZ() const { return nearZ_; }
	/// <summary>
	/// 前端をセット
	/// </summary>
	/// <param name="nearZ"></param>
	inline void SetNearZ(float nearZ) { nearZ_ = nearZ; }
	/// <summary>
	/// 奥端を取得
	/// </summary>
	/// <returns></returns>
	inline float GetFarZ() const { return farZ_; }
	/// <summary>
	/// 奥端をセット
	/// </summary>
	/// <param name="farZ"></param>
	inline void SetFarZ(float farZ) { farZ_ = farZ; }
	/// <summary>
	/// 射影行列を取得
	/// </summary>
	/// <returns></returns>
	inline const Matrix44& GetProjMatrix() const { return projMatrix_; }

	/// <summary>
	/// 前方ベクトルを取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetForward() const { return target_ - eye_; }
	/// <summary>
	/// 前方ベクトルをセット
	/// </summary>
	/// <param name="ray"></param>
	inline void SetForward(const Vector3& ray) { target_ = eye_ + ray; }
	/// <summary>
	/// 右ベクトルを取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetRight() const { return Cross(GetForward(), up_).Normalized(); }
	
	/// <summary>
	/// 注視点移動
	/// </summary>
	/// <param name="move"></param>
	inline void MoveVector(const Vector3& move) { eye_ += move; target_ += move; }
	/// <summary>
	/// 視点移動
	/// </summary>
	/// <param name="move"></param>
	inline void MoveEyeVector(const Vector3& move) { eye_ += move; }

	/// <summary>
	/// バッファを生成
	/// </summary>
	void CreateBuffer();
	/// <summary>
	/// 行列を更新
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// 行列を転送
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="rootParameterIndexWorldTransform">ルートパラメータのインデックス</param>
	void SetGraphicsCommand(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

private:
	// 視点座標
	Vector3 eye_ = Vector3(0.0f, 0.0f, -1.0f);
	// 注視点座標
	Vector3 target_ = Vector3(0.0f, 0.0f, 0.0f);
	// 上ベクトル
	Vector3 up_ = Vector3(0.0f, 1.0f, 0.0f);
	// ビュー行列
	Matrix44 viewMatrix_;

	// 上下画角
	float fovAngleY_ = Math::ToRadians(45.0f);
	// アスペクト比
	float aspectRatio_ = 1280.0f / 720.0f;
	// 前端
	float nearZ_ = 0.1f;
	// 奥端
	float farZ_ = 1000.0f;
	// 射影行列
	Matrix44 projMatrix_;

	//　定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	// マップ用
	ConstDataCameraTransform* map_ = nullptr;

};

class Camera2D {
public:
	/// <summary>
	/// カメラの座標を取得
	/// </summary>
	/// <returns></returns>
	const Vector2& GetPosition() const { return position_; }
	/// <summary>
	/// カメラの座標をセット
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector2& position) { position_ = position; }
	/// <summary>
	/// カメラの倍率を取得
	/// </summary>
	/// <returns></returns>
	const Vector2& GetZoom() const { return zoom_; }
	/// <summary>
	/// カメラの倍率をセット
	/// </summary>
	/// <param name="zoom"></param>
	void SetZoom(const Vector2& zoom) { zoom_ = zoom; }
	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	/// <returns></returns>
	const Matrix44& GetViewMatrix() const { return viewMatrix_; }

	/// <summary>
	/// カメラ座標の左端を取得
	/// </summary>
	/// <returns></returns>
	float GetLeft() const { return left_; }
	/// <summary>
	/// カメラ座標の左端をセット
	/// </summary>
	/// <param name="left"></param>
	void SetLeft(float left) { left_ = left; }
	/// <summary>
	/// カメラ座標の右端を取得
	/// </summary>
	/// <returns></returns>
	float GetRight() const { return right_; }
	/// <summary>
	/// カメラ座標の右端をセット
	/// </summary>
	/// <param name="right"></param>
	void SetRight(float right) { right_ = right; }
	/// <summary>
	/// カメラ座標の上端を取得
	/// </summary>
	/// <returns></returns>
	float GetTop() const { return top_; }
	/// <summary>
	/// カメラ座標の上端をセット
	/// </summary>
	/// <param name="top"></param>
	void SetTop(float top) { top_ = top; }
	/// <summary>
	/// カメラ座標の下端を取得
	/// </summary>
	/// <returns></returns>
	float GetBottom() const { return bottom_; }
	/// <summary>
	/// カメラ座標の下端をセット
	/// </summary>
	/// <param name="bottom"></param>
	void SetBottom(float bottom) { bottom_ = bottom; }
	/// <summary>
	/// 射影行列を取得
	/// </summary>
	/// <returns></returns>
	const Matrix44& GetProjMatrix() const { return projMatrix_; }

	/// <summary>
	/// ビュー射影行列を取得
	/// </summary>
	/// <returns></returns>
	const Matrix44& GetViewProjMatrix() const { return viewProjMatrix_; }

	/// <summary>
	/// 行列を更新
	/// </summary>
	void UpdateMatrix();

private:
	// カメラのワールド座標
	Vector2 position_ = {};
	// ズーム
	Vector2 zoom_ = Vector2(1.0f, 1.0f);
	// ビュー行列
	Matrix44 viewMatrix_;

	// 左
	float left_ = -640.0f;
	// 右
	float right_ = 640.0f;
	// 上
	float top_ = 360;
	// 下
	float bottom_ = -360;
	// 前端
	float nearZ_ = 0.1f;
	// 奥端
	float farZ_ = 1000.0f;
	// 射影行列
	Matrix44 projMatrix_;
	// ビュー射影行列
	Matrix44 viewProjMatrix_;

};

#endif
