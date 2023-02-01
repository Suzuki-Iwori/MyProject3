#pragma once

enum SceneSection {
	Scene_Title,
	Scene_StageSelect,
	Scene_Stage1,
	Scene_Stage2,
	Scene_Stage3,
	Scene_Num
};

class Scene {
protected:
	int sceneInCount_; //シーン入力時カウント(演出用)
	int sceneOutCount_; //シーン退出時カウント(演出用)

public:
	static bool sceneTransition[Scene_Num]; //シーン切り替えフラグ(静的)

	Scene() {
		sceneInCount_ = 0;
		sceneOutCount_ = 0;
		for (int i = 0; i < Scene_Num; i++) {
			sceneTransition[i] = false;
		}
	}

	virtual void Initialize() = 0;
	virtual void Update(const char inputKeys[], const char preInputKeys[]) = 0;
	virtual void Draw() = 0;

	//シーン切り替えフラグの初期化
	void SceneFragReset() {
		for (int i = 0; i < Scene_Num; i++) {
			sceneTransition[i] = false;
		}
	}

	virtual ~Scene(){}

};