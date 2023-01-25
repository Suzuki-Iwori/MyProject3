#pragma once
#include "scene.h"

class Title :public Scene {
private:
	enum SceneSelect {
		Title_Start,
		Title_Continue,
		Title_Setting,
		Title_Exist,
		Title_SceneNum
	};

	int sceneSelect;

public:
	Title();

	//������
	void Initialize() override;
	//�X�V
	void Update(const char inputKeys[], const char preInputKeys[]) override;
	//�`��
	void Draw() override;

	~Title(){}

};