#include <iostream>

using namespace std;

int main()
{
    int *a = new int[10];
    int i;
    for (int j = 0; j < 10; j++)
        a[j] = j;
    cin >> i;
    cout << a[i] << endl;
    delete[] a;
}
