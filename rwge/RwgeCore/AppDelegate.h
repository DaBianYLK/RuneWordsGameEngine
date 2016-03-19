#pragma once

class AppDelegate
{
public:
	AppDelegate();
	virtual ~AppDelegate();

	virtual void Initialize() = 0;
	virtual void Update(float fDeltaTime) = 0;
	virtual void Cleanup() = 0;
};

