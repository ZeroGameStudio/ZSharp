﻿// Copyright Zero Games. All Rights Reserved.

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
		virtual void InitializeValue_InContainer(void* dest) const = 0;
		virtual void GetValue_InContainer(const void* src, FZCallBufferSlot& dest) const = 0;
		virtual void GetRef_InContainer(const void* src, FZCallBufferSlot& dest) const = 0;
		virtual void SetValue_InContainer(void* dest, const FZCallBufferSlot& src) const = 0;

	public:
		bool IsObjectSemantics() const { return !IsValueSemantics(); }
		
	};
}

