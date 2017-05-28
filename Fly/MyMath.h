#pragma once
#ifndef _Math
#define _Math
#include "AEVec2.h"
#include "AEEngine.h"
#include <time.h>
extern class Timer
{
private:
	long start = 0, end = 0;
public:
	void Start();
	void End();
	void Reset();
	long getLength() const;
};
int IsCrash(AEVec2 pInst, AEVec2 pInstOther, float scale1, float scale2);
void IsNull(AEGfxVertexList *x);
void MatrixScale(AEMtx33 &matrix, float scale);
void MatrixTranslate(AEMtx33 &matrix, float x, float y);
void MatrixRot(AEMtx33 &matrix, float dircur);
void MatrixConcat(AEMtx33 &result, AEMtx33 &m2, AEMtx33 &m3);
#endif

