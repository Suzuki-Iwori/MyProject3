#pragma once
#include "scene.h"

class Title :public Scene {
private:
	enum SceneSelect {
		Title_Start,
		Title_Setting,
		Title_Exist,
		Title_SceneNum
	};

	int sceneSelect; //タイトル→シーン切り替え

public:
	Title();

	//初期化
	void Initialize() override;
	//更新
	void Update(const char inputKeys[], const char preInputKeys[]) override;
	//描写
	void Draw() override;

	~Title(){}

};