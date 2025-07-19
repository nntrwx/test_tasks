#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
using namespace std;

void load_visits(const string& filename,
                unordered_map<string, unordered_set<string>>& userVisits) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string user, product, timestamp;

        getline(ss, user, ',');
        getline(ss, product, ',');
        getline(ss, timestamp, ',');

        userVisits[user].insert(product);
    }
}

int main() {
    unordered_map<string, unordered_set<string>> day1;
    unordered_map<string, unordered_set<string>> day2;

    load_visits("day1.csv", day1);
    load_visits("day2.csv", day2);

    for (const auto& [user, day2Products] : day2) {
        if (day1.find(user) == day1.end()) continue;

        const auto& day1_product = day1[user];
        bool new_product = false;

        for (const auto& product : day2Products) {
            if (day1_product.find(product) == day1_product.end()) {
                new_product = true;
                break;
            }
        }

        if (new_product) {
            cout << user << endl;
        }
    }

    return 0;
}
