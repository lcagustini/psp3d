#ifndef render_h
#define render_h

#include <model.h>

struct renderComponent {
    int model_id;
};

struct renderComponent *getComponentRender(int entity_id);
void addComponentRenderToEntity(int entity_id);

#endif
