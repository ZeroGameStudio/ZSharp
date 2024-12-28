// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(34)]
[ConjugateKey("Unreal.Optional")]
public sealed class UnrealOptional<T> : UnrealConjugateBase
	, IConjugate<UnrealOptional<T>>
	, ICloneable<UnrealOptional<T>>
{
	
	public static UnrealOptional<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealOptional()
	{
		if (!PropertyHelper.CanBeValue(typeof(T)))
		{
			Unmanaged = DEAD_ADDR;
			GC.SuppressFinalize(this);
			throw new NotSupportedException($"Element type {typeof(T).FullName} is not supported.");
		}
		
		InternalConstruct();
	}

	public UnrealOptional(T value) : this() => Set(value);

	public UnrealOptional<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool TryGetValue([MaybeNullWhen(false)] out T value)
	{
		MasterAlcCache.GuardInvariant();
		return InternalTryGetValue(out value);
	}

	public void Set(T value)
	{
		MasterAlcCache.GuardInvariant();
		InternalSet(value);
	}

	public void Reset()
	{
		MasterAlcCache.GuardInvariant();
		InternalReset();
	}

	public static implicit operator UnrealOptional<T>(T value) => new(value);

	public bool IsSet
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsSet;
		}
	}

	[StructLayout(LayoutKind.Sequential)]
	private struct Userdata
	{
		public PropertyDesc ElementProperty;
	}
	
	private UnrealOptional(IntPtr unmanaged) : base(unmanaged){}

	private UnrealOptional(UnrealOptional<T>? other) : this()
	{
		MasterAlcCache.GuardInvariant();
		if (other is not null && other.TryGetValue(out var value))
		{
			Set(value);
		}
	}
	
	private unsafe void InternalConstruct()
	{
		Userdata userdata = new();
		Userdata* pUserdata = &userdata;
		PropertyHelper.TryGetPropertyDesc(typeof(T), out userdata.ElementProperty);
		
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, (IntPtr)pUserdata);
	}

	private unsafe bool InternalTryGetValue([MaybeNullWhen(false)] out T value)
	{
		ZCallBufferSlot result = ZCallBufferSlot.FromType(typeof(T));
		if (UnrealOptional_Interop.Get(ConjugateHandle.FromConjugate(this), ref result) > 0)
		{
			value = (T)result.Object!;
			return true;
		}
		
		value = default;
		return false;
	}

	private unsafe void InternalSet(T value) => UnrealOptional_Interop.Set(ConjugateHandle.FromConjugate(this), ZCallBufferSlot.FromObject(value));
	private unsafe void InternalReset() => UnrealOptional_Interop.Reset(ConjugateHandle.FromConjugate(this));
	
	private unsafe bool InternalIsSet => UnrealOptional_Interop.IsSet(ConjugateHandle.FromConjugate(this)) > 0;
	
}


