#include "sceneManager.h"
#include "scene.h"
#include "title.h"
#include "stage.h"
#include "stageSelect.h"

SceneManager::SceneManager() {

	sceneInCount_ = 0;
	sceneOutCount_ = 0;

	HaveScene[Scene_Title] = new Title;
	HaveScene[Scene_StageSelect] = new StageSelect;
	HaveScene[Scene_Stage1] = new Stage(1);
	HaveScene[Scene_Stage2] = new Stage(2);
	HaveScene[Scene_Stage3] = new Stage(3);

	OperationScene = Scene_Title;

}

//‰Šú‰»
void SceneManager::Initialize() {
}
//XV
void SceneManager::Update(const char inputKeys[], const char preInputKeys[]) {
	for (int i = 0; i < Scene_Num; i++) {
		if (OperationScene == i) {
			HaveScene[i]->Update(inputKeys, preInputKeys);
		}

		if (sceneTransition[i]) {
			OperationScene = i;
			SceneFragReset();
			HaveScene[i]->Initialize();

		}
	}
}
//•`Ê
void SceneManager::Draw() {
	for (int i = 0; i < Scene_Num; i++) {
		if (OperationScene == i) {
			HaveScene[i]->Draw();
		}
	}
}
SceneManager::~SceneManager() {
	for (int i = 0; i < Scene_Num; i++) {
		delete HaveScene[i];
	}
}