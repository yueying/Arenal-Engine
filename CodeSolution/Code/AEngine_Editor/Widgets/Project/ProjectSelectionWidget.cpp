/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\AEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "ProjectSelectionWidget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ProjectSelectionWidget::ProjectSelectionWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ProjectSelectionWidgetQtUI.setupUi(this);

	//InitFields();
}

ProjectSelectionWidget::~ProjectSelectionWidget()
{
}