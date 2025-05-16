// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class UnrealObjectWrapperBase : UnrealConjugateBase, IUnrealObjectWrapper
{
	
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
	
	protected UnrealObjectWrapperBase(){}
	protected UnrealObjectWrapperBase(IntPtr unmanaged) : base(unmanaged){}

	protected abstract bool InternalIsValid(bool evenIfGarbage);
	protected abstract bool InternalIsNull();
	protected abstract UObject? InternalGetTarget(bool evenIfGarbage);
	protected abstract void InternalSetTarget(UObject? target);
	
}


