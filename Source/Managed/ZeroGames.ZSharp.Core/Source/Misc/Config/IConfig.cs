// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.Core;

/*
 * @TODO:
 * Currently, there is a bug in Section and Array:
 * If an element itself contains a ';', it will be incorrectly split.
 * A solution is to implement an escape sequence, converting the ';' within the element to ';;'.
 * When splitting, only split on ';' but not on ';;', and after splitting, revert ';;' back to ';'.
 */
public interface IConfig
{
	string GetFileName(string baseIniName);
	bool TryGetSectionByFileName(string fileName, string section, [NotNullWhen(true)] out string[]? values);
	bool TryGetArrayByFileName(string fileName, string section, string key, [NotNullWhen(true)] out string[]? values);
	bool TryGetStringByFileName(string fileName, string section, string key, [NotNullWhen(true)] out string? value);
}

public static class ConfigExtensions
{
	
	public static bool TryGetSection(this IConfig @this, string baseIniName, string section, [NotNullWhen(true)] out string[]? values)
		=> @this.TryGetSectionByFileName(@this.GetFileName(baseIniName), section, out values);
	
	public static bool TryGetArray(this IConfig @this, string baseIniName, string section, string key, [NotNullWhen(true)] out string[]? values)
		=> @this.TryGetArrayByFileName(@this.GetFileName(baseIniName), section, key, out values);
	
	public static bool TryGetString(this IConfig @this, string baseIniName, string section, string key, [NotNullWhen(true)] out string? value)
		=> @this.TryGetStringByFileName(@this.GetFileName(baseIniName), section, key, out value);
	
	public static bool TryGetInt32(this IConfig @this, string baseIniName, string section, string key, [NotNullWhen(true)] out int32? value)
		=> @this.TryGetInt32ByFileName(@this.GetFileName(baseIniName), section, key, out value);
	
	public static bool TryGetInt64(this IConfig @this, string baseIniName, string section, string key, [NotNullWhen(true)] out int64? value)
		=> @this.TryGetInt64ByFileName(@this.GetFileName(baseIniName), section, key, out value);
	
	public static bool TryGetFloat(this IConfig @this, string baseIniName, string section, string key, [NotNullWhen(true)] out float? value)
		=> @this.TryGetFloatByFileName(@this.GetFileName(baseIniName), section, key, out value);
	
	public static bool TryGetDouble(this IConfig @this, string baseIniName, string section, string key, [NotNullWhen(true)] out double? value)
		=> @this.TryGetDoubleByFileName(@this.GetFileName(baseIniName), section, key, out value);
	
	public static bool TryGetBool(this IConfig @this, string baseIniName, string section, string key, [NotNullWhen(true)] out bool? value)
		=> @this.TryGetBoolByFileName(@this.GetFileName(baseIniName), section, key, out value);
	
	public static bool TryGetInt32ByFileName(this IConfig @this, string fileName, string section, string key, [NotNullWhen(true)] out int32? value)
	{
		value = default;
		
		if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
		{
			value = int32.Parse(stringValue);
		}

		return value is not null;
	}
	
	public static bool TryGetInt64ByFileName(this IConfig @this, string fileName, string section, string key, [NotNullWhen(true)] out int64? value)
	{
		value = default;
		
		if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
		{
			value = int64.Parse(stringValue);
		}

		return value is not null;
	}
	
	public static bool TryGetFloatByFileName(this IConfig @this, string fileName, string section, string key, [NotNullWhen(true)] out float? value)
	{
		value = default;
		
		if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
		{
			value = float.Parse(stringValue);
		}

		return value is not null;
	}
	
	public static bool TryGetDoubleByFileName(this IConfig @this, string fileName, string section, string key, [NotNullWhen(true)] out double? value)
	{
		value = default;
		
		if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
		{
			value = double.Parse(stringValue);
		}

		return value is not null;
	}
	
	public static bool TryGetBoolByFileName(this IConfig @this, string fileName, string section, string key, [NotNullWhen(true)] out bool? value)
	{
		value = default;
		
		if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
		{
			value = bool.Parse(stringValue);
		}

		return value is not null;
	}
	
}


