// Copyright Zero Games. All Rights Reserved.

#if UE_MODULE_ENHANCEDINPUT

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.EnhancedInput;

public partial class EnhancedInputComponent
{

	public UnrealObject BindAction(InputAction inputAction, ETriggerEvent triggerEvent, EnhancedInputActionHandlerDynamicSignature.Signature @delegate)
	{
		MasterAlcCache.GuardInvariant();
		return InternalBindAction(inputAction, triggerEvent, @delegate);
	}
	
	public UnrealObject BindAction<TState>(InputAction inputAction, ETriggerEvent triggerEvent, EnhancedInputActionHandlerDynamicSignature.Signature<TState> @delegate, TState state)
	{
		MasterAlcCache.GuardInvariant();
		return InternalBindAction(inputAction, triggerEvent, @delegate, state);
	}

	public bool RemoveBinding(UnrealObject handle)
	{
		MasterAlcCache.GuardInvariant();
		return InternalRemoveBinding(handle);
	}

	private unsafe UnrealObject InternalBindAction(InputAction action, ETriggerEvent triggerEvent, EnhancedInputActionHandlerDynamicSignature.Signature @delegate)
		=> EnhancedInputComponent_Interop.BindStatelessAction(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(action), (int64)triggerEvent, GCHandle.Alloc(@delegate)).GetTargetChecked<UnrealObject>();
	
	private unsafe UnrealObject InternalBindAction<TState>(InputAction action, ETriggerEvent triggerEvent, EnhancedInputActionHandlerDynamicSignature.Signature<TState> @delegate, TState state)
		=> EnhancedInputComponent_Interop.BindStatefulAction(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(action), (int64)triggerEvent, GCHandle.Alloc(@delegate), GCHandle.Alloc(state)).GetTargetChecked<UnrealObject>();
	
	private unsafe bool InternalRemoveBinding(UnrealObject handle) => EnhancedInputComponent_Interop.RemoveBinding(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(handle)) > 0;

}

#endif


