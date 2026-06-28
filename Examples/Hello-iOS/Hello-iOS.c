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

#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

#include <Objectively.h>
#include <ObjectivelyGPU.h>

typedef struct {
	float x, y, z;
	float r, g, b;
} Vertex;

static const Vertex vertex_data[] = {
	{ -0.5f,  0.5f, -0.5f,  1, 0, 0 }, {  0.5f, -0.5f, -0.5f, 0, 0, 1 }, { -0.5f, -0.5f, -0.5f, 0, 1, 0 },
	{ -0.5f,  0.5f, -0.5f,  1, 0, 0 }, {  0.5f,  0.5f, -0.5f, 1, 1, 0 }, {  0.5f, -0.5f, -0.5f, 0, 0, 1 },
	{ -0.5f,  0.5f,  0.5f,  1, 1, 1 }, { -0.5f, -0.5f, -0.5f, 0, 1, 0 }, { -0.5f, -0.5f,  0.5f, 0, 1, 1 },
	{ -0.5f,  0.5f,  0.5f,  1, 1, 1 }, { -0.5f,  0.5f, -0.5f, 1, 0, 0 }, { -0.5f, -0.5f, -0.5f, 0, 1, 0 },
	{ -0.5f,  0.5f,  0.5f,  1, 1, 1 }, {  0.5f,  0.5f, -0.5f, 1, 1, 0 }, { -0.5f,  0.5f, -0.5f, 1, 0, 0 },
	{ -0.5f,  0.5f,  0.5f,  1, 1, 1 }, {  0.5f,  0.5f,  0.5f, 0, 0, 0 }, {  0.5f,  0.5f, -0.5f, 1, 1, 0 },
	{  0.5f,  0.5f, -0.5f,  1, 1, 0 }, {  0.5f, -0.5f,  0.5f, 1, 0, 1 }, {  0.5f, -0.5f, -0.5f, 0, 0, 1 },
	{  0.5f,  0.5f, -0.5f,  1, 1, 0 }, {  0.5f,  0.5f,  0.5f, 0, 0, 0 }, {  0.5f, -0.5f,  0.5f, 1, 0, 1 },
	{  0.5f,  0.5f,  0.5f,  0, 0, 0 }, { -0.5f, -0.5f,  0.5f, 0, 1, 1 }, {  0.5f, -0.5f,  0.5f, 1, 0, 1 },
	{  0.5f,  0.5f,  0.5f,  0, 0, 0 }, { -0.5f,  0.5f,  0.5f, 1, 1, 1 }, { -0.5f, -0.5f,  0.5f, 0, 1, 1 },
	{ -0.5f, -0.5f, -0.5f,  0, 1, 0 }, {  0.5f, -0.5f,  0.5f, 1, 0, 1 }, { -0.5f, -0.5f,  0.5f, 0, 1, 1 },
	{ -0.5f, -0.5f, -0.5f,  0, 1, 0 }, {  0.5f, -0.5f, -0.5f, 0, 0, 1 }, {  0.5f, -0.5f,  0.5f, 1, 0, 1 },
};

typedef struct {
	SDL_Window *window;
	RenderDevice *renderDevice;
	Framebuffer *framebuffer;
	SDL_GPUBuffer *vertexBuffer;
	SDL_GPUGraphicsPipeline *pipeline;
	vec2 angles;
	Uint64 lastTicks;
} App;

static App app;

SDL_AppResult SDL_AppInit(void **unused, int argc, char *argv[]) {
	(void) unused; (void) argc; (void) argv;

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_memset(&app, 0, sizeof(app));

	const char *basePath = SDL_GetBasePath();
	if (basePath) {
		$$(Resource, addResourcePath, basePath);
	}

	app.window = SDL_CreateWindow("ObjectivelyGPU Hello",
		0, 0, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_FULLSCREEN);
	if (!app.window) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	app.renderDevice = $(alloc(RenderDevice), initWithWindow, app.window);

	int w = 0, h = 0;
	SDL_GetWindowSizeInPixels(app.window, &w, &h);
	app.framebuffer = $(alloc(Framebuffer), initWithDevice, app.renderDevice,
		&MakeSize(w, h),
		SDL_GPU_TEXTUREFORMAT_INVALID,
		SDL_GPU_TEXTUREFORMAT_D16_UNORM);

	app.vertexBuffer = $(app.renderDevice, createBuffer, &(SDL_GPUBufferCreateInfo) {
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size = sizeof(vertex_data),
	});

	SDL_GPUTransferBuffer *transferBuffer = $(app.renderDevice, createTransferBuffer, &(SDL_GPUTransferBufferCreateInfo) {
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = sizeof(vertex_data),
	});
	void *mapped = $(app.renderDevice, mapTransferBuffer, transferBuffer, false);
	SDL_memcpy(mapped, vertex_data, sizeof(vertex_data));
	$(app.renderDevice, unmapTransferBuffer, transferBuffer);

	CommandBuffer *cmd = $(app.renderDevice, acquireCommandBuffer);
	CopyPass *copyPass = $(cmd, beginCopyPass);
	$(copyPass, uploadBuffer,
	  &(SDL_GPUTransferBufferLocation){ .transfer_buffer = transferBuffer },
	  &(SDL_GPUBufferRegion){ .buffer = app.vertexBuffer, .size = sizeof(vertex_data) },
	  false);
	release(copyPass);
	$(cmd, submit);
	release(cmd);
	$(app.renderDevice, releaseTransferBuffer, transferBuffer);

	SDL_GPUShader *vertexShader = $(app.renderDevice, loadShader, "Hello.vert", &(SDL_GPUShaderCreateInfo) {
		.stage = SDL_GPU_SHADERSTAGE_VERTEX,
		.num_uniform_buffers = 1,
	});
	SDL_GPUShader *fragmentShader = $(app.renderDevice, loadShader, "Hello.frag", &(SDL_GPUShaderCreateInfo) {
		.stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
	});

	SDL_GPUColorTargetDescription colorTarget = {
		.format = $(app.renderDevice, getSwapchainTextureFormat, app.window),
	};
	SDL_GPUVertexBufferDescription vbDesc = {
		.slot = 0,
		.pitch = sizeof(Vertex),
		.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
	};
	SDL_GPUVertexAttribute vertexAttribs[] = {
		{ .location = 0, .buffer_slot = 0, .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, .offset = 0  },
		{ .location = 1, .buffer_slot = 0, .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, .offset = 12 },
	};

	app.pipeline = $(app.renderDevice, createGraphicsPipeline, &(SDL_GPUGraphicsPipelineCreateInfo) {
		.vertex_shader = vertexShader,
		.fragment_shader = fragmentShader,
		.vertex_input_state = {
			.vertex_buffer_descriptions = &vbDesc,
			.num_vertex_buffers = 1,
			.vertex_attributes = vertexAttribs,
			.num_vertex_attributes = (Uint32) SDL_arraysize(vertexAttribs),
		},
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.rasterizer_state = {
			.fill_mode = SDL_GPU_FILLMODE_FILL,
			.cull_mode = SDL_GPU_CULLMODE_BACK,
			.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
			.enable_depth_clip = true,
		},
		.depth_stencil_state = {
			.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL,
			.enable_depth_test = true,
			.enable_depth_write = true,
		},
		.target_info = {
			.color_target_descriptions = &colorTarget,
			.num_color_targets = 1,
			.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D16_UNORM,
			.has_depth_stencil_target = true,
		},
	});

	$(app.renderDevice, releaseShader, vertexShader);
	$(app.renderDevice, releaseShader, fragmentShader);

	app.lastTicks = SDL_GetTicks();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *unused) {
	(void) unused;

	Uint64 ticks = SDL_GetTicks();
	float dt = (float)(ticks - app.lastTicks) / 1000.0f;
	app.lastTicks = ticks;

	app.angles.x += dt * 30.0f;
	app.angles.y += dt * 60.0f;
	while (app.angles.x >= 360.0f) app.angles.x -= 360.0f;
	while (app.angles.y >= 360.0f) app.angles.y -= 360.0f;

	CommandBuffer *cmd = $(app.renderDevice, acquireCommandBuffer);
	SwapchainTexture swapchain = { 0 };
	if (!$(cmd, acquireSwapchainTexture, &swapchain)) {
		$(cmd, cancel);
		release(cmd);
		return SDL_APP_CONTINUE;
	}

	$(app.framebuffer, resize, &swapchain.size);

	mat4 modelView = mat4_rotation(app.angles.x, vec3_new(1.f, 0.f, 0.f));
	modelView = mat4_mul(mat4_rotation(app.angles.y, vec3_new(0.f, 1.f, 0.f)), modelView);
	modelView = mat4_mul(mat4_translation(vec3_new(0.f, 0.f, -2.5f)), modelView);

	const mat4 projection = mat4_perspective(45.f,
		(float) swapchain.size.w / (float) swapchain.size.h, 0.01f, 100.f);
	const mat4 modelViewProjection = mat4_mul(projection, modelView);

	SDL_GPUColorTargetInfo colorInfo = {
		.texture = swapchain.texture,
		.clear_color = { 0.1f, 0.1f, 0.2f, 1.0f },
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_STORE,
	};
	SDL_GPUDepthStencilTargetInfo depthInfo = $(app.framebuffer, depthTargetInfo,
		SDL_GPU_LOADOP_CLEAR, SDL_GPU_STOREOP_DONT_CARE, 1.f);

	RenderPass *renderPass = $(cmd, beginRenderPass, &colorInfo, 1, &depthInfo);
	$(renderPass, bindPipeline, app.pipeline);
	$(renderPass, bindVertexBuffers, 0, &(SDL_GPUBufferBinding){ .buffer = app.vertexBuffer }, 1);
	$(cmd, pushVertexUniformData, 0, modelViewProjection.f, sizeof(modelViewProjection));
	$(renderPass, drawPrimitives, (Uint32) SDL_arraysize(vertex_data), 1, 0, 0);
	release(renderPass);

	$(cmd, submit);
	release(cmd);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *unused, SDL_Event *event) {
	(void) unused;
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *unused, SDL_AppResult result) {
	(void) unused; (void) result;

	if (app.renderDevice) {
		$(app.renderDevice, waitForIdle);
	}
	if (app.pipeline) {
		$(app.renderDevice, releaseGraphicsPipeline, app.pipeline);
	}
	if (app.vertexBuffer) {
		$(app.renderDevice, releaseBuffer, app.vertexBuffer);
	}
	if (app.framebuffer) {
		release(app.framebuffer);
	}
	if (app.renderDevice) {
		release(app.renderDevice);
	}
	if (app.window) {
		SDL_DestroyWindow(app.window);
	}
	SDL_Quit();
}
