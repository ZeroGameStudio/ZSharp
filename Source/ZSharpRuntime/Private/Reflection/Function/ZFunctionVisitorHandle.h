// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class FZFunctionVisitor;
	
	struct FZFunctionVisitorHandle
	{
		
		friend class FZFunctionVisitorRegistry;

	public:
		FZFunctionVisitorHandle() = default;

	public:
		const FZFunctionVisitor* Get() const;
		const bool IsValid() const { return !!Get(); }

		const FZFunctionVisitor* operator->() const { return Get(); }
		const FZFunctionVisitor& operator*() const { return *Get(); }
		operator bool() const { return IsValid(); }

		friend uint32 GetTypeHash(const FZFunctionVisitorHandle& self) { return GetTypeHash(self.Key); }
		friend bool operator==(const FZFunctionVisitorHandle& lhs, const FZFunctionVisitorHandle& rhs) { return lhs.Key == rhs.Key; }

	private:
		FZFunctionVisitorHandle(FObjectKey key) : Key(key){}
		
	private:
		FObjectKey Key;
		
	};
}


