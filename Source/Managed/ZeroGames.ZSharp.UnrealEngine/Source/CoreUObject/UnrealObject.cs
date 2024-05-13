// // Copyright Zero Games. All Rights Reserved.
//
// using ZeroGames.ZSharp.Core;
// using ZeroGames.ZSharp.UnrealEngine.Core;
//
// namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;
//
// public class UnrealObject : UnrealClassExportedObjectBase, IConjugate<UnrealObject>
// {
//
//     public static UnrealObject BuildConjugate(IntPtr unmanaged) => new(unmanaged);
//
//     public unsafe UnrealObject Class
//     {
//         get
//         {
//             IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
//             const int32 numSlots = 2;
//             ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
//             {
//                 ZCallBufferSlot.FromConjugate(this),
//                 ZCallBufferSlot.FromConjugate(null),
//             };
//             ZCallBuffer buffer = new(slots, numSlots);
//             alc.ZCall("ex://Object.GetClass", &buffer);
//             return slots[1].ReadConjugate<UnrealObject>() ?? throw new InvalidOperationException();
//         }
//     }
//
//     public unsafe UnrealObject? Outer
//     {
//         get
//         {
//             IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
//             const int32 numSlots = 2;
//             ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
//             {
//                 ZCallBufferSlot.FromConjugate(this),
//                 ZCallBufferSlot.FromConjugate(null),
//             };
//             ZCallBuffer buffer = new(slots, numSlots);
//             alc.ZCall("ex://Object.GetOuter", &buffer);
//             return slots[1].ReadConjugate<UnrealObject>() ?? throw new InvalidOperationException();
//         }
//     }
//
//     public unsafe UnrealString Name
//     {
//         get
//         {
//             IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
//             const int32 numSlots = 2;
//             ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
//             {
//                 ZCallBufferSlot.FromConjugate(this),
//                 ZCallBufferSlot.FromConjugate(null),
//             };
//             ZCallBuffer buffer = new(slots, numSlots);
//             alc.ZCall("ex://Object.GetName", &buffer);
//             return slots[1].ReadConjugate<UnrealString>() ?? throw new InvalidOperationException();
//         }
//     }
//
//     protected override void ReleaseUnmanagedResource()
//     {
//         Logger.Log($"===================== Release Object {Unmanaged} =====================");
//     }
//
//     private UnrealObject(IntPtr unmanaged) : base(unmanaged){}
//     
// }
//
//
