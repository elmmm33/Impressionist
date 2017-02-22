#pragma once
//
// S_PointBrush.h
//
// The header file for Scattered Point Brush. 
//

#ifndef S_POINTBRUSH_H
#define S_POINTBRUSH_H

#include "ImpBrush.h"

class S_PointBrush : public ImpBrush
{
public:
	S_PointBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif