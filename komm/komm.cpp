#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

const int INF = INT_MAX;

class TSPSolver {
private:
    vector<vector<int>> graph;
    int n;

    int calculateCost(vector<int>& path) {
        int cost = 0;
        for (int i = 0; i < n - 1; ++i) {
            cost += graph[path[i]][path[i + 1]];
        }
        cost += graph[path[n - 1]][path[0]]; // возврат в исходный город
        return cost;
    }


    void bruteForceTSP(vector<bool>& visited, int currentLength, int& minLength, vector<int>& minPath) {
        vector<int> path;
        path.push_back(0);  // начинаем с первого города

        bruteForceTSPHelper(path, visited, currentLength, minLength, minPath);
    }

    void bruteForceTSPHelper(vector<int>& path, vector<bool>& visited, int currentLength, int& minLength, vector<int>& minPath) {
        if (path.size() == n) {
            int totalCost = currentLength + graph[path.back()][0];
            if (totalCost < minLength) {
                minLength = totalCost;
                minPath = path;
            }
            return;
        }

        for (int i = 1; i < n; ++i) { // Начинаем с 1 города потому что 0 уже занят
            if (!visited[i]) {
                visited[i] = true;
                path.push_back(i);
                bruteForceTSPHelper(path, visited, currentLength + graph[path[path.size() - 2]][path.back()], minLength, minPath);
                path.pop_back();
                visited[i] = false;
            }
        }
    }


    int findNearestNeighbor(int current, vector<bool>& visited) {
        int n = graph.size();
        int nearest = -1;
        int minDistance = INF;

        for (int i = 0; i < n; ++i) {
            if (!visited[i] && i != current && graph[current][i] < minDistance) {
                minDistance = graph[current][i];
                nearest = i;
            }
        }

        return nearest;
    }

   void nearestNeighborTSP(vector<int>& minPath, int& minLength) {
       int n = graph.size();
       vector<bool> visited(n, false);
    vector<int> path;

    int current = 0; // начинаем с первого города
    path.push_back(current);
    visited[current] = true;

    while (path.size() < n) {
        int next = findNearestNeighbor(current, visited);
        if (next == -1) break;
        path.push_back(next);
        visited[next] = true;
        current = next;
    }

    minLength = calculateCost(path);
    minPath = path;
}



    void branchAndBoundTSP(vector<int>& minPath, int& minLength) {
        vector<bool> visited(n, false);
        vector<int> path;
        path.push_back(0); // Start with the first city

        minLength = INF;
        branchAndBoundTSPHelper(path, visited, 0, minLength, minPath);
    }

    void branchAndBoundTSPHelper(vector<int>& path, vector<bool>& visited, int currentLength, int& minLength, vector<int>& minPath) {
        if (path.size() == n) {
            int totalCost = currentLength + graph[path.back()][0];
            if (totalCost < minLength) {
                minLength = totalCost;
                minPath = path;
            }
            return;
        }

        for (int i = 1; i < n; ++i) { // Начинаем с 1 города потому что 0 уже занят
            if (!visited[i]) {
                int edgeCost = graph[path.back()][i];
                if (currentLength + edgeCost < minLength) {
                    path.push_back(i);
                    visited[i] = true;
                    branchAndBoundTSPHelper(path, visited, currentLength + edgeCost, minLength, minPath);
                    visited[i] = false;
                    path.pop_back();
                }
            }
        }
    }


public:
    TSPSolver(vector<vector<int>>& _graph) : graph(_graph) {
        n = graph.size();
    }

    void solveTSP(int method) {
        vector<int> minPath;
        int minLength = INF;

        switch (method) {
        case 1:
        {
            vector<bool> visited(n, false);
            bruteForceTSP(visited, 0, minLength, minPath);
        }
        break;

        case 2:
            nearestNeighborTSP(minPath, minLength);
            break;
        case 3:
            branchAndBoundTSP(minPath, minLength);
            break;
        default:
            cout << "Неверный выбор метода!" << endl;
            return;
        }

        minPath.push_back(0); // Добавляем исходный город в конец пути только здесь

        cout << "Минимальная стоимость: " << minLength << endl;
        cout << "Путь: ";
        for (int city : minPath) {
            cout << city << " ";
        }
        cout << endl;
    }

};

int main() {
    setlocale(LC_ALL, ".1251");
    vector<vector<int>> graph = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    TSPSolver solver(graph);

    int method;
    while (true) {
        cout << "Выберите метод решения задачи Коммивояжёра:" << endl;
        cout << "1. Полный перебор" << endl;
        cout << "2. Метод ближайшего соседа" << endl;
        cout << "3. Метод ветвей и границ" << endl;
        cout << "0. Выход" << endl;
        cin >> method;

        if (method == 0) {
            cout << "Программа завершена." << endl;
            break;
        }
        else if (method == 1 || method == 2 || method == 3) {
            solver.solveTSP(method);
        }
        else {
            cout << "Неверный выбор метода! Пожалуйста, выберите снова." << endl;
            continue;
        }
    }

    return 0;
}
