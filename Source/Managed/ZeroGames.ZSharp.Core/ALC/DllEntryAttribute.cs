// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

[AttributeUsage(AttributeTargets.Assembly)]
public class DllEntryAttribute(Type entryType) : Attribute
{
    public Type EntryType { get; } = entryType;
}


