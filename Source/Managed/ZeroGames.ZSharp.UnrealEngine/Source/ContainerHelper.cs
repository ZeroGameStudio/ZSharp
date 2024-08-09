// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.UnrealEngine;

internal static class ContainerHelper
{

	public static bool CanBeKey(Type type)
	{
		// Text is not hashable.
		if (type == typeof(UnrealText))
		{
			return false;
		}
		
		// Delegates is hashable but not supported by unreal reflection system.
		if (type.IsAssignableTo(typeof(UnrealDelegateBase))
		    || type.IsAssignableTo(typeof(UnrealMulticastInlineDelegateBase))
		    || type.IsAssignableTo(typeof(UnrealMulticastSparseDelegateBase)))
		{
			return false;
		}
		
		bool res = CanBeValue(type);
		if (type.IsAssignableTo(typeof(UnrealStructBase)))
		{
			res &= type.GetCustomAttribute<HashableUnrealStructAttribute>() is not null;
		}

		return res;
	}

	public static bool CanBeValue(Type type)
	{
		// 8 integers + 2 floats + 1 boolean
		if (type.IsPrimitive)
		{
			return true;
		}

		// 3 strings
		if (type == typeof(UnrealString)
		    || type == typeof(UnrealName)
		    || type == typeof(UnrealText))
		{
			return true;
		}
        
		// UClasses + UStructs + UEnums
		if (type.IsAssignableTo(typeof(UnrealObjectBase))
		    || type.IsAssignableTo(typeof(UnrealStructBase))
		    || UnrealEnum.IsUnrealEnumType(type))
		{
			return true;
		}
		
		// 6 UObject wrappers
		if (type.IsAssignableTo(typeof(SubclassOfBase))
		    || type.IsAssignableTo(typeof(SoftClassPtrBase))
		    || type.IsAssignableTo(typeof(SoftObjectPtrBase))
		    || type.IsAssignableTo(typeof(WeakObjectPtrBase))
		    || type.IsAssignableTo(typeof(LazyObjectPtrBase))
		    || type.IsAssignableTo(typeof(ScriptInterfaceBase)))
		{
			return true;
		}
		
		// 2 delegates (sparse delegate is not supported)
		if (type.IsAssignableTo(typeof(UnrealDelegateBase)) || type.IsAssignableTo(typeof(UnrealMulticastInlineDelegateBase)))
		{
			return true;
		}
		
		// Special types
		if (type == typeof(FieldPath))
		{
			return true;
		}

		return false;
	}

	public static bool TryGetPropertyDesc(Type type, out PropertyDesc desc)
	{
		desc = default;
		
		// 8 integers + 2 floats + 1 boolean + 3 strings + special types
		IntPtr id;
		if (IntrinsicTypeIds.STypeMap.TryGetValue(type, out id))
		{
			desc.Descriptor = id;
			return true;
		}
		
		// UClasses
		if (type.IsAssignableTo(typeof(UnrealObjectBase)))
		{
			desc.Descriptor = ((UnrealClass)type.GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!).Unmanaged;
			return true;
		}
		
		// UStructs
		if (type.IsAssignableTo(typeof(UnrealStructBase)))
		{
			desc.Descriptor = ((UnrealScriptStruct)type.GetProperty(nameof(IStaticStruct.SStaticStruct))!.GetValue(null)!).Unmanaged;
			return true;
		}
		
		// UEnums
		if (UnrealEnum.IsUnrealEnumType(type))
		{
			desc.Descriptor = UnrealEnum.GetUnrealEnum(type).Unmanaged;
			return true;
		}
		
		// 6 UObject wrappers
		if (type.IsAssignableTo(typeof(SubclassOfBase)))
		{
			desc.Descriptor = IntrinsicTypeIds.KSubclassOf;
			desc.Metadata = ((UnrealClass)type.GetGenericArguments()[0].GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!).Unmanaged;
			return true;
		}
		if (type.IsAssignableTo(typeof(SubclassOfBase)))
		{
			desc.Descriptor = IntrinsicTypeIds.KSubclassOf;
			desc.Metadata = ((UnrealClass)type.GetGenericArguments()[0].GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!).Unmanaged;
			return true;
		}
		if (type.IsAssignableTo(typeof(SubclassOfBase)))
		{
			desc.Descriptor = IntrinsicTypeIds.KSubclassOf;
			desc.Metadata = ((UnrealClass)type.GetGenericArguments()[0].GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!).Unmanaged;
			return true;
		}
		if (type.IsAssignableTo(typeof(SubclassOfBase)))
		{
			desc.Descriptor = IntrinsicTypeIds.KSubclassOf;
			desc.Metadata = ((UnrealClass)type.GetGenericArguments()[0].GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!).Unmanaged;
			return true;
		}
		if (type.IsAssignableTo(typeof(SubclassOfBase)))
		{
			desc.Descriptor = IntrinsicTypeIds.KSubclassOf;
			desc.Metadata = ((UnrealClass)type.GetGenericArguments()[0].GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!).Unmanaged;
			return true;
		}
		if (type.IsAssignableTo(typeof(SubclassOfBase)))
		{
			desc.Descriptor = IntrinsicTypeIds.KSubclassOf;
			desc.Metadata = ((UnrealClass)type.GetGenericArguments()[0].GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!).Unmanaged;
			return true;
		}
		
		// 2 delegates.
		return false;
	}

}


