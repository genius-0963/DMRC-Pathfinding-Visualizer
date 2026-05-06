#include "Graph.hpp"
#include "Router.hpp"
#include <iostream>

int main() {
    dmrc::MetroGraph graph;
    if (!graph.load_from_json("data/dmrc_data.json")) {
        return 1;
    }

    dmrc::Router router(graph);
    
    std::cout << "Testing Route: HUDA -> SIKANDERPUR..." << std::endl;
    auto result = router.find_path("HUDA", "SIKANDERPUR");

    if (result.success) {
        std::cout << "Path Found! Distance: " << result.total_distance << " KM" << std::endl;
        for (const auto& id : result.path) {
            std::cout << " -> " << id;
        }
        std::cout << std::endl;
    } else {
        std::cout << "Path not found." << std::endl;
    }

    return 0;
}
