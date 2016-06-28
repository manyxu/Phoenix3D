// PX2ReferencesObject.hpp

#ifndef PX2REFERENCESOBJECT_HPP
#define PX2REFERENCESOBJECT_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM RefObject
	{
	public: 
		RefObject();
		virtual ~RefObject();

	public:
		void IncrementReferences();
		void DecrementReferences();
		int GetReferences() const;

	private:
		int mReferences;
	};

#include "PX2ReferencesObject.inl"

}

#endif