
//***** glfwgame.h *****

class BBGlfwGame : public BBGame{
public:
	BBGlfwGame();

	static BBGlfwGame *GlfwGame(){ return _glfwGame; }
	
	virtual void SetUpdateRate( int hertz );
	virtual int Millisecs();
	virtual bool PollJoystick( int port,Array<Float> joyx,Array<Float> joyy,Array<Float> joyz,Array<bool> buttons );
	virtual void OpenUrl( String url );
	virtual void SetMouseVisible( bool visible );

	virtual String PathToFilePath( String path );

	virtual unsigned char *LoadImageData( String path,int *width,int *height,int *depth );
	virtual unsigned char *LoadAudioData( String path,int *length,int *channels,int *format,int *hertz );
	
	virtual void Run();
	
private:
	static BBGlfwGame *_glfwGame;

	bool _iconified;
	
	double _nextUpdate;
	double _updatePeriod;
		
protected:
	static int TransKey( int key );
	static int KeyToChar( int key );
	
	static void  OnKey(GLFWwindow* window, int key,int action );
	static void  OnChar(GLFWwindow* window, unsigned int chr );
	static void  OnMouseButton(GLFWwindow* window, int button,int action );
	static void  OnMousePos(GLFWwindow* window,  int x,int y );
	static void  OnWindowClose(GLFWwindow* window);
};

//***** glfwgame.cpp *****

#define _QUOTE(X) #X
#define _STRINGIZE( X ) _QUOTE(X)

enum{
	VKEY_BACKSPACE=8,VKEY_TAB,
	VKEY_ENTER=13,
	VKEY_SHIFT=16,
	VKEY_CONTROL=17,
	VKEY_ESC=27,
	VKEY_SPACE=32,
	VKEY_PAGEUP=33,VKEY_PAGEDOWN,VKEY_END,VKEY_HOME,
	VKEY_LEFT=37,VKEY_UP,VKEY_RIGHT,VKEY_DOWN,
	VKEY_INSERT=45,VKEY_DELETE,
	VKEY_0=48,VKEY_1,VKEY_2,VKEY_3,VKEY_4,VKEY_5,VKEY_6,VKEY_7,VKEY_8,VKEY_9,
	VKEY_A=65,VKEY_B,VKEY_C,VKEY_D,VKEY_E,VKEY_F,VKEY_G,VKEY_H,VKEY_I,VKEY_J,
	VKEY_K,VKEY_L,VKEY_M,VKEY_N,VKEY_O,VKEY_P,VKEY_Q,VKEY_R,VKEY_S,VKEY_T,
	VKEY_U,VKEY_V,VKEY_W,VKEY_X,VKEY_Y,VKEY_Z,
	
	VKEY_LSYS=91,VKEY_RSYS,
	
	VKEY_NUM0=96,VKEY_NUM1,VKEY_NUM2,VKEY_NUM3,VKEY_NUM4,
	VKEY_NUM5,VKEY_NUM6,VKEY_NUM7,VKEY_NUM8,VKEY_NUM9,
	VKEY_NUMMULTIPLY=106,VKEY_NUMADD,VKEY_NUMSLASH,
	VKEY_NUMSUBTRACT,VKEY_NUMDECIMAL,VKEY_NUMDIVIDE,

	VKEY_F1=112,VKEY_F2,VKEY_F3,VKEY_F4,VKEY_F5,VKEY_F6,
	VKEY_F7,VKEY_F8,VKEY_F9,VKEY_F10,VKEY_F11,VKEY_F12,

	VKEY_LSHIFT=160,VKEY_RSHIFT,
	VKEY_LCONTROL=162,VKEY_RCONTROL,
	VKEY_LALT=164,VKEY_RALT,

	VKEY_TILDE=192,VKEY_MINUS=189,VKEY_EQUALS=187,
	VKEY_OPENBRACKET=219,VKEY_BACKSLASH=220,VKEY_CLOSEBRACKET=221,
	VKEY_SEMICOLON=186,VKEY_QUOTES=222,
	VKEY_COMMA=188,VKEY_PERIOD=190,VKEY_SLASH=191
};

BBGlfwGame *BBGlfwGame::_glfwGame;

BBGlfwGame::BBGlfwGame():
_iconified( false ){
	_glfwGame=this;
}

//***** BBGame *****

void BBGlfwGame::SetUpdateRate( int updateRate ){
	BBGame::SetUpdateRate( updateRate );
	if( _updateRate ){
		_updatePeriod=1.0/_updateRate;
		_nextUpdate=glfwGetTime()+_updatePeriod;
	}
}

int BBGlfwGame::Millisecs(){
	return int( glfwGetTime()*1000.0 );
}

bool BBGlfwGame::PollJoystick( int port,Array<Float> joyx,Array<Float> joyy,Array<Float> joyz,Array<bool> buttons ){

	int joy=GLFW_JOYSTICK_1+port;
	if( !glfwGetJoystickParam( joy,GLFW_PRESENT ) ) return false;

	int n_axes=glfwGetJoystickParam( joy,GLFW_AXES );
	int n_buttons=glfwGetJoystickParam( joy,GLFW_BUTTONS );

	float pos[6];
	memset( pos,0,sizeof(pos) );	
	glfwGetJoystickAxes( joy,pos,n_axes );
	
	float t;
	switch( n_axes ){
	case 4:	//my saitek...axes=4, buttons=14
		pos[4]=pos[2];
		pos[3]=pos[3];
		pos[2]=0;
		break;
	case 5:	//xbox360...axes=5, buttons=10
		t=pos[3];
		pos[3]=pos[4];
		pos[4]=t;
		break;
	}
	
	joyx[0]=pos[0];joyx[1]=pos[3];
	joyy[0]=pos[1];joyy[1]=pos[4];
	joyz[0]=pos[2];joyz[1]=pos[5];

	//Buttons...
	//	
	unsigned char buts[32];
	memset( buts,0,sizeof(buts) );
	glfwGetJoystickButtons( port,buts,n_buttons );

	for( int i=0;i<n_buttons;++i ) buttons[i]=(buts[i]==GLFW_PRESS);
	return true;
}

void BBGlfwGame::OpenUrl( String url ){
#if _WIN32
	char *cstr=url.ToCString<char>();
	ShellExecute( HWND_DESKTOP,"open",cstr,0,0,SW_SHOWNORMAL );
#elif __APPLE__
	UInt8 *cstr=url.ToCString<UInt8>();
	if( CFURLRef cfurl=CFURLCreateWithBytes( 0,cstr,url.Length(),kCFStringEncodingASCII,0 ) ){
		LSOpenCFURLRef( cfurl,0 );
		CFRelease( cfurl );
	}
#elif __linux
	char *cstr=( String( "xdg-open \"" )+url+"\"" ).ToCString<char>();
	system( cstr );
#endif
}

void BBGlfwGame::SetMouseVisible( bool visible ){
	if( visible ){
		glfwSetInputMode(_window, GLFW_CURSOR_MODE, 1);
	}else{
		glfwSetInputMode(_window, GLFW_CURSOR_MODE, 0);
	}
}

String BBGlfwGame::PathToFilePath( String path ){
	if( !path.StartsWith( "monkey:" ) ){
		return path;
	}else if( path.StartsWith( "monkey://data/" ) ){
		return String("./data/")+path.Slice(14);
	}else if( path.StartsWith( "monkey://internal/" ) ){
		return String("./internal/")+path.Slice(18);
	}else if( path.StartsWith( "monkey://external/" ) ){
		return String("./external/")+path.Slice(18);
	}
	return "";
}

unsigned char *BBGlfwGame::LoadImageData( String path,int *width,int *height,int *depth ){

	FILE *f=OpenFile( path,"rb" );
	if( !f ) return 0;
	
	unsigned char *data=stbi_load_from_file( f,width,height,depth,0 );
	fclose( f );
	
	gc_force_sweep=true;
	
	return data;
}

unsigned char *BBGlfwGame::LoadAudioData( String path,int *length,int *channels,int *format,int *hertz ){

	FILE *f=OpenFile( path,"rb" );
	if( !f ) return 0;
	
	unsigned char *data=0;
	
	if( path.ToLower().EndsWith( ".wav" ) ){
		data=LoadWAV( f,length,channels,format,hertz );
	}else if( path.ToLower().EndsWith( ".ogg" ) ){
		data=LoadOGG( f,length,channels,format,hertz );
	}
	fclose( f );

	gc_force_sweep=true;
	
	return data;
}

//glfw key to monkey key!
int BBGlfwGame::TransKey( int key ){

	if( key>='0' && key<='9' ) return key;
	if( key>='A' && key<='Z' ) return key;

	switch( key ){

	case ' ':return VKEY_SPACE;
	case ';':return VKEY_SEMICOLON;
	case '=':return VKEY_EQUALS;
	case ',':return VKEY_COMMA;
	case '-':return VKEY_MINUS;
	case '.':return VKEY_PERIOD;
	case '/':return VKEY_SLASH;
	case '~':return VKEY_TILDE;
	case '[':return VKEY_OPENBRACKET;
	case ']':return VKEY_CLOSEBRACKET;
	case '\"':return VKEY_QUOTES;
	case '\\':return VKEY_BACKSLASH;
	
	case '`':return VKEY_TILDE;
	case '\'':return VKEY_QUOTES;
	
	case GLFW_KEY_LSHIFT:return VKEY_LSHIFT;
	case GLFW_KEY_RSHIFT:return VKEY_RSHIFT;
	case GLFW_KEY_LCTRL:return VKEY_LCONTROL;
	case GLFW_KEY_RCTRL:return VKEY_RCONTROL;
	
	case GLFW_KEY_BACKSPACE:return VKEY_BACKSPACE;
	case GLFW_KEY_TAB:return VKEY_TAB;
	case GLFW_KEY_ENTER:return VKEY_ENTER;
	case GLFW_KEY_ESC:return VKEY_ESC;
	case GLFW_KEY_INSERT:return VKEY_INSERT;
	case GLFW_KEY_DEL:return VKEY_DELETE;
	case GLFW_KEY_PAGEUP:return VKEY_PAGEUP;
	case GLFW_KEY_PAGEDOWN:return VKEY_PAGEDOWN;
	case GLFW_KEY_HOME:return VKEY_HOME;
	case GLFW_KEY_END:return VKEY_END;
	case GLFW_KEY_UP:return VKEY_UP;
	case GLFW_KEY_DOWN:return VKEY_DOWN;
	case GLFW_KEY_LEFT:return VKEY_LEFT;
	case GLFW_KEY_RIGHT:return VKEY_RIGHT;
	
	case GLFW_KEY_F1:return VKEY_F1;
	case GLFW_KEY_F2:return VKEY_F2;
	case GLFW_KEY_F3:return VKEY_F3;
	case GLFW_KEY_F4:return VKEY_F4;
	case GLFW_KEY_F5:return VKEY_F5;
	case GLFW_KEY_F6:return VKEY_F6;
	case GLFW_KEY_F7:return VKEY_F7;
	case GLFW_KEY_F8:return VKEY_F8;
	case GLFW_KEY_F9:return VKEY_F9;
	case GLFW_KEY_F10:return VKEY_F10;
	case GLFW_KEY_F11:return VKEY_F11;
	case GLFW_KEY_F12:return VKEY_F12;
	}
	return 0;
}

//monkey key to special monkey char
int BBGlfwGame::KeyToChar( int key ){
	switch( key ){
	case VKEY_BACKSPACE:
	case VKEY_TAB:
	case VKEY_ENTER:
	case VKEY_ESC:
		return key;
	case VKEY_PAGEUP:
	case VKEY_PAGEDOWN:
	case VKEY_END:
	case VKEY_HOME:
	case VKEY_LEFT:
	case VKEY_UP:
	case VKEY_RIGHT:
	case VKEY_DOWN:
	case VKEY_INSERT:
		return key | 0x10000;
	case VKEY_DELETE:
		return 127;
	}
	return 0;
}

void BBGlfwGame::OnMouseButton(GLFWwindow* window, int button,int action ){
	switch( button ){
	case GLFW_MOUSE_BUTTON_LEFT:button=0;break;
	case GLFW_MOUSE_BUTTON_RIGHT:button=1;break;
	case GLFW_MOUSE_BUTTON_MIDDLE:button=2;break;
	default:return;
	}
	int x,y;
	glfwGetCursorPos(_window, &x, &y);
	//glfwGetMousePos( &x,&y );
	switch( action ){
	case GLFW_PRESS:
		_glfwGame->MouseEvent( BBGameEvent::MouseDown,button,x,y );
		break;
	case GLFW_RELEASE:
		_glfwGame->MouseEvent( BBGameEvent::MouseUp,button,x,y );
		break;
	}
}

void BBGlfwGame::OnMousePos(GLFWwindow* window,  int x,int y ){
	_game->MouseEvent( BBGameEvent::MouseMove,-1,x,y );
}

void BBGlfwGame::OnWindowClose(GLFWwindow* window){
	_game->KeyEvent( BBGameEvent::KeyDown,0x1b0 );
	_game->KeyEvent( BBGameEvent::KeyUp,0x1b0 );
	//return GL_FALSE;
}

void BBGlfwGame::OnKey(GLFWwindow* window, int key,int action ){

	key=TransKey( key );
	if( !key ) return;
	
	switch( action ){
	case GLFW_PRESS:
		_glfwGame->KeyEvent( BBGameEvent::KeyDown,key );
		if( int chr=KeyToChar( key ) ) _game->KeyEvent( BBGameEvent::KeyChar,chr );
		break;
	case GLFW_RELEASE:
		_glfwGame->KeyEvent( BBGameEvent::KeyUp,key );
		break;
	}
}

void BBGlfwGame::OnChar(GLFWwindow* window, unsigned int chr ){

	//switch( action ){
	//case GLFW_PRESS:
		_glfwGame->KeyEvent( BBGameEvent::KeyChar,chr );
	//	break;
	//}
}

void BBGlfwGame::Run(){
	// TODO: ? 
	//glfwEnable( GLFW_KEY_REPEAT );
	//glfwDisable( GLFW_AUTO_POLL_EVENTS );

	glfwSetKeyCallback( _window, OnKey );
	glfwSetCharCallback( _window, OnChar );
	glfwSetMouseButtonCallback(_window, OnMouseButton );
	glfwSetCursorPosCallback(_window, OnMousePos );
	glfwSetWindowCloseCallback(	_window, OnWindowClose );
	
	StartGame();

	RenderGame();
	
	for( ;; ){
	
		glfwPollEvents();
		// ? if( !glfwGetWindowParam( GLFW_OPENED ) ) break;
	
		if( glfwGetWindowParam(_window, GLFW_ICONIFIED ) ){
			if( !_suspended ){
				_iconified=true;
				SuspendGame();
			}
		}else if( glfwGetWindowParam(_window, GLFW_FOCUSED ) ){ // was GLFW_ACTIVE
			if( _suspended ){
				_iconified=false;
				ResumeGame();
				_nextUpdate=glfwGetTime();
			}
		}else if( CFG_MOJO_AUTO_SUSPEND_ENABLED ){
			if( !_suspended ){
				SuspendGame();
			}
		}
	
		if( !_updateRate || _suspended ){
			if( !_iconified ) RenderGame();
			glfwWaitEvents();
			continue;
		}
		
		double delay=_nextUpdate-glfwGetTime();
		if( delay>0 ){
			// ? glfwSleep( delay ); // function has been removed from glfw
			continue;
		}

		int updates;
		for( updates=0;updates<4;++updates ){
			_nextUpdate+=_updatePeriod;
			UpdateGame();
			if( !_updateRate ) break;
			delay=_nextUpdate-glfwGetTime();
			if( delay>0 ) break;
		}
		
		RenderGame();
		if( !_updateRate ) continue;
		
		if( updates==4 ) _nextUpdate=glfwGetTime();
	}
}

