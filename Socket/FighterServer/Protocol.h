// ��Ŷ ������ ����
enum class PacketType : int {
	FIGHTER_REP_CREATE_MY_CHARACTER = 0,
	FIGHTER_CMD_CREATE_OTHER_CHARACTER,
	FIGHTER_CMD_DELETE_CHARACTER,
	
	FIGHTER_QRY_MOVE_START = 10,
	FIGHTER_REP_MOVE_START,
	FIGHTER_QRY_MOVE_STOP,
	FIGHTER_REP_MOVE_STOP,

	FIGHTER_QRY_ATTACK_001 = 20,
	FIGHTER_REP_ATTACK_001,
	FIGHTER_QRY_ATTACK_002,
	FIGHTER_REP_ATTACK_002,
	FIGHTER_QRY_ATTACK_003,
	FIGHTER_REP_ATTACK_003,
	FIGHTER_CMD_DAMAGE,

	FIGHTER_QRY_SYNC = 250,
	FIGHTER_REP_SYNC,

};

enum class MoveType : char{
	MOVE_DIR_LL = 0,
	MOVE_DIR_LU,
	MOVE_DIR_UU,
	MOVE_DIR_RU,
	MOVE_DIR_RR,
	MOVE_DIR_RD,
	MOVE_DIR_DD,
	MOVE_DIR_LD
};

enum class Direction : char {
	LL = 0,
	RR
};

struct PACKET_HEADER {
	char ByCode;
	char BySize;
	char ByType;
};

//---------------------------------------------------------------
// Ŭ���̾�Ʈ �ڽ��� ĳ���� �Ҵ�		Server -> Client
//
// ������ ���ӽ� ���ʷ� �ްԵǴ� ��Ŷ���� �ڽ��� �Ҵ���� ID ��
// �ڽ��� ���� ��ġ, HP �� �ް� �ȴ�. (ó���� �ѹ� �ް� ��)
// 
// �� ��Ŷ�� ������ �ڽ��� ID,X,Y,HP �� �����ϰ� ĳ���͸� �������Ѿ� �Ѵ�.
//
//	4	-	ID
//	1	-	Direction	(LL / RR)
//	2	-	X
//	2	-	Y
//	1	-	HP
//
//---------------------------------------------------------------
struct FIGHTER_REP_CREATE_MY_CHARACTER {
	int ID;
	Direction Direct;
	short X;
	short Y;
	char HP;
};

//---------------------------------------------------------------
// �ٸ� Ŭ���̾�Ʈ�� ĳ���� ���� ��Ŷ		Server -> Client
//
// ó�� ������ ���ӽ� �̹� ���ӵǾ� �ִ� ĳ���͵��� ����
// �Ǵ� �����߿� ���ӵ� Ŭ���̾�Ʈ���� ������ ����.
//
//
//	4	-	ID
//	1	-	Direction	(LL / RR)
//	2	-	X
//	2	-	Y
//	1	-	HP
//
//---------------------------------------------------------------
struct FIGHTER_CMD_CREATE_OTHER_CHARACTER {
	int ID;
	Direction Direct;
	short X;
	short Y;
	char HP;
};

//---------------------------------------------------------------
// ĳ���� ���� ��Ŷ						Server -> Client
//
// ĳ������ �������� �Ǵ� ĳ���Ͱ� �׾����� ���۵�.
//
//	4	-	ID
//
//---------------------------------------------------------------
struct FIGHTER_CMD_DELETE_CHARACTER {
	int ID;
};

//---------------------------------------------------------------
// ĳ���� �̵����� ��Ŷ						Client -> Server
//
// �ڽ��� ĳ���� �̵����۽� �� ��Ŷ�� ������.
// �̵� �߿��� �� ��Ŷ�� ������ ������, Ű �Է��� ����Ǿ��� ��쿡��
// ������� �Ѵ�.
//
// (���� �̵��� ���� �̵� / ���� �̵��� ���� ���� �̵�... ���)
//
//	1	-	Direction	( ���� ������ �� 8���� ��� )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_QRY_MOVE_START {
	MoveType Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// ĳ���� �̵����� ��Ŷ						Server -> Client
//
// �ٸ� ������ ĳ���� �̵��� �� ��Ŷ�� �޴´�.
// ��Ŷ ���Ž� �ش� ĳ���͸� ã�� �̵�ó���� ���ֵ��� �Ѵ�.
// 
// ��Ŷ ���� �� �ش� Ű�� ����ؼ� ���������� �����ϰ�
// �ش� �������� ��� �̵��� �ϰ� �־�߸� �Ѵ�.
//
//	4	-	ID
//	1	-	Direction	( ���� ������ �� 8���� )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_MOVE_START {
	int ID;
	MoveType Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// ĳ���� �̵����� ��Ŷ						Client -> Server
//
// �̵��� Ű���� �Է��� ��� �����Ǿ��� ��, �� ��Ŷ�� ������ �����ش�.
// �̵��� ���� ��ȯ�ÿ��� ��ž�� ������ �ʴ´�.
//
//	1	-	Direction	( ���� ������ �� ��/�츸 ��� )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_QRY_MOVE_STOP {
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// ĳ���� �̵����� ��Ŷ						Server -> Client
//
// ID �� �ش��ϴ� ĳ���Ͱ� �̵��� ������̹Ƿ� 
// ĳ���͸� ã�Ƽ� �����, ��ǥ�� �Է����ְ� ���ߵ��� ó���Ѵ�.
//
//	4	-	ID
//	1	-	Direction	( ���� ������ ��. ��/�츸 ��� )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_MOVE_STOP {
	int ID;
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// ĳ���� ���� ��Ŷ							Client -> Server
//
// ���� Ű �Է½� �� ��Ŷ�� �������� ������.
// �浹 �� �������� ���� ����� �������� �˷� �� ���̴�.
//
// ���� ���� ���۽� �ѹ��� �������� ������� �Ѵ�.
//
//	1	-	Direction	( ���� ������ ��. ��/�츸 ��� )
//	2	-	X
//	2	-	Y	
//
//---------------------------------------------------------------
struct FIGHTER_QRY_ATTACK_001 {
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// ĳ���� ���� ��Ŷ							Server -> Client
//
// ��Ŷ ���Ž� �ش� ĳ���͸� ã�Ƽ� ����1�� �������� �׼��� �����ش�.
// ������ �ٸ� ��쿡�� �ش� �������� �ٲ� �� ���ش�.
//
//	4	-	ID
//	1	-	Direction	( ���� ������ ��. ��/�츸 ��� )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_ATTACK_001 {
	int ID;
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// ĳ���� ���� ��Ŷ							Client -> Server
//
// ���� Ű �Է½� �� ��Ŷ�� �������� ������.
// �浹 �� �������� ���� ����� �������� �˷� �� ���̴�.
//
// ���� ���� ���۽� �ѹ��� �������� ������� �Ѵ�.
//
//	1	-	Direction	( ���� ������ ��. ��/�츸 ��� )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_QRY_ATTACK_002 {
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// ĳ���� ���� ��Ŷ							Server -> Client
//
// ��Ŷ ���Ž� �ش� ĳ���͸� ã�Ƽ� ����2�� �������� �׼��� �����ش�.
// ������ �ٸ� ��쿡�� �ش� �������� �ٲ� �� ���ش�.
//
//	4	-	ID
//	1	-	Direction	( ���� ������ ��. ��/�츸 ��� )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_ATTACK_002 {
	int ID;
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// ĳ���� ���� ��Ŷ							Client -> Server
//
// ���� Ű �Է½� �� ��Ŷ�� �������� ������.
// �浹 �� �������� ���� ����� �������� �˷� �� ���̴�.
//
// ���� ���� ���۽� �ѹ��� �������� ������� �Ѵ�.
//
//	1	-	Direction	( ���� ������ ��. ��/�츸 ��� )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_QRY_ATTACK_003 {
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// ĳ���� ���� ��Ŷ							Server -> Client
//
// ��Ŷ ���Ž� �ش� ĳ���͸� ã�Ƽ� ����3�� �������� �׼��� �����ش�.
// ������ �ٸ� ��쿡�� �ش� �������� �ٲ� �� ���ش�.
//
//	4	-	ID
//	1	-	Direction	( ���� ������ ��. ��/�츸 ��� )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_ATTACK_003 {
	int ID;
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// ĳ���� ������ ��Ŷ							Server -> Client
//
// ���ݿ� ���� ĳ������ ������ ����.
//
//	4	-	AttackID	( ������ ID )
//	4	-	DamageID	( ������ ID )
//	1	-	DamageHP	( ������ HP )
//
//---------------------------------------------------------------
struct FIGHTER_CMD_DAMAGE {
	int AttackID;
	int DamageID;
	char DamageHP;
};


// ������...
//---------------------------------------------------------------
// ����ȭ�� ���� ��Ŷ					Client -> Server
//
//
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_QRY_SYNC {
	short X;
	short Y;
};

//---------------------------------------------------------------
// ����ȭ�� ���� ��Ŷ					Server -> Client
//
// �����κ��� ����ȭ ��Ŷ�� ������ �ش� ĳ���͸� ã�Ƽ�
// ĳ���� ��ǥ�� �������ش�.
//
//	4	-	ID
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_SYNC {
	int ID;
	short X;
	short Y;
};