#pragma once
#ifndef DIRECTXCOMMON_H_
#define DIRECTXCOMMON_H_

#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>

#include "WinApp.h"
#include "MathUtility.h"

class DirectXCommon final
{
private:
	template<class TYPE>
	using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

public: // 静的メンバ関数
	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns>インスタンスのポインタ</returns>
	static DirectXCommon* GetInstance();

public: // メンバ関数
	/// <summary>
	/// デバイスを取得
	/// </summary>
	/// <returns></returns>
	inline ID3D12Device* GetDevice() { return device_.Get(); }
	/// <summary>
	/// コマンドリストを取得
	/// </summary>
	/// <returns></returns>
	inline ID3D12GraphicsCommandList* GetCommandList() { return cmdList_.Get(); }
	/// <summary>
	/// スワップチェーンを取得
	/// </summary>
	/// <returns></returns>
	inline IDXGISwapChain4* GetSwapChain() { return swapChain_.Get(); }
	/// <summary>
	/// バックバッファを取得
	/// </summary>
	/// <param name="i">番号</param>
	/// <returns></returns>
	inline ID3D12Resource* GetBackBaffer(size_t i) { return backBuffers_[i].Get(); }

	/// <summary>
	/// クリアカラーをセット
	/// </summary>
	/// <param name="color"></param>
	inline void SetClearColor(const Vector4& color) { clearColor_ = color; }
	/// <summary>
	/// ビューポートをセット
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void SetViewport(float left, float top, float width, float height);
	/// <summary>
	/// シザー矩形をセット
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	void SetScissorRect(int left, int top, int right, int bottom);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize();
	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();
	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();
	/// <summary>
	/// レンダーターゲットをクリア
	/// </summary>
	void ClearRenderTarget();
	/// <summary>
	/// 深度バッファをクリア
	/// </summary>
	void ClearDepthBuffer();

	/// <summary>
	/// リソースバッファの生成
	/// </summary>
	/// <param name="size"></param>
	/// <returns></returns>
	ComPtr<ID3D12Resource> CreateResourceBuffer(UINT64 size);

private: // メンバ関数
	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

	void CreateFactory();
	void CreateDevice();
	void CreateCommand();
	void CreateSwapChain();
	void CreateRenderTargetView();
	void CreateDepthBuffer();
	void CreateFence();

#ifdef _DEBUG
	void DebugLayer();
	void DebugSuppressError();
#endif // _DEBUG

private: // メンバ変数
	WinApp* winApp_ = nullptr;

	ComPtr<ID3D12Device>				 device_;
	ComPtr<IDXGIFactory6>				 dxgiFactory_;
	ComPtr<ID3D12CommandAllocator>		 cmdAllocator_;
	ComPtr<ID3D12GraphicsCommandList>	 cmdList_;
	ComPtr<ID3D12CommandQueue>			 cmdQueue_;
	ComPtr<IDXGISwapChain4>				 swapChain_;
	std::vector<ComPtr<ID3D12Resource>>	 backBuffers_;
	ComPtr<ID3D12Resource>				 depthBuff_;
	ComPtr<ID3D12Fence>					 fence_;
	ComPtr<ID3D12DescriptorHeap>		 rtvHeap_;
	ComPtr<ID3D12DescriptorHeap>		 dsvHeap_;
	UINT64 fenceVal_ = 0;
	Vector4 clearColor_ = Vector4(0.1f, 0.25f, 0.5f, 0.0f);

};

#ifdef _DEBUG
void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob);
#endif // _DEBUG

#endif // DIRECTXCOMMON_H_
