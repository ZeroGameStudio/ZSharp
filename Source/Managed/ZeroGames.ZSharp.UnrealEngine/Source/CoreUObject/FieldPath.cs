// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed class FieldPath : PlainExportedObjectBase, IConjugate<FieldPath>
{
	public static FieldPath BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	public FieldPath(){}
	private FieldPath(IntPtr unmanaged) : base(unmanaged){}
}


