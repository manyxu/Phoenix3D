// PX2FunObject.hpp

#ifndef PX2FUNOBJECT_HPP
#define PX2FUNOBJECT_HPP

#include "PX2CorePre.hpp"
#include "PX2Any.hpp"
#include "PX2SmartPointerRef.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	enum PX2_ENGINE_ITEM FunParamType
	{
		FPT_NONE,
		FPT_INT,
		FPT_FLOAT,
		FPT_APOINT,
		FPT_AVECTOR,
		FPT_BOOL,
		FPT_STRING,
		FPT_POINTER,
		FPT_POINTER_THIS,
		FPT_POINTER_THIS_STATIC,
		FPT_MAX_TYPE
	};

	struct PX2_ENGINE_ITEM FunParam
	{
		FunParam();
		~FunParam();

		std::string Name;
		FunParamType Type;
		Any Value;
	};

	class PX2_ENGINE_ITEM FunObject
	{
	public:
		FunObject();
		~FunObject();

		bool IsClassCatalogue;

		std::string ClassName;
		std::string FunName;
		void AddInput(const std::string &paramName, FunParamType type,
			const Any &paramValue);
		void AddOutput(const std::string &paramName, FunParamType type,
			const Any &paramValue);

		const std::vector<FunParam> &GetInParams() const;
		int GetNumInParams() const;
		const FunParam &GetInParam(int i) const;
		void ClearInParams();

		const std::vector<FunParam> &GetOutParams() const;
		int GetNumOutParams() const;
		const FunParam &GetOutParam(int i) const;
		void ClearOutParams();

	protected:
		std::vector<FunParam> mInParams;
		std::vector<FunParam> mOutParams;

	public:
		FunObject *GetAddClass (const std::string &className);
		FunObject *GetFunObject(const std::string &className,
			const std::string &funName);

		void AddFunObject(FunObject *funObj);
		bool IsHasFunObject(FunObject *funObj);

	public:
		std::vector<Pointer0<FunObject> > mChildFunObjectVec_Class;
		std::vector<Pointer0<FunObject> > mChildFunObjectVec;
	};

	typedef Pointer0<FunObject> FunObjectPtr;

	class PX2_ENGINE_ITEM FunObjectManager : public Singleton<FunObjectManager>
	{
	public:
		FunObjectManager();
		virtual ~FunObjectManager();

		bool Initlize();
		bool Terminate();

		FunObject *GetFunObject();

		std::map<std::string, FunObjectPtr> GetFunMap();
		FunObject *GetAddFunMap(const std::string &name);

	protected:
		FunObjectPtr mObjectFunObject;
		std::map<std::string, FunObjectPtr> mFunMap;
	};

}

#endif