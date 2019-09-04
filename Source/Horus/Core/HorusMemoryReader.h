// ©Justin Camden 2019, all rights reserved.

#include "Runtime/Core/Public/Serialization/MemoryReader.h"
#include "Runtime/CoreUObject/Public/Serialization/ArchiveUObject.h"
/*
* Variant of MemoryReader that allows serialization of weak references.
*/
class FHorusMemoryReader : public FMemoryReader
{
public:
	explicit FHorusMemoryReader(const TArray<uint8>& InBytes, bool bIsPersistent = false)
		: FMemoryReader(InBytes, bIsPersistent) {}

	virtual FArchive& operator<<(FLazyObjectPtr& Value) override { return FArchiveUObject::SerializeLazyObjectPtr(*this, Value); }
	virtual FArchive& operator<<(FSoftObjectPtr& Value) override { return FArchiveUObject::SerializeSoftObjectPtr(*this, Value); }
	virtual FArchive& operator<<(FSoftObjectPath& Value) override { return FArchiveUObject::SerializeSoftObjectPath(*this, Value); }
	virtual FArchive& operator<<(FWeakObjectPtr& Value) override { return FArchiveUObject::SerializeWeakObjectPtr(*this, Value); }
};