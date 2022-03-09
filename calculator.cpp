#include <iostream>

using namespace std;

bool isNotADigit(char a)
{
    return a < '0' || a > '9';
}

int extractNumber(string str)
{
}

int indexAfterNumber(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (isNotADigit(str.at(i)))
            return i;
    }
    return str.length();
}

int main()
{
    string input;
    cout << "Please type an expression:" << endl;
    cin >> input;
    cout << "First index not a digit:" << indexAfterNumber(input);
    cout << "The operator is:" << input.at(indexAfterNumber(input));
}