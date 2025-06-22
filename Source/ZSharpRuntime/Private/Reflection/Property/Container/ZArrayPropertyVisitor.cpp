// Copyright Zero Games. All Rights Reserved.

#include "ZArrayPropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"

void ZSharp::FZArrayPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	const FZSelfDescriptiveScriptArray* sdsa = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().Conjugate(dest.ReadConjugate());
	if (!sdsa)
	{
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().Conjugate(UnderlyingArrayProperty->Inner, [src, this](FZSelfDescriptiveScriptArray& sdsa){ UnderlyingProperty->CopySingleValue(sdsa.GetUnderlyingInstance(), src); }));
	}
	else
	{
		auto srcEnumProperty = CastField<const FEnumProperty>(UnderlyingArrayProperty->Inner);
		auto destEnumProperty = CastField<const FEnumProperty>(sdsa->GetDescriptor());
		if (srcEnumProperty && destEnumProperty && srcEnumProperty->GetUnderlyingProperty()->GetClass() != destEnumProperty->GetUnderlyingProperty()->GetClass())
		{
			InternalCopyEnumArray(srcEnumProperty, src, destEnumProperty, sdsa->GetUnderlyingInstance());
		}
		else
		{
			check(sdsa->GetDescriptor()->GetClass() == UnderlyingArrayProperty->Inner->GetClass());
			UnderlyingProperty->CopySingleValue(sdsa->GetUnderlyingInstance(), src);
		}
	}
}

void ZSharp::FZArrayPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().Conjugate(UnderlyingArrayProperty->Inner, (FScriptArray*)src));
}

void ZSharp::FZArrayPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FZSelfDescriptiveScriptArray* sdsa = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().Conjugate(src.ReadConjugate());
	if (!sdsa)
	{
		UnderlyingProperty->InitializeValue(dest);
		return;
	}

	auto srcEnumProperty = CastField<const FEnumProperty>(sdsa->GetDescriptor());
	auto destEnumProperty = CastField<const FEnumProperty>(UnderlyingArrayProperty->Inner);
	if (srcEnumProperty && destEnumProperty && srcEnumProperty->GetUnderlyingProperty()->GetClass() != destEnumProperty->GetUnderlyingProperty()->GetClass())
	{
		InternalCopyEnumArray(srcEnumProperty, sdsa->GetUnderlyingInstance(), destEnumProperty, dest);
	}
	else
	{
		check(sdsa->GetDescriptor()->GetClass() == UnderlyingArrayProperty->Inner->GetClass());
		UnderlyingProperty->CopySingleValue(dest, sdsa->GetUnderlyingInstance());
	}
}

void ZSharp::FZArrayPropertyVisitor::InternalCopyEnumArray(const FEnumProperty* srcElementProperty, const void* src, const FEnumProperty* destElementProperty, void* dest) const
{
	FScriptArrayHelper srcHelper = FScriptArrayHelper::CreateHelperFormInnerProperty(srcElementProperty, src);
	FScriptArrayHelper destHelper = FScriptArrayHelper::CreateHelperFormInnerProperty(destElementProperty, dest);
	
	int32 num = srcHelper.Num();
	destHelper.EmptyAndAddValues(num);
	for (int32 i = 0; i < num; ++i)
	{
		void* srcValue = srcHelper.GetElementPtr(i);
		void* destValue = destHelper.GetElementPtr(i);
		FZReflectionHelper::ChangeEnumValueType(srcElementProperty, srcValue, destElementProperty, destValue);
	}
}


