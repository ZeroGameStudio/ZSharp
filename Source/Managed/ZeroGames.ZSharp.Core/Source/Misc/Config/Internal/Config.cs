// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.Core;

internal sealed class Config : IConfig
{
	
	public Config(IntPtr unmanaged)
	{
		_unmanaged = unmanaged;
	}

	public unsafe string GetFileName(string baseIniName)
	{
		using InteropString fileName = new();
		fixed (char* baseIniBuffer = baseIniName)
		{
			Config_Interop.GetFileName(_unmanaged, baseIniBuffer, fileName.Address);
		}

		return fileName.Data;
	}

	public unsafe bool TryGetSectionByFileName(string fileName, string section, [NotNullWhen(true)] out string[]? values)
	{
		values = default;
		
		bool suc;
		using InteropString valueString = new();
		fixed (char* fileNameBuffer = fileName)
		fixed (char* sectionBuffer = section)
		{
			suc = Config_Interop.TryGetSection(_unmanaged, fileNameBuffer, sectionBuffer, valueString.Address) > 0;
		}

		if (suc)
		{
			values = valueString.Data.Split(';');
		}
		
		return suc;
	}
	
	public unsafe bool TryGetArrayByFileName(string fileName, string section, string key, [NotNullWhen(true)] out string[]? values)
	{
		values = default;
		
		bool suc;
		using InteropString valueString = new();
		fixed (char* fileNameBuffer = fileName)
		fixed (char* sectionBuffer = section)
		fixed (char* keyBuffer = key)
		{
			suc = Config_Interop.TryGetArray(_unmanaged, fileNameBuffer, sectionBuffer, keyBuffer, valueString.Address) > 0;
		}

		if (suc)
		{
			values = valueString.Data.Split(';');
		}
		
		return suc;
	}

	public unsafe bool TryGetStringByFileName(string fileName, string section, string key, [NotNullWhen(true)] out string? value)
	{
		value = default;
		
		bool suc;
		using InteropString valueString = new();
		fixed (char* fileNameBuffer = fileName)
		fixed (char* sectionBuffer = section)
		fixed (char* keyBuffer = key)
		{
			suc = Config_Interop.TryGetArray(_unmanaged, fileNameBuffer, sectionBuffer, keyBuffer, valueString.Address) > 0;
		}

		if (suc)
		{
			value = valueString.Data;
		}
		
		return suc;
	}

	private readonly IntPtr _unmanaged;

}


