// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image


	int		clearCanvas();                  // called by the UI to clear the drawing canvas
	void		setBrushType(int type);			// called by the UI to set the brushType
	
	//add by sherry
	int		getSize();						// get the UI size
	int		getLineWidth();				// get the Line width
	int		getLineAngle();				// get the Line angle
	double getAlpha();					// get alpha value
	double	getR_Color();				//get RGB color
	double	getG_Color();
	double	getB_Color();

	void		setSize(int size);				// set the UI size
	void		setStrokeDirect(int type);		//set brush direction
	void		setLineWidth(int width);		//set the Line width	
	void		setLineAngle(int angle);			//set the Linea angle

	//end

	char*	getImageName();					// get the current image name
	

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;

	// Add by Sherry
	int lineDirectPattern;				// dafult value for the direction of stroke
	int* m_ucAngle;						// parametersf for line brush angle	
	unsigned char* m_ucGray;
	unsigned char* m_ucBlur;

	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	Point currentPoint;		// Add by Sherry

	// Size of the brush.
	int m_nSize;							

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
