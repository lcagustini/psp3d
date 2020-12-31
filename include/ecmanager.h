#ifndef ecmanager_h
#define ecmanager_h

#include <components/transform.h>
#include <components/render.h>
#include <components/motion.h>
#include <components/light.h>
#include <components/camera.h>
#include <components/controller.h>

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

    struct {
        int entity_id;
        struct lightComponent component;
    } components_light[MAX_COMPONENTS];
    int components_light_size;

    struct {
        int entity_id;
        struct cameraComponent component;
    } components_camera[MAX_COMPONENTS];
    int components_camera_size;

    struct {
        int entity_id;
        struct controllerComponent component;
    } components_controller[MAX_COMPONENTS];
    int components_controller_size;
} ecmanager_data;

int createEntity();

#endif
