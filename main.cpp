#include <Novice.h>
#include "class.h"

const char kWindowTitle[] = "トライ2Dアクション";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kscreenWidth, kscreenHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	int i;
	int j;

	int map[kblockQuantityY][kblockQuantityX] = {

{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1},
{1,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
{1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}

	};

	Dynamic* Player = new Dynamic;

	Static* MapInfo[kblockQuantityY][kblockQuantityX]{};

	for (i = 0; i < kblockQuantityX; i++) {
		for (j = 0; j < kblockQuantityY; j++) {

			MapInfo[j][i] = new Static(i, j, map[j][i]);

		}
	}

	Field* AquaTerritory = new Field(32.0f, 608.0f, 1568.0f, 800.0f);

	const Vector2 scrollPoint = { 400.0f,800.0f };
	Vector2 world{};
	Vector2 local{};
	Vector2 scroll{};

	Scene scene = TITLE;

	bool isGool = false;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		switch (scene) {
		case TITLE:

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = LOADING;
			}

			break;

		case LOADING:

			Player->ResetPlayer();
			isGool = false;

			Novice::DrawBox(0, 0, kscreenWidth, kscreenHeight, 0.0f, 0x000000FF, kFillModeSolid);

			scene = PLAYING;

			break;

		case PLAYING:

			///
			/// ↓更新処理ここから
			///

			Player->SetPrePosition();
			Player->SetVertex();

			SetField(Player, AquaTerritory, Water);

			Player->SetMove(keys, preKeys);

			for (i = 0; i < kblockQuantityX; i++) {
				for (j = 0; j < kblockQuantityY; j++) {

					SetCollision(Player, MapInfo[j][i], isGool);

				}
			}

			if (isGool) {
				scene = GAMECLEAR;
			}

			if (Player->GetDamage()) {
				scene = GAMEOVER;
			}

			SetScroll(Player, scrollPoint, world, local, scroll);

			Player->ResetCollision();

			///
			/// ↑更新処理ここまで
			///

			///
			/// ↓描画処理ここから
			///

			Novice::DrawBox(0, 0, kscreenWidth, kscreenHeight, 0.0f, 0x000000FF, kFillModeSolid);

			Novice::DrawEllipse(int(local.x), int(local.y), int(Player->GetRadius().x), int(Player->GetRadius().y), 0.0f, 0xFFFFFFFF, kFillModeSolid);

			Novice::DrawBox(int(-scroll.x + AquaTerritory->GetPosition(0).x), int(-scroll.y + AquaTerritory->GetPosition(0).y),
				int(AquaTerritory->GetPosition(1).x - AquaTerritory->GetPosition(0).x),
				int(AquaTerritory->GetPosition(1).y - AquaTerritory->GetPosition(0).y),0.0f, 0x0000FF70, kFillModeSolid);

			for (i = 0; i < kblockQuantityX; i++) {
				for (j = 0; j < kblockQuantityY; j++) {

					if (-scroll.x + MapInfo[j][i]->GetPosition().x > -32.0f && -scroll.x + MapInfo[j][i]->GetPosition().x < kscreenWidth + 32.0f
						&& -scroll.y + MapInfo[j][i]->GetPosition().y > -32.0f && -scroll.y + MapInfo[j][i]->GetPosition().y < kscreenHeight + 32.0f) {
						if (map[j][i] != None) {

							Novice::DrawBox(int(-scroll.x + MapInfo[j][i]->GetPosition().x), int(-scroll.y + MapInfo[j][i]->GetPosition().y), kblockSizeX, kblockSizeY, 0.0f, MapInfo[j][i]->GetColor(), kFillModeSolid);

						}
					}

				}
			}

			///
			/// ↑描画処理ここまで
			///

			break;

		case GAMECLEAR:

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = TITLE;
			}

			break;

		case GAMEOVER:

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = TITLE;
			}

			break;

		}

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	delete Player;
	delete AquaTerritory;

	for (i = 0; i < kblockQuantityX; i++) {
		for (j = 0; j < kblockQuantityY; j++) {

			delete MapInfo[j][i];

		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
