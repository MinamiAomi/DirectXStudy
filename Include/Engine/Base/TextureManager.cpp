#include "TextureManager.h"

#include <cassert>
#include <DirectXTex.h>
#include <d3dx12.h>

using namespace DirectX;

TextureManager* TextureManager::GetInstance() {
    static TextureManager instance;
    return &instance;
}

UINT TextureManager::LoadTexture(const std::string& filePath) {
    return GetInstance()->InternalLoadTexture(filePath);
}

D3D12_RESOURCE_DESC TextureManager::GetResourceDesc(UINT texHandle) {
    assert(nextLoadIndex_ < kDescriptorCount);
    return textures_[texHandle].buffer->GetDesc();
}

void TextureManager::Initalize() {
    directXCommon_ = DirectXCommon::GetInstance();

    ResetAll();

    descriptorIncrementSize_ = directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TextureManager::ResetAll() {
	HRESULT result = S_FALSE;
	// デスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kDescriptorCount;

	result = directXCommon_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	for (auto& it : textures_) {
		it.buffer.Reset();
		it.cpuHandle.ptr = 0;
		it.gpuHandle.ptr = 0;
		it.name.erase();
	}
	nextLoadIndex_ = 0;
}

void TextureManager::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* cmdlist, UINT rootParamIndex, UINT texNumber) {
	assert(texNumber < kDescriptorCount);
	assert(textures_[texNumber].gpuHandle.ptr != 0); // テクスチャがロードされていない

	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	cmdlist->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdlist->SetGraphicsRootDescriptorTable(rootParamIndex, textures_[texNumber].gpuHandle);
}

UINT TextureManager::InternalLoadTexture(const std::string& filePath) {
	assert(nextLoadIndex_ < kDescriptorCount);

	auto it = std::find_if(
		textures_.begin(), textures_.end(), 
		[&](const auto& tex) { return tex.name == filePath; });

	if (it != textures_.end()) {
		return static_cast<UINT>(std::distance(textures_.begin(), it));
	}

	wchar_t wpath[256] = {};
	MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wpath, _countof(wpath));

	HRESULT result = S_FALSE;

	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	result = LoadFromWICFile(wpath, WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	ScratchImage mipChain = {};

	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	metadata.format = MakeSRGB(metadata.format);


	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC texResourceDesc = {};
	texResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texResourceDesc.Format = metadata.format;
	texResourceDesc.Width = metadata.width;
	texResourceDesc.Height = (UINT)metadata.height;
	texResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	texResourceDesc.SampleDesc.Count = 1;


	result = directXCommon_->GetDevice()->CreateCommittedResource(
		&texHeapProp, D3D12_HEAP_FLAG_NONE, &texResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&textures_[nextLoadIndex_].buffer));
	assert(SUCCEEDED(result));

	// 全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = textures_[nextLoadIndex_].buffer->WriteToSubresource(
			(UINT)i, nullptr, img->pixels,
			(UINT)img->rowPitch, (UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}

	textures_[nextLoadIndex_].cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		srvHeap_->GetCPUDescriptorHandleForHeapStart(), nextLoadIndex_, descriptorIncrementSize_);
	textures_[nextLoadIndex_].gpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		srvHeap_->GetGPUDescriptorHandleForHeapStart(), nextLoadIndex_, descriptorIncrementSize_);


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texResourceDesc.MipLevels;

	directXCommon_->GetDevice()->CreateShaderResourceView(textures_[nextLoadIndex_].buffer.Get(), &srvDesc, textures_[nextLoadIndex_].cpuHandle);

	textures_[nextLoadIndex_].name = filePath;

	return nextLoadIndex_++;
}
