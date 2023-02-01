#pragma once
#include "scene.h"

class StageSelect :public Scene {
private:
	enum Select {
		Select_Stage1,
		Select_Stage2,
		Select_Stage3,
		Select_Exist,
		Select_Num
	};

	int stageSelectNumber; //タイトル→シーン切り替え

public:
	StageSelect();

	//初期化
	void Initialize() override;
	//更新
	void Update(const char inputKeys[], const char preInputKeys[]) override;
	//描写
	void Draw() override;

	~StageSelect() {}

};