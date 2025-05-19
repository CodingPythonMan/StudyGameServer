#include <iostream>
#include <string>
#include <utility>

void Overload(std::string& s) {
    std::cout << "LValue overload called: " << s << std::endl;
}

void Overload(std::string&& s) {
    std::cout << "RValue overload called: " << s << std::endl;
}

// forward ���� ����
template<typename T>
void NoForwardWrapper(T&& arg) {
    std::cout << "[NoForwardWrapper] ";
    Overload(arg); // �׻� lvalue�� ó����
}

// forward ���
template<typename T>
void ForwardWrapper(T&& arg) {
    std::cout << "[ForwardWrapper] ";
    Overload(std::forward<T>(arg)); // ��Ȯ�� value category ����
}

int main() {
    std::string name = "KimJuYoung";

    // lvalue ����
    NoForwardWrapper(name);
    ForwardWrapper(name);

    std::cout << "------" << std::endl;

    // rvalue ����
    //NoForwardWrapper(std::string("Temp"));
    NoForwardWrapper("KimJuYoung");
    ForwardWrapper(std::string("Temp"));
}