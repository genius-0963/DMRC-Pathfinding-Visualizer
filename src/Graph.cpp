#include "Graph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace dmrc {

bool MetroGraph::load_from_json(const std::string& json_path) {
    std::ifstream file(json_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open: " << json_path << std::endl;
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return load_from_string(buffer.str());
}

bool MetroGraph::load_from_string(const std::string& json_str) {
    json data;
    try {
        data = json::parse(json_str);
    } catch (const json::parse_error& e) {
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
        return false;
    }

    // Parse Stations
    for (const auto& item : data["stations"]) {
        Station station;
        station.id = item["id"].get<std::string>();
        station.name = item["name"].get<std::string>();
        station.latitude = item["lat"].get<double>();
        station.longitude = item["lon"].get<double>();
        station.lines = item["lines"].get<std::vector<std::string>>();
        
        stations_[station.id] = std::move(station);
    }

    // Parse Connections (Adjacency List)
    for (const auto& item : data["connections"]) {
        std::string u = item["source"].get<std::string>();
        std::string v = item["target"].get<std::string>();
        double dist = item["distance"].get<double>();
        std::string line = item["line"].get<std::string>();

        if (stations_.contains(u) && stations_.contains(v)) {
            // Bidirectional graph for Metro tracks
            stations_[u].connections.push_back({v, dist, line});
            stations_[v].connections.push_back({u, dist, line});
        }
    }

    std::cout << "Graph loaded: " << stations_.size() << " stations." << std::endl;
    return true;
}

const Station* MetroGraph::get_station(const std::string& id) const {
    auto it = stations_.find(id);
    return (it != stations_.end()) ? &(it->second) : nullptr;
}

} // namespace dmrc
