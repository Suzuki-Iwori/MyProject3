#include <Novice.h>
#include <time.h>
//#include "stage.h"
//#include "title.h"

#include "sceneManager.h"

const char kWindowTitle[] = "マインスイーパー";

const int kscreenWidth = 1200; //スクリーン横幅
const int kscreenHeight = 720; //スクリーン縦幅

bool Scene::sceneTransition[Scene_Num];

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kscreenWidth, kscreenHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	srand(unsigned int(time(nullptr)));

	/*Title* title = new Title;

	Stage* stage1 = new Stage;

	SceneSection scene = Scene_Title;*/

	SceneManager* GameScene = new SceneManager;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		GameScene->Update(keys, preKeys);

		GameScene->Draw();

		/*switch (scene) {
		case Scene_Title:

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = Scene_Loading;
			}

			title->Update(keys, preKeys);
			 
			title->Draw();

			break;

		case Scene_Loading:

			stage1->Initialize();

			scene = Scene_Stage1;

			break;

		case Scene_Stage1:

			///
			/// ↓更新処理ここから
			///

			stage1->Update(keys, preKeys);

			///
			/// ↑更新処理ここまで
			///

			///
			/// ↓描画処理ここから
			///

			stage1->Draw();

			///
			/// ↑描画処理ここまで
			///

			break;

		case Scene_GameClear:

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = Scene_Title;
			}

			break;

		case Scene_GameOver:

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = Scene_Title;
			}

			break;

		}*/

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	//delete stage1;

	delete GameScene;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
