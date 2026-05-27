/* Viture SpaceWalker — automated IDA export (display / Metal path) */
/* Binary: /Users/sutherland/Downloads/space mac/Contents/MacOS/SpaceWalker */

/* --- Priority class methods --- */

/* ========================================================================
 * -[_TtC11SpaceWalker12VTIMUManager .cxx_destruct]
 * EA: 0x100088074
 ======================================================================== */

void __cdecl -[VTIMUManager .cxx_destruct](_TtC11SpaceWalker12VTIMUManager *self, SEL a2)
{
  ;
}

/* ========================================================================
 * -[_TtC11SpaceWalker12VTIMUManager init]
 * EA: 0x100087ea0
 ======================================================================== */

_TtC11SpaceWalker12VTIMUManager *__cdecl -[VTIMUManager init](_TtC11SpaceWalker12VTIMUManager *self, SEL a2)
{
  return (_TtC11SpaceWalker12VTIMUManager *)sub_1000876E4();
}

/* ========================================================================
 * -[_TtC11SpaceWalker13CaptureEngine .cxx_destruct]
 * EA: 0x100080ffc
 ======================================================================== */

void __cdecl -[CaptureEngine .cxx_destruct](_TtC11SpaceWalker13CaptureEngine *self, SEL a2)
{
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_stream));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_videoSampleBufferQueue));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker13CaptureEngine_streamOutput));
}

/* ========================================================================
 * -[_TtC11SpaceWalker13CaptureEngine init]
 * EA: 0x100080fac
 ======================================================================== */

_TtC11SpaceWalker13CaptureEngine *__cdecl -[CaptureEngine init](_TtC11SpaceWalker13CaptureEngine *self, SEL a2)
{
  return (_TtC11SpaceWalker13CaptureEngine *)sub_100080DD4();
}

/* ========================================================================
 * -[_TtC11SpaceWalker13WideStripView .cxx_destruct]
 * EA: 0x1000adb00
 ======================================================================== */

void __cdecl -[WideStripView .cxx_destruct](_TtC11SpaceWalker13WideStripView *self, SEL a2)
{
  objc_release(*(id *)((char *)&self->super.super.super.isa + OBJC_IVAR____TtC11SpaceWalker13WideStripView_renderer));
  swift_release(*(Class *)((char *)&self->super.super.super.isa + OBJC_IVAR____TtC11SpaceWalker13WideStripView_handler));
}

/* ========================================================================
 * -[_TtC11SpaceWalker13WideStripView initWithCoder:]
 * EA: 0x1000ada5c
 ======================================================================== */

_TtC11SpaceWalker13WideStripView *__cdecl __noreturn -[WideStripView initWithCoder:](
        _TtC11SpaceWalker13WideStripView *self,
        SEL a2,
        id a3)
{
  _TtC11SpaceWalker13WideStripView *result; // x0

  result = (_TtC11SpaceWalker13WideStripView *)_assertionFailure(_:_:file:line:flags:)(
                                                 "Fatal error",
                                                 11,
                                                 2,
                                                 0,
                                                 0xE000000000000000LL,
                                                 "SpaceWalker/WideStripMTKRenderer.swift",
                                                 38,
                                                 2,
                                                 70,
                                                 0);
  __break(1u);
  return result;
}

/* ========================================================================
 * -[_TtC11SpaceWalker13WideStripView initWithFrame:]
 * EA: 0x1000adaa4
 ======================================================================== */

// local variable allocation has failed, the output may be wrong!
_TtC11SpaceWalker13WideStripView *__cdecl __noreturn -[WideStripView initWithFrame:](
        _TtC11SpaceWalker13WideStripView *self,
        SEL a2,
        CGRect a3)
{
  _TtC11SpaceWalker13WideStripView *result; // x0

  result = (_TtC11SpaceWalker13WideStripView *)_swift_stdlib_reportUnimplementedInitializer(
                                                 "SpaceWalker.WideStripView",
                                                 25,
                                                 "init(frame:)",
                                                 12,
                                                 0,
                                                 (__n128)a3.origin,
                                                 *(__n128 *)&a3.origin.y,
                                                 (__n128)a3.size,
                                                 *(__n128 *)&a3.size.height);
  __break(1u);
  return result;
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
 * -[_TtC11SpaceWalker14ScreenRecorder init]
 * EA: 0x1001098e4
 ======================================================================== */

_TtC11SpaceWalker14ScreenRecorder *__cdecl -[ScreenRecorder init](_TtC11SpaceWalker14ScreenRecorder *self, SEL a2)
{
  return (_TtC11SpaceWalker14ScreenRecorder *)sub_100109810();
}

/* ========================================================================
 * -[_TtC11SpaceWalker14VTIMUSlerpUtil .cxx_destruct]
 * EA: 0x1000498c4
 ======================================================================== */

void __cdecl -[VTIMUSlerpUtil .cxx_destruct](_TtC11SpaceWalker14VTIMUSlerpUtil *self, SEL a2)
{
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_rawIMUs));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_tiStatistics));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker14VTIMUSlerpUtil_lock));
}

/* ========================================================================
 * -[_TtC11SpaceWalker14VTIMUSlerpUtil init]
 * EA: 0x100048e3c
 ======================================================================== */

_TtC11SpaceWalker14VTIMUSlerpUtil *__cdecl -[VTIMUSlerpUtil init](_TtC11SpaceWalker14VTIMUSlerpUtil *self, SEL a2)
{
  return (_TtC11SpaceWalker14VTIMUSlerpUtil *)sub_100048D30();
}

/* ========================================================================
 * -[_TtC11SpaceWalker17VTPipeLineManager .cxx_destruct]
 * EA: 0x10002bdf0
 ======================================================================== */

void __cdecl -[VTPipeLineManager .cxx_destruct](_TtC11SpaceWalker17VTPipeLineManager *self, SEL a2)
{
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.isa
                                     + OBJC_IVAR____TtC11SpaceWalker17VTPipeLineManager_screenRecoderArray));
  sub_10002CD70(
    *(Class *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker17VTPipeLineManager_capturePreview),
    *(_QWORD *)&self->screenRecoderArray[OBJC_IVAR____TtC11SpaceWalker17VTPipeLineManager_capturePreview],
    *(_QWORD *)&self->capturePreview[OBJC_IVAR____TtC11SpaceWalker17VTPipeLineManager_capturePreview]);
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker17VTPipeLineManager_r6PreView));
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.isa
                                     + OBJC_IVAR____TtC11SpaceWalker17VTPipeLineManager_availableDisplays));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker17VTPipeLineManager_timer));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker17VTPipeLineManager_sphereView));
}

/* ========================================================================
 * -[_TtC11SpaceWalker17VTPipeLineManager cleanup]
 * EA: 0x100025464
 ======================================================================== */

void __cdecl -[VTPipeLineManager cleanup](_TtC11SpaceWalker17VTPipeLineManager *self, SEL a2)
{
  _TtC11SpaceWalker17VTPipeLineManager *v2; // x20

  v2 = objc_retain(self);
  sub_100025154();
  objc_release(v2);
}

/* ========================================================================
 * -[_TtC11SpaceWalker17VTPipeLineManager connect]
 * EA: 0x10002548c
 ======================================================================== */

// attributes: thunk
void __cdecl -[VTPipeLineManager connect](_TtC11SpaceWalker17VTPipeLineManager *self, SEL a2)
{
  -[VTPipeLineManager connect]_0(self, a2);
}

/* ========================================================================
 * -[_TtC11SpaceWalker17VTPipeLineManager connect]_0
 * EA: 0x10002be90
 ======================================================================== */

__int64 -[VTPipeLineManager connect]_0()
{
  __int64 v0; // x19
  __int64 v1; // x20
  __int64 v2; // x0
  __int64 v3; // x21
  void *v4; // x20
  id v5; // x19
  id v6; // x22
  __int64 v7; // x0
  __int64 result; // x0
  _QWORD v9[2]; // [xsp+8h] [xbp-88h] BYREF
  _BYTE v10[24]; // [xsp+18h] [xbp-78h] BYREF
  __int128 v11; // [xsp+30h] [xbp-60h]
  __int128 v12; // [xsp+40h] [xbp-50h]
  __int64 v13; // [xsp+50h] [xbp-40h]
  __int128 v14; // [xsp+60h] [xbp-30h] BYREF

  v0 = type metadata accessor for VTLogger(0);
  v1 = static os_log_type_t.info.getter();
  v2 = static os_log_type_t.info.getter();
  sub_100091CD8(v1, v2, 0x287463656E6E6F63LL, 0xE900000000000029LL, v0);
  if ( qword_100339348 != -1 )
    swift_once(&qword_100339348, sub_1000593D4);
  swift_beginAccess(&xmmword_10033D3D0, v10, 0, 0);
  v11 = xmmword_10033D3D0;
  v12 = xmmword_10033D3E0;
  v13 = qword_10033D3F0;
  v3 = *((_QWORD *)&xmmword_10033D3D0 + 1);
  v4 = (void *)xmmword_10033D3E0;
  v14 = *(__int128 *)((char *)&xmmword_10033D3E0 + 8);
  v5 = objc_retain((id)xmmword_10033D3D0);
  swift_retain(v3);
  v6 = objc_retain(v4);
  sub_10001D39C(&v14, v9);
  v7 = sub_100004DF0(&unk_10033C6E8, &unk_10025BEE0);
  FoilDefaultStorage.wrappedValue.getter(v9, v7);
  objc_release(v6);
  swift_release(v3);
  objc_release(v5);
  sub_10001D3D8(&v14);
  result = sub_10008C568(v9[0]);
  if ( (unsigned __int8)result != 10 )
  {
    if ( qword_100339330 != -1 )
      swift_once(&qword_100339330, sub_100024F20);
    return sub_100025B1C();
  }
  return result;
}

/* ========================================================================
 * -[_TtC11SpaceWalker17VTPipeLineManager disconnect]
 * EA: 0x100025698
 ======================================================================== */

void __cdecl -[VTPipeLineManager disconnect](_TtC11SpaceWalker17VTPipeLineManager *self, SEL a2)
{
  _TtC11SpaceWalker17VTPipeLineManager *v2; // x20

  v2 = objc_retain(self);
  sub_100025490();
  objc_release(v2);
}

/* ========================================================================
 * -[_TtC11SpaceWalker17VTPipeLineManager init]
 * EA: 0x100025134
 ======================================================================== */

_TtC11SpaceWalker17VTPipeLineManager *__cdecl -[VTPipeLineManager init](
        _TtC11SpaceWalker17VTPipeLineManager *self,
        SEL a2)
{
  return (_TtC11SpaceWalker17VTPipeLineManager *)sub_100024F4C();
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
 * -[_TtC11SpaceWalker18VTCameraController cleanup]
 * EA: 0x1000314d0
 ======================================================================== */

void __cdecl -[VTCameraController cleanup](_TtC11SpaceWalker18VTCameraController *self, SEL a2)
{
  _TtC11SpaceWalker18VTCameraController *v2; // x20

  v2 = objc_retain(self);
  sub_100031334();
  objc_release(v2);
}

/* ========================================================================
 * -[_TtC11SpaceWalker18VTCameraController dealloc]
 * EA: 0x10002d318
 ======================================================================== */

void __cdecl -[VTCameraController dealloc](_TtC11SpaceWalker18VTCameraController *self, SEL a2)
{
  _TtC11SpaceWalker18VTCameraController *v2; // x0

  v2 = objc_retain(self);
  sub_10002D278();
}

/* ========================================================================
 * -[_TtC11SpaceWalker18VTCameraController goFar:]
 * EA: 0x100030cbc
 ======================================================================== */

void __cdecl -[VTCameraController goFar:](_TtC11SpaceWalker18VTCameraController *self, SEL a2, id a3)
{
  sub_100030E18(self, a2, a3, sub_100030B78);
}

/* ========================================================================
 * -[_TtC11SpaceWalker18VTCameraController goNear:]
 * EA: 0x100030e0c
 ======================================================================== */

void __cdecl -[VTCameraController goNear:](_TtC11SpaceWalker18VTCameraController *self, SEL a2, id a3)
{
  sub_100030E18(self, a2, a3, sub_100030CC8);
}

/* ========================================================================
 * -[_TtC11SpaceWalker18VTCameraController imuDataUpdated:]
 * EA: 0x10002dab8
 ======================================================================== */

void __cdecl -[VTCameraController imuDataUpdated:](_TtC11SpaceWalker18VTCameraController *self, SEL a2, id a3)
{
  sub_1000302F8(self, a2, a3, sub_10002D3E0);
}

/* ========================================================================
 * -[_TtC11SpaceWalker18VTCameraController init]
 * EA: 0x1000314f8
 ======================================================================== */

_TtC11SpaceWalker18VTCameraController *__cdecl __noreturn -[VTCameraController init](
        _TtC11SpaceWalker18VTCameraController *self,
        SEL a2)
{
  _TtC11SpaceWalker18VTCameraController *result; // x0

  result = (_TtC11SpaceWalker18VTCameraController *)_swift_stdlib_reportUnimplementedInitializer(
                                                      "SpaceWalker.VTCameraController",
                                                      30,
                                                      "init()",
                                                      6,
                                                      0);
  __break(1u);
  return result;
}

/* ========================================================================
 * -[_TtC11SpaceWalker18VTCameraController onReset:]
 * EA: 0x1000302ec
 ======================================================================== */

void __cdecl -[VTCameraController onReset:](_TtC11SpaceWalker18VTCameraController *self, SEL a2, id a3)
{
  sub_1000302F8(self, a2, a3, sub_10002ECDC);
}

/* ========================================================================
 * -[_TtC11SpaceWalker18VTCameraController renderer:updateAtTime:]
 * EA: 0x100031544
 ======================================================================== */

void __cdecl -[VTCameraController renderer:updateAtTime:](
        _TtC11SpaceWalker18VTCameraController *self,
        SEL a2,
        id a3,
        double a4)
{
  _TtC11SpaceWalker18VTCameraController *v6; // x20

  swift_unknownObjectRetain(a3, a2, a4);
  v6 = objc_retain(self);
  sub_10003260C();
  swift_unknownObjectRelease(a3);
  objc_release(v6);
}

/* ========================================================================
 * -[_TtC11SpaceWalker18VTCameraController resetAdjustedData]
 * EA: 0x1000304a4
 ======================================================================== */

void __cdecl -[VTCameraController resetAdjustedData](_TtC11SpaceWalker18VTCameraController *self, SEL a2)
{
  _TtC11SpaceWalker18VTCameraController *v2; // x20

  v2 = objc_retain(self);
  sub_1000303A0();
  objc_release(v2);
}

/* ========================================================================
 * -[_TtC11SpaceWalker18VTCameraController setVSyncEnabled:]
 * EA: 0x10002de50
 ======================================================================== */

// local variable allocation has failed, the output may be wrong!
void __cdecl -[VTCameraController setVSyncEnabled:](_TtC11SpaceWalker18VTCameraController *self, SEL a2, bool a3)
{
  _BOOL8 v3; // x19
  _TtC11SpaceWalker18VTCameraController *v4; // x20

  v3 = a3;
  v4 = objc_retain(self);
  sub_10002DC60(v3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC11SpaceWalker20VTImuPosTransManager onReset:]
 * EA: 0x10010bc1c
 ======================================================================== */

void __cdecl -[VTImuPosTransManager onReset:](_TtC11SpaceWalker20VTImuPosTransManager *self, SEL a2, id a3)
{
  __int64 v5; // x21
  __int64 v6; // x23
  _QWORD *v7; // x22
  void (__fastcall *v8)(_QWORD *, __int64); // x19
  __int64 v9; // x8
  unsigned __int8 v10; // w9
  __int64 v11; // [xsp+0h] [xbp-30h] BYREF

  v5 = type metadata accessor for Notification(0, a2);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = (__int64 *)((char *)&v11 - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL));
  static Notification._unconditionallyBridgeFromObjectiveC(_:)(v7, a3);
  v8 = *(void (__fastcall **)(_QWORD *, __int64))(v6 + 8);
  swift_retain(self);
  v8(v7, v5);
  v9 = *(_QWORD *)&self->originEuler[23];
  v10 = self[1]._TtCs12_SwiftObject_opaque[6];
  *(_OWORD *)self->anchorEuler = *(_OWORD *)&self->originEuler[7];
  *(_QWORD *)&self->anchorEuler[16] = v9;
  self->anchorEuler[24] = v10;
  swift_release(self);
}

/* ========================================================================
 * -[_TtC11SpaceWalker21VTSmoothFollowManager .cxx_destruct]
 * EA: 0x10008ed3c
 ======================================================================== */

void __cdecl -[VTSmoothFollowManager .cxx_destruct](_TtC11SpaceWalker21VTSmoothFollowManager *self, SEL a2)
{
  ;
}

/* ========================================================================
 * -[_TtC11SpaceWalker21VTSmoothFollowManager init]
 * EA: 0x10008ecec
 ======================================================================== */

_TtC11SpaceWalker21VTSmoothFollowManager *__cdecl -[VTSmoothFollowManager init](
        _TtC11SpaceWalker21VTSmoothFollowManager *self,
        SEL a2)
{
  return (_TtC11SpaceWalker21VTSmoothFollowManager *)sub_10008EC0C();
}

/* ========================================================================
 * -[_TtC11SpaceWalker22SCNCaptureVideoPreview .cxx_destruct]
 * EA: 0x100119744
 ======================================================================== */

void __cdecl -[SCNCaptureVideoPreview .cxx_destruct](_TtC11SpaceWalker22SCNCaptureVideoPreview *self, SEL a2)
{
  __int64 v3; // x1

  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC11SpaceWalker22SCNCaptureVideoPreview_cameraNode));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC11SpaceWalker22SCNCaptureVideoPreview_planeNode));
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.super.super.super.isa
                                     + OBJC_IVAR____TtC11SpaceWalker22SCNCaptureVideoPreview_planeNodeGroup));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC11SpaceWalker22SCNCaptureVideoPreview_textureCache));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC11SpaceWalker22SCNCaptureVideoPreview_textNode));
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC11SpaceWalker22SCNCaptureVideoPreview_cameraController));
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.super.super.super.isa
                                     + OBJC_IVAR____TtC11SpaceWalker22SCNCaptureVideoPreview_imageBuffers));
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.super.super.super.isa
                                     + OBJC_IVAR____TtC11SpaceWalker22SCNCaptureVideoPreview_debugImagePool));
  swift_unknownObjectWeakDestroy((char *)self + OBJC_IVAR____TtC11SpaceWalker22SCNCaptureVideoPreview_timer, v3);
  objc_release(*(id *)((char *)&self->super.super.super.super.isa
                     + OBJC_IVAR____TtC11SpaceWalker22SCNCaptureVideoPreview_fpsStatisticUtil));
}

/* ========================================================================
 * -[_TtC11SpaceWalker22SCNCaptureVideoPreview dealloc]
 * EA: 0x1001196d4
 ======================================================================== */

void __cdecl -[SCNCaptureVideoPreview dealloc](_TtC11SpaceWalker22SCNCaptureVideoPreview *self, SEL a2)
{
  _TtC11SpaceWalker22SCNCaptureVideoPreview *v2; // x20
  void *Strong; // x0
  void *v4; // x19
  objc_super v5; // [xsp+0h] [xbp-20h] BYREF

  v2 = objc_retain(self);
  sub_10011BBCC();
  Strong = (void *)swift_unknownObjectWeakLoadStrong((char *)v2 + OBJC_IVAR____TtC11SpaceWalker22SCNCaptureVideoPreview_timer);
  if ( Strong )
  {
    v4 = Strong;
    objc_msgSend(Strong, "invalidate");
    objc_release(v4);
  }
  v5.receiver = v2;
  v5.super_class = (Class)type metadata accessor for SCNCaptureVideoPreview();
  -[SCNCaptureVideoPreview dealloc](&v5, "dealloc");
}

/* ========================================================================
 * -[_TtC11SpaceWalker22SCNCaptureVideoPreview imuDataUpdated:]
 * EA: 0x10011bd8c
 ======================================================================== */

void __cdecl -[SCNCaptureVideoPreview imuDataUpdated:](_TtC11SpaceWalker22SCNCaptureVideoPreview *self, SEL a2, id a3)
{
  __int64 v4; // x20
  __int64 v5; // x22
  _QWORD *v6; // x21
  __int64 v7; // [xsp+0h] [xbp-20h] BYREF

  v4 = type metadata accessor for Notification(0, a2);
  v5 = *(_QWORD *)(v4 - 8);
  v6 = (__int64 *)((char *)&v7 - ((*(_QWORD *)(v5 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL));
  static Notification._unconditionallyBridgeFromObjectiveC(_:)(v6, a3);
  (*(void (__fastcall **)(_QWORD *, __int64))(v5 + 8))(v6, v4);
}

/* ========================================================================
 * -[_TtC11SpaceWalker22SCNCaptureVideoPreview initWithCoder:]
 * EA: 0x10011b94c
 ======================================================================== */

_TtC11SpaceWalker22SCNCaptureVideoPreview *__cdecl __noreturn -[SCNCaptureVideoPreview initWithCoder:](
        _TtC11SpaceWalker22SCNCaptureVideoPreview *self,
        SEL a2,
        id a3)
{
  sub_10011DAC8(objc_retain(a3));
}

/* ========================================================================
 * -[_TtC11SpaceWalker22SCNCaptureVideoPreview initWithFrame:]
 * EA: 0x10011bd40
 ======================================================================== */

_TtC11SpaceWalker22SCNCaptureVideoPreview *__cdecl __noreturn -[SCNCaptureVideoPreview initWithFrame:](
        _TtC11SpaceWalker22SCNCaptureVideoPreview *self,
        SEL a2,
        CGRect a3)
{
  _TtC11SpaceWalker22SCNCaptureVideoPreview *result; // x0

  result = (_TtC11SpaceWalker22SCNCaptureVideoPreview *)_swift_stdlib_reportUnimplementedInitializer(
                                                          "SpaceWalker.SCNCaptureVideoPreview",
                                                          34,
                                                          "init(frame:)",
                                                          12,
                                                          0);
  __break(1u);
  return result;
}

/* ========================================================================
 * -[_TtC11SpaceWalker22SCNCaptureVideoPreview initWithFrame:options:]
 * EA: 0x10011bd14
 ======================================================================== */

_TtC11SpaceWalker22SCNCaptureVideoPreview *__cdecl __noreturn -[SCNCaptureVideoPreview initWithFrame:options:](
        _TtC11SpaceWalker22SCNCaptureVideoPreview *self,
        SEL a2,
        CGRect a3,
        id a4)
{
  _TtC11SpaceWalker22SCNCaptureVideoPreview *result; // x0

  result = (_TtC11SpaceWalker22SCNCaptureVideoPreview *)_swift_stdlib_reportUnimplementedInitializer(
                                                          "SpaceWalker.SCNCaptureVideoPreview",
                                                          34,
                                                          "init(frame:options:)",
                                                          20,
                                                          0);
  __break(1u);
  return result;
}

/* ========================================================================
 * -[_TtC11SpaceWalker22SCNCaptureVideoPreview performKeyEquivalent:]
 * EA: 0x10011c394
 ======================================================================== */

bool __cdecl -[SCNCaptureVideoPreview performKeyEquivalent:](
        _TtC11SpaceWalker22SCNCaptureVideoPreview *self,
        SEL a2,
        id a3)
{
  return (unsigned int)objc_msgSend(a3, "keyCode") == 53;
}

/* ========================================================================
 * -[_TtC11SpaceWalker22SCNCaptureVideoPreview renderer:updateAtTime:]
 * EA: 0x10011c350
 ======================================================================== */

void __cdecl -[SCNCaptureVideoPreview renderer:updateAtTime:](
        _TtC11SpaceWalker22SCNCaptureVideoPreview *self,
        SEL a2,
        id a3,
        double a4)
{
  _TtC11SpaceWalker22SCNCaptureVideoPreview *v6; // x20

  swift_unknownObjectRetain(a3, a2, a4);
  v6 = objc_retain(self);
  sub_10011DD00();
  swift_unknownObjectRelease(a3);
  objc_release(v6);
}

/* ========================================================================
 * -[_TtC11SpaceWalker22SCNCaptureVideoPreview screensLostTimerTick]
 * EA: 0x10011cf28
 ======================================================================== */

void __cdecl -[SCNCaptureVideoPreview screensLostTimerTick](_TtC11SpaceWalker22SCNCaptureVideoPreview *self, SEL a2)
{
  _TtC11SpaceWalker22SCNCaptureVideoPreview *v2; // x20

  v2 = objc_retain(self);
  sub_10011C8AC();
  objc_release(v2);
}

/* ========================================================================
 * -[_TtC11SpaceWalker22WideStripMetalRenderer .cxx_destruct]
 * EA: 0x1000ae038
 ======================================================================== */

void __cdecl -[WideStripMetalRenderer .cxx_destruct](_TtC11SpaceWalker22WideStripMetalRenderer *self, SEL a2)
{
  swift_unknownObjectRelease(*(Class *)((char *)&self->super.isa
                                      + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_device));
  swift_unknownObjectRelease(*(Class *)((char *)&self->super.isa
                                      + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_commandQueue));
  swift_unknownObjectRelease(*(Class *)((char *)&self->super.isa
                                      + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_pipeline));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_textureCache));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_metalLayer));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_containerView));
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.isa
                                     + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_latestPixelBuffers));
  objc_release(*(id *)((char *)&self->super.isa + OBJC_IVAR____TtC11SpaceWalker22WideStripMetalRenderer_displayLink));
}

/* ========================================================================
 * -[_TtC11SpaceWalker22WideStripMetalRenderer init]
 * EA: 0x1000ae008
 ======================================================================== */

_TtC11SpaceWalker22WideStripMetalRenderer *__cdecl __noreturn -[WideStripMetalRenderer init](
        _TtC11SpaceWalker22WideStripMetalRenderer *self,
        SEL a2)
{
  _TtC11SpaceWalker22WideStripMetalRenderer *result; // x0

  result = (_TtC11SpaceWalker22WideStripMetalRenderer *)_swift_stdlib_reportUnimplementedInitializer(
                                                          "SpaceWalker.WideStripMetalRenderer",
                                                          34,
                                                          "init()",
                                                          6,
                                                          0);
  __break(1u);
  return result;
}

/* --- String xref functions --- */

/* --- Large render-related functions (fallback) --- */

/* ========================================================================
 * fallback -[_TtC11SpaceWalker31VerticallyCenteredTextFieldCell drawInteriorWithFrame:inView:]
 * EA: 0x100072e84
 ======================================================================== */

void __cdecl -[VerticallyCenteredTextFieldCell drawInteriorWithFrame:inView:](
        _TtC11SpaceWalker31VerticallyCenteredTextFieldCell *self,
        SEL a2,
        CGRect a3,
        id a4)
{
  double height; // d8
  double width; // d9
  double y; // d10
  double x; // d11
  objc_class *v10; // x21
  id v11; // x19
  _TtC11SpaceWalker31VerticallyCenteredTextFieldCell *v12; // x20
  double v13; // d0
  double v14; // d12
  double v15; // d1
  double v16; // d13
  double v17; // d2
  double v18; // d14
  double v19; // d3
  CGFloat v20; // d15
  double v21; // d1
  double v22; // d8
  CGFloat v23; // d0
  objc_super v24; // [xsp+0h] [xbp-80h] BYREF
  objc_super v25; // [xsp+10h] [xbp-70h] BYREF
  CGRect v26; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8

  height = a3.size.height;
  width = a3.size.width;
  y = a3.origin.y;
  x = a3.origin.x;
  v10 = (objc_class *)type metadata accessor for VerticallyCenteredTextFieldCell(self, a2);
  v25.receiver = self;
  v25.super_class = v10;
  v11 = objc_retain(a4);
  v12 = objc_retain(self);
  -[VerticallyCenteredTextFieldCell titleRectForBounds:](&v25, "titleRectForBounds:", x, y, width, height);
  v14 = v13;
  v16 = v15;
  v18 = v17;
  v20 = v19;
  -[VerticallyCenteredTextFieldCell cellSizeForBounds:](v12, "cellSizeForBounds:", x, y, width, height);
  v22 = v21;
  v26.origin.x = v14;
  v26.origin.y = v16;
  v26.size.width = v18;
  v26.size.height = v20;
  v23 = CGRectGetHeight(v26);
  v24.receiver = v12;
  v24.super_class = v10;
  -[VerticallyCenteredTextFieldCell drawInteriorWithFrame:inView:](
    &v24,
    "drawInteriorWithFrame:inView:",
    v11,
    v14,
    v16 + (v23 - v22) * 0.5,
    v18,
    v22);
  objc_release(v11);
  objc_release(v12);
}

/* ========================================================================
 * fallback -[_TtC11SpaceWalker14VTSwitchButton initWithFrame:]
 * EA: 0x100054324
 ======================================================================== */

_TtC11SpaceWalker14VTSwitchButton *__cdecl -[VTSwitchButton initWithFrame:](
        _TtC11SpaceWalker14VTSwitchButton *self,
        SEL a2,
        CGRect a3)
{
  double height; // d8
  double width; // d9
  double y; // d10
  double x; // d11
  __int64 v8; // x21
  void *v9; // x20
  __int64 v10; // x21
  objc_super v12; // [xsp+0h] [xbp-50h] BYREF

  height = a3.size.height;
  width = a3.size.width;
  y = a3.origin.y;
  x = a3.origin.x;
  v8 = OBJC_IVAR____TtC11SpaceWalker14VTSwitchButton_selectedColor;
  v9 = (void *)objc_opt_self(&OBJC_CLASS___NSColor, a2);
  *(Class *)((char *)&self->super.super.super.super.super.isa + v8) = (Class)objc_retainAutoreleasedReturnValue(
                                                                               objc_msgSend(
                                                                                 v9,
                                                                                 "colorWithRed:green:blue:alpha:",
                                                                                 0.466666667,
                                                                                 0.643137255,
                                                                                 0.996078431,
                                                                                 1.0));
  v10 = OBJC_IVAR____TtC11SpaceWalker14VTSwitchButton_unselectedColor;
  *(Class *)((char *)&self->super.super.super.super.super.isa + v10) = (Class)objc_retainAutoreleasedReturnValue(
                                                                                objc_msgSend(
                                                                                  v9,
                                                                                  "colorWithRed:green:blue:alpha:",
                                                                                  0.235294118,
                                                                                  0.305882353,
                                                                                  0.411764706,
                                                                                  1.0));
  *((_BYTE *)&self->super.super.super.super.super.isa + OBJC_IVAR____TtC11SpaceWalker14VTSwitchButton_isSelected) = 0;
  v12.receiver = self;
  v12.super_class = (Class)type metadata accessor for VTSwitchButton();
  return -[VTSwitchButton initWithFrame:](&v12, "initWithFrame:", x, y, width, height);
}

/* Total decompiled blocks: 44 */
