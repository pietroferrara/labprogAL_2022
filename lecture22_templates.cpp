#include <string>
#include <iostream>

using namespace std;

class Pippo
{
};

template <typename T>
class Wrapper
{
public:
    Wrapper(T v)
    {
        this->value = v;
    }

    T get()
    {
        return this->value;
    }

    void set(T v)
    {
        this->value = v;
    }

    Wrapper<T> operator+(const Wrapper<T> &value) const
    {
        Wrapper<T> result = Wrapper<T>{this->value + value.value};
        return result;
    }

private:
    T value;
};

template <typename T1>
T1 increment(T1 value)
{
    return value + 1.0;
}

int main()
{
    Wrapper<int> int_wrapper{10}, int_wrapper2{20};
    Wrapper<int> result = int_wrapper + int_wrapper2;
    cout << result.get() << endl;
    Pippo pippo{}, pippo2{};

    Wrapper<Pippo> pippo_wrapper{pippo}, pippo2_wrapper{pippo2};
    // Wrapper<Pippo> pipporesult = pippo_wrapper + pippo2_wrapper;
    cout << "I'm fine" << endl;

    int i = 10;
    i = increment(i);
    cout << i << endl;

    string s = "pippo";
    s = increment(s);
    cout << s << endl;
}