#include <iostream>

using namespace std;

int main()
{
    int *a = new int[10];
    cout << a[-1] << endl;
    cout << a[10] << endl;
    delete[] a;
}
