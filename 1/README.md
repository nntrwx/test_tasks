This is a simple console-based orderbook engine for exchanging UAH and USD. It matches buy and sell orders based on price and updates user balances accordingly.

*How to compile and run:*
- Compile the program using a C++ compiler(for example g++):
g++ main.cpp -o program_name
program_name.exe

- The program accepts two types of input:
 Place Order
user_id amount price side
(user_id: integer (e.g., 101)
amount: integer (amount of UAH to buy or sell)
price: price for 1 UAH in USD
side: 1 for buy, 0 for sell)

 Check Balance:
balance user_id

- To exit the program:
Press `Ctrl + C` in the terminal.

*Notes:*
- How It Works
Two priority queues are used:
buyOrders: max-heap, orders sorted from highest to lowest price
sellOrders: min-heap, orders sorted from lowest to highest price

Orders are matched if the top buy price ≥ top sell price.
Trades are executed for the minimum available amount.
After each match, users' balances are updated and shown using BalanceChange{} messages.

- Why This Solution Is Efficient
This implementation uses std::priority_queue, which is backed by a binary heap, allowing:
O(log n) time complexity for inserting and removing orders
O(1) access to the best available buy/sell order via top()

Matching is performed only when an incoming order is placed, and it always attempts to execute against the best price available on the opposite side. As a result:
Time complexity per order placement is O(log n) for each insert or pop operation.
Total matching per order can take up to O(m log n) in the worst case, where m is the number of matches performed for that order.

For typical usage where orders are matched quickly, performance remains efficient and scalable for moderate orderbook sizes.

I spent about 5.5 hours on this task
