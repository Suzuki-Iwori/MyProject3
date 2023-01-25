#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "system.h"
#include "object.h"

///↓Objectクラス

void Object::SetPosition(const float& setX, const float& setY) {
	position_.x = setX;
	position_.y = setY;
}
void Object::SetPosition(const Vector2& setPosition) {
	position_ = setPosition;
}
Vector2 Object::GetPosition() {
	return position_;
}
unsigned int Object::GetColor() {
	return color_;
}

///↑Objectクラス

///↓Blastクラス

Blast::Blast(float setBlastRadius) {
	position_ = { -200.0f,-200.0f };
	color_ = 0xFFFFFF70;
	isBlastDetonation_ = false;
	calmDownCount_ = 0;
	blastRadius_ = setBlastRadius;
}
void Blast::ResetBlast() {
	isBlastDetonation_ = false;
	calmDownCount_ = 0;
	SetPosition(-200.0f, -200.0f);
}
void Blast::SetBlast(const Vector2& setPosition) {
	SetPosition(setPosition);
	isBlastDetonation_ = true;
}
void Blast::SetBlastDetonation() {
	calmDownCount_++;

	if (calmDownCount_ >= 24) {
		ResetBlast();
	}

}
bool Blast::GetBlastDetonation() {
	return isBlastDetonation_;
}
bool Blast::GetAfterBlastDetonation() {
	if (calmDownCount_ >= 10) {
		return true;
	}
	else {
		return false;
	}
}
float Blast::GetBlastRadius() {
	return blastRadius_;
}

///↑Blastクラス

///↓Dynamicクラス

void Dynamic::SetPrePosition() {

	prePosition_ = position_;

}
void Dynamic::SetVertex() {

	vertex_.LeftTop.x = position_.x - radius_.x;
	vertex_.LeftTop.y = position_.y - radius_.y;

	vertex_.RightTop.x = position_.x + radius_.x - 1;
	vertex_.RightTop.y = position_.y - radius_.y;

	vertex_.LeftBottom.x = position_.x - radius_.x;
	vertex_.LeftBottom.y = position_.y + radius_.y - 1;

	vertex_.RightBottom.x = position_.x + radius_.x - 1;
	vertex_.RightBottom.y = position_.y + radius_.y - 1;

}
void Dynamic::SetCollisionFlag(Direct direct) {

	collisionDirect_[direct] = true;

}
void Dynamic::ResetCollision() {

	collisionDirect_[Direct_Up] = false;
	collisionDirect_[Direct_Down] = false;
	collisionDirect_[Direct_Left] = false;
	collisionDirect_[Direct_Right] = false;

}
Vector2 Dynamic::GetPrePosition() {
	return prePosition_;
}
Vector2 Dynamic::GetRadius() {
	return radius_;
}

///↑Dynamicクラス

///↓Weaponクラス

Weapon::Weapon() {
	prePosition_ = {};
	radius_ = { 12.0f,12.0f };
	vertex_ = {};
	verocity_ = { 0.0f };
	acceleration_ = { 0.0f };
	collisionDirect_[0] = { false };
	isBodyExist_ = false;
	detonationCount_ = 0;

	weaponBlast_ = new Blast(kblastRadius);

}
void Weapon::ResetWeapon() {

	position_.x = -200.0f;
	position_.y = -200.0f;
	isBodyExist_ = false;
	detonationCount_ = 0;

}
void Weapon::SetThrowWeapon(const Vector2& weaponPosition, const PlayerDirect& weaponDirect) {
	position_ = weaponPosition;
	verocity_.y = -12.6f;

	if (weaponDirect == Player_Left || weaponDirect == Player_LeftUp) {
		verocity_.x = -4.2f;
	}
	else if (weaponDirect == Player_Right || weaponDirect == Player_RightUp) {
		verocity_.x = 4.2f;
	}

	isBodyExist_ = true;

}
void Weapon::SetMoveWeapon() {

	if (!weaponBlast_->GetBlastDetonation()) {

		detonationCount_++;

		acceleration_.y = kgravity;

		verocity_.y += acceleration_.y;

		if (verocity_.y > 12.0f) {
			verocity_.y = 12.0f;
		}

		if (verocity_.x != 0.0f) {
			if (verocity_.x < 0.0f) {
				verocity_.x += 0.045f;
			}
			if (verocity_.x > 0.0f) {
				verocity_.x -= 0.045f;
			}
		}

		position_.x += verocity_.x;
		position_.y += verocity_.y;

		if (detonationCount_ >= 120) {
			weaponBlast_->SetBlast(position_);
		}

	}
	else {

		weaponBlast_->SetBlastDetonation();

		if (!weaponBlast_->GetBlastDetonation()) {
			ResetWeapon();
		}

	}

}
void Weapon::SetCollisionVerocity(const Direct& direct) {
	if (direct == Direct_Up || direct == Direct_Down) {
		verocity_.y = 0.0f;
	}
	if (direct == Direct_Left || direct == Direct_Right) {
		verocity_.x *= -1.0f;
	}
}
bool Weapon::GetBodyExist() {
	return isBodyExist_;
}
Blast* Weapon::GetWeaponBlast() {
	return weaponBlast_;
}
Weapon::~Weapon() {
	delete weaponBlast_;
}

///↑Weaponクラス

///↓Characterクラス

Character::Character() {
	position_ = {};
	radius_ = { 18.0f,24.0f };
	collisionDirect_[0] = { false };
	isJump_[0] = { false };
	isThrough_ = false;
	color_ = 0xFFFFFFFF;
	prePosition_ = {};
	vertex_ = {};
	verocity_ = {};
	acceleration_ = {};
	additionalVerocity_ = {};
	direct_ = {};

	playerWeapon_ = new Weapon;

}
void Character::SetAttack(const char inputKey[], const char preInputKey[]) {

	playerWeapon_->SetPrePosition();

	if (inputKey[DIK_Z] && !preInputKey[DIK_Z] && !playerWeapon_->GetBodyExist()) {

		playerWeapon_->SetThrowWeapon(position_, direct_);

	}
	if (playerWeapon_->GetBodyExist()) {
		playerWeapon_->SetMoveWeapon();
	}

}
void Character::SetMove(const char inputKey[], const char preInputKey[]) {

	SetPrePosition();

	additionalVerocity_.y = -9.4f;

	if (inputKey[DIK_LEFT]) {
		verocity_.x = -3.0f;
	}
	else if (inputKey[DIK_RIGHT]) {
		verocity_.x = 3.0f;
	}
	else if (!inputKey[DIK_LEFT] && !inputKey[DIK_RIGHT]) {
		verocity_.x = 0.0f;
	}

	if (inputKey[DIK_DOWN]) {
		isThrough_ = true;
	}
	else {
		isThrough_ = false;
	}

	if (verocity_.y != 0.0f) {
		isJump_[0] = true;
	}

	acceleration_.y = kgravity;

	if (inputKey[DIK_SPACE] && !preInputKey[DIK_SPACE] && !isJump_[0]) {
		verocity_.y = additionalVerocity_.y;
		isJump_[0] = true;
	}
	else if (inputKey[DIK_SPACE] && !preInputKey[DIK_SPACE] && isJump_[0] && !isJump_[1]) {
		verocity_.y = additionalVerocity_.y * 0.8f;
		isJump_[1] = true;
	}

	verocity_.y += acceleration_.y;

	if (verocity_.y > 12.0f) {
		verocity_.y = 12.0f;
	}

	if (verocity_.x > 0.0f) {
		direct_ = Player_Right;
	}
	else if (verocity_.x < 0.0f) {
		direct_ = Player_Left;
	}
	if (verocity_.y < 0.0f) {
		if (direct_ == Player_Left) {
			direct_ = Player_LeftUp;
		}
		else if (direct_ == Player_Right) {
			direct_ = Player_RightUp;
		}
	}
	if (verocity_.y >= 0.0f) {
		if (direct_ == Player_LeftUp) {
			direct_ = Player_Left;
		}
		else if (direct_ == Player_RightUp) {
			direct_ = Player_Right;
		}
	}

	position_.x += verocity_.x;
	position_.y += verocity_.y;

	SetAttack(inputKey, preInputKey);

}
void Character::ResetJumpFlag(Direct direct) {
	verocity_.y = 0.0f;

	if (direct == Direct_Up) {
		isJump_[0] = false;
		isJump_[1] = false;
	}

}
void Character::ResetPlayer(float setX, float setY) {

	position_.x = setX;
	position_.y = setY;
	verocity_.y = 0.0f;
	isJump_[1] = false;

	playerWeapon_->ResetWeapon();
	playerWeapon_->GetWeaponBlast()->ResetBlast();

}
Weapon* Character::GetWeapon() {
	return playerWeapon_;
}
bool Character::GetThrough() {
	return isThrough_;
}
void Character::PrintCollision() {

	if (collisionDirect_[Direct_Up]) {
		Novice::ScreenPrintf(0, 0, "1");
	}
	else if (!collisionDirect_[Direct_Up]) {
		Novice::ScreenPrintf(0, 0, "0");
	}

	if (collisionDirect_[Direct_Down]) {
		Novice::ScreenPrintf(0, 20, "1");
	}
	else if (!collisionDirect_[Direct_Down]) {
		Novice::ScreenPrintf(0, 20, "0");
	}

	if (collisionDirect_[Direct_Left]) {
		Novice::ScreenPrintf(0, 40, "1");
	}
	else if (!collisionDirect_[Direct_Left]) {
		Novice::ScreenPrintf(0, 40, "0");
	}

	if (collisionDirect_[Direct_Right]) {
		Novice::ScreenPrintf(0, 60, "1");
	}
	else if (!collisionDirect_[Direct_Right]) {
		Novice::ScreenPrintf(0, 60, "0");
	}

}
Character::~Character() {
	delete playerWeapon_;
}

///↑Characterクラス

///↓Enemyクラス

Enemy::Enemy() {
	position_ = { -200.0f,-200.0f };
	color_ = 0xFF0000FF;
	isExist_ = false;
	enemyRadius_ = 0;
	verocity_ = { 0.0f,0.0f };
	enemyAngle_ = 0.0f;
	enemyToPlayerLength_ = 0.0f;
	freeMoveCount_ = 0;

	enemyBlast_ = new Blast(kblastRadius);

}
void Enemy::SetEnemy(const float& setX, const float& setY, const float& setRadius) {
	SetPosition(setX, setY);
	enemyRadius_ = setRadius;
	isExist_ = true;
	enemyBlast_->ResetBlast();
}
void Enemy::DefeatEnemy() {
	SetPosition(-200.0f, -200.0f);
	verocity_ = { 0.0f,0.0f };
	isExist_ = false;
}
void Enemy::MoveEnemy(Character* getPlayer) {

	if (!enemyBlast_->GetBlastDetonation() && isExist_) {

		enemyToPlayerLength_ = sqrtf((GetPosition().x - getPlayer->GetPosition().x) * (GetPosition().x - getPlayer->GetPosition().x) +
			(GetPosition().y - getPlayer->GetPosition().y) * (GetPosition().y - getPlayer->GetPosition().y));

		freeMoveCount_++;

		if (freeMoveCount_ == 1) {
			enemyAngle_ = float(M_PI * (float(SetRandom(0, 2000)) * 0.001));
		}
		if (enemyToPlayerLength_ < 180.0f) {
			enemyAngle_ = float(atan2(GetPosition().y - getPlayer->GetPosition().y, GetPosition().x - getPlayer->GetPosition().x) + M_PI);
		}
		if (freeMoveCount_ >= SetRandom(100, 220)) {
			freeMoveCount_ = 0;
		}

		verocity_.x = 0.85f * cosf(enemyAngle_);
		verocity_.y = 0.85f * sinf(enemyAngle_);

		position_.x += verocity_.x;
		position_.y += verocity_.y;

		if (position_.x < 0.0f + enemyRadius_) {
			position_.x = 0.0f + enemyRadius_;
		}
		if (position_.x > kworldWidth - enemyRadius_) {
			position_.x = kworldWidth - enemyRadius_;
		}
		if (position_.y < 0.0f + enemyRadius_) {
			position_.y = 0.0f + enemyRadius_;
		}
		if (position_.y > kworldHeight - enemyRadius_) {
			position_.y = kworldHeight - enemyRadius_;
		}

	}
	else if (enemyBlast_->GetBlastDetonation()) {

		enemyBlast_->SetBlastDetonation();

		if (!enemyBlast_->GetBlastDetonation()) {
			DefeatEnemy();
		}

	}

}
bool Enemy::GetExist() {
	return isExist_;
}
float Enemy::GetEnemyRadius() {
	return enemyRadius_;
}
Blast* Enemy::GetEnemyBlast() {
	return enemyBlast_;
}
Enemy::~Enemy() {
	delete enemyBlast_;
}

///↑Enemyクラス

///↓Mapクラス

Map::Map(int x, int y, int map) {

	position_ = { float(x) * 32.0f,float(y) * 32.0f };
	mapChipPosition_ = { float(x),float(y) };
	mapChip_ = map;

	if (mapChip_ == Map_None) {
		color_ = 0x00000000;
	}
	else if (mapChip_ == Map_Block) {
		color_ = 0x787878FF;
	}
	else if (mapChip_ == Map_Fragile) {
		color_ = 0xFF00FFFF;
	}
	else if (mapChip_ == Map_Slip) {
		color_ = 0x00FFFFFF;
	}
	else if (mapChip_ == Map_Gool) {
		color_ = 0xFFFF0070;
	}

}
int Map::GetMapChip() {
	return mapChip_;
}
void Map::SetMapChip(int map) {

	mapChip_ = map;

	if (mapChip_ == Map_None) {
		color_ = 0x00000000;
	}
	else if (mapChip_ == Map_Block) {
		color_ = 0x787878FF;
	}
	else if (mapChip_ == Map_Fragile) {
		color_ = 0xFF00FFFF;
	}
	else if (mapChip_ == Map_Slip) {
		color_ = 0x00FFFFFF;
	}
	else if (mapChip_ == Map_Gool) {
		color_ = 0xFFFF0070;
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