#include "Sprite.h"

#include <cassert>
#include <d3dcompiler.h>
#include <d3dx12.h>

#include "CameraTransform.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "WinApp.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;

DirectXCommon* Sprite::sDiXCom_ = nullptr;
TextureManager* Sprite::sTexMana_ = nullptr;
ID3D12GraphicsCommandList* Sprite::sCmdList_ = nullptr;
ComPtr<ID3D12RootSignature> Sprite::sRootSignature_;
std::array<ComPtr<ID3D12PipelineState>, Sprite::kBlendModeCount> Sprite::sPipelineState_; 
Matrix44 Sprite::sDefultProjMatrix_;

void Sprite::StaticInitalize()
{
	assert(sDiXCom_ == nullptr);
	assert(sTexMana_ == nullptr);

	sDiXCom_ = DirectXCommon::GetInstance();
	sTexMana_ = TextureManager::GetInstance();
	auto winApp = WinApp::GetInstance();
	sDefultProjMatrix_ = Matrix44::CreateOrthographicProjection(0, 0, 
		static_cast<float>(winApp->GetWindowWidth()),
		static_cast<float>(winApp->GetWindowHeight()));

	CreatePipelineSet();
}

void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	assert(sCmdList_ == nullptr);
	sCmdList_ = cmdList;
}

void Sprite::SetPipeline(BlendMode blendMode)
{
	assert(sCmdList_ != nullptr);
	// �p�C�v���C�����Z�b�g
	sCmdList_->SetPipelineState(sPipelineState_[blendMode].Get());
	// �V�O�l�`�����Z�b�g
	sCmdList_->SetGraphicsRootSignature(sRootSignature_.Get());
	// �O�p�`���X�g�ɃZ�b�g
	sCmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::PostDraw()
{
	sCmdList_ = nullptr;
}

void Sprite::CreatePipelineSet()
{
	auto diXCom = DirectXCommon::GetInstance();

	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"../Resources/Shaders/SpriteVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0, &vsBlob, &errorBlob);

#ifdef _DEBUG
	ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG

	result = D3DCompileFromFile(
		L"../Resources/Shaders/SpritePS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0, &psBlob, &errorBlob);

#ifdef _DEBUG
	ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG


	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};


	// ���[�g�V�O�l�`��

	// �f�B�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange =
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootParams[kRootParameterCount] = {};
	rootParams[kConstData].InitAsConstantBufferView(0);
	rootParams[kTexture].InitAsDescriptorTable(1, &descriptorRange);

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// ���[�g�V�O�l�`���ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams; // ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);		// ���[�g�p�����[�^�̐�
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = diXCom->GetDevice()->CreateRootSignature(0,
		rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(result));


	for (int i = 0; i < kBlendModeCount; i++) {

		// �O���t�B�b�N�X�p�C�v���C���ݒ�
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};
		// �V�F�[�_�̐ݒ�
		pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
		pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

		// �T���v���}�X�N�ݒ�
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		// ���X�^���C�U�̐ݒ�
		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
		//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ���C���[�t���[��

		// �u�����h�X�e�[�g
		pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBA���ׂẴ`�����l����`��
		pipelineDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;			// ���Z
		pipelineDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;			// �\�[�X�̒l�� 100% �g��
		pipelineDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;			// �f�X�g�̒l��   0% �g��

		switch (static_cast<BlendMode>(i))
		{
		case kBlendModeNone: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = false;						// �u�����h���Ȃ�
			break;
			}
		case kBlendModeNormal: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;						// �u�����h��L���ɂ���
			pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-�\�[�X�̃A���t�@�l
			break;
			}
		case kBlendModeAdd: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;						// �u�����h��L���ɂ���
			pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;				// ���Z
			pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;				// �\�[�X�̒l�� 100% �g��
			pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;				// �f�X�g�̒l�� 100% �g��
			break;
			}
		case kBlendModeSubtract: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;						// �u�����h��L���ɂ���
			pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	// �f�X�g����\�[�X�����Z
			pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;				// �\�[�X�̒l�� 100% �g��
			pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;				// �f�X�g�̒l�� 100% �g��
			break;
			}
		case kBlendModeMultiply: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;						// �u�����h��L���ɂ���
			pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;				// ���Z
			pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;				// �g��Ȃ�
			pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;		// �f�X�g�̒l �~ �\�[�X�̒l
			break;
			}
		case kBlendModeInversion: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;						// �u�����h��L���ɂ���
			pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;				// ���Z
			pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	// 1.0f-�f�X�g�J���[�̒l
			pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;				// �g��Ȃ�
			break;
			}
		default:
			assert(false);
		}

		// ���_���C�A�E�g�̐ݒ�
		pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
		pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

		// �}�`�̌`��ݒ�
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ͂P��
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0 ~ 255�w���RGBA
		pipelineDesc.SampleDesc.Count = 1; // �P�s�N�Z���ɒ����P��T���v�����O

		//pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
		pipelineDesc.pRootSignature = sRootSignature_.Get();

		// �p�C�v���C���X�e�[�g�̐���
		result = diXCom->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&sPipelineState_[i]));
		assert(SUCCEEDED(result));
	}
}

void Sprite::CreateBuffers() {
	// �ÓI�������֐����Ă΂�Ă��Ȃ�
	assert(sDiXCom_ != nullptr);
	assert(sTexMana_ != nullptr);
	
	constexpr UINT kConstBufferByteSize = (sizeof(ConstDataMatrixColor) + 0xFF) & ~0xFF;
	constexpr UINT kVertexBufferByteSize = static_cast<UINT>(sizeof(VertexPosUv) * kVertexCount);
	
	// ���_�o�b�t�@�̍쐬
	vertexBuffer_ = sDiXCom_->CreateResourceBuffer(kVertexBufferByteSize);
	// �萔�o�b�t�@�̍쐬
	constBuffer_ = sDiXCom_->CreateResourceBuffer(kConstBufferByteSize);

	// �r���[�𐶐�
	// GPU���z�A�h���X
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = kVertexBufferByteSize;
	vertexBufferView_.StrideInBytes = sizeof(VertexPosUv);

	UpdateVertexBuffer();
	UpdateConstBuffer(sDefultProjMatrix_);
}

void Sprite::Draw() 
{
	if (isNeedVertexUpdate_) {
		UpdateVertexBuffer();
	}
	UpdateConstBuffer(sDefultProjMatrix_);
	sCmdList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
	sCmdList_->SetGraphicsRootConstantBufferView(kConstData, constBuffer_->GetGPUVirtualAddress());
	sTexMana_->SetGraphicsRootDescriptorTable(sCmdList_, kTexture,textureHandle_);
	sCmdList_->DrawInstanced(kVertexCount, 1, 0, 0);
}

void Sprite::Draw(const Camera2D& camera)
{
	if (isNeedVertexUpdate_) {
		UpdateVertexBuffer();
	}
	UpdateConstBuffer(camera.GetViewProjMatrix());
	sCmdList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
	sCmdList_->SetGraphicsRootConstantBufferView(kConstData, constBuffer_->GetGPUVirtualAddress());
	sTexMana_->SetGraphicsRootDescriptorTable(sCmdList_, kTexture,textureHandle_);
	sCmdList_->DrawInstanced(kVertexCount, 1, 0, 0);
}

void Sprite::UpdateVertexBuffer()
{
	HRESULT result = S_FALSE;

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	VertexPosUv vertices[kVertexCount] = {};

	vertices[0].position = Vector3(left, bottom, 0.0f);		// ����
	vertices[1].position = Vector3(left, top, 0.0f);		// ����
	vertices[2].position = Vector3(right, bottom, 0.0f);	// �E��
	vertices[3].position = Vector3(right, top, 0.0f);		// �E��

	auto resDesc = sTexMana_->GetResourceDesc(textureHandle_);

	float uvLeft = textureBase_.x / resDesc.Width;
	float uvRight = (textureBase_.x + textureSize_.x) / resDesc.Width;
	float uvTop = textureBase_.y / resDesc.Height;
	float uvBottom = (textureBase_.y + textureSize_.y) / resDesc.Height;

	vertices[0].uv = Vector2(uvLeft, uvBottom);		// ����
	vertices[1].uv = Vector2(uvLeft, uvTop);		// ����
	vertices[2].uv = Vector2(uvRight, uvBottom);	// �E��
	vertices[3].uv = Vector2(uvRight, uvTop);		// �E��

	VertexPosUv* map = nullptr;
	result = vertexBuffer_->Map(0, nullptr, (void**)&map);
	assert(SUCCEEDED(result));
	std::copy(std::begin(vertices), std::end(vertices), map);
	vertexBuffer_->Unmap(0, nullptr);
}

void Sprite::UpdateConstBuffer(const Matrix44& mat)
{
	HRESULT result = S_FALSE;

	worldMatrix_ = Matrix44::Identity;
	worldMatrix_ *= Matrix44::CreateRotationZ(rotate_);
	worldMatrix_ *= Matrix44::CreateTranslation(Vector3(position_, 0.0f));
	
	ConstDataMatrixColor* map = nullptr;
	result = constBuffer_->Map(0, nullptr, (void**)&map);
	assert(SUCCEEDED(result));
	map->matrix = worldMatrix_ * mat;
	map->color = color_;
	constBuffer_->Unmap(0, nullptr);
}

