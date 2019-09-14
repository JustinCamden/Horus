// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "Runtime/Core/Public/Serialization/BufferArchive.h"
#include "Runtime/CoreUObject/Public/Serialization/ArchiveUObject.h"

/*
* Variant of BufferArchive that allows serialization of weak references.
* This in the editor module because it is used exclusively for saving data, and should never be needed during runtime.
*/
class FHorusBufferArchive : public FBufferArchive
{
public:
	FHorusBufferArchive(bool bIsPersistent = false, const FName InArchiveName = NAME_None)
		:FBufferArchive(bIsPersistent, InArchiveName) {}

	virtual FArchive& operator<<(FLazyObjectPtr& Value) override { return FArchiveUObject::SerializeLazyObjectPtr(*this, Value); }
	virtual FArchive& operator<<(FSoftObjectPtr& Value) override { return FArchiveUObject::SerializeSoftObjectPtr(*this, Value); }
	virtual FArchive& operator<<(FSoftObjectPath& Value) override { return FArchiveUObject::SerializeSoftObjectPath(*this, Value); }
	virtual FArchive& operator<<(FWeakObjectPtr& Value) override { return FArchiveUObject::SerializeWeakObjectPtr(*this, Value); }
};