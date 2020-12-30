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

#include <callbacks.h>
#include <vram.h>
#include <vector.h>
#include <model.h>

#include <ecmanager.h>
#include <systems/drawing.h>

PSP_MODULE_INFO("psp3d", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

static unsigned int __attribute__((aligned(16))) list[262144];

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

#define LIGHT_DISTANCE 3.0f

int main(int argc, char* argv[]) {
    setupCallbacks();

    // flush cache so that no stray data remains
    sceKernelDcacheWritebackAll();

    // setup GU
    void* fbp0 = valloc(getVRAMSize(BUF_WIDTH, SCR_HEIGHT, GU_PSM_8888));
    void* fbp1 = valloc(getVRAMSize(BUF_WIDTH, SCR_HEIGHT, GU_PSM_8888));
    void* zbp = valloc(getVRAMSize(BUF_WIDTH, SCR_HEIGHT, GU_PSM_4444));

    sceGuInit();

    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888, (void *)(fbp0 - sceGeEdramGetAddr()), BUF_WIDTH);
    sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, (void *)(fbp1 - sceGeEdramGetAddr()), BUF_WIDTH);
    sceGuDepthBuffer((void *)(zbp - sceGeEdramGetAddr()), BUF_WIDTH);
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

    {
        int id = createEntity();
        addComponentTransformToEntity(id);
        addComponentRenderToEntity(id);
        getComponentRender(id)->model_id = loadModel("assets/test.obj", "assets/test.png", VERTEX_ALL);
    }

    int val = 0;
    while(running()) {
        sceGuStart(GU_DIRECT, list);

        // clear screen
        sceGuClearColor(0x554433);
        sceGuClearDepth(0);
        sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

        // setup lights
        {
            ScePspFVector3 pos = { cosf(val * (GU_PI/180)) * LIGHT_DISTANCE, 0, sinf(val * (GU_PI/180)) * LIGHT_DISTANCE };
            sceGuLight(0, GU_POINTLIGHT, GU_DIFFUSE_AND_SPECULAR, &pos);
            sceGuLightColor(0, GU_DIFFUSE, 0xffffffff);
            sceGuLightColor(0, GU_SPECULAR, 0xffffffff);
            sceGuLightAtt(0, 0.0f, 1.0f, 0.0f);
        }
        sceGuSpecular(12.0f);
        sceGuAmbient(0x00222222);

        // setup projection (lens)
        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();
        sceGumPerspective(75.0f, 16.0f / 9.0f, 1.0f, 100.0f);

        // setup camera
        sceGumMatrixMode(GU_VIEW);
        {
            ScePspFVector3 pos = {0, 0, -3.5f};

            sceGumLoadIdentity();
            sceGumTranslate(&pos);
        }

        updateSystemDrawing();

        sceGuFinish();
        sceGuSync(0, 0);

        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();

        val++;
    }

    vfree(fbp0);
    vfree(fbp1);
    vfree(zbp);

    sceGuTerm();

    sceKernelExitGame();
    return 0;
}
