#pragma once
#ifndef WORLDTRANSFORM_H_
#define WORLDTRANSFORM_H_

#include <d3d12.h>
#include <wrl.h>

#include "MathUtility.h"

class WorldTransform
{
private:
	struct ConstDataWorldTransform {
		Matrix44 world;
	};

public:
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

public:
	// 座標
	Vector3 position;
	// 回転
	Quaternion rotate;
	// 拡縮
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
	// ワールド行列
	Matrix44 worldMatrix;
	// 親へのポインタ
	const WorldTransform* parent = nullptr;

private:
	//　定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	// マップ用
	ConstDataWorldTransform* map_ = nullptr;
};

#endif
