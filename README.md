# Bagual Engine

It's a general purpose game engine using a custom made pathtracing renderer, no OpenGL, Vulkan or DirectX raytracing functionalities are used here.

## Goal

To learn and create a custom game engine and a 3D renderer from scratch, mainly to understand the basics of how a game engine works and to experiment stuff with C++20.

## Current State

The engine compiles a game project with engine test modules. It's using ImGUI to aid rendering and debugging. User can create their own modules or simply make engine calls from any other context where the engine is integrate to.

<img src="https://user-images.githubusercontent.com/5270978/150307202-7acd6387-8c12-4799-b655-0141e7af5fb7.png" width="350" height="287"><img src="https://user-images.githubusercontent.com/5270978/150307209-ac3a3041-d041-4692-abbb-b6ef44b38171.png" width="350" height="287">

**Windows**: Compiles and run<br>
**Linux (arm)**: Compiles but fails to run

## Features

* Scene with node and component structure
* Multithread CPU renderer (GPU in the plans)
* SIMD and non-SIMD renderer implementations
* OBJ mesh file importer (only triangles for now, materials not supported atm)
* Super simple to use straight in main.cpp
* Very customizable and easy to improve uppon

## Next steps

Improve rendering performance by adding a fast BVH solution to find the minimal triangle set to test intersection with.

