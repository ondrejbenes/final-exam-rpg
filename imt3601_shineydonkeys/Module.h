#pragma once
class Module
{

public:
	Module();
	virtual ~Module();

	void initialize();
	virtual void update();

};