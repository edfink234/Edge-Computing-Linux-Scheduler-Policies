/*
 Single Threaded Implementation
 ------------------------------
 Calculates the mean, variance, standard deviation, minimum, and maximum
 of uniformly distributed random integers
 */

// https://stats.stackexchange.com/questions/25848/how-to-sum-a-standard-deviation
// https://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos

//https://stackoverflow.com/a/28574413/18255427

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>
#include <cmath>
#include <algorithm>

using Clock = std::chrono::high_resolution_clock;

namespace Stats
{
    long long count;
    double mean;
    double var;
    double stdev;
    int min;
    int max;
    
};

namespace Vars
{
    const unsigned int N = std::thread::hardware_concurrency(); //number of threads on device
    const unsigned int NumPerThread = 5e6; //number of random numbers to generate per thread
    std::vector<int> RandNums(NumPerThread*N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    int sz = 0;
}
    
void AddN()
{
    using namespace Vars;
    for (unsigned int i=0; i<NumPerThread*N; i++)
    {
        RandNums[i] = dis(gen);
        ++sz;
    }
}

void Describe()
{
    using namespace Vars;
    using namespace Stats;
    count = RandNums.size();
    
    //https://stackoverflow.com/a/12405793/18255427
    mean = 0.0;
    std::for_each (RandNums.begin(), RandNums.end(), [&](const double d) {
        mean += d/count;
    });
    
    var = 0.0;
    //Population standard deviation
    std::for_each (RandNums.begin(), RandNums.end(), [&](const double d) {
        var += ((d - mean) * (d - mean))/count;
    });
    
    stdev = sqrt(var);
    
    const auto [min_local, max_local] =
    std::minmax_element(begin(RandNums), end(RandNums));
    min = *min_local;
    max = *max_local;
}


int main()
{
    using namespace Vars;
    using namespace Stats;
    
    AddN();
    auto start_time = Clock::now();
    
    Describe();
    std::cout << "\n\nCount = " << count
    << "\nMean = " << mean
    << "\nVariance = " << var
    << "\nStandard Deviation = " << stdev
    << "\nMin = " << min
    << "\nMax = " << max << "\n\n\n";
    auto end_time = Clock::now();
    std::cout << "Time difference = "
    << std::chrono::duration<double, std::nano>(end_time - start_time).count() << " nanoseconds\n";
    std::cout << "size = " << sz << '\n';
}
