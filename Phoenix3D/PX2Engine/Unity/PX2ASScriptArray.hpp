// PX2ASScriptArray.hpp

#ifndef PX2ASSCRIPTARRAY_HPP
#define PX2ASSCRIPTARRAY_HPP

#include "PX2UnityPre.hpp"

#ifndef ANGELSCRIPT_H 
// Avoid having to inform include path if header is already include before
#include <angelscript.h>
#endif

namespace PX2
{

	struct SArrayBuffer;
	struct SArrayCache;

	/// %Script array class.
	class ScriptArray
	{
	public:
		// Set the memory functions that should be used by all CScriptArrays
		static void SetMemoryFunctions(asALLOCFUNC_t allocFunc, asFREEFUNC_t freeFunc);

		// Factory functions
		static ScriptArray *Create(asITypeInfo *ot);
		static ScriptArray *Create(asITypeInfo *ot, asUINT length);
		static ScriptArray *Create(asITypeInfo *ot, asUINT length, void *defaultValue);
		static ScriptArray *Create(asITypeInfo *ot, void *listBuffer);

		// Memory management
		void AddRef() const;
		void Release() const;

		// Type information
		asITypeInfo *GetArrayObjectType() const;
		int            GetArrayTypeId() const;
		int            GetElementTypeId() const;

		// Get the current size
		asUINT GetSize() const;

		// Returns true if the array is empty
		bool   IsEmpty() const;

		// Pre-allocates memory for elements
		void   Reserve(asUINT maxElements);

		// Resize the array
		void   Resize(asUINT numElements);

		// Get a pointer to an element. Returns 0 if out of bounds
		void       *At(asUINT index);
		const void *At(asUINT index) const;

		// Set value of an element. 
		// The value arg should be a pointer to the value that will be copied to the element.
		// Remember, if the array holds handles the value parameter should be the 
		// address of the handle. The refCount of the object will also be incremented
		void  SetValue(asUINT index, void *value);

		// Copy the contents of one array to another (only if the types are the same)
		ScriptArray &operator=(const ScriptArray&);

		// Compare two arrays
		bool operator==(const ScriptArray &) const;

		// Array manipulation
		void InsertAt(asUINT index, void *value);
		void RemoveAt(asUINT index);
		void InsertLast(void *value);
		void RemoveLast();
		void SortAsc();
		void SortDesc();
		void SortAsc(asUINT startAt, asUINT count);
		void SortDesc(asUINT startAt, asUINT count);
		void Sort(asUINT startAt, asUINT count, bool asc);
		void Reverse();
		int  Find(void *value) const;
		int  Find(asUINT startAt, void *value) const;
		int  FindByRef(void *ref) const;
		int  FindByRef(asUINT startAt, void *ref) const;

		// GC methods
		int  GetRefCount();
		void SetFlag();
		bool GetFlag();
		void EnumReferences(asIScriptEngine *engine);
		void ReleaseAllHandles(asIScriptEngine *engine);

	protected:
		mutable int       refCount;
		mutable bool      gcFlag;
		asITypeInfo    *objType;
		SArrayBuffer     *buffer;
		int               elementSize;
		int               subTypeId;

		// Constructors
		ScriptArray(asITypeInfo *ot, void *initBuf); // Called from script when initialized with list
		ScriptArray(asUINT length, asITypeInfo *ot);
		ScriptArray(asUINT length, void *defVal, asITypeInfo *ot);
		ScriptArray(const ScriptArray &other);
		virtual ~ScriptArray();

		bool  Less(const void *a, const void *b, bool asc, asIScriptContext *ctx, SArrayCache *cache);
		void *GetArrayItemPointer(int index);
		void *GetDataPointer(void *buffer);
		void  Copy(void *dst, void *src);
		void  Precache();
		bool  CheckMaxSize(asUINT numElements);
		void  Resize(int delta, asUINT at);
		void  CreateBuffer(SArrayBuffer **buf, asUINT numElements);
		void  DeleteBuffer(SArrayBuffer *buf);
		void  CopyBuffer(SArrayBuffer *dst, SArrayBuffer *src);
		void  Construct(SArrayBuffer *buf, asUINT start, asUINT end);
		void  Destruct(SArrayBuffer *buf, asUINT start, asUINT end);
		bool  Equals(const void *a, const void *b, asIScriptContext *ctx, SArrayCache *cache) const;
	};

	class ScriptDictionary;

	/// %Script dictionary value.
	class ScriptDictValue
	{
	public:
		// This class must not be declared as local variable in C++, because it needs 
		// to receive the script engine pointer in all operations. The engine pointer
		// is not kept as member in order to keep the size down
		ScriptDictValue();
		ScriptDictValue(asIScriptEngine *engine, void *value, int typeId);

		// Destructor must not be called without first calling FreeValue, otherwise a memory leak will occur
		~ScriptDictValue();

		// Replace the stored value
		void Set(asIScriptEngine *engine, void *value, int typeId);
		void Set(asIScriptEngine *engine, const asINT64 &value);
		void Set(asIScriptEngine *engine, const double &value);

		// Gets the stored value. Returns false if the value isn't compatible with the informed typeId
		bool Get(asIScriptEngine *engine, void *value, int typeId) const;
		bool Get(asIScriptEngine *engine, asINT64 &value) const;
		bool Get(asIScriptEngine *engine, double &value) const;

		// Returns the type id of the stored value
		int  GetTypeId() const;

		// Free the stored value
		void FreeValue(asIScriptEngine *engine);

	protected:
		friend class ScriptDictionary;

		union
		{
			asINT64 m_valueInt;
			double  m_valueFlt;
			void   *m_valueObj;
		};
		int m_typeId;
	};

	/// %Script dictionary class.
	class ScriptDictionary
	{
	public:
		// Factory functions
		static ScriptDictionary *Create(asIScriptEngine *engine);

		// Called from the script to instantiate a dictionary from an initialization list
		static ScriptDictionary *Create(asBYTE *buffer);

		// Reference counting
		void AddRef() const;
		void Release() const;

		// Reassign the dictionary
		ScriptDictionary &operator =(const ScriptDictionary &other);

		// Sets a key/value pair
		void Set(const std::string &key, void *value, int typeId);
		void Set(const std::string &key, const asINT64 &value);
		void Set(const std::string &key, const double &value);

		// Gets the stored value. Returns false if the value isn't compatible with the informed typeId
		bool Get(const std::string &key, void *value, int typeId) const;
		bool Get(const std::string &key, asINT64 &value) const;
		bool Get(const std::string &key, double &value) const;

		// Index accessors. If the dictionary is not const it inserts the value if it doesn't already exist
		// If the dictionary is const then a script exception is set if it doesn't exist and a null pointer is returned
		ScriptDictValue *operator[](const std::string &key);
		const ScriptDictValue *operator[](const std::string &key) const;

		// Returns the type id of the stored value, or negative if it doesn't exist
		int GetTypeId(const std::string &key) const;

		// Returns true if the key is set
		bool Exists(const std::string &key) const;

		// Returns true if there are no key/value pairs in the dictionary
		bool IsEmpty() const;

		// Returns the number of key/value pairs in the dictionary
		asUINT GetSize() const;

		// Deletes the key
		void Delete(const std::string &key);

		// Deletes all keys
		void DeleteAll();

		// Get an array of all keys
		ScriptArray *GetKeys() const;

	public:
		/// STL style iterator for %Script dictionary class.
		class Iterator
		{
		public:
			void operator++();    // Pre-increment
			void operator++(int); // Post-increment

			// This is needed to support C++11 range-for
			Iterator &operator*();

			bool operator==(const Iterator &other) const;
			bool operator!=(const Iterator &other) const;

			// Accessors
			const std::string &GetKey() const;
			int                GetTypeId() const;
			bool               GetValue(asINT64 &value) const;
			bool               GetValue(double &value) const;
			bool               GetValue(void *value, int typeId) const;

		protected:
			friend class ScriptDictionary;

			Iterator();
			Iterator(const ScriptDictionary &dict,
				std::map<std::string, ScriptDictValue>::iterator it);

			Iterator &operator=(const Iterator &) { return *this; } // Not used

			std::map<std::string, ScriptDictValue>::iterator m_it;
			const ScriptDictionary &m_dict;
		};

		Iterator begin() const;
		Iterator end() const;

		// Garbage collections behaviours
		int GetRefCount();
		void SetGCFlag();
		bool GetGCFlag();
		void EnumReferences(asIScriptEngine *engine);
		void ReleaseAllReferences(asIScriptEngine *engine);

	protected:
		ScriptDictionary(asIScriptEngine *engine);
		ScriptDictionary(asBYTE *buffer);
		virtual ~ScriptDictionary();

		asIScriptEngine *engine;
		mutable int refCount;
		mutable bool gcFlag;

		std::map<std::string, ScriptDictValue> dict;
	};

	/// Register the array type to script.
	void RegisterArray(asIScriptEngine* engine);
	/// Register the dictionary type to script.
	void RegisterDictionary(asIScriptEngine* engine);

}

#endif