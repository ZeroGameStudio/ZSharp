// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

[Flags]
public enum EExportedMethodFlags : uint64
{
	None = 0,
	Static = 1 << 0,
	Public = 1 << 1,
	Protected = 1 << 2,
	Private = 1 << 3,
}

public class ExportedMethod
{
	public required string Name { get; set; }
	public required string ZCallName { get; set; }
	public required EExportedMethodFlags Flags { get; set; }
	public required List<ExportedParameter> Parameters { get; set; }
	
	public bool IsStatic => (Flags & EExportedMethodFlags.Static) != EExportedMethodFlags.None;
	public bool IsPublic => (Flags & EExportedMethodFlags.Public) != EExportedMethodFlags.None;
	public bool IsProtected => (Flags & EExportedMethodFlags.Protected) != EExportedMethodFlags.None;
	public bool IsPrivate => (Flags & EExportedMethodFlags.Private) != EExportedMethodFlags.None;

	public ExportedParameter? ReturnParameter => Parameters.Count > 0 ? Parameters[^1] : null;
}