
/********************************************************
*
* Author: Carlos Chac�n N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

#pragma once

#ifndef _XENGINE_EDITOR_H
#define _XENGINE_EDITOR_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "QtWidgets\qmainwindow.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "ui_xengine_editor.h"

/********************
*   Forward Decls   *
*********************/
class GameAsset;
class GameObject;
class ModelAsset;
class ShaderAsset;
class RawGameAsset;
class TextureAsset;
class XEngineViewerWidget;
class CodeEditorMainWindow;
class ProjectSelectionWidget;

/*****************
*   Class Decl   *
******************/
class XEngine_Editor sealed : public QMainWindow
{
	Q_OBJECT

	private:

		Ui::XEngine_EditorClass m_EditorUI;

		XEngineViewerWidget* m_XEngineViewerWidget = nullptr;

		CodeEditorMainWindow* m_CodeEditorMainWindow = nullptr;

		ProjectSelectionWidget* m_ProjectSelectionWidget = nullptr;

		void Initialize();

		void SetObjTypeUserData();

		void closeEvent(QCloseEvent* cEvent) override;

		void ShowProjectSelectionWidget();

		void LoadProject(const std::wstring& projectConfig);

	private slots:

		void on_m_SaveLayoutAction_triggered();

		void on_m_LoadLayoutAction_triggered();

		void on_m_NewRawGameAssetAction_triggered();

		void on_m_ReloadOutdatedRawGameAssetsAction_triggered();

		void on_m_CreateEmptyGameObjectAction_triggered();

		void on_m_AddGOComponentButton_clicked();

		void on_m_RefreshAllViewsAction_triggered();

		/******************
		*   Project Bar   *
		*******************/

		void on_m_MenuFileSaveProjectAction_triggered();

		void on_m_MenuFileCreateProjectAction_triggered();

		/******************
		*   Physics Bar   *
		*******************/

		void on_m_StartPhysXDebuggerAction_triggered();

		void on_m_StopPhysXDebuggerAction_triggered();

		/***************
		*   Tool bar   *
		****************/
		void on_m_PlayAction_triggered();

		void on_m_PauseAction_triggered();

		void on_m_StopAction_triggered();

		/***********************
		*   Game Object Tree   *
		************************/
		void GameObjectChangedEvent(GameObject* gameObject);

		/*****************************
		*   Game Object Props Tree   *
		******************************/
		void GameObjectChangedNameEvent(uint64_t gameObjectID);

		/**********************************
		*   Raw Game Assets Tree Widget   *
		***********************************/
		void NewDropRawAssetsFilesEvent(QStringList fileList);

		void EditCodeFileEvent(QString filepath);

		void RawGameAssetReloadEvent(uint64_t rawGAID);

	public:

		XEngine_Editor(QWidget *parent = nullptr);
		~XEngine_Editor();

		void SaveLayout(const QString& fileName);

		void LoadLayout(const QString& fileName);
};

#endif // XENGINE_EDITOR_H
