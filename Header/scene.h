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
	int sceneInCount_; //�V�[�����͎��J�E���g(���o�p)
	int sceneOutCount_; //�V�[���ޏo���J�E���g(���o�p)

public:
	static bool sceneTransition[Scene_Num]; //�V�[���؂�ւ��t���O(�ÓI)

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

	//�V�[���؂�ւ��t���O�̏�����
	void SceneFragReset() {
		for (int i = 0; i < Scene_Num; i++) {
			sceneTransition[i] = false;
		}
	}

	virtual ~Scene(){}

};