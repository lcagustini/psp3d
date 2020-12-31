#ifndef controller_h
#define controller_h

#include <psptypes.h>
#include <stdbool.h>

struct controllerComponent {
    bool invert_x;
    bool invert_y;
};

struct controllerComponent *getComponentController(int entity_id);
void addComponentControllerToEntity(int entity_id);

#endif
