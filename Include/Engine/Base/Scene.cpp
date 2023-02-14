#include "Scene.h"

BaseScene::BaseScene()
{
    sharedData_ = SceneManager::GetInstance()->GetSharedData();
}

BaseScene::~BaseScene()
{
}

SceneManager* SceneManager::GetInstance()
{
    static SceneManager instance;
    return &instance;
}

void SceneManager::Initalize()
{
    sharedData_ = std::make_shared<SceneSharedData>();
}

void SceneManager::Update()
{
    // ���̃V�[��������ꍇ
    if (nextScene_) {
        currentScene_ = std::move(nextScene_);
        currentScene_->Initalize();
    }
    currentScene_->Update();
}

void SceneManager::Draw()
{
    currentScene_->Draw();
}
