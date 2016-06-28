// PX2Filename.cpp

#include "PX2Filename.hpp"
#include "PX2FileFn.hpp"
using namespace PX2;

#if defined (WIN32) || defined(_WIN32)
#include <windows.h>
#endif

//----------------------------------------------------------------------------
void RemoveTrailingSeparatorsFromPath(std::string& strPath)
{
	while (IsEndsWithPathSeparator(strPath))
	{
		size_t len = strPath.length();
		if (len == 1 || (len == 3 && strPath[len - 2] == ':'))
		{
			break;
		}

		strPath = strPath.substr(0, len - 1);
	}
}

//----------------------------------------------------------------------------
bool PX2FileSystemObjectExists(const std::string& path, int flags)
{
	const bool acceptFile = (flags & PX2_FILE_EXISTS_REGULAR) != 0;
	const bool acceptDir = (flags & PX2_FILE_EXISTS_DIR) != 0;

	std::string strPath(path);

#if defined(WIN32) || defined(_WIN32)
	if (acceptDir)
	{
		RemoveTrailingSeparatorsFromPath(strPath);
	}

	DWORD ret = ::GetFileAttributes(strPath.c_str());

	if (ret == INVALID_FILE_ATTRIBUTES)
		return false;

	if (ret & FILE_ATTRIBUTE_DIRECTORY)
		return acceptDir;

	// Anything else must be a file (perhaps we should check for
	// FILE_ATTRIBUTE_REPARSE_POINT?)
	return acceptFile;
#elif defined(__OS2__)
#else // Non-MSW, non-OS/2
#endif // Platforms
}

//----------------------------------------------------------------------------
Filename::Filename()
{
}
//----------------------------------------------------------------------------
Filename::~Filename()
{
}
//----------------------------------------------------------------------------
bool Filename::IsDirExists(const std::string &dirPath)
{
	return PX2FileSystemObjectExists(dirPath, PX2_FILE_EXISTS_DIR);
}
//----------------------------------------------------------------------------
bool Filename::IsFileExists(const std::string &filePath)
{
	return PX2FileSystemObjectExists(filePath, PX2_FILE_EXISTS_REGULAR);
}
//----------------------------------------------------------------------------