// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class Actor
{

	public T? GetComponentByClass<T>() where T : ActorComponent
	{
		return (T?)GetComponentByClass(new((UnrealClass)typeof(T).GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!));
	}
	
}


