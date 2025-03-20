// Copyright Zero Games. All Rights Reserved.

namespace System;

public interface ICloneable<out T> : ICloneable where T : ICloneable<T>
{
	new T Clone();
}


