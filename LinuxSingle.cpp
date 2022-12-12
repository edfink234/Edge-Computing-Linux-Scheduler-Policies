//https://linuxhint.com/use-linux-chrt-command/
//https://stackoverflow.com/questions/41150975/how-to-display-list-of-running-processes-python

//ps -eLf | grep ./dum

//Linux magix-83 4.19.0-22-amd64 #1 SMP Debian 4.19.260-1 (2022-09-29) x86_64 GNU/Linux
//chrt --help

//https://www.tutorialspoint.com/how-to-create-a-high-resolution-timer-with-cplusplus-and-linux

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>


using Clock = std::chrono::high_resolution_clock;



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
    


using namespace Vars;


void AddN()
{
    for (unsigned int i=0; i<NumPerThread*N; i++)
    {
        RandNums[i] = dis(gen);
        ++sz;
    }
}

int main()
{
    auto start_time = Clock::now();

    AddN();
    
    auto end_time = Clock::now();
    std::cout << "Time difference = "
    << std::chrono::duration<double, std::nano>(end_time - start_time).count() << " nanoseconds\n";
    std::cout << "size = " << sz << '\n';
}

