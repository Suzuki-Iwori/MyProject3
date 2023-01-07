#pragma once
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "system.h"

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

enum EnemyType {
	Enemy_NotMove,
	Enemy_RandomMove,
	Enemy_ApproachingMove
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
	//座標の更新
	void SetPosition(const float &setX, const float &setY) {
		position.x = setX;
		position.y = setY;
	}
	void SetPosition(const Vector2 &setPosition) {
		position = setPosition;
	}

	///アクセッサ
	Vector2 GetPosition() {
		return position;
	}
	unsigned int GetColor() {
		return color;
	}

};

class Blast : public Object {
private:
	bool isBlastDetonation; //爆風出現判定
	int calmDownCount; //起爆継続カウント
	float blastRadius; //爆風半径

public:
	Blast(float setBlastRadius) {
		position = { -200.0f,-200.0f };
		color = 0xFFFFFF70;
		isBlastDetonation = false;
		calmDownCount = 0;
		blastRadius = setBlastRadius;
	}

	void ResetBlast() {
		isBlastDetonation = false;
		calmDownCount = 0;
		SetPosition(-200.0f, -200.0f);
	}
	void SetBlast(const Vector2 &setPosition) {
		SetPosition(setPosition);
		isBlastDetonation = true;
	}
	void SetBlastDetonation() {
		calmDownCount++;

		if (calmDownCount >= 45) {
			ResetBlast();
		}

	}
	bool GetBlastDetonation() {
		return isBlastDetonation;
	}
	float GetBlastRadius() {
		return blastRadius;
	}

};

class Dynamic : public Object {
protected:
	Vector2 prePosition; //1F前の座標
	Vector2 radius; //半径
	Vector2 vertex[4]; //矩形としての4点
	Vector2 verocity; //速度
	Vector2 acceleration; //加速度
	bool collisionDirect[4]; //接触判定

public:
	//1フレーム前の座標を更新(接触判定に使う)(必ず移動処理の前に)
	void SetPrePosition() {

		prePosition = position;

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

		collisionDirect[Direct_Up] = false;
		collisionDirect[Direct_Down] = false;
		collisionDirect[Direct_Left] = false;
		collisionDirect[Direct_Right] = false;

	}

	///アクセッサ
	Vector2 GetPrePosition() {
		return prePosition;
	}
	Vector2 GetRadius() {
		return radius;
	}

};

class Weapon : public Dynamic {
private:
	bool isBodyExist; //本体出現判定
	int detonationCount; //起爆カウント
	Blast* weaponBlast; //爆風

public:
	Weapon() {
		prePosition = {};
		radius = { 12.0f,12.0f };
		vertex[0] = {};
		verocity = { 0.0f };
		acceleration = { 0.0f };
		collisionDirect[0] = { false };
		isBodyExist = false;
		detonationCount = 0;

		weaponBlast = new Blast(108.0f);

	}

	void ResetWeapon() {

		position.x = -200.0f;
		position.y = -200.0f;
		isBodyExist = false;
		detonationCount = 0;

	}

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

	void SetMoveWeapon() {

		if (!weaponBlast->GetBlastDetonation()) {

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

	void SetCollisionVerocity(const Direct &direct) {
		if (direct == Direct_Up || direct == Direct_Down) {
			verocity.y = 0.0f;
		}
		if (direct == Direct_Left || direct == Direct_Right) {
			verocity.x *= -1.0f;
		}
	}

	///アクセッサ
	bool GetBodyExist() {
		return isBodyExist;
	}
	Blast* GetWeaponBlast() {
		return weaponBlast;
	}

	~Weapon() {
		delete weaponBlast;
	}

};

class Character : public Dynamic {
private:
	Vector2 additionalVerocity; //入力時加速度
	PlayerDirect direct; //方向
	bool isJump[2]; //ジャンプ判定
	Weapon* playerWeapon;

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

		playerWeapon = new Weapon;

	}

	//攻撃処理
	void SetAttack(const char inputKey[], const char preInputKey[]) {

		playerWeapon->SetPrePosition();

		if (inputKey[DIK_Z] && !preInputKey[DIK_Z] && !playerWeapon->GetBodyExist()) {

			playerWeapon->SetThrowWeapon(position, direct);

		}
		if (playerWeapon->GetBodyExist()) {
			playerWeapon->SetMoveWeapon();
		}

	}

	//移動処理
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

	//ジャンプリセット
	void ResetJumpFlag(Direct direct) {
		verocity.y = 0.0f;

		if (direct == Direct_Up) {
			isJump[0] = false;
			isJump[1] = false;
		}

	}

	//プレイヤー初期化
	void ResetPlayer(float setX, float setY) {

		position.x = setX;
		position.y = setY;
		verocity.y = 0.0f;
		isJump[1] = false;

		playerWeapon->ResetWeapon();
		playerWeapon->GetWeaponBlast()->ResetBlast();

	}

	///アクセッサ

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
	EnemyType MovingType; //行動タイプ
	bool isExist; //存在フラグ
	float enemyRadius; //半径
	Vector2 verocity; //速度
	Blast* enemyBlast; //爆風

public:
	Enemy() {
		position = { -200.0f,-200.0f };
		color = 0xFF0000FF;
		MovingType = Enemy_NotMove;
		isExist = false;
		enemyRadius = 0;
		verocity = { 0.0f,0.0f };

		enemyBlast = new Blast(92.0f);

	}

	void SetEnemy(const float &setX, const float &setY, const float &setRadius, const EnemyType &setEnemyType) {
		SetPosition(setX, setY);
		enemyRadius = setRadius;
		MovingType = setEnemyType;
		isExist = true;
		enemyBlast->ResetBlast();
	}

	void DefeatEnemy() {
		SetPosition(-200.0f, -200.0f);
		verocity = { 0.0f,0.0f };
		isExist = false;
	}

	void MoveEnemy(Character* getPlayer) {
		if (!enemyBlast->GetBlastDetonation()) {

			position.x += verocity.x;
			position.y += verocity.y;

		}
		else {

			enemyBlast->SetBlastDetonation();

			if (!enemyBlast->GetBlastDetonation()) {
				DefeatEnemy();
			}

		}

	}

	bool GetExist() {
		return isExist;
	}
	float GetEnemyRadius() {
		return enemyRadius;
	}
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
/// 接触処理(爆風->エネミー)
/// </summary>
/// <param name="a">プレイヤーオブジェクト</param>
/// <param name="b">エネミーオブジェクト</param>
void SetBlastToEnemyCollision(Blast* a, Enemy* b) {
	float length = sqrtf((b->GetPosition().x - a->GetPosition().x) * (b->GetPosition().x - a->GetPosition().x) +
		(b->GetPosition().y - a->GetPosition().y) * (b->GetPosition().y - a->GetPosition().y));

	if (a->GetBlastDetonation() && !b->GetEnemyBlast()->GetBlastDetonation() && length <= a->GetBlastRadius() + b->GetEnemyRadius()) {

		b->GetEnemyBlast()->SetBlast(b->GetPosition());

	}

}

/// <summary>
/// 接触処理(爆風->壊せるブロック)
/// </summary>
/// <param name="a">プレイヤーオブジェクト</param>
/// <param name="b">マップオブジェクト</param>
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
/// 接触処理(ウェポン)
/// </summary>
/// <param name="a">ウェポンオブジェクト</param>
/// <param name="b">マップオブジェクト</param>
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
/// 接触処理(プレイヤー)
/// </summary>
/// <param name="a">プレイヤーオブジェクト</param>
/// <param name="b">静的オブジェクト</param>
void SetPlayerToMapCollision(Character* a, Map* b,bool& goolFrag) {

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