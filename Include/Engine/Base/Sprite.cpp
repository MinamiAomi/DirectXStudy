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
	// パイプラインをセット
	sCmdList_->SetPipelineState(sPipelineState_[blendMode].Get());
	// シグネチャをセット
	sCmdList_->SetGraphicsRootSignature(sRootSignature_.Get());
	// 三角形リストにセット
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

	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"../Resources/Shaders/SpriteVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &vsBlob, &errorBlob);

#ifdef _DEBUG
	ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG

	result = D3DCompileFromFile(
		L"../Resources/Shaders/SpritePS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
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


	// ルートシグネチャ

	// ディスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange =
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootParams[kRootParameterCount] = {};
	rootParams[kConstData].InitAsConstantBufferView(0);
	rootParams[kTexture].InitAsDescriptorTable(1, &descriptorRange);

	// テクスチャサンプラーの設定
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

	// ルートシグネチャ設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams; // ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);		// ルートパラメータの数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = diXCom->GetDevice()->CreateRootSignature(0,
		rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(result));


	for (int i = 0; i < kBlendModeCount; i++) {

		// グラフィックスパイプライン設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};
		// シェーダの設定
		pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
		pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

		// サンプルマスク設定
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		// ラスタライザの設定
		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
		//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ワイヤーフレーム

		// ブレンドステート
		pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBAすべてのチャンネルを描画
		pipelineDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;			// 加算
		pipelineDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;			// ソースの値を 100% 使う
		pipelineDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;			// デストの値を   0% 使う

		switch (static_cast<BlendMode>(i))
		{
		case kBlendModeNone: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = false;						// ブレンドしない
			break;
			}
		case kBlendModeNormal: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;						// ブレンドを有効にする
			pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-ソースのアルファ値
			break;
			}
		case kBlendModeAdd: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;						// ブレンドを有効にする
			pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;				// 加算
			pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;				// ソースの値を 100% 使う
			pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;				// デストの値を 100% 使う
			break;
			}
		case kBlendModeSubtract: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;						// ブレンドを有効にする
			pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	// デストからソースを減算
			pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;				// ソースの値を 100% 使う
			pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;				// デストの値を 100% 使う
			break;
			}
		case kBlendModeMultiply: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;						// ブレンドを有効にする
			pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;				// 加算
			pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;				// 使わない
			pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;		// デストの値 × ソースの値
			break;
			}
		case kBlendModeInversion: {
			pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;						// ブレンドを有効にする
			pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;				// 加算
			pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	// 1.0f-デストカラーの値
			pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;				// 使わない
			break;
			}
		default:
			assert(false);
		}

		// 頂点レイアウトの設定
		pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
		pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

		// 図形の形状設定
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		pipelineDesc.NumRenderTargets = 1; // 描画対象は１つ
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0 ~ 255指定のRGBA
		pipelineDesc.SampleDesc.Count = 1; // １ピクセルに着き１回サンプリング

		//pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// パイプラインにルートシグネチャをセット
		pipelineDesc.pRootSignature = sRootSignature_.Get();

		// パイプラインステートの生成
		result = diXCom->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&sPipelineState_[i]));
		assert(SUCCEEDED(result));
	}
}

void Sprite::CreateBuffers() {
	// 静的初期化関数が呼ばれていない
	assert(sDiXCom_ != nullptr);
	assert(sTexMana_ != nullptr);
	
	constexpr UINT kConstBufferByteSize = (sizeof(ConstDataMatrixColor) + 0xFF) & ~0xFF;
	constexpr UINT kVertexBufferByteSize = static_cast<UINT>(sizeof(VertexPosUv) * kVertexCount);
	
	// 頂点バッファの作成
	vertexBuffer_ = sDiXCom_->CreateResourceBuffer(kVertexBufferByteSize);
	// 定数バッファの作成
	constBuffer_ = sDiXCom_->CreateResourceBuffer(kConstBufferByteSize);

	// ビューを生成
	// GPU仮想アドレス
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

	vertices[0].position = Vector3(left, bottom, 0.0f);		// 左下
	vertices[1].position = Vector3(left, top, 0.0f);		// 左上
	vertices[2].position = Vector3(right, bottom, 0.0f);	// 右下
	vertices[3].position = Vector3(right, top, 0.0f);		// 右上

	auto resDesc = sTexMana_->GetResourceDesc(textureHandle_);

	float uvLeft = textureBase_.x / resDesc.Width;
	float uvRight = (textureBase_.x + textureSize_.x) / resDesc.Width;
	float uvTop = textureBase_.y / resDesc.Height;
	float uvBottom = (textureBase_.y + textureSize_.y) / resDesc.Height;

	vertices[0].uv = Vector2(uvLeft, uvBottom);		// 左下
	vertices[1].uv = Vector2(uvLeft, uvTop);		// 左上
	vertices[2].uv = Vector2(uvRight, uvBottom);	// 右下
	vertices[3].uv = Vector2(uvRight, uvTop);		// 右上

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

