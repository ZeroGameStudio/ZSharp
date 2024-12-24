// Copyright Zero Games. All Rights Reserved.

#include "ZSharpRuntimeModule.h"

#include "ZSharpRuntimeLogChannels.h"
#include "CLR/IZSharpClr.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZCommonMethodArgs.h"
#include "Emit/ZUnrealFieldScanner.h"
#include "ZCall/ZCallResolver_Export.h"
#include "ZCall/ZCallResolver_UFunction.h"
#include "ZCall/ZCallResolver_UProperty.h"
#include "Interop/ZUnmanagedFunctionInteropHelper.h"

#include "Interop/Object/ZUnrealObjectGlobals_Interop.h"
#include "Interop/Object/ZUnrealObject_Interop.h"
#include "Interop/Object/ZUnrealStruct_Interop.h"
#include "Interop/Object/ZUnrealClass_Interop.h"
#include "Interop/Struct/ZUnrealScriptStruct_Interop.h"
#include "Interop/String/ZUnrealString_Interop.h"
#include "Interop/String/ZUnrealUtf8String_Interop.h"
#include "Interop/String/ZUnrealAnsiString_Interop.h"
#include "Interop/String/ZUnrealName_Interop.h"
#include "Interop/String/ZUnrealText_Interop.h"
#include "Interop/ObjectWrapper/ZSubclassOf_Interop.h"
#include "Interop/ObjectWrapper/ZSoftClassPtr_Interop.h"
#include "Interop/ObjectWrapper/ZSoftObjectPtr_Interop.h"
#include "Interop/ObjectWrapper/ZWeakObjectPtr_Interop.h"
#include "Interop/ObjectWrapper/ZLazyObjectPtr_Interop.h"
#include "Interop/ObjectWrapper/ZScriptInterface_Interop.h"
#include "Interop/ObjectWrapper/ZStrongObjectPtr_Interop.h"
#include "Interop/Container/ZUnrealArray_Interop.h"
#include "Interop/Container/ZUnrealSet_Interop.h"
#include "Interop/Container/ZUnrealMap_Interop.h"
#include "Interop/Container/ZUnrealOptional_Interop.h"
#include "Interop/Delegate/ZUnrealDelegate_Interop.h"
#include "Interop/Delegate/ZUnrealMulticastInlineDelegate_Interop.h"
#include "Interop/Delegate/ZUnrealMulticastSparseDelegate_Interop.h"
#include "Interop/Engine/ZEngine_Interop.h"
#include "Interop/AsyncLoading/ZStreamableManager_Interop.h"
#include "Interop/AsyncLoading/ZStreamingTask_Interop.h"
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
		if (!ensure(GIsEditor))
		{
			UE_LOG(LogZSharpRuntime, Warning, TEXT("Reloading Master ALC is disallowed in non-editor environment."));
			return;
		}
		
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

	static void LoadEngineAssembly(IZMasterAssemblyLoadContext* alc)
	{
		static FZUnmanagedFunction GUnmanagedFunctions[] =
        {
#define ZSHARP_UNMANAGED_FUNCTION_ASSEMBLY ZeroGames.ZSharp.UnrealEngine

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
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, IsA),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, IsIn),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealObject, Rename),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealStruct, IsChildOf),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealClass, GetDefaultObject),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealClass, IsInterface),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealClass, ImplementsInterface),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealScriptStruct, Copy),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(UnrealScriptStruct, Identical),
			
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

			ZSHARP_BUILD_UNMANAGED_FUNCTION(StrongObjectPtr, Copy),
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

			ZSHARP_BUILD_UNMANAGED_FUNCTION(Engine, GetEngine),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(StreamableManager, GetGlobalStreamableManager),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StreamableManager, RequestAsyncLoading),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(StreamingTask, GetResult),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StreamingTask, GetLoadedCount),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(StreamingTask, Release),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(EnhancedInputComponent, BindStatelessAction),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(EnhancedInputComponent, BindStatefulAction),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(EnhancedInputComponent, RemoveBinding),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(InputActionValue, Deconstruct),

#undef ZSHARP_UNMANAGED_FUNCTION_ASSEMBLY
        };

		static void** GManagedFunctions[] =
		{
			ZSHARP_BUILD_MANAGED_FUNCTION(FZStreamableManager_Interop::GUpdate),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZStreamableManager_Interop::GSignalCompletion),
		};

		static const struct
		{
			FZUnmanagedFunctions UnmanagedFunctions { UE_ARRAY_COUNT(GUnmanagedFunctions), GUnmanagedFunctions };
			void*** ManagedFunctions = GManagedFunctions;
		} GArgs{};

		alc->LoadAssembly(ZSHARP_ENGINE_ASSEMBLY_NAME, (void*)&GArgs);
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

#if DO_CHECK
	void TestParseStartupAssembly();
#endif
	
	bool ParseStartupAssembly(const FString& startupAssembly, FString& outAssemblyName, TArray<FString>& outArgs);

	void HandlePreMasterAlcStartup(ZSharp::IZMasterAssemblyLoadContext* alc);
	void HandleMasterAlcStartup(ZSharp::IZMasterAssemblyLoadContext* alc);

#if WITH_EDITOR
	void HandleBeginPIE(const bool simulating);
	void HandleEndPIE(const bool simulating);
#endif
	
};

IMPLEMENT_MODULE(FZSharpRuntimeModule, ZSharpRuntime)

void FZSharpRuntimeModule::StartupModule()
{
#if DO_CHECK
	TestParseStartupAssembly();
#endif
	
	UE_CLOG(UObjectInitialized(), LogZSharpRuntime, Fatal, TEXT("UObject system is initialized before Z#!!!"));

	ZSharp::IZSharpClr::Get().PreMasterAlcStartup().AddRaw(this, &ThisClass::HandlePreMasterAlcStartup);
	ZSharp::IZSharpClr::Get().OnMasterAlcStartup().AddRaw(this, &ThisClass::HandleMasterAlcStartup);
	
	ZSharp::FZUnrealFieldScanner::Get().Startup();

	ZSharp::ZSharpRuntimeModule_Private::CreateMasterAlc();
	
#if WITH_EDITOR
	FEditorDelegates::PreBeginPIE.AddRaw(this, &ThisClass::HandleBeginPIE);
	FEditorDelegates::EndPIE.AddRaw(this, &ThisClass::HandleEndPIE);
#endif
}

void FZSharpRuntimeModule::ShutdownModule()
{
#if WITH_EDITOR
	FEditorDelegates::PreBeginPIE.RemoveAll(this);
	FEditorDelegates::EndPIE.RemoveAll(this);
#endif

	ZSharp::ZSharpRuntimeModule_Private::UnloadMasterAlc();

	ZSharp::FZUnrealFieldScanner::Get().Shutdown();
	
	ZSharp::IZSharpClr::Get().PreMasterAlcStartup().RemoveAll(this);
	ZSharp::IZSharpClr::Get().OnMasterAlcStartup().RemoveAll(this);
}

#if DO_CHECK

void FZSharpRuntimeModule::TestParseStartupAssembly()
{
	FString assemblyName;
	TArray<FString> args;
	check(!ParseStartupAssembly("", assemblyName, args)); // error: empty
	check(!ParseStartupAssembly(".", assemblyName, args)); // error: . at start && . at end
	check(!ParseStartupAssembly(".123", assemblyName, args)); // error: . at start
	check(!ParseStartupAssembly("123.", assemblyName, args)); // error: . at end
	check(!ParseStartupAssembly("123(", assemblyName, args)); // error: parameter list not closed
	check(!ParseStartupAssembly("123.(", assemblyName, args)); // error: . at end
	check(!ParseStartupAssembly("123     (", assemblyName, args)); // error: parameter list not closed
	check(!ParseStartupAssembly("123.    a", assemblyName, args)); // error: expect (
	check(!ParseStartupAssembly("][df!!    a][", assemblyName, args)); // error: illegal assembly name
	
	check(ParseStartupAssembly("123", assemblyName, args)); // ok
	check(ParseStartupAssembly("123    ", assemblyName, args)); // ok
	check(ParseStartupAssembly("    12__3.a_bc  ", assemblyName, args)); // ok
	check(ParseStartupAssembly("   123()     ", assemblyName, args)); // ok
	check(ParseStartupAssembly("123(\"1 sd.   f;asf.er   []fs;  fer\\\"\\\\\")", assemblyName, args)); // ok
	
	check(!ParseStartupAssembly("123(1)", assemblyName, args)); // error: illegal parameter format
	check(!ParseStartupAssembly("123(\"1er[]p   f]aef   sd.f;  asf.s;fer\\\"\\\\\\abc\")", assemblyName, args)); // error: unexpected escape
	check(!ParseStartupAssembly("123(\")", assemblyName, args)); // error: parameter not closed
	
	check(ParseStartupAssembly("123(\"\")", assemblyName, args)); // ok
	check(ParseStartupAssembly("123(\"\",\"\")", assemblyName, args)); // ok
	check(ParseStartupAssembly("123(   \"\"   ,   \"\"   )", assemblyName, args)); // ok
	
	check(!ParseStartupAssembly("123(   \"\"   ,   \"\"  , )", assemblyName, args)); // error: unexpected ,
	check(!ParseStartupAssembly("123(   \"\"   ,   \"\"  ,", assemblyName, args)); // error: expect )
	check(!ParseStartupAssembly("123(   \"\"   ,   \"\"  ,  ", assemblyName, args)); // error: unexpected ,
	check(!ParseStartupAssembly("123(   \"\"   ,   \"\"  ", assemblyName, args)); // error: expect )
	check(!ParseStartupAssembly("123   \"\"   ,   \"\"  ", assemblyName, args)); // error: illegal assembly name
	check(!ParseStartupAssembly("123   \"\"   ,   \"\"  )", assemblyName, args)); // error: illegal assembly name
	check(!ParseStartupAssembly("123)", assemblyName, args)); // error: illegal assembly name
	check(!ParseStartupAssembly("123,", assemblyName, args)); // error: illegal assembly name
	check(!ParseStartupAssembly("123,)", assemblyName, args)); // error: illegal assembly name
	check(!ParseStartupAssembly("ZeroGames.ZSharp.UnrealEngine( \"ABC\"  ,   \"JIOFSDJF3894  3WJ F90R83J \"    ,   \"jf034f3f3f jifj0 j\\\\\\\\\\\\\\    \" )", assemblyName, args)); // error: unexpected escape
	
	check(ParseStartupAssembly("   ZeroGames.ZSharp.UnrealEngine       ( \"ABC\"  ,   \"JIOFSDJF3894  3WJ F90R83J \"    ,   \"jf034f3f3f jifj0 j\\\\\\\\\\\\\\\"    \" )    ", assemblyName, args)); // ok
}

#endif

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

	ZSharp::ZSharpRuntimeModule_Private::LoadEngineAssembly(alc);
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


