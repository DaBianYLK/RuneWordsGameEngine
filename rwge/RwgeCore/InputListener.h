#pragma once

class InputListener
{
public:
	InputListener();
	virtual ~InputListener();

	virtual void OnKeyUp(unsigned int key) = 0;
	virtual void OnKeyDown(unsigned int key) = 0;
};

