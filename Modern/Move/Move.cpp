#include <iostream>
#include <unordered_map>
#include <utility>

struct MyMap {
    int a = 0;
    MyMap() = default;

    //// 이동 생성자
    MyMap(MyMap&& other) noexcept {
        std::exchange(a, other.a);
        std::cout << "Move Constructor Called\n";
    }

    // 복사 생성자
    MyMap(const MyMap& other) {
        a = other.a;
        std::cout << "Copy Constructor Called\n";
    }
};

int main() {
    std::unordered_map<int, MyMap> dualMap;
    MyMap map;

    map.a = 1;

    // std::move 사용
    dualMap.emplace(1, std::move(map));  // "Move Constructor Called" 출력

    // std::move 미사용
    MyMap map2;
    map2.a = 2;
    dualMap.emplace(2, map2);  // "Copy Constructor Called" 출력

    std::unordered_map<int, std::vector<int>> heyMap;
    auto& hey = heyMap[3];

    hey.push_back(4);
    hey.push_back(5);

    for (auto heyRow : heyMap)
    {
        for (int h : heyRow.second)
        {
            printf("%d\n", h);
        }
    }

    return 0;
}