// Copyright Zero Games. All Rights Reserved.


#include "ZAssembly.h"

#include "Interop/ZAssembly_Interop.h"
#include "ZType.h"

const FString& ZSharp::FZAssembly::GetName() const
{
	if (!CachedName)
	{
		FString data;
		FZAssembly_Interop::GGetName(Handle, data);
		CachedName = MoveTemp(data);
	}

	return *CachedName;
}

const ZSharp::IZType* ZSharp::FZAssembly::GetType(const FString& name) const
{
	const TUniquePtr<IZType>* pType = TypeMap.Find(name);
	if (pType)
	{
		return pType->Get();
	}
	
	FZGCHandle handle = FZAssembly_Interop::GGetType(Handle, *name);
	IZType* type = new FZType { handle };
	TypeMap.Emplace(name, type);

	return type;
}
