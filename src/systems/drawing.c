#include <systems/drawing.h>
#include <ecmanager.h>

void updateSystemDrawing() {
    for (int i = 0; i < ecmanager_data.entity_ids_size; i++) {
        int entity_id = ecmanager_data.entity_ids[i];

        struct transformComponent *transform_component = getComponentTransform(entity_id);
        if (transform_component == NULL) continue;
        struct renderComponent *render_component = getComponentRender(entity_id);
        if (render_component == NULL) continue;

        drawModel(render_component->model_id, &transform_component->position, &transform_component->rotation, &transform_component->scale);
    }
}
