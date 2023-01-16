#pragma once
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "system.h"

const int kscreenWidth = 1200; //スクリーン横幅
const int kscreenHeight = 720; //スクリーン縦幅
const int kworldWidth = 1600; //ワールド横幅
const int kworldHeight = 800; //ワールド縦幅
const int kblockSizeX = 32; //ブロック横幅
const int kblockSizeY = 32; //ブロック縦幅
const int kblockQuantityX = (kworldWidth / kblockSizeX); //ブロック横数
const int kblockQuantityY = (kworldHeight / kblockSizeY); //ブロック縦数
const float kgravity = 0.45f; //重力
const int kenemyNum = 16; //エネミーの最大数
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
	Vector2 position; //座標
	unsigned int color; //色

public:
	//座標の更新(X,Y個別)
	void SetPosition(const float &setX, const float &setY) {
		position.x = setX;
		position.y = setY;
	}
	//座標の更新(Vector2一括)
	void SetPosition(const Vector2 &setPosition) {
		position = setPosition;
	}

	///座標(アクセッサ)
	Vector2 GetPosition() {
		return position;
	}
	///色(アクセッサ)
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

	//爆風の初期化処理
	void ResetBlast() {
		isBlastDetonation = false;
		calmDownCount = 0;
		SetPosition(-200.0f, -200.0f);
	}
	//爆風の発生処理
	void SetBlast(const Vector2 &setPosition) {
		SetPosition(setPosition);
		isBlastDetonation = true;
	}
	//爆風の継続処理
	void SetBlastDetonation() {
		calmDownCount++;

		if (calmDownCount >= 24) {
			ResetBlast();
		}

	}

	//爆風出現判定(アクセッサ)
	bool GetBlastDetonation() {
		return isBlastDetonation;
	}
	//爆風半径(アクセッサ)
	float GetBlastRadius() {
		return blastRadius;
	}

};

class Dynamic : public Object {
protected:
	Vector2 prePosition; //1F前の座標
	Vector2 radius; //半径
	Vertex vertex; //矩形としての4点
	Vector2 verocity; //速度
	Vector2 acceleration; //加速度
	bool collisionDirect[4]; //接触判定

public:
	//1F前の座標を更新(接触判定に使う)(必ず移動処理の前に)
	void SetPrePosition() {

		prePosition = position;

	}
	//矩形として4点の座標を取得
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

	///1F前の座標(アクセッサ)
	Vector2 GetPrePosition() {
		return prePosition;
	}
	///半径(アクセッサ)
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
		vertex = {};
		verocity = { 0.0f };
		acceleration = { 0.0f };
		collisionDirect[0] = { false };
		isBodyExist = false;
		detonationCount = 0;

		weaponBlast = new Blast(kblastRadius);

	}

	//攻撃の初期化処理
	void ResetWeapon() {

		position.x = -200.0f;
		position.y = -200.0f;
		isBodyExist = false;
		detonationCount = 0;

	}
	//攻撃の発生処理
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
	//攻撃の移動処理
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
	//攻撃の加速度更新(ぶつかった時)
	void SetCollisionVerocity(const Direct &direct) {
		if (direct == Direct_Up || direct == Direct_Down) {
			verocity.y = 0.0f;
		}
		if (direct == Direct_Left || direct == Direct_Right) {
			verocity.x *= -1.0f;
		}
	}

	///本体出現判定(アクセッサ)
	bool GetBodyExist() {
		return isBodyExist;
	}
	///攻撃の爆風(アクセッサ)
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
	Weapon* playerWeapon; //プレイヤーの攻撃

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

	//プレイヤーの攻撃処理
	void SetAttack(const char inputKey[], const char preInputKey[]) {

		playerWeapon->SetPrePosition();

		if (inputKey[DIK_Z] && !preInputKey[DIK_Z] && !playerWeapon->GetBodyExist()) {

			playerWeapon->SetThrowWeapon(position, direct);

		}
		if (playerWeapon->GetBodyExist()) {
			playerWeapon->SetMoveWeapon();
		}

	}
	//プレイヤーの移動処理
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
	//プレイヤーのジャンプのリセット
	void ResetJumpFlag(Direct direct) {
		verocity.y = 0.0f;

		if (direct == Direct_Up) {
			isJump[0] = false;
			isJump[1] = false;
		}

	}
	//プレイヤーの初期化
	void ResetPlayer(float setX, float setY) {

		position.x = setX;
		position.y = setY;
		verocity.y = 0.0f;
		isJump[1] = false;

		playerWeapon->ResetWeapon();
		playerWeapon->GetWeaponBlast()->ResetBlast();

	}

	///プレイヤーの攻撃(アクセッサ)
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
	bool isExist; //存在フラグ
	float enemyRadius; //半径
	Vector2 verocity; //速度
	Blast* enemyBlast; //爆風
	float enemyAngle; //エネミーの動く方向
	float enemyToPlayerLength; //エネミーとプレイヤーの距離
	int freeMoveCount; //エネミーの方向転換カウント

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

	//エネミーの設置処理
	void SetEnemy(const float &setX, const float &setY, const float &setRadius) {
		SetPosition(setX, setY);
		enemyRadius = setRadius;
		isExist = true;
		enemyBlast->ResetBlast();
	}
	//エネミーの削除処理
	void DefeatEnemy() {
		SetPosition(-200.0f, -200.0f);
		verocity = { 0.0f,0.0f };
		isExist = false;
	}
	//エネミーの移動処理
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

	//エネミーの存在フラグ(アクセッサ)
	bool GetExist() {
		return isExist;
	}
	//エネミーの存在半径(アクセッサ)
	float GetEnemyRadius() {
		return enemyRadius;
	}
	//エネミーの爆風(アクセッサ)
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

/// <summary>
/// クリア処理
/// </summary>
/// <param name="a">エネミーオブジェクト</param>
/// <param name="b">マップオブジェクト</param>
/// <returns>
/// エネミーがいない且つ壊れるブロックが無いとtrue
/// </returns>
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