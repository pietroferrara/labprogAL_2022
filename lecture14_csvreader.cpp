#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

int main()
{
    string date{"11/12/2020 today"};
    istringstream iss{date};
    /*string day, month, year;
    getline(iss, day, '/');
    getline(iss, month, '/');
    getline(iss, year, '/');

    int dd = stoi(day);
    int mm = stoi(month);
    int yy = stoi(year);*/

    int dd, mm, yy;
    char sep1, sep2;
    iss >> dd >> sep1 >> mm >> sep2 >> yy;
    if (iss.fail() || !iss.eof())
        cout << "Error";
    else
        cout << "day " << to_string(dd) << " month " << to_string(mm) << " year " << to_string(yy);
}