// Gouraud shading with a single light source at (4, 4, 0);
// �P Exmaple4 �ۦP, Light ���ѼƧ令�H���c���覡�ǻ�
// �P�ɹ�{ Per Pixel Lighting�BNonphotorealistic Shading  �P Silhouette


#include "HW/Application.h"

Application* application = nullptr;


//----------------------------------------------------------------------------
// �禡���쫬�ŧi
extern void IdleProcess();
void releaseResources();

void init( void )
{
	application = new Application;
	application->Start();
}

//----------------------------------------------------------------------------
void GL_Display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window
	application->Draw();
	glutSwapBuffers();	// �洫 Frame Buffer
}


void onFrameMove(float delta)
{
	application->Update(delta);
	GL_Display();
}

//----------------------------------------------------------------------------

void Win_Keyboard( unsigned char key, int x, int y )
{
	application->OnKeyboardHit(key, x, y);
}


void Win_Keyboard_Up(unsigned char key, int x, int y) 
{
	application->OnKeyboardRelease(key, x, y);
}



//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) 
{
	application->OnMouseClick(button, state, x, y);
}
//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) 
{
	switch(key) {
		case GLUT_KEY_LEFT:		// �ثe���U���O�V����V��

			break;
		case GLUT_KEY_RIGHT:	// �ثe���U���O�V�k��V��

			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------
// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
void Win_PassiveMotion(int x, int y) 
{
	application->OnMouseMoveWithClick(x, y);
}

// The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
void Win_MouseMotion(int x, int y) 
{
	application->OnMouseMoveWithoutClick(x, y);
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
	glEnable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------

void releaseResources()
{
	application->Destroy();
}

int main( int argc, char **argv )
{  
	glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( SCREEN_SIZE, SCREEN_SIZE );

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow("Shading Example 4");

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE; 
    glewInit();  

    init();
	//glutSetCursor(GLUT_CURSOR_NONE);
	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);  
    glutKeyboardFunc( Win_Keyboard );	// �B�z ASCI ����p A�Ba�BESC ��...����
	glutKeyboardUpFunc(Win_Keyboard_Up);
	glutSpecialFunc( Win_SpecialKeyboard);	// �B�z NON-ASCI ����p F1�BHome�B��V��...����
    glutDisplayFunc( GL_Display );
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc(IdleProcess);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	// �o�@��|�������Q�j�������ɡA�{��������|�^�� glutMainLoop(); ���U�@��
	glutMainLoop();
	releaseResources();
    return 0;
}