// Copyright Zero Games. All Rights Reserved.

#include "Reflection/Wrapper/ZSelfDescriptiveScriptStruct.h"

void ZSharp::FZSelfDescriptiveScriptStruct::AddReferencedObjects(FReferenceCollector& collector)
{
	collector.AddPropertyReferencesWithStructARO(Descriptor, UnderlyingInstance);
}

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


