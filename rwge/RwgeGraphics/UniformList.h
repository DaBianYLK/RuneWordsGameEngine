#pragma once

#include <string>
#include <list>

class UniformNode
{
public:
	UniformNode();
	~UniformNode();

	virtual void* GetValuePtr() const;
	void GetSize() const;

protected:
	std::string m_strName;
	unsigned	m_uSize;		// ×Ö½ÚÊý
};

template<typename UniformType>
class Uniform : public UniformNode
{
public:
	Uniform();
	~Uniform();


	void* GetValuePtr() const;

protected:
	UniformType m_Value;
};

typedef std::list<UniformNode> UniformList;