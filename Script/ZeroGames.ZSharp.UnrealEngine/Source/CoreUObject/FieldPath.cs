// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(51)]
[ConjugateKey("Unreal.FieldPath")]
public sealed class FFieldPath : UnrealConjugateBase, IConjugate<FFieldPath>
{
	public static FFieldPath BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	public FFieldPath(){}
	private FFieldPath(IntPtr unmanaged) : base(unmanaged){}
}


