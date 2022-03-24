#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

struct expression_not_supported
{
    string message;
};

struct expression_malformed
{
    string message;
};

/* Deprecated code
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
}*/

struct Token
{
    bool isNumber;
    double value;
};

vector<Token> tok;

double performOperation(char op, double left, double right)
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
        throw expression_not_supported{"Operator " + std::to_string(op) + " is not supported"};
    }
}

void tokenize(istream &input)
{
    double number = 0;
    char op = 0;
    cin >> op;
    while (op != '=')
    {
        if (op >= '0' && op <= '9')
        {
            cin.putback(op);
            cin >> number;
            tok.push_back(Token{true, number});
        }
        else
            tok.push_back(Token{false, (double)op});
        cin >> op;
    }
}
/*char op;
double number;
tok = vector<Token>(0);
cin >> number >> op;
while (op != '=')
{
    tok.push_back(Token{true, number});
    tok.push_back(Token{false, (double)op});
    cin >> number >> op;
}
tok.push_back(Token{true, number});
}*/

Token get_token()
{
    Token result = tok.at(0);
    tok.erase(tok.begin());
    return result;
}

void put_token_back(Token t)
{
    tok.insert(tok.begin(), t);
}

double term();
double primary();

double expression()
{
    double left = term();
    if (tok.size() == 0)
        return left;
    Token t = get_token();
    if (t.isNumber)
        throw expression_malformed{"Found number " + std::to_string(t.value) + " when instead an operator was expected"};
    double right;
    while (true)
    {
        switch ((char)t.value)
        {
        case '+':
            right = term();
            left = left + right;
            if (tok.empty())
                return left;
            t = get_token();
            break;
        case '-':
            right = term();
            left = left - right;
            if (tok.empty())
                return left;
            t = get_token();
            break;
        default:
            put_token_back(t);
            return left;
        }
    }
}

double term()
{
    double left = primary();
    if (tok.size() == 0)
        return left;
    Token t = get_token();
    if (t.isNumber)
        throw expression_malformed{"Found number " + std::to_string(t.value) + " when instead an operator was expected"};
    double right;
    while (true)
    {
        switch ((char)t.value)
        {
        case '*':
            right = primary();
            left = left * right;
            if (tok.empty())
                return left;
            t = get_token();
            break;
        case '/':
            right = primary();
            left = left / right;
            if (tok.empty())
                return left;
            t = get_token();
            break;
        default:
            put_token_back(t);
            return left;
        }
    }
}

double primary()
{
    Token t = get_token();
    if (!t.isNumber)
    {
        if ((char)t.value != '(')
            throw expression_malformed{"Found operator " + std::to_string((char)t.value) + " when instead a number or an open paranthesis was expected"};
        double value = expression();
        if (tok.size() == 0)
            throw expression_malformed{"Missing a close parenthesis"};
        t = get_token();
        if (t.isNumber || (char)t.value != ')')
            throw expression_malformed{"Found number or other operator when instead a close parenthesis was expected"};
        return value;
    }
    else
        return t.value;
}

double compute_result_of_expression()
{
    return expression();
}

int main()
{
    string input;
    bool correct_expression = false;
    double result;
    while (!correct_expression)
    {
        cout << "Please type an expression:" << endl;
        try
        {
            tokenize(cin);
            result = compute_result_of_expression();
            correct_expression = true;
        }
        catch (expression_not_supported e)
        {
            cout << "You typed an operator that is not supported, please try again" << endl;
            cout << "Detailed error: " << e.message << endl;
        }
        catch (expression_malformed e)
        {
            cout << "The expression you typed in is not correct, please try again" << endl;
            cout << "Detailed error: " << e.message << endl;
        }
    }
    cout << "The result is " << result << endl;
}