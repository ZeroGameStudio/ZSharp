// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
// C# doesn't support ref parameter on operators so we can't use archive like this: ar << x << y << z.
public interface IArchive : IMarshalPointer
{
	public new static IDisposable ToManaged(IntPtr unmanaged) => Archive.ToManaged(unmanaged);
	
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
	
	IArchive Serialize(FString? value);
	IArchive Serialize(out FString value);
	
	IArchive Serialize(FAnsiString? value);
	IArchive Serialize(out FAnsiString value);
	
	IArchive Serialize(FUtf8String? value);
	IArchive Serialize(out FUtf8String value);
	
	IArchive Serialize(FName? value);
	IArchive Serialize(out FName value);
	
	IArchive Serialize(FText? value);
	IArchive Serialize(out FText value);
	
	IArchive Serialize(string? value);
	IArchive Serialize(out string value);
	
	IArchive Serialize(UObject? value);
	IArchive Serialize(out UObject? value);
	
	IArchive Serialize(UnrealScriptStructBase? value);
	IArchive Serialize(out UnrealScriptStructBase value);
	
	bool NetSerialize(UnrealScriptStructBase? value, UPackageMap map, out bool success);
	bool NetSerialize(out UnrealScriptStructBase value, UPackageMap map, out bool success);
	
	IArchive Serialize(FSoftObjectPath? value);
	IArchive Serialize(out FSoftObjectPath value);

	IArchive Serialize(ReadOnlySpan<uint8> value);
	IArchive Serialize(out uint8[] value);
	
	bool IsSaving { get; }
	bool IsLoading { get; }
	
	int64 Position { get; set; }
	int64 TotalSize { get; }
}


