#include "UnsopportedEntityException.h"
#include <string>
#include <sstream>

UnsopportedEntityException::UnsopportedEntityException(std::string entityType) : entityType(entityType)
{
	std::string str = "Entity type " + entityType + " is not supported";
	errMessage = new char[str.length() + 1];
	strcpy(errMessage, str.c_str());
}

const char* UnsopportedEntityException::what() const throw()
{
	return errMessage;
}

UnsopportedEntityException::~UnsopportedEntityException()
{
	delete errMessage;
}
