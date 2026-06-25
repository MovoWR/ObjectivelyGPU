# ObjectivelyGPU — Copilot Instructions

ObjectivelyGPU is an object-oriented GPU framework wrapping SDL3's GPU API, written in GNU C using the [Objectively](https://github.com/jdolan/Objectively) runtime.

## Language & Style

- **GNU C** (gcc/clang). Use `#pragma once` for header guards.
- Follow the existing Objectively OOP pattern exactly: every class has a `struct Foo`, a `struct FooInterface`, and a `Class *_Foo(void)` archetype function.
- `$()` is the Objectively method-call macro: `$(obj, methodName, args...)`.
- `alloc(Foo)` allocates and zero-initialises; `release(obj)` releases (ARC-like).
- `ident` is `void *` (Objectively's generic pointer typedef).
- `assert()` all non-trivial pointer arguments at the top of every function.
- Use `GPU_Assert(cond, fmt, ...)` (from `Types.h`) for unrecoverable SDL GPU failures — never `SDL_assert`.
- License header (zlib) must appear at the top of every `.c` and `.h` file.

## Project Layout

```
Sources/ObjectivelyGPU/   Core classes: CommandBuffer, RenderDevice, RenderPass,
                          CopyPass, ComputePass, Types.h
Sources/ObjectivelyGPU/Shaders/   HLSL shader sources compiled offline with sdl-shadercross
```

## Class Anatomy

**Header (`Foo.h`)**
1. Forward-declare all referenced types with `typedef struct Foo Foo;`.
2. Define `struct Foo { Object object; FooInterface *interface; /* public fields */ };`
3. Define `struct FooInterface { ObjectInterface objectInterface; /* function pointers */ };`
4. Document every interface method with a full Doxygen `@fn` block including `@memberof`.
5. Declare `OBJECTIVELYGPU_EXPORT Class *_Foo(void);` at the bottom.

**Implementation (`Foo.c`)**
1. `#define _Class _Foo` at the top (after includes).
2. Static functions implement interface methods; each gets a one-line `@memberof` comment.
3. Wire everything in `_Foo()` via `$(self, initWithInterface, ...)` / interface assignment.

## SDL3 GPU Conventions

- GPU resources (`SDL_GPUBuffer`, `SDL_GPUTexture`, etc.) are **not** reference-counted — release them explicitly via `RenderDevice` release methods.
- Pass objects begin and end via the `begin*Pass` / `release(pass)` pattern; `dealloc` calls the SDL end function automatically.
- Shader blobs are loaded via `RenderDevice::loadShader`; platform extension (`.msl`, `.spv`, `.dxil`) is appended automatically.
- Use `Drawable` structs (not subclasses) to hook into the `RenderDevice` frame loop when a full class is unnecessary.

## Doxygen

All public API must have Doxygen comments. Use:
- `@file` / `@brief` in header files.
- `@fn ClassName::methodName(...)` with `@param`, `@return`, `@details`, `@memberof` for every interface function pointer.
- `@extends ClassName` on class brief comments.
- `@private` / `@protected` tags on internal struct fields.
