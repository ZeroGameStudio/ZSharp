// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpStruct.inl"
#include "ZSharpStructOps.h"

namespace ZSharp::ZSharpScriptStruct_Private
{
	// Dynamic ICppStructOps implementation for all Z# script structs.
	struct FZSharpStructOps final : public UScriptStruct::ICppStructOps
	{
		explicit FZSharpStructOps(int32 size, int32 align, void* constructThunk, void* destructThunk)
			: ICppStructOps(size, align)
		{
			using namespace UE::CoreUObject::Private;
			
			FCapabilities capabilities{};
			capabilities.HasSerializerObjectReferences = EPropertyObjectReferenceType::Conservative;
			capabilities.HasDestructor = true;
			
			// ICppStructOps has no destructor so make sure vtable is trivially destructible and we don't need to delete it.
			static_assert(std::is_trivially_destructible_v<FZSharpStructOpsFakeVTable>);
			FakeVPtr = (new FZSharpStructOpsFakeVTable
			{
				EStructOpsFakeVTableFlags::Construct | EStructOpsFakeVTableFlags::ConstructForTests | EStructOpsFakeVTableFlags::Destruct,
				capabilities
			})->Set(constructThunk, destructThunk);
		}
	};
}


