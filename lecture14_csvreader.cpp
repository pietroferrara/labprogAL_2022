#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

vector<string> column_names;
vector<vector<string>> rows_values(0);

bool is_date(string s)
{
    istringstream iss{s};
    int dd, mm, yy;
    char sep1, sep2;
    iss >> yy >> sep1 >> mm >> sep2 >> dd;
    if (iss.fail() || !iss.eof())
        return false;
    else if (yy < 2020 || yy > 2030)
        return false;
    else if (mm < 1 || mm > 12)
        return false;
    else if (dd < 1 || dd > 31)
        return false;
    else
        return true;
}

bool is_integer(string s)
{
    istringstream iss{s};
    int i;
    iss >> i;
    if (iss.fail() || !iss.eof())
        return false;
    else if (i < 0)
        return false;
    else
        return true;
}

vector<string> extract_row(string s)
{
    vector<string> result(0);
    istringstream row{s};
    while (row.good())
    {
        string cell;
        getline(row, cell, ',');
        result.push_back(cell);
    }
    return result;
}

XXX get_type(vector<string> values)
{
    int date_counter = 0, integer_counter = 0, string_counter = 0;
    for (int i = 0; i < values.size(); i++)
    {
        if (is_date(values.at(i)))
            date_counter++;
        else if (is_integer(values.at(i)))
            integer_counter++;
        else
            string_counter++;
    }
    if (date_counter == values.size())
        return                                                // data!
            else if (integer_counter == values.size()) return // integer!
            else if (string_counter == values.size()) return  // string!
            else throw error{"Different types of values in the given column"};
}

int main()
{

    ifstream file{"somministrazioni-vaccini-summary-latest.csv"};
    string content;
    if (!file.good())
    {
        cout << "Error when opening the given file" << endl;
        return 1;
    }
    getline(file, content);
    column_names = extract_row(content);
    if (!file.good())
    {
        cout << "Error when extracting the column names" << endl;
        return 1;
    }
    cout << "Column names extracted" << endl;
    while (file.good())
    {
        getline(file, content);
        if (!content.empty())
            rows_values.push_back(extract_row(content));
        cout << "Row " << to_string(rows_values.size()) << " extracted" << endl;
    }

    if (file.good())
        cout << "good" << endl;
    if (file.fail())
        cout << "fail" << endl;
    if (file.bad())
        cout << "bad" << endl;
    if (file.eof())
        cout << "eof" << endl;
}