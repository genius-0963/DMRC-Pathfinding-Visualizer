#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace dmrc {

/**
 * @brief Represents a connection between two metro stations.
 */
struct Edge {
    std::string target_id;
    double distance; // Pre-calculated distance in KM
    std::string line; // Metro line color/name (e.g., "Yellow", "Blue")
};

/**
 * @brief Represents a Metro Station.
 */
struct Station {
    std::string id;
    std::string name;
    double latitude;
    double longitude;
    std::vector<std::string> lines;
    std::vector<Edge> connections;
};

/**
 * @brief Main Graph Engine for DMRC data.
 */
class MetroGraph {
public:
    MetroGraph() = default;

    /**
     * @brief Load station and connection data from a JSON file.
     * @param json_path Path to the data.json
     * @return true if loading was successful.
     */
    bool load_from_json(const std::string& json_path);

    /**
     * @brief Load station and connection data from a JSON string.
     * @param json_str The raw JSON string
     * @return true if loading was successful.
     */
    bool load_from_string(const std::string& json_str);

    const std::unordered_map<std::string, Station>& get_stations() const { return stations_; }
    
    // For A* later: Helper to get a station by ID
    const Station* get_station(const std::string& id) const;

private:
    std::unordered_map<std::string, Station> stations_;
};

} // namespace dmrc
