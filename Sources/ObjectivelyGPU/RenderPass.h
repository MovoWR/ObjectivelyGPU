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

typedef struct RenderPass RenderPass;
typedef struct RenderPassInterface RenderPassInterface;

struct RenderPass {
  Object object;
  RenderPassInterface *interface;
  SDL_GPURenderPass *pass;
};

struct RenderPassInterface {
  ObjectInterface objectInterface;

  void (*bindFragmentSamplers)(const RenderPass *self, Uint32 firstSlot, const SDL_GPUTextureSamplerBinding *bindings, Uint32 num);
  void (*bindFragmentStorageBuffers)(const RenderPass *self, Uint32 firstSlot, SDL_GPUBuffer *const *buffers, Uint32 num);
  void (*bindFragmentStorageTextures)(const RenderPass *self, Uint32 firstSlot, SDL_GPUTexture *const *textures, Uint32 num);
  void (*bindGraphicsPipeline)(const RenderPass *self, SDL_GPUGraphicsPipeline *pipeline);
  void (*bindIndexBuffer)(const RenderPass *self, const SDL_GPUBufferBinding *binding, SDL_GPUIndexElementSize indexElementSize);
  void (*bindVertexBuffers)(const RenderPass *self, Uint32 firstSlot, const SDL_GPUBufferBinding *bindings, Uint32 num);
  void (*bindVertexSamplers)(const RenderPass *self, Uint32 firstSlot, const SDL_GPUTextureSamplerBinding *bindings, Uint32 num);
  void (*bindVertexStorageBuffers)(const RenderPass *self, Uint32 firstSlot, SDL_GPUBuffer *const *buffers, Uint32 num);
  void (*bindVertexStorageTextures)(const RenderPass *self, Uint32 firstSlot, SDL_GPUTexture *const *textures, Uint32 num);
  void (*drawIndexedPrimitives)(const RenderPass *self, Uint32 numIndices, Uint32 numInstances, Uint32 firstIndex, Sint32 vertexOffset, Uint32 firstInstance);
  void (*drawIndexedPrimitivesIndirect)(const RenderPass *self, SDL_GPUBuffer *buffer, Uint32 offset, Uint32 drawCount);
  void (*drawPrimitives)(const RenderPass *self, Uint32 numVertices, Uint32 numInstances, Uint32 firstVertex, Uint32 firstInstance);
  void (*drawPrimitivesIndirect)(const RenderPass *self, SDL_GPUBuffer *buffer, Uint32 offset, Uint32 drawCount);
  RenderPass *(*initWithPass)(RenderPass *self, SDL_GPURenderPass *pass);
  void (*setBlendConstants)(const RenderPass *self, SDL_FColor blendConstants);
  void (*setScissor)(const RenderPass *self, const SDL_Rect *scissor);
  void (*setStencilReference)(const RenderPass *self, Uint8 reference);
  void (*setViewport)(const RenderPass *self, const SDL_GPUViewport *viewport);
};

OBJECTIVELYGPU_EXPORT Class *_RenderPass(void);
