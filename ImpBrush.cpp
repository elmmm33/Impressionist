//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"


// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();


	GLubyte color[4];
	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 );
	

	double chosen_R = pDoc->getR_Color();
	double chosen_G = pDoc->getG_Color();
	double chosen_B = pDoc->getB_Color();

	color[0] = (int)(color[0] * chosen_R);
	color[1] = (int)(color[1] * chosen_G);
	color[2] = (int)(color[2] * chosen_B);
	color[3] = pDoc->getAlpha() * 255;
	glColor4ubv(color);

}

void ImpBrush::savePaintForUndo() 
{
	ImpressionistDoc* pDoc = GetDocument();

	int dimension = pDoc->m_nPaintWidth * pDoc->m_nPaintHeight * 3;
	unsigned char* undo = new unsigned char[dimension];

	memcpy(undo, pDoc->m_ucPainting, dimension);
	pDoc->m_ucPainting_Undo.push(undo);
}