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
			res &= type.GetCustomAttribute<HashableAttribute>() is not null;
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
		if (_typeMap.TryGetValue(type, out id))
		{
			desc.Descriptor = id;
			return true;
		}
		
		// UClasses
		if (type.IsAssignableTo(typeof(UnrealObjectBase)))
		{
			desc.Descriptor = UnrealObjectGlobals.GetClass(type).Unmanaged;
			return true;
		}
		
		// UStructs
		if (type.IsAssignableTo(typeof(UnrealStructBase)))
		{
			desc.Descriptor = ((UnrealScriptStruct)type.GetProperty(nameof(IStaticStruct.StaticStruct))!.GetValue(null)!).Unmanaged;
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
			desc.Descriptor = SUBCLASS_OF_TYPE_ID;
			desc.Metadata = UnrealObjectGlobals.GetClass(type.GetGenericArguments()[0]).Unmanaged;
			return true;
		}
		if (type.IsAssignableTo(typeof(SoftClassPtrBase)))
		{
			desc.Descriptor = SOFT_CLASS_PTR_TYPE_ID;
			desc.Metadata = UnrealObjectGlobals.GetClass(type.GetGenericArguments()[0]).Unmanaged;
			return true;
		}
		if (type.IsAssignableTo(typeof(SoftObjectPtrBase)))
		{
			desc.Descriptor = SOFT_OBJECT_PTR_TYPE_ID;
			desc.Metadata = UnrealObjectGlobals.GetClass(type.GetGenericArguments()[0]).Unmanaged;
			return true;
		}
		if (type.IsAssignableTo(typeof(WeakObjectPtrBase)))
		{
			desc.Descriptor = WEAK_OBJECT_PTR_TYPE_ID;
			desc.Metadata = UnrealObjectGlobals.GetClass(type.GetGenericArguments()[0]).Unmanaged;
			return true;
		}
		if (type.IsAssignableTo(typeof(LazyObjectPtrBase)))
		{
			desc.Descriptor = LAZY_OBJECT_PTR_TYPE_ID;
			desc.Metadata = UnrealObjectGlobals.GetClass(type.GetGenericArguments()[0]).Unmanaged;
			return true;
		}
		if (type.IsAssignableTo(typeof(ScriptInterfaceBase)))
		{
			desc.Descriptor = SCRIPT_INTERFACE_TYPE_ID;
			desc.Metadata = UnrealObjectGlobals.GetClass(type.GetGenericArguments()[0]).Unmanaged;
			return true;
		}
		
		// 2 delegates.
		if (type.IsAssignableTo(typeof(UnrealDelegateBase)) || type.IsAssignableTo(typeof(UnrealMulticastInlineDelegateBase)))
		{
			desc.Descriptor = UnrealDelegateBase.GetUnrealDelegateSignature(type).Unmanaged;
		}
		
		return false;
	}
	
	// IMPORTANT: KEEP SYNC WITH ZPropertyFactory.cpp
	private const IntPtr UINT8_TYPE_ID = 1;
	private const IntPtr UINT16_TYPE_ID = 2;
	private const IntPtr UINT32_TYPE_ID = 3;
	private const IntPtr UINT64_TYPE_ID = 4;
	private const IntPtr INT8_TYPE_ID = 5;
	private const IntPtr INT16_TYPE_ID = 6;
	private const IntPtr INT32_TYPE_ID = 7;
	private const IntPtr INT64_TYPE_ID = 8;
	private const IntPtr FLOAT_TYPE_ID = 9;
	private const IntPtr DOUBLE_TYPE_ID = 10;
	private const IntPtr BOOL_TYPE_ID = 11;

	private const IntPtr STRING_TYPE_ID = 12;
	private const IntPtr NAME_TYPE_ID = 13;
	private const IntPtr TEXT_TYPE_ID = 14;

	private const IntPtr SUBCLASS_OF_TYPE_ID = 15;
	private const IntPtr SOFT_CLASS_PTR_TYPE_ID = 16;
	private const IntPtr SOFT_OBJECT_PTR_TYPE_ID = 17;
	private const IntPtr WEAK_OBJECT_PTR_TYPE_ID = 18;
	private const IntPtr LAZY_OBJECT_PTR_TYPE_ID = 19;
	private const IntPtr SCRIPT_INTERFACE_TYPE_ID = 20;

	private const IntPtr FIELD_PATH_TYPE_ID = 21;

	private static Dictionary<Type, IntPtr> _typeMap => new()
	{
		{ typeof(uint8), UINT8_TYPE_ID },
		{ typeof(uint16), UINT16_TYPE_ID },
		{ typeof(uint32), UINT32_TYPE_ID },
		{ typeof(uint64), UINT64_TYPE_ID },
		{ typeof(int8), INT8_TYPE_ID },
		{ typeof(int16), INT16_TYPE_ID },
		{ typeof(int32), INT32_TYPE_ID },
		{ typeof(int64), INT64_TYPE_ID },
		{ typeof(float), FLOAT_TYPE_ID },
		{ typeof(double), DOUBLE_TYPE_ID },
		{ typeof(bool), BOOL_TYPE_ID },
		
		{ typeof(UnrealString), STRING_TYPE_ID },
		{ typeof(UnrealName), NAME_TYPE_ID },
		{ typeof(UnrealText), TEXT_TYPE_ID },
		
		{ typeof(FieldPath), FIELD_PATH_TYPE_ID },
	};

}


