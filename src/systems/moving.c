#include <systems/moving.h>
#include <ecmanager.h>
#include <pspctrl.h>
#include <vector.h>
#include <stdio.h>

void updateSystemMoving() {
    for (int i = 0; i < ecmanager_data.entity_ids_size; i++) {
        int entity_id = ecmanager_data.entity_ids[i];

        struct motionComponent *motion_component = getComponentMotion(entity_id);
        if (motion_component == NULL) continue;
        struct transformComponent *transform_component = getComponentTransform(entity_id);
        if (transform_component == NULL) continue;

        transform_component->position = vectorAdd(transform_component->position, motion_component->velocity);

        motion_component->velocity = vectorAdd(motion_component->velocity, motion_component->acceleration);
        motion_component->velocity = vectorScale(motion_component->drag, motion_component->velocity);

        ScePspFVector3 forward = {0, 0, 1};
        ScePspFVector3 dir = motion_component->velocity;
        vectorNormalize(&dir);

        ScePspFQuaternion quat = getRotationQuat(forward, dir);
        if (quat.w == 1.0f && quat.x == 0 && quat.y == 0 && quat.z == 0) {
            transform_component->rotation.x = 0;
            transform_component->rotation.y = 0;
            transform_component->rotation.z = 0;
        }
        else {
            transform_component->rotation = quatToEuler(quat);
        }
    }
}
