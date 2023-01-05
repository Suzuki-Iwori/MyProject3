#pragma once
#include <math.h>
#include <Novice.h>

const int kscreenWidth = 1200;
const int kscreenHeight = 720;
const int kworldWidth = 1600;
const int kworldHeight = 800;
const int kblockSizeX = 32;
const int kblockSizeY = 32;
const int kblockQuantityX = (kworldWidth / kblockSizeX);
const int kblockQuantityY = (kworldHeight / kblockSizeY);
const float gravity = 0.45f;

enum Scene {
	TITLE,
	LOADING,
	PLAYING,
	GAMECLEAR,
	GAMEOVER
};

enum Vertex {
	LeftTop,
	RightTop,
	LeftBottom,
	RightBottom
};

enum Direct {
	Up,
	Left,
	Down,
	Right
};

enum PlayerDirect {
	PlayerLeft,
	PlayerRight,
	PlayerLeftUp,
	PlayerRightUp
};

enum MapType {
	None,
	Block,
	Gool
};

struct Vector2 {
	float x;
	float y;
};

class Object {
protected:
	Vector2 position; //���W
	unsigned int color; //�F

public:
	///�A�N�Z�b�T
	Vector2 GetPosition() {
		return position;
	}
	unsigned int GetColor() {
		return color;
	}

};

class Dynamic : public Object {
protected:
	Vector2 prePosition; //1F�O�̍��W
	Vector2 radius; //���a
	Vector2 vertex[4]; //��`�Ƃ��Ă�4�_
	Vector2 verocity; //���x
	Vector2 acceleration; //�����x
	bool collisionDirect[4]; //�ڐG����

public:
	//1�t���[���O�̍��W���X�V(�ڐG����Ɏg��)(�K���ړ������̑O��)
	void SetPrePosition() {

		prePosition = position;

	}

	//���W�̍X�V
	void SetPosition(float setX, float setY) {
			position.x = setX;
			position.y = setY;
	}

	//��`�Ƃ���4�_�̍��W���擾
	void SetVertex() {

		vertex[LeftTop].x = position.x - radius.x;
		vertex[LeftTop].y = position.y - radius.y;

		vertex[RightTop].x = position.x + radius.x - 1;
		vertex[RightTop].y = position.y - radius.y;

		vertex[LeftBottom].x = position.x - radius.x;
		vertex[LeftBottom].y = position.y + radius.y - 1;

		vertex[RightBottom].x = position.x + radius.x - 1;
		vertex[RightBottom].y = position.y + radius.y - 1;

	}

	//�ڐG����X�V
	void SetCollisionFlag(Direct direct) {

		collisionDirect[direct] = true;

	}

	//�ڐG���胊�Z�b�g(�K���Ō��)
	void ResetCollision() {

		collisionDirect[Up] = false;
		collisionDirect[Down] = false;
		collisionDirect[Left] = false;
		collisionDirect[Right] = false;

	}

	///�A�N�Z�b�T
	Vector2 GetPrePosition() {
		return prePosition;
	}
	Vector2 GetRadius() {
		return radius;
	}

};

class Weapon : public Dynamic {
private:
	bool isBodyExist; //�{�̏o������
	bool isBlastDetonation; //�����o������
	int detonationCount; //�N���J�E���g
	int calmDownCount; //�N���p���J�E���g
	Vector2 blastRadius; //�������a

public:
	Weapon() {
		prePosition = {};
		radius = { 12.0f,12.0f };
		blastRadius = { 108.0f,108.0f };
		vertex[0] = {};
		verocity = { 0.0f };
		acceleration = { 0.0f };
		collisionDirect[0] = { false };
		isBodyExist = false;
		isBlastDetonation = false;
		detonationCount = 0;
		calmDownCount = 0;
	}

	void ResetWeapon() {

		position.x = -200.0f;
		position.y = -200.0f;
		isBodyExist = false;
		isBlastDetonation = false;
		detonationCount = 0;
		calmDownCount = 0;

	}

	void SetThrowWeapon(Vector2 weaponPosition, PlayerDirect weaponDirect) {
		position = weaponPosition;
		verocity.y = -12.6f;

		if (weaponDirect == PlayerLeft || weaponDirect == PlayerLeftUp) {
			verocity.x = -4.8f;
		}
		else if (weaponDirect == PlayerRight || weaponDirect == PlayerRightUp) {
			verocity.x = 4.8f;
		}

		isBodyExist = true;

	}

	void SetMoveWeapon() {

		if (!isBlastDetonation) {

			detonationCount++;

			acceleration.y = gravity;

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
				isBlastDetonation = true;
			}

		}
		else if (isBlastDetonation) {

			calmDownCount++;

			if (calmDownCount >= 45) {
				ResetWeapon();
			}

		}

	}

	void SetCollisionVerocity(Direct direct) {
		if (direct == Up || direct == Down) {
			verocity.y = 0.0f;
		}
		if (direct == Left || direct == Right) {
			verocity.x *= -1.0f;
		}
	}

	///�A�N�Z�b�T
	bool GetBodyExist() {
		return isBodyExist;
	}
	bool GetBlastDetonation() {
		return isBlastDetonation;
	}
	Vector2 GetBlastRadius() {
		return blastRadius;
	}

};

class Character : public Dynamic {
private:
	Vector2 additionalVerocity; //���͎������x
	PlayerDirect direct; //����
	bool isJump[2]; //�W�����v����
	Weapon* playerWeapon;

public:
	//�R���X�g���N�^
	Character() {
		position = {};
		radius = { 18.0f,24.0f };
		collisionDirect[0] = { false };
		isJump[0] = { false };
		color = 0xFFFFFFFF;
		prePosition = {};
		vertex[0] = {};
		verocity = {};
		acceleration = {};
		additionalVerocity = {};
		direct = {};

		playerWeapon = new Weapon;

	}

	//�ړ�����
	void SetMove(const char inputKey[], const char preInputKey[]) {

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

		if (verocity.y != 0.0f) {
			isJump[0] = true;
		}

		acceleration.y = gravity;

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
			direct = PlayerRight;
		}
		else if (verocity.x < 0.0f) {
			direct = PlayerLeft;
		}
		if (verocity.y < 0.0f) {
			if (direct == PlayerLeft) {
				direct = PlayerLeftUp;
			}
			else if (direct == PlayerRight) {
				direct = PlayerRightUp;
			}
		}
		if (verocity.y >= 0.0f) {
			if (direct == PlayerLeftUp) {
				direct = PlayerLeft;
			}
			else if (direct == PlayerRightUp) {
				direct = PlayerRight;
			}
		}

		position.x += verocity.x;
		position.y += verocity.y;

	}

	//�U������
	void SetAttack(const char inputKey[], const char preInputKey[]) {

		playerWeapon->SetPrePosition();

		if (inputKey[DIK_Z] && !preInputKey[DIK_Z] && !playerWeapon->GetBodyExist()) {

			playerWeapon->SetThrowWeapon(position, direct);

		}
		if (playerWeapon->GetBodyExist()) {
			playerWeapon->SetMoveWeapon();
		}

	}

	//�W�����v���Z�b�g
	void ResetJumpFlag(Direct direct) {
		verocity.y = 0.0f;

		if (direct == Up) {
			isJump[0] = false;
			isJump[1] = false;
		}

	}

	//�v���C���[������
	void ResetPlayer() {

		position.x = 52.0f;
		position.y = 720.0f;
		verocity.y = 0.0f;
		isJump[1] = false;

	}

	///�A�N�Z�b�T

	Weapon* GetWeapon() {
		return playerWeapon;
	}
	void PrintCollision() {

		if (collisionDirect[Up]) {
			Novice::ScreenPrintf(0, 0, "1");
		}
		else if (!collisionDirect[Up]) {
			Novice::ScreenPrintf(0, 0, "0");
		}

		if (collisionDirect[Down]) {
			Novice::ScreenPrintf(0, 20, "1");
		}
		else if (!collisionDirect[Down]) {
			Novice::ScreenPrintf(0, 20, "0");
		}

		if (collisionDirect[Left]) {
			Novice::ScreenPrintf(0, 40, "1");
		}
		else if (!collisionDirect[Left]) {
			Novice::ScreenPrintf(0, 40, "0");
		}

		if (collisionDirect[Right]) {
			Novice::ScreenPrintf(0, 60, "1");
		}
		else if (!collisionDirect[Right]) {
			Novice::ScreenPrintf(0, 60, "0");
		}

	}

	~Character() {
		delete playerWeapon;
	}

};

class Map : public Object {
private:
	int mapChip;
	Vector2 mapChipPosition;

public:
	Map(int x, int y, int map) {

		position = { float(x) * 32.0f,float(y) * 32.0f };
		mapChipPosition = { float(x),float(y) };
		mapChip = map;

		if (mapChip == None) {
			color = 0x00000000;
		}
		else if (mapChip == Block) {
			color = 0x787878FF;
		}
		else if (mapChip == Gool) {
			color = 0xFFFF0070;
		}

	}
	int GetMapChip() {
		return mapChip;
	}

};

/// <summary>
/// �ڐG����(�v���C���[)
/// </summary>
/// <param name="a">�v���[���[�I�u�W�F�N�g</param>
/// <param name="b">�ÓI�I�u�W�F�N�g</param>
void SetPlayerToMapCollision(Character* a, Map* b,bool& goolFrag) {

	if (a->GetPosition().y > b->GetPosition().y - a->GetRadius().y
		&& a->GetPosition().y < (b->GetPosition().y + kblockSizeY) + a->GetRadius().y
		&& a->GetPosition().x > b->GetPosition().x - a->GetRadius().x
		&& a->GetPosition().x < (b->GetPosition().x + kblockSizeX) + a->GetRadius().x) {

		if (b->GetMapChip() == Block) {
			if (a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y) {

				a->SetPosition(a->GetPosition().x, b->GetPosition().y - a->GetRadius().y);
				a->SetCollisionFlag(Up);
				a->ResetJumpFlag(Up);

			}
			if (!(a->GetPrePosition().x <= b->GetPosition().x - a->GetRadius().x)
				&& !(a->GetPrePosition().x >= (b->GetPosition().x + kblockSizeX) + a->GetRadius().x)) {
				if (a->GetPrePosition().y >= (b->GetPosition().y + kblockSizeY) + a->GetRadius().y) {

					a->SetPosition(a->GetPosition().x, (b->GetPosition().y + kblockSizeY) + a->GetRadius().y);
					a->SetCollisionFlag(Down);
					a->ResetJumpFlag(Down);

				}
			}
			if (a->GetPrePosition().x <= b->GetPosition().x - a->GetRadius().x) {

				a->SetPosition(b->GetPosition().x - a->GetRadius().x, a->GetPosition().y);
				a->SetCollisionFlag(Right);

			}
			if (!(a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y)
				&& !(a->GetPrePosition().y >= (b->GetPosition().y + kblockSizeY) + a->GetRadius().y)) {
				if (a->GetPrePosition().x >= (b->GetPosition().x + kblockSizeX) + a->GetRadius().x) {

					a->SetPosition((b->GetPosition().x + kblockSizeX) + a->GetRadius().x, a->GetPosition().y);
					a->SetCollisionFlag(Left);

				}
			}

		}
		else if (b->GetMapChip() == Gool) {
			goolFrag = true;
		}

	}

}

/// <summary>
/// �ڐG����(�v���C���[)
/// </summary>
/// <param name="a">�E�F�|���I�u�W�F�N�g</param>
/// <param name="b">�ÓI�I�u�W�F�N�g</param>
void SetWeaponToMapCollision(Weapon* a, Map* b) {

	if (a->GetPosition().y > b->GetPosition().y - a->GetRadius().y
		&& a->GetPosition().y < (b->GetPosition().y + kblockSizeY) + a->GetRadius().y
		&& a->GetPosition().x > b->GetPosition().x - a->GetRadius().x
		&& a->GetPosition().x < (b->GetPosition().x + kblockSizeX) + a->GetRadius().x) {

		if (b->GetMapChip() == Block) {
			if (a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y) {

				a->SetPosition(a->GetPosition().x, b->GetPosition().y - a->GetRadius().y);
				a->SetCollisionVerocity(Up);
				a->SetCollisionFlag(Up);

			}
			if (!(a->GetPrePosition().x <= b->GetPosition().x - a->GetRadius().x)
				&& !(a->GetPrePosition().x >= (b->GetPosition().x + kblockSizeX) + a->GetRadius().x)) {
				if (a->GetPrePosition().y >= (b->GetPosition().y + kblockSizeY) + a->GetRadius().y) {

					a->SetPosition(a->GetPosition().x, (b->GetPosition().y + kblockSizeY) + a->GetRadius().y);
					a->SetCollisionVerocity(Down);
					a->SetCollisionFlag(Down);

				}
			}
			if (a->GetPrePosition().x <= b->GetPosition().x - a->GetRadius().x) {

				a->SetPosition(b->GetPosition().x - a->GetRadius().x, a->GetPosition().y);
				a->SetCollisionVerocity(Right);
				a->SetCollisionFlag(Right);

			}
			if (!(a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y)
				&& !(a->GetPrePosition().y >= (b->GetPosition().y + kblockSizeY) + a->GetRadius().y)) {
				if (a->GetPrePosition().x >= (b->GetPosition().x + kblockSizeX) + a->GetRadius().x) {

					a->SetPosition((b->GetPosition().x + kblockSizeX) + a->GetRadius().x, a->GetPosition().y);
					a->SetCollisionVerocity(Left);
					a->SetCollisionFlag(Left);

				}
			}

		}

	}

}

/// <summary>
/// �X�N���[������
/// </summary>
/// <param name="a">���I�I�u�W�F�N�g</param>
/// <param name="scrollPoint">�X�N���[���n�_</param>
/// <param name="world">���[���h���W</param>
/// <param name="local">���[�J�����W</param>
/// <param name="scroll">�X�N���[�����W</param>
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