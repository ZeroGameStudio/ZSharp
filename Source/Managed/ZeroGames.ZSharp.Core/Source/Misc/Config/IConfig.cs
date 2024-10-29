// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.Core;

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
	
	public static bool TryGetInt32(this IConfig @this, string baseIniName, string section, string key, out int32 value)
		=> @this.TryGetInt32ByFileName(@this.GetFileName(baseIniName), section, key, out value);
	
	public static bool TryGetInt64(this IConfig @this, string baseIniName, string section, string key, out int64 value)
		=> @this.TryGetInt64ByFileName(@this.GetFileName(baseIniName), section, key, out value);
	
	public static bool TryGetFloat(this IConfig @this, string baseIniName, string section, string key, out float value)
		=> @this.TryGetFloatByFileName(@this.GetFileName(baseIniName), section, key, out value);
	
	public static bool TryGetDouble(this IConfig @this, string baseIniName, string section, string key, out double value)
		=> @this.TryGetDoubleByFileName(@this.GetFileName(baseIniName), section, key, out value);
	
	public static bool TryGetBool(this IConfig @this, string baseIniName, string section, string key, out bool value)
		=> @this.TryGetBoolByFileName(@this.GetFileName(baseIniName), section, key, out value);

	public static string GetStringOrDefault(this IConfig @this, string baseIniName, string section, string key)
		=> @this.GetStringOrDefault(baseIniName, section, key, string.Empty);
	
	public static string GetStringOrDefault(this IConfig @this, string baseIniName, string section, string key, string defaultValue)
		=> @this.TryGetString(baseIniName, section, key, out var value) ? value : defaultValue;
	
	public static int32 GetInt32OrDefault(this IConfig @this, string baseIniName, string section, string key)
		=> @this.GetInt32OrDefault(baseIniName, section, key, default);
	
	public static int32 GetInt32OrDefault(this IConfig @this, string baseIniName, string section, string key, int32 defaultValue)
		=> @this.TryGetInt32(baseIniName, section, key, out var value) ? value : defaultValue;
	
	public static int64 GetInt64OrDefault(this IConfig @this, string baseIniName, string section, string key)
		=> @this.GetInt64OrDefault(baseIniName, section, key, default);
	
	public static int64 GetInt64OrDefault(this IConfig @this, string baseIniName, string section, string key, int64 defaultValue)
		=> @this.TryGetInt64(baseIniName, section, key, out var value) ? value : defaultValue;
	
	public static float GetFloatOrDefault(this IConfig @this, string baseIniName, string section, string key)
		=> @this.GetFloatOrDefault(baseIniName, section, key, default);
	
	public static float GetFloatOrDefault(this IConfig @this, string baseIniName, string section, string key, float defaultValue)
		=> @this.TryGetFloat(baseIniName, section, key, out var value) ? value : defaultValue;
	
	public static double GetDoubleOrDefault(this IConfig @this, string baseIniName, string section, string key)
		=> @this.GetDoubleOrDefault(baseIniName, section, key, default);
	
	public static double GetDoubleOrDefault(this IConfig @this, string baseIniName, string section, string key, double defaultValue)
		=> @this.TryGetDouble(baseIniName, section, key, out var value) ? value : defaultValue;
	
	public static bool GetBoolOrDefault(this IConfig @this, string baseIniName, string section, string key)
		=> @this.GetBoolOrDefault(baseIniName, section, key, default);
	
	public static bool GetBoolOrDefault(this IConfig @this, string baseIniName, string section, string key, bool defaultValue)
		=> @this.TryGetBool(baseIniName, section, key, out var value) ? value : defaultValue;
	
	public static bool TryGetInt32ByFileName(this IConfig @this, string fileName, string section, string key, out int32 value)
	{
		if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
		{
			value = int32.Parse(stringValue);
			return true;
		}

		value = default;
		return false;
	}
	
	public static bool TryGetInt64ByFileName(this IConfig @this, string fileName, string section, string key, out int64 value)
	{
		if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
		{
			value = int64.Parse(stringValue);
			return true;
		}

		value = default;
		return false;
	}
	
	public static bool TryGetFloatByFileName(this IConfig @this, string fileName, string section, string key, out float value)
	{
		if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
		{
			value = float.Parse(stringValue);
			return true;
		}

		value = default;
		return false;
	}
	
	public static bool TryGetDoubleByFileName(this IConfig @this, string fileName, string section, string key, out double value)
	{
		if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
		{
			value = double.Parse(stringValue);
			return true;
		}

		value = default;
		return false;
	}
	
	public static bool TryGetBoolByFileName(this IConfig @this, string fileName, string section, string key, out bool value)
	{
		if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
		{
			value = bool.Parse(stringValue);
			return true;
		}

		value = default;
		return false;
	}
	
}


