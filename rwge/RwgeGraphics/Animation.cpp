#include "Animation.h"

static const float fAnimationFrameInterval = 2.0f / 60.0f;

Animation::Animation() : 
	m_nStartFrame(0), 
	m_nFrameNum(0),
	m_fPlaySpeed(1.0f),
	m_fCurrentTime(0.0f), 
	m_nCurrentFrame(0),
	m_bIsPlaying(false),
	m_bLoop(false)
{

}

Animation::Animation(int nStartFrame, int nFrameNum) :
	m_nStartFrame(nStartFrame),
	m_nFrameNum(nFrameNum),
	m_fPlaySpeed(1.0f),
	m_fCurrentTime(0.0f),
	m_nCurrentFrame(0),
	m_bIsPlaying(false),
	m_bLoop(false)
{

}

Animation::~Animation()
{

}

void Animation::Set(int startFrame, int frameNum)
{
	m_nStartFrame = startFrame;
	m_nFrameNum = frameNum;
}

void Animation::Play(bool loop)
{
	m_bLoop = loop;
	m_fCurrentTime = 0.0f;
	m_bIsPlaying = true;
}

void Animation::SetPlaySpeed(float playSpeed)
{
	m_fPlaySpeed = playSpeed;
}

void Animation::Update(float deltaTime)
{
	m_fCurrentTime += deltaTime * m_fPlaySpeed;
	int deltaFrame = static_cast<int>(m_fCurrentTime / fAnimationFrameInterval);
	if (deltaFrame >= m_nFrameNum)
	{
		if (m_bLoop)
		{
			deltaFrame = 0;
			m_fCurrentTime = 0.0f;
		}
		else
		{
			deltaFrame = m_nFrameNum - 1;
			m_bIsPlaying = false;
		}
	}

	m_nCurrentFrame = m_nStartFrame + deltaFrame;
}

int Animation::GetFrameIndex()
{
	return m_nCurrentFrame;
}

bool Animation::IsPlaying()
{
	return m_bIsPlaying;
}