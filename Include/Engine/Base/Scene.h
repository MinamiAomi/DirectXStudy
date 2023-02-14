#pragma once
#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include <cassert>
#include <memory>

#include "SceneSharedData.h"

// �V�[���N���X�̐e
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

// �V�[���}�l�[�W���[
class SceneManager
{
public:
	/// <summary>
	/// �V���O���g���C���X�^���X���擾
	/// </summary>
	/// <returns></returns>
	static SceneManager* GetInstance();

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initalize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �V�[���`�F���W(���̃t���[����Update�O�ɕς��)
	/// </summary>
	/// <typeparam name="NEXT_SCENE">���̃V�[��(BaseScene���p��)</typeparam>
	template<class NEXT_SCENE>
	void Transition();

	/// <summary>
	/// ���ʃf�[�^���擾
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<SceneSharedData> GetSharedData() { return sharedData_; }

private:
	// �V�[�����L�f�[�^
	std::shared_ptr<SceneSharedData> sharedData_;
	// ���݂̃V�[��
	std::unique_ptr<BaseScene> currentScene_;
	// ���̃V�[��
	std::unique_ptr<BaseScene> nextScene_;
};

template<class NEXT_SCENE>
inline void SceneManager::Transition()
{
	// NEXT_SCENE��BaseScene���p�����Ă���
	bool isInheritance = std::is_base_of<BaseScene, NEXT_SCENE>::value;
	assert(isInheritance);
	// nextScene�ɃC���X�^���X���Ȃ�
	assert(!nextScene_);
	nextScene_ = std::make_unique<NEXT_SCENE>();
}

#endif
