/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _QUAD_SHAPE_H
#define _QUAD_SHAPE_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "Vertex\VertexBuffer.h"
#include "Vertex\Types\VertexPositionTexture.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class IndexBuffer;
class GraphicDevice;
class IVertexBuffer;

/*****************
*   Class Decl   *
******************/
class QuadShape abstract : public AEObject
{
	protected:
		//Variables
		bool m_IsReady = false;

		bool m_ClockWise = false;

		VertexPositionTexture m_Vtx[4];

		VertexBuffer<VertexPositionTexture>* m_VB = nullptr;

		IndexBuffer* m_IB = nullptr;

		GraphicDevice* m_GraphicDevice = nullptr;

	public:
		//Constructor Destructor.
		QuadShape(GraphicDevice* graphicDevice, bool clockWise = true);
		virtual ~QuadShape();

		//Get Methods
		inline IVertexBuffer*	GetVertexBuffer		()									{ return m_VB; }
		inline IndexBuffer*		GetIndexBuffer		()									{ return m_IB; }

		//Framework Methods
		virtual AEResult BuildQuad() = 0;

		AEResult			DrawQuad			();
};

#endif

