#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <pspgu.h>
#include <pspgum.h>
#include <model.h>
#include <vram.h>
#include <upng.h>

void drawModel(int model, ScePspFVector3 *pos, ScePspFVector3 *rot, ScePspFVector3 *scale) {
    sceGumMatrixMode(GU_MODEL);
    sceGumLoadIdentity();
    sceGumTranslate(pos);
    sceGumRotateXYZ(rot);
    sceGumScale(scale);

    sceGuTexMode(GU_PSM_8888, 0, 0, GU_FALSE);
    sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
    sceGuTexLevelMode(GU_TEXTURE_AUTO, 0);
    sceGuTexFilter(GU_LINEAR, GU_LINEAR);
    sceGuTexImage(0, loaded_models[model].texture_size, loaded_models[model].texture_size, loaded_models[model].texture_size, loaded_models[model].texture_vram);

    sceGuColor(0xffffff);
    sceGumDrawArray(GU_TRIANGLES, TNP_VERTEX_FORMAT | GU_TRANSFORM_3D, loaded_models[model].num_vertices, NULL, loaded_models[model].vertices);
}

static void *loadTexture(const char *texture_filename, enum faceType face_type, int *texture_size) {
    upng_t *upng;

    upng = upng_new_from_file(texture_filename);
    if (upng != NULL) {
        upng_decode(upng);
        if (upng_get_error(upng) == UPNG_EOK) {
            *texture_size = upng_get_width(upng);
            void *texture = valloc(getVRAMSize(*texture_size, *texture_size, GU_PSM_8888));

            memcpy(texture, upng_get_buffer(upng), upng_get_size(upng));
            sceGuTexSync();

            return texture;
        }
        upng_free(upng);
    }

    return NULL;
}

void destroyModel(int model) {
    vfree(loaded_models[model].vertices);
    vfree(loaded_models[model].texture_vram);
    memset(&loaded_models[model], 0, sizeof(loaded_models[model]));
}

void loadModel(const char *obj_filename, const char *texture_filename, enum faceType face_type) {
    struct {
        struct face faces[MAX_OBJ_SIZE];
        int num_faces;
        ScePspFVector3 vertices[MAX_OBJ_SIZE];
        int num_vertices;
        ScePspFVector3 normals[MAX_OBJ_SIZE];
        int num_normals;
        ScePspFVector2 texture_coords[MAX_OBJ_SIZE];
        int num_texture_coords;
    } file = {0};

    FILE *f = fopen(obj_filename, "r");
    char buffer[40] = {0};
    while ((fscanf(f, " %s", buffer)) != EOF) {
        if (!strcmp(buffer, "v")) {
            ScePspFVector3 v = {};
            fscanf(f, " %f %f %f", &v.x, &v.y, &v.z);
            file.vertices[++file.num_vertices] = v;
        }
        else if (!strcmp(buffer, "vn")) {
            ScePspFVector3 n = {};
            fscanf(f, " %f %f %f", &n.x, &n.y, &n.z);
            file.normals[++file.num_normals] = n;
        }
        else if (!strcmp(buffer, "vt")) {
            ScePspFVector2 t = {};
            fscanf(f, " %f %f", &t.x, &t.y);
            t.y = 1 - t.y;
            file.texture_coords[++file.num_texture_coords] = t;
        }
        else if (!strcmp(buffer, "f")) {
            struct face face = {};
            switch (face_type) {
                case VERTEX_ONLY:
                    fscanf(f, " %d %d %d",
                            &face.vertices[0], &face.vertices[1], &face.vertices[2]
                          );
                    break;
                case VERTEX_NORMAL:
                    fscanf(f, " %d//%d %d//%d %d//%d",
                            &face.vertices[0], &face.normals[0], &face.vertices[1],
                            &face.normals[1], &face.vertices[2], &face.normals[2]
                          );
                    break;
                case VERTEX_ALL:
                    fscanf(f, " %d/%d/%d %d/%d/%d %d/%d/%d",
                            &face.vertices[0], &face.texture_coords[0], &face.normals[0],
                            &face.vertices[1], &face.texture_coords[1], &face.normals[1],
                            &face.vertices[2], &face.texture_coords[2], &face.normals[2]
                          );
                    break;
                default:
                    assert(false);
            }

            file.faces[file.num_faces++] = face;
        }
    }
    fclose(f);

    struct model *model = &loaded_models[loaded_models_n];

    if (face_type == VERTEX_ALL) {
        model->texture_vram = loadTexture(texture_filename, face_type, &model->texture_size);
    }
    model->face_type = face_type;
    model->vertices = valloc(3 * file.num_faces * sizeof(struct NPVertex));
    model->num_vertices = 3 * file.num_faces;

    int k = 0;
    for (int i = 0; i < file.num_faces; i++) {
        model->vertices[k].normal = file.normals[file.faces[i].normals[0]];
        model->vertices[k+1].normal = file.normals[file.faces[i].normals[1]];
        model->vertices[k+2].normal = file.normals[file.faces[i].normals[2]];

        model->vertices[k].position = file.vertices[file.faces[i].vertices[0]];
        model->vertices[k+1].position = file.vertices[file.faces[i].vertices[1]];
        model->vertices[k+2].position = file.vertices[file.faces[i].vertices[2]];

        model->vertices[k].texture = file.texture_coords[file.faces[i].texture_coords[0]];
        model->vertices[k+1].texture = file.texture_coords[file.faces[i].texture_coords[1]];
        model->vertices[k+2].texture = file.texture_coords[file.faces[i].texture_coords[2]];

        k += 3;
    }
    loaded_models_n++;
}
