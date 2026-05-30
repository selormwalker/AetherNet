#include <iostream>
#include <chrono>
#include "UDPClient.hpp"

int main() {
    std::cout << "AetherNet: High-Performance Networking Engine" << std::endl;

    // Initialize UDP Client for low-latency market data transmission
    AetherNet::UDPClient client("127.0.0.1", 8080);

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Sending low-latency data packet..." << std::endl;
    client.send("FIX.4.4|9=122|35=D|...|10=171|");

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> elapsed = end - start;

    std::cout << "Packet dispatched in " << elapsed.count() << " microseconds." << std::endl;
    std::cout << "AetherNet standby. Ready for institutional-grade throughput." << std::endl;

    return 0;
}
