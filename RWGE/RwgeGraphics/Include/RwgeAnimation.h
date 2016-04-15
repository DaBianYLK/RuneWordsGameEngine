#pragma once

/*
这个类用于控制动画帧数
*/

class Animation
{
public:
	Animation();
	Animation(int nStartFrame, int nFrameNum);
	~Animation();

	void Set(int nStartFrame, int nFrameNum);
	void Play(bool bLoop = false);
	void SetPlaySpeed(float fPlaySpeed);

	void Update(float fDeltaTime);

	int GetFrameIndex();
	bool IsPlaying();

private:
	int m_nStartFrame;
	int m_nFrameNum;

	float m_fPlaySpeed;
	bool m_bLoop;
	bool m_bIsPlaying;

	float m_fCurrentTime;
	int m_nCurrentFrame;
};

