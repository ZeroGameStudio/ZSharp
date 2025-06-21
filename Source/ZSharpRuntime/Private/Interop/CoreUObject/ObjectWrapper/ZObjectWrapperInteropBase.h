// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

namespace ZSharp
{
	template <typename TConjugateRegistry>
	struct TZObjectWrapperInteropBase
	{
		
		static void Copy(FZConjugateHandle self, FZConjugateHandle other)
		{
			const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
			typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
			const typename TConjugateRegistry::ConjugateWrapperType* sdother = registry.ConjugateUnsafe(other);
			sdself->Set(sdother->Get());
		}
		
		static uint8 Identical(FZConjugateHandle self, FZConjugateHandle other)
		{
			const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
			const typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
			const typename TConjugateRegistry::ConjugateWrapperType* sdother = registry.ConjugateUnsafe(other);
			return sdself == sdother || *sdself->GetUnderlyingInstance() == *sdother->GetUnderlyingInstance();
		}
		
		static int32 Hash(FZConjugateHandle self)
		{
			const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
			const typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
			return GetTypeHash(sdself->GetUnderlyingInstance());
		}
		
		static FZConjugateHandle Get(FZConjugateHandle self, uint8 evenIfGarbage)
		{
			const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
			const typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
			return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->Get(!!evenIfGarbage));
		}
		
		static void Set(FZConjugateHandle self, FZConjugateHandle target)
		{
			const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
            typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
			sdself->Set(Cast<typename TConjugateRegistry::ConjugateWrapperType::ObjectType>(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(target)));
		}
		
		static uint8 IsValid(FZConjugateHandle self, uint8 evenIfGarbage)
		{
			const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
			const typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
			return sdself->IsValid(!!evenIfGarbage);
		}
		
		static uint8 IsNull(FZConjugateHandle self)
		{
			const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>();
			const typename TConjugateRegistry::ConjugateWrapperType* sdself = registry.ConjugateUnsafe(self);
			return sdself->IsNull();
		}
		
	};
}


