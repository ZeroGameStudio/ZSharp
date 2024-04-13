// Copyright Zero Games. All Rights Reserved.


#include "ZMasterAssemblyLoadContext.h"

#include "ZAssembly.h"
#include "ZMasterAssemblyLoadContext_Interop.h"

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

