﻿// Copyright Zero Games. All Rights Reserved.

/*
 * Namespace of a dynamic UClass must be <AssemblyName>.<ModuleName>.
 * <ModuleName> can be removed if it equals the last segment of <AssemblyName>
 */
namespace Game;

/*+
 * Define a UClass that can be inherited by blueprint.
 * Place this class into map, play in editor, and you will see a rotating cube.
 */
[UClass, Blueprintable]
public partial class MagicCube : Actor
{

	/*
	 * Define a pure UFunction.
	 */
	[UFunction, BlueprintPure]
	public Vector GetStartPos() => _startPos;

	/*
	 * Define a virtual UFunction, which means you can override it in either script or blueprint.
	 */
	[UFunction, BlueprintNativeEvent, BlueprintCallable]
	public partial void SayHello();

	/*
	 * Define a pure virtual UFunction, which means you can only override it in blueprint.
	 */
	[UFunction, BlueprintImplementableEvent, BlueprintCallable]
	public partial void ChangeColor();
	
	/*
	 * Define a DSO as UProperty.
	 * Behavior of [DefaultSubobject] is same as calling CreateDefaultSubobject() in C++ class constructor.
	 * Behavior of [RootComponent] is same as calling SetRootComponent() in C++ class constructor.
	 */
	[UProperty, DefaultSubobject, RootComponent, VisibleAnywhere]
	public partial StaticMeshComponent StaticMesh { get; set; }
	
	/*
	 * Define a UProperty that can be edited in property editor.
	 */
	[UProperty, DefaultValue(180), EditAnywhere]
	public partial double RotationSpeed { get; set; }

	/*
	 * Override 'red' constructor, which runs when native UObject is created and Master ALC reloads.
	 */
	public MagicCube(IntPtr unmanaged) : base(unmanaged)
	{
		_translationSpeed = 100f;
		_startPos = new();
	}
	
	protected virtual partial void SayHello_Implementation()
	{
		KismetSystemLibrary.PrintString(this);
	}
	
	protected override void UserConstructionScript_Implementation()
	{
		StaticMesh.SetStaticMesh(LoadObject<StaticMesh>("/Engine/BasicShapes/Cube.Cube"));
	}

	/*
	 * Override an existing BlueprintImplementableEvent ReceiveBeginPlay.
	 * Since it is an override other than a definition, you should not add [UFunction] attribute on it.
	 */
	protected override async void ReceiveBeginPlay_Implementation()
	{
		/*
		 * Native object returned by function is 'red',
		 * which means it's lifecycle is owned by C++ and can be released at any time.
		 * So if you want to cache it in script, you should create a copy from script side.
		 *
		 * Native object created by script is 'black',
		 * which means it's lifecycle is owned by managed garbage collector,
		 * so it will be alive until it is not referenced by anyone.
		 */
		Vector startPos = K2_GetActorLocation();
		_startPos = startPos.Clone();
		ensure(startPos.IsRed);
		ensure(_startPos.IsBlack);
		ensure(_startPos == startPos);
		
		RotateAsync();

		/*
		 * This will suspend the function for 1s but won't block the game thread.
		 */
		await ZeroTask.Delay(1, Lifecycle);
		
		/*
		 * This log will appear after 1s.
		 */
		UE_LOG(LogTemp, $"{GetPathName()} Begin Play!");
		
		await ZeroTask.Delay(2, Lifecycle);
		
		/*
		 * This actor's lifespan is totally 1s + 2s + 2s = 5s.
		 */
		SetLifeSpan(2);
	}
	
	protected override void ReceiveEndPlay_Implementation(EEndPlayReason endPlayReason)
	{
		/*
		 * This log will appear when we are destroyed.
		 */
		UE_LOG(LogTemp, $"{GetPathName()} End Play!");
	}
	
	/*
	 * Unfortunately, Unreal Engine does not call the Native Actor's ReceiveTick.
	 * Since script class is native class, if you directly instantiate an Actor of this type, the function will not execute.
	 * However, if you create a Blueprint subclass of this type and instantiate it, the function will execute.
	 */
	protected override void ReceiveTick_Implementation(float deltaSeconds)
	{
		_accumulatedTime += deltaSeconds;
		double offsetZ = _translationSpeed * Math.Sin(_accumulatedTime);
		K2_SetActorLocation(new(_startPos.X, _startPos.Y, _startPos.Z + offsetZ), false, out _, true);
	}
	
	/*
	 * Define UClass constructor, which runs only when native UObject is created.
	 */
	[UClassConstructor]
	private void ConstructMagicCube()
	{
		PrimaryActorTick.bCanEverTick = true;
	}

	private async ZeroTask RotateAsync()
	{
		await foreach (var deltaTime in ZeroStream.EveryTick(Lifecycle))
		{
			K2_AddActorWorldRotation(new() { Yaw = RotationSpeed * deltaTime }, false, out _, true);
		}
	}

	/*
	 * Z# keeps lifecycle synchronized between native object and script object,
	 * so the script object can also hold states.
	 */
	private Vector _startPos;
	private float _translationSpeed;
	private float _accumulatedTime;

}


