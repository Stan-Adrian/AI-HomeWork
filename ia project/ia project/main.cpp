#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

struct AStar_Node {
    int city;
    vector<int> path;
    int path_size;
    int cost;
    int max_edge;

    bool operator<(const AStar_Node& other) const {
        return cost > other.cost; 
    }
};

int calculate_max_edge(const vector<int>& path, const vector<vector<int>>& dist) {
    int max_edge = 0;
    for (size_t i = 1; i < path.size(); i++) {
        int u = path[i - 1];
        int v = path[i];
        max_edge = max(max_edge, dist[u][v]);
    }
    return max_edge;
}

int heuristic(const vector<int>& path, const vector<vector<int>>& dist, int num_cities) {
    int total = 0;
    int count = 0;
    for (int i = 0; i < num_cities; i++) {
        for (int j = 0; j < num_cities; j++) {
            if (dist[i][j] != numeric_limits<int>::max()) {
                total += dist[i][j];
                count++;
            }
        }
    }
    return (count > 0) ? total / count : 0;
}

void TSP_A_STAR(const vector<vector<int>>& dist) {
    int num_cities = dist.size();
    priority_queue<AStar_Node> pq;

    int min_cost = numeric_limits<int>::max();
    vector<int> best_path;

    AStar_Node initial_node = { 0, {0}, 1, 0, 0 };
    pq.push(initial_node);

    while (!pq.empty()) {
        AStar_Node curr = pq.top();
        pq.pop();

        if (curr.path_size == num_cities + 1 && curr.path.back() == 0) {
            int max_edge = calculate_max_edge(curr.path, dist);
            if (max_edge < min_cost) {
                min_cost = max_edge;
                best_path = curr.path;
            }
            continue;
        }

        for (int i = 0; i < num_cities; i++) {
            if (find(curr.path.begin(), curr.path.end(), i) == curr.path.end() ||
                (curr.path_size == num_cities && i == 0)) {
                AStar_Node next = curr;
                next.path.push_back(i);
                next.path_size++;
                next.cost += dist[curr.city][i] + heuristic(next.path, dist, num_cities);
                next.max_edge = max(next.max_edge, dist[curr.city][i]);
                next.city = i;
                pq.push(next);
            }
        }
    }

    cout << "A* TSP Path: ";
    for (int city : best_path) {
        cout << city << " ";
    }
    cout << "\nMaximum Edge: " << min_cost << endl;
}

struct UCS_Node {
    int city;
    int cost;
    int max_edge;
    int count;
    vector<bool> visited;
    vector<int> path;

    bool operator<(const UCS_Node& other) const {
        return cost > other.cost;
    }
};

void TSP_UCS(const vector<vector<int>>& dist) {
    int num_cities = dist.size();
    priority_queue<UCS_Node> pq;

    int min_cost = numeric_limits<int>::max();
    vector<int> best_path;

    UCS_Node initial_node = { 0, 0, 0, 1, vector<bool>(num_cities, false), {0} };
    initial_node.visited[0] = true;
    pq.push(initial_node);

    while (!pq.empty()) {
        UCS_Node curr = pq.top();
        pq.pop();

        if (curr.count == num_cities && dist[curr.city][0] != numeric_limits<int>::max()) {
            int cost = max(curr.max_edge, dist[curr.city][0]);
            if (cost < min_cost) {
                min_cost = cost;
                best_path = curr.path;
            }
            continue;
        }

        for (int i = 0; i < num_cities; i++) {
            if (!curr.visited[i] && dist[curr.city][i] != numeric_limits<int>::max()) {
                UCS_Node next = curr;
                next.city = i;
                next.cost += dist[curr.city][i];
                next.max_edge = max(next.max_edge, dist[curr.city][i]);
                next.count++;
                next.visited[i] = true;
                next.path.push_back(i);
                pq.push(next);
            }
        }
    }

    cout << "UCS TSP Path: ";
    for (int city : best_path) {
        cout << city << " ";
    }
    cout << "\nMinimum Cost: " << min_cost << endl;
}

struct Node {
    int city;
    vector<int> path;
    int path_size;
    int cost;
    int max_edge;

    bool operator<(const Node& other) const {
        return cost > other.cost;
    }
};

void TSP_BFS(const vector<vector<int>>& dist) {
    int num_cities = dist.size();
    queue<Node> q;

    int min_cost = numeric_limits<int>::max();
    vector<int> best_path;

    Node initial_node = { 0, {0}, 1, 0, 0 };
    q.push(initial_node);

    while (!q.empty()) {
        Node curr = q.front();
        q.pop();

        if (curr.path_size == num_cities + 1 && curr.path.back() == 0) {
            int max_edge = calculate_max_edge(curr.path, dist);
            if (max_edge < min_cost) {
                min_cost = max_edge;
                best_path = curr.path;
            }
            continue;
        }

        for (int i = 0; i < num_cities; i++) {
            if (find(curr.path.begin(), curr.path.end(), i) == curr.path.end() ||
                (curr.path_size == num_cities && i == 0)) {
                Node next = curr;
                next.path.push_back(i);
                next.path_size++;
                next.cost += dist[curr.city][i];
                next.max_edge = max(next.max_edge, dist[curr.city][i]);
                next.city = i;
                q.push(next);
            }
        }
    }

    cout << "BFS TSP Path: ";
    for (int city : best_path) {
        cout << city << " ";
    }
    cout << "\nMaximum Edge: " << min_cost << endl;
}

int main() {
    vector<vector<int>> dist = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    TSP_BFS(dist);
    TSP_UCS(dist);
    TSP_A_STAR(dist);
    return 0;
}
