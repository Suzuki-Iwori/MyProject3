#include <Novice.h>
#include <time.h>
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

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	delete GameScene;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
