# **Water Engine**
## 📅 Version **:** `v1.0.0`

Water Engine is a work-in-progress game engine focused on simplicity, modularity, and rapid prototyping. It provides a structured foundation for 2D games with scene handling, actor behavior, physics, and rendering support.

---

## 🚀 Features

* **2D Rendering** powered by **SFML 3**
* **Physics Simulation** using **Box2D**
* **Actor Component System** for modular gameplay objects
* **Scene Management** with clean initialization + switching
* **Resource Manager** for textures, fonts, and assets
* **Example Template Game Included** for getting started quickly

---

## 📦 Building & Running

### Requirements

* C++17 compatible compiler
* CMake 3.20+
* SFML 3
* Box2D

### Build Instructions

```bash
# Clone the repository
git clone <repo_url>
cd WaterEngine

# Create a build directory
mkdir build
cd build

# Generate project + build files
cmake -S .. -B .

# Build (adjust command for your platform/toolchain)
cmake --build .
```

Includes a **Template Game Project** for testing. Use it as a starting point for your own games or as a reference.

---

## 🔮 Roadmap / Planned for v1.1.0

* **Audio System & Sound Management** (no audio currently implemented)
* Dynamic Interface System
* Additional helper utilities & engine polish
* Better documentation & sample expansions

---

## License

*MIT*

