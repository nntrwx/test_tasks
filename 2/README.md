This program generates a 2D maze with different cell types:

- (A)Entrance — exactly one cell on the outer edge.
- (B)Exit — exactly one cell on the outer edge.
- (.)Road — passable cells.
- (#)Wall — impassable cells.
- (T)Trap — passable cells, but passing through 3 traps kills the player. Between 0 and 5 traps per maze.
- ($)Treasure — one treasure cell, reachable by the player.


The program guarantees at least one path from entrance to exit, ensuring the player won't die from traps, and the treasure is reachable.

*How to compile and run:*
- Compile the program using a C++ compiler(for example g++):
g++ main.cpp -o program_name
program_name.exe

- Enter the maze dimensions (width and height). Both must be odd numbers and at least 5.

*Notes*
- Efficiency and Complexity
Maze generation uses Depth-First Search (DFS) for maze carving.
DFS visits each cell once, so the time complexity is O(N * M), where N and M are maze height and width.

Safe pathfinding uses a modified Breadth-First Search (BFS) that tracks how many traps have been passed.
Since the player can pass up to 2 traps safely, states are tracked with 3 layers (0,1,2 traps).
The maximum number of states is 3 * N * M.
Pathfinding time complexity is therefore O(N * M).

Traps are placed randomly on and off the path with constraints.
If placement is invalid, generation restarts.
Treasure placement checks reachability using BFS — also O(N * M).

I spent about 8.5 hours on this task