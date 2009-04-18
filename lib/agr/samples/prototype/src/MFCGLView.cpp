// MFCGLView.cpp : implementation of the CMFCGLView class
//

#include "stdafx.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "WiimoteTest.h"

#include <vector>

#include "WiiAccelerometer.h"

#include "WiimoteTestDoc.h"

#include "MFCGLView.h"
#include ".\mfcglview.h"

using namespace AGR;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCGLView

IMPLEMENT_DYNCREATE(CMFCGLView, CView)

BEGIN_MESSAGE_MAP(CMFCGLView, CView)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMFCGLView construction/destruction

CMFCGLView::CMFCGLView()
{
	m_fPosX = 0.0f;    // X position of model in camera view
	m_fPosY = 0.0f;    // Y position of model in camera view
	m_fPosZ = -400.0f;  // Z position of model in camera view
	m_fRotX = 0.0f;    // Rotation on model in camera view
	m_fRotY = 0.0f;    // Rotation on model in camera view
}

CMFCGLView::~CMFCGLView()
{
}

BOOL CMFCGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
 
	return CView::PreCreateWindow(cs);
}

// CMFCGLView drawing

void CMFCGLView::OnDraw(CDC* pDC)
{
	CWiimoteTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

#ifdef _DEBUG
void CMFCGLView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWiimoteTestDoc* CMFCGLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWiimoteTestDoc)));
	return (CWiimoteTestDoc*)m_pDocument;
}
#endif //_DEBUG

void CMFCGLView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	oglInitialize();
	SetTimer(1, 20, 0 );
}

void CMFCGLView::oglInitialize(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,    // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16,    // z-buffer depth
		0, 0, 
		PFD_MAIN_PLANE,
		0, 0, 0, 0,
	};

	// Get device context only once.
	m_hdc = GetDC()->m_hDC;

	// Pixel format.
	m_nPixelFormat = ChoosePixelFormat(m_hdc, &pfd);
	SetPixelFormat(m_hdc, m_nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	m_hrc = wglCreateContext(m_hdc);
	wglMakeCurrent(m_hdc, m_hrc);

	// Basic Setup:
	//
	// Set color to use when clearing the background.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Send draw request
	OnDraw(NULL);
}


void CMFCGLView::OnTimer(UINT nIDEvent)
{
	// Clear color and depth buffer bits
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawScene();

	// Swap buffers
	SwapBuffers(m_hdc);

	SetTimer(1, 100, 0 );

	CView::OnTimer(nIDEvent);
}

void CMFCGLView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

	// Map the OpenGL coordinates.
	glViewport(0, 0, cx, cy);

	// Projection view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set our current view perspective
	gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);

	// Model view
	glMatrixMode(GL_MODELVIEW);
}



void CMFCGLView::DrawScene(void)
{
	
	CWiimoteTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	glLoadIdentity();
	glTranslatef( m_fPosX, m_fPosY, m_fPosZ );
	glRotatef( m_fRotX, 1.0f, 0.0f, 0.0f );
	glRotatef( m_fRotY, 0.0f, 1.0f, 0.0f );

//	glTranslatef( 0, 0, -400 );
//	glRotatef( 30, 0.5, -1, 0 );
	glLineWidth( 1.0f );
	
	glBegin(GL_LINES);
		glColor3f( 1.0f, 0.0f, 0.0f );	// red X
		glVertex3f( 0, 0, 0);
		glVertex3f( 50, 0, 0);
		glColor3f( 0.0f, 1.0f, 0.0f );	// green Y
		glVertex3f( 0, 0, 0);
		glVertex3f( 0, 50, 0);
		glColor3f( 0.0f, 0.0f, 1.0f );	// blue Z
		glVertex3f( 0, 0, 0);
		glVertex3f( 0, 0, 50);
	glEnd();


	glColor3f( 0.2f, 0.2f, 0.2f );
	glBegin(GL_LINE_STRIP);
		{
			unsigned int startPoint = pDoc->m_arrayLast1;
			if( startPoint == SIZE_ACC_ARRAY )
				startPoint = 0;
			static const int posGraph1_x = 0;
			static const int posGraph1_y = 100;
			for( int x=0; x<60; x++ )
			{
				int i = (startPoint-x);
				if( i<0 ) 
					i+=SIZE_ACC_ARRAY;
				if( pDoc->m_acc1_x_array[i] != 0 )
					glVertex3f( (pDoc->m_acc1_x_array[i])-128, (pDoc->m_acc1_y_array[i])-128, (pDoc->m_acc1_z_array[i])-128 );
			}			
		}
	glEnd();

#if USE_OLD_CODE

	// Selected movement
	glColor3f( 0.0f, 0.0f, 0.0f );
	glLineWidth( 2.0f );
	glBegin(GL_LINE_STRIP);
		{
			if( pDoc-> m_selectedMovement && !pDoc->m_selectedMovement->empty() )
			{
				std::vector<cVector3int>::iterator iter;
				for( iter = pDoc->m_selectedMovement->begin(); iter != pDoc->m_selectedMovement->end(); iter++ )
				{
					glVertex3f( iter->x-128, iter->y-128, iter->z-128 );
				}
			}
		}
	glEnd();
	glLineWidth( 1.0f );

#else // using GR
	
	// Selected movement 
	glColor3f( 0.0f, 0.0f, 0.0f );
	glLineWidth( 2.0f );
	glBegin(GL_LINE_STRIP);
		{
			if( pDoc->m_GRselectedMovement && 
				0 != pDoc->m_GRselectedMovement->getSize() )
			{
				for( int i=0; i< pDoc->m_GRselectedMovement->getSize(); i++ )
				{
					const cVector3int vec = pDoc->m_GRselectedMovement->getData( i );
					glVertex3f( vec.x-128, vec.y-128, vec.z-128 );
				}
			}
		}
	glEnd();
	glLineWidth( 1.0f );

#endif

		
	// derivate last movement
/*	glColor3f( 0.9f, 0.0f, 0.0f );
	glBegin(GL_LINE_STRIP);	
		if( !pDoc->m_lastMovement.empty() )
		{
			std::list<cVector3int>::iterator iter;
			std::list<cVector3int>::iterator prev = NULL;
			for( iter = pDoc->m_lastMovement.begin(); iter != pDoc->m_lastMovement.end(); iter++ )
			{
				if( prev!=NULL )
				{
					glVertex3f( (iter->x - prev->x), (iter->y - prev->y), (iter->z - prev->z) );
				}
				prev = iter;
			}
		}
	glEnd(); */
}

void CMFCGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	int diffX = (int)(point.x - m_fLastX);
	int diffY = (int)(point.y - m_fLastY);
	m_fLastX  = (float)point.x;
	m_fLastY  = (float)point.y;

	// Left mouse button
	if (nFlags & MK_LBUTTON)
	{
		m_fRotX += (float)0.5f * diffY;

		if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f))
		{
			m_fRotX = 0.0f;
		}

		m_fRotY += (float)0.5f * diffX;

		if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f))
		{
			m_fRotY = 0.0f;
		}
	}

	// Right mouse button
	else if (nFlags & MK_RBUTTON)
	{
		m_fPosZ -= (float)0.5f * diffY;
	}

	// Middle mouse button
	else if (nFlags & MK_MBUTTON)
	{
		m_fPosX += (float)0.2f * diffX;
		m_fPosY -= (float)0.2f * diffY;
	}

   OnDraw(NULL);

	CView::OnMouseMove(nFlags, point);
}
