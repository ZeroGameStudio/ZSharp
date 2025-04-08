// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;

namespace ZeroGames.ZSharp.UnrealEngine;

internal class Archive : IArchive
{

	public static object ToManaged(IntPtr unmanaged) => new Archive(unmanaged);
	
	public void Dispose() => _unmanaged = IntPtr.Zero;
	
	public IArchive Serialize(uint8 value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out uint8 value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(uint16 value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out uint16 value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(uint32 value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out uint32 value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(uint64 value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out uint64 value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(int8 value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out int8 value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(int16 value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out int16 value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(int32 value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out int32 value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(int64 value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out int64 value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(float value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out float value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(double value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out double value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(bool value)
	{
		return Serialize(Convert.ToByte(value));
	}

	public IArchive Serialize(out bool value)
	{
		Serialize(out uint8 b);
		value = b > 0;
		return this;
	}

	public IArchive Serialize(UnrealString? value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out UnrealString value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(UnrealAnsiString? value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out UnrealAnsiString value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(UnrealUtf8String? value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out UnrealUtf8String value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(UnrealName? value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out UnrealName value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(UnrealText? value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out UnrealText value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(string value)
	{
		return Serialize(new UnrealString(value));
	}

	public IArchive Serialize(out string value)
	{
		Serialize(out UnrealString str);
		value = str;
		return this;
	}

	public IArchive Serialize(UnrealObject? value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out UnrealObject? value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(UnrealScriptStructBase? value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out UnrealScriptStructBase value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public bool NetSerialize(UnrealScriptStructBase? value, PackageMap map, out bool success)
	{
		GuardSaving();
		return InternalNetSerialize(value, map, out success);
	}

	public bool NetSerialize(out UnrealScriptStructBase value, PackageMap map, out bool success)
	{
		GuardLoading();
		return InternalNetSerialize(out value, map, out success);
	}

	public IArchive Serialize(SoftObjectPath? value)
	{
		GuardSaving();
		InternalSerialize(value);
		return this;
	}

	public IArchive Serialize(out SoftObjectPath value)
	{
		GuardLoading();
		InternalSerialize(out value);
		return this;
	}

	public IArchive Serialize(ReadOnlySpan<uint8> value)
	{
		GuardSaving();
		return this;
	}

	public IArchive Serialize(out uint8[] value)
	{
		GuardLoading();
		value = [];
		return this;
	}

	public bool IsSaving
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsSaving;
		}
	}
	
	public bool IsLoading
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsLoading;
		}
	}
	
	public int64 Position
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalPosition;
		}
		set
		{
			MasterAlcCache.GuardInvariant();
			InternalPosition = value;
		}
	}

	public int64 TotalSize
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalTotalSize;
		}
	}
	
	private Archive(IntPtr unmanaged)
	{
		_unmanaged = unmanaged;
	}
	
	[Conditional("ASSERTION_CHECK")]
	private void GuardSaving()
	{
		MasterAlcCache.GuardInvariant();
		if (_unmanaged is 0)
		{
			throw new ObjectDisposedException("Archive has already been disposed.");
		}
		
		if (!IsSaving)
		{
			throw new InvalidOperationException();
		}
	}

	[Conditional("ASSERTION_CHECK")]
	private void GuardLoading()
	{
		MasterAlcCache.GuardInvariant();
		if (_unmanaged is 0)
		{
			throw new ObjectDisposedException("Archive has already been disposed.");
		}
		
		if (!IsLoading)
		{
			throw new InvalidOperationException();
		}
	}

	private unsafe void InternalSerialize(uint8 value) => Archive_Interop.SaveUInt8(_unmanaged, value);
	private unsafe void InternalSerialize(out uint8 value) => value = Archive_Interop.LoadUInt8(_unmanaged);

	private unsafe void InternalSerialize(uint16 value) => Archive_Interop.SaveUInt16(_unmanaged, value);
	private unsafe void InternalSerialize(out uint16 value) => value = Archive_Interop.LoadUInt16(_unmanaged);

	private unsafe void InternalSerialize(uint32 value) => Archive_Interop.SaveUInt32(_unmanaged, value);
	private unsafe void InternalSerialize(out uint32 value) => value = Archive_Interop.LoadUInt32(_unmanaged);

	private unsafe void InternalSerialize(uint64 value) => Archive_Interop.SaveUInt64(_unmanaged, value);
	private unsafe void InternalSerialize(out uint64 value) => value = Archive_Interop.LoadUInt64(_unmanaged);

	private unsafe void InternalSerialize(int8 value) => Archive_Interop.SaveInt8(_unmanaged, value);
	private unsafe void InternalSerialize(out int8 value) => value = Archive_Interop.LoadInt8(_unmanaged);

	private unsafe void InternalSerialize(int16 value) => Archive_Interop.SaveInt16(_unmanaged, value);
	private unsafe void InternalSerialize(out int16 value) => value = Archive_Interop.LoadInt16(_unmanaged);

	private unsafe void InternalSerialize(int32 value) => Archive_Interop.SaveInt32(_unmanaged, value);
	private unsafe void InternalSerialize(out int32 value) => value = Archive_Interop.LoadInt32(_unmanaged);

	private unsafe void InternalSerialize(int64 value) => Archive_Interop.SaveInt64(_unmanaged, value);
	private unsafe void InternalSerialize(out int64 value) => value = Archive_Interop.LoadInt64(_unmanaged);

	private unsafe void InternalSerialize(float value) => Archive_Interop.SaveFloat(_unmanaged, value);
	private unsafe void InternalSerialize(out float value) => value = Archive_Interop.LoadFloat(_unmanaged);

	private unsafe void InternalSerialize(double value) => Archive_Interop.SaveDouble(_unmanaged, value);
	private unsafe void InternalSerialize(out double value) => value = Archive_Interop.LoadDouble(_unmanaged);

	private unsafe void InternalSerialize(UnrealString? value) => Archive_Interop.SaveString(_unmanaged, ConjugateHandle.FromConjugate(value));
	private unsafe void InternalSerialize(out UnrealString value) => value = Archive_Interop.LoadString(_unmanaged).GetTargetChecked<UnrealString>();

	private unsafe void InternalSerialize(UnrealAnsiString? value) => Archive_Interop.SaveAnsiString(_unmanaged, ConjugateHandle.FromConjugate(value));
	private unsafe void InternalSerialize(out UnrealAnsiString value) => value = Archive_Interop.LoadAnsiString(_unmanaged).GetTargetChecked<UnrealAnsiString>();

	private unsafe void InternalSerialize(UnrealUtf8String? value) => Archive_Interop.SaveUtf8String(_unmanaged, ConjugateHandle.FromConjugate(value));
	private unsafe void InternalSerialize(out UnrealUtf8String value) => value = Archive_Interop.LoadUtf8String(_unmanaged).GetTargetChecked<UnrealUtf8String>();

	private unsafe void InternalSerialize(UnrealName? value) => Archive_Interop.SaveName(_unmanaged, ConjugateHandle.FromConjugate(value));
	private unsafe void InternalSerialize(out UnrealName value) => value = Archive_Interop.LoadName(_unmanaged).GetTargetChecked<UnrealName>();

	private unsafe void InternalSerialize(UnrealText? value) => Archive_Interop.SaveText(_unmanaged, ConjugateHandle.FromConjugate(value));
	private unsafe void InternalSerialize(out UnrealText value) => value = Archive_Interop.LoadText(_unmanaged).GetTargetChecked<UnrealText>();

	private unsafe void InternalSerialize(UnrealObject? value) => Archive_Interop.SaveObject(_unmanaged, ConjugateHandle.FromConjugate(value));
	private unsafe void InternalSerialize(out UnrealObject? value) => value = Archive_Interop.LoadObject(_unmanaged).GetTargetChecked<UnrealObject>();

	private unsafe void InternalSerialize(UnrealScriptStructBase? value) => Archive_Interop.SaveScriptStruct(_unmanaged, ConjugateHandle.FromConjugate(value));
	private unsafe void InternalSerialize(out UnrealScriptStructBase value) => value = Archive_Interop.LoadScriptStruct(_unmanaged).GetTargetChecked<UnrealScriptStructBase>();

	private unsafe bool InternalNetSerialize(UnrealScriptStructBase? value, PackageMap map, out bool success)
	{
		uint8 res = Archive_Interop.NetSaveScriptStruct(_unmanaged, ConjugateHandle.FromConjugate(value), ConjugateHandle.FromConjugate(map), out var suc);
		success = suc > 0;
		return res > 0;
	}

	private unsafe bool InternalNetSerialize(out UnrealScriptStructBase value, PackageMap map, out bool success)
	{
		uint8 res = Archive_Interop.NetLoadScriptStruct(_unmanaged, out var val, ConjugateHandle.FromConjugate(map), out var suc);
		value = val.GetTargetChecked<UnrealScriptStructBase>();
		success = suc > 0;
		return res > 0;
	}
	
	private unsafe void InternalSerialize(SoftObjectPath? value) => Archive_Interop.SaveSoftObjectPath(_unmanaged, ConjugateHandle.FromConjugate(value));
	private unsafe void InternalSerialize(out SoftObjectPath value) => value = Archive_Interop.LoadSoftObjectPath(_unmanaged).GetTargetChecked<SoftObjectPath>();

	private unsafe bool InternalIsSaving => Archive_Interop.IsSaving(_unmanaged) > 0;
	private unsafe bool InternalIsLoading => Archive_Interop.IsLoading(_unmanaged) > 0;

	private unsafe int64 InternalPosition
	{
		get => Archive_Interop.Tell(_unmanaged);
		set => Archive_Interop.Seek(_unmanaged, value);
	}
	
	private unsafe int64 InternalTotalSize => Archive_Interop.GetTotalSize(_unmanaged);
	
	private IntPtr _unmanaged;

}


