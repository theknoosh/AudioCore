//////////////////////////////////////////////////////////////////////////
// Name:	DirectXFramework.h
//////////////////////////////////////////////////////////////////////////
#pragma once
#pragma comment(lib, "winmm.lib")
//////////////////////////////////////////////////////////////////////////
// Direct3D 9 headers and libraries required
//////////////////////////////////////////////////////////////////////////
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}
#define SOUND_SAFE_RELEASE(x) if(x) {x->release(); x = 0;}
#define ERRCHECK(x) if(x != FMOD_OK) {MessageBox(m_hWnd, (LPCWSTR)x, L"Fmod Error", MB_OK);}

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//////////////////////////////////////////////////////////////////////////
// DirectInput headers and libraries
//////////////////////////////////////////////////////////////////////////
#define DIRECTINPUT_VERSION 0x0800

#include "fmod.hpp"
#include "AudioManager.h"


struct SpriteInfo
{
	D3DXVECTOR3 pos;
	float rotation;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 center;
	D3DXCOLOR color;
	int width;
	int height;
	int curframe;
	int lastframe;
	int animdelay;
	int animcount;
	int movex;
	int movey;
};

class CDirectXFramework
{
	//////////////////////////////////////////////////////////////////////////
	// Application Variables
	//////////////////////////////////////////////////////////////////////////
	HWND				m_hWnd;			// Handle to the window
	bool				m_bVsync;		// Boolean for vertical syncing

	//////////////////////////////////////////////////////////////////////////
	// Direct3D Variables
	//////////////////////////////////////////////////////////////////////////
	IDirect3D9*			m_pD3DObject;	// Direct3D 9 Object
	IDirect3DDevice9*	m_pD3DDevice;	// Direct3D 9 Device
	D3DCAPS9			m_D3DCaps;		// Device Capabilities

	//////////////////////////////////////////////////////////////////////////
	// Font Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXFont*			m_pD3DFont;		// Font Object
	float mFPS;	// Frames per Second

	//////////////////////////////////////////////////////////////////////////
	// Sprite Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXSprite*		m_pD3DSprite;	// Sprite Object
	IDirect3DTexture9*	m_pTexture;		// Sprite Texture

	D3DXIMAGE_INFO		m_imageInfo;	// File details of a texture

	// Create an array of sprite specifications
	SpriteInfo tankInfo;

	// Variables for keyboard and mouse input
	IDirectInput8*			mDInput;

	IDirectInputDevice8*	mKeyboard;
	char					mKeyboardState[256];

	IDirectInputDevice8*	mMouse;
	DIMOUSESTATE2			mMouseState;
	int						oldMousePX;
	int						oldMousePY;

public:
	//////////////////////////////////////////////////////////////////////////
	// Init and Shutdown are preferred to constructors and destructor,
	// due to having more control when to explicitly call them when global.
	//////////////////////////////////////////////////////////////////////////
	CDirectXFramework(void);
	~CDirectXFramework(void);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Init
	// Parameters:	HWND hWnd - Handle to the window for the application
	//				HINSTANCE hInst - Handle to the application instance
	//				bool bWindowed - Boolean to control windowed or full-screen
	// Return:		void
	// Description:	Ran once at the start.  Initialize DirectX components and 
	//				variables to control the application.  
	//////////////////////////////////////////////////////////////////////////
	void Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Update
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					update call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Used 
	//				for updating variables and processing input commands prior
	//				to calling render.
	//////////////////////////////////////////////////////////////////////////
	void Update(float dt);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Render
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					render call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Render
	//				calls all draw call to render objects to the screen.
	//////////////////////////////////////////////////////////////////////////
	void Render();


	//////////////////////////////////////////////////////////////////////////
	// Name:		Shutdown
	// Parameters:	void
	// Return:		void
	// Description:	Runs once at the end of an application.  Destroy COM 
	//				objects and deallocate dynamic memory.
	//////////////////////////////////////////////////////////////////////////
	void Shutdown();

	
};
