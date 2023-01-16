#include <Novice.h>
#include "object.h"
#include "system.h"

const char kWindowTitle[] = "マインスイーパー";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kscreenWidth, kscreenHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	srand(unsigned int(time(nullptr)));

	int i;
	int j;
	int k;

	int map[kblockQuantityY][kblockQuantityX] = {

{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
{1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,1,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,2,2,2,2,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,1,1,1,1,1,2,2,2,2,2,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}

	};

	Character* Player = new Character;

	Enemy* Dust[kenemyNum]{};

	for (i = 0; i < kenemyNum; i++) {
		Dust[i] = new Enemy;
	}

	Map* MapInfo[kblockQuantityY][kblockQuantityX]{};

	for (i = 0; i < kblockQuantityX; i++) {
		for (j = 0; j < kblockQuantityY; j++) {

			MapInfo[j][i] = new Map(i, j, map[j][i]);

		}
	}

	const Vector2 scrollPoint = { float(kscreenWidth / 2.0f),float(kscreenHeight / 2.0f) };
	Vector2 world{};
	Vector2 local{};
	Vector2 scroll{};

	Scene scene = Scene_Title;

	bool isGool = false;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		switch (scene) {
		case Scene_Title:

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = Scene_Loading;
			}

			break;

		case Scene_Loading:

			for (i = 0; i < kblockQuantityX; i++) {
				for (j = 0; j < kblockQuantityY; j++) {

					MapInfo[j][i]->SetMapChip(map[j][i]);

				}
			}

			Player->ResetPlayer(722.0f,168.0f);

			for (i = 0; i < kenemyNum; i++) {
				Dust[i]->SetEnemy(float(SetRandom(0, kworldWidth)), float(SetRandom(0, kworldHeight)), 18.0f);
			}

			isGool = false;

			Novice::DrawBox(0, 0, kscreenWidth, kscreenHeight, 0.0f, 0x000000FF, kFillModeSolid);

			scene = Scene_Stage1;

			break;

		case Scene_Stage1:

			///
			/// ↓更新処理ここから
			///

			Player->SetVertex();

			Player->SetMove(keys, preKeys);

			for (i = 0; i < kenemyNum; i++) {
				Dust[i]->MoveEnemy(Player);
				SetBlastToEnemyCollision(Player->GetWeapon()->GetWeaponBlast(), Dust[i]);
			}

			for (i = 0; i < kenemyNum; i++) {
				for (j = 0; j < kenemyNum; j++) {
					if (i != j) {
						SetBlastToEnemyCollision(Dust[i]->GetEnemyBlast(), Dust[j]);
					}
				}
			}

			for (i = 0; i < kblockQuantityX; i++) {
				for (j = 0; j < kblockQuantityY; j++) {

					SetPlayerToMapCollision(Player, MapInfo[j][i], isGool);
					for (k = 0; k < kenemyNum; k++) {
						SetBlastToFragileCollision(Dust[k]->GetEnemyBlast(), MapInfo[j][i]);
					}

				}
			}

			if (SetClearCount(Dust, MapInfo) && isGool && keys[DIK_UP]) {
				scene = Scene_GameClear;
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

			for (i = 0; i < kblockQuantityX; i++) {
				for (j = 0; j < kblockQuantityY; j++) {

					if (-scroll.x + MapInfo[j][i]->GetPosition().x > -32.0f && -scroll.x + MapInfo[j][i]->GetPosition().x < kscreenWidth + 32.0f
						&& -scroll.y + MapInfo[j][i]->GetPosition().y > -32.0f && -scroll.y + MapInfo[j][i]->GetPosition().y < kscreenHeight + 32.0f) {
						if (map[j][i] != Map_None) {

							Novice::DrawBox(int(-scroll.x + MapInfo[j][i]->GetPosition().x), int(-scroll.y + MapInfo[j][i]->GetPosition().y), kblockSizeX, kblockSizeY, 0.0f, MapInfo[j][i]->GetColor(), kFillModeSolid);

						}
					}

				}
			}

			if (!Player->GetWeapon()->GetWeaponBlast()->GetBlastDetonation()) {
				Novice::DrawEllipse(int(-scroll.x + Player->GetWeapon()->GetPosition().x), int(-scroll.y + Player->GetWeapon()->GetPosition().y),
					int(Player->GetWeapon()->GetRadius().x), int(Player->GetWeapon()->GetRadius().y), 0.0f, 0xFFFFFFFF, kFillModeSolid);
			}
			else {
				Novice::DrawEllipse(int(-scroll.x + Player->GetWeapon()->GetWeaponBlast()->GetPosition().x), int(-scroll.y + Player->GetWeapon()->GetWeaponBlast()->GetPosition().y),
					int(Player->GetWeapon()->GetWeaponBlast()->GetBlastRadius()), int(Player->GetWeapon()->GetWeaponBlast()->GetBlastRadius()), 0.0f, 0xFFFFFF70, kFillModeSolid);
			}

			for (i = 0; i < kenemyNum; i++) {
				if (!Dust[i]->GetEnemyBlast()->GetBlastDetonation()) {
					Novice::DrawEllipse(int(-scroll.x + Dust[i]->GetPosition().x), int(-scroll.y + Dust[i]->GetPosition().y),
						int(Dust[i]->GetEnemyRadius()), int(Dust[i]->GetEnemyRadius()), 0.0f, 0xFF0000FF, kFillModeSolid);
				}
				else {
					Novice::DrawEllipse(int(-scroll.x + Dust[i]->GetEnemyBlast()->GetPosition().x), int(-scroll.y + Dust[i]->GetEnemyBlast()->GetPosition().y),
						int(Dust[i]->GetEnemyBlast()->GetBlastRadius()), int(Dust[i]->GetEnemyBlast()->GetBlastRadius()), 0.0f, 0xFFFFFF70, kFillModeSolid);
				}
			}

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

		}

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	delete Player;

	for (i = 0; i < kenemyNum; i++) {
		delete Dust[i];
	}

	for (i = 0; i < kblockQuantityX; i++) {
		for (j = 0; j < kblockQuantityY; j++) {

			delete MapInfo[j][i];

		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
