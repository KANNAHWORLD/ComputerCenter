#include <benchmark/benchmark.h>
#include <thread>

void function(){
    for(int i = 0; i < 1000; ++i){
        benchmark::DoNotOptimize(i);
    }
}

static void BM_StringCreation(benchmark::State& state) {
    for(int i = 0; i < 1000; ++i){
        benchmark::DoNotOptimize(i);
        if(i == 122){
            std::thread t(function);
            t.join();
        }
    }
    for (auto _ : state){
        std::string empty_string;

        benchmark::DoNotOptimize(empty_string);
        benchmark::ClobberMemory();
    }
    // std::cout << __cplusplus << std::endl;
}

// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
    std::string x = "hello";
    for (auto _ : state)
        std::string copy(x);
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();