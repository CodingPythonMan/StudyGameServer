#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int main()
{
    const int N = 1000000;

    auto start = chrono::high_resolution_clock::now();

    vector<int*> v;
    for (int i = 0; i < N; i++)
    {
        v.push_back(new int(i));
    }

    for (int i = 0; i < N; i++)
    {
        delete v[i];
    }

    auto end = chrono::high_resolution_clock::now();

    cout << "Time: "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " ms\n";
}