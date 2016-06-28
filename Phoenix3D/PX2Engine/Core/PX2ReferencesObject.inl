// PX2ReferencesObject.inl

//----------------------------------------------------------------------------
inline void RefObject::IncrementReferences()
{
	mReferences++;
}
//----------------------------------------------------------------------------
inline int RefObject::GetReferences() const
{
	return mReferences;
}
//----------------------------------------------------------------------------
