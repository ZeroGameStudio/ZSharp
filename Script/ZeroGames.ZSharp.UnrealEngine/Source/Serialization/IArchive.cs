// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

// C# doesn't support ref parameter on operators so we can't use archive like this: ar << x << y << z.
public interface IArchive : IMarshalPointer
{
	public new static object ToManaged(IntPtr unmanaged) => Archive.ToManaged(unmanaged);
	
	IArchive Serialize(uint8 value);
	IArchive Serialize(out uint8 value);
	
	IArchive Serialize(uint16 value);
	IArchive Serialize(out uint16 value);
	
	IArchive Serialize(uint32 value);
	IArchive Serialize(out uint32 value);
	
	IArchive Serialize(uint64 value);
	IArchive Serialize(out uint64 value);
	
	IArchive Serialize(int8 value);
	IArchive Serialize(out int8 value);
	
	IArchive Serialize(int16 value);
	IArchive Serialize(out int16 value);
	
	IArchive Serialize(int32 value);
	IArchive Serialize(out int32 value);
	
	IArchive Serialize(int64 value);
	IArchive Serialize(out int64 value);
	
	IArchive Serialize(float value);
	IArchive Serialize(out float value);
	
	IArchive Serialize(double value);
	IArchive Serialize(out double value);
	
	IArchive Serialize(bool value);
	IArchive Serialize(out bool value);
	
	IArchive Serialize(UnrealString? value);
	IArchive Serialize(out UnrealString value);
	
	IArchive Serialize(UnrealAnsiString? value);
	IArchive Serialize(out UnrealAnsiString value);
	
	IArchive Serialize(UnrealUtf8String? value);
	IArchive Serialize(out UnrealUtf8String value);
	
	IArchive Serialize(UnrealName? value);
	IArchive Serialize(out UnrealName value);
	
	IArchive Serialize(UnrealText? value);
	IArchive Serialize(out UnrealText value);
	
	IArchive Serialize(string? value);
	IArchive Serialize(out string value);
	
	IArchive Serialize(UnrealObject? value);
	IArchive Serialize(out UnrealObject? value);
	
	IArchive Serialize(UnrealScriptStructBase? value);
	IArchive Serialize(out UnrealScriptStructBase value);
	
	bool NetSerialize(UnrealScriptStructBase? value, PackageMap map, out bool success);
	bool NetSerialize(out UnrealScriptStructBase value, PackageMap map, out bool success);
	
	IArchive Serialize(SoftObjectPath? value);
	IArchive Serialize(out SoftObjectPath value);

	IArchive Serialize(ReadOnlySpan<uint8> value);
	IArchive Serialize(out uint8[] value);
	
	bool IsSaving { get; }
	bool IsLoading { get; }
	
	int64 Position { get; set; }
	int64 TotalSize { get; }
}


