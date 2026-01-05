// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

public abstract class UnrealConjugateBase : IConjugate
{

    public void Dispose()
    {
        InternalDispose();
        SuppressFinalize();
    }
    
    public bool IsExpired => Unmanaged == DEAD_ADDR;

    public GCHandle GCHandle { get; }

    public IntPtr Unmanaged { get; private set; }
    public bool IsBlack { get; }
    public bool IsRed => !IsBlack;
    
    protected virtual void HandleExpired(){}

    protected void BuildConjugate_Black(IntPtr userdata)
    {
        if (Unmanaged != IntPtr.Zero)
        {
            throw new InvalidOperationException();
        }
        
        Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, userdata);
    }

    [DoesNotReturn]
    protected void NotifyInitializationFailed(Exception exception)
    {
        if (Unmanaged != IntPtr.Zero)
        {
            throw new InvalidOperationException();
        }
        
        ClearUnmanaged();
        SuppressFinalize();
        
        throw exception;
    }
    
    private protected UnrealConjugateBase()
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        IsBlack = true;
    }

    private protected UnrealConjugateBase(IntPtr unmanaged, bool needsHandleExpired = false)
    {
        GCHandle = GCHandle.Alloc(this);
        Unmanaged = unmanaged;
        IsBlack = false;
        _needsHandleExpired = needsHandleExpired;
        
        SuppressFinalize();
    }

    ~UnrealConjugateBase() => InternalDispose();
    
    void IConjugate.Release()
    {
        assert(IsRed);

        MarkAsDead();
    }

    private void InternalDispose()
    {
        if (!IsBlack)
        {
            throw new InvalidOperationException("Dispose black conjugate.");
        }

        if (!IsInGameThread)
        {
            MasterAlcCache.Instance.PushPendingDisposeConjugate(this);
            return;
        }
        
        if (_disposed)
        {
            return;
        }

        _disposed = true;

        if (IsExpired)
        {
            return;
        }
        
        MasterAlcCache.Instance.ReleaseConjugate(this);
        MarkAsDead();
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private void MarkAsDead()
    {
        assert(!IsExpired);
        
        ClearUnmanaged();

        if (_needsHandleExpired)
        {
            HandleExpired();
        }
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private void ClearUnmanaged()
    {
        GCHandle.Free();
        Unmanaged = DEAD_ADDR;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private void SuppressFinalize()
    {
        GC.SuppressFinalize(this);
    }
    
    private const IntPtr DEAD_ADDR = 0xDEAD;

    private readonly bool _needsHandleExpired;
    private bool _disposed;
    
}


