
//***** monkeygame.h *****

class BBMonkeyGame : public BBGlfwGame{
public:

	static void Main( int args,const char *argv[] );
};

//***** monkeygame.cpp *****

#define _QUOTE(X) #X
#define _STRINGIZE(X) _QUOTE(X)

void Init_GL_Exts();

void BBMonkeyGame::Main( int argc,const char *argv[] ){

        if( !glfwInit() ){
                puts( "glfwInit failed" );           
                exit(-1);
        }

        _monitor = glfwGetPrimaryMonitor();

        GLFWvidmode desktopMode;
        desktopMode = glfwGetVideoMode(_monitor);

        int w=CFG_GLFW_WINDOW_WIDTH;
        if( !w ) w=desktopMode.width;

        int h=CFG_GLFW_WINDOW_HEIGHT;
        if( !h ) h=desktopMode.height;

        glfwWindowHint(GLFW_RESIZABLE,CFG_GLFW_WINDOW_RESIZABLE ? GL_FALSE : GL_TRUE );
        glfwWindowHint(GLFW_VISIBLE, GL_TRUE );
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

        glfwWindowHint(GLFW_RED_BITS, 5);
        glfwWindowHint(GLFW_GREEN_BITS, 6);
        glfwWindowHint(GLFW_BLUE_BITS, 5);
        glfwWindowHint(GLFW_SAMPLES, 4);

        _window = glfwCreateWindow(w, h, _STRINGIZE(CFG_GLFW_WINDOW_TITLE), CFG_GLFW_WINDOW_FULLSCREEN?_monitor:NULL, NULL);
        glfwMakeContextCurrent(_window);

        if (!_window) {
                puts( "glfwCreateWindow failed" );
                exit(-1);
        }    		

        if (!CFG_GLFW_WINDOW_FULLSCREEN) 
        	glfwSetWindowPos( _window, (desktopMode.width-w)/2,(desktopMode.height-h)/2 );



#if CFG_OPENGL_INIT_EXTENSIONS
	Init_GL_Exts();
#endif

	BBMonkeyGame *game=new BBMonkeyGame();
	
	try{
	
		bb_std_main( argc,argv );
		
	}catch( ThrowableObject *ex ){
	
		game->Die( ex );
		return;
	}

	if( game->Delegate() ) game->Run();
	
	glfwTerminate();
	
	exit( 0 );
}
