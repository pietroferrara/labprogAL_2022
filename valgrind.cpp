#include <iostream>

using namespace std;

int main()
{
    int *a = new int[10];
    for (int i = 0; i < 10; i++)
        a[i] = 1;
    delete[] a;
    cout << a[5] << endl;
}
