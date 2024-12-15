// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.EnhancedInput;

public partial class InputActionValue
{

	public void Deconstruct(out double x, out double y, out double z)
	{
		MasterAlcCache.GuardInvariant();
		InternalDeconstruct(out x, out y, out z);
	}
	public void Deconstruct(out double x, out double y) => Deconstruct(out x, out y, out _);

	public static implicit operator double(InputActionValue inputActionValue)
	{
		inputActionValue.Deconstruct(out var x, out _, out _);
		return x;
	}
	public static implicit operator bool(InputActionValue inputActionValue) => inputActionValue > ZERO_TOLERANCE;

	private unsafe void InternalDeconstruct(out double x, out double y, out double z) => InputActionValue_Interop.Deconstruct(ConjugateHandle.FromConjugate(this), out x, out y, out z);

	private const double ZERO_TOLERANCE = 1e-2;

}


