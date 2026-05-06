# DMRC 3D Pathfinding Visualizer

A professional-grade, high-performance 3D visualization tool for the Delhi Metro Rail Corporation (DMRC) network. This project provides a real-time, interactive pathfinding experience, utilizing WebAssembly for optimized routing algorithms and Three.js for immersive 3D rendering.

## 🚀 Key Features

*   **Optimized WASM Pathfinding**: High-performance A* routing engine written in C++ and compiled to WebAssembly for near-native speed
*   **Immersive 3D Visualization**: Professional-grade Three.js scene featuring glowing network nodes, dynamic path highlighting, and animated metro trains
*   **Real-time Metro Train Animation**: Watch a glowing cyan metro train travel along the calculated shortest path
*   **Interactive Cyberpunk UI**: Sleek glassmorphism control panel with responsive design and real-time path statistics
*   **Contextual Station Tooltips**: Hover over stations to see detailed information including connected metro lines
*   **Line Change Penalties**: Smart routing that considers the cost of switching metro lines
*   **Haversine Distance Calculation**: Accurate geographic distance estimation using the Haversine formula

## 🛠 Tech Stack

*   **Graphics & 3D**: [Three.js](https://threejs.org/) r128 with OrbitControls
*   **Core Engine**: C++20 compiled with [Emscripten (WASM)](https://emscripten.org/) 5.0.7
*   **Styling**: [Tailwind CSS](https://tailwindcss.com/) via CDN
*   **Icons**: [Lucide](https://lucide.dev/) for modern iconography
*   **Data Format**: JSON for metro network configuration
*   **Build System**: Shell automation for C++ to WASM compilation

## 📦 Getting Started

### Prerequisites
- **Emscripten SDK** (emsdk) for compiling the C++ engine
- **Python 3** for the local development server
- **Git** for version control

### Installation

1. **Clone the repository**
```bash
git clone <repository-url>
cd dmrc-visualizer
```

2. **Install Emscripten**
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source emsdk_env.sh
```

3. **Build and Run**
```bash
cd ..
./build_and_run.sh
```

This will automatically:
1. Compile the C++ core into `dmrc_engine.js` and `dmrc_engine.wasm`
2. Spin up a local server at `http://localhost:8000`

Open your browser and navigate to `http://localhost:8000` to view the application.

## 🏗 Project Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     BROWSER (Frontend)                         │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │  index.html (Main Application)                          │  │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  │  │
│  │  │   Three.js   │  │  Tailwind    │  │   Lucide     │  │  │
│  │  │   3D Scene   │  │     CSS      │  │   Icons      │  │  │
│  │  └──────────────┘  └──────────────┘  └──────────────┘  │  │
│  │                                                             │  │
│  │  ┌────────────────────────────────────────────────────┐   │  │
│  │  │  JavaScript Logic                                  │   │  │
│  │  │  - Station Data Loading (JSON)                     │   │  │
│  │  │  - 3D Rendering (Stations, Tracks, Train)          │   │  │
│  │  │  - User Interaction (Hover, Click, Drag)           │   │  │
│  │  │  - WASM Interface                                  │   │  │
│  │  └────────────────────────────────────────────────────┘   │  │
│  └──────────────────────────────────────────────────────────┘  │
│                          │                                       │
│                          │ JavaScript ↔ WASM Bridge              │
│                          ▼                                       │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │        WebAssembly Module (dmrc_engine.wasm)             │  │
│  │  ┌────────────────────────────────────────────────────┐  │  │
│  │  │  C++ Compiled Functions                            │  │  │
│  │  │  - MetroGraph::load_from_string()                  │  │  │
│  │  │  - Router::find_path() (A* Algorithm)              │  │  │
│  │  └────────────────────────────────────────────────────┘  │  │
│  └──────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                   DATA LAYER                                    │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │  data/dmrc_data.json                                     │  │
│  │  {                                                       │  │
│  │    "stations": [                                         │  │
│  │      { "id": "HUDA", "name": "HUDA City Centre",        │  │
│  │        "lat": 28.4593, "lon": 77.0727,                  │  │
│  │        "lines": ["Yellow"] },                            │  │
│  │      ...                                                 │  │
│  │    ],                                                    │  │
│  │    "connections": [                                      │  │
│  │      { "source": "HUDA", "target": "IFFCO",             │  │
│  │        "distance": 1.4, "line": "Yellow" },             │  │
│  │      ...                                                 │  │
│  │    ]                                                     │  │
│  │  }                                                       │  │
│  └──────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
```

## 📁 Project Structure

```text
dmrc-visualizer/
├── data/                      # Metro network data
│   ├── dmrc_data.json        # Station and connection data
│   └── dmrc_full_data.json   # Extended dataset (optional)
├── include/                   # C++ header files
│   ├── Graph.hpp             # Metro graph data structures
│   ├── Router.hpp            # A* routing algorithm interface
│   └── nlohmann/json.hpp     # JSON parsing library
├── src/                       # C++ source files
│   ├── Graph.cpp             # Graph implementation
│   ├── Router.cpp            # A* algorithm implementation
│   └── WasmBridge.cpp        # Emscripten bindings
├── css/                       # Stylesheets
│   └── styles.css            # Custom CSS overrides
├── js/                        # JavaScript modules
│   ├── main.js               # Main application logic
│   ├── visualizer.js         # Three.js 3D rendering
│   └── ui.js                 # UI interaction handlers
├── index.html                 # Main HTML file
├── app.js                     # Legacy JavaScript (deprecated)
├── build_and_run.sh           # Build automation script
├── dmrc_engine.js            # Compiled WASM JavaScript glue
├── dmrc_engine.wasm          # Compiled WASM binary
├── convert_csv.py            # CSV to JSON converter utility
└── README.md                 # This file
```

## 🧮 Algorithm: A* Pathfinding with Line Change Penalties

### Overview

The routing engine uses the **A* (A-Star)** algorithm, a heuristic search algorithm that finds the shortest path between two nodes in a weighted graph. Our implementation includes:

1. **Haversine Distance** as the heuristic function for geographic accuracy
2. **Line Change Penalties** to minimize metro line transfers
3. **Priority Queue** for efficient node selection

### Algorithm Flow Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                    A* ALGORITHM FLOW                        │
└─────────────────────────────────────────────────────────────┘

    START
      │
      ▼
┌─────────────────┐
│ Initialize      │
│ - g_score[start] = 0
│ - g_score[others] = ∞
│ - open_set = {start}
│ - came_from = {} │
└─────────────────┘
      │
      ▼
┌─────────────────┐     NO
│ open_set empty? │────────▶ RETURN FAILURE
└─────────────────┘
      │ YES
      ▼
┌─────────────────┐     YES
│ current == goal?│────────▶ RECONSTRUCT PATH
└─────────────────┘
      │ NO
      ▼
┌─────────────────┐
│ current = pop   │
│ lowest f_score  │
│ from open_set   │
└─────────────────┘
      │
      ▼
┌─────────────────┐
│ For each neighbor│
│ of current:      │
└─────────────────┘
      │
      ▼
┌─────────────────────────────────────────────────────────┐
│ Calculate tentative_g = g_score[current] +             │
│   edge.distance + line_change_penalty                  │
│                                                         │
│ line_change_penalty = 5.0 km if switching lines,       │
│                      else 0.0                          │
└─────────────────────────────────────────────────────────┘
      │
      ▼
┌─────────────────────────┐     YES
│ tentative_g < g_score  │────────▶ UPDATE:
│ [neighbor]?            │           - came_from[neighbor] = current
└─────────────────────────┘           - g_score[neighbor] = tentative_g
      │ NO                              - f = g + heuristic(neighbor, goal)
      │                                 - add neighbor to open_set
      ▼
┌─────────────────┐
│ Continue Loop   │
└─────────────────┘
```

### Haversine Formula

The heuristic function uses the **Haversine formula** to calculate the great-circle distance between two points on a sphere (Earth):

```
                    ┌─────────────────────────────────────┐
                    │     HAVERSINE FORMULA               │
                    └─────────────────────────────────────┘

a = sin²(Δlat/2) + cos(lat1) × cos(lat2) × sin²(Δlon/2)
c = 2 × atan2(√a, √(1−a))
d = R × c

Where:
- lat1, lat2: Latitude of points in radians
- lon1, lon2: Longitude of points in radians
- Δlat = lat2 - lat1
- Δlon = lon2 - lon1
- R = Earth's radius (6371 km)
- d = Distance between points
```

### Example: Finding Path from HUDA to Sikanderpur

```
Initial State:
┌─────────────────────────────────────────────────────────┐
│ Stations: HUDA → IFFCO → MG_ROAD → SIKANDERPUR         │
│                                                         │
│ Start: HUDA                                             │
│ Goal: SIKANDERPUR                                       │
│ Line Change Penalty: 5.0 km                             │
└─────────────────────────────────────────────────────────┘

Step 1: Expand HUDA
┌─────────────────────────────────────────────────────────┐
│ Current: HUDA                                           │
│ Neighbors: IFFCO (1.4 km, Yellow line)                 │
│                                                         │
│ g_score[IFFCO] = 0 + 1.4 = 1.4                         │
│ f_score[IFFCO] = 1.4 + heuristic(IFFCO, SIKANDERPUR)   │
└─────────────────────────────────────────────────────────┘

Step 2: Expand IFFCO
┌─────────────────────────────────────────────────────────┐
│ Current: IFFCO                                          │
│ Neighbors:                                               │
│   - HUDA (1.4 km, already visited)                      │
│   - MG_ROAD (1.1 km, Yellow line)                      │
│                                                         │
│ g_score[MG_ROAD] = 1.4 + 1.1 = 2.5                     │
│ f_score[MG_ROAD] = 2.5 + heuristic(MG_ROAD, SIKANDERPUR)│
└─────────────────────────────────────────────────────────┘

Step 3: Expand MG_ROAD
┌─────────────────────────────────────────────────────────┐
│ Current: MG_ROAD                                        │
│ Neighbors:                                               │
│   - IFFCO (1.1 km, already visited)                     │
│   - SIKANDERPUR (1.2 km, Yellow line) ← GOAL!          │
│                                                         │
│ g_score[SIKANDERPUR] = 2.5 + 1.2 = 3.7                 │
│ Goal reached! Reconstruct path...                       │
└─────────────────────────────────────────────────────────┘

Final Path: HUDA → IFFCO → MG_ROAD → SIKANDERPUR
Total Distance: 3.7 km
```

## 🎨 3D Visualization Architecture

### Scene Composition

```
┌─────────────────────────────────────────────────────────────┐
│                    THREE.JS SCENE                          │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ CAMERA (Perspective)                                       │
│ Position: (30, 25, 30)                                     │
│ Controls: OrbitControls (drag, zoom, rotate)               │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│ LIGHTING                                                   │
│ - AmbientLight (base illumination)                         │
│ - DirectionalLight (shadows)                               │
│ - PointLights (cyan & purple accent lights)                │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│ OBJECTS                                                    │
│ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐           │
│ │  Stations   │ │   Tracks    │ │  Path High  │           │
│ │ (Spheres)   │ │  (Tubes)    │ │  (Glow Tube)│           │
│ │ + Glow      │ │ + Curved    │ │ + Arrows    │           │
│ └─────────────┘ └─────────────┘ └─────────────┘           │
│ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐           │
│ │ Metro Train │ │ Grid Helper │ │ Ground      │           │
│ │ (Box + Light)│ │ (Spatial)   │ │ (Plane)     │           │
│ └─────────────┘ └─────────────┘ └─────────────┘           │
└─────────────────────────────────────────────────────────────┘
```

### Metro Train Animation

```
┌─────────────────────────────────────────────────────────────┐
│              TRAIN ANIMATION SYSTEM                         │
└─────────────────────────────────────────────────────────────┘

Path Curve (CatmullRomCurve3)
┌─────────────────────────────────────────────────────────────┐
│ Station 1 ●────●────●────●────● Station N                  │
│           │    │    │    │    │                             │
│           t=0  t=0.25 t=0.5 t=0.75 t=1                    │
└─────────────────────────────────────────────────────────────┘

Animation Loop:
┌─────────────────────────────────────────────────────────────┐
│ 1. Calculate position at current progress (t)              │
│    position = curve.getPointAt(t)                          │
│                                                             │
│ 2. Calculate tangent at current progress                   │
│    tangent = curve.getTangentAt(t)                         │
│                                                             │
│ 3. Update train position                                    │
│    train.position.copy(position)                           │
│                                                             │
│ 4. Orient train along path                                  │
│    train.lookAt(position + tangent)                        │
│                                                             │
│ 5. Increment progress                                      │
│    t += speed                                               │
│    if t >= 1: t = 0 (loop)                                 │
└─────────────────────────────────────────────────────────────┘
```

### Coordinate Transformation

Geographic coordinates (lat/lon) are transformed to 3D Cartesian coordinates:

```
┌─────────────────────────────────────────────────────────────┐
│       LAT/LON TO 3D COORDINATE TRANSFORMATION               │
└─────────────────────────────────────────────────────────────┘

Input: (lat, lon) in degrees
Output: (x, y, z) in 3D space

x = (lon - offset_lon) × scale
y = 0 (all stations on ground plane)
z = (lat - offset_lat) × scale

Where:
- scale = 400 (zoom level)
- offset_lon = 77.15 (center longitude)
- offset_lat = 28.6 (center latitude)

Example:
Station: HUDA City Centre
lat: 28.4593, lon: 77.0727

x = (77.0727 - 77.15) × 400 = -30.92
y = 0
z = (28.4593 - 28.6) × 400 = -56.28
```

## 🎮 User Interface

### Control Panel Layout

```
┌─────────────────────────────────────────────────────────────┐
│  ┌─────────────────────────────────────────────────────┐   │
│  │  🗺️ DMRC NAV                        ● WASM ACTIVE  │   │
│  │  3D Route Visualization System                      │   │
│  │                                                     │   │
│  │  📍 SOURCE STATION                      ┌─────────┐ │   │
│  │  ┌─────────────────────────────────┐   │ Select  │ │   │
│  │  │ HUDA City Centre              ▼   └─────────┘ │   │
│  │  └─────────────────────────────────┘               │   │
│  │                                                     │   │
│  │  🏁 DESTINATION STATION               ┌─────────┐ │   │
│  │  ┌─────────────────────────────────┐   │ Select  │ │   │
│  │  │ Sikanderpur                    ▼   └─────────┘ │   │
│  │  └─────────────────────────────────┘               │   │
│  │                                                     │   │
│  │  ⚡ COMPUTE OPTIMAL ROUTE                           │   │
│  │                                                     │   │
│  │  ┌─────────────────────────────────────────────┐   │   │
│  │  │ ROUTE RESULTS                   ⏱ 0.4ms   │   │   │
│  │  │ ┌───────────┐ ┌───────────┐                 │   │   │
│  │  │ │STATIONS: 4│ │DIST: 3.7km│                 │   │   │
│  │  │ └───────────┘ └───────────┘                 │   │   │
│  │  │                                             │   │   │
│  │  │ ● HUDA City Centre (Yellow)                │   │   │
│  │  │   │                                         │   │   │
│  │  │   ▼                                         │   │   │
│  │  │ ● IFFCO Chowk (Yellow)                     │   │   │
│  │  │   │                                         │   │   │
│  │  │   ▼                                         │   │   │
│  │  │ ● MG Road (Yellow)                         │   │   │
│  │  │   │                                         │   │   │
│  │  │   ▼                                         │   │   │
│  │  │ ● Sikanderpur (Yellow, Rapid)              │   │   │
│  │  └─────────────────────────────────────────────┘   │   │
│  │                                                     │   │
│  │  METRO LINES                                       │   │
│  │  ● Yellow ● Blue ● Magenta ● Green ● Red ● Violet │   │
│  └─────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
```

### Interaction Features

- **Orbit Controls**: Drag to rotate, scroll to zoom, right-click to pan
- **Station Hover**: Hover over stations to see name and connected lines
- **Route Selection**: Choose source and destination from dropdowns
- **Real-time Animation**: Watch metro train travel along calculated path
- **Responsive Design**: Works on desktop and mobile devices

## 🔧 Configuration

### Data Format (JSON)

The metro network is defined in `data/dmrc_data.json`:

```json
{
  "stations": [
    {
      "id": "HUDA",
      "name": "HUDA City Centre",
      "lat": 28.4593,
      "lon": 77.0727,
      "lines": ["Yellow"]
    }
  ],
  "connections": [
    {
      "source": "HUDA",
      "target": "IFFCO",
      "distance": 1.4,
      "line": "Yellow"
    }
  ]
}
```

### Line Colors

| Line | Color | Hex Code |
|------|-------|----------|
| Yellow | Gold | #FFD700 |
| Blue | Royal Blue | #0066FF |
| Magenta | Magenta | #FF00FF |
| Green | Lime | #00FF00 |
| Red | Red | #FF0000 |
| Violet | Purple | #8B00FF |
| Rapid | Dark Orange | #FF8C00 |
| Pink | Hot Pink | #FF69B4 |
| Aqua | Cyan | #00FFFF |
| Gray | Gray | #808080 |

## 📊 Performance Metrics

- **WASM Compilation Time**: ~2-3 seconds
- **Path Calculation**: <1ms for typical routes
- **3D Rendering**: 60 FPS on modern hardware
- **Memory Usage**: ~50MB (including WASM module)
- **Network Size**: Supports 100+ stations efficiently

## 🚧 Future Enhancements

- [ ] Add real-time metro train schedules
- [ ] Implement Dijkstra algorithm as alternative
- [ ] Add accessibility features for screen readers
- [ ] Support for multiple metro networks
- [ ] Export route as GPX/KML
- [ ] Add station search functionality
- [ ] Implement multi-modal routing (metro + bus)

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:

1. **Code Style**: Follow existing code style and conventions
2. **Documentation**: Update README for new features
3. **Testing**: Test thoroughly before submitting PRs
4. **Modular Architecture**: Keep logic separated and maintainable

### Adding New Stations

1. Update `data/dmrc_data.json` with station details
2. Ensure lat/lon coordinates are accurate
3. Add connections to neighboring stations
4. Specify metro line colors

### Modifying Routing Algorithm

1. Edit `src/Router.cpp` for algorithm changes
2. Update `include/Router.hpp` for interface changes
3. Recompile WASM using `./build_and_run.sh`
4. Test with various route combinations

## 📄 License

This project is open source and available for educational purposes.

## 🙏 Acknowledgments

- **Three.js** for the powerful 3D graphics library
- **Emscripten** for WebAssembly compilation
- **Delhi Metro Rail Corporation** for the network data
- **nlohmann/json** for efficient JSON parsing in C++

---

**Built with ❤️ using C++, WebAssembly, and Three.js**
