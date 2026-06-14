# Ray Tracer

## Overview

A C++ CPU Ray Tracer to learn Ray and Path Tracing concepts. 
Started as a semester project for Advanced Computer graphics, I plan to
keep working on this project to learn and implement even more advanced
techniques, and extend it to a path tracer. Currently, I'm following the [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
book to get a skeleton ray tracer, while also changing some parts
of the implementation. 

## Current Render
<img width="800" height="450" alt="render" src="https://github.com/user-attachments/assets/753b1d9d-d800-4753-8671-24e448d7d373" />

## Implemented Features

- Whitted-style ray tracing.
- Render the final image in a GLFW window.
- ImGUI UI that shows render time, editable samples per pixel & ray depth, save render as a PNG, and a re-render button.
- Anti-aliasing
- Implemented Materials:
    - Diffuse Surface
    - Metal (smooth & matte)
    - Dielectric (Reflection + Refraction)
 
## Build and Run
```bash

 git clone https://github.com/SujayAmberkar/MyRayTracer.git
 cd MyRayTracer
```
Open MyRayTracer.sln in Visual Studio 2022.

## Benchmark

For the above image

| Settings          | Value     |
| ----------------- | --------- |
| Max Ray Depth     | 20        |
| Samples per pixel | 50        |
| Render Time       | 63186 ms  |
| Resolution        | 800 x 450 |


