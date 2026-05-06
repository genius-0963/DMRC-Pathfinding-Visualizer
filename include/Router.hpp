#pragma once

#include "Graph.hpp"
#include <vector>
#include <string>

namespace dmrc {

/**
 * @brief Structure to represent a result of the routing algorithm.
 */
struct RouteResult {
    std::vector<std::string> path; // List of station IDs
    double total_distance;
    bool success;
};

class Router {
public:
    explicit Router(const MetroGraph& graph) : graph_(graph) {}

    /**
     * @brief Find the shortest path using A* algorithm.
     * @param start_id Origin station ID
     * @param end_id Destination station ID
     * @param line_penalty Added weight (in KM) for switching lines
     */
    RouteResult find_path(const std::string& start_id, 
                         const std::string& end_id, 
                         double line_penalty = 5.0);

private:
    const MetroGraph& graph_;

    /**
     * @brief Heuristic function: Haversine distance.
     */
    double heuristic(const Station* a, const Station* b) const;
};

} // namespace dmrc
