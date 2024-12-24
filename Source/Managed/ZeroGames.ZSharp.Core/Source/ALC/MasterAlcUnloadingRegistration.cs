// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public readonly struct MasterAlcUnloadingRegistration : IEquatable<MasterAlcUnloadingRegistration>, IDisposable
{

	public void Dispose() => MasterAssemblyLoadContext.UnregisterUnloading(this);

	public bool Equals(MasterAlcUnloadingRegistration other) => _handle == other._handle;
	public override bool Equals(object? obj) => obj is MasterAlcUnloadingRegistration other && Equals(other);
	public override int32 GetHashCode() => _handle.GetHashCode();
	public static bool operator==(MasterAlcUnloadingRegistration lhs, MasterAlcUnloadingRegistration rhs) => lhs.Equals(rhs);
	public static bool operator!=(MasterAlcUnloadingRegistration lhs, MasterAlcUnloadingRegistration rhs) => !lhs.Equals(rhs);

	internal MasterAlcUnloadingRegistration(uint64 handle) => _handle = handle;
	
	private readonly uint64 _handle;

}


