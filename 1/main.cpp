#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
using namespace std;

struct Order {
    int64_t user_id;
    int64_t amount;
    int64_t price;
    bool side;

    Order(int64_t uid, int64_t amt, int64_t prc, bool s)
        : user_id(uid), amount(amt), price(prc), side(s) {}
};

struct BuyComparator {
    bool operator()(const Order& a, const Order& b) {
        return a.price < b.price;
    }
};

struct SellComparator {
    bool operator()(const Order& a, const Order& b) {
        return a.price > b.price;
    }
};

priority_queue<Order, vector<Order>, BuyComparator> buyOrders;
priority_queue<Order, vector<Order>, SellComparator> sellOrders;

unordered_map<int64_t, int64_t> user_usd_balance;
unordered_map<int64_t, int64_t> user_uah_balance;

void apply_balance_change(int64_t user_id, int64_t value, const string& currency) {
    if (currency == "USD") {
        user_usd_balance[user_id] += value;
    } else if (currency == "UAH") {
        user_uah_balance[user_id] += value;
    }

    cout << "BalanceChange{user_id: " << user_id
              << ", value: " << value
              << ", currency: " << currency << "}\n";
}

void match(Order& incoming) {
    if (incoming.side) {
        while (!sellOrders.empty() && incoming.amount > 0) {
            Order topSell = sellOrders.top();
            if (topSell.price > incoming.price) break;

            int64_t tradeAmount = min(incoming.amount, topSell.amount);
            int64_t totalUSD = tradeAmount * topSell.price;

            apply_balance_change(incoming.user_id, -totalUSD, "USD");
            apply_balance_change(incoming.user_id, tradeAmount, "UAH");

            apply_balance_change(topSell.user_id, -tradeAmount, "UAH");
            apply_balance_change(topSell.user_id, totalUSD, "USD");

            incoming.amount -= tradeAmount;
            topSell.amount -= tradeAmount;

            sellOrders.pop();
            if (topSell.amount > 0) sellOrders.push(topSell);
        }

        if (incoming.amount > 0) buyOrders.push(incoming);

    } else {
        while (!buyOrders.empty() && incoming.amount > 0) {
            Order topBuy = buyOrders.top();
            if (topBuy.price < incoming.price) break;

            int64_t tradeAmount = min(incoming.amount, topBuy.amount);
            int64_t totalUSD = tradeAmount * topBuy.price;

            apply_balance_change(incoming.user_id, -tradeAmount, "UAH");
            apply_balance_change(incoming.user_id, totalUSD, "USD");

            apply_balance_change(topBuy.user_id, -totalUSD, "USD");
            apply_balance_change(topBuy.user_id, tradeAmount, "UAH");

            incoming.amount -= tradeAmount;
            topBuy.amount -= tradeAmount;

            buyOrders.pop();
            if (topBuy.amount > 0) buyOrders.push(topBuy);
        }

        if (incoming.amount > 0) sellOrders.push(incoming);
    }
}

void print_user_balance(int64_t user_id) {
    cout << "User " << user_id << " balances:\n";
    cout << "  USD: " << user_usd_balance[user_id] << "\n";
    cout << "  UAH: " << user_uah_balance[user_id] << "\n";
}

int main() {
    cout << "Orderbook UAH/USD started.\n";
    cout << "Commands:\n";
    cout << "  Place order: user_id amount price side (1 = buy, 0 = sell)\n";
    cout << "  Check balance: balance user_id\n";

    while (true) {
        string input;
        cout << "> ";
        cin >> input;

        if (input == "balance") {
            int64_t uid;
            cin >> uid;
            print_user_balance(uid);
            continue;
        }

        try {
            int64_t user_id = stoll(input);
            int64_t amount, price;
            bool side;
            cin >> amount >> price >> side;

            Order newOrder(user_id, amount, price, side);
            match(newOrder);
        } catch (...) {
            cout << "Invalid input. Try again.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    return 0;
}
