#include <systems/controlling.h>
#include <ecmanager.h>
#include <pspctrl.h>

void updateSystemControlling() {
    for (int i = 0; i < ecmanager_data.entity_ids_size; i++) {
        int entity_id = ecmanager_data.entity_ids[i];

        struct controllerComponent *controller_component = getComponentController(entity_id);
        if (controller_component == NULL) continue;
        struct motionComponent *motion_component = getComponentMotion(entity_id);
        if (motion_component == NULL) continue;

        SceCtrlData pad;
        sceCtrlReadBufferPositive(&pad, 1);

        motion_component->acceleration.x = DT * ((controller_component->invert_x ? (128 - pad.Lx) : (pad.Lx - 128)) / 255.0f);
        motion_component->acceleration.z = DT * ((controller_component->invert_y ? (128 - pad.Ly) : (pad.Ly - 128)) / 255.0f);

        if (pad.Buttons != 0){
            if (pad.Buttons & PSP_CTRL_SQUARE){
            }
            if (pad.Buttons & PSP_CTRL_TRIANGLE){
            }
            if (pad.Buttons & PSP_CTRL_CIRCLE){
            }
            if (pad.Buttons & PSP_CTRL_CROSS){
            }

            if (pad.Buttons & PSP_CTRL_UP){
            }
            if (pad.Buttons & PSP_CTRL_DOWN){
            }
            if (pad.Buttons & PSP_CTRL_LEFT){
            }
            if (pad.Buttons & PSP_CTRL_RIGHT){
            }

            if (pad.Buttons & PSP_CTRL_START){
            }
            if (pad.Buttons & PSP_CTRL_SELECT){
            }
            if (pad.Buttons & PSP_CTRL_LTRIGGER){
            }
            if (pad.Buttons & PSP_CTRL_RTRIGGER){
            }
        }
    }
}
