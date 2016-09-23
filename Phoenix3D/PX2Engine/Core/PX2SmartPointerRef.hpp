// PX2SmartPointerRef.hpp

#ifndef PX2SMARTPOINTERREF_HPP
#define PX2SMARTPOINTERREF_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	template <class T>
	class PointerRef
	{
	public:
		PointerRef(T* obj = 0);
		PointerRef(const PointerRef& pointer);
		~PointerRef();

		operator T* () const;
		T& operator* () const;
		T* operator-> () const;

		PointerRef& operator= (T* obj);
		PointerRef& operator= (const PointerRef& rkReference);

		bool operator== (T* obj) const;
		bool operator!= (T* obj) const;
		bool operator== (const PointerRef& rkReference) const;
		bool operator!= (const PointerRef& rkReference) const;

	protected:
		T* mObject;
	};

#include "PX2SmartPointerRef.inl"

}

#endif
