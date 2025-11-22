// Copyright Zero Games. All Rights Reserved.

#include "ZMasterAssemblyLoadContext.h"

#include "ZSharpCoreLogChannels.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Interop/Core/ZMasterAssemblyLoadContext_Interop.h"
#include "Interop/ZInteropRuntimeTypeUri.h"
#include "ZCall/IZCallDispatcher.h"
#include "Conjugate/ZConjugateRegistryDeclarations.h"

ZSharp::FZMasterAssemblyLoadContext::FZMasterAssemblyLoadContext(TUniqueFunction<void()>&& unloadingCallback, TUniqueFunction<void()>&& unloadedCallback)
	: UnloadingCallback(MoveTemp(unloadingCallback))
	, UnloadedCallback(MoveTemp(unloadedCallback))
	, bCoreUObjectShutdown(false)
	, bUnloadingPrepared(false)
	, bUnloaded(false)
	, RedStackDepth(0)
{
	FZConjugateRegistryDeclarations::ForeachDeclaration([this](uint16 id, const TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>& factory)
	{
		ConjugateRegistries.EmplaceAt(id, factory(*this));
	});
	
	GCDelegate = FCoreUObjectDelegates::GarbageCollectComplete.AddRaw(this, &ThisClass::HandleGarbageCollectComplete);
	PreExitDelegate = FCoreDelegates::OnPreExit.AddRaw(this, &ThisClass::HandlePreExit);
	
}

ZSharp::FZMasterAssemblyLoadContext::~FZMasterAssemblyLoadContext()
{
	check(bUnloaded);
	
	FCoreUObjectDelegates::GarbageCollectComplete.Remove(GCDelegate);
}

void ZSharp::FZMasterAssemblyLoadContext::Unload()
{
	checkSlow(IsInGameThread());
	check(!IsInRedStack());

	if (bUnloaded)
	{
		return;
	}

	UnloadingCallback();
	
	PrepareUnloading();

	// Reversely release conjugate registries (finally UObjects).
	TArray<IZConjugateRegistry*> reversedRegistries;
	for (const auto& registry : ConjugateRegistries)
	{
		reversedRegistries.Insert(registry.Get(), 0);
	}
	
	for (const auto& registry : reversedRegistries)
	{
		if (bCoreUObjectShutdown)
		{
			/*
			 * If CoreUObject module has shutdown, then all conjugates should have gone:
			 * 1. All black conjugates have been released during PrepareUnloading().
			 * 2. All UObjects have been purged during FCoreDelegate::OnExit so there are no UObject red conjugates.
			 * 3. We are not in red stack so there are no other red conjugates.
			 */
			check(registry->GetNumConjugates() == 0);
		}
		else
		{
			// If CoreUObject module has not shutdown, then all conjugates should have gone except UObjects.
			constexpr uint16 GUObjectConjugateRegistryId = 1;
			check(registry->GetRegistryId() == GUObjectConjugateRegistryId || registry->GetNumConjugates() == 0);
		}
		
		registry->Release();
	}

	FZMasterAssemblyLoadContext_Interop::GUnload();

	bUnloaded = true;

	UnloadedCallback();
}

ZSharp::EZLoadAssemblyErrorCode ZSharp::FZMasterAssemblyLoadContext::LoadAssembly(const FString& assemblyName, void* args)
{
    return FZMasterAssemblyLoadContext_Interop::GLoadAssembly(*assemblyName, args);
}

ZSharp::EZInvokeMethodErrorCode ZSharp::FZMasterAssemblyLoadContext::InvokeMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args)
{
	return FZMasterAssemblyLoadContext_Interop::GInvokeMethod(*assemblyName, *typeName, *methodName, args);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZMasterAssemblyLoadContext::GetType(const FZRuntimeTypeUri& uri) const
{
	checkSlow(IsInGameThread());

	FZInteropRuntimeTypeUri interopUri { *uri.Uri };
	FZRuntimeTypeHandle handle = FZMasterAssemblyLoadContext_Interop::GGetType(interopUri);
	return handle;
}

ZSharp::IZConjugateRegistry& ZSharp::FZMasterAssemblyLoadContext::GetConjugateRegistry(uint16 id) const
{
	checkSlow(IsInGameThread());
	check(ConjugateRegistries.IsValidIndex(id));

	return *ConjugateRegistries[id];
}

void* ZSharp::FZMasterAssemblyLoadContext::BuildConjugate(void* unmanaged, FZRuntimeTypeHandle type)
{
	checkSlow(IsInGameThread());

	return BuildConjugate_Red(unmanaged, type);
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate(void* unmanaged)
{
	checkSlow(IsInGameThread());

	ReleaseConjugate_Red(unmanaged);
}

void ZSharp::FZMasterAssemblyLoadContext::PushRedFrame()
{
	checkSlow(IsInGameThread());
	
	++RedStackDepth;
	
	for (const auto& registry : ConjugateRegistries)
	{
		registry->PushRedFrame();
	}
}

void ZSharp::FZMasterAssemblyLoadContext::PopRedFrame()
{
	checkSlow(IsInGameThread());
	check(IsInRedStack());
	
	for (const auto& registry : ConjugateRegistries)
	{
		registry->PopRedFrame();
	}

	--RedStackDepth;
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::RegisterZCall(IZCallDispatcher* dispatcher)
{
	checkSlow(IsInGameThread());
	
	FZCallHandle handle = FZCallHandle::Alloc();

	ZCallMap.Emplace(handle, dispatcher);
	ZCallName2Handle.Emplace(dispatcher->GetName(), handle);

	return handle;
}

void ZSharp::FZMasterAssemblyLoadContext::RegisterZCallResolver(IZCallResolver* resolver, uint64 priority)
{
	checkSlow(IsInGameThread());
	
	ZCallResolverLink.Emplace(TTuple<uint64, TUniquePtr<IZCallResolver>> { priority, resolver });
	ZCallResolverLink.StableSort([](auto& lhs, auto& rhs){ return lhs.template Get<0>() < rhs.template Get<0>(); });
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::GetZCallHandle(const FString& name)
{
	checkSlow(IsInGameThread());
	
	return GetZCallHandle_Red(name);
}

ZSharp::EZCallErrorCode ZSharp::FZMasterAssemblyLoadContext::ZCall(FZCallHandle handle, FZCallBuffer* buffer)
{
	checkSlow(IsInGameThread());
	check(IsInRedStack());

	return ZCall_Red(handle, buffer);
}

void* ZSharp::FZMasterAssemblyLoadContext::BuildConjugate_Black(uint16 registryId, void* userdata)
{
	checkSlow(IsInGameThread());

	return GetConjugateRegistry(registryId).BuildConjugate(userdata);
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate_Black(uint16 registryId, void* unmanaged)
{
	checkSlow(IsInGameThread());

	GetConjugateRegistry(registryId).ReleaseConjugate(unmanaged);
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::GetZCallHandle_Black(const FString& name)
{
	checkSlow(IsInGameThread());

	FZCallHandle handle{};
	const FZCallHandle* pHandle = ZCallName2Handle.Find(name);
	if (!pHandle)
	{
		for (const auto& resolver : ZCallResolverLink)
		{
			if (IZCallDispatcher* dispatcher = resolver.Get<1>()->Resolve(name))
			{
				check(dispatcher->GetName() == name);
				handle = RegisterZCall(dispatcher);
			}
		}
	}
	else
	{
		handle = *pHandle;
	}

	return handle;
}

ZSharp::EZCallErrorCode ZSharp::FZMasterAssemblyLoadContext::ZCall_Black(FZCallHandle handle, FZCallBuffer* buffer) const
{
	checkSlow(IsInGameThread());
	check(IsInRedStack());
	
	if (!handle.IsBlack())
	{
		return EZCallErrorCode::InvalidHandle;
	}
	
	const TUniquePtr<IZCallDispatcher>* pDispatcher = ZCallMap.Find(handle);
	if (!pDispatcher)
	{
		return EZCallErrorCode::DispatcherNotFound;
	}

	return (*pDispatcher)->Dispatch(buffer);
}

void* ZSharp::FZMasterAssemblyLoadContext::BuildConjugate_Red(void* unmanaged, FZRuntimeTypeHandle type)
{
	return FZMasterAssemblyLoadContext_Interop::GBuildConjugate_Red(unmanaged, type);
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate_Red(void* unmanaged)
{
	FZMasterAssemblyLoadContext_Interop::GReleaseConjugate_Red(unmanaged);
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::GetZCallHandle_Red(const FString& name)
{
	return FZMasterAssemblyLoadContext_Interop::GGetZCallHandle_Red(*name);
}

ZSharp::EZCallErrorCode ZSharp::FZMasterAssemblyLoadContext::ZCall_Red(FZCallHandle handle, FZCallBuffer* buffer)
{
#if !UE_BUILD_SHIPPING
	FString fatalMessage;
	EZCallErrorCode res = FZMasterAssemblyLoadContext_Interop::GZCall_Red(handle, buffer, &fatalMessage);
	if (!fatalMessage.IsEmpty())
	{
		UE_LOG(LogZSharpCore, Fatal, TEXT("%s"), *fatalMessage);
	}
	
	return res;
#else
	return FZMasterAssemblyLoadContext_Interop::GZCall_Red(handle, buffer, nullptr);
#endif
}

void ZSharp::FZMasterAssemblyLoadContext::PrepareUnloading()
{
	if (bUnloadingPrepared)
	{
		return;
	}
	
	bUnloadingPrepared = true;
	
	FZMasterAssemblyLoadContext_Interop::GPrepareUnloading();
}

void ZSharp::FZMasterAssemblyLoadContext::HandleGarbageCollectComplete()
{
	UE_CLOG(IsInRedStack(), LogZSharpCore, Fatal, TEXT("Collect Garbage inside of Red ZCall!!!"));
}

void ZSharp::FZMasterAssemblyLoadContext::HandlePreExit()
{
	PrepareUnloading();
}

void ZSharp::FZMasterAssemblyLoadContext::HandleExit()
{
	bCoreUObjectShutdown = true;
}


