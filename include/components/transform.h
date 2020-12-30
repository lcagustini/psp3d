#ifndef transform_h
#define transform_h

#include <psptypes.h>

struct transformComponent {
    ScePspFVector3 position;
    ScePspFVector3 rotation;
    ScePspFVector3 scale;
};

struct transformComponent *getComponentTransform(int entity_id);
void addComponentTransformToEntity(int entity_id);

#endif
