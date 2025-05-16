// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(26)]
[ConjugateKey("Unreal.ScriptInterface")]
public sealed class TScriptInterface<T> : ScriptInterfaceBase
	, IConjugate<TScriptInterface<T>>
	, ICloneable<TScriptInterface<T>>
	, IEquatable<TScriptInterface<T>>
	, IEqualityOperators<TScriptInterface<T>?, TScriptInterface<T>?, bool>
	, IUnrealObjectWrapper<UObject>
	where T : IInterface
{

	public static TScriptInterface<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static TScriptInterface<T> From<TSource>(TScriptInterface<TSource> other) where TSource : T => new(other);

	public TScriptInterface() => BuildConjugate_Black(UClass.FromType<T>().Unmanaged);
	
	public TScriptInterface(UObject? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public TScriptInterface<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(TScriptInterface<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();
	
	public static implicit operator TScriptInterface<T>(UObject? target) => new(target);
	public static bool operator ==(TScriptInterface<T>? left, TScriptInterface<T>? right) => Equals(left, right);
	public static bool operator !=(TScriptInterface<T>? left, TScriptInterface<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<TScriptInterface<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	protected override UObject? InternalGetTarget(bool evenIfGarbage)
	{
		MasterAlcCache.GuardInvariant();
		return InternalGet(evenIfGarbage);
	}

	protected override void InternalSetTarget(UObject? target)
	{
		MasterAlcCache.GuardInvariant();
		InternalSet(target);
	}

	private sealed class EqualityComparer : IEqualityComparer<TScriptInterface<T>>
	{
		public bool Equals(TScriptInterface<T>? lhs, TScriptInterface<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(TScriptInterface<T> obj) => obj.GetHashCode();
	}

	private TScriptInterface(IntPtr unmanaged) : base(unmanaged){}
	private TScriptInterface(ScriptInterfaceBase? other) : this() => InternalCopy(other);

	private unsafe void InternalCopy(ScriptInterfaceBase? other)
	{
		if (other is not null)
		{
			ScriptInterface_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
		}
		else
		{
			Target = null;
		}
	}

	private unsafe UObject? InternalGet(bool evenIfGarbage)
		=> ScriptInterface_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<UObject>();

	private unsafe void InternalSet(UObject? target)
	{
		if (target is not null && !target.Implements<T>())
		{
			throw new ArgumentOutOfRangeException($"Target type {target.GetType().FullName} mismatch.");
		}
		
		ScriptInterface_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(target));
	}
	
}


