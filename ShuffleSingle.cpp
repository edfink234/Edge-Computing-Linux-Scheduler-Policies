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

std::vector<std::string> tokens;
const int token_count = 5e7;
const int token_length = 8;

int main()
{
    tokens.reserve(token_count);
    std::string token;
    token.reserve(token_length);
    
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
    
    std::ofstream out("tokensbefore.txt");
    for (auto& i: tokens)
    {
        out << i << '\n';
    }
    out.close();
    
    auto start_time = Clock::now();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(tokens.begin(),tokens.end(), gen);
    auto end_time = Clock::now();
    
    std::ofstream outafter("tokensafter.txt");
    for (auto& i: tokens)
    {
        outafter << i << '\n';
    }
    outafter.close();
    
        std::cout << "\nTime difference = "
        << std::chrono::duration<double, std::nano>(end_time - start_time).count() << " nanoseconds\n";
}


