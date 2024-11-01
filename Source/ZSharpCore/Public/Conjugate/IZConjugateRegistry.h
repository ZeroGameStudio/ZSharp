// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	/**
	 * Registry maintains a specific kind of conjugates.
	 * Generally you don't need to care about this
	 * because Z# has implemented necessary conjugates for interacting with Unreal Engine.
	 *
	 * [Framework Interface] - you should just implement this and have no reason to consume this.
	 */
	class IZConjugateRegistry
	{
		
	public:
		/**
		 * Called when owning Master ALC is unloading.
		 * Should release all RED conjugates (BLACK conjugates are released immediately by managed side after this).
		 */
		virtual void Release() = 0;

		/**
		 * Called when owning Master ALC is building a BLACK conjugate managed by this registry.
		 * Should create unmanaged part of this conjugate and add it into your inner storage for retrieving.
		 * 
		 * @param userdata A transparent additional information about this conjugate.
		 *                 This is created and passed to Master ALC by your managed part (often in constructor).
		 * @return Pointer to the newly created unmanaged part (also known as conjugate handle).
		 */
		virtual void* BuildConjugate(void* userdata) = 0;

		/**
		 * Called when owning Master ALC is releasing a BLACK conjugate managed by this registry.
		 * Should remove unmanaged part of this conjugate from your inner storage and destroy it.
		 * 
		 * @param unmanaged Pointer to the unmanaged part (also known as conjugate handle).
		 */
		virtual void ReleaseConjugate(void* unmanaged) = 0;

		/**
		 * Called when owning Master ALC is pushing red frame.
		 * Should push a frame into your inner storage for capturing incoming RED conjugates.
		 */
		virtual void PushRedFrame() = 0;

		/**
		 * Called when owning Master ALC is popping red frame.
		 * Should pop the top frame of your inner storage and release any conjugates captured by this frame.
		 */
		virtual void PopRedFrame() = 0;

	public:
		virtual ~IZConjugateRegistry(){}
		
	};
}


