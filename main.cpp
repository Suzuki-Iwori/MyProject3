#include <Novice.h>
#include "class.h"
#include "system.h"

const char kWindowTitle[] = "マインスイーパー";

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

	Enemy* Dust = new Enemy;

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

			for (i = 0; i < kblockQuantityX; i++) {
				for (j = 0; j < kblockQuantityY; j++) {

					MapInfo[j][i]->SetMapChip(map[j][i]);

				}
			}

			Player->ResetPlayer(52.0f,720.0f);
			Dust->SetEnemy(870.0f, 360.0f, 18.0f, Enemy_NotMove);

			isGool = false;

			Novice::DrawBox(0, 0, kscreenWidth, kscreenHeight, 0.0f, 0x000000FF, kFillModeSolid);

			scene = PLAYING;

			break;

		case PLAYING:

			///
			/// ↓更新処理ここから
			///

			Player->SetVertex();

			Player->SetMove(keys, preKeys);

			Dust->MoveEnemy(Player);

			SetBlastToEnemyCollision(Player->GetWeapon()->GetWeaponBlast(), Dust);

			for (i = 0; i < kblockQuantityX; i++) {
				for (j = 0; j < kblockQuantityY; j++) {

					SetPlayerToMapCollision(Player, MapInfo[j][i], isGool);
					SetBlastToFragileCollision(Dust->GetEnemyBlast(), MapInfo[j][i]);

				}
			}

			if (isGool && keys[DIK_UP]) {
				scene = GAMECLEAR;
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

			if (!Player->GetWeapon()->GetWeaponBlast()->GetBlastDetonation()) {
				Novice::DrawEllipse(int(-scroll.x + Player->GetWeapon()->GetPosition().x), int(-scroll.y + Player->GetWeapon()->GetPosition().y),
					int(Player->GetWeapon()->GetRadius().x), int(Player->GetWeapon()->GetRadius().y), 0.0f, 0xFFFFFFFF, kFillModeSolid);
			}
			else {
				Novice::DrawEllipse(int(-scroll.x + Player->GetWeapon()->GetWeaponBlast()->GetPosition().x), int(-scroll.y + Player->GetWeapon()->GetWeaponBlast()->GetPosition().y),
					int(Player->GetWeapon()->GetWeaponBlast()->GetBlastRadius()), int(Player->GetWeapon()->GetWeaponBlast()->GetBlastRadius()), 0.0f, 0xFFFFFF70, kFillModeSolid);
			}

			if (!Dust->GetEnemyBlast()->GetBlastDetonation()) {
				Novice::DrawEllipse(int(-scroll.x + Dust->GetPosition().x), int(-scroll.y + Dust->GetPosition().y),
					int(Dust->GetEnemyRadius()), int(Dust->GetEnemyRadius()), 0.0f, 0xFF0000FF, kFillModeSolid);
			}
			else {
				Novice::DrawEllipse(int(-scroll.x + Dust->GetEnemyBlast()->GetPosition().x), int(-scroll.y + Dust->GetEnemyBlast()->GetPosition().y),
					int(Dust->GetEnemyBlast()->GetBlastRadius()), int(Dust->GetEnemyBlast()->GetBlastRadius()), 0.0f, 0xFFFFFF70, kFillModeSolid);
			}

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
	delete Dust;

	for (i = 0; i < kblockQuantityX; i++) {
		for (j = 0; j < kblockQuantityY; j++) {

			delete MapInfo[j][i];

		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
