//#ifndef _Math
//#define _Math
#include "MyMath.h"
#include <math.h>

int IsCrash(AEVec2 pInst,AEVec2 pInstOther,float scale1,float scale2)
{
	return fabsf(sqrtf(powf(pInst.y - pInstOther.y, 2) + powf(pInst.x - pInstOther.x, 2))-scale1-scale2)<5.0f?1:0;
}
void IsNull(AEGfxVertexList *x)
{
	if (x==NULL)
	{
		exit(1);
	}
}
//#endif
