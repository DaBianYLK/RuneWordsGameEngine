#pragma once

class RInputListener
{
public:
	RInputListener();
	virtual ~RInputListener();

	virtual void OnKeyUp(unsigned int key) = 0;
	virtual void OnKeyDown(unsigned int key) = 0;
};

