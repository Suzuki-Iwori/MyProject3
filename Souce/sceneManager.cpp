#include "sceneManager.h"
#include "scene.h"
#include "title.h"
#include "stage.h"

SceneManager::SceneManager() {

	sceneInCount_ = 0;
	sceneOutCount_ = 0;

	SceneFragReset();

	HaveScene[Scene_Title] = new Title;
	HaveScene[Scene_Stage1] = new Stage;
	HaveScene[Scene_GameClear] = new Title;
	HaveScene[Scene_GameOver] = new Title;
	HaveScene[Scene_Loading] = new Title;

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