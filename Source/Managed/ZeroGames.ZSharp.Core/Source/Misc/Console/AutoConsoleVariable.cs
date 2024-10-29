// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.Core;

public abstract class AutoConsoleVariableBase : IDisposable
{

	public AutoConsoleVariableBase(string? name, string? defaultValue, string? help, Action<ConsoleVariable>? onChanged)
	{
		Name = name ?? string.Empty;
		_onChanged = onChanged;

		_registered = ConsoleManager.TryRegisterVariable(name, help, defaultValue);
		if (_registered)
		{
			ConsoleManager.TryRegisterOnChanged(name, onChanged);
		}
	}

	~AutoConsoleVariableBase()
	{
		InternalDispose();
	}
	
	public void Dispose()
	{
		InternalDispose();
		
		GC.SuppressFinalize(this);
	}
	
	private void InternalDispose()
	{
		if (_disposed)
		{
			return;
		}

		_disposed = true;

		if (_registered)
		{
			ConsoleManager.TryUnregisterOnChanged(Name, _onChanged);
			ConsoleManager.TryUnregisterVariable(Name);
		}
	}
	
	public string Name { get; }

	private Action<ConsoleVariable>? _onChanged;
	private bool _registered;
	private bool _disposed;

}

public sealed class AutoBoolConsoleVariable(string name, int32 defaultValue = default, string help = "", Action<ConsoleVariable>? onChanged = default) : AutoConsoleVariableBase(name, defaultValue.ToString(), help, onChanged)
{
	public bool TryGetValue(out bool value) => BoolConsoleVariable.FromName(Name).TryGetValue(out value);
}

public sealed class AutoIntConsoleVariable(string name, int32 defaultValue = default, string help = "", Action<ConsoleVariable>? onChanged = default) : AutoConsoleVariableBase(name, defaultValue.ToString(), help, onChanged)
{
	public bool TryGetValue(out int32 value) => IntConsoleVariable.FromName(Name).TryGetValue(out value);
}

public sealed class AutoFloatConsoleVariable(string name, int32 defaultValue = default, string help = "", Action<ConsoleVariable>? onChanged = default) : AutoConsoleVariableBase(name, defaultValue.ToString(), help, onChanged)
{
	public bool TryGetValue(out float value) => FloatConsoleVariable.FromName(Name).TryGetValue(out value);
}

public sealed class AutoStringConsoleVariable(string name, int32 defaultValue = default, string help = "", Action<ConsoleVariable>? onChanged = default) : AutoConsoleVariableBase(name, defaultValue.ToString(), help, onChanged)
{
	public bool TryGetValue([NotNullWhen(true)] out string? value) => StringConsoleVariable.FromName(Name).TryGetValue(out value);
}


