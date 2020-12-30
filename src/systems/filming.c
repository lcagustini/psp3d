#include <systems/filming.h>
#include <ecmanager.h>
#include <pspgum.h>
#include <pspgu.h>

void updateSystemFilming() {
    for (int i = 0; i < ecmanager_data.entity_ids_size; i++) {
        int entity_id = ecmanager_data.entity_ids[i];

        struct transformComponent *transform_component = getComponentTransform(entity_id);
        if (transform_component == NULL) continue;
        struct cameraComponent *camera_component = getComponentCamera(entity_id);
        if (camera_component == NULL) continue;

        // setup projection (lens)
        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();
        sceGumPerspective(camera_component->fovy, camera_component->aspect_ratio, camera_component->near, camera_component->far);

        // setup camera
        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();
        sceGumTranslate(&transform_component->position);
        sceGumRotateXYZ(&transform_component->rotation);
    }
}
