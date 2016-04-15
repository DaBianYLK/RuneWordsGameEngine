#pragma once

class RAppDelegate
{
public:
	RAppDelegate();
	virtual ~RAppDelegate();

	virtual void Initialize() = 0;
	virtual void Update(float fDeltaTime) = 0;
	virtual void Cleanup() = 0;
};

