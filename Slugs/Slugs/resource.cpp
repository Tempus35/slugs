#include "resource.h"

/*
	class Resource
*/

Resource::Resource(ResourceType resourceType)
{

	type = resourceType;

}

Resource::~Resource()
{

}

const ResourceType Resource::GetType() const
{

	return type;

}