/********************************************************
*
* Author: Carlos Chac�n N.
*
* Created: 4/26/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/


/**********************
*   System Includes   *
***********************/
#include <assert.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "DeferredRendering.h"
#include "Textures\RenderTarget.h"
#include "Utils\XETimeDefs.h"
#include "GameApp\GameApp.h"
#include "GraphicDevice.h"
#include "Materials\DeferredRenderingMaterial.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DeferredRendering::DeferredRendering(GameApp* gameApp, const DeferredRenderingSettings& settings, uint32_t callOrder, const std::string serviceName, const std::string& gameComponentName)
	: DrawableGameComponent(gameApp, gameComponentName, callOrder)
	, m_ServiceName(serviceName)
	, m_Settings(settings)
	, m_DiffuseRT(nullptr)
	, m_NormalRT(nullptr)
	, m_DepthRT(nullptr)
	, m_DREffect(nullptr)
{
	//Register this Game Component as a service so others can use it
	m_GameApp->RegisterGameService(m_ServiceName, this);
}

DeferredRendering::~DeferredRendering()
{
	DeleteMem(m_DREffect);
	DeleteMem(m_DiffuseRT);
	DeleteMem(m_NormalRT);
	DeleteMem(m_DepthRT);
}

void DeferredRendering::Initialize()
{
	//Create Render Targets
	m_DiffuseRT = RenderTarget::CreateRenderTargetInst();
	m_NormalRT = RenderTarget::CreateRenderTargetInst();
	m_DepthRT = RenderTarget::CreateRenderTargetInst();

	//Initialize Render Targets
	uint32_t width = m_GraphicDevice->GetGraphicPP().BackBufferWidth;
	uint32_t height = m_GraphicDevice->GetGraphicPP().BackBufferHeight;

	m_DiffuseRT->CreateRenderTargetTexture(m_GraphicDevice, "Diffuse RT", width, height, XE_POOL_DEFAULT, m_Settings.m_DiffusePixFmt);
	m_NormalRT->CreateRenderTargetTexture(m_GraphicDevice, "Normal RT", width, height, XE_POOL_DEFAULT, m_Settings.m_NormalPixFmt);
	m_DepthRT->CreateRenderTargetTexture(m_GraphicDevice, "Depth RT", width, height, XE_POOL_DEFAULT, m_Settings.m_DepthPixFmt);

	//Create Effect
	m_DREffect = new DeferredRenderingMaterial();

	DrawableGameComponent::Initialize();
}

void DeferredRendering::LoadContent()
{
	//Load Effect
	m_DREffect->LoadMaterial(m_GraphicDevice);
	InitEffectParams();

	DrawableGameComponent::LoadContent();
}

void DeferredRendering::Update(const TimerParams& timerParams)
{
	DrawableGameComponent::Update(timerParams);
}

void DeferredRendering::Render(const TimerParams& timerParams)
{
	XE_Base::BeginUserEventPerf(L"Deferred Rendering");

	//Clean The Render Targets
	m_GraphicDevice->ColorFill(m_DiffuseRT, XMCOLORCornflowerBlue);
	m_GraphicDevice->ColorFill(m_NormalRT, XMCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	m_GraphicDevice->ColorFill(m_DepthRT, XMCOLOR(1));

	//Set Render Targets
	m_GraphicDevice->SetRenderTarget(0, m_DiffuseRT);
	m_GraphicDevice->SetRenderTarget(1, m_NormalRT);
	m_GraphicDevice->SetRenderTarget(2, m_DepthRT);
	
	XE_Base::EndUserEventPerf();

	DrawableGameComponent::Render(timerParams);
}

XEResult DeferredRendering::InitEffectParams()
{
	if(m_DREffect->SetHalfPixel(m_GraphicDevice->GetHalfPixel()) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

void DeferredRendering::OnLostDevice()
{	
	m_DiffuseRT->OnLostDevice();
	m_NormalRT->OnLostDevice();
	m_DepthRT->OnLostDevice();
	m_DREffect->OnLostDevice();

	DrawableGameComponent::OnLostDevice();
}

void DeferredRendering::OnResetDevice()
{
	m_DiffuseRT->OnResetDevice();
	m_NormalRT->OnResetDevice();
	m_DepthRT->OnResetDevice();

	m_DREffect->OnResetDevice();
	m_DREffect->SetHalfPixel(m_GraphicDevice->GetHalfPixel());

	DrawableGameComponent::OnResetDevice();
}