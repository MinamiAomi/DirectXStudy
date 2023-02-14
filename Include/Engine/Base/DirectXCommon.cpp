#include "DirectXCommon.h"

#include <cassert>
#include <d3dx12.h>
#include <string>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

DirectXCommon* DirectXCommon::GetInstance() {
	static DirectXCommon instance;
	return &instance;
}

void DirectXCommon::SetViewport(float left, float top, float width, float height) {
	// ビューポート
	D3D12_VIEWPORT viewport = CD3DX12_VIEWPORT(left, top, width, height);
	cmdList_->RSSetViewports(1, &viewport);
}

void DirectXCommon::SetScissorRect(int left, int top, int right, int bottom) {
	// シザー矩形
	D3D12_RECT scissorRect = CD3DX12_RECT(left, top, right, bottom);
	cmdList_->RSSetScissorRects(1, &scissorRect);
}

void DirectXCommon::Initalize() {
	winApp_ = WinApp::GetInstance();

#ifdef _DEBUG
	DebugLayer();
#endif // _DEBUG

	CreateFactory();
	CreateDevice();

#ifdef _DEBUG
	DebugSuppressError();
#endif // _DEBUG

	CreateCommand();
	CreateSwapChain();
	CreateRenderTargetView();
	CreateDepthBuffer();
	CreateFence();
}

void DirectXCommon::PreDraw() {
	// バックバッファの番号を取得（２つなので０番か１番）
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffers_[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	cmdList_->ResourceBarrier(1, &barrierDesc);

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeap_->GetCPUDescriptorHandleForHeapStart(), bbIndex,
		device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	cmdList_->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	ClearRenderTarget();
	ClearDepthBuffer();


	// ビューポート
	SetViewport(0.0f, 0.0f, static_cast<float>(winApp_->GetWindowWidth()), static_cast<float>(winApp_->GetWindowHeight()));
	// シザー矩形
	SetScissorRect(0, 0, winApp_->GetWindowWidth(), winApp_->GetWindowHeight());
}

void DirectXCommon::PostDraw() {
	HRESULT result = S_FALSE;
	// バックバッファの番号を取得（２つなので０番か１番）
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffers_[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);
	// リソースバリアを戻す
	cmdList_->ResourceBarrier(1, &barrierDesc);

	// 命令のクローズ
	result = cmdList_->Close();
	assert(SUCCEEDED(result));

	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { cmdList_.Get() };
	cmdQueue_->ExecuteCommandLists(1, cmdLists);

	// 画面に表示するバッファをフリップ（裏表の入れ替え）
	result = swapChain_->Present(1, 0);
	assert(SUCCEEDED(result));

	// コマンドの実行完了を待つ
	cmdQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		if (event != 0) {
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
	// キューをクリア
	result = cmdAllocator_->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドリストをためる準備
	result = cmdList_->Reset(cmdAllocator_.Get(), nullptr);
	assert(SUCCEEDED(result));
}

void DirectXCommon::ClearRenderTarget() {
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeap_->GetCPUDescriptorHandleForHeapStart(), bbIndex,
		device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	FLOAT clearcolor[] = { clearColor_.x, clearColor_.y, clearColor_.z, clearColor_.w };
	cmdList_->ClearRenderTargetView(rtvH, clearcolor, 0, nullptr);
}

void DirectXCommon::ClearDepthBuffer() {
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	cmdList_->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

DirectXCommon::ComPtr<ID3D12Resource> DirectXCommon::CreateResourceBuffer(UINT64 size)	{
	HRESULT result = S_FALSE;
	ComPtr<ID3D12Resource> tmpBuffer;

	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
	// バッファの生成
	result = device_->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&tmpBuffer));
	assert(SUCCEEDED(result));

	return tmpBuffer;
}

void DirectXCommon::CreateFactory() {
	HRESULT result = S_FALSE;
	// DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));
}

void DirectXCommon::CreateDevice() {
	HRESULT result = S_FALSE;
	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;

	// パフォーマンスが高いものから順に、すべてのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(
			i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		adapters.push_back(tmpAdapter); // 配列に追加する
	}

	// 妥当なアダプタを選別する
	for (auto& it : adapters) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		it->GetDesc3(&adapterDesc);
		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			tmpAdapter = it;
			break;
		}
	}

	// 対応レベル配列
	D3D_FEATURE_LEVEL levels[] =
	{	D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0, };

	D3D_FEATURE_LEVEL featureLevel;
	for (auto& it : levels) {
		result = D3D12CreateDevice(tmpAdapter.Get(), it, IID_PPV_ARGS(&device_));
		if (result == S_OK) {
			featureLevel = it;
			break;
		}
	}
}

void DirectXCommon::CreateCommand() {
	HRESULT result = S_FALSE;
	// コマンドアロケータを生成
	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator_));
	assert(SUCCEEDED(result));

	// コマンドリストを生成
	result = device_->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator_.Get(), nullptr, IID_PPV_ARGS(&cmdList_));
	assert(SUCCEEDED(result));

	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	// コマンドキューを生成
	result = device_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue_));
	assert(SUCCEEDED(result));
}

void DirectXCommon::CreateSwapChain() {
	HRESULT result = S_FALSE;
	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

	swapChainDesc.Width = winApp_->GetWindowWidth();
	swapChainDesc.Height = winApp_->GetWindowHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapChainDesc.BufferCount = 2; // バッファ数を２つ設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1> swapChain1;

	result = dxgiFactory_->CreateSwapChainForHwnd(
		cmdQueue_.Get(), winApp_->GetHwnd(), &swapChainDesc,
		nullptr, nullptr, &swapChain1);
	assert(SUCCEEDED(result));

	// 生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	result = swapChain1.As(&swapChain_);
	assert(SUCCEEDED(result));
}

void DirectXCommon::CreateRenderTargetView() {
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	result = swapChain_->GetDesc(&swapChainDesc);
	assert(SUCCEEDED(result));

	// デスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビューなので RTV
	rtvHeapDesc.NodeMask = 0;
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // 表裏の２つ
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 特に指定なし

	result = device_->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap_));
	assert(SUCCEEDED(result));


	backBuffers_.resize(swapChainDesc.BufferCount);
	// スワップチェーンのすべてのバッファについて処理する
	for (size_t i = 0; i < backBuffers_.size(); i++) {
		// スワップチェーンからバッファを取得
		result = swapChain_->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers_[i]));
		assert(SUCCEEDED(result));
		// デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		// シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc, rtvHandle);
	}
}

void DirectXCommon::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;

	CD3DX12_HEAP_PROPERTIES depthHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_RESOURCE_DESC depthResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT, winApp_->GetWindowWidth(), winApp_->GetWindowHeight(), 1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	CD3DX12_CLEAR_VALUE depthClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = device_->CreateCommittedResource(
		&depthHeapProp, D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue, IID_PPV_ARGS(&depthBuff_));
	assert(SUCCEEDED(result));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthBuff_.Get(), &dsvDesc, dsvHeap_->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::CreateFence()
{
	HRESULT result = S_FALSE;
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(result));
}

#ifdef _DEBUG
void DirectXCommon::DebugLayer()
{
	ComPtr<ID3D12Debug1> _debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController)))) {
		_debugController->EnableDebugLayer();
		_debugController->SetEnableGPUBasedValidation(TRUE);
	}
}

void DirectXCommon::DebugSuppressError()
{
	ComPtr<ID3D12InfoQueue> _infoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&_infoQueue)))) {
		D3D12_MESSAGE_ID denyIds[] = {
			/*
			 * Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			 * https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			 */
			 D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		// 抑制する表示レベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter = {};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 指定したエラーの表示を抑制する
		_infoQueue->PushStorageFilter(&filter);
		// エラー時にブレークを発生させる
		_infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		_infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
	}
}

void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob)
{
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

#endif // _DEBUG
