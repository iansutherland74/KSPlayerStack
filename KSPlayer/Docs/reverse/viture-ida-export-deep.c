/* Viture ExternalMonitor — deep IDA export */
/* Binary: /Users/sutherland/Downloads/com.viture.p10app-1.9.25-Decrypted/ExternalMonitor */
/* Functions: 154 */

/* ========================================================================
 * sub_10003E4E0
 * EA: 0x10003e4e0
 ======================================================================== */

__int64 __fastcall sub_10003E4E0(__int64 *a1, __int64 *a2)
{
  __int64 result; // x0
  bool v4; // zf

  result = *a1;
  if ( result )
    v4 = (result & 1) == 0;
  else
    v4 = 0;
  if ( !v4 )
  {
    result = swift_getTypeByMangledNameInContext2((char *)a2 + (int)*a2, *a2 >> 32, 0, 0);
    *a1 = result;
  }
  return result;
}

/* ========================================================================
 * sub_100040E14
 * EA: 0x100040e14
 ======================================================================== */

__int64 __fastcall sub_100040E14(__int64 result, unsigned __int64 a2)
{
  unsigned __int64 v2; // x19

  if ( a2 >> 62 != 1 )
  {
    if ( a2 >> 62 != 2 )
      return result;
    v2 = a2;
    swift_release(result);
    a2 = v2;
  }
  return swift_release(a2 & 0x3FFFFFFFFFFFFFFFLL);
}

/* ========================================================================
 * sub_100040EC0
 * EA: 0x100040ec0
 ======================================================================== */

void sub_100040EC0()
{
  __int64 v0; // x20
  __int64 v1; // x28
  __int64 v2; // x0
  char *v3; // x25
  __int64 v4; // x21
  __int64 v5; // x24
  char *v6; // x27
  __int64 v7; // x0
  __int64 v8; // x0
  __int64 v9; // x19
  void *v10; // x0
  void *v11; // x8
  __int64 v12; // x8
  __int64 v13; // x0
  __int64 v14; // x19
  void *v15; // x26
  _BOOL8 v16; // x20
  id v17; // x0
  _BYTE *v18; // x26
  void *v19; // x8
  void *v20; // x19
  id v21; // x0
  id v22; // x22
  id v23; // x20
  __int64 v24; // x0
  __int64 v25; // x0
  void *v26; // x0
  id v27; // x0
  void *v28; // x22
  __int64 v29; // x28
  void *v30; // x0
  id v31; // x0
  __int64 v32; // x1
  void *v33; // x21
  __int64 v34; // x1
  __int64 v35; // x19
  Swift::String v36; // x0
  Swift::String v37; // x0
  void *v38; // x22
  char *v39; // x26
  void (__fastcall *v40)(char *, __int64); // x19
  __int64 v41; // x28
  void *v42; // x27
  __int64 v43; // x24
  __int64 v44; // x20
  __int64 v45; // x0
  __int64 v46; // x21
  char *v47; // x23
  char *v48; // x0
  id v49; // x19
  double v50; // d0
  CGFloat v51; // d8
  double v52; // d1
  CGFloat v53; // d9
  double v54; // d2
  CGFloat v55; // d10
  double v56; // d3
  CGFloat v57; // d11
  __int64 v58; // x0
  __int64 v59; // x0
  __int64 v60; // [xsp+0h] [xbp-E0h] BYREF
  _BYTE *v61; // [xsp+8h] [xbp-D8h]
  char *v62; // [xsp+10h] [xbp-D0h]
  __int64 v63; // [xsp+18h] [xbp-C8h]
  __int64 v64; // [xsp+20h] [xbp-C0h]
  __int64 v65; // [xsp+28h] [xbp-B8h]
  __int64 v66; // [xsp+30h] [xbp-B0h]
  _QWORD aBlock[6]; // [xsp+38h] [xbp-A8h] BYREF
  char v68; // [xsp+6Fh] [xbp-71h] BYREF
  CGRect v69; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8

  v1 = v0;
  v63 = type metadata accessor for DispatchWorkItemFlags(0);
  v66 = *(_QWORD *)(v63 - 8);
  v62 = (char *)&v60 - ((*(_QWORD *)(v66 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v2 = type metadata accessor for DispatchQoS(0);
  v64 = *(_QWORD *)(v2 - 8);
  v65 = v2;
  v3 = (char *)&v60 - ((*(_QWORD *)(v64 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v4 = type metadata accessor for DispatchTime(0);
  v5 = *(_QWORD *)(v4 - 8);
  v6 = (char *)&v60 - ((*(_QWORD *)(v5 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  v68 = 0;
  swift_beginAccess(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__pinMode, aBlock, 33, 0);
  v7 = sub_10003E4E0(&qword_10066A970, (__int64 *)&unk_10053BAD0);
  WrappedDefault.wrappedValue.setter(&v68, v7);
  v8 = swift_endAccess(aBlock);
  sub_1000F4664(v8);
  v9 = OBJC_IVAR____TtC15ExternalMonitor20VTBaseViewController_fsToolBar;
  v10 = *(void **)(v0 + OBJC_IVAR____TtC15ExternalMonitor20VTBaseViewController_fsToolBar);
  if ( v10 )
  {
    objc_msgSend(v10, "removeFromSuperview");
    v11 = *(void **)(v0 + v9);
  }
  else
  {
    v11 = nullptr;
  }
  *(_QWORD *)(v0 + v9) = 0;
  objc_release(v11);
  v12 = *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets);
  if ( (unsigned __int64)v12 >> 62 )
  {
    if ( v12 < 0 )
      v59 = *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets);
    else
      v59 = v12 & 0xFFFFFFFFFFFFFF8LL;
    v13 = _CocoaArrayWrapper.endIndex.getter(v59);
  }
  else
  {
    v13 = *(_QWORD *)((v12 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
  }
  v14 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player;
  v15 = *(void **)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player);
  if ( v15 )
  {
    v16 = v13 != 0;
    v17 = objc_allocWithZone((Class)type metadata accessor for VTPlayerControlViewController(0));
    v18 = (_BYTE *)sub_1000D75D8(objc_retain(v15), v16);
    v18[OBJC_IVAR____TtC15ExternalMonitor29VTPlayerControlViewController_showImmersive3DGuide] = *(_BYTE *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_showImmersive3DGuide);
    swift_unknownObjectWeakAssign(&v18[OBJC_IVAR____TtC15ExternalMonitor29VTPlayerControlViewController_closeVC], v1);
    v19 = *(void **)(v1 + v14);
    v20 = *(void **)&v18[OBJC_IVAR____TtC15ExternalMonitor29VTPlayerControlViewController_player];
    *(_QWORD *)&v18[OBJC_IVAR____TtC15ExternalMonitor29VTPlayerControlViewController_player] = v19;
    v21 = objc_retain(v19);
    objc_release(v20);
    v22 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIApplication), "sharedApplication"));
    v23 = objc_retainAutoreleasedReturnValue(objc_msgSend(v22, "delegate"));
    objc_release(v22);
    if ( v23 )
    {
      v24 = type metadata accessor for AppDelegate(0);
      v25 = swift_dynamicCastClass(v23, v24);
      if ( v25 )
      {
        v26 = *(void **)(v25 + OBJC_IVAR____TtC15ExternalMonitor11AppDelegate_window);
        if ( v26 )
        {
          v27 = objc_retainAutoreleasedReturnValue(objc_msgSend(v26, "rootViewController"));
          if ( v27 )
          {
            v28 = v27;
            objc_msgSend(v27, "presentViewController:animated:completion:", v18, 1, 0);
            objc_release(v28);
          }
        }
      }
      swift_unknownObjectRelease(v23);
    }
    if ( qword_100662550 != -1 )
      swift_once(&qword_100662550, sub_100205F80);
    if ( qword_1006625A0 != -1 )
      swift_once(&qword_1006625A0, sub_100240480);
    v29 = qword_100697238;
    v30 = *(void **)(qword_100697238 + OBJC_IVAR____TtC15ExternalMonitor12VTBLEManager_connectedPeripheral);
    if ( v30 && (v31 = objc_retainAutoreleasedReturnValue(objc_msgSend(v30, "name"))) != nullptr )
    {
      v33 = v31;
      static String._unconditionallyBridgeFromObjectiveC(_:)(v31, v32);
      v35 = v34;
      objc_release(v33);
      v36._countAndFlagsBits = 3486032;
      v36._object = (void *)0xE300000000000000LL;
      if ( !String.hasPrefix(_:)(v36) )
      {
        v37._countAndFlagsBits = 3158352;
        v37._object = (void *)0xE300000000000000LL;
        String.hasPrefix(_:)(v37);
      }
      swift_bridgeObjectRelease(v35);
    }
    else
    {
      sub_100040668(0);
      v38 = (void *)static OS_dispatch_queue.main.getter();
      static DispatchTime.now()();
      v61 = v18;
      v39 = v6;
      + infix(_:_:)(v6, 0.5);
      v40 = *(void (__fastcall **)(char *, __int64))(v5 + 8);
      v60 = v29;
      v41 = v4;
      v40(v6, v4);
      aBlock[4] = sub_100041500;
      aBlock[5] = 0;
      aBlock[0] = _NSConcreteStackBlock;
      aBlock[1] = 1107296256;
      aBlock[2] = sub_100040600;
      aBlock[3] = &unk_1005B8C40;
      v42 = _Block_copy(aBlock);
      static DispatchQoS.unspecified.getter(v42);
      aBlock[0] = &_swiftEmptyArrayStorage;
      v43 = sub_10003EF70(
              &qword_100669D60,
              &type metadata accessor for DispatchWorkItemFlags,
              &protocol conformance descriptor for DispatchWorkItemFlags);
      v44 = sub_10003E4E0((__int64 *)&unk_100668B40, &qword_10053B8F0);
      v45 = sub_100040724();
      v47 = v62;
      v46 = v63;
      dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v44, v45, v63, v43);
      OS_dispatch_queue.asyncAfter(deadline:qos:flags:execute:)(v39, v3, v47, v42);
      _Block_release(v42);
      objc_release(v38);
      (*(void (__fastcall **)(char *, __int64))(v66 + 8))(v47, v46);
      (*(void (__fastcall **)(char *, __int64))(v64 + 8))(v3, v65);
      v48 = v39;
      v18 = v61;
      v40(v48, v41);
    }
    if ( qword_100662540 != -1 )
      swift_once(&qword_100662540, sub_1001F0F34);
    v49 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(qword_100697100
                                                                + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_window), "screen"));
    objc_msgSend(v49, "bounds");
    v51 = v50;
    v53 = v52;
    v55 = v54;
    v57 = v56;
    objc_release(v49);
    v69.origin.x = v51;
    v69.origin.y = v53;
    v69.size.width = v55;
    v69.size.height = v57;
    if ( CGRectGetHeight(v69) < 1200.0 )
      v58 = 1;
    else
      v58 = 5;
    sub_1000755E8(v58);
    objc_release(v18);
  }
  else
  {
    __break(1u);
  }
}

/* ========================================================================
 * sub_100041500
 * EA: 0x100041500
 ======================================================================== */

__int64 sub_100041500()
{
  __int64 v0; // x20
  char *v1; // x8
  unsigned __int64 v2; // x0
  __int64 v3; // x21
  __int64 v4; // x1
  __int64 v5; // x19
  int v6; // w22
  id v7; // x20

  if ( qword_100662540 != -1 )
    swift_once(&qword_100662540, sub_1001F0F34);
  v0 = qword_100697100;
  if ( *(_BYTE *)(qword_100697100 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_isR6Series) )
    v1 = "ve 3D Powered by AI.";
  else
    v1 = "layPreviousNotification";
  if ( *(_BYTE *)(qword_100697100 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_isR6Series) )
    v2 = 0xD000000000000045LL;
  else
    v2 = 0xD000000000000064LL;
  v3 = sub_10024814C(v2, (unsigned __int64)v1 | 0x8000000000000000LL, 0);
  v5 = v4;
  v6 = *(unsigned __int8 *)(v0 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_isUltraWide);
  v7 = objc_retain(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_window));
  if ( v6 == 1 )
  {
    sub_1000BA7C4(v3, v5, v7, 4.0);
  }
  else
  {
    sub_1000BAC98(v3, v5, v7, 1, 4.0);
    sub_1000BAC98(v3, v5, v7, 0, 4.0);
  }
  objc_release(v7);
  return swift_bridgeObjectRelease(v5);
}

/* ========================================================================
 * sub_100041670
 * EA: 0x100041670
 ======================================================================== */

void __fastcall sub_100041670(void *a1, char a2)
{
  _BYTE *v2; // x20
  _BYTE *v3; // x19
  __int64 v5; // x27
  __int64 v6; // x21
  __int64 v7; // x28
  __int64 v8; // x20
  __int64 v9; // x0
  __int64 v10; // x23
  id v11; // x0
  __int64 v12; // x23
  __int64 v13; // x0
  _QWORD *v14; // x20
  __int64 v15; // x20
  __int128 v16; // q1
  __int64 v17; // x23
  void *v18; // x25
  id v19; // x24
  id v20; // x25
  __int64 v21; // x0
  char v22; // w8
  void *v23; // x23
  id v24; // x20
  __int64 v25; // x0
  __int64 inited; // x23
  __int64 v27; // x1
  __int64 v28; // x1
  __int64 v29; // x22
  __int64 v30; // x23
  id v31; // x24
  Class isa; // x25
  id v33; // x23
  void *v34; // x21
  id v35; // x23
  id v36; // x23
  void *v37; // x8
  id v38; // x19
  void *v39; // x23
  char *v40; // x19
  id v41; // x0
  void *v42; // x23
  id v43; // x21
  id v44; // x22
  __int64 v45; // [xsp+8h] [xbp-138h]
  objc_super v47; // [xsp+18h] [xbp-128h] BYREF
  char v48[80]; // [xsp+28h] [xbp-118h] BYREF
  _QWORD v49[2]; // [xsp+78h] [xbp-C8h] BYREF
  char v50[24]; // [xsp+88h] [xbp-B8h] BYREF
  __int128 v51; // [xsp+A0h] [xbp-A0h]
  __int128 v52; // [xsp+B0h] [xbp-90h]
  __int64 v53; // [xsp+C0h] [xbp-80h]
  _OWORD v54[2]; // [xsp+D0h] [xbp-70h] BYREF

  v3 = v2;
  v5 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player] = 0;
  v6 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_playerItem;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_playerItem] = 0;
  v7 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_videoOutput;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_videoOutput] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_displayLink] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_curPixelBuffer] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_lastRenderedBuffer] = 0;
  v8 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_displayVc;
  v9 = type metadata accessor for VTStereoVideoPlayViewController(0);
  *(_QWORD *)&v3[v8] = objc_msgSend(objc_allocWithZone((Class)swift_getObjCClassFromMetadata(v9)), "init");
  *(_QWORD *)&v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_task] = 0;
  v10 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView;
  v11 = objc_allocWithZone((Class)type metadata accessor for SBSFastView());
  *(_QWORD *)&v3[v10] = sub_1000B290C(0, 0.0, 0.0, 0.0, 0.0);
  v12 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsController;
  v13 = type metadata accessor for FPSController();
  v14 = (_QWORD *)swift_allocObject(v13, 144, 15);
  swift_defaultActor_initialize();
  v14[14] = 0;
  v14[15] = 0;
  *(_QWORD *)&v3[v12] = v14;
  v45 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_orientation;
  *(_DWORD *)&v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_orientation] = 1;
  v14[16] = 0;
  v14[17] = &_swiftEmptyArrayStorage;
  v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_showImmersive3DGuide] = 0;
  v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_stereoDisabled] = 0;
  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  v15 = qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ai3DOptionRaw;
  swift_beginAccess(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ai3DOptionRaw, v50, 0, 0);
  v16 = *(_OWORD *)(v15 + 16);
  v51 = *(_OWORD *)v15;
  v52 = v16;
  v53 = *(_QWORD *)(v15 + 32);
  v17 = *((_QWORD *)&v51 + 1);
  v18 = (void *)v16;
  v54[0] = *(_OWORD *)(v15 + 24);
  v19 = objc_retain((id)v51);
  swift_retain(v17);
  v20 = objc_retain(v18);
  sub_10004542C(v54, v49);
  v21 = sub_10003E4E0((__int64 *)&unk_10066AFA0, (__int64 *)&unk_10053BAB0);
  WrappedDefault.wrappedValue.getter(v49, v21);
  objc_release(v20);
  swift_release(v17);
  objc_release(v19);
  sub_10003F604(v54);
  if ( v49[0] >= 3u )
    v22 = 1;
  else
    v22 = v49[0];
  v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_option] = v22;
  *(_QWORD *)&v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsTimer] = 0;
  *(_QWORD *)&v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_lastRemainBattery] = -1;
  *(_QWORD *)&v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets] = &_swiftEmptyArrayStorage;
  *(_QWORD *)&v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_index] = 0;
  v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_shouldCheckBlackEdge] = 0;
  *(_QWORD *)&v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeCheckingCounter] = 240;
  *(_QWORD *)&v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeHeights] = &_swiftEmptyArrayStorage;
  *(_QWORD *)&v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeThreshold] = 20;
  *(_QWORD *)&v3[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_rotationCW90Session] = 0;
  v23 = *(void **)&v3[v6];
  *(_QWORD *)&v3[v6] = a1;
  v24 = objc_retain(objc_retain(a1));
  objc_release(v23);
  v25 = sub_10003E4E0(&qword_10066D920, &qword_10053C530);
  inited = swift_initStackObject(v25, v48);
  *(_OWORD *)(inited + 16) = xmmword_10053B940;
  *(_QWORD *)(inited + 32) = static String._unconditionallyBridgeFromObjectiveC(_:)(
                               kCVPixelBufferPixelFormatTypeKey,
                               v27);
  *(_QWORD *)(inited + 72) = &type metadata for Int;
  *(_QWORD *)(inited + 40) = v28;
  *(_QWORD *)(inited + 48) = 1111970369;
  v29 = sub_100166EF8(inited);
  swift_setDeallocating(inited);
  sub_100045468(inited + 32);
  v30 = sub_10015FA84(v29);
  v31 = objc_allocWithZone((Class)&OBJC_CLASS___AVPlayerItemVideoOutput);
  sub_10003E4E0((__int64 *)&unk_10066E550, &qword_100541AF0);
  isa = Dictionary._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v30);
  v33 = objc_msgSend(v31, "initWithPixelBufferAttributes:", isa);
  objc_release(isa);
  v34 = *(void **)&v3[v7];
  *(_QWORD *)&v3[v7] = v33;
  v35 = objc_retain(v33);
  objc_release(v34);
  if ( v35 )
  {
    objc_msgSend(v24, "addOutput:", v35);
    objc_release(v35);
    v36 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___AVPlayer), "initWithPlayerItem:", v24);
    objc_release(v24);
    v37 = *(void **)&v3[v5];
    *(_QWORD *)&v3[v5] = v36;
    objc_release(v37);
    if ( (a2 & 1) != 0 )
      *(_DWORD *)&v3[v45] = sub_10016E170();
    v47.receiver = v3;
    v47.super_class = (Class)type metadata accessor for VTDepthVideoPlayer(0);
    v38 = objc_msgSendSuper2(&v47, "initWithNibName:bundle:", 0, 0);
    v39 = (void *)objc_opt_self(&OBJC_CLASS___CADisplayLink);
    v40 = (char *)objc_retain(v38);
    v41 = objc_retainAutoreleasedReturnValue(objc_msgSend(v39, "displayLinkWithTarget:selector:", v40, "displayLinkDidFire"));
    v42 = *(void **)&v40[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_displayLink];
    *(_QWORD *)&v40[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_displayLink] = v41;
    v43 = objc_retain(v41);
    objc_release(v42);
    if ( v43 )
    {
      swift_bridgeObjectRelease(v29);
      v44 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSRunLoop), "currentRunLoop"));
      objc_msgSend(v43, "addToRunLoop:forMode:", v44, NSDefaultRunLoopMode);
      objc_release(v40);
      objc_release(v24);
      objc_release(v43);
      objc_release(v44);
      return;
    }
  }
  else
  {
    __break(1u);
  }
  __break(1u);
}

/* ========================================================================
 * sub_100041BBC
 * EA: 0x100041bbc
 ======================================================================== */

void sub_100041BBC()
{
  __int64 v0; // x20

  objc_release(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player));
  objc_release(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_playerItem));
  objc_release(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_videoOutput));
  objc_release(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_displayLink));
  objc_release(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_curPixelBuffer));
  objc_release(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_lastRenderedBuffer));
  objc_release(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_displayVc));
  swift_release(*(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_task));
  objc_release(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView));
  swift_release(*(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsController));
  objc_release(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsTimer));
  swift_bridgeObjectRelease(*(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets));
  swift_bridgeObjectRelease(*(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeHeights));
  objc_release(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_rotationCW90Session));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor18VTDepthVideoPlayer dealloc]
 * EA: 0x100041d20
 ======================================================================== */

void __cdecl -[VTDepthVideoPlayer dealloc](_TtC15ExternalMonitor18VTDepthVideoPlayer *self, SEL a2)
{
  void *v3; // x20
  _TtC15ExternalMonitor18VTDepthVideoPlayer *v4; // x19
  id v5; // x20
  objc_super v6; // [xsp+0h] [xbp-20h] BYREF

  v3 = (void *)objc_opt_self(&OBJC_CLASS___UIApplication);
  v4 = objc_retain(self);
  v5 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "sharedApplication"));
  objc_msgSend(v5, "setIdleTimerDisabled:", 0);
  objc_release(v5);
  sub_1000422E4();
  v6.receiver = v4;
  v6.super_class = (Class)type metadata accessor for VTDepthVideoPlayer(0);
  -[VTBaseViewController dealloc](&v6, "dealloc");
}

/* ========================================================================
 * -[_TtC15ExternalMonitor18VTDepthVideoPlayer .cxx_destruct]
 * EA: 0x100041db0
 ======================================================================== */

void __cdecl -[VTDepthVideoPlayer .cxx_destruct](_TtC15ExternalMonitor18VTDepthVideoPlayer *self, SEL a2)
{
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_playerItem));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_videoOutput));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_displayLink));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_curPixelBuffer));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_lastRenderedBuffer));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_displayVc));
  swift_release(*(Class *)((char *)&self->super.super.super.super.isa
                         + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_task));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView));
  swift_release(*(Class *)((char *)&self->super.super.super.super.isa
                         + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsController));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsTimer));
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.super.super.super.isa
                                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets));
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.super.super.super.isa
                                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeHeights));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_rotationCW90Session));
}

/* ========================================================================
 * sub_100041EE8
 * EA: 0x100041ee8
 ======================================================================== */

id sub_100041EE8()
{
  char *v0; // x20
  char *v1; // x19
  objc_class *v2; // x0
  id v3; // x21
  id v4; // x21
  __int64 v5; // x0
  id result; // x0
  void *v7; // x21
  __int64 v8; // x22
  id v9; // x21
  __int64 v10; // x0
  char *v11; // x23
  __int64 v12; // x20
  __int64 v13; // x21
  _QWORD *v14; // x0
  __int64 v15; // x0
  objc_super v16; // [xsp+0h] [xbp-40h] BYREF

  v1 = (char *)&v16
     - ((*(_QWORD *)(*(_QWORD *)(sub_10003E4E0((__int64 *)&unk_100668B60, &qword_10053BAE0) - 8) + 64LL) + 15LL)
      & 0xFFFFFFFFFFFFFFF0LL);
  v2 = (objc_class *)type metadata accessor for VTDepthVideoPlayer(0);
  v16.receiver = v0;
  v16.super_class = v2;
  objc_msgSendSuper2(&v16, "viewDidLoad");
  v3 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIApplication), "sharedApplication"));
  objc_msgSend(v3, "setIdleTimerDisabled:", 1);
  objc_release(v3);
  v4 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIDevice), "currentDevice"));
  objc_msgSend(v4, "setBatteryMonitoringEnabled:", 1);
  objc_release(v4);
  v5 = sub_1000427F4();
  sub_1000424D4(v5);
  result = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "view"));
  if ( result )
  {
    v7 = result;
    v8 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView;
    objc_msgSend(result, "addSubview:", *(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView]);
    objc_release(v7);
    v9 = objc_retain(*(id *)&v0[v8]);
    ConstraintViewDSL.makeConstraints(_:)(sub_100042110, 0, v9);
    objc_release(v9);
    result = *(id *)&v0[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player];
    if ( result )
    {
      objc_msgSend(result, "play");
      v10 = type metadata accessor for TaskPriority(0);
      (*(void (__fastcall **)(char *, __int64, __int64, __int64))(*(_QWORD *)(v10 - 8) + 56LL))(v1, 1, 1, v10);
      type metadata accessor for MainActor(0);
      v11 = objc_retain(v0);
      v12 = static MainActor.shared.getter();
      v13 = sub_10003EF70(
              &qword_100671CF0,
              &type metadata accessor for MainActor,
              &protocol conformance descriptor for MainActor);
      v14 = (_QWORD *)swift_allocObject(&unk_1005B8E58, 40, 7);
      v14[2] = v12;
      v14[3] = v13;
      v14[4] = v11;
      v15 = sub_100087664(0, 0, v1, &unk_10053BB20, v14);
      return (id)swift_release(v15);
    }
  }
  else
  {
    __break(1u);
  }
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_1000422E4
 * EA: 0x1000422e4
 ======================================================================== */

void sub_1000422E4()
{
  __int64 v0; // x20
  void *v1; // x0
  void *v2; // x0
  __int64 v3; // x19
  __int64 v4; // x19
  void *v5; // x0

  v1 = *(void **)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_displayLink);
  if ( !v1 )
  {
    __break(1u);
    goto LABEL_7;
  }
  objc_msgSend(v1, "invalidate");
  v2 = *(void **)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player);
  if ( !v2 )
  {
LABEL_7:
    __break(1u);
    return;
  }
  objc_msgSend(v2, "pause");
  v3 = *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_task);
  if ( v3 )
  {
    swift_retain(*(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_task));
    Task.cancel()();
    swift_release(v3);
  }
  v4 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsTimer;
  objc_msgSend(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsTimer), "invalidate");
  v5 = *(void **)(v0 + v4);
  *(_QWORD *)(v0 + v4) = 0;
  objc_release(v5);
}

/* ========================================================================
 * sub_1000423A0
 * EA: 0x1000423a0
 ======================================================================== */

void sub_1000423A0()
{
  __int64 v0; // x20
  __int64 v1; // x19
  void *v2; // x0
  id v3; // x0
  void *v4; // x0
  __int64 v5; // x24
  void *v6; // x0
  __int64 v7; // x19
  int v8; // w21
  int v9; // w22
  __int64 v10; // x23
  void *v11; // x0
  id v12; // x0
  void *v13; // x9
  __int64 v14; // [xsp+8h] [xbp-48h] BYREF
  int v15; // [xsp+10h] [xbp-40h]
  int v16; // [xsp+14h] [xbp-3Ch]
  __int64 v17; // [xsp+18h] [xbp-38h]

  v1 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player;
  v2 = *(void **)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player);
  if ( !v2 )
  {
    __break(1u);
LABEL_11:
    __break(1u);
    goto LABEL_12;
  }
  v3 = objc_retainAutoreleasedReturnValue(objc_msgSend(v2, "currentItem"));
  if ( !v3 )
    return;
  objc_release(v3);
  v4 = *(void **)(v0 + v1);
  if ( !v4 )
    goto LABEL_11;
  objc_msgSend(v4, "currentTime");
  v5 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_videoOutput;
  v6 = *(void **)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_videoOutput);
  if ( !v6 )
  {
LABEL_12:
    __break(1u);
    goto LABEL_13;
  }
  v7 = v14;
  v8 = v15;
  v9 = v16;
  v10 = v17;
  if ( !(unsigned int)objc_msgSend(v6, "hasNewPixelBufferForItemTime:", &v14) )
    return;
  v11 = *(void **)(v0 + v5);
  if ( !v11 )
  {
LABEL_13:
    __break(1u);
    return;
  }
  v14 = v7;
  v15 = v8;
  v16 = v9;
  v17 = v10;
  v12 = objc_msgSend(v11, "copyPixelBufferForItemTime:itemTimeForDisplay:", &v14, 0);
  if ( v12 )
  {
    v13 = *(void **)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_curPixelBuffer);
    *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_curPixelBuffer) = v12;
    objc_release(v13);
  }
}

/* ========================================================================
 * -[_TtC15ExternalMonitor18VTDepthVideoPlayer displayLinkDidFire]
 * EA: 0x1000424a0
 ======================================================================== */

void __cdecl -[VTDepthVideoPlayer displayLinkDidFire](_TtC15ExternalMonitor18VTDepthVideoPlayer *self, SEL a2)
{
  _TtC15ExternalMonitor18VTDepthVideoPlayer *v2; // [xsp+8h] [xbp-18h]

  v2 = objc_retain(self);
  sub_1000423A0();
  objc_release(v2);
}

/* ========================================================================
 * sub_1000424D4
 * EA: 0x1000424d4
 ======================================================================== */

void sub_1000424D4()
{
  __int64 v0; // x20
  void *v1; // x19
  __int64 v2; // x21
  void *v3; // x21
  id v4; // x19
  void *v5; // x9
  _QWORD v6[5]; // [xsp+0h] [xbp-50h] BYREF
  __int64 v7; // [xsp+28h] [xbp-28h]

  v1 = (void *)objc_opt_self(&OBJC_CLASS___NSTimer);
  v2 = swift_allocObject(&unk_1005B8C78, 24, 7);
  swift_unknownObjectWeakInit(v2 + 16, v0);
  v6[4] = sub_100045930;
  v7 = v2;
  v6[0] = _NSConcreteStackBlock;
  v6[1] = 1107296256;
  v6[2] = sub_1001D4714;
  v6[3] = &unk_1005B8DF8;
  v3 = _Block_copy(v6);
  swift_release(v7);
  v4 = objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "scheduledTimerWithTimeInterval:repeats:block:", 1, v3, 60.0));
  _Block_release(v3);
  v5 = *(void **)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsTimer);
  *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsTimer) = v4;
  objc_release(v5);
}

/* ========================================================================
 * sub_1000425C4
 * EA: 0x1000425c4
 ======================================================================== */

void __fastcall sub_1000425C4(__int64 a1, __int64 a2)
{
  char *v3; // x19
  __int64 Strong; // x0
  void *v5; // x20
  __int64 v6; // x0
  id v7; // x23
  __int64 v8; // x20
  __int64 v9; // x21
  _QWORD *v10; // x0
  __int64 v11; // x0
  __int64 v12; // x0
  _BYTE v14[24]; // [xsp+8h] [xbp-48h] BYREF

  v3 = &v14[-((*(_QWORD *)(*(_QWORD *)(sub_10003E4E0((__int64 *)&unk_100668B60, &qword_10053BAE0) - 8) + 64LL) + 15LL)
            & 0xFFFFFFFFFFFFFFF0LL)
          - 8];
  swift_beginAccess(a2 + 16, v14, 0, 0);
  Strong = swift_unknownObjectWeakLoadStrong(a2 + 16);
  if ( Strong )
  {
    v5 = (void *)Strong;
    v6 = type metadata accessor for TaskPriority(0);
    (*(void (__fastcall **)(char *, __int64, __int64, __int64))(*(_QWORD *)(v6 - 8) + 56LL))(v3, 1, 1, v6);
    type metadata accessor for MainActor(0);
    v7 = objc_retain(v5);
    v8 = static MainActor.shared.getter();
    v9 = sub_10003EF70(
           &qword_100671CF0,
           &type metadata accessor for MainActor,
           &protocol conformance descriptor for MainActor);
    v10 = (_QWORD *)swift_allocObject(&unk_1005B8E30, 40, 7);
    v10[2] = v8;
    v10[3] = v9;
    v10[4] = v7;
    v11 = sub_100087664(0, 0, v3, &unk_10053BB10, v10);
    v12 = swift_release(v11);
    sub_1000427F4(v12);
    objc_release(v7);
  }
}

/* ========================================================================
 * sub_100042708
 * EA: 0x100042708
 ======================================================================== */

__int64 __fastcall sub_100042708(__int64 a1, __int64 a2, __int64 a3, __int64 a4)
{
  _QWORD *v4; // x22
  __int64 v5; // x19
  __int64 v6; // x0
  __int64 v7; // x0
  __int64 v8; // x1

  v4[2] = a4;
  v5 = type metadata accessor for MainActor(0);
  v4[3] = static MainActor.shared.getter();
  v6 = sub_10003EF70(
         &qword_100671CF0,
         &type metadata accessor for MainActor,
         &protocol conformance descriptor for MainActor);
  v7 = dispatch thunk of Actor.unownedExecutor.getter(v5, v6);
  v4[4] = v7;
  v4[5] = v8;
  return swift_task_switch(sub_100042798, v7, v8);
}

/* ========================================================================
 * sub_1000427F4
 * EA: 0x1000427f4
 ======================================================================== */

void sub_1000427F4()
{
  char **v0; // x20
  __int64 v1; // x23
  __int64 v2; // x24
  id v3; // x19
  char **v4; // x22
  id v5; // x21
  char **v6; // x25
  float v7; // s0
  float v8; // s8
  float v9; // s0
  int v10; // w8
  __int64 v12; // x0
  __int64 inited; // x21
  float v14; // s0
  float v15; // s8
  float v16; // s0
  __int64 v17; // x19
  __int64 v18; // x1
  __int64 v19; // x20
  void *v20; // x1
  __int64 v21; // x20
  __int64 v22; // x0
  int v23; // w20
  id v24; // x21
  bool v25; // zf
  _BYTE v26[72]; // [xsp+8h] [xbp-98h] BYREF

  v1 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_lastRemainBattery;
  v2 = *(__int64 *)((char *)v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_lastRemainBattery);
  v3 = (id)objc_opt_self(&OBJC_CLASS___UIDevice);
  v4 = &selRef_createAppDelegateProxy;
  v5 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "currentDevice"));
  v6 = &selRef_setLineHeightMultiple_;
  objc_msgSend(v5, "batteryLevel");
  v8 = v7;
  objc_release(v5);
  v9 = v8 * 100.0;
  v10 = fabs(v8 * 100.0);
  if ( v2 < 1 )
  {
    if ( v10 <= 2139095039 )
    {
      if ( v9 > -9.2234e18 )
      {
        if ( v9 < 9.2234e18 )
        {
          v2 = (__int64)v9 / 10;
          goto LABEL_22;
        }
        goto LABEL_28;
      }
LABEL_27:
      __break(1u);
LABEL_28:
      __break(1u);
      goto LABEL_29;
    }
LABEL_26:
    __break(1u);
    goto LABEL_27;
  }
  if ( v10 > 2139095039 )
  {
    __break(1u);
    goto LABEL_24;
  }
  if ( v9 <= -9.2234e18 )
  {
LABEL_24:
    __break(1u);
    goto LABEL_25;
  }
  if ( v9 >= 9.2234e18 )
  {
LABEL_25:
    __break(1u);
    goto LABEL_26;
  }
  v2 = (__int64)v9 / 10;
  if ( (__int64)v9 <= 59 && v2 < *(__int64 *)((char *)v0 + v1) )
  {
    v12 = sub_10003E4E0((__int64 *)&unk_10066AF90, (__int64 *)&unk_10053C880);
    inited = swift_initStackObject(v12, v26);
    *(_OWORD *)(inited + 16) = xmmword_10053B940;
    v3 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "currentDevice"));
    objc_msgSend(v3, "batteryLevel");
    v15 = v14;
    objc_release(v3);
    v16 = v15 * 100.0;
    if ( COERCE_INT(fabs(v15 * 100.0)) <= 2139095039 )
    {
      if ( v16 > -9.2234e18 )
      {
        if ( v16 < 9.2234e18 )
        {
          v6 = v0;
          v17 = dispatch thunk of CustomStringConvertible.description.getter(
                  &type metadata for Int,
                  &protocol witness table for Int);
          v19 = v18;
          *(_QWORD *)(inited + 56) = &type metadata for String;
          *(_QWORD *)(inited + 64) = sub_1000458F0();
          *(_QWORD *)(inited + 32) = v17;
          *(_QWORD *)(inited + 40) = v19;
          v4 = (char **)sub_10024814C(0xD000000000000012LL, 0x80000001004D9DE0LL, inited);
          v3 = v20;
          swift_setDeallocating(inited);
          v21 = *(_QWORD *)(inited + 16);
          v22 = sub_10003E4E0(&qword_1006682D0, &qword_10053BB00);
          swift_arrayDestroy(inited + 32, v21, v22);
          if ( qword_100662540 == -1 )
          {
LABEL_14:
            v23 = *(unsigned __int8 *)(qword_100697100 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_isUltraWide);
            v24 = objc_retain(*(id *)(qword_100697100 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_window));
            v25 = v23 == 1;
            v0 = v6;
            if ( v25 )
            {
              sub_1000BA7C4(v4, v3, v24, 4.0);
            }
            else
            {
              sub_1000BAC98(v4, v3, v24, 1, 4.0);
              sub_1000BAC98(v4, v3, v24, 0, 4.0);
            }
            objc_release(v24);
            swift_bridgeObjectRelease(v3);
            goto LABEL_22;
          }
LABEL_32:
          swift_once(&qword_100662540, sub_1001F0F34);
          goto LABEL_14;
        }
LABEL_31:
        __break(1u);
        goto LABEL_32;
      }
LABEL_30:
      __break(1u);
      goto LABEL_31;
    }
LABEL_29:
    __break(1u);
    goto LABEL_30;
  }
LABEL_22:
  *(char **)((char *)v0 + v1) = (char *)v2;
}

/* ========================================================================
 * sub_100042B3C
 * EA: 0x100042b3c
 ======================================================================== */

__int64 sub_100042B3C()
{
  __int64 v0; // x20
  _QWORD *v1; // x22
  __int64 v2; // x0
  __int64 v3; // x8
  __int64 v4; // x20
  __int64 v5; // x19
  __int64 v6; // x0
  __int64 v7; // x0
  __int64 v8; // x1

  v1[2] = v0;
  v2 = type metadata accessor for ContinuousClock(0);
  v1[3] = v2;
  v3 = *(_QWORD *)(v2 - 8);
  v1[4] = v3;
  v4 = *(_QWORD *)(v3 + 64) + 15LL;
  v1[5] = swift_task_alloc(v4 & 0xFFFFFFFFFFFFFFF0LL);
  v1[6] = swift_task_alloc(v4 & 0xFFFFFFFFFFFFFFF0LL);
  v1[7] = swift_task_alloc(v4 & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for MainActor(0);
  v1[8] = static MainActor.shared.getter();
  v6 = sub_10003EF70(
         &qword_100671CF0,
         &type metadata accessor for MainActor,
         &protocol conformance descriptor for MainActor);
  v7 = dispatch thunk of Actor.unownedExecutor.getter(v5, v6);
  v1[9] = v7;
  v1[10] = v8;
  return swift_task_switch(sub_100042C0C, v7, v8);
}

/* ========================================================================
 * sub_100042C0C
 * EA: 0x100042c0c
 ======================================================================== */

void *sub_100042C0C()
{
  _QWORD *v0; // x22
  __int64 v1; // x8
  void *v2; // x24
  __int64 v3; // x9
  void *v4; // x20
  id v5; // x19
  id v6; // x0
  __int64 v7; // x20
  void *result; // x0
  __int64 v9; // x25
  __int64 v10; // x26
  void *v11; // x21
  id v12; // x21
  _QWORD *v13; // x0
  _QWORD *v14; // x0
  __int64 v15; // x8
  void *v16; // x20
  __int64 v17; // x23
  _QWORD *v18; // x0
  id v19; // x21
  int v20; // w23
  id v21; // x0
  __int64 v22; // x20
  _QWORD *v23; // x0
  void *v24; // x19
  __int64 v25; // x8
  __int64 v26; // x0
  void *v27; // x20
  id v28; // x23
  __int64 v29; // x0
  __int64 v30; // x0
  CGFloat v31; // d8
  __int64 v32; // x0
  __int64 v33; // x0
  __int64 v34; // x0
  void *v35; // x20
  id v36; // x21
  _QWORD *v37; // x0
  CGRect v38; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8

  v1 = v0[2];
  v2 = *(void **)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_curPixelBuffer);
  v0[11] = v2;
  if ( !v2 )
    goto LABEL_10;
  v3 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView;
  v0[12] = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView;
  v4 = *(void **)(v1 + v3);
  v5 = objc_retain(v2);
  v6 = objc_retainAutoreleasedReturnValue(objc_msgSend(v4, "superview"));
  if ( !v6 )
  {
    objc_release(v5);
LABEL_10:
    static Clock<>.continuous.getter();
    v14 = (_QWORD *)swift_task_alloc(128);
    v0[31] = v14;
    *v14 = v0;
    v14[1] = sub_100043720;
    goto LABEL_11;
  }
  v7 = v0[2];
  objc_release(v6);
  result = *(void **)(v7 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player);
  if ( result )
  {
    if ( objc_msgSend(result, "timeControlStatus") != (id)2 && byte_1006969C0 != 1 )
    {
      static Clock<>.continuous.getter();
      v18 = (_QWORD *)swift_task_alloc(128);
      v0[30] = v18;
      *v18 = v0;
      v18[1] = sub_100043630;
      v15 = 50000000000000000LL;
      return (void *)((__int64 (__fastcall *)(__int64, _QWORD, _QWORD, _QWORD, __int64))sub_1001DC42C)(v15, 0, 0, 0, 1);
    }
    v9 = v0[2];
    v10 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_lastRenderedBuffer;
    v11 = *(void **)(v9 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_lastRenderedBuffer);
    v0[13] = v11;
    if ( v11 )
    {
      type metadata accessor for CVBuffer(0);
      sub_10003EF70(&unk_1006682C8, type metadata accessor for CVBuffer, &unk_10053AC84);
      v12 = objc_retain(v11);
      if ( (static _CFObject.== infix(_:_:)() & 1) != 0 )
      {
        static Clock<>.continuous.getter();
        v13 = (_QWORD *)swift_task_alloc(128);
        v0[14] = v13;
        *v13 = v0;
        v13[1] = sub_100043048;
LABEL_11:
        v15 = 2000000000000000LL;
        return (void *)((__int64 (__fastcall *)(__int64, _QWORD, _QWORD, _QWORD, __int64))sub_1001DC42C)(
                         v15,
                         0,
                         0,
                         0,
                         1);
      }
      objc_release(v12);
      v16 = *(void **)(v9 + v10);
      v17 = v0[2];
    }
    else
    {
      v16 = nullptr;
      v17 = v9;
    }
    *(_QWORD *)(v9 + v10) = v2;
    v19 = objc_retain(v5);
    objc_release(v16);
    v20 = *(unsigned __int8 *)(v17 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_stereoDisabled);
    v21 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CIImage), "initWithCVPixelBuffer:", v19);
    v0[15] = v21;
    v22 = v0[2];
    if ( v20 == 1 )
    {
      v23 = (_QWORD *)swift_task_alloc(208);
      v0[16] = v23;
      *v23 = v0;
      v23[1] = sub_100043140;
      return (void *)sub_1001DB840(0, 1);
    }
    else
    {
      v24 = v21;
      v25 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_orientation;
      v0[19] = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_orientation;
      if ( *(_DWORD *)(v22 + v25) == 6 )
      {
        v26 = sub_100044EA8(v19);
        if ( v26 )
        {
          v27 = (void *)v26;
          v28 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CIImage), "initWithCVPixelBuffer:", v26);
          objc_release(v27);
          objc_release(v24);
          v24 = v28;
        }
      }
      v29 = sub_100044BEC(v24);
      v30 = sub_100044B84(v29);
      if ( v30 >= 21 )
      {
        v31 = (double)sub_100044B84(v30);
        objc_msgSend(v24, "extent");
        if ( CGRectGetHeight(v38) * 0.2 > v31 )
        {
          v33 = sub_100044B84(v32);
          v34 = sub_1000874A8(v33);
          if ( v34 )
          {
            v35 = (void *)v34;
            v36 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CIImage), "initWithCVPixelBuffer:", v34);
            objc_release(v35);
            objc_release(v24);
            v24 = v36;
          }
        }
      }
      v0[20] = v24;
      byte_1006969C0 = 0;
      if ( qword_100662428 != -1 )
        swift_once(&qword_100662428, sub_10015B98C);
      v0[21] = qword_100696E10;
      v37 = (_QWORD *)swift_task_alloc(928);
      v0[22] = v37;
      *v37 = v0;
      v37[1] = sub_1000432B4;
      return (void *)sub_10015D4EC(v24);
    }
  }
  else
  {
    __break(1u);
  }
  return result;
}

/* ========================================================================
 * sub_100043048
 * EA: 0x100043048
 ======================================================================== */

__int64 sub_100043048()
{
  __int64 v0; // x20
  __int64 v1; // x22
  _QWORD *v2; // x23
  __int64 v3; // x19
  __int64 v4; // x21
  __int64 v5; // x24
  __int64 v6; // x1
  __int64 v7; // x2
  __int64 (__fastcall *v8)(); // x0

  v2 = *(_QWORD **)v1;
  swift_task_dealloc(*(_QWORD *)(*(_QWORD *)v1 + 112LL));
  v3 = v2[7];
  v4 = v2[3];
  v5 = v2[4];
  if ( v0 )
  {
    swift_errorRelease(v0);
    (*(void (__fastcall **)(__int64, __int64))(v5 + 8))(v3, v4);
    v6 = v2[9];
    v7 = v2[10];
    v8 = sub_100045AA4;
  }
  else
  {
    (*(void (__fastcall **)(_QWORD, _QWORD))(v5 + 8))(v2[7], v2[3]);
    v6 = v2[9];
    v7 = v2[10];
    v8 = sub_1000430E4;
  }
  return swift_task_switch(v8, v6, v7);
}

/* ========================================================================
 * sub_100043320
 * EA: 0x100043320
 ======================================================================== */

__int64 sub_100043320()
{
  __int64 v0; // x22
  __int64 v1; // x19
  __int64 v2; // x21
  __int64 v3; // x20
  __int64 v4; // x0
  __int64 v5; // x20
  __int64 v6; // x20
  __int64 v7; // x19
  __int64 v9; // x1
  _QWORD *v10; // x0

  v1 = *(_QWORD *)(v0 + 184);
  if ( v1 )
  {
    v2 = *(_QWORD *)(v0 + 192);
    if ( (*(_BYTE *)(*(_QWORD *)(v0 + 168) + 129LL) & 1) != 0 )
    {
      v3 = sub_1001152AC(*(_QWORD *)(v0 + 184), *(_QWORD *)(v0 + 192), 1.0);
      v4 = sub_1001152AC(v1, v2, 0.0);
    }
    else
    {
      v3 = sub_100217E4C(*(_QWORD *)(v0 + 184), *(_QWORD *)(v0 + 192));
      v4 = v9;
    }
    *(_QWORD *)(v0 + 200) = v3;
    *(_QWORD *)(v0 + 208) = v4;
    v10 = (_QWORD *)swift_task_alloc(208);
    *(_QWORD *)(v0 + 216) = v10;
    *v10 = v0;
    v10[1] = sub_10004348C;
    return sub_1001DB840(0, 1);
  }
  else
  {
    v5 = *(_QWORD *)(v0 + 64);
    objc_release(*(id *)(v0 + 160));
    swift_release(v5);
    objc_release(*(id *)(v0 + 88));
    v6 = *(_QWORD *)(v0 + 48);
    v7 = *(_QWORD *)(v0 + 40);
    swift_task_dealloc(*(_QWORD *)(v0 + 56));
    swift_task_dealloc(v6);
    swift_task_dealloc(v7);
    return (*(__int64 (**)(void))(v0 + 8))();
  }
}

/* ========================================================================
 * sub_1000434D4
 * EA: 0x1000434d4
 ======================================================================== */

__int64 sub_1000434D4()
{
  _QWORD *v0; // x22
  __int64 v1; // x10
  unsigned int v2; // w8
  __int64 v3; // x20
  void *v4; // x8
  _QWORD *v5; // x0

  v1 = v0[2];
  v2 = *(_DWORD *)(v1 + v0[19]);
  if ( v2 == 6 )
    v3 = 1;
  else
    v3 = v2;
  v4 = *(void **)(v1 + v0[12]);
  v0[28] = v4;
  objc_retain(v4);
  v5 = (_QWORD *)swift_task_alloc(192);
  v0[29] = v5;
  *v5 = v0;
  v5[1] = sub_10004355C;
  return sub_1000B3F20(v0[25], v0[26], v3);
}

/* ========================================================================
 * sub_1000435A8
 * EA: 0x1000435a8
 ======================================================================== */

__int64 sub_1000435A8()
{
  __int64 v0; // x22
  void *v1; // x19
  void *v2; // x21
  void *v3; // x23
  void *v4; // x24
  void *v5; // x25
  __int64 v6; // x20
  __int64 v7; // x20
  __int64 v8; // x19

  v2 = *(void **)(v0 + 200);
  v1 = *(void **)(v0 + 208);
  v4 = *(void **)(v0 + 184);
  v3 = *(void **)(v0 + 192);
  v5 = *(void **)(v0 + 88);
  v6 = *(_QWORD *)(v0 + 64);
  objc_release(*(id *)(v0 + 160));
  swift_release(v6);
  objc_release(v1);
  objc_release(v2);
  objc_release(v3);
  objc_release(v4);
  objc_release(v5);
  v7 = *(_QWORD *)(v0 + 48);
  v8 = *(_QWORD *)(v0 + 40);
  swift_task_dealloc(*(_QWORD *)(v0 + 56));
  swift_task_dealloc(v7);
  swift_task_dealloc(v8);
  return (*(__int64 (**)(void))(v0 + 8))();
}

/* ========================================================================
 * sub_100043630
 * EA: 0x100043630
 ======================================================================== */

__int64 sub_100043630()
{
  __int64 v0; // x20
  __int64 v1; // x22
  _QWORD *v2; // x23
  __int64 v3; // x19
  __int64 v4; // x21
  __int64 v5; // x24
  __int64 v6; // x1
  __int64 v7; // x2
  __int64 (__fastcall *v8)(); // x0

  v2 = *(_QWORD **)v1;
  swift_task_dealloc(*(_QWORD *)(*(_QWORD *)v1 + 240LL));
  v3 = v2[6];
  v4 = v2[3];
  v5 = v2[4];
  if ( v0 )
  {
    swift_errorRelease(v0);
    (*(void (__fastcall **)(__int64, __int64))(v5 + 8))(v3, v4);
    v6 = v2[9];
    v7 = v2[10];
    v8 = sub_100045AA0;
  }
  else
  {
    (*(void (__fastcall **)(_QWORD, _QWORD))(v5 + 8))(v2[6], v2[3]);
    v6 = v2[9];
    v7 = v2[10];
    v8 = sub_1000436CC;
  }
  return swift_task_switch(v8, v6, v7);
}

/* ========================================================================
 * sub_100043720
 * EA: 0x100043720
 ======================================================================== */

__int64 sub_100043720()
{
  __int64 v0; // x20
  __int64 v1; // x22
  _QWORD *v2; // x23
  __int64 v3; // x19
  __int64 v4; // x24
  __int64 v5; // x21
  __int64 v6; // x1
  __int64 v7; // x2
  __int64 (__fastcall *v8)(); // x0

  v2 = *(_QWORD **)v1;
  swift_task_dealloc(*(_QWORD *)(*(_QWORD *)v1 + 248LL));
  v4 = v2[4];
  v3 = v2[5];
  v5 = v2[3];
  if ( v0 )
  {
    swift_errorRelease(v0);
    (*(void (__fastcall **)(__int64, __int64))(v4 + 8))(v3, v5);
    v6 = v2[9];
    v7 = v2[10];
    v8 = sub_100045AAC;
  }
  else
  {
    (*(void (__fastcall **)(_QWORD, _QWORD))(v4 + 8))(v2[5], v2[3]);
    v6 = v2[9];
    v7 = v2[10];
    v8 = sub_1000437BC;
  }
  return swift_task_switch(v8, v6, v7);
}

/* ========================================================================
 * sub_100043808
 * EA: 0x100043808
 ======================================================================== */

__int64 sub_100043808()
{
  __int64 v0; // x20
  _QWORD *v1; // x22
  __int64 v2; // x0
  __int64 v3; // x8
  __int64 v4; // x0
  __int64 v5; // x8
  __int64 v6; // x0
  __int64 v7; // x8
  __int64 v8; // x19
  __int64 v9; // x0
  __int64 v10; // x0
  __int64 v11; // x1

  v1[9] = v0;
  v2 = type metadata accessor for DispatchWorkItemFlags(0);
  v1[10] = v2;
  v3 = *(_QWORD *)(v2 - 8);
  v1[11] = v3;
  v1[12] = swift_task_alloc((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v4 = type metadata accessor for DispatchQoS(0);
  v1[13] = v4;
  v5 = *(_QWORD *)(v4 - 8);
  v1[14] = v5;
  v1[15] = swift_task_alloc((*(_QWORD *)(v5 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v6 = type metadata accessor for DispatchQoS.QoSClass(0);
  v1[16] = v6;
  v7 = *(_QWORD *)(v6 - 8);
  v1[17] = v7;
  v1[18] = swift_task_alloc((*(_QWORD *)(v7 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v8 = type metadata accessor for MainActor(0);
  v1[19] = static MainActor.shared.getter();
  v9 = sub_10003EF70(
         &qword_100671CF0,
         &type metadata accessor for MainActor,
         &protocol conformance descriptor for MainActor);
  v10 = dispatch thunk of Actor.unownedExecutor.getter(v8, v9);
  return swift_task_switch(sub_100043910, v10, v11);
}

/* ========================================================================
 * sub_100043910
 * EA: 0x100043910
 ======================================================================== */

__int64 sub_100043910()
{
  _QWORD *v0; // x22
  __int64 v1; // x23
  __int64 v2; // x27
  __int64 v3; // x28
  __int64 v4; // x21
  __int64 v5; // x19
  __int64 v6; // x25
  void *v7; // x24
  void *v8; // x26
  __int64 v9; // x0
  void *v10; // x27
  __int64 v11; // x28
  __int64 v12; // x24
  __int64 v13; // x0
  __int64 v15; // [xsp+0h] [xbp-70h]
  __int64 v16; // [xsp+8h] [xbp-68h]
  __int64 v17; // [xsp+10h] [xbp-60h]

  v1 = v0[18];
  v2 = v0[16];
  v3 = v0[17];
  v4 = v0[15];
  v5 = v0[12];
  v16 = v0[14];
  v17 = v0[13];
  v6 = v0[10];
  v15 = v0[11];
  v7 = (void *)v0[9];
  swift_release(v0[19]);
  sub_100040668(0);
  (*(void (__fastcall **)(__int64, _QWORD, __int64))(v3 + 104))(v1, enum case for DispatchQoS.QoSClass.default(_:), v2);
  v8 = (void *)static OS_dispatch_queue.global(qos:)(v1);
  (*(void (__fastcall **)(__int64, __int64))(v3 + 8))(v1, v2);
  v9 = swift_allocObject(&unk_1005B8D40, 24, 7);
  *(_QWORD *)(v9 + 16) = v7;
  v0[6] = sub_10004581C;
  v0[7] = v9;
  v0[2] = _NSConcreteStackBlock;
  v0[3] = 1107296256;
  v0[4] = sub_100040600;
  v0[5] = &unk_1005B8D58;
  v10 = _Block_copy(v0 + 2);
  static DispatchQoS.unspecified.getter(objc_retain(v7));
  v0[8] = &_swiftEmptyArrayStorage;
  v11 = sub_10003EF70(
          &qword_100669D60,
          &type metadata accessor for DispatchWorkItemFlags,
          &protocol conformance descriptor for DispatchWorkItemFlags);
  v12 = sub_10003E4E0((__int64 *)&unk_100668B40, &qword_10053B8F0);
  v13 = sub_100040724();
  dispatch thunk of SetAlgebra.init<A>(_:)(v0 + 8, v12, v13, v6, v11);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v4, v5, v10);
  _Block_release(v10);
  objc_release(v8);
  (*(void (__fastcall **)(__int64, __int64))(v15 + 8))(v5, v6);
  (*(void (__fastcall **)(__int64, __int64))(v16 + 8))(v4, v17);
  swift_release(v0[7]);
  swift_task_dealloc(v1);
  swift_task_dealloc(v4);
  swift_task_dealloc(v5);
  return ((__int64 (*)(void))v0[1])();
}

/* ========================================================================
 * sub_100043AFC
 * EA: 0x100043afc
 ======================================================================== */

__int64 __fastcall sub_100043AFC(__int64 a1)
{
  __int64 v2; // x19
  char *v3; // x21
  __int64 v4; // x22
  char *v5; // x23
  char *v6; // x20
  __int64 v7; // x0
  __int64 v8; // x26
  _QWORD *v9; // x0
  __int64 v10; // x26
  void *v11; // x27
  __int64 v12; // x20
  __int64 v13; // x0
  void *v14; // x25
  __int64 v15; // x20
  __int64 v16; // x0
  __int64 v17; // x24
  __int64 v18; // x20
  __int64 v19; // x0
  __int64 v21; // [xsp+0h] [xbp-90h] BYREF
  __int64 v22; // [xsp+8h] [xbp-88h]
  _QWORD aBlock[5]; // [xsp+10h] [xbp-80h] BYREF
  __int64 v24; // [xsp+38h] [xbp-58h]

  v2 = type metadata accessor for DispatchWorkItemFlags(0);
  v22 = *(_QWORD *)(v2 - 8);
  v3 = (char *)&v21 - ((*(_QWORD *)(v22 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v21 = type metadata accessor for DispatchQoS(0);
  v4 = *(_QWORD *)(v21 - 8);
  v5 = (char *)&v21 - ((*(_QWORD *)(v4 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v6 = (char *)&v21
     - ((*(_QWORD *)(*(_QWORD *)(sub_10003E4E0((__int64 *)&unk_100668B60, &qword_10053BAE0) - 8) + 64LL) + 15LL)
      & 0xFFFFFFFFFFFFFFF0LL);
  v7 = type metadata accessor for TaskPriority(0);
  (*(void (__fastcall **)(char *, __int64, __int64, __int64))(*(_QWORD *)(v7 - 8) + 56LL))(v6, 1, 1, v7);
  v8 = swift_allocObject(&unk_1005B8C78, 24, 7);
  swift_unknownObjectWeakInit(v8 + 16, a1);
  v9 = (_QWORD *)swift_allocObject(&unk_1005B8D90, 40, 7);
  v9[2] = 0;
  v9[3] = 0;
  v9[4] = v8;
  v10 = sub_100087664(0, 0, v6, &unk_10053BAF0, v9);
  sub_100040668(0);
  v11 = (void *)static OS_dispatch_queue.main.getter();
  v12 = swift_allocObject(&unk_1005B8C78, 24, 7);
  swift_unknownObjectWeakInit(v12 + 16, a1);
  v13 = swift_allocObject(&unk_1005B8DB8, 32, 7);
  *(_QWORD *)(v13 + 16) = v12;
  *(_QWORD *)(v13 + 24) = v10;
  aBlock[4] = sub_1000458E8;
  v24 = v13;
  aBlock[0] = _NSConcreteStackBlock;
  aBlock[1] = 1107296256;
  aBlock[2] = sub_100040600;
  aBlock[3] = &unk_1005B8DD0;
  v14 = _Block_copy(aBlock);
  v15 = v24;
  swift_retain(v10);
  v16 = swift_release(v15);
  static DispatchQoS.unspecified.getter(v16);
  aBlock[0] = &_swiftEmptyArrayStorage;
  v17 = sub_10003EF70(
          &qword_100669D60,
          &type metadata accessor for DispatchWorkItemFlags,
          &protocol conformance descriptor for DispatchWorkItemFlags);
  v18 = sub_10003E4E0((__int64 *)&unk_100668B40, &qword_10053B8F0);
  v19 = sub_100040724();
  dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v18, v19, v2, v17);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v5, v3, v14);
  _Block_release(v14);
  swift_release(v10);
  objc_release(v11);
  (*(void (__fastcall **)(char *, __int64))(v22 + 8))(v3, v2);
  return (*(__int64 (__fastcall **)(char *, __int64))(v4 + 8))(v5, v21);
}

/* ========================================================================
 * sub_100043DF8
 * EA: 0x100043df8
 ======================================================================== */

__int64 sub_100043DF8()
{
  __int64 v0; // x22
  __int64 v1; // x0
  __int64 Strong; // x0
  _QWORD *v3; // x0

  v1 = swift_beginAccess(*(_QWORD *)(v0 + 40) + 16LL, v0 + 16, 0, 0);
  if ( (static Task<>.isCancelled.getter(v1) & 1) != 0 )
    return (*(__int64 (**)(void))(v0 + 8))();
  Strong = swift_unknownObjectWeakLoadStrong(*(_QWORD *)(v0 + 40) + 16LL);
  *(_QWORD *)(v0 + 48) = Strong;
  if ( !Strong )
    return (*(__int64 (**)(void))(v0 + 8))();
  v3 = (_QWORD *)swift_task_alloc(256);
  *(_QWORD *)(v0 + 56) = v3;
  *v3 = v0;
  v3[1] = sub_100043E88;
  return sub_100042B3C();
}

/* ========================================================================
 * sub_100043FCC
 * EA: 0x100043fcc
 ======================================================================== */

void __fastcall sub_100043FCC(void *a1, __int64 a2, __int64 a3, void (__fastcall *a4)(char *))
{
  id v8; // x0
  id v9; // x22
  char *v10; // x0
  char *v11; // x20
  __int64 v12; // x25
  void *v13; // x23
  id v14; // x24
  char *v15; // x20
  NSString v16; // x26
  id v17; // x23
  char *v18; // x20
  NSString v19; // x25
  char *v20; // [xsp+8h] [xbp-58h]

  if ( a1 )
  {
    v8 = objc_allocWithZone((Class)type metadata accessor for VTDepthVideoPlayer(0));
    v9 = objc_retain(objc_retain(a1));
    sub_100041670(v9, 1);
    v11 = v10;
    v12 = *(_QWORD *)&v10[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets];
    *(_QWORD *)&v10[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets] = a2;
    swift_bridgeObjectRetain(a2);
    swift_bridgeObjectRelease(v12);
    *(_QWORD *)&v11[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_index] = a3;
    v13 = (void *)objc_opt_self(&OBJC_CLASS___NSNotificationCenter);
    v14 = objc_retainAutoreleasedReturnValue(objc_msgSend(v13, "defaultCenter"));
    v15 = objc_retain(v11);
    v16 = String._bridgeToObjectiveC()();
    objc_msgSend(v14, "addObserver:selector:name:object:", v15, "playNext", v16, 0);
    objc_release(v14);
    objc_release(v15);
    objc_release(v16);
    v17 = objc_retainAutoreleasedReturnValue(objc_msgSend(v13, "defaultCenter"));
    v18 = objc_retain(v15);
    v19 = String._bridgeToObjectiveC()();
    objc_msgSend(v17, "addObserver:selector:name:object:", v18, "playPrevious", v19, 0);
    objc_release(v17);
    objc_release(v18);
    objc_release(v19);
    v20 = objc_retain(v18);
    a4(v18);
    objc_release(v9);
    objc_release(v20);
    objc_release(v20);
  }
}

/* ========================================================================
 * sub_10004419C
 * EA: 0x10004419c
 ======================================================================== */

__int64 sub_10004419C()
{
  char *v0; // x20
  __int64 v1; // x21
  __int64 v2; // x8
  __int64 result; // x0
  __int64 v4; // x22
  __int64 v5; // x8
  __int64 v6; // x19
  id v8; // x19
  __int64 v9; // x21
  char *v10; // x0
  __int64 v11; // x0
  __int64 v12; // x0

  v1 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets;
  v2 = *(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets];
  if ( (unsigned __int64)v2 >> 62 )
  {
    if ( v2 < 0 )
      v11 = *(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets];
    else
      v11 = v2 & 0xFFFFFFFFFFFFFF8LL;
    result = _CocoaArrayWrapper.endIndex.getter(v11);
    if ( result >= 1 )
    {
LABEL_3:
      v4 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_index;
      v5 = *(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_index];
      v6 = v5 + 1;
      if ( __OFADD__(v5, 1) )
      {
        __break(1u);
      }
      else
      {
        v5 = *(_QWORD *)&v0[v1];
        if ( !((unsigned __int64)v5 >> 62) )
        {
          result = *(_QWORD *)((v5 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
          if ( result )
          {
LABEL_6:
            if ( v6 == 0x8000000000000000LL && result == -1 )
            {
LABEL_29:
              __break(1u);
              return result;
            }
            v6 %= result;
            *(_QWORD *)&v0[v4] = v6;
            v1 = *(_QWORD *)&v0[v1];
            if ( (v1 & 0xC000000000000001LL) == 0 )
            {
              if ( v6 < 0 )
              {
                __break(1u);
              }
              else if ( (unsigned __int64)v6 < *(_QWORD *)((v1 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
              {
                v8 = objc_retain(*(id *)(v1 + 8 * v6 + 32));
LABEL_14:
                v9 = swift_allocObject(&unk_1005B8D18, 24, 7);
                *(_QWORD *)(v9 + 16) = v0;
                v10 = objc_retain(v0);
                sub_1001CEF28(sub_100045AC0, v9);
                objc_release(v8);
                return swift_release(v9);
              }
              __break(1u);
              goto LABEL_29;
            }
LABEL_26:
            swift_bridgeObjectRetain(v1);
            v8 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(v6, v1);
            swift_bridgeObjectRelease(v1);
            goto LABEL_14;
          }
LABEL_25:
          __break(1u);
          goto LABEL_26;
        }
      }
      if ( v5 < 0 )
        v12 = v5;
      else
        v12 = v5 & 0xFFFFFFFFFFFFFF8LL;
      result = _CocoaArrayWrapper.endIndex.getter(v12);
      if ( result )
        goto LABEL_6;
      goto LABEL_25;
    }
  }
  else
  {
    result = *(_QWORD *)((v2 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
    if ( result >= 1 )
      goto LABEL_3;
  }
  return result;
}

/* ========================================================================
 * sub_10004433C
 * EA: 0x10004433c
 ======================================================================== */

__int64 sub_10004433C()
{
  __int64 v0; // x19
  char *v1; // x20
  __int64 v2; // x21
  __int64 v3; // x8
  __int64 result; // x0
  __int64 v5; // x8
  __int64 v6; // x0
  __int64 v7; // x22
  __int64 v8; // x8
  bool v9; // vf
  __int64 v10; // x8
  id v12; // x19
  __int64 v13; // x21
  char *v14; // x0
  __int64 v15; // x0
  __int64 v16; // x0
  __int64 v17; // x0

  v2 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets;
  v3 = *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets];
  if ( (unsigned __int64)v3 >> 62 )
  {
    if ( v3 < 0 )
      v15 = *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets];
    else
      v15 = v3 & 0xFFFFFFFFFFFFFF8LL;
    result = _CocoaArrayWrapper.endIndex.getter(v15);
    if ( result >= 1 )
      goto LABEL_3;
  }
  else
  {
    result = *(_QWORD *)((v3 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
    if ( result >= 1 )
    {
LABEL_3:
      v5 = *(_QWORD *)&v1[v2];
      if ( (unsigned __int64)v5 >> 62 )
      {
        if ( v5 < 0 )
          v16 = *(_QWORD *)&v1[v2];
        else
          v16 = v5 & 0xFFFFFFFFFFFFFF8LL;
        v6 = _CocoaArrayWrapper.endIndex.getter(v16);
      }
      else
      {
        v6 = *(_QWORD *)((v5 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
      }
      v7 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_index;
      v8 = *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_index];
      v9 = __OFADD__(v6, v8);
      v10 = v6 + v8;
      if ( v9 )
      {
        __break(1u);
      }
      else
      {
        v0 = v10 - 1;
        if ( !__OFSUB__(v10, 1) )
        {
          v10 = *(_QWORD *)&v1[v2];
          if ( !((unsigned __int64)v10 >> 62) )
          {
            result = *(_QWORD *)((v10 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
            if ( result )
            {
LABEL_9:
              if ( v0 == 0x8000000000000000LL && result == -1 )
              {
LABEL_37:
                __break(1u);
                return result;
              }
              v0 %= result;
              *(_QWORD *)&v1[v7] = v0;
              v2 = *(_QWORD *)&v1[v2];
              if ( (v2 & 0xC000000000000001LL) == 0 )
              {
                if ( v0 < 0 )
                {
                  __break(1u);
                }
                else if ( (unsigned __int64)v0 < *(_QWORD *)((v2 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
                {
                  v12 = objc_retain(*(id *)(v2 + 8 * v0 + 32));
LABEL_17:
                  v13 = swift_allocObject(&unk_1005B8CF0, 24, 7);
                  *(_QWORD *)(v13 + 16) = v1;
                  v14 = objc_retain(v1);
                  sub_1001CEF28(sub_1000457F0, v13);
                  objc_release(v12);
                  return swift_release(v13);
                }
                __break(1u);
                goto LABEL_37;
              }
LABEL_34:
              swift_bridgeObjectRetain(v2);
              v12 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(v0, v2);
              swift_bridgeObjectRelease(v2);
              goto LABEL_17;
            }
LABEL_33:
            __break(1u);
            goto LABEL_34;
          }
LABEL_29:
          if ( v10 < 0 )
            v17 = v10;
          else
            v17 = v10 & 0xFFFFFFFFFFFFFF8LL;
          result = _CocoaArrayWrapper.endIndex.getter(v17);
          if ( result )
            goto LABEL_9;
          goto LABEL_33;
        }
      }
      __break(1u);
      goto LABEL_29;
    }
  }
  return result;
}

/* ========================================================================
 * sub_10004454C
 * EA: 0x10004454c
 ======================================================================== */

void *__fastcall sub_10004454C(void *a1)
{
  __int64 v1; // x20
  __int64 v2; // x27
  __int64 v4; // x20
  char *v5; // x21
  __int64 v6; // x28
  char *v7; // x23
  __int64 v8; // x19
  __int64 v9; // x24
  char *v10; // x22
  char *v11; // x25
  void *result; // x0
  void *v13; // x28
  void (__fastcall *v14)(char *, __int64); // x24
  __int64 v15; // x20
  __int64 v16; // x0
  void *v17; // x22
  __int64 v18; // x20
  id v19; // x0
  __int64 v20; // x0
  __int64 v21; // x26
  __int64 v22; // x20
  __int64 v23; // x0
  __int64 v24; // x27
  __int64 v25; // [xsp+0h] [xbp-A0h] BYREF
  __int64 v26; // [xsp+8h] [xbp-98h]
  __int64 v27; // [xsp+10h] [xbp-90h]
  __int64 v28; // [xsp+18h] [xbp-88h]
  _QWORD aBlock[5]; // [xsp+20h] [xbp-80h] BYREF
  __int64 v30; // [xsp+48h] [xbp-58h]

  v2 = v1;
  v27 = type metadata accessor for DispatchWorkItemFlags(0);
  v4 = *(_QWORD *)(v27 - 8);
  v5 = (char *)&v25 - ((*(_QWORD *)(v4 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v6 = type metadata accessor for DispatchQoS(0);
  v28 = *(_QWORD *)(v6 - 8);
  v7 = (char *)&v25 - ((*(_QWORD *)(v28 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v8 = type metadata accessor for DispatchTime(0);
  v9 = *(_QWORD *)(v8 - 8);
  v10 = (char *)&v25 - ((*(_QWORD *)(v9 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v11 = v10;
  result = *(void **)(v2 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player);
  if ( !result )
  {
    __break(1u);
    goto LABEL_6;
  }
  objc_msgSend(result, "pause");
  result = *(void **)(v2 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_playerItem);
  if ( !result )
  {
LABEL_6:
    __break(1u);
    goto LABEL_7;
  }
  v25 = v6;
  v26 = v4;
  if ( *(_QWORD *)(v2 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_videoOutput) )
  {
    objc_msgSend(result, "removeOutput:");
    objc_msgSend(*(id *)(v2 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView), "removeFromSuperview");
    sub_100040668(0);
    v13 = (void *)static OS_dispatch_queue.main.getter();
    static DispatchTime.now()();
    + infix(_:_:)(v10, 0.2);
    v14 = *(void (__fastcall **)(char *, __int64))(v9 + 8);
    v14(v10, v8);
    v15 = swift_allocObject(&unk_1005B8C78, 24, 7);
    swift_unknownObjectWeakInit(v15 + 16, v2);
    v16 = swift_allocObject(&unk_1005B8CA0, 32, 7);
    *(_QWORD *)(v16 + 16) = v15;
    *(_QWORD *)(v16 + 24) = a1;
    aBlock[4] = sub_1000457E8;
    v30 = v16;
    aBlock[0] = _NSConcreteStackBlock;
    aBlock[1] = 1107296256;
    aBlock[2] = sub_100040600;
    aBlock[3] = &unk_1005B8CB8;
    v17 = _Block_copy(aBlock);
    v18 = v30;
    v19 = objc_retain(a1);
    v20 = swift_release(v18);
    static DispatchQoS.unspecified.getter(v20);
    aBlock[0] = &_swiftEmptyArrayStorage;
    v21 = sub_10003EF70(
            &qword_100669D60,
            &type metadata accessor for DispatchWorkItemFlags,
            &protocol conformance descriptor for DispatchWorkItemFlags);
    v22 = sub_10003E4E0((__int64 *)&unk_100668B40, &qword_10053B8F0);
    v23 = sub_100040724();
    v24 = v27;
    dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v22, v23, v27, v21);
    OS_dispatch_queue.asyncAfter(deadline:qos:flags:execute:)(v11, v7, v5, v17);
    _Block_release(v17);
    objc_release(v13);
    (*(void (__fastcall **)(char *, __int64))(v26 + 8))(v5, v24);
    (*(void (__fastcall **)(char *, __int64))(v28 + 8))(v7, v25);
    return (void *)((__int64 (__fastcall *)(char *, __int64))v14)(v11, v8);
  }
LABEL_7:
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_100044868
 * EA: 0x100044868
 ======================================================================== */

void __fastcall sub_100044868(__int64 a1, void *a2)
{
  __int64 Strong; // x0
  char *v5; // x19
  void *v6; // x9
  id v7; // x0
  __int64 v8; // x0
  __int64 v9; // x22
  void *v10; // x8
  id v11; // x0
  void *v12; // x20
  id v13; // x20
  void *v14; // x22
  id v15; // x20
  __int64 v16; // x0
  __int64 inited; // x22
  __int64 v18; // x1
  __int64 v19; // x1
  __int64 v20; // x21
  __int64 v21; // x22
  id v22; // x23
  Class isa; // x24
  id v24; // x22
  void *v25; // x23
  id v26; // x22
  __int64 v27; // x22
  void *v28; // x0
  void *v29; // x8
  id v30; // x20
  _BYTE v31[80]; // [xsp+8h] [xbp-98h] BYREF
  _BYTE v32[24]; // [xsp+58h] [xbp-48h] BYREF

  swift_beginAccess(a1 + 16, v32, 0, 0);
  Strong = swift_unknownObjectWeakLoadStrong(a1 + 16);
  if ( Strong )
  {
    v5 = (char *)Strong;
    v6 = *(void **)(Strong + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_curPixelBuffer);
    *(_QWORD *)(Strong + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_curPixelBuffer) = 0;
    objc_release(v6);
    v7 = objc_allocWithZone((Class)type metadata accessor for SBSFastView());
    v8 = sub_1000B290C(0, 0.0, 0.0, 0.0, 0.0);
    v9 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView;
    v10 = *(void **)&v5[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView];
    *(_QWORD *)&v5[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView] = v8;
    objc_release(v10);
    v11 = objc_retainAutoreleasedReturnValue(objc_msgSend(v5, "view"));
    if ( v11 )
    {
      v12 = v11;
      objc_msgSend(v11, "addSubview:", *(_QWORD *)&v5[v9]);
      objc_release(v12);
      v13 = objc_retain(*(id *)&v5[v9]);
      ConstraintViewDSL.makeConstraints(_:)(sub_100044B1C, 0, v13);
      objc_release(v13);
      v14 = *(void **)&v5[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_playerItem];
      *(_QWORD *)&v5[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_playerItem] = a2;
      v15 = objc_retain(a2);
      objc_release(v14);
      *(_DWORD *)&v5[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_orientation] = sub_10016E170();
      v16 = sub_10003E4E0(&qword_10066D920, &qword_10053C530);
      inited = swift_initStackObject(v16, v31);
      *(_OWORD *)(inited + 16) = xmmword_10053B940;
      *(_QWORD *)(inited + 32) = static String._unconditionallyBridgeFromObjectiveC(_:)(
                                   kCVPixelBufferPixelFormatTypeKey,
                                   v18);
      *(_QWORD *)(inited + 72) = &type metadata for Int;
      *(_QWORD *)(inited + 40) = v19;
      *(_QWORD *)(inited + 48) = 1111970369;
      v20 = sub_100166EF8(inited);
      swift_setDeallocating(inited);
      sub_100045468(inited + 32);
      v21 = sub_10015FA84(v20);
      v22 = objc_allocWithZone((Class)&OBJC_CLASS___AVPlayerItemVideoOutput);
      sub_10003E4E0((__int64 *)&unk_10066E550, &qword_100541AF0);
      isa = Dictionary._bridgeToObjectiveC()().super.isa;
      swift_bridgeObjectRelease(v21);
      v24 = objc_msgSend(v22, "initWithPixelBufferAttributes:", isa);
      objc_release(isa);
      v25 = *(void **)&v5[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_videoOutput];
      *(_QWORD *)&v5[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_videoOutput] = v24;
      v26 = objc_retain(v24);
      objc_release(v25);
      if ( v26 )
      {
        objc_msgSend(v15, "addOutput:", v26);
        objc_release(v26);
        v27 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player;
        v28 = *(void **)&v5[OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player];
        if ( v28 )
        {
          objc_msgSend(v28, "replaceCurrentItemWithPlayerItem:", v15);
          v29 = *(void **)&v5[v27];
          if ( v29 )
          {
            v30 = objc_retain(v29);
            swift_bridgeObjectRelease(v20);
            objc_msgSend(v30, "play");
            objc_release(v5);
            objc_release(v30);
            return;
          }
LABEL_11:
          __break(1u);
          return;
        }
LABEL_10:
        __break(1u);
        goto LABEL_11;
      }
    }
    else
    {
      __break(1u);
    }
    __break(1u);
    goto LABEL_10;
  }
}

/* ========================================================================
 * sub_100044BEC
 * EA: 0x100044bec
 ======================================================================== */

void sub_100044BEC()
{
  __int64 v0; // x19
  __int64 v1; // x20
  __int64 v2; // x22
  __int64 v3; // x23
  __int64 v4; // x21
  __int64 v5; // x8
  __int64 v6; // x0
  char v7; // w1
  __int64 v8; // x0
  char isUniquelyReferenced_nonNull_native; // w0
  unsigned __int64 v10; // x8
  unsigned __int64 v11; // x11
  unsigned __int64 v12; // x9
  __int64 v13; // x8
  bool v14; // vf
  __int64 v15; // x8
  __int64 v16; // x3
  unsigned __int64 v17; // x19
  __int64 v18; // x0
  unsigned __int64 v19; // [xsp+8h] [xbp-38h]

  if ( *(_BYTE *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_shouldCheckBlackEdge) == 1 )
  {
    v4 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeCheckingCounter;
    v5 = *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeCheckingCounter);
    if ( v5 >= 1 )
    {
      if ( __ROR8__(0xAAAAAAAAAAAAAAABLL * v5, 2) > 0x1555555555555555uLL )
        goto LABEL_11;
      v6 = sub_10014ACD4();
      if ( (v7 & 1) != 0 )
        goto LABEL_11;
      if ( v6 > 19 )
      {
        v3 = v6;
        v2 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeHeights;
        v0 = *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeHeights);
        isUniquelyReferenced_nonNull_native = swift_isUniquelyReferenced_nonNull_native(v0);
        *(_QWORD *)(v1 + v2) = v0;
        if ( (isUniquelyReferenced_nonNull_native & 1) == 0 )
          goto LABEL_15;
        while ( 1 )
        {
          v10 = *(_QWORD *)(v0 + 16);
          v11 = *(_QWORD *)(v0 + 24);
          v12 = v10 + 1;
          if ( v10 >= v11 >> 1 )
          {
            v19 = v10 + 1;
            v16 = v0;
            v17 = *(_QWORD *)(v0 + 16);
            v18 = sub_100248DA4(v11 > 1, v10 + 1, 1, v16);
            v10 = v17;
            v12 = v19;
            v0 = v18;
          }
          *(_QWORD *)(v0 + 16) = v12;
          *(_QWORD *)(v0 + 8 * v10 + 32) = v3;
          *(_QWORD *)(v1 + v2) = v0;
LABEL_11:
          v13 = *(_QWORD *)(v1 + v4);
          v14 = __OFSUB__(v13, 1);
          v15 = v13 - 1;
          if ( !v14 )
            break;
          __break(1u);
LABEL_15:
          v0 = sub_100248DA4(0, *(_QWORD *)(v0 + 16) + 1LL, 1, v0);
          *(_QWORD *)(v1 + v2) = v0;
        }
        *(_QWORD *)(v1 + v4) = v15;
      }
      else
      {
        *(_QWORD *)(v1 + v4) = 0;
        v8 = *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeHeights);
        *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeHeights) = &_swiftEmptyArrayStorage;
        swift_bridgeObjectRelease(v8);
      }
    }
  }
}

/* ========================================================================
 * sub_100044D50
 * EA: 0x100044d50
 ======================================================================== */

void sub_100044D50()
{
  __int64 v0; // x20
  __int64 v1; // x19
  OSStatus v2; // w22
  Swift::String v3; // x0
  Swift::String v4; // x0
  void *object; // x22
  __int64 v6; // x0
  __int64 v7; // x0
  VTPixelRotationSessionRef v8; // x8
  void *v9; // x20
  OpaqueVTPixelRotationSession *v10; // x0
  VTPixelRotationSessionRef pixelRotationSessionOut; // [xsp+20h] [xbp-30h] BYREF

  v1 = v0;
  pixelRotationSessionOut = nullptr;
  v2 = VTPixelRotationSessionCreate(nullptr, &pixelRotationSessionOut);
  if ( pixelRotationSessionOut )
    VTSessionSetProperty(pixelRotationSessionOut, kVTPixelRotationPropertyKey_Rotation, kVTRotation_CW90);
  if ( v2 != (unsigned int)noErr.getter() )
  {
    type metadata accessor for VTLogger(0);
    _StringGuts.grow(_:)(59);
    v3._object = (void *)0x80000001004D9D30LL;
    v3._countAndFlagsBits = 0xD000000000000039LL;
    String.append(_:)(v3);
    v4._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                              &type metadata for Int32,
                              &protocol witness table for Int32);
    object = v4._object;
    String.append(_:)(v4);
    v6 = swift_bridgeObjectRelease(object);
    v7 = static os_log_type_t.error.getter(v6);
    sub_1001D8B44(v7, 0, 0xE000000000000000LL);
    swift_bridgeObjectRelease(0xE000000000000000LL);
  }
  v8 = pixelRotationSessionOut;
  v9 = *(void **)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_rotationCW90Session);
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_rotationCW90Session) = pixelRotationSessionOut;
  v10 = objc_retain(v8);
  objc_release(v9);
  objc_release(pixelRotationSessionOut);
}

/* ========================================================================
 * sub_100044EA8
 * EA: 0x100044ea8
 ======================================================================== */

CVPixelBufferRef __fastcall sub_100044EA8(__CVBuffer *a1)
{
  __int64 v1; // x20
  __int64 v3; // x19
  void *v4; // x8
  OpaqueVTPixelRotationSession *v5; // x19
  size_t Width; // x20
  size_t Height; // x22
  OSType PixelFormatType; // w0
  CVPixelBufferRef result; // x0
  OSStatus v10; // w22
  Swift::String v11; // x0
  void *object; // x22
  __int64 v13; // x0
  __int64 v14; // x0
  CVPixelBufferRef pixelBufferOut; // [xsp+20h] [xbp-30h] BYREF

  v3 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_rotationCW90Session;
  v4 = *(void **)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_rotationCW90Session);
  if ( v4 || (sub_100044D50(), (v4 = *(void **)(v1 + v3)) != nullptr) )
  {
    pixelBufferOut = nullptr;
    v5 = objc_retain(v4);
    Width = CVPixelBufferGetWidth(a1);
    Height = CVPixelBufferGetHeight(a1);
    PixelFormatType = CVPixelBufferGetPixelFormatType(a1);
    result = (CVPixelBufferRef)CVPixelBufferCreate(
                                 kCFAllocatorDefault,
                                 Height,
                                 Width,
                                 PixelFormatType,
                                 nullptr,
                                 &pixelBufferOut);
    if ( !pixelBufferOut )
    {
      __break(1u);
      return result;
    }
    v10 = VTPixelRotationSessionRotateImage(v5, a1, pixelBufferOut);
    if ( v10 == (unsigned int)noErr.getter() )
    {
      objc_release(v5);
      return pixelBufferOut;
    }
    type metadata accessor for VTLogger(0);
    _StringGuts.grow(_:)(42);
    swift_bridgeObjectRelease(0xE000000000000000LL);
    v11._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                               &type metadata for Int32,
                               &protocol witness table for Int32);
    object = v11._object;
    String.append(_:)(v11);
    v13 = swift_bridgeObjectRelease(object);
    v14 = static os_log_type_t.error.getter(v13);
    sub_1001D8B44(v14, 0xD000000000000028LL, 0x80000001004D9D00LL);
    swift_bridgeObjectRelease(0x80000001004D9D00LL);
    objc_release(v5);
    objc_release(pixelBufferOut);
  }
  return nullptr;
}

/* ========================================================================
 * sub_100045070
 * EA: 0x100045070
 ======================================================================== */

__int64 __fastcall sub_100045070(__int64 a1)
{
  _QWORD v2[23]; // [xsp+8h] [xbp-B8h] BYREF

  v2[0] = &unk_10053B9A8;
  v2[1] = &unk_10053B9A8;
  v2[2] = &unk_10053B9A8;
  v2[3] = &unk_10053B9A8;
  v2[4] = &unk_10053B9A8;
  v2[5] = &unk_10053B9A8;
  v2[6] = (char *)&value witness table for Builtin.UnknownObject + 64;
  v2[7] = &unk_10053B9A8;
  v2[8] = (char *)&value witness table for Builtin.UnknownObject + 64;
  v2[9] = (char *)&value witness table for Builtin.NativeObject + 64;
  v2[10] = (char *)&value witness table for Builtin.Int32 + 64;
  v2[11] = &unk_10053B9C0;
  v2[12] = &unk_10053B9C0;
  v2[13] = &unk_10053B9D8;
  v2[14] = &unk_10053B9A8;
  v2[15] = (char *)&value witness table for Builtin.Int64 + 64;
  v2[16] = (char *)&value witness table for Builtin.BridgeObject + 64;
  v2[17] = (char *)&value witness table for Builtin.Int64 + 64;
  v2[18] = &unk_10053B9C0;
  v2[19] = (char *)&value witness table for Builtin.Int64 + 64;
  v2[20] = (char *)&value witness table for Builtin.BridgeObject + 64;
  v2[21] = (char *)&value witness table for Builtin.Int64 + 64;
  v2[22] = &unk_10053B9A8;
  return swift_updateClassMetadata2(a1, 256, 23, v2, a1 + 480);
}

/* ========================================================================
 * $s15ExternalMonitor9VTVCStateOwet
 * EA: 0x100045134
 ======================================================================== */

__int64 __fastcall getEnumTagSinglePayload for VTVCState(unsigned __int8 *a1, unsigned int a2)
{
  int v2; // w9
  int v3; // w8
  int v4; // w8
  unsigned int v6; // w8
  bool v7; // cf
  int v8; // w8

  if ( !a2 )
    return 0;
  if ( a2 < 0xFE )
    goto LABEL_17;
  if ( a2 + 2 >= 0xFFFF00 )
    v2 = 4;
  else
    v2 = 2;
  if ( (a2 + 2) >> 8 < 0xFF )
    v3 = 1;
  else
    v3 = v2;
  if ( v3 == 4 )
  {
    v4 = *(_DWORD *)(a1 + 1);
    if ( v4 )
      return (*a1 | (unsigned int)(v4 << 8)) - 2;
  }
  else
  {
    if ( v3 == 2 )
    {
      v4 = *(unsigned __int16 *)(a1 + 1);
      if ( !*(_WORD *)(a1 + 1) )
        goto LABEL_17;
      return (*a1 | (unsigned int)(v4 << 8)) - 2;
    }
    v4 = a1[1];
    if ( a1[1] )
      return (*a1 | (unsigned int)(v4 << 8)) - 2;
  }
LABEL_17:
  v6 = *a1;
  v7 = v6 >= 3;
  v8 = v6 - 3;
  if ( !v7 )
    v8 = -1;
  return (unsigned int)(v8 + 1);
}

/* ========================================================================
 * $s15ExternalMonitor9VTVCStateOwst
 * EA: 0x1000451c4
 ======================================================================== */

__int64 __fastcall storeEnumTagSinglePayload for VTVCState(__int64 result, unsigned int a2, unsigned int a3)
{
  int v3; // w9
  int v4; // w8
  unsigned int v5; // w9

  if ( a3 + 2 >= 0xFFFF00 )
    v3 = 4;
  else
    v3 = 2;
  if ( (a3 + 2) >> 8 < 0xFF )
    v4 = 1;
  else
    v4 = v3;
  if ( a3 < 0xFE )
    v4 = 0;
  if ( a2 > 0xFD )
  {
    v5 = ((a2 - 254) >> 8) + 1;
    *(_BYTE *)result = a2 + 2;
    if ( v4 > 1 )
    {
      if ( v4 == 2 )
        *(_WORD *)(result + 1) = v5;
      else
        *(_DWORD *)(result + 1) = v5;
    }
    else if ( v4 )
    {
      *(_BYTE *)(result + 1) = v5;
    }
    return result;
  }
  if ( v4 > 1 )
  {
    if ( v4 != 2 )
    {
      *(_DWORD *)(result + 1) = 0;
      if ( a2 )
        goto LABEL_20;
      return result;
    }
    *(_WORD *)(result + 1) = 0;
  }
  else if ( v4 )
  {
    *(_BYTE *)(result + 1) = 0;
    if ( !a2 )
      return result;
LABEL_20:
    *(_BYTE *)result = a2 + 2;
    return result;
  }
  if ( a2 )
    goto LABEL_20;
  return result;
}

/* ========================================================================
 * sub_10004542C
 * EA: 0x10004542c
 ======================================================================== */

__int64 __fastcall sub_10004542C(__int64 a1, __int64 a2)
{
  (*(void (__fastcall **)(__int64, __int64))(*(&type metadata for String - 1) + 16LL))(a2, a1);
  return a2;
}

/* ========================================================================
 * sub_1000454C8
 * EA: 0x1000454c8
 ======================================================================== */

void __noreturn sub_1000454C8()
{
  __int64 v0; // x20
  __int64 v1; // x19
  __int64 v2; // x20
  __int64 v3; // x0
  __int64 v4; // x21
  id v5; // x0
  __int64 v6; // x21
  __int64 v7; // x0
  _QWORD *v8; // x20
  __int64 v9; // x20
  __int128 v10; // q1
  __int64 v11; // x21
  void *v12; // x23
  id v13; // x22
  id v14; // x23
  __int64 v15; // x0
  char v16; // w8
  _QWORD v17[2]; // [xsp+18h] [xbp-98h] BYREF
  _BYTE v18[24]; // [xsp+28h] [xbp-88h] BYREF
  __int128 v19; // [xsp+40h] [xbp-70h]
  __int128 v20; // [xsp+50h] [xbp-60h]
  __int64 v21; // [xsp+60h] [xbp-50h]
  __int128 v22; // [xsp+70h] [xbp-40h] BYREF

  v1 = v0;
  *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player) = 0;
  *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_playerItem) = 0;
  *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_videoOutput) = 0;
  *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_displayLink) = 0;
  *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_curPixelBuffer) = 0;
  *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_lastRenderedBuffer) = 0;
  v2 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_displayVc;
  v3 = type metadata accessor for VTStereoVideoPlayViewController(0);
  *(_QWORD *)(v1 + v2) = objc_msgSend(objc_allocWithZone((Class)swift_getObjCClassFromMetadata(v3)), "init");
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_task) = 0;
  v4 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_photoView;
  v5 = objc_allocWithZone((Class)type metadata accessor for SBSFastView());
  *(_QWORD *)(v1 + v4) = sub_1000B290C(0, 0.0, 0.0, 0.0, 0.0);
  v6 = OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsController;
  v7 = type metadata accessor for FPSController();
  v8 = (_QWORD *)swift_allocObject(v7, 144, 15);
  swift_defaultActor_initialize();
  v8[14] = 0;
  v8[15] = 0;
  *(_QWORD *)(v1 + v6) = v8;
  *(_DWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_orientation) = 1;
  v8[16] = 0;
  v8[17] = &_swiftEmptyArrayStorage;
  *(_BYTE *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_showImmersive3DGuide) = 0;
  *(_BYTE *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_stereoDisabled) = 0;
  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  v9 = qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ai3DOptionRaw;
  swift_beginAccess(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ai3DOptionRaw, v18, 0, 0);
  v10 = *(_OWORD *)(v9 + 16);
  v19 = *(_OWORD *)v9;
  v20 = v10;
  v21 = *(_QWORD *)(v9 + 32);
  v11 = *((_QWORD *)&v19 + 1);
  v12 = (void *)v10;
  v22 = *(_OWORD *)(v9 + 24);
  v13 = objc_retain((id)v19);
  swift_retain(v11);
  v14 = objc_retain(v12);
  sub_10004542C((__int64)&v22, (__int64)v17);
  v15 = sub_10003E4E0((__int64 *)&unk_10066AFA0, (__int64 *)&unk_10053BAB0);
  WrappedDefault.wrappedValue.getter(v17, v15);
  objc_release(v14);
  swift_release(v11);
  objc_release(v13);
  sub_10003F604(&v22);
  if ( v17[0] >= 3u )
    v16 = 1;
  else
    v16 = v17[0];
  *(_BYTE *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_option) = v16;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_fpsTimer) = 0;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_lastRemainBattery) = -1;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_assets) = &_swiftEmptyArrayStorage;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_index) = 0;
  *(_BYTE *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_shouldCheckBlackEdge) = 0;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeCheckingCounter) = 240;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeHeights) = &_swiftEmptyArrayStorage;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_blackEdgeThreshold) = 20;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_rotationCW90Session) = 0;
  _assertionFailure(_:_:file:line:flags:)(
    "Fatal error",
    11,
    2,
    0xD000000000000025LL,
    0x80000001004D9660LL,
    "ExternalMonitor/VTDepthVideoPlayer.swift",
    40,
    2,
    104,
    0);
  __break(1u);
}

/* ========================================================================
 * sub_100045AF4
 * EA: 0x100045af4
 ======================================================================== */

id __fastcall sub_100045AF4(double a1, double a2, double a3, double a4)
{
  char *v4; // x20
  char *v5; // x19
  __int64 v10; // x23
  __int64 v11; // x20
  __int64 v12; // x1
  __int64 v13; // x21
  objc_class *v14; // x22
  id v15; // x0
  __int64 v16; // x23
  __int64 v17; // x20
  __int64 v18; // x1
  __int64 v19; // x21
  id v20; // x0
  __int64 v21; // x20
  char *v22; // x8
  char *v23; // x20
  objc_class *v24; // x0
  objc_super v26; // [xsp+0h] [xbp-60h] BYREF

  v5 = v4;
  v10 = OBJC_IVAR____TtC15ExternalMonitor11ToolBarView_settingButton;
  v11 = sub_10024814C(0x676E6974746553LL, 0xE700000000000000LL, 0);
  v13 = v12;
  v14 = (objc_class *)type metadata accessor for VTImgTextButton(0);
  v15 = objc_allocWithZone(v14);
  *(_QWORD *)&v5[v10] = sub_100231184(v11, v13, 0, 0);
  v16 = OBJC_IVAR____TtC15ExternalMonitor11ToolBarView_moreHostsButton;
  v17 = sub_10024814C(0x4E207463656C6553LL, 0xEF74736F48207765LL, 0);
  v19 = v18;
  v20 = objc_allocWithZone(v14);
  *(_QWORD *)&v5[v16] = sub_100231184(v17, v19, 0, 0);
  v21 = OBJC_IVAR____TtC15ExternalMonitor11ToolBarView_titleLable;
  *(_QWORD *)&v5[v21] = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UILabel), "init");
  v22 = &v5[OBJC_IVAR____TtC15ExternalMonitor11ToolBarView_settingButtonBlock];
  *(_QWORD *)v22 = 0;
  *((_QWORD *)v22 + 1) = 0;
  v23 = &v5[OBJC_IVAR____TtC15ExternalMonitor11ToolBarView_moreHostsButtonBlock];
  v24 = (objc_class *)type metadata accessor for ToolBarView();
  *(_QWORD *)v23 = 0;
  *((_QWORD *)v23 + 1) = 0;
  v26.receiver = v5;
  v26.super_class = v24;
  return objc_msgSendSuper2(&v26, "initWithFrame:", a1, a2, a3, a4);
}

/* ========================================================================
 * sub_100045CB4
 * EA: 0x100045cb4
 ======================================================================== */

__int64 __fastcall sub_100045CB4(__int64 a1)
{
  __int64 v2; // x20
  __int64 v3; // x21
  __int64 v4; // x0
  __int64 v5; // x20
  __int64 v6; // x0
  __int64 v7; // x0
  __int64 v8; // x20
  __int64 v9; // x0
  _QWORD v11[5]; // [xsp+8h] [xbp-48h] BYREF

  v2 = (*(__int64 (**)(void))(*(_QWORD *)a1 + 96LL))();
  v3 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)v2 + 160LL))(v2);
  v4 = swift_release(v2);
  v5 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)v3 + 176LL))(v4);
  swift_release(v3);
  v6 = (*(__int64 (__fastcall **)(unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v5 + 112LL))(
         0xD000000000000072LL,
         0x80000001004DA020LL,
         29);
  swift_release(v6);
  v7 = swift_release(v5);
  v8 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)a1 + 152LL))(v7);
  v11[3] = &type metadata for Int;
  v11[4] = &protocol witness table for Int;
  v11[0] = 130;
  v9 = (*(__int64 (__fastcall **)(_QWORD *, unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v8 + 104LL))(
         v11,
         0xD000000000000072LL,
         0x80000001004DA020LL,
         30);
  swift_release(v9);
  swift_release(v8);
  return sub_10003F5DC(v11);
}

/* ========================================================================
 * sub_100045E34
 * EA: 0x100045e34
 ======================================================================== */

id sub_100045E34()
{
  char *v0; // x20
  void *v1; // x21
  void *v2; // x19
  void *v3; // x22
  NSString v4; // x23
  id v5; // x24
  id v6; // x23
  id v7; // x22
  id v8; // x21
  id v9; // x19

  v1 = *(void **)&v0[OBJC_IVAR____TtC15ExternalMonitor11ToolBarView_settingButton];
  objc_msgSend(v0, "addSubview:", v1);
  v2 = *(void **)&v0[OBJC_IVAR____TtC15ExternalMonitor11ToolBarView_moreHostsButton];
  objc_msgSend(v0, "addSubview:", v2);
  v3 = *(void **)&v0[OBJC_IVAR____TtC15ExternalMonitor11ToolBarView_titleLable];
  v4 = String._bridgeToObjectiveC()();
  v5 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIFont), "fontWithName:size:", v4, 35.0));
  objc_release(v4);
  objc_msgSend(v3, "setFont:", v5);
  objc_release(v5);
  v6 = objc_msgSend(
         objc_allocWithZone((Class)&OBJC_CLASS___UIColor),
         "initWithRed:green:blue:alpha:",
         0.694117647,
         0.819607843,
         1.0,
         1.0);
  objc_msgSend(v3, "setTextColor:", v6);
  objc_release(v6);
  objc_msgSend(v0, "addSubview:", v3);
  v7 = objc_retain(v3);
  ConstraintViewDSL.makeConstraints(_:)(sub_100046010, 0, v7);
  objc_release(v7);
  v8 = objc_retain(v1);
  ConstraintViewDSL.makeConstraints(_:)(sub_10004606C, 0, v8);
  objc_release(v8);
  v9 = objc_retain(v2);
  ConstraintViewDSL.makeConstraints(_:)(sub_1000461C4, 0, v9);
  objc_release(v9);
  objc_msgSend(v8, "addTarget:action:forControlEvents:", v0, "settingButtonClicked", 64);
  return objc_msgSend(v9, "addTarget:action:forControlEvents:", v0, "moreHostsButtonClicked", 64);
}

/* ========================================================================
 * sub_10004B430
 * EA: 0x10004b430
 ======================================================================== */

__int64 __fastcall sub_10004B430(__int64 result, unsigned __int64 a2)
{
  unsigned __int64 v2; // x19

  if ( a2 >> 62 != 1 )
  {
    if ( a2 >> 62 != 2 )
      return result;
    v2 = a2;
    swift_retain(result);
    a2 = v2;
  }
  return swift_retain(a2 & 0x3FFFFFFFFFFFFFFFLL);
}

/* ========================================================================
 * sub_10004B4D8
 * EA: 0x10004b4d8
 ======================================================================== */

__int64 __fastcall sub_10004B4D8(__int64 a1, unsigned __int64 *a2, _QWORD *a3)
{
  __int64 result; // x0
  __int64 v5; // x0

  result = *a2;
  if ( !*a2 )
  {
    v5 = objc_opt_self(*a3);
    result = swift_getObjCClassMetadata(v5);
    atomic_store(result, a2);
  }
  return result;
}

/* ========================================================================
 * sub_1000B27BC
 * EA: 0x1000b27bc
 ======================================================================== */

id sub_1000B27BC()
{
  char *v0; // x20
  __int64 v1; // x19
  void *v2; // x0
  objc_super v4; // [xsp+0h] [xbp-20h] BYREF

  v1 = *(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_task];
  if ( v1 )
  {
    swift_retain(*(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_task]);
    Task.cancel()();
    swift_release(v1);
  }
  v2 = *(void **)&v0[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_displayLink];
  if ( v2 )
    objc_msgSend(v2, "invalidate");
  v4.receiver = v0;
  v4.super_class = (Class)type metadata accessor for SBSFastView();
  return objc_msgSendSuper2(&v4, "dealloc");
}

/* ========================================================================
 * sub_1000B290C
 * EA: 0x1000b290c
 ======================================================================== */

_BYTE *__fastcall sub_1000B290C(char a1, double a2, double a3, double a4, double a5)
{
  _BYTE *v5; // x20
  _BYTE *v6; // x21
  __int64 v12; // x22
  __int64 v13; // x24
  char *v14; // x20
  __int64 v15; // x25
  __int64 v16; // x0
  double v17; // d12
  objc_class *v18; // x0
  _BYTE *v19; // x20
  id v20; // x0
  __int64 v21; // x19
  void *v22; // x8
  void *v23; // x21
  id v24; // x19
  void *v25; // x19
  id v26; // x21
  objc_super v28; // [xsp+0h] [xbp-80h] BYREF

  v6 = v5;
  v12 = type metadata accessor for Date(0);
  v13 = *(_QWORD *)(v12 - 8);
  v14 = (char *)&v28 - ((*(_QWORD *)(v13 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  *(_QWORD *)&v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_mtkView] = 0;
  *(_QWORD *)&v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_ciContext] = 0;
  *(_QWORD *)&v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_leftCIImage] = 0;
  *(_QWORD *)&v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_rightCIImage] = 0;
  *(_DWORD *)&v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_orientation] = 1;
  *(_QWORD *)&v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_displayLink] = 0;
  *(_QWORD *)&v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_lastRenderedLeftCIImage] = 0;
  v15 = OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_lastRenderTime;
  v16 = Date.init()();
  v17 = Date.timeIntervalSince1970.getter(v16);
  (*(void (__fastcall **)(char *, __int64))(v13 + 8))(v14, v12);
  *(double *)&v6[v15] = v17;
  *(_QWORD *)&v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_queue] = 0;
  v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_shouldUseDisplayLink] = 0;
  *(_QWORD *)&v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_task] = 0;
  v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_isRendering] = 0;
  *(_QWORD *)&v6[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_lastPresentedTime] = 0;
  v18 = (objc_class *)type metadata accessor for SBSFastView();
  v28.receiver = v6;
  v28.super_class = v18;
  v19 = objc_retainAutoreleasedReturnValue(objc_msgSendSuper2(&v28, "initWithFrame:", a2, a3, a4, a5));
  sub_1000B2CF8();
  v19[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_shouldUseDisplayLink] = a1;
  v20 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___CADisplayLink), "displayLinkWithTarget:selector:", v19, "tick"));
  v21 = OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_displayLink;
  v22 = *(void **)&v19[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_displayLink];
  *(_QWORD *)&v19[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_displayLink] = v20;
  objc_release(v22);
  v23 = *(void **)&v19[v21];
  v24 = v19;
  if ( v23 )
  {
    v25 = (void *)objc_opt_self(&OBJC_CLASS___NSRunLoop);
    v26 = objc_retain(v23);
    v24 = objc_retainAutoreleasedReturnValue(objc_msgSend(v25, "mainRunLoop"));
    objc_msgSend(v26, "addToRunLoop:forMode:", v24, NSRunLoopCommonModes);
    objc_release(v19);
    objc_release(v26);
  }
  objc_release(v24);
  return v19;
}

/* ========================================================================
 * sub_1000B2CF8
 * EA: 0x1000b2cf8
 ======================================================================== */

void *sub_1000B2CF8()
{
  char *v0; // x20
  char *v1; // x25
  __int64 v2; // x19
  __int64 v3; // x26
  char *v4; // x21
  __int64 v5; // x22
  __int64 v6; // x27
  char *v7; // x23
  char *v8; // x20
  id v9; // x0
  id v10; // x24
  id v11; // x26
  __int64 v12; // x28
  void *v13; // x27
  id v14; // x26
  id v15; // x0
  void *v16; // x9
  void *result; // x0
  id v18; // x0
  id v19; // x27
  id v20; // x26
  __int64 v21; // x0
  __int64 v22; // x19
  __int64 v23; // x0
  void *v24; // x8
  id v25; // x26
  __int64 v26; // x0
  __int64 v27; // x26
  _QWORD *v28; // x0
  __int64 v29; // x26
  void *v30; // x27
  __int64 v31; // x20
  __int64 v32; // x0
  void *v33; // x25
  __int64 v34; // x20
  __int64 v35; // x0
  __int64 v36; // x28
  __int64 v37; // x20
  __int64 v38; // x0
  __int64 v39; // [xsp+0h] [xbp-90h] BYREF
  __int64 v40; // [xsp+8h] [xbp-88h]
  _QWORD aBlock[5]; // [xsp+10h] [xbp-80h] BYREF
  __int64 v42; // [xsp+38h] [xbp-58h]

  v1 = v0;
  v2 = type metadata accessor for DispatchWorkItemFlags(0);
  v3 = *(_QWORD *)(v2 - 8);
  v4 = (char *)&v39 - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for DispatchQoS(0);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = (char *)&v39 - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v8 = (char *)&v39
     - ((*(_QWORD *)(*(_QWORD *)(sub_10003E4E0((__int64 *)&unk_100668B60, &qword_10053BAE0) - 8) + 64LL) + 15LL)
      & 0xFFFFFFFFFFFFFFF0LL);
  v9 = MTLCreateSystemDefaultDevice();
  if ( !v9 )
  {
    v21 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
    v22 = swift_allocObject(v21, 64, 7);
    *(_OWORD *)(v22 + 16) = xmmword_10053B940;
    *(_QWORD *)(v22 + 56) = &type metadata for String;
    *(_QWORD *)(v22 + 32) = 0xD00000000000003ALL;
    *(_QWORD *)(v22 + 40) = 0x80000001004E3270LL;
    print(_:separator:terminator:)(v22, 32, 0xE100000000000000LL, 10, 0xE100000000000000LL);
    return (void *)swift_bridgeObjectRelease(v22);
  }
  v10 = v9;
  v39 = v6;
  v40 = v3;
  v11 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___MTKView), "initWithFrame:device:", v9, 0.0, 0.0, 0.0, 0.0);
  objc_msgSend(v11, "setFramebufferOnly:", 0);
  objc_msgSend(v11, "setEnableSetNeedsDisplay:", 1);
  objc_msgSend(v11, "setPaused:", 0);
  v12 = OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_mtkView;
  v13 = *(void **)&v1[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_mtkView];
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_mtkView] = v11;
  v14 = objc_retain(v11);
  objc_release(v13);
  objc_msgSend(v1, "addSubview:", v14);
  objc_release(v14);
  v15 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___CIContext), "contextWithMTLDevice:", v10));
  v16 = *(void **)&v1[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_ciContext];
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_ciContext] = v15;
  objc_release(v16);
  result = *(void **)&v1[v12];
  if ( result )
  {
    v18 = objc_retainAutoreleasedReturnValue(objc_msgSend(result, "device"));
    if ( v18 )
    {
      v19 = v18;
      v20 = objc_msgSend(v18, "newCommandQueue");
      swift_unknownObjectRelease(v19);
    }
    else
    {
      v20 = nullptr;
    }
    v23 = *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_queue];
    *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_queue] = v20;
    result = (void *)swift_unknownObjectRelease(v23);
    v24 = *(void **)&v1[v12];
    if ( v24 )
    {
      v25 = objc_retain(v24);
      ConstraintViewDSL.remakeConstraints(_:)(sub_1000B3E44, 0, v25);
      objc_release(v25);
      static TaskPriority.userInitiated.getter();
      v26 = type metadata accessor for TaskPriority(0);
      (*(void (__fastcall **)(char *, _QWORD, __int64, __int64))(*(_QWORD *)(v26 - 8) + 56LL))(v8, 0, 1, v26);
      v27 = swift_allocObject(&unk_1005BBD30, 24, 7);
      swift_unknownObjectWeakInit(v27 + 16, v1);
      v28 = (_QWORD *)swift_allocObject(&unk_1005BBEE8, 40, 7);
      v28[2] = 0;
      v28[3] = 0;
      v28[4] = v27;
      v29 = sub_10015B480(0, 0, v8, &unk_10053D1F8, v28);
      sub_10005285C(v8, &unk_100668B60, &qword_10053BAE0);
      sub_100040668(0);
      v30 = (void *)static OS_dispatch_queue.main.getter();
      v31 = swift_allocObject(&unk_1005BBD30, 24, 7);
      swift_unknownObjectWeakInit(v31 + 16, v1);
      v32 = swift_allocObject(&unk_1005BBF10, 32, 7);
      *(_QWORD *)(v32 + 16) = v31;
      *(_QWORD *)(v32 + 24) = v29;
      aBlock[4] = sub_1000B5F30;
      v42 = v32;
      aBlock[0] = _NSConcreteStackBlock;
      aBlock[1] = 1107296256;
      aBlock[2] = sub_100040600;
      aBlock[3] = &unk_1005BBF28;
      v33 = _Block_copy(aBlock);
      v34 = v42;
      swift_retain(v29);
      v35 = swift_release(v34);
      static DispatchQoS.unspecified.getter(v35);
      aBlock[0] = &_swiftEmptyArrayStorage;
      v36 = sub_10003EF70(
              &qword_100669D60,
              &type metadata accessor for DispatchWorkItemFlags,
              &protocol conformance descriptor for DispatchWorkItemFlags);
      v37 = sub_10003E4E0((__int64 *)&unk_100668B40, &qword_10053B8F0);
      v38 = sub_100040724();
      dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v37, v38, v2, v36);
      OS_dispatch_queue.async(group:qos:flags:execute:)(0, v7, v4, v33);
      _Block_release(v33);
      swift_unknownObjectRelease(v10);
      swift_release(v29);
      objc_release(v30);
      (*(void (__fastcall **)(char *, __int64))(v40 + 8))(v4, v2);
      return (void *)(*(__int64 (__fastcall **)(char *, __int64))(v39 + 8))(v7, v5);
    }
  }
  else
  {
    __break(1u);
  }
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_1000B3E44
 * EA: 0x1000b3e44
 ======================================================================== */

__int64 __fastcall sub_1000B3E44(__int64 a1)
{
  __int64 v1; // x20
  __int64 v2; // x0

  v1 = (*(__int64 (**)(void))(*(_QWORD *)a1 + 264LL))();
  v2 = (*(__int64 (__fastcall **)(unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v1 + 112LL))(
         0xD000000000000065LL,
         0x80000001004E32B0LL,
         139);
  swift_release(v2);
  return swift_release(v1);
}

/* ========================================================================
 * sub_10015C79C
 * EA: 0x10015c79c
 ======================================================================== */

void __fastcall sub_10015C79C(id a1)
{
  _QWORD *v1; // x20
  __int64 v3; // x20
  __int64 v4; // x0
  __int64 v5; // x22
  id v6; // x24
  id v7; // x0
  void *v8; // x0
  id v9; // x0
  id v10; // x25
  void *v11; // x24
  id v12; // x0
  NSString v13; // x20
  id v14; // x22
  __int64 v15; // x0
  id v16; // x24
  id v17; // x0
  void *v18; // x0
  id v19; // x0
  id v20; // x0
  NSString v21; // x20
  id v22; // x22
  id v23; // x20
  id v24; // x24
  id v25; // [xsp+0h] [xbp-40h] BYREF

  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  if ( *(_BYTE *)(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences_isGaming) == 1 )
  {
    v3 = v1[2];
    if ( !v3 )
    {
      __break(1u);
      goto LABEL_16;
    }
    v4 = type metadata accessor for DepthAnythingV2float16_364_280Input();
    v5 = swift_allocObject(v4, 24, 7);
    *(_QWORD *)(v5 + 16) = a1;
    v6 = objc_allocWithZone((Class)&OBJC_CLASS___MLPredictionOptions);
    v7 = objc_retain(a1);
    swift_retain(v3);
    a1 = objc_msgSend(v6, "init");
    v8 = *(void **)(v3 + 16);
    v25 = nullptr;
    v9 = objc_retainAutoreleasedReturnValue(objc_msgSend(v8, "predictionFromFeatures:options:error:", v5, a1, &v25));
    v10 = v25;
    if ( !v9 )
    {
LABEL_14:
      v24 = objc_retain(v10);
      _convertNSErrorToError(_:)(v10);
      objc_release(v24);
      swift_willThrow();
      swift_release(v3);
      swift_release(v5);
      objc_release(a1);
      return;
    }
    v11 = v9;
    v12 = objc_retain(v25);
    swift_release(v3);
    swift_release(v5);
    objc_release(a1);
    v13 = String._bridgeToObjectiveC()();
    v14 = objc_retainAutoreleasedReturnValue(objc_msgSend(v11, "featureValueForName:", v13));
    objc_release(v13);
    if ( !v14 )
    {
LABEL_17:
      __break(1u);
      goto LABEL_18;
    }
    v1 = objc_retainAutoreleasedReturnValue(objc_msgSend(v14, "imageBufferValue"));
    objc_release(v14);
    if ( v1 )
    {
LABEL_13:
      swift_unknownObjectRelease(v11);
      return;
    }
    __break(1u);
  }
  v3 = v1[3];
  if ( !v3 )
  {
LABEL_16:
    __break(1u);
    goto LABEL_17;
  }
  v15 = type metadata accessor for DepthAnythingV2float16Input();
  v5 = swift_allocObject(v15, 24, 7);
  *(_QWORD *)(v5 + 16) = a1;
  v16 = objc_allocWithZone((Class)&OBJC_CLASS___MLPredictionOptions);
  v17 = objc_retain(a1);
  swift_retain(v3);
  a1 = objc_msgSend(v16, "init");
  v18 = *(void **)(v3 + 16);
  v25 = nullptr;
  v19 = objc_retainAutoreleasedReturnValue(objc_msgSend(v18, "predictionFromFeatures:options:error:", v5, a1, &v25));
  v10 = v25;
  if ( !v19 )
    goto LABEL_14;
  v11 = v19;
  v20 = objc_retain(v25);
  swift_release(v3);
  swift_release(v5);
  objc_release(a1);
  v21 = String._bridgeToObjectiveC()();
  v22 = objc_retainAutoreleasedReturnValue(objc_msgSend(v11, "featureValueForName:", v21));
  objc_release(v21);
  if ( v22 )
  {
    v23 = objc_retainAutoreleasedReturnValue(objc_msgSend(v22, "imageBufferValue"));
    objc_release(v22);
    if ( v23 )
      goto LABEL_13;
    goto LABEL_19;
  }
LABEL_18:
  __break(1u);
LABEL_19:
  __break(1u);
}

/* ========================================================================
 * sub_1001A3FF0
 * EA: 0x1001a3ff0
 ======================================================================== */

unsigned __int64 sub_1001A3FF0()
{
  unsigned __int64 result; // x0

  result = qword_10066FA60;
  if ( !qword_10066FA60 )
  {
    result = swift_getWitnessTable(&unk_100542940, &type metadata for DFUTestError);
    atomic_store(result, (unsigned __int64 *)&qword_10066FA60);
  }
  return result;
}

/* ========================================================================
 * sub_1001A4044
 * EA: 0x1001a4044
 ======================================================================== */

unsigned __int64 sub_1001A4044()
{
  unsigned __int64 result; // x0

  result = qword_10066FA68;
  if ( !qword_10066FA68 )
  {
    result = swift_getWitnessTable(&unk_100542918, &type metadata for DFUTestError);
    atomic_store(result, (unsigned __int64 *)&qword_10066FA68);
  }
  return result;
}

/* ========================================================================
 * sub_1001A4088
 * EA: 0x1001a4088
 ======================================================================== */

void *sub_1001A4088()
{
  __CVBuffer *v0; // x20
  __CVBuffer *v1; // x21
  void *v2; // x19
  vImagePixelCount Width; // x20
  signed __int64 Height; // x24
  size_t BytesPerRow; // x25
  void *BaseAddress; // x0
  void *v7; // x0
  void *v8; // x22
  vImage_Error v9; // x0
  void *v10; // x26
  Swift::String v11; // x0
  void *object; // x24
  __int64 v13; // x0
  __int64 v14; // x0
  __int64 v15; // x0
  __int64 v16; // x0
  __int64 v17; // x0
  char *v18; // x22
  __int64 v19; // x0
  __int64 v20; // x1
  __int64 v21; // x0
  void *v22; // x0
  void *v23; // x23
  vImage_Error v24; // x0
  vImage_Error v25; // x26
  Swift::String v26; // x0
  void *v27; // x25
  __int64 v28; // x0
  __int64 v29; // x0
  __int64 v30; // x0
  __int64 v31; // x0
  void *v33; // x8
  void *v34; // x27
  __CVBuffer *v35; // x26
  void *v36; // x0
  Swift::String v37; // x0
  void *v38; // x25
  __int64 v39; // x0
  __int64 v40; // x0
  __int64 v41; // x0
  __int64 inited; // x27
  __CFString *v43; // x8
  __CFString *v44; // x0
  __int64 v45; // x26
  __int64 v46; // x0
  __CFString *v47; // x8
  CFStringRef v48; // x11
  __CFString *v49; // x28
  __CFString *v50; // x26
  __CFString *v51; // x0
  __CFString *v52; // x0
  __CFString *v53; // x0
  __CFString *v54; // x0
  __int64 v55; // x28
  __int64 v56; // x0
  const __CFDictionary *isa; // x27
  __int64 v58; // x0
  __int64 v59; // x0
  __int64 v60; // x0
  __int64 v61; // x0
  __CFString *v62; // [xsp+8h] [xbp-208h]
  vImage_Buffer v63; // [xsp+30h] [xbp-1E0h] BYREF
  char v64[232]; // [xsp+50h] [xbp-1C0h] BYREF
  vImage_Error v65[3]; // [xsp+138h] [xbp-D8h] BYREF
  vImage_Buffer v66; // [xsp+150h] [xbp-C0h] BYREF
  vImage_Buffer dest; // [xsp+170h] [xbp-A0h] BYREF
  vImage_Buffer src; // [xsp+190h] [xbp-80h] BYREF

  v1 = v0;
  if ( qword_100662498 != -1 )
    swift_once(&qword_100662498, sub_1001A47F8);
  v2 = (void *)qword_10066FA70;
  objc_msgSend((id)qword_10066FA70, "lock");
  if ( CVPixelBufferGetPixelFormatType(v0) != 1278226536 )
  {
    v15 = type metadata accessor for AILogger(0);
    v16 = static os_log_type_t.error.getter(v15);
    sub_1001D8B44(v16, 0x1000000000000041LL, 0x80000001004EF750LL);
LABEL_22:
    objc_msgSend(v2, "unlock");
    return nullptr;
  }
  CVPixelBufferLockBaseAddress(v0, 1u);
  Width = CVPixelBufferGetWidth(v0);
  Height = CVPixelBufferGetHeight(v1);
  BytesPerRow = CVPixelBufferGetBytesPerRow(v1);
  BaseAddress = CVPixelBufferGetBaseAddress(v1);
  if ( !BaseAddress )
  {
    v17 = type metadata accessor for AILogger(0);
    v18 = "neComponent16Half";
    v19 = static os_log_type_t.error.getter(v17);
    v20 = 0x1000000000000025LL;
LABEL_14:
    sub_1001D8B44(v19, v20, (unsigned __int64)v18 | 0x8000000000000000LL);
LABEL_21:
    CVPixelBufferUnlockBaseAddress(v1, 1u);
    goto LABEL_22;
  }
  if ( ((Height | Width) & 0x8000000000000000LL) != 0 )
  {
    __break(1u);
    goto LABEL_33;
  }
  src.data = BaseAddress;
  src.height = Height;
  src.width = Width;
  src.rowBytes = BytesPerRow;
  if ( (Width - 0x2000000000000000LL) >> 62 != 3 )
  {
LABEL_33:
    __break(1u);
LABEL_34:
    __break(1u);
  }
  if ( (unsigned __int128)(Height * (__int128)(__int64)(4 * Width)) >> 64 != (__int64)(Height * 4 * Width) >> 63 )
    goto LABEL_34;
  v7 = malloc(Height * 4 * Width);
  if ( !v7 )
  {
    v21 = type metadata accessor for AILogger(0);
    v18 = "elBuffer 的基地址";
    v19 = static os_log_type_t.error.getter(v21);
    v20 = 0x1000000000000021LL;
    goto LABEL_14;
  }
  v8 = v7;
  dest.data = v7;
  dest.height = Height;
  dest.width = Width;
  dest.rowBytes = 4 * Width;
  v9 = vImageConvert_Planar16FtoPlanarF(&src, &dest, 0);
  if ( v9 )
  {
    v10 = (void *)v9;
    type metadata accessor for AILogger(0);
    _StringGuts.grow(_:)(49);
    swift_bridgeObjectRelease(0xE000000000000000LL);
    v66.data = (void *)0x100000000000002FLL;
    v66.height = 0x80000001004EF800LL;
    v63.data = v10;
    v11._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                               &type metadata for Int,
                               &protocol witness table for Int);
    object = v11._object;
    String.append(_:)(v11);
    v13 = swift_bridgeObjectRelease(object);
    v14 = static os_log_type_t.error.getter(v13);
    sub_1001D8B44(v14, 0x100000000000002FLL, 0x80000001004EF800LL);
    swift_bridgeObjectRelease(0x80000001004EF800LL);
LABEL_20:
    free(v8);
    goto LABEL_21;
  }
  v22 = malloc(Height * 4 * Width);
  if ( !v22 )
  {
    v30 = type metadata accessor for AILogger(0);
    v31 = static os_log_type_t.error.getter(v30);
    sub_1001D8B44(v31, 0x1000000000000021LL, 0x80000001004EF830LL);
    goto LABEL_20;
  }
  v23 = v22;
  v66.data = v22;
  v66.height = Height;
  v66.width = Width;
  v66.rowBytes = 4 * Width;
  v24 = vImageEqualization_PlanarF(&dest, &v66, nullptr, 0x100u, 0.1, 1.0, 0);
  if ( v24 )
  {
    v25 = v24;
    type metadata accessor for AILogger(0);
    _StringGuts.grow(_:)(41);
    swift_bridgeObjectRelease(0xE000000000000000LL);
    v63.data = (void *)0x1000000000000027LL;
    v63.height = 0x80000001004EF860LL;
    v65[0] = v25;
    v26._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                               &type metadata for Int,
                               &protocol witness table for Int);
    v27 = v26._object;
    String.append(_:)(v26);
    v28 = swift_bridgeObjectRelease(v27);
    v29 = static os_log_type_t.error.getter(v28);
    sub_1001D8B44(v29, 0x1000000000000027LL, 0x80000001004EF860LL);
    swift_bridgeObjectRelease(0x80000001004EF860LL);
LABEL_18:
    free(v23);
    goto LABEL_20;
  }
  swift_beginAccess(&qword_100697008, v65, 0, 0);
  v33 = (void *)qword_100697008;
  if ( !qword_100697008 )
  {
    v41 = sub_10003E4E0((__int64 *)&unk_10066B7D0, qword_10053DD50);
    inited = swift_initStackObject(v41, v64);
    *(_OWORD *)(inited + 16) = xmmword_10053DCE0;
    v43 = (__CFString *)kCVPixelBufferIOSurfacePropertiesKey;
    *(_QWORD *)(inited + 32) = kCVPixelBufferIOSurfacePropertiesKey;
    v44 = objc_retain(v43);
    v45 = sub_100166CB0(&_swiftEmptyArrayStorage);
    v46 = sub_10003E4E0(&qword_10066B7C8, (__int64 *)&unk_10053DD40);
    *(_QWORD *)(inited + 40) = v45;
    v47 = (__CFString *)kCVPixelBufferPixelFormatTypeKey;
    *(_QWORD *)(inited + 64) = v46;
    *(_QWORD *)(inited + 72) = v47;
    *(_DWORD *)(inited + 80) = 1278226536;
    v48 = kCVPixelBufferWidthKey;
    v62 = (__CFString *)kCVPixelBufferWidthKey;
    *(_QWORD *)(inited + 104) = &type metadata for UInt32;
    *(_QWORD *)(inited + 112) = v48;
    *(_QWORD *)(inited + 120) = Width;
    v49 = (__CFString *)kCVPixelBufferHeightKey;
    *(_QWORD *)(inited + 144) = &type metadata for Int;
    *(_QWORD *)(inited + 152) = v49;
    *(_QWORD *)(inited + 160) = Height;
    v50 = (__CFString *)kCVPixelBufferMetalCompatibilityKey;
    *(_QWORD *)(inited + 184) = &type metadata for Int;
    *(_QWORD *)(inited + 192) = v50;
    *(_QWORD *)(inited + 224) = &type metadata for Bool;
    *(_BYTE *)(inited + 200) = 1;
    v51 = objc_retain(v47);
    v52 = objc_retain(v62);
    v53 = objc_retain(v49);
    v54 = objc_retain(v50);
    v55 = sub_100167024(inited);
    swift_setDeallocating(inited);
    v56 = sub_10003E4E0(&qword_10066E540, &qword_100541AE0);
    swift_arrayDestroy(inited + 32, 5, v56);
    type metadata accessor for CFString(0);
    sub_1000FE150();
    isa = Dictionary._bridgeToObjectiveC()().super.isa;
    swift_bridgeObjectRelease(v55);
    swift_beginAccess(&qword_100697008, &v63, 33, 0);
    LODWORD(v50) = CVPixelBufferCreate(
                     kCFAllocatorDefault,
                     Width,
                     Height,
                     0x4C303068u,
                     isa,
                     (CVPixelBufferRef *)&qword_100697008);
    swift_endAccess(&v63);
    objc_release(isa);
    if ( (_DWORD)v50 )
    {
      v58 = type metadata accessor for AILogger(0);
      v59 = static os_log_type_t.error.getter(v58);
      sub_1001D8B44(v59, 0x100000000000001ELL, 0x80000001004EF890LL);
      free(v8);
      free(v23);
      goto LABEL_18;
    }
    v33 = (void *)qword_100697008;
    if ( !qword_100697008 )
    {
      v60 = type metadata accessor for AILogger(0);
      v61 = static os_log_type_t.error.getter(v60);
      sub_1001D8B44(v61, 0x100000000000001ELL, 0x80000001004EF890LL);
      goto LABEL_18;
    }
  }
  v34 = v33;
  v35 = objc_retain(v33);
  CVPixelBufferLockBaseAddress(v35, 1u);
  v36 = CVPixelBufferGetBaseAddress(v35);
  if ( v36 )
  {
    v63.data = v36;
    v63.height = Height;
    v63.width = Width;
    v63.rowBytes = BytesPerRow;
    if ( vImageConvert_PlanarFtoPlanar16F(&v66, &v63, 0) )
    {
      type metadata accessor for AILogger(0);
      _StringGuts.grow(_:)(49);
      swift_bridgeObjectRelease(0xE000000000000000LL);
      v37._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                                 &type metadata for Int,
                                 &protocol witness table for Int);
      v38 = v37._object;
      String.append(_:)(v37);
      v39 = swift_bridgeObjectRelease(v38);
      v40 = static os_log_type_t.error.getter(v39);
      sub_1001D8B44(v40, 0x100000000000002FLL, 0x80000001004EF8B0LL);
      swift_bridgeObjectRelease(0x80000001004EF8B0LL);
      CVPixelBufferUnlockBaseAddress(v35, 1u);
      objc_release(v35);
      goto LABEL_18;
    }
  }
  CVPixelBufferUnlockBaseAddress(v35, 1u);
  free(v23);
  free(v8);
  CVPixelBufferUnlockBaseAddress(v1, 1u);
  objc_msgSend(v2, "unlock");
  return v34;
}

/* ========================================================================
 * sub_1001A4828
 * EA: 0x1001a4828
 ======================================================================== */

_QWORD *sub_1001A4828()
{
  _QWORD *v0; // x20
  id v1; // x0
  void *v2; // x19
  id v3; // x0
  id v4; // x22
  __int64 v5; // x0
  __int64 v6; // x23
  id v7; // x0
  CVMetalTextureCacheRef v8; // x21
  id v9; // x25
  __CVMetalTextureCache *v10; // x0
  __int64 v11; // x1
  __int64 v12; // x1
  __int64 v13; // x1
  CVReturn v14; // w21
  void *v15; // x0
  __CVMetalTextureCache *v16; // x24
  __int64 v17; // x25
  __int64 v18; // x0
  CVMetalTextureCacheRef cacheOut; // [xsp+10h] [xbp-50h] BYREF

  v1 = MTLCreateSystemDefaultDevice();
  if ( !v1 )
    goto LABEL_11;
  v2 = v1;
  v3 = objc_msgSend(v1, "newCommandQueue");
  if ( !v3 )
  {
LABEL_9:
    v15 = v2;
    goto LABEL_10;
  }
  v4 = v3;
  v5 = sub_1001A5108(0x1000000000000AEBLL, 0x80000001004EF930LL, v2);
  if ( v5 )
  {
    v6 = v5;
    cacheOut = nullptr;
    v7 = objc_msgSend(v2, "newComputePipelineStateWithFunction:error:", v5, &cacheOut);
    v8 = cacheOut;
    if ( v7 )
    {
      v0[2] = v2;
      v0[3] = v4;
      v0[4] = v7;
      cacheOut = nullptr;
      v9 = v7;
      v10 = objc_retain(v8);
      swift_unknownObjectRetain(v2, v11);
      swift_unknownObjectRetain(v4, v12);
      swift_unknownObjectRetain(v9, v13);
      v14 = CVMetalTextureCacheCreate(kCFAllocatorDefault, nullptr, v2, nullptr, &cacheOut);
      swift_unknownObjectRelease(v2);
      swift_unknownObjectRelease(v9);
      swift_unknownObjectRelease(v4);
      swift_unknownObjectRelease(v6);
      if ( !v14 )
      {
        v0[5] = cacheOut;
        return v0;
      }
      objc_release(cacheOut);
      swift_unknownObjectRelease(v0[2]);
      swift_unknownObjectRelease(v0[3]);
      v15 = (void *)v0[4];
      goto LABEL_10;
    }
    v16 = objc_retain(cacheOut);
    v17 = _convertNSErrorToError(_:)(v8);
    objc_release(v16);
    swift_willThrow();
    swift_unknownObjectRelease(v4);
    swift_unknownObjectRelease(v6);
    swift_errorRelease(v17);
    goto LABEL_9;
  }
  swift_unknownObjectRelease(v2);
  v15 = v4;
LABEL_10:
  swift_unknownObjectRelease(v15);
LABEL_11:
  v18 = type metadata accessor for DepthRatioCalculator();
  swift_deallocPartialClassInstance(v0, v18, 48, 7);
  return nullptr;
}

/* ========================================================================
 * sub_1001A4A08
 * EA: 0x1001a4a08
 ======================================================================== */

id __fastcall sub_1001A4A08(__int64 a1, __int64 a2)
{
  __int64 v2; // x20
  id result; // x0
  void *v5; // x23
  __int64 v6; // x0
  void *v7; // x19
  void *v8; // x25
  id v9; // x0
  void *v10; // x22
  id v11; // x0
  void *v12; // x21
  id v13; // x0
  void *v14; // x24
  id v15; // x0
  __int64 v16; // x1
  void *v17; // x25
  unsigned int *v18; // x27
  id v19; // x0
  __int64 v20; // x1
  unsigned int *v21; // x28
  id v22; // x0
  __int64 v23; // x1
  unsigned int *v24; // x28
  id v25; // x0
  __int64 v26; // x1
  unsigned int *v27; // x28
  id v28; // x0
  id v29; // x0
  void *v30; // x26
  id v31; // x0
  void *v32; // x27
  id v33; // x20
  __int64 v34; // x8
  __int64 v35; // x9
  __int64 v36; // x8
  float v37; // s2
  float v38; // s0
  float v39; // s8
  float v40; // s1
  float v41; // s9
  void *v42; // x28
  __int64 v43; // x0
  __int64 v44; // x19
  Swift::String v45; // x0
  __int64 v46; // x8
  __int64 v47; // x9
  void *v48; // x0
  void *v49; // x0
  unsigned int *v50; // [xsp+8h] [xbp-A8h]
  unsigned int *v51; // [xsp+10h] [xbp-A0h]
  unsigned int *v52; // [xsp+18h] [xbp-98h]
  void *v53; // [xsp+18h] [xbp-98h]
  int64x2_t v54; // [xsp+20h] [xbp-90h] BYREF
  __int64 v55; // [xsp+30h] [xbp-80h]
  __int64 v56; // [xsp+38h] [xbp-78h] BYREF
  __int64 v57; // [xsp+40h] [xbp-70h]
  __int64 v58; // [xsp+48h] [xbp-68h]

  result = (id)((__int64 (*)(void))sub_1001A4F78)();
  if ( !result )
    return result;
  v5 = result;
  v6 = sub_1001A4F78(a2);
  if ( !v6 )
  {
    v48 = v5;
    return (id)swift_unknownObjectRelease(v48);
  }
  v7 = (void *)v6;
  v8 = *(void **)(v2 + 16);
  v9 = objc_msgSend(v8, "newBufferWithLength:options:", 4, 0);
  if ( !v9 )
  {
LABEL_31:
    swift_unknownObjectRelease(v5);
    v48 = v7;
    return (id)swift_unknownObjectRelease(v48);
  }
  v10 = v9;
  v11 = objc_msgSend(v8, "newBufferWithLength:options:", 4, 0);
  if ( !v11 )
  {
    v49 = v10;
LABEL_30:
    swift_unknownObjectRelease(v49);
    goto LABEL_31;
  }
  v12 = v11;
  v13 = objc_msgSend(v8, "newBufferWithLength:options:", 4, 0);
  if ( !v13 )
  {
    swift_unknownObjectRelease(v10);
    v49 = v12;
    goto LABEL_30;
  }
  v14 = v13;
  v15 = objc_msgSend(v8, "newBufferWithLength:options:", 4, 0);
  if ( !v15 )
  {
    swift_unknownObjectRelease(v10);
    swift_unknownObjectRelease(v12);
    v49 = v14;
    goto LABEL_30;
  }
  v17 = v15;
  v18 = (unsigned int *)objc_msgSend((id)swift_unknownObjectRetain(v10, v16), "contents");
  v19 = objc_autorelease(v10);
  *v18 = 0;
  v21 = (unsigned int *)objc_msgSend((id)swift_unknownObjectRetain(v12, v20), "contents");
  v22 = objc_autorelease(v12);
  v52 = v21;
  *v21 = 0;
  v24 = (unsigned int *)objc_msgSend((id)swift_unknownObjectRetain(v14, v23), "contents");
  v25 = objc_autorelease(v14);
  v51 = v24;
  *v24 = 0;
  v27 = (unsigned int *)objc_msgSend((id)swift_unknownObjectRetain(v17, v26), "contents");
  v28 = objc_autorelease(v17);
  *v27 = 0;
  v29 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(v2 + 24), "commandBuffer"));
  if ( !v29 )
  {
LABEL_29:
    swift_unknownObjectRelease(v10);
    swift_unknownObjectRelease(v12);
    swift_unknownObjectRelease(v14);
    v49 = v17;
    goto LABEL_30;
  }
  v30 = v29;
  v50 = v18;
  v31 = objc_retainAutoreleasedReturnValue(objc_msgSend(v29, "computeCommandEncoder"));
  if ( !v31 )
  {
    swift_unknownObjectRelease(v30);
    goto LABEL_29;
  }
  v32 = v31;
  objc_msgSend(v31, "setComputePipelineState:", *(_QWORD *)(v2 + 32));
  objc_msgSend(v32, "setTexture:atIndex:", v5, 0);
  objc_msgSend(v32, "setTexture:atIndex:", v7, 1);
  objc_msgSend(v32, "setBuffer:offset:atIndex:", v10, 0, 0);
  objc_msgSend(v32, "setBuffer:offset:atIndex:", v12, 0, 1);
  objc_msgSend(v32, "setBuffer:offset:atIndex:", v14, 0, 2);
  objc_msgSend(v32, "setBuffer:offset:atIndex:", v17, 0, 3);
  v33 = objc_msgSend(v5, "width");
  result = objc_msgSend(v5, "height");
  v34 = (__int64)v33 + 15;
  if ( __OFADD__(v33, 15) )
  {
    __break(1u);
    goto LABEL_35;
  }
  v35 = (__int64)result + 15;
  if ( __OFADD__(result, 15) )
  {
LABEL_35:
    __break(1u);
    return result;
  }
  if ( v34 < 0 )
    v34 = (__int64)v33 + 30;
  v36 = v34 >> 4;
  if ( v35 < 0 )
    v35 = (__int64)result + 30;
  v56 = v36;
  v57 = v35 >> 4;
  v58 = 1;
  v54 = vdupq_n_s64(0x10u);
  v55 = 1;
  objc_msgSend(v32, "dispatchThreadgroups:threadsPerThreadgroup:", &v56, &v54);
  objc_msgSend(v32, "endEncoding");
  objc_msgSend(v30, "commit");
  objc_msgSend(v30, "waitUntilCompleted");
  v37 = (float)*v50 / 10000.0;
  v38 = (float)*v27 / 10000.0;
  if ( v37 <= 0.0 )
    v39 = 0.0;
  else
    v39 = (float)((float)*v52 / 10000.0) / v37;
  if ( v38 > 0.0 )
  {
    v40 = (float)*v51 / 10000.0;
    if ( v40 > 0.0 && (float)(v40 / v38) > 0.5 )
    {
      v41 = (float)(v38 / v40) * 1.3;
      if ( v41 < v39 )
      {
        v53 = v7;
        v42 = v5;
        v43 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
        v44 = swift_allocObject(v43, 64, 7);
        *(_OWORD *)(v44 + 16) = xmmword_10053B940;
        v56 = 0;
        v57 = 0xE000000000000000LL;
        _StringGuts.grow(_:)(17);
        v45._countAndFlagsBits = 0x7420746968202D2DLL;
        v45._object = (void *)0xEF20706F74206568LL;
        String.append(_:)(v45);
        Float.write<A>(to:)(
          &v56,
          &type metadata for DefaultStringInterpolation,
          &protocol witness table for DefaultStringInterpolation,
          v39 / v41);
        v46 = v56;
        v47 = v57;
        *(_QWORD *)(v44 + 56) = &type metadata for String;
        *(_QWORD *)(v44 + 32) = v46;
        *(_QWORD *)(v44 + 40) = v47;
        print(_:separator:terminator:)(v44, 32, 0xE100000000000000LL, 10, 0xE100000000000000LL);
        swift_bridgeObjectRelease(v44);
        v5 = v10;
        v7 = v12;
        v10 = v14;
        v12 = v17;
        v14 = v30;
        v17 = v32;
        v30 = v42;
        v32 = v53;
      }
    }
  }
  swift_unknownObjectRelease(v5);
  swift_unknownObjectRelease(v7);
  swift_unknownObjectRelease(v10);
  swift_unknownObjectRelease(v12);
  swift_unknownObjectRelease(v14);
  swift_unknownObjectRelease(v17);
  swift_unknownObjectRelease(v30);
  return (id)swift_unknownObjectRelease(v32);
}

/* ========================================================================
 * sub_1001A4F78
 * EA: 0x1001a4f78
 ======================================================================== */

id __fastcall sub_1001A4F78(__CVBuffer *a1)
{
  __int64 v1; // x20
  size_t Width; // x21
  size_t Height; // x0
  void *v5; // x8
  size_t v6; // x22
  id v7; // x19
  __int64 v8; // x0
  __int64 v9; // x19
  id v10; // x20
  CVMetalTextureRef image; // [xsp+10h] [xbp-40h] BYREF

  image = nullptr;
  Width = CVPixelBufferGetWidth(a1);
  Height = CVPixelBufferGetHeight(a1);
  v5 = *(void **)(v1 + 40);
  if ( v5 )
  {
    v6 = Height;
    v7 = objc_retain(v5);
    if ( !CVMetalTextureCacheCreateTextureFromImage(
            kCFAllocatorDefault,
            (CVMetalTextureCacheRef)v7,
            a1,
            nullptr,
            MTLPixelFormatR16Float,
            Width,
            v6,
            0,
            &image)
      && image )
    {
      v10 = objc_retainAutoreleasedReturnValue(CVMetalTextureGetTexture(image));
      objc_release(v7);
      goto LABEL_6;
    }
    objc_release(v7);
  }
  else
  {
    v8 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
    v9 = swift_allocObject(v8, 64, 7);
    *(_OWORD *)(v9 + 16) = xmmword_10053B940;
    *(_QWORD *)(v9 + 56) = &type metadata for String;
    *(_QWORD *)(v9 + 32) = 0xD000000000000014LL;
    *(_QWORD *)(v9 + 40) = 0x80000001004E6620LL;
    print(_:separator:terminator:)(v9, 32, 0xE100000000000000LL, 10, 0xE100000000000000LL);
    swift_bridgeObjectRelease(v9);
  }
  v10 = nullptr;
LABEL_6:
  objc_release(image);
  return v10;
}

/* ========================================================================
 * sub_1001A5108
 * EA: 0x1001a5108
 ======================================================================== */

id __fastcall sub_1001A5108(__int64 a1, __int64 a2, void *a3)
{
  NSString v4; // x20
  id v5; // x19
  id v6; // x20
  id v7; // x0
  NSString v8; // x21
  id v9; // x20
  id v10; // x21
  __int64 v11; // x19
  __int64 v12; // x0
  __int64 v13; // x21
  Swift::String v14; // x0
  __int64 v15; // x0
  id v16; // x8
  unsigned __int64 v17; // x9
  __int64 v19; // [xsp+8h] [xbp-48h] BYREF
  id v20; // [xsp+10h] [xbp-40h] BYREF
  unsigned __int64 v21; // [xsp+18h] [xbp-38h]

  v4 = String._bridgeToObjectiveC()();
  v20 = nullptr;
  v5 = objc_msgSend(a3, "newLibraryWithSource:options:error:", v4, 0, &v20);
  objc_release(v4);
  v6 = v20;
  if ( v5 )
  {
    v7 = objc_retain(v20);
    v8 = String._bridgeToObjectiveC()();
    v9 = objc_msgSend(v5, "newFunctionWithName:", v8);
    swift_unknownObjectRelease(v5);
    objc_release(v8);
  }
  else
  {
    v10 = objc_retain(v20);
    v11 = _convertNSErrorToError(_:)(v6);
    objc_release(v10);
    swift_willThrow();
    v12 = sub_10003E4E0(&unk_100669620, &unk_10053BBC0);
    v13 = swift_allocObject(v12, 64, 7);
    *(_OWORD *)(v13 + 16) = xmmword_10053B940;
    v20 = nullptr;
    v21 = 0xE000000000000000LL;
    _StringGuts.grow(_:)(26);
    v14._countAndFlagsBits = 0xD000000000000018LL;
    v14._object = (void *)0x80000001004E65E0LL;
    String.append(_:)(v14);
    v19 = v11;
    v15 = sub_10003E4E0(&unk_100671D00, &unk_10053BF00);
    _print_unlocked<A, B>(_:_:)(
      &v19,
      &v20,
      v15,
      &type metadata for DefaultStringInterpolation,
      &protocol witness table for DefaultStringInterpolation);
    v16 = v20;
    v17 = v21;
    *(_QWORD *)(v13 + 56) = &type metadata for String;
    *(_QWORD *)(v13 + 32) = v16;
    *(_QWORD *)(v13 + 40) = v17;
    print(_:separator:terminator:)(v13, 32, 0xE100000000000000LL, 10, 0xE100000000000000LL);
    swift_bridgeObjectRelease(v13);
    swift_errorRelease(v11);
    return nullptr;
  }
  return v9;
}

/* ========================================================================
 * sub_1001A531C
 * EA: 0x1001a531c
 ======================================================================== */

void __fastcall sub_1001A531C(__int64 a1, char a2, __int64 a3, void (__fastcall *a4)(__int64))
{
  __int64 v8; // x0
  __int64 v9; // x0
  __int64 v10; // x20
  __int64 v11; // x0
  char v12; // w1
  __int64 v13; // x0
  __int64 v14; // x21
  __int64 v15; // x20
  unsigned __int64 v16; // x1
  unsigned __int64 v17; // x24
  __int64 v18; // x8
  unsigned __int64 v19; // x9
  __int64 v20; // x0
  __int64 v21; // x19
  __int64 v22; // x21
  void *v23; // x24
  __int64 v24; // x25
  id v25; // x20
  id v26; // x26
  __int64 v27; // x20
  __int64 v28; // x0
  char v29; // w1
  void *v30; // x26
  __int64 v31; // x27
  void *v32; // x0
  __int64 v33; // x24
  __int64 v34; // x0
  __int64 inited; // x20
  __int64 v36; // x23
  __int64 v37; // x0
  __int64 v38; // x22
  __int64 v39; // x20
  __int64 v40; // x0
  __int64 v41; // x20
  __int64 v42; // x0
  char v43; // w1
  void *v44; // x20
  __int64 v45; // x0
  __int64 v46; // x20
  __int64 v47; // x0
  char v48; // w1
  void *v49; // x20
  __int64 v50; // x0
  __int64 v51; // x20
  __int64 v52; // x0
  char v53; // w1
  void *v54; // x20
  __int64 v55; // x0
  __int64 v56; // x20
  __int64 v57; // x0
  char v58; // w1
  void *v59; // x20
  __int128 *v60; // x0
  id v61; // x19
  NSString v62; // x20
  char v63; // w22
  char v64; // w22
  char v65; // w22
  id v66; // x20
  NSString v67; // x22
  char v68; // w22
  char v69; // w20
  Swift::String v70; // x0
  __int128 v71; // kr00_16
  __int64 v72; // x0
  __int64 v73; // x0
  __int64 v74; // [xsp+10h] [xbp-190h] BYREF
  void *v75; // [xsp+18h] [xbp-188h]
  char v76[80]; // [xsp+20h] [xbp-180h] BYREF
  __int128 v77; // [xsp+70h] [xbp-130h] BYREF
  __int128 v78; // [xsp+80h] [xbp-120h]
  char v79[80]; // [xsp+90h] [xbp-110h] BYREF
  __int128 v80; // [xsp+E0h] [xbp-C0h] BYREF
  _BYTE v81[25]; // [xsp+F0h] [xbp-B0h]
  char v82; // [xsp+109h] [xbp-97h]
  __int128 v83; // [xsp+110h] [xbp-90h] BYREF
  _OWORD v84[3]; // [xsp+120h] [xbp-80h]

  v8 = sub_10003E4E0((__int64 *)&unk_10066F6A0, &qword_10053D310);
  sub_100052750(a1 + *(int *)(v8 + 52), &v80, &unk_100668718, &unk_10053BCB0);
  if ( (v82 & 1) == 0 )
  {
    v83 = v80;
    v84[0] = *(_OWORD *)v81;
    if ( (a2 & 1) != 0 )
      goto LABEL_37;
    sub_100052750(&v83, &v77, &unk_100669D50, &unk_10053BC30);
    if ( !*((_QWORD *)&v78 + 1) )
      goto LABEL_34;
    v9 = sub_10003E4E0(&qword_100668720, &qword_10053CF20);
    if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, v9, 6) & 1) == 0 )
      goto LABEL_35;
    v10 = v74;
    if ( *(_QWORD *)(v74 + 16) )
    {
      swift_bridgeObjectRetain(v74);
      v11 = sub_100107AD8(0x697372655677656ELL, 0xEA00000000006E6FLL);
      if ( (v12 & 1) != 0 )
      {
        sub_100047B64(*(_QWORD *)(v10 + 56) + 32 * v11, &v77);
        swift_bridgeObjectRelease(v10);
        goto LABEL_18;
      }
      swift_bridgeObjectRelease(v10);
    }
    v77 = 0u;
    v78 = 0u;
LABEL_18:
    swift_bridgeObjectRelease(v10);
    if ( !*((_QWORD *)&v78 + 1) )
      goto LABEL_34;
    if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, &type metadata for String, 6) & 1) != 0 )
    {
      v24 = v74;
      v23 = v75;
      v25 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSBundle), "mainBundle"));
      v26 = objc_retainAutoreleasedReturnValue(objc_msgSend(v25, "infoDictionary"));
      objc_release(v25);
      if ( !v26 )
        goto LABEL_31;
      v27 = static Dictionary._unconditionallyBridgeFromObjectiveC(_:)(
              v26,
              &type metadata for String,
              (char *)&type metadata for Any + 8,
              &protocol witness table for String);
      objc_release(v26);
      if ( *(_QWORD *)(v27 + 16) )
      {
        swift_bridgeObjectRetain(v27);
        v28 = sub_100107AD8(0xD00000000000001ALL, 0x80000001004F0570LL);
        if ( (v29 & 1) != 0 )
        {
          sub_100047B64(*(_QWORD *)(v27 + 56) + 32 * v28, &v77);
          swift_bridgeObjectRelease(v27);
LABEL_26:
          swift_bridgeObjectRelease(v27);
          if ( *((_QWORD *)&v78 + 1) )
          {
            if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, &type metadata for String, 6) & 1) != 0 )
            {
              v31 = v74;
              v30 = v75;
              if ( v74 != v24 || v75 != v23 )
              {
                v69 = _stringCompareWithSmolCheck(_:_:expecting:)(v74, v75, v24, v23, 0);
                swift_bridgeObjectRelease(v23);
                if ( (v69 & 1) == 0 )
                {
                  type metadata accessor for VTLogger(0);
                  *(_QWORD *)&v77 = 0;
                  *((_QWORD *)&v77 + 1) = 0xE000000000000000LL;
                  _StringGuts.grow(_:)(25);
                  swift_bridgeObjectRelease(*((_QWORD *)&v77 + 1));
                  *(_QWORD *)&v77 = 0xD000000000000017LL;
                  *((_QWORD *)&v77 + 1) = 0x80000001004F0590LL;
                  v70._countAndFlagsBits = v31;
                  v70._object = v30;
                  String.append(_:)(v70);
                  v71 = v77;
                  v72 = static os_log_type_t.info.getter();
                  sub_1001D8B44(v72, v71, *((_QWORD *)&v71 + 1));
                  swift_bridgeObjectRelease(*((_QWORD *)&v71 + 1));
                  v74 = v31;
                  v75 = v30;
                  swift_beginAccess(
                    a3 + OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager__lastSafeVersion,
                    &v77,
                    33,
                    0);
                  v73 = sub_10003E4E0((__int64 *)&unk_1006729C0, (__int64 *)&unk_10053CC70);
                  WrappedDefault.wrappedValue.setter(&v74, v73);
                  swift_endAccess(&v77);
                  v33 = 0;
                  goto LABEL_36;
                }
                v32 = v30;
                goto LABEL_32;
              }
              swift_bridgeObjectRelease(v75);
            }
LABEL_31:
            v32 = v23;
LABEL_32:
            swift_bridgeObjectRelease(v32);
            goto LABEL_35;
          }
          swift_bridgeObjectRelease(v23);
LABEL_34:
          sub_10005285C(&v77, &unk_100669D50, &unk_10053BC30);
          goto LABEL_35;
        }
        swift_bridgeObjectRelease(v27);
      }
      v77 = 0u;
      v78 = 0u;
      goto LABEL_26;
    }
LABEL_35:
    v33 = 1;
LABEL_36:
    a4(v33);
    *(_BYTE *)(a3 + OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager_curStartMode) = v33;
    v34 = sub_10003E4E0(&qword_10066D920, &qword_10053C530);
    inited = swift_initStackObject(v34, v76);
    *(_OWORD *)(inited + 16) = xmmword_10053B940;
    *(_QWORD *)(inited + 32) = 0x746C75736572LL;
    *(_QWORD *)(inited + 72) = &type metadata for String;
    *(_QWORD *)(inited + 40) = 0xE600000000000000LL;
    *(_QWORD *)(inited + 48) = 0x73736563637573LL;
    *(_QWORD *)(inited + 56) = 0xE700000000000000LL;
    v36 = sub_100166EF8(inited);
    swift_setDeallocating(inited);
    sub_10005285C(inited + 32, &unk_100668B70, &unk_10053BAC0);
    sub_1000B22CC(0x6153734964616F4CLL, 0xEA00000000006566LL, v36);
    swift_bridgeObjectRelease(v36);
LABEL_37:
    sub_100052750(&v83, &v77, &unk_100669D50, &unk_10053BC30);
    if ( *((_QWORD *)&v78 + 1) )
    {
      v37 = sub_10003E4E0(&qword_100668418, &qword_10053BBD8);
      if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, v37, 6) & 1) != 0 )
      {
        v38 = v74;
        if ( qword_100662550 != -1 )
          swift_once(&qword_100662550, sub_100205F80);
        v39 = *(_QWORD *)(qword_100697110 + OBJC_IVAR____TtC15ExternalMonitor8VTDevice_otaConfig);
        swift_retain(v39);
        sub_10016B874(v38);
        swift_bridgeObjectRelease(v38);
        swift_release(v39);
      }
    }
    else
    {
      sub_10005285C(&v77, &unk_100669D50, &unk_10053BC30);
    }
    sub_100052750(&v83, &v77, &unk_100669D50, &unk_10053BC30);
    if ( !*((_QWORD *)&v78 + 1) )
      goto LABEL_55;
    v40 = sub_10003E4E0(&qword_100668720, &qword_10053CF20);
    if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, v40, 6) & 1) == 0 )
      goto LABEL_56;
    v41 = v74;
    if ( *(_QWORD *)(v74 + 16) )
    {
      swift_bridgeObjectRetain(v74);
      v42 = sub_100107AD8(0x6C69626F4D657375LL, 0xEC00000042545965LL);
      if ( (v43 & 1) != 0 )
      {
        sub_100047B64(*(_QWORD *)(v41 + 56) + 32 * v42, &v77);
        swift_bridgeObjectRelease(v41);
        goto LABEL_50;
      }
      swift_bridgeObjectRelease(v41);
    }
    v77 = 0u;
    v78 = 0u;
LABEL_50:
    swift_bridgeObjectRelease(v41);
    if ( *((_QWORD *)&v78 + 1) )
    {
      if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, &type metadata for String, 6) & 1) == 0 )
        goto LABEL_56;
      v44 = v75;
      if ( v74 == 49 && v75 == (void *)0xE100000000000000LL )
      {
        swift_bridgeObjectRelease(0xE100000000000000LL);
      }
      else
      {
        v63 = _stringCompareWithSmolCheck(_:_:expecting:)(v74, v75, 49, 0xE100000000000000LL, 0);
        swift_bridgeObjectRelease(v44);
        if ( (v63 & 1) == 0 )
          goto LABEL_56;
      }
      *(_BYTE *)(a3 + OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager_isUseMobileYoutubeSite) = 1;
LABEL_56:
      sub_100052750(&v83, &v77, &unk_100669D50, &unk_10053BC30);
      if ( !*((_QWORD *)&v78 + 1) )
        goto LABEL_68;
      v45 = sub_10003E4E0(&qword_100668720, &qword_10053CF20);
      if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, v45, 6) & 1) == 0 )
        goto LABEL_69;
      v46 = v74;
      if ( *(_QWORD *)(v74 + 16) )
      {
        swift_bridgeObjectRetain(v74);
        v47 = sub_100107AD8(0x576E656469626F66LL, 0xEF7265766F486265LL);
        if ( (v48 & 1) != 0 )
        {
          sub_100047B64(*(_QWORD *)(v46 + 56) + 32 * v47, &v77);
          swift_bridgeObjectRelease(v46);
          goto LABEL_63;
        }
        swift_bridgeObjectRelease(v46);
      }
      v77 = 0u;
      v78 = 0u;
LABEL_63:
      swift_bridgeObjectRelease(v46);
      if ( *((_QWORD *)&v78 + 1) )
      {
        if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, &type metadata for String, 6) & 1) == 0 )
          goto LABEL_69;
        v49 = v75;
        if ( v74 == 49 && v75 == (void *)0xE100000000000000LL )
        {
          swift_bridgeObjectRelease(0xE100000000000000LL);
        }
        else
        {
          v64 = _stringCompareWithSmolCheck(_:_:expecting:)(v74, v75, 49, 0xE100000000000000LL, 0);
          swift_bridgeObjectRelease(v49);
          if ( (v64 & 1) == 0 )
            goto LABEL_69;
        }
        *(_BYTE *)(a3 + OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager_isWebHoverFobiden) = 1;
LABEL_69:
        sub_100052750(&v83, &v77, &unk_100669D50, &unk_10053BC30);
        if ( !*((_QWORD *)&v78 + 1) )
          goto LABEL_81;
        v50 = sub_10003E4E0(&qword_100668720, &qword_10053CF20);
        if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, v50, 6) & 1) == 0 )
          goto LABEL_82;
        v51 = v74;
        if ( *(_QWORD *)(v74 + 16) )
        {
          swift_bridgeObjectRetain(v74);
          v52 = sub_100107AD8(0x33656C6261736964LL, 0xED00007370704144LL);
          if ( (v53 & 1) != 0 )
          {
            sub_100047B64(*(_QWORD *)(v51 + 56) + 32 * v52, &v77);
            swift_bridgeObjectRelease(v51);
            goto LABEL_76;
          }
          swift_bridgeObjectRelease(v51);
        }
        v77 = 0u;
        v78 = 0u;
LABEL_76:
        swift_bridgeObjectRelease(v51);
        if ( *((_QWORD *)&v78 + 1) )
        {
          if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, &type metadata for String, 6) & 1) == 0 )
            goto LABEL_82;
          v54 = v75;
          if ( v74 == 49 && v75 == (void *)0xE100000000000000LL )
          {
            swift_bridgeObjectRelease(0xE100000000000000LL);
          }
          else
          {
            v65 = _stringCompareWithSmolCheck(_:_:expecting:)(v74, v75, 49, 0xE100000000000000LL, 0);
            swift_bridgeObjectRelease(v54);
            if ( (v65 & 1) == 0 )
              goto LABEL_82;
          }
          v66 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter), "defaultCenter"));
          v67 = String._bridgeToObjectiveC()();
          objc_msgSend(v66, "postNotificationName:object:userInfo:", v67, 0, 0);
          objc_release(v66);
          objc_release(v67);
LABEL_82:
          sub_100052750(&v83, &v77, &unk_100669D50, &unk_10053BC30);
          if ( !*((_QWORD *)&v78 + 1) )
            goto LABEL_94;
          v55 = sub_10003E4E0(&qword_100668720, &qword_10053CF20);
          if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, v55, 6) & 1) == 0 )
          {
LABEL_95:
            v60 = &v83;
            goto LABEL_96;
          }
          v56 = v74;
          if ( *(_QWORD *)(v74 + 16) )
          {
            swift_bridgeObjectRetain(v74);
            v57 = sub_100107AD8(0x3032796576727573LL, 0xEC00000033303632LL);
            if ( (v58 & 1) != 0 )
            {
              sub_100047B64(*(_QWORD *)(v56 + 56) + 32 * v57, &v77);
              swift_bridgeObjectRelease(v56);
              goto LABEL_89;
            }
            swift_bridgeObjectRelease(v56);
          }
          v77 = 0u;
          v78 = 0u;
LABEL_89:
          swift_bridgeObjectRelease(v56);
          if ( *((_QWORD *)&v78 + 1) )
          {
            if ( (swift_dynamicCast(&v74, &v77, (char *)&type metadata for Any + 8, &type metadata for String, 6) & 1) != 0 )
            {
              v59 = v75;
              if ( v74 == 49 && v75 == (void *)0xE100000000000000LL )
              {
                swift_bridgeObjectRelease(0xE100000000000000LL);
                sub_10005285C(&v83, &unk_100669D50, &unk_10053BC30);
              }
              else
              {
                v68 = _stringCompareWithSmolCheck(_:_:expecting:)(v74, v75, 49, 0xE100000000000000LL, 0);
                swift_bridgeObjectRelease(v59);
                sub_10005285C(&v83, &unk_100669D50, &unk_10053BC30);
                if ( (v68 & 1) == 0 )
                  goto LABEL_97;
              }
              *(_BYTE *)(a3 + OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager_isShowSurvey) = 1;
LABEL_97:
              if ( (a2 & 1) != 0 )
                return;
              goto LABEL_98;
            }
            goto LABEL_95;
          }
LABEL_94:
          sub_10005285C(&v83, &unk_100669D50, &unk_10053BC30);
          v60 = &v77;
LABEL_96:
          sub_10005285C(v60, &unk_100669D50, &unk_10053BC30);
          goto LABEL_97;
        }
LABEL_81:
        sub_10005285C(&v77, &unk_100669D50, &unk_10053BC30);
        goto LABEL_82;
      }
LABEL_68:
      sub_10005285C(&v77, &unk_100669D50, &unk_10053BC30);
      goto LABEL_69;
    }
LABEL_55:
    sub_10005285C(&v77, &unk_100669D50, &unk_10053BC30);
    goto LABEL_56;
  }
  v83 = v80;
  v84[0] = *(_OWORD *)v81;
  *(_OWORD *)((char *)v84 + 9) = *(_OWORD *)&v81[9];
  if ( (a2 & 1) != 0 )
  {
    sub_1000BD534(&v83);
    return;
  }
  v13 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
  v14 = swift_allocObject(v13, 64, 7);
  *(_OWORD *)(v14 + 16) = xmmword_10053B940;
  v15 = sub_1000505D0();
  v17 = v16;
  sub_1000BD534(&v83);
  *(_QWORD *)(v14 + 56) = &type metadata for String;
  if ( v17 )
    v18 = v15;
  else
    v18 = 0;
  v19 = 0xE000000000000000LL;
  if ( v17 )
    v19 = v17;
  *(_QWORD *)(v14 + 32) = v18;
  *(_QWORD *)(v14 + 40) = v19;
  print(_:separator:terminator:)(v14, 32, 0xE100000000000000LL, 10, 0xE100000000000000LL);
  swift_bridgeObjectRelease(v14);
  a4(1);
  *(_BYTE *)(a3 + OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager_curStartMode) = 1;
  v20 = sub_10003E4E0(&qword_10066D920, &qword_10053C530);
  v21 = swift_initStackObject(v20, v79);
  *(_OWORD *)(v21 + 16) = xmmword_10053B940;
  *(_QWORD *)(v21 + 32) = 0x746C75736572LL;
  *(_QWORD *)(v21 + 72) = &type metadata for String;
  *(_QWORD *)(v21 + 40) = 0xE600000000000000LL;
  *(_QWORD *)(v21 + 48) = 0x6572756C696166LL;
  *(_QWORD *)(v21 + 56) = 0xE700000000000000LL;
  v22 = sub_100166EF8(v21);
  swift_setDeallocating(v21);
  sub_10005285C(v21 + 32, &unk_100668B70, &unk_10053BAC0);
  sub_1000B22CC(0x6153734964616F4CLL, 0xEA00000000006566LL, v22);
  swift_bridgeObjectRelease(v22);
LABEL_98:
  v61 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter), "defaultCenter"));
  v62 = String._bridgeToObjectiveC()();
  objc_msgSend(v61, "postNotificationName:object:userInfo:", v62, 0, 0);
  objc_release(v61);
  objc_release(v62);
}

/* ========================================================================
 * sub_1001A60DC
 * EA: 0x1001a60dc
 ======================================================================== */

id sub_1001A60DC()
{
  _BYTE *v0; // x20
  char *v1; // x19
  id v2; // x0
  _QWORD *v3; // x0
  __int128 v4; // q1
  objc_super v6; // [xsp+8h] [xbp-68h] BYREF
  _QWORD v7[2]; // [xsp+18h] [xbp-58h] BYREF
  _OWORD v8[2]; // [xsp+28h] [xbp-48h] BYREF
  __int64 v9; // [xsp+48h] [xbp-28h]

  v0[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager_curStartMode] = 2;
  v0[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager_isUseMobileYoutubeSite] = 0;
  v0[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager_isWebHoverFobiden] = 0;
  v0[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager_isShowSurvey] = 0;
  v1 = &v0[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager__lastSafeVersion];
  v7[0] = 0x302E302E31LL;
  v7[1] = 0xE500000000000000LL;
  v2 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSUserDefaults), "standardUserDefaults"));
  v3 = WrappedDefault.init(wrappedValue:key:userDefaults:)(
         v8,
         v7,
         0xD000000000000014LL,
         0x80000001004F05B0LL,
         v2,
         &type metadata for String,
         &protocol witness table for String);
  v4 = v8[1];
  *(_OWORD *)v1 = v8[0];
  *((_OWORD *)v1 + 1) = v4;
  *((_QWORD *)v1 + 4) = v9;
  v6.receiver = v0;
  v6.super_class = (Class)type metadata accessor for VTAppStartManager(v3);
  return objc_msgSendSuper2(&v6, "init");
}

/* ========================================================================
 * -[_TtC15ExternalMonitor17VTAppStartManager .cxx_destruct]
 * EA: 0x1001a622c
 ======================================================================== */

void __cdecl -[VTAppStartManager .cxx_destruct](_TtC15ExternalMonitor17VTAppStartManager *self, SEL a2)
{
  __int64 v2; // x19
  void *v3; // x20
  id v4; // [xsp+8h] [xbp-18h]

  v2 = *(_QWORD *)&self->curStartMode[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager__lastSafeVersion];
  v4 = *(Class *)((char *)&self->super.isa + OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager__lastSafeVersion);
  v3 = *(void **)&self->_lastSafeVersion[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager__lastSafeVersion + 4];
  swift_bridgeObjectRelease(*(_QWORD *)&self->_lastSafeVersion[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager__lastSafeVersion
                                                             + 20]);
  objc_release(v3);
  swift_release(v2);
  objc_release(v4);
}

/* ========================================================================
 * sub_1001A62B0
 * EA: 0x1001a62b0
 ======================================================================== */

unsigned __int64 sub_1001A62B0()
{
  unsigned __int64 result; // x0

  result = qword_10066FB90;
  if ( !qword_10066FB90 )
  {
    result = swift_getWitnessTable(&unk_100542A20, &type metadata for AppStartMode);
    atomic_store(result, (unsigned __int64 *)&qword_10066FB90);
  }
  return result;
}

/* ========================================================================
 * sub_1001A62F0
 * EA: 0x1001a62f0
 ======================================================================== */

__int64 sub_1001A62F0()
{
  id v0; // x20
  id v1; // x19
  __int64 v2; // x20
  __int64 v3; // x0
  char v4; // w1
  __int64 v6; // [xsp+0h] [xbp-50h] BYREF
  __int128 v7; // [xsp+10h] [xbp-40h] BYREF
  __int128 v8; // [xsp+20h] [xbp-30h]

  v0 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSBundle), "mainBundle"));
  v1 = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "infoDictionary"));
  objc_release(v0);
  if ( !v1 )
    return 0;
  v2 = static Dictionary._unconditionallyBridgeFromObjectiveC(_:)(
         v1,
         &type metadata for String,
         (char *)&type metadata for Any + 8,
         &protocol witness table for String);
  objc_release(v1);
  if ( !*(_QWORD *)(v2 + 16) )
    goto LABEL_8;
  swift_bridgeObjectRetain(v2);
  v3 = sub_100107AD8(0xD00000000000001ALL, 0x80000001004F0570LL);
  if ( (v4 & 1) == 0 )
  {
    swift_bridgeObjectRelease(v2);
LABEL_8:
    v7 = 0u;
    v8 = 0u;
    swift_bridgeObjectRelease(v2);
    goto LABEL_9;
  }
  sub_100047B64(*(_QWORD *)(v2 + 56) + 32 * v3, &v7);
  swift_bridgeObjectRelease_n(v2, 2);
  if ( !*((_QWORD *)&v8 + 1) )
  {
LABEL_9:
    sub_10005285C(&v7, &unk_100669D50, &unk_10053BC30);
    return 0;
  }
  if ( (unsigned int)swift_dynamicCast(&v6, &v7, (char *)&type metadata for Any + 8, &type metadata for String, 6) )
    return v6;
  return 0;
}

/* ========================================================================
 * sub_1001A643C
 * EA: 0x1001a643c
 ======================================================================== */

// local variable allocation has failed, the output may be wrong!
__int64 __fastcall sub_1001A643C(_BYTE *a1, __int64 a2, void *a3)
{
  __int64 v6; // x19
  char *v7; // x23
  __int128 v8; // q1
  __int128 v9; // kr00_16
  void *v10; // x26
  id v11; // x23
  id v12; // x25
  id v13; // x26
  __int64 v14; // x0
  __int64 v15; // x24
  __int64 v16; // x25
  __int64 v17; // x1
  __int64 v18; // x2
  __int64 v19; // x20
  char v20; // w25
  char v21; // w24
  __int64 v22; // x0
  __int64 Strong; // x0
  void *v24; // x20
  id v25; // x20
  NSString v26; // x22
  int v27; // w8
  char *v28; // x22
  unsigned __int64 v29; // x0 OVERLAPPED
  __int64 v30; // x0
  char v31; // w20
  unsigned __int64 v32; // x1
  Swift::String v33; // x0
  __int64 v34; // x20
  unsigned __int64 v35; // x22
  __int64 v36; // x23
  _BYTE *v37; // x0
  __int64 v38; // x20
  _QWORD v40[3]; // [xsp+0h] [xbp-D0h] BYREF
  __int64 v41; // [xsp+18h] [xbp-B8h]
  unsigned __int64 v42; // [xsp+20h] [xbp-B0h]
  char v43[24]; // [xsp+28h] [xbp-A8h] BYREF
  __int128 v44; // [xsp+40h] [xbp-90h]
  __int128 v45; // [xsp+50h] [xbp-80h]
  __int64 v46; // [xsp+60h] [xbp-70h]
  __int128 v47; // [xsp+70h] [xbp-60h] BYREF

  v6 = swift_allocObject(&unk_1005C3ED0, 32, 7);
  *(_QWORD *)(v6 + 16) = a2;
  *(_QWORD *)(v6 + 24) = a3;
  v7 = &a1[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager__lastSafeVersion];
  swift_beginAccess(&a1[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager__lastSafeVersion], v43, 0, 0);
  v8 = *((_OWORD *)v7 + 1);
  v44 = *(_OWORD *)v7;
  v45 = v8;
  v46 = *((_QWORD *)v7 + 4);
  v9 = v44;
  v10 = (void *)v8;
  v47 = *(_OWORD *)(v7 + 24);
  swift_retain(a2);
  v11 = objc_retain(a3);
  v12 = objc_retain((id)v9);
  swift_retain(*((_QWORD *)&v9 + 1));
  v13 = objc_retain(v10);
  sub_10004542C((__int64)&v47, (__int64)v40);
  v14 = sub_10003E4E0((__int64 *)&unk_1006729C0, (__int64 *)&unk_10053CC70);
  WrappedDefault.wrappedValue.getter(v40, v14);
  objc_release(v13);
  swift_release(*((_QWORD *)&v9 + 1));
  objc_release(v12);
  sub_10003F604(&v47);
  v16 = v40[0];
  v15 = v40[1];
  v18 = sub_1001A62F0();
  v19 = v17;
  if ( v16 == v18 && v15 == v17 )
  {
    swift_bridgeObjectRelease(v15);
    swift_bridgeObjectRelease(v19);
  }
  else
  {
    v20 = _stringCompareWithSmolCheck(_:_:expecting:)(v16, v15, v18, v17, 0);
    swift_bridgeObjectRelease(v15);
    swift_bridgeObjectRelease(v19);
    v21 = 0;
    if ( (v20 & 1) == 0 )
      goto LABEL_8;
  }
  type metadata accessor for VTLogger(0);
  v22 = static os_log_type_t.info.getter();
  sub_1001D8B44(v22, 0xD000000000000036LL, 0x80000001004F04E0LL);
  a1[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager_curStartMode] = 0;
  swift_beginAccess(a2 + 16, v40, 0, 0);
  Strong = swift_unknownObjectWeakLoadStrong(a2 + 16);
  if ( Strong )
  {
    v24 = (void *)Strong;
    sub_100076264(0, v11);
    objc_release(v24);
  }
  v25 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter), "defaultCenter"));
  v26 = String._bridgeToObjectiveC()();
  objc_msgSend(v25, "postNotificationName:object:userInfo:", v26, 0, 0);
  objc_release(v25);
  objc_release(v26);
  v21 = 1;
LABEL_8:
  v41 = 0;
  v42 = 0xE000000000000000LL;
  _StringGuts.grow(_:)(41);
  swift_bridgeObjectRelease(v42);
  v41 = 0x2F2F3A7370747468LL;
  v42 = 0xE800000000000000LL;
  if ( qword_1006625D8 != -1 )
    swift_once(&qword_1006625D8, sub_1002489D0);
  v27 = (unsigned __int8)sub_10024AB50();
  if ( v27 )
  {
    if ( v27 == 1 )
    {
      swift_bridgeObjectRelease(0xE700000000000000LL);
      v28 = "VRSample180HSBS.mp4";
      v29 = 0xD000000000000012LL;
      goto LABEL_21;
    }
    v30 = 24938;
  }
  else
  {
    v30 = 28261;
  }
  v31 = _stringCompareWithSmolCheck(_:_:expecting:)(
          v30,
          0xE200000000000000LL,
          0x736E61482D687ALL,
          0xE700000000000000LL,
          0);
  swift_bridgeObjectRelease(0xE200000000000000LL);
  if ( (v31 & 1) != 0 )
    v29 = 0xD000000000000012LL;
  else
    v29 = 0xD000000000000011LL;
  if ( (v31 & 1) != 0 )
    v28 = "VRSample180HSBS.mp4";
  else
    v28 = "play180SampleButtonTapped(_:)";
LABEL_21:
  v32 = (unsigned __int64)v28 | 0x8000000000000000LL;
  String.append(_:)(*(Swift::String *)&v29);
  swift_bridgeObjectRelease((unsigned __int64)v28 | 0x8000000000000000LL);
  v33._countAndFlagsBits = 0xD00000000000001FLL;
  v33._object = (void *)0x80000001004EE760LL;
  String.append(_:)(v33);
  v34 = v41;
  v35 = v42;
  v36 = swift_allocObject(&unk_1005C3EF8, 48, 7);
  *(_BYTE *)(v36 + 16) = v21;
  *(_QWORD *)(v36 + 24) = a1;
  *(_QWORD *)(v36 + 32) = sub_1001A6C48;
  *(_QWORD *)(v36 + 40) = v6;
  v37 = objc_retain(a1);
  swift_retain(v6);
  v38 = sub_1000524F8(v34, v35, 4, sub_1001A6C80, v36);
  swift_release(v6);
  swift_release(v38);
  swift_bridgeObjectRelease(v35);
  return swift_release(v36);
}

/* ========================================================================
 * sub_1001A6828
 * EA: 0x1001a6828
 ======================================================================== */

// local variable allocation has failed, the output may be wrong!
__int64 __fastcall sub_1001A6828(_BYTE *a1, void *a2)
{
  __int64 v4; // x19
  char *v5; // x22
  __int128 v6; // q1
  __int128 v7; // kr00_16
  void *v8; // x25
  id v9; // x22
  id v10; // x24
  id v11; // x25
  __int64 v12; // x0
  unsigned __int64 v13; // x23
  __int64 v14; // x24
  __int64 v15; // x1
  __int64 v16; // x2
  __int64 v17; // x20
  char v18; // w24
  char v19; // w25
  __int64 v20; // x0
  id v21; // x20
  NSString v22; // x22
  int v23; // w8
  char *v24; // x22
  unsigned __int64 v25; // x0 OVERLAPPED
  __int64 v26; // x0
  char v27; // w20
  unsigned __int64 v28; // x1
  Swift::String v29; // x0
  __int64 v30; // x20
  unsigned __int64 v31; // x22
  __int64 v32; // x23
  _BYTE *v33; // x0
  __int64 v34; // x20
  __int64 v36; // [xsp+8h] [xbp-A8h] BYREF
  unsigned __int64 v37; // [xsp+10h] [xbp-A0h]
  char v38[24]; // [xsp+18h] [xbp-98h] BYREF
  __int128 v39; // [xsp+30h] [xbp-80h]
  __int128 v40; // [xsp+40h] [xbp-70h]
  __int64 v41; // [xsp+50h] [xbp-60h]
  __int128 v42; // [xsp+60h] [xbp-50h] BYREF

  v4 = swift_allocObject(&unk_1005C3E80, 24, 7);
  *(_QWORD *)(v4 + 16) = a2;
  v5 = &a1[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager__lastSafeVersion];
  swift_beginAccess(&a1[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager__lastSafeVersion], v38, 0, 0);
  v6 = *((_OWORD *)v5 + 1);
  v39 = *(_OWORD *)v5;
  v40 = v6;
  v41 = *((_QWORD *)v5 + 4);
  v7 = v39;
  v8 = (void *)v6;
  v42 = *(_OWORD *)(v5 + 24);
  v9 = objc_retain(a2);
  v10 = objc_retain((id)v7);
  swift_retain(*((_QWORD *)&v7 + 1));
  v11 = objc_retain(v8);
  sub_10004542C((__int64)&v42, (__int64)&v36);
  v12 = sub_10003E4E0((__int64 *)&unk_1006729C0, (__int64 *)&unk_10053CC70);
  WrappedDefault.wrappedValue.getter(&v36, v12);
  objc_release(v11);
  swift_release(*((_QWORD *)&v7 + 1));
  objc_release(v10);
  sub_10003F604(&v42);
  v14 = v36;
  v13 = v37;
  v16 = sub_1001A62F0();
  v17 = v15;
  if ( v14 == v16 && v13 == v15 )
  {
    swift_bridgeObjectRelease(v13);
    swift_bridgeObjectRelease(v17);
  }
  else
  {
    v18 = _stringCompareWithSmolCheck(_:_:expecting:)(v14, v13, v16, v15, 0);
    swift_bridgeObjectRelease(v13);
    swift_bridgeObjectRelease(v17);
    v19 = 0;
    if ( (v18 & 1) == 0 )
      goto LABEL_6;
  }
  type metadata accessor for VTLogger(0);
  v20 = static os_log_type_t.info.getter();
  sub_1001D8B44(v20, 0xD000000000000036LL, 0x80000001004F04E0LL);
  a1[OBJC_IVAR____TtC15ExternalMonitor17VTAppStartManager_curStartMode] = 0;
  sub_1001F6678(0, v9);
  v21 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter), "defaultCenter"));
  v22 = String._bridgeToObjectiveC()();
  objc_msgSend(v21, "postNotificationName:object:userInfo:", v22, 0, 0);
  objc_release(v21);
  objc_release(v22);
  v19 = 1;
LABEL_6:
  v36 = 0;
  v37 = 0xE000000000000000LL;
  _StringGuts.grow(_:)(41);
  swift_bridgeObjectRelease(v37);
  v36 = 0x2F2F3A7370747468LL;
  v37 = 0xE800000000000000LL;
  if ( qword_1006625D8 != -1 )
    swift_once(&qword_1006625D8, sub_1002489D0);
  v23 = (unsigned __int8)sub_10024AB50();
  if ( v23 )
  {
    if ( v23 == 1 )
    {
      swift_bridgeObjectRelease(0xE700000000000000LL);
      v24 = "VRSample180HSBS.mp4";
      v25 = 0xD000000000000012LL;
      goto LABEL_19;
    }
    v26 = 24938;
  }
  else
  {
    v26 = 28261;
  }
  v27 = _stringCompareWithSmolCheck(_:_:expecting:)(
          v26,
          0xE200000000000000LL,
          0x736E61482D687ALL,
          0xE700000000000000LL,
          0);
  swift_bridgeObjectRelease(0xE200000000000000LL);
  if ( (v27 & 1) != 0 )
    v25 = 0xD000000000000012LL;
  else
    v25 = 0xD000000000000011LL;
  if ( (v27 & 1) != 0 )
    v24 = "VRSample180HSBS.mp4";
  else
    v24 = "play180SampleButtonTapped(_:)";
LABEL_19:
  v28 = (unsigned __int64)v24 | 0x8000000000000000LL;
  String.append(_:)(*(Swift::String *)&v25);
  swift_bridgeObjectRelease((unsigned __int64)v24 | 0x8000000000000000LL);
  v29._countAndFlagsBits = 0xD00000000000001FLL;
  v29._object = (void *)0x80000001004EE760LL;
  String.append(_:)(v29);
  v30 = v36;
  v31 = v37;
  v32 = swift_allocObject(&unk_1005C3EA8, 48, 7);
  *(_BYTE *)(v32 + 16) = v19;
  *(_QWORD *)(v32 + 24) = a1;
  *(_QWORD *)(v32 + 32) = sub_1001A6BFC;
  *(_QWORD *)(v32 + 40) = v4;
  v33 = objc_retain(a1);
  swift_retain(v4);
  v34 = sub_1000524F8(v30, v31, 4, sub_1001A6C0C, v32);
  swift_release(v4);
  swift_release(v34);
  swift_bridgeObjectRelease(v31);
  return swift_release(v32);
}

/* ========================================================================
 * sub_1001A6C88
 * EA: 0x1001a6c88
 ======================================================================== */

__int64 sub_1001A6C88()
{
  void *v0; // x20
  double v1; // d0
  double v2; // d1
  double v3; // d2
  double v4; // d3
  id v5; // x21
  __int64 v6; // x19
  __int64 v7; // x22
  void *v8; // x23
  __int64 v9; // x24
  id v10; // x0
  id v11; // x20
  __int64 result; // x0
  _QWORD v13[5]; // [xsp+0h] [xbp-80h] BYREF
  __int64 v14; // [xsp+28h] [xbp-58h]

  objc_msgSend(v0, "bounds");
  v5 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UIGraphicsImageRenderer), "initWithBounds:", v1, v2, v3, v4);
  v6 = swift_allocObject(&unk_1005C3F58, 24, 7);
  *(_QWORD *)(v6 + 16) = v0;
  v7 = swift_allocObject(&unk_1005C3F80, 32, 7);
  *(_QWORD *)(v7 + 16) = sub_1001A6EBC;
  *(_QWORD *)(v7 + 24) = v6;
  v13[4] = sub_1001A6ED4;
  v14 = v7;
  v13[0] = _NSConcreteStackBlock;
  v13[1] = 1107296256;
  v13[2] = sub_10021ADB0;
  v13[3] = &unk_1005C3F98;
  v8 = _Block_copy(v13);
  v9 = v14;
  v10 = objc_retain(v0);
  swift_retain(v7);
  swift_release(v9);
  v11 = objc_retainAutoreleasedReturnValue(objc_msgSend(v5, "imageWithActions:", v8));
  objc_release(v5);
  _Block_release(v8);
  LOBYTE(v5) = swift_isEscapingClosureAtFileLocation(v7, "", 90, 13, 31, 1);
  swift_release(v6);
  result = swift_release(v7);
  if ( ((unsigned __int8)v5 & 1) == 0 )
    return (__int64)v11;
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_1001A6E1C
 * EA: 0x1001a6e1c
 ======================================================================== */

void __fastcall sub_1001A6E1C(void *a1, id a2)
{
  id v3; // x20
  id v4; // [xsp+8h] [xbp-18h]

  v3 = objc_retainAutoreleasedReturnValue(objc_msgSend(a2, "layer"));
  v4 = objc_retainAutoreleasedReturnValue(objc_msgSend(a1, "CGContext"));
  objc_msgSend(v3, "renderInContext:", v4);
  objc_release(v3);
  objc_release(v4);
}

/* ========================================================================
 * sub_1001A6EF4
 * EA: 0x1001a6ef4
 ======================================================================== */

void sub_1001A6EF4()
{
  char *v0; // x20
  id v1; // x0
  void *v2; // x19
  void *v3; // x21
  id v4; // x19
  __int64 v5; // x23
  __int64 v6; // x21
  __int64 v7; // x19
  __int64 v8; // x0
  void *v9; // x8
  __int64 v10; // x22
  __int64 v11; // x0
  id v12; // x0
  void *v13; // x19
  __int64 v14; // x21
  double height; // d9
  objc_super v16; // [xsp+0h] [xbp-50h] BYREF

  v16.receiver = v0;
  v16.super_class = (Class)type metadata accessor for SBSPhotoViewController(0);
  objc_msgSendSuper2(&v16, "viewDidLoad");
  v1 = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "view"));
  if ( !v1 )
    goto LABEL_27;
  v2 = v1;
  v3 = *(void **)&v0[OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_photoView];
  objc_msgSend(v1, "addSubview:", v3);
  objc_release(v2);
  v4 = objc_retain(v3);
  ConstraintViewDSL.makeConstraints(_:)(sub_1001A70E4, 0, v4);
  objc_release(v4);
  v5 = OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_index;
  v6 = *(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_index];
  v7 = *(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_assets];
  if ( !((unsigned __int64)v7 >> 62) )
  {
    v8 = *(_QWORD *)((v7 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
    if ( v6 < v8 )
    {
      if ( (v7 & 0xC000000000000001LL) == 0 )
        goto LABEL_5;
LABEL_13:
      v11 = v6;
LABEL_25:
      v12 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(v11, v7);
      goto LABEL_20;
    }
    goto LABEL_15;
  }
  if ( v7 < 0 )
    v10 = *(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_assets];
  else
    v10 = v7 & 0xFFFFFFFFFFFFFF8LL;
  if ( v6 >= _CocoaArrayWrapper.endIndex.getter(v10) )
  {
    v8 = _CocoaArrayWrapper.endIndex.getter(v10);
LABEL_15:
    if ( !v8 )
      return;
    if ( (v7 & 0xC000000000000001LL) != 0 )
      goto LABEL_24;
    if ( *(_QWORD *)((v7 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
    {
      v9 = *(void **)(v7 + 32);
      goto LABEL_19;
    }
    __break(1u);
LABEL_27:
    __break(1u);
    return;
  }
  v6 = *(_QWORD *)&v0[v5];
  if ( (v7 & 0xC000000000000001LL) != 0 )
    goto LABEL_13;
LABEL_5:
  if ( v6 < 0 )
  {
    __break(1u);
    goto LABEL_23;
  }
  if ( (unsigned __int64)v6 >= *(_QWORD *)((v7 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
  {
LABEL_23:
    __break(1u);
LABEL_24:
    v11 = 0;
    goto LABEL_25;
  }
  v9 = *(void **)(v7 + 8 * v6 + 32);
LABEL_19:
  v12 = objc_retain(v9);
LABEL_20:
  v13 = v12;
  v14 = swift_allocObject(&unk_1005C3FD0, 24, 7);
  swift_unknownObjectWeakInit(v14 + 16, v0);
  height = PHImageManagerMaximumSize.height;
  swift_retain(v14);
  sub_1001CF35C(0, sub_1001A7D50, v14, PHImageManagerMaximumSize.width, height);
  swift_release_n(v14, 2);
  objc_release(v13);
}

/* ========================================================================
 * sub_1001A70E4
 * EA: 0x1001a70e4
 ======================================================================== */

__int64 __fastcall sub_1001A70E4(__int64 a1)
{
  __int64 v1; // x20
  __int64 v2; // x0

  v1 = (*(__int64 (**)(void))(*(_QWORD *)a1 + 264LL))();
  v2 = (*(__int64 (__fastcall **)(unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v1 + 112LL))(
         0xD000000000000070LL,
         0x80000001004F0690LL,
         23);
  swift_release(v2);
  return swift_release(v1);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor22SBSPhotoViewController initWithCoder:]
 * EA: 0x1001a7174
 ======================================================================== */

_TtC15ExternalMonitor22SBSPhotoViewController *__cdecl __noreturn -[SBSPhotoViewController initWithCoder:](
        _TtC15ExternalMonitor22SBSPhotoViewController *self,
        SEL a2,
        id a3)
{
  __int64 v4; // x21
  id v5; // x0
  _TtC15ExternalMonitor22SBSPhotoViewController *result; // x0

  v4 = OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_photoView;
  v5 = objc_allocWithZone((Class)type metadata accessor for SBSPhotoView(0, a2, a3));
  *(Class *)((char *)&self->super.super.super.super.super.isa + v4) = (Class)sub_1000F5EE8(0);
  *(Class *)((char *)&self->super.super.super.super.super.isa
           + OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_index) = nullptr;
  result = (_TtC15ExternalMonitor22SBSPhotoViewController *)_assertionFailure(_:_:file:line:flags:)(
                                                              "Fatal error",
                                                              11,
                                                              2,
                                                              0xD000000000000025LL,
                                                              0x80000001004D9660LL,
                                                              "ExternalMonitor/SBSPhotoViewController.swift",
                                                              44,
                                                              2,
                                                              31,
                                                              0);
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_1001A7208
 * EA: 0x1001a7208
 ======================================================================== */

void sub_1001A7208()
{
  char *v0; // x20
  void *v1; // x26
  char *v2; // x23
  __int64 v3; // x0
  char *v4; // x25
  __int64 v5; // x28
  __int64 v6; // x24
  char *v7; // x27
  __int64 v8; // x0
  __int64 v9; // x0
  __int64 v10; // x19
  void *v11; // x0
  void *v12; // x8
  objc_class *v13; // x20
  char *v14; // x22
  id v15; // x22
  char *v16; // x26
  id v17; // x22
  id v18; // x20
  __int64 v19; // x0
  __int64 v20; // x0
  void *v21; // x0
  id v22; // x0
  void *v23; // x22
  void *v24; // x0
  id v25; // x0
  __int64 v26; // x1
  void *v27; // x21
  __int64 v28; // x1
  __int64 v29; // x19
  Swift::String v30; // x0
  Swift::String v31; // x0
  void *v32; // x22
  char *v33; // x26
  void (__fastcall *v34)(char *, __int64); // x19
  void *v35; // x27
  __int64 v36; // x0
  __int64 v37; // x24
  __int64 v38; // x20
  __int64 v39; // x0
  __int64 v40; // x21
  char *v41; // x0
  id v42; // x19
  double v43; // d0
  CGFloat v44; // d8
  double v45; // d1
  CGFloat v46; // d9
  double v47; // d2
  CGFloat v48; // d10
  double v49; // d3
  CGFloat v50; // d11
  __int64 v51; // x0
  char *v52; // [xsp+0h] [xbp-F0h] BYREF
  __int64 v53; // [xsp+8h] [xbp-E8h]
  __int64 v54; // [xsp+10h] [xbp-E0h]
  __int64 v55; // [xsp+18h] [xbp-D8h]
  __int64 v56; // [xsp+20h] [xbp-D0h]
  __int64 v57; // [xsp+28h] [xbp-C8h]
  _QWORD aBlock[6]; // [xsp+30h] [xbp-C0h] BYREF
  objc_super v59; // [xsp+60h] [xbp-90h] BYREF
  char v60[9]; // [xsp+77h] [xbp-79h] BYREF
  CGRect v61; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8

  v1 = v0;
  v53 = type metadata accessor for DispatchWorkItemFlags(0);
  v56 = *(_QWORD *)(v53 - 8);
  v2 = (char *)&v52 - ((*(_QWORD *)(v56 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v3 = type metadata accessor for DispatchQoS(0);
  v54 = *(_QWORD *)(v3 - 8);
  v55 = v3;
  v4 = (char *)&v52 - ((*(_QWORD *)(v54 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for DispatchTime(0);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = (char *)&v52 - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  v60[0] = 0;
  swift_beginAccess(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__pinMode, aBlock, 33, 0);
  v8 = sub_10003E4E0(&qword_10066A970, (__int64 *)&unk_10053BAD0);
  WrappedDefault.wrappedValue.setter(v60, v8);
  v9 = swift_endAccess(aBlock);
  sub_1000F4664(v9);
  v10 = OBJC_IVAR____TtC15ExternalMonitor20VTBaseViewController_fsToolBar;
  v11 = *(void **)&v0[OBJC_IVAR____TtC15ExternalMonitor20VTBaseViewController_fsToolBar];
  if ( v11 )
  {
    objc_msgSend(v11, "removeFromSuperview");
    v12 = *(void **)&v0[v10];
  }
  else
  {
    v12 = nullptr;
  }
  *(_QWORD *)&v0[v10] = 0;
  objc_release(v12);
  v13 = (objc_class *)type metadata accessor for PhotosControlViewController(0);
  v14 = (char *)objc_allocWithZone(v13);
  swift_unknownObjectWeakInit(&v14[OBJC_IVAR____TtC15ExternalMonitor27PhotosControlViewController_photoVC], 0);
  swift_unknownObjectWeakInit(&v14[OBJC_IVAR____TtC15ExternalMonitor27PhotosControlViewController_ai3DButton], 0);
  swift_unknownObjectWeakInit(&v14[OBJC_IVAR____TtC15ExternalMonitor27PhotosControlViewController_resetButton], 0);
  swift_unknownObjectWeakInit(&v14[OBJC_IVAR____TtC15ExternalMonitor27PhotosControlViewController_fovButton], 0);
  v59.receiver = v14;
  v59.super_class = v13;
  v15 = objc_retain(v1);
  v16 = (char *)objc_msgSendSuper2(&v59, "initWithNibName:bundle:", 0, 0);
  swift_unknownObjectWeakAssign(&v16[OBJC_IVAR____TtC15ExternalMonitor27PhotosControlViewController_photoVC], v15);
  objc_msgSend(v16, "setModalPresentationStyle:", 5);
  objc_msgSend(v16, "setModalTransitionStyle:", 2);
  objc_msgSend(v16, "setModalInPresentation:", 1);
  objc_release(v15);
  v17 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIApplication), "sharedApplication"));
  v18 = objc_retainAutoreleasedReturnValue(objc_msgSend(v17, "delegate"));
  objc_release(v17);
  if ( v18 )
  {
    v19 = type metadata accessor for AppDelegate(0);
    v20 = swift_dynamicCastClass(v18, v19);
    if ( v20 )
    {
      v21 = *(void **)(v20 + OBJC_IVAR____TtC15ExternalMonitor11AppDelegate_window);
      if ( v21 )
      {
        v22 = objc_retainAutoreleasedReturnValue(objc_msgSend(v21, "rootViewController"));
        if ( v22 )
        {
          v23 = v22;
          objc_msgSend(v22, "presentViewController:animated:completion:", v16, 1, 0);
          objc_release(v23);
        }
      }
    }
    swift_unknownObjectRelease(v18);
  }
  if ( qword_100662550 != -1 )
    swift_once(&qword_100662550, sub_100205F80);
  if ( qword_1006625A0 != -1 )
    swift_once(&qword_1006625A0, sub_100240480);
  v57 = qword_100697238;
  v24 = *(void **)(qword_100697238 + OBJC_IVAR____TtC15ExternalMonitor12VTBLEManager_connectedPeripheral);
  if ( v24 && (v25 = objc_retainAutoreleasedReturnValue(objc_msgSend(v24, "name"))) != nullptr )
  {
    v27 = v25;
    static String._unconditionallyBridgeFromObjectiveC(_:)(v25, v26);
    v29 = v28;
    objc_release(v27);
    v30._countAndFlagsBits = 3486032;
    v30._object = (void *)0xE300000000000000LL;
    if ( !String.hasPrefix(_:)(v30) )
    {
      v31._countAndFlagsBits = 3158352;
      v31._object = (void *)0xE300000000000000LL;
      String.hasPrefix(_:)(v31);
    }
    swift_bridgeObjectRelease(v29);
  }
  else
  {
    sub_100040668(0);
    v32 = (void *)static OS_dispatch_queue.main.getter();
    static DispatchTime.now()();
    v52 = v16;
    v33 = v7;
    + infix(_:_:)(v7, 0.5);
    v34 = *(void (__fastcall **)(char *, __int64))(v6 + 8);
    v34(v7, v5);
    aBlock[4] = sub_100067A58;
    aBlock[5] = 0;
    aBlock[0] = _NSConcreteStackBlock;
    aBlock[1] = 1107296256;
    aBlock[2] = sub_100040600;
    aBlock[3] = &unk_1005C3FE8;
    v35 = _Block_copy(aBlock);
    v36 = static DispatchQoS.unspecified.getter(v35);
    aBlock[0] = &_swiftEmptyArrayStorage;
    v37 = sub_1000406E0(v36);
    v38 = sub_10003E4E0((__int64 *)&unk_100668B40, &qword_10053B8F0);
    v39 = sub_100040724();
    v40 = v53;
    dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v38, v39, v53, v37);
    OS_dispatch_queue.asyncAfter(deadline:qos:flags:execute:)(v33, v4, v2, v35);
    _Block_release(v35);
    objc_release(v32);
    (*(void (__fastcall **)(char *, __int64))(v56 + 8))(v2, v40);
    (*(void (__fastcall **)(char *, __int64))(v54 + 8))(v4, v55);
    v41 = v33;
    v16 = v52;
    v34(v41, v5);
  }
  if ( qword_100662540 != -1 )
    swift_once(&qword_100662540, sub_1001F0F34);
  v42 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(qword_100697100
                                                              + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_window), "screen"));
  objc_msgSend(v42, "bounds");
  v44 = v43;
  v46 = v45;
  v48 = v47;
  v50 = v49;
  objc_release(v42);
  v61.origin.x = v44;
  v61.origin.y = v46;
  v61.size.width = v48;
  v61.size.height = v50;
  if ( CGRectGetHeight(v61) < 1200.0 )
    v51 = 1;
  else
    v51 = 5;
  sub_1000755E8(v51);
  objc_release(v16);
}

/* ========================================================================
 * sub_1001A7858
 * EA: 0x1001a7858
 ======================================================================== */

void __fastcall sub_1001A7858(__int64 a1, __int64 a2)
{
  __int64 Strong; // x0
  void *v5; // x21
  id v6; // x20
  _BYTE v7[24]; // [xsp+8h] [xbp-38h] BYREF

  swift_beginAccess(a2 + 16, v7, 0, 0);
  Strong = swift_unknownObjectWeakLoadStrong(a2 + 16);
  if ( Strong )
  {
    v5 = (void *)Strong;
    v6 = objc_retain(*(id *)(Strong + OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_photoView));
    sub_1000F60A8(a1);
    objc_release(v5);
    objc_release(v6);
  }
}

/* ========================================================================
 * sub_1001A78D0
 * EA: 0x1001a78d0
 ======================================================================== */

void sub_1001A78D0()
{
  __int64 v0; // x20
  __int64 v1; // x19
  __int64 v2; // x0
  __int64 v3; // x21
  __int64 v4; // x8
  __int64 v5; // x22
  __int64 v6; // x0
  id v8; // x0
  __int64 v9; // x19
  double height; // d9
  __int64 v11; // x0
  id v12; // [xsp+8h] [xbp-48h]

  v1 = *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_assets);
  if ( (unsigned __int64)v1 >> 62 )
  {
    if ( v1 < 0 )
      v11 = *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_assets);
    else
      v11 = v1 & 0xFFFFFFFFFFFFFF8LL;
    v2 = _CocoaArrayWrapper.endIndex.getter(v11);
    if ( v2 >= 1 )
    {
LABEL_3:
      v3 = OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_index;
      v4 = *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_index);
      v5 = v4 + 1;
      if ( __OFADD__(v4, 1) )
      {
        __break(1u);
      }
      else
      {
        if ( (unsigned __int64)v1 >> 62 )
        {
          if ( v1 < 0 )
            v6 = v1;
          else
            v6 = v1 & 0xFFFFFFFFFFFFFF8LL;
          v2 = _CocoaArrayWrapper.endIndex.getter(v6);
        }
        else
        {
          v2 = *(_QWORD *)((v1 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
        }
        if ( v2 )
        {
          if ( v5 == 0x8000000000000000LL && v2 == -1 )
          {
LABEL_30:
            __break(1u);
            return;
          }
          v2 = v5 % v2;
          *(_QWORD *)(v0 + v3) = v2;
          if ( (v1 & 0xC000000000000001LL) == 0 )
          {
            if ( v2 < 0 )
            {
              __break(1u);
            }
            else if ( (unsigned __int64)v2 < *(_QWORD *)((v1 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
            {
              v8 = objc_retain(*(id *)(v1 + 8 * v2 + 32));
LABEL_19:
              v12 = v8;
              v9 = swift_allocObject(&unk_1005C3FD0, 24, 7);
              swift_unknownObjectWeakInit(v9 + 16, v0);
              height = PHImageManagerMaximumSize.height;
              swift_retain(v9);
              sub_1001CF35C(0, sub_1001A7D50, v9, PHImageManagerMaximumSize.width, height);
              swift_release_n(v9, 2);
              objc_release(v12);
              return;
            }
            __break(1u);
            goto LABEL_30;
          }
LABEL_27:
          v8 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(v2, v1);
          goto LABEL_19;
        }
      }
      __break(1u);
      goto LABEL_27;
    }
  }
  else
  {
    v2 = *(_QWORD *)((v1 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
    if ( v2 >= 1 )
      goto LABEL_3;
  }
}

/* ========================================================================
 * sub_1001A7A60
 * EA: 0x1001a7a60
 ======================================================================== */

void sub_1001A7A60()
{
  __int64 v0; // x20
  __int64 v1; // x19
  __int64 v2; // x0
  __int64 v3; // x21
  __int64 v4; // x8
  bool v5; // vf
  __int64 v6; // x8
  __int64 v7; // x23
  __int64 v8; // x0
  id v10; // x0
  __int64 v11; // x19
  double height; // d9
  __int64 v13; // x21
  id v14; // [xsp+8h] [xbp-48h]

  v1 = *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_assets);
  if ( (unsigned __int64)v1 >> 62 )
  {
    if ( v1 < 0 )
      v13 = *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_assets);
    else
      v13 = v1 & 0xFFFFFFFFFFFFFF8LL;
    if ( _CocoaArrayWrapper.endIndex.getter(v13) >= 1 )
    {
      v2 = _CocoaArrayWrapper.endIndex.getter(v13);
LABEL_3:
      v3 = OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_index;
      v4 = *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor22SBSPhotoViewController_index);
      v5 = __OFADD__(v2, v4);
      v6 = v2 + v4;
      if ( v5 )
      {
        __break(1u);
      }
      else
      {
        v7 = v6 - 1;
        if ( !__OFSUB__(v6, 1) )
        {
          if ( (unsigned __int64)v1 >> 62 )
          {
            if ( v1 < 0 )
              v8 = v1;
            else
              v8 = v1 & 0xFFFFFFFFFFFFFF8LL;
            v2 = _CocoaArrayWrapper.endIndex.getter(v8);
          }
          else
          {
            v2 = *(_QWORD *)((v1 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
          }
          if ( v2 )
          {
            if ( v7 == 0x8000000000000000LL && v2 == -1 )
            {
LABEL_33:
              __break(1u);
              return;
            }
            v2 = v7 % v2;
            *(_QWORD *)(v0 + v3) = v2;
            if ( (v1 & 0xC000000000000001LL) == 0 )
            {
              if ( v2 < 0 )
              {
                __break(1u);
              }
              else if ( (unsigned __int64)v2 < *(_QWORD *)((v1 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
              {
                v10 = objc_retain(*(id *)(v1 + 8 * v2 + 32));
LABEL_20:
                v14 = v10;
                v11 = swift_allocObject(&unk_1005C3FD0, 24, 7);
                swift_unknownObjectWeakInit(v11 + 16, v0);
                height = PHImageManagerMaximumSize.height;
                swift_retain(v11);
                sub_1001CF35C(0, sub_1001A7D30, v11, PHImageManagerMaximumSize.width, height);
                swift_release_n(v11, 2);
                objc_release(v14);
                return;
              }
              __break(1u);
              goto LABEL_33;
            }
LABEL_30:
            v10 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(v2, v1);
            goto LABEL_20;
          }
LABEL_29:
          __break(1u);
          goto LABEL_30;
        }
      }
      __break(1u);
      goto LABEL_29;
    }
  }
  else
  {
    v2 = *(_QWORD *)((v1 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
    if ( v2 )
      goto LABEL_3;
  }
}

/* ========================================================================
 * +[_TtC15ExternalMonitor16VTEncryptManager shared]
 * EA: 0x1001a7d80
 ======================================================================== */

_TtC15ExternalMonitor16VTEncryptManager *__cdecl +[VTEncryptManager shared](id a1, SEL a2)
{
  if ( qword_1006624A8 != -1 )
    swift_once(&qword_1006624A8, sub_1001A7D54);
  return (_TtC15ExternalMonitor16VTEncryptManager *)objc_retainAutoreleaseReturnValue((id)qword_100697030);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor16VTEncryptManager init]
 * EA: 0x1001a7dc0
 ======================================================================== */

_TtC15ExternalMonitor16VTEncryptManager *__cdecl -[VTEncryptManager init](
        _TtC15ExternalMonitor16VTEncryptManager *self,
        SEL a2)
{
  _QWORD *v2; // x8
  _QWORD *v3; // x8
  objc_super v5; // [xsp+0h] [xbp-20h] BYREF

  v2 = (Class *)((char *)&self->super.isa + OBJC_IVAR____TtC15ExternalMonitor16VTEncryptManager_iv);
  *v2 = 0xD000000000000010LL;
  v2[1] = 0x80000001004F0850LL;
  v3 = (Class *)((char *)&self->super.isa + OBJC_IVAR____TtC15ExternalMonitor16VTEncryptManager_key);
  *v3 = 0xD000000000000020LL;
  v3[1] = 0x80000001004F0870LL;
  v5.receiver = self;
  v5.super_class = (Class)type metadata accessor for VTEncryptManager();
  return -[VTEncryptManager init](&v5, "init");
}

/* ========================================================================
 * sub_1001A7E48
 * EA: 0x1001a7e48
 ======================================================================== */

__int64 __fastcall sub_1001A7E48(__int64 a1, void *a2)
{
  __int64 v2; // x20
  char *v5; // x28
  __int64 v6; // x0
  __int64 v7; // x1
  __int64 v8; // x19
  __int64 v9; // x21
  __int64 v10; // x8
  __int64 v11; // x20
  __int64 v12; // x24
  unsigned __int64 v13; // x1
  __int64 v14; // x2
  __int64 v15; // x3
  __int64 v16; // x24
  unsigned __int64 v17; // x25
  __int64 v18; // x26
  __int64 v19; // x27
  __int64 v20; // x0
  unsigned __int64 v21; // x1
  __int64 v22; // x19
  unsigned __int64 v23; // x21
  __int64 v24; // x0
  __int64 v25; // x0
  __int64 v26; // x20
  __int64 v27; // x1
  __int64 v28; // x28
  Swift::String v29; // x0
  unsigned __int64 v30; // x20
  unsigned __int64 v31; // x21
  __int64 v32; // x0
  __int64 v33; // x0
  __int64 v35; // [xsp+0h] [xbp-70h] BYREF
  __int64 v36; // [xsp+8h] [xbp-68h]
  unsigned __int64 v37; // [xsp+10h] [xbp-60h] BYREF
  unsigned __int64 v38; // [xsp+18h] [xbp-58h]

  v5 = (char *)&v35
     - ((*(_QWORD *)(*(_QWORD *)(type metadata accessor for String.Encoding(0) - 8) + 64LL) + 15LL)
      & 0xFFFFFFFFFFFFFFF0LL);
  v6 = Data.init(base64Encoded:options:)(a1, a2, 0);
  v35 = v7;
  v36 = v6;
  v8 = *(_QWORD *)(v2 + OBJC_IVAR____TtC15ExternalMonitor16VTEncryptManager_key);
  v9 = *(_QWORD *)(v2 + OBJC_IVAR____TtC15ExternalMonitor16VTEncryptManager_key + 8);
  v10 = v2 + OBJC_IVAR____TtC15ExternalMonitor16VTEncryptManager_iv;
  v11 = *(_QWORD *)(v2 + OBJC_IVAR____TtC15ExternalMonitor16VTEncryptManager_iv);
  v12 = *(_QWORD *)(v10 + 8);
  swift_bridgeObjectRetain(v9);
  swift_bridgeObjectRetain(v12);
  v16 = sub_1001AAAEC(v8, v9, v11, v12);
  v17 = v13;
  v18 = v14;
  v19 = v15;
  if ( v13 >> 60 == 15 )
    goto LABEL_4;
  v20 = sub_1001A83E8(v36, v35, 1, v16, v13, v14, v15);
  if ( v21 >> 60 == 15 )
    goto LABEL_4;
  v22 = v20;
  v23 = v21;
  v37 = v20;
  v38 = v21;
  v24 = static String.Encoding.utf8.getter();
  v25 = sub_1001AB7D4(v24);
  v26 = String.init<A>(bytes:encoding:)(&v37, v5, &type metadata for Data, v25);
  v28 = v27;
  sub_1000509CC(v22, v23);
  if ( !v28 )
  {
LABEL_4:
    type metadata accessor for VTLogger(0);
    v37 = 0;
    v38 = 0xE000000000000000LL;
    _StringGuts.grow(_:)(18);
    swift_bridgeObjectRelease(v38);
    v37 = 0xD000000000000010LL;
    v38 = 0x80000001004F0830LL;
    v29._countAndFlagsBits = a1;
    v29._object = a2;
    String.append(_:)(v29);
    v30 = v37;
    v31 = v38;
    v33 = static os_log_type_t.error.getter(v32);
    sub_1001D8B44(v33, v30, v31);
    swift_bridgeObjectRelease(v31);
    v26 = 0;
  }
  sub_1001AAD94(v16, v17, v18, v19);
  sub_1000509CC(v36, v35);
  return v26;
}

/* ========================================================================
 * sub_1001C8128
 * EA: 0x1001c8128
 ======================================================================== */

void sub_1001C8128()
{
  _BYTE *v0; // x20
  __int64 v1; // x21
  double v2; // d0
  double v3; // d8
  double v4; // d1
  double v5; // d9
  double v6; // d0
  CGFloat v7; // d8
  double v8; // d1
  CGFloat v9; // d9
  void *v10; // x19
  __int64 v11; // x0
  void *v12; // x21
  __int64 v13; // x22
  _BYTE *v14; // x20
  double v15; // d0
  CGFloat v16; // d10
  double v17; // d1
  CGFloat v18; // d11
  CGFloat v19; // d12
  void *v20; // x19
  __int64 v21; // x0
  void *v22; // x21
  __int64 v23; // x22
  _BYTE *v24; // x0
  __int64 v25; // x19
  __int64 v26; // x19
  id v27; // [xsp+8h] [xbp-88h]
  void **aBlock; // [xsp+10h] [xbp-80h] BYREF
  __int64 v29; // [xsp+18h] [xbp-78h]
  __int64 (__fastcall *v30)(); // [xsp+20h] [xbp-70h]
  void *v31; // [xsp+28h] [xbp-68h]
  __int64 (__fastcall *v32)(); // [xsp+30h] [xbp-60h]
  __int64 v33; // [xsp+38h] [xbp-58h]
  CGPoint v34; // 0:d4.8,8:d5.8
  CGRect v35; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8
  CGRect v36; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8
  CGRect v37; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8

  if ( qword_100662540 != -1 )
    swift_once(&qword_100662540, sub_1001F0F34);
  v1 = qword_100697100;
  objc_msgSend(*(id *)(qword_100697100 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_cursor), "center");
  v3 = v2;
  v5 = v4;
  v27 = objc_retain(*(id *)(v1 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_window));
  objc_msgSend(v27, "convertPoint:toCoordinateSpace:", v0, v3, v5);
  v7 = v6;
  v9 = v8;
  if ( (unsigned int)objc_msgSend(
                       *(id *)(v1 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_cursorWindow),
                       "isHidden") )
  {
    if ( v0[OBJC_IVAR____TtC15ExternalMonitor9VTToolBar_isShown] == 1 )
    {
      v0[OBJC_IVAR____TtC15ExternalMonitor9VTToolBar_isShown] = 0;
      v10 = (void *)objc_opt_self(&OBJC_CLASS___UIView);
      v11 = swift_allocObject(&unk_1005C5320, 24, 7);
      *(_QWORD *)(v11 + 16) = v0;
      v32 = sub_1001C8924;
      v33 = v11;
      aBlock = _NSConcreteStackBlock;
      v29 = 1107296256;
      v30 = sub_100040600;
      v31 = &unk_1005C5338;
      v12 = _Block_copy(&aBlock);
      v13 = v33;
      v14 = objc_retain(v0);
      swift_release(v13);
      objc_msgSend(v10, "animateWithDuration:animations:", v12, 0.45);
      objc_release(v27);
      _Block_release(v12);
      v14[OBJC_IVAR____TtC15ExternalMonitor9VTToolBar_willHideIn2Secs] = 0;
      return;
    }
LABEL_14:
    objc_release(v27);
    return;
  }
  objc_msgSend(v0, "bounds");
  v16 = v15 + -500.0;
  objc_msgSend(v0, "bounds");
  v18 = v17 + -10.0;
  objc_msgSend(v0, "bounds");
  v19 = CGRectGetWidth(v35) + 1000.0;
  objc_msgSend(v0, "bounds");
  v37.size.height = CGRectGetHeight(v36);
  v37.origin.x = v16;
  v37.origin.y = v18;
  v37.size.width = v19;
  v34.x = v7;
  v34.y = v9;
  if ( !CGRectContainsPoint(v37, v34) )
  {
    v26 = OBJC_IVAR____TtC15ExternalMonitor9VTToolBar_willHideIn2Secs;
    if ( (v0[OBJC_IVAR____TtC15ExternalMonitor9VTToolBar_willHideIn2Secs] & 1) == 0
      && ((unsigned int)objc_msgSend(v0, "isOpaque") & 1) != 0 )
    {
      objc_msgSend(v0, "performSelector:withObject:afterDelay:", "hide", 0, 2.0);
      objc_release(v27);
      v0[v26] = 1;
      return;
    }
    goto LABEL_14;
  }
  if ( (v0[OBJC_IVAR____TtC15ExternalMonitor9VTToolBar_isShown] & 1) == 0 )
  {
    v0[OBJC_IVAR____TtC15ExternalMonitor9VTToolBar_isShown] = 1;
    v20 = (void *)objc_opt_self(&OBJC_CLASS___UIView);
    v21 = swift_allocObject(&unk_1005C52D0, 24, 7);
    *(_QWORD *)(v21 + 16) = v0;
    v32 = sub_1001C87E0;
    v33 = v21;
    aBlock = _NSConcreteStackBlock;
    v29 = 1107296256;
    v30 = sub_100040600;
    v31 = &unk_1005C52E8;
    v22 = _Block_copy(&aBlock);
    v23 = v33;
    v24 = objc_retain(v0);
    swift_release(v23);
    objc_msgSend(v20, "animateWithDuration:animations:", v22, 0.45);
    _Block_release(v22);
  }
  v25 = OBJC_IVAR____TtC15ExternalMonitor9VTToolBar_willHideIn2Secs;
  if ( v0[OBJC_IVAR____TtC15ExternalMonitor9VTToolBar_willHideIn2Secs] == 1 )
  {
    objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSObject), "cancelPreviousPerformRequestsWithTarget:", v0);
    objc_release(v27);
    v0[v25] = 0;
  }
  else
  {
    objc_release(v27);
  }
}

/* ========================================================================
 * sub_1001C8B50
 * EA: 0x1001c8b50
 ======================================================================== */

void sub_1001C8B50()
{
  char *v0; // x20
  char *v1; // x19
  __int64 v2; // x0
  void *Strong; // x0
  void *v4; // x22
  id v5; // x21
  id v6; // x21
  __int64 v7; // x0
  char *v8; // x23
  __int64 v9; // x20
  __int64 v10; // x21
  _QWORD *v11; // x0
  __int64 v12; // x0
  __int64 v13; // [xsp+0h] [xbp-30h] BYREF

  v1 = (char *)&v13
     - ((*(_QWORD *)(*(_QWORD *)(sub_10003E4E0((__int64 *)&unk_100668B60, &qword_10053BAE0) - 8) + 64LL) + 15LL)
      & 0xFFFFFFFFFFFFFFF0LL);
  v2 = sub_1001C9108();
  sub_1001C8E20(v2);
  Strong = (void *)swift_unknownObjectWeakLoadStrong(&v0[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_playerVC]);
  if ( !Strong )
    goto LABEL_4;
  v4 = Strong;
  v5 = objc_retainAutoreleasedReturnValue(objc_msgSend(Strong, "view"));
  objc_release(v4);
  if ( v5 )
  {
    objc_msgSend(
      v5,
      "addSubview:",
      *(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_photoView]);
    objc_release(v5);
LABEL_4:
    v6 = objc_retain(*(id *)&v0[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_photoView]);
    ConstraintViewDSL.makeConstraints(_:)(sub_1001C8CE8, 0, v6);
    objc_release(v6);
    v7 = type metadata accessor for TaskPriority(0);
    (*(void (__fastcall **)(char *, __int64, __int64, __int64))(*(_QWORD *)(v7 - 8) + 56LL))(v1, 1, 1, v7);
    type metadata accessor for MainActor(0);
    v8 = objc_retain(v0);
    v9 = static MainActor.shared.getter();
    v10 = sub_10003EF70(
            &qword_100671CF0,
            &type metadata accessor for MainActor,
            &protocol conformance descriptor for MainActor);
    v11 = (_QWORD *)swift_allocObject(&unk_1005C54D0, 40, 7);
    v11[2] = v9;
    v11[3] = v10;
    v11[4] = v8;
    v12 = sub_100087664(0, 0, v1, &unk_1005433E8, v11);
    swift_release(v12);
    return;
  }
  __break(1u);
}

/* ========================================================================
 * sub_1001C8DA8
 * EA: 0x1001c8da8
 ======================================================================== */

void sub_1001C8DA8()
{
  __int64 v0; // x20
  __int64 v1; // x19
  __int64 v2; // x19
  void *v3; // x0

  v1 = *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_task);
  if ( v1 )
  {
    swift_retain(*(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_task));
    Task.cancel()();
    swift_release(v1);
  }
  v2 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_fpsTimer;
  objc_msgSend(*(id *)(v0 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_fpsTimer), "invalidate");
  v3 = *(void **)(v0 + v2);
  *(_QWORD *)(v0 + v2) = 0;
  objc_release(v3);
}

/* ========================================================================
 * sub_1001C9108
 * EA: 0x1001c9108
 ======================================================================== */

void sub_1001C9108()
{
  char **v0; // x20
  __int64 v1; // x23
  __int64 v2; // x24
  id v3; // x19
  char **v4; // x22
  id v5; // x21
  char **v6; // x25
  float v7; // s0
  float v8; // s8
  float v9; // s0
  int v10; // w8
  __int64 v12; // x0
  __int64 inited; // x21
  float v14; // s0
  float v15; // s8
  float v16; // s0
  __int64 v17; // x19
  __int64 v18; // x1
  __int64 v19; // x20
  void *v20; // x1
  __int64 v21; // x20
  __int64 v22; // x0
  int v23; // w20
  id v24; // x21
  bool v25; // zf
  _BYTE v26[72]; // [xsp+8h] [xbp-98h] BYREF

  v1 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastRemainBattery;
  v2 = *(__int64 *)((char *)v0 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastRemainBattery);
  v3 = (id)objc_opt_self(&OBJC_CLASS___UIDevice);
  v4 = &selRef_createAppDelegateProxy;
  v5 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "currentDevice"));
  v6 = &selRef_setLineHeightMultiple_;
  objc_msgSend(v5, "batteryLevel");
  v8 = v7;
  objc_release(v5);
  v9 = v8 * 100.0;
  v10 = fabs(v8 * 100.0);
  if ( v2 < 1 )
  {
    if ( v10 <= 2139095039 )
    {
      if ( v9 > -9.2234e18 )
      {
        if ( v9 < 9.2234e18 )
        {
          v2 = (__int64)v9 / 10;
          goto LABEL_22;
        }
        goto LABEL_28;
      }
LABEL_27:
      __break(1u);
LABEL_28:
      __break(1u);
      goto LABEL_29;
    }
LABEL_26:
    __break(1u);
    goto LABEL_27;
  }
  if ( v10 > 2139095039 )
  {
    __break(1u);
    goto LABEL_24;
  }
  if ( v9 <= -9.2234e18 )
  {
LABEL_24:
    __break(1u);
    goto LABEL_25;
  }
  if ( v9 >= 9.2234e18 )
  {
LABEL_25:
    __break(1u);
    goto LABEL_26;
  }
  v2 = (__int64)v9 / 10;
  if ( (__int64)v9 <= 59 && v2 < *(__int64 *)((char *)v0 + v1) )
  {
    v12 = sub_10003E4E0((__int64 *)&unk_10066AF90, (__int64 *)&unk_10053C880);
    inited = swift_initStackObject(v12, v26);
    *(_OWORD *)(inited + 16) = xmmword_10053B940;
    v3 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "currentDevice"));
    objc_msgSend(v3, "batteryLevel");
    v15 = v14;
    objc_release(v3);
    v16 = v15 * 100.0;
    if ( COERCE_INT(fabs(v15 * 100.0)) <= 2139095039 )
    {
      if ( v16 > -9.2234e18 )
      {
        if ( v16 < 9.2234e18 )
        {
          v6 = v0;
          v17 = dispatch thunk of CustomStringConvertible.description.getter(
                  &type metadata for Int,
                  &protocol witness table for Int);
          v19 = v18;
          *(_QWORD *)(inited + 56) = &type metadata for String;
          *(_QWORD *)(inited + 64) = sub_1000458F0(v17);
          *(_QWORD *)(inited + 32) = v17;
          *(_QWORD *)(inited + 40) = v19;
          v4 = (char **)sub_10024814C(0xD000000000000012LL, 0x80000001004D9DE0LL, inited);
          v3 = v20;
          swift_setDeallocating(inited);
          v21 = *(_QWORD *)(inited + 16);
          v22 = sub_10003E4E0(&qword_1006682D0, &qword_10053BB00);
          swift_arrayDestroy(inited + 32, v21, v22);
          if ( qword_100662540 == -1 )
          {
LABEL_14:
            v23 = *(unsigned __int8 *)(qword_100697100 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_isUltraWide);
            v24 = objc_retain(*(id *)(qword_100697100 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_window));
            v25 = v23 == 1;
            v0 = v6;
            if ( v25 )
            {
              sub_1000BA7C4(v4, v3, v24, 4.0);
            }
            else
            {
              sub_1000BAC98(v4, v3, v24, 1, 4.0);
              sub_1000BAC98(v4, v3, v24, 0, 4.0);
            }
            objc_release(v24);
            swift_bridgeObjectRelease(v3);
            goto LABEL_22;
          }
LABEL_32:
          swift_once(&qword_100662540, sub_1001F0F34);
          goto LABEL_14;
        }
LABEL_31:
        __break(1u);
        goto LABEL_32;
      }
LABEL_30:
      __break(1u);
      goto LABEL_31;
    }
LABEL_29:
    __break(1u);
    goto LABEL_30;
  }
LABEL_22:
  *(char **)((char *)v0 + v1) = (char *)v2;
}

/* ========================================================================
 * sub_1001C9520
 * EA: 0x1001c9520
 ======================================================================== */

__int64 sub_1001C9520()
{
  _QWORD *v0; // x22
  __int64 v1; // x19
  void *v2; // x20
  __int64 v3; // x8
  void *v4; // x26
  id v5; // x19
  id v6; // x0
  __int64 v7; // x8
  __int64 v8; // x9
  id v9; // x0
  __int64 v10; // x27
  __int64 v11; // x0
  __int64 v12; // x28
  void *v13; // x21
  id v14; // x21
  __int64 v15; // x0
  _QWORD *v16; // x0
  _QWORD *v17; // x0
  __int64 v18; // x8
  __int64 v19; // x24
  __int64 v20; // x25
  double v21; // d0
  _QWORD *v22; // x0
  void *v24; // x20
  id v25; // x21
  int v26; // w23
  id v27; // x0
  __int64 v28; // x20
  _QWORD *v29; // x0
  void *v30; // x19
  __int64 v31; // x8
  __int64 v32; // x0
  void *v33; // x20
  id v34; // x23
  __int64 v35; // x0
  __int64 v36; // x0
  CGFloat v37; // d8
  __int64 v38; // x0
  __int64 v39; // x0
  __int64 v40; // x0
  void *v41; // x20
  id v42; // x21
  __int64 v43; // x20
  _QWORD *v44; // x0
  CGRect v45; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8

  v1 = v0[2];
  v2 = *(void **)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_bufferLock);
  v0[11] = v2;
  objc_msgSend(v2, "lock");
  v3 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController__internalBuffer;
  v0[12] = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController__internalBuffer;
  v4 = *(void **)(v1 + v3);
  v0[13] = v4;
  v5 = objc_retain(v4);
  v6 = objc_msgSend(v2, "unlock");
  if ( !v4 )
    goto LABEL_7;
  v7 = v0[2];
  v8 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_photoView;
  v0[14] = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_photoView;
  v9 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(v7 + v8), "superview"));
  if ( !v9 )
  {
    objc_release(v5);
LABEL_7:
    static Clock<>.continuous.getter(v6);
    v17 = (_QWORD *)swift_task_alloc(128);
    v0[34] = v17;
    *v17 = v0;
    v17[1] = sub_1001CA1E4;
    v18 = 10000000000000000LL;
    return sub_1001DC42C(v18, 0, 0, 0, 1);
  }
  v10 = v0[2];
  objc_release(v9);
  v12 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastRenderedBuffer;
  v13 = *(void **)(v10 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastRenderedBuffer);
  v0[15] = v13;
  if ( v13 )
  {
    type metadata accessor for CVBuffer(0);
    sub_10003EF70(&unk_1006682C8, type metadata accessor for CVBuffer, &unk_10053AC84);
    v14 = objc_retain(v13);
    v15 = static _CFObject.== infix(_:_:)();
    if ( (v15 & 1) != 0 )
    {
      static Clock<>.continuous.getter(v15);
      v16 = (_QWORD *)swift_task_alloc(128);
      v0[16] = v16;
      *v16 = v0;
      v16[1] = sub_1001C998C;
LABEL_11:
      v18 = 2000000000000000LL;
      return sub_1001DC42C(v18, 0, 0, 0, 1);
    }
    objc_release(v14);
  }
  v19 = v0[2];
  v20 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_curBufferIndex;
  v21 = *(double *)(v19 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_curBufferIndex);
  if ( v21 == *(double *)(v19 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastBufferIndex) )
  {
    static Clock<>.continuous.getter(v11);
    v22 = (_QWORD *)swift_task_alloc(128);
    v0[17] = v22;
    *v22 = v0;
    v22[1] = sub_1001C9A84;
    goto LABEL_11;
  }
  *(double *)(v19 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastBufferIndex) = v21;
  v24 = *(void **)(v10 + v12);
  *(_QWORD *)(v10 + v12) = v4;
  v25 = objc_retain(objc_retain(v5));
  objc_release(v24);
  v26 = *(unsigned __int8 *)(v19 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_stereoDisabled);
  v27 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CIImage), "initWithCVPixelBuffer:", v25);
  v0[18] = v27;
  v28 = v0[2];
  if ( v26 == 1 )
  {
    objc_release(v25);
    v29 = (_QWORD *)swift_task_alloc(208);
    v0[19] = v29;
    *v29 = v0;
    v29[1] = sub_1001C9B74;
    return sub_1001DB840(0, 1);
  }
  else
  {
    v30 = v27;
    v31 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_orientation;
    v0[22] = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_orientation;
    if ( *(_DWORD *)(v28 + v31) == 6 )
    {
      v32 = sub_1001CAD70(v25);
      if ( v32 )
      {
        v33 = (void *)v32;
        v34 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CIImage), "initWithCVPixelBuffer:", v32);
        objc_release(v33);
        objc_release(v30);
        v30 = v34;
      }
    }
    v35 = sub_1001CAAB4(v30);
    v36 = sub_1001CAA4C(v35);
    if ( v36 >= 21 )
    {
      v37 = (double)sub_1001CAA4C(v36);
      objc_msgSend(v30, "extent");
      if ( CGRectGetHeight(v45) * 0.2 > v37 )
      {
        v39 = sub_1001CAA4C(v38);
        v40 = sub_1000874A8(v39);
        if ( v40 )
        {
          v41 = (void *)v40;
          v42 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CIImage), "initWithCVPixelBuffer:", v40);
          objc_release(v41);
          objc_release(v30);
          v30 = v42;
        }
      }
    }
    v0[23] = v30;
    byte_1006969C0 = 0;
    if ( qword_100662428 != -1 )
      swift_once(&qword_100662428, sub_10015B98C);
    v43 = qword_100696E10;
    v0[24] = qword_100696E10;
    *(_QWORD *)(v43 + 104) = *(_QWORD *)(v19 + v20);
    v44 = (_QWORD *)swift_task_alloc(928);
    v0[25] = v44;
    *v44 = v0;
    v44[1] = sub_1001C9CE8;
    return sub_10015D4EC(v30);
  }
}

/* ========================================================================
 * sub_1001C9D54
 * EA: 0x1001c9d54
 ======================================================================== */

__int64 sub_1001C9D54()
{
  __int64 v0; // x22
  void *v1; // x8
  void *v2; // x20
  __int64 v3; // x23
  id v4; // x19
  id v5; // x21
  void *v6; // x20
  __int64 v7; // x23
  __int64 v8; // x24
  void *v9; // x27
  id v10; // x24
  id v11; // x23
  __int64 v12; // x20
  __int64 v13; // x0
  void *v14; // x20
  __int64 v15; // x23
  __int64 v16; // x24
  void *v17; // x24
  id v18; // x20
  void *v19; // x19
  __int64 v20; // x20
  __int64 v21; // x20
  __int64 v22; // x19
  __int64 v24; // x20
  __int64 v25; // x0
  __int64 v26; // x1
  _QWORD *v27; // x0

  v1 = *(void **)(v0 + 208);
  if ( !v1 )
  {
    v19 = *(void **)(v0 + 104);
    v20 = *(_QWORD *)(v0 + 64);
    objc_release(*(id *)(v0 + 184));
    swift_release(v20);
    objc_release(v19);
    objc_release(v19);
    v21 = *(_QWORD *)(v0 + 48);
    v22 = *(_QWORD *)(v0 + 40);
    swift_task_dealloc(*(_QWORD *)(v0 + 56));
    swift_task_dealloc(v21);
    swift_task_dealloc(v22);
    return (*(__int64 (**)(void))(v0 + 8))();
  }
  v2 = *(void **)(v0 + 216);
  v3 = qword_100662390;
  v4 = objc_retain(v1);
  v5 = objc_retain(v2);
  if ( v3 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  if ( (*(_BYTE *)(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences_isGaming) & 1) == 0 )
  {
    v11 = *(id *)(v0 + 104);
    goto LABEL_11;
  }
  v6 = *(void **)(v0 + 88);
  v7 = *(_QWORD *)(v0 + 96);
  v8 = *(_QWORD *)(v0 + 16);
  objc_msgSend(v6, "lock");
  v9 = *(void **)(v8 + v7);
  v10 = objc_retain(v9);
  objc_msgSend(v6, "unlock");
  v11 = *(id *)(v0 + 104);
  if ( !v9 )
  {
LABEL_11:
    objc_release(v11);
    goto LABEL_12;
  }
  v12 = type metadata accessor for CVBuffer(0);
  v13 = sub_10003EF70(&unk_1006682C8, type metadata accessor for CVBuffer, &unk_10053AC84);
  LOBYTE(v12) = static _CFObject.== infix(_:_:)(v10, v11, v12, v13);
  objc_release(v11);
  objc_release(v10);
  if ( (v12 & 1) != 0 )
  {
    v14 = *(void **)(v0 + 88);
    v15 = *(_QWORD *)(v0 + 96);
    v16 = *(_QWORD *)(v0 + 16);
    objc_msgSend(v14, "lock");
    v17 = *(void **)(v16 + v15);
    v11 = objc_retain(v17);
    objc_msgSend(v14, "unlock");
    if ( v17 )
    {
      v18 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CIImage), "initWithCVPixelBuffer:", v11);
      objc_release(v4);
      v4 = v18;
      goto LABEL_11;
    }
  }
LABEL_12:
  *(_QWORD *)(v0 + 224) = v4;
  if ( *(_BYTE *)(*(_QWORD *)(v0 + 192) + 129LL) == 1 )
  {
    v24 = sub_1001152AC(v4, v5, 1.0);
    v25 = sub_1001152AC(v4, v5, 0.0);
  }
  else
  {
    v24 = sub_100217E4C(v4, v5);
    v25 = v26;
  }
  *(_QWORD *)(v0 + 232) = v24;
  *(_QWORD *)(v0 + 240) = v25;
  v27 = (_QWORD *)swift_task_alloc(208);
  *(_QWORD *)(v0 + 248) = v27;
  *v27 = v0;
  v27[1] = sub_1001CA028;
  return sub_1001DB840(0, 1);
}

/* ========================================================================
 * sub_1001CA388
 * EA: 0x1001ca388
 ======================================================================== */

__int64 sub_1001CA388()
{
  _QWORD *v0; // x22
  __int64 v1; // x23
  __int64 v2; // x27
  __int64 v3; // x28
  __int64 v4; // x21
  __int64 v5; // x19
  __int64 v6; // x25
  void *v7; // x24
  void *v8; // x26
  __int64 v9; // x0
  void *v10; // x27
  __int64 v11; // x28
  __int64 v12; // x24
  __int64 v13; // x0
  __int64 v15; // [xsp+0h] [xbp-70h]
  __int64 v16; // [xsp+8h] [xbp-68h]
  __int64 v17; // [xsp+10h] [xbp-60h]

  v1 = v0[18];
  v2 = v0[16];
  v3 = v0[17];
  v4 = v0[15];
  v5 = v0[12];
  v16 = v0[14];
  v17 = v0[13];
  v6 = v0[10];
  v15 = v0[11];
  v7 = (void *)v0[9];
  swift_release(v0[19]);
  sub_100040668(0);
  (*(void (__fastcall **)(__int64, _QWORD, __int64))(v3 + 104))(
    v1,
    enum case for DispatchQoS.QoSClass.userInitiated(_:),
    v2);
  v8 = (void *)static OS_dispatch_queue.global(qos:)(v1);
  (*(void (__fastcall **)(__int64, __int64))(v3 + 8))(v1, v2);
  v9 = swift_allocObject(&unk_1005C5390, 24, 7);
  *(_QWORD *)(v9 + 16) = v7;
  v0[6] = sub_1001CAFE8;
  v0[7] = v9;
  v0[2] = _NSConcreteStackBlock;
  v0[3] = 1107296256;
  v0[4] = sub_100040600;
  v0[5] = &unk_1005C53A8;
  v10 = _Block_copy(v0 + 2);
  static DispatchQoS.unspecified.getter(objc_retain(v7));
  v0[8] = &_swiftEmptyArrayStorage;
  v11 = sub_10003EF70(
          &qword_100669D60,
          &type metadata accessor for DispatchWorkItemFlags,
          &protocol conformance descriptor for DispatchWorkItemFlags);
  v12 = sub_10003E4E0((__int64 *)&unk_100668B40, &qword_10053B8F0);
  v13 = sub_100040724();
  dispatch thunk of SetAlgebra.init<A>(_:)(v0 + 8, v12, v13, v6, v11);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v4, v5, v10);
  _Block_release(v10);
  objc_release(v8);
  (*(void (__fastcall **)(__int64, __int64))(v15 + 8))(v5, v6);
  (*(void (__fastcall **)(__int64, __int64))(v16 + 8))(v4, v17);
  swift_release(v0[7]);
  swift_task_dealloc(v1);
  swift_task_dealloc(v4);
  swift_task_dealloc(v5);
  return ((__int64 (*)(void))v0[1])();
}

/* ========================================================================
 * sub_1001CA574
 * EA: 0x1001ca574
 ======================================================================== */

__int64 __fastcall sub_1001CA574(__int64 a1)
{
  __int64 v2; // x19
  char *v3; // x21
  __int64 v4; // x22
  char *v5; // x23
  _QWORD *v6; // x20
  __int64 v7; // x0
  __int64 v8; // x26
  _QWORD *v9; // x0
  __int64 v10; // x26
  void *v11; // x27
  __int64 v12; // x20
  __int64 v13; // x0
  void *v14; // x25
  __int64 v15; // x20
  __int64 v16; // x0
  __int64 v17; // x24
  __int64 v18; // x20
  __int64 v19; // x0
  __int64 v21; // [xsp+0h] [xbp-90h] BYREF
  __int64 v22; // [xsp+8h] [xbp-88h]
  _QWORD aBlock[5]; // [xsp+10h] [xbp-80h] BYREF
  __int64 v24; // [xsp+38h] [xbp-58h]

  v2 = type metadata accessor for DispatchWorkItemFlags(0);
  v22 = *(_QWORD *)(v2 - 8);
  v3 = (char *)&v21 - ((*(_QWORD *)(v22 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v21 = type metadata accessor for DispatchQoS(0);
  v4 = *(_QWORD *)(v21 - 8);
  v5 = (char *)&v21 - ((*(_QWORD *)(v4 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v6 = (__int64 *)((char *)&v21
                 - ((*(_QWORD *)(*(_QWORD *)(sub_10003E4E0((__int64 *)&unk_100668B60, &qword_10053BAE0) - 8) + 64LL)
                   + 15LL)
                  & 0xFFFFFFFFFFFFFFF0LL));
  static TaskPriority.userInitiated.getter(v6);
  v7 = type metadata accessor for TaskPriority(0);
  (*(void (__fastcall **)(_QWORD *, _QWORD, __int64, __int64))(*(_QWORD *)(v7 - 8) + 56LL))(v6, 0, 1, v7);
  v8 = swift_allocObject(&unk_1005C53E0, 24, 7);
  swift_unknownObjectWeakInit(v8 + 16, a1);
  v9 = (_QWORD *)swift_allocObject(&unk_1005C5408, 40, 7);
  v9[2] = 0;
  v9[3] = 0;
  v9[4] = v8;
  v10 = sub_100087664(0, 0, v6, &unk_1005433C8, v9);
  sub_100040668(0);
  v11 = (void *)static OS_dispatch_queue.main.getter();
  v12 = swift_allocObject(&unk_1005C53E0, 24, 7);
  swift_unknownObjectWeakInit(v12 + 16, a1);
  v13 = swift_allocObject(&unk_1005C5430, 32, 7);
  *(_QWORD *)(v13 + 16) = v12;
  *(_QWORD *)(v13 + 24) = v10;
  aBlock[4] = sub_1001CB0F0;
  v24 = v13;
  aBlock[0] = _NSConcreteStackBlock;
  aBlock[1] = 1107296256;
  aBlock[2] = sub_100040600;
  aBlock[3] = &unk_1005C5448;
  v14 = _Block_copy(aBlock);
  v15 = v24;
  swift_retain(v10);
  v16 = swift_release(v15);
  static DispatchQoS.unspecified.getter(v16);
  aBlock[0] = &_swiftEmptyArrayStorage;
  v17 = sub_10003EF70(
          &qword_100669D60,
          &type metadata accessor for DispatchWorkItemFlags,
          &protocol conformance descriptor for DispatchWorkItemFlags);
  v18 = sub_10003E4E0((__int64 *)&unk_100668B40, &qword_10053B8F0);
  v19 = sub_100040724();
  dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v18, v19, v2, v17);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v5, v3, v14);
  _Block_release(v14);
  swift_release(v10);
  objc_release(v11);
  (*(void (__fastcall **)(char *, __int64))(v22 + 8))(v3, v2);
  return (*(__int64 (__fastcall **)(char *, __int64))(v4 + 8))(v5, v21);
}

/* ========================================================================
 * sub_1001CAD70
 * EA: 0x1001cad70
 ======================================================================== */

CVPixelBufferRef __fastcall sub_1001CAD70(__CVBuffer *a1)
{
  __int64 v1; // x20
  __int64 v3; // x19
  void *v4; // x8
  OpaqueVTPixelRotationSession *v5; // x19
  size_t Width; // x20
  size_t Height; // x22
  OSType PixelFormatType; // w0
  CVPixelBufferRef result; // x0
  OSStatus v10; // w22
  Swift::String v11; // x0
  void *object; // x22
  __int64 v13; // x0
  __int64 v14; // x0
  CVPixelBufferRef pixelBufferOut; // [xsp+20h] [xbp-30h] BYREF

  v3 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_rotationCW90Session;
  v4 = *(void **)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_rotationCW90Session);
  if ( v4 || (sub_1001CAC18(), (v4 = *(void **)(v1 + v3)) != nullptr) )
  {
    pixelBufferOut = nullptr;
    v5 = objc_retain(v4);
    Width = CVPixelBufferGetWidth(a1);
    Height = CVPixelBufferGetHeight(a1);
    PixelFormatType = CVPixelBufferGetPixelFormatType(a1);
    result = (CVPixelBufferRef)CVPixelBufferCreate(
                                 kCFAllocatorDefault,
                                 Height,
                                 Width,
                                 PixelFormatType,
                                 nullptr,
                                 &pixelBufferOut);
    if ( !pixelBufferOut )
    {
      __break(1u);
      return result;
    }
    v10 = VTPixelRotationSessionRotateImage(v5, a1, pixelBufferOut);
    if ( v10 == (unsigned int)noErr.getter() )
    {
      objc_release(v5);
      return pixelBufferOut;
    }
    type metadata accessor for VTLogger(0);
    _StringGuts.grow(_:)(42);
    swift_bridgeObjectRelease(0xE000000000000000LL);
    v11._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                               &type metadata for Int32,
                               &protocol witness table for Int32);
    object = v11._object;
    String.append(_:)(v11);
    v13 = swift_bridgeObjectRelease(object);
    v14 = static os_log_type_t.error.getter(v13);
    sub_1001D8B44(v14, 0xD000000000000028LL, 0x80000001004D9D00LL);
    swift_bridgeObjectRelease(0x80000001004D9D00LL);
    objc_release(v5);
    objc_release(pixelBufferOut);
  }
  return nullptr;
}

/* ========================================================================
 * sub_1001CB208
 * EA: 0x1001cb208
 ======================================================================== */

id __fastcall sub_1001CB208(__int64 a1)
{
  _BYTE *v1; // x20
  _BYTE *v2; // x19
  __int64 v4; // x20
  __int128 v5; // q1
  __int64 v6; // x22
  void *v7; // x24
  id v8; // x23
  id v9; // x24
  __int64 v10; // x0
  char v11; // w8
  __int64 v12; // x22
  __int64 v13; // x20
  __int64 v14; // x23
  id v15; // x0
  __int64 v16; // x23
  __int64 v17; // x0
  _QWORD *v18; // x20
  __int64 v19; // x20
  id v20; // x20
  objc_super v22; // [xsp+8h] [xbp-A8h] BYREF
  _QWORD v23[2]; // [xsp+18h] [xbp-98h] BYREF
  char v24[24]; // [xsp+28h] [xbp-88h] BYREF
  __int128 v25; // [xsp+40h] [xbp-70h]
  __int128 v26; // [xsp+50h] [xbp-60h]
  __int64 v27; // [xsp+60h] [xbp-50h]
  __int128 v28; // [xsp+70h] [xbp-40h] BYREF

  v2 = v1;
  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  v4 = qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ai3DOptionRaw;
  swift_beginAccess(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ai3DOptionRaw, v24, 0, 0);
  v5 = *(_OWORD *)(v4 + 16);
  v25 = *(_OWORD *)v4;
  v26 = v5;
  v27 = *(_QWORD *)(v4 + 32);
  v6 = *((_QWORD *)&v25 + 1);
  v7 = (void *)v5;
  v28 = *(_OWORD *)(v4 + 24);
  v8 = objc_retain((id)v25);
  swift_retain(v6);
  v9 = objc_retain(v7);
  sub_10004542C((__int64)&v28, (__int64)v23);
  v10 = sub_10003E4E0((__int64 *)&unk_10066AFA0, (__int64 *)&unk_10053BAB0);
  WrappedDefault.wrappedValue.getter(v23, v10);
  objc_release(v9);
  swift_release(v6);
  objc_release(v8);
  sub_10003F604(&v28);
  if ( v23[0] >= 3u )
    v11 = 1;
  else
    v11 = v23[0];
  v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_option] = v11;
  v12 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_playerVC;
  swift_unknownObjectWeakInit(&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_playerVC], 0);
  v13 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_bufferLock;
  *(_QWORD *)&v2[v13] = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSLock), "init");
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController__internalBuffer] = 0;
  v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_isPaused] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastRenderedBuffer] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_task] = 0;
  v14 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_photoView;
  v15 = objc_allocWithZone((Class)type metadata accessor for SBSFastView());
  *(_QWORD *)&v2[v14] = sub_1000B290C(0, 0.0, 0.0, 0.0, 0.0);
  v16 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_fpsController;
  v17 = type metadata accessor for FPSController();
  v18 = (_QWORD *)swift_allocObject(v17, 144, 15);
  swift_defaultActor_initialize();
  *(_QWORD *)&v2[v16] = v18;
  *(_DWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_orientation] = 1;
  v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_showImmersive3DGuide] = 0;
  v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_stereoDisabled] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_curBufferIndex] = 0;
  v18[14] = 0;
  v18[15] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastBufferIndex] = 0;
  v18[16] = 0;
  v18[17] = &_swiftEmptyArrayStorage;
  v19 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_sequenceKey;
  *(_QWORD *)&v2[v19] = String._bridgeToObjectiveC()();
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_fpsTimer] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastRemainBattery] = -1;
  v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_shouldCheckBlackEdge] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_blackEdgeCheckingCounter] = 240;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_blackEdgeHeights] = &_swiftEmptyArrayStorage;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_blackEdgeThreshold] = 20;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_rotationCW90Session] = 0;
  swift_unknownObjectWeakAssign(&v2[v12], a1);
  v20 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIApplication), "sharedApplication"));
  objc_msgSend(v20, "setIdleTimerDisabled:", 1);
  objc_release(v20);
  v22.receiver = v2;
  v22.super_class = (Class)type metadata accessor for VTVideoDepthInferenceController();
  return objc_msgSendSuper2(&v22, "initWithNibName:bundle:", 0, 0);
}

/* ========================================================================
 * sub_1001CB524
 * EA: 0x1001cb524
 ======================================================================== */

id __fastcall sub_1001CB524(void *a1)
{
  __int64 v1; // x20
  __int64 v3; // x19
  __int64 v4; // x24
  char *v5; // x22
  void *v6; // x21
  void *v7; // x25
  id v8; // x0
  __int64 v9; // x23
  __int64 v10; // x0
  double v11; // d8
  __int64 v13; // [xsp+0h] [xbp-50h] BYREF

  v3 = type metadata accessor for Date(0);
  v4 = *(_QWORD *)(v3 - 8);
  v5 = (char *)&v13 - ((*(_QWORD *)(v4 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v6 = *(void **)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_bufferLock);
  objc_msgSend(v6, "lock");
  v7 = *(void **)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController__internalBuffer);
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController__internalBuffer) = a1;
  v8 = objc_retain(a1);
  objc_release(v7);
  *(double *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_curBufferIndex) = *(double *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_curBufferIndex) + 1.0;
  if ( qword_100662428 != -1 )
    swift_once(&qword_100662428, sub_10015B98C);
  v9 = qword_100696E10;
  if ( *(_BYTE *)(qword_100696E10 + 128) == 1 )
  {
    v10 = Date.init()();
    v11 = Date.timeIntervalSince1970.getter(v10);
    (*(void (__fastcall **)(char *, __int64))(v4 + 8))(v5, v3);
    *(double *)(v9 + 96) = v11;
  }
  return objc_msgSend(v6, "unlock");
}

/* ========================================================================
 * sub_1001CB65C
 * EA: 0x1001cb65c
 ======================================================================== */

void __noreturn sub_1001CB65C()
{
  __int64 v0; // x20
  __int64 v1; // x19
  __int64 v2; // x20
  __int128 v3; // q1
  __int64 v4; // x21
  void *v5; // x23
  id v6; // x22
  id v7; // x23
  __int64 v8; // x0
  char v9; // w8
  __int64 v10; // x20
  __int64 v11; // x21
  id v12; // x0
  __int64 v13; // x21
  __int64 v14; // x0
  _QWORD *v15; // x20
  __int64 v16; // x20
  _QWORD v17[2]; // [xsp+18h] [xbp-98h] BYREF
  _BYTE v18[24]; // [xsp+28h] [xbp-88h] BYREF
  __int128 v19; // [xsp+40h] [xbp-70h]
  __int128 v20; // [xsp+50h] [xbp-60h]
  __int64 v21; // [xsp+60h] [xbp-50h]
  __int128 v22; // [xsp+70h] [xbp-40h] BYREF

  v1 = v0;
  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  v2 = qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ai3DOptionRaw;
  swift_beginAccess(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ai3DOptionRaw, v18, 0, 0);
  v3 = *(_OWORD *)(v2 + 16);
  v19 = *(_OWORD *)v2;
  v20 = v3;
  v21 = *(_QWORD *)(v2 + 32);
  v4 = *((_QWORD *)&v19 + 1);
  v5 = (void *)v3;
  v22 = *(_OWORD *)(v2 + 24);
  v6 = objc_retain((id)v19);
  swift_retain(v4);
  v7 = objc_retain(v5);
  sub_10004542C(&v22, v17);
  v8 = sub_10003E4E0(&unk_10066AFA0, &unk_10053BAB0);
  WrappedDefault.wrappedValue.getter(v17, v8);
  objc_release(v7);
  swift_release(v4);
  objc_release(v6);
  sub_10003F604(&v22);
  if ( v17[0] >= 3u )
    v9 = 1;
  else
    v9 = v17[0];
  *(_BYTE *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_option) = v9;
  swift_unknownObjectWeakInit(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_playerVC, 0);
  v10 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_bufferLock;
  *(_QWORD *)(v1 + v10) = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSLock), "init");
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController__internalBuffer) = 0;
  *(_BYTE *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_isPaused) = 0;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastRenderedBuffer) = 0;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_task) = 0;
  v11 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_photoView;
  v12 = objc_allocWithZone((Class)type metadata accessor for SBSFastView());
  *(_QWORD *)(v1 + v11) = sub_1000B290C(0, 0.0, 0.0, 0.0, 0.0);
  v13 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_fpsController;
  v14 = type metadata accessor for FPSController();
  v15 = (_QWORD *)swift_allocObject(v14, 144, 15);
  swift_defaultActor_initialize();
  *(_QWORD *)(v1 + v13) = v15;
  *(_DWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_orientation) = 1;
  *(_BYTE *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_showImmersive3DGuide) = 0;
  *(_BYTE *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_stereoDisabled) = 0;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_curBufferIndex) = 0;
  v15[14] = 0;
  v15[15] = 0;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastBufferIndex) = 0;
  v15[16] = 0;
  v15[17] = &_swiftEmptyArrayStorage;
  v16 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_sequenceKey;
  *(_QWORD *)(v1 + v16) = String._bridgeToObjectiveC()();
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_fpsTimer) = 0;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastRemainBattery) = -1;
  *(_BYTE *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_shouldCheckBlackEdge) = 0;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_blackEdgeCheckingCounter) = 240;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_blackEdgeHeights) = &_swiftEmptyArrayStorage;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_blackEdgeThreshold) = 20;
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_rotationCW90Session) = 0;
  _assertionFailure(_:_:file:line:flags:)(
    "Fatal error",
    11,
    2,
    0xD000000000000025LL,
    0x80000001004D9660LL,
    "ExternalMonitor/VTVideoDepthInferenceController.swift",
    53,
    2,
    57,
    0);
  __break(1u);
}

/* ========================================================================
 * sub_1001CBC24
 * EA: 0x1001cbc24
 ======================================================================== */

void sub_1001CBC24()
{
  char *v0; // x20
  id v1; // x19
  __int64 v2; // x1
  __int64 v3; // x21
  NSString v4; // x22
  NSString v5; // x21
  id v6; // x22
  id v7; // x21
  id v8; // x0
  void *v9; // x21
  id v10; // x19
  char *v11; // x21
  char *v12; // x0
  id v13; // x0
  void *v14; // x20
  char *v15; // [xsp+8h] [xbp-38h]

  v1 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UILabel), "init");
  sub_10024814C(0x656C616353LL, 0xE500000000000000LL, 0);
  v3 = v2;
  v4 = String._bridgeToObjectiveC()();
  swift_bridgeObjectRelease(v3);
  objc_msgSend(v1, "setText:", v4);
  objc_release(v4);
  v5 = String._bridgeToObjectiveC()();
  v6 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIFont), "fontWithName:size:", v5, 18.0));
  objc_release(v5);
  objc_msgSend(v1, "setFont:", v6);
  objc_release(v6);
  v7 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIColor), "whiteColor"));
  objc_msgSend(v1, "setTextColor:", v7);
  objc_release(v7);
  v8 = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "view"));
  if ( v8 )
  {
    v9 = v8;
    v10 = objc_retain(v1);
    objc_msgSend(v9, "addSubview:", v10);
    objc_release(v9);
    ConstraintViewDSL.makeConstraints(_:)(sub_1001CBE38, 0, v10);
    objc_release(v10);
    v11 = *(char **)&v0[OBJC_IVAR____TtC15ExternalMonitor27VTPCAMSettingViewController_slider];
    v12 = &v11[OBJC_IVAR____TtC15ExternalMonitor21VTPCAmbientModeSlider_delegate];
    *((_QWORD *)v12 + 1) = &off_1005C5548;
    swift_unknownObjectWeakAssign(v12, v0);
    v13 = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "view"));
    if ( v13 )
    {
      v14 = v13;
      objc_msgSend(v13, "addSubview:", v11);
      objc_release(v14);
      v15 = objc_retain(v11);
      ConstraintViewDSL.makeConstraints(_:)(sub_1001CBE48, 0, v15);
      objc_release(v10);
      objc_release(v15);
      return;
    }
  }
  else
  {
    __break(1u);
  }
  __break(1u);
}

/* ========================================================================
 * sub_1001CBE48
 * EA: 0x1001cbe48
 ======================================================================== */

__int64 __fastcall sub_1001CBE48(__int64 a1)
{
  __int64 v2; // x20
  __int64 v3; // x0
  __int64 v4; // x0
  __int64 v5; // x20
  __int64 v6; // x0
  __int64 v7; // x0
  __int64 v8; // x20
  __int64 v9; // x0
  __int64 v10; // x0
  __int64 v11; // x20
  __int64 v12; // x0
  _QWORD v14[3]; // [xsp+8h] [xbp-58h] BYREF
  void *v15; // [xsp+20h] [xbp-40h]
  void *v16; // [xsp+28h] [xbp-38h]

  v2 = (*(__int64 (**)(void))(*(_QWORD *)a1 + 144LL))();
  v15 = &type metadata for Int;
  v16 = &protocol witness table for Int;
  v14[0] = 180;
  v3 = (*(__int64 (__fastcall **)(_QWORD *, unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v2 + 104LL))(
         v14,
         0xD000000000000067LL,
         0x80000001004F2BA0LL,
         39);
  swift_release(v3);
  swift_release(v2);
  v4 = sub_10003F5DC(v14);
  v5 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)a1 + 152LL))(v4);
  v15 = &type metadata for Int;
  v16 = &protocol witness table for Int;
  v14[0] = 44;
  v6 = (*(__int64 (__fastcall **)(_QWORD *, unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v5 + 104LL))(
         v14,
         0xD000000000000067LL,
         0x80000001004F2BA0LL,
         40);
  swift_release(v6);
  swift_release(v5);
  v7 = sub_10003F5DC(v14);
  v8 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)a1 + 104LL))(v7);
  v15 = &type metadata for Int;
  v16 = &protocol witness table for Int;
  v14[0] = 341;
  v9 = (*(__int64 (__fastcall **)(_QWORD *, unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v8 + 104LL))(
         v14,
         0xD000000000000067LL,
         0x80000001004F2BA0LL,
         41);
  swift_release(v9);
  swift_release(v8);
  v10 = sub_10003F5DC(v14);
  v11 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)a1 + 96LL))(v10);
  v15 = &type metadata for Int;
  v16 = &protocol witness table for Int;
  v14[0] = 16;
  v12 = (*(__int64 (__fastcall **)(_QWORD *, unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v11 + 104LL))(
          v14,
          0xD000000000000067LL,
          0x80000001004F2BA0LL,
          42);
  swift_release(v12);
  swift_release(v11);
  return sub_10003F5DC(v14);
}

/* ========================================================================
 * sub_1001CC13C
 * EA: 0x1001cc13c
 ======================================================================== */

void sub_1001CC13C()
{
  char *v0; // x20
  char *v1; // x19
  id v2; // x20
  __int64 v3; // x1
  __int64 v4; // x21
  NSString v5; // x22
  id v6; // x21
  NSString v7; // x21
  id v8; // x22
  id v9; // x0
  void *v10; // x21
  id v11; // x20
  __int64 v12; // x0
  void *v13; // x24
  __int64 v14; // x26
  __int64 v15; // x25
  char *v16; // x27
  id v17; // x20
  id v18; // x20
  id v19; // x28
  NSString v20; // x20
  id v21; // x21
  NSString v22; // x20
  id v23; // x21
  id v24; // x20
  id v25; // x20
  __int64 v26; // x20
  __int128 v27; // q1
  __int64 v28; // x21
  void *v29; // x23
  id v30; // x22
  id v31; // x23
  __int64 v32; // x0
  id v33; // x20
  id v34; // x0
  void *v35; // x20
  NSString v36; // x21
  id v37; // x22
  __int64 v38; // x20
  __int64 v39; // x21
  __int64 v40; // x1
  __int64 v41; // x22
  NSString v42; // x20
  id v43; // x0
  void *v44; // x20
  __int64 v45; // x0
  unsigned __int64 v46; // x8
  unsigned __int64 v47; // x21
  id v48; // x20
  __int64 v49; // x1
  __int64 v50; // x21
  NSString v51; // x22
  NSString v52; // x21
  id v53; // x22
  id v54; // x0
  void *v55; // x21
  NSString v56; // x22
  id v57; // x23
  id v58; // x0
  void *v59; // x21
  id v60; // x20
  _QWORD v61[4]; // [xsp-20h] [xbp-150h] BYREF
  id v62; // [xsp+0h] [xbp-130h]
  id v63; // [xsp+8h] [xbp-128h]
  char *v64; // [xsp+10h] [xbp-120h]
  const char *v65; // [xsp+18h] [xbp-118h]
  char *v66; // [xsp+20h] [xbp-110h]
  const char *v67; // [xsp+28h] [xbp-108h]
  id ObjCClassFromMetadata; // [xsp+30h] [xbp-100h]
  id v69; // [xsp+38h] [xbp-F8h]
  _QWORD v70[3]; // [xsp+40h] [xbp-F0h] BYREF
  char v71[24]; // [xsp+58h] [xbp-D8h] BYREF
  __int128 v72; // [xsp+70h] [xbp-C0h]
  __int128 v73; // [xsp+80h] [xbp-B0h]
  __int64 v74; // [xsp+90h] [xbp-A0h]
  _OWORD v75[2]; // [xsp+A0h] [xbp-90h] BYREF

  v1 = v0;
  v2 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UILabel), "init");
  sub_10024814C(0x6E656D6563616C50LL, 0xE900000000000074LL, 0);
  v4 = v3;
  v5 = String._bridgeToObjectiveC()();
  swift_bridgeObjectRelease(v4);
  objc_msgSend(v2, "setText:", v5);
  objc_release(v5);
  v69 = (id)objc_opt_self(&OBJC_CLASS___UIColor);
  v6 = objc_retainAutoreleasedReturnValue(objc_msgSend(v69, "whiteColor"));
  objc_msgSend(v2, "setTextColor:", v6);
  objc_release(v6);
  v64 = "nitor16proPlanAItemView";
  v7 = String._bridgeToObjectiveC()();
  v63 = (id)objc_opt_self(&OBJC_CLASS___UIFont);
  v8 = objc_retainAutoreleasedReturnValue(objc_msgSend(v63, "fontWithName:size:", v7, 18.0));
  objc_release(v7);
  objc_msgSend(v2, "setFont:", v8);
  objc_release(v8);
  v9 = objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "view"));
  if ( v9 )
  {
    v10 = v9;
    v11 = objc_retain(v2);
    objc_msgSend(v10, "addSubview:", v11);
    objc_release(v10);
    ConstraintViewDSL.makeConstraints(_:)(sub_1001CCA70, 0, v11);
    v62 = v11;
    objc_release(v11);
    v12 = type metadata accessor for VTRadioButton(0);
    ObjCClassFromMetadata = (id)swift_getObjCClassFromMetadata(v12);
    v13 = (void *)objc_opt_self(&OBJC_CLASS___UIImage);
    v14 = 0;
    v67 = "placementButtonTapped:";
    v15 = OBJC_IVAR____TtC15ExternalMonitor27VTPCAMSettingViewController_buttons;
    v66 = "ttons";
    v16 = &aBottomRight_0[8];
    v65 = "RadioUnselectedSmall";
    do
    {
      v17 = objc_retain(objc_retainAutoreleasedReturnValue(objc_msgSend(ObjCClassFromMetadata, "buttonWithType:", 0, v62)));
      objc_msgSend(v17, "setContentHorizontalAlignment:", 1);
      v18 = objc_retain(v17);
      objc_msgSend(v18, "setImageEdgeInsets:", 0.0, 0.0, 0.0, 0.0);
      objc_msgSend(v18, "setTitleEdgeInsets:", 0.0, 12.0, 0.0, 0.0);
      v19 = objc_retain(v18);
      objc_msgSend(v19, "setTag:", v14);
      v20 = String._bridgeToObjectiveC()();
      v21 = objc_retainAutoreleasedReturnValue(objc_msgSend(v13, "imageNamed:", v20));
      objc_release(v20);
      objc_msgSend(v19, "setImage:forState:", v21, 0);
      objc_release(v21);
      v22 = String._bridgeToObjectiveC()();
      v23 = objc_retainAutoreleasedReturnValue(objc_msgSend(v13, "imageNamed:", v22));
      objc_release(v22);
      objc_msgSend(v19, "setImage:forState:", v23, 4);
      objc_release(v23);
      v24 = objc_msgSend(
              objc_allocWithZone((Class)&OBJC_CLASS___UIColor),
              "initWithRed:green:blue:alpha:",
              0.635294118,
              0.701960784,
              0.8,
              1.0);
      objc_msgSend(v19, "setTitleColor:forState:", v24, 0);
      objc_release(v24);
      v25 = objc_retainAutoreleasedReturnValue(objc_msgSend(v69, "whiteColor"));
      objc_msgSend(v19, "setTitleColor:forState:", v25, 4);
      objc_release(v25);
      if ( qword_100662390 != -1 )
        swift_once(&qword_100662390, sub_1000D2180);
      v26 = qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ambientModePlacement;
      swift_beginAccess(
        qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ambientModePlacement,
        v71,
        0,
        0);
      v27 = *(_OWORD *)(v26 + 16);
      v72 = *(_OWORD *)v26;
      v73 = v27;
      v74 = *(_QWORD *)(v26 + 32);
      v28 = *((_QWORD *)&v72 + 1);
      v29 = (void *)v27;
      v75[0] = *(_OWORD *)(v26 + 24);
      v30 = objc_retain((id)v72);
      swift_retain(v28);
      v31 = objc_retain(v29);
      sub_10004542C((__int64)v75, (__int64)v70);
      v32 = sub_10003E4E0((__int64 *)&unk_10066AFA0, (__int64 *)&unk_10053BAB0);
      WrappedDefault.wrappedValue.getter(v70, v32);
      objc_release(v31);
      swift_release(v28);
      objc_release(v30);
      sub_10003F604(v75);
      objc_msgSend(v19, "setSelected:", v14 == v70[0]);
      objc_release(v19);
      objc_msgSend(v19, "setClipsToBounds:", 1);
      v33 = objc_retainAutoreleasedReturnValue(objc_msgSend(v19, "layer"));
      objc_msgSend(v33, "setCornerRadius:", 12.0);
      objc_release(v33);
      v34 = objc_retainAutoreleasedReturnValue(objc_msgSend(v19, "titleLabel"));
      if ( v34 )
      {
        v35 = v34;
        v36 = String._bridgeToObjectiveC()();
        v37 = objc_retainAutoreleasedReturnValue(objc_msgSend(v63, "fontWithName:size:", v36, 14.0));
        objc_release(v36);
        objc_msgSend(v35, "setFont:", v37);
        objc_release(v35);
        objc_release(v37);
      }
      v38 = *((_QWORD *)v16 - 1);
      v39 = *(_QWORD *)v16;
      swift_bridgeObjectRetain(*(_QWORD *)v16);
      sub_10024814C(v38, v39, 0);
      v41 = v40;
      swift_bridgeObjectRelease(v39);
      v42 = String._bridgeToObjectiveC()();
      swift_bridgeObjectRelease(v41);
      objc_msgSend(v19, "setTitle:forState:", v42, 0);
      objc_release(v42);
      objc_msgSend(v19, "addTarget:action:forControlEvents:", v1, v67, 64);
      v43 = objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "view"));
      if ( !v43 )
      {
        __break(1u);
        goto LABEL_16;
      }
      v44 = v43;
      objc_msgSend(v43, "addSubview:", v19);
      objc_release(v44);
      v61[2] = v14;
      ConstraintViewDSL.makeConstraints(_:)(sub_1001CD5C8, v61, v19);
      objc_release(v19);
      v45 = swift_beginAccess(&v1[v15], v70, 33, 0);
      specialized Array._makeUniqueAndReserveCapacityIfNotUnique()(v45);
      v47 = *(_QWORD *)((*(_QWORD *)&v1[v15] & 0xFFFFFFFFFFFFFF8LL) + 0x10);
      v46 = *(_QWORD *)((*(_QWORD *)&v1[v15] & 0xFFFFFFFFFFFFFF8LL) + 0x18);
      if ( v47 >= v46 >> 1 )
        specialized Array._createNewBuffer(bufferIsUnique:minimumCapacity:growForAppend:)(v46 > 1, v47 + 1, 1);
      ++v14;
      specialized Array._appendElementAssumeUniqueAndCapacity(_:newElement:)(v47, v19);
      swift_endAccess(v70);
      objc_release(v19);
      v16 += 16;
    }
    while ( v14 != 5 );
    v48 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UIButton), "init");
    sub_10024814C(0xD000000000000011LL, 0x80000001004DAD70LL, 0);
    v50 = v49;
    v51 = String._bridgeToObjectiveC()();
    swift_bridgeObjectRelease(v50);
    objc_msgSend(v48, "setTitle:forState:", v51, 0);
    objc_release(v51);
    v52 = String._bridgeToObjectiveC()();
    v53 = objc_retainAutoreleasedReturnValue(objc_msgSend(v13, "imageNamed:", v52));
    objc_release(v52);
    objc_msgSend(v48, "setBackgroundImage:forState:", v53, 0);
    objc_release(v53);
    v54 = objc_retainAutoreleasedReturnValue(objc_msgSend(v48, "titleLabel"));
    if ( v54 )
    {
      v55 = v54;
      v56 = String._bridgeToObjectiveC()();
      v57 = objc_retainAutoreleasedReturnValue(objc_msgSend(v63, "fontWithName:size:", v56, 14.0));
      objc_release(v56);
      objc_msgSend(v55, "setFont:", v57);
      objc_release(v55);
      objc_release(v57);
    }
    v58 = objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "view"));
    if ( v58 )
    {
      v59 = v58;
      swift_arrayDestroy(aBottomRight_0, 5, &type metadata for String);
      v60 = objc_retain(v48);
      objc_msgSend(v59, "addSubview:", v60);
      objc_release(v59);
      ConstraintViewDSL.makeConstraints(_:)(sub_1001CCD50, 0, v60);
      objc_release(v60);
      objc_msgSend(v60, "addTarget:action:forControlEvents:", v1, "exit", 1);
      objc_release(v62);
      objc_release(v60);
      return;
    }
  }
  else
  {
LABEL_16:
    __break(1u);
  }
  __break(1u);
}

/* ========================================================================
 * sub_1001CCB88
 * EA: 0x1001ccb88
 ======================================================================== */

__int64 __fastcall sub_1001CCB88(__int64 a1, __int64 a2)
{
  __int64 v4; // x20
  __int64 v5; // x0
  __int64 v6; // x0
  __int64 v7; // x20
  __int64 v8; // x0
  __int64 v9; // x0
  __int64 v10; // x20
  __int64 v11; // x0
  __int64 v12; // x0
  __int64 result; // x0
  __int64 v14; // x8
  bool v15; // vf
  __int64 v16; // x8
  __int64 v17; // x20
  __int64 v18; // x0
  _QWORD v19[3]; // [xsp+8h] [xbp-68h] BYREF
  void *v20; // [xsp+20h] [xbp-50h]
  void *v21; // [xsp+28h] [xbp-48h]

  v4 = (*(__int64 (**)(void))(*(_QWORD *)a1 + 144LL))();
  v20 = &type metadata for Int;
  v21 = &protocol witness table for Int;
  v19[0] = 180;
  v5 = (*(__int64 (__fastcall **)(_QWORD *, unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v4 + 104LL))(
         v19,
         0xD000000000000067LL,
         0x80000001004F2BA0LL,
         88);
  swift_release(v5);
  swift_release(v4);
  v6 = sub_10003F5DC(v19);
  v7 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)a1 + 152LL))(v6);
  v20 = &type metadata for Int;
  v21 = &protocol witness table for Int;
  v19[0] = 47;
  v8 = (*(__int64 (__fastcall **)(_QWORD *, unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v7 + 104LL))(
         v19,
         0xD000000000000067LL,
         0x80000001004F2BA0LL,
         89);
  swift_release(v8);
  swift_release(v7);
  v9 = sub_10003F5DC(v19);
  v10 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)a1 + 96LL))(v9);
  v20 = &type metadata for Int;
  v21 = &protocol witness table for Int;
  v19[0] = 16;
  v11 = (*(__int64 (__fastcall **)(_QWORD *, unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v10 + 104LL))(
          v19,
          0xD000000000000067LL,
          0x80000001004F2BA0LL,
          90);
  swift_release(v11);
  swift_release(v10);
  v12 = sub_10003F5DC(v19);
  result = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)a1 + 104LL))(v12);
  v14 = 47 * a2;
  if ( (unsigned __int128)(a2 * (__int128)47LL) >> 64 == (47 * a2) >> 63 )
  {
    v15 = __OFADD__(v14, 52);
    v16 = v14 + 52;
    if ( !v15 )
    {
      v17 = result;
      v20 = &type metadata for Int;
      v21 = &protocol witness table for Int;
      v19[0] = v16;
      v18 = (*(__int64 (__fastcall **)(_QWORD *, unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)result + 104LL))(
              v19,
              0xD000000000000067LL,
              0x80000001004F2BA0LL,
              91);
      swift_release(v18);
      swift_release(v17);
      return sub_10003F5DC(v19);
    }
  }
  else
  {
    __break(1u);
  }
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_1001CCED8
 * EA: 0x1001cced8
 ======================================================================== */

id __fastcall sub_1001CCED8(void *a1)
{
  __int64 v1; // x20
  __int64 v2; // x21
  id result; // x0
  unsigned __int64 v4; // x19
  __int64 v5; // x26
  __int64 v6; // x23
  __int64 v7; // x22
  __int64 v8; // x0
  __int64 v9; // x24
  void *v10; // x20
  id v11; // x23
  id v12; // x25
  __int64 v13; // x27
  __int64 v14; // x20
  __int64 v15; // x23
  void *v16; // x25
  id v17; // x24
  id v18; // x25
  __int64 v19; // x22
  __int64 Strong; // x0
  __int64 v21; // x20
  double v22; // d1
  __int64 v23; // x20
  __int64 v24; // x20
  unsigned __int64 i; // x22
  id v26; // x0
  void *v27; // x23
  unsigned __int64 v28; // x27
  __int64 v29; // x0
  _QWORD v30[3]; // [xsp+0h] [xbp-100h] BYREF
  char v31[24]; // [xsp+18h] [xbp-E8h] BYREF
  __int128 v32; // [xsp+30h] [xbp-D0h] BYREF
  __int128 v33; // [xsp+40h] [xbp-C0h]
  __int64 v34; // [xsp+50h] [xbp-B0h]
  __int128 v35; // [xsp+60h] [xbp-A0h] BYREF
  __int128 v36; // [xsp+70h] [xbp-90h]
  __int64 v37; // [xsp+80h] [xbp-80h]
  __int128 v38; // [xsp+90h] [xbp-70h] BYREF
  __int128 v39; // [xsp+A0h] [xbp-60h] BYREF

  v2 = v1;
  result = objc_msgSend(a1, "tag");
  if ( (unsigned __int64)result <= 4 )
  {
    v4 = (unsigned __int64)result;
    if ( qword_100662390 != -1 )
      goto LABEL_22;
    while ( 1 )
    {
      v5 = qword_100696D00;
      *(_QWORD *)&v32 = v4;
      v6 = qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ambientModePlacement;
      swift_beginAccess(
        qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ambientModePlacement,
        &v35,
        33,
        0);
      v7 = sub_10003E4E0((__int64 *)&unk_10066AFA0, (__int64 *)&unk_10053BAB0);
      WrappedDefault.wrappedValue.setter(&v32, v7);
      v8 = swift_endAccess(&v35);
      sub_10020558C(v8);
      v35 = *(_OWORD *)v6;
      v36 = *(_OWORD *)(v6 + 16);
      v37 = *(_QWORD *)(v6 + 32);
      v9 = *((_QWORD *)&v35 + 1);
      v10 = (void *)v36;
      v38 = *(_OWORD *)(v6 + 24);
      v11 = objc_retain((id)v35);
      swift_retain(v9);
      v12 = objc_retain(v10);
      sub_10004542C((__int64)&v38, (__int64)&v32);
      WrappedDefault.wrappedValue.getter(&v32, v7);
      objc_release(v12);
      swift_release(v9);
      objc_release(v11);
      sub_10003F604(&v38);
      v13 = v32;
      v14 = v5 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ambientModeScalePercent;
      swift_beginAccess(v5 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ambientModeScalePercent, v31, 0, 0);
      v32 = *(_OWORD *)v14;
      v33 = *(_OWORD *)(v14 + 16);
      v34 = *(_QWORD *)(v14 + 32);
      v15 = *((_QWORD *)&v32 + 1);
      v16 = (void *)v33;
      v39 = *(_OWORD *)(v14 + 24);
      v17 = objc_retain((id)v32);
      swift_retain(v15);
      v18 = objc_retain(v16);
      sub_10004542C((__int64)&v39, (__int64)v30);
      WrappedDefault.wrappedValue.getter(v30, v7);
      objc_release(v18);
      swift_release(v15);
      objc_release(v17);
      sub_10003F604(&v39);
      v19 = v30[0];
      Strong = swift_unknownObjectWeakLoadStrong(v2 + OBJC_IVAR____TtC15ExternalMonitor27VTPCAMSettingViewController_delegate);
      if ( Strong )
      {
        v21 = Strong;
        v22 = 0.4;
        if ( v13 != 4 )
          v22 = 0.0;
        sub_100142220(v4, v22 + (double)v19 / 100.0);
        swift_unknownObjectRelease(v21);
      }
      v23 = OBJC_IVAR____TtC15ExternalMonitor27VTPCAMSettingViewController_buttons;
      swift_beginAccess(v2 + OBJC_IVAR____TtC15ExternalMonitor27VTPCAMSettingViewController_buttons, v30, 0, 0);
      v24 = *(_QWORD *)(v2 + v23);
      if ( (unsigned __int64)v24 >> 62 )
      {
        v29 = v24 < 0 ? v24 : v24 & 0xFFFFFFFFFFFFFF8LL;
        v2 = _CocoaArrayWrapper.endIndex.getter(v29);
      }
      else
      {
        v2 = *(_QWORD *)((v24 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
      }
      swift_bridgeObjectRetain(v24);
      if ( !v2 )
        return (id)swift_bridgeObjectRelease(v24);
      for ( i = 0; ; ++i )
      {
        if ( (v24 & 0xC000000000000001LL) != 0 )
        {
          v26 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(i, v24);
        }
        else
        {
          if ( i >= *(_QWORD *)((v24 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
            goto LABEL_21;
          v26 = objc_retain(*(id *)(v24 + 8 * i + 32));
        }
        v27 = v26;
        v28 = i + 1;
        if ( __OFADD__(i, 1) )
          break;
        objc_msgSend(v26, "setSelected:", v4 == i);
        objc_release(v27);
        if ( v28 == v2 )
          return (id)swift_bridgeObjectRelease(v24);
      }
      __break(1u);
LABEL_21:
      __break(1u);
LABEL_22:
      swift_once(&qword_100662390, sub_1000D2180);
    }
  }
  return result;
}

/* ========================================================================
 * sub_1001CD6A0
 * EA: 0x1001cd6a0
 ======================================================================== */

id sub_1001CD6A0()
{
  void *v0; // x20
  id result; // x0
  void *v2; // x19
  id v3; // x22
  void *v4; // x19
  void *v5; // x21
  double v6; // d0
  double v7; // d8
  double v8; // d1
  double v9; // d9
  double v10; // d2
  double v11; // d10
  double v12; // d3
  double v13; // d11
  void *v14; // x21
  __int64 v15; // x0
  __int64 v16; // x23
  id v17; // x22
  Class isa; // x24
  Class v19; // x20
  id v20; // x19
  _QWORD v21[6]; // [xsp+0h] [xbp-80h] BYREF

  result = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "inputImage"));
  if ( !result )
  {
    __break(1u);
LABEL_9:
    __break(1u);
    return result;
  }
  v2 = result;
  v3 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CISampler), "initWithImage:", result);
  objc_release(v2);
  if ( qword_1006624C8 != -1 )
    swift_once(&qword_1006624C8, sub_1001CD658);
  v4 = (void *)qword_100670498;
  result = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "inputImage"));
  if ( !result )
    goto LABEL_9;
  v5 = result;
  objc_msgSend(result, "extent");
  v7 = v6;
  v9 = v8;
  v11 = v10;
  v13 = v12;
  objc_release(v5);
  v21[4] = j_nullsub_9;
  v21[5] = 0;
  v21[0] = _NSConcreteStackBlock;
  v21[1] = 1107296256;
  v21[2] = sub_1002171FC;
  v21[3] = &unk_1005C5600;
  v14 = _Block_copy(v21);
  v15 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
  v16 = swift_allocObject(v15, 96, 7);
  *(_OWORD *)(v16 + 16) = xmmword_10053C7B0;
  *(_QWORD *)(v16 + 56) = sub_10004B4D8(0, &qword_10066C278, &classRef_CISampler);
  *(_QWORD *)(v16 + 32) = v3;
  v17 = objc_retain(v3);
  isa = (Class)objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "ratio"));
  *(_QWORD *)(v16 + 88) = sub_10004B4D8(0, (unsigned __int64 *)&qword_100669DA0, &classRef_NSNumber);
  if ( !isa )
    isa = NSNumber.init(integerLiteral:)(1).super.super.isa;
  *(_QWORD *)(v16 + 64) = isa;
  v19 = Array._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v16);
  v20 = objc_retainAutoreleasedReturnValue(objc_msgSend(v4, "applyWithExtent:roiCallback:arguments:", v14, v19, v7, v9, v11, v13));
  objc_release(v19);
  objc_release(v17);
  _Block_release(v14);
  return v20;
}

/* ========================================================================
 * -[_TtC15ExternalMonitor16IntegratedFilter .cxx_destruct]
 * EA: 0x1001cd944
 ======================================================================== */

void __cdecl -[IntegratedFilter .cxx_destruct](_TtC15ExternalMonitor16IntegratedFilter *self, SEL a2)
{
  objc_release(*(id *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_inputImage));
  objc_release(*(id *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_ratio));
}

/* ========================================================================
 * sub_1001CD9F8
 * EA: 0x1001cd9f8
 ======================================================================== */

__int64 __fastcall sub_1001CD9F8(__int64 a1, __int64 a2, __int64 *a3, __int64 a4, __int64 a5)
{
  __int64 v7; // x23
  __int64 v8; // x25
  char *v9; // x21
  id v10; // x27
  NSString v11; // x28
  NSString v12; // x22
  id v13; // x26
  __int64 v14; // x26
  unsigned __int64 v15; // x1
  unsigned __int64 v16; // x27
  __int64 v17; // x20
  __int64 result; // x0
  __int64 v19; // [xsp+0h] [xbp-70h] BYREF
  __int64 v20; // [xsp+8h] [xbp-68h]
  __int64 v21; // [xsp+10h] [xbp-60h]

  v20 = a5;
  v21 = a4;
  v7 = type metadata accessor for URL(0);
  v8 = *(_QWORD *)(v7 - 8);
  v9 = (char *)&v19 - ((*(_QWORD *)(v8 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v10 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSBundle), "mainBundle"));
  v11 = String._bridgeToObjectiveC()();
  v12 = String._bridgeToObjectiveC()();
  v13 = objc_retainAutoreleasedReturnValue(objc_msgSend(v10, "URLForResource:withExtension:", v11, v12));
  objc_release(v10);
  objc_release(v11);
  objc_release(v12);
  if ( v13 )
  {
    static URL._unconditionallyBridgeFromObjectiveC(_:)(v13);
    objc_release(v13);
    (*(void (__fastcall **)(char *, char *, __int64))(v8 + 32))(v9, v9, v7);
    v14 = Data.init(contentsOf:options:)(v9, 0);
    v16 = v15;
    sub_10004B4D8(0, &qword_10066C270, &classRef_CIKernel);
    sub_10004B430(v14, v16);
    v17 = sub_1002183B0(a2, 0xE200000000000000LL, v14, v16);
    sub_100040E14(v14, v16);
    (*(void (__fastcall **)(char *, __int64))(v8 + 8))(v9, v7);
    result = sub_100040E14(v14, v16);
    *a3 = v17;
  }
  else
  {
    __break(1u);
    swift_unexpectedError(v9, "ExternalMonitor/VTFilters.swift", 31, 1, v20);
    __break(1u);
    result = swift_unexpectedError(v9, "ExternalMonitor/VTFilters.swift", 31, 1, v21);
    __break(1u);
  }
  return result;
}

/* ========================================================================
 * sub_1001CDC50
 * EA: 0x1001cdc50
 ======================================================================== */

id sub_1001CDC50()
{
  void *v0; // x20
  void *v1; // x21
  id result; // x0
  void *v3; // x20
  id v4; // x19
  void *v5; // x20
  void *v6; // x21
  double v7; // d0
  double v8; // d8
  double v9; // d1
  double v10; // d9
  double v11; // d2
  double v12; // d10
  double v13; // d3
  double v14; // d11
  void *v15; // x21
  __int64 v16; // x0
  __int64 v17; // x22
  id v18; // x19
  Class isa; // x23
  id v20; // x20
  _QWORD v21[6]; // [xsp+0h] [xbp-80h] BYREF

  v1 = v0;
  result = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "inputImage"));
  if ( result )
  {
    v3 = result;
    v4 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CISampler), "initWithImage:", result);
    objc_release(v3);
    if ( qword_1006624D8 != -1 )
      swift_once(&qword_1006624D8, sub_1001CD9E0);
    v5 = (void *)qword_1006704A0;
    result = objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "inputImage"));
    if ( result )
    {
      v6 = result;
      objc_msgSend(result, "extent");
      v8 = v7;
      v10 = v9;
      v12 = v11;
      v14 = v13;
      objc_release(v6);
      v21[4] = j_nullsub_9;
      v21[5] = 0;
      v21[0] = _NSConcreteStackBlock;
      v21[1] = 1107296256;
      v21[2] = sub_1002171FC;
      v21[3] = &unk_1005C55D8;
      v15 = _Block_copy(v21);
      v16 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
      v17 = swift_allocObject(v16, 64, 7);
      *(_OWORD *)(v17 + 16) = xmmword_10053B940;
      *(_QWORD *)(v17 + 56) = sub_10004B4D8(0, &qword_10066C278, &classRef_CISampler);
      *(_QWORD *)(v17 + 32) = v4;
      v18 = objc_retain(v4);
      isa = Array._bridgeToObjectiveC()().super.isa;
      swift_bridgeObjectRelease(v17);
      v20 = objc_retainAutoreleasedReturnValue(objc_msgSend(v5, "applyWithExtent:roiCallback:arguments:", v15, isa, v8, v10, v12, v14));
      objc_release(isa);
      objc_release(v18);
      _Block_release(v15);
      return v20;
    }
  }
  else
  {
    __break(1u);
  }
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_1001CDF0C
 * EA: 0x1001cdf0c
 ======================================================================== */

__int64 sub_1001CDF0C()
{
  __int64 v0; // x19
  __int64 v1; // x26
  char *v2; // x20
  char *v3; // x22
  id v4; // x23
  NSString v5; // x24
  NSString v6; // x25
  id v7; // x21
  __int64 v8; // x23
  unsigned __int64 v9; // x1
  unsigned __int64 v10; // x24
  __int64 v11; // x20
  __int64 result; // x0
  __int64 v13; // [xsp+0h] [xbp-50h] BYREF

  v0 = type metadata accessor for URL(0);
  v1 = *(_QWORD *)(v0 - 8);
  v2 = (char *)&v13 - ((*(_QWORD *)(v1 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v3 = v2;
  v4 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSBundle), "mainBundle"));
  v5 = String._bridgeToObjectiveC()();
  v6 = String._bridgeToObjectiveC()();
  v7 = objc_retainAutoreleasedReturnValue(objc_msgSend(v4, "URLForResource:withExtension:", v5, v6));
  objc_release(v4);
  objc_release(v5);
  objc_release(v6);
  if ( v7 )
  {
    static URL._unconditionallyBridgeFromObjectiveC(_:)(v7);
    objc_release(v7);
    (*(void (__fastcall **)(char *, char *, __int64))(v1 + 32))(v2, v2, v0);
    v8 = Data.init(contentsOf:options:)(v2, 0);
    v10 = v9;
    sub_10004B4D8(0, &qword_10066C270, &classRef_CIKernel);
    sub_10004B430(v8, v10);
    v11 = sub_1002183B0(4543589, 0xE300000000000000LL, v8, v10);
    sub_100040E14(v8, v10);
    (*(void (__fastcall **)(char *, __int64))(v1 + 8))(v3, v0);
    result = sub_100040E14(v8, v10);
    qword_1006704A8 = v11;
  }
  else
  {
    __break(1u);
    swift_unexpectedError(0, "ExternalMonitor/VTFilters.swift", 31, 1, 67);
    __break(1u);
    result = swift_unexpectedError(0, "ExternalMonitor/VTFilters.swift", 31, 1, 68);
    __break(1u);
  }
  return result;
}

/* ========================================================================
 * sub_1001CE154
 * EA: 0x1001ce154
 ======================================================================== */

id sub_1001CE154()
{
  void *v0; // x20
  void *v1; // x21
  id result; // x0
  void *v3; // x20
  id v4; // x19
  void *v5; // x20
  void *v6; // x21
  double v7; // d0
  double v8; // d8
  double v9; // d1
  double v10; // d9
  double v11; // d2
  double v12; // d10
  double v13; // d3
  double v14; // d11
  void *v15; // x21
  __int64 v16; // x0
  __int64 v17; // x22
  id v18; // x19
  Class isa; // x23
  id v20; // x20
  _QWORD v21[6]; // [xsp+0h] [xbp-80h] BYREF

  v1 = v0;
  result = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "inputImage"));
  if ( result )
  {
    v3 = result;
    v4 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CISampler), "initWithImage:", result);
    objc_release(v3);
    if ( qword_1006624E8 != -1 )
      swift_once(&qword_1006624E8, sub_1001CDF0C);
    v5 = (void *)qword_1006704A8;
    result = objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "inputImage"));
    if ( result )
    {
      v6 = result;
      objc_msgSend(result, "extent");
      v8 = v7;
      v10 = v9;
      v12 = v11;
      v14 = v13;
      objc_release(v6);
      v21[4] = j_nullsub_9;
      v21[5] = 0;
      v21[0] = _NSConcreteStackBlock;
      v21[1] = 1107296256;
      v21[2] = sub_1002171FC;
      v21[3] = &unk_1005C55B0;
      v15 = _Block_copy(v21);
      v16 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
      v17 = swift_allocObject(v16, 64, 7);
      *(_OWORD *)(v17 + 16) = xmmword_10053B940;
      *(_QWORD *)(v17 + 56) = sub_10004B4D8(0, &qword_10066C278, &classRef_CISampler);
      *(_QWORD *)(v17 + 32) = v4;
      v18 = objc_retain(v4);
      isa = Array._bridgeToObjectiveC()().super.isa;
      swift_bridgeObjectRelease(v17);
      v20 = objc_retainAutoreleasedReturnValue(objc_msgSend(v5, "applyWithExtent:roiCallback:arguments:", v15, isa, v8, v10, v12, v14));
      objc_release(isa);
      objc_release(v18);
      _Block_release(v15);
      return v20;
    }
  }
  else
  {
    __break(1u);
  }
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_1001CE458
 * EA: 0x1001ce458
 ======================================================================== */

id sub_1001CE458()
{
  void *v0; // x20
  void *v1; // x21
  id result; // x0
  void *v3; // x20
  id v4; // x19
  void *v5; // x20
  void *v6; // x21
  double v7; // d0
  double v8; // d8
  double v9; // d1
  double v10; // d9
  double v11; // d2
  double v12; // d10
  double v13; // d3
  double v14; // d11
  void *v15; // x21
  __int64 v16; // x0
  __int64 v17; // x22
  id v18; // x19
  Class isa; // x23
  id v20; // x20
  _QWORD v21[6]; // [xsp+0h] [xbp-80h] BYREF

  v1 = v0;
  result = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "inputImage"));
  if ( result )
  {
    v3 = result;
    v4 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CISampler), "initWithImage:", result);
    objc_release(v3);
    if ( qword_1006624F8 != -1 )
      swift_once(&qword_1006624F8, sub_1001CE410);
    v5 = (void *)qword_1006704B0;
    result = objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "inputImage"));
    if ( result )
    {
      v6 = result;
      objc_msgSend(result, "extent");
      v8 = v7;
      v10 = v9;
      v12 = v11;
      v14 = v13;
      objc_release(v6);
      v21[4] = j_nullsub_9;
      v21[5] = 0;
      v21[0] = _NSConcreteStackBlock;
      v21[1] = 1107296256;
      v21[2] = sub_1002171FC;
      v21[3] = &unk_1005C5588;
      v15 = _Block_copy(v21);
      v16 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
      v17 = swift_allocObject(v16, 64, 7);
      *(_OWORD *)(v17 + 16) = xmmword_10053B940;
      *(_QWORD *)(v17 + 56) = sub_10004B4D8(0, &qword_10066C278, &classRef_CISampler);
      *(_QWORD *)(v17 + 32) = v4;
      v18 = objc_retain(v4);
      isa = Array._bridgeToObjectiveC()().super.isa;
      swift_bridgeObjectRelease(v17);
      v20 = objc_retainAutoreleasedReturnValue(objc_msgSend(v5, "applyWithExtent:roiCallback:arguments:", v15, isa, v8, v10, v12, v14));
      objc_release(isa);
      objc_release(v18);
      _Block_release(v15);
      return v20;
    }
  }
  else
  {
    __break(1u);
  }
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_1001CE804
 * EA: 0x1001ce804
 ======================================================================== */

__int64 sub_1001CE804()
{
  __int64 v0; // x19
  __int64 v1; // x26
  char *v2; // x20
  char *v3; // x22
  id v4; // x23
  NSString v5; // x24
  NSString v6; // x25
  id v7; // x21
  __int64 v8; // x23
  unsigned __int64 v9; // x1
  unsigned __int64 v10; // x24
  __int64 v11; // x20
  __int64 result; // x0
  __int64 v13; // [xsp+0h] [xbp-50h] BYREF

  v0 = type metadata accessor for URL(0);
  v1 = *(_QWORD *)(v0 - 8);
  v2 = (char *)&v13 - ((*(_QWORD *)(v1 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v3 = v2;
  v4 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSBundle), "mainBundle"));
  v5 = String._bridgeToObjectiveC()();
  v6 = String._bridgeToObjectiveC()();
  v7 = objc_retainAutoreleasedReturnValue(objc_msgSend(v4, "URLForResource:withExtension:", v5, v6));
  objc_release(v4);
  objc_release(v5);
  objc_release(v6);
  if ( v7 )
  {
    static URL._unconditionallyBridgeFromObjectiveC(_:)(v7);
    objc_release(v7);
    (*(void (__fastcall **)(char *, char *, __int64))(v1 + 32))(v2, v2, v0);
    v8 = Data.init(contentsOf:options:)(v2, 0);
    v10 = v9;
    sub_10004B4D8(0, &qword_10066C270, &classRef_CIKernel);
    sub_10004B430(v8, v10);
    v11 = sub_1002183B0(0x7065446F69746172LL, 0xEA00000000006874LL, v8, v10);
    sub_100040E14(v8, v10);
    (*(void (__fastcall **)(char *, __int64))(v1 + 8))(v3, v0);
    result = sub_100040E14(v8, v10);
    qword_1006704B8 = v11;
  }
  else
  {
    __break(1u);
    swift_unexpectedError(0, "ExternalMonitor/VTFilters.swift", 31, 1, 120);
    __break(1u);
    result = swift_unexpectedError(0, "ExternalMonitor/VTFilters.swift", 31, 1, 121);
    __break(1u);
  }
  return result;
}

/* ========================================================================
 * sub_1001CEA58
 * EA: 0x1001cea58
 ======================================================================== */

id sub_1001CEA58()
{
  void *v0; // x20
  void *v1; // x19
  id result; // x0
  void *v3; // x21
  double v4; // d0
  double v5; // d8
  double v6; // d1
  double v7; // d9
  double v8; // d2
  double v9; // d10
  double v10; // d3
  double v11; // d11
  void *v12; // x21
  __int64 v13; // x0
  __int64 v14; // x22
  id v15; // x23
  Class isa; // x23
  Class v17; // x20
  id v18; // x19
  _QWORD v19[6]; // [xsp+0h] [xbp-80h] BYREF

  if ( qword_100662500 != -1 )
    swift_once(&qword_100662500, sub_1001CE804);
  v1 = (void *)qword_1006704B8;
  result = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "originalImage"));
  if ( !result )
  {
    __break(1u);
    goto LABEL_9;
  }
  v3 = result;
  objc_msgSend(result, "extent");
  v5 = v4;
  v7 = v6;
  v9 = v8;
  v11 = v10;
  objc_release(v3);
  v19[4] = j_nullsub_9;
  v19[5] = 0;
  v19[0] = _NSConcreteStackBlock;
  v19[1] = 1107296256;
  v19[2] = sub_1002171FC;
  v19[3] = &unk_1005C5560;
  v12 = _Block_copy(v19);
  v13 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
  v14 = swift_allocObject(v13, 96, 7);
  *(_OWORD *)(v14 + 16) = xmmword_10053C7B0;
  result = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "originalImage"));
  if ( !result )
  {
LABEL_9:
    __break(1u);
    return result;
  }
  v15 = result;
  *(_QWORD *)(v14 + 56) = sub_10004B4D8(0, &qword_10066C268, &classRef_CIImage);
  *(_QWORD *)(v14 + 32) = v15;
  isa = (Class)objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "ratio"));
  *(_QWORD *)(v14 + 88) = sub_10004B4D8(0, (unsigned __int64 *)&qword_100669DA0, &classRef_NSNumber);
  if ( !isa )
    isa = NSNumber.init(integerLiteral:)(1).super.super.isa;
  *(_QWORD *)(v14 + 64) = isa;
  v17 = Array._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v14);
  v18 = objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "applyWithExtent:roiCallback:arguments:", v12, v17, v5, v7, v9, v11));
  objc_release(v17);
  _Block_release(v12);
  return v18;
}

/* ========================================================================
 * sub_1001CF4F4
 * EA: 0x1001cf4f4
 ======================================================================== */

__int64 sub_1001CF4F4()
{
  __int64 v0; // x20
  id v1; // x20
  __int64 v2; // x0
  __int64 v3; // x19
  __int64 i; // x20
  unsigned __int64 v5; // x21
  id v6; // x0
  void *v7; // x25
  unsigned __int64 v8; // x24
  NSString v9; // x27
  id v10; // x26
  __int64 v12; // x0
  char v13; // [xsp+Fh] [xbp-91h] BYREF
  __int128 v14; // [xsp+10h] [xbp-90h] BYREF
  __int128 v15; // [xsp+20h] [xbp-80h]
  _OWORD v16[2]; // [xsp+30h] [xbp-70h] BYREF

  v1 = objc_retainAutoreleasedReturnValue(
         objc_msgSend(
           (id)objc_opt_self(&OBJC_CLASS___PHAssetResource),
           "assetResourcesForAsset:",
           *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor7VTAsset_asset)));
  v2 = sub_10004B4D8(0, &qword_10066F108, &classRef_PHAssetResource);
  v3 = static Array._unconditionallyBridgeFromObjectiveC(_:)(v1, v2);
  objc_release(v1);
  if ( (unsigned __int64)v3 >> 62 )
    goto LABEL_20;
  for ( i = *(_QWORD *)((v3 & 0xFFFFFFFFFFFFFF8LL) + 0x10); i; i = _CocoaArrayWrapper.endIndex.getter(v12) )
  {
    v5 = 0;
    while ( 1 )
    {
      if ( (v3 & 0xC000000000000001LL) != 0 )
      {
        v6 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(v5, v3);
      }
      else
      {
        if ( v5 >= *(_QWORD *)((v3 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
          goto LABEL_19;
        v6 = objc_retain(*(id *)(v3 + 8 * v5 + 32));
      }
      v7 = v6;
      v8 = v5 + 1;
      if ( __OFADD__(v5, 1) )
        break;
      v9 = String._bridgeToObjectiveC()();
      v10 = objc_retainAutoreleasedReturnValue(objc_msgSend(v7, "valueForKey:", v9));
      objc_release(v9);
      if ( v10 )
      {
        _bridgeAnyObjectToAny(_:)(&v14, v10);
        objc_release(v7);
        swift_unknownObjectRelease(v10);
      }
      else
      {
        objc_release(v7);
        v14 = 0u;
        v15 = 0u;
      }
      v16[0] = v14;
      v16[1] = v15;
      if ( *((_QWORD *)&v15 + 1) )
      {
        if ( (swift_dynamicCast(&v13, v16, (char *)&type metadata for Any + 8, &type metadata for Bool, 6) & 1) != 0
          && (v13 & 1) == 0 )
        {
          swift_bridgeObjectRelease(v3);
          return 1;
        }
      }
      else
      {
        sub_10005285C(v16, &unk_100669D50, &unk_10053BC30);
      }
      ++v5;
      if ( v8 == i )
        goto LABEL_24;
    }
    __break(1u);
LABEL_19:
    __break(1u);
LABEL_20:
    if ( v3 < 0 )
      v12 = v3;
    else
      v12 = v3 & 0xFFFFFFFFFFFFFF8LL;
  }
LABEL_24:
  swift_bridgeObjectRelease(v3);
  return 0;
}

/* ========================================================================
 * sub_1001CF800
 * EA: 0x1001cf800
 ======================================================================== */

__int64 __fastcall sub_1001CF800(void *a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5, __int64 a6, __int64 a7)
{
  __int64 v12; // x19
  char *v13; // x21
  __int64 v14; // x0
  char *v15; // x23
  void *v16; // x24
  _QWORD *v17; // x0
  void *v18; // x22
  _QWORD *v19; // x20
  id v20; // x0
  __int64 v21; // x0
  __int64 v22; // x25
  __int64 v23; // x20
  __int64 v24; // x0
  __int64 v26; // [xsp+0h] [xbp-A0h] BYREF
  __int64 v27; // [xsp+8h] [xbp-98h]
  __int64 v28; // [xsp+10h] [xbp-90h]
  __int64 v29; // [xsp+18h] [xbp-88h]
  _QWORD aBlock[5]; // [xsp+20h] [xbp-80h] BYREF
  _QWORD *v31; // [xsp+48h] [xbp-58h]

  v26 = a7;
  v12 = type metadata accessor for DispatchWorkItemFlags(0);
  v29 = *(_QWORD *)(v12 - 8);
  v13 = (char *)&v26 - ((*(_QWORD *)(v29 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v14 = type metadata accessor for DispatchQoS(0);
  v27 = *(_QWORD *)(v14 - 8);
  v28 = v14;
  v15 = (char *)&v26 - ((*(_QWORD *)(v27 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  sub_10004B4D8(0, (unsigned __int64 *)&qword_100668B30, &classRef_OS_dispatch_queue);
  v16 = (void *)static OS_dispatch_queue.main.getter();
  v17 = (_QWORD *)swift_allocObject(a5, 40, 7);
  v17[2] = a3;
  v17[3] = a4;
  v17[4] = a1;
  aBlock[4] = a6;
  v31 = v17;
  aBlock[0] = _NSConcreteStackBlock;
  aBlock[1] = 1107296256;
  aBlock[2] = sub_100040600;
  aBlock[3] = v26;
  v18 = _Block_copy(aBlock);
  v19 = v31;
  v20 = objc_retain(a1);
  swift_retain(a4);
  v21 = swift_release(v19);
  static DispatchQoS.unspecified.getter(v21);
  aBlock[0] = &_swiftEmptyArrayStorage;
  v22 = sub_10003EF70(
          &qword_100669D60,
          &type metadata accessor for DispatchWorkItemFlags,
          &protocol conformance descriptor for DispatchWorkItemFlags);
  v23 = sub_10003E4E0((__int64 *)&unk_100668B40, &qword_10053B8F0);
  v24 = sub_100040724();
  dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v23, v24, v12, v22);
  OS_dispatch_queue.async(group:qos:flags:execute:)(0, v15, v13, v18);
  _Block_release(v18);
  objc_release(v16);
  (*(void (__fastcall **)(char *, __int64))(v29 + 8))(v13, v12);
  return (*(__int64 (__fastcall **)(char *, __int64))(v27 + 8))(v15, v28);
}

/* ========================================================================
 * sub_1001CFBD0
 * EA: 0x1001cfbd0
 ======================================================================== */

__int64 __fastcall sub_1001CFBD0(__int64 a1, void (__fastcall *a2)(__int64, unsigned __int64), __int64 a3, __int64 a4)
{
  __n128 v6; // q0
  __int64 v7; // x0
  Swift::String v8; // x0
  void *object; // x23
  unsigned __int64 v10; // x20
  unsigned __int64 v11; // x23
  __int64 v12; // x0
  __int64 v13; // x0
  __n128 v14; // q0
  __int64 v16; // x21
  unsigned __int64 v17; // x1
  unsigned __int64 v18; // x23
  __int64 v19; // [xsp+0h] [xbp-50h] BYREF
  unsigned __int64 v20; // [xsp+8h] [xbp-48h]
  unsigned __int64 v21; // [xsp+10h] [xbp-40h]

  if ( a1 )
  {
    type metadata accessor for VTLogger(0);
    _StringGuts.grow(_:)(32);
    v6 = swift_bridgeObjectRelease(0xE000000000000000LL);
    v20 = 0xD00000000000001ELL;
    v21 = 0x80000001004F2E50LL;
    v19 = a1;
    swift_errorRetain(a1, v6);
    v7 = sub_10003E4E0(&qword_1006705F8, (__int64 *)&unk_100543560);
    v8._countAndFlagsBits = String.init<A>(describing:)(&v19, v7);
    object = v8._object;
    String.append(_:)(v8);
    swift_bridgeObjectRelease(object);
    v10 = v20;
    v11 = v21;
    v13 = static os_log_type_t.error.getter(v12);
    sub_1001D8B44(v13, v10, v11);
    v14 = swift_bridgeObjectRelease(v11);
    return ((__int64 (__fastcall *)(_QWORD, unsigned __int64, __n128))a2)(0, 0xF000000000000000LL, v14);
  }
  else
  {
    v16 = Data.init(contentsOf:options:)(a4, 0);
    v18 = v17;
    sub_10004B430(v16, v17);
    a2(v16, v18);
    sub_100040E14(v16, v18);
    return sub_100040E14(v16, v18);
  }
}

/* ========================================================================
 * sub_1001CFDE4
 * EA: 0x1001cfde4
 ======================================================================== */

_QWORD *__fastcall sub_1001CFDE4(__int64 a1)
{
  id v1; // x21
  __int64 v2; // x0
  __int64 v3; // x19
  __int64 i; // x28
  unsigned __int64 v5; // x22
  unsigned __int64 v6; // x27
  __int64 v7; // x23
  __n128 v8; // q0
  __int64 v9; // x26
  __int64 v10; // x0
  id v11; // x0
  void *v12; // x25
  unsigned __int64 v13; // x24
  __int64 v14; // x21
  id v15; // x26
  __int64 v16; // x1
  unsigned __int64 v17; // x19
  __int64 v18; // x27
  __int64 v19; // x1
  __int64 v20; // x20
  __int64 v21; // x0
  char v23; // w26
  __int64 v24; // x0
  unsigned __int64 v26; // [xsp+0h] [xbp-60h]

  v1 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___PHAssetResource), "assetResourcesForAsset:", a1));
  v2 = sub_10004B4D8(0, &qword_10066F108, &classRef_PHAssetResource);
  v3 = static Array._unconditionallyBridgeFromObjectiveC(_:)(v1, v2);
  objc_release(v1);
  if ( (unsigned __int64)v3 >> 62 )
    goto LABEL_21;
  for ( i = *(_QWORD *)((v3 & 0xFFFFFFFFFFFFFF8LL) + 0x10); i; i = _CocoaArrayWrapper.endIndex.getter(v24) )
  {
    v5 = 0;
    v6 = 0xEB00000000636965LL;
    v26 = v3 & 0xC000000000000001LL;
    v7 = v3 & 0xFFFFFFFFFFFFFF8LL;
    while ( 1 )
    {
      if ( v26 )
      {
        v11 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(v5, v3);
      }
      else
      {
        if ( v5 >= *(_QWORD *)(v7 + 16) )
          goto LABEL_20;
        v11 = objc_retain(*(id *)(v3 + 8 * v5 + 32));
      }
      v12 = v11;
      v13 = v5 + 1;
      if ( __OFADD__(v5, 1) )
        break;
      v14 = v3;
      v15 = objc_retainAutoreleasedReturnValue(objc_msgSend(v11, "uniformTypeIdentifier"));
      v17 = v6;
      v18 = static String._unconditionallyBridgeFromObjectiveC(_:)(v15, v16);
      v20 = v19;
      objc_release(v15);
      v21 = v18;
      v6 = v17;
      if ( v21 == 0x682E63696C627570LL && v20 == v17 )
      {
        v8 = swift_bridgeObjectRelease(v20);
      }
      else
      {
        v23 = _stringCompareWithSmolCheck(_:_:expecting:)(v21, v20, 0x682E63696C627570LL, v17, 0);
        v8 = swift_bridgeObjectRelease(v20);
        if ( (v23 & 1) == 0 )
        {
          objc_release(v12);
          goto LABEL_6;
        }
      }
      specialized ContiguousArray._makeUniqueAndReserveCapacityIfNotUnique()(v8);
      v9 = _swiftEmptyArrayStorage[2];
      specialized ContiguousArray._reserveCapacityAssumingUniqueBuffer(oldCount:)(v9);
      v10 = specialized ContiguousArray._appendElementAssumeUniqueAndCapacity(_:newElement:)(v9, v12);
      specialized ContiguousArray._endMutation()(v10);
LABEL_6:
      v3 = v14;
      ++v5;
      if ( v13 == i )
        goto LABEL_25;
    }
    __break(1u);
LABEL_20:
    __break(1u);
LABEL_21:
    if ( v3 < 0 )
      v24 = v3;
    else
      v24 = v3 & 0xFFFFFFFFFFFFFF8LL;
  }
LABEL_25:
  swift_bridgeObjectRelease(v3);
  return _swiftEmptyArrayStorage;
}

/* ========================================================================
 * sub_1001D0000
 * EA: 0x1001d0000
 ======================================================================== */

__int64 __fastcall sub_1001D0000(__int64 a1, __int64 a2, __int64 a3)
{
  __int64 v3; // x28
  char *v4; // x20
  __int64 v5; // x19
  __int64 v6; // x26
  __int64 v7; // x25
  id v8; // x24
  id v9; // x22
  __int64 v10; // x0
  __int64 v11; // x22
  __int64 v12; // x1
  __int64 v13; // x24
  __n128 v14; // q0
  void (__fastcall *v15)(char *, __int64, __n128); // x28
  id v16; // x21
  NSURL *v17; // x8
  void *v18; // x0
  void *v19; // x20
  __int64 v20; // x8
  __int64 v21; // x24
  __int64 v22; // x22
  __int64 v23; // x23
  void *v24; // x22
  __int64 v25; // x24
  id v26; // x25
  _QWORD *v28; // [xsp+0h] [xbp-B0h] BYREF
  __int64 v29; // [xsp+8h] [xbp-A8h]
  id v30; // [xsp+10h] [xbp-A0h]
  __int64 v31; // [xsp+18h] [xbp-98h]
  __int64 v32; // [xsp+20h] [xbp-90h]
  __int64 v33; // [xsp+28h] [xbp-88h]
  _QWORD aBlock[5]; // [xsp+30h] [xbp-80h] BYREF
  __int64 v35; // [xsp+58h] [xbp-58h]

  v31 = a2;
  v32 = a3;
  v33 = a1;
  v29 = type metadata accessor for UUID(0);
  v3 = *(_QWORD *)(v29 - 8);
  v4 = (char *)&v28 - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for URL(0);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = *(_QWORD *)(v6 + 64);
  v30 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___PHAssetResourceRequestOptions), "init");
  objc_msgSend(v30, "setNetworkAccessAllowed:", 1);
  v8 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSFileManager), "defaultManager"));
  v9 = objc_retainAutoreleasedReturnValue(objc_msgSend(v8, "temporaryDirectory"));
  objc_release(v8);
  static URL._unconditionallyBridgeFromObjectiveC(_:)(v9);
  objc_release(v9);
  v10 = UUID.init()();
  v11 = UUID.uuidString.getter(v10);
  v13 = v12;
  (*(void (__fastcall **)(char *, __int64))(v3 + 8))(v4, v29);
  URL.appendingPathComponent(_:)((_QWORD **)((char *)&v28 - ((v7 + 15) & 0xFFFFFFFFFFFFFFF0LL)), v11, v13);
  v14 = swift_bridgeObjectRelease(v13);
  v15 = *(void (__fastcall **)(char *, __int64, __n128))(v6 + 8);
  v15((char *)&v28 - ((v7 + 15) & 0xFFFFFFFFFFFFFFF0LL), v5, v14);
  v28 = (_QWORD **)((char *)&v28 - ((v7 + 15) & 0xFFFFFFFFFFFFFFF0LL));
  URL.appendingPathExtension(_:)(v28, 1667851624, 0xE400000000000000LL);
  ((void (__fastcall *)(char *, __int64))v15)((char *)&v28 - ((v7 + 15) & 0xFFFFFFFFFFFFFFF0LL), v5);
  v16 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___PHAssetResourceManager), "defaultManager"));
  URL._bridgeToObjectiveC()(v17);
  v19 = v18;
  (*(void (__fastcall **)(char *, char *, __int64))(v6 + 16))(
    (char *)&v28 - ((v7 + 15) & 0xFFFFFFFFFFFFFFF0LL),
    (char *)&v28 - ((v7 + 15) & 0xFFFFFFFFFFFFFFF0LL),
    v5);
  v20 = *(unsigned __int8 *)(v6 + 80);
  v21 = (v20 + 32) & ~v20;
  v22 = swift_allocObject(&unk_1005C5740, v21 + v7, v20 | 7);
  v23 = v32;
  *(_QWORD *)(v22 + 16) = v31;
  *(_QWORD *)(v22 + 24) = v23;
  (*(void (__fastcall **)(__int64, char *, __int64))(v6 + 32))(
    v22 + v21,
    (char *)&v28 - ((v7 + 15) & 0xFFFFFFFFFFFFFFF0LL),
    v5);
  aBlock[4] = sub_1001D0380;
  v35 = v22;
  aBlock[0] = _NSConcreteStackBlock;
  aBlock[1] = 1107296256;
  aBlock[2] = sub_1001BC60C;
  aBlock[3] = &unk_1005C5758;
  v24 = _Block_copy(aBlock);
  v25 = v35;
  v26 = objc_retain(v30);
  swift_retain(v23);
  swift_release(v25);
  objc_msgSend(v16, "writeDataForAssetResource:toFile:options:completionHandler:", v33, v19, v26, v24);
  _Block_release(v24);
  objc_release(v16);
  objc_release(v19);
  objc_release(v26);
  objc_release(v26);
  return ((__int64 (__fastcall *)(_QWORD *, __int64))v15)(v28, v5);
}

/* ========================================================================
 * sub_1001D03C0
 * EA: 0x1001d03c0
 ======================================================================== */

__int64 sub_1001D03C0()
{
  const __CFData *isa; // x20
  CGImageSource *v1; // x19
  signed __int64 Count; // x0
  signed __int64 v3; // x22
  Swift::String v4; // x0
  Swift::String v5; // x0
  void *object; // x22
  __int64 v7; // x0
  __int64 v8; // x0
  __int64 v9; // x0
  __int64 v10; // x0
  CGImageRef ImageAtIndex; // x0
  CGImageRef v13; // x20
  id v14; // x21
  CGImageRef v15; // x0
  CGImageRef v16; // x20
  CGImageRef v17; // x0
  CGImageRef v18; // x20
  CFDictionaryRef v19; // x0
  CFDictionaryRef v20; // x20
  __int64 v21; // x0
  __int64 v22; // x0
  __int64 v23; // x24
  __int64 v24; // x25
  __int64 v25; // x0
  __int64 v26; // x20
  __int64 v27; // x0
  char v28; // w1
  CFDictionaryRef v29; // x0
  CFDictionaryRef v30; // x20
  __int64 v31; // x0
  __int64 v32; // x0
  __int64 v33; // x1
  __int64 v34; // x20
  __int64 v35; // x0
  __int64 v36; // x1
  __int64 v37; // x24
  __int64 v38; // x25
  __int64 v39; // x0
  char v40; // w1
  __int64 v41; // x0
  bool v42; // w24
  __int64 v43; // x22
  void *v44; // x23
  Swift::String v45; // x0
  __int128 v46; // kr00_16
  __int64 v47; // x0
  __int64 v48; // x0
  __int64 v49; // x0
  signed __int64 v50; // x20
  __int64 v51; // x0
  signed __int64 v52; // x20
  __int64 v53; // x0
  char v54; // w1
  __int64 v55; // x0
  char v56; // w1
  float v57; // s8
  Swift::String v58; // x0
  __int64 v59; // x0
  __int128 v60; // kr10_16
  __int64 v61; // x0
  signed __int64 v62; // [xsp+8h] [xbp-88h] BYREF
  __int128 v63; // [xsp+10h] [xbp-80h] BYREF
  __int128 v64; // [xsp+20h] [xbp-70h]

  isa = Data._bridgeToObjectiveC()().super.isa;
  v1 = CGImageSourceCreateWithData(isa, nullptr);
  objc_release(isa);
  if ( !v1 )
  {
    v9 = type metadata accessor for VTLogger(0);
    v10 = static os_log_type_t.error.getter(v9);
    sub_1001D8B44(v10, 0xD00000000000001DLL, 0x80000001004F2E70LL);
    return 1;
  }
  Count = CGImageSourceGetCount(v1);
  if ( Count < 3 )
  {
    v3 = Count;
    type metadata accessor for VTLogger(0);
    *(_QWORD *)&v63 = 0;
    _StringGuts.grow(_:)(55);
    v4._countAndFlagsBits = 0xD000000000000035LL;
    v4._object = (void *)0x80000001004F2F30LL;
    String.append(_:)(v4);
    v62 = v3;
    v5._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                              &type metadata for Int,
                              &protocol witness table for Int);
    object = v5._object;
    String.append(_:)(v5);
    swift_bridgeObjectRelease(object);
    v8 = static os_log_type_t.error.getter(v7);
    sub_1001D8B44(v8, 0, 0xE000000000000000LL);
    objc_release(v1);
    swift_bridgeObjectRelease(0xE000000000000000LL);
    return 1;
  }
  ImageAtIndex = CGImageSourceCreateImageAtIndex(v1, 0, nullptr);
  if ( ImageAtIndex )
  {
    v13 = ImageAtIndex;
    v14 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UIImage), "initWithCGImage:", ImageAtIndex);
    objc_release(v13);
  }
  else
  {
    v14 = nullptr;
  }
  v15 = CGImageSourceCreateImageAtIndex(v1, 1u, nullptr);
  if ( v15 )
  {
    v16 = v15;
    objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UIImage), "initWithCGImage:", v15);
    objc_release(v16);
  }
  v17 = CGImageSourceCreateImageAtIndex(v1, 2u, nullptr);
  if ( v17 )
  {
    v18 = v17;
    objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UIImage), "initWithCGImage:", v17);
    objc_release(v18);
  }
  v19 = CGImageSourceCopyPropertiesAtIndex(v1, 0, nullptr);
  if ( !v19 )
    goto LABEL_24;
  v20 = v19;
  v21 = objc_opt_self(&OBJC_CLASS___NSDictionary);
  v22 = swift_dynamicCastObjCClass(v20, v21);
  if ( !v22 )
  {
    objc_release(v20);
    goto LABEL_24;
  }
  v23 = v22;
  *(_QWORD *)&v63 = 0;
  v24 = type metadata accessor for CFString(0);
  v25 = sub_10003EF70(&qword_100668020, type metadata accessor for CFString, &unk_10053ADD8);
  static Dictionary._conditionallyBridgeFromObjectiveC(_:result:)(
    v23,
    &v63,
    v24,
    (char *)&type metadata for Any + 8,
    v25);
  objc_release(v20);
  v26 = v63;
  if ( (_QWORD)v63 )
  {
    if ( *(_QWORD *)(v63 + 16) )
    {
      swift_bridgeObjectRetain(v63);
      v27 = sub_100107AEC(kCGImagePropertyOrientation);
      if ( (v28 & 1) != 0 )
      {
        sub_100047B64(*(_QWORD *)(v26 + 56) + 32 * v27, &v63);
        swift_bridgeObjectRelease_n(v26, 2);
        if ( *((_QWORD *)&v64 + 1) )
        {
          swift_dynamicCast(&v62, &v63, (char *)&type metadata for Any + 8, &type metadata for Int, 6);
          goto LABEL_24;
        }
LABEL_23:
        sub_10005285C(&v63, &unk_100669D50, &unk_10053BC30);
        goto LABEL_24;
      }
      swift_bridgeObjectRelease(v26);
    }
    v63 = 0u;
    v64 = 0u;
    swift_bridgeObjectRelease(v26);
    goto LABEL_23;
  }
LABEL_24:
  v29 = CGImageSourceCopyProperties(v1, nullptr);
  if ( !v29 )
    goto LABEL_31;
  v30 = v29;
  v31 = objc_opt_self(&OBJC_CLASS___NSDictionary);
  v32 = swift_dynamicCastObjCClass(v30, v31);
  if ( !v32 )
  {
    objc_release(v30);
    goto LABEL_31;
  }
  *(_QWORD *)&v63 = 0;
  static Dictionary._conditionallyBridgeFromObjectiveC(_:result:)(
    v32,
    &v63,
    &type metadata for String,
    (char *)&type metadata for Any + 8,
    &protocol witness table for String);
  objc_release(v30);
  v34 = v63;
  if ( !(_QWORD)v63 )
  {
LABEL_31:
    v63 = 0u;
    v64 = 0u;
LABEL_32:
    sub_10005285C(&v63, &unk_100669D50, &unk_10053BC30);
    goto LABEL_33;
  }
  v35 = static String._unconditionallyBridgeFromObjectiveC(_:)(kCGImagePropertyGroups, v33);
  v37 = v36;
  if ( *(_QWORD *)(v34 + 16) )
  {
    v38 = v35;
    swift_bridgeObjectRetain(v34);
    v39 = sub_100107AD8(v38, v37);
    if ( (v40 & 1) != 0 )
    {
      sub_100047B64(*(_QWORD *)(v34 + 56) + 32 * v39, &v63);
      swift_bridgeObjectRelease(v37);
      v41 = v34;
      goto LABEL_43;
    }
    swift_bridgeObjectRelease(v34);
  }
  v63 = 0u;
  v64 = 0u;
  v41 = v37;
LABEL_43:
  swift_bridgeObjectRelease(v41);
  swift_bridgeObjectRelease(v34);
  if ( !*((_QWORD *)&v64 + 1) )
    goto LABEL_32;
  v49 = sub_10003E4E0((__int64 *)&unk_10066F6B0, (__int64 *)&unk_1005427B0);
  if ( (swift_dynamicCast(&v62, &v63, (char *)&type metadata for Any + 8, v49, 6) & 1) != 0 )
  {
    v50 = v62;
    if ( *(_QWORD *)(v62 + 16) )
    {
      sub_100047B64(v62 + 32, &v63);
      swift_bridgeObjectRelease(v50);
      v51 = sub_10003E4E0(&qword_100668720, &qword_10053CF20);
      if ( (swift_dynamicCast(&v62, &v63, (char *)&type metadata for Any + 8, v51, 6) & 1) != 0 )
      {
        v52 = v62;
        if ( *(_QWORD *)(v62 + 16) )
        {
          swift_bridgeObjectRetain(v62);
          v53 = sub_100107AD8(0xD000000000000013LL, 0x80000001004F2EC0LL);
          if ( (v54 & 1) != 0 )
          {
            sub_100047B64(*(_QWORD *)(v52 + 56) + 32 * v53, &v63);
            swift_bridgeObjectRelease(v52);
            if ( (swift_dynamicCast(&v62, &v63, (char *)&type metadata for Any + 8, &type metadata for Int, 6) & 1) != 0 )
            {
              v42 = v62 == 2;
              goto LABEL_54;
            }
          }
          else
          {
            swift_bridgeObjectRelease(v52);
          }
        }
        v42 = 0;
LABEL_54:
        if ( *(_QWORD *)(v52 + 16) )
        {
          swift_bridgeObjectRetain(v52);
          v55 = sub_100107AD8(0xD00000000000001DLL, 0x80000001004F2EE0LL);
          if ( (v56 & 1) != 0 )
          {
            sub_100047B64(*(_QWORD *)(v52 + 56) + 32 * v55, &v63);
            swift_bridgeObjectRelease(v52);
            goto LABEL_59;
          }
          swift_bridgeObjectRelease(v52);
        }
        v63 = 0u;
        v64 = 0u;
LABEL_59:
        swift_bridgeObjectRelease(v52);
        if ( *((_QWORD *)&v64 + 1) )
        {
          if ( (swift_dynamicCast(&v62, &v63, (char *)&type metadata for Any + 8, &type metadata for Float, 6) & 1) != 0 )
          {
            v57 = *(float *)&v62;
            type metadata accessor for VTLogger(0);
            *(_QWORD *)&v63 = 0;
            *((_QWORD *)&v63 + 1) = 0xE000000000000000LL;
            _StringGuts.grow(_:)(35);
            v58._countAndFlagsBits = 0xD000000000000021LL;
            v58._object = (void *)0x80000001004F2F00LL;
            String.append(_:)(v58);
            v59 = Float.write<A>(to:)(
                    &v63,
                    &type metadata for DefaultStringInterpolation,
                    &protocol witness table for DefaultStringInterpolation,
                    v57);
            v60 = v63;
            v61 = static os_log_type_t.info.getter(v59);
            sub_1001D8B44(v61, v60, *((_QWORD *)&v60 + 1));
            swift_bridgeObjectRelease(*((_QWORD *)&v60 + 1));
          }
        }
        else
        {
          sub_10005285C(&v63, &unk_100669D50, &unk_10053BC30);
        }
        goto LABEL_34;
      }
    }
    else
    {
      swift_bridgeObjectRelease(v62);
    }
  }
LABEL_33:
  v42 = 0;
LABEL_34:
  *(_QWORD *)&v63 = 0;
  *((_QWORD *)&v63 + 1) = 0xE000000000000000LL;
  _StringGuts.grow(_:)(35);
  swift_bridgeObjectRelease(*((_QWORD *)&v63 + 1));
  *(_QWORD *)&v63 = 0xD000000000000021LL;
  *((_QWORD *)&v63 + 1) = 0x80000001004F2E90LL;
  if ( v42 )
    v43 = 1702195828;
  else
    v43 = 0x65736C6166LL;
  if ( v42 )
    v44 = (void *)0xE400000000000000LL;
  else
    v44 = (void *)0xE500000000000000LL;
  type metadata accessor for VTLogger(0);
  v45._countAndFlagsBits = v43;
  v45._object = v44;
  String.append(_:)(v45);
  swift_bridgeObjectRelease(v44);
  v46 = v63;
  v48 = static os_log_type_t.info.getter(v47);
  sub_1001D8B44(v48, v46, *((_QWORD *)&v46 + 1));
  swift_bridgeObjectRelease(*((_QWORD *)&v46 + 1));
  objc_release(v1);
  return (__int64)v14;
}

/* ========================================================================
 * sub_1001D0C28
 * EA: 0x1001d0c28
 ======================================================================== */

id __fastcall sub_1001D0C28(double a1, double a2, double a3, double a4)
{
  _BYTE *v4; // x20
  id v9; // x19
  void *v10; // x20
  id v11; // x19
  id v12; // x20
  id v13; // x20
  NSString v14; // x21
  id v15; // x22
  id v16; // x21
  id v17; // x21
  id v18; // x22
  __int64 v19; // x1
  __int64 v20; // x23
  NSString v21; // x24
  id v22; // x23
  NSString v23; // x23
  id v24; // x24
  id v25; // x22
  _QWORD v27[4]; // [xsp+0h] [xbp-90h] BYREF
  objc_super v28; // [xsp+20h] [xbp-70h] BYREF

  v4[OBJC_IVAR____TtC15ExternalMonitor17VTXboxDimmingView_shouldHideCurosr] = 0;
  v28.receiver = v4;
  v28.super_class = (Class)type metadata accessor for VTXboxDimmingView();
  v9 = objc_msgSendSuper2(&v28, "initWithFrame:", a1, a2, a3, a4);
  v10 = (void *)objc_opt_self(&OBJC_CLASS___UIColor);
  v11 = objc_retain(objc_retain(v9));
  v12 = objc_retainAutoreleasedReturnValue(objc_msgSend(v10, "blackColor"));
  objc_msgSend(v11, "setBackgroundColor:", v12);
  objc_release(v12);
  objc_msgSend(v11, "setUserInteractionEnabled:", 1);
  v13 = objc_msgSend(
          objc_allocWithZone((Class)&OBJC_CLASS___UITapGestureRecognizer),
          "initWithTarget:action:",
          v11,
          "onViewDidTapped:");
  objc_release(v11);
  objc_msgSend(v11, "addGestureRecognizer:", v13);
  v14 = String._bridgeToObjectiveC()();
  v15 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIImage), "imageNamed:", v14));
  objc_release(v14);
  v16 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UIImageView), "initWithImage:", v15);
  objc_release(v15);
  v17 = objc_retain(v16);
  objc_msgSend(v11, "addSubview:", v17);
  ConstraintViewDSL.makeConstraints(_:)(sub_1001D0F78, 0, v17);
  objc_release(v17);
  v18 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UILabel), "init");
  sub_10024814C(0x72206F7420706154LL, 0xED0000656D757365LL, 0);
  v20 = v19;
  v21 = String._bridgeToObjectiveC()();
  objc_msgSend(v18, "setText:", v21, swift_bridgeObjectRelease(v20).n128_f64[0]);
  objc_release(v21);
  v22 = objc_msgSend(
          objc_allocWithZone((Class)&OBJC_CLASS___UIColor),
          "initWithRed:green:blue:alpha:",
          0.635294118,
          0.701960784,
          0.8,
          1.0);
  objc_msgSend(v18, "setTextColor:", v22);
  objc_release(v22);
  v23 = String._bridgeToObjectiveC()();
  v24 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIFont), "fontWithName:size:", v23, 14.0));
  objc_release(v23);
  objc_msgSend(v18, "setFont:", v24);
  objc_release(v24);
  v25 = objc_retain(v18);
  objc_msgSend(v11, "addSubview:", v25);
  v27[2] = v17;
  ConstraintViewDSL.makeConstraints(_:)(sub_1001D1C4C, v27, v25);
  objc_release(v11);
  objc_release(v13);
  objc_release(v17);
  objc_release(v25);
  objc_release(v25);
  return v11;
}

/* ========================================================================
 * sub_1001D12F4
 * EA: 0x1001d12f4
 ======================================================================== */

void sub_1001D12F4()
{
  void *v0; // x20
  void *v1; // x21
  char *v2; // x19
  __int64 v3; // x0
  void *v4; // x20
  id v5; // x23
  id v6; // x22
  __int64 v7; // x0
  __int64 v8; // x0
  __int64 v9; // x24
  id v10; // x20
  id v11; // x22
  __int64 v12; // x0
  __int64 v13; // x0
  void *v14; // x8
  __int64 v15; // x23
  id v16; // x20
  __int64 v17; // x0
  __int64 v18; // x0
  __int64 v19; // x23
  __int64 v20; // x0
  __int64 v21; // x25
  id v22; // x21
  id v23; // x27
  __int64 v24; // x20
  __int64 v25; // x26
  _QWORD *v26; // x28
  __int64 v27; // x19
  id v28; // [xsp+0h] [xbp-60h] BYREF
  id v29; // [xsp+8h] [xbp-58h]

  v1 = v0;
  v2 = (char *)&v28
     - ((*(_QWORD *)(*(_QWORD *)(sub_10003E4E0((__int64 *)&unk_100668B60, &qword_10053BAE0) - 8) + 64LL) + 15LL)
      & 0xFFFFFFFFFFFFFFF0LL);
  objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSObject), "cancelPreviousPerformRequestsWithTarget:", v0);
  objc_msgSend(v0, "performSelector:withObject:afterDelay:", "show", 0, 7.0);
  type metadata accessor for BXCLogger(0);
  v3 = static os_log_type_t.debug.getter();
  sub_1001D8B44(v3, 0x776F68536F747561LL, 0xEA00000000002928LL);
  v4 = (void *)objc_opt_self(&OBJC_CLASS___UIApplication);
  v5 = objc_retainAutoreleasedReturnValue(objc_msgSend(v4, "sharedApplication"));
  v6 = objc_retainAutoreleasedReturnValue(objc_msgSend(v5, "keyWindow"));
  objc_release(v5);
  if ( v6 )
  {
    v29 = objc_retainAutoreleasedReturnValue(objc_msgSend(v6, "rootViewController"));
    objc_release(v6);
    if ( v29 )
    {
      v7 = type metadata accessor for VTTouchPanelViewController(0);
      v8 = swift_dynamicCastClass(v29, v7);
      if ( !v8
        || (v9 = v8, (sub_1000910D0() & 1) == 0)
        || (v10 = objc_retainAutoreleasedReturnValue(objc_msgSend(v4, "sharedApplication")),
            v11 = objc_retainAutoreleasedReturnValue(objc_msgSend(v10, "delegate")),
            objc_release(v10),
            !v11) )
      {
        objc_release(v29);
        return;
      }
      v12 = type metadata accessor for AppDelegate(0);
      v13 = swift_dynamicCastClass(v11, v12);
      if ( v13 && (v14 = *(void **)(v13 + OBJC_IVAR____TtC15ExternalMonitor11AppDelegate_window)) != nullptr )
      {
        v15 = qword_100662350;
        v16 = objc_retain(v14);
        if ( v15 != -1 )
          swift_once(&qword_100662350, sub_1000CB480);
        v28 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)qword_100696CC8, "activeVC"));
        if ( v28 )
        {
          v17 = type metadata accessor for VTWebViewContainerViewController(0);
          v18 = swift_dynamicCastClass(v28, v17);
          if ( v18 )
          {
            v19 = v18;
            v20 = type metadata accessor for TaskPriority(0);
            (*(void (__fastcall **)(char *, __int64, __int64, __int64))(*(_QWORD *)(v20 - 8) + 56LL))(v2, 1, 1, v20);
            v21 = swift_allocObject(&unk_1005C5830, 24, 7);
            swift_unknownObjectWeakInit(v21 + 16, v1);
            type metadata accessor for MainActor(0);
            v22 = objc_retain(v16);
            swift_retain(v21);
            v28 = objc_retain(v28);
            v23 = objc_retain(v29);
            v24 = static MainActor.shared.getter();
            v25 = sub_1001D1B38();
            v26 = (_QWORD *)swift_allocObject(&unk_1005C5858, 64, 7);
            v26[2] = v24;
            v26[3] = v25;
            v26[4] = v21;
            v26[5] = v19;
            v26[6] = v9;
            v26[7] = v22;
            swift_release(v21);
            v27 = sub_100087664(0, 0, v2, &unk_100543598, v26);
            objc_release(v23);
            swift_unknownObjectRelease(v11);
            swift_release(v27);
            objc_release(v22);
            objc_release(v28);
          }
          else
          {
            objc_release(v29);
            objc_release(v16);
            swift_unknownObjectRelease(v11);
            objc_release(v28);
          }
          return;
        }
        objc_release(v29);
        objc_release(v16);
      }
      else
      {
        objc_release(v29);
      }
      swift_unknownObjectRelease(v11);
    }
  }
}

/* ========================================================================
 * sub_1001D1C54
 * EA: 0x1001d1c54
 ======================================================================== */

_QWORD *__fastcall sub_1001D1C54(unsigned __int64 a1, double a2, double a3, double a4, double a5)
{
  _BYTE *v5; // x20
  __int64 v11; // x19
  __int64 v12; // x19
  __int64 v13; // x19
  void *v14; // x21
  __int64 v15; // x19
  _QWORD *v16; // x20
  void *v17; // x22
  id v18; // x25
  NSString v19; // x26
  void *v20; // x23
  id v21; // x27
  __int64 v22; // x0
  void *v23; // x28
  __int64 v24; // x19
  _QWORD *v25; // x24
  id v26; // x19
  id v27; // x25
  NSString v28; // x26
  id v29; // x27
  __int64 v30; // x0
  void *v31; // x19
  __int64 v32; // x28
  _QWORD *v33; // x24
  id v34; // x28
  id v35; // x22
  NSString v36; // x25
  id v37; // x23
  __int64 v38; // x0
  void *v39; // x19
  __int64 v40; // x26
  _QWORD *v41; // x0
  id v42; // x24
  _QWORD *v43; // x20
  id v44; // x19
  id v45; // x19
  id v46; // x19
  id v47; // x22
  __int64 v48; // x24
  void *v49; // x22
  id v50; // x19
  id v51; // x22
  id v52; // x19
  __int64 v53; // x21
  id v54; // x19
  double v55; // d0
  Class isa; // x19
  __int64 v57; // x23
  __int64 v58; // x24
  void *v59; // x0
  id v60; // x0
  __int64 v61; // x1
  void *v62; // x19
  __int64 v63; // x1
  __int64 v64; // x22
  Swift::String v65; // x0
  __int64 v66; // x8
  unsigned __int64 v68; // [xsp+0h] [xbp-C0h]
  char v69; // [xsp+Ch] [xbp-B4h]
  void **aBlock; // [xsp+10h] [xbp-B0h] BYREF
  __int64 v71; // [xsp+18h] [xbp-A8h]
  __int64 (__fastcall *v72)(); // [xsp+20h] [xbp-A0h]
  void *v73; // [xsp+28h] [xbp-98h]
  __int64 (__fastcall *v74)(); // [xsp+30h] [xbp-90h]
  __int64 v75; // [xsp+38h] [xbp-88h]
  objc_super v76; // [xsp+40h] [xbp-80h] BYREF

  v11 = OBJC_IVAR____TtC15ExternalMonitor9VTStepper_fgView;
  *(_QWORD *)&v5[v11] = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UIView), "init");
  v12 = OBJC_IVAR____TtC15ExternalMonitor9VTStepper_icon;
  *(_QWORD *)&v5[v12] = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___UIImageView), "init");
  *(_QWORD *)&v5[OBJC_IVAR____TtC15ExternalMonitor9VTStepper_gradientLayer] = 0;
  v13 = OBJC_IVAR____TtC15ExternalMonitor9VTStepper_incrementButton;
  v14 = (void *)objc_opt_self(&OBJC_CLASS___UIButton);
  *(_QWORD *)&v5[v13] = objc_retainAutoreleasedReturnValue(objc_msgSend(v14, "buttonWithType:", 0));
  v15 = OBJC_IVAR____TtC15ExternalMonitor9VTStepper_decrementButton;
  *(_QWORD *)&v5[v15] = objc_retainAutoreleasedReturnValue(objc_msgSend(v14, "buttonWithType:", 0));
  *(_QWORD *)&v5[OBJC_IVAR____TtC15ExternalMonitor9VTStepper_value] = 3;
  v5[OBJC_IVAR____TtC15ExternalMonitor9VTStepper_enabled] = 0;
  *(_QWORD *)&v5[OBJC_IVAR____TtC15ExternalMonitor9VTStepper_maxValue] = a1;
  v76.receiver = v5;
  v76.super_class = (Class)type metadata accessor for VTStepper();
  v16 = objc_msgSendSuper2(&v76, "initWithFrame:", a2, a3, a4, a5);
  v69 = (*(__int64 (**)(void))((swift_isaMask & *v16) + 0xE8LL))();
  if ( (v69 & 1) != 0 )
  {
    v17 = (void *)objc_opt_self(&OBJC_CLASS___NSNotificationCenter);
    v18 = objc_retainAutoreleasedReturnValue(objc_msgSend(v17, "defaultCenter"));
    v19 = String._bridgeToObjectiveC()();
    v20 = (void *)objc_opt_self(&OBJC_CLASS___NSOperationQueue);
    v68 = a1;
    v21 = objc_retainAutoreleasedReturnValue(objc_msgSend(v20, "mainQueue"));
    v22 = swift_allocObject(&unk_1005C58A0, 24, 7);
    *(_QWORD *)(v22 + 16) = v16;
    v74 = sub_1001D3AEC;
    v75 = v22;
    aBlock = _NSConcreteStackBlock;
    v71 = 1107296256;
    v72 = sub_100074E88;
    v73 = &unk_1005C58B8;
    v23 = _Block_copy(&aBlock);
    v24 = v75;
    v25 = objc_retain(v16);
    swift_release(v24);
    v26 = objc_retainAutoreleasedReturnValue(objc_msgSend(v18, "addObserverForName:object:queue:usingBlock:", v19, 0, v21, v23));
    _Block_release(v23);
    swift_unknownObjectRelease(v26);
    objc_release(v18);
    objc_release(v19);
    objc_release(v21);
    v27 = objc_retainAutoreleasedReturnValue(objc_msgSend(v17, "defaultCenter"));
    v28 = String._bridgeToObjectiveC()();
    v29 = objc_retainAutoreleasedReturnValue(objc_msgSend(v20, "mainQueue"));
    v30 = swift_allocObject(&unk_1005C58F0, 24, 7);
    *(_QWORD *)(v30 + 16) = v25;
    v74 = sub_1001D3B28;
    v75 = v30;
    aBlock = _NSConcreteStackBlock;
    v71 = 1107296256;
    v72 = sub_100074E88;
    v73 = &unk_1005C5908;
    v31 = _Block_copy(&aBlock);
    v32 = v75;
    v33 = objc_retain(v25);
    swift_release(v32);
    v34 = objc_retainAutoreleasedReturnValue(objc_msgSend(v27, "addObserverForName:object:queue:usingBlock:", v28, 0, v29, v31));
    _Block_release(v31);
    swift_unknownObjectRelease(v34);
    objc_release(v27);
    objc_release(v28);
    objc_release(v29);
    a1 = v68;
    v35 = objc_retainAutoreleasedReturnValue(objc_msgSend(v17, "defaultCenter"));
    v36 = String._bridgeToObjectiveC()();
    v37 = objc_retainAutoreleasedReturnValue(objc_msgSend(v20, "mainQueue"));
    v38 = swift_allocObject(&unk_1005C5940, 24, 7);
    *(_QWORD *)(v38 + 16) = v33;
    v74 = sub_1001D3B48;
    v75 = v38;
    aBlock = _NSConcreteStackBlock;
    v71 = 1107296256;
    v72 = sub_100074E88;
    v73 = &unk_1005C5958;
    v39 = _Block_copy(&aBlock);
    v40 = v75;
    v41 = objc_retain(v33);
    swift_release(v40);
    v42 = objc_retainAutoreleasedReturnValue(objc_msgSend(v35, "addObserverForName:object:queue:usingBlock:", v36, 0, v37, v39));
    _Block_release(v39);
    swift_unknownObjectRelease(v42);
    objc_release(v35);
    objc_release(v36);
    objc_release(v37);
  }
  v43 = objc_retain(v16);
  v44 = objc_retainAutoreleasedReturnValue(objc_msgSend(v43, "layer"));
  objc_msgSend(v44, "setMasksToBounds:", 1);
  objc_release(v44);
  v45 = objc_retainAutoreleasedReturnValue(objc_msgSend(v43, "layer"));
  objc_msgSend(v45, "setCornerRadius:", 24.0);
  objc_release(v45);
  v46 = objc_msgSend(
          objc_allocWithZone((Class)&OBJC_CLASS___UIColor),
          "initWithRed:green:blue:alpha:",
          0.211764706,
          0.243137255,
          0.290196078,
          1.0);
  v47 = objc_retainAutoreleasedReturnValue(objc_msgSend(v46, "colorWithAlphaComponent:", 0.9));
  objc_release(v46);
  objc_msgSend(v43, "setBackgroundColor:", v47);
  objc_release(v47);
  v48 = OBJC_IVAR____TtC15ExternalMonitor9VTStepper_fgView;
  v49 = *(void **)((char *)v43 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_fgView);
  v50 = objc_allocWithZone((Class)&OBJC_CLASS___UIColor);
  v51 = objc_retain(v49);
  v52 = objc_msgSend(v50, "initWithRed:green:blue:alpha:", 0.48627451, 0.57254902, 0.709803922, 1.0);
  objc_msgSend(v51, "setBackgroundColor:", v52);
  objc_release(v51);
  objc_release(v52);
  objc_msgSend(*(id *)((char *)v43 + v48), "setUserInteractionEnabled:", 0);
  objc_msgSend(v43, "addSubview:", *(_QWORD *)((char *)v43 + v48));
  v53 = OBJC_IVAR____TtC15ExternalMonitor9VTStepper_icon;
  objc_msgSend(v43, "addSubview:", *(_QWORD *)((char *)v43 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_icon));
  v54 = objc_retain(*(id *)((char *)v43 + v53));
  ConstraintViewDSL.makeConstraints(_:)(sub_1001D29EC, 0, v54);
  objc_release(v54);
  sub_1001D2438();
  v55 = vcvtd_n_f64_s64(a1, 1u);
  if ( (*(_QWORD *)&v55 & 0x7FFFFFFFFFFFFFFFuLL) > 0x7FEFFFFFFFFFFFFFLL )
  {
    __break(1u);
    goto LABEL_18;
  }
  if ( v55 <= -9.22337204e18 )
  {
LABEL_18:
    __break(1u);
    goto LABEL_19;
  }
  if ( v55 >= 9.22337204e18 )
  {
LABEL_19:
    __break(1u);
LABEL_20:
    swift_once(&qword_1006625A0, sub_100240480);
    goto LABEL_8;
  }
  *(_QWORD *)((char *)v43 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_value) = (__int64)v55;
  isa = Bool._bridgeToObjectiveC()().super.super.isa;
  swift_beginAccess(&unk_100672510, &aBlock, 32, 0);
  objc_setAssociatedObject(v43, &unk_100672510, isa, nullptr);
  swift_endAccess(&aBlock);
  objc_release(v43);
  objc_release(isa);
  if ( (v69 & 1) == 0 )
    return v43;
  if ( qword_1006625A0 != -1 )
    goto LABEL_20;
LABEL_8:
  v57 = qword_100697238;
  v58 = OBJC_IVAR____TtC15ExternalMonitor12VTBLEManager_connectedPeripheral;
  v59 = *(void **)(qword_100697238 + OBJC_IVAR____TtC15ExternalMonitor12VTBLEManager_connectedPeripheral);
  if ( v59 )
  {
    v60 = objc_retainAutoreleasedReturnValue(objc_msgSend(v59, "name"));
    if ( !v60 )
      goto LABEL_12;
    v62 = v60;
    static String._unconditionallyBridgeFromObjectiveC(_:)(v60, v61);
    v64 = v63;
    objc_release(v62);
    v65._countAndFlagsBits = 7693892;
    v65._object = (void *)0xE300000000000000LL;
    LOBYTE(v62) = String.hasPrefix(_:)(v65);
    swift_bridgeObjectRelease(v64);
    if ( ((unsigned __int8)v62 & 1) != 0 )
    {
      LOBYTE(v66) = 1;
    }
    else
    {
LABEL_12:
      v66 = *(_QWORD *)(v57 + v58);
      if ( v66 )
        LOBYTE(v66) = *(_BYTE *)(v57 + OBJC_IVAR____TtC15ExternalMonitor12VTBLEManager_receiveFirstACK);
    }
  }
  else
  {
    LOBYTE(v66) = 0;
  }
  *((_BYTE *)v43 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_enabled) = v66;
  return v43;
}

/* ========================================================================
 * sub_1001D26C8
 * EA: 0x1001d26c8
 ======================================================================== */

__int64 __fastcall sub_1001D26C8(__int64 a1, void *a2)
{
  __int64 v3; // x19
  char *v4; // x21
  __int64 v5; // x0
  char *v6; // x23
  __int64 v7; // x24
  __int64 v8; // x22
  char *v9; // x28
  char *v10; // x25
  void *v11; // x27
  void (__fastcall *v12)(char *, __int64); // x22
  __int64 v13; // x0
  void *v14; // x28
  __int64 v15; // x20
  id v16; // x0
  __int64 v17; // x0
  __int64 v18; // x0
  __int64 v19; // x26
  __int64 v20; // x20
  __int64 v21; // x0
  __int64 v23; // [xsp+0h] [xbp-A0h] BYREF
  __int64 v24; // [xsp+8h] [xbp-98h]
  __int64 v25; // [xsp+10h] [xbp-90h]
  __int64 v26; // [xsp+18h] [xbp-88h]
  _QWORD aBlock[5]; // [xsp+20h] [xbp-80h] BYREF
  __int64 v28; // [xsp+48h] [xbp-58h]

  v3 = type metadata accessor for DispatchWorkItemFlags(0);
  v26 = *(_QWORD *)(v3 - 8);
  v4 = (char *)&v23 - ((*(_QWORD *)(v26 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for DispatchQoS(0);
  v24 = *(_QWORD *)(v5 - 8);
  v25 = v5;
  v6 = (char *)&v23 - ((*(_QWORD *)(v24 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v7 = type metadata accessor for DispatchTime(0);
  v8 = *(_QWORD *)(v7 - 8);
  v9 = (char *)&v23 - ((*(_QWORD *)(v8 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v10 = v9;
  sub_100040668(0);
  v11 = (void *)static OS_dispatch_queue.main.getter();
  static DispatchTime.now()();
  + infix(_:_:)(v9, 0.5);
  v12 = *(void (__fastcall **)(char *, __int64))(v8 + 8);
  v12(v9, v7);
  v13 = swift_allocObject(&unk_1005C5990, 24, 7);
  *(_QWORD *)(v13 + 16) = a2;
  aBlock[4] = sub_1001D3B50;
  v28 = v13;
  aBlock[0] = _NSConcreteStackBlock;
  aBlock[1] = 1107296256;
  aBlock[2] = sub_100040600;
  aBlock[3] = &unk_1005C59A8;
  v14 = _Block_copy(aBlock);
  v15 = v28;
  v16 = objc_retain(a2);
  v17 = swift_release(v15);
  v18 = static DispatchQoS.unspecified.getter(v17);
  aBlock[0] = _swiftEmptyArrayStorage;
  v19 = sub_1000406E0(v18);
  v20 = sub_10003E4E0((__int64 *)&unk_100668B40, &qword_10053B8F0);
  v21 = sub_100040724();
  dispatch thunk of SetAlgebra.init<A>(_:)(aBlock, v20, v21, v3, v19);
  OS_dispatch_queue.asyncAfter(deadline:qos:flags:execute:)(v10, v6, v4, v14);
  _Block_release(v14);
  objc_release(v11);
  (*(void (__fastcall **)(char *, __int64))(v26 + 8))(v4, v3);
  (*(void (__fastcall **)(char *, __int64))(v24 + 8))(v6, v25);
  return ((__int64 (__fastcall *)(char *, __int64))v12)(v10, v7);
}

/* ========================================================================
 * sub_1001D2F20
 * EA: 0x1001d2f20
 ======================================================================== */

void sub_1001D2F20()
{
  char *v0; // x20
  unsigned __int64 v1; // x23
  __int64 v2; // x25
  char *v3; // x19
  __int64 v4; // x0
  __int64 inited; // x22
  id v6; // x20
  id v7; // x21
  id v8; // x21
  _QWORD *v9; // x26
  unsigned __int64 v10; // x8
  id v11; // x0
  void *v12; // x20
  id v13; // x24
  __int64 v14; // x23
  unsigned __int64 v15; // x8
  unsigned __int64 v16; // x27
  id v17; // x0
  void *v18; // x20
  id v19; // x22
  _QWORD *v20; // x20
  Class isa; // x22
  id v22; // x20
  void *v23; // x8
  _QWORD v24[3]; // [xsp+8h] [xbp-A8h] BYREF
  __int64 v25; // [xsp+20h] [xbp-90h]
  _QWORD *v26; // [xsp+28h] [xbp-88h]
  char v27[48]; // [xsp+30h] [xbp-80h] BYREF

  v2 = OBJC_IVAR____TtC15ExternalMonitor9VTStepper_gradientLayer;
  if ( !*(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor9VTStepper_gradientLayer] )
  {
    v3 = v0;
    v4 = sub_10003E4E0((__int64 *)&unk_100669910, &qword_10053C080);
    inited = swift_initStackObject(v4, v27);
    *(_OWORD *)(inited + 16) = xmmword_10053C050;
    *(_QWORD *)(inited + 32) = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIColor), "clearColor"));
    v6 = objc_msgSend(
           objc_allocWithZone((Class)&OBJC_CLASS___UIColor),
           "initWithRed:green:blue:alpha:",
           0.203921569,
           0.521568627,
           1.0,
           1.0);
    v7 = objc_retainAutoreleasedReturnValue(objc_msgSend(v6, "colorWithAlphaComponent:", 0.6));
    objc_release(v6);
    *(_QWORD *)(inited + 40) = v7;
    v8 = objc_retainAutoreleasedReturnValue(objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CAGradientLayer), "init"));
    objc_msgSend(v3, "bounds");
    objc_msgSend(v8, "setFrame:");
    v26 = _swiftEmptyArrayStorage;
    sub_10013076C(0, 2, 0);
    v9 = _swiftEmptyArrayStorage;
    if ( (inited & 0xC000000000000001LL) != 0 )
    {
      v11 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(0, inited);
    }
    else
    {
      v10 = *(_QWORD *)((inited & 0xFFFFFFFFFFFFFF8LL) + 0x10);
      if ( !v10 )
      {
        __break(1u);
LABEL_16:
        sub_10013076C(v10 > 1, inited, 1);
LABEL_12:
        v20 = v26;
        v26[2] = inited;
        sub_100075F34(v24, &v20[4 * v1 + 4]);
        isa = Array._bridgeToObjectiveC()().super.isa;
        swift_release(v20);
        objc_msgSend(v8, "setColors:", isa);
        objc_release(isa);
        objc_msgSend(v8, "setStartPoint:", 0.5, 0.0);
        objc_msgSend(v8, "setEndPoint:", 0.5, 1.0);
        v22 = objc_retainAutoreleasedReturnValue(objc_msgSend(v3, "layer"));
        objc_msgSend(v22, "insertSublayer:atIndex:", v8, 0);
        objc_release(v22);
        objc_msgSend(v8, "setHidden:", 1);
        objc_release(v8);
        v23 = *(void **)&v3[v2];
        *(_QWORD *)&v3[v2] = v8;
        objc_release(v23);
        return;
      }
      v11 = objc_retain(*(id *)(inited + 32));
    }
    v12 = v11;
    v13 = objc_retainAutoreleasedReturnValue(objc_msgSend(v11, "CGColor"));
    v14 = type metadata accessor for CGColor(0);
    v25 = v14;
    objc_release(v12);
    v24[0] = v13;
    v16 = _swiftEmptyArrayStorage[2];
    v15 = _swiftEmptyArrayStorage[3];
    if ( v16 >= v15 >> 1 )
    {
      sub_10013076C(v15 > 1, v16 + 1, 1);
      v9 = v26;
    }
    v9[2] = v16 + 1;
    sub_100075F34(v24, &v9[4 * v16 + 4]);
    if ( (inited & 0xC000000000000001LL) != 0 )
    {
      v17 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(1, inited);
    }
    else
    {
      if ( *(_QWORD *)((inited & 0xFFFFFFFFFFFFFF8LL) + 0x10) < 2u )
      {
        __break(1u);
        return;
      }
      v17 = objc_retain(*(id *)(inited + 40));
    }
    v18 = v17;
    v19 = objc_retainAutoreleasedReturnValue(objc_msgSend(v17, "CGColor", swift_bridgeObjectRelease(inited).n128_f64[0]));
    v25 = v14;
    objc_release(v18);
    v24[0] = v19;
    v1 = v9[2];
    v10 = v9[3];
    inited = v1 + 1;
    if ( v1 < v10 >> 1 )
      goto LABEL_12;
    goto LABEL_16;
  }
}

/* ========================================================================
 * sub_1001D34D8
 * EA: 0x1001d34d8
 ======================================================================== */

void sub_1001D34D8()
{
  _QWORD *v0; // x20
  __int64 v1; // x19
  __int64 v2; // x21
  __int64 v3; // x0
  __int64 v4; // x8
  double v5; // d0
  void *v6; // x8
  id v7; // x21
  id v8; // x0
  id v9; // x20
  id v10; // x0
  void *v11; // x19
  void *v12; // x8
  _QWORD v13[5]; // [xsp-20h] [xbp-60h] BYREF
  id v14; // [xsp+8h] [xbp-38h]

  if ( *((_BYTE *)v0 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_enabled) == 1 )
  {
    v1 = OBJC_IVAR____TtC15ExternalMonitor9VTStepper_value;
    v2 = *(_QWORD *)((char *)v0 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_value);
    if ( v2 < 1 )
      return;
    v3 = (*(__int64 (**)(void))((swift_isaMask & *v0) + 0xA8LL))();
    if ( !__OFSUB__(v2, v3) )
    {
      v4 = (v2 - v3) & ~((v2 - v3) >> 63);
      *(_QWORD *)((char *)v0 + v1) = v4;
      v5 = (double)(unsigned __int64)v4
         / (double)*(__int64 *)((char *)v0 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_maxValue);
      v6 = *(void **)((char *)v0 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_fgView);
      *(double *)&v13[2] = v5;
      v7 = objc_retain(v6);
      ConstraintViewDSL.remakeConstraints(_:)(sub_1001D3B7C, v13, v7);
      objc_release(v7);
      (*(void (**)(void))((swift_isaMask & *v0) + 0x130LL))();
      return;
    }
    __break(1u);
    goto LABEL_17;
  }
  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  if ( (*(_BYTE *)(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences_isP6s) & 1) == 0 )
  {
    if ( qword_100662540 == -1 )
    {
LABEL_9:
      if ( (*(_BYTE *)(qword_100697100 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_isR6Series) & 1) == 0 )
      {
        v8 = objc_retainAutoreleasedReturnValue(
               objc_msgSend(
                 *(id *)(qword_100697100 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_window),
                 "rootViewController"));
        if ( v8 )
        {
          v9 = objc_retain(v8);
          v10 = objc_retainAutoreleasedReturnValue(objc_msgSend(v9, "presentedViewController"));
          v11 = v9;
          if ( v10 )
          {
            v12 = v9;
            do
            {
              v11 = v10;
              objc_release(v12);
              v10 = objc_retainAutoreleasedReturnValue(objc_msgSend(v11, "presentedViewController"));
              v12 = v11;
            }
            while ( v10 );
          }
          objc_release(v9);
          v14 = objc_msgSend(objc_allocWithZone((Class)type metadata accessor for VTBuyPopViewController(0)), "init");
          objc_msgSend(v11, "presentViewController:animated:completion:", v14, 1, 0);
          objc_release(v11);
          objc_release(v14);
        }
      }
      return;
    }
LABEL_17:
    swift_once(&qword_100662540, sub_1001F0F34);
    goto LABEL_9;
  }
}

/* ========================================================================
 * sub_1001D373C
 * EA: 0x1001d373c
 ======================================================================== */

void sub_1001D373C()
{
  _QWORD *v0; // x20
  __int64 v1; // x19
  __int64 v2; // x23
  __int64 v3; // x21
  __int64 v4; // x0
  __int64 v5; // x8
  double v6; // d0
  void *v7; // x8
  id v8; // x21
  id v9; // x0
  id v10; // x20
  id v11; // x0
  void *v12; // x19
  void *v13; // x8
  _QWORD v14[5]; // [xsp-20h] [xbp-60h] BYREF
  id v15; // [xsp+8h] [xbp-38h]

  if ( *((_BYTE *)v0 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_enabled) == 1 )
  {
    v1 = OBJC_IVAR____TtC15ExternalMonitor9VTStepper_value;
    v2 = *(_QWORD *)((char *)v0 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_value);
    v3 = *(_QWORD *)((char *)v0 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_maxValue);
    if ( v2 >= v3 )
      return;
    v4 = (*(__int64 (**)(void))((swift_isaMask & *v0) + 0xA8LL))();
    v5 = v2 + v4;
    if ( !__OFADD__(v2, v4) )
    {
      if ( v5 >= v3 )
        v5 = v3;
      *(_QWORD *)((char *)v0 + v1) = v5;
      v6 = (double)v5 / (double)v3;
      v7 = *(void **)((char *)v0 + OBJC_IVAR____TtC15ExternalMonitor9VTStepper_fgView);
      *(double *)&v14[2] = v6;
      v8 = objc_retain(v7);
      ConstraintViewDSL.remakeConstraints(_:)(sub_100061F98, v14, v8);
      objc_release(v8);
      (*(void (**)(void))((swift_isaMask & *v0) + 0x130LL))();
      return;
    }
    __break(1u);
    goto LABEL_19;
  }
  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  if ( (*(_BYTE *)(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences_isP6s) & 1) == 0 )
  {
    if ( qword_100662540 == -1 )
    {
LABEL_11:
      if ( (*(_BYTE *)(qword_100697100 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_isR6Series) & 1) == 0 )
      {
        v9 = objc_retainAutoreleasedReturnValue(
               objc_msgSend(
                 *(id *)(qword_100697100 + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_window),
                 "rootViewController"));
        if ( v9 )
        {
          v10 = objc_retain(v9);
          v11 = objc_retainAutoreleasedReturnValue(objc_msgSend(v10, "presentedViewController"));
          v12 = v10;
          if ( v11 )
          {
            v13 = v10;
            do
            {
              v12 = v11;
              objc_release(v13);
              v11 = objc_retainAutoreleasedReturnValue(objc_msgSend(v12, "presentedViewController"));
              v13 = v12;
            }
            while ( v11 );
          }
          objc_release(v10);
          v15 = objc_msgSend(objc_allocWithZone((Class)type metadata accessor for VTBuyPopViewController(0)), "init");
          objc_msgSend(v12, "presentViewController:animated:completion:", v15, 1, 0);
          objc_release(v12);
          objc_release(v15);
        }
      }
      return;
    }
LABEL_19:
    swift_once(&qword_100662540, sub_1001F0F34);
    goto LABEL_11;
  }
}

/* ========================================================================
 * sub_1001D3B80
 * EA: 0x1001d3b80
 ======================================================================== */

_QWORD *__fastcall sub_1001D3B80(__int64 a1, __int64 a2)
{
  unsigned __int64 v4; // x0
  __int64 v5; // x23
  __int64 i; // x25
  unsigned __int64 v7; // x24
  __n128 v8; // q0
  __int64 v9; // x28
  __int64 v10; // x0
  id v11; // x0
  void *v12; // x27
  unsigned __int64 v13; // x19
  __int64 v14; // x0
  __int64 v15; // x0
  __int64 v16; // x1
  __int64 v17; // x20
  char v18; // w28
  _QWORD *v19; // x20
  __int64 v20; // x0
  __int64 ObjectType; // [xsp+10h] [xbp-60h] BYREF
  _QWORD *v23; // [xsp+18h] [xbp-58h]

  v4 = sub_1001D3D2C();
  v5 = v4;
  v23 = _swiftEmptyArrayStorage;
  if ( v4 >> 62 )
    goto LABEL_19;
  for ( i = *(_QWORD *)((v4 & 0xFFFFFFFFFFFFFF8LL) + 0x10); i; i = _CocoaArrayWrapper.endIndex.getter(v20) )
  {
    v7 = 0;
    while ( 1 )
    {
      if ( (v5 & 0xC000000000000001LL) != 0 )
      {
        v11 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(v7, v5);
      }
      else
      {
        if ( v7 >= *(_QWORD *)((v5 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
          goto LABEL_18;
        v11 = objc_retain(*(id *)(v5 + 8 * v7 + 32));
      }
      v12 = v11;
      v13 = v7 + 1;
      if ( __OFADD__(v7, 1) )
        break;
      ObjectType = swift_getObjectType(v11);
      v14 = sub_10003E4E0(&qword_100670698, (__int64 *)&unk_1005435C0);
      v15 = String.init<A>(describing:)(&ObjectType, v14);
      if ( v15 == a1 && v16 == a2 )
      {
        v8 = swift_bridgeObjectRelease(v16);
      }
      else
      {
        v17 = v16;
        v18 = _stringCompareWithSmolCheck(_:_:expecting:)(v15, v16, a1, a2, 0);
        v8 = swift_bridgeObjectRelease(v17);
        if ( (v18 & 1) == 0 )
        {
          objc_release(v12);
          goto LABEL_6;
        }
      }
      specialized ContiguousArray._makeUniqueAndReserveCapacityIfNotUnique()(v8);
      v9 = v23[2];
      specialized ContiguousArray._reserveCapacityAssumingUniqueBuffer(oldCount:)(v9);
      v10 = specialized ContiguousArray._appendElementAssumeUniqueAndCapacity(_:newElement:)(v9, v12);
      specialized ContiguousArray._endMutation()(v10);
LABEL_6:
      ++v7;
      if ( v13 == i )
      {
        v19 = v23;
        goto LABEL_24;
      }
    }
    __break(1u);
LABEL_18:
    __break(1u);
LABEL_19:
    if ( v5 < 0 )
      v20 = v5;
    else
      v20 = v5 & 0xFFFFFFFFFFFFFF8LL;
  }
  v19 = _swiftEmptyArrayStorage;
LABEL_24:
  swift_bridgeObjectRelease(v5);
  return v19;
}

/* ========================================================================
 * sub_1001D3D2C
 * EA: 0x1001d3d2c
 ======================================================================== */

__int64 sub_1001D3D2C()
{
  char *v0; // x20
  signed __int64 v1; // x19
  id v2; // x21
  char *v3; // x23
  unsigned __int64 v4; // x22
  __int64 v5; // x0
  __int64 v6; // x22
  id v7; // x21
  __int64 v8; // x24
  unsigned __int64 v9; // x24
  __int64 v10; // x9
  bool v11; // vf
  __int64 v12; // x27
  __int64 v13; // x26
  unsigned __int64 v14; // x28
  __int64 v15; // x22
  __int64 v16; // x0
  __int64 v17; // x28
  signed __int64 v18; // x9
  char **v19; // x22
  __int64 i; // x21
  char *v21; // x8
  __int64 v22; // x21
  void **v23; // x22
  char **v24; // x28
  char *v25; // t1
  char *v26; // x0
  char *v27; // x0
  __int64 v28; // x8
  char *v29; // x8
  __int64 v30; // x0
  void *v31; // x0
  void *v32; // x8
  __int64 v33; // x8
  __int64 v34; // x21
  __int64 v35; // x0
  __int64 v37; // [xsp+0h] [xbp-90h]
  signed __int64 v38; // [xsp+8h] [xbp-88h]
  char *v39; // [xsp+10h] [xbp-80h]
  __int64 v40; // [xsp+18h] [xbp-78h]
  signed __int64 v41; // [xsp+20h] [xbp-70h]
  __int64 v42; // [xsp+28h] [xbp-68h]
  unsigned __int64 v43; // [xsp+30h] [xbp-60h]

  v1 = (signed __int64)&selRef_setStatusBarHidden_;
  v2 = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "subviews"));
  v3 = (char *)sub_10004B4D8(0, &qword_100668530, &classRef_UIView);
  v4 = static Array._unconditionallyBridgeFromObjectiveC(_:)(v2, v3);
  objc_release(v2);
  v5 = v4;
  if ( v4 >> 62 )
    goto LABEL_69;
  v6 = *(_QWORD *)((v4 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
LABEL_3:
  swift_bridgeObjectRelease(v5);
  v7 = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, *(SEL *)(v1 + 1848)));
  v8 = static Array._unconditionallyBridgeFromObjectiveC(_:)(v7, v3);
  objc_release(v7);
  if ( v6 )
  {
    v0 = (char *)objc_retainAutoreleasedReturnValue(objc_msgSend(v0, *(SEL *)(v1 + 1848)));
    v1 = static Array._unconditionallyBridgeFromObjectiveC(_:)(v0, v3);
    objc_release(v0);
    if ( (unsigned __int64)v1 >> 62 )
    {
      if ( v1 < 0 )
        v35 = v1;
      else
        v35 = v1 & 0xFFFFFFFFFFFFFF8LL;
      v5 = _CocoaArrayWrapper.endIndex.getter(v35);
      v37 = v8;
      v38 = v1;
      if ( v5 )
        goto LABEL_6;
    }
    else
    {
      v5 = *(_QWORD *)((v1 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
      v37 = v8;
      v38 = v1;
      if ( v5 )
      {
LABEL_6:
        v39 = v3;
        v9 = 0;
        v42 = v1 & 0xFFFFFFFFFFFFFF8LL;
        v43 = v1 & 0xC000000000000001LL;
        v41 = v1 + 32;
        v40 = v5;
        do
        {
          if ( v43 )
          {
            v5 = specialized _ArrayBuffer._getElementSlowPath(_:)(v9, v38);
          }
          else
          {
            if ( v9 >= *(_QWORD *)(v42 + 16) )
              goto LABEL_62;
            v5 = (__int64)objc_retain(*(id *)(v41 + 8 * v9));
          }
          v0 = (char *)v5;
          v11 = __OFADD__(v9++, 1);
          if ( v11 )
          {
            __break(1u);
LABEL_62:
            __break(1u);
LABEL_63:
            __break(1u);
LABEL_64:
            __break(1u);
LABEL_65:
            __break(1u);
LABEL_66:
            __break(1u);
LABEL_67:
            __break(1u);
LABEL_68:
            __break(1u);
LABEL_69:
            v33 = v5 & 0xFFFFFFFFFFFFFF8LL;
            if ( v5 < 0 )
              v33 = v5;
            v34 = v5;
            v6 = _CocoaArrayWrapper.endIndex.getter(v33);
            v5 = v34;
            goto LABEL_3;
          }
          v12 = sub_1001D3D2C();
          objc_release(v0);
          if ( (unsigned __int64)v12 >> 62 )
          {
            if ( v12 < 0 )
              v30 = v12;
            else
              v30 = v12 & 0xFFFFFFFFFFFFFF8LL;
            v13 = _CocoaArrayWrapper.endIndex.getter(v30);
          }
          else
          {
            v13 = *(_QWORD *)((v12 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
          }
          v14 = (unsigned __int64)&_swiftEmptyArrayStorage >> 62;
          if ( (unsigned __int64)&_swiftEmptyArrayStorage >> 62 )
          {
            if ( (__int64)&_swiftEmptyArrayStorage < 0 )
              v31 = &_swiftEmptyArrayStorage;
            else
              v31 = (void *)((unsigned __int64)&_swiftEmptyArrayStorage & 0xFFFFFFFFFFFFFF8LL);
            v5 = _CocoaArrayWrapper.endIndex.getter(v31);
            v0 = (char *)(v5 + v13);
            if ( __OFADD__(v5, v13) )
              goto LABEL_63;
          }
          else
          {
            v5 = *(_QWORD *)(((unsigned __int64)&_swiftEmptyArrayStorage & 0xFFFFFFFFFFFFFF8LL) + 0x10);
            v0 = (char *)(v5 + v13);
            if ( __OFADD__(v5, v13) )
              goto LABEL_63;
          }
          LODWORD(v5) = swift_isUniquelyReferenced_nonNull_bridgeObject(&_swiftEmptyArrayStorage);
          if ( v14 )
            v5 = 0;
          else
            v5 = (unsigned int)v5;
          if ( (_DWORD)v5 != 1
            || (v1 = (unsigned __int64)&_swiftEmptyArrayStorage & 0xFFFFFFFFFFFFFF8LL,
                (__int64)v0 > *(_QWORD *)(((unsigned __int64)&_swiftEmptyArrayStorage & 0xFFFFFFFFFFFFFF8LL) + 0x18) >> 1) )
          {
            if ( v14 )
            {
              v32 = (void *)((unsigned __int64)&_swiftEmptyArrayStorage & 0xFFFFFFFFFFFFFF8LL);
              if ( (__int64)&_swiftEmptyArrayStorage < 0 )
                v32 = &_swiftEmptyArrayStorage;
              _CocoaArrayWrapper.endIndex.getter(v32);
            }
            v5 = specialized Array._createNewBuffer(bufferIsUnique:minimumCapacity:growForAppend:)();
            v1 = (unsigned __int64)&_swiftEmptyArrayStorage & 0xFFFFFFFFFFFFFF8LL;
          }
          v15 = *(_QWORD *)(v1 + 16);
          v3 = (char *)((*(_QWORD *)(v1 + 24) >> 1) - v15);
          if ( (unsigned __int64)v12 >> 62 )
          {
            if ( v12 < 0 )
              v0 = (char *)v12;
            else
              v0 = (char *)(v12 & 0xFFFFFFFFFFFFFF8LL);
            v16 = _CocoaArrayWrapper.endIndex.getter(v0);
            if ( !v16 )
            {
LABEL_7:
              v5 = swift_bridgeObjectRelease(v12);
              v10 = v40;
              if ( v13 > 0 )
                goto LABEL_64;
              continue;
            }
            v17 = v16;
            v5 = _CocoaArrayWrapper.endIndex.getter(v0);
            if ( (__int64)v3 < v5 )
              goto LABEL_66;
            if ( v17 < 1 )
              goto LABEL_68;
            v0 = (char *)v5;
            v18 = v1 + 8 * v15;
            v19 = (char **)(v18 + 32);
            if ( (v12 & 0xC000000000000001LL) != 0 )
            {
              for ( i = 0; i != v17; ++i )
                v19[i] = (char *)specialized _ArrayBuffer._getElementSlowPath(_:)(i, v12);
            }
            else
            {
              v21 = *(char **)(v12 + 32);
              *v19 = v21;
              v22 = v17 - 1;
              if ( v17 == 1 )
              {
                v3 = v21;
              }
              else
              {
                v23 = (void **)(v12 + 40);
                v24 = (char **)(v18 + 40);
                do
                {
                  v25 = (char *)*v23++;
                  v3 = v25;
                  *v24++ = v25;
                  v26 = objc_retain(v21);
                  v21 = v25;
                  --v22;
                }
                while ( v22 );
              }
              v27 = objc_retain(v3);
            }
          }
          else
          {
            v0 = *(char **)((v12 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
            if ( !v0 )
              goto LABEL_7;
            if ( (__int64)v3 < (__int64)v0 )
              goto LABEL_67;
            swift_arrayInitWithCopy(
              v1 + 8 * v15 + 32,
              (v12 & 0xFFFFFFFFFFFFFF8LL) + 32,
              *(_QWORD *)((v12 & 0xFFFFFFFFFFFFFF8LL) + 0x10),
              v39);
          }
          v5 = swift_bridgeObjectRelease(v12);
          v10 = v40;
          if ( (__int64)v0 < v13 )
            goto LABEL_64;
          if ( (__int64)v0 > 0 )
          {
            v28 = *(_QWORD *)(v1 + 16);
            v11 = __OFADD__(v28, v0);
            v29 = &v0[v28];
            if ( v11 )
              goto LABEL_65;
            *(_QWORD *)(v1 + 16) = v29;
          }
        }
        while ( v9 != v10 );
      }
    }
    swift_bridgeObjectRelease(v38);
    sub_1001D51C4(&_swiftEmptyArrayStorage);
    return v37;
  }
  return v8;
}

/* ========================================================================
 * sub_1001D4894
 * EA: 0x1001d4894
 ======================================================================== */

__int64 __fastcall sub_1001D4894(__int64 a1, unsigned __int64 a2)
{
  __int64 v4; // x0
  __int64 v5; // x22
  __int64 v6; // x23
  __int128 *v7; // x0
  __int64 v8; // x1
  char *v9; // x1
  __int64 v10; // x0
  unsigned __int64 v11; // x1
  __int64 v12; // x0
  __int64 v13; // x1
  unsigned __int64 v14; // x0
  unsigned __int64 v15; // x1
  unsigned __int64 v16; // x2
  __int64 v17; // x3
  unsigned __int64 v18; // x24
  unsigned __int64 v19; // x21
  unsigned __int64 v20; // x25
  __int64 v21; // x10
  __int64 v22; // x11
  __int64 v23; // x12
  bool v24; // vf
  __int64 v25; // x11
  __int64 v26; // x1
  unsigned __int64 v27; // x26
  unsigned int v28; // w28
  __int64 v29; // x8
  __int64 v30; // x22
  unsigned __int64 v31; // x23
  unsigned __int64 v32; // x0
  unsigned __int64 v33; // x20
  __int64 v34; // x0
  char v35; // w20
  __int128 v36; // kr10_16
  __int64 v38; // [xsp+8h] [xbp-108h]
  unsigned __int64 v39; // [xsp+10h] [xbp-100h]
  __int64 v40; // [xsp+18h] [xbp-F8h]
  __int64 v41; // [xsp+20h] [xbp-F0h]
  _OWORD v42[2]; // [xsp+40h] [xbp-D0h] BYREF
  __int64 v43; // [xsp+60h] [xbp-B0h]
  __int128 v44; // [xsp+70h] [xbp-A0h] BYREF
  __int128 v45; // [xsp+88h] [xbp-88h] BYREF
  __int64 v46; // [xsp+A0h] [xbp-70h]
  __int64 v47; // [xsp+A8h] [xbp-68h]

  *(_QWORD *)&v44 = a1;
  *((_QWORD *)&v44 + 1) = a2;
  swift_bridgeObjectRetain(a2);
  v4 = sub_10003E4E0(&qword_1006707E8, &qword_100543738);
  if ( (unsigned int)swift_dynamicCast(v42, &v44, &type metadata for String.UTF8View, v4, 6) )
  {
    sub_1001D7D34(v42, &v45);
    v5 = v46;
    v6 = v47;
    sub_10007BBAC(&v45, v46);
    dispatch thunk of ContiguousBytes.withUnsafeBytes<A>(_:)(
      &v44,
      sub_100245848,
      0,
      &type metadata for Data._Representation,
      v5,
      v6);
    v42[0] = v44;
    sub_10003F5DC(&v45);
    goto LABEL_66;
  }
  v43 = 0;
  memset(v42, 0, sizeof(v42));
  sub_10005285C(v42, &unk_1006707F0, &unk_100543740);
  if ( (a2 & 0x1000000000000000LL) != 0 )
    goto LABEL_70;
  if ( (a2 & 0x2000000000000000LL) != 0 )
  {
    *(_QWORD *)&v45 = a1;
    *((_QWORD *)&v45 + 1) = a2 & 0xFFFFFFFFFFFFFFLL;
    v7 = &v45;
    v9 = (char *)&v45 + (HIBYTE(a2) & 0xF);
  }
  else
  {
    if ( (a1 & 0x1000000000000000LL) != 0 )
    {
      v7 = (__int128 *)((a2 & 0xFFFFFFFFFFFFFFFLL) + 32);
      v8 = a1 & 0xFFFFFFFFFFFFLL;
    }
    else
    {
      v7 = (__int128 *)_StringObject.sharedUTF8.getter(a1, a2);
    }
    if ( v7 )
      v9 = (char *)v7 + v8;
    else
      v9 = nullptr;
  }
  v10 = sub_100245BB8(v7, v9);
  if ( v11 >> 60 != 15 )
  {
    *(_QWORD *)&v42[0] = v10;
    *((_QWORD *)&v42[0] + 1) = v11;
    goto LABEL_66;
  }
  v39 = v11;
  v38 = v10;
  if ( (a2 & 0x2000000000000000LL) != 0 )
    v12 = HIBYTE(a2) & 0xF;
  else
    v12 = a1 & 0xFFFFFFFFFFFFLL;
LABEL_15:
  *(_QWORD *)&v42[0] = sub_1001AADD0(v12);
  *((_QWORD *)&v42[0] + 1) = v13;
  v14 = sub_1001D73EC(sub_1001D7CDC);
  v18 = v14;
  v19 = v15;
  v20 = v16;
  v21 = *((_QWORD *)&v42[0] + 1) >> 62;
  if ( (int)(*((_QWORD *)&v42[0] + 1) >> 62) > 1 )
  {
    if ( (_DWORD)v21 == 2 )
    {
      v23 = *(_QWORD *)(*(_QWORD *)&v42[0] + 16LL);
      v22 = *(_QWORD *)(*(_QWORD *)&v42[0] + 24LL);
      v24 = __OFSUB__(v22, v23);
      v25 = v22 - v23;
      if ( v24 )
        goto LABEL_73;
      if ( v17 != v25 )
        goto LABEL_26;
    }
    else if ( v17 )
    {
      v26 = 0;
      goto LABEL_63;
    }
  }
  else if ( (_DWORD)v21 )
  {
    if ( __OFSUB__(DWORD1(v42[0]), v42[0]) )
      goto LABEL_74;
    if ( v17 != DWORD1(v42[0]) - LODWORD(v42[0]) )
    {
LABEL_26:
      if ( (_DWORD)v21 == 2 )
      {
        v26 = *(_QWORD *)(*(_QWORD *)&v42[0] + 24LL);
      }
      else if ( (_DWORD)v21 == 1 )
      {
        v26 = *(__int64 *)&v42[0] >> 32;
      }
      else
      {
        v26 = BYTE14(v42[0]);
      }
LABEL_63:
      if ( v26 >= v17 )
      {
        Data._Representation.replaceSubrange(_:with:count:)(v17);
LABEL_65:
        swift_bridgeObjectRelease(v19);
        goto LABEL_66;
      }
      __break(1u);
LABEL_73:
      __break(1u);
LABEL_74:
      __break(1u);
    }
  }
  else if ( v17 != BYTE14(v42[0]) )
  {
    goto LABEL_26;
  }
  if ( (v15 & 0x2000000000000000LL) != 0 )
    v27 = HIBYTE(v15) & 0xF;
  else
    v27 = v14 & 0xFFFFFFFFFFFFLL;
  *(_QWORD *)((char *)&v44 + 7) = 0;
  *(_QWORD *)&v44 = 0;
  if ( 4 * v27 == v16 >> 14 )
    goto LABEL_60;
  LOBYTE(v28) = 0;
  v29 = (v14 >> 59) & 1;
  if ( (v15 & 0x1000000000000000LL) == 0 )
    LOBYTE(v29) = 1;
  v30 = 4LL << v29;
  v40 = (v15 & 0xFFFFFFFFFFFFFFFLL) + 32;
  v41 = v15 & 0xFFFFFFFFFFFFFFLL;
  do
  {
    v31 = v20 & 0xC;
    v32 = v20;
    if ( v31 == v30 )
      v32 = sub_10010E880(v20, v18, v19);
    v33 = v32 >> 16;
    if ( v32 >> 16 >= v27 )
    {
      __break(1u);
LABEL_68:
      __break(1u);
LABEL_69:
      __break(1u);
LABEL_70:
      v12 = String.UTF8View._foreignCount()();
      v38 = 0;
      v39 = 0xF000000000000000LL;
      goto LABEL_15;
    }
    if ( (v19 & 0x1000000000000000LL) != 0 )
    {
      v35 = String.UTF8View._foreignSubscript(position:)();
      if ( v31 != v30 )
        goto LABEL_49;
    }
    else if ( (v19 & 0x2000000000000000LL) != 0 )
    {
      *(_QWORD *)&v45 = v18;
      *((_QWORD *)&v45 + 1) = v41;
      v35 = *((_BYTE *)&v45 + v33);
      if ( v31 != v30 )
        goto LABEL_49;
    }
    else
    {
      v34 = v40;
      if ( (v18 & 0x1000000000000000LL) == 0 )
        v34 = _StringObject.sharedUTF8.getter(v18, v19);
      v35 = *(_BYTE *)(v34 + v33);
      if ( v31 != v30 )
      {
LABEL_49:
        if ( (v19 & 0x1000000000000000LL) == 0 )
          goto LABEL_50;
        goto LABEL_53;
      }
    }
    v20 = sub_10010E880(v20, v18, v19);
    if ( (v19 & 0x1000000000000000LL) == 0 )
    {
LABEL_50:
      v20 = (v20 & 0xFFFFFFFFFFFF0000LL) + 65540;
      goto LABEL_55;
    }
LABEL_53:
    if ( v27 <= v20 >> 16 )
      goto LABEL_69;
    v20 = String.UTF8View._foreignIndex(after:)(v20, v18, v19);
LABEL_55:
    *((_BYTE *)&v44 + (unsigned __int8)v28) = v35;
    v28 = (unsigned __int8)v28 + 1;
    if ( ((v28 >> 8) & 1) != 0 )
      goto LABEL_68;
    if ( (unsigned __int8)v28 == 14 )
    {
      *(_QWORD *)&v45 = v44;
      *(_QWORD *)((char *)&v45 + 6) = *(_QWORD *)((char *)&v44 + 6);
      Data._Representation.append(contentsOf:)(&v45, (char *)&v45 + 14);
      LOBYTE(v28) = 0;
    }
  }
  while ( 4 * v27 != v20 >> 14 );
  if ( (_BYTE)v28 )
  {
    *(_QWORD *)&v45 = v44;
    *(_QWORD *)((char *)&v45 + 6) = *(_QWORD *)((char *)&v44 + 6);
    Data._Representation.append(contentsOf:)(&v45, (char *)&v45 + (unsigned __int8)v28);
    sub_1000509CC(v38, v39);
    goto LABEL_65;
  }
LABEL_60:
  swift_bridgeObjectRelease(v19);
  sub_1000509CC(v38, v39);
LABEL_66:
  v36 = v42[0];
  sub_10004B430(*(__int64 *)&v42[0], *((unsigned __int64 *)&v42[0] + 1));
  swift_bridgeObjectRelease(a2);
  sub_100040E14(v36, *((unsigned __int64 *)&v36 + 1));
  return v36;
}

/* ========================================================================
 * sub_1001D4D98
 * EA: 0x1001d4d98
 ======================================================================== */

__int64 __fastcall sub_1001D4D98(__int64 a1, __int64 a2, unsigned int a3, __int64 a4)
{
  __int64 v8; // x3
  unsigned __int64 v9; // x8
  unsigned __int64 v10; // x24
  signed __int64 v11; // x9
  unsigned __int64 v12; // x23
  signed __int64 v13; // x24
  unsigned int v14; // w25
  __int64 v15; // x23
  __int64 v16; // x24
  size_t v17; // x0
  __int64 v18; // x24
  size_t v19; // x0
  _QWORD *v20; // x24
  size_t v21; // x0
  __int64 v22; // x22
  size_t v23; // x0
  __int64 v24; // x21
  size_t v25; // x0
  __int64 v26; // x0
  Swift::String v28; // x0
  Swift::String v29; // x0
  void *object; // x21
  Swift::String v31; // x0
  __int64 v32; // x0
  __int64 v33; // x0
  __int64 v34; // [xsp+20h] [xbp-60h]

  v8 = sub_100248D90(0, 1, 1, _swiftEmptyArrayStorage);
  v10 = *(_QWORD *)(v8 + 16);
  v9 = *(_QWORD *)(v8 + 24);
  v11 = v9 >> 1;
  v12 = v10 + 1;
  if ( v9 >> 1 <= v10 )
  {
    v8 = sub_100248D90(v9 > 1, v10 + 1, 1, v8);
    v9 = *(_QWORD *)(v8 + 24);
    v11 = v9 >> 1;
  }
  *(_QWORD *)(v8 + 16) = v12;
  *(_BYTE *)(v8 + v10 + 32) = BYTE1(a1);
  v13 = v10 + 2;
  if ( v11 < v13 )
    v8 = sub_100248D90(v9 > 1, v13, 1, v8);
  v14 = HIWORD(a3);
  *(_QWORD *)(v8 + 16) = v13;
  *(_BYTE *)(v8 + v12 + 32) = BYTE2(a1);
  v34 = v8;
  v15 = sub_10003E4E0((__int64 *)&unk_100672970, (__int64 *)&unk_100542070);
  v16 = swift_allocObject(v15, 34, 7);
  v17 = malloc_size((const void *)v16);
  *(_QWORD *)(v16 + 16) = 2;
  *(_QWORD *)(v16 + 24) = 2 * v17 - 64;
  *(_WORD *)(v16 + 32) = WORD2(a1);
  sub_1001D50DC(v16);
  v18 = swift_allocObject(v15, 34, 7);
  v19 = malloc_size((const void *)v18);
  *(_QWORD *)(v18 + 16) = 2;
  *(_QWORD *)(v18 + 24) = 2 * v19 - 64;
  *(_WORD *)(v18 + 32) = HIWORD(a1);
  sub_1001D50DC(v18);
  v20 = (_QWORD *)swift_allocObject(v15, 40, 7);
  v21 = malloc_size(v20);
  v20[2] = 8;
  v20[3] = 2 * v21 - 64;
  v20[4] = a2;
  sub_1001D50DC(v20);
  v22 = swift_allocObject(v15, 34, 7);
  v23 = malloc_size((const void *)v22);
  *(_QWORD *)(v22 + 16) = 2;
  *(_QWORD *)(v22 + 24) = 2 * v23 - 64;
  *(_WORD *)(v22 + 32) = a3;
  sub_1001D50DC(v22);
  v24 = swift_allocObject(v15, 34, 7);
  v25 = malloc_size((const void *)v24);
  *(_QWORD *)(v24 + 16) = 2;
  *(_QWORD *)(v24 + 24) = 2 * v25 - 64;
  *(_WORD *)(v24 + 32) = v14;
  sub_1001D50DC(v24);
  v26 = swift_bridgeObjectRetain(a4);
  sub_1001D50DC(v26);
  if ( *(_QWORD *)(v34 + 16) == 64 )
    return sub_1001D4764(v34);
  type metadata accessor for VTLogger(0);
  _StringGuts.grow(_:)(66);
  v28._object = (void *)0x80000001004F31C0LL;
  v28._countAndFlagsBits = 0xD000000000000015LL;
  String.append(_:)(v28);
  swift_bridgeObjectRelease(v34);
  v29._countAndFlagsBits = dispatch thunk of CustomStringConvertible.description.getter(
                             &type metadata for Int,
                             &protocol witness table for Int);
  object = v29._object;
  String.append(_:)(v29);
  swift_bridgeObjectRelease(object);
  v31._countAndFlagsBits = 0xD00000000000002BLL;
  v31._object = (void *)0x80000001004F31E0LL;
  String.append(_:)(v31);
  v33 = static os_log_type_t.error.getter(v32);
  sub_1001D8B44(v33, 0, 0xE000000000000000LL);
  swift_bridgeObjectRelease(0xE000000000000000LL);
  return 0;
}

/* ========================================================================
 * sub_100215E5C
 * EA: 0x100215e5c
 ======================================================================== */

void sub_100215E5C()
{
  __int64 v0; // x20
  __int64 v1; // x19
  __int64 v2; // x0
  __int64 v3; // x26
  __int64 v4; // x24
  char *v5; // x27
  __int64 v6; // x0
  char *v7; // x25
  __int64 *v8; // x23
  __int64 v9; // x22
  __int64 v10; // x0
  __int64 v11; // x0
  __int64 v12; // x20
  unsigned __int64 v13; // x1
  unsigned __int64 v14; // x28
  __int64 v15; // x20
  __int64 (__fastcall *v16)(char *, __int64, __int64); // x28
  __int64 v17; // x0
  __int64 v18; // x20
  __int64 v19; // x1
  __int64 v20; // x22
  void (__fastcall *v21)(char *, __int64); // x21
  __n128 v22; // q0
  __int64 v23; // x0
  __int64 v24; // x20
  __int64 v25; // x1
  __int64 v26; // x22
  __int64 v27; // x20
  __int64 v28; // x1
  __int64 v29; // x21
  char *v30; // x22
  __n128 v31; // q0
  void (__fastcall *v32)(char *, char *, __int64); // x20
  char *v33; // x21
  __int64 v34; // x20
  __int64 v35; // x20
  id v36; // x21
  NSURL *v37; // x8
  void *v38; // x0
  void *v39; // x20
  id v40; // x28
  unsigned __int64 v41; // x22
  __int64 v42; // x0
  __int64 v43; // x0
  __int64 v44; // x21
  unsigned __int64 v45; // x1
  unsigned __int64 v46; // x26
  __n128 v47; // q0
  id v48; // x0
  __int64 v49; // x23
  void *v50; // x22
  char *v51; // x20
  __int64 *v52; // x8
  __int64 v53; // x22
  __int64 v54; // x24
  id v55; // x22
  void *v56; // x23
  id v57; // x24
  void *v58; // x20
  id v59; // x24
  __int64 KeyPath; // x21
  char *v61; // x23
  __int64 v62; // x21
  __int64 v63; // x0
  __int64 v64; // x0
  __int64 v65; // x22
  __int64 v66; // x20
  __int64 v67; // x21
  void *v68; // x20
  id v69; // x21
  id v70; // x21
  id v71; // x0
  void *v72; // x23
  __int64 v73; // x21
  __int64 v74; // x22
  __int64 v75; // x20
  void *v76; // x9
  __int64 v77; // [xsp+20h] [xbp-100h] BYREF
  __int64 v78; // [xsp+28h] [xbp-F8h]
  __int64 v79; // [xsp+30h] [xbp-F0h]
  char *v80; // [xsp+38h] [xbp-E8h]
  char *v81; // [xsp+40h] [xbp-E0h]
  char *v82; // [xsp+48h] [xbp-D8h]
  __int64 v83; // [xsp+50h] [xbp-D0h]
  __int64 v84; // [xsp+58h] [xbp-C8h]
  __int64 v85; // [xsp+60h] [xbp-C0h]
  __int64 v86; // [xsp+68h] [xbp-B8h]
  char v87[16]; // [xsp+70h] [xbp-B0h] BYREF
  char v88[24]; // [xsp+80h] [xbp-A0h] BYREF
  __int64 v89; // [xsp+98h] [xbp-88h] BYREF
  unsigned __int64 v90; // [xsp+A0h] [xbp-80h]
  __int64 v91; // [xsp+B0h] [xbp-70h] BYREF
  __int64 v92; // [xsp+B8h] [xbp-68h]

  v1 = v0;
  v2 = sub_10003E4E0(&qword_1006691C8, qword_100544220);
  v83 = *(_QWORD *)(v2 - 8);
  v84 = v2;
  v82 = (char *)&v77 - ((*(_QWORD *)(v83 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v81 = (char *)&v77
      - ((*(_QWORD *)(*(_QWORD *)(sub_10003E4E0((__int64 *)&unk_100669D80, (__int64 *)&unk_10053BF90) - 8) + 64LL) + 15LL)
       & 0xFFFFFFFFFFFFFFF0LL);
  v3 = type metadata accessor for URL(0);
  v4 = *(_QWORD *)(v3 - 8);
  v80 = (char *)&v77 - ((*(_QWORD *)(v4 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = v80;
  v6 = type metadata accessor for String.Encoding(0);
  v85 = *(_QWORD *)(v6 - 8);
  v86 = v6;
  v7 = (char *)&v77 - ((*(_QWORD *)(v85 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v8 = (__int64 *)(v0 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_manifestStr);
  v9 = *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_manifestStr + 8);
  if ( v9 )
  {
    v91 = *v8;
    v92 = v9;
    v10 = swift_bridgeObjectRetain(v9);
    v11 = static String.Encoding.utf8.getter(v10);
    v79 = sub_100056930(v11);
    v12 = StringProtocol.data(using:allowLossyConversion:)(v7, 0, &type metadata for String);
    v14 = v13;
    (*(void (__fastcall **)(char *, __int64))(v85 + 8))(v7, v86);
    swift_bridgeObjectRelease(v9);
    if ( v14 >> 60 != 15 )
    {
      sub_1000509CC(v12, v14);
      v15 = OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_url;
      swift_beginAccess(v1 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_url, v88, 0, 0);
      v16 = *(__int64 (__fastcall **)(char *, __int64, __int64))(v4 + 16);
      v78 = v15;
      v17 = v16(v5, v1 + v15, v3);
      v18 = URL.absoluteString.getter(v17);
      v20 = v19;
      v21 = *(void (__fastcall **)(char *, __int64))(v4 + 8);
      v21(v5, v3);
      LOBYTE(v18) = sub_1001273B8(1886680168, 0xE400000000000000LL, v18, v20);
      v22 = swift_bridgeObjectRelease(v20);
      if ( (v18 & 1) != 0 )
      {
        v23 = ((__int64 (__fastcall *)(char *, __int64, __int64, __n128))v16)(v5, v1 + v78, v3, v22);
        v24 = URL.absoluteString.getter(v23);
        v26 = v25;
        v21(v5, v3);
        v91 = v24;
        v92 = v26;
        v89 = 1886680168;
        v90 = 0xE400000000000000LL;
        strcpy(v87, "VTm3u8Manifest");
        v87[15] = -18;
        v27 = StringProtocol.replacingOccurrences<A, B>(of:with:options:range:)(
                &v89,
                v87,
                0,
                0,
                0,
                1,
                &type metadata for String,
                &type metadata for String,
                &type metadata for String,
                v79,
                v79,
                v79);
        v29 = v28;
        swift_bridgeObjectRelease(v26);
        v30 = v81;
        URL.init(string:)(v27, v29);
        v31 = swift_bridgeObjectRelease(v29);
        if ( (*(unsigned int (__fastcall **)(char *, __int64, __int64, __n128))(v4 + 48))(v30, 1, v3, v31) == 1 )
        {
          sub_10005285C(v30, &unk_100669D80, &unk_10053BF90);
        }
        else
        {
          v32 = *(void (__fastcall **)(char *, char *, __int64))(v4 + 32);
          v33 = v80;
          v32(v80, v30, v3);
          v32(v5, v33, v3);
          v34 = v78;
          swift_beginAccess(v1 + v78, &v91, 33, 0);
          (*(void (__fastcall **)(__int64, char *, __int64))(v4 + 40))(v1 + v34, v5, v3);
          swift_endAccess(&v91);
        }
      }
    }
  }
  v35 = OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_url;
  swift_beginAccess(v1 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_url, &v91, 0, 0);
  (*(void (__fastcall **)(char *, __int64, __int64))(v4 + 16))(v5, v1 + v35, v3);
  v36 = objc_allocWithZone((Class)&OBJC_CLASS___AVURLAsset);
  URL._bridgeToObjectiveC()(v37);
  v39 = v38;
  v40 = objc_msgSend(v36, "initWithURL:options:", v38, 0);
  objc_release(v39);
  (*(void (__fastcall **)(char *, __int64))(v4 + 8))(v5, v3);
  v41 = v8[1];
  if ( v41 )
  {
    v89 = *v8;
    v90 = v41;
    v42 = swift_bridgeObjectRetain(v41);
    v43 = static String.Encoding.utf8.getter(v42);
    sub_100056930(v43);
    v44 = StringProtocol.data(using:allowLossyConversion:)(v7, 0, &type metadata for String);
    v46 = v45;
    (*(void (__fastcall **)(char *, __int64))(v85 + 8))(v7, v86);
    v47 = swift_bridgeObjectRelease(v41);
    if ( v46 >> 60 != 15 )
    {
      v48 = objc_msgSend(objc_allocWithZone((Class)type metadata accessor for VTResourceLoaderDelegate(0, v47)), "init");
      v49 = OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_loaderDelegate;
      v50 = *(void **)(v1 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_loaderDelegate);
      *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_loaderDelegate) = v48;
      v51 = (char *)objc_retain(v48);
      objc_release(v50);
      if ( v51 )
      {
        v52 = (__int64 *)&v51[OBJC_IVAR____TtC15ExternalMonitor24VTResourceLoaderDelegate_mainifestData];
        v53 = *(_QWORD *)&v51[OBJC_IVAR____TtC15ExternalMonitor24VTResourceLoaderDelegate_mainifestData];
        v54 = *(_QWORD *)&v51[OBJC_IVAR____TtC15ExternalMonitor24VTResourceLoaderDelegate_mainifestData + 8];
        *v52 = v44;
        v52[1] = v46;
        sub_10004B430(v44, v46);
        sub_1000509CC(v53, v54);
        objc_release(v51);
      }
      v55 = objc_retainAutoreleasedReturnValue(objc_msgSend(v40, "resourceLoader"));
      v56 = *(void **)(v1 + v49);
      sub_100040668(0);
      v57 = objc_retain(v56);
      v58 = (void *)static OS_dispatch_queue.main.getter();
      objc_msgSend(v55, "setDelegate:queue:", v57, v58);
      objc_release(v55);
      objc_release(v57);
      objc_release(v58);
      sub_1000509CC(v44, v46);
    }
  }
  v59 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___AVPlayerItem), "initWithAsset:", v40);
  KeyPath = swift_getKeyPath(&unk_100544238);
  v61 = v82;
  _KeyValueCodingAndObservingPublishing<>.publisher<A>(for:options:)(KeyPath, 5);
  swift_release(KeyPath);
  v62 = swift_allocObject(&unk_1005C7438, 24, 7);
  v63 = swift_unknownObjectWeakInit(v62 + 16, v1);
  v64 = sub_100216FE0(v63);
  v65 = v84;
  v66 = Publisher<>.sink(receiveValue:)(sub_100216FD8, v62, v84, v64);
  swift_release(v62);
  (*(void (__fastcall **)(char *, __int64))(v83 + 8))(v61, v65);
  v67 = OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_cancellables;
  swift_beginAccess(v1 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_cancellables, &v89, 33, 0);
  AnyCancellable.store(in:)(v1 + v67);
  swift_endAccess(&v89);
  swift_release(v66);
  v68 = *(void **)(v1 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playerVC);
  v69 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___AVPlayer), "initWithPlayerItem:", v59);
  objc_msgSend(v68, "setPlayer:", v69);
  objc_release(v69);
  objc_msgSend(v68, "setShowsPlaybackControls:", 0);
  objc_msgSend(v68, "setVideoGravity:", AVLayerVideoGravityResizeAspect);
  v70 = objc_retainAutoreleasedReturnValue(objc_msgSend(v68, "player"));
  objc_msgSend(v70, "play");
  objc_release(v70);
  v71 = objc_retainAutoreleasedReturnValue(objc_msgSend(v68, "player"));
  if ( v71 )
  {
    v89 = (__int64)v71;
    v72 = v71;
    v73 = swift_getKeyPath(&unk_100544278);
    v74 = swift_allocObject(&unk_1005C7438, 24, 7);
    swift_unknownObjectWeakInit(v74 + 16, v1);
    v75 = _KeyValueCodingAndObserving.observe<A>(_:options:changeHandler:)(
            v73,
            0,
            sub_100217060,
            v74,
            &protocol witness table for NSObject);
    objc_release(v72);
    objc_release(v40);
    objc_release(v59);
    swift_release(v73);
    swift_release(v74);
  }
  else
  {
    objc_release(v40);
    objc_release(v59);
    v75 = 0;
  }
  v76 = *(void **)(v1 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_observation);
  *(_QWORD *)(v1 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_observation) = v75;
  objc_release(v76);
}

/* ========================================================================
 * sub_100216630
 * EA: 0x100216630
 ======================================================================== */

void __fastcall sub_100216630(__int64 *a1, __int64 a2)
{
  __int64 v3; // x19
  __int64 Strong; // x0
  void *v5; // x20
  _BYTE v6[24]; // [xsp+8h] [xbp-28h] BYREF

  v3 = *a1;
  swift_beginAccess(a2 + 16, v6, 0, 0);
  Strong = swift_unknownObjectWeakLoadStrong(a2 + 16);
  if ( Strong )
  {
    v5 = (void *)Strong;
    sub_1002168CC(v3);
    objc_release(v5);
  }
}

/* ========================================================================
 * sub_100216688
 * EA: 0x100216688
 ======================================================================== */

void __fastcall sub_100216688(__int64 a1, __int64 a2, __int64 a3)
{
  __int64 Strong; // x0
  void *v5; // x20
  id v6; // x21
  id v7; // x20
  id v8; // x21
  __int64 v9; // x0
  void (*v10)(void); // x19
  __int64 v11; // x20
  void *v12; // x21
  __int64 v13; // [xsp+0h] [xbp-50h] BYREF
  _BYTE v14[24]; // [xsp+18h] [xbp-38h] BYREF

  swift_beginAccess(a3 + 16, v14, 0, 0);
  Strong = swift_unknownObjectWeakLoadStrong(a3 + 16);
  if ( Strong )
  {
    v5 = (void *)Strong;
    v6 = objc_retain(*(id *)(Strong + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playerVC));
    objc_release(v5);
    v7 = objc_retainAutoreleasedReturnValue(objc_msgSend(v6, "player"));
    objc_release(v6);
    if ( v7 )
    {
      v8 = objc_msgSend(v7, "timeControlStatus");
      objc_release(v7);
      if ( v8 == (id)2 )
      {
        swift_beginAccess(a3 + 16, &v13, 0, 0);
        v9 = swift_unknownObjectWeakLoadStrong(a3 + 16);
        if ( v9 )
        {
          v10 = *(void (**)(void))(v9 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playerPlayBlock);
          v11 = *(_QWORD *)(v9 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playerPlayBlock + 8);
          v12 = (void *)v9;
          sub_10004645C(v10, v11);
          objc_release(v12);
          if ( v10 )
          {
            v10();
            sub_10004646C(v10, v11);
          }
        }
      }
    }
  }
}

/* ========================================================================
 * sub_10021678C
 * EA: 0x10021678c
 ======================================================================== */

id __fastcall sub_10021678C(__int64 a1)
{
  char *v1; // x20
  __int64 v3; // x21
  char *v4; // x8
  char *v5; // x8
  char *v6; // x8
  __int64 v7; // x22
  __int64 v8; // x21
  __int64 v9; // x23
  id v10; // x20
  objc_super v12; // [xsp+0h] [xbp-40h] BYREF

  v3 = OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playerVC;
  *(_QWORD *)&v1[v3] = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___AVPlayerViewController), "init");
  v4 = &v1[OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_manifestStr];
  *(_QWORD *)v4 = 0;
  *((_QWORD *)v4 + 1) = 0;
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_loaderDelegate] = 0;
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_observation] = 0;
  v5 = &v1[OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playerPlayBlock];
  *(_QWORD *)v5 = 0;
  *((_QWORD *)v5 + 1) = 0;
  v6 = &v1[OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playStatusBlock];
  *(_QWORD *)v6 = 0;
  *((_QWORD *)v6 + 1) = 0;
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_cancellables] = &_swiftEmptySetSingleton;
  v7 = OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_url;
  v8 = type metadata accessor for URL(0);
  v9 = *(_QWORD *)(v8 - 8);
  (*(void (__fastcall **)(char *, __int64, __int64))(v9 + 16))(&v1[v7], a1, v8);
  v12.receiver = v1;
  v12.super_class = (Class)type metadata accessor for VTSimpleAVPlayerViewController(0);
  v10 = objc_msgSendSuper2(&v12, "initWithNibName:bundle:", 0, 0);
  (*(void (__fastcall **)(__int64, __int64))(v9 + 8))(a1, v8);
  return v10;
}

/* ========================================================================
 * sub_1002168CC
 * EA: 0x1002168cc
 ======================================================================== */

// local variable allocation has failed, the output may be wrong!
double __fastcall sub_1002168CC(__int64 a1)
{
  __int64 v1; // x20
  __int64 v2; // x19
  __int64 v3; // x0
  __int64 v4; // x21
  __int64 v5; // x23
  void *v6; // x25
  id v7; // x24
  id v8; // x25
  __int64 v9; // x0
  __int64 v10; // x0 OVERLAPPED
  unsigned __int64 v11; // x1
  unsigned __int64 v12; // x8
  unsigned __int64 v13; // x9
  __int64 v14; // x20
  __int64 v15; // x8
  void (__fastcall *v16)(__int64, __n128); // x19
  __int64 v17; // x20
  __n128 v18; // q0
  __int64 v19; // x0
  __int64 v20; // x0
  __int64 v21; // x19
  unsigned __int64 v22; // x8
  __int64 v23; // x0
  __int64 inited; // x21
  __int64 v25; // x23
  void *v26; // x25
  id v27; // x24
  id v28; // x25
  __int64 v29; // x0
  __int64 v30; // x0 OVERLAPPED
  unsigned __int64 v31; // x1
  unsigned __int64 v32; // x8
  unsigned __int64 v33; // x9
  __int64 v34; // x20
  __int64 v35; // x8
  void (__fastcall *v36)(_QWORD, __n128); // x19
  __int64 v37; // x20
  __n128 v38; // q0
  __int64 v39; // x0
  __int64 v40; // x0
  __int64 v41; // x19
  __int64 v42; // x21
  __int64 v43; // x0
  double result; // d0
  _BYTE v45[80]; // [xsp+18h] [xbp-198h] BYREF
  unsigned __int64 v46; // [xsp+68h] [xbp-148h] BYREF
  unsigned __int64 v47; // [xsp+70h] [xbp-140h]
  _BYTE v48[24]; // [xsp+78h] [xbp-138h] BYREF
  _BYTE v49[80]; // [xsp+90h] [xbp-120h] BYREF
  _BYTE v50[80]; // [xsp+E0h] [xbp-D0h] BYREF
  __int128 v51; // [xsp+130h] [xbp-80h]
  __int128 v52; // [xsp+140h] [xbp-70h]
  __int64 v53; // [xsp+150h] [xbp-60h]
  __int128 v54; // [xsp+160h] [xbp-50h] BYREF

  if ( a1 )
  {
    v2 = v1;
    if ( a1 == 2 )
    {
      v23 = sub_10003E4E0(&qword_10066D920, &qword_10053C530);
      inited = swift_initStackObject(v23, v49);
      *(_QWORD *)(inited + 32) = 0x65756C6176LL;
      *(_OWORD *)(inited + 16) = xmmword_10053B940;
      *(_QWORD *)(inited + 40) = 0xE500000000000000LL;
      if ( qword_100662490 != -1 )
        swift_once(&qword_100662490, sub_10019D4BC);
      swift_beginAccess(&xmmword_10066F7A8, v48, 0, 0);
      v51 = xmmword_10066F7A8;
      v52 = xmmword_10066F7B8;
      v53 = qword_10066F7C8;
      v25 = *((_QWORD *)&xmmword_10066F7A8 + 1);
      v26 = (void *)xmmword_10066F7B8;
      v54 = *(__int128 *)((char *)&xmmword_10066F7B8 + 8);
      v27 = objc_retain((id)xmmword_10066F7A8);
      swift_retain(v25);
      v28 = objc_retain(v26);
      sub_10004542C((__int64)&v54, (__int64)&v46);
      v29 = sub_10003E4E0(&qword_10066A970, (__int64 *)&unk_10053BAD0);
      WrappedDefault.wrappedValue.getter(&v46, v29);
      objc_release(v28);
      swift_release(v25);
      objc_release(v27);
      sub_10003F604(&v54);
      if ( (_BYTE)v46 )
        v30 = 49;
      else
        v30 = 48;
      v46 = 0xD000000000000014LL;
      v47 = 0x80000001004F6D30LL;
      v31 = 0xE100000000000000LL;
      String.append(_:)(*(Swift::String *)&v30);
      swift_bridgeObjectRelease(0xE100000000000000LL);
      v32 = v46;
      v33 = v47;
      *(_QWORD *)(inited + 72) = &type metadata for String;
      *(_QWORD *)(inited + 48) = v32;
      *(_QWORD *)(inited + 56) = v33;
      v34 = sub_100166EF8(inited);
      swift_setDeallocating(inited);
      sub_10005285C(inited + 32, &unk_100668B70, &unk_10053BAC0);
      sub_1000B22CC(0x79616C506576694CLL, 0xE800000000000000LL, v34);
      swift_bridgeObjectRelease(v34);
      v35 = v2 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playStatusBlock;
      v36 = *(void (__fastcall **)(_QWORD, __n128))(v2
                                                  + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playStatusBlock);
      if ( v36 )
      {
        v37 = *(_QWORD *)(v35 + 8);
        v38 = swift_retain(v37);
        v36(0, v38);
        sub_10004646C(v36, v37);
      }
      v39 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
      v21 = swift_allocObject(v39, 64, 7);
      *(_OWORD *)(v21 + 16) = xmmword_10053B940;
      *(_QWORD *)(v21 + 56) = &type metadata for String;
      *(_QWORD *)(v21 + 32) = 0x99E5BE94E6AD92E6LL;
      v22 = 0xAFA5B4E8B1A4E5A8LL;
      goto LABEL_22;
    }
    if ( a1 != 1 )
      return result;
    v3 = sub_10003E4E0(&qword_10066D920, &qword_10053C530);
    v4 = swift_initStackObject(v3, v45);
    *(_QWORD *)(v4 + 32) = 0x65756C6176LL;
    *(_OWORD *)(v4 + 16) = xmmword_10053B940;
    *(_QWORD *)(v4 + 40) = 0xE500000000000000LL;
    if ( qword_100662490 != -1 )
      swift_once(&qword_100662490, sub_10019D4BC);
    swift_beginAccess(&xmmword_10066F7A8, v48, 0, 0);
    v51 = xmmword_10066F7A8;
    v52 = xmmword_10066F7B8;
    v53 = qword_10066F7C8;
    v5 = *((_QWORD *)&xmmword_10066F7A8 + 1);
    v6 = (void *)xmmword_10066F7B8;
    v54 = *(__int128 *)((char *)&xmmword_10066F7B8 + 8);
    v7 = objc_retain((id)xmmword_10066F7A8);
    swift_retain(v5);
    v8 = objc_retain(v6);
    sub_10004542C((__int64)&v54, (__int64)&v46);
    v9 = sub_10003E4E0(&qword_10066A970, (__int64 *)&unk_10053BAD0);
    WrappedDefault.wrappedValue.getter(&v46, v9);
    objc_release(v8);
    swift_release(v5);
    objc_release(v7);
    sub_10003F604(&v54);
    if ( (_BYTE)v46 )
      v10 = 49;
    else
      v10 = 48;
    v46 = 0xD000000000000015LL;
    v47 = 0x80000001004F6D50LL;
    v11 = 0xE100000000000000LL;
    String.append(_:)(*(Swift::String *)&v10);
    swift_bridgeObjectRelease(0xE100000000000000LL);
    v12 = v46;
    v13 = v47;
    *(_QWORD *)(v4 + 72) = &type metadata for String;
    *(_QWORD *)(v4 + 48) = v12;
    *(_QWORD *)(v4 + 56) = v13;
    v14 = sub_100166EF8(v4);
    swift_setDeallocating(v4);
    sub_10005285C(v4 + 32, &unk_100668B70, &unk_10053BAC0);
    sub_1000B22CC(0x79616C506576694CLL, 0xE800000000000000LL, v14);
    swift_bridgeObjectRelease(v14);
    v15 = v2 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playStatusBlock;
    v16 = *(void (__fastcall **)(__int64, __n128))(v2
                                                 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playStatusBlock);
    if ( v16 )
    {
      v17 = *(_QWORD *)(v15 + 8);
      v18 = swift_retain(v17);
      v16(1, v18);
      sub_10004646C(v16, v17);
    }
    v19 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
    v20 = swift_allocObject(v19, 64, 7);
    v21 = v20;
    *(_OWORD *)(v20 + 16) = xmmword_10053B940;
    v22 = 0x80000001004DE850LL;
    *(_QWORD *)(v20 + 56) = &type metadata for String;
  }
  else
  {
    v40 = sub_10003E4E0(&qword_10066D920, &qword_10053C530);
    v41 = swift_initStackObject(v40, v50);
    *(_OWORD *)(v41 + 16) = xmmword_10053B940;
    *(_QWORD *)(v41 + 32) = 0x65756C6176LL;
    *(_QWORD *)(v41 + 72) = &type metadata for String;
    *(_QWORD *)(v41 + 40) = 0xE500000000000000LL;
    *(_QWORD *)(v41 + 48) = 0x64656B63696C63LL;
    *(_QWORD *)(v41 + 56) = 0xE700000000000000LL;
    v42 = sub_100166EF8(v41);
    swift_setDeallocating(v41);
    sub_10005285C(v41 + 32, &unk_100668B70, &unk_10053BAC0);
    sub_1000B22CC(0x79616C506576694CLL, 0xE800000000000000LL, v42);
    swift_bridgeObjectRelease(v42);
    v43 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
    v20 = swift_allocObject(v43, 64, 7);
    v21 = v20;
    *(_OWORD *)(v20 + 16) = xmmword_10053B940;
    v22 = 0x80000001004DE830LL;
    *(_QWORD *)(v20 + 56) = &type metadata for String;
  }
  *(_QWORD *)(v20 + 32) = 0x1000000000000015LL;
LABEL_22:
  *(_QWORD *)(v21 + 40) = v22;
  print(_:separator:terminator:)(v21, 32, 0xE100000000000000LL, 10, 0xE100000000000000LL);
  *(_QWORD *)&result = swift_bridgeObjectRelease(v21).n128_u64[0];
  return result;
}

/* ========================================================================
 * sub_100216EE0
 * EA: 0x100216ee0
 ======================================================================== */

__int64 __fastcall sub_100216EE0(__int64 a1)
{
  __int64 result; // x0
  unsigned __int64 v3; // x1
  _QWORD v4[8]; // [xsp+0h] [xbp-50h] BYREF

  v4[0] = (char *)&value witness table for Builtin.UnknownObject + 64;
  result = type metadata accessor for URL(319);
  if ( v3 <= 0x3F )
  {
    v4[1] = *(_QWORD *)(result - 8) + 64LL;
    v4[2] = &unk_1005441F0;
    v4[3] = &unk_100544208;
    v4[4] = &unk_100544208;
    v4[5] = &unk_1005441F0;
    v4[6] = &unk_1005441F0;
    v4[7] = (char *)&value witness table for Builtin.BridgeObject + 64;
    result = swift_updateClassMetadata2(a1, 256, 8, v4, a1 + 80);
    if ( !result )
      return 0;
  }
  return result;
}

/* ========================================================================
 * sub_100216FE0
 * EA: 0x100216fe0
 ======================================================================== */

unsigned __int64 sub_100216FE0()
{
  unsigned __int64 result; // x0
  __int64 v1; // x0

  result = qword_1006691D8;
  if ( !qword_1006691D8 )
  {
    v1 = sub_100040774(&qword_1006691C8, qword_100544220);
    result = swift_getWitnessTable(&protocol conformance descriptor for NSObject.KeyValueObservingPublisher<A, B>, v1);
    atomic_store(result, (unsigned __int64 *)&qword_1006691D8);
  }
  return result;
}

/* ========================================================================
 * sub_100217068
 * EA: 0x100217068
 ======================================================================== */

void __noreturn sub_100217068()
{
  __int64 v0; // x20
  __int64 v1; // x19
  _QWORD *v2; // x8
  _QWORD *v3; // x8
  _QWORD *v4; // x8

  v1 = OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playerVC;
  *(_QWORD *)(v0 + v1) = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___AVPlayerViewController), "init");
  v2 = (_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_manifestStr);
  *v2 = 0;
  v2[1] = 0;
  *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_loaderDelegate) = 0;
  *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_observation) = 0;
  v3 = (_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playerPlayBlock);
  *v3 = 0;
  v3[1] = 0;
  v4 = (_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_playStatusBlock);
  *v4 = 0;
  v4[1] = 0;
  *(_QWORD *)(v0 + OBJC_IVAR____TtC15ExternalMonitor30VTSimpleAVPlayerViewController_cancellables) = &_swiftEmptySetSingleton;
  _assertionFailure(_:_:file:line:flags:)(
    "Fatal error",
    11,
    2,
    0xD000000000000025LL,
    0x80000001004D9660LL,
    "ExternalMonitor/VTSimpleAVPlayerViewController.swift",
    52,
    2,
    88,
    0);
  __break(1u);
}

/* ========================================================================
 * sub_10021714C
 * EA: 0x10021714c
 ======================================================================== */

__int64 sub_10021714C()
{
  __int64 v0; // x20
  __int64 v1; // x19
  __int64 v2; // x21
  __int64 v3; // x22
  __int64 v4; // x23
  __int64 v5; // x8
  __int64 v6; // x24
  unsigned __int64 v7; // x25
  unsigned __int64 v8; // x26

  v1 = type metadata accessor for URL(0);
  v2 = *(_QWORD *)(v1 - 8);
  v3 = *(unsigned __int8 *)(v2 + 80);
  v4 = (v3 + 32) & ~v3;
  v5 = v4 + *(_QWORD *)(v2 + 64);
  v6 = (v5 + 7) & 0xFFFFFFFFFFFFFF8LL;
  v7 = (v5 + 23) & 0xFFFFFFFFFFFFFFF8LL;
  v8 = (v7 + 23) & 0xFFFFFFFFFFFFFFF8LL;
  swift_unknownObjectRelease(*(_QWORD *)(v0 + 16));
  (*(void (__fastcall **)(__int64, __int64))(v2 + 8))(v0 + v4, v1);
  swift_bridgeObjectRelease(*(_QWORD *)(v0 + v6));
  swift_release(*(_QWORD *)(v0 + v7 + 8));
  swift_release(*(_QWORD *)(v0 + v8 + 8));
  return swift_deallocObject(v0, v8 + 16, v3 | 7);
}

/* ========================================================================
 * sub_1002171FC
 * EA: 0x1002171fc
 ======================================================================== */

double __fastcall sub_1002171FC(__int64 a1, __int64 a2, double a3, double a4, double a5, double a6)
{
  __int64 v11; // x20
  double (__fastcall *v12)(__int64, double, double, double, double); // x21
  double v13; // d8

  v12 = *(double (__fastcall **)(__int64, double, double, double, double))(a1 + 32);
  v11 = *(_QWORD *)(a1 + 40);
  swift_retain(v11);
  v13 = v12(a2, a3, a4, a5, a6);
  swift_release(v11);
  return v13;
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter setIncrementMultiple:]
 * EA: 0x1002173ac
 ======================================================================== */

void __cdecl -[CIStereoShiftMetalFilter setIncrementMultiple:](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa
                + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_incrementMultiple);
  *(Class *)((char *)&self->super.super.isa
           + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_incrementMultiple) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * sub_100217498
 * EA: 0x100217498
 ======================================================================== */

id sub_100217498()
{
  char *v0; // x20
  char *v1; // x19
  id result; // x0
  void *v3; // x21
  id v4; // x20
  void *v5; // x21
  id v6; // x24
  void *v7; // x21
  double v8; // d0
  double v9; // d8
  double v10; // d1
  double v11; // d9
  double v12; // d2
  double v13; // d10
  double v14; // d3
  double v15; // d11
  void *v16; // x22
  __int64 v17; // x0
  __int64 v18; // x25
  __int64 v19; // x0
  id v20; // x23
  id v21; // x24
  Class isa; // x26
  __int64 v23; // x20
  Class v24; // x0
  Class v25; // x0
  Class v26; // x0
  Class v27; // x0
  Class v28; // x19
  id v29; // x20
  _QWORD v30[6]; // [xsp+0h] [xbp-90h] BYREF

  v1 = v0;
  result = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "inputImage"));
  if ( !result )
  {
    __break(1u);
    goto LABEL_15;
  }
  v3 = result;
  v4 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CISampler), "initWithImage:", result);
  objc_release(v3);
  result = objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "inputDepthImage"));
  if ( !result )
  {
LABEL_15:
    __break(1u);
    return result;
  }
  v5 = result;
  v6 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___CISampler), "initWithImage:", result);
  objc_release(v5);
  v7 = *(void **)&v1[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_kernel];
  objc_msgSend(v4, "extent");
  v9 = v8;
  v11 = v10;
  v13 = v12;
  v15 = v14;
  v30[4] = nullsub_9;
  v30[5] = 0;
  v30[0] = _NSConcreteStackBlock;
  v30[1] = 1107296256;
  v30[2] = sub_1002171FC;
  v30[3] = &unk_1005C7478;
  v16 = _Block_copy(v30);
  v17 = sub_10003E4E0((__int64 *)&unk_100669620, (__int64 *)&unk_10053BBC0);
  v18 = swift_allocObject(v17, 256, 7);
  *(_OWORD *)(v18 + 16) = xmmword_1005442B0;
  v19 = sub_10004B4D8(0, &qword_10066C278, &classRef_CISampler);
  *(_QWORD *)(v18 + 32) = v4;
  *(_QWORD *)(v18 + 88) = v19;
  *(_QWORD *)(v18 + 56) = v19;
  *(_QWORD *)(v18 + 64) = v6;
  v20 = objc_retain(v4);
  v21 = objc_retain(v6);
  isa = (Class)objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "inputIncrement"));
  v23 = sub_10004B4D8(0, (unsigned __int64 *)&qword_100669DA0, &classRef_NSNumber);
  *(_QWORD *)(v18 + 120) = v23;
  if ( !isa )
    isa = NSNumber.init(integerLiteral:)(0).super.super.isa;
  *(_QWORD *)(v18 + 96) = isa;
  v24 = (Class)objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "inputMaxShift"));
  *(_QWORD *)(v18 + 152) = v23;
  if ( !v24 )
    v24 = NSNumber.init(integerLiteral:)(0).super.super.isa;
  *(_QWORD *)(v18 + 128) = v24;
  v25 = (Class)objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "incrementMultiple"));
  *(_QWORD *)(v18 + 184) = v23;
  if ( !v25 )
    v25 = NSNumber.init(integerLiteral:)(1).super.super.isa;
  *(_QWORD *)(v18 + 160) = v25;
  v26 = (Class)objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "shiftRatio"));
  *(_QWORD *)(v18 + 216) = v23;
  if ( !v26 )
    v26 = NSNumber.init(integerLiteral:)(0).super.super.isa;
  *(_QWORD *)(v18 + 192) = v26;
  v27 = (Class)objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "sigmoidCoef"));
  *(_QWORD *)(v18 + 248) = v23;
  if ( !v27 )
    v27 = NSNumber.init(integerLiteral:)(5).super.super.isa;
  *(_QWORD *)(v18 + 224) = v27;
  v28 = Array._bridgeToObjectiveC()().super.isa;
  swift_bridgeObjectRelease(v18);
  v29 = objc_retainAutoreleasedReturnValue(objc_msgSend(v7, "applyWithExtent:roiCallback:arguments:", v16, v28, v9, v11, v13, v15));
  objc_release(v28);
  objc_release(v21);
  objc_release(v20);
  _Block_release(v16);
  return v29;
}

/* ========================================================================
 * sub_1002177D4
 * EA: 0x1002177d4
 ======================================================================== */

id sub_1002177D4()
{
  char *v0; // x20
  char *v1; // x19
  __int64 v2; // x22
  __int64 v3; // x27
  char *v4; // x20
  char *v5; // x23
  id v6; // x24
  NSString v7; // x25
  NSString v8; // x26
  id v9; // x21
  __int64 v10; // x24
  __int64 v11; // x1
  __int64 v12; // x25
  __int64 v13; // x20
  __int64 v14; // x21
  objc_class *v15; // x0
  id result; // x0
  __int64 v17; // [xsp+0h] [xbp-70h] BYREF
  objc_super v18; // [xsp+8h] [xbp-68h] BYREF

  v1 = v0;
  v2 = type metadata accessor for URL(0);
  v3 = *(_QWORD *)(v2 - 8);
  v4 = (char *)&v17 - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = v4;
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputImage] = 0;
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputDepthImage] = 0;
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputIncrement] = 0;
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputMaxShift] = 0;
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_incrementMultiple] = 0;
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_shiftRatio] = 0;
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_sigmoidCoef] = 0;
  v6 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSBundle), "mainBundle"));
  v7 = String._bridgeToObjectiveC()();
  v8 = String._bridgeToObjectiveC()();
  v9 = objc_retainAutoreleasedReturnValue(objc_msgSend(v6, "URLForResource:withExtension:", v7, v8));
  objc_release(v6);
  objc_release(v7);
  objc_release(v8);
  if ( v9 )
  {
    static URL._unconditionallyBridgeFromObjectiveC(_:)(v9);
    objc_release(v9);
    (*(void (__fastcall **)(char *, char *, __int64))(v3 + 32))(v4, v4, v2);
    v10 = Data.init(contentsOf:options:)(v4, 0);
    v12 = v11;
    sub_10004B4D8(0, &qword_10066C270, &classRef_CIKernel);
    sub_10004B430(v10, v12);
    v13 = sub_1002183B0(1145457011, 0xE400000000000000LL, v10, v12);
    sub_100040E14(v10, v12);
    v14 = OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_kernel;
    (*(void (__fastcall **)(char *, __int64))(v3 + 8))(v5, v2);
    sub_100040E14(v10, v12);
    *(_QWORD *)&v1[v14] = v13;
    v15 = (objc_class *)type metadata accessor for CIStereoShiftMetalFilter();
    v18.receiver = v1;
    v18.super_class = v15;
    return objc_msgSendSuper2(&v18, "init");
  }
  else
  {
    __break(1u);
    swift_unexpectedError(0, "ExternalMonitor/CIStereoShiftMetalFilter.swift", 46, 1, 29);
    __break(1u);
    result = (id)swift_unexpectedError(0, "ExternalMonitor/CIStereoShiftMetalFilter.swift", 46, 1, 30);
    __break(1u);
  }
  return result;
}

/* ========================================================================
 * sub_100217A88
 * EA: 0x100217a88
 ======================================================================== */

__int64 __fastcall sub_100217A88(void *a1)
{
  char *v1; // x20
  char *v2; // x22
  __int64 v4; // x23
  __int64 v5; // x28
  char *v6; // x20
  char *v7; // x24
  id v8; // x25
  NSString v9; // x26
  NSString v10; // x27
  id v11; // x21
  __int64 v12; // x25
  __int64 v13; // x1
  __int64 v14; // x26
  __int64 v15; // x20
  __int64 v16; // x21
  objc_class *v17; // x0
  id v18; // x20
  __int64 result; // x0
  __int64 v20; // [xsp+0h] [xbp-70h] BYREF
  objc_super v21; // [xsp+8h] [xbp-68h] BYREF

  v2 = v1;
  v4 = type metadata accessor for URL(0);
  v5 = *(_QWORD *)(v4 - 8);
  v6 = (char *)&v20 - ((*(_QWORD *)(v5 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v7 = v6;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputImage] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputDepthImage] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputIncrement] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputMaxShift] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_incrementMultiple] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_shiftRatio] = 0;
  *(_QWORD *)&v2[OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_sigmoidCoef] = 0;
  v8 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSBundle), "mainBundle"));
  v9 = String._bridgeToObjectiveC()();
  v10 = String._bridgeToObjectiveC()();
  v11 = objc_retainAutoreleasedReturnValue(objc_msgSend(v8, "URLForResource:withExtension:", v9, v10));
  objc_release(v8);
  objc_release(v9);
  objc_release(v10);
  if ( v11 )
  {
    static URL._unconditionallyBridgeFromObjectiveC(_:)(v11);
    objc_release(v11);
    (*(void (__fastcall **)(char *, char *, __int64))(v5 + 32))(v6, v6, v4);
    v12 = Data.init(contentsOf:options:)(v6, 0);
    v14 = v13;
    sub_10004B4D8(0, &qword_10066C270, &classRef_CIKernel);
    sub_10004B430(v12, v14);
    v15 = sub_1002183B0(1145457011, 0xE400000000000000LL, v12, v14);
    sub_100040E14(v12, v14);
    v16 = OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_kernel;
    (*(void (__fastcall **)(char *, __int64))(v5 + 8))(v7, v4);
    sub_100040E14(v12, v14);
    *(_QWORD *)&v2[v16] = v15;
    v17 = (objc_class *)type metadata accessor for CIStereoShiftMetalFilter();
    v21.receiver = v2;
    v21.super_class = v17;
    v18 = objc_retainAutoreleasedReturnValue(objc_msgSendSuper2(&v21, "initWithCoder:", a1));
    objc_release(a1);
    if ( v18 )
      objc_release(v18);
    return (__int64)v18;
  }
  else
  {
    __break(1u);
    swift_unexpectedError(0, "ExternalMonitor/CIStereoShiftMetalFilter.swift", 46, 1, 29);
    __break(1u);
    result = swift_unexpectedError(0, "ExternalMonitor/CIStereoShiftMetalFilter.swift", 46, 1, 30);
    __break(1u);
  }
  return result;
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter .cxx_destruct]
 * EA: 0x100217d94
 ======================================================================== */

void __cdecl -[CIStereoShiftMetalFilter .cxx_destruct](_TtC15ExternalMonitor24CIStereoShiftMetalFilter *self, SEL a2)
{
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputImage));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputDepthImage));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputIncrement));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputMaxShift));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_incrementMultiple));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_shiftRatio));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_sigmoidCoef));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_kernel));
}

/* ========================================================================
 * sub_100217E4C
 * EA: 0x100217e4c
 ======================================================================== */

id __fastcall sub_100217E4C(void *a1, __int64 a2)
{
  double Width; // d8
  double v5; // d0
  double v6; // d8
  double v7; // d11
  double v8; // d10
  double v9; // d8
  double v10; // d9
  void *v11; // x19
  __int64 v12; // x21
  __int64 v13; // x22
  __int64 v14; // x23
  __int64 v15; // x1
  __int64 v16; // x20
  NSString v17; // x26
  __int64 v18; // x1
  __int64 v19; // x20
  NSString v20; // x25
  id v21; // x24
  __int64 v22; // x1
  __int64 v23; // x20
  NSString v24; // x25
  id v25; // x24
  __int64 v26; // x1
  __int64 v27; // x20
  NSString v28; // x25
  id v29; // x24
  __int64 v30; // x1
  __int64 v31; // x20
  NSString v32; // x25
  id v33; // x24
  __int64 v34; // x1
  __int64 v35; // x20
  NSString v36; // x25
  int v37; // s9
  id v38; // x0
  double v39; // d0
  id v40; // x24
  __int64 v41; // x1
  __int64 v42; // x20
  NSString v43; // x25
  id result; // x0
  id v45; // x24
  id v46; // x25
  __int64 v47; // x1
  __int64 v48; // x20
  NSString v49; // x21
  CGRect v50; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8
  CGRect v51; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8

  objc_msgSend(a1, "extent");
  Width = CGRectGetWidth(v50);
  objc_msgSend(a1, "extent");
  v5 = Width / CGRectGetHeight(v51);
  if ( v5 <= 1.777 )
    v6 = (1.777 / v5 + -1.0) * 0.7 + 1.0;
  else
    v6 = 1.777 / v5;
  v7 = *(double *)&qword_100671AB0;
  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  v8 = 0.004125 / v6;
  v9 = v6 * v7;
  if ( *(_BYTE *)(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences_isGaming) )
    v10 = 0.00208333333;
  else
    v10 = 0.00104166667;
  if ( qword_100662558 != -1 )
    swift_once(&qword_100662558, sub_1002177A8);
  v11 = (void *)qword_100697150;
  v12 = sub_10003E4E0((__int64 *)&unk_10066B320, &qword_10053BFA0);
  swift_initStaticObject(v12, &unk_100663D78);
  v13 = sub_10003E4E0(&qword_1006729A0, &qword_10053C610);
  v14 = sub_100101F98();
  BidirectionalCollection<>.joined(separator:)(0, 0xE000000000000000LL, v13, v14);
  v16 = v15;
  v17 = String._bridgeToObjectiveC()();
  objc_msgSend(v11, "setValue:forKey:", a1, v17, swift_bridgeObjectRelease(v16).n128_f64[0]);
  objc_release(v17);
  swift_initStaticObject(v12, &unk_100663E40);
  BidirectionalCollection<>.joined(separator:)(0, 0xE000000000000000LL, v13, v14);
  v19 = v18;
  v20 = String._bridgeToObjectiveC()();
  objc_msgSend(v11, "setValue:forKey:", a2, v20, swift_bridgeObjectRelease(v19).n128_f64[0]);
  objc_release(v20);
  v21 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSNumber), "initWithDouble:", v10);
  swift_initStaticObject(v12, &unk_100663F58);
  BidirectionalCollection<>.joined(separator:)(0, 0xE000000000000000LL, v13, v14);
  v23 = v22;
  v24 = String._bridgeToObjectiveC()();
  objc_msgSend(v11, "setValue:forKey:", v21, v24, swift_bridgeObjectRelease(v23).n128_f64[0]);
  objc_release(v21);
  objc_release(v24);
  v25 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSNumber), "initWithDouble:", v9);
  swift_initStaticObject(v12, &unk_100664060);
  BidirectionalCollection<>.joined(separator:)(0, 0xE000000000000000LL, v13, v14);
  v27 = v26;
  v28 = String._bridgeToObjectiveC()();
  objc_msgSend(v11, "setValue:forKey:", v25, v28, swift_bridgeObjectRelease(v27).n128_f64[0]);
  objc_release(v25);
  objc_release(v28);
  v29 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSNumber), "initWithDouble:", 1.0);
  swift_initStaticObject(v12, &unk_100664158);
  BidirectionalCollection<>.joined(separator:)(0, 0xE000000000000000LL, v13, v14);
  v31 = v30;
  v32 = String._bridgeToObjectiveC()();
  objc_msgSend(v11, "setValue:forKey:", v29, v32, swift_bridgeObjectRelease(v31).n128_f64[0]);
  objc_release(v29);
  objc_release(v32);
  v33 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSNumber), "initWithDouble:", v9 / v8);
  swift_initStaticObject(v12, &unk_100664290);
  BidirectionalCollection<>.joined(separator:)(0, 0xE000000000000000LL, v13, v14);
  v35 = v34;
  v36 = String._bridgeToObjectiveC()();
  objc_msgSend(v11, "setValue:forKey:", v33, v36, swift_bridgeObjectRelease(v35).n128_f64[0]);
  objc_release(v33);
  objc_release(v36);
  v37 = dword_100671AA8;
  v38 = objc_allocWithZone((Class)&OBJC_CLASS___NSNumber);
  LODWORD(v39) = v37;
  v40 = objc_msgSend(v38, "initWithFloat:", v39);
  swift_initStaticObject(v12, &unk_100664358);
  BidirectionalCollection<>.joined(separator:)(0, 0xE000000000000000LL, v13, v14);
  v42 = v41;
  v43 = String._bridgeToObjectiveC()();
  objc_msgSend(v11, "setValue:forKey:", v40, v43, swift_bridgeObjectRelease(v42).n128_f64[0]);
  objc_release(v40);
  objc_release(v43);
  result = objc_retainAutoreleasedReturnValue(objc_msgSend(v11, "outputImage"));
  if ( result )
  {
    v45 = result;
    v46 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSNumber), "initWithDouble:", -v9);
    swift_initStaticObject(v12, &unk_100664430);
    BidirectionalCollection<>.joined(separator:)(0, 0xE000000000000000LL, v13, v14);
    v48 = v47;
    v49 = String._bridgeToObjectiveC()();
    objc_msgSend(v11, "setValue:forKey:", v46, v49, swift_bridgeObjectRelease(v48).n128_f64[0]);
    objc_release(v46);
    objc_release(v49);
    result = objc_retainAutoreleasedReturnValue(objc_msgSend(v11, "outputImage"));
    if ( result )
      return v45;
  }
  else
  {
    __break(1u);
  }
  __break(1u);
  return result;
}

/* ========================================================================
 * sub_1002183B0
 * EA: 0x1002183b0
 ======================================================================== */

id __fastcall sub_1002183B0(__int64 a1, __int64 a2)
{
  __int64 v2; // x20
  NSString v4; // x25
  Class isa; // x22
  id v6; // x20
  id v7; // x22
  id v8; // x0
  id v9; // x23
  id v11; // [xsp+0h] [xbp-40h] BYREF

  v4 = String._bridgeToObjectiveC()();
  swift_bridgeObjectRelease(a2);
  isa = Data._bridgeToObjectiveC()().super.isa;
  v11 = nullptr;
  v6 = objc_retainAutoreleasedReturnValue(
         objc_msgSend(
           (id)swift_getObjCClassFromMetadata(v2),
           "kernelWithFunctionName:fromMetalLibraryData:error:",
           v4,
           isa,
           &v11));
  objc_release(v4);
  objc_release(isa);
  v7 = v11;
  if ( v6 )
  {
    v8 = objc_retain(v11);
  }
  else
  {
    v9 = objc_retain(v11);
    _convertNSErrorToError(_:)(v7);
    objc_release(v9);
    swift_willThrow();
  }
  return v6;
}

/* ========================================================================
 * sub_1002184C4
 * EA: 0x1002184c4
 ======================================================================== */

_QWORD *__fastcall sub_1002184C4(__int64 a1, unsigned __int64 a2)
{
  unsigned __int64 v4; // x10
  __int64 v5; // x25
  __int64 v6; // x8
  __int64 v7; // x9
  _QWORD *v8; // x24
  __int64 v9; // x0
  int v10; // w20
  __int64 v11; // x8
  unsigned __int64 v12; // x27
  unsigned __int64 v13; // x26
  __int64 v14; // x19
  __int64 v15; // x0
  __int64 v16; // x28
  __int64 v17; // x0
  __int64 v18; // x8
  __int64 v19; // x0
  __int64 v20; // x0
  char v21; // w19
  __int64 v22; // x0
  __int64 v23; // x0
  __int64 v24; // x1
  unsigned __int64 v25; // x8
  unsigned __int64 v26; // x19
  _QWORD *v27; // x8
  __int64 v28; // x24
  __int64 v30; // [xsp+0h] [xbp-D0h]
  char *v31; // [xsp+10h] [xbp-C0h]
  unsigned __int64 v32; // [xsp+30h] [xbp-A0h]
  unsigned __int64 v33; // [xsp+38h] [xbp-98h]
  __int64 v34; // [xsp+58h] [xbp-78h]
  __int16 v35; // [xsp+62h] [xbp-6Eh] BYREF
  char v36; // [xsp+64h] [xbp-6Ch]
  char v37; // [xsp+65h] [xbp-6Bh]
  char v38; // [xsp+66h] [xbp-6Ah]
  char v39; // [xsp+67h] [xbp-69h]
  char v40; // [xsp+68h] [xbp-68h]
  char v41; // [xsp+69h] [xbp-67h]
  __int16 v42; // [xsp+6Ah] [xbp-66h]
  char v43; // [xsp+6Ch] [xbp-64h]
  char v44; // [xsp+6Dh] [xbp-63h]
  char v45; // [xsp+6Eh] [xbp-62h]
  char v46; // [xsp+6Fh] [xbp-61h]
  _QWORD *v47; // [xsp+70h] [xbp-60h]

  v4 = a2 >> 62;
  v32 = HIDWORD(a1);
  if ( (int)(a2 >> 62) <= 1 )
  {
    if ( !(_DWORD)v4 )
    {
      v5 = BYTE6(a2);
      goto LABEL_10;
    }
    goto LABEL_8;
  }
  if ( (_DWORD)v4 != 2 )
    return _swiftEmptyArrayStorage;
  v7 = *(_QWORD *)(a1 + 16);
  v6 = *(_QWORD *)(a1 + 24);
  v5 = v6 - v7;
  if ( __OFSUB__(v6, v7) )
  {
    __break(1u);
LABEL_8:
    if ( __OFSUB__(HIDWORD(a1), (_DWORD)a1) )
      goto LABEL_49;
    v5 = HIDWORD(a1) - (int)a1;
  }
LABEL_10:
  v8 = _swiftEmptyArrayStorage;
  if ( v5 )
  {
    v47 = _swiftEmptyArrayStorage;
    v33 = a2 >> 62;
    v9 = sub_1001306E0(0, v5 & ~(v5 >> 63), 0);
    v10 = v33;
    if ( (_DWORD)v33 )
    {
      if ( (_DWORD)v33 == 2 )
        v11 = *(_QWORD *)(a1 + 16);
      else
        v11 = (int)a1;
    }
    else
    {
      v11 = 0;
    }
    v34 = v11;
    if ( v5 < 0 )
      goto LABEL_48;
    v12 = 0;
    v8 = v47;
    v31 = (char *)&v35 + v11;
    do
    {
      if ( v12 >= v5 )
      {
        __break(1u);
LABEL_41:
        __break(1u);
LABEL_42:
        __break(1u);
LABEL_43:
        __break(1u);
LABEL_44:
        __break(1u);
LABEL_45:
        __break(1u);
LABEL_46:
        __break(1u);
LABEL_47:
        __break(1u);
LABEL_48:
        __break(1u);
LABEL_49:
        __break(1u);
LABEL_50:
        __break(1u);
LABEL_51:
        __break(1u);
      }
      v13 = v12 + 1;
      if ( __OFADD__(v12, 1) )
        goto LABEL_41;
      v14 = v34 + v12;
      if ( v10 == 2 )
      {
        if ( v14 < *(_QWORD *)(a1 + 16) )
          goto LABEL_43;
        if ( v14 >= *(_QWORD *)(a1 + 24) )
          goto LABEL_45;
        v19 = __DataStorage._bytes.getter(v9);
        if ( !v19 )
          goto LABEL_51;
        v16 = v19;
        v20 = __DataStorage._offset.getter();
        v18 = v14 - v20;
        if ( __OFSUB__(v14, v20) )
          goto LABEL_47;
      }
      else
      {
        if ( v10 != 1 )
        {
          if ( v14 >= BYTE6(a2) )
            goto LABEL_42;
          v35 = a1;
          v36 = BYTE2(a1);
          v37 = BYTE3(a1);
          v38 = v32;
          v39 = BYTE5(a1);
          v40 = BYTE6(a1);
          v41 = HIBYTE(a1);
          v42 = a2;
          v43 = BYTE2(a2);
          v44 = BYTE3(a2);
          v45 = BYTE4(a2);
          v46 = BYTE5(a2);
          v21 = v31[v12];
          goto LABEL_36;
        }
        if ( v14 < (int)a1 || v14 >= a1 >> 32 )
          goto LABEL_44;
        v15 = __DataStorage._bytes.getter(v9);
        if ( !v15 )
          goto LABEL_50;
        v16 = v15;
        v17 = __DataStorage._offset.getter();
        v18 = v14 - v17;
        if ( __OFSUB__(v14, v17) )
          goto LABEL_46;
      }
      v21 = *(_BYTE *)(v16 + v18);
      v10 = v33;
LABEL_36:
      v22 = sub_10003E4E0((__int64 *)&unk_10066AF90, (__int64 *)&unk_10053C880);
      v23 = swift_allocObject(v22, 72, 7);
      *(_OWORD *)(v23 + 16) = xmmword_10053B940;
      *(_QWORD *)(v23 + 56) = &type metadata for UInt8;
      *(_QWORD *)(v23 + 64) = &protocol witness table for UInt8;
      *(_BYTE *)(v23 + 32) = v21;
      v9 = String.init(format:_:)(0x786868323025LL, 0xE600000000000000LL, v23);
      v47 = v8;
      v26 = v8[2];
      v25 = v8[3];
      if ( v26 >= v25 >> 1 )
      {
        v30 = v9;
        v28 = v24;
        sub_1001306E0(v25 > 1, v26 + 1, 1);
        v10 = v33;
        v24 = v28;
        v9 = v30;
        v8 = v47;
      }
      v8[2] = v26 + 1;
      v27 = &v8[2 * v26];
      v27[4] = v9;
      v27[5] = v24;
      ++v12;
    }
    while ( v13 != v5 );
  }
  return v8;
}

/* ========================================================================
 * sub_10021883C
 * EA: 0x10021883c
 ======================================================================== */

__int64 __fastcall sub_10021883C(__int64 a1, __int64 a2, __int64 a3, __int64 a4)
{
  __int64 v8; // x19
  size_t v9; // x25
  size_t v10; // x26
  __int64 v11; // x22
  __int64 v12; // x20
  __int64 v13; // x0
  __int64 v14; // x22
  unsigned __int64 v15; // x1
  unsigned __int64 v16; // x23
  _QWORD *v17; // x21
  __int64 v18; // x19
  __int64 v19; // x0
  __int64 v20; // x19

  v8 = static Array._allocateBufferUninitialized(minimumCapacity:)(20, &type metadata for UInt8);
  *(_QWORD *)(v8 + 16) = 20;
  *(_QWORD *)(v8 + 40) = 0;
  *(_DWORD *)(v8 + 48) = 0;
  *(_QWORD *)(v8 + 32) = 0;
  v9 = String.count.getter(a1, a2);
  v10 = String.count.getter(a3, a4);
  v11 = String.utf8CString.getter(a1, a2);
  v12 = String.utf8CString.getter(a3, a4);
  CCHmac(0, (const void *)(v11 + 32), v9, (const void *)(v12 + 32), v10, (void *)(v8 + 32));
  swift_release(v11);
  swift_release(v12);
  v13 = swift_bridgeObjectRetain(v8);
  v14 = sub_1001D4764(v13);
  v16 = v15;
  v17 = sub_1002184C4(v14, v15);
  swift_bridgeObjectRelease(v8);
  v18 = sub_10003E4E0(&qword_1006729A0, &qword_10053C610);
  v19 = sub_100101F98(v18);
  v20 = BidirectionalCollection<>.joined(separator:)(0, 0xE000000000000000LL, v18, v19);
  sub_100040E14(v14, v16);
  swift_bridgeObjectRelease(v17);
  return v20;
}

/* ========================================================================
 * sub_1002189A0
 * EA: 0x1002189a0
 ======================================================================== */

unsigned __int8 *__usercall sub_1002189A0@<X0>(
        unsigned __int8 *result@<X0>,
        __int64 a2@<X1>,
        unsigned __int64 a3@<X2>,
        __int64 *a4@<X3>,
        unsigned __int8 **a5@<X8>)
{
  unsigned __int8 *v6; // x22
  unsigned __int64 v8; // x8
  unsigned __int64 v9; // x24
  __int64 v10; // x8
  __int64 v11; // x9
  __int64 v12; // x25
  char isUniquelyReferenced_nonNull_native; // w0

  v6 = result;
  v8 = a3 >> 62;
  if ( (int)(a3 >> 62) > 1 )
  {
    if ( (_DWORD)v8 != 2 )
      goto LABEL_13;
    v11 = *(_QWORD *)(a2 + 16);
    v10 = *(_QWORD *)(a2 + 24);
    v9 = v10 - v11;
    if ( !__OFSUB__(v10, v11) )
      goto LABEL_10;
    __break(1u);
LABEL_8:
    if ( __OFSUB__(HIDWORD(a2), (_DWORD)a2) )
      goto LABEL_18;
    v9 = HIDWORD(a2) - (int)a2;
LABEL_10:
    if ( (v9 & 0x8000000000000000LL) == 0 )
    {
      if ( !HIDWORD(v9) )
        goto LABEL_14;
      __break(1u);
LABEL_13:
      LODWORD(v9) = 0;
      goto LABEL_14;
    }
    __break(1u);
LABEL_18:
    __break(1u);
    return result;
  }
  if ( (_DWORD)v8 )
    goto LABEL_8;
  LODWORD(v9) = BYTE6(a3);
LABEL_14:
  v12 = *a4;
  isUniquelyReferenced_nonNull_native = swift_isUniquelyReferenced_nonNull_native(*a4);
  *a4 = v12;
  if ( (isUniquelyReferenced_nonNull_native & 1) == 0 )
    v12 = sub_100248D90(0, *(_QWORD *)(v12 + 16), 0, v12);
  *a4 = v12;
  result = CC_MD5(v6, v9, (unsigned __int8 *)(v12 + 32));
  *a5 = result;
  return result;
}

/* ========================================================================
 * sub_100218A7C
 * EA: 0x100218a7c
 ======================================================================== */

_QWORD *__fastcall sub_100218A7C(__int64 a1, unsigned __int64 a2)
{
  __int64 v4; // x21
  __int64 v5; // x23
  char *v6; // x22
  __int64 v7; // x0
  __int64 v8; // x0
  __int64 v9; // x20
  _QWORD *result; // x0
  unsigned __int8 *v11; // x22
  unsigned __int64 v12; // x8
  _QWORD *v13; // x23
  __int64 v14; // x28
  __int64 v15; // x20
  unsigned __int64 v16; // x23
  char v17; // t1
  __int64 v18; // x0
  __int64 v19; // x0
  __int64 v20; // x26
  void *v21; // x1
  void *v22; // x27
  Swift::String v23; // x0
  __int64 v24; // [xsp+0h] [xbp-80h] BYREF
  _QWORD *v25; // [xsp+8h] [xbp-78h]
  __int128 v26; // [xsp+10h] [xbp-70h]
  __int64 v27; // [xsp+20h] [xbp-60h]
  unsigned __int64 v28; // [xsp+28h] [xbp-58h]

  v4 = type metadata accessor for String.Encoding(0);
  v5 = *(_QWORD *)(v4 - 8);
  v6 = (char *)&v24 - ((*(_QWORD *)(v5 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v27 = a1;
  v28 = a2;
  v7 = static String.Encoding.utf8.getter(v4);
  v8 = sub_100056930(v7);
  v9 = StringProtocol.cString(using:)(v6, &type metadata for String, v8);
  (*(void (__fastcall **)(char *, __int64))(v5 + 8))(v6, v4);
  result = (_QWORD *)static Array._allocateBufferUninitialized(minimumCapacity:)(16, &type metadata for UInt8);
  result[2] = 16;
  result[4] = 0;
  v11 = (unsigned __int8 *)(result + 4);
  result[5] = 0;
  if ( !v9 )
    goto LABEL_12;
  v12 = *(_QWORD *)(v9 + 16);
  if ( !v12 )
  {
    __break(1u);
    goto LABEL_11;
  }
  if ( v12 > (unsigned __int64)&_mh_execute_header )
  {
LABEL_11:
    __break(1u);
LABEL_12:
    __break(1u);
    return result;
  }
  v13 = result;
  CC_MD5((const void *)(v9 + 32), v12 - 1, v11);
  swift_bridgeObjectRelease(v9);
  v25 = v13;
  v14 = v13[2];
  if ( v14 )
  {
    v15 = 0;
    v16 = 0xE000000000000000LL;
    v26 = xmmword_10053B940;
    do
    {
      v17 = *v11++;
      v18 = sub_10003E4E0((__int64 *)&unk_10066AF90, (__int64 *)&unk_10053C880);
      v19 = swift_allocObject(v18, 72, 7);
      *(_OWORD *)(v19 + 16) = v26;
      *(_QWORD *)(v19 + 56) = &type metadata for UInt8;
      *(_QWORD *)(v19 + 64) = &protocol witness table for UInt8;
      *(_BYTE *)(v19 + 32) = v17;
      v20 = String.init(format:_:)(2016555045, 0xE400000000000000LL, v19);
      v22 = v21;
      v27 = v15;
      v28 = v16;
      swift_bridgeObjectRetain(v16);
      v23._countAndFlagsBits = v20;
      v23._object = v22;
      String.append(_:)(v23);
      swift_bridgeObjectRelease(v16);
      swift_bridgeObjectRelease(v22);
      v15 = v27;
      v16 = v28;
      --v14;
    }
    while ( v14 );
  }
  else
  {
    v15 = 0;
  }
  swift_bridgeObjectRelease(v25);
  return (_QWORD *)v15;
}

/* ========================================================================
 * sub_100218C74
 * EA: 0x100218c74
 ======================================================================== */

__int64 __fastcall sub_100218C74(__int64 a1, unsigned __int64 a2)
{
  __int64 v4; // x22
  unsigned __int64 v5; // x1
  unsigned __int64 v6; // x23
  __int64 v7; // x0
  __int64 v8; // x21
  __int64 v9; // x25
  char *v10; // x23
  char v11; // t1
  __int64 v12; // x0
  __int64 v13; // x0
  __int64 v14; // x21
  __int64 v15; // x1
  __int64 v16; // x19
  unsigned __int64 v17; // x8
  unsigned __int64 v18; // x27
  _QWORD *v19; // x8
  __int64 v20; // x19
  __int64 v21; // x0
  __int64 v22; // x19
  __int64 v24; // [xsp+8h] [xbp-98h]
  unsigned __int64 v25; // [xsp+10h] [xbp-90h]
  __int64 v26; // [xsp+48h] [xbp-58h] BYREF

  swift_bridgeObjectRetain(a2);
  v4 = sub_1001D4894(a1, a2);
  v6 = v5;
  v7 = static Array._allocateBufferUninitialized(minimumCapacity:)(20, &type metadata for UInt8);
  *(_QWORD *)(v7 + 16) = 20;
  *(_QWORD *)(v7 + 32) = 0;
  *(_QWORD *)(v7 + 40) = 0;
  *(_DWORD *)(v7 + 48) = 0;
  v26 = v7;
  sub_10004B430(v4, v6);
  sub_100218F68(v4, v6, v4, v6, &v26);
  v8 = v26;
  v9 = *(_QWORD *)(v26 + 16);
  if ( v9 )
  {
    v25 = v6;
    sub_1001306E0(0, v9, 0);
    v24 = v8;
    v10 = (char *)(v8 + 32);
    do
    {
      v11 = *v10++;
      v12 = sub_10003E4E0((__int64 *)&unk_10066AF90, (__int64 *)&unk_10053C880);
      v13 = swift_allocObject(v12, 72, 7);
      *(_OWORD *)(v13 + 16) = xmmword_10053B940;
      *(_QWORD *)(v13 + 56) = &type metadata for UInt8;
      *(_QWORD *)(v13 + 64) = &protocol witness table for UInt8;
      *(_BYTE *)(v13 + 32) = v11;
      v14 = String.init(format:_:)(0x786868323025LL, 0xE600000000000000LL, v13);
      v16 = v15;
      v18 = _swiftEmptyArrayStorage[2];
      v17 = _swiftEmptyArrayStorage[3];
      if ( v18 >= v17 >> 1 )
        sub_1001306E0(v17 > 1, v18 + 1, 1);
      _swiftEmptyArrayStorage[2] = v18 + 1;
      v19 = &_swiftEmptyArrayStorage[2 * v18];
      v19[4] = v14;
      v19[5] = v16;
      --v9;
    }
    while ( v9 );
    v6 = v25;
    v8 = v24;
  }
  v20 = sub_10003E4E0(&qword_1006729A0, &qword_10053C610);
  v21 = sub_100101F98(v20);
  v22 = BidirectionalCollection<>.joined(separator:)(0, 0xE000000000000000LL, v20, v21);
  sub_100040E14(v4, v6);
  swift_bridgeObjectRelease(v8);
  swift_bridgeObjectRelease(_swiftEmptyArrayStorage);
  return v22;
}

/* ========================================================================
 * sub_100218E94
 * EA: 0x100218e94
 ======================================================================== */

unsigned __int8 *__fastcall sub_100218E94(
        unsigned __int8 *result,
        __int64 a2,
        __int64 a3,
        unsigned __int64 a4,
        __int64 *a5)
{
  unsigned __int8 *v6; // x20
  unsigned __int64 v7; // x8
  unsigned __int64 v8; // x23
  __int64 v9; // x8
  __int64 v10; // x9
  __int64 v11; // x24
  char isUniquelyReferenced_nonNull_native; // w0

  v6 = result;
  v7 = a4 >> 62;
  if ( (int)(a4 >> 62) > 1 )
  {
    if ( (_DWORD)v7 != 2 )
      goto LABEL_13;
    v10 = *(_QWORD *)(a3 + 16);
    v9 = *(_QWORD *)(a3 + 24);
    v8 = v9 - v10;
    if ( !__OFSUB__(v9, v10) )
      goto LABEL_10;
    __break(1u);
LABEL_8:
    if ( __OFSUB__(HIDWORD(a3), (_DWORD)a3) )
      goto LABEL_18;
    v8 = HIDWORD(a3) - (int)a3;
LABEL_10:
    if ( (v8 & 0x8000000000000000LL) == 0 )
    {
      if ( !HIDWORD(v8) )
        goto LABEL_14;
      __break(1u);
LABEL_13:
      LODWORD(v8) = 0;
      goto LABEL_14;
    }
    __break(1u);
LABEL_18:
    __break(1u);
    return result;
  }
  if ( (_DWORD)v7 )
    goto LABEL_8;
  LODWORD(v8) = BYTE6(a4);
LABEL_14:
  v11 = *a5;
  isUniquelyReferenced_nonNull_native = swift_isUniquelyReferenced_nonNull_native(*a5);
  *a5 = v11;
  if ( (isUniquelyReferenced_nonNull_native & 1) == 0 )
    v11 = sub_100248D90(0, *(_QWORD *)(v11 + 16), 0, v11);
  *a5 = v11;
  return CC_SHA1(v6, v8, (unsigned __int8 *)(v11 + 32));
}

/* ========================================================================
 * sub_100218F68
 * EA: 0x100218f68
 ======================================================================== */

__int64 __fastcall sub_100218F68(__int64 a1, unsigned __int64 a2, __int64 a3, unsigned __int64 a4, __int64 *a5)
{
  unsigned __int64 v5; // x19
  __int64 v6; // x22
  unsigned __int64 v7; // x9
  char *v8; // x1
  unsigned __int8 *v9; // x0
  __int64 *v10; // x25
  __int64 v11; // x24
  __int64 v12; // x28
  __int64 v13; // x27
  __int64 v14; // x0
  bool v15; // vf
  __int64 v16; // x24
  __int64 v17; // x0
  __int64 v18; // x8
  __int64 v19; // x24
  __int64 v20; // x28
  __int64 v21; // x0
  __int64 v22; // x0
  char *v23; // x8
  _QWORD v25[2]; // [xsp+8h] [xbp-68h] BYREF

  v5 = a4;
  v6 = a3;
  v7 = a2 >> 62;
  if ( (int)(a2 >> 62) <= 1 )
  {
    if ( !(_DWORD)v7 )
    {
      v25[0] = a1;
      LOWORD(v25[1]) = a2;
      BYTE2(v25[1]) = BYTE2(a2);
      BYTE3(v25[1]) = BYTE3(a2);
      BYTE4(v25[1]) = BYTE4(a2);
      BYTE5(v25[1]) = BYTE5(a2);
      v8 = (char *)v25 + BYTE6(a2);
      v9 = (unsigned __int8 *)v25;
      goto LABEL_24;
    }
    v19 = (int)a1;
    v20 = (a1 >> 32) - (int)a1;
    if ( a1 >> 32 >= (int)a1 )
    {
      v10 = a5;
      v13 = __DataStorage._bytes.getter(a1);
      if ( !v13 )
        goto LABEL_16;
      v21 = __DataStorage._offset.getter();
      if ( !__OFSUB__(v19, v21) )
      {
        v13 += v19 - v21;
LABEL_16:
        v22 = __DataStorage._length.getter();
        if ( v22 >= v20 )
          v18 = v20;
        else
          v18 = v22;
        goto LABEL_19;
      }
LABEL_28:
      __break(1u);
    }
    __break(1u);
    goto LABEL_26;
  }
  if ( (_DWORD)v7 != 2 )
  {
    memset(v25, 0, 14);
    v9 = (unsigned __int8 *)v25;
    v8 = (char *)v25;
    goto LABEL_24;
  }
  v10 = a5;
  v12 = *(_QWORD *)(a1 + 16);
  v11 = *(_QWORD *)(a1 + 24);
  v13 = __DataStorage._bytes.getter(a1);
  if ( v13 )
  {
    v14 = __DataStorage._offset.getter();
    if ( __OFSUB__(v12, v14) )
    {
LABEL_27:
      __break(1u);
      goto LABEL_28;
    }
    v13 += v12 - v14;
  }
  v15 = __OFSUB__(v11, v12);
  v16 = v11 - v12;
  if ( v15 )
  {
LABEL_26:
    __break(1u);
    goto LABEL_27;
  }
  v17 = __DataStorage._length.getter();
  if ( v17 >= v16 )
    v18 = v16;
  else
    v18 = v17;
LABEL_19:
  v23 = (char *)(v18 + v13);
  if ( v13 )
    v8 = v23;
  else
    v8 = nullptr;
  v9 = (unsigned __int8 *)v13;
  a3 = v6;
  a4 = v5;
  a5 = v10;
LABEL_24:
  sub_100218E94(v9, (__int64)v8, a3, a4, a5);
  return sub_100040E14(v6, v5);
}

/* ========================================================================
 * sub_10021917C
 * EA: 0x10021917c
 ======================================================================== */

_QWORD *__fastcall sub_10021917C(__int64 a1, __int64 a2)
{
  _QWORD *v4; // x0
  __int64 v5; // x0
  _QWORD *result; // x0
  _QWORD *v7; // x21
  __int64 i; // x25
  __int64 v9; // x0
  __int64 v10; // x0
  Swift::String v11; // x0
  void *object; // x24
  char v13[16]; // [xsp+10h] [xbp-90h] BYREF
  __int64 v14; // [xsp+20h] [xbp-80h]
  __int64 v15; // [xsp+28h] [xbp-78h]
  _QWORD *v16; // [xsp+30h] [xbp-70h]
  _QWORD v17[3]; // [xsp+40h] [xbp-60h] BYREF
  _QWORD *v18; // [xsp+58h] [xbp-48h] BYREF

  v4 = (_QWORD *)static Array._allocateBufferUninitialized(minimumCapacity:)(16, &type metadata for UInt8);
  v4[2] = 16;
  v4[4] = 0;
  v4[5] = 0;
  v18 = v4;
  v14 = a1;
  v15 = a2;
  v16 = &v18;
  v5 = sub_10003E4E0(&qword_100671B20, &qword_1005442E8);
  result = Data.withUnsafeBytes<A, B>(_:)(v17, sub_1002192E0, v13, a1, a2, v5, (char *)&type metadata for () + 8);
  v17[0] = 0;
  v17[1] = 0xE000000000000000LL;
  v7 = v18;
  if ( v18[2] < 0x10u )
  {
    __break(1u);
  }
  else
  {
    for ( i = 32; i != 48; ++i )
    {
      v9 = sub_10003E4E0((__int64 *)&unk_10066AF90, (__int64 *)&unk_10053C880);
      v10 = swift_allocObject(v9, 72, 7);
      *(_OWORD *)(v10 + 16) = xmmword_10053B940;
      *(_QWORD *)(v10 + 56) = &type metadata for UInt8;
      *(_QWORD *)(v10 + 64) = &protocol witness table for UInt8;
      *(_BYTE *)(v10 + 32) = *((_BYTE *)v7 + i);
      v11._countAndFlagsBits = String.init(format:_:)(2016555045, 0xE400000000000000LL, v10);
      object = v11._object;
      String.append(_:)(v11);
      swift_bridgeObjectRelease(object);
    }
    swift_bridgeObjectRelease(v7);
    return (_QWORD *)v17[0];
  }
  return result;
}

/* ========================================================================
 * sub_1002192FC
 * EA: 0x1002192fc
 ======================================================================== */

_QWORD *__fastcall sub_1002192FC(__int64 a1)
{
  __int64 v2; // x19
  _QWORD *v3; // x20
  __int64 i; // x21
  __int64 v5; // x0
  __int64 v6; // x26
  unsigned __int64 v7; // x8
  unsigned __int64 v8; // x27
  __int64 v10; // [xsp+0h] [xbp-80h] BYREF
  _BYTE v11[32]; // [xsp+8h] [xbp-78h] BYREF
  _QWORD *v12; // [xsp+28h] [xbp-58h]

  v2 = *(_QWORD *)(a1 + 16);
  v12 = _swiftEmptyArrayStorage;
  sub_100130788(0, v2, 0);
  v3 = _swiftEmptyArrayStorage;
  if ( v2 )
  {
    for ( i = a1 + 32; ; i += 32 )
    {
      sub_100047B64(i, v11);
      v5 = sub_10003E4E0(&qword_100668720, &qword_10053CF20);
      if ( (swift_dynamicCast(&v10, v11, (char *)&type metadata for Any + 8, v5, 6) & 1) == 0 )
        break;
      v6 = v10;
      v12 = v3;
      v8 = v3[2];
      v7 = v3[3];
      if ( v8 >= v7 >> 1 )
      {
        sub_100130788(v7 > 1, v8 + 1, 1);
        v3 = v12;
      }
      v3[2] = v8 + 1;
      v3[v8 + 4] = v6;
      if ( !--v2 )
        return v3;
    }
    swift_release(v3);
    return nullptr;
  }
  return v3;
}

/* ========================================================================
 * sub_100219420
 * EA: 0x100219420
 ======================================================================== */

_QWORD *__fastcall sub_100219420(__int64 a1)
{
  __int64 v2; // x19
  _QWORD *v3; // x20
  __int64 i; // x21
  __int64 v5; // x25
  unsigned __int64 v6; // x8
  unsigned __int64 v7; // x26
  __int64 v9; // [xsp+8h] [xbp-78h] BYREF
  _BYTE v10[32]; // [xsp+18h] [xbp-68h] BYREF
  _QWORD *v11; // [xsp+38h] [xbp-48h]

  v2 = *(_QWORD *)(a1 + 16);
  v11 = _swiftEmptyArrayStorage;
  sub_1001307C0(0, v2, 0);
  v3 = _swiftEmptyArrayStorage;
  if ( v2 )
  {
    for ( i = a1 + 32; ; i += 32 )
    {
      sub_100047B64(i, v10);
      if ( (swift_dynamicCast(&v9, v10, (char *)&type metadata for Any + 8, &type metadata for Int, 6) & 1) == 0 )
        break;
      v5 = v9;
      v11 = v3;
      v7 = v3[2];
      v6 = v3[3];
      if ( v7 >= v6 >> 1 )
      {
        sub_1001307C0(v6 > 1, v7 + 1, 1);
        v3 = v11;
      }
      v3[2] = v7 + 1;
      v3[v7 + 4] = v5;
      if ( !--v2 )
        return v3;
    }
    swift_release(v3);
    return nullptr;
  }
  return v3;
}

/* ========================================================================
 * sub_100219528
 * EA: 0x100219528
 ======================================================================== */

_QWORD *__fastcall sub_100219528(__int64 a1)
{
  __int64 v2; // x19
  _QWORD *v3; // x20
  __int64 i; // x21
  __int64 v5; // x25
  __int64 v6; // x26
  unsigned __int64 v7; // x8
  unsigned __int64 v8; // x27
  _QWORD *v9; // x8
  _QWORD v11[2]; // [xsp+8h] [xbp-88h] BYREF
  _BYTE v12[32]; // [xsp+18h] [xbp-78h] BYREF
  _QWORD *v13; // [xsp+38h] [xbp-58h]

  v2 = *(_QWORD *)(a1 + 16);
  v13 = _swiftEmptyArrayStorage;
  sub_1001306E0(0, v2, 0);
  v3 = _swiftEmptyArrayStorage;
  if ( v2 )
  {
    for ( i = a1 + 32; ; i += 32 )
    {
      sub_100047B64(i, v12);
      if ( (swift_dynamicCast(v11, v12, (char *)&type metadata for Any + 8, &type metadata for String, 6) & 1) == 0 )
        break;
      v5 = v11[0];
      v6 = v11[1];
      v13 = v3;
      v8 = v3[2];
      v7 = v3[3];
      if ( v8 >= v7 >> 1 )
      {
        sub_1001306E0(v7 > 1, v8 + 1, 1);
        v3 = v13;
      }
      v3[2] = v8 + 1;
      v9 = &v3[2 * v8];
      v9[4] = v5;
      v9[5] = v6;
      if ( !--v2 )
        return v3;
    }
    swift_release(v3);
    return nullptr;
  }
  return v3;
}

/* ========================================================================
 * sub_100219644
 * EA: 0x100219644
 ======================================================================== */

_QWORD *__fastcall sub_100219644(__int64 a1)
{
  __int64 i; // x21
  unsigned __int64 j; // x22
  id v4; // x0
  void *v5; // x20
  unsigned __int64 v6; // x28
  __int64 v7; // x0
  __int64 v8; // x0
  __n128 v9; // q0
  __int64 v10; // x23
  __int64 v11; // x24
  __int64 v12; // x0
  __int64 v14; // x21
  __int64 v15; // x0

  if ( (unsigned __int64)a1 >> 62 )
    goto LABEL_15;
  specialized ContiguousArray.reserveCapacity(_:)(*(_QWORD *)((a1 & 0xFFFFFFFFFFFFFF8LL) + 0x10));
  for ( i = *(_QWORD *)((a1 & 0xFFFFFFFFFFFFFF8LL) + 0x10); i; i = _CocoaArrayWrapper.endIndex.getter(v14) )
  {
    for ( j = 0; ; ++j )
    {
      if ( (a1 & 0xC000000000000001LL) != 0 )
      {
        v4 = (id)specialized _ArrayBuffer._getElementSlowPath(_:)(j, a1);
      }
      else
      {
        if ( j >= *(_QWORD *)((a1 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
          goto LABEL_14;
        v4 = objc_retain(*(id *)(a1 + 8 * j + 32));
      }
      v5 = v4;
      v6 = j + 1;
      if ( __OFADD__(j, 1) )
        break;
      v7 = objc_opt_self(&OBJC_CLASS___UIButton);
      v8 = swift_dynamicCastObjCClass(v5, v7);
      if ( !v8 )
      {
        objc_release(v5);
        swift_release(_swiftEmptyArrayStorage);
        return nullptr;
      }
      v10 = v8;
      specialized ContiguousArray._makeUniqueAndReserveCapacityIfNotUnique()(v9);
      v11 = _swiftEmptyArrayStorage[2];
      specialized ContiguousArray._reserveCapacityAssumingUniqueBuffer(oldCount:)(v11);
      v12 = specialized ContiguousArray._appendElementAssumeUniqueAndCapacity(_:newElement:)(v11, v10);
      specialized ContiguousArray._endMutation()(v12);
      if ( v6 == i )
        return _swiftEmptyArrayStorage;
    }
    __break(1u);
LABEL_14:
    __break(1u);
LABEL_15:
    if ( a1 < 0 )
      v14 = a1;
    else
      v14 = a1 & 0xFFFFFFFFFFFFFF8LL;
    v15 = _CocoaArrayWrapper.endIndex.getter(v14);
    specialized ContiguousArray.reserveCapacity(_:)(v15);
  }
  return _swiftEmptyArrayStorage;
}

/* ========================================================================
 * sub_1002197C0
 * EA: 0x1002197c0
 ======================================================================== */

_QWORD *__fastcall sub_1002197C0(__int64 a1, __int64 (__fastcall *a2)(_QWORD))
{
  __int64 v4; // x19
  __int64 *v5; // x24
  __int64 v6; // x21
  __int64 v7; // x20
  __int64 v8; // t1
  __int64 v9; // x0
  __int64 v10; // x22
  __n128 v11; // q0
  __int64 v12; // x23
  __int64 v13; // x0

  v4 = *(_QWORD *)(a1 + 16);
  specialized ContiguousArray.reserveCapacity(_:)(v4);
  if ( !v4 )
    return _swiftEmptyArrayStorage;
  v5 = (__int64 *)(a1 + 32);
  v6 = a2(0);
  while ( 1 )
  {
    v8 = *v5++;
    v7 = v8;
    v9 = swift_dynamicCastClass(v8, v6);
    if ( !v9 )
      break;
    v10 = v9;
    v11 = swift_retain(v7);
    specialized ContiguousArray._makeUniqueAndReserveCapacityIfNotUnique()(v11);
    v12 = _swiftEmptyArrayStorage[2];
    specialized ContiguousArray._reserveCapacityAssumingUniqueBuffer(oldCount:)(v12);
    v13 = specialized ContiguousArray._appendElementAssumeUniqueAndCapacity(_:newElement:)(v12, v10);
    specialized ContiguousArray._endMutation()(v13);
    if ( !--v4 )
      return _swiftEmptyArrayStorage;
  }
  swift_release(_swiftEmptyArrayStorage);
  return nullptr;
}

/* ========================================================================
 * +[_TtC15ExternalMonitor17VTFavoriteManager shared]
 * EA: 0x1002198c4
 ======================================================================== */

_TtC15ExternalMonitor17VTFavoriteManager *__cdecl +[VTFavoriteManager shared](id a1, SEL a2)
{
  if ( qword_100662560 != -1 )
    swift_once(&qword_100662560, sub_100219898);
  return (_TtC15ExternalMonitor17VTFavoriteManager *)objc_retainAutoreleaseReturnValue((id)qword_100697158);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor17VTFavoriteManager init]
 * EA: 0x100219904
 ======================================================================== */

_TtC15ExternalMonitor17VTFavoriteManager *__cdecl -[VTFavoriteManager init](
        _TtC15ExternalMonitor17VTFavoriteManager *self,
        SEL a2)
{
  _TtC15ExternalMonitor17VTFavoriteManager *v2; // x20
  objc_super v4; // [xsp+0h] [xbp-20h] BYREF

  *(Class *)((char *)&self->super.isa + OBJC_IVAR____TtC15ExternalMonitor17VTFavoriteManager_favorList) = (Class)_swiftEmptyArrayStorage;
  v4.receiver = self;
  v4.super_class = (Class)type metadata accessor for VTFavoriteManager();
  v2 = objc_retainAutoreleasedReturnValue(-[VTFavoriteManager init](&v4, "init"));
  sub_10021A2B8();
  objc_release(v2);
  return v2;
}

/* ========================================================================
 * sub_10021996C
 * EA: 0x10021996c
 ======================================================================== */

void __fastcall sub_10021996C(__int64 a1)
{
  __int64 v1; // x20
  __int64 v2; // x22
  __int64 v3; // x19
  __int64 v5; // x25
  __int64 v6; // x20
  __int64 v7; // x24
  unsigned __int64 v8; // x23
  unsigned __int64 v9; // x28
  __int64 v10; // x1
  __int64 v11; // x3
  __int64 v12; // x0
  __int64 v13; // x2
  __int64 v14; // x8
  __n128 v15; // q0
  __int64 v16; // x19
  __int64 v17; // x0
  unsigned __int64 v18; // x1
  unsigned __int64 v19; // x20
  id v20; // x21
  NSString v21; // x19
  NSString v22; // x20
  NSString v23; // x19
  NSString v24; // x20
  __int64 v25; // x0
  __int64 v26; // x8
  __n128 v27; // q0
  __int64 v28; // x19
  __int64 v29; // x0
  unsigned __int64 v30; // x1
  unsigned __int64 v31; // x20
  id v32; // x21
  __int64 v33; // x0
  __int64 v34; // x0
  __int64 v35; // x0
  _QWORD v36[3]; // [xsp+0h] [xbp-80h] BYREF
  _BYTE v37[24]; // [xsp+18h] [xbp-68h] BYREF

  v3 = v1;
  v5 = OBJC_IVAR____TtC15ExternalMonitor17VTFavoriteManager_favorList;
  swift_beginAccess(v1 + OBJC_IVAR____TtC15ExternalMonitor17VTFavoriteManager_favorList, v37, 0, 0);
  v6 = *(_QWORD *)(v1 + v5);
  if ( (unsigned __int64)v6 >> 62 )
  {
LABEL_31:
    if ( v6 < 0 )
      v33 = v6;
    else
      v33 = v6 & 0xFFFFFFFFFFFFFF8LL;
    v7 = _CocoaArrayWrapper.endIndex.getter(v33);
  }
  else
  {
    v7 = *(_QWORD *)((v6 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
  }
  swift_bridgeObjectRetain(v6);
  if ( v7 )
  {
    v8 = 0;
    do
    {
      if ( (v6 & 0xC000000000000001LL) != 0 )
      {
        v2 = specialized _ArrayBuffer._getElementSlowPath(_:)(v8, v6);
        v9 = v8 + 1;
        if ( __OFADD__(v8, 1) )
        {
LABEL_18:
          __break(1u);
          break;
        }
      }
      else
      {
        if ( v8 >= *(_QWORD *)((v6 & 0xFFFFFFFFFFFFFF8LL) + 0x10) )
        {
          __break(1u);
          goto LABEL_31;
        }
        v2 = *(_QWORD *)(v6 + 8 * v8 + 32);
        swift_retain(v2);
        v9 = v8 + 1;
        if ( __OFADD__(v8, 1) )
          goto LABEL_18;
      }
      v10 = *(_QWORD *)(a1 + 24);
      v11 = *(_QWORD *)(v2 + 24);
      if ( v10 )
      {
        if ( v11 )
        {
          if ( (v12 = *(_QWORD *)(a1 + 16), v13 = *(_QWORD *)(v2 + 16), v12 == v13) && v10 == v11
            || (_stringCompareWithSmolCheck(_:_:expecting:)(v12, v10, v13, v11, 0) & 1) != 0 )
          {
LABEL_24:
            swift_bridgeObjectRelease(v6);
            swift_beginAccess(v3 + v5, v36, 33, 0);
            v25 = sub_1000CFEC0(v8);
            swift_release(v25);
            v26 = *(_QWORD *)(v3 + v5);
            if ( (unsigned __int64)v26 >> 62 )
              goto LABEL_40;
            goto LABEL_25;
          }
        }
      }
      else if ( !v11 )
      {
        goto LABEL_24;
      }
      swift_release(v2);
      ++v8;
    }
    while ( v9 != v7 );
  }
  swift_bridgeObjectRelease(v6);
  swift_beginAccess(v3 + v5, v36, 33, 0);
  v14 = *(_QWORD *)(v3 + v5);
  if ( !((unsigned __int64)v14 >> 62)
    || (v14 < 0 ? (v34 = *(_QWORD *)(v3 + v5)) : (v34 = v14 & 0xFFFFFFFFFFFFFF8LL),
        (_CocoaArrayWrapper.endIndex.getter(v34) & 0x8000000000000000LL) == 0) )
  {
    v15 = swift_retain(a1);
    sub_100246380(0, 0, a1, v15);
    swift_endAccess(v36);
    swift_release(a1);
    v36[0] = *(_QWORD *)(v3 + v5);
    v16 = sub_10003E4E0(&qword_10066FC60, (__int64 *)&unk_100544310);
    v17 = sub_1001AD4C4();
    Collection<>.toJSONString(prettyPrint:)(0, v16, v17, &off_1005C81B0);
    if ( v18 )
      v19 = v18;
    else
      v19 = 0xE000000000000000LL;
    v20 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSUserDefaults), "standardUserDefaults"));
    v21 = String._bridgeToObjectiveC()();
    swift_bridgeObjectRelease(v19);
    v22 = String._bridgeToObjectiveC()();
    objc_msgSend(v20, "setObject:forKey:", v21, v22);
    objc_release(v20);
    objc_release(v21);
    objc_release(v22);
    v23 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter), "defaultCenter"));
    v24 = String._bridgeToObjectiveC()();
    objc_msgSend(v23, "postNotificationName:object:userInfo:", v24, 0, 0);
    goto LABEL_29;
  }
  __break(1u);
LABEL_40:
  if ( v26 < 0 )
    v35 = v26;
  else
    v35 = v26 & 0xFFFFFFFFFFFFFF8LL;
  if ( (_CocoaArrayWrapper.endIndex.getter(v35) & 0x8000000000000000LL) == 0 )
  {
LABEL_25:
    v27 = swift_retain(a1);
    sub_100246380(0, 0, a1, v27);
    swift_endAccess(v36);
    swift_release(a1);
    v36[0] = *(_QWORD *)(v3 + v5);
    v28 = sub_10003E4E0(&qword_10066FC60, (__int64 *)&unk_100544310);
    v29 = sub_1001AD4C4();
    Collection<>.toJSONString(prettyPrint:)(0, v28, v29, &off_1005C81B0);
    if ( v30 )
      v31 = v30;
    else
      v31 = 0xE000000000000000LL;
    v32 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSUserDefaults), "standardUserDefaults"));
    v23 = String._bridgeToObjectiveC()();
    swift_bridgeObjectRelease(v31);
    v24 = String._bridgeToObjectiveC()();
    objc_msgSend(v32, "setObject:forKey:", v23, v24);
    swift_release(v2);
    objc_release(v32);
LABEL_29:
    objc_release(v23);
    objc_release(v24);
    return;
  }
  __break(1u);
}

/* ========================================================================
 * sub_100219DB4
 * EA: 0x100219db4
 ======================================================================== */

void __fastcall sub_100219DB4(__int64 a1, __int64 a2)
{
  __int64 v2; // x20
  __int64 v3; // x21
  __int64 v5; // x22
  __int64 v6; // x23
  __int64 v7; // x24
  unsigned __int64 v8; // x25
  unsigned __int64 v9; // x28
  __int64 v10; // x19
  __int64 v11; // x24
  __int64 v12; // x21
  __int64 v13; // x27
  __int64 v14; // x0
  __int64 v15; // x0
  __int64 v16; // x26
  __n128 v17; // q0
  __int64 v18; // x1
  __int64 v19; // x0
  bool v20; // zf
  __int64 v21; // x0
  __int64 v22; // x19
  __int64 v23; // x0
  unsigned __int64 v24; // x1
  unsigned __int64 v25; // x20
  id v26; // x21
  NSString v27; // x19
  NSString v28; // x20
  id v29; // x19
  NSString v30; // x20
  __int64 v31; // x0
  __int64 v32; // [xsp+8h] [xbp-88h]
  __int64 v33; // [xsp+10h] [xbp-80h]
  __int64 v34; // [xsp+18h] [xbp-78h]
  _BYTE v35[24]; // [xsp+28h] [xbp-68h] BYREF

  if ( a2 )
  {
    v3 = a2;
    v5 = OBJC_IVAR____TtC15ExternalMonitor17VTFavoriteManager_favorList;
    swift_beginAccess(v2 + OBJC_IVAR____TtC15ExternalMonitor17VTFavoriteManager_favorList, v35, 1, 0);
    v6 = *(_QWORD *)(v2 + v5);
    if ( (unsigned __int64)v6 >> 62 )
      goto LABEL_28;
    v7 = *(_QWORD *)((v6 & 0xFFFFFFFFFFFFFF8LL) + 0x10);
LABEL_4:
    swift_bridgeObjectRetain(v6);
    v34 = v6;
    if ( v7 )
    {
      v32 = v5;
      v33 = v2;
      v8 = 0;
      v9 = v6 & 0xC000000000000001LL;
      v10 = v6 & 0xFFFFFFFFFFFFFF8LL;
      v2 = v6;
      while ( 1 )
      {
        if ( v9 )
        {
          v16 = specialized _ArrayBuffer._getElementSlowPath(_:)(v8, v2);
          v5 = v8 + 1;
          if ( __OFADD__(v8, 1) )
          {
LABEL_20:
            __break(1u);
LABEL_21:
            v5 = v32;
            v2 = v33;
            break;
          }
        }
        else
        {
          if ( v8 >= *(_QWORD *)(v10 + 16) )
          {
            __break(1u);
LABEL_28:
            if ( v6 < 0 )
              v31 = v6;
            else
              v31 = v6 & 0xFFFFFFFFFFFFFF8LL;
            v7 = _CocoaArrayWrapper.endIndex.getter(v31);
            goto LABEL_4;
          }
          v16 = *(_QWORD *)(v2 + 8 * v8 + 32);
          v17 = swift_retain(v16);
          v5 = v8 + 1;
          if ( __OFADD__(v8, 1) )
            goto LABEL_20;
        }
        v6 = v7;
        v18 = *(_QWORD *)(v16 + 24);
        if ( v18
          && ((v19 = *(_QWORD *)(v16 + 16), v19 == a1) ? (v20 = v18 == v3) : (v20 = 0),
              v20 || (_stringCompareWithSmolCheck(_:_:expecting:)(v19, v18, a1, v3, 0) & 1) != 0) )
        {
          swift_release(v16);
        }
        else
        {
          specialized ContiguousArray._makeUniqueAndReserveCapacityIfNotUnique()(v17);
          v11 = v3;
          v12 = a1;
          v13 = _swiftEmptyArrayStorage[2];
          specialized ContiguousArray._reserveCapacityAssumingUniqueBuffer(oldCount:)(v13);
          v14 = v13;
          a1 = v12;
          v3 = v11;
          v15 = specialized ContiguousArray._appendElementAssumeUniqueAndCapacity(_:newElement:)(v14, v16);
          specialized ContiguousArray._endMutation()(v15);
          v2 = v34;
        }
        ++v8;
        v7 = v6;
        if ( v5 == v6 )
          goto LABEL_21;
      }
    }
    swift_bridgeObjectRelease(v34);
    v21 = *(_QWORD *)(v2 + v5);
    *(_QWORD *)(v2 + v5) = _swiftEmptyArrayStorage;
    swift_bridgeObjectRelease(v21);
    v22 = sub_10003E4E0(&qword_10066FC60, (__int64 *)&unk_100544310);
    v23 = sub_1001AD4C4();
    Collection<>.toJSONString(prettyPrint:)(0, v22, v23, &off_1005C81B0);
    if ( v24 )
      v25 = v24;
    else
      v25 = 0xE000000000000000LL;
    v26 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSUserDefaults), "standardUserDefaults"));
    v27 = String._bridgeToObjectiveC()();
    swift_bridgeObjectRelease(v25);
    v28 = String._bridgeToObjectiveC()();
    objc_msgSend(v26, "setObject:forKey:", v27, v28);
    objc_release(v26);
    objc_release(v27);
    objc_release(v28);
    v29 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSNotificationCenter), "defaultCenter"));
    v30 = String._bridgeToObjectiveC()();
    objc_msgSend(v29, "postNotificationName:object:userInfo:", v30, 0, 0);
    objc_release(v29);
    objc_release(v30);
  }
}
