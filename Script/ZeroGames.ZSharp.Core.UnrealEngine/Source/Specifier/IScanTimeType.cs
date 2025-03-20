// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine.Specifier;

/// <summary>
/// A transparent fallback runtime type object used on scanner generated System.Attribute object.
/// This is because at scan-time, the actual System.Type object may not in-memory.
/// This interface provides no information, but scanner should know it's actual type and should use a dynamic cast.
/// </summary>
public interface IScanTimeType;


