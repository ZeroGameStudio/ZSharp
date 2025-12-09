// Copyright Zero Games. All Rights Reserved.

#include "ZSharpRuntimeModule.h"

#include "Interop/ZInteropAssertions.h"

#include "ZSharpRuntimeLogChannels.h"
#include "ZSharpRuntimeMacros.h"
#include "CLR/IZSharpClr.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZCommonMethodArgs.h"
#include "ALC/ZRedFrameScope.h"
#include "Emit/ZUnrealFieldScanner.h"
#include "ZCall/ZCallResolver_Export.h"
#include "ZCall/ZCallResolver_UFunction.h"
#include "ZCall/ZCallResolver_UProperty.h"
#include "Interop/ZUnmanagedFunctionInteropHelper.h"

#include "Interop/Serialization/ZArchive_Interop.h"
#include "Interop/CoreUObject/Object/ZUnrealObjectGlobals_Interop.h"
#include "Interop/CoreUObject/Object/ZUnrealObject_Interop.h"
#include "Interop/CoreUObject/Object/ZUnrealStruct_Interop.h"
#include "Interop/CoreUObject/Object/ZUnrealClass_Interop.h"
#include "Interop/CoreUObject/Struct/ZUnrealScriptStructBase_Interop.h"
#include "Interop/CoreUObject/String/ZUnrealString_Interop.h"
#include "Interop/CoreUObject/String/ZUnrealUtf8String_Interop.h"
#include "Interop/CoreUObject/String/ZUnrealAnsiString_Interop.h"
#include "Interop/CoreUObject/String/ZUnrealName_Interop.h"
#include "Interop/CoreUObject/String/ZUnrealText_Interop.h"
#include "Interop/CoreUObject/ObjectWrapper/ZSubclassOf_Interop.h"
#include "Interop/CoreUObject/ObjectWrapper/ZSoftClassPtr_Interop.h"
#include "Interop/CoreUObject/ObjectWrapper/ZSoftObjectPtr_Interop.h"
#include "Interop/CoreUObject/ObjectWrapper/ZWeakObjectPtr_Interop.h"
#include "Interop/CoreUObject/ObjectWrapper/ZLazyObjectPtr_Interop.h"
#include "Interop/CoreUObject/ObjectWrapper/ZScriptInterface_Interop.h"
#include "Interop/CoreUObject/ObjectWrapper/ZStrongObjectPtr_Interop.h"
#include "Interop/CoreUObject/Container/ZUnrealArray_Interop.h"
#include "Interop/CoreUObject/Container/ZUnrealSet_Interop.h"
#include "Interop/CoreUObject/Container/ZUnrealMap_Interop.h"
#include "Interop/CoreUObject/Container/ZUnrealOptional_Interop.h"
#include "Interop/CoreUObject/Delegate/ZUnrealDelegate_Interop.h"
#include "Interop/CoreUObject/Delegate/ZUnrealMulticastInlineDelegate_Interop.h"
#include "Interop/CoreUObject/Delegate/ZUnrealMulticastSparseDelegate_Interop.h"
#include "Interop/CoreUObject/ZUnrealFieldEmitter_Interop.h"
#include "Interop/Engine/ZEngine_Interop.h"
#include "Interop/Engine/ZGameInstance_Interop.h"
#include "Interop/Engine/ZStreamableManager_Interop.h"
#include "Interop/Engine/ZStreamingTask_Interop.h"
#include "Interop/Engine/ZWorld_Interop.h"
#include "Interop/Engine/ZActor_Interop.h"
#include "Interop/Engine/ZActorComponent_Interop.h"
#include "Interop/Engine/ZSceneComponent_Interop.h"
#include "Interop/GameplayTags/ZGameplayTag_Interop.h"
#include "Interop/EnhancedInput/ZEnhancedInputComponent_Interop.h"
#include "Interop/EnhancedInput/ZInputActionValue_Interop.h"

namespace ZSharp::ZSharpRuntimeModule_Private
{
	static void CreateMasterAlc()
	{
		IZSharpClr::Get().CreateMasterAlc();
	}
	
	static bool UnloadMasterAlc()
	{
		if (IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc())
		{
			alc->Unload();
			return true;
		}

		return false;
	}

	static void ReloadMasterAlc()
	{
		if (ZSHARP_WITH_MASTER_ALC_RELOAD && GIsEditor)
		{
			const bool existing = UnloadMasterAlc();
			CreateMasterAlc();
			if (existing)
			{
				UE_LOG(LogZSharpRuntime, Log, TEXT("Master ALC reloaded."));
			}
			else
			{
				UE_LOG(LogZSharpRuntime, Log, TEXT("Master ALC created."));
			}
		}
		else
		{
			UE_LOG(LogZSharpRuntime, Warning, TEXT("Reloading Master ALC is disabled in this environment."));
		}
	}

	static bool LoadEngineAssembly(IZMasterAssemblyLoadContext* alc)
	{
		static FZUnmanagedFunction GUnmanagedFunctions[] =
        {
#define ZSHARP_UNMANAGED_FUNCTION_ASSEMBLY ZeroGames.ZSharp.UnrealEngine

			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveUInt8),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadUInt8),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveUInt16),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadUInt16),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveUInt32),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadUInt32),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveUInt64),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadUInt64),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveInt8),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadInt8),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveInt16),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadInt16),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveInt32),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadInt32),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveInt64),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadInt64),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveFloat),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadFloat),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveDouble),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadDouble),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveString),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadString),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveAnsiString),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadAnsiString),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveUtf8String),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadUtf8String),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveName),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadName),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveText),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadText),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveScriptStruct),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadScriptStruct),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, NetSaveScriptStruct),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, NetLoadScriptStruct),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, SaveSoftObjectPath),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, LoadSoftObjectPath),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, IsSaving),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, IsLoading),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, Tell),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, Seek),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Archive, GetTotalSize),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObjectGlobals, NewObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObjectGlobals, LoadObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObjectGlobals, DuplicateObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObjectGlobals, FindObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObjectGlobals, LowLevelFindObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObjectGlobals, GetTransientPackage),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, GetClass),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, GetName),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, GetPathName),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, GetOuter),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, GetWorld),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, IsA),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, IsIn),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, Rename),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, IsValid),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealStruct, IsChildOf),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealClass, GetDefaultObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealClass, IsInterface),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealClass, ImplementsInterface),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealScriptStructBase, Copy),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealScriptStructBase, Identical),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealScriptStructBase, GetScriptStruct),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealScriptStructBase, IsA),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealString, GetData),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealString, SetData),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealUtf8String, GetData),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealUtf8String, SetData),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealAnsiString, GetData),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealAnsiString, SetData),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealName, GetData),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealName, SetData),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealName, IsNone),	

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealText, GetData),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealText, SetData),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SubclassOf, Copy),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SubclassOf, Identical),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SubclassOf, Hash),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SubclassOf, Get),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SubclassOf, Set),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SubclassOf, IsValid),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SubclassOf, IsNull),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftClassPtr, Copy),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftClassPtr, Identical),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftClassPtr, Hash),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftClassPtr, Get),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftClassPtr, Set),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftClassPtr, IsValid),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftClassPtr, IsNull),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftClassPtr, IsPending),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftClassPtr, Load),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftObjectPtr, Copy),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftObjectPtr, Identical),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftObjectPtr, Hash),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftObjectPtr, Get),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftObjectPtr, Set),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftObjectPtr, IsValid),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftObjectPtr, IsNull),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftObjectPtr, IsPending),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SoftObjectPtr, Load),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(WeakObjectPtr, Copy),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(WeakObjectPtr, Identical),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(WeakObjectPtr, Hash),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(WeakObjectPtr, Get),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(WeakObjectPtr, Set),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(WeakObjectPtr, IsValid),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(WeakObjectPtr, IsNull),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(WeakObjectPtr, IsStale),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(LazyObjectPtr, Copy),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(LazyObjectPtr, Identical),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(LazyObjectPtr, Hash),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(LazyObjectPtr, Get),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(LazyObjectPtr, Set),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(LazyObjectPtr, IsValid),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(LazyObjectPtr, IsNull),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(LazyObjectPtr, IsPending),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(ScriptInterface, Copy),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(ScriptInterface, Identical),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(ScriptInterface, Hash),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(ScriptInterface, Get),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(ScriptInterface, Set),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(ScriptInterface, IsValid),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(ScriptInterface, IsNull),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(StrongObjectPtr, Alloc),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StrongObjectPtr, Free),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StrongObjectPtr, Identical),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StrongObjectPtr, Hash),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StrongObjectPtr, Get),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StrongObjectPtr, Set),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StrongObjectPtr, IsValid),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StrongObjectPtr, IsNull),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealArray, Insert),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealArray, Remove),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealArray, RemoveAt),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealArray, Clear),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealArray, IndexOf),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealArray, Get),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealArray, Set),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealArray, Count),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealSet, Add),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealSet, Remove),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealSet, Clear),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealSet, Contains),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealSet, Count),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealSet, CreateEnumerator),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealSet, ReleaseEnumerator),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealSet, EnumeratorMoveNext),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealSet, EnumeratorCurrent),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMap, TryAdd),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMap, Remove),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMap, Clear),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMap, Contains),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMap, Get),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMap, Set),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMap, Count),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMap, CreateEnumerator),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMap, ReleaseEnumerator),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMap, EnumeratorMoveNext),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMap, EnumeratorCurrent),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealOptional, Get),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealOptional, Set),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealOptional, Reset),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealOptional, IsSet),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealDelegate, Execute),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealDelegate, BindStatelessManagedDelegate),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealDelegate, BindStatefulManagedDelegate),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealDelegate, BindUnrealFunction),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealDelegate, Unbind),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealDelegate, IsBound),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealDelegate, IsBoundToObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealDelegate, GetObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealDelegate, GetFunctionName),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastInlineDelegate, Broadcast),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastInlineDelegate, AddStatelessManagedDelegate),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastInlineDelegate, AddStatefulManagedDelegate),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastInlineDelegate, AddUnrealFunction),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastInlineDelegate, Remove),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastInlineDelegate, RemoveAll),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastInlineDelegate, Clear),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastInlineDelegate, IsBound),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastInlineDelegate, IsBoundToObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastInlineDelegate, Contains),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastSparseDelegate, Broadcast),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastSparseDelegate, AddStatelessManagedDelegate),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastSparseDelegate, AddStatefulManagedDelegate),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastSparseDelegate, AddUnrealFunction),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastSparseDelegate, Remove),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastSparseDelegate, RemoveAll),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastSparseDelegate, Clear),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastSparseDelegate, IsBound),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastSparseDelegate, IsBoundToObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealMulticastSparseDelegate, Contains),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealFieldEmitter, InternalConstructScriptStructInstance),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealFieldEmitter, InternalDestructScriptStructInstance),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealFieldEmitter, InternalReloadStructOpsFakeVTable),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(Engine, IsInitialized),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Engine, GetEngine),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(GameInstance, GetPrimaryInstance),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(StreamableManager, GetGlobalStreamableManager),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StreamableManager, RequestAsyncLoading),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(StreamingTask, GetResult),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StreamingTask, GetLoadedCount),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StreamingTask, Release),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(World, SpawnActor),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(World, GetWorldType),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(World, GetNetMode),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, FinishSpawning),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, AddComponent),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, FinishAddComponent),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, GetNetMode),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, GetActorTransform),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, GetActorLocation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, GetActorRotation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, GetActorScale),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, SetActorTransform),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, SetActorLocation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, SetActorRotation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, SetActorLocationAndRotation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, SetActorScale),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, SetActorRelativeTransform),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, SetActorRelativeLocation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, SetActorRelativeRotation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, SetActorRelativeScale),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, AddActorWorldTransform),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, AddActorWorldTransformKeepScale),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, AddActorWorldOffset),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, AddActorWorldRotation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, AddActorLocalTransform),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, AddActorLocalOffset),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Actor, AddActorLocalRotation),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(ActorComponent, GetNetMode),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, GetComponentTransform),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, GetComponentLocation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, GetComponentRotation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, GetComponentScale),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, GetSocketTransform),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, GetSocketLocation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, GetSocketRotation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, GetSocketScale),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, SetWorldTransform),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, SetWorldLocation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, SetWorldRotation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, SetWorldLocationAndRotation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, SetWorldScale),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, SetRelativeTransform),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, SetRelativeLocation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, SetRelativeRotation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, SetRelativeScale),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, AddWorldTransform),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, AddWorldTransformKeepScale),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, AddWorldOffset),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, AddWorldRotation),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, AddLocalTransform),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, AddLocalOffset),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(SceneComponent, AddLocalRotation),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(GameplayTag, Request),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(EnhancedInputComponent, BindUFunctionAction),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(EnhancedInputComponent, BindStatelessAction),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(EnhancedInputComponent, BindStatefulAction),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(EnhancedInputComponent, RemoveBindingByHandle),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(EnhancedInputComponent, RemoveBinding),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(InputActionValue, Deconstruct),

#undef ZSHARP_UNMANAGED_FUNCTION_ASSEMBLY
        };

		static void** GManagedFunctions[] =
		{
			ZSHARP_BUILD_MANAGED_FUNCTION(FZEngine_Interop::GPostEngineInit),
			
			ZSHARP_BUILD_MANAGED_FUNCTION(FZStreamableManager_Interop::GUpdate),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZStreamableManager_Interop::GSignalCompletion),

			ZSHARP_BUILD_MANAGED_FUNCTION(FZWorld_Interop::GNotifyWorldInitialized),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZWorld_Interop::GNotifyWorldTearingDown),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZWorld_Interop::GPreLoadMap),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZWorld_Interop::GPostLoadMap),
		};

		static const struct
		{
			FZUnmanagedFunctions UnmanagedFunctions { UE_ARRAY_COUNT(GUnmanagedFunctions), GUnmanagedFunctions };
			void*** ManagedFunctions = GManagedFunctions;
		} GArgs{};

		// We know engine assembly doesn't use conjugate so there is no need to push red frame.
		return alc->LoadAssembly(ZSHARP_ENGINE_ASSEMBLY_NAME, (void*)&GArgs) == EZLoadAssemblyErrorCode::Succeed;
	}

	static FAutoConsoleCommand GCmdReloadMasterAlc
	{
		TEXT("z#.reloadmasteralc"),
		TEXT("Reload Master ALC."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>&){ ReloadMasterAlc(); }),
		ECVF_Default
	};

	static TAutoConsoleVariable<bool> GCVarReloadMasterAlcOnBeginPIE
	{
		TEXT("z#.ReloadMasterAlcOnBeginPIE"),
		true,
		TEXT("If enabled, reload Master ALC when a PIE session begins.")
	};

	static TAutoConsoleVariable<bool> GCVarReloadMasterAlcOnEndPIE
	{
		TEXT("z#.ReloadMasterAlcOnEndPIE"),
		true,
		TEXT("If enabled, reload Master ALC when a PIE session ends.")
	};
}

class FZSharpRuntimeModule : public IZSharpRuntimeModule
{

	using ThisClass = FZSharpRuntimeModule;
	
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface

	virtual bool HasEngineAssembly() const override { return bHasEngineAssembly; }
	
	bool ParseStartupAssembly(const FString& startupAssembly, FString& outAssemblyName, TArray<FString>& outArgs);

	void HandlePreMasterAlcStartup(ZSharp::IZMasterAssemblyLoadContext* alc);
	void HandleMasterAlcStartup(ZSharp::IZMasterAssemblyLoadContext* alc);

#if WITH_EDITOR
	void HandleBeginPIE(const bool simulating);
	void HandleEndPIE(const bool simulating);
#endif

private:
	bool bHasEngineAssembly = false;
	
};

IMPLEMENT_MODULE(FZSharpRuntimeModule, ZSharpRuntime)

void FZSharpRuntimeModule::StartupModule()
{
	UE_CLOG(UObjectInitialized(), LogZSharpRuntime, Fatal, TEXT("UObject system is initialized before Z#!!!"));

	ZSharp::IZSharpClr::Get().PreMasterAlcStartup().AddRaw(this, &ThisClass::HandlePreMasterAlcStartup);
	ZSharp::IZSharpClr::Get().OnMasterAlcStartup().AddRaw(this, &ThisClass::HandleMasterAlcStartup);

	ZSharp::FZUnrealFieldScanner::Get().Startup();

	ZSharp::ZSharpRuntimeModule_Private::CreateMasterAlc();
	
#if WITH_EDITOR
	FEditorDelegates::PreBeginPIE.AddRaw(this, &ThisClass::HandleBeginPIE);
	FEditorDelegates::ShutdownPIE.AddRaw(this, &ThisClass::HandleEndPIE);
#endif
}

void FZSharpRuntimeModule::ShutdownModule()
{
	ZSharp::ZSharpRuntimeModule_Private::UnloadMasterAlc();

	ZSharp::FZUnrealFieldScanner::Get().Shutdown();
	
	ZSharp::IZSharpClr::Get().PreMasterAlcStartup().RemoveAll(this);
	ZSharp::IZSharpClr::Get().OnMasterAlcStartup().RemoveAll(this);
	
#if WITH_EDITOR
	FEditorDelegates::PreBeginPIE.RemoveAll(this);
	FEditorDelegates::ShutdownPIE.RemoveAll(this);
#endif
}

bool FZSharpRuntimeModule::ParseStartupAssembly(const FString& startupAssembly, FString& outAssemblyName, TArray<FString>& outArgs)
{
	outAssemblyName.Reset();
	outArgs.Reset();

	FString trimmed = startupAssembly.TrimStartAndEnd();

	uint8 state = 0;
	FString token;
	bool pointed = false;
	bool requireOpenParen = false;
	bool requireCloseParen = false;
	bool escape = false;
	bool requireComma = false;
	bool requireNextParam = false;
	bool inparam = false;
	for (int32 i = 0; i < trimmed.Len(); ++i)
	{
		bool finish = false;
		bool skip = false;
		bool next = false;
		const TCHAR c = trimmed[i];
		if (state == 0)
		{
			if (i == trimmed.Len() - 1)
			{
				if (!FChar::IsAlpha(c) && !FChar::IsDigit(c) && c != TEXT('_'))
				{
					UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], expect alpha, digit, . or _."), *trimmed, i);
					return false;
				}
					
				finish = true;
			}
			else if (requireOpenParen)
			{
				if (c != TEXT('(') && c != TEXT(' '))
				{
					UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], expect (."), *trimmed, i);
					return false;
				}

				skip = true;

				if (c == TEXT('('))
				{
					if (token.IsEmpty() || pointed)
					{
						UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], expect alpha, digit or _."), *trimmed, i);
						return false;
					}

					finish = true;
					requireCloseParen = true;
					next = true;
				}
			}
			else if (c == TEXT('('))
			{
				if (token.IsEmpty() || pointed)
				{
					UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], expect alpha, digit or _."), *trimmed, i);
					return false;
				}

				finish = true;
				skip = true;
				requireCloseParen = true;
				next = true;
			}
			else if (c == TEXT('.'))
			{
				if (token.IsEmpty() || pointed)
				{
					UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], unexpected.."), *trimmed, i);
					return false;
				}

				pointed = true;
			}
			else
			{
				if (!FChar::IsAlpha(c) && !FChar::IsDigit(c) && c != TEXT('_') && c != TEXT(' '))
				{
					UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], expect alpha, digit, . or _."), *trimmed, i);
					return false;
				}
				
				if (c == TEXT(' '))
				{
					if (pointed)
					{
						UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], unexpected.."), *trimmed, i);
						return false;
					}
					
					requireOpenParen = true;
				}

				pointed = false;
			}
		}
		else
		{
			if (i == trimmed.Len() - 1)
			{
				if (inparam)
				{
					UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], parameter not closed."), *trimmed, i);
					return false;
				}
				
				if (requireNextParam)
				{
					UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], unexpected ,."), *trimmed, i);
					return false;
				}
				
				if (c != TEXT(')'))
				{
					UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], expect )."), *trimmed, i);
					return false;
				}

				requireCloseParen = false;
			}
			else if (requireComma)
			{
				if (c != TEXT(',') && c != TEXT(' '))
				{
					UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], expect ,."), *trimmed, i);
					return false;
				}

				skip = true;
				if (c == TEXT(','))
				{
					requireComma = false;
					requireNextParam = true;
				}
			}
			else if (escape)
			{
				if (c != TEXT('\\') && c != TEXT('"'))
				{
					UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], only support escape \\ and \"."), *trimmed, i);
					return false;
				}

				escape = false;
			}
			else if (!inparam)
			{
				if (c != TEXT('"') && c != TEXT(' '))
				{
					UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], expect \"."), *trimmed, i);
					return false;
				}

				skip = true;

				if (c == TEXT('"'))
				{
					inparam = true;
					requireNextParam = false;
				}
			}
			else
			{
				if (c == TEXT('"'))
				{
					finish = true;
					skip = true;
					requireComma = true;
					inparam = false;
				}
				else if (c == TEXT('\\'))
				{
					escape = true;
					skip = true;
				}
			}
		}

		if (!skip)
		{
			token.AppendChar(c);
		}
		
		if (finish)
		{
			if (state == 0)
			{
				outAssemblyName = token;
			}
			else
			{
				outArgs.Emplace(token);
			}
			
			token.Reset();
		}

		if (next)
		{
			++state;
		}
	}

	if (requireCloseParen)
	{
		UE_LOG(LogZSharpRuntime, Warning, TEXT("Tokenize Master ALC startup assembly [%s] failed at [%d], parameter list not closed."), *trimmed, trimmed.Len());
		return false;
	}

	if (outAssemblyName.IsEmpty())
	{
		UE_LOG(LogZSharpRuntime, Warning, TEXT("Parse Master ALC startup assembly failed, assemblyName is empty."));
		return false;
	}

	UE_LOG(LogZSharpRuntime, Log, TEXT("Parsed [%s]"), *trimmed);
	return true;
}

void FZSharpRuntimeModule::HandlePreMasterAlcStartup(ZSharp::IZMasterAssemblyLoadContext* alc)
{
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_Export{}, 0);
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_UFunction{}, 1);
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_UProperty{}, 2);

	bHasEngineAssembly = ZSharp::ZSharpRuntimeModule_Private::LoadEngineAssembly(alc);
}

void FZSharpRuntimeModule::HandleMasterAlcStartup(ZSharp::IZMasterAssemblyLoadContext* alc)
{
	TArray<FString> startupAssemblies;
	GConfig->GetArray(TEXT("MasterALC"), TEXT("StartupAssemblies"), startupAssemblies, GConfig->GetConfigFilename(TEXT("ZSharp")));

	FString assemblyName;
	TArray<FString> args;
	TArray<const TCHAR*> argv;
	
	for (const auto& startupAssembly : startupAssemblies)
	{
		verify(ParseStartupAssembly(startupAssembly, assemblyName, args));

		argv.Reset();
		for (const auto& arg : args)
		{
			argv.Emplace(*arg);
		}
		ZSharp::FZCommonMethodArgs commonArgs = { argv.Num(), argv.GetData() };
		ZSharp::FZRedFrameScope scope;
		const ZSharp::EZLoadAssemblyErrorCode err = alc->LoadAssembly(assemblyName, &commonArgs);
		if (err != ZSharp::EZLoadAssemblyErrorCode::Succeed)
		{
			UE_LOG(LogZSharpRuntime, Fatal, TEXT("Master ALC startup assembly [%s] load failed with error code [%d]"), *assemblyName, err);
		}
	}
}

#if WITH_EDITOR
void FZSharpRuntimeModule::HandleBeginPIE(const bool simulating)
{
	if (ZSharp::ZSharpRuntimeModule_Private::GCVarReloadMasterAlcOnBeginPIE.GetValueOnGameThread())
	{
		ZSharp::ZSharpRuntimeModule_Private::ReloadMasterAlc();
	}
}

void FZSharpRuntimeModule::HandleEndPIE(const bool simulating)
{
	if (ZSharp::ZSharpRuntimeModule_Private::GCVarReloadMasterAlcOnEndPIE.GetValueOnGameThread())
	{
		ZSharp::ZSharpRuntimeModule_Private::ReloadMasterAlc();
	}
}
#endif


