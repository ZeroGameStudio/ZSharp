// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Extensions.Math;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

partial class FTransform
{

	public FTransform(FQuat rotation, FVector translation, FVector scale3D) : this()
	{
		Rotation = rotation;
		Translation = translation;
		Scale3D = scale3D;
	}
	
	public FTransform(FRotator rotation, FVector translation, FVector scale3D) : this(rotation.Quat, translation, scale3D){}
	
	public static implicit operator FTransform(Transform value) => new(value.Rotation, value.Translation, value.Scale);
	public static implicit operator Transform(FTransform value) => new(value.Rotation_Copy, value.Translation, value.Scale3D);
	
}


