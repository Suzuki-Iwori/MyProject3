#pragma once

const int kscreenWidth = 1200; //スクリーン横幅
const int kscreenHeight = 720; //スクリーン縦幅
const int kworldWidth = 1600; //ワールド横幅
const int kworldHeight = 800; //ワールド縦幅
const int kblockSizeX = 32; //ブロック横幅
const int kblockSizeY = 32; //ブロック縦幅
const int kblockQuantityX = (kworldWidth / kblockSizeX); //ブロック横数
const int kblockQuantityY = (kworldHeight / kblockSizeY); //ブロック縦数
const float kgravity = 0.45f; //重力
const int kenemyNum = 24; //エネミーの最大数
const float kblastRadius = 128.0f; //爆風の半径

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
	Map_Slip,
	Map_Gool
};

/// ベクトル(2方向)
struct Vector2 {
	float x;
	float y;
};

/// 矩形4点
struct Vertex {
	Vector2 LeftTop;
	Vector2	RightTop;
	Vector2	LeftBottom;
	Vector2	RightBottom;
};

/// 座標を持つオブジェクト
class Object {
protected:
	Vector2 position_; //座標
	unsigned int color_; //色

public:
	//座標の更新(X,Y個別)
	void SetPosition(const float& setX, const float& setY);
	//座標の更新(Vector2一括)
	void SetPosition(const Vector2& setPosition);

	///座標(アクセッサ)
	Vector2 GetPosition();
	///色(アクセッサ)
	unsigned int GetColor();

};

/// 爆風
class Blast : public Object {
private:
	bool isBlastDetonation_; //爆風出現判定
	int calmDownCount_; //起爆継続カウント
	float blastRadius_; //爆風半径

public:
	Blast(float setBlastRadius);

	//爆風の初期化処理
	void ResetBlast();
	//爆風の発生処理
	void SetBlast(const Vector2& setPosition);
	//爆風の継続処理
	void SetBlastDetonation();

	//爆風出現判定(アクセッサ)
	bool GetBlastDetonation();
	//爆風継続判定(アクセッサ)
	bool GetAfterBlastDetonation();
	//爆風半径(アクセッサ)
	float GetBlastRadius();

};

/// 動くオブジェクト
class Dynamic : public Object {
protected:
	Vector2 prePosition_; //1F前の座標
	Vector2 radius_; //半径
	Vertex vertex_; //矩形としての4点
	Vector2 verocity_; //速度
	Vector2 acceleration_; //加速度
	bool collisionDirect_[4]; //接触判定

public:
	//1F前の座標を更新(接触判定に使う)(必ず移動処理の前に)
	void SetPrePosition();
	//矩形として4点の座標を取得
	void SetVertex();
	//接触判定更新
	void SetCollisionFlag(Direct direct);
	//接触判定リセット(必ず最後に)
	void ResetCollision();

	///1F前の座標(アクセッサ)
	Vector2 GetPrePosition();
	///半径(アクセッサ)
	Vector2 GetRadius();

};

/// 爆発物
class Weapon : public Dynamic {
private:
	bool isBodyExist_; //本体出現判定
	int detonationCount_; //起爆カウント
	Blast* weaponBlast_; //爆風

public:
	Weapon();

	//攻撃の初期化処理
	void ResetWeapon();
	//攻撃の発生処理
	void SetThrowWeapon(const Vector2& weaponPosition, const PlayerDirect& weaponDirect);
	//攻撃の移動処理
	void SetMoveWeapon();
	//攻撃の加速度更新(ぶつかった時)
	void SetCollisionVerocity(const Direct& direct);

	///本体出現判定(アクセッサ)
	bool GetBodyExist();
	///攻撃の爆風(アクセッサ)
	Blast* GetWeaponBlast();

	~Weapon();

};

/// プレイヤー
class Character : public Dynamic {
private:
	Vector2 additionalVerocity_; //入力時加速度
	PlayerDirect direct_; //方向
	bool isJump_[2]; //ジャンプ判定
	bool isThrough_; //すり抜け判定
	Weapon* playerWeapon_; //プレイヤーの攻撃

public:
	Character();

	//プレイヤーの攻撃処理
	void SetAttack(const char inputKey[], const char preInputKey[]);
	//プレイヤーの移動処理
	void SetMove(const char inputKey[], const char preInputKey[]);
	//プレイヤーのジャンプのリセット
	void ResetJumpFlag(Direct direct);
	//プレイヤーの初期化
	void ResetPlayer(float setX, float setY);

	///プレイヤーの攻撃(アクセッサ)
	Weapon* GetWeapon();
	bool GetThrough();
	void PrintCollision();

	~Character();

};

/// エネミー
class Enemy : public Object {
private:
	bool isExist_; //存在フラグ
	float enemyRadius_; //半径
	Vector2 verocity_; //速度
	float enemyAngle_; //エネミーの動く方向
	float enemyToPlayerLength_; //エネミーとプレイヤーの距離
	int freeMoveCount_; //エネミーの方向転換カウント
	Blast* enemyBlast_; //爆風

public:
	Enemy();

	//エネミーの設置処理
	void SetEnemy(const float& setX, const float& setY, const float& setRadius);
	//エネミーの削除処理
	void DefeatEnemy();
	//エネミーの移動処理
	void MoveEnemy(Character* getPlayer);

	//エネミーの存在フラグ(アクセッサ)
	bool GetExist();
	//エネミーの存在半径(アクセッサ)
	float GetEnemyRadius();
	//エネミーの爆風(アクセッサ)
	Blast* GetEnemyBlast();

	~Enemy();

};

/// マップ
class Map : public Object {
private:
	int mapChip_; //マップ種類
	Vector2 mapChipPosition_; //マップチップとしての座標

public:
	Map(int x, int y);
	int GetMapChip();
	void SetMapChip(int map);

};

/// <summary>
/// 接触処理(爆風->エネミー)
/// </summary>
/// <param name="a">爆風オブジェクト</param>
/// <param name="b">エネミーオブジェクト</param>
void SetBlastToEnemyCollision(Blast* a, Enemy* b);

/// <summary>
/// 接触処理(爆風->壊せるブロック)
/// </summary>
/// <param name="a">爆風オブジェクト</param>
/// <param name="b">マップオブジェクト</param>
void SetBlastToFragileCollision(Blast* a, Map* b);

/// <summary>
/// 接触処理(ウェポン->ブロック)
/// </summary>
/// <param name="a">ウェポンオブジェクト</param>
/// <param name="b">マップオブジェクト</param>
void SetWeaponToMapCollision(Weapon* a, Map* b);

/// <summary>
/// 接触処理(プレイヤー->ブロック)
/// </summary>
/// <param name="a">プレイヤーオブジェクト</param>
/// <param name="b">静的オブジェクト</param>
void SetPlayerToMapCollision(Character* a, Map* b, bool& goolFrag);

/// <summary>
/// スクロール処理
/// </summary>
/// <param name="a">動的オブジェクト</param>
/// <param name="scrollPoint">スクロール地点</param>
/// <param name="world">ワールド座標</param>
/// <param name="local">ローカル座標</param>
/// <param name="scroll">スクロール座標</param>
void SetScroll(Character* a, Vector2 scrollPoint, Vector2& world, Vector2& local, Vector2& scroll);

/// <summary>
/// クリア処理
/// </summary>
/// <param name="a">エネミーオブジェクト</param>
/// <param name="b">マップオブジェクト</param>
/// <returns>
/// エネミーがいない且つ壊れるブロックが無いとtrue
/// </returns>
bool SetClearCount(Enemy* a[kenemyNum], Map* b[kblockQuantityY][kblockQuantityX]);