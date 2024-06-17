#include <benchmark/benchmark.h>
#include <thread>
#include "stringOps.h"

void function(){
    for(int i = 0; i < 1000; ++i){
        benchmark::DoNotOptimize(i);
    }
}

// static void BM_StringCreation(benchmark::State& state) {
//     for(int i = 0; i < 1000; ++i){
//         benchmark::DoNotOptimize(i);
//         if(i == 122){
//             std::thread t(function);
//             t.join();
//         }
//     }
//     for (auto _ : state){
//         std::string empty_string;

//         benchmark::DoNotOptimize(empty_string);
//         benchmark::ClobberMemory();
//     }
//     // std::cout << __cplusplus << std::endl;
// }\

static void BM_StringSplitVectorRet(benchmark::State& state) {
    std::string test_string = "Hello World IN place vector creation";
    for (auto _ : state){
        std::vector<std::string_view> result = split_string(test_string, ' ');
        benchmark::DoNotOptimize(result);
    }
}

static void BM_StringSplitInPlaceCreation(benchmark::State& state) {
    std::string test_string = "Hello World IN place vector creation";
    std::vector<std::string_view> result;
    for (auto _ : state){
        split_string(test_string, result, ' ');
        benchmark::DoNotOptimize(result);
    }
}

static void BM_StringSplit_StringVector(benchmark::State& state) {
    std::string test_string = "Hello World IN place vector creation";
    std::vector<std::string> result;
    for (auto _ : state){
        result = split_string_inefficient(test_string, ' ');
        benchmark::DoNotOptimize(result);
    }
}




// Register the function as a benchmark
// BENCHMARK(BM_StringCreation);
BENCHMARK(BM_StringSplitVectorRet);
BENCHMARK(BM_StringSplitInPlaceCreation);
BENCHMARK(BM_StringSplit_StringVector);

// Define another benchmark
// static void BM_StringCopy(benchmark::State& state) {
//     std::string x = "hello";
//     for (auto _ : state)
//         std::string copy(x);
// }
// BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();