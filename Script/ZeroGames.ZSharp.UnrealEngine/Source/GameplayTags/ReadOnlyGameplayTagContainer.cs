// Copyright Zero Games. All Rights Reserved.

#if UE_MODULE_GAMEPLAYTAGS

namespace ZeroGames.ZSharp.UnrealEngine.GameplayTags;

public readonly struct ReadOnlyGameplayTagContainer : ICloneable<GameplayTagContainer>
{
    
    public ReadOnlyGameplayTagContainer()
    {
        _backend = GameplayTagContainerBackend_UE.GetFromPool();
        _token = _backend.Token;
    }
    
    public static implicit operator ReadOnlyGameplayTagContainer(GameplayTagContainer container) => new(container._PRIVATE_Backend, container._PRIVATE_Token);
    public static implicit operator ReadOnlyGameplayTagContainer(ReadWriteGameplayTagContainer container) => new(container._PRIVATE_Backend, container._PRIVATE_Token);
    
    public static implicit operator FGameplayTagContainer(ReadOnlyGameplayTagContainer container)
    {
        FGameplayTagContainer result = new();
        foreach (var tag in container.Tags)
        {
            UBlueprintGameplayTagLibrary.AddGameplayTag(ref result, tag);
        }
        return result;
    }
    
    public bool HasTag(GameplayTag tagToCheck) => _backend.HasTag(tagToCheck, _token);
    public bool HasTagExact(GameplayTag tagToCheck) => _backend.HasTagExact(tagToCheck, _token);
    public bool HasAny(ReadOnlyGameplayTagContainer containerToCheck) => _backend.HasAny(containerToCheck, _token);
    public bool HasAnyExact(ReadOnlyGameplayTagContainer containerToCheck) => _backend.HasAllExact(containerToCheck, _token);
    public bool HasAll(ReadOnlyGameplayTagContainer containerToCheck) => _backend.HasAll(containerToCheck, _token);
    public bool HasAllExact(ReadOnlyGameplayTagContainer containerToCheck) => _backend.HasAllExact(containerToCheck, _token);
    public GameplayTagContainer Filter(ReadOnlyGameplayTagContainer other) => _backend.Filter(other, _token);
    public GameplayTagContainer FilterExact(ReadOnlyGameplayTagContainer other) => _backend.FilterExact(other, _token);
    public bool MatchesQuery(GameplayTagQuery query) => _backend.MatchesQuery(query, _token);
    
    public void AddTag(GameplayTag tag) => _backend.AddTag(tag, _token);
    public bool AddLeafTag(GameplayTag tag) => _backend.RemoveTag(tag, _token);
    public bool RemoveTag(GameplayTag tag) => _backend.RemoveTag(tag, _token);
    public void RemoveTags(ReadOnlyGameplayTagContainer tags) => _backend.RemoveTags(tags, _token);
    public void AppendTags(ReadOnlyGameplayTagContainer other) => _backend.AppendTags(other, _token);
    public void AppendMatchingTags(ReadOnlyGameplayTagContainer otherA, ReadOnlyGameplayTagContainer otherB) => _backend.AppendMatchingTags(otherA, otherB, _token);
    public void Reset() => _backend.Reset(_token);
    
    public int32 Num => _backend.GetNum(_token);
    public GameplayTagContainer Parents => _backend.GetParents(_token);
    public IEnumerable<GameplayTag> Tags => _backend.GetTags(_token);
    
    private ReadOnlyGameplayTagContainer(IGameplayTagContainerBackend backend, GameplayTagContainerToken token)
    {
        _backend = backend;
        _token = token;
    }
    
    private readonly IGameplayTagContainerBackend _backend;
    private readonly GameplayTagContainerToken _token;
    
    #region ICloneable<GameplayTagContainer> Implementations
    
    object ICloneable.Clone() => Clone();
    public GameplayTagContainer Clone()
    {
        GameplayTagContainer result = new();
        result.AppendTags(this);
        return result;
    }
    
    #endregion
}

#endif


