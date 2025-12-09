// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Extensions.Math;
using System.Text;

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

partial class USceneComponent
{
	
	public Transform GetComponentTransform()
	{
		MasterAlcCache.GuardInvariant();
		InternalGetComponentTransform(out var result);
		return result;
	}
	
	public Vector GetComponentLocation()
	{
		MasterAlcCache.GuardInvariant();
		InternalGetComponentLocation(out var result);
		return result;
	}
	
	public Rotator GetComponentRotation()
	{
		MasterAlcCache.GuardInvariant();
		InternalGetComponentRotation(out var result);
		return result;
	}
	
	public Vector GetComponentScale()
	{
		MasterAlcCache.GuardInvariant();
		InternalGetComponentScale(out var result);
		return result;
	}
	
	public Transform GetSocketTransform(string socketName, ERelativeTransformSpace transformSpace = ERelativeTransformSpace.RTS_World)
	{
		MasterAlcCache.GuardInvariant();
		InternalGetSocketTransform(socketName, transformSpace, out var result);
		return result;
	}
	
	public Vector GetSocketLocation(string socketName, ERelativeTransformSpace transformSpace = ERelativeTransformSpace.RTS_World)
	{
		MasterAlcCache.GuardInvariant();
		InternalGetSocketLocation(socketName, transformSpace, out var result);
		return result;
	}
	
	public Rotator GetSocketRotation(string socketName, ERelativeTransformSpace transformSpace = ERelativeTransformSpace.RTS_World)
	{
		MasterAlcCache.GuardInvariant();
		InternalGetSocketRotation(socketName, transformSpace, out var result);
		return result;
	}
	
	public Vector GetSocketScale(string socketName, ERelativeTransformSpace transformSpace = ERelativeTransformSpace.RTS_World)
	{
		MasterAlcCache.GuardInvariant();
		InternalGetSocketScale(socketName, transformSpace, out var result);
		return result;
	}
	
	public void SetWorldTransform(in Transform newTransform, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetWorldTransform(in newTransform, sweep, teleport);
	}
	
	public void SetWorldLocation(in Vector newLocation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetWorldLocation(in newLocation, sweep, teleport);
	}
	
	public void SetWorldRotation(in Rotator newRotation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetWorldRotation(in newRotation, sweep, teleport);
	}
	
	public void SetWorldLocationAndRotation(in Vector newLocation, in Rotator newRotation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetWorldLocationAndRotation(in newLocation, in newRotation, sweep, teleport);
	}
	
	public void SetWorldScale(in Vector newScale)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetWorldScale(in newScale);
	}
	
	public void SetRelativeTransform(in Transform newRelativeTransform, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetRelativeTransform(in newRelativeTransform, sweep, teleport);
	}
	
	public void SetRelativeLocation(in Vector newRelativeLocation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetRelativeLocation(in newRelativeLocation, sweep, teleport);
	}
	
	public void SetRelativeRotation(in Rotator newRelativeRotation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetRelativeRotation(in newRelativeRotation, sweep, teleport);
	}
	
	public void SetRelativeScale(in Vector newRelativeScale)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetRelativeScale(in newRelativeScale);
	}
	
	public void AddWorldTransform(in Transform deltaTransform, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddWorldTransform(in deltaTransform, sweep, teleport);
	}
	
	public void AddWorldTransformKeepScale(in Transform deltaTransform, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddWorldTransformKeepScale(in deltaTransform, sweep, teleport);
	}
	
	public void AddWorldOffset(in Vector deltaLocation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddWorldOffset(in deltaLocation, sweep, teleport);
	}
	
	public void AddWorldRotation(in Rotator deltaRotation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddWorldRotation(in deltaRotation, sweep, teleport);
	}
	
	public void AddLocalTransform(in Transform deltaTransform, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddLocalTransform(in deltaTransform, sweep, teleport);
	}
	
	public void AddLocalOffset(in Vector deltaLocation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddLocalOffset(in deltaLocation, sweep, teleport);
	}
	
	public void AddLocalRotation(in Rotator deltaRotation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddLocalRotation(in deltaRotation, sweep, teleport);
	}
	
	private unsafe void InternalGetComponentTransform(out Transform transform)
		=> SceneComponent_Interop.GetComponentTransform(ConjugateHandle.FromConjugate(this), out transform);
	
	private unsafe void InternalGetComponentLocation(out Vector location)
		=> SceneComponent_Interop.GetComponentLocation(ConjugateHandle.FromConjugate(this), out location);
	
	private unsafe void InternalGetComponentRotation(out Rotator rotation)
		=> SceneComponent_Interop.GetComponentRotation(ConjugateHandle.FromConjugate(this), out rotation);
	
	private unsafe void InternalGetComponentScale(out Vector scale)
		=> SceneComponent_Interop.GetComponentScale(ConjugateHandle.FromConjugate(this), out scale);
	
	private unsafe void InternalGetSocketTransform(string socketName, ERelativeTransformSpace transformSpace, out Transform transform)
	{
		fixed (char* socketNameBuffer = socketName)
		{
			SceneComponent_Interop.GetSocketTransform(ConjugateHandle.FromConjugate(this), socketNameBuffer, Convert.ToInt32(transformSpace), out transform);
		}
	}
	
	private unsafe void InternalGetSocketLocation(string socketName, ERelativeTransformSpace transformSpace, out Vector location)
	{
		fixed (char* socketNameBuffer = socketName)
		{
			SceneComponent_Interop.GetSocketLocation(ConjugateHandle.FromConjugate(this), socketNameBuffer, Convert.ToInt32(transformSpace), out location);
		}
	}
	
	private unsafe void InternalGetSocketRotation(string socketName, ERelativeTransformSpace transformSpace, out Rotator rotation)
	{
		fixed (char* socketNameBuffer = socketName)
		{
			SceneComponent_Interop.GetSocketRotation(ConjugateHandle.FromConjugate(this), socketNameBuffer, Convert.ToInt32(transformSpace), out rotation);
		}
	}
	
	private unsafe void InternalGetSocketScale(string socketName, ERelativeTransformSpace transformSpace, out Vector scale)
	{
		fixed (char* socketNameBuffer = socketName)
		{
			SceneComponent_Interop.GetSocketScale(ConjugateHandle.FromConjugate(this), socketNameBuffer, Convert.ToInt32(transformSpace), out scale);
		}
	}
	
	private unsafe void InternalSetWorldTransform(in Transform newTransform, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.SetWorldTransform(ConjugateHandle.FromConjugate(this), in newTransform, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalSetWorldLocation(in Vector newLocation, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.SetWorldLocation(ConjugateHandle.FromConjugate(this), in newLocation, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalSetWorldRotation(in Rotator newRotation, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.SetWorldRotation(ConjugateHandle.FromConjugate(this), in newRotation, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalSetWorldLocationAndRotation(in Vector newLocation, in Rotator newRotation, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.SetWorldLocationAndRotation(ConjugateHandle.FromConjugate(this), newLocation, in newRotation, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalSetWorldScale(in Vector newScale)
		=> SceneComponent_Interop.SetWorldScale(ConjugateHandle.FromConjugate(this), in newScale);
	
	private unsafe void InternalSetRelativeTransform(in Transform newRelativeTransform, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.SetRelativeTransform(ConjugateHandle.FromConjugate(this), in newRelativeTransform, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalSetRelativeLocation(in Vector newRelativeLocation, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.SetRelativeLocation(ConjugateHandle.FromConjugate(this), in newRelativeLocation, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalSetRelativeRotation(in Rotator newRelativeRotation, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.SetRelativeRotation(ConjugateHandle.FromConjugate(this), in newRelativeRotation, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalSetRelativeScale(in Vector newRelativeScale)
		=> SceneComponent_Interop.SetRelativeScale(ConjugateHandle.FromConjugate(this), in newRelativeScale);
	
	private unsafe void InternalAddWorldTransform(in Transform deltaTransform, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.AddWorldTransform(ConjugateHandle.FromConjugate(this), in deltaTransform, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalAddWorldTransformKeepScale(in Transform deltaTransform, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.AddWorldTransformKeepScale(ConjugateHandle.FromConjugate(this), in deltaTransform, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalAddWorldOffset(in Vector deltaLocation, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.AddWorldOffset(ConjugateHandle.FromConjugate(this), in deltaLocation, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalAddWorldRotation(in Rotator deltaRotation, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.AddWorldRotation(ConjugateHandle.FromConjugate(this), in deltaRotation, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalAddLocalTransform(in Transform deltaTransform, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.AddLocalTransform(ConjugateHandle.FromConjugate(this), in deltaTransform, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalAddLocalOffset(in Vector deltaLocation, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.AddLocalOffset(ConjugateHandle.FromConjugate(this), in deltaLocation, Convert.ToByte(sweep), Convert.ToByte(teleport));
	
	private unsafe void InternalAddLocalRotation(in Rotator deltaRotation, bool sweep, ETeleportType teleport)
		=> SceneComponent_Interop.AddLocalRotation(ConjugateHandle.FromConjugate(this), in deltaRotation, Convert.ToByte(sweep), Convert.ToByte(teleport));

}




