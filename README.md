# DMRC Pathfinding Visualizer

A professional-grade, high-performance 3D visualization tool for the Delhi Metro Rail Corporation (DMRC) network. This project provides a real-time, interactive pathfinding experience, utilizing WebAssembly for optimized routing algorithms and Three.js for immersive 3D rendering.

## 🚀 Key Features

*   **Optimized WASM Pathfinding**: High-performance routing engine written in C++ and compiled to WebAssembly.
*   **Immersive 3D Visualization**: Professional-grade Three.js scene featuring glowing network nodes, dynamic path highlighting, and metro train animations.
*   **Modular Architecture**: Clean, maintainable codebase with separated logic for configuration, UI interaction, and 3D rendering.
*   **Interactive UI**: Sleek, cyberpunk-inspired control panel with glassmorphism effects, responsive design, and real-time path statistics.
*   **Contextual Tooltips**: Professional hover-based data overlays instead of intrusive floating labels.

## 🛠 Tech Stack

*   **Graphics**: [Three.js](https://threejs.org/)
*   **Core Engine**: C++ compiled with [Emscripten (WASM)](https://emscripten.org/)
*   **Styling**: [Tailwind CSS](https://tailwindcss.com/)
*   **Icons**: [Lucide](https://lucide.dev/)
*   **Build System**: Shell-based automation for C++ to WASM compilation.

## 📦 Getting Started

### Prerequisites
- Emscripten SDK (emsdk) for compiling the C++ engine.
- Python 3 for the local development server.

### Build and Run
```bash
# Ensure you are in the project root
./build_and_run.sh
```

This will automatically:
1. Compile the C++ core into `dmrc_engine.js` and `dmrc_engine.wasm`.
2. Spin up a local server at `http://localhost:8000`.

## 🏗 Project Structure
```text
├── css/            # Modular stylesheets
├── js/             # UI and 3D visualizer modules
├── src/            # C++ source for routing engine
├── include/        # C++ header files
├── data/           # Metro network JSON datasets
└── build_and_run.sh # Primary build automation script
```

## 🤝 Contribution
Contributions are welcome! Please follow the established modular architecture when adding new features or improving the routing logic.
