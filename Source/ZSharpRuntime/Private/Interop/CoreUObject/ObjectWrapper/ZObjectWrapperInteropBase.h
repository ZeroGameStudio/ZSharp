// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp
{
	template <typename TConjugateRegistry>
	struct TZObjectWrapperInteropBase
	{
		
		static void Copy(FZConjugateHandle self, FZConjugateHandle other)
		{
			GUARD
			(
				const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
				typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
				const typename TConjugateRegistry::ConjugateWrapperType* sdother = registry.ConjugateUnsafe(other);
				sdself->Set(sdother->Get());
			);
		}
		
		static uint8 Identical(FZConjugateHandle self, FZConjugateHandle other)
		{
			TRY
			{
				const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
				const typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
				const typename TConjugateRegistry::ConjugateWrapperType* sdother = registry.ConjugateUnsafe(other);
				return sdself == sdother || *sdself->GetUnderlyingInstance() == *sdother->GetUnderlyingInstance();
			}
			CATCHR(false)
		}
		
		static int32 Hash(FZConjugateHandle self)
		{
			TRY
			{
				const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
				const typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
				return GetTypeHash(sdself->GetUnderlyingInstance());
			}
			CATCHR(0)
		}
		
		static FZConjugateHandle Get(FZConjugateHandle self, uint8 evenIfGarbage)
		{
			TRY
			{
				const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
				const typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
				return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->Get(!!evenIfGarbage));
			}
			CATCHR({})
		}
		
		static void Set(FZConjugateHandle self, FZConjugateHandle target)
		{
			GUARD
			(
				const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
				typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
				sdself->Set(Cast<typename TConjugateRegistry::ConjugateWrapperType::ObjectType>(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(target)));
			);
		}
		
		static uint8 IsValid(FZConjugateHandle self, uint8 evenIfGarbage)
		{
			TRY
			{
				const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
				const typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
				return sdself->IsValid(!!evenIfGarbage);
			}
			CATCHR(false)
		}
		
		static uint8 IsNull(FZConjugateHandle self)
		{
			TRY
			{
				const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
				const typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
				return sdself->IsNull();
			}
			CATCHR(false)
		}
		
	};
}


