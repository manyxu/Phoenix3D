// PX2NetServerIocp.inl

//-----------------------------------------------------------------------------
inline int ServerIocp::GetNumUsedOLBuffer()
{
	return mServerBufMgr.GetUsedBuffers();
}
//-----------------------------------------------------------------------------