#pragma once
#include "scene.h"

class SceneManager :public Scene {
private:
	int OperationScene; //稼働シーン

	Scene* HaveScene[Scene_Num]; //シーン

public:
	SceneManager();

	//初期化
	void Initialize() override;
	//更新
	void Update(const char inputKeys[], const char preInputKeys[]) override;
	//描写
	void Draw() override;

	~SceneManager() override;

};