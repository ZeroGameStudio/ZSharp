// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveScriptStruct.h"

ZSharp::FZSelfDescriptiveScriptStruct::UnderlyingInstanceType* ZSharp::FZSelfDescriptiveScriptStruct::NewUnderlyingInstance(const DescriptorType* descriptor)
{
	void* instance = FMemory::Malloc(descriptor->GetStructureSize(), descriptor->GetMinAlignment());
	descriptor->InitializeStruct(instance);

	return instance;
}

void ZSharp::FZSelfDescriptiveScriptStruct::DeleteUnderlyingInstance()
{
	Descriptor->DestroyStruct(UnderlyingInstance);
	FMemory::Free(UnderlyingInstance);
}


