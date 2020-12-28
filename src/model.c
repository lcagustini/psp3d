#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <pspgu.h>
#include <pspgum.h>
#include <model.h>

void drawModel(int model, ScePspFVector3 *pos, ScePspFVector3 *rot, ScePspFVector3 *scale) {
    //glBindTexture(GL_TEXTURE_2D, loaded_models[model].texture_id);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    sceGumMatrixMode(GU_MODEL);
    sceGumLoadIdentity();
    sceGumScale(scale);
    sceGumRotateXYZ(rot);
    sceGumTranslate(pos);

    sceGuColor(0xffffff);
    sceGumDrawArray(GU_TRIANGLES,NP_VERTEX_FORMAT|GU_TRANSFORM_3D,loaded_models[model].num_vertices,NULL,loaded_models[model].vertices);

    //glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int loadTexture(const char *texture_filename, enum faceType face_type, int texture_size) {
    return 0;

    //GLuint texture_id;
    //FILE *f = fopen(texture_filename, "rb");

    //struct stat st;
    //stat(texture_filename, &st);

    //char *sur = malloc(st.st_size);
    //fread(sur, 1, st.st_size, f);
    //fclose(f);

    //assert(sur);

    //glGenTextures(1, &texture_id);
    //glBindTexture(GL_TEXTURE_2D, texture_id);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glCompressedTexImage2D(GL_TEXTURE_2D,  /* This must be GL_TEXTURE_2D */
            //0,             /* 0 = Texture does not contain Mip-Maps */
            //face_type == VERTEX_ALL ? GL_UNSIGNED_SHORT_5_6_5_VQ_TWID : GL_UNSIGNED_SHORT_1_5_5_5_VQ_TWID,        /* GL Compressed Color Format */
            //texture_size,           /* Texture Width */
            //texture_size,           /* Texture Height */
            //0,             /* This bit must be set to 0 */
            //st.st_size, /* Compressed Texture Size*/
            //sur);       /* Address of texture data in RAM: OpenGL will load the texture into VRAM for you.
                           //Because of this, make sure to call glDeleteTextures() as needed, as that will
                           //free the VRAM allocated for the texture. */
    //glBindTexture(GL_TEXTURE_2D, 0);

    //free(sur);

    //return texture_id;
}

void destroyModel(int model) {
    free(loaded_models[model].vertices);

    //if (loaded_models[model].face_type == VERTEX_ALL || loaded_models[model].face_type == VERTEX_ALL_ALPHA) {
        //glDeleteTextures(1, &loaded_models[model].texture_id);
    //}
}

void loadModel(const char *obj_filename, const char *texture_filename, enum faceType face_type, int texture_size) {
    struct {
        struct face *faces;
        int num_faces;
        ScePspFVector3 *vertices;
        int num_vertices;
        ScePspFVector3 *normals;
        int num_normals;
        ScePspFVector2 *texture_coords;
        int num_texture_coords;
    } file = {0};

    file.vertices = malloc(MAX_OBJ_SIZE * sizeof(ScePspFVector3));
    file.faces = malloc(MAX_OBJ_SIZE * sizeof(struct face));
    file.normals = malloc(MAX_OBJ_SIZE * sizeof(ScePspFVector3));
    file.texture_coords = malloc(MAX_OBJ_SIZE * sizeof(ScePspFVector2));

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
            t.x = 1 - t.x;
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
        model->texture_id = loadTexture(texture_filename, face_type, texture_size);
    }
    model->face_type = face_type;
    model->vertices = malloc(3 * file.num_faces * sizeof(struct NPVertex));
    model->num_vertices = 3 * file.num_faces;

    int k = 0;
    for (int i = 0; i < file.num_faces; i++) {
        model->vertices[k].normal = file.normals[file.faces[i].normals[0]];
        model->vertices[k+1].normal = file.normals[file.faces[i].normals[1]];
        model->vertices[k+2].normal = file.normals[file.faces[i].normals[2]];

        model->vertices[k].position = file.vertices[file.faces[i].vertices[0]];
        model->vertices[k+1].position = file.vertices[file.faces[i].vertices[1]];
        model->vertices[k+2].position = file.vertices[file.faces[i].vertices[2]];

        k += 3;
    }
    loaded_models_n++;

    free(file.vertices);
    free(file.faces);
    free(file.normals);
    free(file.texture_coords);
}
