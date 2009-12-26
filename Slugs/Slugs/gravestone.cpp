//---------------------------------------------------------------
//
// Slugs
// gravestone.cpp
//
//---------------------------------------------------------------

#include "gravestone.h"

Gravestone::Gravestone(ImageResource* imageResource) : Object(ObjectType_Flavor)
{

	// Gravestones are invulnerable
	invulnerable = true;

	// Set the image
	SetImage(imageResource);

}