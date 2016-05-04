// PX2ReferencesObject.cpp

#include "PX2ReferencesObject.hpp"
#include "PX2Memory.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
RefObject::RefObject() :
mReferences(0)
{
}
//----------------------------------------------------------------------------
RefObject::~RefObject()
{
	assert(mReferences == 0);
}
//----------------------------------------------------------------------------
void RefObject::DecrementReferences()
{
	if (--mReferences == 0)
	{
		delete0((RefObject*)this);
	}
}
//----------------------------------------------------------------------------