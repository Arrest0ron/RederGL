#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP
#include <iostream>
#include <iomanip>

class ChunkBenchmark
{
    public:
    unsigned long long amount;
    float time;
    ChunkBenchmark()
    {
        amount = 0;
        time = 0.0f;
    }
    ~ChunkBenchmark()
    {
        if (amount>0)
        {
            std::cout << "_____________________________________________________\n";
            std::cout << std::left<< std::setw(25)<< "Total chunks rendered: " << amount<<std::endl;
            std::cout << std::setw(25)<< "Total time taken: " << time<<std::endl ;
            std::cout << std::setw(25) << "AVG: "  << time/amount<<std::endl;
            std::cout << std::setw(25) << "CPS: "  << 1.0f/(time/amount)<<std::endl;
            std::cout << "_____________________________________________________\n";
        }

    }
    void frame(float delta, int frame_amount)
    {
        time += delta;
        amount+=frame_amount;
    }
    
};



#endif //BENCHMARK_HPP