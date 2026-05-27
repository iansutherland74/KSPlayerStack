/* Viture SpaceWalker — deep IDA export */
/* Binary: /Users/sutherland/Downloads/space mac/Contents/MacOS/SpaceWalker */
/* Functions: 120 */

/* ========================================================================
 * sub_100025B1C
 * EA: 0x100025b1c
 ======================================================================== */

__int64 __fastcall sub_100025B1C(char a1)
{
  void *v1; // x20
  char *v3; // x21
  char *v4; // x23
  __int64 v5; // x19
  char *v6; // x26
  char *v7; // x25
  __int64 v8; // x28
  __int64 v9; // x22
  __int64 v10; // x0
  __int64 v11; // x0
  __int64 v12; // x0
  CGDirectDisplayID v13; // w22
  __int64 v14; // x0
  __int64 v15; // x22
  __int64 v16; // x0
  __int64 v17; // x0
  __int64 v18; // x28
  id v19; // x0
  __int64 v20; // x28
  double v21; // d8
  void *v22; // x27
  __int64 v23; // x19
  void *v24; // x22
  __int64 v25; // x26
  __int64 v26; // x20
  __int64 v27; // x0
  char *v28; // x24
  char *v29; // x25
  __int64 v30; // x21
  __int64 v31; // x1
  void *v32; // x24
  void *v33; // x22
  __int64 v34; // x25
  __int64 v35; // x20
  __int64 v36; // x0
  __int64 v37; // x19
  __int64 v38; // x21
  unsigned __int64 v39; // x22
  __int64 v40; // x0
  __int64 inited; // x19
  __int64 v42; // x21
  void (__fastcall *v44)(char *, __int64); // [xsp+0h] [xbp-120h] BYREF
  __int64 v45; // [xsp+8h] [xbp-118h]
  __int64 v46; // [xsp+10h] [xbp-110h]
  __int64 v47; // [xsp+18h] [xbp-108h]
  __int64 v48; // [xsp+20h] [xbp-100h]
  __int64 v49; // [xsp+28h] [xbp-F8h]
  CGDisplayConfigRef config; // [xsp+30h] [xbp-F0h] BYREF
  __int64 v51; // [xsp+38h] [xbp-E8h]
  __int64 (__fastcall *v52)(); // [xsp+40h] [xbp-E0h]
  void *v53; // [xsp+48h] [xbp-D8h]
  __int64 (__fastcall *v54)(); // [xsp+50h] [xbp-D0h]
  __int64 v55; // [xsp+58h] [xbp-C8h]
  char v56[80]; // [xsp+60h] [xbp-C0h] BYREF

  v46 = type metadata accessor for DispatchWorkItemFlags(0);
  v49 = *(_QWORD *)(v46 - 8);
  v3 = (char *)&v44 - ((*(_QWORD *)(v49 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v48 = type metadata accessor for DispatchQoS(0);
  v47 = *(_QWORD *)(v48 - 8);
  v4 = (char *)&v44 - ((*(_QWORD *)(v47 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v45 = type metadata accessor for DispatchTime(0);
  v5 = *(_QWORD *)(v45 - 8);
  v6 = (char *)&v44 - ((*(_QWORD *)(v5 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v7 = v6;
  v8 = type metadata accessor for VTLogger(0);
  v9 = ((__int64 (*)(void))static os_log_type_t.info.getter)();
  v10 = ((__int64 (*)(void))static os_log_type_t.info.getter)();
  v11 = sub_100091CD8(v9, v10, 0xD000000000000011LL, 0x800000010026D9C0LL, v8);
  v12 = sub_1000C94CC(v11);
  if ( qword_100344520 )
  {
    v13 = *(_DWORD *)(qword_100344520 + 16);
    config = nullptr;
    CGBeginDisplayConfiguration(&config);
    CGConfigureDisplayMirrorOfDisplay(config, v13, 0);
    v14 = CGCompleteDisplayConfiguration(config, 2u);
  }
  else
  {
    v15 = static os_log_type_t.info.getter(v12);
    v16 = static os_log_type_t.info.getter(v15);
    v14 = sub_100091CD8(v15, v16, 0xD000000000000018LL, 0x800000010026D920LL, v8);
  }
  v17 = sub_1000C94CC(v14);
  sub_1000256C0(v17);
  byte_10033FF68 = a1;
  type metadata accessor for VTDumyLayoutManager(0);
  sub_1000F9044();
  v18 = swift_allocObject(&unk_1002DD9F0, 25, 7);
  *(_QWORD *)(v18 + 16) = v1;
  *(_BYTE *)(v18 + 24) = a1;
  v19 = objc_retain(v1);
  sub_1000F5A30(sub_10002CF8C, v18);
  swift_release(v18);
  if ( qword_1003393A8 != -1 )
    swift_once(&qword_1003393A8, sub_100084ECC);
  v20 = qword_1003444C0;
  if ( *(_BYTE *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_autoTurnOffMainDisplay) == 1 )
  {
    if ( (sub_1000EBFE8() & 1) != 0 )
      v21 = 6.0;
    else
      v21 = 0.0;
    sub_10000C9F0(0, &qword_10033BA50, &classRef_OS_dispatch_queue);
    v22 = (void *)static OS_dispatch_queue.main.getter();
    static DispatchTime.now()();
    + infix(_:_:)(v6, v21);
    v44 = *(void (__fastcall **)(char *, __int64))(v5 + 8);
    v23 = v45;
    v44(v6, v45);
    v54 = sub_1000266E8;
    v55 = 0;
    config = (CGDisplayConfigRef)_NSConcreteStackBlock;
    v51 = 1107296256;
    v52 = sub_10007A08C;
    v53 = &unk_1002DDA30;
    v24 = _Block_copy(&config);
    static DispatchQoS.unspecified.getter();
    config = (CGDisplayConfigRef)&_swiftEmptyArrayStorage;
    v25 = sub_1000052F4(
            &qword_10033C970,
            &type metadata accessor for DispatchWorkItemFlags,
            &protocol conformance descriptor for DispatchWorkItemFlags);
    v26 = sub_100004DF0(&unk_10033BE50, &unk_10025B090);
    v27 = sub_1000147C4();
    v28 = v7;
    v29 = v3;
    v30 = v46;
    dispatch thunk of SetAlgebra.init<A>(_:)(&config, v26, v27, v46, v25);
    OS_dispatch_queue.asyncAfter(deadline:qos:flags:execute:)(v28, v4, v29, v24);
    _Block_release(v24);
    objc_release(v22);
    v31 = v30;
    v3 = v29;
    (*(void (__fastcall **)(char *, __int64))(v49 + 8))(v29, v31);
    (*(void (__fastcall **)(char *, __int64))(v47 + 8))(v4, v48);
    v44(v28, v23);
  }
  if ( *(_BYTE *)(v20 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_reduceMotionBlur) == 1 )
  {
    sub_10000C9F0(0, &qword_10033BA50, &classRef_OS_dispatch_queue);
    v32 = (void *)static OS_dispatch_queue.main.getter();
    v54 = sub_10002695C;
    v55 = 0;
    config = (CGDisplayConfigRef)_NSConcreteStackBlock;
    v51 = 1107296256;
    v52 = sub_10007A08C;
    v53 = &unk_1002DDA08;
    v33 = _Block_copy(&config);
    static DispatchQoS.unspecified.getter();
    config = (CGDisplayConfigRef)&_swiftEmptyArrayStorage;
    v34 = sub_1000052F4(
            &qword_10033C970,
            &type metadata accessor for DispatchWorkItemFlags,
            &protocol conformance descriptor for DispatchWorkItemFlags);
    v35 = sub_100004DF0(&unk_10033BE50, &unk_10025B090);
    v36 = sub_1000147C4();
    v37 = v46;
    dispatch thunk of SetAlgebra.init<A>(_:)(&config, v35, v36, v46, v34);
    OS_dispatch_queue.async(group:qos:flags:execute:)(0, v4, v3, v33);
    _Block_release(v33);
    objc_release(v32);
    (*(void (__fastcall **)(char *, __int64))(v49 + 8))(v3, v37);
    (*(void (__fastcall **)(char *, __int64))(v47 + 8))(v4, v48);
  }
  if ( *(_BYTE *)(v20 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_isExtendMode) )
    v38 = 0x6465646E65747865LL;
  else
    v38 = 0x726F7272696DLL;
  if ( *(_BYTE *)(v20 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_isExtendMode) )
    v39 = 0xE800000000000000LL;
  else
    v39 = 0xE600000000000000LL;
  v40 = sub_100004DF0(&unk_10033C490, &unk_10025B820);
  inited = swift_initStackObject(v40, v56);
  *(_OWORD *)(inited + 16) = xmmword_10025B110;
  *(_QWORD *)(inited + 32) = 0x65756C6176LL;
  *(_QWORD *)(inited + 72) = &type metadata for String;
  *(_QWORD *)(inited + 40) = 0xE500000000000000LL;
  *(_QWORD *)(inited + 48) = v38;
  *(_QWORD *)(inited + 56) = v39;
  v42 = sub_100012D98();
  swift_setDeallocating(inited);
  sub_10000C910(inited + 32, &unk_10033C120, &unk_10025B290);
  sub_1000D4C58(0x6F4D6E6565726353LL, 0xEA00000000006564LL, v42);
  return swift_bridgeObjectRelease(v42);
}

/* ========================================================================
 * sub_10002CFF8
 * EA: 0x10002cff8
 ======================================================================== */

__int64 __fastcall sub_10002CFF8(__int64 a1)
{
  _QWORD *v1; // x20
  __int64 v2; // x22
  __int64 v4; // x21
  __int64 v5; // x23
  __int64 v6; // x20
  _QWORD *v7; // x0

  v4 = v1[2];
  v5 = v1[3];
  v6 = v1[4];
  v7 = (_QWORD *)swift_task_alloc(48);
  *(_QWORD *)(v2 + 16) = v7;
  *v7 = v2;
  v7[1] = sub_100007134;
  return sub_1000259E8(a1, v4, v5, v6);
}

/* ========================================================================
 * sub_10002D088
 * EA: 0x10002d088
 ======================================================================== */

__int64 __fastcall sub_10002D088(__int64 a1)
{
  __int64 v1; // x20
  __int64 v2; // x22
  __int64 v4; // x20
  __int64 v5; // x21
  _QWORD *v6; // x0

  v5 = *(_QWORD *)(v1 + 16);
  v4 = *(_QWORD *)(v1 + 24);
  v6 = (_QWORD *)swift_task_alloc(32);
  *(_QWORD *)(v2 + 16) = v6;
  *v6 = v2;
  v6[1] = sub_100007134;
  return ((__int64 (__fastcall *)(__int64, __int64, __int64))(&off_100261C28 - 352396))(a1, v5, v4);
}

/* ========================================================================
 * sub_10002D1CC
 * EA: 0x10002d1cc
 ======================================================================== */

__int64 sub_10002D1CC()
{
  __int64 v0; // x20
  double v1; // d4
  double v2; // d5
  double v3; // d3
  double y; // d1
  double z; // d2
  double v8; // d7
  double v9; // d16
  double v10; // d6
  double v13; // d2
  __int64 result; // x0

  v1 = *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartIMU);
  v2 = *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartIMU + 8);
  v3 = *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartIMU + 16);
  y = SCNVector3Zero.y;
  z = SCNVector3Zero.z;
  if ( v1 != SCNVector3Zero.x || v2 != y || v3 != z )
  {
    v8 = *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndIMU);
    v9 = *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndIMU + 8);
    v10 = *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndIMU + 16);
    if ( v8 != SCNVector3Zero.x || v9 != y || v10 != z )
    {
      v13 = (*(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndTS)
           - *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartTS))
          * 240.0;
      return SCNVector3.init(_:_:_:)((v8 - v1) / v13, (v9 - v2) / v13, (v10 - v3) / v13);
    }
  }
  return result;
}

/* ========================================================================
 * sub_10002D278
 * EA: 0x10002d278
 ======================================================================== */

id __fastcall sub_10002D278(__int64 a1, __int64 a2)
{
  void *v2; // x20
  id v3; // x0
  __int64 v4; // x19
  __int64 v5; // x21
  __int64 v6; // x0
  __int64 v7; // x0
  objc_super v9; // [xsp+0h] [xbp-30h] BYREF

  v3 = objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSObject, a2), "cancelPreviousPerformRequestsWithTarget:", v2);
  sub_1000304CC(v3);
  v4 = type metadata accessor for VTLogger(0);
  v5 = static os_log_type_t.info.getter(v4);
  v6 = static os_log_type_t.info.getter(v5);
  v7 = sub_100091CD8(v5, v6, 0xD000000000000019LL, 0x800000010026E120LL, v4);
  v9.receiver = v2;
  v9.super_class = (Class)type metadata accessor for VTCameraController(v7);
  return objc_msgSendSuper2(&v9, "dealloc");
}

/* ========================================================================
 * -[_TtC11SpaceWalker18VTCameraController .cxx_destruct]
 * EA: 0x10002d33c
 ======================================================================== */

void __cdecl -[VTCameraController .cxx_destruct](_TtC11SpaceWalker18VTCameraController *self, SEL a2)
{
  void *v3; // x20
  __int64 v4; // x21
  void *v5; // x22

  swift_unknownObjectWeakDestroy((char *)self + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view, a2);
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuSlerpUtil));
  sub_10000C7E4((char *)self + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuController);
  swift_release(*(Class *)((char *)&self->super.isa
                         + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_p6ResetToastController));
  v3 = *(Class *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase);
  v4 = *(_QWORD *)&self->view[OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase];
  v5 = *(void **)&self->interpolationEnabled[OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase];
  swift_bridgeObjectRelease(*(_QWORD *)&self->imuController[OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase
                                                          + 7]);
  objc_release(v5);
  swift_release(v4);
  objc_release(v3);
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.isa
                                     + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuTSPairs));
}

/* ========================================================================
 * sub_10002D3E0
 * EA: 0x10002d3e0
 ======================================================================== */

__int64 sub_10002D3E0()
{
  __int64 v0; // x20
  double v1; // d11
  __int64 v2; // x19
  __int64 result; // x0
  __int64 v4; // x20
  __int64 v5; // x0
  char v6; // w1
  float v7; // s8
  __int64 v8; // x0
  char v9; // w1
  __int64 v10; // x0
  char v11; // w1
  float v12; // s12
  double v13; // d8
  double v14; // d0
  __int64 v15; // x8
  __int64 v16; // d1
  __int64 v17; // d2
  id v18; // x20
  __int64 v19; // d0
  __int64 v20; // d10
  double v21; // d0
  __int64 v22; // x8
  __int64 v23; // d1
  __int64 v24; // d2
  id v25; // x20
  __int64 v26; // d0
  __int64 v27; // d9
  int v28; // w20
  __int64 v29; // x24
  int v30; // w27
  int v31; // w26
  __int64 v32; // x25
  double v33; // d9
  double v34; // d1
  double v35; // d9
  double v36; // d1
  double v37; // d2
  double v38; // d0
  __int64 v39; // x21
  Swift::String v40; // x0
  Swift::String v41; // x0
  void *object; // x22
  Swift::String v43; // x0
  __int64 v44; // d0
  __int64 v45; // d1
  __int64 v46; // d2
  __int64 v47; // x0
  __int64 v48; // x0
  __int128 v49; // kr00_16
  __int64 v50; // x23
  __int64 v51; // x0
  double v52; // d10
  __int64 v53; // x8
  bool v54; // vf
  __int64 v55; // x8
  double v56; // d9
  double *v57; // x8
  double *v58; // x8
  double *v59; // x8
  __int64 v60; // [xsp+8h] [xbp-D8h] BYREF
  unsigned __int64 v61; // [xsp+10h] [xbp-D0h]
  __int128 v62; // [xsp+18h] [xbp-C8h] BYREF
  __int64 v63; // [xsp+28h] [xbp-B8h]
  __int128 v64; // [xsp+40h] [xbp-A0h] BYREF
  __int128 v65; // [xsp+50h] [xbp-90h]

  v2 = v0;
  result = Notification.userInfo.getter();
  if ( !result )
    return result;
  v4 = result;
  v60 = 0x6863746970LL;
  v61 = 0xE500000000000000LL;
  AnyHashable.init<A>(_:)(&v62, &v60, &type metadata for String, &protocol witness table for String);
  if ( *(_QWORD *)(v4 + 16) )
  {
    swift_bridgeObjectRetain(v4);
    v5 = sub_10010B17C(&v62);
    if ( (v6 & 1) != 0 )
    {
      sub_10002CA48(*(_QWORD *)(v4 + 56) + 32 * v5, &v64);
      swift_bridgeObjectRelease(v4);
      goto LABEL_7;
    }
    swift_bridgeObjectRelease(v4);
  }
  v64 = 0u;
  v65 = 0u;
LABEL_7:
  sub_1000328F4(&v62);
  if ( !*((_QWORD *)&v65 + 1) )
  {
LABEL_19:
    swift_bridgeObjectRelease(v4);
    return sub_100032928(&v64);
  }
  if ( (swift_dynamicCast(&v60, &v64, (char *)&type metadata for Any + 8, &type metadata for Float, 6) & 1) == 0 )
    return swift_bridgeObjectRelease(v4);
  v7 = *(float *)&v60;
  v60 = 7823737;
  v61 = 0xE300000000000000LL;
  AnyHashable.init<A>(_:)(&v62, &v60, &type metadata for String, &protocol witness table for String);
  if ( *(_QWORD *)(v4 + 16) )
  {
    swift_bridgeObjectRetain(v4);
    v8 = sub_10010B17C(&v62);
    if ( (v9 & 1) != 0 )
    {
      sub_10002CA48(*(_QWORD *)(v4 + 56) + 32 * v8, &v64);
      swift_bridgeObjectRelease(v4);
      goto LABEL_14;
    }
    swift_bridgeObjectRelease(v4);
  }
  v64 = 0u;
  v65 = 0u;
LABEL_14:
  sub_1000328F4(&v62);
  if ( !*((_QWORD *)&v65 + 1) )
    goto LABEL_19;
  if ( (swift_dynamicCast(&v60, &v64, (char *)&type metadata for Any + 8, &type metadata for Float, 6) & 1) == 0 )
    return swift_bridgeObjectRelease(v4);
  LODWORD(v1) = v60;
  v60 = 1819045746;
  v61 = 0xE400000000000000LL;
  AnyHashable.init<A>(_:)(&v62, &v60, &type metadata for String, &protocol witness table for String);
  if ( *(_QWORD *)(v4 + 16) )
  {
    swift_bridgeObjectRetain(v4);
    v10 = sub_10010B17C(&v62);
    if ( (v11 & 1) != 0 )
    {
      sub_10002CA48(*(_QWORD *)(v4 + 56) + 32 * v10, &v64);
      swift_bridgeObjectRelease(v4);
      goto LABEL_25;
    }
    swift_bridgeObjectRelease(v4);
  }
  v64 = 0u;
  v65 = 0u;
LABEL_25:
  swift_bridgeObjectRelease(v4);
  sub_1000328F4(&v62);
  if ( !*((_QWORD *)&v65 + 1) )
    return sub_100032928(&v64);
  result = swift_dynamicCast(&v60, &v64, (char *)&type metadata for Any + 8, &type metadata for Float, 6);
  if ( (result & 1) == 0 )
    return result;
  v12 = *(float *)&v60;
  v13 = v7;
  if ( *(_BYTE *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustStartNeeded) == 1 )
  {
    *(_BYTE *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustStartNeeded) = 0;
    v14 = SCNVector3.init(_:_:_:)(v13, *(float *)&v1, v12);
    v15 = v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartIMU;
    *(double *)v15 = v14;
    *(_QWORD *)(v15 + 8) = v16;
    *(_QWORD *)(v15 + 16) = v17;
    v18 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSDate), "init");
    objc_msgSend(v18, "timeIntervalSince1970");
    v20 = v19;
    objc_release(v18);
    *(_QWORD *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartTS) = v20;
  }
  if ( *(_BYTE *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustEndNeeded) == 1 )
  {
    *(_BYTE *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustEndNeeded) = 0;
    v21 = SCNVector3.init(_:_:_:)(v13, *(float *)&v1, v12);
    v22 = v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndIMU;
    *(double *)v22 = v21;
    *(_QWORD *)(v22 + 8) = v23;
    *(_QWORD *)(v22 + 16) = v24;
    v25 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSDate), "init");
    objc_msgSend(v25, "timeIntervalSince1970");
    v27 = v26;
    objc_release(v25);
    *(_QWORD *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndTS) = v27;
  }
  v28 = (unsigned __int8)byte_10033FF68;
  if ( qword_1003393A8 != -1 )
    swift_once(&qword_1003393A8, sub_100084ECC);
  v29 = qword_1003444C0;
  if ( v28 != 4 && *(_BYTE *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockYAxis) )
    *(float *)&v1 = 0.0;
  v30 = *(unsigned __int8 *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockXAxis);
  v31 = *(unsigned __int8 *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockZAxis);
  v32 = OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedCounter;
  v33 = (double)*(__int64 *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedCounter);
  sub_10002D1CC();
  v35 = v34 * v33;
  sub_10002D1CC();
  if ( v38 != SCNVector3Zero.x || v36 != SCNVector3Zero.y || v37 != SCNVector3Zero.z )
  {
    v39 = type metadata accessor for VTLogger(0);
    *(_QWORD *)&v62 = 0;
    *((_QWORD *)&v62 + 1) = 0xE000000000000000LL;
    _StringGuts.grow(_:)(31);
    v64 = v62;
    v40._countAndFlagsBits = 0x64657473756A6461LL;
    v40._object = (void *)0xEF20736920776159LL;
    String.append(_:)(v40);
    v41._countAndFlagsBits = Double.description.getter(v35);
    object = v41._object;
    String.append(_:)(v41);
    swift_bridgeObjectRelease(object);
    v43._countAndFlagsBits = 0x726F74636576202CLL;
    v43._object = (void *)0xEC00000020736920LL;
    String.append(_:)(v43);
    sub_10002D1CC();
    *(_QWORD *)&v62 = v44;
    *((_QWORD *)&v62 + 1) = v45;
    v63 = v46;
    v47 = type metadata accessor for SCNVector3(0);
    v48 = _print_unlocked<A, B>(_:_:)(
            &v62,
            &v64,
            v47,
            &type metadata for DefaultStringInterpolation,
            &protocol witness table for DefaultStringInterpolation);
    v49 = v64;
    v50 = static os_log_type_t.debug.getter(v48);
    v51 = static os_log_type_t.debug.getter(v50);
    sub_100091CD8(v50, v51, v49, *((_QWORD *)&v49 + 1), v39);
    swift_bridgeObjectRelease(*((_QWORD *)&v49 + 1));
  }
  if ( v30 )
    v13 = 0.0;
  if ( v31 )
    v52 = -0.0;
  else
    v52 = (float)-v12;
  v53 = *(_QWORD *)(v2 + v32);
  v54 = __OFADD__(v53, 1);
  v55 = v53 + 1;
  if ( v54 )
  {
    __break(1u);
    goto LABEL_56;
  }
  v1 = *(float *)&v1;
  *(_QWORD *)(v2 + v32) = v55;
  if ( qword_100339480 != -1 )
LABEL_56:
    swift_once(&qword_100339480, sub_1000FE3CC);
  v56 = v1 - v35;
  if ( (sub_1000FF5B4() & 1) != 0 )
  {
    if ( *(_BYTE *)(v29 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_dynamicVSyncEnabled) == 1 )
      sub_10002DAC4(v13, v56, v52);
    else
      sub_10002DC60(1);
  }
  result = sub_100048F6C(v13, v56, v52);
  if ( (*(_BYTE *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isIMUInitialized) & 1) != 0 )
  {
    *(_BYTE *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isIMUUpdated) = 1;
    v57 = (double *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU);
    *(_BYTE *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingUp) = v13 < *(double *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU);
    *(_BYTE *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingLeft) = v57[1] < v56;
    *v57 = v13;
    v57[1] = v56;
    v57[2] = v52;
    if ( *(_BYTE *)(v29 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_smoothFollow) == 1 )
      return sub_10002DE80(result);
  }
  else
  {
    *(_BYTE *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isIMUInitialized) = 1;
    v58 = (double *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_anchorIMU);
    *v58 = v13;
    v58[1] = v56;
    v58[2] = v52;
    v59 = (double *)(v2 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU);
    *v59 = v13;
    v59[1] = v56;
    v59[2] = v52;
  }
  return result;
}

/* ========================================================================
 * sub_10002DAC4
 * EA: 0x10002dac4
 ======================================================================== */

double __fastcall sub_10002DAC4(double a1, double a2, double a3)
{
  char *v3; // x20
  __int64 v7; // x1
  double *v8; // x8
  double v9; // d0
  Class isa; // x21
  double result; // d0

  if ( qword_100339480 != -1 )
    swift_once(&qword_100339480, sub_1000FE3CC);
  if ( sub_1000FF000() < 4352
    || sub_1000FF000() == 4608
    || sub_1000FF000() == 4609
    || sub_1000FF000() == 4624
    || sub_1000FF000() == 4625 )
  {
    v8 = (double *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU];
    v9 = 0.01;
    if ( vabdd_f64(*(double *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU], a1) >= 0.01 )
      goto LABEL_13;
  }
  else
  {
    v8 = (double *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU];
    v9 = 0.015;
    if ( vabdd_f64(*(double *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU], a1) >= 0.015 )
      goto LABEL_13;
  }
  if ( vabdd_f64(v8[1], a2) < v9 && vabdd_f64(v8[2], a3) < v9 )
  {
    isa = Bool._bridgeToObjectiveC()().super.super.isa;
    objc_msgSend(v3, "performSelector:withObject:afterDelay:", "setVSyncEnabled:", isa, 0.034);
    objc_release(isa);
    return result;
  }
LABEL_13:
  objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSObject, v7), "cancelPreviousPerformRequestsWithTarget:", v3);
  *(_QWORD *)&result = sub_10002DC60(1).n128_u64[0];
  return result;
}

/* ========================================================================
 * sub_10002DC60
 * EA: 0x10002dc60
 ======================================================================== */

void __fastcall sub_10002DC60(char a1)
{
  __int64 v1; // x20
  __int64 Strong; // x0
  id v4; // x19
  __int64 v5; // x0
  __int64 v6; // x0
  void *v7; // x21
  id v8; // x0
  __int64 v9; // x1
  void *v10; // x22
  __int64 v11; // x0
  void *v12; // x0
  void *v13; // x23
  id v14; // x21
  id v15; // x0
  __int64 v16; // x1
  id v17; // x23
  __int64 v18; // x0
  __int64 v19; // x0
  __int64 v20; // x1
  void *v21; // x22
  void *v22; // x0
  char **v23; // x8
  id v24; // x20
  id v25; // x20

  Strong = swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view);
  if ( Strong )
  {
    v4 = (id)Strong;
    v5 = type metadata accessor for SCNCaptureVideoPreview(0);
    v6 = swift_dynamicCastClass(v4, v5);
    if ( v6 )
    {
      v7 = (void *)v6;
      v4 = objc_retain(v4);
      v8 = objc_retainAutoreleasedReturnValue(objc_msgSend(v7, "layer"));
      if ( v8 )
      {
        v10 = v8;
        v11 = objc_opt_self(&OBJC_CLASS___CAMetalLayer, v9);
        v12 = (void *)swift_dynamicCastObjCClass(v10, v11);
        if ( v12 )
        {
          v13 = v12;
          if ( (unsigned int)objc_msgSend(v12, "displaySyncEnabled") == (a1 & 1) )
          {
            v14 = v4;
            goto LABEL_17;
          }
          objc_msgSend(v13, "setDisplaySyncEnabled:");
        }
        objc_release(v10);
      }
      v14 = objc_retainAutoreleasedReturnValue(objc_msgSend(v7, "superview"));
      objc_release(v4);
      if ( v14 )
      {
        v15 = objc_retainAutoreleasedReturnValue(objc_msgSend(v14, "viewWithTag:", 4096));
        if ( !v15 )
        {
LABEL_18:
          objc_release(v4);
          v4 = v14;
          goto LABEL_19;
        }
        v17 = v15;
        v18 = objc_opt_self(&OBJC_CLASS___NSTextField, v16);
        v19 = swift_dynamicCastObjCClass(v17, v18);
        if ( v19 )
        {
          v21 = (void *)v19;
          v22 = (void *)objc_opt_self(&OBJC_CLASS___NSColor, v20);
          v23 = &selRef_greenColor;
          if ( (a1 & 1) == 0 )
            v23 = &selRef_redColor;
          v24 = objc_retainAutoreleasedReturnValue(objc_msgSend(v22, *v23));
          objc_msgSend(v21, "setTextColor:", v24);
          objc_release(v14);
          v10 = v17;
          v14 = v24;
        }
        else
        {
          v10 = v4;
          v4 = v17;
        }
LABEL_17:
        v25 = v4;
        objc_release(v10);
        v4 = v14;
        v14 = v25;
        goto LABEL_18;
      }
    }
LABEL_19:
    objc_release(v4);
  }
}

/* ========================================================================
 * sub_10002DE80
 * EA: 0x10002de80
 ======================================================================== */

void __fastcall sub_10002DE80(id a1)
{
  __int64 v1; // x20
  int v2; // w24
  bool v3; // w23
  __int64 v4; // x21
  __int64 v5; // x25
  __int64 v6; // x8
  bool v7; // cc
  __int64 v8; // x8
  double *v9; // x28
  double *v10; // x19
  double v11; // d1
  double v12; // d0
  double v13; // d3
  double v14; // d8
  double v15; // d9
  double v16; // d2
  double v17; // x8
  double v18; // d8
  __int64 v19; // d1
  __int64 v20; // d9
  __int64 v21; // d2
  __int64 v22; // d10
  __int64 v23; // x26
  __int64 v24; // x27
  double *v25; // x8
  _QWORD *v26; // x23
  __int64 v27; // x8
  _BOOL4 v28; // w25
  double v29; // d1
  double v30; // d0
  double v31; // x8
  double v32; // d8
  double v33; // d9
  double v34; // d2
  double v35; // x8
  double v36; // d8
  __int64 v37; // d1
  __int64 v38; // d9
  __int64 v39; // d2
  __int64 v40; // d10
  __int64 v41; // x26
  __int64 v42; // x27
  double *v43; // x8
  _QWORD *v44; // x25
  __int64 v45; // x8
  _BOOL4 v46; // w23
  double v47; // d0
  double v48; // d2
  double v49; // d4
  double v50; // d2
  double v51; // d3
  double v52; // d2
  double v53; // d8
  __int64 v54; // d1
  __int64 v55; // d9
  __int64 v56; // d2
  __int64 v57; // d10
  __int64 v58; // x26
  __int64 v59; // x27
  double *v60; // x8
  _QWORD *v61; // x23
  __int64 v62; // x8
  _BOOL4 v63; // w24
  double v64; // d0
  double v65; // d2
  double v66; // d4
  double v67; // d2
  double v68; // d3
  double v69; // d2
  double v70; // d8
  __int64 v71; // d1
  __int64 v72; // d9
  __int64 v73; // d2
  __int64 v74; // d10
  __int64 v75; // x19
  __int64 v76; // x21
  double *v77; // x8

  if ( qword_1003393A8 != -1 )
    a1 = (id)swift_once(&qword_1003393A8, sub_100084ECC);
  v2 = *(unsigned __int8 *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_inertiaEnabled);
  if ( v2 == 1 )
  {
    if ( (*(_BYTE *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingLeft) & 1) != 0 )
    {
      if ( qword_1003393B8 != -1 )
        a1 = (id)swift_once(&qword_1003393B8, sub_10008E520);
    }
    else
    {
      if ( qword_1003393B8 != -1 )
        a1 = (id)swift_once(&qword_1003393B8, sub_10008E520);
      if ( *(_QWORD *)(qword_1003444D0 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationLeftCount) == 120 )
      {
        v3 = 1;
        goto LABEL_15;
      }
    }
    v3 = (unsigned __int64)(*(_QWORD *)(qword_1003444D0
                                      + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationLeftCount)
                          - 1LL) < 0x77;
  }
  else
  {
    if ( qword_1003393B8 != -1 )
      a1 = (id)swift_once(&qword_1003393B8, sub_10008E520);
    v3 = 0;
  }
LABEL_15:
  v4 = qword_1003444D0;
  v5 = OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationLeftCount;
  v6 = *(_QWORD *)(qword_1003444D0 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationLeftCount);
  v7 = v6 < 1;
  v8 = v6 - 1;
  if ( !v7 )
    *(_QWORD *)(qword_1003444D0 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationLeftCount) = v8;
  v9 = (double *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU);
  v10 = (double *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_anchorIMU);
  v11 = *(double *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_anchorIMU + 8);
  v12 = *(double *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU + 8) - v11;
  v13 = v12 + -360.0;
  if ( v12 <= 180.0 )
    v13 = *(double *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU + 8) - v11;
  if ( v12 < -180.0 )
    v14 = v12 + 360.0;
  else
    v14 = v13;
  v15 = *(double *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_leftDegree);
  if ( v3 )
  {
    v16 = v14 - (v15 + ((double (*)(void))sub_100031AB0)() * -0.5 + 9.0 + -4.0);
    if ( v16 <= 0.0 )
      goto LABEL_29;
    v11 = v10[1];
    v17 = 120.0;
  }
  else
  {
    v16 = v14 - v15;
    if ( v14 - v15 <= 0.0 )
      goto LABEL_29;
    v17 = 40.0;
  }
  v18 = SCNVector3.init(_:_:_:)(*v10, v16 / v17 + v11, v10[2]);
  v20 = v19;
  v22 = v21;
  *v10 = v18;
  *((_QWORD *)v10 + 1) = v19;
  *((_QWORD *)v10 + 2) = v21;
  v23 = *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuSlerpUtil);
  v24 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock;
  objc_msgSend(*(id *)(v23 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock), "lock");
  v25 = (double *)(v23 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_anchorIMU);
  *v25 = v18;
  *((_QWORD *)v25 + 1) = v20;
  *((_QWORD *)v25 + 2) = v22;
  a1 = objc_msgSend(*(id *)(v23 + v24), "unlock");
  if ( !v3 && (*(_BYTE *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingLeft) & 1) != 0 )
    *(_QWORD *)(v4 + v5) = 120;
LABEL_29:
  if ( (v2 & 1) == 0 )
  {
    v28 = 0;
    v27 = *(_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationRightCount);
    goto LABEL_35;
  }
  v26 = (_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationRightCount);
  v27 = *(_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationRightCount);
  if ( *(_BYTE *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingLeft) != 1 || v27 != 120 )
  {
    v28 = (unsigned __int64)(v27 - 1) < 0x77;
LABEL_35:
    v26 = (_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationRightCount);
    if ( v27 < 1 )
      goto LABEL_37;
    goto LABEL_36;
  }
  v28 = 1;
LABEL_36:
  *v26 = v27 - 1;
LABEL_37:
  v29 = v10[1];
  v30 = v9[1] - v29;
  if ( v30 >= -180.0 )
  {
    if ( v30 <= 180.0 )
      goto LABEL_42;
    v31 = -360.0;
  }
  else
  {
    v31 = 360.0;
  }
  v30 = v30 + v31;
LABEL_42:
  v32 = -v30;
  v33 = *(double *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_rightDegree);
  if ( v28 )
  {
    v34 = v32 - (v33 + sub_100031AB0(a1) * -0.5 + 9.0 + -4.0);
    if ( v34 <= 0.0 )
      goto LABEL_49;
    v29 = v10[1];
    v35 = 120.0;
  }
  else
  {
    v34 = v32 - v33;
    if ( v32 - v33 <= 0.0 )
      goto LABEL_49;
    v35 = 40.0;
  }
  v36 = SCNVector3.init(_:_:_:)(*v10, v29 - v34 / v35, v10[2]);
  v38 = v37;
  v40 = v39;
  *v10 = v36;
  *((_QWORD *)v10 + 1) = v37;
  *((_QWORD *)v10 + 2) = v39;
  v41 = *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuSlerpUtil);
  v42 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock;
  objc_msgSend(*(id *)(v41 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock), "lock");
  v43 = (double *)(v41 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_anchorIMU);
  *v43 = v36;
  *((_QWORD *)v43 + 1) = v38;
  *((_QWORD *)v43 + 2) = v40;
  objc_msgSend(*(id *)(v41 + v42), "unlock");
  if ( ((v28 | *(unsigned __int8 *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingLeft)) & 1) == 0 )
    *v26 = 120;
LABEL_49:
  if ( (v2 & 1) == 0 )
  {
    v46 = 0;
    v45 = *(_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationTopCount);
    goto LABEL_55;
  }
  v44 = (_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationTopCount);
  v45 = *(_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationTopCount);
  if ( (*(_BYTE *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingUp) & 1) != 0 || v45 != 120 )
  {
    v46 = (unsigned __int64)(v45 - 1) < 0x77;
LABEL_55:
    v44 = (_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationTopCount);
    if ( v45 < 1 )
      goto LABEL_57;
    goto LABEL_56;
  }
  v46 = 1;
LABEL_56:
  *v44 = v45 - 1;
LABEL_57:
  v47 = 40.0;
  if ( v46 )
    v47 = 120.0;
  v48 = *v9 - *v10;
  v49 = v48 + -360.0;
  if ( v48 <= 180.0 )
    v49 = *v9 - *v10;
  if ( v48 < -180.0 )
    v50 = v48 + 360.0;
  else
    v50 = v49;
  v51 = *(double *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_topDegree);
  if ( v46 )
    v51 = v51 + -6.0;
  v52 = -v50 - v51;
  if ( v52 > 0.0 )
  {
    v53 = SCNVector3.init(_:_:_:)(*v10 - v52 / v47, v10[1], v10[2]);
    v55 = v54;
    v57 = v56;
    *v10 = v53;
    *((_QWORD *)v10 + 1) = v54;
    *((_QWORD *)v10 + 2) = v56;
    v58 = *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuSlerpUtil);
    v59 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock;
    objc_msgSend(*(id *)(v58 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock), "lock");
    v60 = (double *)(v58 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_anchorIMU);
    *v60 = v53;
    *((_QWORD *)v60 + 1) = v55;
    *((_QWORD *)v60 + 2) = v57;
    objc_msgSend(*(id *)(v58 + v59), "unlock");
    if ( !v46 && (*(_BYTE *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingUp) & 1) != 0 )
      *v44 = 120;
  }
  if ( (v2 & 1) == 0 )
  {
    v63 = 0;
    v62 = *(_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationBottomCount);
    goto LABEL_75;
  }
  v61 = (_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationBottomCount);
  v62 = *(_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationBottomCount);
  if ( *(_BYTE *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingUp) != 1 || v62 != 120 )
  {
    v63 = (unsigned __int64)(v62 - 1) < 0x77;
LABEL_75:
    v61 = (_QWORD *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_compensationBottomCount);
    if ( v62 < 1 )
      goto LABEL_77;
    goto LABEL_76;
  }
  v63 = 1;
LABEL_76:
  *v61 = v62 - 1;
LABEL_77:
  v64 = 40.0;
  if ( v63 )
    v64 = 120.0;
  v65 = *v9 - *v10;
  v66 = v65 + -360.0;
  if ( v65 <= 180.0 )
    v66 = *v9 - *v10;
  if ( v65 < -180.0 )
    v67 = v65 + 360.0;
  else
    v67 = v66;
  v68 = *(double *)(v4 + OBJC_IVAR____TtC11SpaceWalker21VTSmoothFollowManager_bottomDegree);
  if ( v63 )
    v68 = v68 + -6.0;
  v69 = v67 - v68;
  if ( v69 > 0.0 )
  {
    v70 = SCNVector3.init(_:_:_:)(*v10 + v69 / v64, v10[1], v10[2]);
    v72 = v71;
    v74 = v73;
    *v10 = v70;
    *((_QWORD *)v10 + 1) = v71;
    *((_QWORD *)v10 + 2) = v73;
    v75 = *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuSlerpUtil);
    v76 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock;
    objc_msgSend(*(id *)(v75 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock), "lock");
    v77 = (double *)(v75 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_anchorIMU);
    *v77 = v70;
    *((_QWORD *)v77 + 1) = v72;
    *((_QWORD *)v77 + 2) = v74;
    objc_msgSend(*(id *)(v75 + v76), "unlock");
    if ( ((v63 | *(unsigned __int8 *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingUp)) & 1) == 0 )
      *v61 = 120;
  }
}

/* ========================================================================
 * sub_10002E5F0
 * EA: 0x10002e5f0
 ======================================================================== */

void sub_10002E5F0()
{
  __int64 v0; // x20
  __int64 v1; // x19
  double *v2; // x8
  __int64 v3; // x21
  __int64 v4; // x22
  __int64 v5; // x0
  char v6; // w2
  double v7; // d1
  double v8; // x10
  double v9; // x22
  double v10; // d0
  __int64 v11; // x20
  char v12; // w23
  __int64 v13; // x21
  double v14; // d1
  double v15; // d2
  double v16; // d0
  double v17; // d0
  __int64 v18; // x20
  char v19; // w22
  __int64 v20; // x21
  double v21; // d1
  double v22; // d2
  double v23; // d0
  double v24; // d0
  void *v25; // x21
  id v26; // x20
  void *v27; // x0
  void *v28; // x20
  id v29; // x19
  void *v30; // x0
  double v31; // d0
  double v32; // d1
  double v33; // d2
  double v34; // d8
  double v35; // d1
  double v36; // d9
  double v37; // d2
  double v38; // d10
  __int64 v39; // x20
  __int64 v40; // x19
  __int64 v41; // x21
  double v42; // x0
  __int64 v43; // x1
  unsigned __int64 v44; // x21
  double v45; // d0
  __int64 v46; // v0.d[1]
  __int64 v47; // x1
  double v48; // d0
  void *v49; // x19
  id v50; // x0
  id v51; // x20
  void *Strong; // x0
  void *v53; // x21
  __int128 v54; // [xsp+0h] [xbp-90h]
  double v55; // [xsp+10h] [xbp-80h]
  double v56; // [xsp+10h] [xbp-80h]
  double v57; // [xsp+10h] [xbp-80h]
  __int64 v58; // [xsp+10h] [xbp-80h]
  char v59[24]; // [xsp+20h] [xbp-70h] BYREF
  char v60[8]; // [xsp+38h] [xbp-58h] BYREF

  if ( (*(_BYTE *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isAnimatingReorientation) & 1) != 0 )
    return;
  v1 = v0;
  if ( qword_100339480 != -1 )
    swift_once(&qword_100339480, sub_1000FE3CC);
  if ( sub_1000FF000() == 4353
    || sub_1000FF000() == 4356
    || sub_1000FF000() == 4352
    || sub_1000FF000() == 4355
    || sub_1000FF000() == 514 )
  {
    v2 = (double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastPose);
    if ( (*(_BYTE *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastPose + 32) & 1) == 0 )
    {
      v9 = *v2;
      v54 = *(_OWORD *)v2;
      v10 = v2[2];
      if ( qword_1003393A8 != -1 )
      {
        v57 = v2[2];
        swift_once(&qword_1003393A8, sub_100084ECC);
        v10 = v57;
      }
      v11 = qword_1003444C0;
      v12 = *(_BYTE *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockYAxis);
      v13 = OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockZAxis;
      if ( (*(_BYTE *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockXAxis) & 1) != 0 )
      {
        sub_100031588();
        v16 = 0.0;
        if ( (v12 & 1) == 0 )
          goto LABEL_26;
      }
      else
      {
        if ( (*(_BYTE *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockYAxis) & 1) == 0 )
        {
          if ( *(_BYTE *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockZAxis) != 1 )
          {
            v55 = v10;
            Strong = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view);
            if ( !Strong )
              goto LABEL_30;
            v53 = Strong;
            v26 = objc_retainAutoreleasedReturnValue(objc_msgSend(Strong, "pointOfView"));
            objc_release(v53);
            if ( !v26 )
              goto LABEL_30;
            goto LABEL_29;
          }
          sub_100031588();
LABEL_26:
          if ( *(_BYTE *)(v11 + v13) )
            v15 = 0.0;
          v24 = SCNVector3.init(_:_:_:)(v16, v14, v15);
          v55 = sub_1000316D8(v24);
          v25 = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view);
          v26 = objc_retainAutoreleasedReturnValue(objc_msgSend(v25, "pointOfView"));
          objc_release(v25);
LABEL_29:
          objc_msgSend(v26, "setSimdOrientation:", v55, v54);
          objc_release(v26);
LABEL_30:
          v27 = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view);
          if ( !v27 )
            return;
          v28 = v27;
          v29 = objc_retainAutoreleasedReturnValue(objc_msgSend(v27, "pointOfView"));
          objc_release(v28);
          if ( !v29 )
            return;
          objc_msgSend(
            v29,
            "setSimdWorldPosition:",
            COERCE_DOUBLE(vmul_f32((float32x2_t)__PAIR64__(DWORD1(v54), LODWORD(v9)), (float32x2_t)vdup_n_s32(0x45034000u))));
          v30 = v29;
LABEL_47:
          objc_release(v30);
          return;
        }
        sub_100031588();
      }
      v14 = 0.0;
      goto LABEL_26;
    }
  }
  v3 = *(_QWORD *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuController + 24);
  v4 = *(_QWORD *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuController + 32);
  sub_10000C8A4();
  *(double *)&v5 = COERCE_DOUBLE((*(__int64 (__fastcall **)(__int64, __int64))(v4 + 8))(v3, v4));
  if ( (v6 & 1) == 0 )
  {
    v17 = *(double *)&v5;
    if ( qword_1003393A8 != -1 )
    {
      v58 = v5;
      swift_once(&qword_1003393A8, sub_100084ECC);
      v17 = *(double *)&v58;
    }
    v18 = qword_1003444C0;
    v19 = *(_BYTE *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockYAxis);
    v20 = OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockZAxis;
    if ( (*(_BYTE *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockXAxis) & 1) != 0 )
    {
      sub_100031588();
      v23 = 0.0;
      if ( (v19 & 1) == 0 )
        goto LABEL_43;
    }
    else
    {
      if ( (*(_BYTE *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockYAxis) & 1) == 0 )
      {
        if ( *(_BYTE *)(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences_lockZAxis) != 1 )
        {
          v56 = v17;
          v49 = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view);
          v50 = objc_msgSend(v49, "pointOfView");
LABEL_46:
          v51 = objc_retainAutoreleasedReturnValue(v50);
          objc_release(v49);
          objc_msgSend(v51, "setSimdOrientation:", v56);
          v30 = v51;
          goto LABEL_47;
        }
        sub_100031588();
LABEL_43:
        if ( *(_BYTE *)(v18 + v20) )
          v22 = 0.0;
        v48 = SCNVector3.init(_:_:_:)(v23, v21, v22);
        v56 = sub_1000316D8(v48);
        v49 = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view);
        v50 = objc_msgSend(v49, "pointOfView");
        goto LABEL_46;
      }
      sub_100031588();
    }
    v21 = 0.0;
    goto LABEL_43;
  }
  v7 = *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU + 8)
     - *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_anchorIMU + 8);
  if ( v7 < -180.0 )
  {
    v8 = 360.0;
LABEL_35:
    v7 = v7 + v8;
    goto LABEL_36;
  }
  if ( v7 > 180.0 )
  {
    v8 = -360.0;
    goto LABEL_35;
  }
LABEL_36:
  v31 = SCNVector3.init(_:_:_:)(
          (*(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_anchorIMU)
         - *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU))
        * 3.14159265
        / 180.0,
          v7 * 3.14159265 / 180.0,
          (*(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU + 16)
         - *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_anchorIMU + 16))
        * 3.14159265
        / 180.0);
  v34 = SCNVector3.init(_:_:_:)(
          v31 + *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_startingPosition),
          v32 + *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_startingPosition + 8),
          v33 + *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_startingPosition + 16));
  v36 = v35;
  v38 = v37;
  v39 = *(_QWORD *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuSlerpUtil);
  swift_unknownObjectWeakInit(v60, v1);
  v40 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock;
  objc_msgSend(*(id *)(v39 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock), "lock");
  v41 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_rawIMUs;
  swift_beginAccess(v39 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_rawIMUs, v59, 0, 0);
  v42 = 0.0;
  v43 = 0;
  v44 = *(_QWORD *)(*(_QWORD *)(v39 + v41) + 16LL);
  if ( v44 >= 8 )
  {
    v45 = sub_1000493C8(0, 0);
    v43 = v46;
    v42 = v45;
  }
  sub_10002EBE0(*(_QWORD *)&v42, v43, v44 < 8, v60, v34, v36, v38);
  objc_msgSend(*(id *)(v39 + v40), "unlock");
  swift_unknownObjectWeakDestroy(v60, v47);
}

/* ========================================================================
 * sub_10002EBE0
 * EA: 0x10002ebe0
 ======================================================================== */

void __fastcall sub_10002EBE0(__int64 a1, double a2, double a3, double a4, __int64 a5, char a6, __int64 a7)
{
  __int64 Strong; // x0
  void *v14; // x19
  void *v15; // x0
  void *v16; // x24
  id v17; // x23
  float v18; // s0
  float v19; // s1
  float v20; // s2
  double v21; // d0
  _BYTE v22[24]; // [xsp+8h] [xbp-68h] BYREF

  swift_beginAccess(a7, v22, 0, 0);
  Strong = swift_unknownObjectWeakLoadStrong(a7);
  if ( Strong )
  {
    v14 = (void *)Strong;
    v15 = (void *)swift_unknownObjectWeakLoadStrong(Strong + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view);
    if ( v15 )
    {
      v16 = v15;
      v17 = objc_retainAutoreleasedReturnValue(objc_msgSend(v15, "pointOfView"));
      objc_release(v16);
      if ( v17 )
      {
        if ( (a6 & 1) != 0 )
        {
          v18 = a2;
          v19 = a3;
          v20 = a4;
          sub_1000AD04C(v18, v19, v20);
        }
        else
        {
          v21 = *(double *)&a1;
        }
        objc_msgSend(v17, "setSimdOrientation:", v21);
        objc_release(v17);
      }
    }
    objc_release(v14);
  }
}

/* ========================================================================
 * sub_10002ECDC
 * EA: 0x10002ecdc
 ======================================================================== */

__int64 sub_10002ECDC()
{
  _BYTE *v0; // x20
  _BYTE *v1; // x19
  __int64 result; // x0
  __int64 v3; // x21
  __int64 v4; // x1
  __int64 v5; // x25
  __int64 v6; // x27
  double v7; // d8
  __int64 v8; // x26
  __int128 v9; // q1
  __int128 v10; // q1
  __int128 v11; // q1
  __int128 v12; // q1
  __int128 v13; // q1
  NSApplication *v14; // x20
  id v15; // x0
  id v16; // x22
  __int64 v17; // x0
  __int64 v18; // x0
  void *v19; // x0
  id v20; // x23
  id v21; // x0
  void *v22; // x20
  __int64 v23; // x0
  __int64 v24; // x1
  __int64 v25; // x20
  __int64 v26; // x22
  __int64 v27; // x0
  char v28; // w1
  int v29; // w20
  __int64 *v30; // x8
  __int64 v31; // x0
  __int64 inited; // x20
  __int64 v33; // x0
  char v34; // w2
  __int64 v35; // x21
  unsigned __int64 v36; // x23
  unsigned __int64 v37; // x1
  __int64 v38; // x21
  __int64 v39; // x1
  __int64 v40; // x26
  __int64 v41; // x23
  __int64 v42; // x0
  __int64 v43; // x24
  double v44; // d8
  __int64 v45; // x27
  __int128 v46; // q1
  __int128 v47; // q1
  __int128 v48; // q1
  __int128 v49; // q1
  __int128 v50; // q1
  NSApplication *v51; // x20
  id v52; // x0
  id v53; // x23
  __int64 v54; // x0
  __int64 v55; // x0
  void *v56; // x0
  id v57; // x28
  id v58; // x0
  void *v59; // x20
  char v60; // w1
  char v61; // w20
  _BOOL4 v62; // w8
  _BOOL4 v63; // w9
  __int64 v64; // x20
  char *v65; // x26
  __int64 v66; // x0
  __int64 v67; // x22
  __int64 v68; // x0
  char v69; // w2
  __int64 v70; // x23
  unsigned __int64 v71; // x24
  unsigned __int64 v72; // x1
  __int64 v73; // x24
  __int64 v74; // x1
  __int64 v75; // x25
  __int64 v76; // x20
  __int64 v77; // x0
  __int64 v78; // x23
  double v79; // d8
  __int64 v80; // x26
  __int128 v81; // q1
  __int128 v82; // q1
  __int128 v83; // q1
  __int128 v84; // q1
  __int128 v85; // q1
  NSApplication *v86; // x20
  id v87; // x0
  id v88; // x22
  __int64 v89; // x0
  __int64 v90; // x0
  void *v91; // x0
  id v92; // x28
  id v93; // x0
  void *v94; // x20
  __int64 v95; // x0
  void *v96; // x25
  double v97; // d10
  double v98; // d1
  double v99; // d9
  __int64 v100; // x0
  _QWORD *v101; // x26
  __int64 v102; // x0
  void *v103; // x26
  __int64 v104; // x27
  id v105; // x26
  __int64 v106; // x1
  id v107; // x26
  void *v108; // x26
  void *v109; // x21
  __int64 v110; // x0
  void *v111; // x20
  __int64 v112; // x22
  _BYTE *v113; // x0
  void *v114; // x24
  double v115; // d10
  double v116; // d1
  double v117; // d9
  __int64 v118; // x0
  _QWORD *v119; // x21
  __int64 v120; // x0
  void *v121; // x21
  __int64 v122; // x25
  id v123; // x21
  __int64 v124; // x1
  id v125; // x21
  void *v126; // x21
  void *v127; // x24
  double v128; // d10
  double v129; // d1
  double v130; // d9
  __int64 v131; // x0
  _QWORD *v132; // x21
  __int64 v133; // x0
  void *v134; // x21
  __int64 v135; // x25
  id v136; // x21
  __int64 v137; // x1
  id v138; // x21
  void *v139; // x21
  char v140[24]; // [xsp+10h] [xbp-460h] BYREF
  char v141[24]; // [xsp+28h] [xbp-448h] BYREF
  char v142[24]; // [xsp+40h] [xbp-430h] BYREF
  char v143[24]; // [xsp+58h] [xbp-418h] BYREF
  char v144[72]; // [xsp+70h] [xbp-400h] BYREF
  char v145[32]; // [xsp+B8h] [xbp-3B8h] BYREF
  _QWORD aBlock[5]; // [xsp+D8h] [xbp-398h] BYREF
  __int64 v147; // [xsp+100h] [xbp-370h]
  char v148[24]; // [xsp+190h] [xbp-2E0h] BYREF
  char v149[24]; // [xsp+1A8h] [xbp-2C8h] BYREF
  _BYTE v150[24]; // [xsp+1C0h] [xbp-2B0h] BYREF
  char v151[72]; // [xsp+1D8h] [xbp-298h] BYREF
  _OWORD v152[11]; // [xsp+220h] [xbp-250h] BYREF
  __int64 v153; // [xsp+2D0h] [xbp-1A0h]
  _OWORD v154[2]; // [xsp+2E0h] [xbp-190h] BYREF
  char v155; // [xsp+300h] [xbp-170h]
  _OWORD v156[2]; // [xsp+310h] [xbp-160h] BYREF
  char v157; // [xsp+330h] [xbp-140h]
  __int128 v158; // [xsp+340h] [xbp-130h] BYREF
  __int128 v159; // [xsp+350h] [xbp-120h]
  __int128 v160; // [xsp+360h] [xbp-110h]
  __int128 v161; // [xsp+370h] [xbp-100h]
  __int128 v162; // [xsp+380h] [xbp-F0h]
  __int128 v163; // [xsp+390h] [xbp-E0h]
  __int128 v164; // [xsp+3A0h] [xbp-D0h]
  __int128 v165; // [xsp+3B0h] [xbp-C0h]
  __int128 v166; // [xsp+3C0h] [xbp-B0h]
  __int128 v167; // [xsp+3D0h] [xbp-A0h]
  __int128 v168; // [xsp+3E0h] [xbp-90h]
  __int64 v169; // [xsp+3F0h] [xbp-80h]

  v1 = v0;
  if ( qword_100339480 != -1 )
    swift_once(&qword_100339480, sub_1000FE3CC);
  if ( sub_1000FF000() < 4352
    || sub_1000FF000() == 4608
    || sub_1000FF000() == 4609
    || sub_1000FF000() == 4624
    || sub_1000FF000() == 4625
    || (result = sub_1000317FC(), (result & 1) == 0) )
  {
    v23 = Notification.userInfo.getter();
    if ( !v23 )
      goto LABEL_91;
    v25 = v23;
    v26 = sub_10011109C();
    swift_bridgeObjectRelease(v25);
    if ( !v26 )
      goto LABEL_91;
    if ( !*(_QWORD *)(v26 + 16) )
      goto LABEL_90;
    swift_bridgeObjectRetain(v26);
    v27 = sub_1000B2868(0xD000000000000010LL, 0x800000010026CF50LL);
    if ( (v28 & 1) == 0 )
    {
      swift_bridgeObjectRelease(v26);
      goto LABEL_44;
    }
    v29 = *(unsigned __int8 *)(*(_QWORD *)(v26 + 56) + v27);
    result = swift_bridgeObjectRelease(v26);
    v30 = &OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustEndNeeded;
    if ( !v29 )
      v30 = &OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustStartNeeded;
    v1[*v30] = 1;
    if ( NSApp )
    {
      v31 = sub_100004DF0(&unk_10033BE98, &unk_10025B0D0);
      inited = swift_initStackObject(v31, v144);
      *(_OWORD *)(inited + 16) = xmmword_10025B110;
      if ( qword_100339328 != -1 )
        swift_once(&qword_100339328, sub_100021068);
      if ( qword_1003392C8 != -1 )
        swift_once(&qword_1003392C8, sub_10001EE40);
      v156[0] = xmmword_100344268;
      v156[1] = xmmword_100344278;
      v157 = byte_100344288;
      v33 = sub_100183D60(v156);
      if ( (v34 & 1) != 0 )
      {
        v35 = 0;
        v36 = 0xE000000000000000LL;
      }
      else
      {
        v33 = sub_100194AA8(v33);
        v35 = v33;
        v36 = v37;
      }
      *(_QWORD *)(inited + 56) = &type metadata for String;
      *(_QWORD *)(inited + 64) = sub_10000D4DC(v33);
      *(_QWORD *)(inited + 32) = v35;
      *(_QWORD *)(inited + 40) = v36;
      v38 = sub_100048018(0x1000000000000072LL, 0x800000010026E000LL, inited);
      v40 = v39;
      swift_setDeallocating(inited);
      v41 = *(_QWORD *)(inited + 16);
      v42 = sub_100004DF0(&unk_10033BEA0, &unk_10025C090);
      swift_arrayDestroy(inited + 32, v41, v42);
      if ( qword_100339410 != -1 )
        swift_once(&qword_100339410, sub_10009F608);
      v43 = qword_100344508;
      swift_beginAccess(qword_100344508 + 208, v143, 0, 0);
      v44 = *(double *)(v43 + 208);
      swift_beginAccess(v43 + 216, v142, 0, 0);
      v45 = *(unsigned __int8 *)(v43 + 216);
      result = swift_beginAccess(v43 + 16, v141, 0, 0);
      v46 = *(_OWORD *)(v43 + 160);
      v166 = *(_OWORD *)(v43 + 144);
      v167 = v46;
      v168 = *(_OWORD *)(v43 + 176);
      v169 = *(_QWORD *)(v43 + 192);
      v47 = *(_OWORD *)(v43 + 96);
      v162 = *(_OWORD *)(v43 + 80);
      v163 = v47;
      v48 = *(_OWORD *)(v43 + 128);
      v164 = *(_OWORD *)(v43 + 112);
      v165 = v48;
      v49 = *(_OWORD *)(v43 + 32);
      v158 = *(_OWORD *)(v43 + 16);
      v159 = v49;
      v50 = *(_OWORD *)(v43 + 64);
      v160 = *(_OWORD *)(v43 + 48);
      v161 = v50;
      v51 = NSApp;
      if ( NSApp )
      {
        sub_100032884(&v158, v152);
        v52 = objc_retainAutoreleasedReturnValue(-[NSApplication delegate](v51, "delegate", 1, 2));
        if ( v52 )
        {
          v53 = v52;
          v54 = type metadata accessor for AppDelegate(0);
          v55 = swift_dynamicCastClass(v53, v54);
          if ( v55 )
          {
            v56 = *(void **)(v55 + OBJC_IVAR____TtC11SpaceWalker11AppDelegate_window);
            if ( v56 )
            {
              v57 = objc_retain(v56);
              v58 = objc_retainAutoreleasedReturnValue(objc_msgSend(v57, "contentView"));
              if ( v58 )
              {
                v59 = v58;
                v96 = (void *)sub_10009C704(v38, v40, 0, 0, 0, &v158);
                swift_bridgeObjectRelease(v40);
                v97 = sub_10009DA74(v96, v59, v45);
                v99 = v98;
                if ( qword_1003393F8 != -1 )
                  swift_once(&qword_1003393F8, sub_10009C44C);
                v100 = _s22ToastCompletionWrapperCMa(0);
                v101 = (_QWORD *)swift_allocObject(v100, 32, 7);
                v101[2] = 0;
                v101[3] = 0;
                swift_beginAccess(&qword_10033E7C8, v152, 32, 0);
                objc_setAssociatedObject(v96, &qword_10033E7C8, v101, (void *)1);
                swift_endAccess(v152);
                swift_release(v101);
                v102 = swift_beginAccess(v43 + 200, v140, 0, 0);
                if ( (*(_BYTE *)(v43 + 200) & 1) != 0
                  && (v103 = (void *)sub_10009C570(v102),
                      v104 = (__int64)objc_msgSend(v103, "count"),
                      objc_release(v103),
                      v104 >= 1) )
                {
                  if ( qword_1003393E8 != -1 )
                    swift_once(&qword_1003393E8, sub_10009C40C);
                  v105 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSNumber), "initWithDouble:", v44);
                  swift_beginAccess(&qword_10033E7B8, v152, 32, 0);
                  objc_setAssociatedObject(v96, &qword_10033E7B8, v105, (void *)1);
                  swift_endAccess(v152);
                  objc_release(v105);
                  if ( qword_1003393F0 != -1 )
                    swift_once(&qword_1003393F0, sub_10009C42C);
                  v107 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSValue, v106), "valueWithPoint:", v97, v99));
                  swift_beginAccess(&qword_10033E7C0, v152, 32, 0);
                  objc_setAssociatedObject(v96, &qword_10033E7C0, v107, (void *)1);
                  swift_endAccess(v152);
                  objc_release(v107);
                  v108 = (void *)sub_10009C58C();
                  objc_msgSend(v108, "addObject:", v96);
                  sub_1000328C0(&v158);
                  objc_release(v108);
                }
                else
                {
                  sub_10009DBFC(v96, v44, v97, v99);
                  sub_1000328C0(&v158);
                }
                objc_release(v96);
                swift_unknownObjectRelease(v53);
                objc_release(v59);
                objc_release(v57);
                if ( !*(_QWORD *)(v26 + 16) )
                  goto LABEL_90;
LABEL_45:
                swift_bridgeObjectRetain(v26);
                sub_1000B2868(0x6A64417465736572LL, 0xEF746E656D747375LL);
                v61 = v60;
                result = swift_bridgeObjectRelease(v26);
                if ( (v61 & 1) != 0 )
                {
                  v62 = *(double *)&v1[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartIMU] == SCNVector3Zero.x;
                  if ( *(double *)&v1[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartIMU + 8] != SCNVector3Zero.y )
                    v62 = 0;
                  v63 = *(double *)&v1[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartIMU + 16] == SCNVector3Zero.z;
                  if ( v62 && v63 )
                    v64 = 0x1000000000000075LL;
                  else
                    v64 = 0x100000000000008CLL;
                  if ( v62 && v63 )
                    v65 = "title, and image are all nil";
                  else
                    v65 = "adjustedVector is ";
                  if ( NSApp )
                  {
                    swift_bridgeObjectRelease(v26);
                    v66 = sub_100004DF0(&unk_10033BE98, &unk_10025B0D0);
                    v67 = swift_initStackObject(v66, v151);
                    *(_OWORD *)(v67 + 16) = xmmword_10025B110;
                    if ( qword_100339328 != -1 )
                      swift_once(&qword_100339328, sub_100021068);
                    if ( qword_1003392C8 != -1 )
                      swift_once(&qword_1003392C8, sub_10001EE40);
                    v154[0] = xmmword_100344268;
                    v154[1] = xmmword_100344278;
                    v155 = byte_100344288;
                    v68 = sub_100183D60(v154);
                    if ( (v69 & 1) != 0 )
                    {
                      v70 = 0;
                      v71 = 0xE000000000000000LL;
                    }
                    else
                    {
                      v68 = sub_100194AA8(v68);
                      v70 = v68;
                      v71 = v72;
                    }
                    *(_QWORD *)(v67 + 56) = &type metadata for String;
                    *(_QWORD *)(v67 + 64) = sub_10000D4DC(v68);
                    *(_QWORD *)(v67 + 32) = v70;
                    *(_QWORD *)(v67 + 40) = v71;
                    v73 = sub_100048018(v64, (unsigned __int64)v65 | 0x8000000000000000LL, v67);
                    v75 = v74;
                    swift_bridgeObjectRelease((unsigned __int64)v65 | 0x8000000000000000LL);
                    swift_setDeallocating(v67);
                    v76 = *(_QWORD *)(v67 + 16);
                    v77 = sub_100004DF0(&unk_10033BEA0, &unk_10025C090);
                    swift_arrayDestroy(v67 + 32, v76, v77);
                    if ( qword_100339410 != -1 )
                      swift_once(&qword_100339410, sub_10009F608);
                    v78 = qword_100344508;
                    swift_beginAccess(qword_100344508 + 208, v150, 0, 0);
                    v79 = *(double *)(v78 + 208);
                    swift_beginAccess(v78 + 216, v149, 0, 0);
                    v80 = *(unsigned __int8 *)(v78 + 216);
                    result = swift_beginAccess(v78 + 16, v148, 0, 0);
                    v81 = *(_OWORD *)(v78 + 160);
                    v152[8] = *(_OWORD *)(v78 + 144);
                    v152[9] = v81;
                    v152[10] = *(_OWORD *)(v78 + 176);
                    v153 = *(_QWORD *)(v78 + 192);
                    v82 = *(_OWORD *)(v78 + 96);
                    v152[4] = *(_OWORD *)(v78 + 80);
                    v152[5] = v82;
                    v83 = *(_OWORD *)(v78 + 128);
                    v152[6] = *(_OWORD *)(v78 + 112);
                    v152[7] = v83;
                    v84 = *(_OWORD *)(v78 + 32);
                    v152[0] = *(_OWORD *)(v78 + 16);
                    v152[1] = v84;
                    v85 = *(_OWORD *)(v78 + 64);
                    v152[2] = *(_OWORD *)(v78 + 48);
                    v152[3] = v85;
                    v86 = NSApp;
                    if ( NSApp )
                    {
                      sub_100032884(v152, aBlock);
                      v87 = objc_retainAutoreleasedReturnValue(-[NSApplication delegate](v86, "delegate", 1, 2));
                      if ( v87 )
                      {
                        v88 = v87;
                        v89 = type metadata accessor for AppDelegate(0);
                        v90 = swift_dynamicCastClass(v88, v89);
                        if ( v90 )
                        {
                          v91 = *(void **)(v90 + OBJC_IVAR____TtC11SpaceWalker11AppDelegate_window);
                          if ( v91 )
                          {
                            v92 = objc_retain(v91);
                            v93 = objc_retainAutoreleasedReturnValue(objc_msgSend(v92, "contentView"));
                            if ( v93 )
                            {
                              v94 = v93;
                              v114 = (void *)sub_10009C704(v73, v75, 0, 0, 0, v152);
                              swift_bridgeObjectRelease(v75);
                              v115 = sub_10009DA74(v114, v94, v80);
                              v117 = v116;
                              if ( qword_1003393F8 != -1 )
                                swift_once(&qword_1003393F8, sub_10009C44C);
                              v118 = _s22ToastCompletionWrapperCMa(0);
                              v119 = (_QWORD *)swift_allocObject(v118, 32, 7);
                              v119[2] = 0;
                              v119[3] = 0;
                              swift_beginAccess(&qword_10033E7C8, aBlock, 32, 0);
                              objc_setAssociatedObject(v114, &qword_10033E7C8, v119, (void *)1);
                              swift_endAccess(aBlock);
                              swift_release(v119);
                              v120 = swift_beginAccess(v78 + 200, v145, 0, 0);
                              if ( (*(_BYTE *)(v78 + 200) & 1) != 0
                                && (v121 = (void *)sub_10009C570(v120),
                                    v122 = (__int64)objc_msgSend(v121, "count"),
                                    objc_release(v121),
                                    v122 >= 1) )
                              {
                                if ( qword_1003393E8 != -1 )
                                  swift_once(&qword_1003393E8, sub_10009C40C);
                                v123 = objc_msgSend(
                                         objc_allocWithZone((Class)&OBJC_CLASS___NSNumber),
                                         "initWithDouble:",
                                         v79);
                                swift_beginAccess(&qword_10033E7B8, aBlock, 32, 0);
                                objc_setAssociatedObject(v114, &qword_10033E7B8, v123, (void *)1);
                                swift_endAccess(aBlock);
                                objc_release(v123);
                                if ( qword_1003393F0 != -1 )
                                  swift_once(&qword_1003393F0, sub_10009C42C);
                                v125 = objc_retainAutoreleasedReturnValue(
                                         objc_msgSend(
                                           (id)objc_opt_self(&OBJC_CLASS___NSValue, v124),
                                           "valueWithPoint:",
                                           v115,
                                           v117));
                                swift_beginAccess(&qword_10033E7C0, aBlock, 32, 0);
                                objc_setAssociatedObject(v114, &qword_10033E7C0, v125, (void *)1);
                                swift_endAccess(aBlock);
                                objc_release(v125);
                                v126 = (void *)sub_10009C58C();
                                objc_msgSend(v126, "addObject:", v114);
                                sub_1000328C0(v152);
                                objc_release(v126);
                              }
                              else
                              {
                                sub_10009DBFC(v114, v79, v115, v117);
                                sub_1000328C0(v152);
                              }
                              objc_release(v114);
                              swift_unknownObjectRelease(v88);
                              objc_release(v94);
                            }
                            else
                            {
                              swift_bridgeObjectRelease(v75);
                              sub_1000328C0(v152);
                              swift_unknownObjectRelease(v88);
                            }
                            objc_release(v92);
                          }
                          else
                          {
                            swift_bridgeObjectRelease(v75);
                            sub_1000328C0(v152);
                            v95 = swift_unknownObjectRelease(v88);
                          }
                          goto LABEL_74;
                        }
                        swift_bridgeObjectRelease(v75);
                        swift_unknownObjectRelease(v88);
                      }
                      else
                      {
                        swift_bridgeObjectRelease(v75);
                      }
                      v95 = sub_1000328C0(v152);
LABEL_74:
                      sub_1000303A0(v95);
                      goto LABEL_91;
                    }
LABEL_125:
                    __break(1u);
                    goto LABEL_126;
                  }
LABEL_124:
                  __break(1u);
                  goto LABEL_125;
                }
LABEL_90:
                swift_bridgeObjectRelease(v26);
                goto LABEL_91;
              }
              swift_bridgeObjectRelease(v40);
              sub_1000328C0(&v158);
              swift_unknownObjectRelease(v53);
              objc_release(v57);
            }
            else
            {
              swift_bridgeObjectRelease(v40);
              sub_1000328C0(&v158);
              swift_unknownObjectRelease(v53);
            }
LABEL_44:
            if ( !*(_QWORD *)(v26 + 16) )
              goto LABEL_90;
            goto LABEL_45;
          }
          swift_bridgeObjectRelease(v40);
          swift_unknownObjectRelease(v53);
        }
        else
        {
          swift_bridgeObjectRelease(v40);
        }
        sub_1000328C0(&v158);
        goto LABEL_44;
      }
    }
    else
    {
      __break(1u);
    }
    __break(1u);
    goto LABEL_124;
  }
  if ( !NSApp )
  {
LABEL_126:
    __break(1u);
LABEL_127:
    __break(1u);
    return result;
  }
  v3 = sub_100048018(0x1000000000000073LL, 0x800000010026E080LL, 0);
  v5 = v4;
  if ( qword_100339410 != -1 )
    swift_once(&qword_100339410, sub_10009F608);
  v6 = qword_100344508;
  swift_beginAccess(qword_100344508 + 208, v156, 0, 0);
  v7 = *(double *)(v6 + 208);
  swift_beginAccess(v6 + 216, v154, 0, 0);
  v8 = *(unsigned __int8 *)(v6 + 216);
  result = swift_beginAccess(v6 + 16, v150, 0, 0);
  v9 = *(_OWORD *)(v6 + 160);
  v166 = *(_OWORD *)(v6 + 144);
  v167 = v9;
  v168 = *(_OWORD *)(v6 + 176);
  v169 = *(_QWORD *)(v6 + 192);
  v10 = *(_OWORD *)(v6 + 96);
  v162 = *(_OWORD *)(v6 + 80);
  v163 = v10;
  v11 = *(_OWORD *)(v6 + 128);
  v164 = *(_OWORD *)(v6 + 112);
  v165 = v11;
  v12 = *(_OWORD *)(v6 + 32);
  v158 = *(_OWORD *)(v6 + 16);
  v159 = v12;
  v13 = *(_OWORD *)(v6 + 64);
  v160 = *(_OWORD *)(v6 + 48);
  v161 = v13;
  v14 = NSApp;
  if ( !NSApp )
    goto LABEL_127;
  sub_100032884(&v158, v152);
  v15 = objc_retainAutoreleasedReturnValue(-[NSApplication delegate](v14, "delegate"));
  if ( v15 )
  {
    v16 = v15;
    v17 = type metadata accessor for AppDelegate(0);
    v18 = swift_dynamicCastClass(v16, v17);
    if ( v18 )
    {
      v19 = *(void **)(v18 + OBJC_IVAR____TtC11SpaceWalker11AppDelegate_window);
      if ( v19 )
      {
        v20 = objc_retain(v19);
        v21 = objc_retainAutoreleasedReturnValue(objc_msgSend(v20, "contentView"));
        if ( v21 )
        {
          v22 = v21;
          v127 = (void *)sub_10009C704(v3, v5, 0, 0, 0, &v158);
          swift_bridgeObjectRelease(v5);
          v128 = sub_10009DA74(v127, v22, v8);
          v130 = v129;
          if ( qword_1003393F8 != -1 )
            swift_once(&qword_1003393F8, sub_10009C44C);
          v131 = _s22ToastCompletionWrapperCMa(0);
          v132 = (_QWORD *)swift_allocObject(v131, 32, 7);
          v132[2] = 0;
          v132[3] = 0;
          swift_beginAccess(&qword_10033E7C8, v152, 32, 0);
          objc_setAssociatedObject(v127, &qword_10033E7C8, v132, (void *)1);
          swift_endAccess(v152);
          swift_release(v132);
          v133 = swift_beginAccess(v6 + 200, v152, 0, 0);
          if ( (*(_BYTE *)(v6 + 200) & 1) != 0
            && (v134 = (void *)sub_10009C570(v133),
                v135 = (__int64)objc_msgSend(v134, "count"),
                objc_release(v134),
                v135 >= 1) )
          {
            if ( qword_1003393E8 != -1 )
              swift_once(&qword_1003393E8, sub_10009C40C);
            v136 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSNumber), "initWithDouble:", v7);
            swift_beginAccess(&qword_10033E7B8, aBlock, 32, 0);
            objc_setAssociatedObject(v127, &qword_10033E7B8, v136, (void *)1);
            swift_endAccess(aBlock);
            objc_release(v136);
            if ( qword_1003393F0 != -1 )
              swift_once(&qword_1003393F0, sub_10009C42C);
            v138 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSValue, v137), "valueWithPoint:", v128, v130));
            swift_beginAccess(&qword_10033E7C0, aBlock, 32, 0);
            objc_setAssociatedObject(v127, &qword_10033E7C0, v138, (void *)1);
            swift_endAccess(aBlock);
            objc_release(v138);
            v139 = (void *)sub_10009C58C();
            objc_msgSend(v139, "addObject:", v127);
            sub_1000328C0(&v158);
            objc_release(v139);
          }
          else
          {
            sub_10009DBFC(v127, v7, v128, v130);
            sub_1000328C0(&v158);
          }
          objc_release(v127);
          swift_unknownObjectRelease(v16);
          objc_release(v22);
        }
        else
        {
          swift_bridgeObjectRelease(v5);
          sub_1000328C0(&v158);
          swift_unknownObjectRelease(v16);
        }
        objc_release(v20);
      }
      else
      {
        swift_bridgeObjectRelease(v5);
        sub_1000328C0(&v158);
        swift_unknownObjectRelease(v16);
      }
      goto LABEL_91;
    }
    swift_bridgeObjectRelease(v5);
    swift_unknownObjectRelease(v16);
  }
  else
  {
    swift_bridgeObjectRelease(v5);
  }
  sub_1000328C0(&v158);
LABEL_91:
  v1[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isAnimatingReorientation] = 1;
  v109 = (void *)objc_opt_self(&OBJC_CLASS___SCNTransaction, v24);
  objc_msgSend(v109, "begin");
  sub_100031334(objc_msgSend(v109, "setAnimationDuration:", 0.25));
  v110 = swift_allocObject(&unk_1002DDBE0, 24, 7);
  *(_QWORD *)(v110 + 16) = v1;
  aBlock[4] = sub_100032840;
  v147 = v110;
  aBlock[0] = _NSConcreteStackBlock;
  aBlock[1] = 1107296256;
  aBlock[2] = sub_10007A08C;
  aBlock[3] = &unk_1002DDBF8;
  v111 = _Block_copy(aBlock);
  v112 = v147;
  v113 = objc_retain(v1);
  swift_release(v112);
  objc_msgSend(v109, "setCompletionBlock:", v111);
  _Block_release(v111);
  result = (__int64)objc_msgSend(v109, "commit");
  if ( qword_100339350 != -1 )
    result = swift_once(&qword_100339350, sub_100070B80);
  *(_DWORD *)(qword_100344468 + 120) = 0;
  return result;
}

/* ========================================================================
 * sub_1000302F8
 * EA: 0x1000302f8
 ======================================================================== */

__int64 __fastcall sub_1000302F8(void *a1, __int64 a2, __int64 a3, void (__fastcall *a4)(_QWORD *))
{
  __int64 v7; // x22
  __int64 v8; // x24
  _QWORD *v9; // x23
  id v10; // x20
  __int64 v12; // [xsp+0h] [xbp-30h] BYREF

  v7 = type metadata accessor for Notification(0, a2);
  v8 = *(_QWORD *)(v7 - 8);
  v9 = (__int64 *)((char *)&v12 - ((*(_QWORD *)(v8 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL));
  static Notification._unconditionallyBridgeFromObjectiveC(_:)(v9, a3);
  v10 = objc_retain(a1);
  a4(v9);
  objc_release(v10);
  return (*(__int64 (__fastcall **)(_QWORD *, __int64))(v8 + 8))(v9, v7);
}

/* ========================================================================
 * sub_1000303A0
 * EA: 0x1000303a0
 ======================================================================== */

void __fastcall sub_1000303A0(__int64 a1)
{
  __int64 v1; // x20
  CGFloat y; // d1
  CGFloat z; // d2
  CGFloat *v4; // x8
  CGFloat *v5; // x8
  id v6; // x19
  __int64 v7; // d0
  __int64 v8; // d8
  id v9; // x19
  __int64 v10; // d0
  __int64 v11; // d8

  sub_1000304CC(a1);
  *(_BYTE *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustStartNeeded) = 0;
  *(_BYTE *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustEndNeeded) = 0;
  y = SCNVector3Zero.y;
  z = SCNVector3Zero.z;
  v4 = (CGFloat *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartIMU);
  *v4 = SCNVector3Zero.x;
  v4[1] = y;
  v4[2] = z;
  v5 = (CGFloat *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndIMU);
  *v5 = SCNVector3Zero.x;
  v5[1] = y;
  v5[2] = z;
  v6 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSDate), "init");
  objc_msgSend(v6, "timeIntervalSince1970");
  v8 = v7;
  objc_release(v6);
  *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartTS) = v8;
  v9 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSDate), "init");
  objc_msgSend(v9, "timeIntervalSince1970");
  v11 = v10;
  objc_release(v9);
  *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndTS) = v11;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedCounter) = 0;
}

/* ========================================================================
 * sub_1000304CC
 * EA: 0x1000304cc
 ======================================================================== */

// local variable allocation has failed, the output may be wrong!
__int64 sub_1000304CC()
{
  __int64 v0; // x20
  __int64 result; // x0
  double v2; // d1
  double v3; // d2
  double v4; // d0
  __int64 v7; // d1
  double v8; // d8
  double v9; // d1
  double v10; // d9
  double v11; // d1
  double v12; // d8
  __int64 v13; // x21
  Swift::String v14; // x0
  void *object; // x22
  __int64 v16; // x0
  __int64 v17; // x23
  __int64 v18; // x0
  Swift::String v19; // x0
  void *v20; // x22
  __int64 v21; // x0
  __int64 v22; // x23
  __int64 v23; // x0
  __int64 v24; // x0 OVERLAPPED
  unsigned __int64 v25; // x22
  unsigned __int64 v26; // x1
  __int64 v27; // x0
  __int64 v28; // x23
  __int64 v29; // x0
  __int64 v30; // x0 OVERLAPPED
  unsigned __int64 v31; // x22
  unsigned __int64 v32; // x1
  __int64 v33; // x0
  __int64 v34; // x23
  __int64 v35; // x0
  Swift::String v36; // x0
  __int64 v37; // d0
  __int64 v38; // x22
  __int64 v39; // x0
  __int128 v40; // kr00_16
  __int64 v41; // x26
  __int64 v42; // x0
  Swift::String v43; // x0
  __int64 v44; // d0
  __int64 v45; // x0
  __int128 v46; // kr10_16
  __int64 v47; // x26
  __int64 v48; // x0
  Swift::String v49; // x0
  __int64 v50; // x0
  __int128 v51; // kr20_16
  __int64 v52; // x27
  __int64 v53; // x0
  Swift::String v54; // x0
  __int64 v55; // x0
  __int128 v56; // kr30_16
  __int64 v57; // x27
  __int64 v58; // x0
  Swift::String v59; // x0
  void *v60; // x25
  __int64 v61; // x0
  __int128 v62; // kr40_16
  __int64 v63; // x26
  __int64 v64; // x0
  Swift::String v65; // x0
  __int64 v66; // d0
  __int64 v67; // d1
  __int64 v68; // d2
  __int64 v69; // x0
  __int128 v70; // kr50_16
  __int64 v71; // x22
  __int64 v72; // x0
  __int64 v73; // [xsp+8h] [xbp-98h]
  __int128 v74; // [xsp+10h] [xbp-90h] BYREF
  __int64 v75; // [xsp+20h] [xbp-80h]
  __int128 v76; // [xsp+30h] [xbp-70h] BYREF

  result = sub_10002D1CC();
  if ( v4 != SCNVector3Zero.x || v2 != SCNVector3Zero.y || v3 != SCNVector3Zero.z )
  {
    result = sub_10002D1CC();
    if ( (~v7 & 0x7FF0000000000000LL) != 0 || (v7 & 0xFFFFFFFFFFFFFLL) == 0 )
    {
      v73 = OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedCounter;
      v8 = (double)*(__int64 *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedCounter);
      sub_10002D1CC();
      v10 = v9 * v8;
      sub_10002D1CC();
      v12 = v11 * 240.0 * 60.0;
      v13 = type metadata accessor for VTLogger(0);
      _StringGuts.grow(_:)(17);
      swift_bridgeObjectRelease(0xE000000000000000LL);
      v14._countAndFlagsBits = Double.description.getter(v10);
      object = v14._object;
      String.append(_:)(v14);
      v16 = swift_bridgeObjectRelease(object);
      v17 = static os_log_type_t.info.getter(v16);
      v18 = static os_log_type_t.info.getter(v17);
      sub_100091CD8(v17, v18, 0x64657473756A6461LL, 0xEF20736920776159LL, v13);
      swift_bridgeObjectRelease(0xEF20736920776159LL);
      _StringGuts.grow(_:)(19);
      swift_bridgeObjectRelease(0xE000000000000000LL);
      v19._countAndFlagsBits = Double.description.getter(v12);
      v20 = v19._object;
      String.append(_:)(v19);
      v21 = swift_bridgeObjectRelease(v20);
      v22 = static os_log_type_t.info.getter(v21);
      v23 = static os_log_type_t.info.getter(v22);
      sub_100091CD8(v22, v23, 0xD000000000000011LL, 0x800000010026DDC0LL, v13);
      swift_bridgeObjectRelease(0x800000010026DDC0LL);
      _StringGuts.grow(_:)(19);
      swift_bridgeObjectRelease(0xE000000000000000LL);
      *(_QWORD *)&v74 = 0xD000000000000011LL;
      *((_QWORD *)&v74 + 1) = 0x800000010026DDE0LL;
      if ( *(_BYTE *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustStartNeeded) )
        v24 = 1702195828;
      else
        v24 = 0x65736C6166LL;
      if ( *(_BYTE *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustStartNeeded) )
        v25 = 0xE400000000000000LL;
      else
        v25 = 0xE500000000000000LL;
      v26 = v25;
      String.append(_:)(*(Swift::String *)&v24);
      v27 = swift_bridgeObjectRelease(v25);
      v28 = static os_log_type_t.info.getter(v27);
      v29 = static os_log_type_t.info.getter(v28);
      sub_100091CD8(v28, v29, v74, *((_QWORD *)&v74 + 1), v13);
      swift_bridgeObjectRelease(*((_QWORD *)&v74 + 1));
      _StringGuts.grow(_:)(17);
      swift_bridgeObjectRelease(0xE000000000000000LL);
      *(_QWORD *)&v74 = 0x6E457473756A6461LL;
      *((_QWORD *)&v74 + 1) = 0xEF20736920646564LL;
      if ( *(_BYTE *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustEndNeeded) )
        v30 = 1702195828;
      else
        v30 = 0x65736C6166LL;
      if ( *(_BYTE *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustEndNeeded) )
        v31 = 0xE400000000000000LL;
      else
        v31 = 0xE500000000000000LL;
      v32 = v31;
      String.append(_:)(*(Swift::String *)&v30);
      v33 = swift_bridgeObjectRelease(v31);
      v34 = static os_log_type_t.info.getter(v33);
      v35 = static os_log_type_t.info.getter(v34);
      sub_100091CD8(v34, v35, v74, *((_QWORD *)&v74 + 1), v13);
      swift_bridgeObjectRelease(*((_QWORD *)&v74 + 1));
      *(_QWORD *)&v74 = 0;
      *((_QWORD *)&v74 + 1) = 0xE000000000000000LL;
      _StringGuts.grow(_:)(22);
      v76 = v74;
      v36._countAndFlagsBits = 0xD000000000000014LL;
      v36._object = (void *)0x800000010026DE00LL;
      String.append(_:)(v36);
      v37 = *(_QWORD *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartIMU + 16);
      v74 = *(_OWORD *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartIMU);
      v75 = v37;
      v38 = type metadata accessor for SCNVector3(0);
      v39 = _print_unlocked<A, B>(_:_:)(
              &v74,
              &v76,
              v38,
              &type metadata for DefaultStringInterpolation,
              &protocol witness table for DefaultStringInterpolation);
      v40 = v76;
      v41 = static os_log_type_t.info.getter(v39);
      v42 = static os_log_type_t.info.getter(v41);
      sub_100091CD8(v41, v42, v40, *((_QWORD *)&v40 + 1), v13);
      swift_bridgeObjectRelease(*((_QWORD *)&v40 + 1));
      *(_QWORD *)&v74 = 0;
      *((_QWORD *)&v74 + 1) = 0xE000000000000000LL;
      _StringGuts.grow(_:)(20);
      v76 = v74;
      v43._countAndFlagsBits = 0xD000000000000012LL;
      v43._object = (void *)0x800000010026DE20LL;
      String.append(_:)(v43);
      v44 = *(_QWORD *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndIMU + 16);
      v74 = *(_OWORD *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndIMU);
      v75 = v44;
      v45 = _print_unlocked<A, B>(_:_:)(
              &v74,
              &v76,
              v38,
              &type metadata for DefaultStringInterpolation,
              &protocol witness table for DefaultStringInterpolation);
      v46 = v76;
      v47 = static os_log_type_t.info.getter(v45);
      v48 = static os_log_type_t.info.getter(v47);
      sub_100091CD8(v47, v48, v46, *((_QWORD *)&v46 + 1), v13);
      swift_bridgeObjectRelease(*((_QWORD *)&v46 + 1));
      *(_QWORD *)&v74 = 0;
      *((_QWORD *)&v74 + 1) = 0xE000000000000000LL;
      _StringGuts.grow(_:)(21);
      v49._object = (void *)0x800000010026DE40LL;
      v49._countAndFlagsBits = 0xD000000000000013LL;
      String.append(_:)(v49);
      v50 = Double.write<A>(to:)(
              &v74,
              &type metadata for DefaultStringInterpolation,
              &protocol witness table for DefaultStringInterpolation,
              *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartTS));
      v51 = v74;
      v52 = static os_log_type_t.info.getter(v50);
      v53 = static os_log_type_t.info.getter(v52);
      sub_100091CD8(v52, v53, v51, *((_QWORD *)&v51 + 1), v13);
      swift_bridgeObjectRelease(*((_QWORD *)&v51 + 1));
      *(_QWORD *)&v74 = 0;
      *((_QWORD *)&v74 + 1) = 0xE000000000000000LL;
      _StringGuts.grow(_:)(19);
      v54._object = (void *)0x800000010026DE60LL;
      v54._countAndFlagsBits = 0xD000000000000011LL;
      String.append(_:)(v54);
      v55 = Double.write<A>(to:)(
              &v74,
              &type metadata for DefaultStringInterpolation,
              &protocol witness table for DefaultStringInterpolation,
              *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndTS));
      v56 = v74;
      v57 = static os_log_type_t.info.getter(v55);
      v58 = static os_log_type_t.info.getter(v57);
      sub_100091CD8(v57, v58, v56, *((_QWORD *)&v56 + 1), v13);
      swift_bridgeObjectRelease(*((_QWORD *)&v56 + 1));
      *(_QWORD *)&v74 = 0;
      *((_QWORD *)&v74 + 1) = 0xE000000000000000LL;
      _StringGuts.grow(_:)(21);
      swift_bridgeObjectRelease(*((_QWORD *)&v74 + 1));
      *(_QWORD *)&v74 = 0xD000000000000013LL;
      *((_QWORD *)&v74 + 1) = 0x800000010026DE80LL;
      *(_QWORD *)&v76 = *(_QWORD *)(v0 + v73);
      v59._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                                 &type metadata for Int,
                                 &protocol witness table for Int);
      v60 = v59._object;
      String.append(_:)(v59);
      v61 = swift_bridgeObjectRelease(v60);
      v62 = v74;
      v63 = static os_log_type_t.info.getter(v61);
      v64 = static os_log_type_t.info.getter(v63);
      sub_100091CD8(v63, v64, v62, *((_QWORD *)&v62 + 1), v13);
      swift_bridgeObjectRelease(*((_QWORD *)&v62 + 1));
      *(_QWORD *)&v74 = 0;
      *((_QWORD *)&v74 + 1) = 0xE000000000000000LL;
      _StringGuts.grow(_:)(20);
      v76 = v74;
      v65._countAndFlagsBits = 0xD000000000000012LL;
      v65._object = (void *)0x800000010026DEA0LL;
      String.append(_:)(v65);
      sub_10002D1CC();
      *(_QWORD *)&v74 = v66;
      *((_QWORD *)&v74 + 1) = v67;
      v75 = v68;
      v69 = _print_unlocked<A, B>(_:_:)(
              &v74,
              &v76,
              v38,
              &type metadata for DefaultStringInterpolation,
              &protocol witness table for DefaultStringInterpolation);
      v70 = v76;
      v71 = static os_log_type_t.info.getter(v69);
      v72 = static os_log_type_t.info.getter(v71);
      sub_100091CD8(v71, v72, v70, *((_QWORD *)&v70 + 1), v13);
      return swift_bridgeObjectRelease(*((_QWORD *)&v70 + 1));
    }
  }
  return result;
}

/* ========================================================================
 * sub_100030B78
 * EA: 0x100030b78
 ======================================================================== */

void sub_100030B78()
{
  __int64 v0; // x20
  __int128 *v1; // x19
  __int128 v2; // q1
  __int64 v3; // x21
  void *v4; // x22
  id v5; // x19
  id v6; // x22
  __int64 v7; // x0
  __int64 v8; // x0
  __int64 Strong; // x0
  void *v10; // x19
  __int64 v11; // x0
  _QWORD v12[2]; // [xsp+8h] [xbp-98h] BYREF
  _BYTE v13[24]; // [xsp+18h] [xbp-88h] BYREF
  __int128 v14; // [xsp+30h] [xbp-70h]
  __int128 v15; // [xsp+40h] [xbp-60h]
  __int64 v16; // [xsp+50h] [xbp-50h]
  __int128 v17; // [xsp+60h] [xbp-40h] BYREF

  if ( *(_BYTE *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_distanceAdjustingEnabled) == 1 )
  {
    v1 = (__int128 *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase);
    swift_beginAccess(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase, v13, 0, 0);
    v2 = v1[1];
    v14 = *v1;
    v15 = v2;
    v16 = *((_QWORD *)v1 + 4);
    v3 = *((_QWORD *)&v14 + 1);
    v4 = (void *)v2;
    v17 = *(__int128 *)((char *)v1 + 24);
    v5 = objc_retain((id)v14);
    swift_retain(v3);
    v6 = objc_retain(v4);
    sub_10001D39C(&v17, v12);
    v7 = sub_100004DF0(&unk_10033C720, &unk_100261150);
    FoilDefaultStorage.wrappedValue.getter(v12, v7);
    objc_release(v6);
    swift_release(v3);
    objc_release(v5);
    v8 = sub_10001D3D8(&v17);
    sub_100030EBC(v8, *(float *)v12 + 1.0);
    Strong = swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view);
    if ( Strong )
    {
      v10 = (void *)Strong;
      v11 = type metadata accessor for SCNCaptureVideoPreview(0);
      if ( swift_dynamicCastClass(v10, v11) )
        sub_10011A05C();
      objc_release(v10);
    }
  }
}

/* ========================================================================
 * sub_100030CC8
 * EA: 0x100030cc8
 ======================================================================== */

void sub_100030CC8()
{
  __int64 v0; // x20
  __int128 *v1; // x19
  __int128 v2; // q1
  __int64 v3; // x21
  void *v4; // x22
  id v5; // x19
  id v6; // x22
  __int64 v7; // x0
  __int64 v8; // x0
  __int64 Strong; // x0
  void *v10; // x19
  __int64 v11; // x0
  _QWORD v12[2]; // [xsp+8h] [xbp-98h] BYREF
  _BYTE v13[24]; // [xsp+18h] [xbp-88h] BYREF
  __int128 v14; // [xsp+30h] [xbp-70h]
  __int128 v15; // [xsp+40h] [xbp-60h]
  __int64 v16; // [xsp+50h] [xbp-50h]
  __int128 v17; // [xsp+60h] [xbp-40h] BYREF

  if ( *(_BYTE *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_distanceAdjustingEnabled) == 1 )
  {
    v1 = (__int128 *)(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase);
    swift_beginAccess(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase, v13, 0, 0);
    v2 = v1[1];
    v14 = *v1;
    v15 = v2;
    v16 = *((_QWORD *)v1 + 4);
    v3 = *((_QWORD *)&v14 + 1);
    v4 = (void *)v2;
    v17 = *(__int128 *)((char *)v1 + 24);
    v5 = objc_retain((id)v14);
    swift_retain(v3);
    v6 = objc_retain(v4);
    sub_10001D39C(&v17, v12);
    v7 = sub_100004DF0(&unk_10033C720, &unk_100261150);
    FoilDefaultStorage.wrappedValue.getter(v12, v7);
    objc_release(v6);
    swift_release(v3);
    objc_release(v5);
    v8 = sub_10001D3D8(&v17);
    sub_100030EBC(v8, *(float *)v12 + -1.0);
    Strong = swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view);
    if ( Strong )
    {
      v10 = (void *)Strong;
      v11 = type metadata accessor for SCNCaptureVideoPreview(0);
      if ( swift_dynamicCastClass(v10, v11) )
        sub_10011A05C();
      objc_release(v10);
    }
  }
}

/* ========================================================================
 * sub_100030E18
 * EA: 0x100030e18
 ======================================================================== */

__int64 __fastcall sub_100030E18(void *a1, __int64 a2, __int64 a3, void (*a4)(void))
{
  __int64 v7; // x22
  __int64 v8; // x24
  _QWORD *v9; // x23
  id v10; // x20
  __int64 v12; // [xsp+0h] [xbp-30h] BYREF

  v7 = type metadata accessor for Notification(0, a2);
  v8 = *(_QWORD *)(v7 - 8);
  v9 = (__int64 *)((char *)&v12 - ((*(_QWORD *)(v8 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL));
  static Notification._unconditionallyBridgeFromObjectiveC(_:)(v9, a3);
  v10 = objc_retain(a1);
  a4();
  objc_release(v10);
  return (*(__int64 (__fastcall **)(_QWORD *, __int64))(v8 + 8))(v9, v7);
}

/* ========================================================================
 * sub_100030EBC
 * EA: 0x100030ebc
 ======================================================================== */

void __fastcall sub_100030EBC(float a1)
{
  __int64 v1; // x20
  __int64 v3; // x26
  void *Strong; // x0
  void *v5; // x21
  id v6; // x19
  __int128 *v7; // x21
  __int128 v8; // q1
  __int64 v9; // x23
  void *v10; // x20
  id v11; // x24
  id v12; // x25
  __int64 v13; // x22
  double v14; // d0
  double v15; // d2
  __int128 v16; // q1
  __int64 v17; // x23
  void *v18; // x20
  id v19; // x24
  id v20; // x25
  double v21; // d0
  double v22; // d2
  __int64 v23; // x0
  __int64 v24; // x20
  __int64 v25; // x8
  unsigned __int64 v26; // x9
  __int64 v27; // x1
  id v28; // x20
  void *v29; // x21
  id v30; // x22
  __int64 v31; // [xsp+0h] [xbp-130h]
  __int64 v32; // [xsp+8h] [xbp-128h] BYREF
  unsigned __int64 v33; // [xsp+10h] [xbp-120h]
  float v34; // [xsp+24h] [xbp-10Ch] BYREF
  _BYTE v35[24]; // [xsp+28h] [xbp-108h] BYREF
  _OWORD v36[2]; // [xsp+40h] [xbp-F0h] BYREF
  __int64 v37; // [xsp+60h] [xbp-D0h]
  __int128 v38; // [xsp+70h] [xbp-C0h]
  __int128 v39; // [xsp+80h] [xbp-B0h]
  __int64 v40; // [xsp+90h] [xbp-A0h]
  __int128 v41; // [xsp+A0h] [xbp-90h] BYREF
  _OWORD v42[2]; // [xsp+B0h] [xbp-80h] BYREF

  v3 = OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view;
  Strong = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view);
  if ( Strong )
  {
    v5 = Strong;
    v6 = objc_retainAutoreleasedReturnValue(objc_msgSend(Strong, "pointOfView"));
    objc_release(v5);
    if ( v6 )
    {
      v31 = v1;
      v7 = (__int128 *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase);
      swift_beginAccess(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase, v35, 0, 0);
      v8 = v7[1];
      v38 = *v7;
      v39 = v8;
      v40 = *((_QWORD *)v7 + 4);
      v9 = *((_QWORD *)&v38 + 1);
      v10 = (void *)v8;
      v41 = *(__int128 *)((char *)v7 + 24);
      v11 = objc_retain((id)v38);
      swift_retain(v9);
      v12 = objc_retain(v10);
      sub_10001D39C(&v41, v36);
      v13 = sub_100004DF0(&unk_10033C720, &unk_100261150);
      FoilDefaultStorage.wrappedValue.getter(v36, v13);
      objc_release(v12);
      swift_release(v9);
      objc_release(v11);
      sub_10001D3D8(&v41);
      LODWORD(v14) = v36[0];
      if ( *(float *)v36 >= a1 || (objc_msgSend(v6, "position", v14), v15 < 1000.0) )
      {
        v16 = v7[1];
        v36[0] = *v7;
        v36[1] = v16;
        v37 = *((_QWORD *)v7 + 4);
        v17 = *((_QWORD *)&v36[0] + 1);
        v18 = (void *)v16;
        v42[0] = *(__int128 *)((char *)v7 + 24);
        v19 = objc_retain(*(id *)&v36[0]);
        swift_retain(v17);
        v20 = objc_retain(v18);
        sub_10001D39C(v42, &v32);
        FoilDefaultStorage.wrappedValue.getter(&v32, v13);
        objc_release(v20);
        swift_release(v17);
        objc_release(v19);
        sub_10001D3D8(v42);
        LODWORD(v21) = v32;
        if ( *(float *)&v32 <= a1 || (objc_msgSend(v6, "position", v21), v22 > -1000.0) )
        {
          if ( qword_100339480 != -1 )
            swift_once(&qword_100339480, sub_1000FE3CC);
          if ( sub_1000FF000() == 4353
            || sub_1000FF000() == 4356
            || sub_1000FF000() == 4352
            || sub_1000FF000() == 4355
            || sub_1000FF000() == 514 )
          {
            dword_10033A1C4 = LODWORD(a1);
            v34 = a1;
            swift_beginAccess(v7, &v32, 33, 0);
            FoilDefaultStorage.wrappedValue.setter(&v34, v13);
            swift_endAccess(&v32);
            v23 = sub_100004DF0(&unk_10033C990, &unk_10025C080);
            v24 = swift_allocObject(v23, 64, 7);
            *(_OWORD *)(v24 + 16) = xmmword_10025B110;
            v32 = 0;
            v33 = 0xE000000000000000LL;
            Float.write<A>(to:)(
              &v32,
              &type metadata for DefaultStringInterpolation,
              &protocol witness table for DefaultStringInterpolation,
              a1);
            v25 = v32;
            v26 = v33;
            *(_QWORD *)(v24 + 56) = &type metadata for String;
            *(_QWORD *)(v24 + 32) = v25;
            *(_QWORD *)(v24 + 40) = v26;
            print(_:separator:terminator:)(v24, 32, 0xE100000000000000LL, 10, 0xE100000000000000LL);
            objc_release(v6);
            swift_bridgeObjectRelease(v24);
            return;
          }
          v34 = a1;
          swift_beginAccess(v7, &v32, 33, 0);
          FoilDefaultStorage.wrappedValue.setter(&v34, v13);
          swift_endAccess(&v32);
          v28 = objc_retainAutoreleasedReturnValue(
                  objc_msgSend(
                    (id)objc_opt_self(&OBJC_CLASS___SCNAction, v27),
                    "moveTo:duration:",
                    0.0,
                    0.0,
                    a1 * 200.0,
                    0.3));
          v29 = (void *)swift_unknownObjectWeakLoadStrong(v31 + v3);
          v30 = objc_retainAutoreleasedReturnValue(objc_msgSend(v29, "pointOfView"));
          objc_release(v29);
          objc_msgSend(v30, "runAction:", v28);
          objc_release(v30);
          objc_release(v28);
        }
      }
      objc_release(v6);
    }
  }
}

/* ========================================================================
 * sub_100031334
 * EA: 0x100031334
 ======================================================================== */

__int64 sub_100031334()
{
  __int64 v0; // x20
  __int64 v1; // x19
  __int64 v2; // x21
  __int64 v3; // x22
  char *v4; // x20
  __int64 v5; // x0
  double v6; // d8
  __int64 v7; // x0
  void *Strong; // x0
  void *v9; // x21
  id v10; // x20
  __int64 v11; // x0
  __int64 v12; // x19
  __int64 v13; // x21
  __int64 result; // x0
  __int64 v15; // [xsp+0h] [xbp-30h] BYREF

  v1 = v0;
  v2 = type metadata accessor for Date(0);
  v3 = *(_QWORD *)(v2 - 8);
  v4 = (char *)&v15 - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_yawDriftSpeed) = 0;
  v5 = Date.init()();
  v6 = Date.timeIntervalSince1970.getter(v5);
  v7 = (*(__int64 (__fastcall **)(char *, __int64))(v3 + 8))(v4, v2);
  *(double *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastUpdatedTS) = v6;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_accumulateYawDrift) = 0;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_calibrateCounter) = 0;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_updateTSCounter) = 0;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastYawDriftSpeed) = 0;
  sub_100048E5C(v7);
  *(_BYTE *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isIMUInitialized) = 0;
  Strong = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view);
  if ( Strong )
  {
    v9 = Strong;
    v10 = objc_retainAutoreleasedReturnValue(objc_msgSend(Strong, "pointOfView"));
    objc_release(v9);
    if ( v10 )
    {
      objc_msgSend(
        v10,
        "setEulerAngles:",
        *(double *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_startingPosition),
        *(double *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_startingPosition + 8),
        *(double *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_startingPosition + 16));
      objc_release(v10);
    }
  }
  v11 = v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuController;
  v12 = *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuController + 24);
  v13 = *(_QWORD *)(v11 + 32);
  sub_10000C8A4();
  result = (*(__int64 (__fastcall **)(__int64, __int64))(v13 + 16))(v12, v13);
  qword_10033A190 = 0;
  qword_10033A198 = 0;
  byte_10033A1A0 = 1;
  qword_10033A1B0 = 0;
  qword_10033A1B8 = 0;
  byte_10033A1C0 = 1;
  return result;
}

/* ========================================================================
 * sub_100031588
 * EA: 0x100031588
 ======================================================================== */

double __fastcall sub_100031588(float32x4_t a1)
{
  float v1; // s4
  float v2; // s6
  float v3; // s7
  float v4; // s5
  float v5; // s5
  float v6; // s16
  float v7; // s9
  float v8; // s10
  float v9; // s11
  float v10; // s12
  float v11; // s9

  v1 = -a1.f32[2];
  v2 = vmuls_lane_f32(a1.f32[2], a1, 3);
  v3 = -a1.f32[1];
  v4 = vmuls_lane_f32(-a1.f32[1], a1, 3);
  v5 = (float)(v4 + (float)(a1.f32[0] * a1.f32[2])) + (float)(v4 + (float)(a1.f32[0] * a1.f32[2]));
  v6 = -a1.f32[0];
  if ( fabsf(v5) >= 1.0 )
  {
    v11 = atan2f(
            (float)-(float)(v2 - (float)(a1.f32[0] * a1.f32[1])) * -2.0,
            (float)((float)((float)(a1.f32[2] * v1) + (float)(a1.f32[1] * a1.f32[1])) + (float)(a1.f32[3] * a1.f32[3]))
          + (float)(v6 * a1.f32[0]));
  }
  else
  {
    v7 = (float)((float)((float)(a1.f32[3] * a1.f32[3]) + (float)(a1.f32[2] * a1.f32[2])) + (float)(v6 * a1.f32[0]))
       + (float)(v3 * a1.f32[1]);
    v8 = (float)((float)(a1.f32[0] * a1.f32[3]) + (float)(a1.f32[1] * a1.f32[2]))
       + (float)((float)(a1.f32[0] * a1.f32[3]) + (float)(a1.f32[1] * a1.f32[2]));
    v9 = (float)(v2 + (float)(a1.f32[0] * a1.f32[1])) + (float)(v2 + (float)(a1.f32[0] * a1.f32[1]));
    v10 = (float)((float)((float)(a1.f32[1] * v3) + (float)(a1.f32[0] * a1.f32[0])) + (float)(v1 * a1.f32[2]))
        + (float)(a1.f32[3] * a1.f32[3]);
    asinf(-v5);
    v11 = atan2f(v8, v7);
    atan2f(v9, v10);
  }
  return (float)((float)(v11 * -180.0) / 3.1416);
}

/* ========================================================================
 * sub_1000316D8
 * EA: 0x1000316d8
 ======================================================================== */

double __fastcall sub_1000316D8(double a1, double a2, double a3)
{
  float v3; // s0
  float v4; // s8
  float v5; // s9
  float32x4_t v6; // q0
  float32x4_t v7; // q0
  float32x4_t v8; // q0
  int32x4_t v9; // q2
  int8x16_t v10; // q3
  float32x4_t v11; // q4
  float32x4_t v12; // q3
  float32x4_t v13; // q0
  int32x4_t v14; // q1
  int8x16_t v15; // q2
  float32x4_t v16; // q3
  float32x4_t v17; // q2
  double result; // d0
  float32x4_t v19; // [xsp+0h] [xbp-40h]
  float32x4_t v20; // [xsp+10h] [xbp-30h]
  __float2 v21; // 0:kr00_8.8
  __float2 v22; // 0:kr08_8.8
  __float2 v23; // 0:kr10_8.8

  v3 = a1 / 180.0 * 3.14159265;
  v4 = a2 / 180.0 * 3.14159265;
  v5 = a3 / 180.0 * 3.14159265;
  v21 = __sincosf_stret(v3 * -0.5);
  v6 = vmulq_n_f32((float32x4_t)xmmword_10025BFE0, v21.__sinval);
  v6.i32[3] = LODWORD(v21.__cosval);
  v20 = v6;
  v22 = __sincosf_stret(v4 * 0.5);
  v7 = vmulq_n_f32((float32x4_t)xmmword_10025BFF0, v22.__sinval);
  v7.i32[3] = LODWORD(v22.__cosval);
  v19 = v7;
  v23 = __sincosf_stret(v5 * 0.5);
  v8 = vmulq_n_f32((float32x4_t)xmmword_10025C000, v23.__sinval);
  v9 = (int32x4_t)vnegq_f32(v19);
  v10 = (int8x16_t)vtrn2q_s32((int32x4_t)v19, vtrn1q_s32((int32x4_t)v19, v9));
  v11 = vmlaq_n_f32(
          vmulq_lane_f32((float32x4_t)vextq_s8((int8x16_t)v19, (int8x16_t)v9, 8u), *(float32x2_t *)v8.f32, 1),
          (float32x4_t)vextq_s8(v10, v10, 8u),
          v8.f32[0]);
  v12 = (float32x4_t)vrev64q_s32((int32x4_t)v19);
  v12.i32[0] = v9.i32[1];
  v12.i32[3] = v9.i32[2];
  v13 = vaddq_f32(vmlaq_laneq_f32(vmulq_n_f32(v19, v23.__cosval), v12, v8, 2), v11);
  v14 = (int32x4_t)vnegq_f32(v20);
  v15 = (int8x16_t)vtrn2q_s32((int32x4_t)v20, vtrn1q_s32((int32x4_t)v20, v14));
  v16 = vmlaq_n_f32(
          vmulq_lane_f32((float32x4_t)vextq_s8((int8x16_t)v20, (int8x16_t)v14, 8u), *(float32x2_t *)v13.f32, 1),
          (float32x4_t)vextq_s8(v15, v15, 8u),
          v13.f32[0]);
  v17 = (float32x4_t)vrev64q_s32((int32x4_t)v20);
  v17.i32[0] = v14.i32[1];
  v17.i32[3] = v14.i32[2];
  *(_QWORD *)&result = vaddq_f32(v16, vmlaq_laneq_f32(vmulq_laneq_f32(v20, v13, 3), v17, v13, 2)).u64[0];
  return result;
}

/* ========================================================================
 * sub_1000317FC
 * EA: 0x1000317fc
 ======================================================================== */

__int64 sub_1000317FC()
{
  __int64 v0; // x20
  __int64 v1; // x19
  __int64 v2; // x21
  __int64 v3; // x22
  _QWORD *v4; // x20
  _QWORD *v5; // x0
  double v6; // d8
  void (__fastcall *v7)(_QWORD *, __int64); // x24
  __int64 v8; // x22
  char isUniquelyReferenced_nonNull_native; // w0
  unsigned __int64 v10; // x8
  unsigned __int64 v11; // x25
  double *v12; // x9
  double *v13; // x8
  double v14; // d0
  double v15; // d1
  double v17; // d1
  double v19; // d1
  double v21; // d0
  __int64 result; // x0
  __int64 v24; // x0
  __int64 v25; // x22
  _QWORD *v26; // x0
  double v27; // d8
  __int64 v28; // x0
  __int64 v29; // x8
  bool v30; // vf
  __int64 v31; // x8
  _QWORD v32[2]; // [xsp+0h] [xbp-50h] BYREF

  v1 = v0;
  v2 = type metadata accessor for Date(0);
  v3 = *(_QWORD *)(v2 - 8);
  v4 = (_QWORD *)((char *)v32 - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL));
  if ( *(__int64 *)(v1 + 24) > 2 )
    return 0;
  v5 = Date.init()((_QWORD *)((char *)v32 - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL)));
  v6 = Date.timeIntervalSince1970.getter(v5);
  v7 = *(void (__fastcall **)(_QWORD *, __int64))(v3 + 8);
  v7(v4, v2);
  v8 = *(_QWORD *)(v1 + 16);
  isUniquelyReferenced_nonNull_native = swift_isUniquelyReferenced_nonNull_native(v8);
  *(_QWORD *)(v1 + 16) = v8;
  if ( (isUniquelyReferenced_nonNull_native & 1) == 0 )
  {
    v8 = sub_10000937C(0, *(_QWORD *)(v8 + 16) + 1LL, 1, v8);
    *(_QWORD *)(v1 + 16) = v8;
  }
  v11 = *(_QWORD *)(v8 + 16);
  v10 = *(_QWORD *)(v8 + 24);
  if ( v11 >= v10 >> 1 )
    v8 = sub_10000937C(v10 > 1, v11 + 1, 1, v8);
  *(_QWORD *)(v8 + 16) = v11 + 1;
  v12 = (double *)(v8 + 32);
  v13 = (double *)(v8 + 32 + 8 * v11);
  *v13 = v6;
  *(_QWORD *)(v1 + 16) = v8;
  if ( v11 < 4 )
    return 0;
  v14 = *v12;
  v15 = *(v13 - 3) - *v12;
  if ( v15 < 60.0 || v15 > 600.0 )
    return 0;
  v17 = *(v13 - 2) - v14;
  if ( v17 < 60.0 || v17 > 600.0 )
    return 0;
  v19 = *(v13 - 1) - v14;
  if ( v19 < 60.0 || v19 > 600.0 )
    return 0;
  v21 = *v13 - v14;
  if ( v21 < 60.0 || v21 > 600.0 )
    return 0;
  v24 = sub_100004DF0(&unk_10033C960, &unk_10025C640);
  v25 = swift_allocObject(v24, 40, 7);
  *(_OWORD *)(v25 + 16) = xmmword_10025B110;
  v26 = Date.init()(v4);
  v27 = Date.timeIntervalSince1970.getter(v26);
  v7(v4, v2);
  *(double *)(v25 + 32) = v27;
  v28 = *(_QWORD *)(v1 + 16);
  *(_QWORD *)(v1 + 16) = v25;
  result = swift_bridgeObjectRelease(v28);
  v29 = *(_QWORD *)(v1 + 24);
  v30 = __OFADD__(v29, 1);
  v31 = v29 + 1;
  if ( v30 )
  {
    __break(1u);
  }
  else
  {
    *(_QWORD *)(v1 + 24) = v31;
    return 1;
  }
  return result;
}

/* ========================================================================
 * sub_100031AB0
 * EA: 0x100031ab0
 ======================================================================== */

double sub_100031AB0()
{
  double result; // d0
  bool v2; // zf
  double v3; // x8

  if ( qword_100339480 != -1 )
    swift_once(&qword_100339480, sub_1000FE3CC);
  if ( sub_1000FF000() == 4353
    || sub_1000FF000() == 4356
    || sub_1000FF000() == 4352
    || sub_1000FF000() == 4355
    || sub_1000FF000() == 514 )
  {
    return 45.0;
  }
  if ( sub_1000FF000() < 4352
    || sub_1000FF000() == 4608
    || sub_1000FF000() == 4609
    || sub_1000FF000() == 4624
    || sub_1000FF000() == 4625 )
  {
    if ( sub_1000FF000() == 4121 )
      return 38.0;
    v2 = sub_1000FF000() == 4125;
    result = 34.0;
    v3 = 38.0;
  }
  else
  {
    v2 = sub_1000FF000() == 4401;
    result = 42.0;
    v3 = 35.0;
  }
  if ( v2 )
    return v3;
  return result;
}

/* ========================================================================
 * sub_100031C00
 * EA: 0x100031c00
 ======================================================================== */

char *__fastcall sub_100031C00(__int64 a1, int a2)
{
  _BYTE *v2; // x20
  _BYTE *v3; // x21
  __int64 v4; // x23
  __int64 v5; // x27
  _QWORD *v6; // x20
  __int64 v7; // x24
  _QWORD *v8; // x25
  objc_class *v9; // x24
  id v10; // x0
  __int64 v11; // x19
  __int64 v12; // x0
  __int64 v13; // x24
  __int64 v14; // x0
  __int64 v15; // x25
  _QWORD *v16; // x0
  double v17; // d8
  void (__fastcall *v18)(_QWORD *, __int64); // x27
  CGFloat *v19; // x8
  CGFloat y; // d9
  CGFloat z; // d10
  CGFloat *v22; // x8
  CGFloat *v23; // x8
  char *v24; // x19
  __int64 v25; // x1
  id v26; // x0
  __int128 v27; // q1
  CGFloat *v28; // x8
  CGFloat *v29; // x8
  __int64 v30; // x19
  id v31; // x24
  __int64 v32; // d0
  __int64 v33; // d8
  __int64 v34; // x19
  id v35; // x24
  __int64 v36; // d0
  __int64 v37; // d8
  char *v38; // x8
  __int64 v39; // x19
  _QWORD *v40; // x0
  double v41; // d8
  __int64 v42; // x0
  objc_class *v43; // x0
  char *v44; // x20
  __int64 v45; // x19
  void *Strong; // x19
  __int64 v47; // x1
  char *v48; // x20
  id v49; // x21
  void *v50; // x19
  id v51; // x22
  char *v52; // x20
  NSString v53; // x23
  id v54; // x22
  char *v55; // x20
  NSString v56; // x23
  id v57; // x21
  char *v58; // x20
  NSString v59; // x23
  id v60; // x21
  char *v61; // x20
  NSString v62; // x23
  id v63; // x21
  char *v64; // x20
  NSString v65; // x23
  id v66; // x22
  char *v67; // x21
  NSString v68; // x20
  id v69; // x19
  char *v70; // x19
  __int64 v71; // x22
  void *v72; // x20
  id v73; // x19
  id v74; // x23
  __int64 v75; // x0
  __int64 v76; // x19
  __int64 v77; // x19
  __int64 v78; // x20
  __int64 v79; // x0
  __int64 v81; // [xsp+0h] [xbp-140h] BYREF
  int v82; // [xsp+Ch] [xbp-134h]
  __int64 v83; // [xsp+10h] [xbp-130h]
  float v84[6]; // [xsp+18h] [xbp-128h] BYREF
  char v85[24]; // [xsp+30h] [xbp-110h] BYREF
  objc_super v86; // [xsp+48h] [xbp-F8h] BYREF
  _OWORD v87[2]; // [xsp+58h] [xbp-E8h] BYREF
  __int64 v88; // [xsp+78h] [xbp-C8h]
  __int128 v89; // [xsp+80h] [xbp-C0h] BYREF
  __int128 v90; // [xsp+90h] [xbp-B0h]
  __int64 v91; // [xsp+A0h] [xbp-A0h]
  _OWORD v92[2]; // [xsp+B0h] [xbp-90h] BYREF

  v3 = v2;
  v82 = a2;
  v83 = a1;
  v4 = type metadata accessor for Date(0);
  v5 = *(_QWORD *)(v4 - 8);
  v6 = (__int64 *)((char *)&v81 - ((*(_QWORD *)(v5 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL));
  swift_unknownObjectWeakInit(&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view], 0);
  v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_interpolationEnabled] = 1;
  v7 = OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuSlerpUtil;
  *(_QWORD *)&v3[v7] = objc_msgSend(objc_allocWithZone((Class)type metadata accessor for VTIMUSlerpUtil(0)), "init");
  v8 = &v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuController];
  v9 = (objc_class *)type metadata accessor for VTIMUMagnetometerController(0);
  v10 = objc_msgSend(objc_allocWithZone(v9), "init");
  v8[3] = v9;
  v8[4] = &off_1002E7BC8;
  *v8 = v10;
  v11 = OBJC_IVAR____TtC11SpaceWalker18VTCameraController_p6ResetToastController;
  v12 = type metadata accessor for P6ResetToastController(0);
  v13 = swift_allocObject(v12, 32, 7);
  v14 = sub_100004DF0(&unk_10033C960, &unk_10025C640);
  v15 = swift_allocObject(v14, 40, 7);
  *(_OWORD *)(v15 + 16) = xmmword_10025B110;
  v16 = Date.init()(v6);
  v17 = Date.timeIntervalSince1970.getter(v16);
  v18 = *(void (__fastcall **)(_QWORD *, __int64))(v5 + 8);
  v18(v6, v4);
  *(double *)(v15 + 32) = v17;
  *(_QWORD *)&v3[v11] = v13;
  v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isAnimatingReorientation] = 0;
  v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isIMUInitialized] = 0;
  v19 = (CGFloat *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastIMU];
  y = SCNVector3Zero.y;
  z = SCNVector3Zero.z;
  *v19 = SCNVector3Zero.x;
  v19[1] = y;
  v19[2] = z;
  v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isIMUUpdated] = 1;
  *(_QWORD *)(v13 + 16) = v15;
  *(_QWORD *)(v13 + 24) = 0;
  v22 = (CGFloat *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_anchorIMU];
  *v22 = SCNVector3Zero.x;
  v22[1] = y;
  v22[2] = z;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_curFov] = 0x4056800000000000LL;
  v23 = (CGFloat *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_startingPosition];
  *v23 = SCNVector3Zero.x;
  v23[1] = y;
  v23[2] = z;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_nearZPosition] = 0xC08F400000000000LL;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_farZPosition] = 0x408F400000000000LL;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_phaseCount] = 0x4024000000000000LL;
  v24 = &v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase];
  LODWORD(v89) = 0;
  v26 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSUserDefaults, v25), "standardUserDefaults"));
  FoilDefaultStorage.init(wrappedValue:key:userDefaults:)(
    v87,
    &v89,
    0xD000000000000012LL,
    0x800000010026E140LL,
    v26,
    &type metadata for Float,
    &protocol witness table for Float);
  v27 = v87[1];
  *(_OWORD *)v24 = v87[0];
  *((_OWORD *)v24 + 1) = v27;
  *((_QWORD *)v24 + 4) = v88;
  v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingUp] = 1;
  v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_isMovingLeft] = 1;
  v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustStartNeeded] = 0;
  v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustEndNeeded] = 0;
  v28 = (CGFloat *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartIMU];
  *v28 = SCNVector3Zero.x;
  v28[1] = y;
  v28[2] = z;
  v29 = (CGFloat *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndIMU];
  *v29 = SCNVector3Zero.x;
  v29[1] = y;
  v29[2] = z;
  v30 = OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedStartTS;
  v31 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSDate), "init");
  objc_msgSend(v31, "timeIntervalSince1970");
  v33 = v32;
  objc_release(v31);
  *(_QWORD *)&v3[v30] = v33;
  v34 = OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedEndTS;
  v35 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSDate), "init");
  objc_msgSend(v35, "timeIntervalSince1970");
  v37 = v36;
  objc_release(v35);
  *(_QWORD *)&v3[v34] = v37;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_adjustedCounter] = 0;
  v38 = &v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastPose];
  *(_OWORD *)v38 = 0u;
  *((_OWORD *)v38 + 1) = 0u;
  v38[32] = 1;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_imuTSPairs] = &_swiftEmptyArrayStorage;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_yawDriftSpeed] = 0;
  v39 = OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastUpdatedTS;
  v40 = Date.init()(v6);
  v41 = Date.timeIntervalSince1970.getter(v40);
  v42 = ((__int64 (__fastcall *)(_QWORD *, __int64))v18)(v6, v4);
  *(double *)&v3[v39] = v41;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_accumulateYawDrift] = 0;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_calibrateCounter] = 0;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_updateTSCounter] = 0;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_lastYawDriftSpeed] = 0;
  v3[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_distanceAdjustingEnabled] = v82;
  v43 = (objc_class *)type metadata accessor for VTCameraController(v42);
  v86.receiver = v3;
  v86.super_class = v43;
  v44 = (char *)objc_msgSendSuper2(&v86, "init");
  v45 = OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view;
  swift_unknownObjectWeakAssign(&v44[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_view], v83);
  Strong = (void *)swift_unknownObjectWeakLoadStrong(&v44[v45]);
  v48 = objc_retain(v44);
  if ( Strong )
  {
    v49 = objc_retainAutoreleasedReturnValue(objc_msgSend(Strong, "pointOfView"));
    objc_release(Strong);
    if ( v49 )
    {
      objc_msgSend(
        v49,
        "setEulerAngles:",
        *(double *)&v48[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_startingPosition],
        *(double *)&v48[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_startingPosition + 8],
        *(double *)&v48[OBJC_IVAR____TtC11SpaceWalker18VTCameraController_startingPosition + 16]);
      objc_release(v49);
    }
  }
  v50 = (void *)objc_opt_self(&OBJC_CLASS___NSNotificationCenter, v47);
  v51 = objc_retainAutoreleasedReturnValue(objc_msgSend(v50, "defaultCenter"));
  v52 = objc_retain(v48);
  v53 = String._bridgeToObjectiveC()();
  objc_msgSend(v51, "addObserver:selector:name:object:", v52, "imuDataUpdated:", v53, 0);
  objc_release(v51);
  objc_release(v52);
  objc_release(v53);
  v54 = objc_retainAutoreleasedReturnValue(objc_msgSend(v50, "defaultCenter"));
  v55 = objc_retain(v52);
  v56 = String._bridgeToObjectiveC()();
  objc_msgSend(v54, "addObserver:selector:name:object:", v55, "imuDataUpdated:", v56, 0);
  objc_release(v54);
  objc_release(v55);
  objc_release(v56);
  v57 = objc_retainAutoreleasedReturnValue(objc_msgSend(v50, "defaultCenter"));
  v58 = objc_retain(v55);
  v59 = String._bridgeToObjectiveC()();
  objc_msgSend(v57, "addObserver:selector:name:object:", v58, "onReset:", v59, 0);
  objc_release(v57);
  objc_release(v58);
  objc_release(v59);
  v60 = objc_retainAutoreleasedReturnValue(objc_msgSend(v50, "defaultCenter"));
  v61 = objc_retain(v58);
  v62 = String._bridgeToObjectiveC()();
  objc_msgSend(v60, "addObserver:selector:name:object:", v61, "cleanup", v62, 0);
  objc_release(v60);
  objc_release(v61);
  objc_release(v62);
  v63 = objc_retainAutoreleasedReturnValue(objc_msgSend(v50, "defaultCenter"));
  v64 = objc_retain(v61);
  v65 = String._bridgeToObjectiveC()();
  objc_msgSend(v63, "addObserver:selector:name:object:", v64, "goNear:", v65, 0);
  objc_release(v63);
  objc_release(v64);
  objc_release(v65);
  v66 = objc_retainAutoreleasedReturnValue(objc_msgSend(v50, "defaultCenter"));
  v67 = objc_retain(v64);
  v68 = String._bridgeToObjectiveC()();
  objc_msgSend(v66, "addObserver:selector:name:object:", v67, "goFar:", v68, 0);
  objc_release(v66);
  objc_release(v67);
  objc_release(v68);
  v69 = objc_retainAutoreleasedReturnValue(objc_msgSend(v50, "defaultCenter"));
  objc_msgSend(
    v69,
    "addObserver:selector:name:object:",
    v67,
    "resetAdjustedData",
    NSApplicationWillTerminateNotification,
    0);
  objc_release(v69);
  objc_release(v67);
  if ( qword_100339480 != -1 )
    swift_once(&qword_100339480, sub_1000FE3CC);
  v70 = &v67[OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase];
  swift_beginAccess(&v67[OBJC_IVAR____TtC11SpaceWalker18VTCameraController__curPhase], v85, 0, 0);
  v89 = *(_OWORD *)v70;
  v90 = *((_OWORD *)v70 + 1);
  v91 = *((_QWORD *)v70 + 4);
  v71 = *((_QWORD *)&v89 + 1);
  v72 = (void *)v90;
  v92[0] = *(_OWORD *)(v70 + 24);
  v73 = objc_retain((id)v89);
  swift_retain(v71);
  v74 = objc_retain(v72);
  sub_10001D39C(v92, v84);
  v75 = sub_100004DF0(&unk_10033C720, &unk_100261150);
  FoilDefaultStorage.wrappedValue.getter(v84, v75);
  objc_release(v74);
  swift_release(v71);
  objc_release(v73);
  sub_10001D3D8(v92);
  sub_100030EBC(v84[0]);
  if ( sub_1000FF000() == 4353
    || sub_1000FF000() == 4356
    || sub_1000FF000() == 4352
    || sub_1000FF000() == 4355
    || sub_1000FF000() == 514 )
  {
    if ( qword_100339488 != -1 )
      swift_once(&qword_100339488, sub_10011530C);
    v76 = qword_100344798;
    swift_beginAccess(qword_100344798 + 16, v84, 1, 0);
    *(_QWORD *)(v76 + 24) = &off_1002DDB48;
    swift_unknownObjectWeakAssign(v76 + 16, v67);
  }
  v77 = type metadata accessor for VTLogger(0);
  v78 = static os_log_type_t.info.getter(v77);
  v79 = static os_log_type_t.info.getter(v78);
  sub_100091CD8(v78, v79, 0xD000000000000017LL, 0x800000010026E180LL, v77);
  return v67;
}

/* ========================================================================
 * sub_10003260C
 * EA: 0x10003260c
 ======================================================================== */

__int64 sub_10003260C()
{
  void *v0; // x20
  void *v1; // x25
  __int64 v2; // x19
  __int64 v3; // x27
  char *v4; // x21
  __int64 v5; // x22
  __int64 v6; // x28
  char *v7; // x23
  void *v8; // x24
  __int64 v9; // x0
  void *v10; // x26
  __int64 v11; // x20
  id v12; // x0
  __int64 v13; // x0
  __int64 v14; // x0
  __int64 v15; // x25
  __int64 v16; // x20
  __int64 v17; // x0
  _QWORD aBlock[5]; // [xsp+0h] [xbp-80h] BYREF
  __int64 v20; // [xsp+28h] [xbp-58h]

  v1 = v0;
  v2 = type metadata accessor for DispatchWorkItemFlags(0);
  v3 = *(_QWORD *)(v2 - 8);
  v4 = (char *)aBlock - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for DispatchQoS(0);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = (char *)aBlock - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  sub_100004E40(0);
  v8 = (void *)static OS_dispatch_queue.main.getter();
  v9 = swift_allocObject(&unk_1002DDB90, 24, 7);
  *(_QWORD *)(v9 + 16) = v0;
  aBlock[4] = sub_100032808;
  v20 = v9;
  aBlock[0] = _NSConcreteStackBlock;
  aBlock[1] = 1107296256;
  aBlock[2] = sub_10007A08C;
  aBlock[3] = &unk_1002DDBA8;
  v10 = _Block_copy(aBlock);
  v11 = v20;
  v12 = objc_retain(v1);
  v13 = swift_release(v11);
  v14 = static DispatchQoS.unspecified.getter(v13);
  aBlock[0] = &_swiftEmptyArrayStorage;
  v15 = sub_10000C824(v14);
  v16 = sub_100004DF0(&unk_10033BE50, &unk_10025B090);
  v17 = sub_1000147C4();
  dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v16, v17, v2, v15);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v7, v4, v10);
  _Block_release(v10);
  objc_release(v8);
  (*(void (__fastcall **)(char *, __int64))(v3 + 8))(v4, v2);
  return (*(__int64 (__fastcall **)(char *, __int64))(v6 + 8))(v7, v5);
}

/* ========================================================================
 * sub_100032C88
 * EA: 0x100032c88
 ======================================================================== */

void *sub_100032C88()
{
  __int64 v0; // x20
  __int64 v1; // x24
  unsigned int v2; // w8
  void *result; // x0
  void *v4; // x19
  id v5; // x0
  __int64 v6; // x1
  void *v7; // x21
  id v8; // x22
  id v9; // x23
  __int64 v10; // x1
  void *v11; // x19
  id v12; // x21
  void *v13; // x19
  id v14; // x0
  __int64 v15; // x1
  void *v16; // x21
  id v17; // x22
  id v18; // x23
  __int64 v19; // x1
  void *v20; // x19
  id v21; // x21
  void *v22; // x19
  id v23; // x0
  __int64 v24; // x1
  void *v25; // x21
  id v26; // x22
  id v27; // x23
  __int64 v28; // x1
  void *v29; // x19
  id v30; // x21
  void *v31; // x19
  id v32; // x0
  __int64 v33; // x1
  void *v34; // x21
  id v35; // x22
  id v36; // x23
  __int64 v37; // x1
  void *v38; // x19
  id v39; // x21
  void *v40; // x19
  __int64 v41; // x0
  _BYTE v42[24]; // [xsp+0h] [xbp-60h] BYREF
  __int64 v43; // [xsp+18h] [xbp-48h] BYREF

  sub_1000332E0();
  v1 = OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_selectedLayoutType;
  v2 = *(unsigned __int8 *)(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_selectedLayoutType);
  if ( v2 <= 5 )
  {
    if ( v2 == 4 )
      goto LABEL_18;
    if ( v2 != 5 )
      goto LABEL_30;
    result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_singleDisplayButton);
    if ( !result )
    {
      __break(1u);
      goto LABEL_34;
    }
    v4 = result;
    v5 = objc_retainAutoreleasedReturnValue(objc_msgSend(result, "layer"));
    if ( v5 )
    {
      v7 = v5;
      v8 = objc_retainAutoreleasedReturnValue(
             objc_msgSend(
               (id)objc_opt_self(&OBJC_CLASS___NSColor, v6),
               "colorWithRed:green:blue:alpha:",
               0.392156863,
               0.584313725,
               1.0,
               1.0));
      v9 = objc_retainAutoreleasedReturnValue(objc_msgSend(v8, "CGColor"));
      objc_release(v8);
      objc_msgSend(v7, "setBorderColor:", v9);
      objc_release(v7);
      objc_release(v9);
    }
    objc_release(v4);
    result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_singleDisplayLabel);
    if ( !result )
      goto LABEL_37;
    v11 = result;
    v12 = objc_retainAutoreleasedReturnValue(
            objc_msgSend(
              (id)objc_opt_self(&OBJC_CLASS___NSColor, v10),
              "colorWithRed:green:blue:alpha:",
              1.0,
              0.37254902,
              0.203921569,
              1.0));
    objc_msgSend(v11, "setTextColor:", v12);
    objc_release(v11);
    objc_release(v12);
    result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_singleDisplayBgView);
    if ( result )
      goto LABEL_29;
    __break(1u);
  }
  if ( v2 == 6 )
    goto LABEL_24;
  if ( v2 == 9 )
  {
    result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonThreeButton);
    if ( result )
    {
      v13 = result;
      v14 = objc_retainAutoreleasedReturnValue(objc_msgSend(result, "layer"));
      if ( v14 )
      {
        v16 = v14;
        v17 = objc_retainAutoreleasedReturnValue(
                objc_msgSend(
                  (id)objc_opt_self(&OBJC_CLASS___NSColor, v15),
                  "colorWithRed:green:blue:alpha:",
                  0.392156863,
                  0.584313725,
                  1.0,
                  1.0));
        v18 = objc_retainAutoreleasedReturnValue(objc_msgSend(v17, "CGColor"));
        objc_release(v17);
        objc_msgSend(v16, "setBorderColor:", v18);
        objc_release(v16);
        objc_release(v18);
      }
      objc_release(v13);
      result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonThreeLabel);
      if ( !result )
        goto LABEL_38;
      v20 = result;
      v21 = objc_retainAutoreleasedReturnValue(
              objc_msgSend(
                (id)objc_opt_self(&OBJC_CLASS___NSColor, v19),
                "colorWithRed:green:blue:alpha:",
                1.0,
                0.37254902,
                0.203921569,
                1.0));
      objc_msgSend(v20, "setTextColor:", v21);
      objc_release(v20);
      objc_release(v21);
      result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonThreeBgView);
      if ( result )
        goto LABEL_29;
      __break(1u);
LABEL_18:
      result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_ultrawideButton);
      if ( result )
      {
        v22 = result;
        v23 = objc_retainAutoreleasedReturnValue(objc_msgSend(result, "layer"));
        if ( v23 )
        {
          v25 = v23;
          v26 = objc_retainAutoreleasedReturnValue(
                  objc_msgSend(
                    (id)objc_opt_self(&OBJC_CLASS___NSColor, v24),
                    "colorWithRed:green:blue:alpha:",
                    0.392156863,
                    0.584313725,
                    1.0,
                    1.0));
          v27 = objc_retainAutoreleasedReturnValue(objc_msgSend(v26, "CGColor"));
          objc_release(v26);
          objc_msgSend(v25, "setBorderColor:", v27);
          objc_release(v25);
          objc_release(v27);
        }
        objc_release(v22);
        result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_ultraWideLabel);
        if ( !result )
          goto LABEL_39;
        v29 = result;
        v30 = objc_retainAutoreleasedReturnValue(
                objc_msgSend(
                  (id)objc_opt_self(&OBJC_CLASS___NSColor, v28),
                  "colorWithRed:green:blue:alpha:",
                  1.0,
                  0.37254902,
                  0.203921569,
                  1.0));
        objc_msgSend(v29, "setTextColor:", v30);
        objc_release(v29);
        objc_release(v30);
        result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_ultrawideBgView);
        if ( !result )
        {
          __break(1u);
LABEL_24:
          result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonTwoButton);
          if ( result )
          {
            v31 = result;
            v32 = objc_retainAutoreleasedReturnValue(objc_msgSend(result, "layer"));
            if ( v32 )
            {
              v34 = v32;
              v35 = objc_retainAutoreleasedReturnValue(
                      objc_msgSend(
                        (id)objc_opt_self(&OBJC_CLASS___NSColor, v33),
                        "colorWithRed:green:blue:alpha:",
                        0.392156863,
                        0.584313725,
                        1.0,
                        1.0));
              v36 = objc_retainAutoreleasedReturnValue(objc_msgSend(v35, "CGColor"));
              objc_release(v35);
              objc_msgSend(v34, "setBorderColor:", v36);
              objc_release(v34);
              objc_release(v36);
            }
            objc_release(v31);
            result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonTwoLabel);
            if ( result )
            {
              v38 = result;
              v39 = objc_retainAutoreleasedReturnValue(
                      objc_msgSend(
                        (id)objc_opt_self(&OBJC_CLASS___NSColor, v37),
                        "colorWithRed:green:blue:alpha:",
                        1.0,
                        0.37254902,
                        0.203921569,
                        1.0));
              objc_msgSend(v38, "setTextColor:", v39);
              objc_release(v38);
              objc_release(v39);
              result = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonTwoBgView);
              if ( result )
                goto LABEL_29;
LABEL_41:
              __break(1u);
              return result;
            }
LABEL_40:
            __break(1u);
            goto LABEL_41;
          }
          goto LABEL_36;
        }
LABEL_29:
        v40 = result;
        objc_msgSend(result, "setHidden:", 0);
        objc_release(v40);
        goto LABEL_30;
      }
LABEL_35:
      __break(1u);
LABEL_36:
      __break(1u);
LABEL_37:
      __break(1u);
LABEL_38:
      __break(1u);
LABEL_39:
      __break(1u);
      goto LABEL_40;
    }
LABEL_34:
    __break(1u);
    goto LABEL_35;
  }
LABEL_30:
  if ( qword_1003393A8 != -1 )
    swift_once(&qword_1003393A8, sub_100084ECC);
  v43 = *(unsigned __int8 *)(v0 + v1) - 1LL;
  swift_beginAccess(qword_1003444C0 + OBJC_IVAR____TtC11SpaceWalker13VTPreferences__vt_R6LayoutType, v42, 33, 0);
  v41 = sub_100004DF0(&unk_10033C6E8, &unk_10025BEE0);
  FoilDefaultStorage.wrappedValue.setter(&v43, v41);
  return (void *)swift_endAccess(v42);
}

/* ========================================================================
 * sub_1000332E0
 * EA: 0x1000332e0
 ======================================================================== */

void sub_1000332E0()
{
  __int64 v0; // x20
  __int64 v1; // x19
  void *Strong; // x0
  void *v3; // x21
  id v4; // x0
  void *v5; // x22
  void *v6; // x20
  __int64 v7; // x0
  __int64 v8; // x1
  void *v9; // x20
  void *v10; // x21
  id v11; // x22
  id v12; // x23
  void *v13; // x0
  void *v14; // x22
  id v15; // x0
  void *v16; // x23
  void *v17; // x20
  __int64 v18; // x0
  void *v19; // x20
  id v20; // x22
  id v21; // x23
  void *v22; // x0
  void *v23; // x22
  id v24; // x0
  void *v25; // x23
  void *v26; // x20
  __int64 v27; // x0
  void *v28; // x20
  id v29; // x22
  id v30; // x23
  void *v31; // x0
  void *v32; // x22
  id v33; // x0
  void *v34; // x23
  void *v35; // x20
  __int64 v36; // x0
  void *v37; // x20
  id v38; // x21
  id v39; // x22
  void *v40; // x0
  void *v41; // x20
  void *v42; // x0
  void *v43; // x20
  void *v44; // x0
  void *v45; // x20
  void *v46; // x0
  void *v47; // x19

  v1 = v0;
  Strong = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_ultrawideButton);
  if ( !Strong )
  {
    __break(1u);
LABEL_23:
    __break(1u);
    goto LABEL_24;
  }
  v3 = Strong;
  v4 = objc_retainAutoreleasedReturnValue(objc_msgSend(Strong, "layer"));
  if ( v4 )
  {
    v5 = v4;
    type metadata accessor for CGColor(0);
    v6 = (void *)static CGColorRef.clear.getter();
    objc_msgSend(v5, "setBorderColor:", v6);
    objc_release(v5);
    objc_release(v6);
  }
  objc_release(v3);
  v7 = swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_ultraWideLabel);
  if ( !v7 )
    goto LABEL_23;
  v9 = (void *)v7;
  v10 = (void *)objc_opt_self(&OBJC_CLASS___NSColor, v8);
  v11 = objc_retainAutoreleasedReturnValue(objc_msgSend(v10, "whiteColor"));
  v12 = objc_retainAutoreleasedReturnValue(objc_msgSend(v11, "colorWithAlphaComponent:", 0.6));
  objc_release(v11);
  objc_msgSend(v9, "setTextColor:", v12);
  objc_release(v9);
  objc_release(v12);
  v13 = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_singleDisplayButton);
  if ( !v13 )
  {
LABEL_24:
    __break(1u);
LABEL_25:
    __break(1u);
    goto LABEL_26;
  }
  v14 = v13;
  v15 = objc_retainAutoreleasedReturnValue(objc_msgSend(v13, "layer"));
  if ( v15 )
  {
    v16 = v15;
    type metadata accessor for CGColor(0);
    v17 = (void *)static CGColorRef.clear.getter();
    objc_msgSend(v16, "setBorderColor:", v17);
    objc_release(v16);
    objc_release(v17);
  }
  objc_release(v14);
  v18 = swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_singleDisplayLabel);
  if ( !v18 )
    goto LABEL_25;
  v19 = (void *)v18;
  v20 = objc_retainAutoreleasedReturnValue(objc_msgSend(v10, "whiteColor"));
  v21 = objc_retainAutoreleasedReturnValue(objc_msgSend(v20, "colorWithAlphaComponent:", 0.6));
  objc_release(v20);
  objc_msgSend(v19, "setTextColor:", v21);
  objc_release(v19);
  objc_release(v21);
  v22 = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonTwoButton);
  if ( !v22 )
  {
LABEL_26:
    __break(1u);
LABEL_27:
    __break(1u);
    goto LABEL_28;
  }
  v23 = v22;
  v24 = objc_retainAutoreleasedReturnValue(objc_msgSend(v22, "layer"));
  if ( v24 )
  {
    v25 = v24;
    type metadata accessor for CGColor(0);
    v26 = (void *)static CGColorRef.clear.getter();
    objc_msgSend(v25, "setBorderColor:", v26);
    objc_release(v25);
    objc_release(v26);
  }
  objc_release(v23);
  v27 = swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonTwoLabel);
  if ( !v27 )
    goto LABEL_27;
  v28 = (void *)v27;
  v29 = objc_retainAutoreleasedReturnValue(objc_msgSend(v10, "whiteColor"));
  v30 = objc_retainAutoreleasedReturnValue(objc_msgSend(v29, "colorWithAlphaComponent:", 0.6));
  objc_release(v29);
  objc_msgSend(v28, "setTextColor:", v30);
  objc_release(v28);
  objc_release(v30);
  v31 = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonThreeButton);
  if ( !v31 )
  {
LABEL_28:
    __break(1u);
LABEL_29:
    __break(1u);
    goto LABEL_30;
  }
  v32 = v31;
  v33 = objc_retainAutoreleasedReturnValue(objc_msgSend(v31, "layer"));
  if ( v33 )
  {
    v34 = v33;
    type metadata accessor for CGColor(0);
    v35 = (void *)static CGColorRef.clear.getter();
    objc_msgSend(v34, "setBorderColor:", v35);
    objc_release(v34);
    objc_release(v35);
  }
  objc_release(v32);
  v36 = swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonThreeLabel);
  if ( !v36 )
    goto LABEL_29;
  v37 = (void *)v36;
  v38 = objc_retainAutoreleasedReturnValue(objc_msgSend(v10, "whiteColor"));
  v39 = objc_retainAutoreleasedReturnValue(objc_msgSend(v38, "colorWithAlphaComponent:", 0.6));
  objc_release(v38);
  objc_msgSend(v37, "setTextColor:", v39);
  objc_release(v37);
  objc_release(v39);
  v40 = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_ultrawideBgView);
  if ( !v40 )
  {
LABEL_30:
    __break(1u);
    goto LABEL_31;
  }
  v41 = v40;
  objc_msgSend(v40, "setHidden:", 1);
  objc_release(v41);
  v42 = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_singleDisplayBgView);
  if ( !v42 )
  {
LABEL_31:
    __break(1u);
    goto LABEL_32;
  }
  v43 = v42;
  objc_msgSend(v42, "setHidden:", 1);
  objc_release(v43);
  v44 = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonTwoBgView);
  if ( !v44 )
  {
LABEL_32:
    __break(1u);
    goto LABEL_33;
  }
  v45 = v44;
  objc_msgSend(v44, "setHidden:", 1);
  objc_release(v45);
  v46 = (void *)swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_horizonThreeBgView);
  if ( v46 )
  {
    v47 = v46;
    objc_msgSend(v46, "setHidden:", 1);
    objc_release(v47);
    return;
  }
LABEL_33:
  __break(1u);
}

/* ========================================================================
 * sub_1000337AC
 * EA: 0x1000337ac
 ======================================================================== */

void sub_1000337AC()
{
  __int64 v0; // x20
  unsigned int v1; // w8
  void *Strong; // x0
  id v4; // x19

  if ( qword_100339358 != -1 )
    swift_once(&qword_100339358, sub_100073E7C);
  v1 = *(unsigned __int8 *)(qword_100344470 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_displayMode);
  if ( v1 > 6 )
  {
    if ( *(unsigned __int8 *)(qword_100344470 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_displayMode) > 9u || v1 == 7 )
      goto LABEL_16;
    if ( v1 != 8 )
      goto LABEL_19;
LABEL_13:
    Strong = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_rr90HzButton);
    if ( Strong )
    {
LABEL_22:
      v4 = objc_retain(Strong);
      sub_100035D24();
      objc_release(v4);
      objc_release(v4);
      return;
    }
    __break(1u);
    goto LABEL_15;
  }
  if ( *(unsigned __int8 *)(qword_100344470 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_displayMode) <= 2u )
  {
    if ( !*(_BYTE *)(qword_100344470 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_displayMode) )
      goto LABEL_21;
    if ( v1 == 1 )
      goto LABEL_16;
    goto LABEL_13;
  }
LABEL_15:
  if ( v1 - 4 >= 2 )
    goto LABEL_18;
LABEL_16:
  Strong = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_rr120HzButton);
  if ( Strong )
    goto LABEL_22;
  __break(1u);
LABEL_18:
  if ( v1 != 3 )
    goto LABEL_21;
LABEL_19:
  Strong = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_rr120HzButton);
  if ( Strong )
    goto LABEL_22;
  __break(1u);
LABEL_21:
  Strong = (void *)swift_unknownObjectWeakLoadStrong(v0 + OBJC_IVAR____TtC11SpaceWalker33LaunchModeSelectionViewController_rr60HzButton);
  if ( Strong )
    goto LABEL_22;
  __break(1u);
}

/* ========================================================================
 * sub_100048018
 * EA: 0x100048018
 ======================================================================== */

__int64 __fastcall sub_100048018(__int64 countAndFlagsBits, void *object, __int64 a3)
{
  __int64 v5; // x0
  char *v6; // x25
  char *v7; // x24
  _QWORD *v8; // x0
  __int64 v9; // x23
  unsigned __int64 v10; // x27
  _QWORD *v11; // x20
  __int64 v12; // x0
  unsigned __int64 v13; // x28
  __int64 v14; // x21
  __int64 v15; // x1
  id v16; // x21
  NSString v17; // x26
  NSString v18; // x27
  id v19; // x20
  __int64 v20; // x26
  __int64 v21; // x27
  id v22; // x21
  NSURL *v23; // x8
  void *v24; // x0
  void *v25; // x25
  id v26; // x20
  Swift::String v27; // x0
  Swift::String v28; // x5
  Swift::String v29; // kr00_16
  __int64 v30; // x0
  __int64 v31; // x21
  __int64 v32; // x0
  __int64 v33; // x1
  __int64 v34; // x22
  __int64 v35; // x19
  _QWORD *v36; // x21
  __int64 v37; // x0
  unsigned __int64 v38; // x20
  __int64 v39; // x24
  __int64 isUniquelyReferenced_nonNull_native; // x0
  unsigned __int64 v41; // x8
  unsigned __int64 v42; // x25
  __int64 v43; // x20
  __int64 v45; // [xsp+20h] [xbp-B0h] BYREF
  __int64 v46; // [xsp+28h] [xbp-A8h]
  __int64 v47; // [xsp+30h] [xbp-A0h]
  __int64 v48; // [xsp+38h] [xbp-98h] BYREF
  unsigned __int64 v49; // [xsp+40h] [xbp-90h]
  void *v50; // [xsp+50h] [xbp-80h]
  __int64 v51; // [xsp+58h] [xbp-78h]
  _QWORD v52[2]; // [xsp+60h] [xbp-70h] BYREF
  __int64 v53; // [xsp+70h] [xbp-60h] BYREF
  unsigned __int64 v54; // [xsp+78h] [xbp-58h]
  Swift::String v55; // 0:x7.8,8:^10.8

  v47 = a3;
  v5 = type metadata accessor for URL(0);
  v45 = *(_QWORD *)(v5 - 8);
  v46 = v5;
  v6 = (char *)&v45 - ((*(_QWORD *)(v45 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v7 = v6;
  v8 = (_QWORD *)static Locale.preferredLanguages.getter();
  if ( v8[2] )
  {
    v9 = v8[4];
    v10 = v8[5];
    v11 = v8;
    swift_bridgeObjectRetain(v10);
    v12 = swift_bridgeObjectRelease(v11);
    v48 = v9;
    v49 = v10;
    v13 = 0xE700000000000000LL;
    v53 = 0x746E61482D687ALL;
    v54 = 0xE700000000000000LL;
    v14 = sub_10000DF9C(v12);
    if ( (StringProtocol.contains<A>(_:)(&v53, &type metadata for String, &type metadata for String, v14, v14) & 1) != 0
      || (v48 = v9,
          v49 = v10,
          v53 = 0x736E61482D687ALL,
          v54 = 0xE700000000000000LL,
          (StringProtocol.contains<A>(_:)(&v53, &type metadata for String, &type metadata for String, v14, v14) & 1) != 0) )
    {
      swift_bridgeObjectRelease(v10);
    }
    else
    {
      v48 = v9;
      v49 = v10;
      v13 = 0xE200000000000000LL;
      v53 = 24938;
      v54 = 0xE200000000000000LL;
      StringProtocol.contains<A>(_:)(&v53, &type metadata for String, &type metadata for String, v14, v14);
      swift_bridgeObjectRelease(v10);
    }
  }
  else
  {
    swift_bridgeObjectRelease(v8);
    v13 = 0xE200000000000000LL;
  }
  v16 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSBundle, v15), "mainBundle"));
  v17 = String._bridgeToObjectiveC()();
  swift_bridgeObjectRelease(v13);
  v18 = String._bridgeToObjectiveC()();
  v19 = objc_retainAutoreleasedReturnValue(objc_msgSend(v16, "URLForResource:withExtension:", v17, v18));
  objc_release(v16);
  objc_release(v17);
  objc_release(v18);
  if ( !v19 )
  {
LABEL_12:
    v30 = swift_bridgeObjectRetain(object);
    v31 = v47;
    if ( !v47 )
      return countAndFlagsBits;
    goto LABEL_13;
  }
  static URL._unconditionallyBridgeFromObjectiveC(_:)(v19);
  objc_release(v19);
  v21 = v45;
  v20 = v46;
  (*(void (__fastcall **)(char *, char *, __int64))(v45 + 32))(v6, v6, v46);
  v22 = objc_allocWithZone((Class)&OBJC_CLASS___NSBundle);
  URL._bridgeToObjectiveC()(v23);
  v25 = v24;
  v26 = objc_msgSend(v22, "initWithURL:", v24);
  objc_release(v25);
  if ( !v26 )
  {
    (*(void (__fastcall **)(char *, __int64))(v21 + 8))(v7, v20);
    goto LABEL_12;
  }
  v55._object = (void *)0xE000000000000000LL;
  v27._countAndFlagsBits = countAndFlagsBits;
  v27._object = object;
  v28._countAndFlagsBits = 0;
  v28._object = (void *)0xE000000000000000LL;
  v55._countAndFlagsBits = 0;
  v29 = NSLocalizedString(_:tableName:bundle:value:comment:)(v27, (Swift::String_optional)0, (NSBundle)v26, v28, v55);
  countAndFlagsBits = v29._countAndFlagsBits;
  object = v29._object;
  objc_release(v26);
  v30 = (*(__int64 (__fastcall **)(char *, __int64))(v21 + 8))(v7, v20);
  v31 = v47;
  if ( !v47 )
    return countAndFlagsBits;
LABEL_13:
  v48 = countAndFlagsBits;
  v49 = (unsigned __int64)object;
  v53 = 29477;
  v54 = 0xE200000000000000LL;
  v52[0] = 16421;
  v52[1] = 0xE200000000000000LL;
  v32 = sub_10000DF9C(v30);
  v46 = StringProtocol.replacingOccurrences<A, B>(of:with:options:range:)(
          &v53,
          v52,
          0,
          0,
          0,
          1,
          &type metadata for String,
          &type metadata for String,
          &type metadata for String,
          v32,
          v32,
          v32);
  v47 = v33;
  swift_bridgeObjectRelease(object);
  v34 = *(_QWORD *)(v31 + 16);
  if ( v34 )
  {
    v35 = v31 + 32;
    v36 = &_swiftEmptyArrayStorage;
    do
    {
      v48 = 0;
      v49 = 0xE000000000000000LL;
      v37 = sub_100004DF0(&unk_10033BEA0, &unk_10025C090);
      _print_unlocked<A, B>(_:_:)(
        v35,
        &v48,
        v37,
        &type metadata for DefaultStringInterpolation,
        &protocol witness table for DefaultStringInterpolation);
      v39 = v48;
      v38 = v49;
      isUniquelyReferenced_nonNull_native = swift_isUniquelyReferenced_nonNull_native(v36);
      if ( (isUniquelyReferenced_nonNull_native & 1) == 0 )
      {
        isUniquelyReferenced_nonNull_native = sub_1000097C8(0, v36[2] + 1LL, 1, v36);
        v36 = (_QWORD *)isUniquelyReferenced_nonNull_native;
      }
      v42 = v36[2];
      v41 = v36[3];
      if ( v42 >= v41 >> 1 )
      {
        isUniquelyReferenced_nonNull_native = sub_1000097C8(v41 > 1, v42 + 1, 1, v36);
        v36 = (_QWORD *)isUniquelyReferenced_nonNull_native;
      }
      v50 = &type metadata for String;
      v51 = sub_10000D4DC(isUniquelyReferenced_nonNull_native);
      v48 = v39;
      v49 = v38;
      v36[2] = v42 + 1;
      sub_10000C9BC(&v48, &v36[5 * v42 + 4]);
      v35 += 40;
      --v34;
    }
    while ( v34 );
  }
  else
  {
    v36 = &_swiftEmptyArrayStorage;
  }
  v43 = v47;
  countAndFlagsBits = String.init(format:arguments:)(v46, v47, v36);
  swift_bridgeObjectRelease(v36);
  swift_bridgeObjectRelease(v43);
  return countAndFlagsBits;
}

/* ========================================================================
 * sub_100048580
 * EA: 0x100048580
 ======================================================================== */

__int64 __fastcall sub_100048580(_BYTE *a1, _BYTE *a2)
{
  int v2; // w9
  unsigned __int64 v3; // x12
  __int64 v4; // x13
  __int64 v5; // x0
  unsigned __int64 v6; // x19
  unsigned __int64 v7; // x11
  __int64 v8; // x12
  __int64 v9; // x2
  unsigned __int64 v10; // x20
  char v11; // w21

  v2 = (unsigned __int8)*a1;
  v3 = 0xE700000000000000LL;
  v4 = 0x736E61482D687ALL;
  if ( v2 != 1 )
  {
    v4 = 24938;
    v3 = 0xE200000000000000LL;
  }
  if ( *a1 )
    v5 = v4;
  else
    v5 = 28261;
  if ( v2 )
    v6 = v3;
  else
    v6 = 0xE200000000000000LL;
  v7 = 0xE700000000000000LL;
  v8 = 0x736E61482D687ALL;
  if ( *a2 != 1 )
  {
    v8 = 24938;
    v7 = 0xE200000000000000LL;
  }
  if ( *a2 )
    v9 = v8;
  else
    v9 = 28261;
  if ( *a2 )
    v10 = v7;
  else
    v10 = 0xE200000000000000LL;
  if ( v5 == v9 && v6 == v10 )
    v11 = 1;
  else
    v11 = _stringCompareWithSmolCheck(_:_:expecting:)();
  swift_bridgeObjectRelease(v6);
  swift_bridgeObjectRelease(v10);
  return v11 & 1;
}

/* ========================================================================
 * sub_100048664
 * EA: 0x100048664
 ======================================================================== */

Swift::Int sub_100048664()
{
  unsigned __int8 *v0; // x20
  int v1; // w19
  unsigned __int64 v2; // x10
  __int64 v3; // x11
  __int64 v4; // x1
  unsigned __int64 v5; // x19
  _QWORD v7[9]; // [xsp+8h] [xbp-58h] BYREF

  v1 = *v0;
  Hasher.init(_seed:)(v7, 0);
  v2 = 0xE700000000000000LL;
  v3 = 0x736E61482D687ALL;
  if ( v1 != 1 )
  {
    v3 = 24938;
    v2 = 0xE200000000000000LL;
  }
  if ( v1 )
    v4 = v3;
  else
    v4 = 28261;
  if ( v1 )
    v5 = v2;
  else
    v5 = 0xE200000000000000LL;
  String.hash(into:)(v7, v4, v5);
  swift_bridgeObjectRelease(v5);
  return Hasher._finalize()();
}

/* ========================================================================
 * sub_1000486F0
 * EA: 0x1000486f0
 ======================================================================== */

__int64 __fastcall sub_1000486F0(__int64 a1)
{
  _BYTE *v1; // x20
  unsigned __int64 v2; // x11
  __int64 v3; // x12
  __int64 v4; // x1
  unsigned __int64 v5; // x19

  v2 = 0xE700000000000000LL;
  v3 = 0x736E61482D687ALL;
  if ( *v1 != 1 )
  {
    v3 = 24938;
    v2 = 0xE200000000000000LL;
  }
  if ( *v1 )
    v4 = v3;
  else
    v4 = 28261;
  if ( *v1 )
    v5 = v2;
  else
    v5 = 0xE200000000000000LL;
  String.hash(into:)(a1, v4, v5);
  return swift_bridgeObjectRelease(v5);
}

/* ========================================================================
 * sub_100048758
 * EA: 0x100048758
 ======================================================================== */

Swift::Int __fastcall sub_100048758(__int64 a1)
{
  unsigned __int8 *v1; // x20
  int v2; // w19
  unsigned __int64 v3; // x10
  __int64 v4; // x11
  __int64 v5; // x1
  unsigned __int64 v6; // x19
  _QWORD v8[9]; // [xsp+8h] [xbp-58h] BYREF

  v2 = *v1;
  Hasher.init(_seed:)(v8, a1);
  v3 = 0xE700000000000000LL;
  v4 = 0x736E61482D687ALL;
  if ( v2 != 1 )
  {
    v4 = 24938;
    v3 = 0xE200000000000000LL;
  }
  if ( v2 )
    v5 = v4;
  else
    v5 = 28261;
  if ( v2 )
    v6 = v3;
  else
    v6 = 0xE200000000000000LL;
  String.hash(into:)(v8, v5, v6);
  swift_bridgeObjectRelease(v6);
  return Hasher._finalize()();
}

/* ========================================================================
 * sub_100048890
 * EA: 0x100048890
 ======================================================================== */

__int64 __fastcall sub_100048890(__int64 a1, __int64 a2)
{
  __int64 v2; // x20
  __int64 v3; // x19
  id v4; // x0
  __int128 v5; // kr00_16
  void *v6; // x20
  __int128 v7; // q1
  id v8; // x23
  id v9; // x24
  __int64 v10; // x21
  __int64 v11; // x0
  unsigned __int64 v12; // x22
  __int64 v13; // x24
  int v14; // w8
  unsigned __int64 v15; // x20
  __int64 v16; // x23
  __int64 v17; // x8
  char v19; // w24
  __int64 v20; // [xsp+0h] [xbp-90h] BYREF
  unsigned __int64 v21; // [xsp+8h] [xbp-88h]
  _QWORD v22[2]; // [xsp+18h] [xbp-78h] BYREF
  __int128 v23; // [xsp+28h] [xbp-68h] BYREF
  _BYTE v24[24]; // [xsp+38h] [xbp-58h]
  __int128 v25; // [xsp+50h] [xbp-40h] BYREF

  v3 = v2;
  v20 = 0;
  v21 = 0xE000000000000000LL;
  v4 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSUserDefaults, a2), "standardUserDefaults"));
  FoilDefaultStorage.init(wrappedValue:key:userDefaults:)(
    &v23,
    &v20,
    0x676E614C7473614CLL,
    0xEC00000065676175LL,
    v4,
    &type metadata for String,
    &protocol witness table for String);
  v5 = v23;
  v6 = *(void **)v24;
  v25 = *(_OWORD *)&v24[8];
  v7 = *(_OWORD *)v24;
  *(_OWORD *)(v3 + 16) = v23;
  *(_OWORD *)(v3 + 32) = v7;
  *(_QWORD *)(v3 + 48) = *(_QWORD *)&v24[16];
  *(_BYTE *)(v3 + 56) = 0;
  v8 = objc_retain((id)v5);
  swift_retain(*((_QWORD *)&v5 + 1));
  v9 = objc_retain(v6);
  sub_10001D39C(&v25, &v20);
  v10 = sub_100004DF0(&unk_10033C708, &unk_10025CC30);
  FoilDefaultStorage.wrappedValue.getter(&v20, v10);
  objc_release(v9);
  swift_release(*((_QWORD *)&v5 + 1));
  objc_release(v8);
  v11 = sub_10001D3D8(&v25);
  v13 = v20;
  v12 = v21;
  v14 = (unsigned __int8)sub_100048C24(v11);
  if ( v14 )
  {
    if ( v14 == 1 )
    {
      v15 = 0xE700000000000000LL;
      v16 = 0x736E61482D687ALL;
    }
    else
    {
      v15 = 0xE200000000000000LL;
      v16 = 24938;
    }
  }
  else
  {
    v15 = 0xE200000000000000LL;
    v16 = 28261;
  }
  v17 = HIBYTE(v12) & 0xF;
  if ( (v12 & 0x2000000000000000LL) == 0 )
    v17 = v13 & 0xFFFFFFFFFFFFLL;
  if ( !v17 || v16 == v13 && v15 == v12 )
  {
    swift_bridgeObjectRelease(v12);
  }
  else
  {
    v19 = _stringCompareWithSmolCheck(_:_:expecting:)(v16, v15, v13, v12, 0);
    swift_bridgeObjectRelease(v12);
    if ( (v19 & 1) == 0 )
      *(_BYTE *)(v3 + 56) = 1;
  }
  v22[0] = v16;
  v22[1] = v15;
  swift_beginAccess(v3 + 16, &v20, 33, 0);
  FoilDefaultStorage.wrappedValue.setter(v22, v10);
  swift_endAccess(&v20);
  return v3;
}

/* ========================================================================
 * sub_100048AE8
 * EA: 0x100048ae8
 ======================================================================== */

__int64 __fastcall sub_100048AE8(__int64 a1, void *a2, __int64 a3)
{
  __int64 v6; // x0
  Swift::OpaquePointer v7; // x0
  Swift::String v8; // x1
  unsigned __int64 v9; // x19

  v6 = sub_100004DF0(&unk_10033C568, &unk_10025BC00);
  v7._rawValue = (void *)swift_initStaticObject(v6, a3);
  v8._countAndFlagsBits = a1;
  v8._object = a2;
  v9 = _findStringSwitchCase(cases:string:)(v7, v8);
  swift_bridgeObjectRelease(a2);
  if ( v9 >= 3 )
    return 3;
  else
    return v9;
}

/* ========================================================================
 * sub_100048C24
 * EA: 0x100048c24
 ======================================================================== */

__int64 sub_100048C24()
{
  _QWORD *v0; // x0
  __int64 v1; // x19
  __int64 v2; // x23
  _QWORD *v3; // x20
  __int64 v4; // x0
  __int64 v5; // x21
  unsigned int v6; // w24
  __int64 v8; // [xsp+0h] [xbp-50h] BYREF
  unsigned __int64 v9; // [xsp+8h] [xbp-48h]
  __int64 v10; // [xsp+10h] [xbp-40h]
  __int64 v11; // [xsp+18h] [xbp-38h]

  v0 = (_QWORD *)static Locale.preferredLanguages.getter();
  if ( v0[2] )
  {
    v2 = v0[4];
    v1 = v0[5];
    v3 = v0;
    swift_bridgeObjectRetain(v1);
    v4 = swift_bridgeObjectRelease(v3);
    v10 = v2;
    v11 = v1;
    v8 = 0x6E61482D687ALL;
    v9 = 0xE600000000000000LL;
    v5 = sub_10000DF9C(v4);
    v6 = StringProtocol.contains<A>(_:)(&v8, &type metadata for String, &type metadata for String, v5, v5) & 1;
    v10 = v2;
    v11 = v1;
    v8 = 24938;
    v9 = 0xE200000000000000LL;
    LOBYTE(v3) = StringProtocol.contains<A>(_:)(&v8, &type metadata for String, &type metadata for String, v5, v5);
    swift_bridgeObjectRelease(v1);
    if ( ((unsigned __int8)v3 & 1) != 0 )
      return 2;
    else
      return v6;
  }
  else
  {
    swift_bridgeObjectRelease(v0);
    return 0;
  }
}

/* ========================================================================
 * sub_100048D30
 * EA: 0x100048d30
 ======================================================================== */

id sub_100048D30()
{
  _BYTE *v0; // x20
  CGFloat *v1; // x8
  CGFloat z; // d0
  __int64 v3; // x19
  __int64 v4; // x19
  id v5; // x0
  objc_super v7; // [xsp+0h] [xbp-30h] BYREF

  *(_QWORD *)&v0[OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_rawIMUs] = &_swiftEmptyArrayStorage;
  *(_QWORD *)&v0[OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_capacity] = 16;
  v0[OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_isIMUInitialized] = 0;
  v1 = (CGFloat *)&v0[OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_anchorIMU];
  z = SCNVector3Zero.z;
  *(_OWORD *)v1 = *(_OWORD *)&SCNVector3Zero.x;
  v1[2] = z;
  v0[OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_alwaysUsePrediction] = 0;
  *(_QWORD *)&v0[OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_fps] = 0x404E000000000000LL;
  v0[OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_statisticEnabled] = 1;
  v3 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_tiStatistics;
  *(_QWORD *)&v0[v3] = sub_100004D14(0xD000000000000010LL, 0x800000010026F200LL, 1024);
  v4 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock;
  v5 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSLock), "init");
  *(_QWORD *)&v0[v4] = v5;
  v7.receiver = v0;
  v7.super_class = (Class)type metadata accessor for VTIMUSlerpUtil(v5);
  return objc_msgSendSuper2(&v7, "init");
}

/* ========================================================================
 * sub_100048E5C
 * EA: 0x100048e5c
 ======================================================================== */

id sub_100048E5C()
{
  __int64 v0; // x20
  void *v1; // x19
  __int64 v2; // x21
  __int64 v3; // x0
  id result; // x0
  CGFloat z; // d0
  __int64 v6; // x9
  __int64 v7; // x23
  id v8; // x21
  __int64 v9; // x8
  __int64 v10; // x0
  _BYTE v11[24]; // [xsp+8h] [xbp-48h] BYREF

  v1 = *(void **)(v0 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock);
  objc_msgSend(v1, "lock");
  v2 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_rawIMUs;
  swift_beginAccess(v0 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_rawIMUs, v11, 1, 0);
  v3 = *(_QWORD *)(v0 + v2);
  *(_QWORD *)(v0 + v2) = &_swiftEmptyArrayStorage;
  result = (id)swift_bridgeObjectRelease(v3);
  *(_BYTE *)(v0 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_isIMUInitialized) = 0;
  z = SCNVector3Zero.z;
  v6 = v0 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_anchorIMU;
  *(_OWORD *)v6 = *(_OWORD *)&SCNVector3Zero.x;
  *(CGFloat *)(v6 + 16) = z;
  if ( *(_BYTE *)(v0 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_statisticEnabled) == 1 )
  {
    v7 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_tiStatistics;
    v8 = objc_retain(*(id *)(v0 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_tiStatistics));
    sub_10000384C();
    objc_release(v8);
    v9 = *(_QWORD *)(v0 + v7);
    v10 = *(_QWORD *)(v9 + OBJC_IVAR____TtC11SpaceWalker16VTStatisticsUtil_values);
    *(_QWORD *)(v9 + OBJC_IVAR____TtC11SpaceWalker16VTStatisticsUtil_values) = &_swiftEmptyArrayStorage;
    swift_bridgeObjectRelease(v10);
    return objc_msgSend(v1, "unlock");
  }
  return result;
}

/* ========================================================================
 * sub_100048F6C
 * EA: 0x100048f6c
 ======================================================================== */

__int64 __fastcall sub_100048F6C(double a1, double a2, double a3)
{
  __int64 v3; // x20
  __int64 v4; // x19
  __int64 v8; // x23
  __int64 v9; // x24
  double *v10; // x28
  double *v11; // x25
  char *v12; // x20
  __int64 v13; // x21
  __int64 v14; // x26
  char *v15; // x22
  __int64 v16; // x27
  __int64 v17; // x8
  __int64 v18; // x9
  void (__fastcall *v19)(char *, char *, __int64); // x24
  char *v20; // x24
  char *v21; // x22
  double v22; // d11
  __int64 v23; // x8
  __int64 v24; // x20
  char *v25; // x25
  __int64 v26; // x20
  __int64 result; // x0
  __int64 v28; // x8
  __int64 v29; // x8
  unsigned __int64 v30; // x0
  double *v31; // x20
  double *v32; // x8
  id v33; // x20
  void (__fastcall *v34)(char *, __int64); // x19
  __int64 v35; // [xsp+0h] [xbp-D0h] BYREF
  __int64 v36; // [xsp+8h] [xbp-C8h]
  double *v37; // [xsp+10h] [xbp-C0h]
  id v38; // [xsp+18h] [xbp-B8h]
  char *v39; // [xsp+20h] [xbp-B0h]
  char *v40; // [xsp+28h] [xbp-A8h]
  _BYTE v41[24]; // [xsp+30h] [xbp-A0h] BYREF
  char v42[24]; // [xsp+48h] [xbp-88h] BYREF

  v4 = v3;
  v39 = (char *)&v35
      - ((*(_QWORD *)(*(_QWORD *)(sub_100004DF0(&unk_10033D218, &unk_10025CB50) - 8) + 64LL) + 15LL)
       & 0xFFFFFFFFFFFFFFF0LL);
  v8 = sub_100004DF0(&unk_10033BE20, &unk_10025B060);
  v9 = *(_QWORD *)(v8 - 8);
  v37 = (double *)((char *)&v35 - ((*(_QWORD *)(v9 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL));
  v10 = v37;
  v11 = v37;
  v12 = (char *)&v35
      - ((*(_QWORD *)(*(_QWORD *)(sub_100004DF0(&unk_10033D220, &unk_10025CB58) - 8) + 64LL) + 15LL)
       & 0xFFFFFFFFFFFFFFF0LL);
  v13 = type metadata accessor for Date(0);
  v14 = *(_QWORD *)(v13 - 8);
  v40 = (char *)&v35 - ((*(_QWORD *)(v14 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v15 = v40;
  v38 = *(id *)(v4 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock);
  objc_msgSend(v38, "lock");
  v16 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_rawIMUs;
  swift_beginAccess(v4 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_rawIMUs, v42, 1, 0);
  v17 = *(_QWORD *)(v4 + v16);
  v18 = *(_QWORD *)(v17 + 16);
  v36 = v9;
  if ( v18 )
  {
    sub_10004992C(
      v17 + ((*(unsigned __int8 *)(v9 + 80) + 32LL) & ~(unsigned __int64)*(unsigned __int8 *)(v9 + 80)),
      v11);
    v19 = *(void (__fastcall **)(char *, char *, __int64))(v14 + 32);
    v19(v12, (char *)v11 + *(int *)(v8 + 48), v13);
    (*(void (__fastcall **)(char *, _QWORD, __int64, __int64))(v14 + 56))(v12, 0, 1, v13);
    v19(v15, v12, v13);
    v20 = v15;
  }
  else
  {
    (*(void (__fastcall **)(char *, __int64, __int64, __int64))(v14 + 56))(v12, 1, 1, v13);
    Date.init()(v15);
    v20 = v15;
    if ( (*(unsigned int (__fastcall **)(char *, __int64, __int64))(v14 + 48))(v12, 1, v13) != 1 )
      sub_10000C910(v12, &unk_10033D220, &unk_10025CB58);
  }
  v21 = v40;
  Date.init()(v40);
  v22 = Date.timeIntervalSince(_:)(v20);
  v23 = *(int *)(v8 + 48);
  v24 = *(int *)(v8 + 64);
  *v10 = a1;
  v10[1] = a2;
  v10[2] = a3;
  (*(void (__fastcall **)(char *, char *, __int64))(v14 + 16))((char *)v10 + v23, v21, v13);
  *(double *)((char *)v10 + v24) = v22;
  swift_beginAccess(v4 + v16, v41, 33, 0);
  v25 = v39;
  sub_10004997C(v10, v39);
  sub_10003ACBC(0, 0, v25);
  swift_endAccess(v41);
  v26 = *(_QWORD *)(v4 + v16);
  if ( *(_QWORD *)(v26 + 16) <= 0x10u )
    goto LABEL_9;
  result = swift_isUniquelyReferenced_nonNull_native(*(_QWORD *)(v4 + v16));
  if ( (result & 1) == 0 )
  {
    result = sub_100011D2C(v26);
    v26 = result;
    v28 = *(_QWORD *)(result + 16);
    if ( v28 )
      goto LABEL_8;
LABEL_15:
    __break(1u);
    return result;
  }
  v28 = *(_QWORD *)(v26 + 16);
  if ( !v28 )
    goto LABEL_15;
LABEL_8:
  v29 = v28 - 1;
  v30 = v26
      + ((*(unsigned __int8 *)(v36 + 80) + 32LL) & ~(unsigned __int64)*(unsigned __int8 *)(v36 + 80))
      + *(_QWORD *)(v36 + 72) * v29;
  *(_QWORD *)(v26 + 16) = v29;
  *(_QWORD *)(v4 + v16) = v26;
  v31 = v37;
  sub_10004997C(v30, v37);
  (*(void (__fastcall **)(char *, __int64))(v14 + 8))((char *)v31 + *(int *)(v8 + 48), v13);
LABEL_9:
  if ( (*(_BYTE *)(v4 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_isIMUInitialized) & 1) == 0 )
  {
    *(_BYTE *)(v4 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_isIMUInitialized) = 1;
    v32 = (double *)(v4 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_anchorIMU);
    *v32 = a1;
    v32[1] = a2;
    v32[2] = a3;
  }
  if ( *(_BYTE *)(v4 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_statisticEnabled) == 1 )
  {
    v33 = objc_retain(*(id *)(v4 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_tiStatistics));
    sub_10000371C(v22 * 1000.0);
    objc_release(v33);
  }
  objc_msgSend(v38, "unlock");
  v34 = *(void (__fastcall **)(char *, __int64))(v14 + 8);
  v34(v21, v13);
  return ((__int64 (__fastcall *)(char *, __int64))v34)(v20, v13);
}

/* ========================================================================
 * sub_1000493C8
 * EA: 0x1000493c8
 ======================================================================== */

void sub_1000493C8()
{
  __int64 v0; // x20
  __int64 v1; // x22
  __int64 v2; // x23
  double *v3; // x19
  __int64 v4; // x26
  __int64 v5; // x8
  __int64 v6; // x8
  double v7; // d8
  double v8; // d10
  double v9; // d9
  __int64 v10; // x27
  __int64 v11; // x28
  __int64 v12; // d11
  __int64 v13; // x23
  void (__fastcall *v14)(char *, char *, __int64); // x25
  double *v15; // x24
  double v16; // d1
  double v17; // x8
  float v18; // s0
  double v19; // d1
  double v20; // d2
  float32x4_t v21; // q0
  __int64 v22; // x8
  double v23; // d10
  double v24; // d11
  double v25; // d9
  __int64 v26; // x8
  __int64 v27; // x20
  __int64 v28; // d12
  double v29; // d1
  double v30; // x8
  float v31; // s0
  double v32; // d1
  double v33; // d2
  float32x4_t v34; // q0
  float32x4_t v35; // q2
  float32x4_t v36; // q0
  float32x4_t v37; // q1
  float32x4_t v38; // q3
  float32x4_t v39; // q0
  int8x16_t v40; // q0
  float32x4_t v41; // q1
  int8x16_t v42; // q1
  __int128 v43; // q2
  float v44; // s8
  float v45; // s10
  bool v46; // zf
  float32x2_t v47; // d0
  float32x2_t v48; // d1
  float32x2_t v49; // d1
  float32x2_t v50; // d0
  int32x4_t v51; // q3
  float v52; // s8
  int32x4_t v53; // [xsp+0h] [xbp-E0h] BYREF
  __int128 v54; // [xsp+10h] [xbp-D0h]
  float32x4_t v55; // [xsp+20h] [xbp-C0h]
  float32x4_t v56; // [xsp+30h] [xbp-B0h]
  _BYTE v57[24]; // [xsp+48h] [xbp-98h] BYREF

  v1 = sub_100004DF0(&unk_10033BE20, &unk_10025B060);
  v2 = *(_QWORD *)(v1 - 8);
  v3 = (double *)((char *)v53.i64 - ((*(_QWORD *)(v2 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL));
  v4 = OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_rawIMUs;
  swift_beginAccess(v0 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_rawIMUs, v57, 0, 0);
  v5 = *(_QWORD *)(v0 + v4);
  if ( *(_QWORD *)(v5 + 16) < 2u )
  {
    __break(1u);
    goto LABEL_21;
  }
  v6 = v5 + *(_QWORD *)(v2 + 72);
  v56.i64[0] = (*(unsigned __int8 *)(v2 + 80) + 32LL) & ~(unsigned __int64)*(unsigned __int8 *)(v2 + 80);
  sub_10004992C(v6 + v56.i64[0], v3);
  v7 = *v3;
  v8 = v3[1];
  v9 = v3[2];
  v10 = *(int *)(v1 + 48);
  v11 = *(int *)(v1 + 64);
  v12 = *(_QWORD *)((char *)v3 + v11);
  *v3 = *v3;
  v3[1] = v8;
  v3[2] = v9;
  v13 = type metadata accessor for Date(0);
  v14 = *(void (__fastcall **)(char *, char *, __int64))(*(_QWORD *)(v13 - 8) + 32LL);
  v14((char *)v3 + v10, (char *)v3 + v10, v13);
  *(_QWORD *)((char *)v3 + v11) = v12;
  sub_10000C910(v3, &unk_10033BE20, &unk_10025B060);
  v15 = (double *)(v0 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_anchorIMU);
  v16 = v8 - *(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_anchorIMU + 8);
  if ( v16 >= -180.0 )
  {
    if ( v16 <= 180.0 )
      goto LABEL_7;
    v17 = -360.0;
  }
  else
  {
    v17 = 360.0;
  }
  v16 = v16 + v17;
LABEL_7:
  v18 = SCNVector3.init(_:_:_:)(
          (*(double *)(v0 + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_anchorIMU) - v7) * 3.14159265 / 180.0,
          v16 * 3.14159265 / 180.0,
          (v9 - v15[2]) * 3.14159265 / 180.0);
  *(float *)&v19 = v19;
  *(float *)&v20 = v20;
  sub_1000AD04C(v18, *(float *)&v19, *(float *)&v20);
  v22 = *(_QWORD *)(v0 + v4);
  if ( !*(_QWORD *)(v22 + 16) )
  {
LABEL_21:
    __break(1u);
    return;
  }
  v55 = v21;
  sub_10004992C(v22 + v56.i64[0], v3);
  v23 = *v3;
  v24 = v3[1];
  v25 = v3[2];
  v26 = *(int *)(v1 + 48);
  v27 = *(int *)(v1 + 64);
  v28 = *(_QWORD *)((char *)v3 + v27);
  *v3 = *v3;
  v3[1] = v24;
  v3[2] = v25;
  v14((char *)v3 + v26, (char *)v3 + v26, v13);
  *(_QWORD *)((char *)v3 + v27) = v28;
  sub_10000C910(v3, &unk_10033BE20, &unk_10025B060);
  v29 = v24 - v15[1];
  if ( v29 < -180.0 )
  {
    v30 = 360.0;
LABEL_12:
    v29 = v29 + v30;
    goto LABEL_13;
  }
  if ( v29 > 180.0 )
  {
    v30 = -360.0;
    goto LABEL_12;
  }
LABEL_13:
  v31 = SCNVector3.init(_:_:_:)(
          (*v15 - v23) * 3.14159265 / 180.0,
          v29 * 3.14159265 / 180.0,
          (v25 - v15[2]) * 3.14159265 / 180.0);
  *(float *)&v32 = v32;
  *(float *)&v33 = v33;
  *(double *)v34.i64 = sub_1000AD04C(v31, *(float *)&v32, *(float *)&v33);
  v35 = v34;
  v36 = vmulq_f32(v55, v34);
  v37 = (float32x4_t)vextq_s8((int8x16_t)v36, (int8x16_t)v36, 8u);
  *(float32x2_t *)v36.f32 = vadd_f32(*(float32x2_t *)v36.f32, *(float32x2_t *)v37.f32);
  v36.f32[0] = vaddv_f32(*(float32x2_t *)v36.f32);
  v37.i64[0] = 0;
  v38 = (float32x4_t)vbslq_s8(
                       (int8x16_t)vdupq_lane_s32((int32x2_t)vmvnq_s8((int8x16_t)vcgeq_f32(v36, v37)), 0),
                       (int8x16_t)vnegq_f32(v55),
                       (int8x16_t)v55);
  v39 = vsubq_f32(v35, v38);
  v40 = (int8x16_t)vmulq_f32(v39, v39);
  v55 = v38;
  v56 = v35;
  v41 = vaddq_f32(v35, v38);
  v42 = (int8x16_t)vmulq_f32(v41, v41);
  v47.f32[0] = atan2f(
                 sqrtf(vaddv_f32(vadd_f32(*(float32x2_t *)v40.i8, (float32x2_t)vextq_s8(v40, v40, 8u)))),
                 sqrtf(vaddv_f32(vadd_f32(*(float32x2_t *)v42.i8, (float32x2_t)vextq_s8(v42, v42, 8u)))));
  v44 = v47.f32[0] + v47.f32[0];
  v45 = 1.0;
  v46 = (float)(v47.f32[0] + v47.f32[0]) == 0.0;
  v47.i32[0] = 1.0;
  if ( !v46 )
    v47.f32[0] = sinf(v44) / v44;
  v48 = vrecpe_f32(v47);
  v49 = vmul_f32(v48, vrecps_f32(v47, v48));
  v50 = vrecps_f32(v47, v49);
  *(float32x2_t *)&v43 = vmul_f32(v49, v50);
  if ( (float)(v44 * 0.15) != 0.0 )
  {
    v54 = v43;
    v50.f32[0] = sinf(v44 * 0.15);
    v43 = v54;
    v45 = v50.f32[0] / (float)(v44 * 0.15);
  }
  v50.f32[0] = (float)(*(float *)&v43 * v45) * 0.15;
  v51 = vdupq_lane_s32((int32x2_t)v50, 0);
  v52 = v44 * 0.85;
  if ( v52 != 0.0 )
  {
    v53 = v51;
    v54 = v43;
    sinf(v52);
  }
}

/* ========================================================================
 * sub_1000499CC
 * EA: 0x1000499cc
 ======================================================================== */

void sub_1000499CC()
{
  __int64 v0; // x20
  __int64 v1; // x0
  __int64 inited; // x19
  __int128 v3; // q0
  __int128 v4; // q0
  __int128 v5; // q1
  __int128 v6; // q2
  __int64 v7; // x20
  __int64 v8; // x1
  id v9; // x19
  NSString v10; // x21
  __int64 v11; // x22
  Class isa; // x20
  __int64 v13; // [xsp+0h] [xbp-110h] BYREF
  _BYTE v14[136]; // [xsp+58h] [xbp-B8h] BYREF

  v1 = sub_100004DF0(&unk_10033D228, &unk_10025CB88);
  inited = swift_initStackObject(v1, v14);
  *(_OWORD *)(inited + 16) = xmmword_10025B110;
  *(_QWORD *)(inited + 32) = 0x6567617373656DLL;
  *(_QWORD *)(inited + 40) = 0xE700000000000000LL;
  v3 = *(_OWORD *)(v0 + 16);
  *(_OWORD *)(inited + 48) = *(_OWORD *)v0;
  *(_OWORD *)(inited + 64) = v3;
  v5 = *(_OWORD *)(v0 + 48);
  v4 = *(_OWORD *)(v0 + 64);
  v6 = *(_OWORD *)(v0 + 32);
  *(_QWORD *)(inited + 128) = *(_QWORD *)(v0 + 80);
  *(_OWORD *)(inited + 96) = v5;
  *(_OWORD *)(inited + 112) = v4;
  *(_OWORD *)(inited + 80) = v6;
  sub_1000139DC(v0, &v13);
  v7 = sub_100012AD8(inited);
  swift_setDeallocating(inited);
  sub_10004A3C0(inited + 32);
  v9 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter, v8), "defaultCenter"));
  v10 = String._bridgeToObjectiveC()();
  v11 = sub_10000F868(v7);
  swift_bridgeObjectRelease(v7);
  isa = Dictionary._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v11);
  objc_msgSend(v9, "postNotificationName:object:userInfo:", v10, 0, isa);
  objc_release(v9);
  objc_release(v10);
  objc_release(isa);
}

/* ========================================================================
 * sub_100049C44
 * EA: 0x100049c44
 ======================================================================== */

__int64 __usercall sub_100049C44@<X0>(__int64 a1@<X0>, __int64 a2@<X1>, __int64 a3@<X8>)
{
  __int64 v6; // x24
  __int64 v7; // x23
  _QWORD *v8; // x28
  __int64 v9; // x0
  __int64 v10; // x20
  __int64 v11; // x1
  __int64 v12; // x25
  __int64 v13; // x26
  __int64 v14; // x1
  __int64 v15; // x27
  __int128 v16; // q0
  __int64 v17; // x0
  __int64 v18; // x20
  __int64 v19; // x1
  __int64 v20; // x25
  __int64 v21; // x26
  __int64 v22; // x1
  __int64 v23; // x27
  __int128 v24; // q0
  __int64 v25; // x0
  __int64 v26; // x20
  __int64 v27; // x1
  __int64 v28; // x25
  __int64 v29; // x26
  __int64 v30; // x1
  __int64 v31; // x27
  __int128 v32; // q0
  __int64 v33; // x0
  __int64 v34; // x20
  __int64 v35; // x1
  __int64 v36; // x25
  __int64 v37; // x26
  __int64 v38; // x1
  __int64 v39; // x27
  __int128 v40; // q0
  __int64 v41; // x0
  __int64 v42; // x20
  __int64 v43; // x1
  __int64 v44; // x25
  __int64 v45; // x26
  __int64 v46; // x1
  __int64 v47; // x27
  __int128 v48; // q0
  __int64 v49; // x0
  __int64 v50; // x20
  __int64 v51; // x1
  __int64 v52; // x25
  __int64 v53; // x26
  __int64 v54; // x1
  __int64 v55; // x27
  __int128 v56; // q0
  __int64 v57; // x0
  __int64 v58; // x20
  __int64 v59; // x1
  __int64 v60; // x25
  __int64 v61; // x26
  __int64 v62; // x1
  __int64 v63; // x27
  __int128 v64; // q0
  __int64 v65; // x0
  __int64 v66; // x20
  __int64 v67; // x1
  __int64 v68; // x25
  __int64 v69; // x26
  __int64 v70; // x1
  __int64 v71; // x27
  __int128 v72; // q0
  __int64 v73; // x0
  __int64 v74; // x20
  __int64 v75; // x1
  __int64 v76; // x25
  __int64 v77; // x26
  __int64 v78; // x1
  __int64 v79; // x27
  double v80; // d0
  int v81; // s8
  __int64 v82; // x27
  __int64 result; // x0
  __int64 v84; // x0
  __int64 v85; // x20
  __int64 v86; // x1
  __int64 v87; // x26
  __int64 v88; // x28
  __int64 v89; // x1
  __int64 v90; // x23
  int v91; // w25
  __int64 v92; // x0
  __int64 v93; // x20
  __int64 v94; // x1
  __int64 v95; // x23
  __int64 v96; // x28
  __int64 v97; // x1
  __int64 v98; // x26
  __int64 v99; // x0
  __int64 v100; // x20
  __int64 v101; // x1
  __int64 v102; // x23
  __int64 v103; // x26
  __int64 v104; // x1
  __int64 v105; // x28
  __int64 v106; // x27
  __int64 v107; // x0
  __int64 v108; // x20
  __int64 v109; // x1
  __int64 v110; // x23
  __int64 v111; // x26
  __int64 v112; // x27
  __int64 v113; // x1
  __int64 v114; // x24
  __int64 v115; // x0
  __int64 v116; // x20
  __int64 v117; // x1
  __int64 v118; // x23
  __int64 v119; // x24
  __int64 v120; // x1
  __int64 v121; // x26
  int v122; // w28
  __int64 v123; // x25
  _QWORD *v124; // x20
  _QWORD *v125; // x0
  double v126; // d9
  __int128 v127; // q0
  __int128 v128; // q1
  unsigned __int64 v129; // x8
  __int64 v130; // x10
  __int64 v131; // x9
  __int64 v132; // [xsp+0h] [xbp-120h] BYREF
  __int64 v133; // [xsp+8h] [xbp-118h]
  __int64 v134; // [xsp+10h] [xbp-110h]
  unsigned int v135; // [xsp+1Ch] [xbp-104h]
  _QWORD *v136; // [xsp+20h] [xbp-100h]
  __int64 v137; // [xsp+28h] [xbp-F8h]
  __int128 v138; // [xsp+30h] [xbp-F0h]
  __int128 v139; // [xsp+40h] [xbp-E0h]
  __int128 v140; // [xsp+50h] [xbp-D0h]
  __int128 v141; // [xsp+60h] [xbp-C0h]
  __int128 v142; // [xsp+70h] [xbp-B0h]
  __int128 v143; // [xsp+80h] [xbp-A0h]
  __int128 v144; // [xsp+90h] [xbp-90h]
  __int128 v145; // [xsp+A0h] [xbp-80h]
  __int64 v146; // [xsp+B0h] [xbp-70h]

  v6 = type metadata accessor for Date(0);
  v7 = *(_QWORD *)(v6 - 8);
  v8 = (__int64 *)((char *)&v132 - ((*(_QWORD *)(v7 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL));
  if ( *(_QWORD *)(a1 + 16) != 46 )
  {
    swift_bridgeObjectRelease(a1);
    result = swift_bridgeObjectRelease(a2);
    v122 = 0;
    a2 = 0;
    a1 = 0;
    v131 = 0;
    v129 = 0;
    v130 = 0;
    v127 = 0u;
    v126 = 0.0;
    v81 = 0;
    v128 = 0u;
    goto LABEL_8;
  }
  v9 = swift_bridgeObjectRetain(a1);
  v10 = sub_100007D64(v9);
  v12 = v11;
  v13 = Data._Representation.subscript.getter(0, 4, v10, v11);
  v15 = v14;
  *(double *)&v16 = sub_10000E528(1, v13, v14);
  v145 = v16;
  sub_10000C5FC(v13, v15);
  sub_10000C5FC(v10, v12);
  v17 = swift_bridgeObjectRetain(a1);
  v18 = sub_100007D64(v17);
  v20 = v19;
  v21 = Data._Representation.subscript.getter(4, 8, v18, v19);
  v23 = v22;
  *(double *)&v24 = sub_10000E528(1, v21, v22);
  v144 = v24;
  sub_10000C5FC(v21, v23);
  sub_10000C5FC(v18, v20);
  v25 = swift_bridgeObjectRetain(a1);
  v26 = sub_100007D64(v25);
  v28 = v27;
  v29 = Data._Representation.subscript.getter(8, 12, v26, v27);
  v31 = v30;
  *(double *)&v32 = sub_10000E528(1, v29, v30);
  v143 = v32;
  sub_10000C5FC(v29, v31);
  sub_10000C5FC(v26, v28);
  v33 = swift_bridgeObjectRetain(a1);
  v34 = sub_100007D64(v33);
  v36 = v35;
  v37 = Data._Representation.subscript.getter(12, 16, v34, v35);
  v39 = v38;
  *(double *)&v40 = sub_10000E528(1, v37, v38);
  v141 = v40;
  sub_10000C5FC(v37, v39);
  sub_10000C5FC(v34, v36);
  v41 = swift_bridgeObjectRetain(a1);
  v42 = sub_100007D64(v41);
  v44 = v43;
  v45 = Data._Representation.subscript.getter(16, 20, v42, v43);
  v47 = v46;
  *(double *)&v48 = sub_10000E528(1, v45, v46);
  v142 = v48;
  sub_10000C5FC(v45, v47);
  sub_10000C5FC(v42, v44);
  v49 = swift_bridgeObjectRetain(a1);
  v50 = sub_100007D64(v49);
  v52 = v51;
  v53 = Data._Representation.subscript.getter(20, 24, v50, v51);
  v55 = v54;
  *(double *)&v56 = sub_10000E528(1, v53, v54);
  v140 = v56;
  sub_10000C5FC(v53, v55);
  sub_10000C5FC(v50, v52);
  v57 = swift_bridgeObjectRetain(a1);
  v58 = sub_100007D64(v57);
  v60 = v59;
  v61 = Data._Representation.subscript.getter(24, 28, v58, v59);
  v63 = v62;
  *(double *)&v64 = sub_10000E528(1, v61, v62);
  v139 = v64;
  sub_10000C5FC(v61, v63);
  sub_10000C5FC(v58, v60);
  v65 = swift_bridgeObjectRetain(a1);
  v66 = sub_100007D64(v65);
  v68 = v67;
  v69 = Data._Representation.subscript.getter(28, 32, v66, v67);
  v71 = v70;
  *(double *)&v72 = sub_10000E528(1, v69, v70);
  v138 = v72;
  sub_10000C5FC(v69, v71);
  sub_10000C5FC(v66, v68);
  v73 = swift_bridgeObjectRetain(a1);
  v74 = sub_100007D64(v73);
  v76 = v75;
  v77 = Data._Representation.subscript.getter(32, 36, v74, v75);
  v79 = v78;
  v80 = sub_10000E528(1, v77, v78);
  v81 = LODWORD(v80);
  sub_10000C5FC(v77, v79);
  sub_10000C5FC(v74, v76);
  v82 = sub_100004DF0(&unk_10033BD88, &unk_10025AEF0);
  result = swift_initStaticObject(v82, &unk_100339C50);
  if ( *(_QWORD *)(a1 + 16) < 0x27u )
  {
    __break(1u);
    goto LABEL_10;
  }
  v136 = v8;
  v137 = v7;
  v146 = result;
  v84 = swift_bridgeObjectRetain(a1);
  sub_10000912C(v84, a1 + 32, 36, 79);
  v85 = sub_100007D64(v146);
  v87 = v86;
  v88 = Data._Representation.subscript.getter(0, 4, v85, v86);
  v90 = v89;
  v91 = sub_10000E364(1, v88, v89);
  sub_10000C5FC(v88, v90);
  sub_10000C5FC(v85, v87);
  result = swift_initStaticObject(v82, &unk_100339C80);
  if ( *(_QWORD *)(a1 + 16) < 0x2Au )
  {
LABEL_10:
    __break(1u);
    goto LABEL_11;
  }
  v146 = result;
  v92 = swift_bridgeObjectRetain(a1);
  sub_10000912C(v92, a1 + 32, 39, 85);
  v93 = sub_100007D64(v146);
  v95 = v94;
  v96 = Data._Representation.subscript.getter(0, 4, v93, v94);
  v98 = v97;
  v135 = sub_10000E364(1, v96, v97);
  sub_10000C5FC(v96, v98);
  sub_10000C5FC(v93, v95);
  result = swift_initStaticObject(v82, &unk_100339CB0);
  if ( *(_QWORD *)(a1 + 16) < 0x2Du )
  {
LABEL_11:
    __break(1u);
    goto LABEL_12;
  }
  v146 = result;
  v99 = swift_bridgeObjectRetain(a1);
  sub_10000912C(v99, a1 + 32, 42, 91);
  v100 = sub_100007D64(v146);
  v102 = v101;
  v103 = Data._Representation.subscript.getter(0, 4, v100, v101);
  v105 = v104;
  v106 = sub_10000E364(1, v103, v104);
  sub_10000C5FC(v103, v105);
  result = sub_10000C5FC(v100, v102);
  if ( *(_QWORD *)(a1 + 16) >= 0x2Eu )
  {
    v134 = *(unsigned __int8 *)(a1 + 77);
    v107 = swift_bridgeObjectRetain(a2);
    v108 = sub_100007D64(v107);
    v110 = v109;
    v111 = Data._Representation.subscript.getter(6, 10, v108, v109);
    v133 = v106;
    v112 = v6;
    v114 = v113;
    HIDWORD(v132) = sub_10000E364(0, v111, v113);
    sub_10000C5FC(v111, v114);
    sub_10000C5FC(v108, v110);
    v115 = swift_bridgeObjectRetain(a2);
    v116 = sub_100007D64(v115);
    v118 = v117;
    v119 = Data._Representation.subscript.getter(10, 14, v116, v117);
    v121 = v120;
    v122 = v91;
    v123 = sub_10000E364(0, v119, v120);
    sub_10000C5FC(v119, v121);
    sub_10000C5FC(v116, v118);
    v124 = v136;
    v125 = Date.init()(v136);
    v126 = Date.timeIntervalSince1970.getter(v125);
    result = (*(__int64 (__fastcall **)(_QWORD *, __int64))(v137 + 8))(v124, v112);
    *(_QWORD *)&v127 = __PAIR64__(v144, v145);
    *((_QWORD *)&v127 + 1) = __PAIR64__(v141, v143);
    *(_QWORD *)&v128 = __PAIR64__(v140, v142);
    *((_QWORD *)&v128 + 1) = __PAIR64__(v138, v139);
    v129 = v135 | (unsigned __int64)(v133 << 32);
    v130 = v134 | (v123 << 32);
    v131 = HIDWORD(v132);
LABEL_8:
    *(_OWORD *)a3 = v127;
    *(_OWORD *)(a3 + 16) = v128;
    *(_DWORD *)(a3 + 32) = v81;
    *(_DWORD *)(a3 + 36) = v122;
    *(_QWORD *)(a3 + 40) = v129;
    *(_QWORD *)(a3 + 48) = v130;
    *(_QWORD *)(a3 + 56) = v131;
    *(double *)(a3 + 64) = v126;
    *(_QWORD *)(a3 + 72) = a2;
    *(_QWORD *)(a3 + 80) = a1;
    return result;
  }
LABEL_12:
  __break(1u);
  return result;
}

/* ========================================================================
 * $s11SpaceWalker23VTIMUMagnetometerHIDMsgVwca
 * EA: 0x10004a48c
 ======================================================================== */

__int64 __fastcall assignWithCopy for VTIMUMagnetometerHIDMsg(__int64 a1, __int64 a2)
{
  __int64 v4; // x0
  __int64 v5; // x21
  __int64 v6; // x0
  __int64 v7; // x19

  *(_DWORD *)a1 = *(_DWORD *)a2;
  *(_DWORD *)(a1 + 4) = *(_DWORD *)(a2 + 4);
  *(_DWORD *)(a1 + 8) = *(_DWORD *)(a2 + 8);
  *(_DWORD *)(a1 + 12) = *(_DWORD *)(a2 + 12);
  *(_DWORD *)(a1 + 16) = *(_DWORD *)(a2 + 16);
  *(_DWORD *)(a1 + 20) = *(_DWORD *)(a2 + 20);
  *(_DWORD *)(a1 + 24) = *(_DWORD *)(a2 + 24);
  *(_DWORD *)(a1 + 28) = *(_DWORD *)(a2 + 28);
  *(_DWORD *)(a1 + 32) = *(_DWORD *)(a2 + 32);
  *(_DWORD *)(a1 + 36) = *(_DWORD *)(a2 + 36);
  *(_DWORD *)(a1 + 40) = *(_DWORD *)(a2 + 40);
  *(_DWORD *)(a1 + 44) = *(_DWORD *)(a2 + 44);
  *(_BYTE *)(a1 + 48) = *(_BYTE *)(a2 + 48);
  *(_DWORD *)(a1 + 52) = *(_DWORD *)(a2 + 52);
  *(_DWORD *)(a1 + 56) = *(_DWORD *)(a2 + 56);
  *(_QWORD *)(a1 + 64) = *(_QWORD *)(a2 + 64);
  v4 = *(_QWORD *)(a2 + 72);
  v5 = *(_QWORD *)(a1 + 72);
  *(_QWORD *)(a1 + 72) = v4;
  swift_bridgeObjectRetain(v4);
  swift_bridgeObjectRelease(v5);
  v6 = *(_QWORD *)(a2 + 80);
  v7 = *(_QWORD *)(a1 + 80);
  *(_QWORD *)(a1 + 80) = v6;
  swift_bridgeObjectRetain(v6);
  swift_bridgeObjectRelease(v7);
  return a1;
}

/* ========================================================================
 * $s11SpaceWalker23VTIMUMagnetometerHIDMsgVwta
 * EA: 0x10004a58c
 ======================================================================== */

__int64 __fastcall assignWithTake for VTIMUMagnetometerHIDMsg(__int64 a1, __int64 a2)
{
  __int128 v4; // q1
  __int64 v5; // x0

  v4 = *(_OWORD *)(a2 + 16);
  *(_OWORD *)a1 = *(_OWORD *)a2;
  *(_OWORD *)(a1 + 16) = v4;
  *(_DWORD *)(a1 + 32) = *(_DWORD *)(a2 + 32);
  *(_QWORD *)(a1 + 36) = *(_QWORD *)(a2 + 36);
  *(_DWORD *)(a1 + 44) = *(_DWORD *)(a2 + 44);
  *(_BYTE *)(a1 + 48) = *(_BYTE *)(a2 + 48);
  *(_QWORD *)(a1 + 52) = *(_QWORD *)(a2 + 52);
  *(_QWORD *)(a1 + 64) = *(_QWORD *)(a2 + 64);
  swift_bridgeObjectRelease(*(_QWORD *)(a1 + 72));
  v5 = *(_QWORD *)(a1 + 80);
  *(_OWORD *)(a1 + 72) = *(_OWORD *)(a2 + 72);
  swift_bridgeObjectRelease(v5);
  return a1;
}

/* ========================================================================
 * -[_TtC11SpaceWalker20LaunchViewController loadView]
 * EA: 0x10004a6ac
 ======================================================================== */

void __cdecl -[LaunchViewController loadView](_TtC11SpaceWalker20LaunchViewController *self, SEL a2)
{
  id v3; // x20
  _TtC11SpaceWalker20LaunchViewController *v4; // x19
  id v5; // x20

  v3 = objc_allocWithZone((Class)&OBJC_CLASS___NSView);
  v4 = objc_retain(self);
  v5 = objc_msgSend(v3, "init");
  -[LaunchViewController setView:](v4, "setView:", v5);
  objc_release(v5);
  objc_release(v4);
}

/* ========================================================================
 * sub_10004A718
 * EA: 0x10004a718
 ======================================================================== */

void __fastcall sub_10004A718(__int64 a1, __int64 a2)
{
  __int64 v2; // x20
  void *v3; // x19
  id v4; // x22
  NSString v5; // x23
  id v6; // x22
  NSString v7; // x23
  id v8; // x21
  NSString v9; // x23
  id v10; // x21
  id v11; // x19
  NSString v12; // x22

  v3 = (void *)objc_opt_self(&OBJC_CLASS___NSNotificationCenter, a2);
  v4 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "defaultCenter"));
  v5 = String._bridgeToObjectiveC()();
  objc_msgSend(v4, "addObserver:selector:name:object:", v2, "showLaunchViewOnDisconnection", v5, 0);
  objc_release(v4);
  objc_release(v5);
  v6 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "defaultCenter"));
  v7 = String._bridgeToObjectiveC()();
  objc_msgSend(v6, "addObserver:selector:name:object:", v2, "showLaunchViewOnDisconnection", v7, 0);
  objc_release(v6);
  objc_release(v7);
  v8 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "defaultCenter"));
  v9 = String._bridgeToObjectiveC()();
  objc_msgSend(v8, "addObserver:selector:name:object:", v2, "hideConnectionViewIfPossible", v9, 0);
  objc_release(v8);
  objc_release(v9);
  v10 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "defaultCenter"));
  objc_msgSend(
    v10,
    "addObserver:selector:name:object:",
    v2,
    "onScreenParametersChanged",
    NSApplicationDidChangeScreenParametersNotification,
    0);
  objc_release(v10);
  v11 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "defaultCenter"));
  v12 = String._bridgeToObjectiveC()();
  objc_msgSend(v11, "addObserver:selector:name:object:", v2, "onVTGlassConnectedNotification:", v12, 0);
  objc_release(v11);
  objc_release(v12);
}

/* ========================================================================
 * sub_10007FE70
 * EA: 0x10007fe70
 ======================================================================== */

__int64 __fastcall sub_10007FE70(char a1)
{
  __int64 v2; // x19
  __int64 v3; // x27
  char *v4; // x21
  __int64 v5; // x22
  __int64 v6; // x28
  char *v7; // x23
  void *v8; // x24
  __int64 v9; // x0
  void *v10; // x25
  __int64 v11; // x0
  __int64 v12; // x0
  __int64 v13; // x26
  __int64 v14; // x20
  __int64 v15; // x0
  __int64 v16; // x19
  unsigned __int64 v17; // x20
  __int64 v18; // x0
  __int64 v19; // x21
  unsigned __int64 v20; // x1
  unsigned __int64 v21; // x22
  Swift::String v22; // x0
  void **v23; // x20
  unsigned __int64 v24; // x21
  __int64 v25; // x0
  __int64 v26; // x22
  __int64 v27; // x0
  void **aBlock; // [xsp+0h] [xbp-80h] BYREF
  unsigned __int64 v30; // [xsp+8h] [xbp-78h]
  __int64 (__fastcall *v31)(); // [xsp+10h] [xbp-70h]
  void *v32; // [xsp+18h] [xbp-68h]
  __int64 (__fastcall *v33)(); // [xsp+20h] [xbp-60h]
  __int64 v34; // [xsp+28h] [xbp-58h]

  v2 = type metadata accessor for DispatchWorkItemFlags(0);
  v3 = *(_QWORD *)(v2 - 8);
  v4 = (char *)&aBlock - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for DispatchQoS(0);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = (char *)&aBlock - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  sub_100004E40(0);
  v8 = (void *)static OS_dispatch_queue.main.getter();
  v9 = swift_allocObject(&unk_1002E16A0, 17, 7);
  *(_BYTE *)(v9 + 16) = a1;
  v33 = sub_1000802B0;
  v34 = v9;
  aBlock = _NSConcreteStackBlock;
  v30 = 1107296256;
  v31 = sub_10007A08C;
  v32 = &unk_1002E16B8;
  v10 = _Block_copy(&aBlock);
  v11 = swift_release(v34);
  v12 = static DispatchQoS.unspecified.getter(v11);
  aBlock = (void **)&_swiftEmptyArrayStorage;
  v13 = sub_10000C824(v12);
  v14 = sub_100004DF0(&unk_10033BE50, &unk_10025B090);
  v15 = sub_1000147C4();
  dispatch thunk of SetAlgebra.init<A>(_:)(&aBlock, v14, v15, v2, v13);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v7, v4, v10);
  _Block_release(v10);
  objc_release(v8);
  (*(void (__fastcall **)(char *, __int64))(v3 + 8))(v4, v2);
  (*(void (__fastcall **)(char *, __int64))(v6 + 8))(v7, v5);
  v16 = type metadata accessor for VTLogger(0);
  aBlock = nullptr;
  v30 = 0xE000000000000000LL;
  _StringGuts.grow(_:)(21);
  v17 = v30;
  aBlock = (void **)&type metadata for VTStart7911UpdateMsg;
  v18 = sub_100004DF0(&unk_10033DDB0, &unk_10025DE68);
  v19 = String.init<A>(describing:)(&aBlock, v18);
  v21 = v20;
  swift_bridgeObjectRelease(v17);
  aBlock = (void **)v19;
  v30 = v21;
  v22._object = (void *)0x800000010026ECA0LL;
  v22._countAndFlagsBits = 0xD000000000000013LL;
  String.append(_:)(v22);
  v23 = aBlock;
  v24 = v30;
  v26 = static os_log_type_t.debug.getter(v25);
  v27 = static os_log_type_t.debug.getter(v26);
  sub_100091CD8(v26, v27, v23, v24, v16);
  return swift_bridgeObjectRelease(v24);
}

/* ========================================================================
 * sub_1000800F0
 * EA: 0x1000800f0
 ======================================================================== */

void __fastcall sub_1000800F0(unsigned __int8 a1, __int64 a2)
{
  id v3; // x19
  NSString v4; // x20
  __int64 v5; // x0
  __int64 inited; // x22
  __int64 v7; // x21
  Class isa; // x22
  _QWORD v9[2]; // [xsp+8h] [xbp-A8h] BYREF
  _BYTE v10[104]; // [xsp+18h] [xbp-98h] BYREF

  v3 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter, a2), "defaultCenter"));
  v4 = String._bridgeToObjectiveC()();
  v5 = sub_100004DF0(&unk_10033CDE0, &unk_10025C6A0);
  inited = swift_initStackObject(v5, v10);
  *(_OWORD *)(inited + 16) = xmmword_10025B110;
  v9[0] = 0xD000000000000012LL;
  v9[1] = 0x8000000100270080LL;
  AnyHashable.init<A>(_:)((_QWORD *)(inited + 32), v9, &type metadata for String, &protocol witness table for String);
  *(_QWORD *)(inited + 96) = &type metadata for Int;
  *(_QWORD *)(inited + 72) = a1;
  v7 = sub_100013868(inited);
  swift_setDeallocating(inited);
  sub_1000406D0(inited + 32);
  isa = Dictionary._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v7);
  objc_msgSend(v3, "postNotificationName:object:userInfo:", v4, 0, isa);
  objc_release(v3);
  objc_release(v4);
  objc_release(isa);
}

/* ========================================================================
 * sub_1000802E0
 * EA: 0x1000802e0
 ======================================================================== */

void __fastcall sub_1000802E0(__int64 a1, __int64 a2, __int64 a3, __int64 a4)
{
  char *v8; // x26
  __int64 v9; // x22
  __int64 v10; // x27
  void (__fastcall *v11)(char *, __int64, __int64); // x8
  void (__fastcall *v12)(char *, __int64, __int64, __int64); // x20
  objc_class *v13; // x27
  char *v14; // x28
  __int64 v15; // x23
  id v16; // x27
  void *v17; // x25
  id v18; // x23
  id v19; // x0
  __int64 v20; // x25
  void *v21; // x24
  id v22; // x21
  __int64 v23; // x4
  unsigned int v24; // w24
  id v25; // x0
  void *v26; // x0
  id v27; // x19
  id v28; // x19
  id v29; // x21
  void *v30; // x19
  __int64 v31; // x20
  __int64 v32; // x21
  __int64 v33; // x0
  __int64 v34; // [xsp+0h] [xbp-90h] BYREF
  objc_super v35; // [xsp+10h] [xbp-80h] BYREF
  id v36[3]; // [xsp+20h] [xbp-70h] BYREF

  v8 = (char *)&v34
     - ((*(_QWORD *)(*(_QWORD *)(sub_100004DF0(&unk_10033DE48, &unk_10025DF08) - 8) + 64LL) + 15LL)
      & 0xFFFFFFFFFFFFFFF0LL);
  v9 = sub_100004DF0(&unk_10033DE40, &unk_10025DEF0);
  v10 = *(_QWORD *)(v9 - 8);
  v11 = *(void (__fastcall **)(char *, __int64, __int64))(v10 + 16);
  v34 = a1;
  v11(v8, a1, v9);
  v12 = *(void (__fastcall **)(char *, __int64, __int64, __int64))(v10 + 56);
  v12(v8, 0, 1, v9);
  v13 = (objc_class *)type metadata accessor for CaptureEngineStreamOutput(0);
  v14 = (char *)objc_allocWithZone(v13);
  v15 = OBJC_IVAR____TtC11SpaceWalkerP33_13031BFAB07D68F2E0DC27BB266D257F25CaptureEngineStreamOutput_continuation;
  v12(
    &v14[OBJC_IVAR____TtC11SpaceWalkerP33_13031BFAB07D68F2E0DC27BB266D257F25CaptureEngineStreamOutput_continuation],
    1,
    1,
    v9);
  swift_beginAccess(&v14[v15], v36, 33, 0);
  sub_100081658(v8, &v14[v15]);
  swift_endAccess(v36);
  v35.receiver = v14;
  v35.super_class = v13;
  v16 = objc_msgSendSuper2(&v35, "init");
  sub_10000C910(v8, &unk_10033DE48, &unk_10025DF08);
  v17 = *(void **)(a2 + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_streamOutput);
  *(_QWORD *)(a2 + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_streamOutput) = v16;
  v18 = objc_retain(v16);
  objc_release(v17);
  v19 = objc_msgSend(
          objc_allocWithZone((Class)&OBJC_CLASS___SCStream),
          "initWithFilter:configuration:delegate:",
          a3,
          a4,
          v18);
  v20 = OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_stream;
  v21 = *(void **)(a2 + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_stream);
  *(_QWORD *)(a2 + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_stream) = v19;
  v22 = objc_retain(v19);
  objc_release(v21);
  if ( v22 )
  {
    v23 = *(_QWORD *)(a2 + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_videoSampleBufferQueue);
    v36[0] = nullptr;
    v24 = (unsigned int)objc_msgSend(v22, "addStreamOutput:type:sampleHandlerQueue:error:", v18, 0, v23, v36);
    objc_release(v22);
    if ( !v24 )
    {
      v28 = v36[0];
      v29 = objc_retain(v36[0]);
      v30 = (void *)_convertNSErrorToError(_:)(v28);
      objc_release(v29);
      swift_willThrow();
      v36[0] = v30;
      swift_errorRetain(v30);
      AsyncThrowingStream.Continuation.finish(throwing:)(v36, v9);
      v31 = type metadata accessor for VTLogger(0);
      v32 = static os_log_type_t.error.getter();
      v33 = static os_log_type_t.error.getter();
      sub_100091CD8(v32, v33, 0xD000000000000015LL, 0x80000001002726C0LL, v31);
      objc_release(v18);
      swift_errorRelease(v30);
      return;
    }
    v25 = objc_retain(v36[0]);
  }
  v26 = *(void **)(a2 + v20);
  if ( v26 )
  {
    v27 = objc_retain(v26);
    objc_msgSend(v27, "startCaptureWithCompletionHandler:", 0);
    objc_release(v27);
  }
  objc_release(v18);
}

/* ========================================================================
 * sub_1000805FC
 * EA: 0x1000805fc
 ======================================================================== */

__int64 sub_1000805FC()
{
  __int64 v0; // x22
  __int64 v1; // x8
  __int64 v2; // x9
  void *v3; // x20
  __int64 v4; // x19
  __int64 v5; // x0
  __int64 v7; // x8
  void *v8; // x0

  v1 = *(_QWORD *)(v0 + 144);
  v2 = OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_stream;
  *(_QWORD *)(v0 + 152) = OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_stream;
  v3 = *(void **)(v1 + v2);
  *(_QWORD *)(v0 + 160) = v3;
  if ( v3 )
  {
    *(_QWORD *)(v0 + 16) = v0;
    *(_QWORD *)(v0 + 24) = sub_1000806FC;
    v4 = swift_continuation_init(v0 + 16, 1);
    v5 = sub_100004DF0(&unk_10033DE70, &unk_10025DF38);
    *(_QWORD *)(v0 + 80) = _NSConcreteStackBlock;
    *(_QWORD *)(v0 + 136) = v5;
    *(_QWORD *)(v0 + 88) = 1107296256;
    *(_QWORD *)(v0 + 96) = sub_100080860;
    *(_QWORD *)(v0 + 104) = &unk_1002E17F0;
    *(_QWORD *)(v0 + 112) = v4;
    objc_msgSend(objc_retain(v3), "stopCaptureWithCompletionHandler:", v0 + 80);
    return swift_continuation_await(v0 + 16);
  }
  else
  {
    *(_QWORD *)(v1 + v2) = 0;
    v7 = *(_QWORD *)(v0 + 144);
    v8 = *(void **)(v7 + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_streamOutput);
    *(_QWORD *)(v7 + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_streamOutput) = 0;
    objc_release(v8);
    return (*(__int64 (**)(void))(v0 + 8))();
  }
}

/* ========================================================================
 * sub_1000807B4
 * EA: 0x1000807b4
 ======================================================================== */

__int64 sub_1000807B4()
{
  __int64 v0; // x22
  void *v1; // x19
  __int64 v2; // x20
  __int64 v3; // x19
  __int64 v4; // x21
  __int64 v5; // x0
  __int64 v6; // x8
  void *v7; // x0

  v1 = *(void **)(v0 + 160);
  v2 = *(_QWORD *)(v0 + 168);
  swift_willThrow();
  objc_release(v1);
  v3 = type metadata accessor for VTLogger(0);
  v4 = static os_log_type_t.error.getter();
  v5 = static os_log_type_t.error.getter();
  sub_100091CD8(v4, v5, 0xD000000000000014LL, 0x80000001002727C0LL, v3);
  swift_errorRelease(v2);
  v6 = *(_QWORD *)(v0 + 144);
  v7 = *(void **)(v6 + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_streamOutput);
  *(_QWORD *)(v6 + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_streamOutput) = 0;
  objc_release(v7);
  return (*(__int64 (**)(void))(v0 + 8))();
}

/* ========================================================================
 * sub_100080860
 * EA: 0x100080860
 ======================================================================== */

__int64 __fastcall sub_100080860(__int64 a1, void *a2)
{
  _QWORD *v3; // x0
  __int64 v4; // x20
  __int64 v5; // x0
  __int64 v6; // x21
  _QWORD *v7; // x1
  id v8; // x0

  v3 = (_QWORD *)sub_10000C8A4(a1 + 32, *(_QWORD *)(a1 + 56));
  v4 = *v3;
  if ( !a2 )
    return swift_continuation_throwingResume(*v3);
  v5 = sub_100004DF0(&unk_10033C9B0, &unk_10025B810);
  v6 = swift_allocError(v5, &protocol self-conformance witness table for Error, 0, 0);
  *v7 = a2;
  v8 = objc_retain(a2);
  return swift_continuation_throwingResumeWithError(v4, v6);
}

/* ========================================================================
 * sub_100080908
 * EA: 0x100080908
 ======================================================================== */

__int64 sub_100080908()
{
  __int64 v0; // x22
  __int64 v1; // x8
  __int64 v2; // x9
  void *v3; // x20
  __int64 v4; // x19
  __int64 v5; // x21
  __int64 v6; // x0

  v1 = *(_QWORD *)(v0 + 224);
  v2 = OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_stream;
  *(_QWORD *)(v0 + 232) = OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_stream;
  v3 = *(void **)(v1 + v2);
  *(_QWORD *)(v0 + 240) = v3;
  if ( !v3 )
    return (*(__int64 (**)(void))(v0 + 8))();
  v4 = *(_QWORD *)(v0 + 208);
  *(_QWORD *)(v0 + 16) = v0;
  *(_QWORD *)(v0 + 24) = sub_1000809F4;
  v5 = swift_continuation_init(v0 + 16, 1);
  v6 = sub_100004DF0(&unk_10033DE70, &unk_10025DF38);
  *(_QWORD *)(v0 + 144) = _NSConcreteStackBlock;
  *(_QWORD *)(v0 + 200) = v6;
  *(_QWORD *)(v0 + 152) = 1107296256;
  *(_QWORD *)(v0 + 160) = sub_100080860;
  *(_QWORD *)(v0 + 168) = &unk_1002E17A0;
  *(_QWORD *)(v0 + 176) = v5;
  objc_msgSend(objc_retain(v3), "updateConfiguration:completionHandler:", v4, v0 + 144);
  return swift_continuation_await(v0 + 16);
}

/* ========================================================================
 * sub_100080A50
 * EA: 0x100080a50
 ======================================================================== */

__int64 sub_100080A50()
{
  __int64 v0; // x22
  __int64 v1; // x20
  __int64 v2; // x19
  void *v3; // x20
  __int64 v4; // x19
  __int64 v5; // x21
  __int64 v6; // x0

  v1 = *(_QWORD *)(v0 + 232);
  v2 = *(_QWORD *)(v0 + 224);
  objc_release(*(id *)(v0 + 240));
  v3 = *(void **)(v2 + v1);
  *(_QWORD *)(v0 + 256) = v3;
  if ( !v3 )
    return (*(__int64 (**)(void))(v0 + 8))();
  v4 = *(_QWORD *)(v0 + 216);
  *(_QWORD *)(v0 + 80) = v0;
  *(_QWORD *)(v0 + 88) = sub_100080B38;
  v5 = swift_continuation_init(v0 + 80, 1);
  v6 = sub_100004DF0(&unk_10033DE70, &unk_10025DF38);
  *(_QWORD *)(v0 + 144) = _NSConcreteStackBlock;
  *(_QWORD *)(v0 + 200) = v6;
  *(_QWORD *)(v0 + 152) = 1107296256;
  *(_QWORD *)(v0 + 160) = sub_100080860;
  *(_QWORD *)(v0 + 168) = &unk_1002E17C8;
  *(_QWORD *)(v0 + 176) = v5;
  objc_msgSend(objc_retain(v3), "updateContentFilter:completionHandler:", v4, v0 + 144);
  return swift_continuation_await(v0 + 80);
}

/* ========================================================================
 * sub_100080BC4
 * EA: 0x100080bc4
 ======================================================================== */

__int64 sub_100080BC4()
{
  __int64 v0; // x22
  __int64 v1; // x19
  __int64 v2; // x21
  __int64 v3; // x0
  Swift::String v4; // x0
  void *object; // x23
  __int64 v6; // x0
  __int64 v7; // x24
  __int64 v8; // x0

  swift_willThrow();
  v1 = *(_QWORD *)(v0 + 248);
  objc_release(*(id *)(v0 + 240));
  v2 = type metadata accessor for VTLogger(0);
  _StringGuts.grow(_:)(39);
  swift_bridgeObjectRelease(0xE000000000000000LL);
  *(_QWORD *)(v0 + 144) = v1;
  swift_errorRetain(v1);
  v3 = sub_100004DF0(&unk_10033C9B0, &unk_10025B810);
  v4._countAndFlagsBits = String.init<A>(describing:)(v0 + 144, v3);
  object = v4._object;
  String.append(_:)(v4);
  v6 = swift_bridgeObjectRelease(object);
  v7 = static os_log_type_t.error.getter(v6);
  v8 = static os_log_type_t.error.getter(v7);
  sub_100091CD8(v7, v8, 0xD000000000000025LL, 0x8000000100272790LL, v2);
  swift_bridgeObjectRelease(0x8000000100272790LL);
  swift_errorRelease(v1);
  return (*(__int64 (**)(void))(v0 + 8))();
}

/* ========================================================================
 * sub_100080CCC
 * EA: 0x100080ccc
 ======================================================================== */

__int64 sub_100080CCC()
{
  __int64 v0; // x22
  __int64 v1; // x19
  __int64 v2; // x21
  __int64 v3; // x0
  Swift::String v4; // x0
  void *object; // x23
  __int64 v6; // x0
  __int64 v7; // x24
  __int64 v8; // x0

  swift_willThrow();
  v1 = *(_QWORD *)(v0 + 264);
  objc_release(*(id *)(v0 + 256));
  v2 = type metadata accessor for VTLogger(0);
  _StringGuts.grow(_:)(39);
  swift_bridgeObjectRelease(0xE000000000000000LL);
  *(_QWORD *)(v0 + 144) = v1;
  swift_errorRetain(v1);
  v3 = sub_100004DF0(&unk_10033C9B0, &unk_10025B810);
  v4._countAndFlagsBits = String.init<A>(describing:)(v0 + 144, v3);
  object = v4._object;
  String.append(_:)(v4);
  v6 = swift_bridgeObjectRelease(object);
  v7 = static os_log_type_t.error.getter(v6);
  v8 = static os_log_type_t.error.getter(v7);
  sub_100091CD8(v7, v8, 0xD000000000000025LL, 0x8000000100272790LL, v2);
  swift_bridgeObjectRelease(0x8000000100272790LL);
  swift_errorRelease(v1);
  return (*(__int64 (**)(void))(v0 + 8))();
}

/* ========================================================================
 * sub_100080DD4
 * EA: 0x100080dd4
 ======================================================================== */

id sub_100080DD4()
{
  char *v0; // x20
  char *v1; // x19
  __int64 v2; // x28
  char *v3; // x22
  __int64 v4; // x20
  char *v5; // x23
  char *v6; // x24
  __int64 v7; // x25
  __int64 v8; // x0
  __int64 v9; // x26
  __int64 v10; // x27
  __int64 v11; // x0
  __int64 v12; // x0
  objc_class *v13; // x0
  __int64 v15; // [xsp+0h] [xbp-80h] BYREF
  __int64 v16; // [xsp+8h] [xbp-78h]
  __int64 v17; // [xsp+10h] [xbp-70h]
  objc_super v18; // [xsp+18h] [xbp-68h] BYREF
  void *v19; // [xsp+28h] [xbp-58h] BYREF

  v1 = v0;
  v17 = type metadata accessor for OS_dispatch_queue.AutoreleaseFrequency(0);
  v2 = *(_QWORD *)(v17 - 8);
  v3 = (char *)&v15 - ((*(_QWORD *)(v2 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v4 = type metadata accessor for OS_dispatch_queue.Attributes(0);
  v5 = (char *)&v15 - ((*(_QWORD *)(*(_QWORD *)(v4 - 8) + 64LL) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v6 = (char *)&v15
     - ((*(_QWORD *)(*(_QWORD *)(type metadata accessor for DispatchQoS(0) - 8) + 64LL) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  *(_QWORD *)&v1[OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_stream] = 0;
  v16 = OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_videoSampleBufferQueue;
  v7 = sub_100004E40(0);
  v8 = static DispatchQoS.unspecified.getter(v7);
  v19 = &_swiftEmptyArrayStorage;
  v9 = sub_100082094(v8);
  v10 = sub_100004DF0(&unk_10033BA60, &unk_100259F90);
  v11 = sub_100004E84();
  dispatch thunk of SetAlgebra.init<A>(_:)(&v19, v10, v11, v4, v9);
  (*(void (__fastcall **)(char *, _QWORD, __int64))(v2 + 104))(
    v3,
    enum case for OS_dispatch_queue.AutoreleaseFrequency.inherit(_:),
    v17);
  v12 = OS_dispatch_queue.init(label:qos:attributes:autoreleaseFrequency:target:)(
          0xD000000000000033LL,
          0x80000001002727E0LL,
          v6,
          v5,
          v3,
          0);
  *(_QWORD *)&v1[v16] = v12;
  *(_QWORD *)&v1[OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_streamOutput] = 0;
  v13 = (objc_class *)type metadata accessor for CaptureEngine();
  v18.receiver = v1;
  v18.super_class = v13;
  return objc_msgSendSuper2(&v18, "init");
}

/* ========================================================================
 * sub_100081064
 * EA: 0x100081064
 ======================================================================== */

id sub_100081064()
{
  char *v0; // x20
  char *v1; // x19
  char *v2; // x21
  __int64 v3; // x22
  __int64 v4; // x24
  char *v5; // x20
  __int64 v6; // x23
  objc_class *v7; // x0
  objc_super v9; // [xsp+0h] [xbp-70h] BYREF
  _QWORD v10[3]; // [xsp+10h] [xbp-60h] BYREF
  char v11[24]; // [xsp+28h] [xbp-48h] BYREF

  v1 = v0;
  v2 = (char *)&v9
     - ((*(_QWORD *)(*(_QWORD *)(sub_100004DF0(&unk_10033DE48, &unk_10025DF08) - 8) + 64LL) + 15LL)
      & 0xFFFFFFFFFFFFFFF0LL);
  v3 = sub_100004DF0(&unk_10033DE40, &unk_10025DEF0);
  v4 = *(_QWORD *)(v3 - 8);
  v5 = (char *)&v9 - ((*(_QWORD *)(v4 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v6 = OBJC_IVAR____TtC11SpaceWalkerP33_13031BFAB07D68F2E0DC27BB266D257F25CaptureEngineStreamOutput_continuation;
  swift_beginAccess(
    &v1[OBJC_IVAR____TtC11SpaceWalkerP33_13031BFAB07D68F2E0DC27BB266D257F25CaptureEngineStreamOutput_continuation],
    v11,
    0,
    0);
  if ( !(*(unsigned int (__fastcall **)(char *, __int64, __int64))(v4 + 48))(&v1[v6], 1, v3) )
  {
    (*(void (__fastcall **)(char *, char *, __int64))(v4 + 16))(v5, &v1[v6], v3);
    v10[0] = 0;
    AsyncThrowingStream.Continuation.finish(throwing:)(v10, v3);
    (*(void (__fastcall **)(char *, __int64))(v4 + 8))(v5, v3);
  }
  (*(void (__fastcall **)(char *, __int64, __int64, __int64))(v4 + 56))(v2, 1, 1, v3);
  swift_beginAccess(&v1[v6], v10, 33, 0);
  sub_10008202C(v2, &v1[v6]);
  swift_endAccess(v10);
  v7 = (objc_class *)type metadata accessor for CaptureEngineStreamOutput(0);
  v9.receiver = v1;
  v9.super_class = v7;
  return objc_msgSendSuper2(&v9, "dealloc");
}

/* ========================================================================
 * -[_TtC11SpaceWalkerP33_13031BFAB07D68F2E0DC27BB266D257F25CaptureEngineStreamOutput stream:didOutputSampleBuffer:ofType:]
 * EA: 0x10008126c
 ======================================================================== */

void __cdecl -[CaptureEngineStreamOutput stream:didOutputSampleBuffer:ofType:](
        _TtC11SpaceWalkerP33_13031BFAB07D68F2E0DC27BB266D257F25CaptureEngineStreamOutput *self,
        SEL a2,
        id a3,
        opaqueCMSampleBuffer *a4,
        signed __int64 a5)
{
  id v8; // x22
  opaqueCMSampleBuffer *v9; // x23
  _TtC11SpaceWalkerP33_13031BFAB07D68F2E0DC27BB266D257F25CaptureEngineStreamOutput *v10; // x20

  v8 = objc_retain(a3);
  v9 = objc_retain(a4);
  v10 = objc_retain(self);
  sub_100081B38(v9, a5);
  objc_release(v8);
  objc_release(v9);
  objc_release(v10);
}

/* ========================================================================
 * -[_TtC11SpaceWalkerP33_13031BFAB07D68F2E0DC27BB266D257F25CaptureEngineStreamOutput stream:didStopWithError:]
 * EA: 0x1000812e4
 ======================================================================== */

void __cdecl -[CaptureEngineStreamOutput stream:didStopWithError:](
        _TtC11SpaceWalkerP33_13031BFAB07D68F2E0DC27BB266D257F25CaptureEngineStreamOutput *self,
        SEL a2,
        id a3,
        id a4)
{
  id v6; // x21
  id v7; // x19
  _TtC11SpaceWalkerP33_13031BFAB07D68F2E0DC27BB266D257F25CaptureEngineStreamOutput *v8; // x20

  v6 = objc_retain(a3);
  v7 = objc_retain(a4);
  v8 = objc_retain(self);
  sub_100081E70(v7);
  objc_release(v6);
  objc_release(v8);
  objc_release(v7);
}

/* ========================================================================
 * sub_100081378
 * EA: 0x100081378
 ======================================================================== */

__int64 __fastcall sub_100081378(__int64 a1)
{
  __int64 result; // x0
  unsigned __int64 v3; // x1
  __int64 v4; // [xsp+8h] [xbp-18h] BYREF

  result = sub_1000813E4(319);
  if ( v3 <= 0x3F )
  {
    v4 = *(_QWORD *)(result - 8) + 64LL;
    result = swift_updateClassMetadata2(a1, 256, 1, &v4, a1 + 80);
    if ( !result )
      return 0;
  }
  return result;
}

/* ========================================================================
 * sub_1000813E4
 * EA: 0x1000813e4
 ======================================================================== */

void __fastcall sub_1000813E4(__int64 a1)
{
  __int64 v2; // x0
  unsigned __int64 v3; // x0
  __int64 v4; // x1

  if ( !qword_10033DE38 )
  {
    v2 = sub_100004ED4(&unk_10033DE40, &unk_10025DEF0);
    v3 = type metadata accessor for Optional(a1, v2);
    if ( !v4 )
      atomic_store(v3, (unsigned __int64 *)&qword_10033DE38);
  }
}

/* ========================================================================
 * $s11SpaceWalker13CapturedFrameVwca
 * EA: 0x1000814b0
 ======================================================================== */

__int64 __fastcall assignWithCopy for CapturedFrame(__int64 a1, __int64 a2)
{
  void *v4; // x21
  void *v5; // x0
  id v6; // x0
  void *v7; // x21
  void *v8; // x0
  id v9; // x0

  v4 = *(void **)a1;
  v5 = *(void **)a2;
  *(_QWORD *)a1 = *(_QWORD *)a2;
  v6 = objc_retain(v5);
  objc_release(v4);
  v7 = *(void **)(a1 + 8);
  v8 = *(void **)(a2 + 8);
  *(_QWORD *)(a1 + 8) = v8;
  v9 = objc_retain(v8);
  objc_release(v7);
  *(_QWORD *)(a1 + 16) = *(_QWORD *)(a2 + 16);
  *(_QWORD *)(a1 + 24) = *(_QWORD *)(a2 + 24);
  *(_QWORD *)(a1 + 32) = *(_QWORD *)(a2 + 32);
  *(_QWORD *)(a1 + 40) = *(_QWORD *)(a2 + 40);
  *(_QWORD *)(a1 + 48) = *(_QWORD *)(a2 + 48);
  *(_QWORD *)(a1 + 56) = *(_QWORD *)(a2 + 56);
  return a1;
}

/* ========================================================================
 * $s11SpaceWalker13CapturedFrameVwst
 * EA: 0x1000815e4
 ======================================================================== */

__int64 __fastcall storeEnumTagSinglePayload for CapturedFrame(__int64 result, unsigned int a2, unsigned int a3)
{
  if ( a2 > 0x7FFFFFFE )
  {
    *(_QWORD *)(result + 56) = 0;
    *(_OWORD *)(result + 40) = 0u;
    *(_OWORD *)(result + 24) = 0u;
    *(_OWORD *)(result + 8) = 0u;
    *(_QWORD *)result = a2 - 0x7FFFFFFF;
    if ( a3 >= 0x7FFFFFFF )
      *(_BYTE *)(result + 64) = 1;
  }
  else
  {
    if ( a3 >= 0x7FFFFFFF )
      *(_BYTE *)(result + 64) = 0;
    if ( a2 )
      *(_QWORD *)result = a2;
  }
  return result;
}

/* ========================================================================
 * sub_1000816A8
 * EA: 0x1000816a8
 ======================================================================== */

void __usercall sub_1000816A8(opaqueCMSampleBuffer *a1@<X0>, __int64 a2@<X8>)
{
  const __CFArray *v3; // x0
  __int64 v4; // x1
  __int128 v5; // q0
  const __CFArray *v6; // x20
  __int64 v7; // x0
  __int64 v8; // x0
  __int64 v9; // x22
  __int64 v10; // x0
  __int64 v11; // x20
  __int64 v12; // x22
  __int64 v13; // x0
  char v14; // w1
  __CVBuffer *v15; // x0
  __CVBuffer *v16; // x21
  __CVBuffer *v17; // x23
  IOSurfaceRef IOSurface; // x0
  __int64 v19; // x26
  __IOSurface *v20; // x0
  __IOSurface *v21; // x24
  __IOSurface *v22; // x24
  __int64 v23; // x0
  char v24; // w1
  __int64 v25; // x0
  __int64 v26; // x0
  char v27; // w1
  __int64 v28; // x0
  char v29; // w1
  __int64 v30; // x8
  __int64 v31; // x9
  __int128 v32; // q1
  __CVBuffer *v33; // x0
  __int64 v34; // x0
  __CVBuffer *v35; // x0
  __int64 v36; // [xsp+8h] [xbp-F8h]
  __int128 v37; // [xsp+10h] [xbp-F0h]
  __int128 v38; // [xsp+20h] [xbp-E0h]
  __int64 v39; // [xsp+38h] [xbp-C8h] BYREF
  __int128 v40; // [xsp+40h] [xbp-C0h] BYREF
  __int128 v41; // [xsp+50h] [xbp-B0h]
  _QWORD v42[4]; // [xsp+60h] [xbp-A0h] BYREF
  _OWORD v43[2]; // [xsp+80h] [xbp-80h] BYREF
  char v44; // [xsp+A0h] [xbp-60h]

  v3 = objc_retainAutoreleasedReturnValue(CMSampleBufferGetSampleAttachmentsArray(a1, 0));
  v5 = 0u;
  if ( v3 )
  {
    v6 = v3;
    v7 = objc_opt_self(&OBJC_CLASS___NSArray, v4);
    v8 = swift_dynamicCastObjCClass(v6, v7);
    if ( !v8 )
    {
      v33 = v6;
LABEL_22:
      objc_release(v33);
      goto LABEL_32;
    }
    v9 = v8;
    *(_QWORD *)&v43[0] = 0;
    v10 = sub_100004DF0(&unk_10033DE68, &unk_10025DF28);
    static Array._conditionallyBridgeFromObjectiveC(_:result:)(v9, v43, v10);
    objc_release(v6);
    v11 = *(_QWORD *)&v43[0];
    if ( !*(_QWORD *)&v43[0] )
    {
LABEL_32:
      v16 = nullptr;
      goto LABEL_33;
    }
    if ( !*(_QWORD *)(*(_QWORD *)&v43[0] + 16LL) )
    {
      v34 = *(_QWORD *)&v43[0];
LABEL_31:
      swift_bridgeObjectRelease(v34);
      goto LABEL_32;
    }
    v12 = *(_QWORD *)(*(_QWORD *)&v43[0] + 32LL);
    swift_bridgeObjectRetain(v12);
    swift_bridgeObjectRelease(v11);
    if ( *(_QWORD *)(v12 + 16) )
    {
      swift_bridgeObjectRetain(v12);
      v13 = sub_10010BC18(SCStreamFrameInfoStatus);
      if ( (v14 & 1) == 0 )
      {
LABEL_29:
        swift_bridgeObjectRelease(v12);
        goto LABEL_30;
      }
      sub_10002CA48(*(_QWORD *)(v12 + 56) + 32 * v13, v43);
      swift_bridgeObjectRelease(v12);
      if ( (swift_dynamicCast(v42, v43, (char *)&type metadata for Any + 8, &type metadata for Int, 6) & 1) == 0
        || v42[0] )
      {
        goto LABEL_30;
      }
      v15 = (__CVBuffer *)CMSampleBufferRef.imageBuffer.getter();
      v16 = v15;
      if ( !v15 )
      {
        swift_bridgeObjectRelease(v12);
LABEL_33:
        v30 = 0;
        v31 = 0;
        v19 = 1;
        v32 = 0u;
        v5 = 0u;
        goto LABEL_34;
      }
      CVPixelBufferGetPixelFormatType(v15);
      v17 = objc_retain(v16);
      IOSurface = CVPixelBufferGetIOSurface(v17);
      if ( !IOSurface )
      {
        swift_bridgeObjectRelease(v12);
        v35 = v17;
LABEL_26:
        objc_release(v35);
        v33 = v17;
        goto LABEL_22;
      }
      v19 = (__int64)IOSurface;
      v20 = objc_retain(IOSurface);
      if ( *(_QWORD *)(v12 + 16) )
      {
        v21 = v20;
        swift_bridgeObjectRetain(v12);
        v22 = objc_retain(v21);
        v23 = sub_10010BC18(SCStreamFrameInfoContentRect);
        if ( (v24 & 1) != 0 )
        {
          sub_10002CA48(*(_QWORD *)(v12 + 56) + 32 * v23, v43);
          swift_bridgeObjectRelease(v12);
          sub_100012AC8(v43, v42);
          sub_10002CA48(v42, &v40);
          v25 = type metadata accessor for CFDictionary(0);
          swift_dynamicCast(&v39, &v40, (char *)&type metadata for Any + 8, v25, 7);
          CGRect.init(dictionaryRepresentation:)(v43, v39);
          if ( (v44 & 1) == 0 && *(_QWORD *)(v12 + 16) )
          {
            v37 = v43[0];
            v38 = v43[1];
            swift_bridgeObjectRetain(v12);
            v26 = sub_10010BC18(SCStreamFrameInfoContentScale);
            if ( (v27 & 1) == 0 )
            {
              objc_release(v22);
              objc_release(v17);
              objc_release(v17);
              swift_bridgeObjectRelease(v12);
              goto LABEL_36;
            }
            sub_10002CA48(*(_QWORD *)(v12 + 56) + 32 * v26, &v40);
            swift_bridgeObjectRelease(v12);
            if ( (swift_dynamicCast(&v39, &v40, (char *)&type metadata for Any + 8, &type metadata for CGFloat, 6) & 1) != 0 )
            {
              v36 = v39;
              if ( *(_QWORD *)(v12 + 16) )
              {
                swift_bridgeObjectRetain(v12);
                v28 = sub_10010BC18(SCStreamFrameInfoScaleFactor);
                if ( (v29 & 1) != 0 )
                {
                  sub_10002CA48(*(_QWORD *)(v12 + 56) + 32 * v28, &v40);
                  objc_release(v17);
                  swift_bridgeObjectRelease(v12);
LABEL_40:
                  swift_bridgeObjectRelease(v12);
                  sub_10000C7E4(v42);
                  objc_release(v22);
                  if ( !*((_QWORD *)&v41 + 1) )
                  {
                    objc_release(v22);
                    objc_release(v17);
                    sub_10000C910(&v40, &unk_10033C280, &unk_10025B4F0);
                    goto LABEL_32;
                  }
                  if ( (swift_dynamicCast(&v39, &v40, (char *)&type metadata for Any + 8, &type metadata for CGFloat, 6)
                      & 1) != 0 )
                  {
                    v31 = v39;
                    v5 = v37;
                    v32 = v38;
                    v30 = v36;
                    goto LABEL_34;
                  }
                  v35 = v22;
                  goto LABEL_26;
                }
                swift_bridgeObjectRelease(v12);
              }
              v40 = 0u;
              v41 = 0u;
              objc_release(v17);
              goto LABEL_40;
            }
          }
          objc_release(v22);
          objc_release(v17);
          objc_release(v17);
LABEL_36:
          swift_bridgeObjectRelease(v12);
          sub_10000C7E4(v42);
          v33 = v22;
          goto LABEL_22;
        }
        objc_release(v22);
        objc_release(v22);
        objc_release(v17);
        objc_release(v17);
        goto LABEL_29;
      }
      objc_release(v20);
      objc_release(v17);
      objc_release(v17);
    }
LABEL_30:
    v34 = v12;
    goto LABEL_31;
  }
  v16 = nullptr;
  v30 = 0;
  v31 = 0;
  v19 = 1;
  v32 = 0u;
LABEL_34:
  *(_QWORD *)a2 = v19;
  *(_QWORD *)(a2 + 8) = v16;
  *(_OWORD *)(a2 + 16) = v5;
  *(_OWORD *)(a2 + 32) = v32;
  *(_QWORD *)(a2 + 48) = v30;
  *(_QWORD *)(a2 + 56) = v31;
}

/* ========================================================================
 * sub_1000876E4
 * EA: 0x1000876e4
 ======================================================================== */

id sub_1000876E4()
{
  _BYTE *v0; // x20
  id v1; // x19
  __int64 v2; // x1
  void *v3; // x20
  id v4; // x22
  id v5; // x19
  NSString v6; // x20
  __int64 v7; // x1
  id v8; // x21
  __int64 v9; // x0
  void *v10; // x23
  __int64 v11; // x24
  id v12; // x22
  id v13; // x24
  _QWORD v15[5]; // [xsp+0h] [xbp-70h] BYREF
  __int64 v16; // [xsp+28h] [xbp-48h]
  objc_super v17; // [xsp+30h] [xbp-40h] BYREF

  v0[OBJC_IVAR____TtC11SpaceWalker12VTIMUManager_targetFrequency] = 4;
  *(_QWORD *)&v0[OBJC_IVAR____TtC11SpaceWalker12VTIMUManager_retryInterval] = 0x3FF0000000000000LL;
  v0[OBJC_IVAR____TtC11SpaceWalker12VTIMUManager_state] = 0;
  v0[OBJC_IVAR____TtC11SpaceWalker12VTIMUManager_isR6OpeningIMU] = 2;
  v17.receiver = v0;
  v17.super_class = (Class)type metadata accessor for VTIMUManager();
  v1 = objc_msgSendSuper2(&v17, "init");
  v3 = (void *)objc_opt_self(&OBJC_CLASS___NSNotificationCenter, v2);
  v4 = objc_retain(v1);
  v5 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "defaultCenter"));
  v6 = String._bridgeToObjectiveC()();
  v8 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSOperationQueue, v7), "mainQueue"));
  v9 = swift_allocObject(&unk_1002E1BE8, 24, 7);
  *(_QWORD *)(v9 + 16) = v4;
  v15[4] = sub_1000883A0;
  v16 = v9;
  v15[0] = _NSConcreteStackBlock;
  v15[1] = 1107296256;
  v15[2] = sub_1000CDBC0;
  v15[3] = &unk_1002E1C00;
  v10 = _Block_copy(v15);
  v11 = v16;
  v12 = objc_retain(v4);
  swift_release(v11);
  v13 = objc_retainAutoreleasedReturnValue(objc_msgSend(v5, "addObserverForName:object:queue:usingBlock:", v6, 0, v8, v10));
  _Block_release(v10);
  objc_release(v12);
  swift_unknownObjectRelease(v13);
  objc_release(v5);
  objc_release(v6);
  objc_release(v8);
  return v12;
}

/* ========================================================================
 * sub_1000AD0EC
 * EA: 0x1000ad0ec
 ======================================================================== */

__int64 __fastcall sub_1000AD0EC(char a1)
{
  __int64 v2; // x19
  __int64 v3; // x27
  char *v4; // x21
  __int64 v5; // x22
  __int64 v6; // x28
  char *v7; // x23
  void *v8; // x24
  __int64 v9; // x0
  void *v10; // x25
  __int64 v11; // x0
  __int64 v12; // x0
  __int64 v13; // x26
  __int64 v14; // x20
  __int64 v15; // x0
  _QWORD aBlock[5]; // [xsp+0h] [xbp-80h] BYREF
  __int64 v18; // [xsp+28h] [xbp-58h]

  v2 = type metadata accessor for DispatchWorkItemFlags(0);
  v3 = *(_QWORD *)(v2 - 8);
  v4 = (char *)aBlock - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for DispatchQoS(0);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = (char *)aBlock - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  sub_100004E40(0);
  v8 = (void *)static OS_dispatch_queue.main.getter();
  v9 = swift_allocObject(&unk_1002E3960, 17, 7);
  *(_BYTE *)(v9 + 16) = a1;
  aBlock[4] = sub_1000AD4DC;
  v18 = v9;
  aBlock[0] = _NSConcreteStackBlock;
  aBlock[1] = 1107296256;
  aBlock[2] = sub_10007A08C;
  aBlock[3] = &unk_1002E3978;
  v10 = _Block_copy(aBlock);
  v11 = swift_release(v18);
  v12 = static DispatchQoS.unspecified.getter(v11);
  aBlock[0] = &_swiftEmptyArrayStorage;
  v13 = sub_10000C824(v12);
  v14 = sub_100004DF0(&unk_10033BE50, &unk_10025B090);
  v15 = sub_1000147C4();
  dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v14, v15, v2, v13);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v7, v4, v10);
  _Block_release(v10);
  objc_release(v8);
  (*(void (__fastcall **)(char *, __int64))(v3 + 8))(v4, v2);
  return (*(__int64 (__fastcall **)(char *, __int64))(v6 + 8))(v7, v5);
}

/* ========================================================================
 * sub_1000AD2B8
 * EA: 0x1000ad2b8
 ======================================================================== */

__int64 __fastcall sub_1000AD2B8(char a1)
{
  __int64 v2; // x0
  __int64 inited; // x20
  __int64 v4; // x22
  __int64 v5; // x1
  id v6; // x20
  NSString v7; // x21
  __int64 v8; // x23
  Class isa; // x22
  __int64 v10; // x21
  Swift::String v11; // x0
  Swift::String v12; // x0
  void *object; // x19
  __int64 v14; // x0
  __int64 v15; // x22
  __int64 v16; // x0
  _BYTE v18[56]; // [xsp+18h] [xbp-68h] BYREF

  v2 = sub_100004DF0(&unk_10033C198, &unk_10025B358);
  inited = swift_initStackObject(v2, v18);
  *(_OWORD *)(inited + 16) = xmmword_10025B110;
  *(_QWORD *)(inited + 32) = 0x656E746867697262LL;
  *(_QWORD *)(inited + 40) = 0xEA00000000007373LL;
  *(_BYTE *)(inited + 48) = a1;
  v4 = sub_100012C94();
  swift_setDeallocating(inited);
  sub_100014814(inited + 32);
  v6 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter, v5), "defaultCenter"));
  v7 = String._bridgeToObjectiveC()();
  v8 = sub_10000FB94(v4);
  swift_bridgeObjectRelease(v4);
  isa = Dictionary._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v8);
  objc_msgSend(v6, "postNotificationName:object:userInfo:", v7, 0, isa);
  objc_release(v6);
  objc_release(v7);
  objc_release(isa);
  v10 = type metadata accessor for VTLogger(0);
  _StringGuts.grow(_:)(51);
  v11._countAndFlagsBits = 0xD000000000000031LL;
  v11._object = (void *)0x8000000100275900LL;
  String.append(_:)(v11);
  v12._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                             &type metadata for UInt8,
                             &protocol witness table for UInt8);
  object = v12._object;
  String.append(_:)(v12);
  v14 = swift_bridgeObjectRelease(object);
  v15 = static os_log_type_t.info.getter(v14);
  v16 = static os_log_type_t.info.getter(v15);
  sub_100091CD8(v15, v16, 0, 0xE000000000000000LL, v10);
  return swift_bridgeObjectRelease(0xE000000000000000LL);
}

/* ========================================================================
 * sub_1000AD6F8
 * EA: 0x1000ad6f8
 ======================================================================== */

void __fastcall sub_1000AD6F8(__int64 a1, unsigned __int64 a2)
{
  _QWORD *v2; // x20
  void *v3; // x23
  void *v5; // x19
  id v6; // x21
  _QWORD *v7; // x24
  id v8; // x21
  char isUniquelyReferenced_nonNull_native; // w0
  void **v10; // x26
  void *v11; // x25
  unsigned __int64 v12; // x8
  void *v13; // x23
  void *v14; // x22
  void *v15; // x25
  __int64 v16; // x26
  __int64 v17; // x0
  __int64 v18; // x24
  __int64 v19; // x27
  id v20; // x20
  id v21; // x22
  id v22; // x23
  id v23; // x25
  id v24; // x20
  id v25; // x22
  __int64 v26; // x0
  __int64 v27; // x25
  __int64 v28; // x0
  __int64 v29; // x24
  __int64 v30; // x26
  id v31; // x20
  id v32; // x22
  __int64 v33; // x0
  _BYTE v34[24]; // [xsp+8h] [xbp-68h] BYREF

  v3 = *(void **)(a1 + 8);
  if ( v3 )
  {
    v5 = (void *)v2[3];
    v6 = objc_retain(v3);
    objc_msgSend(v5, "lock");
    swift_beginAccess(v2 + 2, v34, 33, 0);
    v7 = (_QWORD *)v2[2];
    v8 = objc_retain(v6);
    isUniquelyReferenced_nonNull_native = swift_isUniquelyReferenced_nonNull_native(v7);
    v2[2] = v7;
    if ( (isUniquelyReferenced_nonNull_native & 1) != 0 )
    {
      if ( (a2 & 0x8000000000000000LL) == 0 )
        goto LABEL_4;
    }
    else
    {
      v7 = (_QWORD *)sub_100011D18(v7);
      v2[2] = v7;
      if ( (a2 & 0x8000000000000000LL) == 0 )
      {
LABEL_4:
        if ( v7[2] > a2 )
        {
          v10 = (void **)(v7 + 4);
          v11 = (void *)v7[a2 + 4];
          v7[a2 + 4] = v3;
          v2[2] = v7;
          swift_endAccess(v34);
          objc_release(v11);
          v12 = v7[2];
          if ( v12 )
          {
            v13 = *v10;
            if ( !*v10 )
              goto LABEL_14;
            if ( v12 != 1 )
            {
              v14 = (void *)v7[5];
              if ( v14 )
              {
                if ( v12 >= 3 )
                {
                  v15 = (void *)v7[6];
                  if ( v15 )
                  {
                    v16 = v2[4];
                    v17 = sub_100004DF0(&unk_10033C230, &unk_10025B470);
                    v18 = swift_allocObject(v17, 56, 7);
                    *(_OWORD *)(v18 + 16) = xmmword_10025B3A0;
                    *(_QWORD *)(v18 + 32) = v13;
                    *(_QWORD *)(v18 + 40) = v14;
                    *(_QWORD *)(v18 + 48) = v15;
                    v19 = OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_lock;
                    swift_beginAccess(v16 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_lock, v34, 33, 0);
                    v20 = objc_retain(v13);
                    v21 = objc_retain(v14);
                    v22 = objc_retain(v15);
                    v23 = objc_retain(v20);
                    v24 = objc_retain(v21);
                    v25 = objc_retain(v22);
                    os_unfair_lock_lock((os_unfair_lock_t)(v16 + v19));
                    swift_endAccess(v34);
                    v26 = *(_QWORD *)(v16 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_latestPixelBuffers);
                    *(_QWORD *)(v16 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_latestPixelBuffers) = v18;
                    swift_bridgeObjectRelease(v26);
                    swift_beginAccess(v16 + v19, v34, 33, 0);
                    os_unfair_lock_unlock((os_unfair_lock_t)(v16 + v19));
                    swift_endAccess(v34);
                    objc_release(v23);
                  }
                  else
                  {
                    v27 = v2[4];
                    v28 = sub_100004DF0(&unk_10033C230, &unk_10025B470);
                    v29 = swift_allocObject(v28, 48, 7);
                    *(_OWORD *)(v29 + 16) = xmmword_10025B3C0;
                    *(_QWORD *)(v29 + 32) = v13;
                    *(_QWORD *)(v29 + 40) = v14;
                    v30 = OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_lock;
                    swift_beginAccess(v27 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_lock, v34, 33, 0);
                    v31 = objc_retain(v13);
                    v32 = objc_retain(v14);
                    v24 = objc_retain(v31);
                    v25 = objc_retain(v32);
                    os_unfair_lock_lock((os_unfair_lock_t)(v27 + v30));
                    swift_endAccess(v34);
                    v33 = *(_QWORD *)(v27 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_latestPixelBuffers);
                    *(_QWORD *)(v27 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_latestPixelBuffers) = v29;
                    swift_bridgeObjectRelease(v33);
                    swift_beginAccess(v27 + v30, v34, 33, 0);
                    os_unfair_lock_unlock((os_unfair_lock_t)(v27 + v30));
                    swift_endAccess(v34);
                  }
                  objc_release(v24);
                  objc_release(v25);
                  goto LABEL_14;
                }
LABEL_21:
                __break(1u);
                return;
              }
LABEL_14:
              objc_msgSend(v5, "unlock");
              objc_release(v8);
              return;
            }
LABEL_20:
            __break(1u);
            goto LABEL_21;
          }
LABEL_19:
          __break(1u);
          goto LABEL_20;
        }
LABEL_18:
        __break(1u);
        goto LABEL_19;
      }
    }
    __break(1u);
    goto LABEL_18;
  }
}

/* ========================================================================
 * sub_1000ADB58
 * EA: 0x1000adb58
 ======================================================================== */

__int64 __fastcall sub_1000ADB58(void *a1, double a2, double a3)
{
  char *v3; // x20
  objc_class *ObjectType; // x26
  __int64 v8; // x1
  double v9; // d0
  id v10; // x0
  double *v11; // x8
  id v12; // x0
  __int64 v13; // x22
  id v14; // x0
  __int64 v15; // x28
  CVMetalTextureCacheRef v16; // x0
  __CVMetalTextureCache *v17; // x0
  id v18; // x0
  void *v19; // x22
  id v20; // x23
  NSString v21; // x24
  id v22; // x25
  NSString v23; // x24
  id v24; // x25
  id v25; // x25
  id v26; // x24
  id v27; // x8
  id v28; // x0
  id v29; // x0
  char *v30; // x21
  __int64 v31; // x24
  char *v32; // x20
  __CVDisplayLink *v33; // x0
  __CVDisplayLink *v34; // x0
  __int64 result; // x0
  id v36; // x19
  id v37; // x20
  __int64 v38; // x19
  objc_super v39; // [xsp+0h] [xbp-A0h] BYREF
  id v40[3]; // [xsp+18h] [xbp-88h] BYREF
  CVMetalTextureCacheRef cacheOut; // [xsp+30h] [xbp-70h] BYREF

  ObjectType = (objc_class *)swift_getObjectType(v3);
  *(_DWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_spacing] = 1092616192;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_latestPixelBuffers] = &_swiftEmptyArrayStorage;
  *(_DWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_lock] = 0;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_displayLink] = 0;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_device] = a1;
  v10 = objc_msgSend((id)swift_unknownObjectRetain(a1, v8, v9), "newCommandQueue");
  if ( !v10 )
  {
    __break(1u);
    goto LABEL_10;
  }
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_commandQueue] = v10;
  v11 = (double *)&v3[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_drawableSize];
  *v11 = a2;
  v11[1] = a3;
  v12 = objc_msgSend(
          objc_allocWithZone((Class)type metadata accessor for MetalContainerView(0)),
          "initWithFrame:",
          0.0,
          0.0,
          a2,
          a3);
  v13 = OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_containerView;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_containerView] = v12;
  v14 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CAMetalLayer), "init");
  v15 = OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_metalLayer;
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_metalLayer] = v14;
  objc_msgSend(v14, "setDevice:", a1);
  objc_msgSend(*(id *)&v3[v15], "setPixelFormat:", 80);
  objc_msgSend(*(id *)&v3[v15], "setFramebufferOnly:", 0);
  objc_msgSend(*(id *)&v3[v15], "setDrawableSize:", a2, a3);
  objc_msgSend(*(id *)&v3[v15], "setContentsScale:", 1.0);
  objc_msgSend(*(id *)&v3[v13], "setWantsLayer:", 1);
  objc_msgSend(*(id *)&v3[v13], "setLayer:", *(_QWORD *)&v3[v15]);
  cacheOut = nullptr;
  CVMetalTextureCacheCreate(nullptr, nullptr, a1, nullptr, &cacheOut);
  v16 = cacheOut;
  if ( !cacheOut )
  {
LABEL_10:
    __break(1u);
    goto LABEL_11;
  }
  *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_textureCache] = cacheOut;
  v17 = objc_retain(v16);
  v18 = objc_msgSend(a1, "newDefaultLibrary");
  if ( !v18 )
  {
LABEL_11:
    __break(1u);
    goto LABEL_12;
  }
  v19 = v18;
  v20 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___MTLRenderPipelineDescriptor), "init");
  v21 = String._bridgeToObjectiveC()();
  v22 = objc_msgSend(v19, "newFunctionWithName:", v21);
  objc_release(v21);
  objc_msgSend(v20, "setVertexFunction:", v22);
  swift_unknownObjectRelease(v22);
  v23 = String._bridgeToObjectiveC()();
  v24 = objc_msgSend(v19, "newFunctionWithName:", v23);
  objc_release(v23);
  objc_msgSend(v20, "setFragmentFunction:", v24);
  swift_unknownObjectRelease(v24);
  v25 = objc_retainAutoreleasedReturnValue(objc_msgSend(v20, "colorAttachments"));
  v26 = objc_retainAutoreleasedReturnValue(objc_msgSend(v25, "objectAtIndexedSubscript:", 0));
  objc_release(v25);
  if ( !v26 )
  {
LABEL_12:
    __break(1u);
    goto LABEL_13;
  }
  objc_msgSend(v26, "setPixelFormat:", objc_msgSend(*(id *)&v3[v15], "pixelFormat"));
  objc_release(v26);
  v40[0] = nullptr;
  v27 = objc_msgSend(a1, "newRenderPipelineStateWithDescriptor:error:", v20, v40);
  v28 = v40[0];
  if ( v27 )
  {
    *(_QWORD *)&v3[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_pipeline] = v27;
    v39.receiver = v3;
    v39.super_class = ObjectType;
    v29 = objc_retain(v28);
    v30 = (char *)objc_msgSendSuper2(&v39, "init");
    v31 = OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_displayLink;
    swift_beginAccess(&v30[OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_displayLink], v40, 33, 0);
    v32 = objc_retain(v30);
    CVDisplayLinkCreateWithActiveCGDisplays((CVDisplayLinkRef *)&v30[v31]);
    swift_endAccess(v40);
    v33 = *(__CVDisplayLink **)&v30[v31];
    if ( v33 )
    {
      CVDisplayLinkSetOutputCallback(v33, (CVDisplayLinkOutputCallback)sub_1000AE704, v32);
      v34 = *(__CVDisplayLink **)&v30[v31];
      if ( v34 )
      {
        CVDisplayLinkStart(v34);
        swift_unknownObjectRelease(v19);
        objc_release(v20);
        swift_unknownObjectRelease(a1);
        objc_release(v32);
        objc_release(cacheOut);
        return (__int64)v32;
      }
LABEL_14:
      __break(1u);
    }
LABEL_13:
    __break(1u);
    goto LABEL_14;
  }
  v36 = v40[0];
  v37 = objc_retain(v40[0]);
  v38 = _convertNSErrorToError(_:)(v36);
  objc_release(v37);
  swift_willThrow();
  result = swift_unexpectedError(v38, "SpaceWalker/WideStripMTKRenderer.swift", 38, 1, 125);
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_1000AE0F0
 * EA: 0x1000ae0f0
 ======================================================================== */

void sub_1000AE0F0()
{
  __int64 v0; // x20
  id v1; // x0
  void *v2; // x23
  id v3; // x0
  void *v4; // x26
  __int64 v5; // x24
  id v6; // x25
  id v7; // x19
  id v8; // x21
  id v9; // x19
  id v10; // x21
  id v11; // x19
  id v12; // x19
  id v13; // x21
  id v14; // x0
  id v15; // x20
  __int64 v16; // x22
  double v17; // d8
  double v18; // d9
  __int64 v19; // x19
  __int64 v20; // x24
  __int64 i; // x25
  unsigned __int64 v22; // x26
  float v23; // s11
  __CVMetalTextureCache *v24; // x27
  unsigned __int64 v25; // x23
  __CVBuffer *v26; // x0
  __CVBuffer *v27; // x0
  __CVBuffer *v28; // x28
  unsigned __int64 v29; // x20
  size_t Width; // x19
  size_t Height; // x0
  id v32; // x21
  __CVMetalTextureCache *v33; // x24
  unsigned __int64 v34; // x27
  __int64 v35; // x23
  float v36; // s10
  float v37; // s10
  __int64 v38; // x19
  id v39; // x0
  __int64 v40; // x25
  id v41; // x22
  __int64 v42; // x0
  id v43; // [xsp+10h] [xbp-D0h]
  id v44; // [xsp+18h] [xbp-C8h]
  void *v45; // [xsp+20h] [xbp-C0h]
  __int64 v46; // [xsp+28h] [xbp-B8h]
  __int64 v47; // [xsp+30h] [xbp-B0h]
  __int64 v48; // [xsp+38h] [xbp-A8h]
  __int64 v49; // [xsp+40h] [xbp-A0h]
  id v50; // [xsp+48h] [xbp-98h]
  id textureOut[3]; // [xsp+50h] [xbp-90h] BYREF

  v1 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(v0
                                                             + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_metalLayer), "nextDrawable"));
  if ( !v1 )
    return;
  v2 = v1;
  v3 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(v0
                                                             + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_commandQueue), "commandBuffer"));
  if ( !v3 )
  {
    swift_unknownObjectRelease(v2);
    return;
  }
  v4 = v3;
  v5 = v0;
  v6 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___MTLRenderPassDescriptor), "init");
  v7 = objc_retainAutoreleasedReturnValue(objc_msgSend(v6, "colorAttachments"));
  v8 = objc_retainAutoreleasedReturnValue(objc_msgSend(v7, "objectAtIndexedSubscript:", 0));
  objc_release(v7);
  if ( !v8 )
  {
    __break(1u);
    goto LABEL_35;
  }
  v9 = objc_retainAutoreleasedReturnValue(objc_msgSend(v2, "texture"));
  objc_msgSend(v8, "setTexture:", v9);
  objc_release(v8);
  swift_unknownObjectRelease(v9);
  v10 = objc_retainAutoreleasedReturnValue(objc_msgSend(v6, "colorAttachments"));
  v11 = objc_retainAutoreleasedReturnValue(objc_msgSend(v10, "objectAtIndexedSubscript:", 0));
  objc_release(v10);
  if ( !v11 )
  {
LABEL_35:
    __break(1u);
    goto LABEL_36;
  }
  objc_msgSend(v11, "setLoadAction:", 2);
  objc_release(v11);
  v12 = objc_retainAutoreleasedReturnValue(objc_msgSend(v6, "colorAttachments"));
  v13 = objc_retainAutoreleasedReturnValue(objc_msgSend(v12, "objectAtIndexedSubscript:", 0));
  objc_release(v12);
  if ( !v13 )
  {
LABEL_36:
    __break(1u);
    return;
  }
  objc_msgSend(v13, "setStoreAction:", 1);
  objc_release(v13);
  v14 = objc_retainAutoreleasedReturnValue(objc_msgSend(v4, "renderCommandEncoderWithDescriptor:", v6));
  if ( v14 )
  {
    v15 = v14;
    v16 = v5;
    objc_msgSend(
      v14,
      "setRenderPipelineState:",
      *(_QWORD *)(v5 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_pipeline));
    v17 = *(double *)(v5 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_drawableSize);
    v18 = *(double *)(v5 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_drawableSize + 8);
    v19 = OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_lock;
    swift_beginAccess(v5 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_lock, textureOut, 33, 0);
    os_unfair_lock_lock((os_unfair_lock_t)(v5 + v19));
    v20 = *(_QWORD *)(v5 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_latestPixelBuffers);
    swift_bridgeObjectRetain(v20);
    os_unfair_lock_unlock((os_unfair_lock_t)(v16 + v19));
    swift_endAccess(textureOut);
    v44 = v6;
    v45 = v2;
    v43 = v4;
    v50 = v15;
    if ( (unsigned __int64)v20 >> 62 )
      goto LABEL_27;
    for ( i = *(_QWORD *)((v20 & 0xFFFFFFFFFFFFFF8LL) + 0x10); i; i = _CocoaArrayWrapper.endIndex.getter(v42) )
    {
      v22 = 0;
      *(float *)&v17 = v17;
      v23 = v18;
      v48 = OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_spacing;
      v49 = OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_device;
      v24 = *(__CVMetalTextureCache **)(v16 + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_textureCache);
      v25 = v20 & 0xC000000000000001LL;
      v46 = v20 & 0xFFFFFFFFFFFFFF8LL;
      v47 = v20;
      v18 = 0.0;
      while ( 1 )
      {
        if ( v25 )
        {
          v27 = (__CVBuffer *)sub_1000A0460(v22, v20);
        }
        else
        {
          if ( v22 >= *(_QWORD *)(v46 + 16) )
            goto LABEL_26;
          v27 = (__CVBuffer *)objc_retain(*(id *)(v20 + 8 * v22 + 32));
        }
        v28 = v27;
        v29 = v22 + 1;
        if ( __OFADD__(v22, 1) )
          break;
        textureOut[0] = nullptr;
        Width = CVPixelBufferGetWidth(v27);
        Height = CVPixelBufferGetHeight(v28);
        CVMetalTextureCacheCreateTextureFromImage(
          nullptr,
          v24,
          v28,
          nullptr,
          MTLPixelFormatBGRA8Unorm,
          Width,
          Height,
          0,
          (CVMetalTextureRef *)textureOut);
        if ( textureOut[0] )
        {
          v32 = objc_retainAutoreleasedReturnValue(CVMetalTextureGetTexture((CVMetalTextureRef)textureOut[0]));
          objc_release(textureOut[0]);
          if ( v32 )
          {
            v33 = v24;
            v34 = v25;
            v35 = i;
            v36 = (float)(__int64)CVPixelBufferGetWidth(v28);
            v37 = (float)(v23 / (float)(__int64)CVPixelBufferGetHeight(v28)) * v36;
            v38 = sub_1000AE8A0(*(float *)&v18, v37, *(float *)&v17);
            v39 = objc_msgSend(*(id *)(v16 + v49), "newBufferWithBytes:length:options:", v38 + 32, 96, 0);
            if ( !v39 )
              __break(1u);
            v40 = v16;
            v41 = v39;
            swift_bridgeObjectRelease(v38);
            objc_msgSend(v50, "setVertexBuffer:offset:atIndex:", v41, 0, 0);
            objc_msgSend(v50, "setFragmentTexture:atIndex:", v32, 0);
            objc_msgSend(v50, "drawPrimitives:vertexStart:vertexCount:", 3, 0, 6);
            objc_release(v28);
            swift_unknownObjectRelease(v32);
            swift_unknownObjectRelease(v41);
            *(float *)&v18 = *(float *)&v18 + (float)(v37 + *(float *)(v40 + v48));
            v16 = v40;
            i = v35;
            v25 = v34;
            v24 = v33;
            v20 = v47;
            goto LABEL_12;
          }
          v26 = v28;
        }
        else
        {
          objc_release(v28);
          v26 = (__CVBuffer *)textureOut[0];
        }
        objc_release(v26);
LABEL_12:
        ++v22;
        if ( v29 == i )
          goto LABEL_31;
      }
      __break(1u);
LABEL_26:
      __break(1u);
LABEL_27:
      if ( v20 < 0 )
        v42 = v20;
      else
        v42 = v20 & 0xFFFFFFFFFFFFFF8LL;
    }
LABEL_31:
    swift_bridgeObjectRelease(v20);
    objc_msgSend(v50, "endEncoding");
    objc_msgSend(v43, "presentDrawable:", v45);
    objc_msgSend(v43, "commit");
    swift_unknownObjectRelease(v43);
    swift_unknownObjectRelease(v45);
    swift_unknownObjectRelease(v50);
    objc_release(v44);
  }
  else
  {
    swift_unknownObjectRelease(v2);
    swift_unknownObjectRelease(v4);
    objc_release(v6);
  }
}

/* ========================================================================
 * sub_1000AE8A0
 * EA: 0x1000ae8a0
 ======================================================================== */

__int64 __fastcall sub_1000AE8A0(float a1, float a2, float a3)
{
  float v3; // s9
  float v4; // s10
  __int64 v5; // x0
  __int64 v6; // x19
  __int64 v7; // x20
  __int64 inited; // x21
  double v9; // d8
  __int64 v10; // x0
  double v11; // d0
  __int64 v12; // x21
  double v13; // d8
  __int64 v14; // x0
  double v15; // d0
  __int64 v16; // x21
  double v17; // d8
  __int64 v18; // x0
  double v19; // d0
  __int64 v20; // x21
  double v21; // d8
  __int64 v22; // x0
  double v23; // d0
  __int64 v24; // x21
  double v25; // d8
  __int64 v26; // x0
  double v27; // d0
  __int64 v28; // x21
  double v29; // d8
  __int64 v30; // x0
  double v31; // d0
  _BYTE v33[40]; // [xsp+10h] [xbp-140h] BYREF
  _BYTE v34[40]; // [xsp+38h] [xbp-118h] BYREF
  _BYTE v35[40]; // [xsp+60h] [xbp-F0h] BYREF
  _BYTE v36[40]; // [xsp+88h] [xbp-C8h] BYREF
  _BYTE v37[40]; // [xsp+B0h] [xbp-A0h] BYREF
  _BYTE v38[40]; // [xsp+D8h] [xbp-78h] BYREF

  v3 = (float)((float)(a1 / a3) + (float)(a1 / a3)) + -1.0;
  v4 = (float)((float)((float)(a1 + a2) / a3) + (float)((float)(a1 + a2) / a3)) + -1.0;
  v5 = sub_100004DF0(&unk_10033EEF0, &unk_10025F360);
  v6 = swift_allocObject(v5, 128, 7);
  *(_OWORD *)(v6 + 16) = xmmword_10025D1E0;
  v7 = sub_100004DF0(&unk_10033CBB0, &unk_10025C310);
  inited = swift_initStackObject(v7, v38);
  *(_OWORD *)(inited + 16) = xmmword_10025C0F0;
  *(float *)(inited + 32) = v3;
  *(_DWORD *)(inited + 36) = -1082130432;
  v9 = ((double (*)(void))sub_1000AE868)();
  swift_setDeallocating(inited);
  v10 = swift_initStaticObject(v7, &unk_10033B490);
  v11 = sub_1000AE868(v10);
  *(double *)(v6 + 32) = v9;
  *(double *)(v6 + 40) = v11;
  v12 = swift_initStackObject(v7, v37);
  *(_OWORD *)(v12 + 16) = xmmword_10025C0F0;
  *(float *)(v12 + 32) = v4;
  *(_DWORD *)(v12 + 36) = -1082130432;
  v13 = sub_1000AE868(v12);
  swift_setDeallocating(v12);
  v14 = swift_initStaticObject(v7, &unk_10033B4C0);
  v15 = sub_1000AE868(v14);
  *(double *)(v6 + 48) = v13;
  *(double *)(v6 + 56) = v15;
  v16 = swift_initStackObject(v7, v36);
  *(_OWORD *)(v16 + 16) = xmmword_10025C0F0;
  *(float *)(v16 + 32) = v3;
  *(_DWORD *)(v16 + 36) = 1065353216;
  v17 = sub_1000AE868(v16);
  swift_setDeallocating(v16);
  v18 = swift_initStaticObject(v7, &unk_10033B4F0);
  v19 = sub_1000AE868(v18);
  *(double *)(v6 + 64) = v17;
  *(double *)(v6 + 72) = v19;
  v20 = swift_initStackObject(v7, v35);
  *(_OWORD *)(v20 + 16) = xmmword_10025C0F0;
  *(float *)(v20 + 32) = v4;
  *(_DWORD *)(v20 + 36) = -1082130432;
  v21 = sub_1000AE868(v20);
  swift_setDeallocating(v20);
  v22 = swift_initStaticObject(v7, &unk_10033B520);
  v23 = sub_1000AE868(v22);
  *(double *)(v6 + 80) = v21;
  *(double *)(v6 + 88) = v23;
  v24 = swift_initStackObject(v7, v34);
  *(_OWORD *)(v24 + 16) = xmmword_10025C0F0;
  *(float *)(v24 + 32) = v4;
  *(_DWORD *)(v24 + 36) = 1065353216;
  v25 = sub_1000AE868(v24);
  swift_setDeallocating(v24);
  v26 = swift_initStaticObject(v7, &unk_10033B550);
  v27 = sub_1000AE868(v26);
  *(double *)(v6 + 96) = v25;
  *(double *)(v6 + 104) = v27;
  v28 = swift_initStackObject(v7, v33);
  *(_OWORD *)(v28 + 16) = xmmword_10025C0F0;
  *(float *)(v28 + 32) = v3;
  *(_DWORD *)(v28 + 36) = 1065353216;
  v29 = sub_1000AE868(v28);
  swift_setDeallocating(v28);
  v30 = swift_initStaticObject(v7, &unk_10033B580);
  v31 = sub_1000AE868(v30);
  *(double *)(v6 + 112) = v29;
  *(double *)(v6 + 120) = v31;
  return v6;
}

/* ========================================================================
 * sub_1000AEB1C
 * EA: 0x1000aeb1c
 ======================================================================== */

void __fastcall sub_1000AEB1C(__int64 a1)
{
  __int64 v2; // x19
  Swift::String v3; // x0
  Swift::String v4; // x0
  void *object; // x21
  Swift::String v6; // x0
  Swift::String v7; // x0
  void *v8; // x21
  Swift::String v9; // x0
  __int64 v10; // x0
  __int64 v11; // x22
  __int64 v12; // x0

  v2 = type metadata accessor for VTLogger(0);
  _StringGuts.grow(_:)(75);
  v3._object = (void *)0x8000000100275D20LL;
  v3._countAndFlagsBits = 0xD00000000000001ELL;
  String.append(_:)(v3);
  if ( *(_QWORD *)(a1 + 16) < 2u )
  {
    __break(1u);
  }
  else
  {
    v4._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                              &type metadata for UInt8,
                              &protocol witness table for UInt8);
    object = v4._object;
    String.append(_:)(v4);
    swift_bridgeObjectRelease(object);
    v6._countAndFlagsBits = 0x65646E6920746120LL;
    v6._object = (void *)0xEA00000000002078LL;
    String.append(_:)(v6);
    v7._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                              &type metadata for Int,
                              &protocol witness table for Int);
    v8 = v7._object;
    String.append(_:)(v7);
    swift_bridgeObjectRelease(v8);
    v9._countAndFlagsBits = 0xD00000000000001FLL;
    v9._object = (void *)0x8000000100275D40LL;
    String.append(_:)(v9);
    v11 = static os_log_type_t.error.getter(v10);
    v12 = static os_log_type_t.error.getter(v11);
    sub_100091CD8(v11, v12, 0, 0xE000000000000000LL, v2);
    swift_bridgeObjectRelease(0xE000000000000000LL);
  }
}

/* ========================================================================
 * sub_1000AEC80
 * EA: 0x1000aec80
 ======================================================================== */

void __fastcall sub_1000AEC80(__int64 a1)
{
  __int64 v2; // x19
  Swift::String v3; // x0
  Swift::String v4; // x0
  void *object; // x21
  Swift::String v6; // x0
  Swift::String v7; // x0
  void *v8; // x21
  Swift::String v9; // x0
  __int64 v10; // x0
  __int64 v11; // x22
  __int64 v12; // x0

  v2 = type metadata accessor for VTLogger(0);
  _StringGuts.grow(_:)(67);
  v3._countAndFlagsBits = 0xD000000000000020LL;
  v3._object = (void *)0x8000000100275CD0LL;
  String.append(_:)(v3);
  if ( *(_QWORD *)(a1 + 16) < 3u )
  {
    __break(1u);
  }
  else
  {
    v4._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                              &type metadata for UInt8,
                              &protocol witness table for UInt8);
    object = v4._object;
    String.append(_:)(v4);
    swift_bridgeObjectRelease(object);
    v6._countAndFlagsBits = 0x65646E6920746120LL;
    v6._object = (void *)0xEA00000000002078LL;
    String.append(_:)(v6);
    v7._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                              &type metadata for Int,
                              &protocol witness table for Int);
    v8 = v7._object;
    String.append(_:)(v7);
    swift_bridgeObjectRelease(v8);
    v9._object = (void *)0x8000000100275D00LL;
    v9._countAndFlagsBits = 0xD000000000000015LL;
    String.append(_:)(v9);
    v11 = static os_log_type_t.error.getter(v10);
    v12 = static os_log_type_t.error.getter(v11);
    sub_100091CD8(v11, v12, 0, 0xE000000000000000LL, v2);
    swift_bridgeObjectRelease(0xE000000000000000LL);
  }
}

/* ========================================================================
 * sub_1000AEDE4
 * EA: 0x1000aede4
 ======================================================================== */

void __fastcall sub_1000AEDE4(unsigned int a1)
{
  char v1; // w19
  unsigned int v2; // w24
  __int64 v3; // x21
  Swift::String v4; // x0
  Swift::String v5; // x0
  __int64 v6; // x0
  __int64 v7; // x20
  unsigned __int64 v8; // x22
  __int64 v9; // x23
  __int64 v10; // x0
  __int64 v11; // x0
  __int64 inited; // x20
  __int64 v13; // x19
  __int64 v14; // x1
  id v15; // x20
  NSString v16; // x21
  __int64 v17; // x22
  Class isa; // x19
  __int64 v19; // [xsp+0h] [xbp-80h] BYREF
  char v20; // [xsp+3Fh] [xbp-41h] BYREF
  __int64 v21; // [xsp+40h] [xbp-40h] BYREF
  unsigned __int64 v22; // [xsp+48h] [xbp-38h]

  v1 = a1;
  v2 = a1 >> 8;
  v3 = type metadata accessor for VTLogger(0);
  v21 = 0;
  v22 = 0xE000000000000000LL;
  _StringGuts.grow(_:)(64);
  v4._countAndFlagsBits = 0xD000000000000031LL;
  v4._object = (void *)0x8000000100275C60LL;
  String.append(_:)(v4);
  v20 = v1;
  _print_unlocked<A, B>(_:_:)(
    &v20,
    &v21,
    &type metadata for VTDisplayMode,
    &type metadata for DefaultStringInterpolation,
    &protocol witness table for DefaultStringInterpolation);
  v5._countAndFlagsBits = 0x646F4D666F64202CLL;
  v5._object = (void *)0xEB00000000203A65LL;
  String.append(_:)(v5);
  v20 = v2;
  v6 = _print_unlocked<A, B>(_:_:)(
         &v20,
         &v21,
         &type metadata for NativeDoFMode,
         &type metadata for DefaultStringInterpolation,
         &protocol witness table for DefaultStringInterpolation);
  v7 = v21;
  v8 = v22;
  v9 = static os_log_type_t.debug.getter(v6);
  v10 = static os_log_type_t.debug.getter(v9);
  sub_100091CD8(v9, v10, v7, v8, v3);
  swift_bridgeObjectRelease(v8);
  v11 = sub_100004DF0(&unk_10033EEF8, &unk_10025F390);
  inited = swift_initStackObject(v11, &v19);
  *(_OWORD *)(inited + 16) = xmmword_10025B110;
  *(_QWORD *)(inited + 32) = 1701080941;
  *(_QWORD *)(inited + 40) = 0xE400000000000000LL;
  *(_BYTE *)(inited + 48) = v1;
  v13 = sub_1000126D8();
  swift_setDeallocating(inited);
  sub_1000AF1D4(inited + 32);
  v15 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter, v14), "defaultCenter"));
  v16 = String._bridgeToObjectiveC()();
  v17 = sub_10000F22C(v13);
  swift_bridgeObjectRelease(v13);
  isa = Dictionary._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v17);
  objc_msgSend(v15, "postNotificationName:object:userInfo:", v16, 0, isa);
  objc_release(v15);
  objc_release(v16);
  objc_release(isa);
}

/* ========================================================================
 * sub_1000AF054
 * EA: 0x1000af054
 ======================================================================== */

void __fastcall sub_1000AF054(__int64 a1)
{
  __int64 v2; // x19
  Swift::String v3; // x0
  Swift::String v4; // x0
  void *object; // x21
  Swift::String v6; // x0
  __int64 v7; // x0
  __int64 v8; // x22
  __int64 v9; // x0
  unsigned __int64 v10; // x0

  if ( (sub_100007580() & 1) == 0 )
  {
    v10 = a1;
    goto LABEL_5;
  }
  if ( *(_QWORD *)(a1 + 16) < 3u )
  {
    swift_bridgeObjectRelease(a1);
    v2 = type metadata accessor for VTLogger(0);
    _StringGuts.grow(_:)(61);
    v3._object = (void *)0x8000000100275CA0LL;
    v3._countAndFlagsBits = 0xD00000000000002ELL;
    String.append(_:)(v3);
    v4._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                              &type metadata for Int,
                              &protocol witness table for Int);
    object = v4._object;
    String.append(_:)(v4);
    swift_bridgeObjectRelease(object);
    v6._countAndFlagsBits = 0x657078652033202CLL;
    v6._object = (void *)0xED00002E64657463LL;
    String.append(_:)(v6);
    v8 = static os_log_type_t.error.getter(v7);
    v9 = static os_log_type_t.error.getter(v8);
    sub_100091CD8(v8, v9, 0, 0xE000000000000000LL, v2);
    v10 = 0xE000000000000000LL;
LABEL_5:
    swift_bridgeObjectRelease(v10);
    return;
  }
  if ( (unsigned __int8)sub_1000FB878(*(unsigned __int8 *)(a1 + 33)) == 14 )
    sub_1000AEB1C(a1);
  if ( *(_QWORD *)(a1 + 16) < 3u )
  {
    __break(1u);
  }
  else
  {
    if ( *(unsigned __int8 *)(a1 + 34) >= 3u )
      sub_1000AEC80(a1);
    swift_bridgeObjectRelease(a1);
  }
}

/* ========================================================================
 * sub_1000AF370
 * EA: 0x1000af370
 ======================================================================== */

__int64 __fastcall sub_1000AF370(unsigned __int64 a1, int a2)
{
  __int16 v3; // w26
  unsigned __int64 v4; // x27
  __int64 v5; // x19
  char *v6; // x21
  __int64 v7; // x22
  __int64 v8; // x28
  char *v9; // x23
  void *v10; // x24
  __int64 v11; // x0
  void *v12; // x25
  __int64 v13; // x0
  __int64 v14; // x0
  __int64 v15; // x26
  __int64 v16; // x20
  __int64 v17; // x0
  __int64 v19; // [xsp+0h] [xbp-90h] BYREF
  __int64 v20; // [xsp+8h] [xbp-88h]
  _QWORD aBlock[5]; // [xsp+10h] [xbp-80h] BYREF
  __int64 v22; // [xsp+38h] [xbp-58h]

  v3 = a1;
  v4 = HIDWORD(a1);
  v5 = type metadata accessor for DispatchWorkItemFlags(0);
  v20 = *(_QWORD *)(v5 - 8);
  v6 = (char *)&v19 - ((*(_QWORD *)(v20 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v7 = type metadata accessor for DispatchQoS(0);
  v8 = *(_QWORD *)(v7 - 8);
  v9 = (char *)&v19 - ((*(_QWORD *)(v8 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  sub_100004E40(0);
  v10 = (void *)static OS_dispatch_queue.main.getter();
  v11 = swift_allocObject(&unk_1002E3C98, 28, 7);
  *(_WORD *)(v11 + 16) = v3;
  *(_DWORD *)(v11 + 20) = v4;
  *(_DWORD *)(v11 + 24) = a2;
  aBlock[4] = sub_1000AF704;
  v22 = v11;
  aBlock[0] = _NSConcreteStackBlock;
  aBlock[1] = 1107296256;
  aBlock[2] = sub_10007A08C;
  aBlock[3] = &unk_1002E3CB0;
  v12 = _Block_copy(aBlock);
  v13 = swift_release(v22);
  v14 = static DispatchQoS.unspecified.getter(v13);
  aBlock[0] = &_swiftEmptyArrayStorage;
  v15 = sub_10000C824(v14);
  v16 = sub_100004DF0(&unk_10033BE50, &unk_10025B090);
  v17 = sub_1000147C4();
  dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v16, v17, v5, v15);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v9, v6, v12);
  _Block_release(v12);
  objc_release(v10);
  (*(void (__fastcall **)(char *, __int64))(v20 + 8))(v6, v5);
  return (*(__int64 (__fastcall **)(char *, __int64))(v8 + 8))(v9, v7);
}

/* ========================================================================
 * sub_1000AF550
 * EA: 0x1000af550
 ======================================================================== */

void __fastcall sub_1000AF550(unsigned __int64 a1, int a2)
{
  __int16 v3; // w20
  unsigned __int64 v4; // x23
  __int64 v5; // x0
  __int64 inited; // x21
  __int64 v7; // x19
  __int64 v8; // x1
  id v9; // x20
  NSString v10; // x21
  __int64 v11; // x22
  Class isa; // x19
  __int64 v13; // [xsp+0h] [xbp-70h] BYREF

  v3 = a1;
  v4 = HIDWORD(a1);
  v5 = sub_100004DF0(&unk_10033EF08, &unk_10025F3E8);
  inited = swift_initStackObject(v5, &v13);
  *(_QWORD *)(inited + 32) = 0x6567617373656DLL;
  *(_OWORD *)(inited + 16) = xmmword_10025B110;
  *(_QWORD *)(inited + 40) = 0xE700000000000000LL;
  *(_WORD *)(inited + 48) = v3;
  *(_DWORD *)(inited + 52) = v4;
  *(_DWORD *)(inited + 56) = a2;
  v7 = sub_100013C28();
  swift_setDeallocating(inited);
  sub_1000AF8B8(inited + 32);
  v9 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter, v8), "defaultCenter"));
  v10 = String._bridgeToObjectiveC()();
  v11 = sub_100010ECC(v7);
  swift_bridgeObjectRelease(v7);
  isa = Dictionary._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v11);
  objc_msgSend(v9, "postNotificationName:object:userInfo:", v10, 0, isa);
  objc_release(v9);
  objc_release(v10);
  objc_release(isa);
}

/* ========================================================================
 * sub_1000AF72C
 * EA: 0x1000af72c
 ======================================================================== */

unsigned __int64 __fastcall sub_1000AF72C(__int64 a1)
{
  __int64 v3; // x0
  __int64 v4; // x21
  __int64 v5; // x1
  __int64 v6; // x22
  __int64 v7; // x23
  __int64 v8; // x1
  __int64 v9; // x24
  unsigned __int16 v10; // w19
  __int64 v11; // x0
  __int64 v12; // x21
  __int64 v13; // x1
  __int64 v14; // x22
  __int64 v15; // x23
  __int64 v16; // x1
  __int64 v17; // x24
  __int64 v18; // x25
  __int64 v19; // x20
  __int64 v20; // x1
  __int64 v21; // x21
  __int64 v22; // x22
  __int64 v23; // x1
  __int64 v24; // x23

  if ( *(_QWORD *)(a1 + 16) < 0xBu || *(_BYTE *)(a1 + 32) )
  {
    swift_bridgeObjectRelease(a1);
    return 0;
  }
  else
  {
    v3 = swift_bridgeObjectRetain(a1);
    v4 = sub_100007D64(v3);
    v6 = v5;
    v7 = Data._Representation.subscript.getter(1, 3, v4, v5);
    v9 = v8;
    v10 = sub_10000DFDC(1, v7, v8);
    sub_10000C5FC(v7, v9);
    sub_10000C5FC(v4, v6);
    v11 = swift_bridgeObjectRetain(a1);
    v12 = sub_100007D64(v11);
    v14 = v13;
    v15 = Data._Representation.subscript.getter(3, 7, v12, v13);
    v17 = v16;
    v18 = sub_10000E364(1, v15, v16);
    sub_10000C5FC(v15, v17);
    sub_10000C5FC(v12, v14);
    v19 = sub_100007D64(a1);
    v21 = v20;
    v22 = Data._Representation.subscript.getter(7, 11, v19, v20);
    v24 = v23;
    sub_10000E364(1, v22, v23);
    sub_10000C5FC(v22, v24);
    sub_10000C5FC(v19, v21);
    return v10 | (unsigned __int64)(v18 << 32);
  }
}

/* ========================================================================
 * sub_1000AF910
 * EA: 0x1000af910
 ======================================================================== */

__int64 __fastcall sub_1000AF910(char a1)
{
  __int64 v2; // x19
  __int64 v3; // x27
  char *v4; // x21
  __int64 v5; // x22
  __int64 v6; // x28
  char *v7; // x23
  void *v8; // x24
  __int64 v9; // x0
  void *v10; // x25
  __int64 v11; // x0
  __int64 v12; // x0
  __int64 v13; // x26
  __int64 v14; // x20
  __int64 v15; // x0
  __int64 v16; // x19
  unsigned __int64 v17; // x20
  __int64 v18; // x0
  __int64 v19; // x21
  unsigned __int64 v20; // x1
  unsigned __int64 v21; // x22
  Swift::String v22; // x0
  void **v23; // x20
  unsigned __int64 v24; // x21
  __int64 v25; // x0
  __int64 v26; // x22
  __int64 v27; // x0
  void **aBlock; // [xsp+0h] [xbp-80h] BYREF
  unsigned __int64 v30; // [xsp+8h] [xbp-78h]
  __int64 (__fastcall *v31)(); // [xsp+10h] [xbp-70h]
  void *v32; // [xsp+18h] [xbp-68h]
  __int64 (__fastcall *v33)(); // [xsp+20h] [xbp-60h]
  __int64 v34; // [xsp+28h] [xbp-58h]

  v2 = type metadata accessor for DispatchWorkItemFlags(0);
  v3 = *(_QWORD *)(v2 - 8);
  v4 = (char *)&aBlock - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for DispatchQoS(0);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = (char *)&aBlock - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  sub_100004E40(0);
  v8 = (void *)static OS_dispatch_queue.main.getter();
  v9 = swift_allocObject(&unk_1002E3DA0, 17, 7);
  *(_BYTE *)(v9 + 16) = a1;
  v33 = sub_1000AFD14;
  v34 = v9;
  aBlock = _NSConcreteStackBlock;
  v30 = 1107296256;
  v31 = sub_10007A08C;
  v32 = &unk_1002E3DB8;
  v10 = _Block_copy(&aBlock);
  v11 = swift_release(v34);
  v12 = static DispatchQoS.unspecified.getter(v11);
  aBlock = (void **)&_swiftEmptyArrayStorage;
  v13 = sub_10000C824(v12);
  v14 = sub_100004DF0(&unk_10033BE50, &unk_10025B090);
  v15 = sub_1000147C4();
  dispatch thunk of SetAlgebra.init<A>(_:)(&aBlock, v14, v15, v2, v13);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v7, v4, v10);
  _Block_release(v10);
  objc_release(v8);
  (*(void (__fastcall **)(char *, __int64))(v3 + 8))(v4, v2);
  (*(void (__fastcall **)(char *, __int64))(v6 + 8))(v7, v5);
  v16 = type metadata accessor for VTLogger(0);
  aBlock = nullptr;
  v30 = 0xE000000000000000LL;
  _StringGuts.grow(_:)(21);
  v17 = v30;
  aBlock = (void **)&type metadata for VTGJumpToAppMsg;
  v18 = sub_100004DF0(&unk_10033EF18, &unk_10025F440);
  v19 = String.init<A>(describing:)(&aBlock, v18);
  v21 = v20;
  swift_bridgeObjectRelease(v17);
  aBlock = (void **)v19;
  v30 = v21;
  v22._object = (void *)0x800000010026ECA0LL;
  v22._countAndFlagsBits = 0xD000000000000013LL;
  String.append(_:)(v22);
  v23 = aBlock;
  v24 = v30;
  v26 = static os_log_type_t.debug.getter(v25);
  v27 = static os_log_type_t.debug.getter(v26);
  sub_100091CD8(v26, v27, v23, v24, v16);
  return swift_bridgeObjectRelease(v24);
}

/* ========================================================================
 * sub_1000AFB90
 * EA: 0x1000afb90
 ======================================================================== */

void __fastcall sub_1000AFB90(unsigned __int8 a1, __int64 a2)
{
  id v3; // x19
  NSString v4; // x20
  __int64 v5; // x0
  __int64 inited; // x22
  __int64 v7; // x21
  Class isa; // x22
  _WORD v9[8]; // [xsp+8h] [xbp-98h] BYREF
  _BYTE v10[104]; // [xsp+18h] [xbp-88h] BYREF

  v3 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter, a2), "defaultCenter"));
  v4 = String._bridgeToObjectiveC()();
  v5 = sub_100004DF0(&unk_10033CDE0, &unk_10025C6A0);
  inited = swift_initStackObject(v5, v10);
  *(_OWORD *)(inited + 16) = xmmword_10025B110;
  strcpy((char *)v9, "JumpToAppACK");
  HIBYTE(v9[6]) = 0;
  v9[7] = -5120;
  AnyHashable.init<A>(_:)((_QWORD *)(inited + 32), v9, &type metadata for String, &protocol witness table for String);
  *(_QWORD *)(inited + 96) = &type metadata for Int;
  *(_QWORD *)(inited + 72) = a1;
  v7 = sub_100013868(inited);
  swift_setDeallocating(inited);
  sub_1000406D0(inited + 32);
  isa = Dictionary._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v7);
  objc_msgSend(v3, "postNotificationName:object:userInfo:", v4, 0, isa);
  objc_release(v3);
  objc_release(v4);
  objc_release(isa);
}

/* ========================================================================
 * sub_1000AFD44
 * EA: 0x1000afd44
 ======================================================================== */

__int64 __fastcall sub_1000AFD44(char a1)
{
  __int64 v2; // x19
  __int64 v3; // x27
  char *v4; // x21
  __int64 v5; // x22
  __int64 v6; // x28
  char *v7; // x23
  void *v8; // x24
  __int64 v9; // x0
  void *v10; // x25
  __int64 v11; // x0
  __int64 v12; // x0
  __int64 v13; // x26
  __int64 v14; // x20
  __int64 v15; // x0
  _QWORD aBlock[5]; // [xsp+0h] [xbp-80h] BYREF
  __int64 v18; // [xsp+28h] [xbp-58h]

  v2 = type metadata accessor for DispatchWorkItemFlags(0);
  v3 = *(_QWORD *)(v2 - 8);
  v4 = (char *)aBlock - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for DispatchQoS(0);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = (char *)aBlock - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  sub_100004E40(0);
  v8 = (void *)static OS_dispatch_queue.main.getter();
  v9 = swift_allocObject(&unk_1002E3E48, 17, 7);
  *(_BYTE *)(v9 + 16) = a1;
  aBlock[4] = sub_1000B0124;
  v18 = v9;
  aBlock[0] = _NSConcreteStackBlock;
  aBlock[1] = 1107296256;
  aBlock[2] = sub_10007A08C;
  aBlock[3] = &unk_1002E3E60;
  v10 = _Block_copy(aBlock);
  v11 = swift_release(v18);
  v12 = static DispatchQoS.unspecified.getter(v11);
  aBlock[0] = &_swiftEmptyArrayStorage;
  v13 = sub_10000C824(v12);
  v14 = sub_100004DF0(&unk_10033BE50, &unk_10025B090);
  v15 = sub_1000147C4();
  dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v14, v15, v2, v13);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v7, v4, v10);
  _Block_release(v10);
  objc_release(v8);
  (*(void (__fastcall **)(char *, __int64))(v3 + 8))(v4, v2);
  return (*(__int64 (__fastcall **)(char *, __int64))(v6 + 8))(v7, v5);
}

/* ========================================================================
 * sub_1000AFF10
 * EA: 0x1000aff10
 ======================================================================== */

__int64 __fastcall sub_1000AFF10(char a1)
{
  __int64 v2; // x0
  __int64 inited; // x20
  __int64 v4; // x22
  __int64 v5; // x1
  id v6; // x20
  NSString v7; // x21
  __int64 v8; // x23
  Class isa; // x22
  __int64 v10; // x21
  Swift::String v11; // x0
  void *object; // x19
  __int64 v13; // x0
  __int64 v14; // x22
  __int64 v15; // x0
  _BYTE v17[56]; // [xsp+18h] [xbp-68h] BYREF

  v2 = sub_100004DF0(&unk_10033C198, &unk_10025B358);
  inited = swift_initStackObject(v2, v17);
  *(_OWORD *)(inited + 16) = xmmword_10025B110;
  *(_QWORD *)(inited + 32) = 0x656D756C6F76LL;
  *(_QWORD *)(inited + 40) = 0xE600000000000000LL;
  *(_BYTE *)(inited + 48) = a1;
  v4 = sub_100012C94();
  swift_setDeallocating(inited);
  sub_100014814(inited + 32);
  v6 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter, v5), "defaultCenter"));
  v7 = String._bridgeToObjectiveC()();
  v8 = sub_10000FB94(v4);
  swift_bridgeObjectRelease(v4);
  isa = Dictionary._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v8);
  objc_msgSend(v6, "postNotificationName:object:userInfo:", v7, 0, isa);
  objc_release(v6);
  objc_release(v7);
  objc_release(isa);
  v10 = type metadata accessor for VTLogger(0);
  _StringGuts.grow(_:)(40);
  swift_bridgeObjectRelease(0xE000000000000000LL);
  v11._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                             &type metadata for UInt8,
                             &protocol witness table for UInt8);
  object = v11._object;
  String.append(_:)(v11);
  v13 = swift_bridgeObjectRelease(object);
  v14 = static os_log_type_t.debug.getter(v13);
  v15 = static os_log_type_t.debug.getter(v14);
  sub_100091CD8(v14, v15, 0xD000000000000026LL, 0x8000000100275D60LL, v10);
  return swift_bridgeObjectRelease(0x8000000100275D60LL);
}

/* ========================================================================
 * $s11SpaceWalker8FirmwareVwcp
 * EA: 0x1000b01fc
 ======================================================================== */

__int64 __fastcall initializeWithCopy for Firmware(__int64 a1, __int64 a2)
{
  __int64 v4; // x0
  __int64 v5; // x28
  __int64 v6; // x21
  __int64 v7; // x22
  __int64 v8; // x23
  __int64 v9; // x24
  __int64 v10; // x26
  __int64 v11; // x27
  __int64 v12; // x25
  __int64 v13; // x20
  __int64 v15; // [xsp+0h] [xbp-80h]
  __int64 v16; // [xsp+8h] [xbp-78h]
  __int64 v17; // [xsp+10h] [xbp-70h]
  __int64 v18; // [xsp+18h] [xbp-68h]
  __int64 v19; // [xsp+20h] [xbp-60h]
  __int64 v20; // [xsp+28h] [xbp-58h]

  v4 = *(_QWORD *)(a2 + 8);
  *(_QWORD *)a1 = *(_QWORD *)a2;
  *(_QWORD *)(a1 + 8) = v4;
  v20 = *(_QWORD *)(a2 + 24);
  *(_QWORD *)(a1 + 16) = *(_QWORD *)(a2 + 16);
  *(_QWORD *)(a1 + 24) = v20;
  v19 = *(_QWORD *)(a2 + 40);
  *(_QWORD *)(a1 + 32) = *(_QWORD *)(a2 + 32);
  *(_QWORD *)(a1 + 40) = v19;
  v18 = *(_QWORD *)(a2 + 56);
  *(_QWORD *)(a1 + 48) = *(_QWORD *)(a2 + 48);
  *(_QWORD *)(a1 + 56) = v18;
  v17 = *(_QWORD *)(a2 + 72);
  *(_QWORD *)(a1 + 64) = *(_QWORD *)(a2 + 64);
  *(_QWORD *)(a1 + 72) = v17;
  v16 = *(_QWORD *)(a2 + 88);
  *(_QWORD *)(a1 + 80) = *(_QWORD *)(a2 + 80);
  *(_QWORD *)(a1 + 88) = v16;
  v15 = *(_QWORD *)(a2 + 104);
  *(_QWORD *)(a1 + 96) = *(_QWORD *)(a2 + 96);
  *(_QWORD *)(a1 + 104) = v15;
  v5 = *(_QWORD *)(a2 + 120);
  *(_QWORD *)(a1 + 112) = *(_QWORD *)(a2 + 112);
  *(_QWORD *)(a1 + 120) = v5;
  v6 = *(_QWORD *)(a2 + 136);
  *(_QWORD *)(a1 + 128) = *(_QWORD *)(a2 + 128);
  *(_QWORD *)(a1 + 136) = v6;
  v7 = *(_QWORD *)(a2 + 152);
  *(_QWORD *)(a1 + 144) = *(_QWORD *)(a2 + 144);
  *(_QWORD *)(a1 + 152) = v7;
  v8 = *(_QWORD *)(a2 + 168);
  *(_QWORD *)(a1 + 160) = *(_QWORD *)(a2 + 160);
  *(_QWORD *)(a1 + 168) = v8;
  v9 = *(_QWORD *)(a2 + 184);
  *(_QWORD *)(a1 + 176) = *(_QWORD *)(a2 + 176);
  *(_QWORD *)(a1 + 184) = v9;
  v10 = *(_QWORD *)(a2 + 200);
  *(_QWORD *)(a1 + 192) = *(_QWORD *)(a2 + 192);
  *(_QWORD *)(a1 + 200) = v10;
  *(_OWORD *)(a1 + 208) = *(_OWORD *)(a2 + 208);
  v11 = *(_QWORD *)(a2 + 232);
  *(_QWORD *)(a1 + 224) = *(_QWORD *)(a2 + 224);
  *(_QWORD *)(a1 + 232) = v11;
  v12 = *(_QWORD *)(a2 + 248);
  swift_bridgeObjectRetain(v4);
  swift_bridgeObjectRetain(v20);
  swift_bridgeObjectRetain(v19);
  swift_bridgeObjectRetain(v18);
  swift_bridgeObjectRetain(v17);
  swift_bridgeObjectRetain(v16);
  swift_bridgeObjectRetain(v15);
  swift_bridgeObjectRetain(v5);
  swift_bridgeObjectRetain(v6);
  swift_bridgeObjectRetain(v7);
  swift_bridgeObjectRetain(v8);
  swift_bridgeObjectRetain(v9);
  swift_bridgeObjectRetain(v10);
  swift_bridgeObjectRetain(v11);
  if ( v12 )
  {
    *(_QWORD *)(a1 + 240) = *(_QWORD *)(a2 + 240);
    *(_QWORD *)(a1 + 248) = v12;
    v13 = *(_QWORD *)(a2 + 256);
    *(_QWORD *)(a1 + 256) = v13;
    swift_bridgeObjectRetain(v12);
    swift_bridgeObjectRetain(v13);
  }
  else
  {
    *(_OWORD *)(a1 + 240) = *(_OWORD *)(a2 + 240);
    *(_QWORD *)(a1 + 256) = *(_QWORD *)(a2 + 256);
  }
  return a1;
}

/* ========================================================================
 * $s11SpaceWalker8FirmwareVwca
 * EA: 0x1000b0380
 ======================================================================== */

_QWORD *__fastcall assignWithCopy for Firmware(_QWORD *a1, _QWORD *a2)
{
  __int64 v4; // x0
  __int64 v5; // x21
  __int64 v6; // x0
  __int64 v7; // x21
  __int64 v8; // x0
  __int64 v9; // x21
  __int64 v10; // x0
  __int64 v11; // x21
  __int64 v12; // x0
  __int64 v13; // x21
  __int64 v14; // x0
  __int64 v15; // x21
  __int64 v16; // x0
  __int64 v17; // x21
  __int64 v18; // x0
  __int64 v19; // x21
  __int64 v20; // x0
  __int64 v21; // x21
  __int64 v22; // x0
  __int64 v23; // x21
  __int64 v24; // x0
  __int64 v25; // x21
  __int64 v26; // x0
  __int64 v27; // x21
  __int64 v28; // x0
  __int64 v29; // x21
  __int64 v30; // x0
  __int64 v31; // x21
  __int64 v32; // x21
  __int64 v33; // x8
  __int64 v34; // x0
  __int64 v35; // x0
  __int64 v36; // x20
  __int64 v37; // x0
  __int64 v38; // x20
  __int64 v39; // x8
  __int128 v40; // q0

  *a1 = *a2;
  v4 = a2[1];
  v5 = a1[1];
  a1[1] = v4;
  swift_bridgeObjectRetain(v4);
  swift_bridgeObjectRelease(v5);
  a1[2] = a2[2];
  v6 = a2[3];
  v7 = a1[3];
  a1[3] = v6;
  swift_bridgeObjectRetain(v6);
  swift_bridgeObjectRelease(v7);
  a1[4] = a2[4];
  v8 = a2[5];
  v9 = a1[5];
  a1[5] = v8;
  swift_bridgeObjectRetain(v8);
  swift_bridgeObjectRelease(v9);
  a1[6] = a2[6];
  v10 = a2[7];
  v11 = a1[7];
  a1[7] = v10;
  swift_bridgeObjectRetain(v10);
  swift_bridgeObjectRelease(v11);
  a1[8] = a2[8];
  v12 = a2[9];
  v13 = a1[9];
  a1[9] = v12;
  swift_bridgeObjectRetain(v12);
  swift_bridgeObjectRelease(v13);
  a1[10] = a2[10];
  v14 = a2[11];
  v15 = a1[11];
  a1[11] = v14;
  swift_bridgeObjectRetain(v14);
  swift_bridgeObjectRelease(v15);
  a1[12] = a2[12];
  v16 = a2[13];
  v17 = a1[13];
  a1[13] = v16;
  swift_bridgeObjectRetain(v16);
  swift_bridgeObjectRelease(v17);
  a1[14] = a2[14];
  v18 = a2[15];
  v19 = a1[15];
  a1[15] = v18;
  swift_bridgeObjectRetain(v18);
  swift_bridgeObjectRelease(v19);
  a1[16] = a2[16];
  v20 = a2[17];
  v21 = a1[17];
  a1[17] = v20;
  swift_bridgeObjectRetain(v20);
  swift_bridgeObjectRelease(v21);
  a1[18] = a2[18];
  v22 = a2[19];
  v23 = a1[19];
  a1[19] = v22;
  swift_bridgeObjectRetain(v22);
  swift_bridgeObjectRelease(v23);
  a1[20] = a2[20];
  v24 = a2[21];
  v25 = a1[21];
  a1[21] = v24;
  swift_bridgeObjectRetain(v24);
  swift_bridgeObjectRelease(v25);
  a1[22] = a2[22];
  v26 = a2[23];
  v27 = a1[23];
  a1[23] = v26;
  swift_bridgeObjectRetain(v26);
  swift_bridgeObjectRelease(v27);
  a1[24] = a2[24];
  v28 = a2[25];
  v29 = a1[25];
  a1[25] = v28;
  swift_bridgeObjectRetain(v28);
  swift_bridgeObjectRelease(v29);
  a1[26] = a2[26];
  a1[27] = a2[27];
  a1[28] = a2[28];
  v30 = a2[29];
  v31 = a1[29];
  a1[29] = v30;
  swift_bridgeObjectRetain(v30);
  swift_bridgeObjectRelease(v31);
  v32 = a1[31];
  v33 = a2[31];
  if ( v32 )
  {
    if ( v33 )
    {
      a1[30] = a2[30];
      v34 = a2[31];
      a1[31] = v34;
      swift_bridgeObjectRetain(v34);
      swift_bridgeObjectRelease(v32);
      v35 = a2[32];
      v36 = a1[32];
      a1[32] = v35;
      swift_bridgeObjectRetain(v35);
      swift_bridgeObjectRelease(v36);
    }
    else
    {
      sub_1000B061C(a1 + 30);
      v39 = a2[32];
      *((_OWORD *)a1 + 15) = *((_OWORD *)a2 + 15);
      a1[32] = v39;
    }
  }
  else if ( v33 )
  {
    a1[30] = a2[30];
    v37 = a2[31];
    a1[31] = v37;
    v38 = a2[32];
    a1[32] = v38;
    swift_bridgeObjectRetain(v37);
    swift_bridgeObjectRetain(v38);
  }
  else
  {
    v40 = *((_OWORD *)a2 + 15);
    a1[32] = a2[32];
    *((_OWORD *)a1 + 15) = v40;
  }
  return a1;
}

/* ========================================================================
 * $s11SpaceWalker8FirmwareVwta
 * EA: 0x1000b0654
 ======================================================================== */

__int64 __fastcall assignWithTake for Firmware(__int64 a1, __int64 a2)
{
  __int64 v4; // x9
  __int64 v5; // x0
  __int64 v6; // x9
  __int64 v7; // x0
  __int64 v8; // x9
  __int64 v9; // x0
  __int64 v10; // x9
  __int64 v11; // x0
  __int64 v12; // x9
  __int64 v13; // x0
  __int64 v14; // x9
  __int64 v15; // x0
  __int64 v16; // x9
  __int64 v17; // x0
  __int64 v18; // x9
  __int64 v19; // x0
  __int64 v20; // x9
  __int64 v21; // x0
  __int64 v22; // x9
  __int64 v23; // x0
  __int64 v24; // x9
  __int64 v25; // x0
  __int64 v26; // x9
  __int64 v27; // x0
  __int64 v28; // x9
  __int64 v29; // x0
  __int64 v30; // x9
  __int64 v31; // x0
  __int64 v32; // x0
  __int64 v33; // x8
  __int64 v34; // x0

  v4 = *(_QWORD *)(a2 + 8);
  v5 = *(_QWORD *)(a1 + 8);
  *(_QWORD *)a1 = *(_QWORD *)a2;
  *(_QWORD *)(a1 + 8) = v4;
  swift_bridgeObjectRelease(v5);
  v6 = *(_QWORD *)(a2 + 24);
  v7 = *(_QWORD *)(a1 + 24);
  *(_QWORD *)(a1 + 16) = *(_QWORD *)(a2 + 16);
  *(_QWORD *)(a1 + 24) = v6;
  swift_bridgeObjectRelease(v7);
  v8 = *(_QWORD *)(a2 + 40);
  v9 = *(_QWORD *)(a1 + 40);
  *(_QWORD *)(a1 + 32) = *(_QWORD *)(a2 + 32);
  *(_QWORD *)(a1 + 40) = v8;
  swift_bridgeObjectRelease(v9);
  v10 = *(_QWORD *)(a2 + 56);
  v11 = *(_QWORD *)(a1 + 56);
  *(_QWORD *)(a1 + 48) = *(_QWORD *)(a2 + 48);
  *(_QWORD *)(a1 + 56) = v10;
  swift_bridgeObjectRelease(v11);
  v12 = *(_QWORD *)(a2 + 72);
  v13 = *(_QWORD *)(a1 + 72);
  *(_QWORD *)(a1 + 64) = *(_QWORD *)(a2 + 64);
  *(_QWORD *)(a1 + 72) = v12;
  swift_bridgeObjectRelease(v13);
  v14 = *(_QWORD *)(a2 + 88);
  v15 = *(_QWORD *)(a1 + 88);
  *(_QWORD *)(a1 + 80) = *(_QWORD *)(a2 + 80);
  *(_QWORD *)(a1 + 88) = v14;
  swift_bridgeObjectRelease(v15);
  v16 = *(_QWORD *)(a2 + 104);
  v17 = *(_QWORD *)(a1 + 104);
  *(_QWORD *)(a1 + 96) = *(_QWORD *)(a2 + 96);
  *(_QWORD *)(a1 + 104) = v16;
  swift_bridgeObjectRelease(v17);
  v18 = *(_QWORD *)(a2 + 120);
  v19 = *(_QWORD *)(a1 + 120);
  *(_QWORD *)(a1 + 112) = *(_QWORD *)(a2 + 112);
  *(_QWORD *)(a1 + 120) = v18;
  swift_bridgeObjectRelease(v19);
  v20 = *(_QWORD *)(a2 + 136);
  v21 = *(_QWORD *)(a1 + 136);
  *(_QWORD *)(a1 + 128) = *(_QWORD *)(a2 + 128);
  *(_QWORD *)(a1 + 136) = v20;
  swift_bridgeObjectRelease(v21);
  v22 = *(_QWORD *)(a2 + 152);
  v23 = *(_QWORD *)(a1 + 152);
  *(_QWORD *)(a1 + 144) = *(_QWORD *)(a2 + 144);
  *(_QWORD *)(a1 + 152) = v22;
  swift_bridgeObjectRelease(v23);
  v24 = *(_QWORD *)(a2 + 168);
  v25 = *(_QWORD *)(a1 + 168);
  *(_QWORD *)(a1 + 160) = *(_QWORD *)(a2 + 160);
  *(_QWORD *)(a1 + 168) = v24;
  swift_bridgeObjectRelease(v25);
  v26 = *(_QWORD *)(a2 + 184);
  v27 = *(_QWORD *)(a1 + 184);
  *(_QWORD *)(a1 + 176) = *(_QWORD *)(a2 + 176);
  *(_QWORD *)(a1 + 184) = v26;
  swift_bridgeObjectRelease(v27);
  v28 = *(_QWORD *)(a2 + 200);
  v29 = *(_QWORD *)(a1 + 200);
  *(_QWORD *)(a1 + 192) = *(_QWORD *)(a2 + 192);
  *(_QWORD *)(a1 + 200) = v28;
  swift_bridgeObjectRelease(v29);
  *(_OWORD *)(a1 + 208) = *(_OWORD *)(a2 + 208);
  v30 = *(_QWORD *)(a2 + 232);
  v31 = *(_QWORD *)(a1 + 232);
  *(_QWORD *)(a1 + 224) = *(_QWORD *)(a2 + 224);
  *(_QWORD *)(a1 + 232) = v30;
  swift_bridgeObjectRelease(v31);
  v32 = *(_QWORD *)(a1 + 248);
  if ( !v32 )
    goto LABEL_5;
  v33 = *(_QWORD *)(a2 + 248);
  if ( !v33 )
  {
    sub_1000B061C(a1 + 240);
LABEL_5:
    *(_OWORD *)(a1 + 240) = *(_OWORD *)(a2 + 240);
    *(_QWORD *)(a1 + 256) = *(_QWORD *)(a2 + 256);
    return a1;
  }
  *(_QWORD *)(a1 + 240) = *(_QWORD *)(a2 + 240);
  *(_QWORD *)(a1 + 248) = v33;
  swift_bridgeObjectRelease(v32);
  v34 = *(_QWORD *)(a1 + 256);
  *(_QWORD *)(a1 + 256) = *(_QWORD *)(a2 + 256);
  swift_bridgeObjectRelease(v34);
  return a1;
}

/* ========================================================================
 * sub_1000B0A3C
 * EA: 0x1000b0a3c
 ======================================================================== */

__int64 sub_1000B0A3C()
{
  _QWORD *v0; // x22
  _QWORD *v1; // x19
  __int64 v2; // x0
  __int64 v4; // x23
  __int64 v5; // x19
  __int64 v6; // x24
  __int64 v7; // x25
  __int64 v8; // x21
  __int64 v9; // x0
  __int64 v10; // x20
  __int64 v11; // x8
  __int64 v12; // x26
  __int64 v13; // x23

  if ( _stdlib_isOSVersionAtLeastOrVariantVersionAtLeast(_:_:_:_:_:_:)(0xFu, 0, 0, 0x12u, 0, 0) )
  {
    v1 = (_QWORD *)swift_task_alloc(async function pointer to withCheckedContinuation<A>(isolation:function:_:)[1]);
    v0[12] = v1;
    v2 = sub_100004DF0(&unk_10033EF78, &unk_10025F5D0);
    *v1 = v0;
    v1[1] = sub_1000B0C44;
    return withCheckedContinuation<A>(isolation:function:_:)(
             v0 + 10,
             0,
             0,
             0x29286863746566LL,
             0xE700000000000000LL,
             sub_1000B479C,
             0,
             v2);
  }
  else
  {
    v0[7] = v0 + 11;
    v0[2] = v0;
    v0[3] = sub_1000B0C88;
    v4 = swift_continuation_init(v0 + 2, 0);
    v5 = sub_100004DF0(&unk_10033EF70, &unk_10025F5C8);
    v6 = *(_QWORD *)(v5 - 8);
    v7 = *(_QWORD *)(v6 + 64);
    v8 = swift_task_alloc((v7 + 15) & 0xFFFFFFFFFFFFFFF0LL);
    v9 = sub_100004DF0(&unk_10033EF78, &unk_10025F5D0);
    CheckedContinuation.init(continuation:function:)(
      v4,
      0x29286863746566LL,
      0xE700000000000000LL,
      v9,
      &type metadata for Never,
      &protocol witness table for Never);
    v10 = swift_task_alloc((v7 + 15) & 0xFFFFFFFFFFFFFFF0LL);
    (*(void (__fastcall **)(__int64, __int64, __int64))(v6 + 16))(v10, v8, v5);
    v11 = *(unsigned __int8 *)(v6 + 80);
    v12 = (v11 + 16) & ~v11;
    v13 = swift_allocObject(&unk_1002E4008, v12 + v7, v11 | 7);
    (*(void (__fastcall **)(__int64, __int64, __int64))(v6 + 32))(v13 + v12, v10, v5);
    sub_1000B2F34(sub_1000B47A4, v13);
    swift_release(v13);
    (*(void (__fastcall **)(__int64, __int64))(v6 + 8))(v8, v5);
    swift_task_dealloc(v10);
    swift_task_dealloc(v8);
    return swift_continuation_await(v0 + 2);
  }
}

/* ========================================================================
 * sub_1000B0CC4
 * EA: 0x1000b0cc4
 ======================================================================== */

__int64 sub_1000B0CC4()
{
  __int64 v0; // x20
  _QWORD *v1; // x22
  __int64 v2; // x0
  __int64 v3; // x0
  __int64 v4; // x8
  __int64 v5; // x0
  __int64 v6; // x0
  __int64 v7; // x8

  v1[2] = v0;
  v2 = type metadata accessor for URLError.Code(0);
  v1[3] = swift_task_alloc((*(_QWORD *)(*(_QWORD *)(v2 - 8) + 64LL) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v3 = type metadata accessor for URLError(0);
  v1[4] = v3;
  v4 = *(_QWORD *)(v3 - 8);
  v1[5] = v4;
  v1[6] = swift_task_alloc((*(_QWORD *)(v4 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = sub_100004DF0(&unk_10033C408, &unk_10025B750);
  v1[7] = swift_task_alloc((*(_QWORD *)(*(_QWORD *)(v5 - 8) + 64LL) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v6 = type metadata accessor for URL(0);
  v1[8] = v6;
  v7 = *(_QWORD *)(v6 - 8);
  v1[9] = v7;
  v1[10] = swift_task_alloc((*(_QWORD *)(v7 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  return swift_task_switch(sub_1000B0D98, 0, 0);
}

/* ========================================================================
 * sub_1000B0D98
 * EA: 0x1000b0d98
 ======================================================================== */

__int64 sub_1000B0D98()
{
  __int64 v0; // x22
  __int64 v1; // x20
  __int64 v2; // x21
  __int64 v3; // x19
  __int64 v4; // x19
  __int64 v5; // x21
  __int64 v6; // x24
  __int64 v7; // x20
  __int64 v8; // x0
  __int64 v9; // x23
  __int64 v10; // x0
  __int64 v11; // x0
  __int64 v12; // x0
  __int64 v13; // x19
  __int64 v14; // x21
  __int64 v15; // x23
  __int64 v17; // x1
  _QWORD *v18; // x0

  v1 = *(_QWORD *)(v0 + 64);
  v2 = *(_QWORD *)(v0 + 72);
  v3 = *(_QWORD *)(v0 + 56);
  URL.init(string:)(*(_QWORD *)(*(_QWORD *)(v0 + 16) + 32LL), *(_QWORD *)(*(_QWORD *)(v0 + 16) + 40LL));
  if ( (*(unsigned int (__fastcall **)(__int64, __int64, __int64))(v2 + 48))(v3, 1, v1) == 1 )
  {
    v4 = *(_QWORD *)(v0 + 48);
    v5 = *(_QWORD *)(v0 + 32);
    v6 = *(_QWORD *)(v0 + 40);
    v7 = *(_QWORD *)(v0 + 24);
    v8 = sub_10000C910(*(_QWORD *)(v0 + 56), &unk_10033C408, &unk_10025B750);
    static URLError.Code.badURL.getter(v8);
    v9 = sub_100012D98(&_swiftEmptyArrayStorage);
    v10 = sub_1000B4758();
    v11 = _BridgedStoredNSError.init(_:userInfo:)(v7, v9, v5, v10);
    URLError._nsError.getter(v11);
    v12 = (*(__int64 (__fastcall **)(__int64, __int64))(v6 + 8))(v4, v5);
    swift_willThrow(v12);
    v14 = *(_QWORD *)(v0 + 48);
    v13 = *(_QWORD *)(v0 + 56);
    v15 = *(_QWORD *)(v0 + 24);
    swift_task_dealloc(*(_QWORD *)(v0 + 80));
    swift_task_dealloc(v13);
    swift_task_dealloc(v14);
    swift_task_dealloc(v15);
    return (*(__int64 (**)(void))(v0 + 8))();
  }
  else
  {
    (*(void (__fastcall **)(_QWORD, _QWORD, _QWORD))(*(_QWORD *)(v0 + 72) + 32LL))(
      *(_QWORD *)(v0 + 80),
      *(_QWORD *)(v0 + 56),
      *(_QWORD *)(v0 + 64));
    *(_QWORD *)(v0 + 88) = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(
                                                                                 &OBJC_CLASS___NSURLSession,
                                                                                 v17), "sharedSession"));
    v18 = (_QWORD *)swift_task_alloc(async function pointer to NSURLSession.data(from:delegate:)[1]);
    *(_QWORD *)(v0 + 96) = v18;
    *v18 = v0;
    v18[1] = sub_1000B0F28;
    return NSURLSession.data(from:delegate:)(*(_QWORD *)(v0 + 80), 0);
  }
}

/* ========================================================================
 * sub_1000B0FB8
 * EA: 0x1000b0fb8
 ======================================================================== */

__int64 sub_1000B0FB8()
{
  __int64 v0; // x20
  _QWORD *v1; // x22
  __int64 v2; // x0
  __int64 v3; // x0
  __int64 v4; // x8
  __int64 v5; // x0
  __int64 v6; // x0
  __int64 v7; // x8

  v1[2] = v0;
  v2 = type metadata accessor for URLError.Code(0);
  v1[3] = swift_task_alloc((*(_QWORD *)(*(_QWORD *)(v2 - 8) + 64LL) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v3 = type metadata accessor for URLError(0);
  v1[4] = v3;
  v4 = *(_QWORD *)(v3 - 8);
  v1[5] = v4;
  v1[6] = swift_task_alloc((*(_QWORD *)(v4 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = sub_100004DF0(&unk_10033C408, &unk_10025B750);
  v1[7] = swift_task_alloc((*(_QWORD *)(*(_QWORD *)(v5 - 8) + 64LL) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v6 = type metadata accessor for URL(0);
  v1[8] = v6;
  v7 = *(_QWORD *)(v6 - 8);
  v1[9] = v7;
  v1[10] = swift_task_alloc((*(_QWORD *)(v7 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  return swift_task_switch(sub_1000B108C, 0, 0);
}

/* ========================================================================
 * sub_1000B108C
 * EA: 0x1000b108c
 ======================================================================== */

__int64 __fastcall sub_1000B108C(__int64 a1)
{
  __int64 v1; // x22
  __int64 v2; // x8
  __int64 v3; // x1
  __int64 v4; // x20
  __int64 v5; // x21
  __int64 v6; // x19
  __int64 v7; // x19
  __int64 v8; // x24
  __int64 v9; // x20
  __int64 v10; // x21
  __int64 v11; // x23
  __int64 v12; // x0
  __int64 v13; // x0
  __int64 v14; // x0
  __int64 v15; // x19
  __int64 v16; // x21
  __int64 v17; // x23
  __int64 v19; // x1
  _QWORD *v20; // x0

  v2 = *(_QWORD *)(v1 + 16);
  v3 = *(_QWORD *)(v2 + 120);
  if ( !v3 )
    goto LABEL_4;
  v4 = *(_QWORD *)(v1 + 64);
  v5 = *(_QWORD *)(v1 + 72);
  v6 = *(_QWORD *)(v1 + 56);
  URL.init(string:)(*(_QWORD *)(v2 + 112), v3);
  if ( (*(unsigned int (__fastcall **)(__int64, __int64, __int64))(v5 + 48))(v6, 1, v4) == 1 )
  {
    a1 = sub_10000C910(*(_QWORD *)(v1 + 56), &unk_10033C408, &unk_10025B750);
LABEL_4:
    v8 = *(_QWORD *)(v1 + 40);
    v7 = *(_QWORD *)(v1 + 48);
    v9 = *(_QWORD *)(v1 + 24);
    v10 = *(_QWORD *)(v1 + 32);
    static URLError.Code.badURL.getter(a1);
    v11 = sub_100012D98(&_swiftEmptyArrayStorage);
    v12 = sub_1000B4758();
    v13 = _BridgedStoredNSError.init(_:userInfo:)(v9, v11, v10, v12);
    URLError._nsError.getter(v13);
    v14 = (*(__int64 (__fastcall **)(__int64, __int64))(v8 + 8))(v7, v10);
    swift_willThrow(v14);
    v16 = *(_QWORD *)(v1 + 48);
    v15 = *(_QWORD *)(v1 + 56);
    v17 = *(_QWORD *)(v1 + 24);
    swift_task_dealloc(*(_QWORD *)(v1 + 80));
    swift_task_dealloc(v15);
    swift_task_dealloc(v16);
    swift_task_dealloc(v17);
    return (*(__int64 (**)(void))(v1 + 8))();
  }
  (*(void (__fastcall **)(_QWORD, _QWORD, _QWORD))(*(_QWORD *)(v1 + 72) + 32LL))(
    *(_QWORD *)(v1 + 80),
    *(_QWORD *)(v1 + 56),
    *(_QWORD *)(v1 + 64));
  *(_QWORD *)(v1 + 88) = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(
                                                                               &OBJC_CLASS___NSURLSession,
                                                                               v19), "sharedSession"));
  v20 = (_QWORD *)swift_task_alloc(async function pointer to NSURLSession.data(from:delegate:)[1]);
  *(_QWORD *)(v1 + 96) = v20;
  *v20 = v1;
  v20[1] = sub_1000B1224;
  return NSURLSession.data(from:delegate:)(*(_QWORD *)(v1 + 80), 0);
}

/* ========================================================================
 * sub_1000B1394
 * EA: 0x1000b1394
 ======================================================================== */

__int64 sub_1000B1394()
{
  __int64 v0; // x20
  _QWORD *v1; // x22
  __int64 v2; // x0
  __int64 v3; // x0
  __int64 v4; // x8
  __int64 v5; // x0
  __int64 v6; // x0
  __int64 v7; // x8

  v1[2] = v0;
  v2 = type metadata accessor for URLError.Code(0);
  v1[3] = swift_task_alloc((*(_QWORD *)(*(_QWORD *)(v2 - 8) + 64LL) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v3 = type metadata accessor for URLError(0);
  v1[4] = v3;
  v4 = *(_QWORD *)(v3 - 8);
  v1[5] = v4;
  v1[6] = swift_task_alloc((*(_QWORD *)(v4 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = sub_100004DF0(&unk_10033C408, &unk_10025B750);
  v1[7] = swift_task_alloc((*(_QWORD *)(*(_QWORD *)(v5 - 8) + 64LL) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v6 = type metadata accessor for URL(0);
  v1[8] = v6;
  v7 = *(_QWORD *)(v6 - 8);
  v1[9] = v7;
  v1[10] = swift_task_alloc((*(_QWORD *)(v7 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  return swift_task_switch(sub_1000B1468, 0, 0);
}

/* ========================================================================
 * sub_1000B1468
 * EA: 0x1000b1468
 ======================================================================== */

__int64 sub_1000B1468()
{
  __int64 v0; // x22
  __int64 v1; // x20
  __int64 v2; // x21
  __int64 v3; // x19
  __int64 v4; // x19
  __int64 v5; // x21
  __int64 v6; // x24
  __int64 v7; // x20
  __int64 v8; // x0
  __int64 v9; // x23
  __int64 v10; // x0
  __int64 v11; // x0
  __int64 v12; // x0
  __int64 v13; // x19
  __int64 v14; // x21
  __int64 v15; // x23
  __int64 v17; // x1
  _QWORD *v18; // x0

  v1 = *(_QWORD *)(v0 + 64);
  v2 = *(_QWORD *)(v0 + 72);
  v3 = *(_QWORD *)(v0 + 56);
  URL.init(string:)(*(_QWORD *)(*(_QWORD *)(v0 + 16) + 48LL), *(_QWORD *)(*(_QWORD *)(v0 + 16) + 56LL));
  if ( (*(unsigned int (__fastcall **)(__int64, __int64, __int64))(v2 + 48))(v3, 1, v1) == 1 )
  {
    v4 = *(_QWORD *)(v0 + 48);
    v5 = *(_QWORD *)(v0 + 32);
    v6 = *(_QWORD *)(v0 + 40);
    v7 = *(_QWORD *)(v0 + 24);
    v8 = sub_10000C910(*(_QWORD *)(v0 + 56), &unk_10033C408, &unk_10025B750);
    static URLError.Code.badURL.getter(v8);
    v9 = sub_100012D98(&_swiftEmptyArrayStorage);
    v10 = sub_1000B4758();
    v11 = _BridgedStoredNSError.init(_:userInfo:)(v7, v9, v5, v10);
    URLError._nsError.getter(v11);
    v12 = (*(__int64 (__fastcall **)(__int64, __int64))(v6 + 8))(v4, v5);
    swift_willThrow(v12);
    v14 = *(_QWORD *)(v0 + 48);
    v13 = *(_QWORD *)(v0 + 56);
    v15 = *(_QWORD *)(v0 + 24);
    swift_task_dealloc(*(_QWORD *)(v0 + 80));
    swift_task_dealloc(v13);
    swift_task_dealloc(v14);
    swift_task_dealloc(v15);
    return (*(__int64 (**)(void))(v0 + 8))();
  }
  else
  {
    (*(void (__fastcall **)(_QWORD, _QWORD, _QWORD))(*(_QWORD *)(v0 + 72) + 32LL))(
      *(_QWORD *)(v0 + 80),
      *(_QWORD *)(v0 + 56),
      *(_QWORD *)(v0 + 64));
    *(_QWORD *)(v0 + 88) = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(
                                                                                 &OBJC_CLASS___NSURLSession,
                                                                                 v17), "sharedSession"));
    v18 = (_QWORD *)swift_task_alloc(async function pointer to NSURLSession.data(from:delegate:)[1]);
    *(_QWORD *)(v0 + 96) = v18;
    *v18 = v0;
    v18[1] = sub_1000B0F28;
    return NSURLSession.data(from:delegate:)(*(_QWORD *)(v0 + 80), 0);
  }
}

/* ========================================================================
 * sub_1000B15F8
 * EA: 0x1000b15f8
 ======================================================================== */

__int64 __fastcall sub_1000B15F8(__int64 a1, __int64 (__fastcall *a2)(__int64))
{
  __int64 v4; // x21
  __int64 v5; // x0
  __int64 v6; // x20
  void *v7; // x1
  void *v8; // x21
  __int64 v9; // x23
  __int64 v10; // x24
  Swift::String v11; // x0
  __int64 v12; // x0
  unsigned __int64 v13; // x20
  unsigned __int64 v14; // x21
  __int64 v15; // x23
  __int64 v16; // x0
  __int64 *v18; // x8
  unsigned __int64 v19; // x23
  __int64 v20; // x20
  __int64 v21; // x21
  __int64 v22; // x0
  __int64 v23; // x24
  __int64 v24; // x0
  __int64 v25; // x0
  __int64 v26; // x20
  __int64 v27; // x0
  __int64 v28; // x20
  __int64 v29; // x21
  __int64 v30; // x19
  __int64 v31; // x20
  __int64 v32; // x0
  __int128 v33; // [xsp+10h] [xbp-C0h] BYREF
  _BYTE v34[32]; // [xsp+20h] [xbp-B0h]
  unsigned __int64 v35; // [xsp+40h] [xbp-90h]
  unsigned __int64 v36; // [xsp+48h] [xbp-88h]
  __int128 v37; // [xsp+50h] [xbp-80h] BYREF
  _BYTE v38[25]; // [xsp+60h] [xbp-70h]
  char v39; // [xsp+79h] [xbp-57h]

  v4 = sub_100004DF0(&unk_10033C278, &unk_10025BE40);
  sub_10000C8C8(a1 + *(int *)(v4 + 52), &v37, &unk_10033C270, &unk_10025B4E0);
  if ( (v39 & 1) != 0 )
  {
    v33 = v37;
    *(_OWORD *)v34 = *(_OWORD *)v38;
    *(_OWORD *)&v34[9] = *(_OWORD *)&v38[9];
    _StringGuts.grow(_:)(37);
    v5 = swift_bridgeObjectRelease(0xE000000000000000LL);
    v35 = 0xD000000000000023LL;
    v36 = 0x8000000100275DD0LL;
    v6 = sub_100017E08(v5);
    v8 = v7;
    sub_10001D888(&v33);
    if ( v8 )
      v9 = v6;
    else
      v9 = 0;
    if ( !v8 )
      v8 = (void *)0xE000000000000000LL;
    v10 = type metadata accessor for VTLogger(0);
    v11._countAndFlagsBits = v9;
    v11._object = v8;
    String.append(_:)(v11);
    v12 = swift_bridgeObjectRelease(v8);
    v13 = v35;
    v14 = v36;
    v15 = static os_log_type_t.error.getter(v12);
    v16 = static os_log_type_t.error.getter(v15);
    sub_100091CD8(v15, v16, v13, v14, v10);
    swift_bridgeObjectRelease(v14);
    return a2(0);
  }
  else
  {
    v18 = (__int64 *)(a1 + *(int *)(v4 + 48));
    v19 = v18[1];
    if ( v19 >> 60 == 15 )
    {
      v20 = type metadata accessor for VTLogger(0);
      v21 = static os_log_type_t.error.getter(v20);
      v22 = static os_log_type_t.error.getter(v21);
      sub_100091CD8(v21, v22, 0xD000000000000024LL, 0x8000000100275E00LL, v20);
      a2(0);
    }
    else
    {
      v23 = *v18;
      v24 = type metadata accessor for JSONDecoder(0);
      swift_allocObject(v24, *(unsigned int *)(v24 + 48), *(unsigned __int16 *)(v24 + 52));
      v25 = sub_10000C63C(v23, v19);
      v26 = JSONDecoder.init()(v25);
      v27 = sub_1000B47D4();
      dispatch thunk of JSONDecoder.decode<A>(_:from:)(
        &v33,
        &type metadata for FirmwareList,
        v23,
        v19,
        &type metadata for FirmwareList,
        v27);
      swift_release(v26);
      v28 = *((_QWORD *)&v33 + 1);
      v29 = *(_QWORD *)&v34[24];
      swift_bridgeObjectRelease(*(_QWORD *)&v34[16]);
      swift_bridgeObjectRelease(v28);
      a2(v29);
      swift_bridgeObjectRelease(v29);
      v30 = type metadata accessor for VTLogger(0);
      v31 = static os_log_type_t.info.getter(v30);
      v32 = static os_log_type_t.info.getter(v31);
      sub_100091CD8(v31, v32, 0xD00000000000001ELL, 0x8000000100275E60LL, v30);
      sub_10000C9A8(v23, v19);
    }
    return sub_10000C910(&v37, &unk_10033C270, &unk_10025B4E0);
  }
}

/* ========================================================================
 * sub_1000B1980
 * EA: 0x1000b1980
 ======================================================================== */

__int64 __fastcall sub_1000B1980(__int64 a1)
{
  __int64 v2; // x20
  __int64 v3; // x23
  __int64 v4; // x21
  __int64 v5; // x8
  __int64 v6; // x24
  __int64 v7; // x19
  _QWORD v9[2]; // [xsp+0h] [xbp-30h] BYREF

  v2 = sub_100004DF0(&unk_10033EF70, &unk_10025F5C8);
  v3 = *(_QWORD *)(v2 - 8);
  v4 = *(_QWORD *)(v3 + 64);
  (*(void (__fastcall **)(char *, __int64, __int64))(v3 + 16))((char *)v9 - ((v4 + 15) & 0xFFFFFFFFFFFFFFF0LL), a1, v2);
  v5 = *(unsigned __int8 *)(v3 + 80);
  v6 = (v5 + 16) & ~v5;
  v7 = swift_allocObject(&unk_1002E4058, v6 + v4, v5 | 7);
  (*(void (__fastcall **)(__int64, char *, __int64))(v3 + 32))(
    v7 + v6,
    (char *)v9 - ((v4 + 15) & 0xFFFFFFFFFFFFFFF0LL),
    v2);
  sub_1000B2F34(sub_1000B4E6C, v7);
  return swift_release(v7);
}

/* ========================================================================
 * sub_1000B1AA4
 * EA: 0x1000b1aa4
 ======================================================================== */

__int64 __fastcall sub_1000B1AA4(__int64 a1)
{
  __int64 *v1; // x20
  __int64 v2; // x21
  __int64 *v3; // x23
  __int64 v5; // x19
  __int64 v6; // x27
  char *v7; // x22
  __int64 v8; // x25
  __int64 v9; // x26
  __int64 v10; // x0
  __int64 v11; // x0
  __int64 v12; // x1
  __int64 v13; // x0
  __int64 v14; // x0
  __int64 v15; // x1
  __int64 v16; // x20
  __int64 v17; // x0
  __int64 v19; // [xsp+0h] [xbp-60h] BYREF
  char v20; // [xsp+Fh] [xbp-51h] BYREF
  __int64 v21; // [xsp+10h] [xbp-50h] BYREF
  char v22; // [xsp+1Dh] [xbp-43h] BYREF
  char v23; // [xsp+1Eh] [xbp-42h] BYREF
  char v24; // [xsp+1Fh] [xbp-41h] BYREF

  v3 = v1;
  v5 = sub_100004DF0(&unk_10033EFE0, &unk_10025F898);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = (char *)&v19 - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v8 = *(_QWORD *)(a1 + 24);
  v9 = *(_QWORD *)(a1 + 32);
  sub_10000C8A4(a1, v8);
  v10 = sub_1000B4C54();
  dispatch thunk of Encoder.container<A>(keyedBy:)(
    &type metadata for FirmwareList.CodingKeys,
    &type metadata for FirmwareList.CodingKeys,
    v10,
    v8,
    v9);
  v11 = *v3;
  v12 = v3[1];
  v24 = 0;
  KeyedEncodingContainer.encode(_:forKey:)(v11, v12, &v24, v5);
  if ( !v2 )
  {
    v13 = v3[2];
    v23 = 1;
    KeyedEncodingContainer.encode(_:forKey:)(v13, &v23, v5);
    v14 = v3[3];
    v15 = v3[4];
    v22 = 2;
    KeyedEncodingContainer.encode(_:forKey:)(v14, v15, &v22, v5);
    v21 = v3[5];
    v20 = 3;
    v16 = sub_100004DF0(&unk_10033EFC8, &unk_10025F890);
    v17 = sub_1000B4CD4(&unk_10033EFE8, sub_1000B4D44, &protocol conformance descriptor for <A> [A]);
    KeyedEncodingContainer.encodeIfPresent<A>(_:forKey:)(&v21, &v20, v5, v16, v17);
  }
  return (*(__int64 (__fastcall **)(char *, __int64))(v6 + 8))(v7, v5);
}

/* ========================================================================
 * sub_1000B20B0
 * EA: 0x1000b20b0
 ======================================================================== */

__int64 __fastcall sub_1000B20B0(__int64 a1, __int64 a2, __int64 a3, __int64 a4)
{
  __int64 v4; // x21
  __int64 v8; // x19
  __int64 v9; // x23
  char *v10; // x22
  __int64 v11; // x27
  __int64 v12; // x28
  __int64 v13; // x0
  __int64 v14; // x20
  __int64 v15; // x0
  __int64 v17; // [xsp+0h] [xbp-60h] BYREF
  __int64 v18; // [xsp+8h] [xbp-58h] BYREF
  char v19; // [xsp+1Eh] [xbp-42h] BYREF
  char v20; // [xsp+1Fh] [xbp-41h] BYREF

  v17 = a4;
  v8 = sub_100004DF0(&unk_10033EF60, &unk_10025F5A0);
  v9 = *(_QWORD *)(v8 - 8);
  v10 = (char *)&v17 - ((*(_QWORD *)(v9 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v11 = *(_QWORD *)(a1 + 24);
  v12 = *(_QWORD *)(a1 + 32);
  sub_10000C8A4(a1, v11);
  v13 = sub_1000B46B0();
  dispatch thunk of Encoder.container<A>(keyedBy:)(
    &type metadata for Metadata.CodingKeys,
    &type metadata for Metadata.CodingKeys,
    v13,
    v11,
    v12);
  v20 = 0;
  KeyedEncodingContainer.encode(_:forKey:)(a2, a3, &v20, v8);
  if ( !v4 )
  {
    v18 = v17;
    v19 = 1;
    v14 = sub_100004DF0(&unk_10033BDE8, &unk_10025C710);
    v15 = sub_1000B46F0(&unk_10033EF68, &protocol witness table for UInt8, &protocol conformance descriptor for <A> [A]);
    KeyedEncodingContainer.encode<A>(_:forKey:)(&v18, &v19, v8, v14, v15);
  }
  return (*(__int64 (__fastcall **)(char *, __int64))(v9 + 8))(v10, v8);
}

/* ========================================================================
 * sub_1000B229C
 * EA: 0x1000b229c
 ======================================================================== */

__int64 sub_1000B229C()
{
  return 0x656D614E676B70LL;
}

/* ========================================================================
 * sub_1000B2420
 * EA: 0x1000b2420
 ======================================================================== */

__int64 __fastcall sub_1000B2420(__int64 a1)
{
  __int64 *v1; // x20
  __int64 v2; // x21
  __int64 *v3; // x23
  __int64 v5; // x19
  __int64 v6; // x27
  char *v7; // x22
  __int64 v8; // x25
  __int64 v9; // x26
  __int64 v10; // x0
  __int64 v11; // x0
  __int64 v12; // x1
  __int64 v13; // x0
  __int64 v14; // x1
  __int64 v15; // x0
  __int64 v16; // x1
  __int64 v17; // x0
  __int64 v18; // x1
  __int64 v19; // x0
  __int64 v20; // x1
  __int64 v21; // x0
  __int64 v22; // x1
  __int64 v23; // x0
  __int64 v24; // x1
  __int64 v25; // x0
  __int64 v26; // x1
  __int64 v27; // x0
  __int64 v28; // x1
  __int64 v29; // x0
  __int64 v30; // x1
  __int64 v31; // x0
  __int64 v32; // x1
  __int64 v33; // x0
  __int64 v34; // x1
  __int64 v35; // x0
  __int64 v36; // x1
  __int64 v37; // x0
  __int64 v38; // x0
  __int64 v39; // x0
  __int64 v40; // x1
  __int64 v41; // x0
  __int64 v42; // x0
  __int64 v44; // [xsp+0h] [xbp-70h] BYREF
  char v45; // [xsp+Fh] [xbp-61h] BYREF
  __int128 v46; // [xsp+10h] [xbp-60h] BYREF
  __int64 v47; // [xsp+20h] [xbp-50h]

  v3 = v1;
  v5 = sub_100004DF0(&unk_10033EF38, &unk_10025F590);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = (char *)&v44 - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v8 = *(_QWORD *)(a1 + 24);
  v9 = *(_QWORD *)(a1 + 32);
  sub_10000C8A4(a1, v8);
  v10 = sub_1000B45F0();
  dispatch thunk of Encoder.container<A>(keyedBy:)(
    &type metadata for Firmware.CodingKeys,
    &type metadata for Firmware.CodingKeys,
    v10,
    v8,
    v9);
  v11 = *v3;
  v12 = v3[1];
  LOBYTE(v46) = 0;
  KeyedEncodingContainer.encode(_:forKey:)(v11, v12, &v46, v5);
  if ( !v2 )
  {
    v13 = v3[2];
    v14 = v3[3];
    LOBYTE(v46) = 1;
    KeyedEncodingContainer.encode(_:forKey:)(v13, v14, &v46, v5);
    v15 = v3[4];
    v16 = v3[5];
    LOBYTE(v46) = 2;
    KeyedEncodingContainer.encode(_:forKey:)(v15, v16, &v46, v5);
    v17 = v3[6];
    v18 = v3[7];
    LOBYTE(v46) = 3;
    KeyedEncodingContainer.encode(_:forKey:)(v17, v18, &v46, v5);
    v19 = v3[8];
    v20 = v3[9];
    LOBYTE(v46) = 4;
    KeyedEncodingContainer.encodeIfPresent(_:forKey:)(v19, v20, &v46, v5);
    v21 = v3[10];
    v22 = v3[11];
    LOBYTE(v46) = 5;
    KeyedEncodingContainer.encode(_:forKey:)(v21, v22, &v46, v5);
    v23 = v3[12];
    v24 = v3[13];
    LOBYTE(v46) = 6;
    KeyedEncodingContainer.encode(_:forKey:)(v23, v24, &v46, v5);
    v25 = v3[14];
    v26 = v3[15];
    LOBYTE(v46) = 7;
    KeyedEncodingContainer.encodeIfPresent(_:forKey:)(v25, v26, &v46, v5);
    v27 = v3[16];
    v28 = v3[17];
    LOBYTE(v46) = 8;
    KeyedEncodingContainer.encodeIfPresent(_:forKey:)(v27, v28, &v46, v5);
    v29 = v3[18];
    v30 = v3[19];
    LOBYTE(v46) = 9;
    KeyedEncodingContainer.encodeIfPresent(_:forKey:)(v29, v30, &v46, v5);
    v31 = v3[20];
    v32 = v3[21];
    LOBYTE(v46) = 10;
    KeyedEncodingContainer.encodeIfPresent(_:forKey:)(v31, v32, &v46, v5);
    v33 = v3[22];
    v34 = v3[23];
    LOBYTE(v46) = 11;
    KeyedEncodingContainer.encode(_:forKey:)(v33, v34, &v46, v5);
    v35 = v3[24];
    v36 = v3[25];
    LOBYTE(v46) = 12;
    KeyedEncodingContainer.encode(_:forKey:)(v35, v36, &v46, v5);
    v37 = v3[26];
    LOBYTE(v46) = 13;
    KeyedEncodingContainer.encode(_:forKey:)(v37, &v46, v5);
    v38 = v3[27];
    LOBYTE(v46) = 14;
    KeyedEncodingContainer.encode(_:forKey:)(v38, &v46, v5);
    v39 = v3[28];
    v40 = v3[29];
    LOBYTE(v46) = 15;
    v41 = KeyedEncodingContainer.encode(_:forKey:)(v39, v40, &v46, v5);
    v46 = *((_OWORD *)v3 + 15);
    v47 = v3[32];
    v45 = 16;
    v42 = sub_1000B4670(v41);
    KeyedEncodingContainer.encodeIfPresent<A>(_:forKey:)(&v46, &v45, v5, &type metadata for Metadata, v42);
  }
  return (*(__int64 (__fastcall **)(char *, __int64))(v6 + 8))(v7, v5);
}

/* ========================================================================
 * sub_1000B2980
 * EA: 0x1000b2980
 ======================================================================== */

__int64 sub_1000B2980()
{
  __int64 v0; // x19
  __int64 v1; // x8
  __int64 v2; // x0
  __int64 v3; // x26
  unsigned __int64 v4; // x0
  __int64 v5; // x1
  _QWORD *v6; // x9
  __int64 v7; // x8
  __int64 *v8; // x10
  __int64 v9; // x11
  void *v10; // x23
  id v11; // x20
  __int64 v12; // x21
  __int64 v13; // x1
  Swift::String v14; // x0
  Swift::String v15; // x0
  void *object; // x25
  Swift::String v17; // x0
  Swift::String v18; // x0
  void *v19; // x24
  __int64 v20; // x21
  __int64 v21; // x27
  __int64 v22; // x0
  __int64 inited; // x24
  __int64 v24; // x0
  __int64 v25; // x25
  id v26; // x23
  id v27; // x25
  __int64 v28; // x23
  __int64 v29; // x1
  __int64 v30; // x26
  __int64 v31; // x19
  __int64 v32; // x0
  __int64 v34; // x8
  __int64 *v35; // x9
  __int64 v36; // x8
  __int64 v37; // t1
  unsigned __int64 v38; // [xsp+8h] [xbp-2D8h]
  __int64 v39; // [xsp+10h] [xbp-2D0h]
  unsigned __int64 v40; // [xsp+18h] [xbp-2C8h]
  __int64 v41; // [xsp+20h] [xbp-2C0h]
  unsigned __int64 v42; // [xsp+28h] [xbp-2B8h]
  __int64 v43; // [xsp+30h] [xbp-2B0h]
  _BYTE v44[560]; // [xsp+38h] [xbp-2A8h] BYREF
  __int64 v45; // [xsp+268h] [xbp-78h]
  __int64 v46; // [xsp+270h] [xbp-70h]
  __int64 v47; // [xsp+278h] [xbp-68h]
  __int64 v48; // [xsp+280h] [xbp-60h]

  if ( qword_100339480 != -1 )
    swift_once(&qword_100339480, sub_1000FE3CC);
  v0 = sub_1000FF000();
  if ( sub_1000FF000() != 4353
    && sub_1000FF000() != 4356
    && sub_1000FF000() != 4352
    && sub_1000FF000() != 4355
    && sub_1000FF000() != 514 )
  {
    if ( qword_100339358 != -1 )
      swift_once(&qword_100339358, sub_100073E7C);
    v34 = *(_QWORD *)(qword_100344470 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_glasses);
    v35 = (__int64 *)(v34 + 56);
    v36 = *(_QWORD *)(v34 + 16) + 1LL;
    while ( --v36 )
    {
      v37 = *v35;
      v35 += 4;
      if ( v37 == v0 )
      {
        if ( (v0 & 0x8000000000000001LL) != 1 )
          goto LABEL_13;
        goto LABEL_16;
      }
    }
    goto LABEL_13;
  }
  if ( sub_1000FF000() == 4352 )
  {
    v1 = qword_100339358;
    goto LABEL_11;
  }
  v2 = sub_1000FF000();
  v1 = qword_100339358;
  if ( v2 == 4355 )
  {
LABEL_11:
    if ( v1 != -1 )
      swift_once(&qword_100339358, sub_100073E7C);
LABEL_13:
    v39 = 0;
    v3 = qword_100344470;
    v4 = *(_QWORD *)(qword_100344470 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_glassVersion + 8);
    v42 = v4;
    v43 = *(_QWORD *)(qword_100344470 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_glassVersion);
    v38 = 0xE000000000000000LL;
    goto LABEL_17;
  }
  if ( qword_100339358 != -1 )
    swift_once(&qword_100339358, sub_100073E7C);
LABEL_16:
  v43 = 0;
  v3 = qword_100344470;
  v4 = *(_QWORD *)(qword_100344470 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_glassVersion + 8);
  v38 = v4;
  v39 = *(_QWORD *)(qword_100344470 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_glassVersion);
  v42 = 0xE000000000000000LL;
LABEL_17:
  swift_bridgeObjectRetain(v4);
  v6 = *(_QWORD **)(v3 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_glasses);
  v7 = v6[2] + 1LL;
  while ( --v7 )
  {
    v8 = v6 + 4;
    v9 = v6[7];
    v6 += 4;
    if ( v9 == v0 )
    {
      v40 = v8[1];
      v41 = *v8;
      swift_bridgeObjectRetain(v40);
      goto LABEL_22;
    }
  }
  v40 = 0xE000000000000000LL;
  v41 = 0;
LABEL_22:
  v10 = (void *)objc_opt_self(&OBJC_CLASS___NSProcessInfo, v5);
  v11 = objc_retainAutoreleasedReturnValue(objc_msgSend(v10, "processInfo"));
  objc_msgSend(v11, "operatingSystemVersion");
  v12 = v48;
  objc_release(v11);
  v46 = dispatch thunk of CustomStringConvertible.description.getter(
          &type metadata for Int,
          &protocol witness table for Int);
  v47 = v13;
  v14._countAndFlagsBits = 46;
  v14._object = (void *)0xE100000000000000LL;
  String.append(_:)(v14);
  v15._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                             &type metadata for Int,
                             &protocol witness table for Int);
  object = v15._object;
  String.append(_:)(v15);
  swift_bridgeObjectRelease(object);
  v17._countAndFlagsBits = 46;
  v17._object = (void *)0xE100000000000000LL;
  String.append(_:)(v17);
  v45 = v12;
  v18._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                             &type metadata for Int,
                             &protocol witness table for Int);
  v19 = v18._object;
  String.append(_:)(v18);
  swift_bridgeObjectRelease(v19);
  v21 = v46;
  v20 = v47;
  v22 = sub_100004DF0(&unk_10033C490, &unk_10025B820);
  inited = swift_initStackObject(v22, v44);
  *(_OWORD *)(inited + 16) = xmmword_10025F9B0;
  *(_QWORD *)(inited + 32) = 0x6973726556727563LL;
  *(_QWORD *)(inited + 40) = 0xEA00000000006E6FLL;
  *(_QWORD *)(inited + 48) = 48;
  *(_QWORD *)(inited + 56) = 0xE100000000000000LL;
  *(_QWORD *)(inited + 72) = &type metadata for String;
  strcpy((char *)(inited + 80), "curSubVersion");
  *(_WORD *)(inited + 94) = -4864;
  v24 = *(_QWORD *)(v3 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_glass7911FWVersion + 8);
  *(_QWORD *)(inited + 96) = *(_QWORD *)(v3 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_glass7911FWVersion);
  *(_QWORD *)(inited + 104) = v24;
  *(_QWORD *)(inited + 120) = &type metadata for String;
  *(_QWORD *)(inited + 128) = 0x4E6E6F6973726576LL;
  *(_QWORD *)(inited + 136) = 0xEB00000000656D61LL;
  *(_QWORD *)(inited + 144) = v39;
  *(_QWORD *)(inited + 152) = v38;
  *(_QWORD *)(inited + 168) = &type metadata for String;
  *(_QWORD *)(inited + 176) = 20051;
  *(_QWORD *)(inited + 184) = 0xE200000000000000LL;
  v25 = *(_QWORD *)(v3 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_glassSN + 8);
  *(_QWORD *)(inited + 192) = *(_QWORD *)(v3 + OBJC_IVAR____TtC11SpaceWalker7VTGlass_glassSN);
  *(_QWORD *)(inited + 200) = v25;
  *(_QWORD *)(inited + 216) = &type metadata for String;
  *(_QWORD *)(inited + 224) = 0x65707954736FLL;
  *(_QWORD *)(inited + 232) = 0xE600000000000000LL;
  swift_bridgeObjectRetain(v24);
  swift_bridgeObjectRetain(v25);
  v26 = objc_retainAutoreleasedReturnValue(objc_msgSend(v10, "processInfo"));
  v27 = objc_retainAutoreleasedReturnValue(objc_msgSend(v26, "operatingSystemVersionString"));
  objc_release(v26);
  v28 = static String._unconditionallyBridgeFromObjectiveC(_:)(v27);
  v30 = v29;
  objc_release(v27);
  *(_QWORD *)(inited + 240) = v28;
  *(_QWORD *)(inited + 248) = v30;
  *(_QWORD *)(inited + 264) = &type metadata for String;
  *(_QWORD *)(inited + 272) = 0x6F6973726556736FLL;
  *(_QWORD *)(inited + 280) = 0xE90000000000006ELL;
  *(_QWORD *)(inited + 288) = v21;
  *(_QWORD *)(inited + 296) = v20;
  *(_QWORD *)(inited + 312) = &type metadata for String;
  *(_QWORD *)(inited + 320) = 1701869940;
  *(_QWORD *)(inited + 328) = 0xE400000000000000LL;
  *(_QWORD *)(inited + 336) = 49;
  *(_QWORD *)(inited + 344) = 0xE100000000000000LL;
  *(_QWORD *)(inited + 360) = &type metadata for String;
  *(_QWORD *)(inited + 368) = 0x6C65646F6DLL;
  *(_QWORD *)(inited + 376) = 0xE500000000000000LL;
  *(_QWORD *)(inited + 384) = v41;
  *(_QWORD *)(inited + 392) = v40;
  *(_QWORD *)(inited + 408) = &type metadata for String;
  *(_QWORD *)(inited + 416) = 6580592;
  *(_QWORD *)(inited + 424) = 0xE300000000000000LL;
  *(_QWORD *)(inited + 432) = v0;
  *(_QWORD *)(inited + 456) = &type metadata for Int;
  *(_QWORD *)(inited + 464) = 0x73726556746F6F62LL;
  *(_QWORD *)(inited + 472) = 0xEB000000006E6F69LL;
  *(_QWORD *)(inited + 480) = v43;
  *(_QWORD *)(inited + 488) = v42;
  *(_QWORD *)(inited + 504) = &type metadata for String;
  *(_QWORD *)(inited + 512) = 0x654C657461647075LL;
  *(_QWORD *)(inited + 520) = 0xEB000000006C6576LL;
  *(_QWORD *)(inited + 552) = &type metadata for Int;
  *(_QWORD *)(inited + 528) = 1;
  v31 = sub_100012D98(inited);
  swift_setDeallocating(inited);
  v32 = sub_100004DF0(&unk_10033C120, &unk_10025B290);
  swift_arrayDestroy(inited + 32, 11, v32);
  return v31;
}

/* ========================================================================
 * sub_1000B2F34
 * EA: 0x1000b2f34
 ======================================================================== */

__int64 __fastcall sub_1000B2F34(__int64 a1, __int64 a2)
{
  __int64 v3; // x21
  __int64 v4; // x22
  __int64 v5; // x0
  __int64 v6; // x22
  _QWORD *v7; // x26
  __int64 v8; // x23
  __int64 v9; // x24
  __int64 v10; // x0
  __int64 v11; // x0
  __int64 inited; // x21
  __int64 v13; // x25
  __int64 v14; // x21
  __int64 v15; // x25
  __int64 isUniquelyReferenced_nonNull_native; // x0
  __int64 v17; // x27
  __int64 v18; // x0
  __int64 v19; // x27
  __int64 v20; // x3
  char v21; // w21
  char v22; // w26
  __int64 v23; // x21
  __int64 v24; // x0
  _BYTE v27[120]; // [xsp+8h] [xbp-1C8h] BYREF
  _QWORD v28[2]; // [xsp+80h] [xbp-150h] BYREF
  char v29; // [xsp+90h] [xbp-140h]
  __int64 v30; // [xsp+98h] [xbp-138h]
  __int64 v31; // [xsp+A0h] [xbp-130h]
  char v32; // [xsp+A8h] [xbp-128h]
  __int64 v33; // [xsp+B0h] [xbp-120h]
  __int64 v34; // [xsp+B8h] [xbp-118h]
  __int64 v35; // [xsp+C0h] [xbp-110h]
  __int128 v36; // [xsp+C8h] [xbp-108h]
  __int64 v37; // [xsp+D8h] [xbp-F8h]
  __int64 v38; // [xsp+E0h] [xbp-F0h]
  char v39; // [xsp+E8h] [xbp-E8h]
  __int64 v40; // [xsp+F0h] [xbp-E0h]
  _QWORD v41[2]; // [xsp+100h] [xbp-D0h] BYREF
  char v42; // [xsp+110h] [xbp-C0h]
  __int64 v43; // [xsp+118h] [xbp-B8h]
  __int64 v44; // [xsp+120h] [xbp-B0h]
  char v45; // [xsp+128h] [xbp-A8h]
  __int64 v46; // [xsp+130h] [xbp-A0h]
  __int64 v47; // [xsp+138h] [xbp-98h]
  __int64 v48; // [xsp+140h] [xbp-90h]
  __int128 v49; // [xsp+148h] [xbp-88h]
  __int64 v50; // [xsp+158h] [xbp-78h]
  __int64 v51; // [xsp+160h] [xbp-70h]
  char v52; // [xsp+168h] [xbp-68h]
  __int64 v53; // [xsp+170h] [xbp-60h]

  v3 = type metadata accessor for VTLogger(0);
  v4 = static os_log_type_t.info.getter(v3);
  v5 = static os_log_type_t.info.getter(v4);
  sub_100091CD8(v4, v5, 0xD00000000000001CLL, 0x8000000100275D90LL, v3);
  v6 = sub_1000B2980();
  v7 = (_QWORD *)HTTPMethod.post.unsafeMutableAddressor();
  v8 = *v7;
  v9 = v7[1];
  v10 = swift_bridgeObjectRetain(v9);
  HTTPMethod.get.unsafeMutableAddressor(v10);
  v11 = sub_100004DF0(&unk_10033C4A0, &unk_10025BE30);
  inited = swift_initStaticObject(v11, &unk_10033A5C8);
  swift_bridgeObjectRetain(v9);
  swift_bridgeObjectRetain(v6);
  v13 = sub_100012EC4(inited);
  sub_10000C910(inited + 32, &unk_10033C4A8, &unk_10025B840);
  swift_bridgeObjectRelease(v13);
  v14 = sub_100012EC4(&_swiftEmptyArrayStorage);
  v15 = sub_10000FE60();
  swift_bridgeObjectRelease(v14);
  swift_bridgeObjectRetain_n(v15, 2);
  isUniquelyReferenced_nonNull_native = swift_isUniquelyReferenced_nonNull_native(v15);
  v41[0] = v15;
  sub_1000D0D24(v6, sub_1000D0814, 0, isUniquelyReferenced_nonNull_native, v41);
  swift_bridgeObjectRelease(v6);
  v17 = v41[0];
  v18 = swift_isUniquelyReferenced_nonNull_native(v41[0]);
  v41[0] = v17;
  sub_1000D0D24(v15, sub_1000D0814, 0, v18, v41);
  swift_bridgeObjectRelease(v15);
  v19 = v41[0];
  v20 = v7[1];
  if ( v8 == *v7 && v9 == v20 )
  {
    swift_bridgeObjectRelease(v9);
LABEL_5:
    v22 = 1;
    goto LABEL_6;
  }
  v21 = _stringCompareWithSmolCheck(_:_:expecting:)(v8, v9, *v7, v20, 0);
  swift_bridgeObjectRelease(v9);
  v22 = 0;
  if ( (v21 & 1) != 0 )
    goto LABEL_5;
LABEL_6:
  sub_10000C9A8(0, 0xF000000000000000LL);
  v41[0] = 0xD00000000000001BLL;
  v41[1] = 0x8000000100275DB0LL;
  v42 = 0;
  v43 = v8;
  v44 = v9;
  v45 = v22;
  v46 = 0x4024000000000000LL;
  v47 = v19;
  v49 = xmmword_10025B7B0;
  v50 = 0;
  v51 = 0;
  v48 = v15;
  v52 = 0;
  v53 = v6;
  v28[0] = 0xD00000000000001BLL;
  v28[1] = 0x8000000100275DB0LL;
  v29 = 0;
  v30 = v8;
  v31 = v9;
  v32 = v22;
  v33 = 0x4024000000000000LL;
  v34 = v19;
  v36 = xmmword_10025B7B0;
  v37 = 0;
  v38 = 0;
  v35 = v15;
  v39 = 0;
  v40 = v6;
  sub_10001A108(v41, v27);
  sub_10001D828(v28);
  v23 = swift_allocObject(&unk_1002E4030, 32, 7);
  *(_QWORD *)(v23 + 16) = a1;
  *(_QWORD *)(v23 + 24) = a2;
  swift_retain(a2);
  v24 = sub_100019F40(v41, sub_1000B47CC, v23);
  swift_release(v24);
  swift_release(v23);
  return sub_10001D828(v41);
}

/* ========================================================================
 * sub_100108EC8
 * EA: 0x100108ec8
 ======================================================================== */

__int64 sub_100108EC8()
{
  _QWORD *v0; // x22
  __int64 v1; // x23
  void *v2; // x20
  id v3; // x19
  __int64 v4; // x0
  __int64 v5; // x21
  __int64 v6; // x8
  __int64 v7; // x0
  id v8; // x20
  __int64 v9; // x0
  __int64 v10; // x19
  __int64 v11; // x0
  __int64 v12; // x19
  __int64 v13; // x0
  __int64 v14; // x0
  __int64 v15; // x1

  v1 = v0[23];
  v2 = (void *)v0[21];
  v0[25] = v2;
  v3 = objc_retainAutoreleasedReturnValue(objc_msgSend(v2, "displays"));
  v4 = sub_10000C9F0(0, &unk_10033C6F8, &classRef_SCDisplay);
  v5 = static Array._unconditionallyBridgeFromObjectiveC(_:)(v3, v4);
  objc_release(v3);
  v6 = OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_availableDisplays;
  v0[26] = OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_availableDisplays;
  v7 = *(_QWORD *)(v1 + v6);
  *(_QWORD *)(v1 + v6) = v5;
  swift_bridgeObjectRelease(v7);
  v8 = objc_retainAutoreleasedReturnValue(objc_msgSend(v2, "applications"));
  v9 = sub_10000C9F0(0, &unk_1003403E0, &classRef_SCRunningApplication);
  v10 = static Array._unconditionallyBridgeFromObjectiveC(_:)(v8, v9);
  objc_release(v8);
  v11 = *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_availableApps);
  *(_QWORD *)(v1 + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_availableApps) = v10;
  swift_bridgeObjectRelease(v11);
  v12 = type metadata accessor for MainActor(0);
  v0[27] = v12;
  v0[28] = static MainActor.shared.getter();
  v13 = sub_1000052F4(
          &qword_10033BD60,
          &type metadata accessor for MainActor,
          &protocol conformance descriptor for MainActor);
  v0[29] = v13;
  v14 = dispatch thunk of Actor.unownedExecutor.getter(v12, v13);
  return swift_task_switch(sub_10010902C, v14, v15);
}

/* ========================================================================
 * sub_100109074
 * EA: 0x100109074
 ======================================================================== */

void sub_100109074()
{
  _QWORD *v0; // x22
  __int64 v1; // x19
  __int64 v2; // x21
  __int64 v3; // x0
  __int64 v4; // x0
  __int64 v5; // x1

  if ( v0[30] )
  {
    v1 = v0[29];
    v2 = v0[27];
    v3 = swift_release(v0[28]);
    v0[31] = static MainActor.shared.getter(v3);
    v4 = dispatch thunk of Actor.unownedExecutor.getter(v2, v1);
    swift_task_switch(sub_1001090EC, v4, v5);
  }
  else
  {
    __break(1u);
  }
}

/* ========================================================================
 * sub_1001090EC
 * EA: 0x1001090ec
 ======================================================================== */

__int64 sub_1001090EC()
{
  _QWORD *v0; // x22
  void *v1; // x20

  v1 = (void *)v0[30];
  swift_release(v0[31]);
  v0[32] = objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "delegate"));
  objc_release(v1);
  return swift_task_switch(sub_10010914C, 0, 0);
}

/* ========================================================================
 * sub_10010914C
 * EA: 0x10010914c
 ======================================================================== */

id sub_10010914C()
{
  __int64 v0; // x22
  __int64 v1; // x20
  __int64 v2; // x0
  __int64 v3; // x0
  id result; // x0
  __int64 v5; // x19
  __int64 v6; // x21
  __int64 v7; // x0
  __int64 v8; // x1
  __int64 v9; // x8
  void *v10; // x0
  __int64 v11; // x20
  void *v12; // x19
  __int64 v13; // x0
  __int64 v14; // x19

  v1 = *(_QWORD *)(v0 + 256);
  if ( v1 )
  {
    v2 = type metadata accessor for AppDelegate(0);
    v3 = swift_dynamicCastClass(v1, v2);
    if ( v3 )
    {
      result = *(id *)(v3 + OBJC_IVAR____TtC11SpaceWalker11AppDelegate_window);
      *(_QWORD *)(v0 + 264) = result;
      if ( result )
      {
        v5 = *(_QWORD *)(v0 + 232);
        v6 = *(_QWORD *)(v0 + 216);
        *(_QWORD *)(v0 + 272) = static MainActor.shared.getter(objc_retain(result));
        v7 = dispatch thunk of Actor.unownedExecutor.getter(v6, v5);
        return (id)swift_task_switch(sub_1001092C0, v7, v8);
      }
      goto LABEL_22;
    }
    swift_unknownObjectRelease(v1);
  }
  v9 = *(_QWORD *)(v0 + 184);
  if ( *(_QWORD *)(v9 + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_selectedDisplay) )
  {
    v10 = *(void **)(v0 + 200);
LABEL_14:
    objc_release(v10);
    return (id)(*(__int64 (**)(void))(v0 + 8))();
  }
  v11 = *(_QWORD *)(v9 + *(_QWORD *)(v0 + 208));
  if ( (unsigned __int64)v11 >> 62 )
  {
    if ( v11 < 0 )
      v13 = *(_QWORD *)(v9 + *(_QWORD *)(v0 + 208));
    else
      v13 = v11 & 0xFFFFFFFFFFFFFF8LL;
    result = (id)_CocoaArrayWrapper.endIndex.getter(v13);
    if ( !result )
      goto LABEL_13;
  }
  else
  {
    result = *(id *)((v11 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
    if ( !result )
    {
LABEL_13:
      v12 = *(void **)(v0 + 200);
      sub_10010786C(result);
      v10 = v12;
      goto LABEL_14;
    }
  }
  if ( (v11 & 0xC000000000000001LL) != 0 )
  {
    swift_bridgeObjectRetain(v11);
    v14 = sub_1000A0258(0, v11);
    swift_bridgeObjectRelease(v11);
    result = (id)v14;
    goto LABEL_13;
  }
  if ( *(_QWORD *)((v11 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
  {
    result = objc_retain(*(id *)(v11 + 32));
    goto LABEL_13;
  }
  __break(1u);
LABEL_22:
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_100109318
 * EA: 0x100109318
 ======================================================================== */

id sub_100109318()
{
  __int64 v0; // x22
  id v1; // x20
  __int64 v2; // x0
  __int64 v3; // x19
  __int64 v4; // x23
  __int64 v5; // x21
  unsigned __int64 v6; // x25
  unsigned __int64 v7; // x26
  __int64 v8; // x20
  unsigned __int64 v9; // x24
  id v10; // x0
  void *v11; // x23
  unsigned int v12; // w0
  unsigned __int64 v13; // x8
  unsigned __int64 v14; // x24
  unsigned __int64 v15; // x9
  __int64 v16; // x0
  __int64 v17; // x22
  id v18; // x0
  void *v19; // x21
  __int64 v20; // x19
  __int64 v21; // x25
  __int64 v22; // x24
  void *v23; // x20
  id v24; // x21
  __int64 v25; // x23
  void *v26; // x0
  id v27; // x0
  __int64 v28; // x0
  Swift::String v29; // x0
  void *object; // x24
  __int64 v31; // x0
  __int64 v32; // x25
  __int64 v33; // x0
  __int64 v34; // x8
  void *v35; // x0
  id result; // x0
  void *v37; // x19
  __int64 v38; // x0
  __int64 v39; // x19
  __int64 v40; // [xsp+10h] [xbp-70h]

  v1 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(v0 + 200), "windows"));
  v2 = sub_10000C9F0(0, &unk_10033BDC0, &classRef_SCWindow);
  v3 = static Array._unconditionallyBridgeFromObjectiveC(_:)(v1, v2);
  objc_release(v1);
  if ( !((unsigned __int64)v3 >> 62) )
  {
    v4 = v3 & 0xFFFFFFFFFFFFFF8LL;
    v5 = *(_QWORD *)((v3 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
    v40 = v0;
    if ( v5 )
      goto LABEL_3;
LABEL_29:
    v8 = (__int64)&_swiftEmptyArrayStorage;
    goto LABEL_30;
  }
LABEL_25:
  v4 = v3 & 0xFFFFFFFFFFFFFF8LL;
  if ( v3 < 0 )
    v16 = v3;
  else
    v16 = v3 & 0xFFFFFFFFFFFFFF8LL;
  v5 = _CocoaArrayWrapper.endIndex.getter(v16);
  v40 = v0;
  if ( !v5 )
    goto LABEL_29;
LABEL_3:
  v6 = 0;
  v7 = *(_QWORD *)(v0 + 280);
  v8 = (__int64)&_swiftEmptyArrayStorage;
  v0 = v4;
  do
  {
    v9 = v6;
    while ( 1 )
    {
      if ( (v3 & 0xC000000000000001LL) != 0 )
      {
        v10 = (id)sub_1000A026C(v9, v3);
      }
      else
      {
        if ( v9 >= *(_QWORD *)(v0 + 16) )
          goto LABEL_22;
        v10 = objc_retain(*(id *)(v3 + 8 * v9 + 32));
      }
      v11 = v10;
      v6 = v9 + 1;
      if ( __OFADD__(v9, 1) )
      {
        __break(1u);
LABEL_22:
        __break(1u);
LABEL_23:
        __break(1u);
LABEL_24:
        __break(1u);
        goto LABEL_25;
      }
      v12 = (unsigned int)objc_msgSend(v10, "windowID");
      if ( (v7 & 0x8000000000000000LL) != 0 )
        goto LABEL_23;
      if ( HIDWORD(v7) )
        goto LABEL_24;
      if ( v12 == (_DWORD)v7 )
        break;
      objc_release(v11);
      ++v9;
      if ( v6 == v5 )
        goto LABEL_30;
    }
    if ( (swift_isUniquelyReferenced_nonNull_native(&_swiftEmptyArrayStorage) & 1) == 0 )
      sub_100039F8C(0, *((_QWORD *)&_swiftEmptyArrayStorage + 2) + 1LL, 1);
    v14 = *((_QWORD *)&_swiftEmptyArrayStorage + 2);
    v13 = *((_QWORD *)&_swiftEmptyArrayStorage + 3);
    v15 = v14 + 1;
    if ( v14 >= v13 >> 1 )
    {
      sub_100039F8C(v13 > 1, v14 + 1, 1);
      v15 = v14 + 1;
    }
    *((_QWORD *)&_swiftEmptyArrayStorage + 2) = v15;
    *((_QWORD *)&_swiftEmptyArrayStorage + v14 + 4) = v11;
  }
  while ( v6 != v5 );
LABEL_30:
  swift_bridgeObjectRelease(v3);
  if ( ((unsigned __int64)&_swiftEmptyArrayStorage & 0x8000000000000000LL) != 0
    || ((unsigned __int64)&_swiftEmptyArrayStorage & 0x4000000000000000LL) != 0 )
  {
    v17 = v40;
    if ( _CocoaArrayWrapper.endIndex.getter(&_swiftEmptyArrayStorage) )
      goto LABEL_33;
LABEL_38:
    swift_release(&_swiftEmptyArrayStorage);
    v19 = nullptr;
    goto LABEL_39;
  }
  v17 = v40;
  if ( !*((_QWORD *)&_swiftEmptyArrayStorage + 2) )
    goto LABEL_38;
LABEL_33:
  if ( ((unsigned __int64)&_swiftEmptyArrayStorage & 0xC000000000000001LL) != 0 )
  {
    v18 = (id)sub_1000A026C(0, &_swiftEmptyArrayStorage);
  }
  else
  {
    if ( !*((_QWORD *)&_swiftEmptyArrayStorage + 2) )
    {
      __break(1u);
      goto LABEL_50;
    }
    v18 = objc_retain(*((id *)&_swiftEmptyArrayStorage + 4));
  }
  v19 = v18;
  swift_release(&_swiftEmptyArrayStorage);
LABEL_39:
  v20 = *(_QWORD *)(v17 + 256);
  v21 = *(_QWORD *)(v17 + 184);
  v22 = OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_mainSCWindow;
  v23 = *(void **)(v21 + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_mainSCWindow);
  *(_QWORD *)(v21 + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_mainSCWindow) = v19;
  v24 = objc_retain(v19);
  objc_release(v23);
  v25 = type metadata accessor for VTLogger(0);
  _StringGuts.grow(_:)(18);
  swift_bridgeObjectRelease(0xE000000000000000LL);
  v26 = *(void **)(v21 + v22);
  *(_QWORD *)(v17 + 80) = v26;
  v27 = objc_retain(v26);
  v28 = sub_100004DF0(&unk_1003403E8, &unk_100261BE8);
  v29._countAndFlagsBits = String.init<A>(describing:)(v17 + 80, v28);
  object = v29._object;
  String.append(_:)(v29);
  v31 = swift_bridgeObjectRelease(object);
  v32 = static os_log_type_t.info.getter(v31);
  v33 = static os_log_type_t.info.getter(v32);
  sub_100091CD8(v32, v33, 0xD000000000000010LL, 0x8000000100279130LL, v25);
  swift_bridgeObjectRelease(0x8000000100279130LL);
  objc_release(v24);
  swift_unknownObjectRelease(v20);
  v34 = *(_QWORD *)(v17 + 184);
  if ( *(_QWORD *)(v34 + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_selectedDisplay) )
  {
    v35 = *(void **)(v17 + 200);
LABEL_47:
    objc_release(v35);
    return (id)(*(__int64 (**)(void))(v17 + 8))();
  }
  v8 = *(_QWORD *)(v34 + *(_QWORD *)(v17 + 208));
  if ( (unsigned __int64)v8 >> 62 )
  {
LABEL_50:
    if ( v8 < 0 )
      v38 = v8;
    else
      v38 = v8 & 0xFFFFFFFFFFFFFF8LL;
    result = (id)_CocoaArrayWrapper.endIndex.getter(v38);
    if ( !result )
      goto LABEL_46;
    goto LABEL_43;
  }
  result = *(id *)((v8 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
  if ( !result )
  {
LABEL_46:
    v37 = *(void **)(v17 + 200);
    sub_10010786C(result);
    v35 = v37;
    goto LABEL_47;
  }
LABEL_43:
  if ( (v8 & 0xC000000000000001LL) != 0 )
  {
    swift_bridgeObjectRetain(v8);
    v39 = sub_1000A0258(0, v8);
    swift_bridgeObjectRelease(v8);
    result = (id)v39;
    goto LABEL_46;
  }
  if ( *(_QWORD *)((v8 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
  {
    result = objc_retain(*(id *)(v8 + 32));
    goto LABEL_46;
  }
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_10010971C
 * EA: 0x10010971c
 ======================================================================== */

__int64 __fastcall sub_10010971C(__int64 a1)
{
  __int64 v1; // x22
  __int64 v2; // x19
  __int64 v3; // x21
  Swift::String v4; // x0
  Swift::String v5; // x0
  void *object; // x23
  __int64 v7; // x0
  __int64 v8; // x24
  __int64 v9; // x0

  v2 = *(_QWORD *)(v1 + 192);
  swift_willThrow(a1);
  v3 = type metadata accessor for VTLogger(0);
  _StringGuts.grow(_:)(55);
  v4._object = (void *)0x80000001002790F0LL;
  v4._countAndFlagsBits = 0xD000000000000035LL;
  String.append(_:)(v4);
  swift_getErrorValue(v2, v1 + 176, v1 + 144);
  v5._countAndFlagsBits = Error.localizedDescription.getter(*(_QWORD *)(v1 + 152), *(_QWORD *)(v1 + 160));
  object = v5._object;
  String.append(_:)(v5);
  v7 = swift_bridgeObjectRelease(object);
  v8 = static os_log_type_t.error.getter(v7);
  v9 = static os_log_type_t.error.getter(v8);
  sub_100091CD8(v8, v9, 0, 0xE000000000000000LL, v3);
  swift_bridgeObjectRelease(0xE000000000000000LL);
  swift_errorRelease(v2);
  return (*(__int64 (**)(void))(v1 + 8))();
}

/* ========================================================================
 * sub_100109810
 * EA: 0x100109810
 ======================================================================== */

id sub_100109810()
{
  _BYTE *v0; // x20
  _QWORD *v1; // x8
  __int64 v2; // x19
  objc_super v4; // [xsp+0h] [xbp-30h] BYREF

  v0[OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_isRunning] = 0;
  v1 = &v0[OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_frameUpdateBlock];
  *v1 = 0;
  v1[1] = 0;
  *(_QWORD *)&v0[OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_selectedDisplay] = 0;
  *(_QWORD *)&v0[OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_availableApps] = _swiftEmptyArrayStorage;
  *(_QWORD *)&v0[OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_availableDisplays] = _swiftEmptyArrayStorage;
  *(_QWORD *)&v0[OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_mainSCWindow] = 0;
  v2 = OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_captureEngine;
  *(_QWORD *)&v0[v2] = objc_msgSend(objc_allocWithZone((Class)type metadata accessor for CaptureEngine(0)), "init");
  v0[OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_isSetup] = 0;
  *(_QWORD *)&v0[OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_subscriptions] = &_swiftEmptySetSingleton;
  v4.receiver = v0;
  v4.super_class = (Class)type metadata accessor for ScreenRecorder();
  return objc_msgSendSuper2(&v4, "init");
}

/* ========================================================================
 * -[_TtC11SpaceWalker14ScreenRecorder .cxx_destruct]
 * EA: 0x100109934
 ======================================================================== */

void __cdecl -[ScreenRecorder .cxx_destruct](_TtC11SpaceWalker14ScreenRecorder *self, SEL a2)
{
  sub_100024D40(
    *(Class *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_frameUpdateBlock),
    *(_QWORD *)&self->isRunning[OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_frameUpdateBlock]);
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_selectedDisplay));
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.isa
                                     + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_availableApps));
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.isa
                                     + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_availableDisplays));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_mainSCWindow));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_captureEngine));
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.isa
                                     + OBJC_IVAR____TtC11SpaceWalker14ScreenRecorder_subscriptions));
}

/* ========================================================================
 * sub_1001099F8
 * EA: 0x1001099f8
 ======================================================================== */

__int64 __fastcall sub_1001099F8(__int64 a1, int *a2)
{
  __int64 v2; // x22
  _QWORD *v4; // x0
  __int64 (__fastcall *v6)(__int64); // [xsp+8h] [xbp-28h]

  v6 = (__int64 (__fastcall *)(__int64))((char *)a2 + *a2);
  v4 = (_QWORD *)swift_task_alloc((unsigned int)a2[1]);
  *(_QWORD *)(v2 + 16) = v4;
  *v4 = v2;
  v4[1] = sub_100109A5C;
  return v6(a1);
}

/* ========================================================================
 * sub_100109B28
 * EA: 0x100109b28
 ======================================================================== */

__int64 __fastcall sub_100109B28(__int64 a1)
{
  _QWORD *v1; // x20
  __int64 v2; // x22
  __int64 v4; // x21
  __int64 v5; // x23
  __int64 v6; // x20
  _QWORD *v7; // x0

  v4 = v1[2];
  v5 = v1[3];
  v6 = v1[4];
  v7 = (_QWORD *)swift_task_alloc(32);
  *(_QWORD *)(v2 + 16) = v7;
  *v7 = v2;
  v7[1] = sub_10000717C;
  return sub_100107D84(a1, v4, v5, v6);
}

/* ========================================================================
 * sub_100109BBC
 * EA: 0x100109bbc
 ======================================================================== */

__int64 __fastcall sub_100109BBC(__int64 a1)
{
  _QWORD *v1; // x20
  __int64 v2; // x22
  __int64 v4; // x21
  __int64 v5; // x23
  __int64 v6; // x20
  _QWORD *v7; // x0

  v4 = v1[2];
  v5 = v1[3];
  v6 = v1[4];
  v7 = (_QWORD *)swift_task_alloc(48);
  *(_QWORD *)(v2 + 16) = v7;
  *v7 = v2;
  v7[1] = sub_100007134;
  return sub_1001088B4(a1, v4, v5, v6);
}

/* ========================================================================
 * sub_100109C28
 * EA: 0x100109c28
 ======================================================================== */

__int64 __fastcall sub_100109C28(__int64 a1)
{
  __int64 v1; // x20
  __int64 v2; // x22
  __int64 v4; // x20
  __int64 v5; // x21
  _QWORD *v6; // x0

  v5 = *(_QWORD *)(v1 + 16);
  v4 = *(_QWORD *)(v1 + 24);
  v6 = (_QWORD *)swift_task_alloc(48);
  *(_QWORD *)(v2 + 16) = v6;
  *v6 = v2;
  v6[1] = sub_10000717C;
  return ((__int64 (__fastcall *)(__int64, __int64, __int64))(&off_100261C50 - 392891))(a1, v5, v4);
}

/* ========================================================================
 * sub_100109C98
 * EA: 0x100109c98
 ======================================================================== */

__int64 __fastcall sub_100109C98(__int64 a1)
{
  __int64 v1; // x20
  __int64 v2; // x22
  __int64 v4; // x20
  __int64 v5; // x21
  _QWORD *v6; // x0

  v5 = *(_QWORD *)(v1 + 16);
  v4 = *(_QWORD *)(v1 + 24);
  v6 = (_QWORD *)swift_task_alloc(48);
  *(_QWORD *)(v2 + 16) = v6;
  *v6 = v2;
  v6[1] = sub_100007134;
  return ((__int64 (__fastcall *)(__int64, __int64, __int64))(&off_100261C50 - 392891))(a1, v5, v4);
}

/* ========================================================================
 * sub_100109D18
 * EA: 0x100109d18
 ======================================================================== */

void sub_100109D18()
{
  __int64 v0; // x19
  __int64 v1; // x27
  char *v2; // x21
  char *v3; // x20
  const __CFDictionary *v4; // x0
  io_service_t MatchingService; // w22
  __CFString *v6; // x24
  CFTypeRef CFProperty; // x23
  __int64 v8; // x23
  __int64 v9; // x24
  __int64 v10; // x0
  __int64 v11; // x1
  __int64 v12; // x26
  __int64 v13; // x0
  __int64 v14; // x0
  __int64 v15; // [xsp+0h] [xbp-70h] BYREF
  __int64 v16; // [xsp+8h] [xbp-68h] BYREF
  __int64 v17; // [xsp+10h] [xbp-60h]
  CFTypeRef v18; // [xsp+18h] [xbp-58h] BYREF

  v0 = type metadata accessor for CharacterSet(0);
  v1 = *(_QWORD *)(v0 - 8);
  v2 = (char *)&v15 - ((*(_QWORD *)(v1 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v3 = (char *)&v15
     - ((*(_QWORD *)(*(_QWORD *)(type metadata accessor for String.Encoding(0) - 8) + 64LL) + 15LL)
      & 0xFFFFFFFFFFFFFFF0LL);
  v4 = IOServiceMatching("IOPlatformExpertDevice");
  MatchingService = IOServiceGetMatchingService(kIOMasterPortDefault, v4);
  v6 = (__CFString *)String._bridgeToObjectiveC()();
  CFProperty = IORegistryEntryCreateCFProperty(MatchingService, v6, kCFAllocatorDefault, 0);
  objc_release(v6);
  if ( CFProperty )
  {
    v18 = CFProperty;
    if ( (swift_dynamicCast(&v16, &v18, (char *)&type metadata for Swift.AnyObject + 8, &type metadata for Data, 6) & 1) != 0 )
    {
      v8 = v16;
      v9 = v17;
      static String.Encoding.utf8.getter();
      v10 = String.init(data:encoding:)(v8, v9, v3);
      if ( v11 )
      {
        v16 = v10;
        v17 = v11;
        v12 = v11;
        v13 = static CharacterSet.controlCharacters.getter();
        v14 = sub_10000DF9C(v13);
        StringProtocol.trimmingCharacters(in:)(v2, &type metadata for String, v14);
        sub_10000C5FC(v8, v9);
        (*(void (__fastcall **)(char *, __int64))(v1 + 8))(v2, v0);
        swift_bridgeObjectRelease(v12);
      }
      else
      {
        sub_10000C5FC(v8, v9);
      }
    }
    IOObjectRelease(MatchingService);
  }
  else
  {
    __break(1u);
  }
}

/* ========================================================================
 * sub_100109F18
 * EA: 0x100109f18
 ======================================================================== */

__int64 __fastcall sub_100109F18(__int64 a1, void *a2)
{
  __int64 v4; // x22
  char *v5; // x23
  __int64 v6; // x24
  char *v7; // x25
  void *v8; // x26
  __int64 v9; // x0
  void *v10; // x27
  __int64 v11; // x20
  __int64 v12; // x0
  __int64 v13; // x0
  __int64 v14; // x28
  __int64 v15; // x19
  __int64 v16; // x0
  __int64 v17; // x22
  unsigned __int64 v18; // x19
  __int64 v19; // x0
  __int64 v20; // x20
  unsigned __int64 v21; // x1
  unsigned __int64 v22; // x21
  Swift::String v23; // x0
  Swift::String v24; // x0
  void **v25; // x19
  unsigned __int64 v26; // x20
  __int64 v27; // x0
  __int64 v28; // x21
  __int64 v29; // x0
  __int64 v31; // [xsp+0h] [xbp-A0h] BYREF
  void *v32; // [xsp+8h] [xbp-98h]
  __int64 v33; // [xsp+10h] [xbp-90h]
  __int64 v34; // [xsp+18h] [xbp-88h]
  void **aBlock; // [xsp+20h] [xbp-80h] BYREF
  unsigned __int64 v36; // [xsp+28h] [xbp-78h]
  __int64 (__fastcall *v37)(); // [xsp+30h] [xbp-70h]
  void *v38; // [xsp+38h] [xbp-68h]
  __int64 (__fastcall *v39)(); // [xsp+40h] [xbp-60h]
  __int64 v40; // [xsp+48h] [xbp-58h]

  v34 = a1;
  v4 = type metadata accessor for DispatchWorkItemFlags(0);
  v33 = *(_QWORD *)(v4 - 8);
  v5 = (char *)&v31 - ((*(_QWORD *)(v33 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v31 = type metadata accessor for DispatchQoS(0);
  v6 = *(_QWORD *)(v31 - 8);
  v7 = (char *)&v31 - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  sub_100004E40(0);
  v8 = (void *)static OS_dispatch_queue.main.getter();
  v9 = swift_allocObject(&unk_1002E7E20, 32, 7);
  *(_QWORD *)(v9 + 16) = a1;
  *(_QWORD *)(v9 + 24) = a2;
  v32 = a2;
  v39 = sub_10010A378;
  v40 = v9;
  aBlock = _NSConcreteStackBlock;
  v36 = 1107296256;
  v37 = sub_10007A08C;
  v38 = &unk_1002E7E38;
  v10 = _Block_copy(&aBlock);
  v11 = v40;
  swift_bridgeObjectRetain(a2);
  v12 = swift_release(v11);
  v13 = static DispatchQoS.unspecified.getter(v12);
  aBlock = _swiftEmptyArrayStorage;
  v14 = sub_10000C824(v13);
  v15 = sub_100004DF0(&unk_10033BE50, &unk_10025B090);
  v16 = sub_100041DC0(&qword_10033C980, &unk_10033BE50, &unk_10025B090);
  dispatch thunk of SetAlgebra.init<A>(_:)(&aBlock, v15, v16, v4, v14);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v7, v5, v10);
  _Block_release(v10);
  objc_release(v8);
  (*(void (__fastcall **)(char *, __int64))(v33 + 8))(v5, v4);
  (*(void (__fastcall **)(char *, __int64))(v6 + 8))(v7, v31);
  v17 = type metadata accessor for VTLogger(0);
  aBlock = nullptr;
  v36 = 0xE000000000000000LL;
  _StringGuts.grow(_:)(35);
  v18 = v36;
  aBlock = (void **)&type metadata for VT7911FWVersionFromMCUHIDMsg;
  v19 = sub_100004DF0(&unk_100340430, &unk_100261C88);
  v20 = String.init<A>(describing:)(&aBlock, v19);
  v22 = v21;
  swift_bridgeObjectRelease(v18);
  aBlock = (void **)v20;
  v36 = v22;
  v23._object = (void *)0x8000000100271770LL;
  v23._countAndFlagsBits = 0xD00000000000001FLL;
  String.append(_:)(v23);
  v24._countAndFlagsBits = v34;
  v24._object = v32;
  String.append(_:)(v24);
  v25 = aBlock;
  v26 = v36;
  v28 = static os_log_type_t.debug.getter(v27);
  v29 = static os_log_type_t.debug.getter(v28);
  sub_100091CD8(v28, v29, v25, v26, v17);
  return swift_bridgeObjectRelease(v26);
}

/* ========================================================================
 * sub_10010A1DC
 * EA: 0x10010a1dc
 ======================================================================== */

void __fastcall sub_10010A1DC(__int64 a1, __int64 a2)
{
  id v4; // x19
  NSString v5; // x20
  __int64 v6; // x0
  __int64 inited; // x23
  __int64 v8; // x21
  Class isa; // x22
  _QWORD v10[2]; // [xsp+8h] [xbp-A8h] BYREF
  _BYTE v11[104]; // [xsp+18h] [xbp-98h] BYREF

  v4 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter, a2), "defaultCenter"));
  v5 = String._bridgeToObjectiveC()();
  v6 = sub_100004DF0(&unk_10033CDE0, &unk_10025C6A0);
  inited = swift_initStackObject(v6, v11);
  *(_OWORD *)(inited + 16) = xmmword_10025B110;
  v10[0] = 0x6E6F6973726576LL;
  v10[1] = 0xE700000000000000LL;
  AnyHashable.init<A>(_:)((_QWORD *)(inited + 32), v10, &type metadata for String, &protocol witness table for String);
  *(_QWORD *)(inited + 96) = &type metadata for String;
  *(_QWORD *)(inited + 72) = a1;
  *(_QWORD *)(inited + 80) = a2;
  swift_bridgeObjectRetain(a2);
  v8 = sub_100013868(inited);
  swift_setDeallocating(inited);
  sub_1000406D0(inited + 32);
  isa = Dictionary._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v8);
  objc_msgSend(v4, "postNotificationName:object:userInfo:", v5, 0, isa);
  objc_release(v4);
  objc_release(v5);
  objc_release(isa);
}

/* ========================================================================
 * -[_TtC11SpaceWalker15FlippedClipView initWithFrame:]
 * EA: 0x10010a3c4
 ======================================================================== */

_TtC11SpaceWalker15FlippedClipView *__cdecl -[FlippedClipView initWithFrame:](
        _TtC11SpaceWalker15FlippedClipView *self,
        SEL a2,
        CGRect a3)
{
  double height; // d8
  double width; // d9
  double y; // d10
  double x; // d11
  _TtC11SpaceWalker15FlippedClipView *v7; // x19
  objc_super v9; // [xsp+0h] [xbp-40h] BYREF

  height = a3.size.height;
  width = a3.size.width;
  y = a3.origin.y;
  x = a3.origin.x;
  v9.receiver = self;
  v9.super_class = (Class)swift_getObjectType(self);
  v7 = -[FlippedClipView initWithFrame:](&v9, "initWithFrame:", x, y, width, height);
  -[FlippedClipView setDrawsBackground:](v7, "setDrawsBackground:", 0);
  return v7;
}
