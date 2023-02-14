#pragma once
#ifndef SPRITE_H_
#define SPRITE_H_

#include <array>
#include <d3d12.h>
#include <wrl.h>

#include "MathUtility.h"

class DirectXCommon;
class TextureManager;
class Camera2D;

class Sprite
{
public:
	enum BlendMode {
		kBlendModeNone,			// ブレンドなし
		kBlendModeNormal,		// 通常
		kBlendModeAdd,			// 加算合成
		kBlendModeSubtract,		// 減算合成
		kBlendModeMultiply,		// 乗算合成
		kBlendModeInversion,	// 色反転

		kBlendModeCount
	};

	struct VertexPosUv {
		Vector3 position;
		Vector2 uv;
	};

	struct ConstDataMatrixColor {
		Vector4 color;
		Matrix44 matrix;
	};

private:
	enum RootParameter {
		kConstData,
		kTexture,

		kRootParameterCount
	};

private:
	static const int kVertexCount = 4;

public:
	static void StaticInitalize();
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	static void SetPipeline(BlendMode blendMode);
	static void PostDraw();

private:
	static void CreatePipelineSet();

private:
	static DirectXCommon* sDiXCom_;
	static TextureManager* sTexMana_;
	static ID3D12GraphicsCommandList* sCmdList_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kBlendModeCount> sPipelineState_;
	static Matrix44 sDefultProjMatrix_;

public:
	inline void SetTextureHandle(UINT textureHandle);
	inline void SetColor(const Vector4& color);
	inline void SetPosition(const Vector2& position);
	inline void SetRotate(float rotate);
	inline void SetSize(const Vector2& size);
	inline void SetAnchorPoint(const Vector2& anchorPoint);
	inline void SetIsFlipX(bool isFlipX);
	inline void SetIsFlipY(bool isFlipY);
	inline void SetTextureBase(const Vector2& base);
	inline void SetTextureSize(const Vector2& size);
	inline void SetTextureRect(const Vector2& base, const Vector2& size);

	void CreateBuffers();
	void Draw();
	void Draw(const Camera2D& camera);

private:
	void UpdateVertexBuffer();
	void UpdateConstBuffer(const Matrix44& mat);

private:
	UINT textureHandle_ = 0;
	Vector2 position_ = {};
	float rotate_ = {};
	Vector2 size_ = Vector2(100.0f, 100.0f);
	Vector2 anchorPoint_ = {}; // 回転移動を行う基準点
	Matrix44 worldMatrix_; // ワールド行列
	Vector4 color_ = Color::White; // 色
	bool isFlipX_ = false; // 左右反転を行うか
	bool isFlipY_ = false; // 上下反転を行うか
	Vector2 textureBase_ = {};
	Vector2 textureSize_ = Vector2(1.0f, 1.0f);

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	bool isNeedVertexUpdate_ = false;
};

#pragma region // インライン関数の実装
inline void Sprite::SetTextureHandle(UINT textureHandle) {
	textureHandle_ = textureHandle;
}
inline void Sprite::SetColor(const Vector4& color) {
	color_ = color;
}
inline void Sprite::SetPosition(const Vector2& position) {
	position_ = position;
	isNeedVertexUpdate_ = true;
}
inline void Sprite::SetRotate(float rotate) {
	rotate_ = rotate;
	isNeedVertexUpdate_ = true;
}
inline void Sprite::SetSize(const Vector2& size) {
	size_ = size;
	isNeedVertexUpdate_ = true;
}
inline void Sprite::SetAnchorPoint(const Vector2& anchorPoint) {
	anchorPoint_ = anchorPoint;
	isNeedVertexUpdate_ = true;
}
inline void Sprite::SetIsFlipX(bool isFlipX) {
	isFlipX_ = isFlipX;
	isNeedVertexUpdate_ = true;
}
inline void Sprite::SetIsFlipY(bool isFlipY) {
	isFlipY_ = isFlipY;
	isNeedVertexUpdate_ = true;
}
inline void Sprite::SetTextureBase(const Vector2& base) {
	textureBase_ = base;
	isNeedVertexUpdate_ = true;
}
inline void Sprite::SetTextureSize(const Vector2& size) {
	textureSize_ = size;
	isNeedVertexUpdate_ = true;
}
inline void Sprite::SetTextureRect(const Vector2& base, const Vector2& size) {
	textureBase_ = base;
	textureSize_ = size;
	isNeedVertexUpdate_ = true;
}
#pragma endregion

#endif
