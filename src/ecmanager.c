#include <ecmanager.h>

int global_entity_counter = 0;

int createEntity() {
    int id = global_entity_counter++;
    ecmanager_data.entity_ids[ecmanager_data.entity_ids_size++] = id;
    return id;
}
