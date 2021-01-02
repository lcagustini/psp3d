#ifndef camera_h
#define camera_h

#include <psptypes.h>

struct cameraComponent {
    float fovy;
    float aspect_ratio;
    float near, far;

    int follow_entity;
};

struct cameraComponent *getComponentCamera(int entity_id);
void addComponentCameraToEntity(int entity_id);

#endif
