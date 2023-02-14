#include "CameraTransform.h"

#include <cassert>
#include <d3dx12.h>

#include "DirectXCommon.h"

void Camera3D::CreateBuffer()
{
	HRESULT result = S_FALSE;
	auto device = DirectXCommon::GetInstance()->GetDevice();

	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstDataCameraTransform) + 0xFF) & ~0xFF);
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

void Camera3D::UpdateMatrix()
{
	viewMatrix_ = Matrix44::CreateView(eye_, target_, up_);
	projMatrix_ = Matrix44::CreatePerspectiveProjection(fovAngleY_, aspectRatio_, nearZ_, farZ_);
}

void Camera3D::SetGraphicsCommand(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	map_->view = viewMatrix_;
	map_->proj = projMatrix_;
	map_->eye = eye_;
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuffer_->GetGPUVirtualAddress());
}

void Camera2D::UpdateMatrix()
{
	Matrix33 camera = Matrix33::Identity;
	camera *= Matrix33::CreateScaling(zoom_);
	camera *= Matrix33::CreateTranslation(position_);

	viewMatrix_ = Matrix44::CreateFromMatrix33(camera.Inverse());
	projMatrix_ = Matrix44::CreateOrthographicProjection(left_, top_, right_, bottom_, nearZ_, farZ_);
	viewProjMatrix_ = viewMatrix_ * projMatrix_;
}
