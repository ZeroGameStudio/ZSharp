// Copyright Zero Games. All Rights Reserved.


#include "ZMasterAssemblyLoadContext.h"

#include "ZAssembly.h"
#include "ZMasterAssemblyLoadContext_Interop.h"
#include "Interop/IZCallDispatcher.h"
#include "Interop/IZType.h"

ZSharp::FZGCHandle ZSharp::FZMasterAssemblyLoadContext::GetGCHandle() const
{
	return Handle;
}

void ZSharp::FZMasterAssemblyLoadContext::Unload()
{
	FZMasterAssemblyLoadContext_Interop::GUnload();
	UnloadCallback();
}

const ZSharp::IZAssembly* ZSharp::FZMasterAssemblyLoadContext::LoadAssembly(const TArray<uint8>& buffer, void* args)
{
	FZGCHandle handle = FZMasterAssemblyLoadContext_Interop::GLoadAssembly(buffer.GetData(), buffer.Num(), args);
	IZAssembly* assembly = new FZAssembly { handle };
	AssemblyMap.Emplace(assembly->GetName(), assembly);

	return assembly;
}

const ZSharp::IZAssembly* ZSharp::FZMasterAssemblyLoadContext::GetAssembly(const FString& name) const
{
	const TUniquePtr<IZAssembly>* assembly = AssemblyMap.Find(name);
	if (assembly)
	{
		return assembly->Get();
	}
	
	return assembly ? assembly->Get() : nullptr;
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::RegisterZCall(IZCallDispatcher* dispatcher)
{
	FZCallHandle handle = AllocateZCallHandle();

	ZCallMap.Emplace(handle, dispatcher);
	Name2ZCall.Emplace(dispatcher->GetName(), dispatcher);
	ZCall2Handle.Emplace(dispatcher, handle);

	return handle;
}

const ZSharp::IZCallDispatcher* ZSharp::FZMasterAssemblyLoadContext::GetZCallDispatcher(FZCallHandle handle) const
{
	const TUniquePtr<IZCallDispatcher>* pDispatcher = ZCallMap.Find(handle);
	return pDispatcher ? pDispatcher->Get() : nullptr;
}

const ZSharp::IZCallDispatcher* ZSharp::FZMasterAssemblyLoadContext::GetZCallDispatcher(const FString& name) const
{
	IZCallDispatcher* const* pDispatcher = Name2ZCall.Find(name);
	return pDispatcher ? *pDispatcher : nullptr;
}

const ZSharp::IZCallDispatcher* ZSharp::FZMasterAssemblyLoadContext::GetOrResolveZCallDispatcher(const FString& name)
{
	if (IZCallDispatcher* const* pDispatcher = Name2ZCall.Find(name))
	{
		return *pDispatcher;
	}

	for (const auto& resolver : ZCallResolverLink)
	{
		if (IZCallDispatcher* dispatcher = resolver.Get<1>()->Resolve(name))
		{
			(void)RegisterZCall(dispatcher);
			return dispatcher;
		}
	}
	
	return nullptr;
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::GetZCallHandle(const IZCallDispatcher* dispatcher) const
{
	const FZCallHandle* pHandle = ZCall2Handle.Find(dispatcher);
	return pHandle ? *pHandle : FZCallHandle{};
}

void ZSharp::FZMasterAssemblyLoadContext::RegisterZCallResolver(IZCallResolver* resolver, uint64 priority)
{
	ZCallResolverLink.Emplace(TTuple<uint64, TUniquePtr<IZCallResolver>>(priority, resolver));
	ZCallResolverLink.StableSort([](auto& lhs, auto& rhs){ return lhs.template Get<0>() < rhs.template Get<0>(); });
}

ZSharp::FZConjugateHandle ZSharp::FZMasterAssemblyLoadContext::BuildConjugate(void* unmanaged, const IZType* managedType)
{
	FZConjugateHandle managed = managedType->BuildConjugate(unmanaged);
	BuildConjugate(unmanaged, managed);

	return managed;
}

void ZSharp::FZMasterAssemblyLoadContext::BuildConjugate(void* unmanaged, FZConjugateHandle managed)
{
	ConjugateUnmanaged2Managed.Emplace(unmanaged, managed);
	ConjugateManaged2Unmanaged.Emplace(managed, unmanaged);
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate(void* unmanaged)
{
	FZConjugateHandle managed;
	ConjugateUnmanaged2Managed.RemoveAndCopyValue(unmanaged, managed);
	ConjugateManaged2Unmanaged.Remove(managed);
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate(FZConjugateHandle managed)
{
	void* unmanaged;
	ConjugateManaged2Unmanaged.RemoveAndCopyValue(managed, unmanaged);
	ConjugateUnmanaged2Managed.Remove(unmanaged);
}

ZSharp::FZConjugateHandle ZSharp::FZMasterAssemblyLoadContext::Conjugate(void* unmanaged) const
{
	const FZConjugateHandle* managed = ConjugateUnmanaged2Managed.Find(unmanaged);
	return managed ? *managed : FZConjugateHandle{};
}

void* ZSharp::FZMasterAssemblyLoadContext::Conjugate(FZConjugateHandle managed) const
{
	void* const* unmanaged = ConjugateManaged2Unmanaged.Find(managed);
	return unmanaged ? *unmanaged : nullptr;
}


