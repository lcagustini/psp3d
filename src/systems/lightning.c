#include <systems/lightning.h>
#include <ecmanager.h>
#include <pspgu.h>

void updateSystemLightning() {
    for (int i = 0; i < ecmanager_data.entity_ids_size; i++) {
        int entity_id = ecmanager_data.entity_ids[i];

        struct transformComponent *transform_component = getComponentTransform(entity_id);
        if (transform_component == NULL) continue;
        struct lightComponent *light_component = getComponentLight(entity_id);
        if (light_component == NULL) continue;

        sceGuLight(light_component->light_id, GU_POINTLIGHT, GU_DIFFUSE_AND_SPECULAR, &transform_component->position);
        sceGuLightColor(light_component->light_id, GU_DIFFUSE, light_component->color);
        sceGuLightColor(light_component->light_id, GU_SPECULAR, light_component->color);
        sceGuLightAtt(light_component->light_id, 0.0f, 1.0f, 0.0f);
    }

    sceGuSpecular(12.0f);
    sceGuAmbient(0x00222222);
}
