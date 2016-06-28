// PX2ASClassType.hpp

#include "PX2ASClassType.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ASClassType::ASClassType() :
ClassType(0),
FactoryFunc(0),
OnAttachedFunc(0),
OnDetachFunc(0),
OnInitUpdateFunc(0),
OnUpdateFunc(0),
OnFixUpdateFunc(0),
OnResetPlayFunc(0),
OnResetFunc(0),
OnPlayFunc(0),
OnStopFunc(0),
OnPlayUpdateFunc(0)
{
}
//----------------------------------------------------------------------------
ASClassType::~ASClassType()
{
}
//----------------------------------------------------------------------------