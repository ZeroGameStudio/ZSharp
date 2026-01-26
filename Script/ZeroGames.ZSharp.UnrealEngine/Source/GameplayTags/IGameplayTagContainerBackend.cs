// Copyright Zero Games. All Rights Reserved.

#if UE_MODULE_GAMEPLAYTAGS

namespace ZeroGames.ZSharp.UnrealEngine.GameplayTags;

public interface IGameplayTagContainerBackend
{
    bool HasTag(GameplayTag tagToCheck, GameplayTagContainerToken token);
    bool HasTagExact(GameplayTag tagToCheck, GameplayTagContainerToken token);
    bool HasAny(ReadOnlyGameplayTagContainer containerToCheck, GameplayTagContainerToken token);
    bool HasAnyExact(ReadOnlyGameplayTagContainer containerToCheck, GameplayTagContainerToken token);
    bool HasAll(ReadOnlyGameplayTagContainer containerToCheck, GameplayTagContainerToken token);
    bool HasAllExact(ReadOnlyGameplayTagContainer containerToCheck, GameplayTagContainerToken token);
    GameplayTagContainer Filter(ReadOnlyGameplayTagContainer other, GameplayTagContainerToken token);
    GameplayTagContainer FilterExact(ReadOnlyGameplayTagContainer other, GameplayTagContainerToken token);
    bool MatchesQuery(GameplayTagQuery query, GameplayTagContainerToken token);
    
    void AddTag(GameplayTag tag, GameplayTagContainerToken token);
    bool AddLeafTag(GameplayTag tag, GameplayTagContainerToken token);
    bool RemoveTag(GameplayTag tag, GameplayTagContainerToken token);
    void RemoveTags(ReadOnlyGameplayTagContainer tags, GameplayTagContainerToken token);
    void AppendTags(ReadOnlyGameplayTagContainer other, GameplayTagContainerToken token);
    void AppendMatchingTags(ReadOnlyGameplayTagContainer otherA, ReadOnlyGameplayTagContainer otherB, GameplayTagContainerToken token);
    void Reset(GameplayTagContainerToken token);
    
    int32 GetNum(GameplayTagContainerToken token);
    GameplayTagContainer GetParents(GameplayTagContainerToken token);
    IEnumerable<GameplayTag> GetTags(GameplayTagContainerToken token);

    void Recycle(GameplayTagContainerToken token);
    
    GameplayTagContainerToken Token { get; }
}

#endif


