#pragma once

#ifndef _Simulation_
#define _Simulation_

#include "pch.h"
#include "StepTimer.h"
#include "SimulationData.h"
#include <dinput.h> 
#include "Effects.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "Cube.h"
#include "Shape.h"
#include "Sphere.h"
#include "Light.h"

using namespace std;
using namespace DirectX;

class Object;
class Camera;
class Light;

struct DrawData;
struct SimulationData;

class Simulation
{
public:

	Simulation();
	~Simulation();

	// Initialization and management
	void Initialize(HWND window);

	// Basic Simulation loop
	void Tick();
	void Render();

	// Rendering helpers
	void Clear();
	void Present();

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged();

	// Properties
	void GetDefaultSize(size_t& width, size_t& height) const;
	Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;


	void InitialiseInput(HINSTANCE _Instance);

	void CalculateFPS();
	float i;
protected:
	bool ReadKeyboard();
	bool ReadMouse();

	IDirectInput8*			m_pDirectInput;
	IDirectInputDevice8*	m_pKeyboard;
	unsigned char			m_KeyboardState[256];
	unsigned char			m_PreviousKeyboardState[256];
	IDirectInputDevice8*	m_pMouse;
	DIMOUSESTATE			m_MouseState;
	Camera*					m_Camera;
	VertexBuffer*			m_VertexBuffer;
	Cube*					m_Cube;
	Light*					m_Light;

private:

	void Update(DX::StepTimer const& timer);

	void CreateDevice();
	void CreateResources();
	void CreateObjects();

	void OnDeviceLost();

	// Application state
	HWND                                            m_window;

	// Direct3D Objects
	D3D_FEATURE_LEVEL                               m_featureLevel;
	
	Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;
	
	// Rendering resources
	Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_depthStencil;

	// Simulation state
	DX::StepTimer                                   m_timer;

	DrawData*										m_DD;


//	TwBar*											m_Bar;
	SimulationData*									m_SD;
	EffectFactory*									m_EffectFactory;
	CommonStates*									m_States;
};
#endif