# Loki

*A custom deferred renderer and game engine*

![Logo](/Logos//loki_logo_small.png)

Loki started out as a personal research project with the goal of implementing an OpenGL-based deferred renderer. Over time it grew in scope and features to include:

- Data-oriented material-definition system
- nVidia CG Effect File support for materials
- A Templated Component-based Entity System
- The renderer includes features such as:
    - Point-, Spot- and Directional lights
    - Texture projection for spot and point lights
    - Diffuse/Specular-/Normal-/Reflection-mapped materials
- Physics engine using Bullet Library
- Scripting support (Lua & Squirrel)
- Multi-threaded job-management
- HTML/CSS/JavaScript based UI logic+rendering and in-engine web-browsing using Awesomium Library
- Various general-purpose code and utility classes which have been used in numerous other projects, including code and classes for:
    - Simple and fast logging system with multiple levels of verbosity and storage modes
    - File systems
    - Window-handling
    - Time-keeping
    - Input-handling
    - Abstraction layers & wrappers for OpenGL-objects
    - C#-like 'properties' for C++
    - STL container & iterator typedef macros, also usable in templated classes
- Visual debugging capabilities
