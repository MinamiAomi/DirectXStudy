#pragma once
#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include <cassert>
#include <memory>

#include "SceneSharedData.h"

// シーンクラスの親
class BaseScene 
{
public:
	BaseScene();
	virtual ~BaseScene();

	virtual void Initalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	std::shared_ptr<SceneSharedData> sharedData_;

};

// シーンマネージャー
class SceneManager
{
public:
	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns></returns>
	static SceneManager* GetInstance();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// シーンチェンジ(次のフレームのUpdate前に変わる)
	/// </summary>
	/// <typeparam name="NEXT_SCENE">次のシーン(BaseSceneを継承)</typeparam>
	template<class NEXT_SCENE>
	void Transition();

	/// <summary>
	/// 共通データを取得
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<SceneSharedData> GetSharedData() { return sharedData_; }

private:
	// シーン共有データ
	std::shared_ptr<SceneSharedData> sharedData_;
	// 現在のシーン
	std::unique_ptr<BaseScene> currentScene_;
	// 次のシーン
	std::unique_ptr<BaseScene> nextScene_;
};

template<class NEXT_SCENE>
inline void SceneManager::Transition()
{
	// NEXT_SCENEがBaseSceneを継承している
	bool isInheritance = std::is_base_of<BaseScene, NEXT_SCENE>::value;
	assert(isInheritance);
	// nextSceneにインスタンスがない
	assert(!nextScene_);
	nextScene_ = std::make_unique<NEXT_SCENE>();
}

#endif
