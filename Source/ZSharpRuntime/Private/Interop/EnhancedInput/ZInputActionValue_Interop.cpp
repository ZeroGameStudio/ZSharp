// Copyright Zero Games. All Rights Reserved.

#include "ZInputActionValue_Interop.h"

#include "InputActionValue.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"

void ZSharp::FZInputActionValue_Interop::Deconstruct(FZConjugateHandle self, double& x, double& y, double& z)
{
	FZSelfDescriptiveScriptStruct* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().ConjugateUnsafe(self);
	if (!ensure(sdself->GetDescriptor() == FInputActionValue::StaticStruct()))
	{
		x = y = z = 0;
		return;
	}

	const auto inputActionValue = static_cast<FInputActionValue*>(sdself->GetUnderlyingInstance());
	const auto value = inputActionValue->Get<FVector>();
	x = value.X;
	y = value.Y;
	z = value.Z;
}


