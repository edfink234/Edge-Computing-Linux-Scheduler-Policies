//https://linuxhint.com/use-linux-chrt-command/
//https://stackoverflow.com/questions/41150975/how-to-display-list-of-running-processes-python
//https://stackoverflow.com/questions/3503879/assign-output-of-os-system-to-a-variable-and-prevent-it-from-being-displayed-on
//ps -eLf | grep ./dum

//Linux magix-83 4.19.0-22-amd64 #1 SMP Debian 4.19.260-1 (2022-09-29) x86_64 GNU/Linux
//chrt --help

//https://www.tutorialspoint.com/how-to-create-a-high-resolution-timer-with-cplusplus-and-linux

#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include <unordered_map>
#include <fstream>

using Clock = std::chrono::high_resolution_clock;
std::unordered_map<int,double> ThreadTimes; //run-times of each individual thread

namespace SharedVars
{
    const unsigned int N = std::thread::hardware_concurrency(); //number of threads on device
    const unsigned int NumPerThread = 5e5; //number of random numbers to generate per thread
    std::vector<int> RandNums(NumPerThread*N);
    std::mutex mtx;
}

void PerThread_AddN(int threadNumber)
{
    auto TimeNow = Clock::now();
    using namespace SharedVars;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);

    std::vector<int>::iterator from;
    std::vector<int>::iterator to;
    {
        std::lock_guard<std::mutex> lock(mtx);  // hold the lock only while accessing shared vector, not while accessing its contents
        from = RandNums.begin () + threadNumber*NumPerThread;
        to = from + NumPerThread;
    }
    for (auto i = from; i < to; ++i)
    {
        *i = dis(gen);
    }
    ThreadTimes[threadNumber+1] = std::chrono::duration<double, std::nano>(Clock::now() - TimeNow).count();
}

int main()
{
    using namespace SharedVars;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto start_time = Clock::now();
    std::vector<std::thread> threads;
    threads.reserve(N);
    
    for (unsigned int i=0; i<N; i++)
    {
        threads.emplace_back(std::move(std::thread(PerThread_AddN, i)));
    }
    
    for (auto &i: threads)
    {
//        std::cout << i.get_id() << '\n';
        i.join();
    }
    
    auto end_time = Clock::now();
//    std::cout << "\nTime difference = "
//    << std::chrono::duration<double, std::nano>(end_time - start_time).count() << " nanoseconds\n";
    
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
}
