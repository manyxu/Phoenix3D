// PX2ASScriptweakref.hpp

#ifndef PX2ASSCRIPTWEAKREF_HPP
#define PX2ASSCRIPTWEAKREF_HPP

// The ScriptWeakRef class was originally implemented by vroad in March 2013

//#ifndef ANGELSCRIPT_H 
// Avoid having to inform include path if header is already include before
#include <angelscript.h>
//#endif

namespace PX2
{

	class ScriptWeakRef
	{
	public:
		// Constructors
		ScriptWeakRef(asITypeInfo *type);
		ScriptWeakRef(const ScriptWeakRef &other);
		ScriptWeakRef(void *ref, asITypeInfo *type);

		~ScriptWeakRef();

		// Copy the stored value from another weakref object
		ScriptWeakRef &operator=(const ScriptWeakRef &other);

		// Compare equalness
		bool operator==(const ScriptWeakRef &o) const;
		bool operator!=(const ScriptWeakRef &o) const;

		// Sets a new reference
		ScriptWeakRef &Set(void *newRef);

		// Returns the object if it is still alive
		// This will increment the refCount of the returned object
		void *Get() const;

		// Returns true if the contained reference is the same
		bool Equals(void *ref) const;

		// Returns the type of the reference held
		asITypeInfo *GetRefType() const;

	protected:
		// These functions need to have access to protected
		// members in order to call them from the script engine
		friend void RegisterScriptWeakRef_Native(asIScriptEngine *engine);

		void                  *m_ref;
		asITypeInfo         *m_type;
		asILockableSharedBool *m_weakRefFlag;
	};

	void RegisterScriptWeakRef(asIScriptEngine *engine);

}

#endif