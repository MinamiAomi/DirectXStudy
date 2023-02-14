#pragma once
#ifndef WORLDTRANSFORM_H_
#define WORLDTRANSFORM_H_

#include <d3d12.h>
#include <wrl.h>

#include "MathUtility.h"

class WorldTransform
{
private:
	struct ConstDataWorldTransform {
		Matrix44 world;
	};

public:
	/// <summary>
	/// �o�b�t�@�𐶐�
	/// </summary>
	void CreateBuffer();
	/// <summary>
	/// �s����X�V
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// �s���]��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="rootParameterIndexWorldTransform">���[�g�p�����[�^�̃C���f�b�N�X</param>
	void SetGraphicsCommand(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

public:
	// ���W
	Vector3 position;
	// ��]
	Quaternion rotate;
	// �g�k
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
	// ���[���h�s��
	Matrix44 worldMatrix;
	// �e�ւ̃|�C���^
	const WorldTransform* parent = nullptr;

private:
	//�@�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	// �}�b�v�p
	ConstDataWorldTransform* map_ = nullptr;
};

#endif
