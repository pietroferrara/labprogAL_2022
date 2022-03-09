#include <iostream>

// This method returns the square root of x
// If x is <= 0, it returns -1
int square_root(double x, double e)
{
    if (x <= 0)
        return -1;
    double g = 1;
    while (std::abs(x - g * g) > e)
    {
        g = (g + x / g) / 2;
    }
    return g;
}

int main(int argc, char *argv[])
{
    int i1;
    std::cin >> i1;
    std::cout << square_root(i1, 0.01);
    return 0;
}