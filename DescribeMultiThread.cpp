// https://stats.stackexchange.com/questions/25848/how-to-sum-a-standard-deviation
// https://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos

//https://stackoverflow.com/a/28574413/18255427

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <fstream>

using Clock = std::chrono::high_resolution_clock;
std::unordered_map<int,double> ThreadTimes; //run-times of each individual thread

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
    std::vector<int> mins;
    std::vector<int> maxs;

    std::random_device rd;
    std::mt19937 gen(rd());
//    std::mt19937 gen;
    std::uniform_int_distribution<> dis(1, 1000);
    int sz = 0;
    std::mutex mtx;
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

//Count = 40000000
//Mean = 500.596
//Variance = 83355.7
//Standard Deviation = 288.714
//Min = 1
//Max = 1000
//
//
//Time difference = 1.39862e+09 nanoseconds
//size = 40000000

void Describe(int threadNumber, char option)
{
    auto TimeNow = Clock::now();
    using namespace Vars;
    using namespace Stats;
    
    std::vector<int>::iterator from;
    std::vector<int>::iterator to;
    if (option == 'm')
    {
        {
            std::lock_guard<std::mutex> lock(mtx);  // hold the lock only while accessing shared vector, not while accessing its contents
            from = RandNums.begin () + threadNumber*NumPerThread;
            to = from + NumPerThread;
        }
        double temp_mean = 0;
        //https://stackoverflow.com/a/12405793/18255427
        std::for_each (from, to, [&](const double d) {
            temp_mean += d/count;
        });
        
        const auto [min_local, max_local] =
        std::minmax_element(from, to);
        mins.push_back(*min_local);
        maxs.push_back(*max_local);
        
        {
            std::lock_guard<std::mutex> lock(mtx);  // hold the lock only while accessing shared vector, not while accessing its contents
            mean += temp_mean;
        }
    }
    else
    {
        {
            std::lock_guard<std::mutex> lock(mtx);  // hold the lock only while accessing shared vector, not while accessing its contents
            from = RandNums.begin () + threadNumber*NumPerThread;
            to = from + NumPerThread;
        }
        double temp_var = 0.0;
        //https://stackoverflow.com/a/12405793/18255427
        std::for_each (from, to, [&](const double d) {
            temp_var += ((d - mean) * (d - mean))/count;
        });
        {
            std::lock_guard<std::mutex> lock(mtx);  // hold the lock only while accessing shared vector, not while accessing its contents
            var += temp_var;
        }
        
    }
    
    ThreadTimes[threadNumber+1] += std::chrono::duration<double, std::nano>(Clock::now() - TimeNow).count();
}


int main()
{
    using namespace Vars;
    using namespace Stats;
    
    AddN(); //generate random number vector
    auto start_time = Clock::now();
    mean = 0.0;
    var = 0.0;
    mins.reserve(N);
    maxs.reserve(N);
//    Describe();
    std::vector<std::thread> threads;
    threads.reserve(N);
    
    count = RandNums.size();
    for (unsigned int i=0; i<N; i++)
    {
        threads.emplace_back(std::move(std::thread(Describe, i, 'm')));
    }
    
    for (auto &i: threads)
    {
        i.join();
    }
    
    min = *std::min_element(mins.begin(), mins.end());
    max = *std::max_element(maxs.begin(), maxs.end());
    
    threads.clear();

    for (unsigned int i=0; i<N; i++)
    {
        threads.emplace_back(std::move(std::thread(Describe, i, 'v')));
    }
    
    for (auto &i: threads)
    {
        i.join();
    }

    stdev = sqrt(var);
    
//    std::cout << "\n\nCount = " << count
//    << "\nMean = " << mean
//    << "\nVariance = " << var
//    << "\nStandard Deviation = " << stdev
//    << "\nMin = " << min
//    << "\nMax = " << max << "\n\n\n";
    
    auto end_time = Clock::now();
    
    double TotalTime = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
//    std::cout << TotalTime << '\n';
    
    std::ofstream out("ThreadTimes.txt", std::ios::trunc);
    
    out << "total," << TotalTime << '\n';
    for (auto& i: ThreadTimes)
    {
        out << i.first << ',' << i.second << '\n';
    }
    
    out.close();
    
//    std::cout << "size = " << sz << '\n';
}
