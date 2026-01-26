// Copyright Zero Games. All Rights Reserved.

#if UE_MODULE_GAMEPLAYTAGS

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.GameplayTags;

public class GameplayTagContainerBackend_UE : IGameplayTagContainerBackend
{

    public static GameplayTagContainerBackend_UE GetFromPool()
    {
        if (_pool.TryDequeue(out var backend))
        {
            backend.Initialize();
            return backend;
        }

        return new();
    }

    private GameplayTagContainerBackend_UE() => Initialize();

    [MemberNotNull(nameof(_inner))]
    private void Initialize()
    {
        _inner = new();
    }

    private void GuardToken(GameplayTagContainerToken token)
    {
        if (token != _token)
        {
            throw new ObjectDisposedException("Access recycled gameplay tag container.");
        }
    }

    private const int32 MAX_POOL_COUNT = 1024;
    private static readonly Queue<GameplayTagContainerBackend_UE> _pool = [];

    private FGameplayTagContainer _inner;
    private GameplayTagContainerToken _token = GameplayTagContainerToken.First;
    
    #region IGameplayTagContainerBackend Implementations
    
    public bool HasTag(GameplayTag tagToCheck, GameplayTagContainerToken token)
    {
        GuardToken(token);
        
        return UBlueprintGameplayTagLibrary.HasTag(_inner, tagToCheck, false);
    }

    public bool HasTagExact(GameplayTag tagToCheck, GameplayTagContainerToken token)
    {
        GuardToken(token);
        
        return UBlueprintGameplayTagLibrary.HasTag(_inner, tagToCheck, true);
    }

    public bool HasAny(ReadOnlyGameplayTagContainer containerToCheck, GameplayTagContainerToken token)
    {
        GuardToken(token);

        return UBlueprintGameplayTagLibrary.HasAnyTags(_inner, containerToCheck, false);
    }

    public bool HasAnyExact(ReadOnlyGameplayTagContainer containerToCheck, GameplayTagContainerToken token)
    {
        GuardToken(token);
        
        return UBlueprintGameplayTagLibrary.HasAnyTags(_inner, containerToCheck, true);
    }

    public bool HasAll(ReadOnlyGameplayTagContainer containerToCheck, GameplayTagContainerToken token)
    {
        GuardToken(token);
        
        return UBlueprintGameplayTagLibrary.HasAllTags(_inner, containerToCheck, false);
    }

    public bool HasAllExact(ReadOnlyGameplayTagContainer containerToCheck, GameplayTagContainerToken token)
    {
        GuardToken(token);
        
        return UBlueprintGameplayTagLibrary.HasAllTags(_inner, containerToCheck, true);
    }

    public GameplayTagContainer Filter(ReadOnlyGameplayTagContainer other, GameplayTagContainerToken token)
    {
        GuardToken(token);

        return UBlueprintGameplayTagLibrary.Filter(_inner, other, false);
    }

    public GameplayTagContainer FilterExact(ReadOnlyGameplayTagContainer other, GameplayTagContainerToken token)
    {
        GuardToken(token);
        
        return UBlueprintGameplayTagLibrary.Filter(_inner, other, true);
    }

    public bool MatchesQuery(GameplayTagQuery query, GameplayTagContainerToken token)
    {
        GuardToken(token);
        
        throw new NotImplementedException();
    }

    public void AddTag(GameplayTag tag, GameplayTagContainerToken token)
    {
        GuardToken(token);

        UBlueprintGameplayTagLibrary.AddGameplayTag(ref _inner, tag);
    }

    public bool AddLeafTag(GameplayTag tag, GameplayTagContainerToken token)
    {
        GuardToken(token);

        throw new NotImplementedException();
    }

    public bool RemoveTag(GameplayTag tag, GameplayTagContainerToken token)
    {
        GuardToken(token);

        return UBlueprintGameplayTagLibrary.RemoveGameplayTag(ref _inner, tag);
    }

    public void RemoveTags(ReadOnlyGameplayTagContainer tags, GameplayTagContainerToken token)
    {
        GuardToken(token);

        throw new NotImplementedException();
    }

    public void AppendTags(ReadOnlyGameplayTagContainer other, GameplayTagContainerToken token)
    {
        GuardToken(token);
        
        UBlueprintGameplayTagLibrary.AppendGameplayTagContainers(ref _inner, other);
    }

    public void AppendMatchingTags(ReadOnlyGameplayTagContainer otherA, ReadOnlyGameplayTagContainer otherB, GameplayTagContainerToken token)
    {
        GuardToken(token);

        throw new NotImplementedException();
    }

    public void Reset(GameplayTagContainerToken token)
    {
        GuardToken(token);

        _inner = new();
    }

    public int32 GetNum(GameplayTagContainerToken token)
    {
        GuardToken(token);

        return UBlueprintGameplayTagLibrary.GetNumGameplayTagsInContainer(_inner);
    }

    public GameplayTagContainer GetParents(GameplayTagContainerToken token)
    {
        GuardToken(token);

        throw new NotImplementedException();
    }

    public IEnumerable<GameplayTag> GetTags(GameplayTagContainerToken token)
    {
        GuardToken(token);

        return _inner.GameplayTags_Copy.Select(tag => (GameplayTag)tag);
    }

    public void Recycle(GameplayTagContainerToken token)
    {
        GuardToken(token);
        
        // Check token overflow.
        if ((_token = _token.Next).IsValid && _pool.Count < MAX_POOL_COUNT)
        {
            _pool.Enqueue(this);
        }
    }

    GameplayTagContainerToken IGameplayTagContainerBackend.Token => _token;

    #endregion

}

#endif


