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

ZSharp::IZAssembly* ZSharp::FZMasterAssemblyLoadContext::LoadAssembly(const TArray<uint8>& buffer)
{
	FZGCHandle handle = FZMasterAssemblyLoadContext_Interop::GLoadAssembly(buffer.GetData(), buffer.Num());
	IZAssembly* assembly = new FZAssembly(handle);
	AssemblyMap.Emplace(assembly->GetName(), assembly);

	return assembly;
}

ZSharp::IZAssembly* ZSharp::FZMasterAssemblyLoadContext::GetAssembly(const FString& name) const
{
	const TUniquePtr<IZAssembly>* assembly = AssemblyMap.Find(name);
	if (assembly)
	{
		return assembly->Get();
	}
	
	return assembly ? assembly->Get() : nullptr;
}

ZSharp::IZType* ZSharp::FZMasterAssemblyLoadContext::GetType(const FString& name) const
{
	checkNoEntry();
	return nullptr;
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::RegisterZCall(IZCallDispatcher* dispatcher)
{
	FZCallHandle handle = FAllocZCallHandle::Alloc();

	ZCallMap.Emplace(handle, dispatcher);
	Name2ZCall.Emplace(dispatcher->GetName(), dispatcher);
	ZCall2Handle.Emplace(dispatcher, handle);

	return handle;
}

ZSharp::IZCallDispatcher* ZSharp::FZMasterAssemblyLoadContext::GetZCallDispatcher(FZCallHandle handle) const
{
	const TUniquePtr<IZCallDispatcher>* pDispatcher = ZCallMap.Find(handle);
	return pDispatcher ? pDispatcher->Get() : nullptr;
}

ZSharp::IZCallDispatcher* ZSharp::FZMasterAssemblyLoadContext::GetZCallDispatcher(const FString& name) const
{
	IZCallDispatcher* const* pDispatcher = Name2ZCall.Find(name);
	return pDispatcher ? *pDispatcher : nullptr;
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::GetZCallHandle(const IZCallDispatcher* dispatcher) const
{
	const FZCallHandle* pHandle = ZCall2Handle.Find(dispatcher);
	return pHandle ? *pHandle : FZCallHandle();
}

ZSharp::FZGCHandle ZSharp::FZMasterAssemblyLoadContext::BuildConjugate(void* native, const IZType* type)
{
	FZGCHandle handle = type->New();
	BuildConjugate(native, handle);

	return handle;
}

void ZSharp::FZMasterAssemblyLoadContext::BuildConjugate(void* native, FZGCHandle handle)
{
	Native2Conjugate.Emplace(native, handle);
	Conjugate2Native.Emplace(handle, native);
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate(void* native)
{
	FZGCHandle handle;
	Native2Conjugate.RemoveAndCopyValue(native, handle);
	Conjugate2Native.Remove(handle);
}

void ZSharp::FZMasterAssemblyLoadContext::ReleaseConjugate(FZGCHandle handle)
{
	void* native;
	Conjugate2Native.RemoveAndCopyValue(handle, native);
	Native2Conjugate.Remove(native);
}


