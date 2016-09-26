#pragma once
#include <string>

class UnsopportedEntityException : public std::exception
{
public:
	explicit UnsopportedEntityException(std::string entityType);
	const char* what() const throw() override;
	~UnsopportedEntityException();
private:
	std::string entityType;
	char* errMessage;
};