// Copyright Zero Games. All Rights Reserved.

namespace System.Runtime.CompilerServices;

/// <summary>
/// This exists to ensure the type implements range pattern.
/// </summary>
/// <typeparam name="TImpl">Implementation type</typeparam>
public interface ISliceable<out TImpl> where TImpl : ISliceable<TImpl>
{
	TImpl Slice(int32 start, int32 length);
}


