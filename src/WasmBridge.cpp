#include <emscripten/bind.h>
#include "Graph.hpp"
#include "Router.hpp"

using namespace emscripten;
using namespace dmrc;

EMSCRIPTEN_BINDINGS(dmrc_bridge) {
    // Bind the RouteResult struct
    register_vector<std::string>("StringVector");

    value_object<RouteResult>("RouteResult")
        .field("path", &RouteResult::path)
        .field("total_distance", &RouteResult::total_distance)
        .field("success", &RouteResult::success);

    // Bind the MetroGraph class
    class_<MetroGraph>("MetroGraph")
        .constructor<>()
        .function("load_from_string", &MetroGraph::load_from_string)
        .function("load_from_json", &MetroGraph::load_from_json);

    // Bind the Router class
    class_<Router>("Router")
        .constructor<const MetroGraph&>()
        .function("find_path", &Router::find_path);
}
