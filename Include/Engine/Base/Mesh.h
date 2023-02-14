#pragma once
#ifndef MESH_H_
#define MESH_H_

#include <cassert>
#include <d3d12.h>
#include <d3dx12.h>
#include <vector>
#include <wrl.h>

#include "DirectXCommon.h"

/// <summary>
/// ���b�V���i���_�ƃC���f�b�N�X�j
/// </summary>
/// <typeparam name="VERTEX_TYPE">���_�̌^</typeparam>
template<class VERTEX_TYPE>
class Mesh
{
public:
	/// <summary>
	/// ���_��ǉ�
	/// </summary>
	/// <param name="vertex">���_</param>
	inline void AddVertex(const VERTEX_TYPE& vertex) { indcies_.emplace_back(vertex); }
	/// <summary>
	/// �C���f�b�N�X��ǉ�
	/// </summary>
	/// <param name="index">�C���f�b�N�X</param>
	inline void AddIndex(uint16_t index) { indcies_.emplace_back(index); }
	/// <summary>
	/// ���_�����擾
	/// </summary>
	/// <returns></returns>
	inline size_t GetVertexCount() const { return vertcies_.size(); }
	/// <summary>
	/// �C���f�b�N�X�����擾
	/// </summary>
	/// <returns></returns>
	inline size_t GetIndexCount() const { return indcies_.size(); }
	/// <summary>
	/// ���_�z����擾
	/// </summary>
	/// <returns></returns>
	inline const std::vector<VERTEX_TYPE>& GetVertcies() const { return vertcies_; }
	/// <summary>
	/// �C���f�b�N�X�z����擾
	/// </summary>
	/// <returns></returns>
	inline const std::vector<uint16_t>& GetIndcies() const { return indcies_; }
	/// <summary>
	/// �o�b�t�@�𐶐�
	/// </summary>
	void CreateBuffers();
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	
private:
	Mesh(const Mesh&) = delete;
	const Mesh& operator=(const Mesh&) = delete;

	void CreateVertexBuffer();
	void CreateIndexBuffer();

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};
	std::vector<VERTEX_TYPE> vertcies_;
	std::vector<uint16_t> indcies_;

};

#pragma region // �֐��̎���

template<class VERTEX_TYPE>
inline void Mesh<VERTEX_TYPE>::CreateBuffers()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
}

template<class VERTEX_TYPE>
inline void Mesh<VERTEX_TYPE>::Draw(ID3D12GraphicsCommandList* cmdList) {
	cmdList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	cmdList->IASetIndexBuffer(&indexBufferView_);
	cmdList->DrawIndexedInstanced(indcies_.size(), 1, 0, 0, 0);
}

template<class VERTEX_TYPE>
void Mesh<VERTEX_TYPE>::CreateVertexBuffer() {
	HRESULT result = S_FALSE;
	auto dixCom = DirectXCommon::GetInstance();

	UINT byteSize = static_cast<UINT>(sizeof(VERTEX_TYPE) * vertcies_.size());
	vertexBuffer_ = DirectXCommon::GetInstance()->CreateResourceBuffer(byteSize);

	// �r���[�𐶐�
	// GPU���z�A�h���X
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = byteSize;
	vertexBufferView_.StrideInBytes = sizeof(VERTEX_TYPE);

	// �}�b�s���O
	VERTEX_TYPE* map = nullptr;
	result = vertexBuffer_->Map(0, nullptr, (void**)&map);
	assert(SUCCEEDED(result));
	// �R�s�[
	std::copy(vertcies_.begin(), vertcies_.end(), map);
	vertexBuffer_->Unmap(0, nullptr);
}

template<class VERTEX_TYPE>
void Mesh<VERTEX_TYPE>::CreateIndexBuffer() {
	HRESULT result = S_FALSE;
	auto dixCom = DirectXCommon::GetInstance();

	UINT byteSize = static_cast<UINT>(sizeof(uint16_t) * indcies_.size());
	indexBuffer_ = DirectXCommon::GetInstance()->CreateResourceBuffer(byteSize);

	// �r���[�𐶐�
	// GPU���z�A�h���X
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.Format = DXGI_FORMAT_R16_UINT;
	indexBufferView_.SizeInBytes = byteSize;

	// �}�b�s���O
	uint16_t* map = nullptr;
	result = indexBuffer_->Map(0, nullptr, (void**)&map); // �}�b�s���O
	assert(SUCCEEDED(result));
	// �R�s�[
	std::copy(indcies_.begin(), indcies_.end(), map);
	indexBuffer_->Unmap(0, nullptr);
}

#pragma endregion

#endif
