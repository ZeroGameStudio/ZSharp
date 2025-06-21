// Copyright Zero Games. All Rights Reserved.

#if UE_MODULE_ENHANCEDINPUT

using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.EnhancedInput;

public readonly record struct EnhancedInputBinding
{
	public bool IsValid => Handle > 0;
	
	internal EnhancedInputBinding(uint32 handle)
	{
		Handle = handle;
	}
	
	internal uint32 Handle { get; }
}

public partial class UEnhancedInputComponent
{

	public EnhancedInputBinding BindAction(UInputAction? inputAction, ETriggerEvent triggerEvent, UObject? obj, string functionName)
	{
		if (inputAction is null || obj is null || string.IsNullOrWhiteSpace(functionName))
		{
			return default;
		}
		
		MasterAlcCache.GuardInvariant();
		return new(InternalBindAction(inputAction, triggerEvent, obj, functionName));
	}

	[return: NotNullIfNotNull(nameof(inputAction))]
	public UObject? BindAction(UInputAction? inputAction, ETriggerEvent triggerEvent, FEnhancedInputActionHandlerDynamicSignature.Signature @delegate)
	{
		MasterAlcCache.GuardInvariant();
		return inputAction is not null ? InternalBindAction(inputAction, triggerEvent, @delegate) : null;
	}
	
	[return: NotNullIfNotNull(nameof(inputAction))]
	public UObject? BindAction<TState>(UInputAction? inputAction, ETriggerEvent triggerEvent, FEnhancedInputActionHandlerDynamicSignature.Signature<TState> @delegate, TState state)
	{
		MasterAlcCache.GuardInvariant();
		return inputAction is not null ? InternalBindAction(inputAction, triggerEvent, @delegate, state) : null;
	}

	public bool RemoveBinding(EnhancedInputBinding handle)
	{
		MasterAlcCache.GuardInvariant();
		return handle.IsValid && InternalRemoveBinding(handle.Handle);
	}

	public bool RemoveBinding(UObject? handle)
	{
		MasterAlcCache.GuardInvariant();
		return handle.IsValid() && InternalRemoveBinding(handle);
	}

	private unsafe uint32 InternalBindAction(UInputAction action, ETriggerEvent triggerEvent, UObject obj, string name)
	{
		fixed (char* nameBuffer = name)
		{
			return EnhancedInputComponent_Interop.BindUFunctionAction(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(action), (int64)triggerEvent, ConjugateHandle.FromConjugate(obj), nameBuffer);
		}
	}

	private unsafe UObject InternalBindAction(UInputAction action, ETriggerEvent triggerEvent, FEnhancedInputActionHandlerDynamicSignature.Signature @delegate)
		=> EnhancedInputComponent_Interop.BindStatelessAction(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(action), (int64)triggerEvent, GCHandle.Alloc(@delegate)).GetTargetChecked<UObject>();
	
	private unsafe UObject InternalBindAction<TState>(UInputAction action, ETriggerEvent triggerEvent, FEnhancedInputActionHandlerDynamicSignature.Signature<TState> @delegate, TState state)
		=> EnhancedInputComponent_Interop.BindStatefulAction(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(action), (int64)triggerEvent, GCHandle.Alloc(@delegate), GCHandle.Alloc(state)).GetTargetChecked<UObject>();

	private unsafe bool InternalRemoveBinding(uint32 handle)
		=> EnhancedInputComponent_Interop.RemoveBindingByHandle(ConjugateHandle.FromConjugate(this), handle) > 0;
	
	private unsafe bool InternalRemoveBinding(UObject handle)
		=> EnhancedInputComponent_Interop.RemoveBinding(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(handle)) > 0;

}

#endif


