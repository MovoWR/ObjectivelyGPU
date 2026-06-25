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

typedef struct CopyPass CopyPass;
typedef struct CopyPassInterface CopyPassInterface;

struct CopyPass {
  Object object;
  CopyPassInterface *interface;
  SDL_GPUCopyPass *pass;
};

struct CopyPassInterface {
  ObjectInterface objectInterface;

  void (*copyBufferToBuffer)(const CopyPass *self, const SDL_GPUBufferLocation *src, const SDL_GPUBufferLocation *dst, Uint32 size, bool cycle);
  void (*copyTextureToTexture)(const CopyPass *self, const SDL_GPUTextureLocation *src, const SDL_GPUTextureLocation *dst, Uint32 w, Uint32 h, Uint32 d, bool cycle);
  void (*downloadBuffer)(const CopyPass *self, const SDL_GPUBufferRegion *src, const SDL_GPUTransferBufferLocation *dst);
  void (*downloadTexture)(const CopyPass *self, const SDL_GPUTextureRegion *src, const SDL_GPUTextureTransferInfo *dst);
  CopyPass *(*initWithPass)(CopyPass *self, SDL_GPUCopyPass *pass);
  void (*uploadBuffer)(const CopyPass *self, const SDL_GPUTransferBufferLocation *src, const SDL_GPUBufferRegion *dst, bool cycle);
  void (*uploadTexture)(const CopyPass *self, const SDL_GPUTextureTransferInfo *src, const SDL_GPUTextureRegion *dst, bool cycle);
};

OBJECTIVELYGPU_EXPORT Class *_CopyPass(void);
