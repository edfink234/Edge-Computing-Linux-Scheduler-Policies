// C++ program to print all
// permutations with duplicates allowed
//https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
//https://www.geeksforgeeks.org/find-the-n-th-lexicographic-permutation-of-string-using-factoradic-method/
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>
#include <unordered_set>
#include <fstream>
#include <stack>
#include <unordered_map>

using Clock = std::chrono::high_resolution_clock;

const unsigned int N = std::thread::hardware_concurrency(); //number of threads on device
std::unordered_map<int,double> ThreadTimes; //run-times of each individual thread

std::vector<std::string> tokens;
std::vector<std::thread> threads;
const int token_count = 5e6;
const int token_length = 8;
std::mutex mtx;

// Driver Code
int main()
{
    tokens.reserve(token_count);
    std::string token;
    token.reserve(token_length);
    threads.reserve(N);
    
    auto generate_token = [&]()
    {
        token.clear();
        for (int i=0; i < token_length; i++)
        {
            token += static_cast<char>(65+rand()%26);
        }
        return token;
    };
    
    for (int i=0; i<token_count; i++)
    {
        tokens.push_back(generate_token());
    }
    
    std::vector<std::string>::iterator from;
    std::vector<std::string>::iterator to;
    
    auto thread_shuffle = [&](int start, int stop, int threadNumber) mutable
    {
        auto TimeNow = Clock::now();
        std::random_device rd;
        std::mt19937 gen(rd());
        {
            std::lock_guard<std::mutex> lock(mtx);  // hold the lock only while accessing shared vector, not while accessing its contents
            from = tokens.begin()+start;
            to = tokens.begin()+stop;
        }
        std::shuffle(from, to, gen);
        ThreadTimes[threadNumber] = std::chrono::duration<double, std::nano>(Clock::now() - TimeNow).count();
    };
    
    int step = token_count/token_length;
    
    auto start_time = Clock::now();
    
    for (unsigned int i=0; i<N; i++)
    {
        threads.emplace_back(std::thread(thread_shuffle, (i*step), (i+1)*step, i+1));
    }
    
    for (auto &i: threads)
    {
        i.join();
    }
    
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
}


