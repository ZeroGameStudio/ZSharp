// Copyright Zero Games. All Rights Reserved.


#include "ManagedDelegateProxy.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"

void UManagedDelegateProxy::BeginDestroy()
{
	Delegate.Free();
	
	UObject::BeginDestroy();
}

void UManagedDelegateProxy::ProcessEvent(UFunction* function, void* parms)
{
	if (function->GetName() != GET_FUNCTION_NAME_CHECKED(ThisClass, __ZStub))
	{
		return Super::ProcessEvent(function, parms);
	}
	
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (alc)
	{
		alc->PrepareForZCall();
		ZSharp::FZCallBuffer buffer;
		buffer.Slots = new ZSharp::FZCallBufferSlot[1];
		buffer.NumSlots = 1;
		buffer[0] = ZSharp::FZCallBufferSlot::FromGCHandle(Delegate);
		ZSharp::FZCallHandle handle = alc->GetZCallHandle("d://");
		alc->ZCall(handle, &buffer);
	}
}


