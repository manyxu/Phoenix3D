// PX2ProjectEvent.cpp

#include "PX2ProjectEvent.hpp"
using namespace PX2;

PX2_IMPLEMENT_EVENT(ProjectES);
std::string ProjectES::EventStrs[E_QUANTITY] =
{
	"NewProject",
	"LoadedProject",
	"SavedProject",
	"CloseProject",
	"NewScene",
	"SavedScene",
	"CloseScene",
	"NewUI",
	"CloseUI",
	"NewBP",
	"CloseBP",
	"Play",
	"Stop"
};
