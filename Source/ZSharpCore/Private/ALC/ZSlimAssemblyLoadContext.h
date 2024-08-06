// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/IZSlimAssemblyLoadContext.h"

namespace ZSharp
{
	class FZSlimAssemblyLoadContext : public IZSlimAssemblyLoadContext
	{

	public:
		FZSlimAssemblyLoadContext(FZGCHandle handle, TUniqueFunction<void()>&& unloadCallback, const FString& name)
			: Handle(handle)
			, UnloadCallback(MoveTemp(unloadCallback))
			, Name(name)
			, RunningCount(0)
			, bUnloading(false){}

		virtual ~FZSlimAssemblyLoadContext() override { Handle.Free(); }

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }
		
		// IZAssemblyLoadContext
		virtual void Unload() override;

		// IZSlimAssemblyLoadContext
		virtual FString GetName() const override { return Name; }
		
		virtual EZLoadAssemblyErrorCode LoadAssembly(const TArray<uint8>& buffer, void* args) override;

		virtual EZCallMethodErrorCode CallMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args) const override;

	private:
		FZGCHandle Handle;
		TUniqueFunction<void()>&& UnloadCallback;
		FString Name;

		mutable TAtomic<uint64> RunningCount;
		mutable TAtomic<bool> bUnloading;
		
	};
}


