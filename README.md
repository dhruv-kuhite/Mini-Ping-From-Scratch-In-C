# Custom ICMP Ping Utility in Pure C

A lightweight, low-level network diagnostic tool built from scratch in C using **Winsock Raw Sockets (`SOCK_RAW`)** and custom header serialization. Designed to measure network latency and packet reachability directly at Layer 3 of the OSI model.

---

## 🚀 Features

* **Manual Packet Crafting:** Manually constructs IPv4 and ICMP Echo Request headers without relying on high-level networking libraries.
* **RFC 1071 Internet Checksum:** Implements the standard one's complement sum algorithm to ensure packet integrity and prevent kernel-level transmission drops.
* **High-Precision Timing:** Utilizes Windows High-Performance Counters (`QueryPerformanceCounter`) to measure Round-Trip Time (RTT) down to microsecond precision.
* **Robust Error Handling:** Manages custom timeouts (`SO_RCVTIMEO`) and handles unreachable hosts gracefully.
* **Configurable CLI:** Supports command-line arguments for custom target IPs and ping counts.


## 📺 Video Walkthrough & Rebuilding Session(Only Part 1 now + Other videos comming soon)

Watch the complete, unedited live-coding session where this custom raw socket ping utility is rebuilt from scratch in pure C (part 1):

[![Watch the Rebuild Session on YouTube](https://img.youtube.com/vi/3JrdKYex6TQ/0.jpg)](https://www.youtube.com/watch?v=3JrdKYex6TQ)


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
```

### Execution
Run the compiled executable from an **Administrator Command Prompt**:

```bash
# Default (runs with default packet count)
miniping 8.8.8.8

# Custom (Target IP and specific ping count)
miniping 8.8.8.8 5
```

---

## 🧩 Usage & Output Examples

### 1. Usage Guide (Incorrect Arguments)
If you run the program without proper arguments, it will display the syntax guide:

```text
Enter like this : miniping [ip]
or like this : miniping [ip] [Number of ping]
```

### 2. Successful Execution Example
When run with valid arguments from an Administrator terminal:

```text
ICMP SOCKET CREATED
bind success
pinging 8.8.8.8 with 32 bytes of Data......
Reply from 8.8.8.8 bytes=32 TTL=122 time:15 ms
Reply from 8.8.8.8 bytes=32 TTL=122 time:14 ms
Reply from 8.8.8.8 bytes=32 TTL=122 time:16 ms
Reply from 8.8.8.8 bytes=32 TTL=122 time:15 ms
```

---

## 💡 Architectural Highlights & Challenges Solved

1. **The Checksum Challenge:** Raw sockets require the developer to manually calculate the ICMP header checksum. This required zeroing out the checksum field initially, running the 16-bit one's complement sum, and storing the result in network byte order.
2. **Preventing Kernel Drops:** Ensured proper struct alignment and header layout so that local OS networking stacks and downstream routers didn't silently drop the crafted packets.
3. **Accurate RTT Tracking:** Bypassed standard wall-clock millisecond timers to avoid OS scheduling jitter by leveraging hardware-level CPU ticks via `QueryPerformanceCounter`.
