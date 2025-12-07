// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Extensions.Math;

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

partial class AActor
{
	
	public Transform GetActorTransform()
	{
		MasterAlcCache.GuardInvariant();
		InternalGetActorTransform(out var result);
		return result;
	}
	
	public Vector GetActorLocation()
	{
		MasterAlcCache.GuardInvariant();
		InternalGetActorLocation(out var result);
		return result;
	}
	
	public Rotator GetActorRotation()
	{
		MasterAlcCache.GuardInvariant();
		InternalGetActorRotation(out var result);
		return result;
	}
	
	public Vector GetActorScale()
	{
		MasterAlcCache.GuardInvariant();
		InternalGetActorScale(out var result);
		return result;
	}

	public bool SetActorTransform(in Transform newTransform, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		return InternalSetActorTransform(in newTransform, sweep, teleport);
	}
	
	public bool SetActorLocation(in Vector newLocation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		return InternalSetActorLocation(in newLocation, sweep, teleport);
	}
	
	public bool SetActorRotation(in Rotator newRotation, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		return InternalSetActorRotation(in newRotation, teleport);
	}
	
	public bool SetActorLocationAndRotation(in Vector newLocation, in Rotator newRotation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		return InternalSetActorLocationAndRotation(in newLocation, in newRotation, sweep, teleport);
	}
	
	public void SetActorScale(in Vector newScale)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetActorScale(in newScale);
	}
	
	public void SetActorRelativeTransform(in Transform newRelativeTransform, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetActorRelativeTransform(in newRelativeTransform, sweep, teleport);
	}
	
	public void SetActorRelativeLocation(in Vector newRelativeLocation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetActorRelativeLocation(in newRelativeLocation, sweep, teleport);
	}
	
	public void SetActorRelativeRotation(in Rotator newRelativeRotation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetActorRelativeRotation(in newRelativeRotation, sweep, teleport);
	}
	
	public void SetActorRelativeScale(in Vector newRelativeScale)
	{
		MasterAlcCache.GuardInvariant();
		InternalSetActorRelativeScale(in newRelativeScale);
	}
	
	public void AddActorWorldTransform(in Transform deltaTransform, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddActorWorldTransform(in deltaTransform, sweep, teleport);
	}
	
	public void AddActorWorldTransformKeepScale(in Transform deltaTransform, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddActorWorldTransformKeepScale(in deltaTransform, sweep, teleport);
	}
	
	public void AddActorWorldOffset(in Vector deltaLocation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddActorWorldOffset(in deltaLocation, sweep, teleport);
	}
	
	public void AddActorWorldRotation(in Rotator deltaRotation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddActorWorldRotation(in deltaRotation, sweep, teleport);
	}
	
	public void AddActorLocalTransform(in Transform deltaTransform, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddActorLocalTransform(in deltaTransform, sweep, teleport);
	}
	
	public void AddActorLocalOffset(in Vector deltaLocation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddActorLocalOffset(in deltaLocation, sweep, teleport);
	}
	
	public void AddActorLocalRotation(in Rotator deltaRotation, bool sweep = false, ETeleportType teleport = ETeleportType.None)
	{
		MasterAlcCache.GuardInvariant();
		InternalAddActorLocalRotation(in deltaRotation, sweep, teleport);
	}
	
	private unsafe void InternalGetActorTransform(out Transform transform)
		=> Actor_Interop.GetActorTransform(ConjugateHandle.FromConjugate(this), out transform);
	
	private unsafe void InternalGetActorLocation(out Vector location)
		=> Actor_Interop.GetActorLocation(ConjugateHandle.FromConjugate(this), out location);
	
	private unsafe void InternalGetActorRotation(out Rotator rotation)
		=> Actor_Interop.GetActorRotation(ConjugateHandle.FromConjugate(this), out rotation);
	
	private unsafe void InternalGetActorScale(out Vector scale)
		=> Actor_Interop.GetActorScale(ConjugateHandle.FromConjugate(this), out scale);
	
	private unsafe bool InternalSetActorTransform(in Transform newTransform, bool sweep, ETeleportType teleport)
		=> Actor_Interop.SetActorTransform(ConjugateHandle.FromConjugate(this), in newTransform, Convert.ToByte(sweep), teleport) > 0;
	
	private unsafe bool InternalSetActorLocation(in Vector newLocation, bool sweep, ETeleportType teleport)
		=> Actor_Interop.SetActorLocation(ConjugateHandle.FromConjugate(this), newLocation, Convert.ToByte(sweep), teleport) > 0;
	
	private unsafe bool InternalSetActorRotation(in Rotator newRotation, ETeleportType teleport)
		=> Actor_Interop.SetActorRotation(ConjugateHandle.FromConjugate(this), in newRotation, teleport) > 0;
	
	private unsafe bool InternalSetActorLocationAndRotation(in Vector newLocation, in Rotator newRotation, bool sweep, ETeleportType teleport)
		=> Actor_Interop.SetActorLocationAndRotation(ConjugateHandle.FromConjugate(this), newLocation, in newRotation, Convert.ToByte(sweep), teleport) > 0;
	
	private unsafe void InternalSetActorScale(in Vector newScale)
		=> Actor_Interop.SetActorScale(ConjugateHandle.FromConjugate(this), in newScale);
	
	private unsafe void InternalSetActorRelativeTransform(in Transform newRelativeTransform, bool sweep, ETeleportType teleport)
		=> Actor_Interop.SetActorRelativeTransform(ConjugateHandle.FromConjugate(this), in newRelativeTransform, Convert.ToByte(sweep), teleport);
	
	private unsafe void InternalSetActorRelativeLocation(in Vector newRelativeLocation, bool sweep, ETeleportType teleport)
		=> Actor_Interop.SetActorRelativeLocation(ConjugateHandle.FromConjugate(this), in newRelativeLocation, Convert.ToByte(sweep), teleport);
	
	private unsafe void InternalSetActorRelativeRotation(in Rotator newRelativeRotation, bool sweep, ETeleportType teleport)
		=> Actor_Interop.SetActorRelativeRotation(ConjugateHandle.FromConjugate(this), in newRelativeRotation, Convert.ToByte(sweep), teleport);
	
	private unsafe void InternalSetActorRelativeScale(in Vector newRelativeScale)
		=> Actor_Interop.SetActorRelativeScale(ConjugateHandle.FromConjugate(this), in newRelativeScale);
	
	private unsafe void InternalAddActorWorldTransform(in Transform deltaTransform, bool sweep, ETeleportType teleport)
		=> Actor_Interop.AddActorWorldTransform(ConjugateHandle.FromConjugate(this), in deltaTransform, Convert.ToByte(sweep), teleport);
	
	private unsafe void InternalAddActorWorldTransformKeepScale(in Transform deltaTransform, bool sweep, ETeleportType teleport)
		=> Actor_Interop.AddActorWorldTransformKeepScale(ConjugateHandle.FromConjugate(this), in deltaTransform, Convert.ToByte(sweep), teleport);
	
	private unsafe void InternalAddActorWorldOffset(in Vector deltaLocation, bool sweep, ETeleportType teleport)
		=> Actor_Interop.AddActorWorldOffset(ConjugateHandle.FromConjugate(this), in deltaLocation, Convert.ToByte(sweep), teleport);
	
	private unsafe void InternalAddActorWorldRotation(in Rotator deltaRotation, bool sweep, ETeleportType teleport)
		=> Actor_Interop.AddActorWorldRotation(ConjugateHandle.FromConjugate(this), in deltaRotation, Convert.ToByte(sweep), teleport);
	
	private unsafe void InternalAddActorLocalTransform(in Transform deltaTransform, bool sweep, ETeleportType teleport)
		=> Actor_Interop.AddActorLocalTransform(ConjugateHandle.FromConjugate(this), in deltaTransform, Convert.ToByte(sweep), teleport);
	
	private unsafe void InternalAddActorLocalOffset(in Vector deltaLocation, bool sweep, ETeleportType teleport)
		=> Actor_Interop.AddActorLocalOffset(ConjugateHandle.FromConjugate(this), in deltaLocation, Convert.ToByte(sweep), teleport);
	
	private unsafe void InternalAddActorLocalRotation(in Rotator deltaRotation, bool sweep, ETeleportType teleport)
		=> Actor_Interop.AddActorLocalRotation(ConjugateHandle.FromConjugate(this), in deltaRotation, Convert.ToByte(sweep), teleport);

}


