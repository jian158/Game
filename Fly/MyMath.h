#pragma once
#ifndef _Math
#define _Math
#include "System.h"
#include <time.h>
#include "Vector.h"
class Timer
{
private:
	long start = 0, end = 0;
public:
	void Start();
	void End();
	void Reset();
	long getLength() const;
};
int IsCrash(Vec2 pInst, Vec2 pInstOther, float scale1, float scale2);
void IsNull(AEGfxVertexList *x);
void MatrixScale(Matrix &matrix, float xscale,float yscale);
void MatrixTranslate(Matrix &matrix, float x, float y);
void MatrixRot(Matrix &matrix, float dircur);
void MatrixConcat(Matrix &result, Matrix &m2, Matrix &m3);
void Vec2Add(Vec2 &pResult, Vec2& pVec0, Vec2& pVec1);
void Vec2Set(Vec2 &pResult, float x, float y);
void Vec2Sub(Vec2 &pResult, Vec2& pVec0, Vec2& pVec1);
#endif

