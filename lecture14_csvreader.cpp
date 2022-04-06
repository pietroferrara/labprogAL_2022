#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

enum data_types
{
    date,
    number,
    string_type
};

struct file_error
{
    string error_message;
};

struct format_error
{
    string error_message;
};

struct query_error
{
    string error_message;
};

struct not_a_date_error
{
};

class Date
{
public:
    Date() {}
    Date(string value)
    {
        istringstream iss{value};
        char sep1, sep2;
        iss >> this->year >> sep1 >> this->month >> sep2 >> this->day;
        if (iss.fail() || !iss.eof())
            throw not_a_date_error{};
        else if (this->year < 2020 || this->year > 2030)
            throw not_a_date_error{};
        else if (this->month < 1 || this->month > 12)
            throw not_a_date_error{};
        else if (this->day < 1 || this->day > 31)
            throw not_a_date_error{};
    }

private:
    int day, month, year;
};

class Table
{
public:
    Table(string file_name)
    {
        ifstream stream{file_name};
        string content;
        if (!stream.good())
            throw file_error{"Error when opening the given file"};
        getline(stream, content);
        extract_column_names(content);
        if (!stream.good())
            throw file_error{"Error when extracting the column names"};
        cout << "Column names extracted" << endl;

        vector<vector<string>> data{this->column_names.size()};
        while (stream.good())
        {
            getline(stream, content);
            if (!content.empty())
                extract_row(content, data);
            if (data.at(0).size() % 1000 == 0)
                cout << "Row " << to_string(data.at(0).size()) << " extracted" << endl;
        }
        if (!stream.eof())
            throw file_error{"We should be at the end of the file, but we are not"};
        else
            cout << "End of the processing of the given file" << endl;

        for (int i = 0; i < data.size(); i++)
            column_types.push_back(get_type(data.at(i)));

        for (int i = 0; i < data.size(); i++)
        {
            switch (column_types.at(i))
            {
            case date:
                date_data.push_back(to_date(data.at(i)));
                break;
            case number:
                number_data.push_back(to_number(data.at(i)));
                break;
            case string_type:
                string_data.push_back(data.at(i));
                break;
            }
        }
    }

    Table project(vector<string> projected_columns)
    {
        vector<string> column_names = projected_columns;
        vector<data_types> column_types = project_column_types(projected_columns);
        vector<vector<Date>> date_data;
        vector<vector<int>> number_data;
        vector<vector<string>> string_data;
        return Table{column_names, column_types, date_data, number_data, string_data};
    }

private:
    vector<string> column_names;
    vector<data_types> column_types;
    vector<vector<Date>> date_data;
    vector<vector<int>> number_data;
    vector<vector<string>> string_data;

    Table(vector<string> cn, vector<data_types> ct, vector<vector<Date>> dd, vector<vector<int>> nd, vector<vector<string>> sd)
    {
        this->column_names = cn;
        this->column_types = ct;
        this->date_data = dd;
        this->number_data = nd;
        this->string_data = sd;
    }

    vector<data_types> project_column_types(vector<string> column_names)
    {
        vector<data_types> result;
        for (string name : column_names)
        {
            result.push_back(column_types.at(get_index_of_column_name(name)));
        }
        return result;
    }

    int get_index_of_column_name(string name)
    {
        for (int i = 0; i < this->column_names.size(); i++)
            if (name == this->column_names.at(i))
                return i;
        throw query_error{"The column " + name + " does not exist"};
    }

    vector<Date> to_date(vector<string> &data)
    {
        vector<Date> result;
        for (int i = 0; i < data.size(); i++)
            result.push_back(Date{data.at(i)});
        return result;
    }

    vector<int> to_number(vector<string> &data)
    {
        vector<int> result;
        for (int i = 0; i < data.size(); i++)
            result.push_back(stoi(data.at(i)));
        return result;
    }

    void extract_column_names(string s)
    {
        istringstream row{s};
        while (row.good())
        {
            string cell;
            getline(row, cell, ',');
            this->column_names.push_back(cell);
        }
    }

    void extract_row(string s, vector<vector<string>> &data)
    {
        istringstream row{s};
        int index = 0;
        while (row.good())
        {
            string cell;
            getline(row, cell, ',');
            if (index < column_names.size())
            {
                data.at(index).push_back(cell);
                index++;
            }
            else
                throw format_error{"This row has too many cells"};
        }
        if (index != column_names.size())
            throw format_error{"This row has too few cells"};
    }

    data_types get_type(vector<string> values)
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
            return date; // data!
        else if (integer_counter == values.size())
            return number; // integer!
        else if (string_counter == values.size())
            return string_type;
        else
            throw format_error{"Different types of values in the given column"};
    }

    bool is_date(string s)
    {
        try
        {
            Date d{s};
            return true;
        }
        catch (not_a_date_error)
        {
            return false;
        }
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
};

int main()
{
    Table t{"somministrazioni-vaccini-summary-latest.csv"};
    vector<string> project_columns;
    project_columns.push_back("totale");
    project_columns.push_back("data_somministrazione");
    t.project(project_columns);
    return 0;
}