#pragma once
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "system.h"

const int kscreenWidth = 1200; //�X�N���[������
const int kscreenHeight = 720; //�X�N���[���c��
const int kworldWidth = 1600; //���[���h����
const int kworldHeight = 800; //���[���h�c��
const int kblockSizeX = 32; //�u���b�N����
const int kblockSizeY = 32; //�u���b�N�c��
const int kblockQuantityX = (kworldWidth / kblockSizeX); //�u���b�N����
const int kblockQuantityY = (kworldHeight / kblockSizeY); //�u���b�N�c��
const float kgravity = 0.45f; //�d��
const int kenemyNum = 16; //�G�l�~�[�̍ő吔
const float kblastRadius = 124.0f;

enum Scene {
	Scene_Title,
	Scene_Loading,
	Scene_Stage1,
	Scene_GameClear,
	Scene_GameOver
};

enum Direct {
	Direct_Up,
	Direct_Left,
	Direct_Down,
	Direct_Right
};

enum PlayerDirect {
	Player_Left,
	Player_Right,
	Player_LeftUp,
	Player_RightUp
};

enum MapType {
	Map_None,
	Map_Block,
	Map_Fragile,
	Map_Gool
};

struct Vector2 {
	float x;
	float y;
};

struct Vertex {
	Vector2 LeftTop;
	Vector2	RightTop;
	Vector2	LeftBottom;
	Vector2	RightBottom;
};

class Object {
protected:
	Vector2 position; //���W
	unsigned int color; //�F

public:
	//���W�̍X�V(X,Y��)
	void SetPosition(const float &setX, const float &setY) {
		position.x = setX;
		position.y = setY;
	}
	//���W�̍X�V(Vector2�ꊇ)
	void SetPosition(const Vector2 &setPosition) {
		position = setPosition;
	}

	///���W(�A�N�Z�b�T)
	Vector2 GetPosition() {
		return position;
	}
	///�F(�A�N�Z�b�T)
	unsigned int GetColor() {
		return color;
	}

};

class Blast : public Object {
private:
	bool isBlastDetonation; //�����o������
	int calmDownCount; //�N���p���J�E���g
	float blastRadius; //�������a

public:
	Blast(float setBlastRadius) {
		position = { -200.0f,-200.0f };
		color = 0xFFFFFF70;
		isBlastDetonation = false;
		calmDownCount = 0;
		blastRadius = setBlastRadius;
	}

	//�����̏���������
	void ResetBlast() {
		isBlastDetonation = false;
		calmDownCount = 0;
		SetPosition(-200.0f, -200.0f);
	}
	//�����̔�������
	void SetBlast(const Vector2 &setPosition) {
		SetPosition(setPosition);
		isBlastDetonation = true;
	}
	//�����̌p������
	void SetBlastDetonation() {
		calmDownCount++;

		if (calmDownCount >= 24) {
			ResetBlast();
		}

	}

	//�����o������(�A�N�Z�b�T)
	bool GetBlastDetonation() {
		return isBlastDetonation;
	}
	//�������a(�A�N�Z�b�T)
	float GetBlastRadius() {
		return blastRadius;
	}

};

class Dynamic : public Object {
protected:
	Vector2 prePosition; //1F�O�̍��W
	Vector2 radius; //���a
	Vertex vertex; //��`�Ƃ��Ă�4�_
	Vector2 verocity; //���x
	Vector2 acceleration; //�����x
	bool collisionDirect[4]; //�ڐG����

public:
	//1F�O�̍��W���X�V(�ڐG����Ɏg��)(�K���ړ������̑O��)
	void SetPrePosition() {

		prePosition = position;

	}
	//��`�Ƃ���4�_�̍��W���擾
	void SetVertex() {

		vertex.LeftTop.x = position.x - radius.x;
		vertex.LeftTop.y = position.y - radius.y;

		vertex.RightTop.x = position.x + radius.x - 1;
		vertex.RightTop.y = position.y - radius.y;

		vertex.LeftBottom.x = position.x - radius.x;
		vertex.LeftBottom.y = position.y + radius.y - 1;

		vertex.RightBottom.x = position.x + radius.x - 1;
		vertex.RightBottom.y = position.y + radius.y - 1;

	}
	//�ڐG����X�V
	void SetCollisionFlag(Direct direct) {

		collisionDirect[direct] = true;

	}
	//�ڐG���胊�Z�b�g(�K���Ō��)
	void ResetCollision() {

		collisionDirect[Direct_Up] = false;
		collisionDirect[Direct_Down] = false;
		collisionDirect[Direct_Left] = false;
		collisionDirect[Direct_Right] = false;

	}

	///1F�O�̍��W(�A�N�Z�b�T)
	Vector2 GetPrePosition() {
		return prePosition;
	}
	///���a(�A�N�Z�b�T)
	Vector2 GetRadius() {
		return radius;
	}

};

class Weapon : public Dynamic {
private:
	bool isBodyExist; //�{�̏o������
	int detonationCount; //�N���J�E���g
	Blast* weaponBlast; //����

public:
	Weapon() {
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

	//�U���̏���������
	void ResetWeapon() {

		position.x = -200.0f;
		position.y = -200.0f;
		isBodyExist = false;
		detonationCount = 0;

	}
	//�U���̔�������
	void SetThrowWeapon(const Vector2 &weaponPosition, const PlayerDirect &weaponDirect) {
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
	//�U���̈ړ�����
	void SetMoveWeapon() {

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
	//�U���̉����x�X�V(�Ԃ�������)
	void SetCollisionVerocity(const Direct &direct) {
		if (direct == Direct_Up || direct == Direct_Down) {
			verocity.y = 0.0f;
		}
		if (direct == Direct_Left || direct == Direct_Right) {
			verocity.x *= -1.0f;
		}
	}

	///�{�̏o������(�A�N�Z�b�T)
	bool GetBodyExist() {
		return isBodyExist;
	}
	///�U���̔���(�A�N�Z�b�T)
	Blast* GetWeaponBlast() {
		return weaponBlast;
	}

	~Weapon() {
		delete weaponBlast;
	}

};

class Character : public Dynamic {
private:
	Vector2 additionalVerocity; //���͎������x
	PlayerDirect direct; //����
	bool isJump[2]; //�W�����v����
	Weapon* playerWeapon; //�v���C���[�̍U��

public:
	Character() {
		position = {};
		radius = { 18.0f,24.0f };
		collisionDirect[0] = { false };
		isJump[0] = { false };
		color = 0xFFFFFFFF;
		prePosition = {};
		vertex = {};
		verocity = {};
		acceleration = {};
		additionalVerocity = {};
		direct = {};

		playerWeapon = new Weapon;

	}

	//�v���C���[�̍U������
	void SetAttack(const char inputKey[], const char preInputKey[]) {

		playerWeapon->SetPrePosition();

		if (inputKey[DIK_Z] && !preInputKey[DIK_Z] && !playerWeapon->GetBodyExist()) {

			playerWeapon->SetThrowWeapon(position, direct);

		}
		if (playerWeapon->GetBodyExist()) {
			playerWeapon->SetMoveWeapon();
		}

	}
	//�v���C���[�̈ړ�����
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
	//�v���C���[�̃W�����v�̃��Z�b�g
	void ResetJumpFlag(Direct direct) {
		verocity.y = 0.0f;

		if (direct == Direct_Up) {
			isJump[0] = false;
			isJump[1] = false;
		}

	}
	//�v���C���[�̏�����
	void ResetPlayer(float setX, float setY) {

		position.x = setX;
		position.y = setY;
		verocity.y = 0.0f;
		isJump[1] = false;

		playerWeapon->ResetWeapon();
		playerWeapon->GetWeaponBlast()->ResetBlast();

	}

	///�v���C���[�̍U��(�A�N�Z�b�T)
	Weapon* GetWeapon() {
		return playerWeapon;
	}
	void PrintCollision() {

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

	~Character() {
		delete playerWeapon;
	}

};

class Enemy : public Object {
private:
	bool isExist; //���݃t���O
	float enemyRadius; //���a
	Vector2 verocity; //���x
	Blast* enemyBlast; //����
	float enemyAngle; //�G�l�~�[�̓�������
	float enemyToPlayerLength; //�G�l�~�[�ƃv���C���[�̋���
	int freeMoveCount; //�G�l�~�[�̕����]���J�E���g

public:
	Enemy() {
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

	//�G�l�~�[�̐ݒu����
	void SetEnemy(const float &setX, const float &setY, const float &setRadius) {
		SetPosition(setX, setY);
		enemyRadius = setRadius;
		isExist = true;
		enemyBlast->ResetBlast();
	}
	//�G�l�~�[�̍폜����
	void DefeatEnemy() {
		SetPosition(-200.0f, -200.0f);
		verocity = { 0.0f,0.0f };
		isExist = false;
	}
	//�G�l�~�[�̈ړ�����
	void MoveEnemy(Character* getPlayer) {

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
		else if(enemyBlast->GetBlastDetonation()) {

			enemyBlast->SetBlastDetonation();

			if (!enemyBlast->GetBlastDetonation()) {
				DefeatEnemy();
			}

		}

	}

	//�G�l�~�[�̑��݃t���O(�A�N�Z�b�T)
	bool GetExist() {
		return isExist;
	}
	//�G�l�~�[�̑��ݔ��a(�A�N�Z�b�T)
	float GetEnemyRadius() {
		return enemyRadius;
	}
	//�G�l�~�[�̔���(�A�N�Z�b�T)
	Blast* GetEnemyBlast() {
		return enemyBlast;
	}

	~Enemy() {
		delete enemyBlast;
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

		if (mapChip == Map_None) {
			color = 0x00000000;
		}
		else if (mapChip == Map_Block) {
			color = 0x787878FF;
		}
		else if (mapChip == Map_Fragile) {
			color = 0xFF00FFFF;
		}
		else if (mapChip == Map_Gool) {
			color = 0xFFFF0070;
		}

	}
	int GetMapChip() {
		return mapChip;
	}
	void SetMapChip(int map) {

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
		else if (mapChip == Map_Gool) {
			color = 0xFFFF0070;
		}

	}

};

/// <summary>
/// �ڐG����(����->�G�l�~�[)
/// </summary>
/// <param name="a">�v���C���[�I�u�W�F�N�g</param>
/// <param name="b">�G�l�~�[�I�u�W�F�N�g</param>
void SetBlastToEnemyCollision(Blast* a, Enemy* b) {
	float length = sqrtf((b->GetPosition().x - a->GetPosition().x) * (b->GetPosition().x - a->GetPosition().x) +
		(b->GetPosition().y - a->GetPosition().y) * (b->GetPosition().y - a->GetPosition().y));

	if (a->GetBlastDetonation() && !b->GetEnemyBlast()->GetBlastDetonation() && length <= a->GetBlastRadius() + b->GetEnemyRadius()) {

		b->GetEnemyBlast()->SetBlast(b->GetPosition());

	}

}

/// <summary>
/// �ڐG����(����->�󂹂�u���b�N)
/// </summary>
/// <param name="a">�v���C���[�I�u�W�F�N�g</param>
/// <param name="b">�}�b�v�I�u�W�F�N�g</param>
void SetBlastToFragileCollision(Blast* a, Map* b) {
	float length = sqrtf((b->GetPosition().x + (kblockSizeX / 2.0f) - a->GetPosition().x) * (b->GetPosition().x + (kblockSizeX / 2.0f) - a->GetPosition().x) +
		(b->GetPosition().y + (kblockSizeY / 2.0f) - a->GetPosition().y) * (b->GetPosition().y + (kblockSizeY / 2.0f) - a->GetPosition().y));

	if (b->GetMapChip() == Map_Fragile) {

		if (a->GetBlastDetonation() && length <= a->GetBlastRadius() + (kblockSizeX / 2.0f)) {

			b->SetMapChip(Map_None);

		}

	}

}

/// <summary>
/// �ڐG����(�E�F�|��)
/// </summary>
/// <param name="a">�E�F�|���I�u�W�F�N�g</param>
/// <param name="b">�}�b�v�I�u�W�F�N�g</param>
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

	}

	SetBlastToFragileCollision(a->GetWeaponBlast(), b);

}

/// <summary>
/// �ڐG����(�v���C���[)
/// </summary>
/// <param name="a">�v���C���[�I�u�W�F�N�g</param>
/// <param name="b">�ÓI�I�u�W�F�N�g</param>
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
		else if (b->GetMapChip() == Map_Gool) {
			goolFrag = true;
		}

	}

	SetWeaponToMapCollision(a->GetWeapon(), b);

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

/// <summary>
/// �N���A����
/// </summary>
/// <param name="a">�G�l�~�[�I�u�W�F�N�g</param>
/// <param name="b">�}�b�v�I�u�W�F�N�g</param>
/// <returns>
/// �G�l�~�[�����Ȃ�������u���b�N��������true
/// </returns>
bool SetClearCount(Enemy* a[kenemyNum], Map* b[kblockQuantityY][kblockQuantityX]) {

	int fragileRemain = 0; //����u���b�N�̎c�萔
	int enemyRemain = 0; //�G�l�~�[�̎c�萔

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