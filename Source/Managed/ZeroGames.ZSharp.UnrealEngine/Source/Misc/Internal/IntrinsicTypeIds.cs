// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static class IntrinsicTypeIds
{
	// NOTE: KEEP SYNC WITH ZPropertyFactory.cpp
	public const IntPtr KUInt8 = 1;
	public const IntPtr KUInt16 = 2;
	public const IntPtr KUInt32 = 3;
	public const IntPtr KUInt64 = 4;
	public const IntPtr KInt8 = 5;
	public const IntPtr KInt16 = 6;
	public const IntPtr KInt32 = 7;
	public const IntPtr KInt64 = 8;
	public const IntPtr KFloat = 9;
	public const IntPtr KDouble = 10;
	public const IntPtr KBool = 11;

	public const IntPtr KString = 12;
	public const IntPtr KName = 13;
	public const IntPtr KText = 14;

	public const IntPtr KSubclassOf = 15;
	public const IntPtr KSoftClassPtr = 16;
	public const IntPtr KSoftObjectPtr = 17;
	public const IntPtr KWeakObjectPtr = 18;
	public const IntPtr KLazyObjectPtr = 19;
	public const IntPtr KScriptInterface = 20;

	public const IntPtr KFieldPath = 21;

	public static Dictionary<Type, IntPtr> STypeMap => new()
	{
		{ typeof(uint8), KUInt8 },
		{ typeof(uint16), KUInt16 },
		{ typeof(uint32), KUInt32 },
		{ typeof(uint64), KUInt64 },
		{ typeof(int8), KInt8 },
		{ typeof(int16), KInt16 },
		{ typeof(int32), KInt32 },
		{ typeof(int64), KInt64 },
		{ typeof(float), KFloat },
		{ typeof(double), KDouble },
		{ typeof(bool), KBool },
		
		{ typeof(UnrealString), KString },
		{ typeof(UnrealName), KName },
		{ typeof(UnrealText), KText },
		
		{ typeof(FieldPath), KFieldPath },
	};
}


