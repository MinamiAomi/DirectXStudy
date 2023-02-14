#include "GameScene.h"

#include "DirectXCommon.h"
#include "Sprite.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initalize()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->CreateBuffers();
}

void GameScene::Update()
{
}

void GameScene::Draw()
{
	auto cmdList = DirectXCommon::GetInstance()->GetCommandList();
	Sprite::PreDraw(cmdList);
	Sprite::SetPipeline(Sprite::kBlendModeNormal);

	sprite_->Draw();

	Sprite::PostDraw();
}
