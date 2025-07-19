#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

const char WALL = '#';
const char PATH = '.';
const char ENTRY = 'A';
const char EXIT = 'B';
const char TRAP = 'T';
const char TREASURE = '$';

struct Cell {
    int x, y;
};

class Maze {
private:
    int width, height;
    vector<vector<char>> grid;
    vector<vector<bool>> visited;
    mt19937 rng;

    const vector<Cell> directions = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    };

    struct State {
        int x, y;
        int trapsPassed;
    };

public:
    Maze(int w, int h) : width(w), height(h), rng((unsigned)time(0)) {
        if (width % 2 == 0) width++;
        if (height % 2 == 0) height++;
    }

    void generate() {
        while (true) {
            grid.assign(height, vector<char>(width, WALL));
            visited.assign(height, vector<bool>(width, false));

            dfs(1, 1);
            grid[1][0] = ENTRY;
            grid[height - 2][width - 1] = EXIT;

            if (place_traps_and_treasures())
                break;
        }
    }

    void dfs(int x, int y) {
        visited[y][x] = true;
        grid[y][x] = PATH;

        vector<Cell> dirs = directions;
        shuffle(dirs.begin(), dirs.end(), rng);

        for (const auto& d : dirs) {
            int nx = x + d.x * 2;
            int ny = y + d.y * 2;

            if (in_bounds(nx, ny) && !visited[ny][nx]) {
                grid[y + d.y][x + d.x] = PATH;
                dfs(nx, ny);
            }
        }
    }

    bool in_bounds(int x, int y) const {
        return x > 0 && y > 0 && x < width - 1 && y < height - 1;
    }

    vector<Cell> find_safe_path(Cell start, Cell goal) {
        vector<vector<vector<bool>>> visited(height, vector<vector<bool>>(width, vector<bool>(3, false)));
        vector<vector<vector<Cell>>> parent(height, vector<vector<Cell>>(width, vector<Cell>(3, {-1, -1})));

        queue<State> q;
        q.push({start.x, start.y, 0});
        visited[start.y][start.x][0] = true;

        while (!q.empty()) {
            State cur = q.front(); q.pop();

            if (cur.x == goal.x && cur.y == goal.y) {
                vector<Cell> path;
                int trapsCount = cur.trapsPassed;
                Cell c = {cur.x, cur.y};
                while (!(c.x == -1 && c.y == -1)) {
                    path.push_back(c);
                    Cell p = parent[c.y][c.x][trapsCount];
                    if (grid[c.y][c.x] == TRAP && trapsCount > 0) trapsCount--;
                    c = p;
                }
                reverse(path.begin(), path.end());
                return path;
            }

            for (const auto& d : directions) {
                int nx = cur.x + d.x;
                int ny = cur.y + d.y;
                if (in_bounds(nx, ny)) {
                    char cellChar = grid[ny][nx];
                    if (cellChar == WALL) continue;

                    int newTrapCount = cur.trapsPassed + (cellChar == TRAP ? 1 : 0);
                    if (newTrapCount > 2) continue;

                    if (!visited[ny][nx][newTrapCount]) {
                        visited[ny][nx][newTrapCount] = true;
                        parent[ny][nx][newTrapCount] = {cur.x, cur.y};
                        q.push({nx, ny, newTrapCount});
                    }
                }
            }
        }
        return {};
    }

    bool place_traps_and_treasures() {
        Cell start{1, 1};
        Cell exitC{width - 2, height - 2};

        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                if (grid[y][x] == TRAP || grid[y][x] == TREASURE)
                    grid[y][x] = PATH;

        uniform_int_distribution<int> trapCountDist(0, 5);
        int trapCount = trapCountDist(rng);

        auto path = find_safe_path(start, exitC);
        if (path.empty()) return false;

        int trapsOnPathMax = min(2, trapCount);

        vector<int> candidateIndices;
        for (int i = 1; i < (int)path.size() - 1; i++) {
            candidateIndices.push_back(i);
        }
        shuffle(candidateIndices.begin(), candidateIndices.end(), rng);

        int trapsPlacedOnPath = 0;
        int trapsPlacedTotal = 0;

        for (int idx : candidateIndices) {
            if (trapsPlacedOnPath >= trapsOnPathMax) break;
            Cell c = path[idx];
            if (grid[c.y][c.x] == PATH) {
                grid[c.y][c.x] = TRAP;
                trapsPlacedOnPath++;
                trapsPlacedTotal++;
            }
        }

        int trapsLeft = trapCount - trapsPlacedTotal;
        vector<Cell> freeCells;
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                if (grid[y][x] == PATH) {
                    bool onPath = false;
                    for (auto& pc : path) {
                        if (pc.x == x && pc.y == y) {
                            onPath = true;
                            break;
                        }
                    }
                    if (!onPath) freeCells.push_back({x, y});
                }
            }
        }
        shuffle(freeCells.begin(), freeCells.end(), rng);
        for (int i = 0; i < trapsLeft && i < (int)freeCells.size(); i++) {
            Cell c = freeCells[i];
            grid[c.y][c.x] = TRAP;
        }

        uniform_int_distribution<int> treasureChance(0, 1);
        bool placeTreasure = treasureChance(rng) == 1;

        if (placeTreasure) {
            vector<Cell> treasureCandidates;
            for (int y = 1; y < height - 1; y++) {
                for (int x = 1; x < width - 1; x++) {
                    char c = grid[y][x];
                    if (c == PATH || c == TRAP) {
                        treasureCandidates.push_back({x, y});
                    }
                }
            }
            shuffle(treasureCandidates.begin(), treasureCandidates.end(), rng);

            for (auto& c : treasureCandidates) {
                char oldChar = grid[c.y][c.x];
                grid[c.y][c.x] = TREASURE;

                auto pathToTreasure = find_safe_path(start, c);
                if (!pathToTreasure.empty()) {
                    return true;
                } else {
                    grid[c.y][c.x] = oldChar;
                }
            }
        }
        return true;
    }

    void print() const {
        for (const auto& row : grid) {
            for (char c : row) {
                switch (c) {
                case WALL:
                    cout << "\033[90m" << c << "\033[0m";
                    break;
                case PATH:
                    cout << "\033[97m" << c << "\033[0m";
                    break;
                case ENTRY:
                    cout << "\033[32m" << c << "\033[0m";
                    break;
                case TRAP:
                    cout << "\033[31m" << c << "\033[0m";
                    break;
                case TREASURE:
                    cout << "\033[33m" << c << "\033[0m";
                    break;
                case EXIT:
                    cout << "\033[35m" << c << "\033[0m";
                    break;
                default:
                    cout << c;
                }
            }
            cout << '\n';
        }
    }
};

int main() {
    int width, height;

    cout << "Enter the width of the maze (odd number >= 5): ";
    cin >> width;
    cout << "Enter the height of the maze (odd number >= 5): ";
    cin >> height;

    if (width < 5 || height < 5) {
        cout << "Error: maze dimensions must be at least 5x5." << endl;
        return 1;
    }

    Maze maze(width, height);
    maze.generate();
    maze.print();

    return 0;
}
