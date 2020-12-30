#include <components/render.h>
#include <ecmanager.h>

struct renderComponent *getComponentRender(int entity_id) {
    for (int i = 0; i < ecmanager_data.components_render_size; i++) {
        if (ecmanager_data.components_render[i].entity_id != entity_id) continue;

        return &ecmanager_data.components_render[i].component;
    }

    return NULL;
}

void addComponentRenderToEntity(int entity_id) {
    ecmanager_data.components_render[ecmanager_data.components_render_size++].entity_id = entity_id;
}
