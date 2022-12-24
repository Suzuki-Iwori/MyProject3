#pragma once
#include <math.h>
#include <Novice.h>

const int kscreenWidth = 800;
const int kscreenHeight = 800;
const int kworldWidth = 1600;
const int kworldHeight = 800;
const int kblockSizeX = 32;
const int kblockSizeY = 32;
const int kblockQuantityX = (kworldWidth / kblockSizeX);
const int kblockQuantityY = (kworldHeight / kblockSizeY);
const float gravity = 0.45f;
const float gravityInWater = 0.05f;

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

enum FieldType {
	Normal,
	Water
};

struct Vector2 {
	float x;
	float y;
};

class Object {
protected:
	Vector2 position;
	unsigned int color;

public:
	Vector2 GetPosition() {
		return position;
	}
	unsigned int GetColor() {
		return color;
	}

};

class Dynamic : public Object {
private:
	Vector2 prePosition;
	Vector2 vertex[4];
	Vector2 verocity;
	Vector2 acceleration;
	Vector2 additionalVerocity;
	Vector2 radius;
	PlayerDirect direct;
	bool collisionDirect[4];
	bool isJump[2];
	bool getDamage;
	FieldType state;

public:
	//�R���X�g���N�^
	Dynamic() {
		position = { 48.0f,48.0f };
		radius = { 12.0f,15.0f };
		state = Normal;
		collisionDirect[0] = { false };
		isJump[0] = { false };
		getDamage = { false };
		color = 0xFFFFFFFF;
		prePosition = {};
		vertex[0] = {};
		verocity = {};
		acceleration = {};
		additionalVerocity = {};
		direct = {};
	}

	//1�t���[���O�̍��W���X�V(�ڐG����Ɏg��)(�K���ړ������̑O��)
	void SetPrePosition(){

		prePosition = position;

	}

	//���W�̍X�V(�ύX���Ȃ��ꍇ��NULL�����)
	void SetPosition(float setX, float setY) {
		if (setX != NULL) {
			position.x = setX;
		}
		if (setY != NULL) {
			position.y = setY;
		}
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

	//�ړ�����
	void SetMove(const char inputKey[], const char preInputKey[]) {

		additionalVerocity.y = -9.4f;

		if (inputKey[DIK_LEFT]) {
			if (state == Normal) {
				verocity.x = -3.0f;
			}
			else if (state == Water) {
				verocity.x = -2.25f;
			}
		}
		else if (inputKey[DIK_RIGHT]) {
			if (state == Normal) {
				verocity.x = 3.0f;
			}
			else if (state == Water) {
				verocity.x = 2.25f;
			}
		}
		else if (!inputKey[DIK_LEFT] && !inputKey[DIK_RIGHT]) {
			verocity.x = 0.0f;
		}

		if (verocity.y != 0.0f) {
			isJump[0] = true;
		} 

		if (state == Normal) {

			acceleration.y = gravity;

			if (inputKey[DIK_SPACE] && !preInputKey[DIK_SPACE] && !isJump[0]) {
				verocity.y = additionalVerocity.y;
				isJump[0] = true;
			}
			else if (inputKey[DIK_SPACE] && !preInputKey[DIK_SPACE] && isJump[0] && !isJump[1]) {
				verocity.y = additionalVerocity.y * 0.8f;
				isJump[1] = true;
			}

		}
		else if (state == Water) {

			acceleration.y = gravityInWater;

			if (inputKey[DIK_SPACE] && !preInputKey[DIK_SPACE]) {
				verocity.y = additionalVerocity.y * 0.275f;
				isJump[0] = true;
			}

		}

		verocity.y += acceleration.y;

		if (state == Normal) {
			if (verocity.y > 12.0f) {
				verocity.y = 12.0f;
			}
		}
		else if (state == Water) {
			if (verocity.y > 1.0f) {
				verocity.y = 1.0f;
			}
			if (verocity.y < -3.0f) {
				verocity.y = -3.0f;
			}
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

	//�ڐG����X�V
	void SetCollisionFlag(Direct direct) {

		collisionDirect[direct] = true;

	}

	//�t�B�[���h��ԍX�V
	void SetState(FieldType environment) {

		state = environment;

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

	//�ڐG���胊�Z�b�g(�K���Ō��)
	void ResetCollision() {

		collisionDirect[Up] = false;
		collisionDirect[Down] = false;
		collisionDirect[Left] = false;
		collisionDirect[Right] = false;

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

		position.x = 48.0f;
		position.y = 48.0f;
		verocity.y = 0.0f;
		isJump[1] = false;
		getDamage = false;

	}

	///�A�N�Z�b�T

	Vector2 GetPrePosition() {
		return prePosition;
	}
	Vector2 GetRadius() {
		return radius;
	}
	bool GetDamage() {
		return getDamage;
	}

};

class Static : public Object {
private:
	int mapChip;
	Vector2 mapChipPosition;

public:
	Static(int x, int y, int map) {

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

class Field {
private:
	Vector2 position[2];

public:
	Field(float startX, float startY, float endX, float endY) {

		position[0].x = startX;
		position[0].y = startY;
		position[1].x = endX;
		position[1].y = endY;

	}
	Vector2 GetPosition(int array) {
		return position[array];
	}

};

/// <summary>
/// �ڐG����
/// </summary>
/// <param name="a">���I�I�u�W�F�N�g</param>
/// <param name="b">�ÓI�I�u�W�F�N�g</param>
void SetCollision(Dynamic* a, Static* b,bool& goolFrag) {

	if (a->GetPosition().y > b->GetPosition().y - a->GetRadius().y
		&& a->GetPosition().y < (b->GetPosition().y + kblockSizeY) + a->GetRadius().y
		&& a->GetPosition().x > b->GetPosition().x - a->GetRadius().x
		&& a->GetPosition().x < (b->GetPosition().x + kblockSizeX) + a->GetRadius().x) {

		if (b->GetMapChip() == Block) {
			if (a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y) {

				a->SetPosition(NULL, b->GetPosition().y - a->GetRadius().y);
				a->SetCollisionFlag(Up);
				a->ResetJumpFlag(Up);

			}
			if (!(a->GetPrePosition().x <= b->GetPosition().x - a->GetRadius().x)
				&& !(a->GetPrePosition().x >= (b->GetPosition().x + kblockSizeX) + a->GetRadius().x)) {
				if (a->GetPrePosition().y >= (b->GetPosition().y + kblockSizeY) + a->GetRadius().y) {

					a->SetPosition(NULL, (b->GetPosition().y + kblockSizeY) + a->GetRadius().y);
					a->SetCollisionFlag(Down);
					a->ResetJumpFlag(Down);

				}
			}
			if (a->GetPrePosition().x <= b->GetPosition().x - a->GetRadius().x) {

				a->SetPosition(b->GetPosition().x - a->GetRadius().x, NULL);
				a->SetCollisionFlag(Right);

			}
			if (!(a->GetPrePosition().y <= b->GetPosition().y - a->GetRadius().y)
				&& !(a->GetPrePosition().y >= (b->GetPosition().y + kblockSizeY) + a->GetRadius().y)) {
				if (a->GetPrePosition().x >= (b->GetPosition().x + kblockSizeX) + a->GetRadius().x) {

					a->SetPosition((b->GetPosition().x + kblockSizeX) + a->GetRadius().x, NULL);
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
/// �X�N���[������
/// </summary>
/// <param name="a">���I�I�u�W�F�N�g</param>
/// <param name="scrollPoint">�X�N���[���n�_</param>
/// <param name="world">���[���h���W</param>
/// <param name="local">���[�J�����W</param>
/// <param name="scroll">�X�N���[�����W</param>
void SetScroll(Dynamic* a, Vector2 scrollPoint, Vector2& world, Vector2& local, Vector2& scroll) {

	float heightMax = kscreenHeight;
	float widthMax = kscreenWidth;

	world.x = a->GetPosition().x;
	world.y = a->GetPosition().y;

	local.x = world.x - scroll.x;
	local.y = world.y - scroll.y;

	scroll.x = world.x - scrollPoint.x;
	scroll.y = world.y - scrollPoint.y;

	if (scroll.x > 0.0f && scroll.x < heightMax) {
		local.x = scrollPoint.x;
	}
	if (scroll.y > 0.0f && scroll.y < widthMax) {
		local.y = scrollPoint.y;
	}

	if (scroll.x < 0.0f) {
		scroll.x = 0.0f;
	}
	if (scroll.x > heightMax) {
		scroll.x = heightMax;
	}

	if (scroll.y < 0.0f) {
		scroll.y = 0.0f;
	}
	if (scroll.y > widthMax) {
		scroll.y = widthMax;
	}
	return;
}

void SetField(Dynamic* a, Field* b, FieldType c) {

	if (a->GetPosition().y > b->GetPosition(0).y && a->GetPosition().y < b->GetPosition(1).y && a->GetPosition().x > b->GetPosition(0).x && a->GetPosition().x < b->GetPosition(1).x) {

		if (c == Water){
			a->SetState(Water);
		}

	}
	else {
		a->SetState(Normal);
	}

}