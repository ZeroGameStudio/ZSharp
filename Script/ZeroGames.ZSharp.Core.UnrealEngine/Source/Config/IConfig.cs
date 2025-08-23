// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public interface IConfig
{
	string GetFileName(string baseIniName);
	bool TryGetSectionByFileName(string fileName, string section, [NotNullWhen(true)] out string[]? values);
	bool TryGetArrayByFileName(string fileName, string section, string key, [NotNullWhen(true)] out string[]? values);
	bool TryGetStringByFileName(string fileName, string section, string key, [NotNullWhen(true)] out string? value);
}

public static class ConfigExtensions
{
	extension(IConfig @this)
	{
		public bool TryGetSection(string baseIniName, string section, [NotNullWhen(true)] out string[]? values)
			=> @this.TryGetSectionByFileName(@this.GetFileName(baseIniName), section, out values);

		public bool TryGetArray(string baseIniName, string section, string key, [NotNullWhen(true)] out string[]? values)
			=> @this.TryGetArrayByFileName(@this.GetFileName(baseIniName), section, key, out values);

		public bool TryGetString(string baseIniName, string section, string key, [NotNullWhen(true)] out string? value)
			=> @this.TryGetStringByFileName(@this.GetFileName(baseIniName), section, key, out value);

		public bool TryGetInt32(string baseIniName, string section, string key, out int32 value)
			=> @this.TryGetInt32ByFileName(@this.GetFileName(baseIniName), section, key, out value);

		public bool TryGetInt64(string baseIniName, string section, string key, out int64 value)
			=> @this.TryGetInt64ByFileName(@this.GetFileName(baseIniName), section, key, out value);

		public bool TryGetFloat(string baseIniName, string section, string key, out float value)
			=> @this.TryGetFloatByFileName(@this.GetFileName(baseIniName), section, key, out value);

		public bool TryGetDouble(string baseIniName, string section, string key, out double value)
			=> @this.TryGetDoubleByFileName(@this.GetFileName(baseIniName), section, key, out value);

		public bool TryGetBool(string baseIniName, string section, string key, out bool value)
			=> @this.TryGetBoolByFileName(@this.GetFileName(baseIniName), section, key, out value);

		public string GetStringOrDefault(string baseIniName, string section, string key)
			=> @this.GetStringOrDefault(baseIniName, section, key, string.Empty);

		public string GetStringOrDefault(string baseIniName, string section, string key, string defaultValue)
			=> @this.TryGetString(baseIniName, section, key, out var value) ? value : defaultValue;

		public int32 GetInt32OrDefault(string baseIniName, string section, string key)
			=> @this.GetInt32OrDefault(baseIniName, section, key, default);

		public int32 GetInt32OrDefault(string baseIniName, string section, string key, int32 defaultValue)
			=> @this.TryGetInt32(baseIniName, section, key, out var value) ? value : defaultValue;

		public int64 GetInt64OrDefault(string baseIniName, string section, string key)
			=> @this.GetInt64OrDefault(baseIniName, section, key, default);

		public int64 GetInt64OrDefault(string baseIniName, string section, string key, int64 defaultValue)
			=> @this.TryGetInt64(baseIniName, section, key, out var value) ? value : defaultValue;

		public float GetFloatOrDefault(string baseIniName, string section, string key)
			=> @this.GetFloatOrDefault(baseIniName, section, key, default);

		public float GetFloatOrDefault(string baseIniName, string section, string key, float defaultValue)
			=> @this.TryGetFloat(baseIniName, section, key, out var value) ? value : defaultValue;

		public double GetDoubleOrDefault(string baseIniName, string section, string key)
			=> @this.GetDoubleOrDefault(baseIniName, section, key, default);

		public double GetDoubleOrDefault(string baseIniName, string section, string key, double defaultValue)
			=> @this.TryGetDouble(baseIniName, section, key, out var value) ? value : defaultValue;

		public bool GetBoolOrDefault(string baseIniName, string section, string key)
			=> @this.GetBoolOrDefault(baseIniName, section, key, default);

		public bool GetBoolOrDefault(string baseIniName, string section, string key, bool defaultValue)
			=> @this.TryGetBool(baseIniName, section, key, out var value) ? value : defaultValue;

		public bool TryGetInt32ByFileName(string fileName, string section, string key, out int32 value)
		{
			if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
			{
				value = int32.Parse(stringValue);
				return true;
			}

			value = 0;
			return false;
		}

		public bool TryGetInt64ByFileName(string fileName, string section, string key, out int64 value)
		{
			if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
			{
				value = int64.Parse(stringValue);
				return true;
			}

			value = 0;
			return false;
		}

		public bool TryGetFloatByFileName(string fileName, string section, string key, out float value)
		{
			if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
			{
				value = float.Parse(stringValue);
				return true;
			}

			value = 0;
			return false;
		}

		public bool TryGetDoubleByFileName(string fileName, string section, string key, out double value)
		{
			if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
			{
				value = double.Parse(stringValue);
				return true;
			}

			value = 0;
			return false;
		}

		public bool TryGetBoolByFileName(string fileName, string section, string key, out bool value)
		{
			if (@this.TryGetStringByFileName(fileName, section, key, out var stringValue))
			{
				value = bool.Parse(stringValue);
				return true;
			}

			value = false;
			return false;
		}
	}
}


