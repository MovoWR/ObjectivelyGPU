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
typedef struct CopyPass CopyPass;
typedef struct RenderPass RenderPass;
typedef struct CommandBuffer CommandBuffer;
typedef struct CommandBufferInterface CommandBufferInterface;

struct CommandBuffer {
  Object object;
  CommandBufferInterface *interface;
  SDL_GPUCommandBuffer *cmd;
};

struct CommandBufferInterface {
  ObjectInterface objectInterface;

  bool (*acquireSwapchainTexture)(const CommandBuffer *self, SDL_Window *window, SDL_GPUTexture **texture, Uint32 *w, Uint32 *h);
  ComputePass *(*beginComputePass)(const CommandBuffer *self, const SDL_GPUStorageTextureReadWriteBinding *storageTextures, Uint32 numStorageTextures, const SDL_GPUStorageBufferReadWriteBinding *storageBuffers, Uint32 numStorageBuffers);
  CopyPass *(*beginCopyPass)(const CommandBuffer *self);
  RenderPass *(*beginRenderPass)(const CommandBuffer *self, const SDL_GPUColorTargetInfo *colorTargets, Uint32 numColorTargets, const SDL_GPUDepthStencilTargetInfo *depthStencil);
  void (*blitTexture)(const CommandBuffer *self, const SDL_GPUBlitInfo *info);
  bool (*cancel)(const CommandBuffer *self);
  void (*generateMipmaps)(const CommandBuffer *self, SDL_GPUTexture *texture);
  CommandBuffer *(*initWithCommandBuffer)(CommandBuffer *self, SDL_GPUCommandBuffer *cmd);
  void (*insertDebugLabel)(const CommandBuffer *self, const char *text);
  void (*popDebugGroup)(const CommandBuffer *self);
  void (*pushComputeUniformData)(const CommandBuffer *self, Uint32 slot, const void *data, Uint32 length);
  void (*pushDebugGroup)(const CommandBuffer *self, const char *name);
  void (*pushFragmentUniformData)(const CommandBuffer *self, Uint32 slot, const void *data, Uint32 length);
  void (*pushVertexUniformData)(const CommandBuffer *self, Uint32 slot, const void *data, Uint32 length);
  bool (*submit)(const CommandBuffer *self);
  SDL_GPUFence *(*submitAndFence)(const CommandBuffer *self);
  bool (*waitAndAcquireSwapchainTexture)(const CommandBuffer *self, SDL_Window *window, SDL_GPUTexture **texture, Uint32 *w, Uint32 *h);
};

OBJECTIVELYGPU_EXPORT Class *_CommandBuffer(void);
