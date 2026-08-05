import * as THREE from 'https://cdnjs.cloudflare.com/ajax/libs/three.js/0.185.0/three.module.js';
 
 // --setup--
 const scene = new THREE.Scene();

 const renderer = new THREE.WebGLRenderer();
  renderer.setSize(window.innerWidth, window.innerHeight);
  document.body.appendChild(renderer.domElement);

  const camera = new THREE.PerspectiveCamera(
    75,
    window.innerWidth / window.innerHeight,
    0.1,
    1000
  );
  //const orbit = new OrbitControls(camera, renderer.domElement);
  camera.position.set(0, 2.5, 0);
  //orbit.update();
  


  //--objects--
  
  const sphereG = new THREE.SphereGeometry(1);
  const sphereM = new THREE.MeshBasicMaterial({
      color: 0x95a2ff,
      wireframe: false
  });
  const ball = new THREE.Mesh(sphereG, sphereM);
  
  const planeG = new THREE.PlaneGeometry(9, 20);
  
  const planeM = new THREE.MeshBasicMaterial({color: 0x9cff95, side: THREE.DoubleSide});
  const plane = new THREE.Mesh(planeG, planeM);
  
  const playerG = new THREE.CapsuleGeometry();
  const playerM = new THREE.MeshBasicMaterial({color: 0xfff100});
  const player = new THREE.Mesh(playerG, playerM);
  
 // logic
 scene.add(ball);
  ball.position.set(0, 4, 2);
  scene.add(plane);
  plane.position.set(0, 0, 0);
  plane.rotation.x += Math.PI / 2;
  scene.add(player);
  player.position.set(0, 2, 0)
  camera.add(player);
let moveForward = false;
let moveBackward = false;
let moveLeft = false;
let moveRight = false;

// Add event listeners for buttons
document.getElementById('forward').addEventListener('touchstart', () => { moveForward = true; });
document.getElementById('forward').addEventListener('touchend', () => { moveForward = false; });

document.getElementById('backward').addEventListener('touchstart', () => { moveBackward = true; });
document.getElementById('backward').addEventListener('touchend', () => { moveBackward = false; });

document.getElementById('left').addEventListener('touchstart', () => { moveLeft = true; });
document.getElementById('left').addEventListener('touchend', () => { moveLeft = false; });

document.getElementById('right').addEventListener('touchstart', () => { moveRight = true; });
document.getElementById('right').addEventListener('touchend', () => { moveRight = false; });

// Touch gesture for looking around
let lastTouchX = 0;
let lastTouchY = 0;

window.addEventListener('touchstart', (e) => {
  if (e.touches.length === 1) {
    lastTouchX = e.touches[0].clientX;
    lastTouchY = e.touches[0].clientY;
  }
});

window.addEventListener('touchmove', (e) => {
  if (e.touches.length === 1) {
    const deltaX = e.touches[0].clientX - lastTouchX;
    const deltaY = e.touches[0].clientY - lastTouchY;

    // Rotate camera based on delta
    camera.rotation.y -= deltaX * 0.005;
    camera.rotation.x -= deltaY * 0.005;

    // Clamp pitch to avoid flipping
    const maxPitch = Math.PI / 2 - 0.1;
    const minPitch = -Math.PI / 2 + 0.1;
    camera.rotation.x = Math.max(minPitch, Math.min(maxPitch, camera.rotation.x));

    lastTouchX = e.touches[0].clientX;
    lastTouchY = e.touches[0].clientY;
  }
});
const timer = new THREE.Timer();
function animate() {
 
 timer.update();
 
  const delta = timer.getDelta();
  const moveSpeed = 5; // Adjust speed

  if (controls.isLocked || true) { // Use controls if needed

    const direction = new THREE.Vector3();

    if (moveForward) {
      direction.z -= moveSpeed * delta;
    }
    if (moveBackward) {
      direction.z += moveSpeed * delta;
    }
    if (moveLeft) {
      direction.x -= moveSpeed * delta;
    }
    if (moveRight) {
      direction.x += moveSpeed * delta;
    }

    // Move camera in local space
    camera.translateX(direction.x);
    camera.translateZ(direction.z);
  }

const raycaster = new THREE.Raycaster();
const mouse = new THREE.Vector2(0, 0); // Center of the screen

  raycaster.setFromCamera(mouse, camera);
  const intersects = raycaster.intersectObjects(scene.children, true);
  if (intersects.lenght > 0) {
     const firstObject = intersects[0].object;
     firstObject.material.color.setHex(0xffffff);
     window.addEventListener('click', () =>
        {
  if (ball.material.color.setHex(0xffffff)) {
    camera.add(firstObject);
    alert('Object clicked: ' + highlightedObject.name);
  }
  });
  }
  renderer.render(scene, camera);
}
  renderer.setAnimationLoop(animate);
