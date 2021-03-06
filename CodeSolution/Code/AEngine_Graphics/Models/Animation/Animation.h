/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _ANIMATION_H
#define _ANIMATION_H

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <string>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "KeyFrame.h"
#include "Base\Base.h"
#include "AnimationDefs.h"
#include "Resource\GameResource.h"

/********************
*   Forward Decls   *
*********************/
struct TimerParams;

/*****************
*   Class Decl   *
******************/
class Animation sealed : public GameResource
{
	private:

		std::vector<AnimationCallBack> m_AnimationCallbacks;

		std::vector<KeyFrame> m_KeyFrames;

		uint32_t m_LastKeyFrameCheck = 0;

		float m_Duration = 0.0f;

		bool m_NeedSort = false;
		
		void CleanUp();
		
		void SortKeyFrames();

		virtual ~Animation();

	public:

		/// <summary>
		/// Animation Constructor
		/// </summary>
		/// <param name="resourceName">Resource Name</param>
		Animation(const std::wstring& resourceName);

		inline float GetDuration() const
		{
			return m_Duration;
		}

		inline uint32_t GetFrameCount() const
		{
			return (uint32_t)m_KeyFrames.size();
		}

		AEResult Load() override;

		AEResult CheckCallBacks(uint32_t currentKeyFrame);

		AEResult AddAnimationCallBack(float time, AnimationCallBackFunc animCallBack);

		AEResult AddKeyFrame(const KeyFrame& keyFrame);
		
		void Update(const TimerParams& timerParams); 

		const KeyFrame& GetKeyFrame(uint32_t index);

		const KeyFrame& operator[](uint32_t index);

		// Prevent copying.
		Animation(Animation const&) = delete;
		Animation& operator= (Animation const&) = delete;
};

#endif

