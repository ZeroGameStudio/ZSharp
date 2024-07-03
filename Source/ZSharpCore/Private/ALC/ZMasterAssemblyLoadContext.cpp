// Copyright Zero Games. All Rights Reserved.


#include "ZMasterAssemblyLoadContext.h"

#include "ZSharpCoreLogChannels.h"
#include "Interop/ZMasterAssemblyLoadContext_Interop.h"
#include "ZCall/IZCallDispatcher.h"
#include "ZCall/ZConjugateRegistryDeclarations.h"

ZSharp::FZMasterAssemblyLoadContext::FZMasterAssemblyLoadContext(FZGCHandle handle, TUniqueFunction<void()>&& unloadCallback)
	: Handle(handle)
	, UnloadCallback(MoveTemp(unloadCallback))
	, bUnloaded(false)
	, RedZCallDepth(0)
	, bZCallPrepared(false)
{
	FZConjugateRegistryDeclarations::ForeachDeclaration([this](uint16 id, const TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>& factory)
	{
		ConjugateRegistries.EmplaceAt(id, factory(*this));
	});

	TickDelegate = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &ThisClass::Tick));
	GCDelegate = FCoreUObjectDelegates::GarbageCollectComplete.AddRaw(this, &ThisClass::HandleGarbageCollectComplete);
}

ZSharp::FZMasterAssemblyLoadContext::~FZMasterAssemblyLoadContext()
{
	check(bUnloaded);
	
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegate);
	FCoreUObjectDelegates::GarbageCollectComplete.Remove(GCDelegate);
	
	Handle.Free();
}

void ZSharp::FZMasterAssemblyLoadContext::Unload()
{
	check(IsInGameThread());
	check(!IsInsideOfRedZCall());

	if (bUnloaded)
	{
		return;
	}

	for (const auto& registry : ConjugateRegistries)
	{
		registry->Release();
	}

	FZMasterAssemblyLoadContext_Interop::GUnload();

	bUnloaded = true;

	UnloadCallback();
}

void ZSharp::FZMasterAssemblyLoadContext::LoadAssembly(const TArray<uint8>& buffer, void* args)
{
	check(IsInGameThread());
	
	FZMasterAssemblyLoadContext_Interop::GLoadAssembly(buffer.GetData(), buffer.Num(), args);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZMasterAssemblyLoadContext::GetType(const FString& assemblyName, const FString& typeName)
{
	check(IsInGameThread());
	
	return FZMasterAssemblyLoadContext_Interop::GGetType(*assemblyName, *typeName);
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::RegisterZCall(IZCallDispatcher* dispatcher)
{
	check(IsInGameThread());
	
	FZCallHandle handle = FZCallHandle::Alloc();

	ZCallMap.Emplace(handle, dispatcher);
	ZCallName2Handle.Emplace(dispatcher->GetName(), handle);

	return handle;
}

void ZSharp::FZMasterAssemblyLoadContext::RegisterZCallResolver(IZCallResolver* resolver, uint64 priority)
{
	check(IsInGameThread());
	
	ZCallResolverLink.Emplace(TTuple<uint64, TUniquePtr<IZCallResolver>> { priority, resolver });
	ZCallResolverLink.StableSort([](auto& lhs, auto& rhs){ return lhs.template Get<0>() < rhs.template Get<0>(); });
}

void ZSharp::FZMasterAssemblyLoadContext::PrepareForZCall()
{
	check(IsInGameThread());
	check(!bZCallPrepared);
	
	PushRedFrame();
	bZCallPrepared = true;
}

int32 ZSharp::FZMasterAssemblyLoadContext::ZCall(FZCallHandle handle, FZCallBuffer* buffer)
{
	check(IsInGameThread());
	check(bZCallPrepared);

	bZCallPrepared = false;
	return ZCall_Red(handle, buffer);
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::GetZCallHandle(const FString& name)
{
	check(IsInGameThread());
	
	return GetZCallHandle_Red(name);
}

void* ZSharp::FZMasterAssemblyLoadContext::BuildConjugate(void* unmanaged, FZRuntimeTypeHandle type)
{
	check(IsInGameThread());
	
	return BuildConjugate_Red(unmanaged, type);
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate(void* unmanaged)
{
	check(IsInGameThread());

	ReleaseConjugate_Red(unmanaged);
}

ZSharp::IZConjugateRegistry& ZSharp::FZMasterAssemblyLoadContext::GetConjugateRegistry(uint16 id)
{
	check(IsInGameThread());
	check(ConjugateRegistries.IsValidIndex(id));

	return *ConjugateRegistries[id];
}

int32 ZSharp::FZMasterAssemblyLoadContext::ZCall_Black(FZCallHandle handle, FZCallBuffer* buffer) const
{
	check(IsInGameThread());
	check(IsInsideOfRedZCall());
	
	if (!handle.IsBlack())
	{
		return -1;
	}
	
	const TUniquePtr<IZCallDispatcher>* pDispatcher = ZCallMap.Find(handle);
	if (!pDispatcher)
	{
		return -1;
	}

	return (*pDispatcher)->Dispatch(buffer);
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::GetZCallHandle_Black(const FString& name)
{
	check(IsInGameThread());

	FZCallHandle handle{};
	const FZCallHandle* pHandle = ZCallName2Handle.Find(name);
	if (!pHandle)
	{
		for (const auto& resolver : ZCallResolverLink)
		{
			if (IZCallDispatcher* dispatcher = resolver.Get<1>()->Resolve(name))
			{
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

void* ZSharp::FZMasterAssemblyLoadContext::BuildConjugate_Black(uint16 registryId, void* userdata)
{
	check(IsInGameThread());

	return GetConjugateRegistry(registryId).BuildConjugate(userdata);
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate_Black(uint16 registryId, void* unmanaged)
{
	check(IsInGameThread());

	GetConjugateRegistry(registryId).ReleaseConjugate(unmanaged);
}

bool ZSharp::FZMasterAssemblyLoadContext::Tick(float deltaTime)
{
	return FZMasterAssemblyLoadContext_Interop::GTick(deltaTime) > 0;
}

int32 ZSharp::FZMasterAssemblyLoadContext::ZCall_Red(FZCallHandle handle, FZCallBuffer* buffer)
{
	ON_SCOPE_EXIT { PopRedFrame(); };

	return FZMasterAssemblyLoadContext_Interop::GZCall_Red(handle, buffer);
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::GetZCallHandle_Red(const FString& name)
{
	return FZMasterAssemblyLoadContext_Interop::GGetZCallHandle_Red(*name);
}

void* ZSharp::FZMasterAssemblyLoadContext::BuildConjugate_Red(void* unmanaged, FZRuntimeTypeHandle type)
{
	return FZMasterAssemblyLoadContext_Interop::GBuildConjugate_Red(unmanaged, type);
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate_Red(void* unmanaged)
{
	FZMasterAssemblyLoadContext_Interop::GReleaseConjugate_Red(unmanaged);
}

void ZSharp::FZMasterAssemblyLoadContext::PushRedFrame()
{
	++RedZCallDepth;
	
	for (const auto& registry : ConjugateRegistries)
	{
		registry->PushRedFrame();
	}
}

void ZSharp::FZMasterAssemblyLoadContext::PopRedFrame()
{
	for (const auto& registry : ConjugateRegistries)
	{
		registry->PopRedFrame();
	}

	--RedZCallDepth;
}

void ZSharp::FZMasterAssemblyLoadContext::HandleGarbageCollectComplete()
{
	UE_CLOG(IsInsideOfRedZCall(), LogZSharpCore, Fatal, TEXT("Collect Garbage inside of Red ZCall!!!"));
}


