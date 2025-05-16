// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// StrongObjectPtr is not conjugate so can't inherit from UnrealObjectWrapperBase.
public abstract class StrongObjectPtrBase
	: IDisposable
	, IEquatable<StrongObjectPtrBase>
	, IEqualityOperators<StrongObjectPtrBase?, StrongObjectPtrBase?, bool>
	, IUnrealObjectWrapper
{
	
	public void Dispose()
	{
		Thrower.ThrowIfNotInGameThread();

		InternalDispose();
		
		GC.SuppressFinalize(this);
	}

	public bool Equals(StrongObjectPtrBase? other)
	{
		MasterAlcCache.GuardInvariant();
		return ReferenceEquals(this, other) || InternalEquals(other);
	}
	public override bool Equals(object? obj) => obj is StrongObjectPtrBase other && Equals(other);
	
	public override int32 GetHashCode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetHashCode();
	}
	
	public static bool operator ==(StrongObjectPtrBase? left, StrongObjectPtrBase? right) => Equals(left, right);
	public static bool operator !=(StrongObjectPtrBase? left, StrongObjectPtrBase? right) => !Equals(left, right);
	
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
	
	public UObject? Target
	{
		get => InternalGetTarget(false);
		set => InternalSetTarget(value);
	}
	public UObject? TargetEvenIfGarbage => InternalGetTarget(true);

	protected StrongObjectPtrBase(UObject? target) => InternalAlloc(target);
	
	protected abstract UObject? InternalGetTarget(bool evenIfGarbage);
	protected abstract void InternalSetTarget(UObject? target);
	
	protected IntPtr Unmanaged { get; private set; }
	
	~StrongObjectPtrBase() => InternalDispose();

	private unsafe void InternalAlloc(UObject? target)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.GuardInvariant();
		Unmanaged = StrongObjectPtr_Interop.Alloc(ConjugateHandle.FromConjugate(target));
	}

	private unsafe void InternalDispose()
	{
		if (Unmanaged == IntPtr.Zero)
		{
			return;
		}

		IntPtr unmanaged = Unmanaged;
		Unmanaged = IntPtr.Zero;

		if (IsInGameThread)
		{
			StrongObjectPtr_Interop.Free(unmanaged);
		}
		else
		{
			IGameThreadScheduler.Instance.Post(state => StrongObjectPtr_Interop.Free((IntPtr)state!), unmanaged);
		}
	}

	private unsafe bool InternalEquals(StrongObjectPtrBase? other)
		=> other is not null && StrongObjectPtr_Interop.Identical(Unmanaged, other.Unmanaged) > 0;
	
	private unsafe int32 InternalGetHashCode() => StrongObjectPtr_Interop.Hash(Unmanaged);
	
	private unsafe bool InternalIsValid(bool evenIfGarbage)
		=> StrongObjectPtr_Interop.IsValid(Unmanaged, Convert.ToByte(evenIfGarbage)) > 0;
	
	private unsafe bool InternalIsNull()
		=> StrongObjectPtr_Interop.IsNull(Unmanaged) > 0;

}