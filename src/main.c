/*
 * PSP Software Development Kit - https://github.com/pspdev
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * Copyright (c) 2005 Jesper Svennevid
 */

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>

#include <callbacks.h>
#include <vram.h>
#include <model.h>

#include <ecmanager.h>
#include <systems/drawing.h>
#include <systems/lightning.h>
#include <systems/filming.h>
#include <systems/controlling.h>
#include <systems/moving.h>

PSP_MODULE_INFO("psp3d", 0, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

static unsigned int __attribute__((aligned(16))) list[262144];

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

int main(int argc, char* argv[]) {
    setupCallbacks();

    // flush cache so that no stray data remains
    sceKernelDcacheWritebackAll();

    // setup GU
    void* fbp0 = valloc(getVRAMSize(BUF_WIDTH, SCR_HEIGHT, GU_PSM_8888));
    void* zbp = valloc(getVRAMSize(BUF_WIDTH, SCR_HEIGHT, GU_PSM_4444));

    sceGuInit();

    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888, vrelptr(fbp0), BUF_WIDTH);
    sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, vrelptr(fbp0), BUF_WIDTH);
    sceGuDepthBuffer(vrelptr(zbp), BUF_WIDTH);
    sceGuOffset(2048 - (SCR_WIDTH / 2), 2048 - (SCR_HEIGHT / 2));
    sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);
    sceGuDepthRange(0xc350, 0x2710);
    sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuDepthFunc(GU_GEQUAL);
    sceGuEnable(GU_DEPTH_TEST);
    sceGuFrontFace(GU_CCW);
    sceGuShadeModel(GU_SMOOTH);
    sceGuEnable(GU_CULL_FACE);
    sceGuEnable(GU_CLIP_PLANES);
    sceGuEnable(GU_TEXTURE_2D);
    sceGuEnable(GU_LIGHTING);
    sceGuEnable(GU_LIGHT0);
    sceGuFinish();
    sceGuSync(0, 0);

    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    {
        int id = createEntity();
        addComponentTransformToEntity(id);
        getComponentTransform(id)->position.z = -3.5f;
        addComponentCameraToEntity(id);
        getComponentCamera(id)->fovy = 75.0f;
        getComponentCamera(id)->aspect_ratio = 16.0f/9.0f;
        getComponentCamera(id)->near = 1.0f;
        getComponentCamera(id)->far = 100.0f;
        getComponentCamera(id)->follow_entity = 3;
    }
    {
        int id = createEntity();
        addComponentTransformToEntity(id);
        getComponentTransform(id)->position.y = 1.0f;
        getComponentTransform(id)->position.x = 1.0f;
        addComponentLightToEntity(id);
        getComponentLight(id)->light_id = 0;
        getComponentLight(id)->color = 0xFFFFFFFF;
    }
    {
        int id = createEntity();
        addComponentTransformToEntity(id);
        getComponentTransform(id)->position.y = -2.0f;
        addComponentRenderToEntity(id);
        getComponentRender(id)->model_id = loadModel("assets/map.obj", "assets/map.dds", VERTEX_ALL, 256);
    }
    {
        int id = createEntity();
        addComponentTransformToEntity(id);
        getComponentTransform(id)->position.y = -1.4f;
        addComponentRenderToEntity(id);
        getComponentRender(id)->model_id = loadModel("assets/test.obj", "assets/test.dds", VERTEX_ALL, 128);
        addComponentMotionToEntity(id);
        getComponentMotion(id)->drag = 0.95f;
        addComponentControllerToEntity(id);
    }

    int val = 0;
    while(running()) {
        sceGuStart(GU_DIRECT, list);

        // clear screen
        sceGuClearColor(0x554433);
        sceGuClearDepth(0);
        sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

        updateSystemControlling();
        updateSystemMoving();
        updateSystemLightning();
        updateSystemFilming();
        updateSystemDrawing();

        sceGuFinish();
        sceGuSync(0, 0);

        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();

        val++;
    }

    vfree(fbp0);
    vfree(zbp);

    sceGuTerm();

    sceKernelExitGame();
    return 0;
}
