// Copyright Zero Games. All Rights Reserved.

/*
 * Namespace of a dynamic UClass must be <AssemblyName>.<ModuleName>.
 */
namespace Game.${GameModuleName};

/*
 * Define a UClass that can be inherited by blueprint.
 * Place this class into map, play in editor, and you will see a rotating cube.
 * Behavior of [PropertyDefaultOverride] is same as directly set property value in C++ constructor, but only valid for UProperty.
 *
 * Bug: Directly set StaticMesh property will cause an ensure.
 */
[UClass, Blueprintable]
[PropertyDefaultOverride(Property = "StaticMesh.StaticMesh", Default = "/Engine/BasicShapes/Cube.Cube")]
[PropertyDefaultOverride(Property = "PrimaryActorTick.bCanEverTick", Default = true)]
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
	protected virtual partial void SayHello_Implementation()
	{
		KismetSystemLibrary.PrintString(this);
	}

	/*
	 * Define a pure virtual UFunction, which means you can only override it in blueprint.
	 */
	[UFunction, BlueprintImplementableEvent, BlueprintCallable]
	public partial void ChangeColor();
	
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
		 *
		 * By the way, in Z# interop system, a native object is either black or red.
		 */
		Vector startPos = K2_GetActorLocation();
		_startPos = new(startPos.X, startPos.Y, startPos.Z);
		ensure(startPos.IsRed && _startPos.IsBlack);
		
		/*
		 * Register a timer to rotate the cube.
		 * ReactiveLifecycle.Explicit(this) binds the timer's lifecycle with us,
		 * so once we are destroyed, the timer will automatically stop as well.
		 */
		GlobalTimerScheduler.Paused.Register((deltaTime, _) =>
		{
			TickRotate((float)deltaTime.Ticks / TimeSpan.TicksPerSecond);
		}, null, TimeSpan.FromMilliseconds(10), true, ReactiveLifecycle.Explicit(this));

		/*
		 * This will suspend the function for 1s but won't block the game thread.
		 */
		await ZeroTask.Delay(1000);
		
		/*
		 * This log will appear after 1s.
		 */
		UE_LOG(LogTemp, $"{GetPathName()} Begin Play!");
		
		await ZeroTask.Delay(2000);
		
		/*
		 * This actor's lifespan is totally 1s + 2s + 2s = 5s.
		 */
		UE_LOG(LogTemp, $"{GetPathName()} ??????????????????????????????????????");
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
		double offsetZ = 100 * Math.Sin(_accumulatedTime);
		K2_SetActorLocation(new(_startPos.X, _startPos.Y, _startPos.Z + offsetZ), false, out _, true);
	}

	private void TickRotate(float deltaTime)
	{
		K2_AddActorWorldRotation(new(0, RotationSpeed * deltaTime, 0), false, out _, true);
	}
	
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
	[UProperty(Default = 180), EditAnywhere]
	public partial double RotationSpeed { get; set; }

	/*
	 * Z# keeps lifecycle synchronized between native object and script object,
	 * so the script object can also hold states.
	 */
	private Vector _startPos = null!;
	private float _accumulatedTime;

}


