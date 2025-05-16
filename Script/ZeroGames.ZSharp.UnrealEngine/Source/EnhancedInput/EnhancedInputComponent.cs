// Copyright Zero Games. All Rights Reserved.

#if UE_MODULE_ENHANCEDINPUT

using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.EnhancedInput;

public partial class UEnhancedInputComponent
{

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

	public bool RemoveBinding(UObject? handle)
	{
		MasterAlcCache.GuardInvariant();
		return handle.IsValid() && InternalRemoveBinding(handle);
	}

	private unsafe UObject InternalBindAction(UInputAction action, ETriggerEvent triggerEvent, FEnhancedInputActionHandlerDynamicSignature.Signature @delegate)
		=> EnhancedInputComponent_Interop.BindStatelessAction(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(action), (int64)triggerEvent, GCHandle.Alloc(@delegate)).GetTargetChecked<UObject>();
	
	private unsafe UObject InternalBindAction<TState>(UInputAction action, ETriggerEvent triggerEvent, FEnhancedInputActionHandlerDynamicSignature.Signature<TState> @delegate, TState state)
		=> EnhancedInputComponent_Interop.BindStatefulAction(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(action), (int64)triggerEvent, GCHandle.Alloc(@delegate), GCHandle.Alloc(state)).GetTargetChecked<UObject>();
	
	private unsafe bool InternalRemoveBinding(UObject handle) => EnhancedInputComponent_Interop.RemoveBinding(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(handle)) > 0;

}

#endif


