// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"
#include "IZAssemblyLoadContext.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "ZCall/ZCallHandle.h"
#include "Concept/CZConjugateRegistryImpl.h"

namespace ZSharp
{
	class IZCallDispatcher;
	class IZCallResolver;
	class IZConjugateRegistry;
	struct FZCallBuffer;
	struct FZRuntimeTypeUri;

	/**
  	 * Represents ZeroGames.ZSharp.Core.IMasterAssemblyLoadContext.Instance.
  	 * IMPORTANT: This is not thread-safe by design and all interfaces in this is limited to game thread only.
  	 *
  	 * [Library Interface] - you should just consume this and have no reason to implement this.
  	 */
	class IZMasterAssemblyLoadContext : public IZAssemblyLoadContext
	{
		
	public:
		/**
		 * Gets an instance of System.Type represented by the given URI in Master ALC or Default ALC.
		 * 
		 * @param uri The URI identifies a unique System.Type object.
		 * @return An opaque handle to the instance of System.Type.
		 */
		virtual FZRuntimeTypeHandle GetType(const FZRuntimeTypeUri& uri) const = 0;

	public:
		/**
		 * Gets the conjugate registry with the given ID.
		 * Generally you don't need to care about this
		 * because Z# has implemented necessary conjugates for interacting with Unreal Engine.
		 * 
		 * @param id ID of the conjugate registry.
		 * @return The conjugate registry with the given ID.
		 */
		virtual IZConjugateRegistry& GetConjugateRegistry(uint16 id) const = 0;

		/**
		 * Builds a RED conjugate.
		 * RED means: Unmanaged part creates first and managed part follows. Lifecycle is handled by unmanaged side.
		 * IMPORTANT: This should only interact with conjugate registries so don't call unless you know what you are doing!
		 * 
		 * @param unmanaged Pointer to the unmanaged part (also known as conjugate handle).
		 * @param type Type of the managed part to create.
		 * @return equals to unmanaged if managed part creates successfully, otherwise nullptr.
		 */
		virtual void* BuildConjugate(void* unmanaged, FZRuntimeTypeHandle type) = 0;

		/**
		 * Releases a RED conjugate.
		 * IMPORTANT: This should only interact with conjugate registries so don't call unless you know what you are doing!
		 *
		 * @param unmanaged Pointer to the unmanaged part (also known as conjugate handle).
		 */
		virtual void ReleaseConjugate(void* unmanaged) = 0;

		/**
		 * Pushes a frame on the top of the Red Stack.
		 * Red Stack will capture all newly created RED conjugates into it's top frame
		 * and will automatically release them when the frame is pop out.
		 * 
		 * Generally you should use FZRedFrameScope instead of this.
		 * If you push manually, it's up to you to balance the stack.
		 *
		 * IMPORTANT: Master ALC could not be unloaded if Red Stack is not empty.
		 */
		virtual void PushRedFrame() = 0;

		/**
		 * Pops a frame from the top of the Red Stack and release all RED conjugates captured by this frame.
		 * Generally you should use FZRedFrameScope instead of this.
		 */
		virtual void PopRedFrame() = 0;
		
	public:
		/**
		 * Registers a BLACK ZCall.
		 * BLACK means: Implementation of this ZCall is in unmanaged side.
		 * 
		 * @param dispatcher The dispatcher forwards the call to its implementation.
		 * @return An opaque handle to the ZCall.
		 */
		virtual FZCallHandle RegisterZCall(IZCallDispatcher* dispatcher) = 0;

		/**
 		 * Registers a resolver produces dynamic BLACK ZCall by resolving its name.
 		 * BLACK means: Implementation of this ZCall is in unmanaged side.
 		 * 
 		 * @param resolver The resolver produces dynamic BLACK ZCall.
 		 * @param priority The priority of this resolver in the chain of responsibility.
 		 *                 Smaller value means higher priority.
 		 */
		virtual void RegisterZCallResolver(IZCallResolver* resolver, uint64 priority) = 0;

		/**
		 * Gets the opaque handle to the RED ZCall with the given name.
		 * RED means: Implementation of this ZCall is in managed side.
		 * 
		 * @param name The unique name of the ZCall.
		 * @return The opaque handle to the ZCall with the given name.
		 */
		virtual FZCallHandle GetZCallHandle(const FString& name) = 0;

		/**
		 * Performs a RED ZCall.
		 * RED means: Implementation of this ZCall is in managed side.
		 * IMPORTANT: This requires at least one frame in the Red Stack.
		 * See EZCallErrorCode for more details.
		 * 
		 * @param handle The opaque handle to the ZCall.
		 * @param buffer Parameters of this ZCall, including inputs and outputs.
		 * @return The error code of the ZCall.
		 */
		virtual EZCallErrorCode ZCall(FZCallHandle handle, FZCallBuffer* buffer) = 0;
	
	public:
		/**
		 * Template version of GetConjugateRegistry().
		 * Conjugate registries must implement [static constexpr uint16 RegistryId;] pattern so we can retrieve ID from type.
		 * 
		 * @tparam T Type of the conjugate registry.
		 * @return The conjugate registry with the given type.
		 */
		template <CZConjugateRegistryImpl T>
		T& GetConjugateRegistry() const
		{
			return static_cast<T&>(GetConjugateRegistry(T::RegistryId));
		}
		
	};
}


