# Ray Tracer

## Overview

A personal project to learn Ray Tracing while building a Ray Tracer. 
Started as semester project for Advanced Computer graphics, I plan to
keep working on this project to learn and implement even more advanced
techniques. Currently I'm following the [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
book to get a skeleton ray tracer, while also changing(or trying to) some parts
of the implementation. 

## Current Render
![Rendered Image]("render.png")

## Implemented Features

- Whitted style Ray Tracing.
- Render final image in a GLFW window.
- ImGUI UI that shows render time, editable samples per pixel & ray depth, and re-render button.
- Implemented Materials:
    - Diffused Surface
    - Metal(smooth & matte)
    - Dielectric (Reflection + Refraction)

## Build and Run
```bash

 git clone https://github.com/SujayAmberkar/MyRayTracer.git
 cd MyRayTracer
```
Open MyRayTracer.sln in Visual Studio(I used VS 2022).
