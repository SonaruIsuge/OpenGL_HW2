// Gouraud shading with a single light source at (4, 4, 0);
// 與 Exmaple4 相同, Light 的參數改成以結構的方式傳遞
// 同時實現 Per Pixel Lighting、Nonphotorealistic Shading  與 Silhouette


#include "HW/Application.h"

Application* application = nullptr;


//----------------------------------------------------------------------------
// 函式的原型宣告
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
	glutSwapBuffers();	// 交換 Frame Buffer
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
		case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵

			break;
		case GLUT_KEY_RIGHT:	// 目前按下的是向右方向鍵

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
    glutKeyboardFunc( Win_Keyboard );	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutKeyboardUpFunc(Win_Keyboard_Up);
	glutSpecialFunc( Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
    glutDisplayFunc( GL_Display );
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc(IdleProcess);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	// 這一行會讓視窗被強迫關閉時，程式的執行會回到 glutMainLoop(); 的下一行
	glutMainLoop();
	releaseResources();
    return 0;
}