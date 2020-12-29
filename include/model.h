#ifndef model_h
#define model_h

#include <psptypes.h>

#define MAX_OBJ_SIZE 10000

struct TNPVertex { // Texture, Normal, Position
    ScePspFVector2 texture;
    ScePspFVector3 normal;
    ScePspFVector3 position;
};
#define TNP_VERTEX_FORMAT (GU_TEXTURE_32BITF|GU_NORMAL_32BITF|GU_VERTEX_32BITF)

struct NPVertex { // Normal, Position
    ScePspFVector3 normal;
    ScePspFVector3 position;
};
#define NP_VERTEX_FORMAT (GU_NORMAL_32BITF|GU_VERTEX_32BITF)

struct PVertex { // Position
    ScePspFVector3 position;
};
#define P_VERTEX_FORMAT (GU_VERTEX_32BITF)

struct face {
    int vertices[3];
    int normals[3];
    int texture_coords[3];
};

enum faceType {
    VERTEX_ONLY,
    VERTEX_NORMAL,
    VERTEX_ALL
};

struct model {
    enum faceType face_type;

    void *texture_vram;
    int texture_size;

    struct TNPVertex *vertices;
    int num_vertices;
};

struct model loaded_models[10];
int loaded_models_n;

void drawModel(int model, ScePspFVector3 *pos, ScePspFVector3 *rot, ScePspFVector3 *scale);
void *loadTexture(const char *texture_filename, enum faceType face_type, int *texture_size);
void destroyModel(int model);
void loadModel(const char *obj_filename, const char *texture_filename, enum faceType face_type);

#endif
