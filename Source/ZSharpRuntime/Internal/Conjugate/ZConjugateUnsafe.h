// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateMacros.h"
#include "ZConjugateRegistry_UObject.h"
#include "ZStrangeConjugateRegistries.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Trait/ZIsUClass.h"

namespace ZSharp
{
	template <typename T>
	T* ConjugateUnsafe(FZConjugateHandle handle);
	
	template <typename T>
	requires TZIsUClass_V<T>
	T* ConjugateUnsafe(FZConjugateHandle handle)
	{
#if ZSHARP_ENABLE_CONJUGATE_UNSAFE_FAST
		return static_cast<T*>(handle.Handle); // Caller guarantees it's runtime type.
#else
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<T>(handle);
#endif
	}
	
	template <typename T>
	requires TZIsUScriptStruct_V<T>
	T* ConjugateUnsafe(FZConjugateHandle handle)
	{
#if ZSHARP_ENABLE_CONJUGATE_UNSAFE_FAST
		return static_cast<T*>(handle.Handle); // Caller guarantees it's runtime type.
#else
		const FZSelfDescriptiveScriptStruct* sdss = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().ConjugateUnsafe(handle);
		return sdss ? sdss->GetTypedUnderlyingInstance<T>() : nullptr;
#endif
	}
}


