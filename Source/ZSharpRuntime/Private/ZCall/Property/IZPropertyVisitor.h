// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZCallBufferSlot;
	
	class IZPropertyVisitor
	{

	public:
		static TUniquePtr<IZPropertyVisitor> Create(const FProperty* prop);

	public:
		virtual ~IZPropertyVisitor(){}

	public:
		virtual bool IsPrimitive() const = 0;
		virtual bool IsValueSemantics() const = 0;

	public:
		virtual void InitializeValue(void* dest) const = 0;
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const = 0;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const = 0;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const = 0;

	public:
		template <typename T = void>
		T* ContainerPtrToValuePtr(const void* container) const { return const_cast<T*>(GetUnderlyingProperty()->ContainerPtrToValuePtr<T>(container)); }

	public:
		bool IsObjectSemantics() const { return !IsValueSemantics(); }

		void InitializeValue_InContainer(void* dest) const { InitializeValue(ContainerPtrToValuePtr(dest)); }
		void GetValue_InContainer(const void* src, FZCallBufferSlot& dest) const { GetValue(ContainerPtrToValuePtr(src), dest); }
		void GetRef_InContainer(const void* src, FZCallBufferSlot& dest) const { GetRef(ContainerPtrToValuePtr(src), dest); }
		void SetValue_InContainer(void* dest, const FZCallBufferSlot& src) const { SetValue(ContainerPtrToValuePtr(dest), src); }

	protected:
		virtual const FProperty* GetUnderlyingProperty() const = 0;
		
	};
}


