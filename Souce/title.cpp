#include <Novice.h>
#include "title.h"

const int kscreenWidth = 1200; //�X�N���[������
const int kscreenHeight = 720; //�X�N���[���c��

Title::Title() {
	sceneInCount_ = 60;
	sceneOutCount_ = 0;
	sceneSelect = Title_Start;
}
void Title::Initialize() {

	SceneFragReset();

	sceneInCount_ = 60;
	sceneOutCount_ = 0;
}
//�X�V
void Title::Update(const char inputKeys[], const char preInputKeys[]) {
	if (inputKeys[DIK_DOWN] && !preInputKeys[DIK_DOWN]) {
		sceneSelect = (sceneSelect + 1) % Title_SceneNum;
	}
	if (inputKeys[DIK_UP] && !preInputKeys[DIK_UP]) {
		sceneSelect = (sceneSelect + (Title_SceneNum - 1)) % Title_SceneNum;
	}

	if (sceneSelect == Title_Start&& inputKeys[DIK_Z] && !preInputKeys[DIK_Z]) {
		sceneTransition[Scene_Stage1] = true;
	}

}
//�`��
void Title::Draw() {

	Novice::DrawBox(0, 0, kscreenWidth, kscreenHeight, 0.0f, 0x000000FF, kFillModeSolid);

	if (sceneSelect == Title_Start) {
		Novice::ScreenPrintf(0, 0, "Start");
	}
	if (sceneSelect == Title_Continue) {
		Novice::ScreenPrintf(0, 0, "Continue");
	}
	if (sceneSelect == Title_Setting) {
		Novice::ScreenPrintf(0, 0, "Setting");
	}
	if (sceneSelect == Title_Exist) {
		Novice::ScreenPrintf(0, 0, "Exist");
	}

}
