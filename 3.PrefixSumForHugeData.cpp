#include<iostream>
#include<thread>
#include<vector>
#include<chrono>
#include<functional>
#include<numeric>
using namespace std;

constexpr int MIN_BLOCK_SIZE = 1000;

template<typename iterator>
vector<int> SerialprefixSum(iterator start, iterator end){
    vector<int> op;
    int sum = 0;
    for(auto it = start; it != end; ++it){
        sum += *it;
        op.push_back(sum);
    }
    return op;
}


template<typename iterator, typename T>
void parallelPrefixSum(iterator start, iterator end, vector<T>& arr){
    T sum = 0;
    for(auto it = start; it != end; ++it){
        sum += *it;
        arr.push_back(sum);
    }
}

template<typename iterator, typename T>
vector<T> parallelAccumulatePrefixSum(iterator start, iterator end){

    // To find the input Size.
    int InputSize = distance(start, end);
    if(InputSize == 0){
        return {};
    }
    
    // To find the allowed Threads.
    int nThreadsByElements = InputSize/MIN_BLOCK_SIZE;
    int nThreadsByHardware = thread::hardware_concurrency();
    int nThreads = min(nThreadsByElements, nThreadsByHardware);
    
    if(nThreads == 0){
        nThreads = 1;
    } 

    // To find the Block Size.
    int BlockSize = InputSize / nThreads;
    
    // Dynamic Arrays to store the threads & results.
    vector<thread> threads(nThreads - 1);
    vector<vector<T>> results(nThreads);

    iterator BlockStart;
    iterator BlockEnd;
    for(int i = 0 ; i < nThreads - 1; i++){
        BlockStart = start + i * BlockSize;
        BlockEnd = start +(i + 1) * BlockSize;
        threads[i] = thread([=, &results](){
            parallelPrefixSum(BlockStart, BlockEnd, results[i]);
        });
    }

    iterator LastBlockStart = start + (nThreads - 1) * BlockSize;
    iterator LastBlockEnd = end;
    parallelPrefixSum(LastBlockStart, LastBlockEnd, results[nThreads - 1]);

    for(auto& t : threads){
        if(t.joinable()) t.join();
    }

    /* Here the logic is that for every new block, the last element of the previous block is not added, so we compute it each time and add it to the current block. Meaning, let's say {1, 2, 3, 4, 5, 6} gets divided into {1, 2, 3} and {4, 5, 6}, then the prefix sum should be {1, 3, 6, 10, 15, 21}. But without the below calculation, it would be {1, 3, 6}, {4, 9, 15}, This is the problem.The difference is exactly the last element of the previous block, and for every block, to make the overall offset correct, so we have to add the offsets from the previous blocks.
    */

    vector<T> Offsets(nThreads, 0);
    for(int i = 1; i < nThreads; ++i){
        Offsets[i] = Offsets[i - 1] + results[i - 1].back();
        for(auto& val : results[i]){
            val += Offsets[i];
        }
    }

    // Merging the results in single vector.
    vector<T> BigVector;
    for(const auto& smallBlock : results){
        BigVector.insert(BigVector.end(), smallBlock.begin(), smallBlock.end());
    }

    return BigVector;
}


int main(){
    vector<int> arr(100'000'000);
    
    // Will fill up the value starting from 1 way up to 10,000
    iota(arr.begin(), arr.end(), 1);

    auto serialStart = chrono::high_resolution_clock::now();
    auto serialRes = SerialprefixSum(arr.begin(), arr.end());
    auto serialEnd = chrono::high_resolution_clock::now();
    auto serialTotalTime = chrono::duration_cast<chrono::milliseconds>(serialEnd - serialStart).count();

    auto parallelStart = chrono::high_resolution_clock::now();
    auto parallelRes = parallelAccumulatePrefixSum<vector<int>::iterator, int>(arr.begin(), arr.end());
    auto parallelEnd = chrono::high_resolution_clock::now();
    auto parallelTotalTime = chrono::duration_cast<chrono::milliseconds>(parallelEnd - parallelStart).count();

    cout<<"Time taken for Serial Prefix sum : "<<serialTotalTime<<"ms"<<endl;
    cout<<"Time taken for Parallel Prefix sum : "<<parallelTotalTime<<"ms"<<endl;
}

/*
    Note to Me
    Now parallelPrefix is taking more time than the serial.
*/ 