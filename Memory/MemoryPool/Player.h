class Player {
public:
	Player();
	virtual ~Player();

	void OnlyOneAdd();
	void MakeName(const char* Name);
	void Move(int X, int Y);
	void NotifyPos();

private:
	unsigned char _Fly;
	int _X;
	int _Y;
	char _Name[30];
};