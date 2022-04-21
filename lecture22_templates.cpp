#include <string>
#include <iostream>

using namespace std;

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

private:
    T value;
};

int main()
{
    Wrapper<string> string_wrapper{"pippo"};
    cout << string_wrapper.get() << endl;
    Wrapper<int> int_wrapper{10};
    cout << int_wrapper.get() << endl;
    Wrapper<int> int2_wrapper{20};

    float a = 30.5;
    int2_wrapper.set(a);
    Wrapper<float> float_wrapper{20.0};
    int i = 1;
    float_wrapper.set(i);

    float_wrapper = int2_wrapper;

    cout << int_wrapper.get() << endl;
    cout << int2_wrapper.get() << endl;
}