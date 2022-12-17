/*
 Program to calculate all of the possible permutations of a heterogram, single-thread
 */

//https://stackoverflow.com/questions/30865231/parallel-code-for-next-permutation?noredirect=1&lq=1
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <fstream>

using Clock = std::chrono::high_resolution_clock;

const unsigned int N = std::thread::hardware_concurrency(); //number of threads on device
std::unordered_map<int,double> ThreadTimes; //run-times of each individual std::thread

std::vector<std::string> permutations;
std::mutex mtx;
std::vector<std::thread> threads;

//calculate factorial
unsigned long long fact(int N)
{
    if (N==0 || N==1)
    {
        return 1;
    }
    unsigned long long total = 1;
    for (int i=1; i<=N; i++)
    {
        total *= i;
    }
    return total;
}

/*
 calculate permutations for i'th partition by rotating the first
 character and then permuting the second to last ones
 */
void permute(int ThreadNumber, unsigned long long partition, std::string vprime, int index)
{
    auto TimeNow = Clock::now();
    std::rotate(vprime.begin(), vprime.begin() + index, vprime.begin() + index + 1);
    std::vector<std::string>::iterator from;
    std::vector<std::string>::iterator to;
    {
        std::lock_guard<std::mutex> lock(mtx);  // hold the lock only while accessing shared vector, not while accessing its contents
        from = permutations.begin () + index*partition;
        to = from + partition;
    }
    
    for (auto i = from; i < to; ++i, std::next_permutation(vprime.begin() + 1, vprime.end()))
    {
        *i = vprime;
    }
    ThreadTimes[ThreadNumber+1] += std::chrono::duration<double, std::nano>(Clock::now() - TimeNow).count();
}

// Driver Code
int main()
{
    const int factor = 11; //Don't really have this many threads though, jtlyk
    std::string str;
    unsigned long long int number = fact(factor);
    
    str.reserve(factor);
    
    permutations.resize(number);
    
    threads.reserve(factor);
    
    for (int i=65; i<65+factor; i++)
    {
        str.push_back(static_cast<char>(i));
    }
//    std::cout << str << '\n';
    // Function call
    auto start_time = Clock::now();

    std::random_device rd;
    std::mt19937 gen(rd());
    unsigned long long partition = (number/factor);
    for (unsigned int i=0; i<factor; i++)
    {
        threads.emplace_back(std::move(std::thread(permute, i, partition, str, i)));
    }

    for (auto &i: threads)
    {
        i.join();
    }
    
    auto end_time = Clock::now();

//    std::cout << permutations.size() << '\n';
//    std::cout << "Time difference = "
//    << std::chrono::duration<double, std::nano>(end_time - start_time).count() << " nanoseconds\n";
    
    double TotalTime = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    std::ofstream out("ThreadTimes.txt", std::ios::trunc);
    
    out << "total," << TotalTime << '\n';
    for (auto& i: ThreadTimes)
    {
        out << i.first << ',' << i.second << '\n';
    }
    
    out.close();
    
//    std::unordered_set test(permutations.begin(),permutations.end());
//    std::cout << test.size() << '\n';
    
//    for (auto& i: permutations)
//    {
//        std::cout << i << '\n';
//    }
}


