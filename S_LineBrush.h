#pragma once
//
// S_LineBrush.h
//
// The header file for Scattered Line Brush. 
//

#ifndef S_LINEBRUSH_H
#define S_LINEBRUSH_H

#include "ImpBrush.h"

class S_LineBrush : public ImpBrush
{
public:
	S_LineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif