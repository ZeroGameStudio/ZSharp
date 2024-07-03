// Copyright Zero Games. All Rights Reserved.


#include "ZCall/Conjugate/ZConjugateRegistryBase.h"

void ZSharp::FZConjugateRegistryBase::Release()
{
	TArray<void*> conjugates;
	GetAllConjugates(conjugates);
	for (const auto conjugate : conjugates)
	{
		ReleaseConjugate(conjugate);
	}
}

void ZSharp::FZConjugateRegistryBase::PushRedFrame()
{
	RedStack.Push({});
}

void ZSharp::FZConjugateRegistryBase::PopRedFrame()
{
	ZConjugateRegistryBase_Private::FZRedFrame frame = RedStack.Pop();
	for (const auto& conjugate : frame.CapturedConjugates)
	{
		ReleaseConjugate(conjugate);
	}
}

void ZSharp::FZConjugateRegistryBase::CaptureConjugate(void* unmanaged)
{
	RedStack.Top().CapturedConjugates.Emplace(unmanaged);
}


