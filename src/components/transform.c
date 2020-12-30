#include <components/transform.h>
#include <ecmanager.h>

struct transformComponent *getComponentTransform(int entity_id) {
    for (int i = 0; i < ecmanager_data.components_transform_size; i++) {
        if (ecmanager_data.components_transform[i].entity_id != entity_id) continue;

        return &ecmanager_data.components_transform[i].component;
    }

    return NULL;
}

void addComponentTransformToEntity(int entity_id) {
    ecmanager_data.components_transform[ecmanager_data.components_transform_size].entity_id = entity_id;
    ecmanager_data.components_transform[ecmanager_data.components_transform_size].component.scale.x = 1.0f;
    ecmanager_data.components_transform[ecmanager_data.components_transform_size].component.scale.y = 1.0f;
    ecmanager_data.components_transform[ecmanager_data.components_transform_size].component.scale.z = 1.0f;
    ecmanager_data.components_transform_size++;
}
