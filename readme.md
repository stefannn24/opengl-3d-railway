# OpenGL 3D Railway Engine

A 3D interactive graphics scene built in C++ and modern OpenGL, featuring hierarchical matrix transformations, dual camera perspectives, and path-based train movement.

### Note on the Framework
This project was developed on top of the university framework provided by the UPB Computer Graphics (EGC) course. The framework handles the foundational boilerplate (OpenGL context creation, windowing, shader management, and input handling).
My core implementations include:

- Hierarchical Modeling: Multi-component train built from primitives (wheels, chassis, boiler, cabin) using nested matrix transformations.
- Dual Camera System: Main perspective camera (free-roam and third-person follow modes) plus an orthographic overhead minimap rendered in a second pass.
- Path-Based Movement: Train follows a fixed rectangular path with deterministic position calculation based on distance traveled.
- Camera Tracking: Framerate-independent third-person camera with linear interpolation smoothing that rotates around the train as it turns.
- Procedural Geometry: Terrain tiles, track segments, stations, and train components generated programmatically using custom mesh builders.
---

### Build & Run Instructions
This project requires the UPB EGC framework. Build instructions and setup documentation will be added soon.