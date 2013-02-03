//////////////////////////////////////////////////////////////////////////
// Name:	DirectXFramework.cpp
//////////////////////////////////////////////////////////////////////////
#define DIMOUSE_LEFTBUTTON 0
#define DIMOUSE_RIGHTBUTTON 1

#include "DirectXFramework.h"
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <DxErr.h>
#include "ResourceManager.h"


cResourceManager* resourceManager = new cResourceManager();


CDirectXFramework::CDirectXFramework(void)
{
	// Init or NULL objects before use to avoid any undefined behavior
	m_bVsync		= false;
	m_pD3DObject	= 0;
	m_pD3DDevice	= 0;
	
}

CDirectXFramework::~CDirectXFramework(void)
{
	// If Shutdown is not explicitly called correctly, call it when 
	// this class is destroyed or falls out of scope as an error check.
	Shutdown();
}

void CDirectXFramework::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
{
	m_hWnd = hWnd;
	HRESULT hr;
	TCHAR *ErrorMessage;

	

	//////////////////////////////////////////////////////////////////////////
	// Direct3D Foundations - D3D Object, Present Parameters, and D3D Device
	//////////////////////////////////////////////////////////////////////////

	// Create the D3D Object
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	// Find the width and height of window using hWnd and GetWindowRect()
	RECT rect;
	GetWindowRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// Set D3D Device presentation parameters before creating the device
	D3DPRESENT_PARAMETERS D3Dpp;
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));  // NULL the structure's memory

	D3Dpp.hDeviceWindow					= hWnd;										// Handle to the focus window
	D3Dpp.Windowed						= bWindowed;								// Windowed or Full-screen boolean
	D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth/stencil buffer, 24 bit depth, 8 bit stencil
	D3Dpp.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer (Depth Buffer)
	D3Dpp.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
	D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;							// Back-buffer format, 8 bits for each pixel
	D3Dpp.BackBufferHeight				= height;									// Make sure resolution is supported, use adapter modes
	D3Dpp.BackBufferWidth				= width;									// (Same as above)
	D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer, must stay discard to support multi-sample
	D3Dpp.PresentationInterval			= m_bVsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE; // Present back-buffer immediately, unless V-Sync is on								
	D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
	D3Dpp.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;	// Full-screen refresh rate, use adapter modes or default
	D3Dpp.MultiSampleQuality			= 0;										// MSAA currently off, check documentation for support.
	D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;						// MSAA currently off, check documentation for support.

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);

	// Determine vertex processing mode
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		// Hardware vertex processing supported? (Video Card)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	}
	else
	{
		// If not, use software (CPU)
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	}
	
	// If hardware vertex processing is on, check pure device support
	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}
	
	// Create the D3D Device with the present parameters and device flags above
	m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
		D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
		hWnd,					// handle to the focus window
		deviceBehaviorFlags,	// behavior flags
		&D3Dpp,					// presentation parameters
		&m_pD3DDevice);			// returned device pointer

	//*************************************************************************
	//////////////////////////////////////////////////////////////////////////
	// Start resource manager
	//////////////////////////////////////////////////////////////////////////
	
	resourceManager->loadFromXMLFile("Resources.xml");
	resourceManager->setCurrentScope(0);

	//*************************************************************************

	//////////////////////////////////////////////////////////////////////////
	// Create a Font Object
	//////////////////////////////////////////////////////////////////////////
	
	// Load a font for private use for this process
	// Load external font
	AddFontResourceEx(L"Delicious-Roman.otf",FR_PRIVATE,0);

	// Load D3DXFont, each font style you want to support will need an ID3DXFont
	D3DXFONT_DESC fontFace;
	fontFace.Height			= 24;
	fontFace.Width			= 0;
	fontFace.Weight			= 0;
	fontFace.MipLevels		= 1;
	fontFace.Italic			= false;
	fontFace.CharSet		= DEFAULT_CHARSET;
	fontFace.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontFace.Quality		= DEFAULT_QUALITY;
	fontFace.PitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy(fontFace.FaceName, _T("Delicious-Roman"));

	D3DXCreateFontIndirect(m_pD3DDevice, &fontFace, &m_pD3DFont);


	//////////////////////////////////////////////////////////////////////////
	// Create Sprite Object and Textures
	//////////////////////////////////////////////////////////////////////////

	// Create a sprite object, note you will only need one for all 2D sprites
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	// Load texture for sprite
	// D3DXCreateTextureFromFile(m_pD3DDevice, L"BattleTankAnimation.png", &m_pTexture);


	// Setup for keyboard and mouse input
	// Clear memory for the following two variables
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));

	DirectInput8Create(hInst , DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&mDInput, 0);
	
	// Set up the keyboard
	mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0);
	mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	// mKeyboard->SetCooperativeLevel(g_hInstance->getMainWnd , keyboardCoopFlags);

	// Set up the mouse
	mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0);
	mMouse->SetDataFormat(&c_dfDIMouse2);
	// mMouse->SetCooperativeLevel(g_hInstance->getMainWnd, mouseCoopFlags);

	// Create Sprite
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	D3DXCreateTextureFromFile(m_pD3DDevice, L"TeamATitle.png",&m_pTexture);

}


void CDirectXFramework::Update(float dt)
{
	
	// ************* Display frame count on screen ********************
	// Make static so their values persist across function calls
	static float numFrames = 0.0f;
	static float timeElapsed = 0.0f;

	// Increment the frame count
	numFrames += 1.0f;

	// Accumlate time passed
	timeElapsed += dt;

	//Compute frame stats once per second
	if(timeElapsed >= 1.0f)
	{
		mFPS = numFrames;

		timeElapsed = 0.0f;
		numFrames = 0.0f;
	}
	// ************* End of frame count on screen *********************

	// Check for keyboard activity
	mKeyboard->Acquire();

	HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeyboardState),
		(void**)&mKeyboardState);
	if( FAILED(hr))
	{
		// keyboard lost, zero out structure
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		// Try to reaquire for next time
		hr = mKeyboard->Acquire();
	}

	// if ((mKeyboardState[DIK_M] & 0x80)!=0) gameMode = 'm'; // MainMenu mode


	// Check for mouse activity
	mMouse->Acquire();

	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState);
	if( FAILED(hr))
	{
		// Mouse lost, zero out structure
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		// Try to reaquire for next time
		hr = mMouse->Acquire();
	}

}

void CDirectXFramework::Render()
{
	// If the device was not created successfully, return
	if(!m_pD3DDevice)
		return;
	//*************************************************************************


	//////////////////////////////////////////////////////////////////////////
	// All draw calls between swap chain's functions, and pre-render and post- 
	// render functions (Clear and Present, BeginScene and EndScene)
	//////////////////////////////////////////////////////////////////////////

	// Clear the back buffer, call BeginScene()
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET,D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f),1.0f,0);

	m_pD3DDevice->BeginScene();
	m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pD3DSprite->Draw(m_pTexture,NULL,NULL,&D3DXVECTOR3(0.0f,0.0f,0.0f),0xFFFFFFFF);


	m_pD3DSprite->Flush();

	// Make static so memory not allocated each call
	static char buffer[30];

	sprintf(buffer, "Frames Per Second = %3.1f", mFPS);
	// sprintf(distanceBuffer,"%3.1f",distance);
	RECT R = {15, 15, 5, 5};

	

	m_pD3DFont->DrawTextA(0, buffer, -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(255,255,255));

	m_pD3DSprite->End();
	m_pD3DDevice->EndScene();

	//Present back buffer
	m_pD3DDevice->Present(0,0,0,0);
}

void CDirectXFramework::Shutdown()
{
	//*************************************************************************
	// Release COM objects in the opposite order they were created in

	// Font

	// 3DDevice
	m_pD3DDevice->Release();
		
	//Keyboard and mouse
	mDInput->Release();
	mKeyboard->Unacquire();
	mKeyboard->Release();
	mMouse->Unacquire();
	mMouse->Release();

}
