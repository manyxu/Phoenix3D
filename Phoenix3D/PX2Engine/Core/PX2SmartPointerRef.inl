// PX2SmartPointerRef.hpp

//----------------------------------------------------------------------------
template <class T>
PointerRef<T>::PointerRef(T* obj)
{
	mObject = obj;
	if (mObject)
	{
		mObject->IncrementReferences();
	}
}
//----------------------------------------------------------------------------
template <class T>
PointerRef<T>::PointerRef(const PointerRef& pointer)
{
	mObject = pointer.mObject;
	if (mObject)
	{
		mObject->IncrementReferences();
	}
}
//----------------------------------------------------------------------------
template <class T>
PointerRef<T>::~PointerRef()
{
	if (mObject)
	{
		mObject->DecrementReferences();
	}
}
//----------------------------------------------------------------------------
template <class T>
PointerRef<T>::operator T* () const
{
	return mObject;
}
//----------------------------------------------------------------------------
template <class T>
T& PointerRef<T>::operator* () const
{
	return *mObject;
}
//----------------------------------------------------------------------------
template <class T>
T* PointerRef<T>::operator-> () const
{
	return mObject;
}
//----------------------------------------------------------------------------
template <class T>
PointerRef<T>& PointerRef<T>::operator= (T* obj)
{
	if (mObject != obj)
	{
		if (obj)
		{
			obj->IncrementReferences();
		}

		if (mObject)
		{
			mObject->DecrementReferences();
		}

		mObject = obj;
	}
	return *this;
}
//----------------------------------------------------------------------------
template <class T>
PointerRef<T>& PointerRef<T>::operator= (const PointerRef& pointer)
{
	if (mObject != pointer.mObject)
	{
		if (pointer.mObject)
		{
			pointer.mObject->IncrementReferences();
		}

		if (mObject)
		{
			mObject->DecrementReferences();
		}

		mObject = pointer.mObject;
	}
	return *this;
}
//----------------------------------------------------------------------------
template <class T>
bool PointerRef<T>::operator== (T* obj) const
{
	return mObject == obj;
}
//----------------------------------------------------------------------------
template <class T>
bool PointerRef<T>::operator!= (T* obj) const
{
	return mObject != obj;
}
//----------------------------------------------------------------------------
template <class T>
bool PointerRef<T>::operator== (const PointerRef& pointer) const
{
	return mObject == pointer.mObject;
}
//----------------------------------------------------------------------------
template <class T>
bool PointerRef<T>::operator!= (const PointerRef& pointer) const
{
	return mObject != pointer.mObject;
}
//----------------------------------------------------------------------------
