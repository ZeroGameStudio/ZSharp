﻿// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(26)]
[ConjugateKey("Unreal.ScriptInterface")]
public sealed class ScriptInterface<T> : ScriptInterfaceBase
	, IConjugate<ScriptInterface<T>>
	, ICloneable<ScriptInterface<T>>
	, IEquatable<ScriptInterface<T>>
	, IEqualityOperators<ScriptInterface<T>?, ScriptInterface<T>?, bool>
	, IUnrealObjectWrapper<UnrealObject>
	where T : IUnrealInterface
{

	public static ScriptInterface<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static ScriptInterface<T> From<TSource>(ScriptInterface<TSource> other) where TSource : T => new(other);

	public ScriptInterface() => BuildConjugate_Black(UnrealClass.FromType<T>().Unmanaged);
	
	public ScriptInterface(UnrealObject? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public ScriptInterface<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(ScriptInterface<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();
	
	public static implicit operator ScriptInterface<T>(UnrealObject? target) => new(target);
	public static bool operator ==(ScriptInterface<T>? left, ScriptInterface<T>? right) => Equals(left, right);
	public static bool operator !=(ScriptInterface<T>? left, ScriptInterface<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<ScriptInterface<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	public UnrealObject? Target
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalGet(false);
		}
		set
		{
			MasterAlcCache.GuardInvariant();
			InternalSet(value);
		}
	}

	public UnrealObject? TargetEvenIfGarbage
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalGet(true);
		}
	}

	public bool IsValid
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsValid(false);
		}
	}

	public bool IsValidEventIfGarbage
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsValid(true);
		}
	}

	public bool IsNull
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsNull();
		}
	}

	private sealed class EqualityComparer : IEqualityComparer<ScriptInterface<T>>
	{
		public bool Equals(ScriptInterface<T>? lhs, ScriptInterface<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(ScriptInterface<T> obj) => obj.GetHashCode();
	}

	private ScriptInterface(IntPtr unmanaged) : base(unmanaged){}
	private ScriptInterface(ScriptInterfaceBase? other) : this() => InternalCopy(other);

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

	private unsafe UnrealObject? InternalGet(bool evenIfGarbage)
		=> ScriptInterface_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<UnrealObject>();

	private unsafe void InternalSet(UnrealObject? target)
	{
		if (target is not null && !target.Implements<T>())
		{
			throw new ArgumentOutOfRangeException($"Target type {target.GetType().FullName} mismatch.");
		}
		
		ScriptInterface_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(target));
	}
	
	private unsafe bool InternalIsValid(bool evenIfGarbage)
		=> ScriptInterface_Interop.IsValid(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)) > 0;

	private unsafe bool InternalIsNull()
		=> ScriptInterface_Interop.IsNull(ConjugateHandle.FromConjugate(this)) > 0;
	
}


