This program calculates the maximum possible profit you can earn by buying and reselling laptops, given a starting capital and a limited number of purchases.

*How to compile and run:*
- Compile the program using a C++ compiler(for example g++):
g++ main.cpp -o program_name
program_name.exe

*After launching, the program expects input in the following format:*
N — maximum number of laptops you can buy
C — starting capital
K — number of available laptops
Then K integers for gain values
Then K integers for price values (same order)

*Notes:*
The program skips laptops that generate no profit (gain <= price).
Laptops are sorted by efficiency — defined as (gain - price) / price — in descending order.
The algorithm selects the most efficient affordable laptops first, until the maximum count or capital runs out.

I spent about 4 hours on this task