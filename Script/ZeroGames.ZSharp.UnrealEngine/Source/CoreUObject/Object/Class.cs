// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Reflection;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UClass
{
	
	public new static UClass FromType(Type type)
	{
		PropertyInfo? staticUnrealFieldProperty = null;
		if (type.IsAssignableTo(typeof(IUnrealObject)))
		{
			staticUnrealFieldProperty = type.GetProperty(nameof(IStaticClass.StaticClass), BindingFlags.Public | BindingFlags.Static);
		}

		if (staticUnrealFieldProperty is null)
		{
			throw new ArgumentOutOfRangeException($"Type {type.FullName} is not a valid unreal field.");
		}
		
		return (UClass)staticUnrealFieldProperty.GetValue(null)!;
	}
	public new static UClass FromType<T>() where T : IUnrealObject => FromType(typeof(T));

	public bool TryGetDefaultObject([NotNullWhen(true)] out UObject? result)
	{
		MasterAlcCache.GuardInvariant();
		if (IsInterface)
		{
			throw new InvalidOperationException();
		}
		
		result = InternalGetDefaultObject(false);
		return result is not null;
	}

	public UObject GetDefaultObject()
	{
		MasterAlcCache.GuardInvariant();
		if (IsInterface)
		{
			throw new InvalidOperationException();
		}
		
		return InternalGetDefaultObject(true)!;
	}

	public bool ImplementsInterface(UClass @interface)
	{
		MasterAlcCache.GuardInvariant();
		if (!@interface.IsInterface)
		{
			throw new ArgumentOutOfRangeException(nameof(@interface));
		}
		
		return InternalImplementsInterface(@interface);
	}
	public bool ImplementsInterface(Type @interface) => ImplementsInterface(FromType(@interface));
	public bool ImplementsInterface<T>() where T : IInterface => ImplementsInterface(typeof(T));
	
	public bool IsInterface
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsInterface;
		}
	}
	
	private unsafe UObject? InternalGetDefaultObject(bool createIfNeeded) => UnrealClass_Interop.GetDefaultObject(ConjugateHandle.FromConjugate(this), Convert.ToByte(createIfNeeded)).GetTarget<UObject>();
	private unsafe bool InternalImplementsInterface(UClass other) => UnrealClass_Interop.ImplementsInterface(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;

	private unsafe bool InternalIsInterface => UnrealClass_Interop.IsInterface(ConjugateHandle.FromConjugate(this)) > 0;

}


