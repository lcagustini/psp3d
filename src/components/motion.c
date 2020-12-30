#include <components/motion.h>
#include <ecmanager.h>

struct motionComponent *getComponentMotion(int entity_id) {
    for (int i = 0; i < ecmanager_data.components_motion_size; i++) {
        if (ecmanager_data.components_motion[i].entity_id != entity_id) continue;

        return &ecmanager_data.components_motion[i].component;
    }

    return NULL;
}

void addComponentMotionToEntity(int entity_id) {
    ecmanager_data.components_motion[ecmanager_data.components_motion_size++].entity_id = entity_id;
}
