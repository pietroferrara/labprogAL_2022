#include <iostream>
#include <cmath>

using namespace std;

struct expression_not_supported
{
};

bool isNotADigit(char a)
{
    return a < '0' || a > '9';
}

int extractNumber(string str)
{
    int result = 0;
    /*Implementation left to right
    for (int i = 0; i < str.size(); i++)
    {
        result = result * 10 + (str.at(i) - '0');
    }*/
    /*Implementation right to left
    for (int i = 0; i < str.size(); i++)
    {
        int index = str.size() - i - 1;
        result = result + (str.at(index) - '0') * std::pow(10, i);
    }
    */
    result = stoi(str);
    return result;
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

int performOperation(char op, int left, int right)
{
    switch (op)
    {
    case '+':
        return left + right;
    case '-':
        return left - right;
    case '*':
        return left * right;
    case '/':
        return left / right;
    default:
        throw expression_not_supported();
    }
}

int compute_result_of_expression(string input)
{
    string left = input.substr(0, indexAfterNumber(input));
    char op = input.at(indexAfterNumber(input));
    string right = input.substr(
        indexAfterNumber(input) + 1,
        input.size() - (indexAfterNumber(input) + 1));
    int leftNumber = extractNumber(left),
        rightNumber = extractNumber(right);
    return performOperation(op, leftNumber, rightNumber);
}

int main()
{
    string input;
    bool correct_expression = false;
    int result;
    while (!correct_expression)
    {
        cout << "Please type an expression:" << endl;
        cin >> input;
        try
        {
            result = compute_result_of_expression(input);
            correct_expression = true;
        }
        catch (expression_not_supported)
        {
            cout << "The expression you typed in is not correct, please try again" << endl;
        }
    }
    cout << "The result is " << result << endl;
}