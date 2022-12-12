// C++ program to print all
// permutations with duplicates allowed
//https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
//https://www.geeksforgeeks.org/find-the-n-th-lexicographic-permutation-of-string-using-factoradic-method/
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include <unordered_set>
#include <fstream>
#include <stack>

using Clock = std::chrono::high_resolution_clock;

const unsigned int N = std::thread::hardware_concurrency(); //number of threads on device

std::vector<std::string> permutations;
std::mutex mtx;
std::vector<std::thread> threads;
int number;

std::string string_permutation(long long int n, std::string str)
{
    // Creating an empty stack
    std::stack<int> s;
    std::string result;

    // Subtracting 1 from N because the
    // permutations start from 0 in
    // Factoradic method
    n = n - 1;

    // Loop to generate the factroid
    // of the sequence
    for (int i = 1; i < str.size() + 1; i++) {
        s.push(n % i);
        n = n / i;
    }

    // Loop to generate nth permutation
    for (int i = 0; i < str.size(); i++) {
        int a = s.top();
        result += str[a];
        int j;

        // Remove 1-element in each cycle
        for (j = a; j < str.length(); j++)
            str[j] = str[j + 1];
        str[j + 1] = '\0';
        s.pop();
    }

    // Final answer
    return result;
}

int fact(int N)
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

void permute(int partition, std::string &s, int index)
{
//    for (int j = (i*partition)+1; j < ((i+1)*partition)+1; ++j)
//    {
//        permutations.push_back(string_permutation(j,s));
//    }
    std::vector<std::string>::iterator from;
    std::vector<std::string>::iterator to;
    {
        std::lock_guard<std::mutex> lock(mtx);  // hold the lock only while accessing shared vector, not while accessing its contents
        from = permutations.begin () + index*partition;
        to = from + partition;
    }
    
    int j = (index*partition)+1;
    for (auto i = from; i < to; ++i)
    {
        *i = string_permutation(j++,s);
    }
}

// Driver Code
int main()
{
    const int factor = 1*N;
    std::string str;
    number = fact(factor);
    str.reserve(factor);
    
    permutations.resize(number);
    threads.reserve(N);
    
    for (int i=65; i<65+factor; i++)
    {
        str.push_back(static_cast<char>(i));
    }
    std::cout << str << '\n';
    // Function call
    auto start_time = Clock::now();

    std::random_device rd;
    std::mt19937 gen(rd());
    int partition = number/N;
    
    for (unsigned int i=0; i<N; i++)
    {
        threads.emplace_back(std::move(std::thread(permute, partition, std::ref(str), i)));
    }
//    std::shuffle(str.begin(),str.end(), gen);
//    permute(str);
    
    for (auto &i: threads)
    {
        i.join();
    }
    
    auto end_time = Clock::now();

    std::cout << permutations.size() << '\n';
    std::cout << "Time difference = "
    << std::chrono::duration<double, std::nano>(end_time - start_time).count() << " nanoseconds\n";
    
    std::unordered_set test(permutations.begin(),permutations.end());
    std::cout << test.size() << '\n';
    
//    for (auto& i: permutations)
//    {
//        std::cout << i << '\n';
//    }
}


