#include <iostream>
#include <string>
#include <utility>

void Overload(std::string& s) {
    std::cout << "LValue overload called: " << s << std::endl;
}

void Overload(std::string&& s) {
    std::cout << "RValue overload called: " << s << std::endl;
}

// forward 없이 전달
template<typename T>
void NoForwardWrapper(T&& arg) {
    std::cout << "[NoForwardWrapper] ";
    Overload(arg); // 항상 lvalue로 처리됨
}

// forward 사용
template<typename T>
void ForwardWrapper(T&& arg) {
    std::cout << "[ForwardWrapper] ";
    Overload(std::forward<T>(arg)); // 정확한 value category 유지
}

int main() {
    std::string name = "KimJuYoung";

    // lvalue 전달
    NoForwardWrapper(name);
    ForwardWrapper(name);

    std::cout << "------" << std::endl;

    // rvalue 전달
    //NoForwardWrapper(std::string("Temp"));
    NoForwardWrapper("KimJuYoung");
    ForwardWrapper(std::string("Temp"));
}