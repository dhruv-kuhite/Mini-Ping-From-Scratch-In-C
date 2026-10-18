# Mini-Ping-from-Scratch in C

# Custom ICMP Ping Utility in Pure C

A lightweight, low-level network diagnostic tool built from scratch in C using **Winsock Raw Sockets (`SOCK_RAW`)** and custom header serialization. Designed to measure network latency and packet reachability directly at Layer 3 of the OSI model.

---

## 🚀 Features

* **Manual Packet Crafting:** Manually constructs IPv4 and ICMP Echo Request headers without relying on high-level networking libraries.
* **RFC 1071 Internet Checksum:** Implements the standard one's complement sum algorithm to ensure packet integrity and prevent kernel-level transmission drops.
* **High-Precision Timing:** Utilizes Windows High-Performance Counters (`QueryPerformanceCounter`) to measure Round-Trip Time (RTT) down to microsecond precision.
* **Robust Error Handling:** Manages custom timeouts (`SO_RCVTIMEO`) and handles unreachable hosts gracefully.
* **Configurable CLI:** Supports command-line arguments for custom target IPs and ping counts.

---

## 🛠️ Tech Stack & Concepts

* **Language:** Pure C (C99)
* **API:** Winsock2 (`ws2_32.lib`)
* **Core Concepts:** Raw Sockets, Network Byte Order (`htons`/`ntohs`), Struct Padding, Memory Alignment, Performance Timers.

---

## ⚙️ How to Build and Run

### Prerequisites
* A Windows environment (uses Windows-specific Winsock and Performance Counter APIs).
* A C compiler (e.g., GCC via MinGW or MSVC). *Note: Running raw sockets on Windows typically requires administrator/elevated privileges.*

### Compilation (MinGW / GCC)
```bash
gcc main.c -o miniping -lws2_32
