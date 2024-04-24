// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZSlimAssemblyLoadContext.h"

namespace ZSharp
{
	class FZSlimAssemblyLoadContext : public IZSlimAssemblyLoadContext
	{

	public:
		FZSlimAssemblyLoadContext(FZGCHandle handle, const TFunction<void()>& unloadCallback, const FString& name)
			: Handle(handle)
			, UnloadCallback(unloadCallback)
			, Name(name)
			, RunningCount(0)
			, bUnloading(false){}

		virtual ~FZSlimAssemblyLoadContext() override { Free(Handle); }

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }
		
		// IZAssemblyLoadContext
		virtual void Unload() override;

		// IZSlimAssemblyLoadContext
		virtual const FString& GetName() const override { return Name; }
		
		virtual int32 LoadAssembly(const TArray<uint8>& buffer, void* args) override;

		virtual int32 CallMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args) const override;

	private:
		FZGCHandle Handle;
		TFunction<void()> UnloadCallback;
		FString Name;

		mutable TAtomic<uint64> RunningCount;
		mutable TAtomic<bool> bUnloading;
		
	};
}


