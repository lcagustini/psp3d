#ifndef motion_h
#define motion_h

#include <psptypes.h>
#include <stdbool.h>

struct motionComponent {
    ScePspFVector3 velocity;
    ScePspFVector3 acceleration;

    float drag;
    float gravity;
};

struct motionComponent *getComponentMotion(int entity_id);
void addComponentMotionToEntity(int entity_id);

#endif
