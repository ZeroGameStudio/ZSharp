// Copyright Zero Games. All Rights Reserved.



// Migrate from ScriptCore.cpp.
// Original execCallMathFunction crashes execZCall so we have to do this low level hack.
#if PER_FUNCTION_SCRIPT_STATS
static int32 GMaxFunctionStatDepth = MAX_uint8;
static FAutoConsoleVariableRef CVarMaxFunctionStatDepth(
	TEXT("bp.MaxFunctionStatDepth"),
	GMaxFunctionStatDepth,
	TEXT("Script stack threshold for recording per function stats.\n"),
	ECVF_Default
);
#endif

static DEFINE_FUNCTION(execCallMathFunction)
{
	UFunction* Function = (UFunction*)Stack.ReadObject();
	checkSlow(Function);
	checkSlow(Function->FunctionFlags & FUNC_Native);
	// ProcessContext is the arbiter of net callspace, so we can't call net functions using this instruction:
	checkSlow(!Function->HasAnyFunctionFlags(FUNC_NetFuncFlags|FUNC_BlueprintAuthorityOnly|FUNC_BlueprintCosmetic|FUNC_NetRequest|FUNC_NetResponse));
	UObject* NewContext = Function->GetOuterUClassUnchecked()->ClassDefaultObject;
	checkSlow(NewContext);
	{
#if PER_FUNCTION_SCRIPT_STATS
		const bool bShouldTrackFunction = Stack.DepthCounter <= GMaxFunctionStatDepth;
		FScopeCycleCounterUObject FunctionScope(bShouldTrackFunction ? Function : nullptr);
#endif // PER_FUNCTION_SCRIPT_STATS

		// Begin Z# MODIFY
		
		// CurrentNativeFunction is used by execZCall.
		TGuardValue<UFunction*> NativeFuncGuard(Stack.CurrentNativeFunction, Function);

		// END Z# MODIFY
		
		FNativeFuncPtr Func = Function->GetNativeFunc();
		checkSlow(Func);
		(*Func)(NewContext, Stack, RESULT_PARAM);
	}
}

#if IS_MONOLITHIC
#define CONDITIONAL_DLLIMPORT
#else
#define CONDITIONAL_DLLIMPORT DLLIMPORT
#endif

CONDITIONAL_DLLIMPORT extern FNativeFuncPtr GNatives[EX_Max];

struct FZHookCallMathBytecode
{
	FZHookCallMathBytecode()
	{
		FCoreDelegates::OnInit.AddLambda([]
		{
			GNatives[EX_CallMath] = execCallMathFunction;
			FNativeFunctionRegistrar { UObject::StaticClass(), "execCallMathFunction", execCallMathFunction };
		});
	}
} GHookCallMathBytecode{};


