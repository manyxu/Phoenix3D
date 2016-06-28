// PX2ASScriptweakref.cpp

#include "PX2ASScriptweakref.hpp"
#include <new>
#include <assert.h>
#include <string.h> // strstr()

namespace PX2
{

	static void ScriptWeakRefConstruct(asITypeInfo *type, void *mem)
	{
		new(mem)ScriptWeakRef(type);
	}

	static void ScriptWeakRefConstruct2(asITypeInfo *type, void *ref, void *mem)
	{
		new(mem)ScriptWeakRef(ref, type);

		// It's possible the constructor raised a script exception, in which case we
		// need to call the destructor in order to cleanup the memory before returning
		asIScriptContext *ctx = asGetActiveContext();
		if (ctx && ctx->GetState() == asEXECUTION_EXCEPTION)
			reinterpret_cast<ScriptWeakRef*>(mem)->~ScriptWeakRef();
	}

	static void ScriptWeakRefDestruct(ScriptWeakRef *obj)
	{
		obj->~ScriptWeakRef();
	}

	static bool ScriptWeakRefTemplateCallback(asITypeInfo *ot, bool &/*dontGarbageCollect*/)
	{
		asITypeInfo *subType = ot->GetSubType();

		// Weak references only work for reference types
		if (subType == 0) return false;
		if (!(subType->GetFlags() & asOBJ_REF)) return false;

		// The subtype shouldn't be a handle
		if (ot->GetSubTypeId() & asTYPEID_OBJHANDLE)
			return false;

		// Make sure the type really supports weak references
		asUINT cnt = subType->GetBehaviourCount();
		for (asUINT n = 0; n < cnt; n++)
		{
			asEBehaviours beh;
			subType->GetBehaviourByIndex(n, &beh);
			if (beh == asBEHAVE_GET_WEAKREF_FLAG)
				return true;
		}

		ot->GetEngine()->WriteMessage("weakref", 0, 0, asMSGTYPE_ERROR, "The subtype doesn't support weak references");
		return false;
	}

	ScriptWeakRef::ScriptWeakRef(asITypeInfo *type)
	{
		m_ref = 0;
		m_type = type;
		m_type->AddRef();
		m_weakRefFlag = 0;
	}

	ScriptWeakRef::ScriptWeakRef(const ScriptWeakRef &other)
	{
		m_ref = other.m_ref;
		m_type = other.m_type;
		m_type->AddRef();
		m_weakRefFlag = other.m_weakRefFlag;
		if (m_weakRefFlag)
			m_weakRefFlag->AddRef();
	}

	ScriptWeakRef::ScriptWeakRef(void *ref, asITypeInfo *type)
	{
		m_ref = ref;
		m_type = type;
		m_type->AddRef();

		// The given type should be the weakref template instance
		assert(strcmp(type->GetName(), "weakref") == 0 ||
			strcmp(type->GetName(), "const_weakref") == 0);

		// Get the shared flag that will tell us when the object has been destroyed
		// This is threadsafe as we hold a strong reference to the object
		m_weakRefFlag = m_type->GetEngine()->GetWeakRefFlagOfScriptObject(m_ref, m_type->GetSubType());
		if (m_weakRefFlag)
			m_weakRefFlag->AddRef();
	}

	ScriptWeakRef::~ScriptWeakRef()
	{
		if (m_type)
			m_type->Release();
		if (m_weakRefFlag)
			m_weakRefFlag->Release();
	}

	ScriptWeakRef &ScriptWeakRef::operator =(const ScriptWeakRef &other)
	{
		// Don't do anything if it is the same reference
		if (m_ref == other.m_ref)
			return *this;

		// Must not allow changing the type
		if (m_type != other.m_type)
		{
			// We can allow a weakref to be assigned to a const_weakref
			if (!(strcmp(m_type->GetName(), "const_weakref") == 0 &&
				strcmp(other.m_type->GetName(), "weakref") == 0 &&
				m_type->GetSubType() == other.m_type->GetSubType()))
			{
				assert(false);
				return *this;
			}
		}

		m_ref = other.m_ref;

		if (m_weakRefFlag)
			m_weakRefFlag->Release();
		m_weakRefFlag = other.m_weakRefFlag;
		if (m_weakRefFlag)
			m_weakRefFlag->AddRef();

		return *this;
	}

	ScriptWeakRef &ScriptWeakRef::Set(void *newRef)
	{
		// Release the previous weak ref 
		if (m_weakRefFlag)
			m_weakRefFlag->Release();

		// Retrieve the new weak ref
		m_ref = newRef;
		if (newRef)
		{
			m_weakRefFlag = m_type->GetEngine()->GetWeakRefFlagOfScriptObject(newRef, m_type->GetSubType());
			m_weakRefFlag->AddRef();
		}
		else
			m_weakRefFlag = 0;

		// Release the newRef since we're only supposed to hold a weakref
		m_type->GetEngine()->ReleaseScriptObject(newRef, m_type->GetSubType());

		return *this;
	}

	asITypeInfo *ScriptWeakRef::GetRefType() const
	{
		return m_type->GetSubType();
	}

	bool ScriptWeakRef::operator==(const ScriptWeakRef &o) const
	{
		if (m_ref == o.m_ref &&
			m_type == o.m_type)
			return true;

		// TODO: If type is not the same, we should attempt to do a dynamic cast,
		//       which may change the pointer for application registered classes

		return false;
	}

	bool ScriptWeakRef::operator!=(const ScriptWeakRef &o) const
	{
		return !(*this == o);
	}

	// AngelScript: used as '@obj = ref.get();'
	void *ScriptWeakRef::Get() const
	{
		// If we hold a null handle, then just return null
		if (m_ref == 0 || m_weakRefFlag == 0)
			return 0;

		// Lock on the shared bool, so we can be certain it won't be changed to true
		// between the inspection of the flag and the increase of the ref count in the
		// owning object.
		m_weakRefFlag->Lock();
		if (!m_weakRefFlag->Get())
		{
			m_type->GetEngine()->AddRefScriptObject(m_ref, m_type->GetSubType());
			m_weakRefFlag->Unlock();
			return m_ref;
		}
		m_weakRefFlag->Unlock();

		return 0;
	}

	bool ScriptWeakRef::Equals(void *ref) const
	{
		// Release the ref since we'll not keep it
		m_type->GetEngine()->ReleaseScriptObject(ref, m_type->GetSubType());

		if (m_ref == ref)
			return true;

		return false;
	}

	void RegisterScriptWeakRef_Native(asIScriptEngine *engine)
	{
		int r;

		// Register a type for non-const handles
		r = engine->RegisterObjectType("weakref<class T>", sizeof(ScriptWeakRef), asOBJ_VALUE | asOBJ_ASHANDLE | asOBJ_TEMPLATE | asOBJ_APP_CLASS_DAK); assert(r >= 0);

		r = engine->RegisterObjectBehaviour("weakref<T>", asBEHAVE_CONSTRUCT, "void f(int&in)", asFUNCTION(ScriptWeakRefConstruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("weakref<T>", asBEHAVE_CONSTRUCT, "void f(int&in, T@+)", asFUNCTION(ScriptWeakRefConstruct2), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("weakref<T>", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptWeakRefDestruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("weakref<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(ScriptWeakRefTemplateCallback), asCALL_CDECL); assert(r >= 0);

		r = engine->RegisterObjectMethod("weakref<T>", "T@ opImplCast()", asMETHOD(ScriptWeakRef, Get), asCALL_THISCALL);
		r = engine->RegisterObjectMethod("weakref<T>", "T@ get() const", asMETHODPR(ScriptWeakRef, Get, () const, void*), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("weakref<T>", "weakref<T> &opHndlAssign(const weakref<T> &in)", asMETHOD(ScriptWeakRef, operator=), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("weakref<T>", "bool opEquals(const weakref<T> &in) const", asMETHODPR(ScriptWeakRef, operator==, (const ScriptWeakRef &) const, bool), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("weakref<T>", "weakref<T> &opHndlAssign(T@)", asMETHOD(ScriptWeakRef, Set), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("weakref<T>", "bool opEquals(const T@) const", asMETHOD(ScriptWeakRef, Equals), asCALL_THISCALL);

		// Register another type for const handles
		r = engine->RegisterObjectType("const_weakref<class T>", sizeof(ScriptWeakRef), asOBJ_VALUE | asOBJ_ASHANDLE | asOBJ_TEMPLATE | asOBJ_APP_CLASS_DAK); assert(r >= 0);

		r = engine->RegisterObjectBehaviour("const_weakref<T>", asBEHAVE_CONSTRUCT, "void f(int&in)", asFUNCTION(ScriptWeakRefConstruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("const_weakref<T>", asBEHAVE_CONSTRUCT, "void f(int&in, const T@+)", asFUNCTION(ScriptWeakRefConstruct2), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("const_weakref<T>", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptWeakRefDestruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("const_weakref<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(ScriptWeakRefTemplateCallback), asCALL_CDECL); assert(r >= 0);

		r = engine->RegisterObjectMethod("const_weakref<T>", "const T@ opImplCast() const", asMETHOD(ScriptWeakRef, Get), asCALL_THISCALL);
		r = engine->RegisterObjectMethod("const_weakref<T>", "const T@ get() const", asMETHODPR(ScriptWeakRef, Get, () const, void*), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("const_weakref<T>", "const_weakref<T> &opHndlAssign(const const_weakref<T> &in)", asMETHOD(ScriptWeakRef, operator=), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("const_weakref<T>", "bool opEquals(const const_weakref<T> &in) const", asMETHODPR(ScriptWeakRef, operator==, (const ScriptWeakRef &) const, bool), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("const_weakref<T>", "const_weakref<T> &opHndlAssign(const T@)", asMETHOD(ScriptWeakRef, Set), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("const_weakref<T>", "bool opEquals(const T@) const", asMETHOD(ScriptWeakRef, Equals), asCALL_THISCALL);

		// Allow non-const weak references to be converted to const weak references
		r = engine->RegisterObjectMethod("const_weakref<T>", "const_weakref<T> &opHndlAssign(const weakref<T> &in)", asMETHOD(ScriptWeakRef, operator=), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("const_weakref<T>", "bool opEquals(const weakref<T> &in) const", asMETHODPR(ScriptWeakRef, operator==, (const ScriptWeakRef &) const, bool), asCALL_THISCALL); assert(r >= 0);
	}

	static void ScriptWeakRefConstruct_Generic(asIScriptGeneric *gen)
	{
		asITypeInfo *ot = *reinterpret_cast<asITypeInfo**>(gen->GetAddressOfArg(0));

		ScriptWeakRefConstruct(ot, gen->GetObject());
	}

	static void ScriptWeakRefConstruct2_Generic(asIScriptGeneric *gen)
	{
		asITypeInfo *ot = *reinterpret_cast<asITypeInfo**>(gen->GetAddressOfArg(0));
		void *ref = gen->GetArgAddress(1);

		ScriptWeakRefConstruct2(ot, ref, gen->GetObject());
	}

	static void ScriptWeakRefDestruct_Generic(asIScriptGeneric *gen)
	{
		ScriptWeakRef *self = reinterpret_cast<ScriptWeakRef*>(gen->GetObject());
		self->~ScriptWeakRef();
	}

	void ScriptWeakRef_Get_Generic(asIScriptGeneric *gen)
	{
		ScriptWeakRef *self = reinterpret_cast<ScriptWeakRef*>(gen->GetObject());
		gen->SetReturnAddress(self->Get());
	}

	void ScriptWeakRef_Assign_Generic(asIScriptGeneric *gen)
	{
		ScriptWeakRef *other = reinterpret_cast<ScriptWeakRef*>(gen->GetArgAddress(0));
		ScriptWeakRef *self = reinterpret_cast<ScriptWeakRef*>(gen->GetObject());
		*self = *other;
		gen->SetReturnAddress(self);
	}

	void ScriptWeakRef_Assign2_Generic(asIScriptGeneric *gen)
	{
		void *other = gen->GetArgAddress(0);
		ScriptWeakRef *self = reinterpret_cast<ScriptWeakRef*>(gen->GetObject());

		// Must increase the refcount of the object, since Set() will decrease it
		// If this is not done, the object will be destroyed too early since the 
		// generic interface also automatically decreases the refcount of received handles
		gen->GetEngine()->AddRefScriptObject(other, self->GetRefType());

		self->Set(other);
		gen->SetReturnAddress(self);
	}

	void ScriptWeakRef_Equals_Generic(asIScriptGeneric *gen)
	{
		ScriptWeakRef *other = reinterpret_cast<ScriptWeakRef*>(gen->GetArgAddress(0));
		ScriptWeakRef *self = reinterpret_cast<ScriptWeakRef*>(gen->GetObject());
		gen->SetReturnByte(*self == *other);
	}

	void ScriptWeakRef_Equals2_Generic(asIScriptGeneric *gen)
	{
		void *other = gen->GetArgAddress(0);
		ScriptWeakRef *self = reinterpret_cast<ScriptWeakRef*>(gen->GetObject());

		// Must increase the refcount of the object, since Equals() will decrease it
		// If this is not done, the object will be destroyed too early since the 
		// generic interface also automatically decreases the refcount of received handles
		gen->GetEngine()->AddRefScriptObject(other, self->GetRefType());

		gen->SetReturnByte(self->Equals(other));
	}

	static void ScriptWeakRefTemplateCallback_Generic(asIScriptGeneric *gen)
	{
		asITypeInfo *ot = *reinterpret_cast<asITypeInfo**>(gen->GetAddressOfArg(0));
		bool *dontGarbageCollect = *reinterpret_cast<bool**>(gen->GetAddressOfArg(1));
		*reinterpret_cast<bool*>(gen->GetAddressOfReturnLocation()) = ScriptWeakRefTemplateCallback(ot, *dontGarbageCollect);
	}

	void RegisterScriptWeakRef_Generic(asIScriptEngine *engine)
	{
		int r;

		// Register a type for non-const handles
		r = engine->RegisterObjectType("weakref<class T>", sizeof(ScriptWeakRef), asOBJ_VALUE | asOBJ_ASHANDLE | asOBJ_TEMPLATE | asOBJ_APP_CLASS_DAK); assert(r >= 0);

		r = engine->RegisterObjectBehaviour("weakref<T>", asBEHAVE_CONSTRUCT, "void f(int&in)", asFUNCTION(ScriptWeakRefConstruct_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("weakref<T>", asBEHAVE_CONSTRUCT, "void f(int&in, T@)", asFUNCTION(ScriptWeakRefConstruct2_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("weakref<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(ScriptWeakRefTemplateCallback_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("weakref<T>", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptWeakRefDestruct_Generic), asCALL_GENERIC); assert(r >= 0);

		r = engine->RegisterObjectMethod("weakref<T>", "T@ opImplCast()", asFUNCTION(ScriptWeakRef_Get_Generic), asCALL_GENERIC);
		r = engine->RegisterObjectMethod("weakref<T>", "T@ get() const", asFUNCTION(ScriptWeakRef_Get_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectMethod("weakref<T>", "weakref<T> &opHndlAssign(const weakref<T> &in)", asFUNCTION(ScriptWeakRef_Assign_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectMethod("weakref<T>", "bool opEquals(const weakref<T> &in) const", asFUNCTION(ScriptWeakRef_Equals_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectMethod("weakref<T>", "weakref<T> &opHndlAssign(T@)", asFUNCTION(ScriptWeakRef_Assign2_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectMethod("weakref<T>", "bool opEquals(const T@) const", asFUNCTION(ScriptWeakRef_Equals2_Generic), asCALL_GENERIC);

		// Register another type for const handles
		r = engine->RegisterObjectType("const_weakref<class T>", sizeof(ScriptWeakRef), asOBJ_VALUE | asOBJ_ASHANDLE | asOBJ_TEMPLATE | asOBJ_APP_CLASS_DAK); assert(r >= 0);

		r = engine->RegisterObjectBehaviour("const_weakref<T>", asBEHAVE_CONSTRUCT, "void f(int&in)", asFUNCTION(ScriptWeakRefConstruct_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("const_weakref<T>", asBEHAVE_CONSTRUCT, "void f(int&in, const T@)", asFUNCTION(ScriptWeakRefConstruct2_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("const_weakref<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(ScriptWeakRefTemplateCallback_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("const_weakref<T>", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptWeakRefDestruct_Generic), asCALL_GENERIC); assert(r >= 0);

		r = engine->RegisterObjectMethod("const_weakref<T>", "const T@ opImplCast() const", asFUNCTION(ScriptWeakRef_Get_Generic), asCALL_GENERIC);
		r = engine->RegisterObjectMethod("const_weakref<T>", "const T@ get() const", asFUNCTION(ScriptWeakRef_Get_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectMethod("const_weakref<T>", "const_weakref<T> &opHndlAssign(const const_weakref<T> &in)", asFUNCTION(ScriptWeakRef_Assign_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectMethod("const_weakref<T>", "bool opEquals(const const_weakref<T> &in) const", asFUNCTION(ScriptWeakRef_Equals_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectMethod("const_weakref<T>", "const_weakref<T> &opHndlAssign(const T@)", asFUNCTION(ScriptWeakRef_Assign2_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectMethod("const_weakref<T>", "bool opEquals(const T@) const", asFUNCTION(ScriptWeakRef_Equals2_Generic), asCALL_GENERIC);

		// Allow non-const weak references to be converted to const weak references
		r = engine->RegisterObjectMethod("const_weakref<T>", "const_weakref<T> &opHndlAssign(const weakref<T> &in)", asFUNCTION(ScriptWeakRef_Assign_Generic), asCALL_GENERIC); assert(r >= 0);
		r = engine->RegisterObjectMethod("const_weakref<T>", "bool opEquals(const weakref<T> &in) const", asFUNCTION(ScriptWeakRef_Equals_Generic), asCALL_GENERIC); assert(r >= 0);
	}

	void RegisterScriptWeakRef(asIScriptEngine *engine)
	{
		if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY"))
			RegisterScriptWeakRef_Generic(engine);
		else
			RegisterScriptWeakRef_Native(engine);
	}
}