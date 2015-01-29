
/********************************************************
*
* Author: Carlos Chac�n N.
*
* Created: 7/6/2014
*
* Desc:
*
*
* Copyright 2014
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_SCRIPT_PROPERTY_TREE_WIDGET_H
#define _GAME_OBJECT_SCRIPT_PROPERTY_TREE_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qspinbox.h"
#include "qcheckbox.h"
#include "qtreewidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"

/********************
*   Forward Decls   *
*********************/
struct GameObjectScriptProperty;
struct GameObjectScriptProperties;

/*****************
*   Class Decl   *
******************/
class GameObjectScriptPropertyWidget sealed : public QTreeWidget
{
	Q_OBJECT

	private:

		bool m_IsReady = false;

		XEResult FillPropertiesRows(GameObjectScriptProperties* gameObjectScriptProperties);

		XEResult AddScalarWidget(QTreeWidgetItem* item, GameObjectScriptProperty* gosProp);

		QSpinBox* CreateSpinBox(GameObjectScriptProperty* gosProp);

		QDoubleSpinBox* CreateDoubleSpinBox(GameObjectScriptProperty* gosProp);

		QCheckBox* CreateCheckBox(GameObjectScriptProperty* gosProp);

	private slots:

		void SpinBoxChanged();

		void CheckBoxChanged(int newState);

	public:
		GameObjectScriptPropertyWidget(QWidget *parent = nullptr);
		~GameObjectScriptPropertyWidget();

		XEResult Initialize();

		XEResult Populate(GameObjectScriptProperties* gameObjectScriptProperties);
};

#endif
