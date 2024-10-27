// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZConjugateRegistry;
	
	template <typename T>
	concept CZConjugateRegistryImpl = static_cast<bool>(TPointerIsConvertibleFromTo<T, IZConjugateRegistry>::Value);
}


