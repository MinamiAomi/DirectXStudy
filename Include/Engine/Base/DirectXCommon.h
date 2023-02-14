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

public: // �ÓI�����o�֐�
	/// <summary>
	/// �V���O���g���C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X�̃|�C���^</returns>
	static DirectXCommon* GetInstance();

public: // �����o�֐�
	/// <summary>
	/// �f�o�C�X���擾
	/// </summary>
	/// <returns></returns>
	inline ID3D12Device* GetDevice() { return device_.Get(); }
	/// <summary>
	/// �R�}���h���X�g���擾
	/// </summary>
	/// <returns></returns>
	inline ID3D12GraphicsCommandList* GetCommandList() { return cmdList_.Get(); }
	/// <summary>
	/// �X���b�v�`�F�[�����擾
	/// </summary>
	/// <returns></returns>
	inline IDXGISwapChain4* GetSwapChain() { return swapChain_.Get(); }
	/// <summary>
	/// �o�b�N�o�b�t�@���擾
	/// </summary>
	/// <param name="i">�ԍ�</param>
	/// <returns></returns>
	inline ID3D12Resource* GetBackBaffer(size_t i) { return backBuffers_[i].Get(); }

	/// <summary>
	/// �N���A�J���[���Z�b�g
	/// </summary>
	/// <param name="color"></param>
	inline void SetClearColor(const Vector4& color) { clearColor_ = color; }
	/// <summary>
	/// �r���[�|�[�g���Z�b�g
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void SetViewport(float left, float top, float width, float height);
	/// <summary>
	/// �V�U�[��`���Z�b�g
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	void SetScissorRect(int left, int top, int right, int bottom);

	/// <summary>
	/// ������
	/// </summary>
	void Initalize();
	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDraw();
	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDraw();
	/// <summary>
	/// �����_�[�^�[�Q�b�g���N���A
	/// </summary>
	void ClearRenderTarget();
	/// <summary>
	/// �[�x�o�b�t�@���N���A
	/// </summary>
	void ClearDepthBuffer();

	/// <summary>
	/// ���\�[�X�o�b�t�@�̐���
	/// </summary>
	/// <param name="size"></param>
	/// <returns></returns>
	ComPtr<ID3D12Resource> CreateResourceBuffer(UINT64 size);

private: // �����o�֐�
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

private: // �����o�ϐ�
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
