// PX2LuaManager.cpp

#include "PX2ASContext.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2Assert.hpp"
#include "PX2Controlledable.hpp"
#include "PX2ASClassType.hpp"
#include "PX2ASScriptbuilder.hpp"
#include "PX2Log.hpp"
#include "PX2ScriptController.hpp"
#include "PX2ASScriptstdstring.hpp"
#include "PX2ASScriptController.hpp"
#include "PX2ASScriptweakref.hpp"
#include "PX2ASScriptArray.hpp"
#include "PX2StringHelp.hpp"
#include "angelscript.h"
using namespace PX2;

//----------------------------------------------------------------------------
template<class A, class B>
B* _RefCast(A* a)
{
	if (!a)
		return 0;
	B* b = dynamic_cast<B*>(a);    if (b != 0)
	{
	}
	return b;
}
//----------------------------------------------------------------------------
ASContext::ASContext() :
ScriptContext(ScriptContext::CT_ANGELSCRIPT),
IsHasCompileErrors(false),
mASEngine(0)
{
	typedef void *(*asALLOCFUNC_t)(size_t);
	typedef void(*asFREEFUNC_t)(void *);

	mASEngine = asCreateScriptEngine();
	if (!mASEngine)
	{
		PX2_LOG_ERROR("Could not create AngelScript engine");
		return;
	}

	mASEngine->SetUserData(this);
	mASEngine->SetEngineProperty(asEP_USE_CHARACTER_LITERALS, (asPWORD)true);
	mASEngine->SetEngineProperty(asEP_ALLOW_UNSAFE_REFERENCES, (asPWORD)true);
	mASEngine->SetEngineProperty(asEP_ALLOW_IMPLICIT_HANDLE_TYPES, (asPWORD)true);
	mASEngine->SetEngineProperty(asEP_BUILD_WITHOUT_LINE_CUES, (asPWORD)true);
	mASEngine->SetMessageCallback(asMETHOD(ASContext, MessageCallback), this, asCALL_THISCALL);

	RegisterStdString(mASEngine);
	RegisterScriptWeakRef(mASEngine);
	RegisterArray(mASEngine);
	RegisterDictionary(mASEngine);

	int r = 0;
	r = mASEngine->RegisterInterface("IScript");
	assert(r >= 0);
}
//----------------------------------------------------------------------------
ASContext::~ASContext()
{
	mASClassTypes.clear();
	mModules.clear();

	for (unsigned i = 0; i < (int)mContextsPool.size(); ++i)
		mContextsPool[i]->Release();

	if (mASEngine)
	{
		mASEngine->Release();
		mASEngine = 0;
	}
}
//----------------------------------------------------------------------------
bool ASContext::CallString(const std::string &str)
{
	return CallBuffer(str.c_str(), str.length(), "");
}
//----------------------------------------------------------------------------
bool ASContext::CallFile(const std::string &filename)
{
	_BuildModule(filename);

	return true;
}
//----------------------------------------------------------------------------
bool ASContext::CallBuffer(const char *buffer, unsigned long size, 
	const std::string &name)
{
	PX2_UNUSED(buffer);
	PX2_UNUSED(size);
	PX2_UNUSED(name);

	return true;
}
//----------------------------------------------------------------------------
bool ASContext::CallFileFunction(const std::string &filename,
	const std::string &funName)
{
	_BuildModule(filename);

	asIScriptFunction *func = mASEngine->GetModule(filename.c_str())
		->GetFunctionByDecl(funName.c_str());
	if (func)
	{
		asIScriptContext *ctx = _PrepareContextFromPool(func);

		_ContextExecute(ctx);
		_ReturnContextToPool(ctx);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void ASContext::SetUserTypePointer(const std::string &luaName,
	const std::string &className, void *ptr)
{
	std::string asStr = className + " @" + luaName;
	int r = mASEngine->RegisterGlobalProperty(asStr.c_str(), &ptr);
	assert(r >= 0);
}
//----------------------------------------------------------------------------
void ASContext::SetUserFunction(const std::string &funName,
	const std::string &returnClassTypeName, ScriptGlobalFun globalFun)
{
	std::string asStr = returnClassTypeName + " @" + funName + "()";
	int r = mASEngine->RegisterGlobalFunction(asStr.c_str(), asFUNCTION(globalFun),
		asCALL_CDECL);
	assert(r >= 0);
}
//----------------------------------------------------------------------------
bool ASContext::CallObjectFuntionValist(const std::string &funName,
	Object *paramObj, const std::string &format, va_list valist)
{
	std::string funStr =  "void " + funName + "(" + "Object @obj";
	const char *pfmt = format.c_str();
	int count = 0;
	while (pfmt[count])
	{
		if (*pfmt == 'i')
		{
			funStr += ", int val";
		}
		else if (*pfmt == 'f')
		{
			funStr += ", float val";
		}
		else if (*pfmt == 's')
		{
			funStr += ", string val";
		}
		else
		{
			assertion(false, "");
		}

		count++;
	}
	funStr += +")";

	count = 0;
	asIScriptFunction *func = 0;
	std::map<std::string, asIScriptModule*>::iterator it = mModules.begin();
	for (; it != mModules.end(); it++)
	{
		asIScriptModule *module = it->second;
		asIScriptFunction *func = module->GetFunctionByDecl(funStr.c_str());

		if (func)
		{
			asIScriptContext *ctx = _PrepareContextFromPool(func);
			ctx->SetArgObject(0, paramObj);

			const char *pfmt = format.c_str();
			while (pfmt[count])
			{
				if (*pfmt == 'i')
				{
					int value = va_arg(valist, int);
					ctx->SetArgDWord(count + 1, value);
				}
				else if (*pfmt == 'f')
				{
					float value = (float)(va_arg(valist, double));
					ctx->SetArgFloat(count + 1, value);
				}
				else if (*pfmt == 's')
				{
					char *str = va_arg(valist, char *);
					ctx->SetArgAddress(count + 1, str);
				}
				else
				{
					assertion(false, "");
				}

				count++;
			}

			_ContextExecute(ctx);
			_ReturnContextToPool(ctx);

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
asIScriptEngine *ASContext::GetASScriptEngine()
{
	return mASEngine;
}
//----------------------------------------------------------------------------
static APoint &AssignAPoint(const APoint &f, APoint &dest)
{
	dest = f;
	return dest;
}
//----------------------------------------------------------------------------
void ASContext::RegistOperators()
{
	int r = 0;
	r = mASEngine->RegisterObjectMethod("APoint", "APoint & opAssign(const APoint & pnt)"
		, asFUNCTION(AssignAPoint), asCALL_CDECL_OBJLAST);
	assert(r >= 0);
}
//----------------------------------------------------------------------------
ScriptController *ASContext::CreateScriptController(
	const std::string &filename, const std::string &className)
{
	ASScriptController *asScriptCtrl = new0 ASScriptController();
	asIScriptObject *asObject = _CreateASScriptObject(filename, className,
		asScriptCtrl);
	asScriptCtrl->ASIScriptObject = asObject;

	return asScriptCtrl;
}
//----------------------------------------------------------------------------
asIScriptObject *ASContext::_CreateASScriptObject(const std::string &filename,
	const std::string &className, ScriptController *ctrl)
{
	int r;
	asIScriptObject *obj = 0;

	ASClassType *classType = _GetASClassType(filename, className);
	if (!classType) return 0;

	// 使用构造函数创建对象
	asIScriptContext *ctx = _PrepareContextFromPool(classType->FactoryFunc);

	// 传递obj指针到构造函数
	ctx->SetArgObject(0, ctrl);

	// Make the call and take care of any errors that may happen
	r = _ContextExecute(ctx);
	if (r == asEXECUTION_FINISHED)
	{
		obj = *((asIScriptObject**)ctx->GetAddressOfReturnValue());
		obj->AddRef();
	}

	_ReturnContextToPool(ctx);

	return obj;
}
//----------------------------------------------------------------------------
void ASContext::CallOnAttached(asIScriptObject *object)
{
	ASClassType *classType = reinterpret_cast<ASClassType*>(object->GetObjectType()
		->GetUserData());

	if (classType->OnAttachedFunc != 0)
	{
		asIScriptContext *ctx = _PrepareContextFromPool(classType->OnAttachedFunc);
		ctx->SetObject(object);
		_ContextExecute(ctx);
		_ReturnContextToPool(ctx);
	}
}
//----------------------------------------------------------------------------
void ASContext::CallOnDetach(asIScriptObject *object)
{
	ASClassType *classType = reinterpret_cast<ASClassType*>(object->GetObjectType()
		->GetUserData());

	if (classType->OnDetachFunc != 0)
	{
		asIScriptContext *ctx = _PrepareContextFromPool(classType->OnDetachFunc);
		ctx->SetObject(object);
		_ContextExecute(ctx);
		_ReturnContextToPool(ctx);
	}
}
//----------------------------------------------------------------------------
void ASContext::CallOnInitUpdate(asIScriptObject *object)
{
	ASClassType *classType = reinterpret_cast<ASClassType*>(object->GetObjectType()
		->GetUserData());

	if (classType->OnInitUpdateFunc != 0)
	{
		asIScriptContext *ctx = _PrepareContextFromPool(classType->OnInitUpdateFunc);
		ctx->SetObject(object);
		_ContextExecute(ctx);
		_ReturnContextToPool(ctx);
	}
}
//----------------------------------------------------------------------------
void ASContext::CallOnUpdate(asIScriptObject *object)
{
	ASClassType *classType = reinterpret_cast<ASClassType*>(object->GetObjectType()
		->GetUserData());

	if (classType->OnUpdateFunc != 0)
	{
		asIScriptContext *ctx = _PrepareContextFromPool(classType->OnUpdateFunc);
		ctx->SetObject(object);
		_ContextExecute(ctx);
		_ReturnContextToPool(ctx);
	}
}
//----------------------------------------------------------------------------
void ASContext::CallOnFixUpdate(asIScriptObject *object)
{
	ASClassType *classType = reinterpret_cast<ASClassType*>(object->GetObjectType()
		->GetUserData());

	if (classType->OnFixUpdateFunc != 0)
	{
		asIScriptContext *ctx = _PrepareContextFromPool(classType->OnFixUpdateFunc);
		ctx->SetObject(object);
		_ContextExecute(ctx);
		_ReturnContextToPool(ctx);
	}
}
//----------------------------------------------------------------------------
void ASContext::CallOnResetPlay(asIScriptObject *object)
{
	ASClassType *classType = reinterpret_cast<ASClassType*>(object->GetObjectType()
		->GetUserData());

	if (classType->OnResetPlayFunc != 0)
	{
		asIScriptContext *ctx = _PrepareContextFromPool(classType->OnResetPlayFunc);
		ctx->SetObject(object);
		_ContextExecute(ctx);
		_ReturnContextToPool(ctx);
	}
}
//----------------------------------------------------------------------------
void ASContext::CallOnReset(asIScriptObject *object)
{
	ASClassType *classType = reinterpret_cast<ASClassType*>(object->GetObjectType()
		->GetUserData());

	if (classType->OnResetFunc != 0)
	{
		asIScriptContext *ctx = _PrepareContextFromPool(classType->OnResetFunc);
		ctx->SetObject(object);
		_ContextExecute(ctx);
		_ReturnContextToPool(ctx);
	}
}
//----------------------------------------------------------------------------
void ASContext::CallOnPlay(asIScriptObject *object)
{
	ASClassType *classType = reinterpret_cast<ASClassType*>(object->GetObjectType()
		->GetUserData());

	if (classType->OnPlayFunc != 0)
	{
		asIScriptContext *ctx = _PrepareContextFromPool(classType->OnPlayFunc);
		ctx->SetObject(object);
		_ContextExecute(ctx);
		_ReturnContextToPool(ctx);
	}
}
//----------------------------------------------------------------------------
void ASContext::CallOnStop(asIScriptObject *object)
{
	ASClassType *classType = reinterpret_cast<ASClassType*>(object->GetObjectType()
		->GetUserData());

	if (classType->OnStopFunc != 0)
	{
		asIScriptContext *ctx = _PrepareContextFromPool(classType->OnStopFunc);
		ctx->SetObject(object);
		_ContextExecute(ctx);
		_ReturnContextToPool(ctx);
	}
}
//----------------------------------------------------------------------------
void ASContext::CallOnPlayUpdate(asIScriptObject *object)
{
	ASClassType *classType = reinterpret_cast<ASClassType*>(object->GetObjectType()
		->GetUserData());

	if (classType->OnPlayUpdateFunc != 0)
	{
		asIScriptContext *ctx = _PrepareContextFromPool(classType->OnPlayUpdateFunc);
		ctx->SetObject(object);
		_ContextExecute(ctx);
		_ReturnContextToPool(ctx);
	}
}
//----------------------------------------------------------------------------
asIScriptContext *ASContext::_PrepareContextFromPool(asIScriptFunction *func)
{
	asIScriptContext *ctx = 0;
	if (mContextsPool.size() > 0)
	{
		ctx = *mContextsPool.rbegin();
		mContextsPool.pop_back();
	}
	else
	{
		ctx = mASEngine->CreateContext();
	}

	int r = ctx->Prepare(func); 
	assertion(r >= 0, "ctx Prepare failed.");

	return ctx;
}
//----------------------------------------------------------------------------
void ASContext::_ReturnContextToPool(asIScriptContext *ctx)
{
	mContextsPool.push_back(ctx);
	ctx->Unprepare();
}
//----------------------------------------------------------------------------
ASClassType *ASContext::_GetASClassType(const std::string &filename, 
	const std::string &className)
{
	int r;

	// Find the cached controller
	for (unsigned int n = 0; n < (int)mASClassTypes.size(); n++)
	{
		if (mASClassTypes[n]->ClassName == className)
			return mASClassTypes[n];
	}

	asIScriptModule *mod = mASEngine->GetModule(filename.c_str(),
		asGM_ONLY_IF_EXISTS);
	if (mod)
	{
		// 查看以前有没有尝试过加载，如果已经尝试过且加载失败了，就不需要再尝试加载了
		return 0;
	}

	_BuildModule(filename);

	// push in ASClassType
	ASClassType *classType = new0 ASClassType();
	mASClassTypes.push_back(classType);
	classType->ClassName = className;

	// Find class
	mod = mASEngine->GetModule(filename.c_str(), asGM_ONLY_IF_EXISTS);
	asITypeInfo *type = 0;
	int tc = mod->GetObjectTypeCount();
	for (int n = 0; n < tc; n++)
	{
		bool found = false;
		type = mod->GetObjectTypeByIndex(n);
		int ic = type->GetInterfaceCount();
		for (int i = 0; i < ic; i++)
		{
			if (strcmp(type->GetInterface(i)->GetName(), "IScript") == 0)
			{
				found = true;
				break;
			}
		}

		if (found == true)
		{
			classType->ClassType = type;
			break;
		}
	}

	if (classType->ClassType == 0)
	{
		PX2_LOG_ERROR("Couldn't find the ClassType class for the type: %s",
			className.c_str());
		mASClassTypes.pop_back();
		return 0;
	}

	// 构造函数
	// The game engine will pass in the owning Controlledable to the controller
	// for storage
	std::string s = std::string(type->GetName()) + "@ " +
		std::string(type->GetName()) + "(ScriptController @)";
	classType->FactoryFunc = type->GetFactoryByDecl(s.c_str());
	if (classType->FactoryFunc == 0)
	{
		PX2_LOG_ERROR("Couldn't find the appropriate factory for the type: %s",
			className.c_str());
		mASClassTypes.pop_back();
		return 0;
	}

	// 其他函数
	classType->OnAttachedFunc = type->GetMethodByDecl("void OnAttached()");
	classType->OnDetachFunc = type->GetMethodByDecl("void OnDetach()");
	classType->OnInitUpdateFunc = type->GetMethodByDecl("void OnInitUpdate()");
	classType->OnUpdateFunc = type->GetMethodByDecl("void OnUpdate()");
	classType->OnFixUpdateFunc = type->GetMethodByDecl("void OnFixUpdate()");

	classType->OnResetPlayFunc = type->GetMethodByDecl("void OnPResetPlay()");
	classType->OnResetFunc = type->GetMethodByDecl("void OnPReset()");
	classType->OnPlayFunc = type->GetMethodByDecl("void OnPPlay()");
	classType->OnStopFunc = type->GetMethodByDecl("void OnPStop()");
	classType->OnPlayUpdateFunc = type->GetMethodByDecl("void OnPUpdate()");

	// UserData
	type->SetUserData(classType);

	return classType;
}
//----------------------------------------------------------------------------
void ASContext::_BuildModule(const std::string &filename)
{
	int r = 0;

	CScriptBuilder builder;
	r = builder.StartNewModule(mASEngine, filename.c_str());
	if (r < 0)
		return;

	// 如果文件不存在，直接返回0
	int bufSize = 0;
	char *buffer = 0;
	if (!PX2_RM.LoadBuffer(filename, bufSize, buffer))
		return;

	r = builder.AddSectionFromMemory(filename.c_str(), buffer, bufSize);
	if (r < 0)
		return;

	r = builder.BuildModule();
	if (r < 0)
		return;

	asIScriptModule *module = builder.GetModule();
	mModules[filename] = module;
}
//----------------------------------------------------------------------------
int ASContext::_ContextExecute(asIScriptContext *ctx)
{
	int r = ctx->Execute();

	if (r != asEXECUTION_FINISHED)
	{
		if (r == asEXECUTION_EXCEPTION)
		{
			std::string errorStr;
			errorStr += "Exception: " + std::string(ctx->GetExceptionString()) + "\n";
			errorStr += "Function: " + std::string(ctx->GetExceptionFunction()->GetDeclaration()) + "\n";
			errorStr += "Line: " + StringHelp::IntToString(ctx->GetExceptionLineNumber()) + "\n";

			// It is possible to print more information about the location of the 
			// exception, for example the call stack, values of variables, etc if 
			// that is of interest.
		}
	}

	return r;
}
//----------------------------------------------------------------------------
void ASContext::MessageCallback(const asSMessageInfo &msg)
{
	std::string str = std::string(msg.section) +
		" (" + StringHelp::IntToString(msg.row) + ", " +
		StringHelp::IntToString(msg.col) + ") : " + 
		" : " + std::string(msg.message);

	if (msg.type == asMSGTYPE_WARNING)
	{
		PX2_LOG_INFO(str.c_str());
	}
	else if (msg.type == asMSGTYPE_INFORMATION)
	{
		PX2_LOG_INFO(str.c_str());
	}

	if (msg.type == asMSGTYPE_ERROR)
	{
		PX2_LOG_ERROR(str.c_str());

		IsHasCompileErrors = true;
	}
}
//----------------------------------------------------------------------------