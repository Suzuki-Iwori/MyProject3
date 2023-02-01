#pragma once
#include "scene.h"

class SceneManager :public Scene {
private:
	int OperationScene; //�ғ��V�[��

	Scene* HaveScene[Scene_Num]; //�V�[��

public:
	SceneManager();

	//������
	void Initialize() override;
	//�X�V
	void Update(const char inputKeys[], const char preInputKeys[]) override;
	//�`��
	void Draw() override;

	~SceneManager() override;

};