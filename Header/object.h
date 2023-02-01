#pragma once

const int kscreenWidth = 1200; //�X�N���[������
const int kscreenHeight = 720; //�X�N���[���c��
const int kworldWidth = 1600; //���[���h����
const int kworldHeight = 800; //���[���h�c��
const int kblockSizeX = 32; //�u���b�N����
const int kblockSizeY = 32; //�u���b�N�c��
const int kblockQuantityX = (kworldWidth / kblockSizeX); //�u���b�N����
const int kblockQuantityY = (kworldHeight / kblockSizeY); //�u���b�N�c��
const float kgravity = 0.45f; //�d��
const int kenemyNum = 24; //�G�l�~�[�̍ő吔
const float kblastRadius = 128.0f; //�����̔��a

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

/// �x�N�g��(2����)
struct Vector2 {
	float x;
	float y;
};

/// ��`4�_
struct Vertex {
	Vector2 LeftTop;
	Vector2	RightTop;
	Vector2	LeftBottom;
	Vector2	RightBottom;
};

/// ���W�����I�u�W�F�N�g
class Object {
protected:
	Vector2 position_; //���W
	unsigned int color_; //�F

public:
	//���W�̍X�V(X,Y��)
	void SetPosition(const float& setX, const float& setY);
	//���W�̍X�V(Vector2�ꊇ)
	void SetPosition(const Vector2& setPosition);

	///���W(�A�N�Z�b�T)
	Vector2 GetPosition();
	///�F(�A�N�Z�b�T)
	unsigned int GetColor();

};

/// ����
class Blast : public Object {
private:
	bool isBlastDetonation_; //�����o������
	int calmDownCount_; //�N���p���J�E���g
	float blastRadius_; //�������a

public:
	Blast(float setBlastRadius);

	//�����̏���������
	void ResetBlast();
	//�����̔�������
	void SetBlast(const Vector2& setPosition);
	//�����̌p������
	void SetBlastDetonation();

	//�����o������(�A�N�Z�b�T)
	bool GetBlastDetonation();
	//�����p������(�A�N�Z�b�T)
	bool GetAfterBlastDetonation();
	//�������a(�A�N�Z�b�T)
	float GetBlastRadius();

};

/// �����I�u�W�F�N�g
class Dynamic : public Object {
protected:
	Vector2 prePosition_; //1F�O�̍��W
	Vector2 radius_; //���a
	Vertex vertex_; //��`�Ƃ��Ă�4�_
	Vector2 verocity_; //���x
	Vector2 acceleration_; //�����x
	bool collisionDirect_[4]; //�ڐG����

public:
	//1F�O�̍��W���X�V(�ڐG����Ɏg��)(�K���ړ������̑O��)
	void SetPrePosition();
	//��`�Ƃ���4�_�̍��W���擾
	void SetVertex();
	//�ڐG����X�V
	void SetCollisionFlag(Direct direct);
	//�ڐG���胊�Z�b�g(�K���Ō��)
	void ResetCollision();

	///1F�O�̍��W(�A�N�Z�b�T)
	Vector2 GetPrePosition();
	///���a(�A�N�Z�b�T)
	Vector2 GetRadius();

};

/// ������
class Weapon : public Dynamic {
private:
	bool isBodyExist_; //�{�̏o������
	int detonationCount_; //�N���J�E���g
	Blast* weaponBlast_; //����

public:
	Weapon();

	//�U���̏���������
	void ResetWeapon();
	//�U���̔�������
	void SetThrowWeapon(const Vector2& weaponPosition, const PlayerDirect& weaponDirect);
	//�U���̈ړ�����
	void SetMoveWeapon();
	//�U���̉����x�X�V(�Ԃ�������)
	void SetCollisionVerocity(const Direct& direct);

	///�{�̏o������(�A�N�Z�b�T)
	bool GetBodyExist();
	///�U���̔���(�A�N�Z�b�T)
	Blast* GetWeaponBlast();

	~Weapon();

};

/// �v���C���[
class Character : public Dynamic {
private:
	Vector2 additionalVerocity_; //���͎������x
	PlayerDirect direct_; //����
	bool isJump_[2]; //�W�����v����
	bool isThrough_; //���蔲������
	Weapon* playerWeapon_; //�v���C���[�̍U��

public:
	Character();

	//�v���C���[�̍U������
	void SetAttack(const char inputKey[], const char preInputKey[]);
	//�v���C���[�̈ړ�����
	void SetMove(const char inputKey[], const char preInputKey[]);
	//�v���C���[�̃W�����v�̃��Z�b�g
	void ResetJumpFlag(Direct direct);
	//�v���C���[�̏�����
	void ResetPlayer(float setX, float setY);

	///�v���C���[�̍U��(�A�N�Z�b�T)
	Weapon* GetWeapon();
	bool GetThrough();
	void PrintCollision();

	~Character();

};

/// �G�l�~�[
class Enemy : public Object {
private:
	bool isExist_; //���݃t���O
	float enemyRadius_; //���a
	Vector2 verocity_; //���x
	float enemyAngle_; //�G�l�~�[�̓�������
	float enemyToPlayerLength_; //�G�l�~�[�ƃv���C���[�̋���
	int freeMoveCount_; //�G�l�~�[�̕����]���J�E���g
	Blast* enemyBlast_; //����

public:
	Enemy();

	//�G�l�~�[�̐ݒu����
	void SetEnemy(const float& setX, const float& setY, const float& setRadius);
	//�G�l�~�[�̍폜����
	void DefeatEnemy();
	//�G�l�~�[�̈ړ�����
	void MoveEnemy(Character* getPlayer);

	//�G�l�~�[�̑��݃t���O(�A�N�Z�b�T)
	bool GetExist();
	//�G�l�~�[�̑��ݔ��a(�A�N�Z�b�T)
	float GetEnemyRadius();
	//�G�l�~�[�̔���(�A�N�Z�b�T)
	Blast* GetEnemyBlast();

	~Enemy();

};

/// �}�b�v
class Map : public Object {
private:
	int mapChip_; //�}�b�v���
	Vector2 mapChipPosition_; //�}�b�v�`�b�v�Ƃ��Ă̍��W

public:
	Map(int x, int y);
	int GetMapChip();
	void SetMapChip(int map);

};

/// <summary>
/// �ڐG����(����->�G�l�~�[)
/// </summary>
/// <param name="a">�����I�u�W�F�N�g</param>
/// <param name="b">�G�l�~�[�I�u�W�F�N�g</param>
void SetBlastToEnemyCollision(Blast* a, Enemy* b);

/// <summary>
/// �ڐG����(����->�󂹂�u���b�N)
/// </summary>
/// <param name="a">�����I�u�W�F�N�g</param>
/// <param name="b">�}�b�v�I�u�W�F�N�g</param>
void SetBlastToFragileCollision(Blast* a, Map* b);

/// <summary>
/// �ڐG����(�E�F�|��->�u���b�N)
/// </summary>
/// <param name="a">�E�F�|���I�u�W�F�N�g</param>
/// <param name="b">�}�b�v�I�u�W�F�N�g</param>
void SetWeaponToMapCollision(Weapon* a, Map* b);

/// <summary>
/// �ڐG����(�v���C���[->�u���b�N)
/// </summary>
/// <param name="a">�v���C���[�I�u�W�F�N�g</param>
/// <param name="b">�ÓI�I�u�W�F�N�g</param>
void SetPlayerToMapCollision(Character* a, Map* b, bool& goolFrag);

/// <summary>
/// �X�N���[������
/// </summary>
/// <param name="a">���I�I�u�W�F�N�g</param>
/// <param name="scrollPoint">�X�N���[���n�_</param>
/// <param name="world">���[���h���W</param>
/// <param name="local">���[�J�����W</param>
/// <param name="scroll">�X�N���[�����W</param>
void SetScroll(Character* a, Vector2 scrollPoint, Vector2& world, Vector2& local, Vector2& scroll);

/// <summary>
/// �N���A����
/// </summary>
/// <param name="a">�G�l�~�[�I�u�W�F�N�g</param>
/// <param name="b">�}�b�v�I�u�W�F�N�g</param>
/// <returns>
/// �G�l�~�[�����Ȃ�������u���b�N��������true
/// </returns>
bool SetClearCount(Enemy* a[kenemyNum], Map* b[kblockQuantityY][kblockQuantityX]);