// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZCallDispatcher;

	/**
	 * Resolvers can be registered to Master ALC to produces dynamic ZCall by resolving its name.
	 */
	class IZCallResolver
	{

	public:
		/**
		 * Called when owning Master ALC is retrieving ZCall with unregistered name.
		 * Should resolve the given name and create a dispatcher for it.
		 * IMPORTANT: The returned dispatcher is owned by owning Master ALC so NEVER reuse dispatcher.
		 * 
		 * @param name The unique name of the ZCall.
		 * @return A dynamic ZCall dispatcher matches the given name.
		 *         If you can't resolve the given name, return nullptr
		 *         and the request will be forwarded to the next resolver.
		 */
		virtual IZCallDispatcher* Resolve(const FString& name) const = 0;
		
	public:
		virtual ~IZCallResolver(){}
		
	};
}


