#include <queue>
#include <vector>

class Test {
public:
	bool PutTestData(int Data);
	bool PutReliableData(int Data);

private:
	std::priority_queue<int, std::vector<int>, std::greater<int>> MyQueue;
	std::priority_queue<int, std::vector<int>, std::greater<int>> ReliableQueue;
};