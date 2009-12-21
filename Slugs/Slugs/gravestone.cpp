//---------------------------------------------------------------
//
// Slugs
// gravestone.cpp
//
//---------------------------------------------------------------

#include "gravestone.h"

Gravestone::Gravestone(ImageResource* imageResource) : Object(ObjectType_Gravestone)
{

	// Gravestones are invulnerable
	invulnerable = true;

	// Set the image
	SetImage(imageResource);

}