// Copyright Zero Games. All Rights Reserved.


#include "ZMasterAssemblyLoadContext.h"

#include "ZAssembly.h"
#include "ZGCHandle_Interop.h"
#include "ZMasterAssemblyLoadContext_Interop.h"
#include "ZSharpCLRLogChannels.h"
#include "Interop/IZCallDispatcher.h"
#include "Interop/IZType.h"

void ZSharp::FZMasterAssemblyLoadContext::Unload()
{
	check(IsInGameThread());

	UnloadCallback();

	// Check that all conjugates have been released correctly, otherwise there may be a memory leak.
	check(ConjugateManaged2Unmanaged.IsEmpty() && ConjugateUnmanaged2Managed.IsEmpty());
	
	FZMasterAssemblyLoadContext_Interop::GUnload();
}

const ZSharp::IZAssembly* ZSharp::FZMasterAssemblyLoadContext::LoadAssembly(const TArray<uint8>& buffer, void* args)
{
	check(IsInGameThread());
	
	FZGCHandle handle = FZMasterAssemblyLoadContext_Interop::GLoadAssembly(buffer.GetData(), buffer.Num(), args);
	IZAssembly* assembly = new FZAssembly { handle };
	AssemblyMap.Emplace(assembly->GetName(), assembly);

	return assembly;
}

const ZSharp::IZAssembly* ZSharp::FZMasterAssemblyLoadContext::GetAssembly(const FString& name) const
{
	check(IsInGameThread());
	
	const TUniquePtr<IZAssembly>* assembly = AssemblyMap.Find(name);
	if (assembly)
	{
		return assembly->Get();
	}
	
	return assembly ? assembly->Get() : nullptr;
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::RegisterZCall(IZCallDispatcher* dispatcher)
{
	check(IsInGameThread());
	
	FZCallHandle handle = AllocateZCallHandle();

	ZCallMap.Emplace(handle, dispatcher);
	Name2ZCall.Emplace(dispatcher->GetName(), dispatcher);
	ZCall2Handle.Emplace(dispatcher, handle);

	return handle;
}

const ZSharp::IZCallDispatcher* ZSharp::FZMasterAssemblyLoadContext::GetZCallDispatcher(FZCallHandle handle) const
{
	check(IsInGameThread());
	
	const TUniquePtr<IZCallDispatcher>* pDispatcher = ZCallMap.Find(handle);
	return pDispatcher ? pDispatcher->Get() : nullptr;
}

const ZSharp::IZCallDispatcher* ZSharp::FZMasterAssemblyLoadContext::GetZCallDispatcher(const FString& name) const
{
	check(IsInGameThread());
	
	IZCallDispatcher* const* pDispatcher = Name2ZCall.Find(name);
	return pDispatcher ? *pDispatcher : nullptr;
}

const ZSharp::IZCallDispatcher* ZSharp::FZMasterAssemblyLoadContext::GetOrResolveZCallDispatcher(const FString& name)
{
	check(IsInGameThread());
	
	if (IZCallDispatcher* const* pDispatcher = Name2ZCall.Find(name))
	{
		return *pDispatcher;
	}

	for (const auto& resolver : ZCallResolverLink)
	{
		if (IZCallDispatcher* dispatcher = resolver.Get<1>()->Resolve(name))
		{
			RegisterZCall(dispatcher);
			return dispatcher;
		}
	}
	
	return nullptr;
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::GetZCallHandle(const IZCallDispatcher* dispatcher) const
{
	check(IsInGameThread());
	
	const FZCallHandle* pHandle = ZCall2Handle.Find(dispatcher);
	return pHandle ? *pHandle : FZCallHandle{};
}

void ZSharp::FZMasterAssemblyLoadContext::RegisterZCallResolver(IZCallResolver* resolver, uint64 priority)
{
	check(IsInGameThread());
	
	ZCallResolverLink.Emplace(TTuple<uint64, TUniquePtr<IZCallResolver>>(priority, resolver));
	ZCallResolverLink.StableSort([](auto& lhs, auto& rhs){ return lhs.template Get<0>() < rhs.template Get<0>(); });
}

FDelegateHandle ZSharp::FZMasterAssemblyLoadContext::RegisterPreZCallToManaged(FZPreZCallToManaged::FDelegate delegate)
{
	return PreZCallToManaged.Add(delegate);
}

void ZSharp::FZMasterAssemblyLoadContext::UnregisterPreZCallToManaged(FDelegateHandle delegate)
{
	PreZCallToManaged.Remove(delegate);
}

void ZSharp::FZMasterAssemblyLoadContext::UnregisterPreZCallToManaged(const void* userObject)
{
	PreZCallToManaged.RemoveAll(userObject);
}

FDelegateHandle ZSharp::FZMasterAssemblyLoadContext::RegisterPostZCallToManaged(FZPostZCallToManaged::FDelegate delegate)
{
	return PostZCallToManaged.Add(delegate);
}

void ZSharp::FZMasterAssemblyLoadContext::UnregisterPostZCallToManaged(FDelegateHandle delegate)
{
	PostZCallToManaged.Remove(delegate);
}

void ZSharp::FZMasterAssemblyLoadContext::UnregisterPostZCallToManaged(const void* userObject)
{
	PostZCallToManaged.RemoveAll(userObject);
}

ZSharp::FZConjugateHandle ZSharp::FZMasterAssemblyLoadContext::BuildConjugate(void* unmanaged, const IZType* managedType)
{
	check(IsInGameThread());
	
	FZConjugateHandle managed = managedType->BuildConjugate(unmanaged);
	BuildConjugate(unmanaged, managed);

	return managed;
}

void ZSharp::FZMasterAssemblyLoadContext::BuildConjugate(void* unmanaged, FZConjugateHandle managed)
{
	check(IsInGameThread());
	
	ConjugateUnmanaged2Managed.Emplace(unmanaged, managed);
	ConjugateManaged2Unmanaged.Emplace(managed, unmanaged);
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate(void* unmanaged)
{
	check(IsInGameThread());
	
	FZConjugateHandle managed;
	if (ConjugateUnmanaged2Managed.RemoveAndCopyValue(unmanaged, managed))
	{
		ConjugateManaged2Unmanaged.Remove(managed);
		FZMasterAssemblyLoadContext_Interop::GReleaseConjugate(managed);
	}
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate(FZConjugateHandle managed)
{
	check(IsInGameThread());
	
	void* unmanaged;
	if (ConjugateManaged2Unmanaged.RemoveAndCopyValue(managed, unmanaged))
	{
		ConjugateUnmanaged2Managed.Remove(unmanaged);
		FZMasterAssemblyLoadContext_Interop::GReleaseConjugate(managed);
	}
}

ZSharp::FZConjugateHandle ZSharp::FZMasterAssemblyLoadContext::Conjugate(void* unmanaged) const
{
	check(IsInGameThread());
	
	const FZConjugateHandle* managed = ConjugateUnmanaged2Managed.Find(unmanaged);
	return managed ? *managed : FZConjugateHandle{};
}

void* ZSharp::FZMasterAssemblyLoadContext::Conjugate(FZConjugateHandle managed) const
{
	check(IsInGameThread());
	
	void* const* unmanaged = ConjugateManaged2Unmanaged.Find(managed);
	return unmanaged ? *unmanaged : nullptr;
}

void ZSharp::FZMasterAssemblyLoadContext::NotifyPreZCallToManaged() const
{
	UE_LOG(LogZSharpCLR, Verbose, TEXT("===================== Pre ZCall To Managed ====================="));
	
	PreZCallToManaged.Broadcast();
}

void ZSharp::FZMasterAssemblyLoadContext::NotifyPostZCallToManaged() const
{
	PostZCallToManaged.Broadcast();

	UE_LOG(LogZSharpCLR, Verbose, TEXT("===================== Post ZCall To Managed ====================="));
}


