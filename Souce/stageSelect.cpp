#include <Novice.h>
#include "stageSelect.h"

const int kscreenWidth = 1200; //スクリーン横幅
const int kscreenHeight = 720; //スクリーン縦幅

StageSelect::StageSelect() {
	sceneInCount_ = 60;
	sceneOutCount_ = 0;
	stageSelectNumber = Select_Stage1;
}
void StageSelect::Initialize() {
	sceneInCount_ = 60;
	sceneOutCount_ = 0;
	stageSelectNumber = Select_Stage1;
}
//更新
void StageSelect::Update(const char inputKeys[], const char preInputKeys[]) {
	if (inputKeys[DIK_DOWN] && !preInputKeys[DIK_DOWN]) {
		stageSelectNumber = (stageSelectNumber + 1) % Select_Num;
	}
	if (inputKeys[DIK_UP] && !preInputKeys[DIK_UP]) {
		stageSelectNumber = (stageSelectNumber + (Select_Num - 1)) % Select_Num;
	}

	if (inputKeys[DIK_Z] && !preInputKeys[DIK_Z]) {
		if (stageSelectNumber == Select_Stage1) {
			sceneTransition[Scene_Stage1] = true;
		}
		else if (stageSelectNumber == Select_Stage2) {
			sceneTransition[Scene_Stage2] = true;
		}
		else if (stageSelectNumber == Select_Stage3) {
			sceneTransition[Scene_Stage3] = true;
		}
		else if (stageSelectNumber == Select_Exist) {
			sceneTransition[Scene_Title] = true;
		}
	}

}
//描写
void StageSelect::Draw() {

	Novice::DrawBox(0, 0, kscreenWidth, kscreenHeight, 0.0f, 0x000000FF, kFillModeSolid);

	if (stageSelectNumber == Select_Stage1) {
		Novice::ScreenPrintf(0, 0, "Stage1");
	}
	if (stageSelectNumber == Select_Stage2) {
		Novice::ScreenPrintf(0, 0, "Stage2");
	}
	if (stageSelectNumber == Select_Stage3) {
		Novice::ScreenPrintf(0, 0, "Stage3");
	}
	if (stageSelectNumber == Select_Exist) {
		Novice::ScreenPrintf(0, 0, "Exist");
	}

}
