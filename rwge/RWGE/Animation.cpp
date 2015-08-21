#include "Animation.h"

#include "AppConfig.h"

Animation::Animation() {
	m_PlaySpeed = 1.0f;
	m_CurrentTime = 0.0f;
	m_IsPlaying = false;
	m_Loop = false;
}

Animation::Animation(int startFrame, int frameNum) {
	m_StartFrame = startFrame;
	m_FrameNum = frameNum;
	m_PlaySpeed = 1.0f;
	m_CurrentTime = 0.0f;
	m_IsPlaying = false;
	m_Loop = false;
}

Animation::~Animation() {

}

void Animation::Set(int startFrame, int frameNum) {
	m_StartFrame = startFrame;
	m_FrameNum = frameNum;
}

void Animation::Play(bool loop) {
	m_Loop = loop;
	m_CurrentTime = 0.0f;
	m_IsPlaying = true;
}

void Animation::SetPlaySpeed(float playSpeed) {
	m_PlaySpeed = playSpeed;
}

void Animation::Update(float deltaTime) {
	m_CurrentTime += deltaTime * m_PlaySpeed;
	int deltaFrame = m_CurrentTime / AppConfig::animationFrameInterval;
	if (deltaFrame >= m_FrameNum) {
		if (m_Loop) {
			deltaFrame = 0;
			m_CurrentTime = 0.0f;
		}
		else {
			deltaFrame = m_FrameNum - 1;
			m_IsPlaying = false;
		}
	}

	m_CurrentFrame = m_StartFrame + deltaFrame;
}

int Animation::GetFrameIndex() {
	return m_CurrentFrame;
}

bool Animation::IsPlaying() {
	return m_IsPlaying;
}