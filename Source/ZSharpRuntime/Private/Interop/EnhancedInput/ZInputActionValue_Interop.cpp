// Copyright Zero Games. All Rights Reserved.

#include "ZInputActionValue_Interop.h"

#include "InputActionValue.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"

void ZSharp::FZInputActionValue_Interop::Deconstruct(FZConjugateHandle self, double& x, double& y, double& z)
{
	GUARD
	(
		FZSelfDescriptiveScriptStruct* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().ConjugateUnsafe(self);
		if (!ensure(sdself->GetDescriptor() == FInputActionValue::StaticStruct()))
		{
			x = y = z = 0;
			return;
		}
		
		auto inputActionValue = sdself->GetTypedUnderlyingInstance<FInputActionValue>();
		auto value = inputActionValue->Get<FVector>();
		x = value.X;
		y = value.Y;
		z = value.Z;
	);
}


