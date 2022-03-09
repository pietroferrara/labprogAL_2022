#include <iostream>
#include <vector>

using namespace std;

int &foo()
{

    int b;
    return b;
}

int main()
{
    std::cout << foo();
}