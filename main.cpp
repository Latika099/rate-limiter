#include <iostream>
#include <unordered_map>
#include <vector>
#include <ctime>
#include<string>

using namespace std;

// RateLimiter supports:
// - Fixed Window
// - Sliding Window Log
// - Per-client custom limits
class RateLimiter {
private:
    int maxRequests;

    int windowSeconds;
    unordered_map<string, pair<int,int>> clientConfig;
// client_id -> {maxRequests, windowSeconds}
    unordered_map<string, pair<int, long long>> fixedWindow;
    unordered_map<string, vector<long long>> slidingWindow;

public:
    RateLimiter(int maxReq, int windowSec) {
        maxRequests = maxReq;
        windowSeconds = windowSec;
    }

    long long getTime() {
        return time(0);
    }
    void setClientLimit(string client, int maxReq, int windowSec) {
    clientConfig[client] = {maxReq, windowSec};
}

    // Fixed Window
   bool allowFixed(string client_id) {
    

    int maxReq = maxRequests;
    int windowSec = windowSeconds;

    auto it = clientConfig.find(client_id);
    if (it != clientConfig.end()) {
        maxReq = it->second.first;
        windowSec = it->second.second;
    }

    long long current = getTime();

    if (fixedWindow.find(client_id) == fixedWindow.end()) {
        fixedWindow[client_id] = {1, current};
        return true;
    }

    int count = fixedWindow[client_id].first;
    long long start = fixedWindow[client_id].second;

    if (current - start >= windowSec) {
        fixedWindow[client_id] = {1, current};
        return true;
    }

    if (count < maxReq) {
        fixedWindow[client_id].first++;
        return true;
    }

    return false;
}

    // Sliding Window
    bool allowSliding(string client_id) {
  

    int maxReq = maxRequests;
    int windowSec = windowSeconds;

    auto it = clientConfig.find(client_id);
    if (it != clientConfig.end()) {
        maxReq = it->second.first;
        windowSec = it->second.second;
    }

    long long current = getTime();

    vector<long long> &timestamps = slidingWindow[client_id];

    vector<long long> newWindow;
    for (long long t : timestamps) {
        if (current - t < windowSec)
            newWindow.push_back(t);
    }
    timestamps = newWindow;

    if ((int)timestamps.size() < maxReq) {
        timestamps.push_back(current);
        return true;
    }

    return false;
}
};

int main(int argc, char* argv[]) {

    int maxReq = 5;
    int windowSec = 5;

    // CLI config
    if (argc >= 3) {
    try {
        maxReq = stoi(argv[1]);
        windowSec = stoi(argv[2]);
    } catch (...) {
        cout << "Invalid CLI arguments. Using default values.\n";
    }
} else {
    cout << "Using default config: maxReq=5, windowSec=5\n";
}

    RateLimiter limiter(maxReq, windowSec);
    limiter.setClientLimit("A", 3, 5);   // stricter
    limiter.setClientLimit("B", 7, 5);   // more permissive
    vector<string> clients = {"A", "B", "C", "D", "E"};

    int total = 0, allowedCount = 0, rejectedCount = 0;
    unordered_map<string, int> clientAllowed;

   for (int round = 0; round < 20; round++) {
    for (string client : clients) {

        bool fixedAllowed = limiter.allowFixed(client);
        bool slidingAllowed = limiter.allowSliding(client);

        total++;

        if (fixedAllowed) {
            allowedCount++;
            clientAllowed[client]++;
        } else {
            rejectedCount++;
        }

        cout << time(0) << " | " << client
             << " | Fixed: " << (fixedAllowed ? "ALLOWED" : "RATE_LIMITED")
             << " | Sliding: " << (slidingAllowed ? "ALLOWED" : "RATE_LIMITED")
             << endl;
    }
}

    // ===== SUMMARY =====
    cout << "\n===== SUMMARY =====\n";
    cout << "Total Requests: " << total << endl;
    cout << "Allowed: " << allowedCount << endl;
    cout << "Rejected: " << rejectedCount << endl;

    cout << "\nPer Client Allowed:\n";
    for (auto &p : clientAllowed) {
        cout << p.first << " -> " << p.second << endl;
    }

    return 0;
}