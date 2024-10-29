// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Globalization;

namespace ZeroGames.ZSharp.Core;

public readonly struct ConsoleVariable : IEquatable<ConsoleVariable>
{

	public static ConsoleVariable FromName(string? name) => new(name ?? string.Empty);
	
	public override bool Equals(object? obj)
	{
		return obj is ConsoleVariable other && Equals(other);
	}

	public override int32 GetHashCode()
	{
		return Name.GetHashCode();
	}
	
	public bool Equals(ConsoleVariable other)
	{
		return Name == other.Name;
	}

	public bool TryGetBool(out bool value)
	{
		if (ConsoleManager.TryGetValue(Name, out var str))
		{
			return bool.TryParse(str, out value);
		}

		value = default;
		return false;
	}

	public bool TryGetInt(out int32 value)
	{
		if (ConsoleManager.TryGetValue(Name, out var str))
		{
			return int32.TryParse(str, out value);
		}

		value = default;
		return false;
	}
	
	public bool TryGetFloat(out float value)
	{
		if (ConsoleManager.TryGetValue(Name, out var str))
		{
			return float.TryParse(str, out value);
		}

		value = default;
		return false;
	}

	public bool TryGetString([NotNullWhen(true)] out string? value) => ConsoleManager.TryGetValue(Name, out value);

	public bool TrySetBool(bool value) => ConsoleManager.TrySetValue(Name, value.ToString());
	public bool TrySetInt(int32 value) => ConsoleManager.TrySetValue(Name, value.ToString());
	public bool TrySetFloat(float value) => ConsoleManager.TrySetValue(Name, value.ToString(CultureInfo.InvariantCulture));
	public bool TrySetString(string? value) => ConsoleManager.TrySetValue(Name, value);
	
	public string Name { get; }
	public event Action<ConsoleVariable> OnChanged
	{
		add => ConsoleManager.TryRegisterOnChanged(Name, value);
		remove => ConsoleManager.TryUnregisterOnChanged(Name, value);
	}
	
	private ConsoleVariable(string name) => Name = name;

}

public readonly struct BoolConsoleVariable : IEquatable<BoolConsoleVariable>
{
	
	public static BoolConsoleVariable FromName(string? name) => new(name ?? string.Empty);
	
	public override bool Equals(object? obj)
	{
		return obj is BoolConsoleVariable other && Equals(other);
	}

	public override int32 GetHashCode()
	{
		return Name.GetHashCode();
	}
	
	public bool Equals(BoolConsoleVariable other)
	{
		return Name == other.Name;
	}

	public bool TryGetValue(out bool value) => Generalize().TryGetBool(out value);
	public bool TrySetValue(bool value) => Generalize().TrySetBool(value);

	public ConsoleVariable Generalize() => ConsoleVariable.FromName(Name);

	public string Name { get; }
	public event Action<ConsoleVariable> OnChanged
	{
		add => ConsoleManager.TryRegisterOnChanged(Name, value);
		remove => ConsoleManager.TryUnregisterOnChanged(Name, value);
	}
	
	private BoolConsoleVariable(string name) => Name = name;

}

public readonly struct IntConsoleVariable : IEquatable<IntConsoleVariable>
{
	
	public static IntConsoleVariable FromName(string? name) => new(name ?? string.Empty);
	
	public override bool Equals(object? obj)
	{
		return obj is IntConsoleVariable other && Equals(other);
	}

	public override int32 GetHashCode()
	{
		return Name.GetHashCode();
	}
	
	public bool Equals(IntConsoleVariable other)
	{
		return Name == other.Name;
	}

	public bool TryGetValue(out int32 value) => Generalize().TryGetInt(out value);
	public bool TrySetValue(int32 value) => Generalize().TrySetInt(value);

	public ConsoleVariable Generalize() => ConsoleVariable.FromName(Name);

	public string Name { get; }
	public event Action<ConsoleVariable> OnChanged
	{
		add => ConsoleManager.TryRegisterOnChanged(Name, value);
		remove => ConsoleManager.TryUnregisterOnChanged(Name, value);
	}
	
	private IntConsoleVariable(string name) => Name = name;

}

public readonly struct FloatConsoleVariable : IEquatable<FloatConsoleVariable>
{
	
	public static FloatConsoleVariable FromName(string? name) => new(name ?? string.Empty);
	
	public override bool Equals(object? obj)
	{
		return obj is FloatConsoleVariable other && Equals(other);
	}

	public override int32 GetHashCode()
	{
		return Name.GetHashCode();
	}
	
	public bool Equals(FloatConsoleVariable other)
	{
		return Name == other.Name;
	}

	public bool TryGetValue(out float value) => Generalize().TryGetFloat(out value);
	public bool TrySetValue(float value) => Generalize().TrySetFloat(value);

	public ConsoleVariable Generalize() => ConsoleVariable.FromName(Name);
	
	public string Name { get; }
	public event Action<ConsoleVariable> OnChanged
	{
		add => ConsoleManager.TryRegisterOnChanged(Name, value);
		remove => ConsoleManager.TryUnregisterOnChanged(Name, value);
	}
	
	private FloatConsoleVariable(string name) => Name = name;

}

public readonly struct StringConsoleVariable : IEquatable<StringConsoleVariable>
{
	
	public static StringConsoleVariable FromName(string? name) => new(name ?? string.Empty);
	
	public override bool Equals(object? obj)
	{
		return obj is StringConsoleVariable other && Equals(other);
	}

	public override int32 GetHashCode()
	{
		return Name.GetHashCode();
	}
	
	public bool Equals(StringConsoleVariable other)
	{
		return Name == other.Name;
	}

	public bool TryGetValue([NotNullWhen(true)] out string? value) => Generalize().TryGetString(out value);
	public bool TrySetValue(string? value) => Generalize().TrySetString(value);

	public ConsoleVariable Generalize() => ConsoleVariable.FromName(Name);
	
	public string Name { get; }
	public event Action<ConsoleVariable> OnChanged
	{
		add => ConsoleManager.TryRegisterOnChanged(Name, value);
		remove => ConsoleManager.TryUnregisterOnChanged(Name, value);
	}
	
	private StringConsoleVariable(string name) => Name = name;

}


