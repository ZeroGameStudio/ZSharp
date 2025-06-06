﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZCallBufferSlot;
	
	class ZSHARPRUNTIME_API IZPropertyVisitor
	{

	public:
		static TUniquePtr<IZPropertyVisitor> Create(const FProperty* prop);

	public:
		virtual ~IZPropertyVisitor(){}

	public:
		virtual void InitializeValue(void* dest) const = 0;
		virtual void DestructValue(void* dest) const = 0;
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const = 0;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const = 0;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const = 0;

	public:
		template <typename T = void>
		T* ContainerPtrToValuePtr(const void* container, int32 index) const { return const_cast<T*>(GetUnderlyingProperty()->ContainerPtrToValuePtr<T>(container, index)); }

	public:
		void InitializeValue_InContainer(void* dest) const;
		void DestructValue_InContainer(void* dest) const;
		void GetValue_InContainer(const void* src, FZCallBufferSlot& dest, int32 index) const;
		void GetRef_InContainer(const void* src, FZCallBufferSlot& dest, int32 index) const;
		void SetValue_InContainer(void* dest, const FZCallBufferSlot& src, int32 index) const;
		void GetPropertyValue_InContainer(const void* src, FZCallBufferSlot& dest, int32 index) const;
		void SetPropertyValue_InContainer(void* dest, const FZCallBufferSlot& src, int32 index) const;

	public:
		virtual const FProperty* GetUnderlyingProperty() const = 0;
		
	};
}


