#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

// this types is used to track the type of data contained in the different columns
enum data_types
{
    date,
    number,
    string_type
};

// various structs to represent various types of errors

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

// This class represents a date composed by day, month and year
class Date
{
public:
    Date() {}

    // Build up an instance of class Date by parsing the given string
    // The expected format is YYYY-MM-DD
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

    string format_string()
    {
        return to_string(year) + "-" + to_string(month) + "-" + to_string(day);
    }

private:
    // Store the information about the date
    int day, month, year;
};

// This class represents a tabular structure of data
// Columns can store data of type integer, dates, or generic strings
class Table
{
public:
    // Builds up a table from a given csv file
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

    // Projects the current on the given columns
    // It does not change the current table, and it returns the projected table
    Table project(vector<string> projected_columns)
    {
        vector<string> column_names = projected_columns;
        vector<data_types> column_types = project_column_types(projected_columns);
        vector<vector<Date>> date_data = project_date_columns(projected_columns);
        vector<vector<int>> number_data = project_number_columns(projected_columns);
        vector<vector<string>> string_data = project_string_columns(projected_columns);
        return Table{column_names, column_types, date_data, number_data, string_data};
    }

    // Writes the current table on the given file
    void write_to_file(string file_name)
    {
        ofstream output{file_name};
        if (!output.good())
            throw file_error{"The output file " + file_name + " is not writable"};
        for (int i = 0; i < this->column_names.size(); i++)
        {
            output << this->column_names.at(i);
            if (i != this->column_names.size() - 1)
                output << ",";
            else
                output << "\n";
        }
        int row_numbers = this->get_row_numbers();
        for (int row_index = 0; row_index < row_numbers; row_index++)
        {
            for (int column_index = 0; column_index < this->column_names.size(); column_index++)
            {
                switch (this->column_types.at(column_index))
                {
                case date:
                    output << this->date_data.at(this->get_index_of_date_column(this->column_names.at(column_index))).at(row_index).format_string();
                    break;
                case number:
                    output << this->number_data.at(this->get_index_of_number_column(this->column_names.at(column_index))).at(row_index);
                    break;
                case string_type:
                    output << this->string_data.at(this->get_index_of_string_column(this->column_names.at(column_index))).at(row_index);
                    break;
                }
                // Write the data of the given cell
                if (column_index != this->column_names.size() - 1)
                    output << ",";
            }
            output << "\n";
        }

        if (!output.good())
            throw file_error{"Impossible to write data on file " + file_name};
        output.close();
        if (output.fail())
            throw file_error{"Impossible to close file " + file_name};
    }

    // This method expects that only one column with string values and all the others must be numbers
    Table summarize()
    {
        if (this->string_data.size() != 1)
            throw query_error{"Cannot summarize a table with " + to_string(this->string_data.size()) + " columns with string values"};
        if (this->date_data.size() != 0)
            throw query_error{"Cannot summarize a table with date values"};
        vector<string> column_names = this->column_names;
        vector<data_types> column_types = this->column_types;
        vector<vector<Date>> date_data{0};
        vector<vector<int>> number_data{this->number_data.size()};
        vector<vector<string>> string_data{1};

        for (int string_key = 0; string_key < this->string_data.at(0).size(); string_key++)
        {
            string string_identifier = this->string_data.at(0).at(string_key);
            auto lookup = std::find(string_data.at(0).begin(), string_data.at(0).end(), string_identifier);
            if (lookup != string_data.at(0).end())
            {
                int index = lookup - string_data.at(0).begin();
                for (int j = 0; j < number_data.size(); j++)
                    number_data.at(j).at(index) += this->number_data.at(j).at(string_key);
            }
            else
            {
                string_data.at(0).push_back(string_identifier);
                for (int j = 0; j < number_data.size(); j++)
                    number_data.at(j).push_back(this->number_data.at(j).at(string_key));
            }
        }
        return Table{column_names, column_types, date_data, number_data, string_data};
    }

private:
    // Contains the names of the table columns
    vector<string> column_names;
    // Contains the types of the table columns
    vector<data_types> column_types;
    // Contains the data of the columns of type date
    vector<vector<Date>> date_data;
    // Contains the data of the columns of type number
    vector<vector<int>> number_data;
    // Contains the data of the columns of type string
    vector<vector<string>> string_data;

    // Build up the table from the given fields
    Table(vector<string> cn, vector<data_types> ct, vector<vector<Date>> dd, vector<vector<int>> nd, vector<vector<string>> sd)
    {
        this->column_names = cn;
        this->column_types = ct;
        this->date_data = dd;
        this->number_data = nd;
        this->string_data = sd;
    }

    // Projects the date columns on the given column names
    vector<vector<Date>> project_date_columns(vector<string> column_names)
    {
        vector<vector<Date>> result;
        for (string name : column_names)
            if (column_types.at(get_index_of_column_name(name)) == date)
                result.push_back(this->date_data.at(get_index_of_date_column(name)));
        return result;
    }

    // Projects the number columns on the given column names
    vector<vector<int>> project_number_columns(vector<string> column_names)
    {
        vector<vector<int>> result;
        for (string name : column_names)
            if (column_types.at(get_index_of_column_name(name)) == number)
                result.push_back(this->number_data.at(get_index_of_number_column(name)));
        return result;
    }

    // Projects the string columns on the given column names
    vector<vector<string>> project_string_columns(vector<string> column_names)
    {
        vector<vector<string>> result;
        for (string name : column_names)
            if (column_types.at(get_index_of_column_name(name)) == string_type)
                result.push_back(this->string_data.at(get_index_of_string_column(name)));
        return result;
    }

    // Projects the types columns on the given column names
    vector<data_types> project_column_types(vector<string> column_names)
    {
        vector<data_types> result;
        for (string name : column_names)
        {
            result.push_back(column_types.at(get_index_of_column_name(name)));
        }
        return result;
    }

    // Get the index of the given date column inside the fields containing date values
    int get_index_of_date_column(string name)
    {
        return get_index_of_type_column(name, date);
    }

    // Get the index of the given number column inside the fields containing number values
    int get_index_of_number_column(string name)
    {
        return get_index_of_type_column(name, number);
    }

    // Get the index of the given string column inside the fields containing string values
    int get_index_of_string_column(string name)
    {
        return get_index_of_type_column(name, string_type);
    }

    // Get the index of the given type column inside the fields containing the given types of values
    int get_index_of_type_column(string name, data_types typ)
    {
        int index = get_index_of_column_name(name);
        int result = 0;
        for (int i = 0; i < index; i++)
            if (this->column_types.at(i) == typ)
                result++;
        return result;
    }

    // Returns the index in the initial table of the given column
    int get_index_of_column_name(string name)
    {
        for (int i = 0; i < this->column_names.size(); i++)
            if (name == this->column_names.at(i))
                return i;
        throw query_error{"The column " + name + " does not exist"};
    }

    // Transforms the given vector of string data in dates
    vector<Date> to_date(vector<string> &data)
    {
        vector<Date> result;
        for (int i = 0; i < data.size(); i++)
            result.push_back(Date{data.at(i)});
        return result;
    }

    // Transforms the given vector of string data in number
    vector<int> to_number(vector<string> &data)
    {
        vector<int> result;
        for (int i = 0; i < data.size(); i++)
            result.push_back(stoi(data.at(i)));
        return result;
    }

    // Extract all the names of columns from the given string
    // It expects that the names are separated by a comma ',' character
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

    // Extract all the values of rows from the given string
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

    // Returns the type of the given data (number, date, or just string values)
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
            return date; // date!
        else if (integer_counter == values.size())
            return number; // integer!
        else if (string_counter == values.size())
            return string_type;
        else
            throw format_error{"Different types of values in the given column"};
    }

    // Return true if the given string represents a date value
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

    // Return true if the given string represents an integer value
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

    // Return the number of rows of the current table
    int get_row_numbers()
    {
        if (this->date_data.size() > 0)
            return this->date_data.at(0).size();
        else if (this->number_data.size() > 0)
            return this->number_data.at(0).size();
        else if (this->string_data.size() > 0)
            return this->string_data.at(0).size();
        else
            return 0;
    }
};

int main()
{
    Table t{"somministrazioni-vaccini-summary-latest.csv"};
    vector<string> project_columns;
    project_columns.push_back("nome_area");
    project_columns.push_back("totale");
    project_columns.push_back("prima_dose");
    project_columns.push_back("seconda_dose");
    project_columns.push_back("dose_addizionale_booster");
    Table result = t.project(project_columns);
    result = result.summarize();
    result.write_to_file("projected-table.csv");
    return 0;
}