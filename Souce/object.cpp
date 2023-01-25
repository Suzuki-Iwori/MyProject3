#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "system.h"
#include "object.h"

///↓Objectクラス

void Object::SetPosition(const float& setX, const float& setY) {
	position.x = setX;
	position.y = setY;
}
void Object::SetPosition(const Vector2& setPosition) {
	position = setPosition;
}
Vector2 Object::GetPosition() {
	return position;
}
unsigned int Object::GetColor() {
	return color;
}

///↑Objectクラス

///↓Blastクラス

Blast::Blast(float setBlastRadius) {
	position = { -200.0f,-200.0f };
	color = 0xFFFFFF70;
	isBlastDetonation = false;
	calmDownCount = 0;
	blastRadius = setBlastRadius;
}
void Blast::ResetBlast() {
	isBlastDetonation = false;
	calmDownCount = 0;
	SetPosition(-200.0f, -200.0f);
}
void Blast::SetBlast(const Vector2& setPosition) {
	SetPosition(setPosition);
	isBlastDetonation = true;
}
void Blast::SetBlastDetonation() {
	calmDownCount++;

	if (calmDownCount >= 24) {
		ResetBlast();
	}

}
bool Blast::GetBlastDetonation() {
	return isBlastDetonation;
}
bool Blast::GetAfterBlastDetonation() {
	if (calmDownCount >= 10) {
		return true;
	}
	else {
		return false;
	}
}
float Blast::GetBlastRadius() {
	return blastRadius;
}

///↑Blastクラス

///↓Dynamicクラス

void Dynamic::SetPrePosition() {

	prePosition = position;

}
void Dynamic::SetVertex() {

	vertex.LeftTop.x = position.x - radius.x;
	vertex.LeftTop.y = position.y - radius.y;

	vertex.RightTop.x = position.x + radius.x - 1;
	vertex.RightTop.y = position.y - radius.y;

	vertex.LeftBottom.x = position.x - radius.x;
	vertex.LeftBottom.y = position.y + radius.y - 1;

	vertex.RightBottom.x = position.x + radius.x - 1;
	vertex.RightBottom.y = position.y + radius.y - 1;

}
void Dynamic::SetCollisionFlag(Direct direct) {

	collisionDirect[direct] = true;

}
void Dynamic::ResetCollision() {

	collisionDirect[Direct_Up] = false;
	collisionDirect[Direct_Down] = false;
	collisionDirect[Direct_Left] = false;
	collisionDirect[Direct_Right] = false;

}
Vector2 Dynamic::GetPrePosition() {
	return prePosition;
}
Vector2 Dynamic::GetRadius() {
	return radius;
}

///↑Dynamicクラス

///↓Weaponクラス

Weapon::Weapon() {
	prePosition = {};
	radius = { 12.0f,12.0f };
	vertex = {};
	verocity = { 0.0f };
	acceleration = { 0.0f };
	collisionDirect[0] = { false };
	isBodyExist = false;
	detonationCount = 0;

	weaponBlast = new Blast(kblastRadius);

}
void Weapon::ResetWeapon() {

	position.x = -200.0f;
	position.y = -200.0f;
	isBodyExist = false;
	detonationCount = 0;

}
void Weapon::SetThrowWeapon(const Vector2& weaponPosition, const PlayerDirect& weaponDirect) {
	position = weaponPosition;
	verocity.y = -12.6f;

	if (weaponDirect == Player_Left || weaponDirect == Player_LeftUp) {
		verocity.x = -4.2f;
	}
	else if (weaponDirect == Player_Right || weaponDirect == Player_RightUp) {
		verocity.x = 4.2f;
	}

	isBodyExist = true;

}
void Weapon::SetMoveWeapon() {

	if (!weaponBlast->GetBlastDetonation()) {

		detonationCount++;

		acceleration.y = kgravity;

		verocity.y += acceleration.y;

		if (verocity.y > 12.0f) {
			verocity.y = 12.0f;
		}

		if (verocity.x != 0.0f) {
			if (verocity.x < 0.0f) {
				verocity.x += 0.045f;
			}
			if (verocity.x > 0.0f) {
				verocity.x -= 0.045f;
			}
		}

		position.x += verocity.x;
		position.y += verocity.y;

		if (detonationCount >= 120) {
			weaponBlast->SetBlast(position);
		}

	}
	else {

		weaponBlast->SetBlastDetonation();

		if (!weaponBlast->GetBlastDetonation()) {
			ResetWeapon();
		}

	}

}
void Weapon::SetCollisionVerocity(const Direct& direct) {
	if (direct == Direct_Up || direct == Direct_Down) {
		verocity.y = 0.0f;
	}
	if (direct == Direct_Left || direct == Direct_Right) {
		verocity.x *= -1.0f;
	}
}
bool Weapon::GetBodyExist() {
	return isBodyExist;
}
Blast* Weapon::GetWeaponBlast() {
	return weaponBlast;
}
Weapon::~Weapon() {
	delete weaponBlast;
}

///↑Weaponクラス

///↓Characterクラス

Character::Character() {
	position = {};
	radius = { 18.0f,24.0f };
	collisionDirect[0] = { false };
	isJump[0] = { false };
	isThrough = false;
	color = 0xFFFFFFFF;
	prePosition = {};
	vertex = {};
	verocity = {};
	acceleration = {};
	additionalVerocity = {};
	direct = {};

	playerWeapon = new Weapon;

}
void Character::SetAttack(const char inputKey[], const char preInputKey[]) {

	playerWeapon->SetPrePosition();

	if (inputKey[DIK_Z] && !preInputKey[DIK_Z] && !playerWeapon->GetBodyExist()) {

		playerWeapon->SetThrowWeapon(position, direct);

	}
	if (playerWeapon->GetBodyExist()) {
		playerWeapon->SetMoveWeapon();
	}

}
void Character::SetMove(const char inputKey[], const char preInputKey[]) {

	SetPrePosition();

	additionalVerocity.y = -9.4f;

	if (inputKey[DIK_LEFT]) {
		verocity.x = -3.0f;
	}
	else if (inputKey[DIK_RIGHT]) {
		verocity.x = 3.0f;
	}
	else if (!inputKey[DIK_LEFT] && !inputKey[DIK_RIGHT]) {
		verocity.x = 0.0f;
	}

	if (inputKey[DIK_DOWN]) {
		isThrough = true;
	}
	else {
		isThrough = false;
	}

	if (verocity.y != 0.0f) {
		isJump[0] = true;
	}

	acceleration.y = kgravity;

	if (inputKey[DIK_SPACE] && !preInputKey[DIK_SPACE] && !isJump[0]) {
		verocity.y = additionalVerocity.y;
		isJump[0] = true;
	}
	else if (inputKey[DIK_SPACE] && !preInputKey[DIK_SPACE] && isJump[0] && !isJump[1]) {
		verocity.y = additionalVerocity.y * 0.8f;
		isJump[1] = true;
	}

	verocity.y += acceleration.y;

	if (verocity.y > 12.0f) {
		verocity.y = 12.0f;
	}

	if (verocity.x > 0.0f) {
		direct = Player_Right;
	}
	else if (verocity.x < 0.0f) {
		direct = Player_Left;
	}
	if (verocity.y < 0.0f) {
		if (direct == Player_Left) {
			direct = Player_LeftUp;
		}
		else if (direct == Player_Right) {
			direct = Player_RightUp;
		}
	}
	if (verocity.y >= 0.0f) {
		if (direct == Player_LeftUp) {
			direct = Player_Left;
		}
		else if (direct == Player_RightUp) {
			direct = Player_Right;
		}
	}

	position.x += verocity.x;
	position.y += verocity.y;

	SetAttack(inputKey, preInputKey);

}
void Character::ResetJumpFlag(Direct direct) {
	verocity.y = 0.0f;

	if (direct == Direct_Up) {
		isJump[0] = false;
		isJump[1] = false;
	}

}
void Character::ResetPlayer(float setX, float setY) {

	position.x = setX;
	position.y = setY;
	verocity.y = 0.0f;
	isJump[1] = false;

	playerWeapon->ResetWeapon();
	playerWeapon->GetWeaponBlast()->ResetBlast();

}
Weapon* Character::GetWeapon() {
	return playerWeapon;
}
bool Character::GetThrough() {
	return isThrough;
}
void Character::PrintCollision() {

	if (collisionDirect[Direct_Up]) {
		Novice::ScreenPrintf(0, 0, "1");
	}
	else if (!collisionDirect[Direct_Up]) {
		Novice::ScreenPrintf(0, 0, "0");
	}

	if (collisionDirect[Direct_Down]) {
		Novice::ScreenPrintf(0, 20, "1");
	}
	else if (!collisionDirect[Direct_Down]) {
		Novice::ScreenPrintf(0, 20, "0");
	}

	if (collisionDirect[Direct_Left]) {
		Novice::ScreenPrintf(0, 40, "1");
	}
	else if (!collisionDirect[Direct_Left]) {
		Novice::ScreenPrintf(0, 40, "0");
	}

	if (collisionDirect[Direct_Right]) {
		Novice::ScreenPrintf(0, 60, "1");
	}
	else if (!collisionDirect[Direct_Right]) {
		Novice::ScreenPrintf(0, 60, "0");
	}

}
Character::~Character() {
	delete playerWeapon;
}

///↑Characterクラス

///↓Enemyクラス

Enemy::Enemy() {
	position = { -200.0f,-200.0f };
	color = 0xFF0000FF;
	isExist = false;
	enemyRadius = 0;
	verocity = { 0.0f,0.0f };
	enemyAngle = 0.0f;
	enemyToPlayerLength = 0.0f;
	freeMoveCount = 0;

	enemyBlast = new Blast(kblastRadius);

}
void Enemy::SetEnemy(const float& setX, const float& setY, const float& setRadius) {
	SetPosition(setX, setY);
	enemyRadius = setRadius;
	isExist = true;
	enemyBlast->ResetBlast();
}
void Enemy::DefeatEnemy() {
	SetPosition(-200.0f, -200.0f);
	verocity = { 0.0f,0.0f };
	isExist = false;
}
void Enemy::MoveEnemy(Character* getPlayer) {

	if (!enemyBlast->GetBlastDetonation() && isExist) {

		enemyToPlayerLength = sqrtf((GetPosition().x - getPlayer->GetPosition().x) * (GetPosition().x - getPlayer->GetPosition().x) +
			(GetPosition().y - getPlayer->GetPosition().y) * (GetPosition().y - getPlayer->GetPosition().y));

		freeMoveCount++;

		if (freeMoveCount == 1) {
			enemyAngle = float(M_PI * (float(SetRandom(0, 2000)) * 0.001));
		}
		if (enemyToPlayerLength < 180.0f) {
			enemyAngle = float(atan2(GetPosition().y - getPlayer->GetPosition().y, GetPosition().x - getPlayer->GetPosition().x) + M_PI);
		}
		if (freeMoveCount >= SetRandom(100, 220)) {
			freeMoveCount = 0;
		}

		verocity.x = 0.85f * cosf(enemyAngle);
		verocity.y = 0.85f * sinf(enemyAngle);

		position.x += verocity.x;
		position.y += verocity.y;

		if (position.x < 0.0f + enemyRadius) {
			position.x = 0.0f + enemyRadius;
		}
		if (position.x > kworldWidth - enemyRadius) {
			position.x = kworldWidth - enemyRadius;
		}
		if (position.y < 0.0f + enemyRadius) {
			position.y = 0.0f + enemyRadius;
		}
		if (position.y > kworldHeight - enemyRadius) {
			position.y = kworldHeight - enemyRadius;
		}

	}
	else if (enemyBlast->GetBlastDetonation()) {

		enemyBlast->SetBlastDetonation();

		if (!enemyBlast->GetBlastDetonation()) {
			DefeatEnemy();
		}

	}

}
bool Enemy::GetExist() {
	return isExist;
}
float Enemy::GetEnemyRadius() {
	return enemyRadius;
}
Blast* Enemy::GetEnemyBlast() {
	return enemyBlast;
}
Enemy::~Enemy() {
	delete enemyBlast;
}

///↑Enemyクラス

///↓Mapクラス

Map::Map(int x, int y, int map) {

	position = { float(x) * 32.0f,float(y) * 32.0f };
	mapChipPosition = { float(x),float(y) };
	mapChip = map;

	if (mapChip == Map_None) {
		color = 0x00000000;
	}
	else if (mapChip == Map_Block) {
		color = 0x787878FF;
	}
	else if (mapChip == Map_Fragile) {
		color = 0xFF00FFFF;
	}
	else if (mapChip == Map_Slip) {
		color = 0x00FFFFFF;
	}
	else if (mapChip == Map_Gool) {
		color = 0xFFFF0070;
	}

}
int Map::GetMapChip() {
	return mapChip;
}
void Map::SetMapChip(int map) {

	mapChip = map;

	if (mapChip == Map_None) {
		color = 0x00000000;
	}
	else if (mapChip == Map_Block) {
		color = 0x787878FF;
	}
	else if (mapChip == Map_Fragile) {
		color = 0xFF00FFFF;
	}
	else if (mapChip == Map_Slip) {
		color = 0x00FFFFFF;
	}
	else if (mapChip == Map_Gool) {
		color = 0xFFFF0070;
	}

}

///↑Mapクラス

///↓関数

void SetBlastToEnemyCollision(Blast* a, Enemy* b) {
	float length = sqrtf((b->GetPosition().x - a->GetPosition().x) * (b->GetPosition().x - a->GetPosition().x) +
		(b->GetPosition().y - a->GetPosition().y) * (b->GetPosition().y - a->GetPosition().y));

	if (a->GetBlastDetonation() && a->GetAfterBlastDetonation() && !b->GetEnemyBlast()->GetBlastDetonation() && length <= a->GetBlastRadius() + b->GetEnemyRadius()) {

		b->GetEnemyBlast()->SetBlast(b->GetPosition());

	}

}

void SetBlastToFragileCollision(Blast* a, Map* b) {
	float length = sqrtf((b->GetPosition().x + (kblockSizeX / 2.0f) - a->GetPosition().x) * (b->GetPosition().x + (kblockSizeX / 2.0f) - a->GetPosition().x) +
		(b->GetPosition().y + (kblockSizeY / 2.0f) - a->GetPosition().y) * (b->GetPosition().y + (kblockSizeY / 2.0f) - a->GetPosition().y));

	if (b->GetMapChip() == Map_Fragile) {

		if (a->GetBlastDetonation() && length <= a->GetBlastRadius() + (kblockSizeX / 2.0f)) {

			b->SetMapChip(Map_None);

		}

	}

}

void SetWeaponToMapCollision(Weapon* a, Map* b) {

	if (a->GetPosition().y > b->GetPosition().y - a->GetRadius().y
		&& a->GetPosition().y < (b->GetPosition().y + kblockSizeY) + a->GetRadius().y
		&& a->GetPosition().x > b->GetPosition().x - a->GetRadius().x
		&& a->GetPosition().x < (b->GetPosition().x + kblockSizeX) + a->GetRadius().x) {

		if (b->GetMapChip() == Map_Block || b->GetMapChip() == Map_Fragile) {
			if (a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y) {

				a->SetPosition(a->GetPosition().x, b->GetPosition().y - a->GetRadius().y);
				a->SetCollisionVerocity(Direct_Up);
				a->SetCollisionFlag(Direct_Up);

			}
			if (!(a->GetPrePosition().x <= b->GetPosition().x - a->GetRadius().x)
				&& !(a->GetPrePosition().x >= (b->GetPosition().x + kblockSizeX) + a->GetRadius().x)) {
				if (a->GetPrePosition().y >= (b->GetPosition().y + kblockSizeY) + a->GetRadius().y) {

					a->SetPosition(a->GetPosition().x, (b->GetPosition().y + kblockSizeY) + a->GetRadius().y);
					a->SetCollisionVerocity(Direct_Down);
					a->SetCollisionFlag(Direct_Down);

				}
			}
			if (a->GetPrePosition().x <= b->GetPosition().x - a->GetRadius().x) {

				a->SetPosition(b->GetPosition().x - a->GetRadius().x, a->GetPosition().y);
				a->SetCollisionVerocity(Direct_Right);
				a->SetCollisionFlag(Direct_Right);

			}
			if (!(a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y)
				&& !(a->GetPrePosition().y >= (b->GetPosition().y + kblockSizeY) + a->GetRadius().y)) {
				if (a->GetPrePosition().x >= (b->GetPosition().x + kblockSizeX) + a->GetRadius().x) {

					a->SetPosition((b->GetPosition().x + kblockSizeX) + a->GetRadius().x, a->GetPosition().y);
					a->SetCollisionVerocity(Direct_Left);
					a->SetCollisionFlag(Direct_Left);

				}
			}

		}
		else if (b->GetMapChip() == Map_Slip) {
			if (a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y) {

				a->SetPosition(a->GetPosition().x, b->GetPosition().y - a->GetRadius().y);
				a->SetCollisionVerocity(Direct_Up);
				a->SetCollisionFlag(Direct_Up);

			}
		}

	}

	SetBlastToFragileCollision(a->GetWeaponBlast(), b);

}

void SetPlayerToMapCollision(Character* a, Map* b, bool& goolFrag) {

	if (a->GetPosition().y > b->GetPosition().y - a->GetRadius().y
		&& a->GetPosition().y < (b->GetPosition().y + kblockSizeY) + a->GetRadius().y
		&& a->GetPosition().x > b->GetPosition().x - a->GetRadius().x
		&& a->GetPosition().x < (b->GetPosition().x + kblockSizeX) + a->GetRadius().x) {

		if (b->GetMapChip() == Map_Block || b->GetMapChip() == Map_Fragile) {
			if (a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y) {

				a->SetPosition(a->GetPosition().x, b->GetPosition().y - a->GetRadius().y);
				a->SetCollisionFlag(Direct_Up);
				a->ResetJumpFlag(Direct_Up);

			}
			if (!(a->GetPrePosition().x <= b->GetPosition().x - a->GetRadius().x)
				&& !(a->GetPrePosition().x >= (b->GetPosition().x + kblockSizeX) + a->GetRadius().x)) {
				if (a->GetPrePosition().y >= (b->GetPosition().y + kblockSizeY) + a->GetRadius().y) {

					a->SetPosition(a->GetPosition().x, (b->GetPosition().y + kblockSizeY) + a->GetRadius().y);
					a->SetCollisionFlag(Direct_Down);
					a->ResetJumpFlag(Direct_Down);

				}
			}
			if (a->GetPrePosition().x <= b->GetPosition().x - a->GetRadius().x) {

				a->SetPosition(b->GetPosition().x - a->GetRadius().x, a->GetPosition().y);
				a->SetCollisionFlag(Direct_Right);

			}
			if (!(a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y)
				&& !(a->GetPrePosition().y >= (b->GetPosition().y + kblockSizeY) + a->GetRadius().y)) {
				if (a->GetPrePosition().x >= (b->GetPosition().x + kblockSizeX) + a->GetRadius().x) {

					a->SetPosition((b->GetPosition().x + kblockSizeX) + a->GetRadius().x, a->GetPosition().y);
					a->SetCollisionFlag(Direct_Left);

				}
			}

		}
		else if (b->GetMapChip() == Map_Slip) {
			if (!a->GetThrough() && a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y) {

				a->SetPosition(a->GetPosition().x, b->GetPosition().y - a->GetRadius().y);
				a->SetCollisionFlag(Direct_Up);
				a->ResetJumpFlag(Direct_Up);

			}
		}
		else if (b->GetMapChip() == Map_Gool) {
			goolFrag = true;
		}

	}

	SetWeaponToMapCollision(a->GetWeapon(), b);

}

void SetScroll(Character* a, Vector2 scrollPoint, Vector2& world, Vector2& local, Vector2& scroll) {

	float heightMax = float(kworldHeight - kscreenHeight);
	float widthMax = float(kworldWidth - kscreenWidth);

	world.x = a->GetPosition().x;
	world.y = a->GetPosition().y;

	local.x = world.x - scroll.x;
	local.y = world.y - scroll.y;

	scroll.x = world.x - scrollPoint.x;
	scroll.y = world.y - scrollPoint.y;

	if (scroll.x > 0.0f && scroll.x < widthMax) {
		local.x = scrollPoint.x;
	}
	if (scroll.y > 0.0f && scroll.y < heightMax) {
		local.y = scrollPoint.y;
	}

	if (scroll.x < 0.0f) {
		scroll.x = 0.0f;
	}
	if (scroll.x > widthMax) {
		scroll.x = widthMax;
	}

	if (scroll.y < 0.0f) {
		scroll.y = 0.0f;
	}
	if (scroll.y > heightMax) {
		scroll.y = heightMax;
	}

	return;
}

bool SetClearCount(Enemy* a[kenemyNum], Map* b[kblockQuantityY][kblockQuantityX]) {

	int fragileRemain = 0; //壊れるブロックの残り数
	int enemyRemain = 0; //エネミーの残り数

	for (int i = 0; i < kblockQuantityX; i++) {
		for (int j = 0; j < kblockQuantityY; j++) {

			if (b[j][i]->GetMapChip() == Map_Fragile) {
				fragileRemain++;
			}

		}
	}

	for (int i = 0; i < kenemyNum; i++) {

		if (a[i]->GetExist()) {
			enemyRemain++;
		}

	}

	if ((fragileRemain + enemyRemain) == 0) {
		return true;
	}
	else {
		return false;
	}

}