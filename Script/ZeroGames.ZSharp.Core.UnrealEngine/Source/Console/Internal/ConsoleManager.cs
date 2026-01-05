// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal static class ConsoleManager
{

	public static unsafe bool TryExecuteCommand(string? name, ReadOnlySpan<string?> args)
	{
		if (string.IsNullOrWhiteSpace(name))
		{
			return false;
		}
		
		string[] trimmedArgs = args.ToArray().Where(arg => !string.IsNullOrWhiteSpace(arg)).ToArray()!;

		// No need to interop with unmanaged side if this is a managed command.
		if (_commandMap.TryGetValue(name, out var action))
		{
			action(trimmedArgs);
			return true;
		}

		string command = string.Join(' ', [ name, ..trimmedArgs ]);
		fixed (char* commandBuffer = command)
		{
			return Console_Interop.TryExecuteCommand(commandBuffer) > 0;
		}
	}

	public static unsafe bool TryGetValue(string? name, [NotNullWhen(true)] out string? value)
	{
		if (string.IsNullOrWhiteSpace(name))
		{
			value = null;
			return false;
		}
		
		using InteropString valueBuffer = new();

		fixed (char* nameBuffer = name)
		{
			if (Console_Interop.TryGetValue(nameBuffer, valueBuffer.Address) > 0)
			{
				value = valueBuffer;
				return true;
			}
		}

		value = null;
		return false;
	}

	public static unsafe bool TrySetValue(string? name, string? value)
	{
		if (string.IsNullOrWhiteSpace(name))
		{
			return false;
		}
		
		fixed (char* nameBuffer = name)
		fixed (char* valueBuffer = value)
		{
			return Console_Interop.TrySetValue(nameBuffer, valueBuffer) > 0;
		}
	}

	public static unsafe bool TryRegisterOnChanged(string? name, Action<ConsoleVariable>? action)
	{
		if (string.IsNullOrWhiteSpace(name))
		{
			return false;
		}

		if (action is null)
		{
			return false;
		}
		
		if (!_onChangedMap.TryGetValue(name, out var invocationList))
		{
			fixed (char* nameBuffer = name)
			{
				if (Console_Interop.TryRegisterOnChanged(nameBuffer) == 0)
				{
					return false;
				}
			}
			
			invocationList = new();
			_onChangedMap[name] = invocationList;
		}
		
		invocationList.Add(action);
		return true;
	}

	public static unsafe bool TryUnregisterOnChanged(string? name, Action<ConsoleVariable>? action)
	{
		if (string.IsNullOrWhiteSpace(name))
		{
			return false;
		}
		
		if (action is null)
		{
			return false;
		}

		if (!_onChangedMap.TryGetValue(name, out var invocationList))
		{
			return false;
		}

		if (!invocationList.Remove(action))
		{
			return false;
		}

		if (invocationList.Count == 0)
		{
			fixed (char* nameBuffer = name)
			{
				assert(Console_Interop.TryUnregisterOnChanged(nameBuffer) > 0);
			}
			
			_onChangedMap.Remove(name);
		}
		
		return true;
	}

	public static unsafe bool TryRegisterCommand(string? name, string? help, Action<string[]>? action)
	{
		if (string.IsNullOrWhiteSpace(name))
		{
			return false;
		}

		action ??= _ => {};

		if (_commandMap.ContainsKey(name))
		{
			return false;
		}
		
		fixed (char* nameBuffer = name)
		fixed (char* helpBuffer = help)
		{
			if (Console_Interop.TryRegisterCommand(nameBuffer, helpBuffer) == 0)
			{
				return false;
			}
		}

		_commandMap[name] = action;
		return true;
	}

	public static unsafe bool TryUnregisterCommand(string? name)
	{
		if (string.IsNullOrWhiteSpace(name))
		{
			return false;
		}

		fixed (char* nameBuffer = name)
		{
			if (Console_Interop.TryUnregisterObject(nameBuffer) == 0)
			{
				return false;
			}
		}
		
		return ensure(_commandMap.Remove(name));
	}

	public static unsafe bool TryRegisterVariable(string? name, string? help, string? defaultValue)
	{
		if (string.IsNullOrWhiteSpace(name))
		{
			return false;
		}

		if (_registeredVariables.Contains(name))
		{
			return false;
		}
		
		fixed (char* nameBuffer = name)
		fixed (char* helpBuffer = help)
		fixed (char* defaultValueBuffer = defaultValue)
		{
			if (Console_Interop.TryRegisterVariable(nameBuffer, helpBuffer, defaultValueBuffer) == 0)
			{
				return false;
			}
		}

		_registeredVariables.Add(name);
		return true;
	}

	public static unsafe bool TryUnregisterVariable(string? name)
	{
		if (string.IsNullOrWhiteSpace(name))
		{
			return false;
		}

		fixed (char* nameBuffer = name)
		{
			if (Console_Interop.TryUnregisterObject(nameBuffer) == 0)
			{
				return false;
			}
		}

		return ensure(_registeredVariables.Remove(name));
	}

	public static void HandleExecuteCommand(string name, string[]? args)
	{
		verify(_commandMap.TryGetValue(name, out var action));
		action(args ?? []);
	}

	public static void HandleVariableChanged(string name)
	{
		verify(_onChangedMap.TryGetValue(name, out var invocationList));
		assert(invocationList.Count > 0);
		foreach (var action in invocationList)
		{
			action(ConsoleVariable.FromName(name));
		}
	}

	private static readonly Dictionary<string, Action<string[]>> _commandMap = new();
	private static readonly HashSet<string> _registeredVariables = new();
	private static readonly Dictionary<string, List<Action<ConsoleVariable>>> _onChangedMap = new();

}


