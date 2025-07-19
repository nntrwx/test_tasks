#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Laptop {
    long long gain;
    long long price;
    double efficiency() const {
        if (price == 0) return (gain > 0) ? 1e9 : -1e9;
        return static_cast<double>(gain - price) / price;
    }
};

static vector<Laptop> read_laptops(int K) {
    vector<Laptop> laptops(K);
    cout << "Enter expected gains for " << K << " laptops:\n";
    for (int i = 0; i < K; ++i) cin >> laptops[i].gain;

    cout << "Enter prices for " << K << " laptops:\n";
    for (int i = 0; i < K; ++i) cin >> laptops[i].price;

    return laptops;
}

static vector<Laptop> filter_profitable(const vector<Laptop>& laptops) {
    vector<Laptop> filtered;
    filtered.reserve(laptops.size());
    for (const auto &l : laptops) {
        if ((l.price == 0 && l.gain > 0) || (l.price > 0 && l.gain > l.price)) {
            filtered.push_back(l);
        }
    }
    return filtered;
}

static void sort_efficience(vector<Laptop>& laptops) {
    sort(laptops.begin(), laptops.end(), [](const Laptop &a, const Laptop &b) {
        return a.efficiency() > b.efficiency();
    });
}

static long long maximize_capital(int N, long long capital, const vector<Laptop>& laptops) {
    int count = 0;
    for (const auto &l : laptops) {
        if (count == N) break;
        if (l.price <= capital) {
            capital -= l.price;
            capital += l.gain;
            ++count;
        }
    }
    return capital;
}

int main() {
    int N, K;
    long long C;

    cout << "Enter maximum number of laptops to buy (N): ";
    cin >> N;

    cout << "Enter initial capital (C): ";
    cin >> C;

    cout << "Enter total number of available laptops (K): ";
    cin >> K;

    if (N < 0 || K < 0 || C < 0) {
        cout << "Invalid input values\n";
        return 1;
    }

    vector<Laptop> laptops = read_laptops(K);
    vector<Laptop> filtered = filter_profitable(laptops);
    sort_efficience(filtered);

    long long final_capital = maximize_capital(N, C, filtered);

    cout << "Final capital after summer: " << final_capital << "\n";
    return 0;
}
