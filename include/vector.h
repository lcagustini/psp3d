#ifndef vector_h
#define vector_h

#include <psptypes.h>

float vectorLenSquared(ScePspFVector3 v);
float vectorLen(ScePspFVector3 v);
void vectorNormalize2D(ScePspFVector3 *v);
void vectorNormalize(ScePspFVector3 *v);

ScePspFVector3 vectorScale(float a, ScePspFVector3 v);
float vectorDot(ScePspFVector3 a, ScePspFVector3 b);
ScePspFVector3 vectorCross(ScePspFVector3 a, ScePspFVector3 b);
ScePspFVector3 vectorAdd(ScePspFVector3 a, ScePspFVector3 b);
ScePspFVector3 vectorSubtract(ScePspFVector3 a, ScePspFVector3 b);

ScePspFVector3 vectorRotate(ScePspFVector3 v, ScePspFQuaternion q);
ScePspFQuaternion getRotationQuat(ScePspFVector3 from, ScePspFVector3 to);
ScePspFQuaternion quatMult(ScePspFQuaternion a, ScePspFQuaternion b);
ScePspFVector3 quatToEuler(ScePspFQuaternion q);

#endif
