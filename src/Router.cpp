#include "Router.hpp"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <numbers>
#include <algorithm>

namespace dmrc {

// Helper for A* priority queue
struct Node {
    std::string id;
    double f_score;
    
    // Min-heap comparison
    bool operator>(const Node& other) const {
        return f_score > other.f_score;
    }
};

double Router::heuristic(const Station* a, const Station* b) const {
    if (!a || !b) return 0.0;
    
    // Haversine formula
    constexpr double R = 6371.0; // Earth's radius in KM
    double lat1 = a->latitude * std::numbers::pi / 180.0;
    double lat2 = b->latitude * std::numbers::pi / 180.0;
    double dlat = (b->latitude - a->latitude) * std::numbers::pi / 180.0;
    double dlon = (b->longitude - a->longitude) * std::numbers::pi / 180.0;

    double x = std::sin(dlat / 2) * std::sin(dlat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dlon / 2) * std::sin(dlon / 2);
    double y = 2 * std::atan2(std::sqrt(x), std::sqrt(1 - x));
    
    return R * y;
}

RouteResult Router::find_path(const std::string& start_id, 
                             const std::string& end_id, 
                             double line_penalty) {
    const Station* start_node = graph_.get_station(start_id);
    const Station* end_node = graph_.get_station(end_id);

    if (!start_node || !end_node) return {{}, 0.0, false};

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_set;
    std::unordered_map<std::string, std::string> came_from;
    std::unordered_map<std::string, std::string> arrival_line; // Track line used to reach station
    std::unordered_map<std::string, double> g_score;

    for (const auto& [id, _] : graph_.get_stations()) {
        g_score[id] = std::numeric_limits<double>::infinity();
    }

    g_score[start_id] = 0.0;
    open_set.push({start_id, heuristic(start_node, end_node)});

    while (!open_set.empty()) {
        std::string current = open_set.top().id;
        if (current == end_id) {
            // Reconstruct path
            std::vector<std::string> path;
            std::string temp = current;
            while (temp != start_id) {
                path.push_back(temp);
                temp = came_from[temp];
            }
            path.push_back(start_id);
            std::reverse(path.begin(), path.end());
            return {path, g_score[end_id], true};
        }

        open_set.pop();

        const Station* current_station = graph_.get_station(current);
        for (const auto& edge : current_station->connections) {
            double penalty = 0.0;
            
            // Apply line change penalty
            if (came_from.contains(current)) {
                if (arrival_line[current] != edge.line) {
                    penalty = line_penalty;
                }
            }

            double tentative_g = g_score[current] + edge.distance + penalty;
            
            if (tentative_g < g_score[edge.target_id]) {
                came_from[edge.target_id] = current;
                arrival_line[edge.target_id] = edge.line;
                g_score[edge.target_id] = tentative_g;
                double f = tentative_g + heuristic(graph_.get_station(edge.target_id), end_node);
                open_set.push({edge.target_id, f});
            }
        }
    }

    return {{}, 0.0, false};
}

} // namespace dmrc
