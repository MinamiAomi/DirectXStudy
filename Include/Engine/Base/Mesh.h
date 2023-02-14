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
/// メッシュ（頂点とインデックス）
/// </summary>
/// <typeparam name="VERTEX_TYPE">頂点の型</typeparam>
template<class VERTEX_TYPE>
class Mesh
{
public:
	/// <summary>
	/// 頂点を追加
	/// </summary>
	/// <param name="vertex">頂点</param>
	inline void AddVertex(const VERTEX_TYPE& vertex) { indcies_.emplace_back(vertex); }
	/// <summary>
	/// インデックスを追加
	/// </summary>
	/// <param name="index">インデックス</param>
	inline void AddIndex(uint16_t index) { indcies_.emplace_back(index); }
	/// <summary>
	/// 頂点数を取得
	/// </summary>
	/// <returns></returns>
	inline size_t GetVertexCount() const { return vertcies_.size(); }
	/// <summary>
	/// インデックス数を取得
	/// </summary>
	/// <returns></returns>
	inline size_t GetIndexCount() const { return indcies_.size(); }
	/// <summary>
	/// 頂点配列を取得
	/// </summary>
	/// <returns></returns>
	inline const std::vector<VERTEX_TYPE>& GetVertcies() const { return vertcies_; }
	/// <summary>
	/// インデックス配列を取得
	/// </summary>
	/// <returns></returns>
	inline const std::vector<uint16_t>& GetIndcies() const { return indcies_; }
	/// <summary>
	/// バッファを生成
	/// </summary>
	void CreateBuffers();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
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

#pragma region // 関数の実装

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

	// ビューを生成
	// GPU仮想アドレス
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = byteSize;
	vertexBufferView_.StrideInBytes = sizeof(VERTEX_TYPE);

	// マッピング
	VERTEX_TYPE* map = nullptr;
	result = vertexBuffer_->Map(0, nullptr, (void**)&map);
	assert(SUCCEEDED(result));
	// コピー
	std::copy(vertcies_.begin(), vertcies_.end(), map);
	vertexBuffer_->Unmap(0, nullptr);
}

template<class VERTEX_TYPE>
void Mesh<VERTEX_TYPE>::CreateIndexBuffer() {
	HRESULT result = S_FALSE;
	auto dixCom = DirectXCommon::GetInstance();

	UINT byteSize = static_cast<UINT>(sizeof(uint16_t) * indcies_.size());
	indexBuffer_ = DirectXCommon::GetInstance()->CreateResourceBuffer(byteSize);

	// ビューを生成
	// GPU仮想アドレス
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.Format = DXGI_FORMAT_R16_UINT;
	indexBufferView_.SizeInBytes = byteSize;

	// マッピング
	uint16_t* map = nullptr;
	result = indexBuffer_->Map(0, nullptr, (void**)&map); // マッピング
	assert(SUCCEEDED(result));
	// コピー
	std::copy(indcies_.begin(), indcies_.end(), map);
	indexBuffer_->Unmap(0, nullptr);
}

#pragma endregion

#endif
