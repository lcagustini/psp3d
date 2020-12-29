#ifndef vector_h
#define vector_h

#include <math.h>

inline static float vectorLenSquared(ScePspFVector3 v) {
    float len = v.x * v.x + v.y * v.y + v.z * v.z;
    return len;
}

inline static float vectorLen(ScePspFVector3 v) {
    return sqrt(vectorLenSquared(v));
}

inline static void vectorNormalize2D(ScePspFVector3 *v) {
    float len = v->x * v->x + v->y * v->y;
    if (!len) return;
    float div = sqrt(len);
    v->x *= div;
    v->y *= div;
}

inline static void vectorNormalize(ScePspFVector3 *v) {
    float len = v->x * v->x + v->y * v->y + v->z * v->z;
    if (!len) return;
    float div = sqrt(len);
    v->x *= div;
    v->y *= div;
    v->z *= div;
}

inline static float vectorDot(ScePspFVector3 a, ScePspFVector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline ScePspFVector3 vectorCross(ScePspFVector3 a, ScePspFVector3 b) {
    ScePspFVector3 n = {
        .x = a.y*b.z - a.z*b.y,
        .y = a.z*b.x - a.x*b.z,
        .z = a.x*b.y - a.y*b.x
    };
    return n;
}

inline ScePspFVector3 vectorScale(float a, ScePspFVector3 v) {
    v.x *= a;
    v.y *= a;
    v.z *= a;
    return v;
}

inline ScePspFVector3 vectorAdd(ScePspFVector3 a, ScePspFVector3 b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

inline ScePspFVector3 vectorSubtract(ScePspFVector3 a, ScePspFVector3 b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

// https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
ScePspFVector3 vectorRotate(ScePspFVector3 v, ScePspFQuaternion q) {
    ScePspFVector3 u = {.x = q.x, .y = q.y, .z = q.z};
    float s = q.w;

    return vectorAdd(vectorScale(2.0f * vectorDot(u, v), u),
            vectorAdd(vectorScale(s*s - vectorDot(u, u), v),
                vectorScale(2.0f * s, vectorCross(u, v))));
}

ScePspFQuaternion getRotationQuat(ScePspFVector3 from, ScePspFVector3 to) {
    ScePspFQuaternion result;
    ScePspFVector3 H;

    H.x = from.x + to.x;
    H.y = from.y + to.y;
    H.z = from.z + to.z;
    vectorNormalize(&H);

    result.w = vectorDot(from, H);
    result.x = from.y*H.z - from.z*H.y;
    result.y = from.z*H.x - from.x*H.z;
    result.z = from.x*H.y - from.y*H.x;

    return result;
}

ScePspFQuaternion quatMult(ScePspFQuaternion a, ScePspFQuaternion b) {
    ScePspFQuaternion ret;

    ret.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
    ret.x = a.w*b.x + a.x*b.w - a.y*b.z + a.z*b.y;
    ret.y = a.w*b.y + a.x*b.z + a.y*b.w - a.z*b.x;
    ret.z = a.w*b.z - a.x*b.y + a.y*b.x + a.z*b.w;

    return ret;
}

#endif
