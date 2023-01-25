#pragma once

enum SceneSection {
	Scene_Title,
	Scene_Loading,
	Scene_Stage1,
	Scene_GameClear,
	Scene_GameOver,
	Scene_Num
};

class Scene {
protected:
	int sceneInCount_;
	int sceneOutCount_;

public:
	Scene() {
		sceneInCount_ = 0;
		sceneOutCount_ = 0;
	}

	static bool sceneTransition[Scene_Num];

	virtual void Initialize() = 0;
	virtual void Update(const char inputKeys[], const char preInputKeys[]) = 0;
	virtual void Draw() = 0;

	void SceneFragReset() {
		for (int i = 0; i < Scene_Num; i++) {
			sceneTransition[i] = false;
		}
	}

	virtual ~Scene(){}

};