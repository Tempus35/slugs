#pragma once

/*
	Enumeration of resource types
*/

enum ResourceType
{

	ResourceType_Image,
	ResourceType_Sound,
	ResourceType_Font,
	ResourceType_Text,
	ResourceType_TextureBuffer,

};

/*
	class Resource
	Base class for resources
*/

class Resource
{

protected:

	ResourceType type;

public:

	// Constructor
	Resource(ResourceType resourceType);

	// Destructor
	virtual ~Resource();

	// Gets the type of the resource
	const ResourceType GetType() const;

};


