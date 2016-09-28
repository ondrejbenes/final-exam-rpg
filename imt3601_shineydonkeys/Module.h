#pragma once
class Module
{

public:
	Module();
	virtual ~Module();

	virtual bool initialize() = 0;
	virtual void update();

};