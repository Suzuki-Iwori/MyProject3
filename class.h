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
	Vector2 position; //座標
	unsigned int color; //色

public:
	Vector2 GetPosition() {
		return position;
	}
	unsigned int GetColor() {
		return color;
	}

};

class Dynamic : public Object {
protected:
	Vector2 prePosition; //1F前の座標
	Vector2 radius; //半径
	Vector2 vertex[4]; //矩形としての4点
	Vector2 verocity; //速度
	Vector2 acceleration; //加速度
	Vector2 additionalVerocity; //追加加速度
	bool collisionDirect[4]; //接触判定

public:
	//1フレーム前の座標を更新(接触判定に使う)(必ず移動処理の前に)
	void SetPrePosition() {

		prePosition = position;

	}

	//座標の更新(変更しない場合はNULLを入力)
	void SetPosition(float setX, float setY) {
		if (setX != NULL) {
			position.x = setX;
		}
		if (setY != NULL) {
			position.y = setY;
		}
	}

	//矩形として4点の座標を取得
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

	//接触判定更新
	void SetCollisionFlag(Direct direct) {

		collisionDirect[direct] = true;

	}

	//接触判定リセット(必ず最後に)
	void ResetCollision() {

		collisionDirect[Up] = false;
		collisionDirect[Down] = false;
		collisionDirect[Left] = false;
		collisionDirect[Right] = false;

	}

};

class Weapon : public Dynamic {
private:
	bool isBodyExist; //本体出現判定
	bool isBlastDetonation; //爆風出現判定
	int detonationCount; //起爆判定
	int calmDownCount; //起爆継続判定
	Vector2 blastRadius; //爆風半径

public:






};

class Character : public Dynamic {
private:
	PlayerDirect direct; //方向
	bool isJump[2]; //ジャンプ判定

public:
	//コンストラクタ
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
	}

	//移動処理
	void SetMove(const char inputKey[], const char preInputKey[]) {

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

	//ジャンプリセット
	void ResetJumpFlag(Direct direct) {
		verocity.y = 0.0f;

		if (direct == Up) {
			isJump[0] = false;
			isJump[1] = false;
		}

	}

	//プレイヤー初期化
	void ResetPlayer() {

		position.x = 52.0f;
		position.y = 720.0f;
		verocity.y = 0.0f;
		isJump[1] = false;

	}

	///アクセッサ

	Vector2 GetPrePosition() {
		return prePosition;
	}
	Vector2 GetRadius() {
		return radius;
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
/// 接触処理(プレイヤー)
/// </summary>
/// <param name="a">動的オブジェクト</param>
/// <param name="b">静的オブジェクト</param>
void SetPlayerToMapCollision(Character* a, Map* b,bool& goolFrag) {

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
/// スクロール処理
/// </summary>
/// <param name="a">動的オブジェクト</param>
/// <param name="scrollPoint">スクロール地点</param>
/// <param name="world">ワールド座標</param>
/// <param name="local">ローカル座標</param>
/// <param name="scroll">スクロール座標</param>
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