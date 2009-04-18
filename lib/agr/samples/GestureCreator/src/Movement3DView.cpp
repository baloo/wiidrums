// MFCGLView.cpp : implementation of the CMFCGLView class
//

#include "stdafx.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>

#include "GestureCreator.h"
#include "GestureCreatorDoc.h"
#include "Movement3DView.h"

#include "WiiAccelerometer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCGLView

IMPLEMENT_DYNCREATE(cMovement3DView, CView)

BEGIN_MESSAGE_MAP(cMovement3DView, CView)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMFCGLView construction/destruction

cMovement3DView::cMovement3DView()
{
	m_fPosX = 0.0f;		// X position of model in camera view
	m_fPosY = 0.0f;		// Y position of model in camera view
	m_fPosZ = -400.0f;	// Z position of model in camera view
	m_fRotX = 0.0f;		// Rotation on model in camera view
	m_fRotY = 0.0f;		// Rotation on model in camera view
}

cMovement3DView::~cMovement3DView()
{
}

BOOL cMovement3DView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CMFCGLView drawing

void cMovement3DView::OnDraw(CDC* pDC)
{
	cGestureCreatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

#ifdef _DEBUG
void cMovement3DView::AssertValid() const
{
	CView::AssertValid();
}

void cMovement3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

cGestureCreatorDoc* cMovement3DView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(cGestureCreatorDoc)));
	return (cGestureCreatorDoc*)m_pDocument;
}
#endif //_DEBUG

void cMovement3DView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	oglInitialize();
	SetTimer(1, 20, 0 );

	CRect rect;
	GetWindowRect( &rect );
	OnSize( 0, rect.Width(), rect.Height() );

	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return ;
	pDoc->m_ptMovement3DView = this;
}

void cMovement3DView::oglInitialize(void)
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


void cMovement3DView::OnTimer(UINT nIDEvent)
{
	// Clear color and depth buffer bits
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawScene();

	// Swap buffers
	SwapBuffers(m_hdc);

	SetTimer(1, 100, 0 );

	CView::OnTimer(nIDEvent);
}

void cMovement3DView::OnSize(UINT nType, int cx, int cy)
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



void cMovement3DView::DrawScene(void)
{
	
	cGestureCreatorDoc* pDoc = GetDocument();
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

	// Last accelerations 
	glColor3f( 0.2f, 0.2f, 0.2f );
	glBegin(GL_LINE_STRIP);
		{
			unsigned int startPoint = pDoc->m_bufferAccelerationLast;
			if( startPoint == pDoc->s_accelarationBufferSize )
				startPoint = 0;
			for( int x=0; x<pDoc->s_accelarationBufferSize; x++ )
			{
				int i = (startPoint-x);
				if( i<0 ) 
					i+=pDoc->s_accelarationBufferSize;
				if( pDoc->m_accelerationBuffer[i] != cVector3int( 0,0,0) )
					glVertex3i( pDoc->m_accelerationBuffer[i].x-128, 
								pDoc->m_accelerationBuffer[i].y-128, 
								pDoc->m_accelerationBuffer[i].z-128 );
			}			
		}
	glEnd();


	// Selected movement 
	glColor3f( 0.0f, 0.0f, 0.0f );
	glLineWidth( 2.0f );
	glBegin(GL_LINE_STRIP);
		{
			if( pDoc->m_selectedMovement && 
				0 != pDoc->m_selectedMovement->getSize() )
			{
				for( unsigned int i=0; i< pDoc->m_selectedMovement->getSize(); i++ )
				{
					const cVector3int vec = pDoc->m_selectedMovement->getData( i );
					glVertex3i( vec.x-128, vec.y-128, vec.z-128 );
				}
			}
		}
	glEnd();
	glLineWidth( 1.0f );

}

void cMovement3DView::OnMouseMove(UINT nFlags, CPoint point)
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
