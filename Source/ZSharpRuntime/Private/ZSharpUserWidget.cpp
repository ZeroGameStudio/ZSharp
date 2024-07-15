// Copyright Zero Games. All Rights Reserved.


#include "ZSharpUserWidget.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZConjugateRegistry_UScriptStruct.h"
#include "ZCall/ZCallBuffer.h"

UZSharpUserWidget::UZSharpUserWidget(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	OnInitializedZCall.MethodName = "OnInitialized";
    ConstructZCall.MethodName = "Construct";
    DestructZCall.MethodName = "Destruct";
    TickZCall.MethodName = "Tick";
}

void UZSharpUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (alc)
	{
		alc->PrepareForZCall();
		ZSHARP_STACK_ALLOC_ZCALL_BUFFER(ZSharp::FZCallBufferSlot::FromConjugate(alc->GetConjugateRegistry<ZSharp::FZConjugateRegistry_UObject>().Conjugate(this)));

		ZSharp::FZCallHandle handle = alc->GetZCallHandle(*OnInitializedZCall);
		alc->ZCall(handle, &buffer);
	}
}

void UZSharpUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (alc)
	{
		alc->PrepareForZCall();
		ZSHARP_STACK_ALLOC_ZCALL_BUFFER(ZSharp::FZCallBufferSlot::FromConjugate(alc->GetConjugateRegistry<ZSharp::FZConjugateRegistry_UObject>().Conjugate(this)));

		ZSharp::FZCallHandle handle = alc->GetZCallHandle(*ConstructZCall);
		alc->ZCall(handle, &buffer);
	}
}

void UZSharpUserWidget::NativeDestruct()
{
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (alc)
	{
		alc->PrepareForZCall();
		ZSHARP_STACK_ALLOC_ZCALL_BUFFER(ZSharp::FZCallBufferSlot::FromConjugate(alc->GetConjugateRegistry<ZSharp::FZConjugateRegistry_UObject>().Conjugate(this)));

		ZSharp::FZCallHandle handle = alc->GetZCallHandle(*DestructZCall);
		alc->ZCall(handle, &buffer);
	}
	
	Super::NativeDestruct();
}

void UZSharpUserWidget::NativeTick(const FGeometry& geometry, float deltaTime)
{
	Super::NativeTick(geometry, deltaTime);

	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (alc)
	{
		alc->PrepareForZCall();
		ZSHARP_STACK_ALLOC_ZCALL_BUFFER(
			ZSharp::FZCallBufferSlot::FromConjugate(alc->GetConjugateRegistry<ZSharp::FZConjugateRegistry_UObject>().Conjugate(this)),
			ZSharp::FZCallBufferSlot::FromConjugate(alc->GetConjugateRegistry<ZSharp::FZConjugateRegistry_UScriptStruct>().Conjugate(FGeometry::StaticStruct(), &geometry)),
			ZSharp::FZCallBufferSlot::FromFloat(deltaTime));

		ZSharp::FZCallHandle handle = alc->GetZCallHandle(*TickZCall);
		alc->ZCall(handle, &buffer);
	}
}


