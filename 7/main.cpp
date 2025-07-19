#include <iostream>
#include <vector>
using namespace std;

long long max_candies_DP(int left, int right, const vector<int>& pinatas, vector<vector<long long>>& dp) {
    if (left > right) return 0;

    if (dp[left][right] != -1) return dp[left][right];

    long long maxCandies = 0;

    for (int i = left; i <= right; i++) {
        long long leftVal = (left == 0) ? 1 : pinatas[left - 1];
        long long rightVal = (right == (int)pinatas.size() - 1) ? 1 : pinatas[right + 1];

        long long candies = leftVal * (long long)pinatas[i] * rightVal;

        long long total = candies
                 + max_candies_DP(left, i - 1, pinatas, dp)
                 + max_candies_DP(i + 1, right, pinatas, dp);

        if (total > maxCandies) {
            maxCandies = total;
        }
    }

    dp[left][right] = maxCandies;
    return maxCandies;
}

int main() {
    int n;
    cout << "Enter number of pinatas (max 300 recommended): ";
    while (!(cin >> n) || n <= 0 || n > 300) {
        cout << "Invalid input. Enter a positive integer <= 300: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    vector<int> pinatas(n);
    cout << "Enter the value of each pinata (positive integers):\n";
    for (int i = 0; i < n; ++i) {
        while (!(cin >> pinatas[i]) || pinatas[i] <= 0) {
            cout << "Invalid input. Enter a positive integer: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    vector<vector<long long>> dp(n, vector<long long>(n, -1));

    long long result = max_candies_DP(0, n - 1, pinatas, dp);

    cout << "Maximum amount of candies: " << result << endl;

    return 0;
}
