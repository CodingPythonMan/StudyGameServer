class Player {
public:
	Player();
	virtual ~Player();

	void MakeName(char* Name);
	void Move(int X, int Y);
	void NotifyPos();

private:
	int _X;
	int _Y;
	char* _Name;
};