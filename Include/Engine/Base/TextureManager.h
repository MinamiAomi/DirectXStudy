#pragma once
#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <array>
#include <string>
#include <wrl.h>

#include "DirectXCommon.h"

class TextureManager
{
private: // �^
	template<class TYPE>
	using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

	struct Texture {
		ComPtr<ID3D12Resource> buffer; // �o�b�t�@
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = {}; // CPU�n���h��
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = {}; // GPU�n���h��
		std::string name; // �t�@�C����
	};

private: // �萔
	static constexpr size_t kDescriptorCount = 50;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �V���O���g���C���X�^���X���擾
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();
	/// <summary>
	/// �e�N�X�`�������[�h
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns>�e�N�X�`���n���h��</returns>
	static UINT LoadTexture(const std::string& filePath);

public: // �����o�֐�
	/// <summary>
	/// ���\�[�X�f�X�N���擾����
	/// </summary>
	/// <param name="texHanfle">�e�N�X�`���n���h��</param>
	/// <returns></returns>
	D3D12_RESOURCE_DESC GetResourceDesc(UINT texHandle);
	/// <summary>
	/// ������
	/// </summary>
	void Initalize();
	/// <summary>
	/// ���[�h�����e�N�X�`�������
	/// </summary>
	void ResetAll();
	/// <summary>
	/// �R�}���h���X�g�ɃZ�b�g����
	/// </summary>
	/// <param name="cmdlist"></param>
	/// <param name="rootParamIndex"></param>
	/// <param name="texNumber"></param>
	void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* cmdlist, UINT rootParamIndex, UINT texNumber);

private: // �����o�֐�
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	const TextureManager& operator=(const TextureManager&) = delete;
	/// <summary>
	/// �e�N�X�`�������[�h(����)
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	UINT InternalLoadTexture(const std::string& filePath);

private: // �����o�ϐ�
	DirectXCommon* directXCommon_ = nullptr;

	ComPtr<ID3D12DescriptorHeap> srvHeap_;
	UINT descriptorIncrementSize_ = 0;
	std::array<Texture, kDescriptorCount> textures_;

	UINT nextLoadIndex_ = 0;

};

#endif
