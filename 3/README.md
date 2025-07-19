This console application processes two CSV files representing user visits to product pages on two different days. It identifies users who:
- Visited some pages on both days
- On the second day, visited at least one product page not visited on the first day

*How to compille and run:*
Prepare two CSV files, `day1.csv` and `day2.csv`, with such structure: user_id,product_id,timestamp
Place these files in the same directory as the executable or provide full paths inside the source code.
- Compile the program using a C++ compiler(for example g++):
g++ main.cpp -o program_name
program_name.exe

*Notes*
- Explanation of the solution efficiency
The solution uses efficient data structures to ensure fast lookups and minimal memory overhead:
unordered_map<string, unordered_set<string>> stores user visits per day.
Each user's visited products are stored in an unordered set to eliminate duplicates and allow O(1) average lookup time.
Comparing visits between two days for each user involves iterating over the second day's visited products and checking if any are absent in the first day's set.

- Time Complexity
Reading each CSV file takes O(N) time, where N is the number of lines (visits) in the file.
For each user in day 2, the program checks their products against day 1. Assuming average M products per user, this is O(M) per user.
Overall, if there are U users and an average of M products per user, the complexity is approximately:
O(N) + O(U * M)

- Space Complexity
The program stores each user's visited products in memory.
Space is roughly proportional to the number of unique users and their unique visited products:
O(U * M)

I spent about 2.5 hours on this task