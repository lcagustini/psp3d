#ifndef motion_h
#define motion_h

#include <psptypes.h>

struct motionComponent {
    ScePspFVector3 velocity;
    ScePspFVector3 acceleration;
};

struct motionComponent *getComponentMotion(int entity_id);
void addComponentMotionToEntity(int entity_id);

#endif
