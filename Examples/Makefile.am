noinst_PROGRAMS = \
	Hello \
	HelloCompute

CFLAGS += \
	-I$(top_srcdir)/Sources \
	-DEXAMPLES=\"$(abs_srcdir)\" \
	@HOST_CFLAGS@ \
	@OBJECTIVELY_CFLAGS@ \
	@SDL3_CFLAGS@

LDADD = \
	$(top_builddir)/Sources/ObjectivelyGPU/libObjectivelyGPU.la \
	@HOST_LIBS@ \
	@OBJECTIVELY_LIBS@ \
	@SDL3_LIBS@ \
	-lm

Hello_SOURCES = Hello.c
HelloCompute_SOURCES = HelloCompute.c

# All GLSL sources and compiled blobs are versioned in the repository.
# Normal builds never invoke glslc or shadercross.  Run 'make shaders' after
# editing a .glsl file to regenerate the platform blobs.

GLSLC = glslc
SHADERCROSS = shadercross

GLSL_SHADERS = \
	Hello.vert.glsl \
	Hello.frag.glsl \
	HelloCompute.comp.glsl \
	HelloCompute.vert.glsl \
	HelloCompute.frag.glsl

METAL_SHADERS = \
	Hello.vert.metal \
	Hello.frag.metal \
	HelloCompute.comp.metal \
	HelloCompute.vert.metal \
	HelloCompute.frag.metal

SPIRV_SHADERS = \
	Hello.vert.spv \
	Hello.frag.spv \
	HelloCompute.comp.spv \
	HelloCompute.vert.spv \
	HelloCompute.frag.spv

EXTRA_DIST = $(GLSL_SHADERS) $(METAL_SHADERS) $(SPIRV_SHADERS)

.PHONY: shaders
shaders: $(METAL_SHADERS) $(SPIRV_SHADERS)

$(srcdir)/%.vert.metal: $(srcdir)/%.vert.spv
	$(SHADERCROSS) $< -s SPIRV -d MSL -t vertex -o $@

$(srcdir)/%.frag.metal: $(srcdir)/%.frag.spv
	$(SHADERCROSS) $< -s SPIRV -d MSL -t fragment -o $@

$(srcdir)/%.comp.metal: $(srcdir)/%.comp.spv
	$(SHADERCROSS) $< -s SPIRV -d MSL -t compute -o $@

$(srcdir)/%.vert.spv: $(srcdir)/%.vert.glsl
	$(GLSLC) -fshader-stage=vert $< -o $@

$(srcdir)/%.frag.spv: $(srcdir)/%.frag.glsl
	$(GLSLC) -fshader-stage=frag $< -o $@

$(srcdir)/%.comp.spv: $(srcdir)/%.comp.glsl
	$(GLSLC) -fshader-stage=comp $< -o $@
