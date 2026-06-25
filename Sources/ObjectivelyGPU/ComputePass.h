/*
 * ObjectivelyGPU: Object oriented MVC framework for SDL3 and GNU C.
 * Copyright (C) 2026 Jay Dolan <jay@jaydolan.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#pragma once

#include <SDL3/SDL_gpu.h>

#include <Objectively/Object.h>

#include <ObjectivelyGPU/Types.h>

typedef struct ComputePass ComputePass;
typedef struct ComputePassInterface ComputePassInterface;

struct ComputePass {
  Object object;
  ComputePassInterface *interface;
  SDL_GPUComputePass *pass;
};

struct ComputePassInterface {
  ObjectInterface objectInterface;

  void (*bindComputePipeline)(const ComputePass *self, SDL_GPUComputePipeline *pipeline);
  void (*bindSamplers)(const ComputePass *self, Uint32 firstSlot, const SDL_GPUTextureSamplerBinding *bindings, Uint32 num);
  void (*bindStorageBuffers)(const ComputePass *self, Uint32 firstSlot, SDL_GPUBuffer *const *buffers, Uint32 num);
  void (*bindStorageTextures)(const ComputePass *self, Uint32 firstSlot, SDL_GPUTexture *const *textures, Uint32 num);
  void (*dispatchCompute)(const ComputePass *self, Uint32 groupCountX, Uint32 groupCountY, Uint32 groupCountZ);
  void (*dispatchComputeIndirect)(const ComputePass *self, SDL_GPUBuffer *buffer, Uint32 offset);
  ComputePass *(*initWithPass)(ComputePass *self, SDL_GPUComputePass *pass);
};

OBJECTIVELYGPU_EXPORT Class *_ComputePass(void);
