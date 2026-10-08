# FIX 4.4 Equities Low-Latency Engine - Ubuntu 24.04 CLI

**DATA FIDELIS SERVICES | Equities Regular, NO Crypto**
Quantitative Infrastructure Consultant | HKPR | B2B Flexirate

## Problem
QuickFIX allocates heap per field -> 200-500us p99. Breaks sub-2ms P99 SLO.

## Solution
C++20 zero-copy engine: string_view slices over recv() buffer, no malloc in hot path, linear scan cache-friendly, rdtsc + lfence measurement. Target <50us p99, >500K msg/sec.

## Platform
- Ubuntu 24.04 LTS CLI only, no VS Code, no IDE
- g++-13, C++20, -O3 -march=native
- Equities: AAPL, MSFT, HSBA.HK, 0700.HK (no BTC/USD)

## Build & Run
```bash
make
./fix_demo
