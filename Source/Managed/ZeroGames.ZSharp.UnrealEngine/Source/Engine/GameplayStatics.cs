// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class GameplayStatics
{

	public static Character? GetPlayerCharacter(UnrealObject? worldContextObject, int32 playerIndex) => ZCallEx.ZCall("uf://Script/Engine.GameplayStatics:GetPlayerCharacter", worldContextObject, playerIndex, null)[2].ReadConjugate<Character>();

}