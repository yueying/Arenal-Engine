/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _CODE_EDITOR_TEXT_EDIT_H
#define _CODE_EDITOR_TEXT_EDIT_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "QtWidgets\qplaintextedit.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Time\AETimeDefs.h"

/********************
*   Forward Decls   *
*********************/
class CodeSyntaxHighlither;

/*****************
*   Class Decl   *
******************/
class CodeEditorTextEdit sealed : public QPlainTextEdit
{
	Q_OBJECT

	private:

		QWidget* m_LineNumberArea;

		CodeSyntaxHighlither* m_CodeSyntaxHighlither;

		QString m_Filename;

		TimeStamp m_LastModifiedTimeStamp;

		QByteArray m_ModifiedHash;

		bool m_IsReady;

		void OpenCodeFile();

		bool CheckIfModifiedOutside();

		void resizeEvent(QResizeEvent* rEvent) override;

		void focusInEvent(QFocusEvent* focusEvent) override;

	private slots:

		void UpdateLineNumberAreaWidth(int newBlockCount);

		void UpdateLineNumberArea(const QRect& rectArea, int difference);

	public:
		CodeEditorTextEdit(const QString& filename, QWidget* parent = nullptr);
		~CodeEditorTextEdit();

		inline bool IsReady() const
		{
			return m_IsReady;
		}

		bool WasModified() const;

		inline const QString& GetFilename() const
		{
			return m_Filename;
		}

		int32_t GetLineNumberAreaWidth();

		void LineNumberAreaPaintEvent(QPaintEvent* pEvent);

		AEResult ReloadFile();

		AEResult SaveFile();
};

#endif // AEngine_EDITOR_H
