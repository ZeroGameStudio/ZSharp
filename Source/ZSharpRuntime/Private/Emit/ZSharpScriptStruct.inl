// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpStruct.inl"

namespace ZSharp::ZSharpScriptStruct_Private
{
	// Dynamic ICppStructOps implementation for all Z# script structs.
	struct FZSharpStructOps final : public UScriptStruct::ICppStructOps
	{
		
		explicit FZSharpStructOps(const UScriptStruct* scriptStruct, const FZSharpScriptStruct* zsstrct, int32 size, int32 align)
			: ICppStructOps(size, align)
			, ScriptStruct(scriptStruct)
			, ZSharpScriptStruct(zsstrct)
		{
			Capabilities.HasSerializerObjectReferences = EPropertyObjectReferenceType::Conservative;
			Capabilities.HasDestructor = true;
		}

		void Fixup()
		{
			Capabilities.ComputedPropertyFlags =
				(ScriptStruct->StructFlags & STRUCT_IsPlainOldData ? CPF_IsPlainOldData : CPF_None) |
				(ScriptStruct->StructFlags & STRUCT_NoDestructor ? CPF_NoDestructor : CPF_None) |
				(ScriptStruct->StructFlags & STRUCT_ZeroConstructor ? CPF_ZeroConstructor : CPF_None);
		}

		virtual FCapabilities GetCapabilities() const override { return Capabilities; }

		virtual void Construct(void* dest) override
		{
			// We have to construct super manually because super may have user defined CppStructOps.
			if (UStruct* super = ScriptStruct->GetSuperStruct())
			{
				super->InitializeStruct(dest);
			}
			
			for (TFieldIterator<FProperty> it(ScriptStruct, EFieldIteratorFlags::ExcludeSuper); it; ++it)
			{
				check(it->ArrayDim == 1);
				it->InitializeValue_InContainer(dest);
			}

			ZSharpStruct_Private::SetupPropertyDefaults(ZSharpScriptStruct, dest);
		}

		virtual void ConstructForTests(void* dest) override { Construct(dest); }

		virtual void Destruct(void* dest) override
		{
			UStruct* super = ScriptStruct->GetSuperStruct();
			for (FProperty* property = ScriptStruct->DestructorLink; property; property = property->DestructorLinkNext)
			{
				// hit super.
				if (super && property->IsInContainer(super->GetStructureSize()))
				{
					break;
				}

				check(property->ArrayDim == 1);
				if (!property->HasAnyPropertyFlags(CPF_NoDestructor))
				{
					property->DestroyValue_InContainer(dest);
				}
			}

			// We have to destruct super manually because super may have user defined CppStructOps.
			if (super)
			{
				super->DestroyStruct(dest);
			}
		}

		virtual bool Serialize(FArchive& ar, void* data) override { checkNoEntry(); return false; }
		virtual bool Serialize(FStructuredArchive::FSlot slot, void* data) override { checkNoEntry(); return false; }
		virtual void PostSerialize(const FArchive& ar, void* data) override { checkNoEntry(); }
		virtual bool NetSerialize(FArchive& ar, UPackageMap* map, bool& outSuccess, void* data) override { checkNoEntry(); return false; }
		virtual bool NetDeltaSerialize(FNetDeltaSerializeInfo& deltaParms, void* data) override { checkNoEntry(); return false; }
		virtual void PostScriptConstruct(void* data) override { checkNoEntry(); }
		virtual void GetPreloadDependencies(void* data, TArray<UObject*>& outDeps) override { checkNoEntry(); }
		virtual bool Copy(void* dest, void const* src, int32 arrayDim) override { checkNoEntry(); return false; }
		virtual bool Identical(const void* a, const void* b, uint32 portFlags, bool& outResult) override { checkNoEntry(); return false; }
		virtual bool ExportTextItem(FString& valueStr, const void* propertyValue, const void* defaultValue, UObject* parent, int32 portFlags, UObject* exportRootScope) override { checkNoEntry(); return false; }
		virtual bool ImportTextItem(const TCHAR*& buffer, void* data, int32 portFlags, UObject* ownerObject, FOutputDevice* errorText) override { checkNoEntry(); return false; }
		virtual bool FindInnerPropertyInstance(FName propertyName, const void* data, const FProperty*& outProp, const void*& outData) const override { checkNoEntry(); return false; }
		virtual TPointerToAddStructReferencedObjects AddStructReferencedObjects() override { checkNoEntry(); return [](void*, FReferenceCollector&){}; }
		virtual bool SerializeFromMismatchedTag(FPropertyTag const& tag, FArchive& ar, void* data) override { checkNoEntry(); return false; }
		virtual bool StructuredSerializeFromMismatchedTag(FPropertyTag const& tag, FStructuredArchive::FSlot slot, void* data) override { checkNoEntry(); return false; }
		virtual uint32 GetStructTypeHash(const void* src) override { checkNoEntry(); return 0; }
		virtual void InitializeIntrusiveUnsetOptionalValue(void* data) const override { checkNoEntry(); }
		virtual bool IsIntrusiveOptionalValueSet(const void* data) const override { checkNoEntry(); return false; }
		virtual void ClearIntrusiveOptionalValue(void* data) const override { checkNoEntry(); }
		virtual bool IsIntrusiveOptionalSafeForGC() const override { checkNoEntry(); return false; }

#if WITH_EDITOR
		virtual bool CanEditChange(const FEditPropertyChain& propertyChain, const void* data) const override { checkNoEntry(); return false; }
#endif

		virtual EPropertyVisitorControlFlow Visit(FPropertyVisitorPath& path, const FPropertyVisitorData& data, const TFunctionRef<EPropertyVisitorControlFlow(const FPropertyVisitorPath&, const FPropertyVisitorData&)> func) const override { checkNoEntry(); return EPropertyVisitorControlFlow::StepOver; }
		virtual void* ResolveVisitedPathInfo(void* data, const FPropertyVisitorInfo& info) const override { checkNoEntry(); return nullptr; }

	private:
		const UScriptStruct* ScriptStruct;
		const FZSharpScriptStruct* ZSharpScriptStruct;
		FCapabilities Capabilities{};

	};
}


