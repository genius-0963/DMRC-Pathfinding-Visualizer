import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import { SCENE_CONFIG } from './config.js';

export class MetroVisualizer {
    constructor(containerId) {
        this.container = document.getElementById(containerId);
        this.scene = new THREE.Scene();
        this.camera = new THREE.PerspectiveCamera(60, window.innerWidth / window.innerHeight, 0.1, 1000);
        this.renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });
        this.init();
    }
    init() {
        this.renderer.setSize(window.innerWidth, window.innerHeight);
        this.container.appendChild(this.renderer.domElement);
        this.camera.position.set(SCENE_CONFIG.cameraStart.x, SCENE_CONFIG.cameraStart.y, SCENE_CONFIG.cameraStart.z);
        this.controls = new OrbitControls(this.camera, this.renderer.domElement);
    }
}
