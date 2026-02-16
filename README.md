# 2D Physics Engine

A custom **2D physics engine** written in **C++**, built from scratch with a focus on understanding the fundamentals of real-time physics simulation.

This project implements rigid body dynamics, collision detection, impulse-based resolution, and constraint solving without relying on external physics libraries.

---

## ✨ Features

- Rigid body dynamics (linear & angular)
- Circle, box, and convex polygon shapes
- Impulse-based collision resolution
- Friction & restitution (bounciness)
- Gravity and external forces
- Constraint solver with warm starting
- Point-to-point (joint) constraints
- Real-time visualization using **SDL2 + ImGui**

---

## 🎮 Demonstrations

### Different Object Types
Circles, boxes, and polygons interacting together with proper collision handling.

![Different Objects](assets/gifs/different_objects.gif)

---

### Gravity
Objects accelerate under gravity and collide naturally with the environment.

![Gravity](assets/gifs/gravity.gif)

---

### Friction
Demonstrates tangential impulse resolution and friction limits during contact.

![Friction](assets/gifs/friction.gif)

---

### Restitution (Bounciness)
Shows how restitution affects collision response and energy preservation.

![Restitution](assets/gifs/restitution.gif)

---

### Mass & Scale
Objects with different masses respond differently to forces and impulses.

![Mass and Scale](assets/gifs/mass_and_scale.gif)

---

### Joint Constraint
A point-to-point (ball) joint that keeps two anchor points coincident in world space.

![Joint Constraint](assets/gifs/joint_constraint.gif)

---

## 🧠 Technical Overview

### Rigid Bodies
Each body maintains:
- Position & rotation
- Linear & angular velocity
- Mass and moment of inertia
- Accumulated forces and torques

Integration is done using a semi-implicit Euler method.

---

### Collision Detection
- **Circle vs Circle**
- **Polygon vs Polygon** (SAT + clipping)
- **Polygon vs Circle**

Contact manifolds are generated using **reference / incident edge clipping** for stable collision points.

---

### Collision Resolution
- Impulse-based resolution
- Normal impulses prevent penetration
- Tangential impulses simulate friction
- Baumgarte stabilization for positional correction
- Restitution handled via velocity bias

---

### Constraint Solver
- Jacobian-based constraints
- Iterative Gauss–Seidel solver
- Warm starting using accumulated impulses
- Supports:
  - Penetration constraints
  - Point-to-point (joint) constraints

---

## 🛠️ Controls & Interaction

- **Left Click**: Spawn objects
- **Right Click + Drag**: Grab and throw objects
- **Joint Tool**: Click two bodies to connect them
- **UI Sliders**: Adjust mass, friction, restitution, scale, gravity
- **Debug Mode**: Visualize collision and constraint data

---

## 📦 Dependencies

- **C++17**
- **SDL2**
- **SDL2_image**
- **Dear ImGui**

No external physics libraries are used.

---

## 🎯 Goals of This Project

- Deep understanding of physics engine internals
- Learn how real-time constraint solvers work
- Explore numerical stability and iteration-based solvers
- Build a solid foundation for more advanced systems (XPBD, motors, joints, etc.)

---

## 📌 Notes

This engine is **educational by design**.  
Clarity, correctness, and learning were prioritized over raw performance.

---

## 📷 Assets

Textures and GIFs are used for demonstration purposes only.

---

## 🧩 Future Improvements

- Distance and hinge joints
- Continuous collision detection (CCD)
- Sleeping bodies
- Island solving
- XPBD-style constraints
- Better inertia calculation for polygons

---

## 👤 Author

Built by **lordipio**  
Feel free to explore, fork, or experiment with the code.
