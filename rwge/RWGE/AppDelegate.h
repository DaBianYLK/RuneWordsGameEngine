#pragma once

class AppDelegate {
public:
	AppDelegate();
	~AppDelegate();

	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Cleanup() = 0;
};

