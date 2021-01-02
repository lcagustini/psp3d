#include <systems/filming.h>
#include <ecmanager.h>
#include <pspgum.h>
#include <pspgu.h>
#include <vector.h>

void updateSystemFilming() {
    for (int i = 0; i < ecmanager_data.entity_ids_size; i++) {
        int entity_id = ecmanager_data.entity_ids[i];

        struct transformComponent *transform_component = getComponentTransform(entity_id);
        if (transform_component == NULL) continue;
        struct cameraComponent *camera_component = getComponentCamera(entity_id);
        if (camera_component == NULL) continue;

        if (camera_component->follow_entity >= 0) {
            struct transformComponent *follow_transform = getComponentTransform(camera_component->follow_entity);
            struct motionComponent *follow_motion = getComponentMotion(camera_component->follow_entity);

            if (follow_transform && follow_motion) {
                ScePspFVector3 dir = follow_motion->velocity;
                vectorNormalize(&dir);
                dir = vectorScale(5, dir);

                transform_component->position = follow_transform->position;
                transform_component->position.x = -transform_component->position.x;
                transform_component->position.z = -transform_component->position.z;
                transform_component->position = vectorAdd(transform_component->position, dir);

                transform_component->rotation = follow_transform->rotation;
                transform_component->rotation.y = GU_PI-transform_component->rotation.y;
            }
        }

        // setup projection (lens)
        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();
        sceGumPerspective(camera_component->fovy, camera_component->aspect_ratio, camera_component->near, camera_component->far);

        // setup camera
        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();
        sceGumRotateXYZ(&transform_component->rotation);
        sceGumTranslate(&transform_component->position);
    }
}
