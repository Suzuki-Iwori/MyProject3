#include <Novice.h>
#include "system.h"
#include "stage.h"

const Vector2 scrollPoint = { float(kscreenWidth / 2.0f),float(kscreenHeight / 2.0f) }; //スクロール点

Stage::Stage() {
	Player = new Character;
	for (int i = 0; i < kenemyNum; i++) {
		Dust[i] = new Enemy;
	}
	for (int i = 0; i < kblockQuantityX; i++) {
		for (int j = 0; j < kblockQuantityY; j++) {

			MapInfo[j][i] = new Map(i, j, map[j][i]);

		}
	}

	world = { 0.0f,0.0f };
	local = { 0.0f,0.0f };
	scroll = { 0.0f,0.0f };
	isGool = false;

}
void Stage::Initialize() {
	for (int i = 0; i < kblockQuantityX; i++) {
		for (int j = 0; j < kblockQuantityY; j++) {

			MapInfo[j][i]->SetMapChip(map[j][i]);

		}
	}

	Player->ResetPlayer(722.0f, 168.0f);

	for (int i = 0; i < kenemyNum; i++) {
		Dust[i]->SetEnemy(float(SetRandom(0, kworldWidth)), float(SetRandom(0, kworldHeight)), 18.0f);
	}

	isGool = false;
}
void Stage::Update(const char inputKeys[], const char preInputKeys[]) {

	Player->SetVertex();

	Player->SetMove(inputKeys, preInputKeys);

	for (int i = 0; i < kenemyNum; i++) {
		Dust[i]->MoveEnemy(Player);
		SetBlastToEnemyCollision(Player->GetWeapon()->GetWeaponBlast(), Dust[i]);
	}

	for (int i = 0; i < kenemyNum; i++) {
		for (int j = 0; j < kenemyNum; j++) {
			if (i != j) {
				SetBlastToEnemyCollision(Dust[i]->GetEnemyBlast(), Dust[j]);
			}
		}
	}

	for (int i = 0; i < kblockQuantityX; i++) {
		for (int j = 0; j < kblockQuantityY; j++) {

			SetPlayerToMapCollision(Player, MapInfo[j][i], isGool);
			for (int k = 0; k < kenemyNum; k++) {
				SetBlastToFragileCollision(Dust[k]->GetEnemyBlast(), MapInfo[j][i]);
			}

		}
	}

	if (SetClearCount(Dust, MapInfo) && isGool && inputKeys[DIK_UP]) {
	}

	SetScroll(Player, scrollPoint, world, local, scroll);

	Player->ResetCollision();

}
void Stage::Draw() {

	Novice::DrawBox(0, 0, kscreenWidth, kscreenHeight, 0.0f, 0x000000FF, kFillModeSolid);

	Novice::DrawEllipse(int(local.x), int(local.y), int(Player->GetRadius().x), int(Player->GetRadius().y), 0.0f, 0xFFFFFFFF, kFillModeSolid);

	for (int i = 0; i < kblockQuantityX; i++) {
		for (int j = 0; j < kblockQuantityY; j++) {

			if (-scroll.x + MapInfo[j][i]->GetPosition().x > -32.0f && -scroll.x + MapInfo[j][i]->GetPosition().x < kscreenWidth + 32.0f
				&& -scroll.y + MapInfo[j][i]->GetPosition().y > -32.0f && -scroll.y + MapInfo[j][i]->GetPosition().y < kscreenHeight + 32.0f) {

				if (map[j][i] == Map_Block || map[j][i] == Map_Fragile || map[j][i] == Map_Gool) {
					Novice::DrawBox(int(-scroll.x + MapInfo[j][i]->GetPosition().x), int(-scroll.y + MapInfo[j][i]->GetPosition().y), kblockSizeX, kblockSizeY, 0.0f, MapInfo[j][i]->GetColor(), kFillModeSolid);
				}
				else if (map[j][i] == Map_Slip) {
					Novice::DrawLine(int(-scroll.x + MapInfo[j][i]->GetPosition().x), int(-scroll.y + MapInfo[j][i]->GetPosition().y),
						int(-scroll.x + MapInfo[j][i]->GetPosition().x + kblockSizeX), int(-scroll.y + MapInfo[j][i]->GetPosition().y), MapInfo[j][i]->GetColor());
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

	for (int i = 0; i < kenemyNum; i++) {
		if (!Dust[i]->GetEnemyBlast()->GetBlastDetonation()) {
			Novice::DrawEllipse(int(-scroll.x + Dust[i]->GetPosition().x), int(-scroll.y + Dust[i]->GetPosition().y),
				int(Dust[i]->GetEnemyRadius()), int(Dust[i]->GetEnemyRadius()), 0.0f, 0xFF0000FF, kFillModeSolid);
		}
		else {
			Novice::DrawEllipse(int(-scroll.x + Dust[i]->GetEnemyBlast()->GetPosition().x), int(-scroll.y + Dust[i]->GetEnemyBlast()->GetPosition().y),
				int(Dust[i]->GetEnemyBlast()->GetBlastRadius()), int(Dust[i]->GetEnemyBlast()->GetBlastRadius()), 0.0f, 0xFFFFFF70, kFillModeSolid);
		}
	}

}
Stage::~Stage() {

	delete Player;

	for (int i = 0; i < kenemyNum; i++) {
		delete Dust[i];
	}
	for (int i = 0; i < kblockQuantityX; i++) {
		for (int j = 0; j < kblockQuantityY; j++) {

			delete MapInfo[j][i];

		}
	}

}