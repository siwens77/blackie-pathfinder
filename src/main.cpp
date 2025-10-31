#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <fstream>
#include <queue>
#include <climits>
#include <random>
#include <limits>
#include <cmath>
#include <chrono>
using namespace std;

vector<vector<tuple<int, int, int>>> createGraph(int,int,int,int);
int print_graph(vector<vector<tuple<int, int, int>>>& graph);
tuple<int, int, int, int> readInput();
bool isPrime(int);
tuple<vector<int>, int, int> dijkstra(int, vector<vector<tuple<int,int,int>>>&, int, int, int, bool, double);
void printPath(const vector<int>&, int);
int calculatePath(const vector<int>& , vector<vector<tuple<int, int, int>>>);
float calculateProb(int, int, int, double);

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start_time = chrono::high_resolution_clock::now();
    double time_limit = 19.0; 
    double add_prob = 0;
    
    int num_nodes, num_edges, start, target;
    tie(num_nodes, num_edges, start, target) = readInput();
    vector<vector<tuple<int, int, int>>> graph = createGraph(num_nodes, num_edges, start, target);
    //print_graph(graph);
    constexpr double i = INT_MAX;
    bool first = true;
    auto [path, numNodes, totalDist] = dijkstra(num_nodes, graph, start, target, i, first, add_prob);
    //cout<<"\nfirst distance:"<<totalDist<<"\n";
    first = false;
    //printPath(path, numNodes);
    int firstDist = totalDist;

    while(true){
        auto current_time = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = current_time - start_time;
        if (elapsed.count() >= time_limit) break;
        if(elapsed.count()>= time_limit*7.5/10.0 && firstDist==totalDist){add_prob +=0.1;}
        if(elapsed.count()>= time_limit*9.0/10.0 && firstDist==totalDist){add_prob +=0.1;}


        auto [new_path, new_numNodes, newDist] = dijkstra(num_nodes, graph, start, target, totalDist, first,add_prob);
        //printPath(new_path,new_numNodes);

        if(newDist<totalDist){
            totalDist = newDist;
            path = new_path;
            numNodes = new_numNodes;
        }
    }
    printPath(path, numNodes);
    //cout<<"\ntotal distance is: " << totalDist<<'\n';

    return 0;
}


tuple<int,int,int,int> readInput() {
    int num_nodes, num_edges, start, target;
    cin >> num_nodes >> num_edges >> start >> target;
    return make_tuple(num_nodes, num_edges, start, target);
}

vector<vector<tuple<int, int, int>>> createGraph(int num_nodes, int num_edges, int start, int target){

    vector<vector<tuple<int, int, int>>> graph(num_nodes);

    int n1, n2, w1, w2;
    while (cin >> n1 >> n2 >> w1 >> w2)
    {
        graph[n1].push_back({n2, w1, w2});
        graph[n2].push_back({n1, w1, w2});
    }
    return graph;
}

int print_graph(vector<vector<tuple<int, int, int>>>& graph){
    for (int i = 0; i < graph.size(); i++)
    {
        cout << "Node " << i << ":\n";
        for (auto& edge : graph[i])
        {
            int target, w1, w2;
            tie(target, w1, w2) = edge;
            cout << "  -> " << target << " (w1: " << w1 << ", w2: " << w2 << ")\n";
        }
    }
    return 0;
}

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i*i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

tuple<vector<int>, int, int> dijkstra(int V, vector<vector<tuple<int,int,int>>> &edges, int start, int target, int bestDist, bool first, double add_prob) {
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> pq;
    vector<int> dist(V, INT_MAX);
    vector<int> parent(V, -1);

    pq.push({0, start, 0});
    dist[start] = 0;

    random_device rd;//PROB
    mt19937 gen(rd());
    uniform_real_distribution<> rando(0.0, 1.0);
    int iter = 0;

    while(!pq.empty()) {
        iter++;
        auto top = pq.top();
        pq.pop(); 

        int d = top[0];
        int u = top[1];
        int edge_counter = top[2];
        if (d > dist[u]) continue;
        //if(u == target) break;

        for(auto edge : edges[u]) {
            auto [vertex, w1, w2] = edge;       
            int next_edge_count = edge_counter + 1;
            int weight = isPrime(next_edge_count) ? 3*w2 : w1;
            
            //if (dist[u] + weight > bestDist){
            //continue;}

            float p = first? 1.0f : calculateProb(next_edge_count, iter, V, add_prob);//probability

            if(dist[vertex] > dist[u] + weight && rando(gen)<p ){
                dist[vertex] = dist[u] + weight;
                pq.push({dist[vertex], vertex, next_edge_count});
                parent[vertex] = u; 
            }
        }
    }


    if (parent[target] == -1 && target != start) {
    return { {}, 0, INT_MAX };
    }


    vector<int> path;
    for (int cur = target; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }reverse(path.begin(), path.end());

    int numNodes = path.size();
    int totalDist = dist[target];//wrong probably?

    int base_dist = calculatePath(path, edges);
    return {path, numNodes, base_dist};
    }

void printPath(const vector<int>& path, int numNodes) {
    cout << numNodes << "\n";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i != path.size() - 1) cout << " ";
    }cout<<"\n";
}

int calculatePath(const vector<int>& path, vector<vector<tuple<int, int, int>>> graph){
    int distance = 0;
    for(int i=0; i+1<path.size(); i++){
        int u = path[i];
        int v = path[i+1];
        for (auto& [to, w1, w2] : graph[u]) {
            if(to==v){
                distance+=isPrime(i+1)? 3*w2:w1;
                break;
            }
        }
    }
    return distance;
}

float calculateProb(int next_edge_count, int iter, int numNodes, double add_prob){
    //cout<<next_edge_count<<" "<<iter<<" "<<numNodes<<" "<<numNodes/next_edge_count<<"\n";

    if(isPrime(next_edge_count)){
        return 0.99f;
    }
    if(numNodes/next_edge_count>=iter){
        return (0.9f-add_prob/2);
    }
    if(iter> numNodes/2 || iter==0){
        return 0.97f;
    }
    if(isPrime(next_edge_count+2)){
        return (0.9f);
    }
    if(iter<numNodes/5){
        return (0.8f-add_prob/8);
    }
    return (0.98f-add_prob/4);
}
