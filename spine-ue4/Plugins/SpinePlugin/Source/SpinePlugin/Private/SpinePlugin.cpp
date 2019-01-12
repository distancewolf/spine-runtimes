/******************************************************************************
 * Spine Runtimes Software License v2.5
 *
 * Copyright (c) 2013-2016, Esoteric Software
 * All rights reserved.
 *
 * You are granted a perpetual, non-exclusive, non-sublicensable, and
 * non-transferable license to use, install, execute, and perform the Spine
 * Runtimes software and derivative works solely for personal or internal
 * use. Without the written permission of Esoteric Software (see Section 2 of
 * the Spine Software License Agreement), you may not (a) modify, translate,
 * adapt, or develop new applications using the Spine Runtimes or otherwise
 * create derivative works or improvements of the Spine Runtimes or (b) remove,
 * delete, alter, or obscure any trademarks or any copyright, trademark, patent,
 * or other intellectual property or proprietary rights notices on or in the
 * Software, including any copy thereof. Redistributions in binary or source
 * form must include this license and terms.
 *
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, BUSINESS INTERRUPTION, OR LOSS OF
 * USE, DATA, OR PROFITS) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "SpinePluginPrivatePCH.h"

class FSpinePlugin : public SpinePlugin {
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FSpinePlugin, SpinePlugin )

// These should be filled with UE4's specific allocator functions.
extern "C" {
	void _spSetMalloc(void* (*_malloc) (size_t size));
	void _spSetFree(void(*_free) (void* ptr));
	void _spSetRealloc(void* (*_realloc) (void* ptr, size_t size));
}
static void * SpineMalloc(size_t size) {
	return FMemory::Malloc(size);
}
static void * SpineRealloc(void* ptr, size_t size) {
	return FMemory::Realloc(ptr, size);
}
void FSpinePlugin::StartupModule() {
// Needed for consoles, see https://github.com/EsotericSoftware/spine-runtimes/pull/1089
#if !UE_EDITOR && !PLATFORM_WINDOWS && !PLATFORM_MAC && !PLATFORM_LINUX && !PLATFORM_IOS && !PLATFORM_ANDROID && !PLATFORM_HTML5
	_spSetMalloc(&SpineMalloc);
	_spSetRealloc(&SpineRealloc);
	_spSetFree(FMemory::Free);
#endif
}

void FSpinePlugin::ShutdownModule() { }

// These are not used in the Spine UE4 plugin, see SpineAtlasAsset on how atlas page textures
// are loaded, See SpineSkeletonRendererComponent on how these textures are used for rendering.
extern "C" {
	void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) { }
	void _spAtlasPage_disposeTexture (spAtlasPage* self) { }
	char* _spUtil_readFile (const char* path, int* length) { return 0; }
}



