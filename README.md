# Bagual Engine

It's a general purpose game engine using a custom made pathtracing renderer, no OpenGL, Vulkan or DirectX raytracing functionalities are used here.

## Goal

To learn and create a custom game engine and a 3D renderer from scratch, mainly to understand the basics of how a game engine works and to experiment stuff with C++20.

## Compiling

**Windows**:
* Install Visual Studio 2022
* Clone repository
* Run GenerateProjectFiles.bat
* Open BagualEngine.sln
* Select **Release** compilation target for best performance
* Press F5

Note: You can change engine test module being used by editing BEngine::RegisterModules() function.

**Linux**:
* Doesn't compile atm, support to be added asap

## Current State

The engine compiles a game project with engine test modules. It's using ImGUI to aid rendering and debugging. User can create their own modules or simply make engine calls from any other context where the engine is integrate to.

<img src="https://user-images.githubusercontent.com/5270978/220289030-98004569-da4b-4255-9088-3b7c961c4f6e.png" width="350" height="287"><img src="https://user-images.githubusercontent.com/5270978/220289078-3911d9dd-521a-4272-b89a-562feaa8c5a8.png" width="350" height="287">

## Current Features

* Scene with node and component structure
* Multithread CPU renderer
* SIMD and non-SIMD renderer implementations
* OBJ mesh file importer (only triangles for now, materials not supported atm)
* Super simple to use straight in main.cpp
* Very customizable and easy to improve uppon
* Wireframe rasterizing

## Future Features

* Mirror surfaces
* Portals
* Lighting
* Global Ilumination
* 3D Navmesh
* Audio Occlusion
* Physics
* Input Bindings
* GPU Rendering
* Editor

## Current goal

Improve rendering performance by adding a fast BVH solution to find the minimal triangle set to test ray intersection with.

