#  Concurrent Rate Limiter (C++)

##  Overview

This project implements a **thread-safe rate limiter** supporting multiple algorithms:

* Fixed Window
* Sliding Window Log

It simulates multiple clients sending requests and enforces per-client rate limits.
Each request is evaluated and marked as **ALLOWED** or **RATE_LIMITED**.

---

## ⚙️ Features

* ✅ Fixed Window rate limiting
* ✅ Sliding Window Log algorithm
* ✅ Per-client custom limits
* ✅ Thread-safe design using `std::mutex`
* ✅ CLI-based configuration
* ✅ Simulated concurrent client load
* ✅ Detailed request logs
* ✅ Summary statistics (total, allowed, rejected, per-client)

---

##  Design Decisions

### 1. Data Structures

* `unordered_map<string, pair<int, long long>>`
  → Used for Fixed Window (count + window start time)

* `unordered_map<string, vector<long long>>`
  → Used for Sliding Window (timestamps of requests)

* `unordered_map<string, pair<int,int>>`
  → Stores per-client custom limits

---

### 2. Thread Safety

* Used `std::mutex` to protect shared data structures
* Ensures no race conditions when multiple clients access limiter

---

### 3. Algorithms

#### 🔹 Fixed Window

* Tracks request count within a fixed time window
* Resets after window expires

#### 🔹 Sliding Window Log

* Stores timestamps of requests
* Removes outdated entries dynamically
* More accurate than fixed window

---

##  How to Run

### Compile:

```bash
g++ -std=c++11 main.cpp -o app
```

### Run:

```bash
./app
```

### Optional CLI:

```bash
./app 10 60
```

Where:

* `10` → max requests
* `60` → window (seconds)

---

##  Sample Output

```
1712345678 | A | Fixed: ALLOWED | Sliding: ALLOWED
1712345678 | B | Fixed: RATE_LIMITED | Sliding: ALLOWED
...

=== SUMMARY ===
Total Requests: 100
Allowed: 25
Rejected: 75

Per Client Allowed:
A -> 3
B -> 7
C -> 5
D -> 5
E -> 5
```

---

## ⚖️ Trade-offs & Limitations

* Current simulation is sequential but represents concurrent behavior
* Statistics counters are not separately locked (can be improved for strict concurrency)
* Sliding Window uses vector cleanup (can be optimized using deque)

---

## ⭐ Future Improvements

* Add HTTP server (`/request`, `/stats`)
* Dynamic config update endpoint
* Implement Token Bucket algorithm
* Improve performance with optimized data structures
* True parallel load testing with full thread support

---

## 🧾 Conclusion

This project demonstrates core concepts used in real-world systems like API gateways and trading platforms:

* Rate limiting strategies
* Thread safety
* System design thinking
* Trade-off awareness

---

## 👩‍💻 Author

Latika Dhiman
