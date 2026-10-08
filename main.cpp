#include "fix_tags.h"
#include "fix_message.h"
#include "fix_builder.h"
#include <iostream>
#include <string>
#include <chrono>
#include <x86intrin.h>

int main() {
    using namespace FIX;

    std::cout << "=== FIX 4.4 Engine - Ubuntu 24.04 CLI Demo ===\n";
    std::cout << "DATA FIDELIS SERVICES - Equities Regular, NO Crypto\n\n";

    FixBuilder b1;
    std::string logon = b1.begin("FIX.4.4")
        .set(Tags::MsgType, MsgTypes::Logon)
        .set(Tags::SenderCompID, "DATAFIDE")
        .set(Tags::TargetCompID, "BROKER01")
        .set(Tags::MsgSeqNum, 1)
        .setSendingTimeNow()
        .set(Tags::EncryptMethod, 0)
        .set(Tags::HeartBtInt, 30)
        .build();

    FixMessage m1(logon);
    std::cout << "[OUT] Logon: " << m1.toHumanReadable() << "\n\n";

    FixBuilder b2;
    std::string nos = b2.begin("FIX.4.4")
        .set(Tags::MsgType, MsgTypes::NewOrderSingle)
        .set(Tags::SenderCompID, "DATAFIDE")
        .set(Tags::TargetCompID, "BROKER01")
        .set(Tags::MsgSeqNum, 2)
        .setSendingTimeNow()
        .set(Tags::ClOrdID, "ORD-20241008-001")
        .set(Tags::Symbol, "AAPL")
        .set(Tags::Side, 1)
        .set(Tags::OrderQty, 100)
        .set(Tags::OrdType, 2)
        .set(Tags::Price, "185.50")
        .set(Tags::TimeInForce, 0)
        .build();

    FixMessage m2(nos);
    std::cout << "[OUT] NewOrderSingle: " << m2.toHumanReadable() << "\n\n";

    FixBuilder b3;
    std::string exec = b3.begin("FIX.4.4")
        .set(Tags::MsgType, MsgTypes::ExecutionReport)
        .set(Tags::SenderCompID, "BROKER01")
        .set(Tags::TargetCompID, "DATAFIDE")
        .set(Tags::MsgSeqNum, 2)
        .setSendingTimeNow()
        .set(Tags::OrderID, "EXEC-001")
        .set(Tags::ClOrdID, "ORD-20241008-001")
        .set(Tags::ExecID, "E-001")
        .set(Tags::OrdStatus, 0)
        .set(Tags::ExecType, 0)
        .set(Tags::Symbol, "AAPL")
        .set(Tags::Side, 1)
        .set(Tags::OrderQty, 100)
        .build();

    _mm_lfence();
    uint64_t c0 = __rdtsc();
    _mm_lfence();
    FixMessage parsed(exec);
    _mm_lfence();
    uint64_t c1 = __rdtsc();
    _mm_lfence();

    if (parsed.isValid()) {
        std::cout << "[IN] ExecutionReport parse OK cycles=" << (c1 - c0) << "\n";
        std::cout << "     " << parsed.toHumanReadable() << "\n\n";
    }

    const int N = 1000000;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        FixMessage m(exec);
        (void)m.get(Tags::ClOrdID);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    double mps = static_cast<double>(N) / static_cast<double>(us) * 1000000.0;
    std::cout << "Benchmark: " << N << " parses in " << us << " us => " << static_cast<long>(mps) << " msg/sec\n";
    std::cout << "\n=== Ready for Ubuntu CLI ===\n";
    return 0;
}
