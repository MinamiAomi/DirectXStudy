#include "WinApp.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include "Scene.h"
#include "GameScene.h"
#include "Sprite.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {

	auto winApp = WinApp::GetInstance();
	winApp->CreateGameWindow();
	auto dixCom = DirectXCommon::GetInstance();
	dixCom->Initalize();
	auto texMana = TextureManager::GetInstance();
	texMana->Initalize();
	TextureManager::LoadTexture("../Resources/Images/white1x1.png");
	auto input = Input::GetInstance();
	input->Initialize();
	Sprite::StaticInitalize();

	auto scene = SceneManager::GetInstance();
	scene->Initalize();
	// Å‰‚ÌƒV[ƒ“‚ðGameScene‚É
	scene->Transition<GameScene>();

	while (winApp->WindowQuit() == false)
	{
		dixCom->PreDraw();
		input->Update();
		
		scene->Update();
		
		scene->Draw();
		
		dixCom->PostDraw();
	}

	winApp->TerminateGameWindow();

	return 0;
}