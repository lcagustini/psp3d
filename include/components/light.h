#ifndef light_h
#define light_h

#include <psptypes.h>

struct lightComponent {
    int light_id;
    uint32_t color;
};

struct lightComponent *getComponentLight(int entity_id);
void addComponentLightToEntity(int entity_id);

#endif
