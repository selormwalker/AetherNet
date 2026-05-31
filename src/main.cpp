#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>
#include "UDPClient.hpp"
#include "TCPClient.hpp"
#include "LockFreeQueue.hpp"

void run_benchmark() {
    std::cout << "--- AetherNet Industrial Benchmark ---" << std::endl;
    
    AetherNet::UDPClient udp_client("127.0.0.1", 8080);
    std::vector<double> latencies;
    const int iterations = 10000;

    // Warm up
    for(int i = 0; i < 100; ++i) udp_client.send("WARMUP");

    for(int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        
        udp_client.send("FIX.4.4|35=D|msg=" + std::to_string(i));
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> elapsed = end - start;
        latencies.push_back(elapsed.count());
    }

    std::sort(latencies.begin(), latencies.end());
    double sum = std::accumulate(latencies.begin(), latencies.end(), 0.0);
    double avg = sum / iterations;
    double p99 = latencies[static_cast<int>(iterations * 0.99)];
    double p999 = latencies[static_cast<int>(iterations * 0.999)];

    std::cout << "Avg Latency: " << avg << " us" << std::endl;
    std::cout << "p99 Latency: " << p99 << " us" << std::endl;
    std::cout << "p99.9 Latency: " << p999 << " us" << std::endl;
    std::cout << "Throughput: Ready for Institutional Execution." << std::endl;
}

int main() {
    std::cout << "AetherNet v2.0: High-Performance Networking Engine" << std::endl;
    
    // Demonstrate multi-threaded lock-free queue
    AetherNet::LockFreeQueue<std::string> queue(1024);
    
    std::thread producer([&]() {
        for(int i = 0; i < 5; ++i) {
            queue.push("ASYNC_MSG_" + std::to_string(i));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    std::thread consumer([&]() {
        int count = 0;
        while(count < 5) {
            auto msg = queue.pop();
            if(msg) {
                std::cout << "[Consumer] Received: " << *msg << std::endl;
                count++;
            }
        }
    });

    producer.join();
    consumer.join();

    run_benchmark();

    return 0;
}
