#include <components/light.h>
#include <ecmanager.h>

struct lightComponent *getComponentLight(int entity_id) {
    for (int i = 0; i < ecmanager_data.components_light_size; i++) {
        if (ecmanager_data.components_light[i].entity_id != entity_id) continue;

        return &ecmanager_data.components_light[i].component;
    }

    return NULL;
}

void addComponentLightToEntity(int entity_id) {
    ecmanager_data.components_light[ecmanager_data.components_light_size++].entity_id = entity_id;
}
