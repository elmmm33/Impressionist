//
// S_CircleBrush.h
//
// The header file for Scattered Circle Brush. 
//

#ifndef S_CIRCLEBRUSH_H
#define S_CIRCLEBRUSH_H

#include "ImpBrush.h"

class S_CircleBrush : public ImpBrush
{
public:
	S_CircleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
	void DrawCircle(Point source, Point target, float r);
	int size, num_points;
};

#endif