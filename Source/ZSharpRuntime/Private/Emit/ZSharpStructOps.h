// Copyright Zero Games. All Rights Reserved.

#pragma once

// Z# script struct ops fake vtable implementation. This uses .NET thunk functions to provide compile time type info.
struct FZSharpStructOpsFakeVTable : UE::CoreUObject::Private::FStructOpsFakeVTable
{
	static void FallbackThunk(void* instance)
	{
		checkNoEntry();
	}
	
	FZSharpStructOpsFakeVTable* Clear()
	{
		ConstructThunk = &FallbackThunk;
		ConstructForTestThunk = &FallbackThunk;
		DestructThunk = &FallbackThunk;
		
		return this;
	}
	
	FZSharpStructOpsFakeVTable* Set(void* constructThunk, void* destructThunk)
	{
		ConstructThunk = constructThunk;
		ConstructForTestThunk = constructThunk;
		DestructThunk = destructThunk;
		
		return this;
	}
	
	void* ConstructThunk;
	void* ConstructForTestThunk;
	void* DestructThunk;
};


