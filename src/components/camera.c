#include <components/camera.h>
#include <ecmanager.h>

struct cameraComponent *getComponentCamera(int entity_id) {
    for (int i = 0; i < ecmanager_data.components_camera_size; i++) {
        if (ecmanager_data.components_camera[i].entity_id != entity_id) continue;

        return &ecmanager_data.components_camera[i].component;
    }

    return NULL;
}

void addComponentCameraToEntity(int entity_id) {
    ecmanager_data.components_camera[ecmanager_data.components_camera_size++].entity_id = entity_id;
}
