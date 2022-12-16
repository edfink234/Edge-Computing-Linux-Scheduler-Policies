// C++ program to print all
// permutations with duplicates allowed
//https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
// This is code is contributed by rathbhupendra

#include <algorithm>
#include <vector>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <string>
#include <unordered_set>

using Clock = std::chrono::high_resolution_clock;

const unsigned int N = std::thread::hardware_concurrency(); //number of threads on device

std::vector<std::string> permutations;

long long int fact(int N)
{
    if (N==0 || N==1)
    {
        return 1;
    }
    int total = 1;
    for (int i=1; i<=N; i++)
    {
        total *= i;
    }
    return total;
}

// Driver Code
int main()
{
    srand(time(0));
    const int factor = 11;
    std::string str;

    str.reserve(factor);
   
    permutations.reserve(fact(factor));
    
    for (int i=65; i<65+factor; i++)
    {
        str.push_back(static_cast<char>(i));
    }
    std::cout << str << '\n';
    // Function call
    auto start_time = Clock::now();
    do {permutations.push_back(str);} while(std::next_permutation(str.begin(),str.end()));
    auto end_time = Clock::now();

//    std::cout << permutations.size() << '\n';
//    for (auto& i: permutations)
//    {
//        std::cout << i << '\n';
//    }
    
    std::cout << "Time difference = "
    << std::chrono::duration<double, std::nano>(end_time - start_time).count() << " nanoseconds\n";
    
//    std::unordered_set test(permutations.begin(),permutations.end());
//    std::cout << test.size() << '\n';
}
