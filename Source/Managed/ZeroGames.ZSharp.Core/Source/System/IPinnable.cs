// Copyright Zero Games. All Rights Reserved.

namespace System.Runtime.CompilerServices;

/// <summary>
/// This exists to ensure the type implements pinnable pattern.
/// </summary>
/// <typeparam name="T">Pinnable reference type</typeparam>
public interface IPinnable<T>
{
	ref readonly T GetPinnableReference();
}


