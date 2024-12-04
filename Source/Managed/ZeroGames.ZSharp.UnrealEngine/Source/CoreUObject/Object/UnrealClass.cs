// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public partial class UnrealClass
{
	
	public new static UnrealClass FromType(Type type)
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
		
		return (UnrealClass)staticUnrealFieldProperty.GetValue(null)!;
	}
	public new static UnrealClass FromType<T>() where T : IUnrealObject => FromType(typeof(T));

	public UnrealObject GetDefaultObject(bool createIfNeeded)
	{
		MasterAlcCache.GuardInvariant();
		if (IsInterface)
		{
			throw new InvalidOperationException();
		}
		
		return InternalGetDefaultObject(createIfNeeded);
	}

	public bool ImplementsInterface(UnrealClass @interface)
	{
		MasterAlcCache.GuardInvariant();
		if (!@interface.IsInterface)
		{
			throw new ArgumentOutOfRangeException(nameof(@interface));
		}
		
		return InternalImplementsInterface(@interface);
	}
	public bool ImplementsInterface(Type @interface) => ImplementsInterface(FromType(@interface));
	public bool ImplementsInterface<T>() where T : IUnrealInterface => ImplementsInterface(typeof(T));
	
	public bool IsInterface
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsInterface;
		}
	}
	
	private unsafe UnrealObject InternalGetDefaultObject(bool createIfNeeded) => UnrealClass_Interop.GetDefaultObject(ConjugateHandle.FromConjugate(this), Convert.ToByte(createIfNeeded)).GetTargetChecked<UnrealObject>();
	private unsafe bool InternalImplementsInterface(UnrealClass other) => UnrealClass_Interop.ImplementsInterface(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;

	private unsafe bool InternalIsInterface => UnrealClass_Interop.IsInterface(ConjugateHandle.FromConjugate(this)) > 0;

}


