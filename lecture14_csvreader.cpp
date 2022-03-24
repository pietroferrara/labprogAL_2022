#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

int main()
{
    int value;
    cin >> value;
    cout << value << endl;
    if (cin.good())
        cout << "cin is good" << endl;
    if (cin.eof())
        cout << "cin is at the end" << endl;
    if (cin.fail())
        cout << "cin failed" << endl;
    if (cin.bad())
        cout << "cin is broken" << endl;
    cout << "End of everything" << endl;
}