// PX2ObjCast.cpp

#include "PX2ObjCast.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Movable *Cast::ToMovable(Object *obj)
{
	return DynamicCast<Movable>(obj);
}
//----------------------------------------------------------------------------
Node *Cast::ToNode(Object *obj)
{
	return DynamicCast<Node>(obj);
}
//----------------------------------------------------------------------------