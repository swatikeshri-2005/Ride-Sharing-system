#include <bits/stdc++.h>
using namespace std;

class Graph {
    int V;
    unordered_map<string, vector<pair<string, int>>> adj;

public:
    void addEdge(string u, string v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    pair<int, vector<string>> dijkstra(string src, string dest) {
        unordered_map<string, int> dist;
        unordered_map<string, string> parent;

        for (auto &p : adj)
            dist[p.first] = INT_MAX;

        priority_queue<pair<int,string>, vector<pair<int,string>>, greater<pair<int,string>>> pq;
        pq.push({0, src});
        dist[src] = 0;

        while (!pq.empty()) {
            auto node = pq.top();
            pq.pop();

            string u = node.second;

            if (u == dest) break;

            for (auto &nbr : adj[u]) {
                string v = nbr.first;
                int wt = nbr.second;

                if (dist[u] + wt < dist[v]) {
                    dist[v] = dist[u] + wt;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        vector<string> path;
        string temp = dest;

        if (dist[dest] == INT_MAX)
            return {INT_MAX, {}};

        while (temp != src) {
            path.push_back(temp);
            temp = parent[temp];
        }

        path.push_back(src);
        reverse(path.begin(), path.end());

        return {dist[dest], path};
    }

    bool nodeExists(string x){
        return adj.find(x) != adj.end();
    }

    void displayGraph() {
        cout << "\nCity Map (Graph):\n";
        for (auto &p : adj) {
            cout << p.first << " -> ";
            for (auto &x : p.second)
                cout << "(" << x.first << "," << x.second << "km) ";
            cout << endl;
        }
    }
};

struct Cab {
    string id;
    string location;
    bool available;

    Cab(string id, string loc) {
        this->id = id;
        location = loc;
        available = true;
    }
};

int main() {
    Graph city;

    // Sample City Map (You can modify / expand)
    city.addEdge("A", "B", 6);
    city.addEdge("A", "C", 3);
    city.addEdge("B", "D", 2);
    city.addEdge("C", "D", 4);
    city.addEdge("C", "E", 5);
    city.addEdge("D", "E", 1);

    vector<Cab> cabs = {
        Cab("CAB1", "A"),
        Cab("CAB2", "C"),
        Cab("CAB3", "E")
    };

    cout << "========== RIDE SHARING SYSTEM ==========\n";
    city.displayGraph();

    string source, destination;
    cout << "\nEnter your Pickup Location: ";
    cin >> source;
    cout << "Enter Destination: ";
    cin >> destination;

    if (!city.nodeExists(source) || !city.nodeExists(destination)) {
        cout << "\nInvalid Source or Destination!\n";
        return 0;
    }

    // Find Nearest Available Cab
    string chosenCab = "";
    string cabLocation = "";
    int minDist = INT_MAX;
    vector<string> cabPath;

    cout << "\nSearching Nearest Cab...\n";

    for (auto &cab : cabs) {
        if (!cab.available) continue;

        auto result = city.dijkstra(cab.location, source);

        if (result.first < minDist) {
            minDist = result.first;
            chosenCab = cab.id;
            cabLocation = cab.location;
            cabPath = result.second;
        }
    }

    if (chosenCab == "") {
        cout << "\nNo Cabs Available!\n";
        return 0;
    }

    cout << "\nNearest Cab Found!\n";
    cout << "Cab ID: " << chosenCab << endl;
    cout << "Cab Current Location: " << cabLocation << endl;
    cout << "Distance to Pickup: " << minDist << " km\n";

    cout << "Cab Route to Pickup: ";
    for (auto s : cabPath) cout << s << " ";
    cout << endl;

    // Trip Route
    auto trip = city.dijkstra(source, destination);

    if (trip.first == INT_MAX) {
        cout << "\nNo Route Found Between Pickup & Destination!\n";
        return 0;
    }

    cout << "\n=========== Ride Details ===========\n";
    cout << "Pickup: " << source << endl;
    cout << "Drop: " << destination << endl;

    cout << "Trip Route: ";
    for (auto s : trip.second) cout << s << " ";
    cout << endl;

    cout << "Trip Distance: " << trip.first << " km\n";

    // Fare Calculation
    int baseFare = 50;
    int perKm = 10;
    int fare = baseFare + trip.first * perKm;

    // ETA
    double speed = 40.0;
    double eta = (double)trip.first / speed * 60;

    cout << "Estimated Time: " << ceil(eta) << " minutes\n";
    cout << "Fare: ₹" << fare << endl;

    cout << "\nRide Booked Successfully! 🚕\n";
    return 0;
}
