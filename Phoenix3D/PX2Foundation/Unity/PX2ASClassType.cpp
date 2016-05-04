// PX2ASClassType.hpp

#include "PX2ASClassType.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ASClassType::ASClassType() :
ClassType(0),
FactoryFunc(0),
OnStartFunc(0),
OnUpdateFunc(0)
{
}
//----------------------------------------------------------------------------
ASClassType::~ASClassType()
{
}
//----------------------------------------------------------------------------