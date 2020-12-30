#ifndef ecmanager_h
#define ecmanager_h

#include <components/transform.h>
#include <components/render.h>
#include <components/motion.h>

#define MAX_ENTITIES 20
#define MAX_COMPONENTS 10

struct {
    int entity_ids[MAX_ENTITIES];
    int entity_ids_size;

    struct {
        int entity_id;
        struct transformComponent component;
    } components_transform[MAX_COMPONENTS];
    int components_transform_size;

    struct {
        int entity_id;
        struct renderComponent component;
    } components_render[MAX_COMPONENTS];
    int components_render_size;

    struct {
        int entity_id;
        struct motionComponent component;
    } components_motion[MAX_COMPONENTS];
    int components_motion_size;
} ecmanager_data;

int createEntity();

#endif
