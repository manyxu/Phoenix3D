// PX2DirData.cpp

#include "PX2DirData.hpp"
#include "PX2Memory.hpp"
#include "PX2Log.hpp"
#include "PX2Dir.hpp"
using namespace PX2;

#if defined (WIN32) || defined (_WIN32)
#include <windows.h>

//----------------------------------------------------------------------------
typedef WIN32_FIND_DATA FIND_STRUCT;
typedef HANDLE FIND_DATA;
typedef DWORD FIND_ATTR;
//----------------------------------------------------------------------------
inline FIND_DATA InitFindData()
{
	return INVALID_HANDLE_VALUE;
}
//----------------------------------------------------------------------------
inline bool IsFindDataOk(FIND_DATA fd)
{
	return fd != INVALID_HANDLE_VALUE;
}
//----------------------------------------------------------------------------
inline void FreeFindData(FIND_DATA fd)
{
	if (!::FindClose(fd))
	{
	}
}
const char *GetNameFromFindData(const FIND_STRUCT *finddata)
{
	return finddata->cFileName;
}
//----------------------------------------------------------------------------
inline bool CheckFoundMatch(const FIND_STRUCT* finddata,
	const std::string& filter)
{
	//if (filter.empty())
	//	return true;

	//std::string fn(GetNameFromFindData(finddata));

	//if (filter.find_first_not_of("*?.") == std::string::npos)
	//	return fn.Matches(filter);

	//return fn.MakeUpper().Matches(filter.Upper());

	return true;
}
//----------------------------------------------------------------------------
inline bool FindNext(FIND_DATA fd, const std::string& filter,
	FIND_STRUCT *finddata)
{
	for (;;)
	{
		if (!::FindNextFile(fd, finddata))
			return false;

		// If we did find something, check that it really matches.
		if (CheckFoundMatch(finddata, filter))
			return true;
	}
}
//----------------------------------------------------------------------------
inline FIND_DATA FindFirst(const std::string& spec, const std::string& filter,
	FIND_STRUCT *finddata)
{
	FIND_DATA fd = ::FindFirstFile(spec.c_str(), finddata);

	if (IsFindDataOk(fd) && !CheckFoundMatch(finddata, filter))
	{
		if (!FindNext(fd, filter, finddata))
		{
			FreeFindData(fd);

			return INVALID_HANDLE_VALUE;
		}
	}

	return fd;
}
//----------------------------------------------------------------------------
inline FIND_ATTR GetAttrFromFindData(FIND_STRUCT *finddata)
{
	return finddata->dwFileAttributes;
}
//----------------------------------------------------------------------------
inline bool IsDir(FIND_ATTR attr)
{
	return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
//----------------------------------------------------------------------------
inline bool IsHidden(FIND_ATTR attr)
{
	return (attr & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)) != 0;
}
#endif

//----------------------------------------------------------------------------
DirData::DirData(const std::string& dirname) :
mDirname(dirname)
{
	mFinddata = InitFindData();
}
//----------------------------------------------------------------------------
DirData::~DirData()
{
	Close();
}
//----------------------------------------------------------------------------
void DirData::SetFileSpec(const std::string& filespec)
{
	mFilespec = filespec;
}
//----------------------------------------------------------------------------
void DirData::SetFlags(int flags)
{
	mFlags = flags;
}
//----------------------------------------------------------------------------
void DirData::Close()
{
	if (IsFindDataOk(mFinddata))
	{
		FreeFindData(mFinddata);

		mFinddata = InitFindData();
	}
}
//----------------------------------------------------------------------------
void DirData::Rewind()
{
	Close();
}
//----------------------------------------------------------------------------
bool DirData::Read(std::string *filename)
{
	bool first = false;

	WIN32_FIND_DATA finddata;
#define PTR_TO_FINDDATA (&finddata)

	if (!IsFindDataOk(mFinddata))
	{
		// open first
		std::string filespec = mDirname;
		//if (!wxEndsWithPathSeparator(filespec))
		{
			filespec += '\\';
		}
		if (mFilespec.empty())
			filespec += "*.*";
		else
			filespec += mFilespec;

		mFinddata = FindFirst(filespec, mFilespec, PTR_TO_FINDDATA);

		first = true;
	}

	if (!IsFindDataOk(mFinddata))
	{
#if defined (WIN32) || defined (_WIN32)
		DWORD err = ::GetLastError();

		if (err != ERROR_FILE_NOT_FOUND && err != ERROR_NO_MORE_FILES)
		{
			PX2_LOG_ERROR("Cannot enumerate files in directory %s",
				mDirname.c_str());
		}
#endif

		return false;
	}

	const char *name = 0;
	FIND_ATTR attr;

	for (;;)
	{
		if (first)
		{
			first = false;
		}
		else
		{
			if (!FindNext(mFinddata, mFilespec, PTR_TO_FINDDATA))
			{
#if defined (WIN32) || defined (_WIN32)
				DWORD err = ::GetLastError();

				if (err != ERROR_NO_MORE_FILES)
				{
					PX2_LOG_ERROR("FindNext");
				}
#endif // __WIN32__
				//else: not an error, just no more (such) files

				return false;
			}
		}

		name = GetNameFromFindData(PTR_TO_FINDDATA);
		attr = GetAttrFromFindData(PTR_TO_FINDDATA);

		// don't return "." and ".." unless asked for
		if (name[0] == ('.') &&
			((name[1] == ('.') && name[2] == ('\0')) ||
			(name[1] == ('\0'))))
		{
			if (!(mFlags & Dir::DIR_DOTDOT))
				continue;
		}

		// check the type now
		if (!(mFlags & Dir::DIR_FILES) && !IsDir(attr))
		{
			// it's a file, but we don't want them
			continue;
		}
		else if (!(mFlags & Dir::DIR_DIRS) && IsDir(attr))
		{
			// it's a dir, and we don't want it
			continue;
		}

		// finally, check whether it's a hidden file
		if (!(mFlags & Dir::DIR_HIDDEN))
		{
			if (IsHidden(attr))
			{
				// it's a hidden file, skip it
				continue;
			}
		}

		*filename = name;

		break;
	}

	return true;
}
//----------------------------------------------------------------------------
std::string DirData::GetName()
{
	return mDirname;
}
//----------------------------------------------------------------------------