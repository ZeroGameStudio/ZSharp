// Copyright Zero Games. All Rights Reserved.


#include "ZMasterAssemblyLoadContext.h"

#include "ZAssembly.h"
#include "ZMasterAssemblyLoadContext_Interop.h"
#include "Interop/IZCallDispatcher.h"
#include "Interop/ZCallBuffer.h"

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

class FZTestZCall : public ZSharp::IZCallDispatcher
{
	virtual const FString& GetName() const override { return Name; }
	virtual int32 Dispatch(ZSharp::FZCallBuffer* buffer) const override
	{
		UE_LOG(LogTemp, Error, TEXT("===================== TestZCall: [%f] ====================="), buffer->Slots[0].Content.Float);
		return 1;
	}
	FString Name = TEXT("TestZCall");
};

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::RegisterZCall(IZCallDispatcher* dispatcher)
{
	return FZCallHandle();
}

ZSharp::IZCallDispatcher* ZSharp::FZMasterAssemblyLoadContext::GetZCallDispatcher(FZCallHandle handle) const
{
	return new FZTestZCall;
}

ZSharp::IZCallDispatcher* ZSharp::FZMasterAssemblyLoadContext::GetZCallDispatcher(const FString& name) const
{
	return new FZTestZCall;
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext::GetZCallHandle(const IZCallDispatcher* dispatcher) const
{
	return FZCallHandle();
}

