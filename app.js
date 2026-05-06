import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';

class MetroVisualizer {
    constructor(containerId) {
        this.container = document.getElementById(containerId);
        this.scene = new THREE.Scene();
        this.camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
        this.renderer = new THREE.WebGLRenderer({ antialias: true });
        
        this.stationsMap = new Map();
        this.graphGroup = new THREE.Group();
        this.pathGroup = new THREE.Group();
        
        this.init();
    }

    init() {
        this.renderer.setSize(window.innerWidth, window.innerHeight);
        this.renderer.setPixelRatio(window.devicePixelRatio);
        this.container.appendChild(this.renderer.domElement);

        this.camera.position.set(0, 50, 100);
        this.controls = new OrbitControls(this.camera, this.renderer.domElement);
        
        // Lighting
        const ambientLight = new THREE.AmbientLight(0xffffff, 0.5);
        const pointLight = new THREE.PointLight(0xffffff, 1);
        pointLight.position.set(50, 50, 50);
        this.scene.add(ambientLight, pointLight);
        
        this.scene.add(this.graphGroup);
        this.scene.add(this.pathGroup);

        window.addEventListener('resize', () => {
            this.camera.aspect = window.innerWidth / window.innerHeight;
            this.camera.updateProjectionMatrix();
            this.renderer.setSize(window.innerWidth, window.innerHeight);
        });

        this.animate();
    }

    // Convert Lat/Lon to 3D Coordinates
    // Using a simple scaling for Delhi region
    project(lat, lon) {
        const scale = 500;
        const centerLat = 28.6139; // Delhi Center
        const centerLon = 77.2090;
        
        return new THREE.Vector3(
            (lon - centerLon) * scale,
            0,
            (centerLat - lat) * scale
        );
    }

    renderGraph(data) {
        const stationGeometry = new THREE.SphereGeometry(0.8, 16, 16);
        const lineColors = {
            "Yellow": 0xFFD700,
            "Blue": 0x0000FF,
            "Rapid": 0x00FFFF
        };

        // Render Stations
        data.stations.forEach(s => {
            const pos = this.project(s.lat, s.lon);
            const material = new THREE.MeshPhongMaterial({ color: 0xaaaaaa });
            const mesh = new THREE.Mesh(stationGeometry, material);
            mesh.position.copy(pos);
            mesh.userData = { id: s.id, name: s.name };
            this.graphGroup.add(mesh);
            this.stationsMap.set(s.id, pos);
        });

        // Render Connections
        data.connections.forEach(c => {
            const p1 = this.stationsMap.get(c.source);
            const p2 = this.stationsMap.get(c.target);
            if (p1 && p2) {
                const curve = new THREE.CatmullRomCurve3([p1, p2]);
                const geometry = new THREE.TubeGeometry(curve, 1, 0.2, 8, false);
                const material = new THREE.MeshBasicMaterial({ 
                    color: lineColors[c.line] || 0xffffff,
                    transparent: true,
                    opacity: 0.4
                });
                const tube = new THREE.Mesh(geometry, material);
                this.graphGroup.add(tube);
            }
        });
    }

    highlightPath(pathIds) {
        this.pathGroup.clear();
        const points = pathIds.map(id => this.stationsMap.get(id));
        
        if (points.length < 2) return;

        const curve = new THREE.CatmullRomCurve3(points);
        const geometry = new THREE.TubeGeometry(curve, points.length * 4, 0.6, 8, false);
        const material = new THREE.MeshPhongMaterial({ 
            color: 0x00ffcc, 
            emissive: 0x00ffcc,
            emissiveIntensity: 1.0,
            transparent: true,
            opacity: 0.8
        });
        
        const pathTube = new THREE.Mesh(geometry, material);
        this.pathGroup.add(pathTube);

        // Add a "Glow" Pulse Animation
        this.pathTube = pathTube;
        this.pulseTime = 0;

        // Zoom to fit path
        const box = new THREE.Box3().setFromObject(this.pathGroup);
        const center = box.getCenter(new THREE.Vector3());
        const size = box.getSize(new THREE.Vector3());
        
        const maxDim = Math.max(size.x, size.y, size.z);
        const fov = this.camera.fov * (Math.PI / 180);
        let cameraZ = Math.abs(maxDim / 2 / Math.tan(fov / 2));
        cameraZ *= 1.5; // Zoom out a bit

        this.controls.target.copy(center);
        this.camera.position.set(center.x, cameraZ, center.z + cameraZ);
        this.controls.update();
    }

    animate() {
        requestAnimationFrame(() => this.animate());
        
        if (this.pathTube) {
            this.pulseTime += 0.05;
            this.pathTube.material.emissiveIntensity = 0.5 + Math.sin(this.pulseTime) * 0.5;
        }

        this.controls.update();
        this.renderer.render(this.scene, this.camera);
    }
}

// --- Bootstrap Logic ---
async function startApp() {
    // 1. Load Data
    const response = await fetch('data/dmrc_data.json');
    const metroData = await response.json();

    // 2. Initialize WASM Engine
    const DMRC = await DMRCModule();
    const graph = new DMRC.MetroGraph();
    graph.load_from_string(JSON.stringify(metroData));
    const router = new DMRC.Router(graph);

    // 3. Initialize Visualizer
    const visualizer = new MetroVisualizer('canvas-container');
    visualizer.renderGraph(metroData);

    // 4. Populate UI
    const startSelect = document.getElementById('startStation');
    const endSelect = document.getElementById('endStation');
    const infoDiv = document.getElementById('info');

    metroData.stations.forEach(s => {
        const opt1 = new Option(s.name, s.id);
        const opt2 = new Option(s.name, s.id);
        startSelect.add(opt1);
        endSelect.add(opt2);
    });

    // 5. Handle Routing
    document.getElementById('findRoute').onclick = () => {
        const startId = startSelect.value;
        const endId = endSelect.value;
        
        const result = router.find_path(startId, endId, 5.0); // 5km penalty for line change
        
        if (result.success) {
            const pathArray = [];
            for(let i=0; i < result.path.size(); i++) {
                pathArray.push(result.path.get(i));
            }
            
            visualizer.highlightPath(pathArray);
            infoDiv.innerHTML = `
                <b style="color:#00ffcc">Route Found!</b><br>
                Total Weight: ${result.total_distance.toFixed(2)} KM<br>
                Stops: ${pathArray.length}
            `;
        } else {
            infoDiv.innerText = "No route found.";
        }
    };
}

startApp();
