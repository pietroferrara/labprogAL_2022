#include <iostream>
#include <vector>
#include <chrono>

constexpr int N = 10000000;
constexpr int M = 1000;

/*
 * creates N random numbers, stores them
 * in a vector, extracts M random numbers from this 
 * vector, and computes their average 
 */
int random_access(){

    std::vector<int> points;
    points.reserve(N);

    srand(time(NULL));

    for(int i=0;i<N;++i)
        points.push_back(rand());


    //pre-compute a set of M random indexes between 0 and N-1
    std::vector<int> I;
    for(int i=0;i<M;++i) I.push_back(rand()%N);

    long int sum = 0;

    auto t1 = std::chrono::high_resolution_clock::now();

    for(int i=0;i<M;++i)
        sum += points[I[i]];

    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>( t2 - t1 ).count();

    std::cout << "total time (random accesses) " << duration << " nanoseconds." << std::endl;

    return sum/M;

}

/*
 * creates N random numbers, stores them
 * in a vector, extracts the first M numbers 
 * from this vector, and computes their average 
 */
int sequential_access(){

    std::vector<int> points;
    points.reserve(N);

    srand(time(NULL));

    for(int i=0;i<N;++i)
        points.push_back(rand());

    long int sum = 0;

    auto t1 = std::chrono::high_resolution_clock::now();

    for(int i=0;i<M;++i)
        sum += points[i];

    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>( t2 - t1 ).count();

    std::cout << "total time (sequential accesses): " << duration << " nanoseconds." << std::endl;

    return sum/M;

}

int main(){

    random_access();
    sequential_access();
   
}
