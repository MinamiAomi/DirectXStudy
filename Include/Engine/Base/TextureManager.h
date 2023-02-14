#pragma once
#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <array>
#include <string>
#include <wrl.h>

#include "DirectXCommon.h"

class TextureManager
{
private: // 型
	template<class TYPE>
	using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

	struct Texture {
		ComPtr<ID3D12Resource> buffer; // バッファ
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = {}; // CPUハンドル
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = {}; // GPUハンドル
		std::string name; // ファイル名
	};

private: // 定数
	static constexpr size_t kDescriptorCount = 50;

public: // 静的メンバ関数
	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();
	/// <summary>
	/// テクスチャをロード
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns>テクスチャハンドル</returns>
	static UINT LoadTexture(const std::string& filePath);

public: // メンバ関数
	/// <summary>
	/// リソースデスクを取得する
	/// </summary>
	/// <param name="texHanfle">テクスチャハンドル</param>
	/// <returns></returns>
	D3D12_RESOURCE_DESC GetResourceDesc(UINT texHandle);
	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize();
	/// <summary>
	/// ロードしたテクスチャを解放
	/// </summary>
	void ResetAll();
	/// <summary>
	/// コマンドリストにセットする
	/// </summary>
	/// <param name="cmdlist"></param>
	/// <param name="rootParamIndex"></param>
	/// <param name="texNumber"></param>
	void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* cmdlist, UINT rootParamIndex, UINT texNumber);

private: // メンバ関数
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	const TextureManager& operator=(const TextureManager&) = delete;
	/// <summary>
	/// テクスチャをロード(内部)
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	UINT InternalLoadTexture(const std::string& filePath);

private: // メンバ変数
	DirectXCommon* directXCommon_ = nullptr;

	ComPtr<ID3D12DescriptorHeap> srvHeap_;
	UINT descriptorIncrementSize_ = 0;
	std::array<Texture, kDescriptorCount> textures_;

	UINT nextLoadIndex_ = 0;

};

#endif
