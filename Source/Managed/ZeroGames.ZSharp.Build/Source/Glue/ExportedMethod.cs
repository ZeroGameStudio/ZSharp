// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.Build.Glue;

[Flags]
public enum EExportedMethodFlags : uint64
{
	None = 0,
	Public = 1 << 0,
	Protected = 1 << 1,
	Private = 1 << 2,
	Static = 1 << 3,
	Virtual = 1 << 4,
	Abstract = 1 << 5,
}

public class ExportedMethod
{
	public required string Name { get; set; }
	public required string ZCallName { get; set; }
	public FullyExportedTypeName OwnerInterface { get; set; }
	public required EExportedMethodFlags Flags { get; set; }
	public required List<ExportedParameter> Parameters { get; set; }
	
	public bool IsPublic => Flags.HasFlag(EExportedMethodFlags.Public);
	public bool IsProtected => Flags.HasFlag(EExportedMethodFlags.Protected);
	public bool IsPrivate => Flags.HasFlag(EExportedMethodFlags.Private);
	public bool IsStatic => Flags.HasFlag(EExportedMethodFlags.Static);
	public bool IsVirtual => Flags.HasFlag(EExportedMethodFlags.Virtual);
	public bool IsAbstract => Flags.HasFlag(EExportedMethodFlags.Abstract);
	[MemberNotNullWhen(true, nameof(OwnerInterface))]
	public bool IsInterfaceMethod => !string.IsNullOrWhiteSpace(OwnerInterface.Name);

	public ExportedParameter? ReturnParameter => Parameters.Count > 0 && Parameters[^1].IsReturn ? Parameters[^1] : null;
}