#include "InvalidEngineStateException.h"

const char* InvalidEngineStateException::what() const throw()
{
	return "Engine is in invalid state for requested operation.";
}
