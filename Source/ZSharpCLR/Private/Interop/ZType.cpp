// Copyright Zero Games. All Rights Reserved.


#include "ZType.h"

#include "ZType_Interop.h"
#include "ZMethodInfo.h"
#include "ZPropertyInfo.h"

const FString& ZSharp::FZType::GetName() const
{
	if (!CachedName)
	{
		FString data;
		FZType_Interop::GGetName(Handle, data);
		CachedName = MoveTemp(data);
	}

	return *CachedName;
}

bool ZSharp::FZType::IsZCallable() const
{
	return true;
}

ZSharp::FZConjugateHandle ZSharp::FZType::BuildConjugate(void* unmanaged) const
{
	if (!IsZCallable())
	{
		return FZConjugateHandle{};
	}
	
	return FZType_Interop::GBuildConjugate(Handle, unmanaged);
}

const ZSharp::IZMethodInfo* ZSharp::FZType::GetMethod(const FString& name) const
{
	const TUniquePtr<IZMethodInfo>* pMethod = MethodMap.Find(name);
	if (pMethod)
	{
		return pMethod->Get();
	}
	
	FZGCHandle handle = FZType_Interop::GGetMethodInfo(Handle, *name);
	IZMethodInfo* method = new FZMethodInfo { handle };
	MethodMap.Emplace(name, method);

	return method;
}

const ZSharp::IZPropertyInfo* ZSharp::FZType::GetProperty(const FString& name) const
{
	const TUniquePtr<IZPropertyInfo>* pProperty = PropertyMap.Find(name);
	if (pProperty)
	{
		return pProperty->Get();
	}
	
	FZGCHandle handle = FZType_Interop::GGetPropertyInfo(Handle, *name);
	IZPropertyInfo* property = new FZPropertyInfo { handle };
	PropertyMap.Emplace(name, property);

	return property;
}
