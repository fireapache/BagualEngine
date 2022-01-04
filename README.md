# Bagual Engine

It's a general purpose game engine using a custom made pathtracing renderer, no OpenGL, Vulkan or DirectX raytracing functionalities are used here.

## Goal

To learn and create a custom game engine and a 3D renderer from scratch, mainly to understand the basics of how a game engine works and to experiment stuff with C++20.

## Current State

The engine compiles an super early editor and a game project and render a mockup character. It's using ImGUI to set some static variables to aid rendering and debugging.

<img src="https://user-images.githubusercontent.com/5270978/148115029-74c9b870-29e3-4fc5-8653-c80dd669338a.png" width="485" height="287">

**Windows**: Compiles and run<br>
**Linux (arm)**: Compiles but fails to run

## Next steps

Build an scene to hold actors and components, starting with a static mesh renderer and a camera component.
