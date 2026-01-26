// Copyright Zero Games. All Rights Reserved.

#include "ZGameplayTag_Interop.h"

#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "NativeGameplayTags.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZGameplayTag_Interop_Private
{
	static TMap<FName, FNativeGameplayTag*> GManagedNativeTags;
	static struct FZManagedNativeTagsGuard
	{
		~FZManagedNativeTagsGuard()
		{
			check(GManagedNativeTags.IsEmpty());
		}
	} GManagedNativeTagsGuard;
	
	static FZConjugateHandle Request(const TCHAR* tagName)
	{
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(FGameplayTag::StaticStruct(), [tagName](const FZSelfDescriptiveScriptStruct& sdss)
		{
			*sdss.GetTypedUnderlyingInstance<FGameplayTag>() = FGameplayTag::RequestGameplayTag(tagName, false);
		});
	}
}

FNativeGameplayTag* ZSharp::FZGameplayTag_Interop::TryRegisterNativeTag(const TCHAR* tagName, const TCHAR* comment, const TCHAR* pluginName, const TCHAR* moduleName)
{
	if (UGameplayTagsManager::Get().RequestGameplayTag(tagName, false).IsValid())
	{
		ensureAlways(false);
		
		// Conflict with managed native tag, can resolve.
		if (FNativeGameplayTag** existingTag = ZGameplayTag_Interop_Private::GManagedNativeTags.Find(tagName))
		{
			check(*existingTag);
			return *existingTag;
		}
		// Conflict with tag from other sources, can't resolve.
		else
		{
			return nullptr;
		}
	}
	
	auto tag = new FNativeGameplayTag { pluginName, moduleName, tagName, comment, ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD };
	ZGameplayTag_Interop_Private::GManagedNativeTags.Add(tagName, tag);
	
	return tag;
}

uint8 ZSharp::FZGameplayTag_Interop::TryUnregisterNativeTag(FNativeGameplayTag* tag)
{
	if (!tag)
	{
		return false;
	}
	
	FGameplayTag t = tag->GetTag();
	if (!t.IsValid())
	{
		return false;
	}
	
	FName tagName = t.GetTagName();
	
	// Managed native tag conflict case, already released.
	FNativeGameplayTag** existingTag = ZGameplayTag_Interop_Private::GManagedNativeTags.Find(tagName);
	if (!existingTag)
	{
		ensureAlways(false);
		check(!UGameplayTagsManager::Get().RequestGameplayTag(tagName, false).IsValid());
		return false;
	}
	
	check(*existingTag);
	ZGameplayTag_Interop_Private::GManagedNativeTags.Remove(tagName);
	delete tag;
	
	return true;
}

ZSharp::FZConjugateHandle ZSharp::FZGameplayTag_Interop::Request(const TCHAR* tagName)
{
	TRY
	{
		return ZGameplayTag_Interop_Private::Request(tagName);
	}
	CATCHR({})
}


