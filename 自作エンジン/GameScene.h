#pragma once
#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include <memory>

#include "Scene.h"

class GameScene :
    public BaseScene
{
public:
	GameScene();
	~GameScene();
	void Initalize();
	void Update();
	void Draw();

private:
	std::unique_ptr<class Sprite> sprite_;
};

#endif
