#include <iostream>
#include <cmath>

using std::cin;

struct wrong_expression
{
};

int S();

int T()
{
    char c;
    cin >> c;
    if (c != '(')
        throw wrong_expression();
    int result = S();
    cin >> c;
    if (c != ')')
        throw wrong_expression();
    return result + 1;
}

int S()
{
    char c;
    cin >> c;
    cin.putback(c);
    if (c == '(')
    {
        int h1 = T();
        int h2 = S();
        if (h1 > h2)
            return h1;
        else
            return h2;
    }
    else
        return 0;
}

int main()
{
    std::cout << "Type a BPS expression" << std::endl;
    int height = T() - 1;
    std::cout << "The height of the given expression is " << height;
}