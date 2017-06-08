//#ifndef _Math
//#define _Math
#include "MyMath.h"
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

int IsCrash(Vec2 pInst,Vec2 pInstOther,float scale1,float scale2)
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

void MatrixScale(Matrix& matrix, float xscale,float yscale)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			matrix.m[i][j] = 0;
	matrix.m[0][0] = xscale;
	matrix.m[1][1] = yscale;
	matrix.m[2][2] = 1.0f;
}

void MatrixTranslate(Matrix& matrix, float x, float y)
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

void MatrixRot(Matrix& matrix, float dircur)
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

void MatrixConcat(Matrix& result, Matrix& m2, Matrix& m3)
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

void Vec2Add(Vec2& pResult, Vec2& pVec0, Vec2& pVec1)
{
	pResult.x = pVec0.x + pVec1.x;
	pResult.y = pVec0.y + pVec1.y;
}

void Vec2Set(Vec2& pResult, float x, float y)
{
	pResult.x = x;
	pResult.y = y;
}

void Vec2Sub(Vec2& pResult, Vec2& pVec0, Vec2& pVec1)
{
	pResult.x = pVec0.x - pVec1.x;
	pResult.y = pVec0.y - pVec1.y;
}

//#endif
