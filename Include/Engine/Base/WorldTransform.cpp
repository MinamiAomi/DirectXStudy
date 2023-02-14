#include "WorldTransform.h"

#include <cassert>
#include <d3dx12.h>

#include "DirectXCommon.h"


void WorldTransform::CreateBuffer()
{
	HRESULT result = S_FALSE;
	auto device = DirectXCommon::GetInstance()->GetDevice();

	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstDataWorldTransform) + 0xFF) & ~0xFF);
	// バッファの生成
	result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&constBuffer_));
	assert(SUCCEEDED(result));

	// マッピング
	result = constBuffer_->Map(0, nullptr, (void**)&map_); // マッピング
	assert(SUCCEEDED(result));
}

void WorldTransform::UpdateMatrix()
{
	worldMatrix = Matrix44::Identity;
	worldMatrix *= Matrix44::CreateScaling(scale);
	worldMatrix *= Matrix44::CreateRotationFromQuaternion(rotate);
	worldMatrix *= Matrix44::CreateTranslation(position);

	if (parent != nullptr) {
		worldMatrix *= parent->worldMatrix;
	}
}

void WorldTransform::SetGraphicsCommand(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	map_->world = worldMatrix;
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuffer_->GetGPUVirtualAddress());
}
