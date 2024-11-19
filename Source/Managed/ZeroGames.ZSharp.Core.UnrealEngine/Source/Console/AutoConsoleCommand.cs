// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public sealed class AutoConsoleCommand : IDisposable
{

	public AutoConsoleCommand(string? name, string? help, Action<string[]>? action)
	{
		Name = name ?? string.Empty;
		
		_registered = ConsoleManager.TryRegisterCommand(name, help, action);
	}

	~AutoConsoleCommand()
	{
		InternalDispose();
	}
	
	public void Dispose()
	{
		InternalDispose();
		
		GC.SuppressFinalize(this);
	}

	public void TryExecute(params string[]? args) => ConsoleCommand.FromName(Name).TryExecute(args);
	
	private void InternalDispose()
	{
		if (_disposed)
		{
			return;
		}

		_disposed = true;

		if (_registered)
		{
			ConsoleManager.TryUnregisterCommand(Name);
		}
	}
	
	public string Name { get; }

	private bool _registered;
	private bool _disposed;

}


