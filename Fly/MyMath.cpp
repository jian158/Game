//#ifndef _Math
//#define _Math
#include "MyMath.h"
#include <math.h>
void Timer::Start()
{
	start=clock();
}
void Timer::End()
{
	end = clock();
}
void Timer::Reset()
{
	start = end;
}

long Timer::getLength() const
{
	return end - start;
}

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

void MatrixScale(AEMtx33& matrix, float scale)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			matrix.m[i][j] = 0;
	matrix.m[0][0] = scale;
	matrix.m[1][1] = scale;
	matrix.m[2][2] = 1.0f;
}

void MatrixTranslate(AEMtx33& matrix, float x, float y)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			matrix.m[i][j] = 0;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[0][2] = x;
	matrix.m[1][2] = y;
}

void MatrixRot(AEMtx33& matrix, float dircur)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			matrix.m[i][j] = 0;
	matrix.m[0][0] = cosf(dircur);
	matrix.m[1][1] = cosf(dircur);
	matrix.m[2][2] = 1.0f;
	matrix.m[0][1] = -sinf(dircur);
	matrix.m[1][0] = sinf(dircur);
}

void MatrixConcat(AEMtx33& result, AEMtx33& m2, AEMtx33& m3)
{
	int i, j, k;
	float s = 0;
	for (i=0;i<3;i++)
	for (k=0;k<3;k++)
	{
		for (j=0;j<3;j++)
		{
			s+=m2.m[i][j] * m3.m[j][k];
		}
		result.m[i][k] = s;
		s = 0;
	}		
}

//#endif
