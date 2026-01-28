<p align="center">
  <img src="https://i.imgur.com/bj0OOhf.png" alt="Water Engine" width="150">
</p>

<h1 align="center">Water Engine</h1>

`v2.0.0` [IN DEVELOPMENT]

`v1.0.0` [STABLE RELEASE]

Water Engine is a work-in-progress game engine focused on simplicity, modularity, and rapid prototyping. It provides a structured foundation for 2D games with scene handling, actor behavior, physics, and rendering support.

---

## ⚙️ Features

* **2D Rendering** powered by **SFML 3**
* **Physics Simulation** using **Box2D**
* **Actor Component System** for modular gameplay objects
* **Scene Management** with clean initialization + switching
* **Resource Manager** for textures, fonts, and assets
* **Example Template Game Included** for getting started quickly

---

## 🛠️ Building & Running

### Requirements

* C++20 compatible compiler
* CMake 3.30.3+

Includes a **Template Game Project** for testing. Use it as a starting point for your own games or as a reference.

---

## 🗺️ Roadmap (v2.x)

Water Engine v2 represents a complete architectural redesign and expansion, focusing on modularity, flexibility, and modern C++ practices. Planned features include:

### Core & Architecture
- Full **refactor of engine architecture** for better modularity and maintainability  
- Separation of **window, rendering, input, and game logic systems**  
- Event-driven and state-based **input management** supporting keyboard, mouse, and controllers  

### Rendering & UI
- **2D Rendering** via SFML 3  
- Integrated **TGUI** for flexible user interfaces  
- Window system supporting **borderless fullscreen, windowed, and dynamic resizing**  
- Full support for **render scaling and aspect ratio management**

### Physics & Simulation
- **Box2D** physics engine integration   

### Resources & Config
- **PhysicsFS** for virtual filesystem and content management 
- **Content packing system** for assets (textures, fonts, audio) into `.pak` files  
- Configurables via **JSON** using **nlohmann/json**  
- Full support for **magic_enum** for type-safe enums and reflection  

### Logging & Debug
- **spdlog** integrated for fast, flexible logging and debug output

### Input & Controllers
- **Full controller support** (gamepads, joysticks)  
- **Event-based input** for immediate reactions  
- **State-based input** for sustained key/button actions  

### Utilities & Developer Tools
- Centralized **ResourceManager** for textures, fonts, and other assets  
- Helper utilities for vectors, containers, and memory management (`unique`, `shared`, `weak`, `optional`)  
- Template game included as a **reference implementation**  

## License

*MIT*

