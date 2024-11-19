// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public readonly struct ConsoleCommand : IEquatable<ConsoleCommand>
{
	
	public static ConsoleCommand FromName(string? name) => new(name ?? string.Empty);
	
	public override bool Equals(object? obj)
	{
		return obj is ConsoleCommand other && Equals(other);
	}

	public override int32 GetHashCode()
	{
		return Name.GetHashCode();
	}
	
	public bool Equals(ConsoleCommand other)
	{
		return Name == other.Name;
	}

	public bool TryExecute(params string[]? args) => ConsoleManager.TryExecuteCommand(Name, args);

	public string Name { get; }
	
	private ConsoleCommand(string name) => Name = name;

}


