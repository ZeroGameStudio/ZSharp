// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class StrongObjectPtrBase : IDisposable, IEquatable<StrongObjectPtrBase>, IEqualityOperators<StrongObjectPtrBase?, StrongObjectPtrBase?, bool>
{
	
	public void Dispose()
	{
		Thrower.ThrowIfNotInGameThread();

		InternalDispose();
		
		GC.SuppressFinalize(this);
	}
	
	public bool Equals(StrongObjectPtrBase? other) => ReferenceEquals(this, other) || InternalEquals(other);
	public override bool Equals(object? obj) => obj is StrongObjectPtrBase other && Equals(other);
	public override int32 GetHashCode() => InternalGetHashCode();
	
	public static bool operator ==(StrongObjectPtrBase? left, StrongObjectPtrBase? right) => Equals(left, right);
	public static bool operator !=(StrongObjectPtrBase? left, StrongObjectPtrBase? right) => !Equals(left, right);

	protected StrongObjectPtrBase(UnrealObject? target) => InternalAlloc(target);
	
	protected IntPtr Unmanaged { get; private set; }
	
	~StrongObjectPtrBase() => InternalDispose();

	private unsafe void InternalAlloc(UnrealObject? target)
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
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return other is not null && StrongObjectPtr_Interop.Identical(Unmanaged, other.Unmanaged) > 0;
	}
	
	private unsafe int32 InternalGetHashCode()
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return StrongObjectPtr_Interop.Hash(Unmanaged);
	}

}