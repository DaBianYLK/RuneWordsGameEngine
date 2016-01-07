#pragma once

class Animation {
public:
	Animation();
	Animation(int startFrame, int frameNum);
	~Animation();

	void Set(int startFrame, int frameNum);
	void Play(bool loop = false);
	void SetPlaySpeed(float playSpeed);

	void Update(float deltaTime);

	int GetFrameIndex();
	bool IsPlaying();

private:
	int m_StartFrame;
	int m_FrameNum;

	float m_PlaySpeed;
	bool m_Loop;
	bool m_IsPlaying;

	float m_CurrentTime;
	int m_CurrentFrame;
};

