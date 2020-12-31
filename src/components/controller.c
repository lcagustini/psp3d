#include <components/controller.h>
#include <ecmanager.h>

struct controllerComponent *getComponentController(int entity_id) {
    for (int i = 0; i < ecmanager_data.components_controller_size; i++) {
        if (ecmanager_data.components_controller[i].entity_id != entity_id) continue;

        return &ecmanager_data.components_controller[i].component;
    }

    return NULL;
}

void addComponentControllerToEntity(int entity_id) {
    ecmanager_data.components_controller[ecmanager_data.components_controller_size++].entity_id = entity_id;
}
