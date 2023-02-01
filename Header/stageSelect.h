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

	int stageSelectNumber; //�^�C�g�����V�[���؂�ւ�

public:
	StageSelect();

	//������
	void Initialize() override;
	//�X�V
	void Update(const char inputKeys[], const char preInputKeys[]) override;
	//�`��
	void Draw() override;

	~StageSelect() {}

};