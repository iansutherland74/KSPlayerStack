/* Viture ExternalMonitor — automated IDA export (2D→3D) */
/* Binary: /Users/sutherland/Downloads/com.viture.p10app-1.9.25-Decrypted/ExternalMonitor */

/* --- Priority class methods --- */

/* ========================================================================
 * +[_TtC15ExternalMonitor11SBSFastView isDrawbleExist]
 * EA: 0x1000b27a4
 ======================================================================== */

bool __cdecl +[SBSFastView isDrawbleExist](id a1, SEL a2)
{
  return byte_100696C48;
}

/* ========================================================================
 * +[_TtC15ExternalMonitor11SBSFastView setIsDrawbleExist:]
 * EA: 0x1000b27b0
 ======================================================================== */

void __cdecl +[SBSFastView setIsDrawbleExist:](id a1, SEL a2, bool a3)
{
  byte_100696C48 = a3;
}

/* ========================================================================
 * -[_TtC15ExternalMonitor11SBSFastView .cxx_destruct]
 * EA: 0x1000b2874
 ======================================================================== */

void __cdecl -[SBSFastView .cxx_destruct](_TtC15ExternalMonitor11SBSFastView *self, SEL a2)
{
  objc_release(*(id *)((char *)&self->super.super.super.isa + OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_mtkView));
  objc_release(*(id *)((char *)&self->super.super.super.isa + OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_ciContext));
  objc_release(*(id *)((char *)&self->super.super.super.isa + OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_leftCIImage));
  objc_release(*(id *)((char *)&self->super.super.super.isa + OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_rightCIImage));
  objc_release(*(id *)((char *)&self->super.super.super.isa + OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_displayLink));
  objc_release(*(id *)((char *)&self->super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_lastRenderedLeftCIImage));
  swift_unknownObjectRelease(*(Class *)((char *)&self->super.super.super.isa
                                      + OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_queue));
  swift_release(*(Class *)((char *)&self->super.super.super.isa + OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_task));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor11SBSFastView dealloc]
 * EA: 0x1000b2850
 ======================================================================== */

void __cdecl -[SBSFastView dealloc](_TtC15ExternalMonitor11SBSFastView *self, SEL a2)
{
  _TtC15ExternalMonitor11SBSFastView *v2; // x0

  v2 = objc_retain(self);
  sub_1000B27BC();
}

/* ========================================================================
 * -[_TtC15ExternalMonitor11SBSFastView drawRect:]
 * EA: 0x1000b5a20
 ======================================================================== */

void __cdecl -[SBSFastView drawRect:](_TtC15ExternalMonitor11SBSFastView *self, SEL a2, CGRect a3)
{
  _TtC15ExternalMonitor11SBSFastView *v3; // [xsp+8h] [xbp-18h]

  v3 = objc_retain(self);
  sub_1000B5F38();
  objc_release(v3);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor11SBSFastView initWithCoder:]
 * EA: 0x1000b2cd0
 ======================================================================== */

_TtC15ExternalMonitor11SBSFastView *__cdecl -[SBSFastView initWithCoder:](
        _TtC15ExternalMonitor11SBSFastView *self,
        SEL a2,
        id a3)
{
  return (_TtC15ExternalMonitor11SBSFastView *)sub_1000B2B5C(objc_retain(a3));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor11SBSFastView initWithFrame:]
 * EA: 0x1000b5a54
 ======================================================================== */

// local variable allocation has failed, the output may be wrong!
_TtC15ExternalMonitor11SBSFastView *__cdecl __noreturn -[SBSFastView initWithFrame:](
        _TtC15ExternalMonitor11SBSFastView *self,
        SEL a2,
        CGRect a3)
{
  _TtC15ExternalMonitor11SBSFastView *result; // x0

  result = (_TtC15ExternalMonitor11SBSFastView *)_swift_stdlib_reportUnimplementedInitializer(
                                                   "ExternalMonitor.SBSFastView",
                                                   27,
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
 * -[_TtC15ExternalMonitor11SBSFastView layoutSubviews]
 * EA: 0x1000b3ea0
 ======================================================================== */

void __cdecl -[SBSFastView layoutSubviews](_TtC15ExternalMonitor11SBSFastView *self, SEL a2)
{
  char *v2; // x19
  void *v3; // x8
  id v4; // x20
  objc_super v5; // [xsp+0h] [xbp-20h] BYREF

  v5.receiver = self;
  v5.super_class = (Class)type metadata accessor for SBSFastView();
  v2 = (char *)objc_retain(v5.receiver);
  -[SBSFastView layoutSubviews](&v5, "layoutSubviews");
  v3 = *(void **)&v2[OBJC_IVAR____TtC15ExternalMonitor11SBSFastView_mtkView];
  if ( v3 )
  {
    v4 = objc_retain(v3);
    ConstraintViewDSL.remakeConstraints(_:)(sub_1000B3E44, 0, v4);
    objc_release(v4);
    objc_release(v2);
  }
  else
  {
    __break(1u);
  }
}

/* ========================================================================
 * -[_TtC15ExternalMonitor11SBSFastView tick]
 * EA: 0x1000b2cf4
 ======================================================================== */

void __cdecl -[SBSFastView tick](_TtC15ExternalMonitor11SBSFastView *self, SEL a2)
{
  ;
}

/* ========================================================================
 * -[_TtC15ExternalMonitor15VT3DMaskManager onScreenDidConnect:]
 * EA: 0x10016f528
 ======================================================================== */

void __cdecl -[VT3DMaskManager onScreenDidConnect:](_TtC15ExternalMonitor15VT3DMaskManager *self, SEL a2, id a3)
{
  sub_10016F78C(self, a2, a3, sub_10016F05C);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor15VT3DMaskManager onScreenDidDisconnect:]
 * EA: 0x10016f780
 ======================================================================== */

void __cdecl -[VT3DMaskManager onScreenDidDisconnect:](_TtC15ExternalMonitor15VT3DMaskManager *self, SEL a2, id a3)
{
  sub_10016F78C(self, a2, a3, sub_10016F534);
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
 * -[_TtC15ExternalMonitor16IntegratedFilter init]
 * EA: 0x1001cd900
 ======================================================================== */

_TtC15ExternalMonitor16IntegratedFilter *__cdecl -[IntegratedFilter init](
        _TtC15ExternalMonitor16IntegratedFilter *self,
        SEL a2)
{
  return sub_1001CEC90(
           self,
           a2,
           &OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_inputImage,
           &OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_ratio,
           type metadata accessor for IntegratedFilter);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor16IntegratedFilter initWithCoder:]
 * EA: 0x1001cd91c
 ======================================================================== */

_TtC15ExternalMonitor16IntegratedFilter *__cdecl -[IntegratedFilter initWithCoder:](
        _TtC15ExternalMonitor16IntegratedFilter *self,
        SEL a2,
        id a3)
{
  return sub_1001CECF8(
           self,
           a2,
           a3,
           &OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_inputImage,
           &OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_ratio,
           type metadata accessor for IntegratedFilter);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor16IntegratedFilter inputImage]
 * EA: 0x1001cd5d0
 ======================================================================== */

CIImage *__cdecl -[IntegratedFilter inputImage](_TtC15ExternalMonitor16IntegratedFilter *self, SEL a2)
{
  return (CIImage *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                            + OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_inputImage));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor16IntegratedFilter outputImage]
 * EA: 0x1001cd670
 ======================================================================== */

CIImage *__cdecl -[IntegratedFilter outputImage](_TtC15ExternalMonitor16IntegratedFilter *self, SEL a2)
{
  _TtC15ExternalMonitor16IntegratedFilter *v2; // x20
  void *v3; // x19

  v2 = objc_retain(self);
  v3 = (void *)sub_1001CD6A0();
  objc_release(v2);
  return (CIImage *)objc_autoreleaseReturnValue(v3);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor16IntegratedFilter ratio]
 * EA: 0x1001cd614
 ======================================================================== */

NSNumber *__cdecl -[IntegratedFilter ratio](_TtC15ExternalMonitor16IntegratedFilter *self, SEL a2)
{
  return (NSNumber *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                             + OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_ratio));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor16IntegratedFilter setInputImage:]
 * EA: 0x1001cd5e0
 ======================================================================== */

void __cdecl -[IntegratedFilter setInputImage:](_TtC15ExternalMonitor16IntegratedFilter *self, SEL a2, id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_inputImage);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_inputImage) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor16IntegratedFilter setRatio:]
 * EA: 0x1001cd624
 ======================================================================== */

void __cdecl -[IntegratedFilter setRatio:](_TtC15ExternalMonitor16IntegratedFilter *self, SEL a2, id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_ratio);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor16IntegratedFilter_ratio) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
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
 * -[_TtC15ExternalMonitor18VTDepthVideoPlayer initWithCoder:]
 * EA: 0x100041b98
 ======================================================================== */

_TtC15ExternalMonitor18VTDepthVideoPlayer *__cdecl __noreturn -[VTDepthVideoPlayer initWithCoder:](
        _TtC15ExternalMonitor18VTDepthVideoPlayer *self,
        SEL a2,
        id a3)
{
  sub_1000454C8(objc_retain(a3));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor18VTDepthVideoPlayer initWithNibName:bundle:]
 * EA: 0x100045044
 ======================================================================== */

_TtC15ExternalMonitor18VTDepthVideoPlayer *__cdecl __noreturn -[VTDepthVideoPlayer initWithNibName:bundle:](
        _TtC15ExternalMonitor18VTDepthVideoPlayer *self,
        SEL a2,
        id a3,
        id a4)
{
  _TtC15ExternalMonitor18VTDepthVideoPlayer *result; // x0

  result = (_TtC15ExternalMonitor18VTDepthVideoPlayer *)_swift_stdlib_reportUnimplementedInitializer(
                                                          "ExternalMonitor.VTDepthVideoPlayer",
                                                          34,
                                                          "init(nibName:bundle:)",
                                                          21,
                                                          0);
  __break(1u);
  return result;
}

/* ========================================================================
 * -[_TtC15ExternalMonitor18VTDepthVideoPlayer playNext]
 * EA: 0x100044308
 ======================================================================== */

void __cdecl -[VTDepthVideoPlayer playNext](_TtC15ExternalMonitor18VTDepthVideoPlayer *self, SEL a2)
{
  _TtC15ExternalMonitor18VTDepthVideoPlayer *v2; // [xsp+8h] [xbp-18h]

  v2 = objc_retain(self);
  sub_10004419C();
  objc_release(v2);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor18VTDepthVideoPlayer playPrevious]
 * EA: 0x100044518
 ======================================================================== */

void __cdecl -[VTDepthVideoPlayer playPrevious](_TtC15ExternalMonitor18VTDepthVideoPlayer *self, SEL a2)
{
  _TtC15ExternalMonitor18VTDepthVideoPlayer *v2; // [xsp+8h] [xbp-18h]

  v2 = objc_retain(self);
  sub_10004433C();
  objc_release(v2);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor18VTDepthVideoPlayer viewDidLayoutSubviews]
 * EA: 0x100042260
 ======================================================================== */

void __cdecl -[VTDepthVideoPlayer viewDidLayoutSubviews](_TtC15ExternalMonitor18VTDepthVideoPlayer *self, SEL a2)
{
  void *v2; // x0

  v2 = *(Class *)((char *)&self->super.super.super.super.isa
                + OBJC_IVAR____TtC15ExternalMonitor18VTDepthVideoPlayer_player);
  if ( v2 )
    objc_msgSend(v2, "play");
  else
    __break(1u);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor18VTDepthVideoPlayer viewDidLoad]
 * EA: 0x10004222c
 ======================================================================== */

void __cdecl -[VTDepthVideoPlayer viewDidLoad](_TtC15ExternalMonitor18VTDepthVideoPlayer *self, SEL a2)
{
  _TtC15ExternalMonitor18VTDepthVideoPlayer *v2; // [xsp+8h] [xbp-18h]

  v2 = objc_retain(self);
  sub_100041EE8();
  objc_release(v2);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor18VTDepthVideoPlayer viewWillDisappear:]
 * EA: 0x100042280
 ======================================================================== */

// local variable allocation has failed, the output may be wrong!
void __cdecl -[VTDepthVideoPlayer viewWillDisappear:](_TtC15ExternalMonitor18VTDepthVideoPlayer *self, SEL a2, bool a3)
{
  _BOOL8 v3; // x19
  id v4; // x20
  id v5; // x0
  objc_super v6; // [xsp+0h] [xbp-30h] BYREF

  v3 = a3;
  v6.receiver = self;
  v6.super_class = (Class)type metadata accessor for VTDepthVideoPlayer(0);
  v4 = objc_retain(v6.receiver);
  v5 = -[VTDepthVideoPlayer viewWillDisappear:](&v6, "viewWillDisappear:", v3);
  sub_1000422E4(v5);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter .cxx_destruct]
 * EA: 0x1001146c8
 ======================================================================== */

void __cdecl -[DepthImageShiftFilter .cxx_destruct](_TtC15ExternalMonitor21DepthImageShiftFilter *self, SEL a2)
{
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputImage));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputDepthImage));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputIncrement));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputMaxShift));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_incrementMultiple));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_shiftRatio));
  objc_release(*(id *)((char *)&self->super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_sigmoidCoef));
  objc_release(*(id *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_kernel));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter incrementMultiple]
 * EA: 0x100113d48
 ======================================================================== */

NSNumber *__cdecl -[DepthImageShiftFilter incrementMultiple](
        _TtC15ExternalMonitor21DepthImageShiftFilter *self,
        SEL a2)
{
  return (NSNumber *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                             + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_incrementMultiple));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter init]
 * EA: 0x1001143b4
 ======================================================================== */

_TtC15ExternalMonitor21DepthImageShiftFilter *__cdecl -[DepthImageShiftFilter init](
        _TtC15ExternalMonitor21DepthImageShiftFilter *self,
        SEL a2)
{
  return (_TtC15ExternalMonitor21DepthImageShiftFilter *)sub_100114114();
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter initWithCoder:]
 * EA: 0x100114698
 ======================================================================== */

_TtC15ExternalMonitor21DepthImageShiftFilter *__cdecl -[DepthImageShiftFilter initWithCoder:](
        _TtC15ExternalMonitor21DepthImageShiftFilter *self,
        SEL a2,
        id a3)
{
  return (_TtC15ExternalMonitor21DepthImageShiftFilter *)sub_1001143D4(objc_retain(a3));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter inputDepthImage]
 * EA: 0x100113c7c
 ======================================================================== */

CIImage *__cdecl -[DepthImageShiftFilter inputDepthImage](_TtC15ExternalMonitor21DepthImageShiftFilter *self, SEL a2)
{
  return (CIImage *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                            + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputDepthImage));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter inputImage]
 * EA: 0x100113c38
 ======================================================================== */

CIImage *__cdecl -[DepthImageShiftFilter inputImage](_TtC15ExternalMonitor21DepthImageShiftFilter *self, SEL a2)
{
  return (CIImage *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                            + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputImage));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter inputIncrement]
 * EA: 0x100113cc0
 ======================================================================== */

NSNumber *__cdecl -[DepthImageShiftFilter inputIncrement](_TtC15ExternalMonitor21DepthImageShiftFilter *self, SEL a2)
{
  return (NSNumber *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                             + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputIncrement));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter inputMaxShift]
 * EA: 0x100113d04
 ======================================================================== */

NSNumber *__cdecl -[DepthImageShiftFilter inputMaxShift](_TtC15ExternalMonitor21DepthImageShiftFilter *self, SEL a2)
{
  return (NSNumber *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                             + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputMaxShift));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter outputImage]
 * EA: 0x100113e14
 ======================================================================== */

CIImage *__cdecl -[DepthImageShiftFilter outputImage](_TtC15ExternalMonitor21DepthImageShiftFilter *self, SEL a2)
{
  _TtC15ExternalMonitor21DepthImageShiftFilter *v2; // x20
  void *v3; // x19

  v2 = objc_retain(self);
  v3 = (void *)sub_100113E44();
  objc_release(v2);
  return (CIImage *)objc_autoreleaseReturnValue(v3);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter setIncrementMultiple:]
 * EA: 0x100113d58
 ======================================================================== */

void __cdecl -[DepthImageShiftFilter setIncrementMultiple:](
        _TtC15ExternalMonitor21DepthImageShiftFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa
                + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_incrementMultiple);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_incrementMultiple) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter setInputDepthImage:]
 * EA: 0x100113c8c
 ======================================================================== */

void __cdecl -[DepthImageShiftFilter setInputDepthImage:](
        _TtC15ExternalMonitor21DepthImageShiftFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa
                + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputDepthImage);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputDepthImage) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter setInputImage:]
 * EA: 0x100113c48
 ======================================================================== */

void __cdecl -[DepthImageShiftFilter setInputImage:](_TtC15ExternalMonitor21DepthImageShiftFilter *self, SEL a2, id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputImage);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputImage) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter setInputIncrement:]
 * EA: 0x100113cd0
 ======================================================================== */

void __cdecl -[DepthImageShiftFilter setInputIncrement:](
        _TtC15ExternalMonitor21DepthImageShiftFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa
                + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputIncrement);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputIncrement) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter setInputMaxShift:]
 * EA: 0x100113d14
 ======================================================================== */

void __cdecl -[DepthImageShiftFilter setInputMaxShift:](
        _TtC15ExternalMonitor21DepthImageShiftFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputMaxShift);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_inputMaxShift) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter setShiftRatio:]
 * EA: 0x100113d9c
 ======================================================================== */

void __cdecl -[DepthImageShiftFilter setShiftRatio:](_TtC15ExternalMonitor21DepthImageShiftFilter *self, SEL a2, id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_shiftRatio);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_shiftRatio) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter setSigmoidCoef:]
 * EA: 0x100113de0
 ======================================================================== */

void __cdecl -[DepthImageShiftFilter setSigmoidCoef:](
        _TtC15ExternalMonitor21DepthImageShiftFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_sigmoidCoef);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_sigmoidCoef) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter shiftRatio]
 * EA: 0x100113d8c
 ======================================================================== */

NSNumber *__cdecl -[DepthImageShiftFilter shiftRatio](_TtC15ExternalMonitor21DepthImageShiftFilter *self, SEL a2)
{
  return (NSNumber *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                             + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_shiftRatio));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor21DepthImageShiftFilter sigmoidCoef]
 * EA: 0x100113dd0
 ======================================================================== */

NSNumber *__cdecl -[DepthImageShiftFilter sigmoidCoef](_TtC15ExternalMonitor21DepthImageShiftFilter *self, SEL a2)
{
  return (NSNumber *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                             + OBJC_IVAR____TtC15ExternalMonitor21DepthImageShiftFilter_sigmoidCoef));
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
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter incrementMultiple]
 * EA: 0x10021739c
 ======================================================================== */

NSNumber *__cdecl -[CIStereoShiftMetalFilter incrementMultiple](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2)
{
  return (NSNumber *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                             + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_incrementMultiple));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter init]
 * EA: 0x100217a68
 ======================================================================== */

_TtC15ExternalMonitor24CIStereoShiftMetalFilter *__cdecl -[CIStereoShiftMetalFilter init](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2)
{
  return (_TtC15ExternalMonitor24CIStereoShiftMetalFilter *)sub_1002177D4();
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter initWithCoder:]
 * EA: 0x100217d40
 ======================================================================== */

_TtC15ExternalMonitor24CIStereoShiftMetalFilter *__cdecl -[CIStereoShiftMetalFilter initWithCoder:](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2,
        id a3)
{
  return (_TtC15ExternalMonitor24CIStereoShiftMetalFilter *)sub_100217A88(objc_retain(a3));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter inputDepthImage]
 * EA: 0x1002172d0
 ======================================================================== */

CIImage *__cdecl -[CIStereoShiftMetalFilter inputDepthImage](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2)
{
  return (CIImage *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                            + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputDepthImage));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter inputImage]
 * EA: 0x10021728c
 ======================================================================== */

CIImage *__cdecl -[CIStereoShiftMetalFilter inputImage](_TtC15ExternalMonitor24CIStereoShiftMetalFilter *self, SEL a2)
{
  return (CIImage *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                            + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputImage));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter inputIncrement]
 * EA: 0x100217314
 ======================================================================== */

NSNumber *__cdecl -[CIStereoShiftMetalFilter inputIncrement](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2)
{
  return (NSNumber *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                             + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputIncrement));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter inputMaxShift]
 * EA: 0x100217358
 ======================================================================== */

NSNumber *__cdecl -[CIStereoShiftMetalFilter inputMaxShift](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2)
{
  return (NSNumber *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                             + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputMaxShift));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter outputImage]
 * EA: 0x100217468
 ======================================================================== */

CIImage *__cdecl -[CIStereoShiftMetalFilter outputImage](_TtC15ExternalMonitor24CIStereoShiftMetalFilter *self, SEL a2)
{
  _TtC15ExternalMonitor24CIStereoShiftMetalFilter *v2; // x20
  void *v3; // x19

  v2 = objc_retain(self);
  v3 = (void *)sub_100217498();
  objc_release(v2);
  return (CIImage *)objc_autoreleaseReturnValue(v3);
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
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter setInputDepthImage:]
 * EA: 0x1002172e0
 ======================================================================== */

void __cdecl -[CIStereoShiftMetalFilter setInputDepthImage:](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa
                + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputDepthImage);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputDepthImage) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter setInputImage:]
 * EA: 0x10021729c
 ======================================================================== */

void __cdecl -[CIStereoShiftMetalFilter setInputImage:](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputImage);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputImage) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter setInputIncrement:]
 * EA: 0x100217324
 ======================================================================== */

void __cdecl -[CIStereoShiftMetalFilter setInputIncrement:](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa
                + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputIncrement);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputIncrement) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter setInputMaxShift:]
 * EA: 0x100217368
 ======================================================================== */

void __cdecl -[CIStereoShiftMetalFilter setInputMaxShift:](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa
                + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputMaxShift);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_inputMaxShift) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter setShiftRatio:]
 * EA: 0x1002173f0
 ======================================================================== */

void __cdecl -[CIStereoShiftMetalFilter setShiftRatio:](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_shiftRatio);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_shiftRatio) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter setSigmoidCoef:]
 * EA: 0x100217434
 ======================================================================== */

void __cdecl -[CIStereoShiftMetalFilter setSigmoidCoef:](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2,
        id a3)
{
  id v3; // x0
  id v4; // [xsp+8h] [xbp-8h]

  v4 = *(Class *)((char *)&self->super.super.isa
                + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_sigmoidCoef);
  *(Class *)((char *)&self->super.super.isa + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_sigmoidCoef) = (Class)a3;
  v3 = objc_retain(a3);
  objc_release(v4);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter shiftRatio]
 * EA: 0x1002173e0
 ======================================================================== */

NSNumber *__cdecl -[CIStereoShiftMetalFilter shiftRatio](_TtC15ExternalMonitor24CIStereoShiftMetalFilter *self, SEL a2)
{
  return (NSNumber *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                             + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_shiftRatio));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor24CIStereoShiftMetalFilter sigmoidCoef]
 * EA: 0x100217424
 ======================================================================== */

NSNumber *__cdecl -[CIStereoShiftMetalFilter sigmoidCoef](
        _TtC15ExternalMonitor24CIStereoShiftMetalFilter *self,
        SEL a2)
{
  return (NSNumber *)objc_retainAutoreleaseReturnValue(*(id *)((char *)&self->super.super.isa
                                                             + OBJC_IVAR____TtC15ExternalMonitor24CIStereoShiftMetalFilter_sigmoidCoef));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor26VTAI3DOptionViewController .cxx_destruct]
 * EA: 0x10007cef8
 ======================================================================== */

void __cdecl -[VTAI3DOptionViewController .cxx_destruct](
        _TtC15ExternalMonitor26VTAI3DOptionViewController *self,
        SEL a2)
{
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.super.super.isa
                                     + OBJC_IVAR____TtC15ExternalMonitor26VTAI3DOptionViewController_buttons));
  swift_unknownObjectRelease(*(Class *)((char *)&self->super.super.super.isa
                                      + OBJC_IVAR____TtC15ExternalMonitor26VTAI3DOptionViewController_player));
  sub_10007CFB0((char *)self + OBJC_IVAR____TtC15ExternalMonitor26VTAI3DOptionViewController_controlVC);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor26VTAI3DOptionViewController exit]
 * EA: 0x10007ccfc
 ======================================================================== */

void __cdecl -[VTAI3DOptionViewController exit](_TtC15ExternalMonitor26VTAI3DOptionViewController *self, SEL a2)
{
  _TtC15ExternalMonitor26VTAI3DOptionViewController *v2; // [xsp+8h] [xbp-18h]

  v2 = objc_retain(self);
  sub_10007CC40();
  objc_release(v2);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor26VTAI3DOptionViewController initWithCoder:]
 * EA: 0x10007ce18
 ======================================================================== */

_TtC15ExternalMonitor26VTAI3DOptionViewController *__cdecl -[VTAI3DOptionViewController initWithCoder:](
        _TtC15ExternalMonitor26VTAI3DOptionViewController *self,
        SEL a2,
        id a3)
{
  _QWORD *v4; // x8
  char *v5; // x8
  id v6; // x21
  _TtC15ExternalMonitor26VTAI3DOptionViewController *v7; // x19
  objc_super v9; // [xsp+0h] [xbp-30h] BYREF

  *(Class *)((char *)&self->super.super.super.isa + OBJC_IVAR____TtC15ExternalMonitor26VTAI3DOptionViewController_buttons) = (Class)&_swiftEmptyArrayStorage;
  v4 = (Class *)((char *)&self->super.super.super.isa
               + OBJC_IVAR____TtC15ExternalMonitor26VTAI3DOptionViewController_player);
  *v4 = 0;
  v4[1] = 0;
  v5 = (char *)self + OBJC_IVAR____TtC15ExternalMonitor26VTAI3DOptionViewController_controlVC;
  *(_OWORD *)v5 = 0u;
  *((_OWORD *)v5 + 1) = 0u;
  *((_QWORD *)v5 + 4) = 0;
  v9.receiver = self;
  v9.super_class = (Class)type metadata accessor for VTAI3DOptionViewController();
  v6 = objc_retain(a3);
  v7 = objc_retainAutoreleasedReturnValue(-[VTAI3DOptionViewController initWithCoder:](&v9, "initWithCoder:", v6));
  objc_release(v6);
  if ( v7 )
    objc_release(v7);
  return v7;
}

/* ========================================================================
 * -[_TtC15ExternalMonitor26VTAI3DOptionViewController initWithNibName:bundle:]
 * EA: 0x10007cd30
 ======================================================================== */

_TtC15ExternalMonitor26VTAI3DOptionViewController *__cdecl -[VTAI3DOptionViewController initWithNibName:bundle:](
        _TtC15ExternalMonitor26VTAI3DOptionViewController *self,
        SEL a2,
        id a3,
        id a4)
{
  __int64 v6; // x1
  __int64 v7; // x21
  _QWORD *v8; // x8
  char *v9; // x8
  id v10; // x0
  NSString v11; // x22
  _TtC15ExternalMonitor26VTAI3DOptionViewController *v12; // x20
  objc_super v14; // [xsp+0h] [xbp-30h] BYREF

  if ( a3 )
  {
    static String._unconditionallyBridgeFromObjectiveC(_:)(a3, a2);
    v7 = v6;
  }
  else
  {
    v7 = 0;
  }
  *(Class *)((char *)&self->super.super.super.isa + OBJC_IVAR____TtC15ExternalMonitor26VTAI3DOptionViewController_buttons) = (Class)&_swiftEmptyArrayStorage;
  v8 = (Class *)((char *)&self->super.super.super.isa
               + OBJC_IVAR____TtC15ExternalMonitor26VTAI3DOptionViewController_player);
  *v8 = 0;
  v8[1] = 0;
  v9 = (char *)self + OBJC_IVAR____TtC15ExternalMonitor26VTAI3DOptionViewController_controlVC;
  *(_OWORD *)v9 = 0u;
  *((_OWORD *)v9 + 1) = 0u;
  *((_QWORD *)v9 + 4) = 0;
  v10 = objc_retain(a4);
  if ( v7 )
  {
    v11 = String._bridgeToObjectiveC()();
    swift_bridgeObjectRelease(v7);
  }
  else
  {
    v11 = nullptr;
  }
  v14.receiver = self;
  v14.super_class = (Class)type metadata accessor for VTAI3DOptionViewController();
  v12 = -[VTAI3DOptionViewController initWithNibName:bundle:](&v14, "initWithNibName:bundle:", v11, a4);
  objc_release(v11);
  objc_release(a4);
  return v12;
}

/* ========================================================================
 * -[_TtC15ExternalMonitor26VTAI3DOptionViewController optionButtonTapped:]
 * EA: 0x10007cbe0
 ======================================================================== */

void __cdecl -[VTAI3DOptionViewController optionButtonTapped:](
        _TtC15ExternalMonitor26VTAI3DOptionViewController *self,
        SEL a2,
        id a3)
{
  _TtC15ExternalMonitor26VTAI3DOptionViewController *v5; // x20
  _QWORD v6[4]; // [xsp+0h] [xbp-30h] BYREF

  swift_unknownObjectRetain(a3, a2);
  v5 = objc_retain(self);
  _bridgeAnyObjectToAny(_:)(v6, a3);
  swift_unknownObjectRelease(a3);
  sub_10007CA0C(v6);
  objc_release(v5);
  sub_10003F5DC(v6);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor26VTAI3DOptionViewController viewDidLoad]
 * EA: 0x10007bd08
 ======================================================================== */

void __cdecl -[VTAI3DOptionViewController viewDidLoad](_TtC15ExternalMonitor26VTAI3DOptionViewController *self, SEL a2)
{
  _TtC15ExternalMonitor26VTAI3DOptionViewController *v2; // [xsp+8h] [xbp-18h]

  v2 = objc_retain(self);
  sub_10007BBD0();
  objc_release(v2);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor31VTVideoDepthInferenceController .cxx_destruct]
 * EA: 0x1001c8a88
 ======================================================================== */

void __cdecl -[VTVideoDepthInferenceController .cxx_destruct](
        _TtC15ExternalMonitor31VTVideoDepthInferenceController *self,
        SEL a2)
{
  swift_unknownObjectWeakDestroy(
    (char *)self + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_playerVC,
    a2);
  objc_release(*(id *)((char *)&self->super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_bufferLock));
  objc_release(*(id *)((char *)&self->super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController__internalBuffer));
  objc_release(*(id *)((char *)&self->super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_lastRenderedBuffer));
  swift_release(*(Class *)((char *)&self->super.super.super.isa
                         + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_task));
  objc_release(*(id *)((char *)&self->super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_photoView));
  swift_release(*(Class *)((char *)&self->super.super.super.isa
                         + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_fpsController));
  objc_release(*(id *)((char *)&self->super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_sequenceKey));
  objc_release(*(id *)((char *)&self->super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_fpsTimer));
  swift_bridgeObjectRelease(*(Class *)((char *)&self->super.super.super.isa
                                     + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_blackEdgeHeights));
  objc_release(*(id *)((char *)&self->super.super.super.isa
                     + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_rotationCW90Session));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor31VTVideoDepthInferenceController curPixelBuffer]
 * EA: 0x1001c892c
 ======================================================================== */

__CVBuffer *__cdecl -[VTVideoDepthInferenceController curPixelBuffer](
        _TtC15ExternalMonitor31VTVideoDepthInferenceController *self,
        SEL a2)
{
  __int64 v3; // x22
  void *v4; // x20
  _TtC15ExternalMonitor31VTVideoDepthInferenceController *v5; // x21
  void *v6; // x19
  id v7; // x20

  v3 = OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_bufferLock;
  v4 = *(Class *)((char *)&self->super.super.super.isa
                + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController_bufferLock);
  v5 = objc_retain(self);
  objc_msgSend(v4, "lock");
  v6 = *(Class *)((char *)&self->super.super.super.isa + v3);
  v7 = objc_retain(*(id *)((char *)&v5->super.super.super.isa
                         + OBJC_IVAR____TtC15ExternalMonitor31VTVideoDepthInferenceController__internalBuffer));
  objc_msgSend(v6, "unlock");
  objc_release(v5);
  return (__CVBuffer *)objc_autoreleaseReturnValue(v7);
}

/* ========================================================================
 * -[_TtC15ExternalMonitor31VTVideoDepthInferenceController dealloc]
 * EA: 0x1001c8a44
 ======================================================================== */

void __cdecl -[VTVideoDepthInferenceController dealloc](
        _TtC15ExternalMonitor31VTVideoDepthInferenceController *self,
        SEL a2)
{
  _TtC15ExternalMonitor31VTVideoDepthInferenceController *v2; // x20
  objc_super v3; // [xsp+0h] [xbp-20h] BYREF

  v2 = objc_retain(self);
  sub_1001C8DA8();
  v3.receiver = v2;
  v3.super_class = (Class)type metadata accessor for VTVideoDepthInferenceController();
  -[VTVideoDepthInferenceController dealloc](&v3, "dealloc");
}

/* ========================================================================
 * -[_TtC15ExternalMonitor31VTVideoDepthInferenceController initWithCoder:]
 * EA: 0x1001c89ec
 ======================================================================== */

_TtC15ExternalMonitor31VTVideoDepthInferenceController *__cdecl __noreturn -[VTVideoDepthInferenceController initWithCoder:](
        _TtC15ExternalMonitor31VTVideoDepthInferenceController *self,
        SEL a2,
        id a3)
{
  sub_1001CB65C(objc_retain(a3));
}

/* ========================================================================
 * -[_TtC15ExternalMonitor31VTVideoDepthInferenceController initWithNibName:bundle:]
 * EA: 0x1001caf0c
 ======================================================================== */

_TtC15ExternalMonitor31VTVideoDepthInferenceController *__cdecl __noreturn -[VTVideoDepthInferenceController initWithNibName:bundle:](
        _TtC15ExternalMonitor31VTVideoDepthInferenceController *self,
        SEL a2,
        id a3,
        id a4)
{
  _TtC15ExternalMonitor31VTVideoDepthInferenceController *result; // x0

  result = (_TtC15ExternalMonitor31VTVideoDepthInferenceController *)_swift_stdlib_reportUnimplementedInitializer(
                                                                       "ExternalMonitor.VTVideoDepthInferenceController",
                                                                       47,
                                                                       "init(nibName:bundle:)",
                                                                       21,
                                                                       0);
  __break(1u);
  return result;
}

/* ========================================================================
 * -[_TtC15ExternalMonitor31VTVideoDepthInferenceController setCurPixelBuffer:]
 * EA: 0x1001c89a4
 ======================================================================== */

void __cdecl -[VTVideoDepthInferenceController setCurPixelBuffer:](
        _TtC15ExternalMonitor31VTVideoDepthInferenceController *self,
        SEL a2,
        __CVBuffer *a3)
{
  _TtC15ExternalMonitor31VTVideoDepthInferenceController *v5; // x20
  __CVBuffer *v6; // [xsp+8h] [xbp-18h]

  v6 = objc_retain(a3);
  v5 = objc_retain(self);
  sub_1001CB524(a3);
  objc_release(v5);
  objc_release(v6);
}

/* --- String xref functions --- */

/* ========================================================================
 * xref 'VTVideoDepthInferenceController' → sub_1001C8CE8
 * EA: 0x1001c8ce8
 ======================================================================== */

__int64 __fastcall sub_1001C8CE8(__int64 a1)
{
  __int64 v1; // x20
  __int64 v2; // x0

  v1 = (*(__int64 (**)(void))(*(_QWORD *)a1 + 264LL))();
  v2 = (*(__int64 (__fastcall **)(unsigned __int64, unsigned __int64, __int64))(*(_QWORD *)v1 + 112LL))(
         0xD000000000000079LL,
         0x80000001004F29E0LL,
         93);
  swift_release(v2);
  return swift_release(v1);
}

/* ========================================================================
 * xref 'VTVideoDepthInferenceController' → sub_1001CB65C
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
 * xref 'DepthRatioCalculator' → sub_1001A4828
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
 * xref 'CIStereoShiftMetalFilter' → sub_1002177D4
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
    sub_10004B4D8(0, &unk_10066C270, &classRef_CIKernel);
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
 * xref 'CIStereoShiftMetalFilter' → sub_100217A88
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
    sub_10004B4D8(0, &unk_10066C270, &classRef_CIKernel);
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
 * xref 'DepthAnythingV2float16' → sub_10024B6EC
 * EA: 0x10024b6ec
 ======================================================================== */

void sub_10024B6EC()
{
  __int64 v0; // x20
  __int64 ObjCClassFromMetadata; // x20
  NSString v2; // x21
  NSString v3; // x22
  id v4; // x20
  id v5; // [xsp+8h] [xbp-28h]

  ObjCClassFromMetadata = swift_getObjCClassFromMetadata(v0);
  v5 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSBundle), "bundleForClass:", ObjCClassFromMetadata));
  v2 = String._bridgeToObjectiveC()();
  v3 = String._bridgeToObjectiveC()();
  v4 = objc_retainAutoreleasedReturnValue(objc_msgSend(v5, "URLForResource:withExtension:", v2, v3));
  objc_release(v2);
  objc_release(v3);
  if ( v4 )
  {
    static URL._unconditionallyBridgeFromObjectiveC(_:)(v4);
    objc_release(v4);
    objc_release(v5);
  }
  else
  {
    __break(1u);
  }
}

/* ========================================================================
 * xref 'DepthAnythingV2float16' → sub_10024BBAC
 * EA: 0x10024bbac
 ======================================================================== */

void sub_10024BBAC()
{
  __int64 v0; // x20
  __int64 ObjCClassFromMetadata; // x20
  NSString v2; // x21
  NSString v3; // x22
  id v4; // x20
  id v5; // [xsp+8h] [xbp-28h]

  ObjCClassFromMetadata = swift_getObjCClassFromMetadata(v0);
  v5 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSBundle), "bundleForClass:", ObjCClassFromMetadata));
  v2 = String._bridgeToObjectiveC()();
  v3 = String._bridgeToObjectiveC()();
  v4 = objc_retainAutoreleasedReturnValue(objc_msgSend(v5, "URLForResource:withExtension:", v2, v3));
  objc_release(v2);
  objc_release(v3);
  if ( v4 )
  {
    static URL._unconditionallyBridgeFromObjectiveC(_:)(v4);
    objc_release(v4);
    objc_release(v5);
  }
  else
  {
    __break(1u);
  }
}

/* ========================================================================
 * xref 'computeWeightedRatio' → sub_1001A5108
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
 * xref 'predict' → sub_10015C79C
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
 * xref 'predict' → sub_10037A9EC
 * EA: 0x10037a9ec
 ======================================================================== */

__int64 __fastcall sub_10037A9EC(__int64 *a1)
{
  bool v2; // [xsp+24h] [xbp-ECh]
  bool v3; // [xsp+28h] [xbp-E8h]
  char v4; // [xsp+2Ch] [xbp-E4h]
  size_t v5; // [xsp+50h] [xbp-C0h]
  __int64 v6; // [xsp+58h] [xbp-B8h]
  int v7; // [xsp+60h] [xbp-B0h]
  int k; // [xsp+64h] [xbp-ACh]
  int m; // [xsp+64h] [xbp-ACh]
  int v10; // [xsp+68h] [xbp-A8h]
  int __n; // [xsp+6Ch] [xbp-A4h]
  size_t __n_4; // [xsp+70h] [xbp-A0h]
  __int64 v13; // [xsp+78h] [xbp-98h]
  int v14; // [xsp+84h] [xbp-8Ch]
  __int64 v15; // [xsp+88h] [xbp-88h]
  __int64 v16; // [xsp+90h] [xbp-80h]
  unsigned int i; // [xsp+98h] [xbp-78h]
  unsigned int j; // [xsp+98h] [xbp-78h]
  int v19; // [xsp+9Ch] [xbp-74h]
  __int64 *v20; // [xsp+A0h] [xbp-70h]
  int v21; // [xsp+ACh] [xbp-64h]
  __int64 v22; // [xsp+B0h] [xbp-60h]
  unsigned int v23; // [xsp+BCh] [xbp-54h]
  int v24; // [xsp+C0h] [xbp-50h]
  int v25; // [xsp+C4h] [xbp-4Ch]
  _BYTE *v26; // [xsp+C8h] [xbp-48h]
  _QWORD *v27; // [xsp+D0h] [xbp-40h]
  __int64 v28; // [xsp+D8h] [xbp-38h]
  unsigned int v29; // [xsp+E4h] [xbp-2Ch]
  unsigned int v30; // [xsp+E8h] [xbp-28h]
  unsigned int v31; // [xsp+ECh] [xbp-24h]

  v31 = *((_DWORD *)a1 + 17) + *((_DWORD *)a1 + 18);
  if ( v31 != (unsigned __int16)(*((_DWORD *)a1 + 15) - *((_DWORD *)a1 + 14)) + 1 )
    __assert_rtn(
      "reconstructFrame",
      "RtpVideoQueue.c",
      197,
      "totalPackets == ((unsigned short) ((queue->bufferHighestSequenceNumber - queue->bufferLowestSequenceNumber) & 65535)) + 1U");
  v30 = *((_DWORD *)a1 + 17) + (*((_DWORD *)a1 + 22) != 0);
  if ( v31 - v30 > *((_DWORD *)a1 + 18) )
    __assert_rtn(
      "reconstructFrame",
      "RtpVideoQueue.c",
      206,
      "totalPackets - neededPackets <= queue->bufferParityPackets");
  if ( *((_DWORD *)a1 + 4) < v30 )
  {
    if ( (*((_BYTE *)a1 + 101) & 1) == 0 && (*((_BYTE *)a1 + 116) & 1) == 0 )
    {
      if ( *((_DWORD *)a1 + 24) <= v31 - v30 )
      {
        if ( v30 - *((_DWORD *)a1 + 4) > (unsigned __int16)(*((_DWORD *)a1 + 15) - *((_DWORD *)a1 + 21)) )
          __assert_rtn(
            "reconstructFrame",
            "RtpVideoQueue.c",
            222,
            "neededPackets - queue->pendingFecBlockList.count <= ((unsigned short) ((queue->bufferHighestSequenceNumber -"
            " queue->receivedHighestSequenceNumber) & 65535))");
      }
      else
      {
        sub_10037CEE8(*((unsigned int *)a1 + 26), 1);
        *((_BYTE *)a1 + 101) = 1;
      }
    }
    return (unsigned int)-1;
  }
  if ( *((_DWORD *)a1 + 24) > *((_DWORD *)a1 + 18) )
    __assert_rtn("reconstructFrame", "RtpVideoQueue.c", 232, "queue->missingPackets <= queue->bufferParityPackets");
  v4 = 1;
  if ( (*((_BYTE *)a1 + 101) & 1) != 0 )
    v4 = *((_BYTE *)a1 + 116);
  if ( (((unsigned __int8)v4 ^ 1) & 1) != 0 )
    __assert_rtn("reconstructFrame", "RtpVideoQueue.c", 233, "!queue->reportedLostFrame || queue->receivedOosData");
  if ( (*((_BYTE *)a1 + 101) & 1) != 0 && (*((_BYTE *)a1 + 116) & 1) == 0 )
  {
    *((_BYTE *)a1 + 116) = 1;
    *((_DWORD *)a1 + 28) = *(_DWORD *)(*a1 + 32);
    if ( off_100697310 )
      ((void (*)(const char *, ...))off_100697310)(
        "Leaving speculative RFI mode due to incorrect loss prediction of frame %u\n",
        *((_DWORD *)a1 + 26));
  }
  if ( (!*((_DWORD *)a1 + 22) || dword_100697350 < 5) && *((_DWORD *)a1 + 19) == *((_DWORD *)a1 + 17) )
    return 0;
  if ( dword_100697350 < 5 )
  {
    if ( off_100697310 )
      ((void (*)(const char *, ...))off_100697310)("FEC recovery not supported on Gen %d servers\n", dword_100697350);
    return (unsigned int)-1;
  }
  v28 = 0;
  v27 = calloc(v31, 8u);
  v26 = calloc(v31, 1u);
  if ( v27 && v26 )
  {
    v28 = sub_10038B758(*((unsigned int *)a1 + 17), *((unsigned int *)a1 + 18));
    if ( !v28 )
      __assert_rtn("reconstructFrame", "RtpVideoQueue.c", 271, "rs != ((void*)0)");
    __memset_chk(v26, 1, v31, -1);
    v25 = dword_100697454 + 16;
    v24 = dword_100697454 + 64;
    v23 = (unsigned int)rand() % *((_DWORD *)a1 + 17);
    v22 = 0;
    v21 = 0;
    v20 = (__int64 *)*a1;
    while ( v20 )
    {
      v19 = (unsigned __int16)(*(_WORD *)(v20[2] + 2) - *((_DWORD *)a1 + 14));
      if ( v19 == v23 )
      {
        v22 = v20[2];
        v21 = *((_DWORD *)v20 + 9);
        v20 = (__int64 *)*v20;
      }
      else
      {
        if ( v27[(unsigned __int16)(*(_WORD *)(v20[2] + 2) - *((_DWORD *)a1 + 14))] )
          __assert_rtn("reconstructFrame", "RtpVideoQueue.c", 305, "packets[index] == ((void*)0)");
        if ( !v26[(unsigned __int16)(*(_WORD *)(v20[2] + 2) - *((_DWORD *)a1 + 14))] )
          __assert_rtn("reconstructFrame", "RtpVideoQueue.c", 306, "marks[index] != 0");
        v27[(unsigned __int16)(*(_WORD *)(v20[2] + 2) - *((_DWORD *)a1 + 14))] = v20[2];
        v26[v19] = 0;
        if ( *((_DWORD *)v20 + 9) < v25 )
          __memset_chk(v27[v19] + *((int *)v20 + 9), 0, v25 - *((_DWORD *)v20 + 9), -1);
        v20 = (__int64 *)*v20;
      }
    }
    for ( i = 0; i < v31; ++i )
    {
      if ( v26[i] )
      {
        v27[i] = malloc(v24);
        if ( !v27[i] )
        {
          v29 = -4;
          goto LABEL_59;
        }
      }
    }
    v29 = sub_10038C9B0(v28, v27, v26, v31, (unsigned int)v25);
    if ( v29 )
      __assert_rtn("reconstructFrame", "RtpVideoQueue.c", 334, "ret == 0");
    if ( *((_DWORD *)a1 + 17) != *((_DWORD *)a1 + 19) )
    {
      if ( off_100697310 )
        ((void (*)(const char *, ...))off_100697310)(
          "Recovered %d video data shards from frame %d\n",
          *((_DWORD *)a1 + 17) - *((_DWORD *)a1 + 19),
          *((_DWORD *)a1 + 26));
      sub_10037A548(a1);
    }
LABEL_59:
    for ( j = 0; ; ++j )
    {
      if ( j >= v31 )
        goto LABEL_130;
      if ( v26[j] )
      {
        if ( v29 || j >= *((_DWORD *)a1 + 17) )
        {
          if ( !v27[j] )
            continue;
          goto LABEL_128;
        }
        v16 = v27[j] + v25;
        v15 = v27[j];
        *(_WORD *)(v15 + 2) = j + *((_DWORD *)a1 + 14);
        *(_BYTE *)v15 = **(_BYTE **)(*a1 + 16);
        *(_DWORD *)(v15 + 4) = *(_DWORD *)(*(_QWORD *)(*a1 + 16) + 4LL);
        *(_DWORD *)(v15 + 8) = *(_DWORD *)(*(_QWORD *)(*a1 + 16) + 8LL);
        v14 = 12;
        if ( (*(_BYTE *)v15 & 0x10) != 0 )
          v14 = 16;
        v13 = v15 + v14;
        *(_DWORD *)(v13 + 4) = *((_DWORD *)a1 + 26);
        *(_BYTE *)(v13 + 11) = 16 * (*((_BYTE *)a1 + 109) | (4 * *((_BYTE *)a1 + 110)));
        if ( j == v23 && v22 )
        {
          __n_4 = v22 + v14;
          __n = v21 - v14 - 16;
          v10 = dword_100697454 - 16;
          v7 = 0;
          if ( __n > dword_100697454 - 16 )
            __assert_rtn("reconstructFrame", "RtpVideoQueue.c", 379, "droppedDataLength <= recoveredDataLength");
          v3 = 1;
          if ( __n != v10 )
            v3 = (*(_BYTE *)(v13 + 8) & 2) != 0;
          if ( !v3 )
            __assert_rtn(
              "reconstructFrame",
              "RtpVideoQueue.c",
              380,
              "droppedDataLength == recoveredDataLength || (nvPacket->flags & 0x2)");
          if ( *(unsigned __int8 *)(v13 + 8) != *(unsigned __int8 *)(__n_4 + 8) )
            __assert_rtn("reconstructFrame", "RtpVideoQueue.c", 383, "nvPacket->flags == droppedNvPacket->flags");
          if ( *(_DWORD *)(v13 + 4) != *(_DWORD *)(__n_4 + 4) )
            __assert_rtn(
              "reconstructFrame",
              "RtpVideoQueue.c",
              384,
              "nvPacket->frameIndex == droppedNvPacket->frameIndex");
          if ( *(_DWORD *)v13 != *(_DWORD *)__n_4 )
            __assert_rtn(
              "reconstructFrame",
              "RtpVideoQueue.c",
              385,
              "nvPacket->streamPacketIndex == droppedNvPacket->streamPacketIndex");
          if ( *(unsigned __int8 *)(v13 + 9) != *(unsigned __int8 *)(__n_4 + 9) )
            __assert_rtn("reconstructFrame", "RtpVideoQueue.c", 386, "nvPacket->reserved == droppedNvPacket->reserved");
          v2 = 1;
          if ( (*((_BYTE *)a1 + 108) & 1) != 0 )
            v2 = *(unsigned __int8 *)(v13 + 11) == *(unsigned __int8 *)(__n_4 + 11);
          if ( !v2 )
            __assert_rtn(
              "reconstructFrame",
              "RtpVideoQueue.c",
              387,
              "!queue->multiFecCapable || nvPacket->multiFecBlocks == droppedNvPacket->multiFecBlocks");
          if ( memcmp((const void *)(v13 + 16), (const void *)(__n_4 + 16), __n) )
          {
            v6 = v13 + 16;
            v5 = __n_4 + 16;
            for ( k = 0; k < __n; ++k )
            {
              if ( *(unsigned __int8 *)(v6 + k) != *(unsigned __int8 *)(v5 + k) )
              {
                if ( off_100697310 )
                  ((void (*)(const char *, ...))off_100697310)(
                    "Recovery error at %d: expected 0x%02x, actual 0x%02x\n",
                    k,
                    *(unsigned __int8 *)(v5 + k),
                    *(unsigned __int8 *)(v6 + k));
                ++v7;
              }
            }
          }
          for ( m = v21 - v14 - 16; m < v10; ++m )
          {
            if ( *(_BYTE *)(v13 + 16 + m) )
            {
              if ( off_100697310 )
                ((void (*)(const char *, ...))off_100697310)(
                  "Recovery error at %d: expected 0x00, actual 0x%02x\n",
                  m,
                  *(unsigned __int8 *)(v13 + 16 + m));
              ++v7;
            }
          }
          if ( v7 )
            __assert_rtn("reconstructFrame", "RtpVideoQueue.c", 412, "recoveryErrors == 0");
LABEL_128:
          free((void *)v27[j]);
          continue;
        }
        if ( !j && (*(_BYTE *)(v13 + 8) & 4) == 0 )
        {
          v29 = -1;
          if ( off_100697310 )
            ((void (*)(const char *, ...))off_100697310)(
              "FEC recovery returned corrupt packet %d (frame %d)",
              *(unsigned __int16 *)(v15 + 2),
              *((_DWORD *)a1 + 26));
          goto LABEL_128;
        }
        if ( j == *((_DWORD *)a1 + 17) - 1 && (*(_BYTE *)(v13 + 8) & 2) == 0 )
        {
          v29 = -1;
          if ( off_100697310 )
            ((void (*)(const char *, ...))off_100697310)(
              "FEC recovery returned corrupt packet %d (frame %d)",
              *(unsigned __int16 *)(v15 + 2),
              *((_DWORD *)a1 + 26));
          goto LABEL_128;
        }
        if ( j && j < *((_DWORD *)a1 + 17) - 1 && (*(_BYTE *)(v13 + 8) & 1) == 0 )
        {
          v29 = -1;
          if ( off_100697310 )
            ((void (*)(const char *, ...))off_100697310)(
              "FEC recovery returned corrupt packet %d (frame %d)",
              *(unsigned __int16 *)(v15 + 2),
              *((_DWORD *)a1 + 26));
          goto LABEL_128;
        }
        if ( (*(_BYTE *)(v13 + 8) & 0xF8) != 0 )
        {
          v29 = -1;
          if ( off_100697310 )
            ((void (*)(const char *, ...))off_100697310)(
              "FEC recovery returned corrupt packet %d (frame %d)",
              *(unsigned __int16 *)(v15 + 2),
              *((_DWORD *)a1 + 26));
          goto LABEL_128;
        }
        if ( (unsigned __int16)(*(_WORD *)(v15 + 2) - *((_DWORD *)a1 + 16)) < 0x8000u )
          __assert_rtn(
            "reconstructFrame",
            "RtpVideoQueue.c",
            444,
            "(((unsigned short) (((rtpPacket->sequenceNumber) - (queue->bufferFirstParitySequenceNumber)) & 65535)) > (65535/2))");
        sub_10037A630(a1, v16, v15, (unsigned int)(dword_100697454 + v14), 0, 1);
      }
    }
  }
  v29 = -2;
LABEL_130:
  sub_10038C7E4(v28);
  if ( v27 )
    free(v27);
  if ( v26 )
    free(v26);
  return v29;
}

/* ========================================================================
 * xref 'predict' → sub_10037CEE8
 * EA: 0x10037cee8
 ======================================================================== */

__int64 __fastcall sub_10037CEE8(unsigned int a1, char a2)
{
  __int64 result; // x0
  char v3; // [xsp+1Bh] [xbp-5h]

  v3 = a2 & 1;
  if ( a1 < dword_100682EBC )
    __assert_rtn("notifyFrameLost", "VideoDepacketizer.c", 1123, "frameNumber >= startFrameNumber");
  result = sub_10037D078();
  if ( (byte_100682EC1 & 1) == 0 )
  {
    if ( (((unsigned __int8)byte_100682EC2 ^ 1) & 1) != 0 )
      __assert_rtn("notifyFrameLost", "VideoDepacketizer.c", 1130, "waitingForRefInvalFrame");
    if ( (v3 & 1) != 0 )
    {
      if ( off_100697310 )
        ((void (*)(const char *, ...))off_100697310)(
          "Sending speculative RFI request for predicted loss of frame %d\n",
          a1);
    }
    else if ( off_100697310 )
    {
      ((void (*)(const char *, ...))off_100697310)("Sending RFI request for unrecoverable frame %d\n", a1);
    }
    dword_100682EB8 = a1 + 1;
    return sub_100364A18((unsigned int)dword_100682EBC, a1);
  }
  return result;
}

/* --- Large named/sub depth-related functions (fallback) --- */

/* ========================================================================
 * fallback sub_1003d3094
 * EA: 0x1003d3094
 ======================================================================== */

bool __fastcall sub_1003D3094(void *a1, size_t a2, __int64 a3, unsigned int *a4)
{
  unsigned int v4; // w8
  __int64 v5; // x10
  unsigned int v6; // w8
  unsigned __int64 v7; // x8
  unsigned __int64 v8; // x9
  unsigned __int64 v9; // x11
  unsigned __int64 v10; // x10
  unsigned __int64 v11; // x1
  unsigned __int64 v12; // x19
  unsigned __int64 v13; // x12
  unsigned __int64 v14; // x15
  __int64 v15; // x16
  unsigned __int64 v16; // x14
  unsigned __int8 v17; // w7
  unsigned __int64 v18; // x8
  int v19; // w19
  __int32 v20; // w28
  __int32 v21; // w19
  int v22; // w24
  int v23; // w27
  __int32 v24; // w21
  __int32 v25; // w20
  int v26; // w25
  int v27; // w22
  __int32 v28; // w23
  __int32 v29; // w26
  __int64 i; // x8
  unsigned int v32; // w10
  _BYTE *v33; // x12
  unsigned __int64 v34; // x8
  _BYTE *v35; // x11
  __int64 v36; // x13
  unsigned int v37; // w8
  unsigned int v38; // w8
  unsigned int v39; // w8
  unsigned int v40; // w8
  unsigned int v41; // w8
  unsigned int v42; // w8
  unsigned int v43; // w8
  unsigned int v44; // w8
  unsigned int v45; // w8
  unsigned int v46; // w8
  unsigned int v47; // w8
  unsigned int v48; // w8
  unsigned int v49; // w8
  unsigned int v50; // w8
  int v51; // w15
  int v52; // w14
  int v53; // w16
  int v54; // w15
  int v55; // w14
  unsigned __int64 v56; // x14
  int v57; // w15
  int v58; // w16
  int v59; // w15
  __int64 v60; // x14
  unsigned __int64 v61; // x14
  int v62; // w15
  int v63; // w16
  int v64; // w15
  bool v65; // cf
  unsigned __int64 v66; // x14
  int v67; // w15
  int v68; // w16
  int v69; // w15
  unsigned __int64 v70; // x14
  int v71; // w15
  int v72; // w16
  int v73; // w15
  unsigned __int64 v74; // x14
  int v75; // w15
  int v76; // w16
  int v77; // w15
  __int64 j; // x8
  unsigned int v79; // w10
  _BYTE *v80; // x12
  unsigned __int64 v81; // x8
  _BYTE *v82; // x11
  __int64 v83; // x13
  int v84; // w15
  int v85; // w14
  int v86; // w16
  int v87; // w15
  int v88; // w14
  unsigned __int64 v89; // x14
  int v90; // w15
  int v91; // w16
  int v92; // w15
  __int64 v93; // x14
  unsigned __int64 v94; // x14
  int v95; // w15
  int v96; // w16
  int v97; // w15
  unsigned __int64 v98; // x14
  int v99; // w15
  int v100; // w16
  int v101; // w15
  unsigned __int64 v102; // x14
  int v103; // w15
  int v104; // w16
  int v105; // w15
  unsigned __int64 v106; // x14
  int v107; // w15
  int v108; // w16
  int v109; // w15
  __int64 v110; // x19
  __int64 v111; // x10
  unsigned __int8 v112; // w28
  int v113; // w28
  _DWORD *v114; // x28
  __int32 v115; // w16
  __int32 v116; // w17
  __int32 v117; // w12
  __int32 v118; // w13
  __int32 v119; // w14
  int32x2_t v120; // kr00_8
  char *v121; // x28
  __int64 v126; // [xsp+20h] [xbp-C10h]
  __int32 v127; // [xsp+30h] [xbp-C00h]
  int v128; // [xsp+38h] [xbp-BF8h]
  __int32 v129; // [xsp+38h] [xbp-BF8h]
  __int32 v130; // [xsp+3Ch] [xbp-BF4h]
  __int32 v131; // [xsp+3Ch] [xbp-BF4h]
  int v132; // [xsp+40h] [xbp-BF0h]
  __int32 v133; // [xsp+48h] [xbp-BE8h]
  int v134; // [xsp+50h] [xbp-BE0h]
  __int32 v135; // [xsp+58h] [xbp-BD8h]
  int v136; // [xsp+60h] [xbp-BD0h]
  __int32 v137; // [xsp+68h] [xbp-BC8h]
  int v138; // [xsp+70h] [xbp-BC0h]
  int v139; // [xsp+78h] [xbp-BB8h]
  int32x4_t v140; // [xsp+80h] [xbp-BB0h] BYREF
  int32x4_t v141; // [xsp+90h] [xbp-BA0h]
  int32x2_t v142; // [xsp+A0h] [xbp-B90h]
  __int128 v143; // [xsp+A8h] [xbp-B88h] BYREF
  __int128 v144; // [xsp+B8h] [xbp-B78h]
  __int64 v145; // [xsp+C8h] [xbp-B68h]
  _BYTE v146[40]; // [xsp+D0h] [xbp-B60h] BYREF
  int32x4_t v147; // [xsp+F8h] [xbp-B38h] BYREF
  int32x4_t v148; // [xsp+108h] [xbp-B28h]
  int32x2_t v149; // [xsp+118h] [xbp-B18h]
  _BYTE v150[64]; // [xsp+120h] [xbp-B10h] BYREF
  __int128 v151; // [xsp+160h] [xbp-AD0h] BYREF
  __int128 v152; // [xsp+170h] [xbp-AC0h]
  _OWORD v153[3]; // [xsp+180h] [xbp-AB0h] BYREF
  int v154; // [xsp+1B0h] [xbp-A80h] BYREF
  __int128 v155; // [xsp+1B4h] [xbp-A7Ch]
  __int128 v156; // [xsp+1C4h] [xbp-A6Ch]
  int v157; // [xsp+1D4h] [xbp-A5Ch]
  int32x4_t v158; // [xsp+1E0h] [xbp-A50h] BYREF
  int32x4_t v159; // [xsp+1F0h] [xbp-A40h]
  int32x2_t v160; // [xsp+200h] [xbp-A30h]
  _BYTE v161[32]; // [xsp+2C0h] [xbp-970h] BYREF
  int v162; // [xsp+2E0h] [xbp-950h] BYREF
  int v163; // [xsp+2E4h] [xbp-94Ch]
  int v164; // [xsp+2E8h] [xbp-948h]
  int v165; // [xsp+2ECh] [xbp-944h]
  int v166; // [xsp+2F0h] [xbp-940h]
  int v167; // [xsp+2F4h] [xbp-93Ch]
  int v168; // [xsp+2F8h] [xbp-938h]
  int v169; // [xsp+2FCh] [xbp-934h]
  int v170; // [xsp+300h] [xbp-930h]
  int v171; // [xsp+304h] [xbp-92Ch]
  __int64 v172; // [xsp+308h] [xbp-928h] BYREF
  __int64 v173; // [xsp+330h] [xbp-900h] BYREF
  __int64 v174; // [xsp+358h] [xbp-8D8h] BYREF
  int32x4_t v175; // [xsp+380h] [xbp-8B0h] BYREF
  int32x4_t v176; // [xsp+390h] [xbp-8A0h]
  int32x2_t v177; // [xsp+3A0h] [xbp-890h]
  __int128 v178; // [xsp+3A8h] [xbp-888h] BYREF
  __int128 v179; // [xsp+3B8h] [xbp-878h]
  __int64 v180; // [xsp+3C8h] [xbp-868h]
  __int128 v181; // [xsp+3D0h] [xbp-860h] BYREF
  __int128 v182; // [xsp+3E0h] [xbp-850h]
  __int64 v183; // [xsp+3F0h] [xbp-840h]
  _BYTE v184[40]; // [xsp+3F8h] [xbp-838h] BYREF
  int32x4_t v185; // [xsp+420h] [xbp-810h] BYREF
  int32x4_t v186; // [xsp+430h] [xbp-800h]
  int32x2_t v187; // [xsp+440h] [xbp-7F0h]
  int v188; // [xsp+448h] [xbp-7E8h] BYREF
  int v189; // [xsp+44Ch] [xbp-7E4h]
  int v190; // [xsp+450h] [xbp-7E0h]
  int v191; // [xsp+454h] [xbp-7DCh]
  int v192; // [xsp+458h] [xbp-7D8h]
  int v193; // [xsp+45Ch] [xbp-7D4h]
  int v194; // [xsp+460h] [xbp-7D0h]
  int v195; // [xsp+464h] [xbp-7CCh]
  int v196; // [xsp+468h] [xbp-7C8h]
  int v197; // [xsp+46Ch] [xbp-7C4h]
  int v198; // [xsp+470h] [xbp-7C0h] BYREF
  int v199; // [xsp+474h] [xbp-7BCh]
  int v200; // [xsp+478h] [xbp-7B8h]
  int v201; // [xsp+47Ch] [xbp-7B4h]
  int v202; // [xsp+480h] [xbp-7B0h]
  int v203; // [xsp+484h] [xbp-7ACh]
  int v204; // [xsp+488h] [xbp-7A8h]
  int v205; // [xsp+48Ch] [xbp-7A4h]
  int v206; // [xsp+490h] [xbp-7A0h]
  int v207; // [xsp+494h] [xbp-79Ch]
  int v208; // [xsp+498h] [xbp-798h] BYREF
  int v209; // [xsp+49Ch] [xbp-794h]
  int v210; // [xsp+4A0h] [xbp-790h]
  int v211; // [xsp+4A4h] [xbp-78Ch]
  int v212; // [xsp+4A8h] [xbp-788h]
  int v213; // [xsp+4ACh] [xbp-784h]
  int v214; // [xsp+4B0h] [xbp-780h]
  int v215; // [xsp+4B4h] [xbp-77Ch]
  int v216; // [xsp+4B8h] [xbp-778h]
  int v217; // [xsp+4BCh] [xbp-774h]
  _DWORD v218[20]; // [xsp+4C8h] [xbp-768h] BYREF
  __int128 v219; // [xsp+518h] [xbp-718h]
  __int128 v220; // [xsp+528h] [xbp-708h]
  __int64 v221; // [xsp+538h] [xbp-6F8h]
  __int64 v222; // [xsp+540h] [xbp-6F0h] BYREF
  _DWORD v223[20]; // [xsp+568h] [xbp-6C8h] BYREF
  __int128 v224; // [xsp+5B8h] [xbp-678h]
  __int128 v225; // [xsp+5C8h] [xbp-668h]
  __int64 v226; // [xsp+5D8h] [xbp-658h]
  __int64 v227; // [xsp+5E0h] [xbp-650h] BYREF
  _DWORD v228[20]; // [xsp+608h] [xbp-628h] BYREF
  __int128 v229; // [xsp+658h] [xbp-5D8h]
  __int128 v230; // [xsp+668h] [xbp-5C8h]
  __int64 v231; // [xsp+678h] [xbp-5B8h]
  __int64 v232; // [xsp+680h] [xbp-5B0h] BYREF
  _DWORD v233[20]; // [xsp+6A8h] [xbp-588h] BYREF
  __int128 v234; // [xsp+6F8h] [xbp-538h]
  __int128 v235; // [xsp+708h] [xbp-528h]
  __int64 v236; // [xsp+718h] [xbp-518h]
  __int64 v237; // [xsp+720h] [xbp-510h] BYREF
  _DWORD v238[20]; // [xsp+748h] [xbp-4E8h] BYREF
  __int128 v239; // [xsp+798h] [xbp-498h]
  __int128 v240; // [xsp+7A8h] [xbp-488h]
  __int64 v241; // [xsp+7B8h] [xbp-478h]
  __int64 v242; // [xsp+7C0h] [xbp-470h] BYREF
  _DWORD v243[20]; // [xsp+7E8h] [xbp-448h] BYREF
  __int128 v244; // [xsp+838h] [xbp-3F8h]
  __int128 v245; // [xsp+848h] [xbp-3E8h]
  __int64 v246; // [xsp+858h] [xbp-3D8h]
  __int64 v247; // [xsp+860h] [xbp-3D0h] BYREF
  _DWORD v248[20]; // [xsp+888h] [xbp-3A8h] BYREF
  __int128 v249; // [xsp+8D8h] [xbp-358h]
  __int128 v250; // [xsp+8E8h] [xbp-348h]
  __int64 v251; // [xsp+8F8h] [xbp-338h]
  _DWORD v252[30]; // [xsp+900h] [xbp-330h] BYREF
  __int128 v253; // [xsp+978h] [xbp-2B8h]
  __int128 v254; // [xsp+988h] [xbp-2A8h]
  __int64 v255; // [xsp+998h] [xbp-298h]
  __int64 v256; // [xsp+9A0h] [xbp-290h] BYREF
  _BYTE v257[2]; // [xsp+9C8h] [xbp-268h] BYREF
  _BYTE v258[254]; // [xsp+9CAh] [xbp-266h] BYREF
  _BYTE v259[2]; // [xsp+AC8h] [xbp-168h] BYREF
  _BYTE v260[254]; // [xsp+ACAh] [xbp-166h] BYREF

  v4 = *(unsigned __int8 *)(a3 + 63);
  if ( v4 > 0x10 )
    return 0;
  v5 = a3 + 32;
  if ( v4 == 16 )
  {
    if ( *(_QWORD *)(a3 + 48) | *(_QWORD *)(a3 + 55) )
      return 0;
    v6 = *(unsigned __int8 *)(a3 + 47);
    if ( v6 >= 0x14 )
    {
      if ( v6 != 20 )
        return 0;
      v37 = *(unsigned __int8 *)(a3 + 46);
      if ( v37 >= 0xDE )
      {
        if ( v37 != 222 )
          return 0;
        v38 = *(unsigned __int8 *)(a3 + 45);
        if ( v38 >= 0xF9 )
        {
          if ( v38 != 249 )
            return 0;
          v39 = *(unsigned __int8 *)(a3 + 44);
          if ( v39 >= 0xDE )
          {
            if ( v39 != 222 )
              return 0;
            v40 = *(unsigned __int8 *)(a3 + 43);
            if ( v40 >= 0xA2 )
            {
              if ( v40 != 162 )
                return 0;
              v41 = *(unsigned __int8 *)(a3 + 42);
              if ( v41 >= 0xF7 )
              {
                if ( v41 != 247 )
                  return 0;
                v42 = *(unsigned __int8 *)(a3 + 41);
                if ( v42 >= 0x9C )
                {
                  if ( v42 != 156 )
                    return 0;
                  v43 = *(unsigned __int8 *)(a3 + 40);
                  if ( v43 >= 0xD6 )
                  {
                    if ( v43 != 214 )
                      return 0;
                    v44 = *(unsigned __int8 *)(a3 + 39);
                    v5 = a3 + 32;
                    if ( v44 >= 0x58 )
                    {
                      if ( v44 != 88 )
                        return 0;
                      v45 = *(unsigned __int8 *)(a3 + 38);
                      v5 = a3 + 32;
                      if ( v45 >= 0x12 )
                      {
                        if ( v45 != 18 )
                          return 0;
                        v46 = *(unsigned __int8 *)(a3 + 37);
                        v5 = a3 + 32;
                        if ( v46 >= 0x63 )
                        {
                          if ( v46 != 99 )
                            return 0;
                          v47 = *(unsigned __int8 *)(a3 + 36);
                          v5 = a3 + 32;
                          if ( v47 >= 0x1A )
                          {
                            if ( v47 != 26 )
                              return 0;
                            v48 = *(unsigned __int8 *)(a3 + 35);
                            v5 = a3 + 32;
                            if ( v48 >= 0x5C )
                            {
                              if ( v48 != 92 )
                                return 0;
                              v49 = *(unsigned __int8 *)(a3 + 34);
                              v5 = a3 + 32;
                              if ( v49 >= 0xF5 )
                              {
                                if ( v49 != 245 )
                                  return 0;
                                v50 = *(unsigned __int8 *)(a3 + 33);
                                v5 = a3 + 32;
                                if ( v50 >= 0xD3 )
                                {
                                  if ( v50 != 211 )
                                    return 0;
                                  v5 = a3 + 32;
                                  if ( *(unsigned __int8 *)(a3 + 32) >= 0xEDu )
                                    return 0;
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  v126 = v5;
  v7 = (*((unsigned __int8 *)a4 + 4) << 6) & 0x3FFF
     | ((unsigned __int64)*((unsigned __int8 *)a4 + 5) << 14) & 0xFFFFFFFFC03FFFFFLL
     | ((unsigned __int64)*((unsigned __int8 *)a4 + 6) << 22);
  v8 = (8 * *((unsigned __int8 *)a4 + 10)) & 0x7FF
     | ((unsigned __int64)*((unsigned __int8 *)a4 + 11) << 11) & 0xFFFFFFFFF807FFFFLL
     | ((unsigned __int64)*((unsigned __int8 *)a4 + 12) << 19);
  v9 = (32 * *((unsigned __int8 *)a4 + 23)) & 0x1FFF
     | ((unsigned __int64)*((unsigned __int8 *)a4 + 24) << 13) & 0xFFFFFFFFE01FFFFFLL
     | ((unsigned __int64)*((unsigned __int8 *)a4 + 25) << 21);
  v10 = (16 * *((unsigned __int8 *)a4 + 26)) & 0xFFF | ((unsigned __int64)*((unsigned __int8 *)a4 + 27) << 12);
  v11 = ((32 * *((unsigned __int8 *)a4 + 7)) & 0x1FFF
       | ((unsigned __int64)*((unsigned __int8 *)a4 + 8) << 13) & 0xFFFFFFFFE01FFFFFLL
       | ((unsigned __int64)*((unsigned __int8 *)a4 + 9) << 21))
      + ((v7 + 0x1000000) >> 25);
  v12 = ((4 * *((unsigned __int8 *)a4 + 13)) & 0x3FF
       | ((unsigned __int64)*((unsigned __int8 *)a4 + 14) << 10) & 0xFFFFFFFFFC03FFFFLL
       | ((unsigned __int64)*((unsigned __int8 *)a4 + 15) << 18))
      + ((v8 + 0x1000000) >> 25);
  v13 = (4 * *((unsigned __int8 *)a4 + 29)) & 0x3FF
      | ((unsigned __int64)*((unsigned __int8 *)a4 + 30) << 10)
      | (*((unsigned __int8 *)a4 + 31) << 18) & 0x1FC0000;
  v14 = *a4 + 19 * ((v13 + 0x1000000) >> 25);
  v15 = a4[4];
  v16 = ((*((unsigned __int8 *)a4 + 20) << 7) & 0x7FFF
       | ((unsigned __int64)*((unsigned __int8 *)a4 + 21) << 15) & 0xFFFFFFFF807FFFFFLL
       | ((unsigned __int64)*((unsigned __int8 *)a4 + 22) << 23))
      + ((unsigned __int64)(v15 + 0x1000000) >> 25);
  v17 = *((_BYTE *)a4 + 28);
  LODWORD(v143) = v14 - ((v14 + 0x2000000) & 0xFC000000);
  DWORD1(v143) = v7 - ((v7 + 0x1000000) & 0x7E000000) + ((v14 + 0x2000000) >> 26);
  DWORD2(v143) = v11 - ((v11 + 0x2000000) & 0xFC000000);
  HIDWORD(v143) = ((v11 + 0x2000000) >> 26) + v8 - ((v8 + 0x1000000) & 0xE000000);
  LODWORD(v144) = v12 - ((v12 + 0x2000000) & 0xFC000000);
  DWORD1(v144) = v15 + ((v12 + 0x2000000) >> 26) - ((v15 + 0x1000000) & 0xFE000000);
  v18 = (v10 & 0xFFFFFFFFF00FFFFFLL | ((unsigned __int64)v17 << 20)) + ((v9 + 0x1000000) >> 25);
  DWORD2(v144) = v16 - ((v16 + 0x2000000) & 0xFC000000);
  HIDWORD(v144) = ((v16 + 0x2000000) >> 26) + v9 - ((v9 + 0x1000000) & 0x3E000000);
  LODWORD(v145) = v18 - ((v18 + 0x2000000) & 0xFC000000);
  HIDWORD(v145) = v13 + ((v18 + 0x2000000) >> 26) - ((v13 + 0x1000000) & 0x2000000);
  memset(&v146[4], 0, 36);
  *(_DWORD *)v146 = 1;
  sub_1003DA00C(&v158);
  sub_1003D8E14(&v185, &v158, &unk_100561A94);
  v158 = vsubq_s32(v158, *(int32x4_t *)v146);
  v159 = vsubq_s32(v159, *(int32x4_t *)&v146[16]);
  v160 = vsub_s32(v160, *(int32x2_t *)&v146[32]);
  v185 = vaddq_s32(v185, *(int32x4_t *)v146);
  v186 = vaddq_s32(v186, *(int32x4_t *)&v146[16]);
  v187 = vadd_s32(v187, *(int32x2_t *)&v146[32]);
  sub_1003DA00C(&v175);
  sub_1003D8E14(&v175, &v175, &v185);
  sub_1003DA00C(&v140);
  sub_1003D8E14(&v140, &v140, &v185);
  sub_1003D8E14(&v140, &v140, &v158);
  sub_1003DA00C(v218);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003D8E14(v259, &v140, v259);
  sub_1003D8E14(v218, v218, v259);
  sub_1003DA00C(v218);
  sub_1003D8E14(v218, v259, v218);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003D8E14(v218, v259, v218);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003D8E14(v259, v259, v218);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003DA00C(v257);
  sub_1003D8E14(v259, v257, v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003D8E14(v218, v259, v218);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003D8E14(v259, v259, v218);
  sub_1003DA00C(v257);
  v19 = 99;
  do
  {
    sub_1003DA00C(v257);
    --v19;
  }
  while ( v19 );
  sub_1003D8E14(v259, v257, v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003DA00C(v259);
  sub_1003D8E14(v218, v259, v218);
  sub_1003DA00C(v218);
  sub_1003DA00C(v218);
  sub_1003D8E14(&v140, v218, &v140);
  sub_1003D8E14(&v140, &v140, &v175);
  sub_1003D8E14(&v140, &v140, &v158);
  sub_1003DA00C(&v162);
  sub_1003D8E14(&v162, &v162, &v185);
  v139 = v162;
  v135 = v158.i32[0];
  v138 = v163;
  v137 = v158.i32[1];
  LODWORD(v151) = v162 - v158.i32[0];
  DWORD1(v151) = v163 - v158.i32[1];
  v136 = v164;
  v134 = v165;
  v133 = v158.i32[3];
  DWORD2(v151) = v164 - v158.i32[2];
  HIDWORD(v151) = v165 - v158.i32[3];
  v20 = v159.i32[0];
  v21 = v159.i32[1];
  v130 = v158.i32[2];
  v132 = v166;
  v128 = v167;
  LODWORD(v152) = v166 - v159.i32[0];
  DWORD1(v152) = v167 - v159.i32[1];
  v22 = v168;
  v23 = v169;
  v24 = v159.i32[2];
  v25 = v159.i32[3];
  DWORD2(v152) = v168 - v159.i32[2];
  HIDWORD(v152) = v169 - v159.i32[3];
  v26 = v170;
  v27 = v171;
  v28 = v160.i32[0];
  v29 = v160.i32[1];
  LODWORD(v153[0]) = v170 - v160.i32[0];
  DWORD1(v153[0]) = v171 - v160.i32[1];
  sub_1003D91D8(v161, &v151);
  if ( (unsigned int)sub_100391BA0(v161, &unk_100561AE4, 32) )
  {
    LODWORD(v151) = v135 + v139;
    DWORD1(v151) = v137 + v138;
    DWORD2(v151) = v130 + v136;
    HIDWORD(v151) = v133 + v134;
    LODWORD(v152) = v20 + v132;
    DWORD1(v152) = v21 + v128;
    DWORD2(v152) = v24 + v22;
    HIDWORD(v152) = v25 + v23;
    LODWORD(v153[0]) = v28 + v26;
    DWORD1(v153[0]) = v29 + v27;
    sub_1003D91D8(v161, &v151);
    if ( (unsigned int)sub_100391BA0(v161, &unk_100561AE4, 32) )
      return 0;
    sub_1003D8E14(&v140, &v140, &unk_100561ABC);
  }
  sub_1003D91D8(v161, &v140);
  if ( (v161[0] & 1) != *((unsigned __int8 *)a4 + 31) >> 7 )
  {
    v140 = vnegq_s32(v140);
    v141 = vnegq_s32(v141);
    v142 = vneg_s32(v142);
  }
  sub_1003D8E14(&v147, &v140, &v143);
  v140 = vnegq_s32(v140);
  v141 = vnegq_s32(v141);
  v142 = vneg_s32(v142);
  v147 = vnegq_s32(v147);
  v148 = vnegq_s32(v148);
  v149 = vneg_s32(v149);
  sub_100466EA0(&v158);
  sub_100467180((int)&v158, (void *)a3, 0x20u);
  sub_100467180((int)&v158, a4, 0x20u);
  sub_100467180((int)&v158, a1, a2);
  sub_100466EE0(v150, &v158);
  sub_1003D235C(v150);
  for ( i = 0; i != 256; i += 2 )
  {
    v32 = (unsigned __int8)v150[(unsigned int)i >> 3];
    v33 = &v259[i];
    *v33 = (v32 >> (i & 6)) & 1;
    v33[1] = (v32 >> (i & 6 | 1)) & 1;
  }
  v34 = 0;
  v35 = v260;
  v36 = 1;
  do
  {
    if ( !v259[v34] || v34 > 0xFE )
      goto LABEL_48;
    v51 = (char)v259[v36];
    if ( v259[v36] )
    {
      v52 = (char)v259[v34];
      v53 = 2 * v51;
      v54 = 2 * v51 + v52;
      if ( v54 > 15 )
      {
        v55 = v52 - v53;
        if ( v55 < -15 )
          goto LABEL_48;
        v259[v34] = v55;
        v56 = v34;
        while ( v259[v56 + 1] )
        {
          v259[++v56] = 0;
          if ( v56 >= 0xFF )
            goto LABEL_59;
        }
        v259[v56 + 1] = 1;
        if ( v34 >= 0xFE )
          goto LABEL_48;
      }
      else
      {
        v259[v34] = v54;
        v259[v36] = 0;
        if ( v34 >= 0xFE )
          goto LABEL_48;
      }
    }
    else
    {
LABEL_59:
      if ( v34 >= 0xFE )
        goto LABEL_48;
    }
    if ( v259[v36 + 1] )
    {
      v57 = (char)v259[v34];
      v58 = 4 * (char)v259[v36 + 1];
      if ( v58 + v57 >= 16 )
      {
        v59 = v57 - v58;
        if ( v59 < -15 )
          goto LABEL_48;
        v60 = 0;
        v259[v34] = v59;
        while ( v35[v60] )
        {
          v35[v60++] = 0;
          if ( (unsigned __int64)(v36 + v60) >= 0xFF )
            goto LABEL_68;
        }
        v35[v60] = 1;
        if ( v34 > 0xFC )
          goto LABEL_48;
      }
      else
      {
        v259[v34] = v58 + v57;
        v259[v36 + 1] = 0;
        if ( v34 > 0xFC )
          goto LABEL_48;
      }
    }
    else
    {
LABEL_68:
      if ( v34 > 0xFC )
        goto LABEL_48;
    }
    v61 = v36 + 2;
    if ( v259[v36 + 2] )
    {
      v62 = (char)v259[v34];
      v63 = 8 * (char)v259[v36 + 2];
      if ( v63 + v62 >= 16 )
      {
        v64 = v62 - v63;
        if ( v64 < -15 )
          goto LABEL_48;
        v259[v34] = v64;
        while ( v259[v61] )
        {
          v259[v61] = 0;
          v65 = v61++ >= 0xFF;
          if ( v65 )
            goto LABEL_80;
        }
        v259[v61] = 1;
        if ( v34 > 0xFB )
          goto LABEL_48;
      }
      else
      {
        v259[v34] = v63 + v62;
        v259[v61] = 0;
        if ( v34 > 0xFB )
          goto LABEL_48;
      }
    }
    else
    {
LABEL_80:
      if ( v34 > 0xFB )
        goto LABEL_48;
    }
    v66 = v36 + 3;
    if ( v259[v36 + 3] )
    {
      v67 = (char)v259[v34];
      v68 = 16 * (char)v259[v36 + 3];
      if ( v68 + v67 >= 16 )
      {
        v69 = v67 - v68;
        if ( v69 < -15 )
          goto LABEL_48;
        v259[v34] = v69;
        while ( v259[v66] )
        {
          v259[v66] = 0;
          v65 = v66++ >= 0xFF;
          if ( v65 )
            goto LABEL_95;
        }
        v259[v66] = 1;
      }
      else
      {
        v259[v34] = v68 + v67;
        v259[v66] = 0;
      }
    }
LABEL_95:
    if ( v34 <= 0xFA )
    {
      v70 = v36 + 4;
      if ( v259[v36 + 4] )
      {
        v71 = (char)v259[v34];
        v72 = 32 * (char)v259[v36 + 4];
        if ( v72 + v71 >= 16 )
        {
          v73 = v71 - v72;
          if ( v73 < -15 )
            goto LABEL_48;
          v259[v34] = v73;
          while ( v259[v70] )
          {
            v259[v70] = 0;
            v65 = v70++ >= 0xFF;
            if ( v65 )
              goto LABEL_106;
          }
          v259[v70] = 1;
        }
        else
        {
          v259[v34] = v72 + v71;
          v259[v70] = 0;
        }
      }
LABEL_106:
      if ( v34 <= 0xF9 )
      {
        v74 = v36 + 5;
        if ( v259[v36 + 5] )
        {
          v75 = (char)v259[v34];
          v76 = (char)v259[v36 + 5] << 6;
          if ( v76 + v75 < 16 )
          {
            v259[v34] = v76 + v75;
            v259[v74] = 0;
          }
          else
          {
            v77 = v75 - v76;
            if ( v77 >= -15 )
            {
              v259[v34] = v77;
              while ( v259[v74] )
              {
                v259[v74] = 0;
                v65 = v74++ >= 0xFF;
                if ( v65 )
                  goto LABEL_48;
              }
              v259[v74] = 1;
            }
          }
        }
      }
    }
LABEL_48:
    ++v34;
    ++v36;
    ++v35;
  }
  while ( v34 != 256 );
  for ( j = 0; j != 256; j += 2 )
  {
    v79 = *(unsigned __int8 *)(v126 + ((unsigned int)j >> 3));
    v80 = &v257[j];
    *v80 = (v79 >> (j & 6)) & 1;
    v80[1] = (v79 >> (j & 6 | 1)) & 1;
  }
  v81 = 0;
  v82 = v258;
  v83 = 1;
  while ( 2 )
  {
    if ( v257[v81] && v81 <= 0xFE )
    {
      v84 = (char)v257[v83];
      if ( v257[v83] )
      {
        v85 = (char)v257[v81];
        v86 = 2 * v84;
        v87 = 2 * v84 + v85;
        if ( v87 > 15 )
        {
          v88 = v85 - v86;
          if ( v88 >= -15 )
          {
            v257[v81] = v88;
            v89 = v81;
            while ( v257[v89 + 1] )
            {
              v257[++v89] = 0;
              if ( v89 >= 0xFF )
                goto LABEL_131;
            }
            v257[v89 + 1] = 1;
            if ( v81 < 0xFE )
            {
LABEL_132:
              if ( v257[v83 + 1] )
              {
                v90 = (char)v257[v81];
                v91 = 4 * (char)v257[v83 + 1];
                if ( v91 + v90 >= 16 )
                {
                  v92 = v90 - v91;
                  if ( v92 < -15 )
                    goto LABEL_120;
                  v93 = 0;
                  v257[v81] = v92;
                  while ( v82[v93] )
                  {
                    v82[v93++] = 0;
                    if ( (unsigned __int64)(v83 + v93) >= 0xFF )
                      goto LABEL_140;
                  }
                  v82[v93] = 1;
                  if ( v81 > 0xFC )
                    goto LABEL_120;
                }
                else
                {
                  v257[v81] = v91 + v90;
                  v257[v83 + 1] = 0;
                  if ( v81 > 0xFC )
                    goto LABEL_120;
                }
              }
              else
              {
LABEL_140:
                if ( v81 > 0xFC )
                  goto LABEL_120;
              }
              v94 = v83 + 2;
              if ( v257[v83 + 2] )
              {
                v95 = (char)v257[v81];
                v96 = 8 * (char)v257[v83 + 2];
                if ( v96 + v95 >= 16 )
                {
                  v97 = v95 - v96;
                  if ( v97 < -15 )
                    goto LABEL_120;
                  v257[v81] = v97;
                  while ( v257[v94] )
                  {
                    v257[v94] = 0;
                    v65 = v94++ >= 0xFF;
                    if ( v65 )
                      goto LABEL_152;
                  }
                  v257[v94] = 1;
                  if ( v81 > 0xFB )
                    goto LABEL_120;
                }
                else
                {
                  v257[v81] = v96 + v95;
                  v257[v94] = 0;
                  if ( v81 > 0xFB )
                    goto LABEL_120;
                }
              }
              else
              {
LABEL_152:
                if ( v81 > 0xFB )
                  goto LABEL_120;
              }
              v98 = v83 + 3;
              if ( v257[v83 + 3] )
              {
                v99 = (char)v257[v81];
                v100 = 16 * (char)v257[v83 + 3];
                if ( v100 + v99 >= 16 )
                {
                  v101 = v99 - v100;
                  if ( v101 < -15 )
                    goto LABEL_120;
                  v257[v81] = v101;
                  while ( v257[v98] )
                  {
                    v257[v98] = 0;
                    v65 = v98++ >= 0xFF;
                    if ( v65 )
                      goto LABEL_167;
                  }
                  v257[v98] = 1;
                }
                else
                {
                  v257[v81] = v100 + v99;
                  v257[v98] = 0;
                }
              }
LABEL_167:
              if ( v81 <= 0xFA )
              {
                v102 = v83 + 4;
                if ( v257[v83 + 4] )
                {
                  v103 = (char)v257[v81];
                  v104 = 32 * (char)v257[v83 + 4];
                  if ( v104 + v103 >= 16 )
                  {
                    v105 = v103 - v104;
                    if ( v105 < -15 )
                      goto LABEL_120;
                    v257[v81] = v105;
                    while ( v257[v102] )
                    {
                      v257[v102] = 0;
                      v65 = v102++ >= 0xFF;
                      if ( v65 )
                        goto LABEL_178;
                    }
                    v257[v102] = 1;
                  }
                  else
                  {
                    v257[v81] = v104 + v103;
                    v257[v102] = 0;
                  }
                }
LABEL_178:
                if ( v81 <= 0xF9 )
                {
                  v106 = v83 + 5;
                  if ( v257[v83 + 5] )
                  {
                    v107 = (char)v257[v81];
                    v108 = (char)v257[v83 + 5] << 6;
                    if ( v108 + v107 < 16 )
                    {
                      v257[v81] = v108 + v107;
                      v257[v106] = 0;
                    }
                    else
                    {
                      v109 = v107 - v108;
                      if ( v109 >= -15 )
                      {
                        v257[v81] = v109;
                        while ( v257[v106] )
                        {
                          v257[v106] = 0;
                          v65 = v106++ >= 0xFF;
                          if ( v65 )
                            goto LABEL_120;
                        }
                        v257[v106] = 1;
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else
        {
          v257[v81] = v87;
          v257[v83] = 0;
          if ( v81 < 0xFE )
            goto LABEL_132;
        }
      }
      else
      {
LABEL_131:
        if ( v81 < 0xFE )
          goto LABEL_132;
      }
    }
LABEL_120:
    ++v81;
    ++v83;
    ++v82;
    if ( v81 != 256 )
      continue;
    break;
  }
  v218[0] = v140.i32[0] + v143;
  v218[1] = v140.i32[1] + DWORD1(v143);
  v218[2] = v140.i32[2] + DWORD2(v143);
  v218[3] = v140.i32[3] + HIDWORD(v143);
  v218[4] = v141.i32[0] + v144;
  v218[5] = v141.i32[1] + DWORD1(v144);
  v218[6] = v141.i32[2] + DWORD2(v144);
  v218[7] = v141.i32[3] + HIDWORD(v144);
  v218[8] = v142.i32[0] + v145;
  v218[9] = v142.i32[1] + HIDWORD(v145);
  v218[10] = v143 - v140.i32[0];
  v218[11] = DWORD1(v143) - v140.i32[1];
  v218[12] = DWORD2(v143) - v140.i32[2];
  v218[13] = HIDWORD(v143) - v140.i32[3];
  v218[14] = v144 - v141.i32[0];
  v218[15] = DWORD1(v144) - v141.i32[1];
  v218[16] = DWORD2(v144) - v141.i32[2];
  v218[17] = HIDWORD(v144) - v141.i32[3];
  v218[18] = v145 - v142.i32[0];
  v218[19] = HIDWORD(v145) - v142.i32[1];
  v221 = *(_QWORD *)&v146[32];
  v219 = *(_OWORD *)v146;
  v220 = *(_OWORD *)&v146[16];
  sub_1003D8E14(&v222, &v147, &unk_100561EC4);
  v175 = v140;
  v176 = v141;
  v178 = v143;
  v179 = v144;
  v177 = v142;
  v180 = v145;
  v181 = *(_OWORD *)v146;
  v182 = *(_OWORD *)&v146[16];
  v183 = *(_QWORD *)&v146[32];
  sub_1003D9B58(&v185, &v175);
  sub_1003D8E14(&v162, &v185, &v208);
  sub_1003D8E14(&v172, &v188, &v198);
  sub_1003D8E14(&v173, &v198, &v208);
  sub_1003D8E14(&v174, &v185, &v188);
  sub_1003DA258(&v185, &v162, v218);
  sub_1003D8E14(&v175, &v185, &v208);
  sub_1003D8E14(&v178, &v188, &v198);
  sub_1003D8E14(&v181, &v198, &v208);
  sub_1003D8E14(v184, &v185, &v188);
  v223[0] = v175.i32[0] + v178;
  v223[1] = v175.i32[1] + DWORD1(v178);
  v223[2] = v175.i32[2] + DWORD2(v178);
  v223[3] = v175.i32[3] + HIDWORD(v178);
  v223[4] = v176.i32[0] + v179;
  v223[5] = v176.i32[1] + DWORD1(v179);
  v223[6] = v176.i32[2] + DWORD2(v179);
  v223[7] = v176.i32[3] + HIDWORD(v179);
  v223[8] = v177.i32[0] + v180;
  v223[9] = v177.i32[1] + HIDWORD(v180);
  v223[10] = v178 - v175.i32[0];
  v223[11] = DWORD1(v178) - v175.i32[1];
  v223[12] = DWORD2(v178) - v175.i32[2];
  v223[13] = HIDWORD(v178) - v175.i32[3];
  v223[14] = v179 - v176.i32[0];
  v223[15] = DWORD1(v179) - v176.i32[1];
  v223[16] = DWORD2(v179) - v176.i32[2];
  v223[17] = HIDWORD(v179) - v176.i32[3];
  v223[18] = v180 - v177.i32[0];
  v223[19] = HIDWORD(v180) - v177.i32[1];
  v226 = v183;
  v224 = v181;
  v225 = v182;
  sub_1003D8E14(&v227, v184, &unk_100561EC4);
  sub_1003DA258(&v185, &v162, v223);
  sub_1003D8E14(&v175, &v185, &v208);
  sub_1003D8E14(&v178, &v188, &v198);
  sub_1003D8E14(&v181, &v198, &v208);
  sub_1003D8E14(v184, &v185, &v188);
  v228[0] = v175.i32[0] + v178;
  v228[1] = v175.i32[1] + DWORD1(v178);
  v228[2] = v175.i32[2] + DWORD2(v178);
  v228[3] = v175.i32[3] + HIDWORD(v178);
  v228[4] = v176.i32[0] + v179;
  v228[5] = v176.i32[1] + DWORD1(v179);
  v228[6] = v176.i32[2] + DWORD2(v179);
  v228[7] = v176.i32[3] + HIDWORD(v179);
  v228[8] = v177.i32[0] + v180;
  v228[9] = v177.i32[1] + HIDWORD(v180);
  v228[10] = v178 - v175.i32[0];
  v228[11] = DWORD1(v178) - v175.i32[1];
  v228[12] = DWORD2(v178) - v175.i32[2];
  v228[13] = HIDWORD(v178) - v175.i32[3];
  v228[14] = v179 - v176.i32[0];
  v228[15] = DWORD1(v179) - v176.i32[1];
  v228[16] = DWORD2(v179) - v176.i32[2];
  v228[17] = HIDWORD(v179) - v176.i32[3];
  v228[18] = v180 - v177.i32[0];
  v228[19] = HIDWORD(v180) - v177.i32[1];
  v231 = v183;
  v229 = v181;
  v230 = v182;
  sub_1003D8E14(&v232, v184, &unk_100561EC4);
  sub_1003DA258(&v185, &v162, v228);
  sub_1003D8E14(&v175, &v185, &v208);
  sub_1003D8E14(&v178, &v188, &v198);
  sub_1003D8E14(&v181, &v198, &v208);
  sub_1003D8E14(v184, &v185, &v188);
  v233[0] = v175.i32[0] + v178;
  v233[1] = v175.i32[1] + DWORD1(v178);
  v233[2] = v175.i32[2] + DWORD2(v178);
  v233[3] = v175.i32[3] + HIDWORD(v178);
  v233[4] = v176.i32[0] + v179;
  v233[5] = v176.i32[1] + DWORD1(v179);
  v233[6] = v176.i32[2] + DWORD2(v179);
  v233[7] = v176.i32[3] + HIDWORD(v179);
  v233[8] = v177.i32[0] + v180;
  v233[9] = v177.i32[1] + HIDWORD(v180);
  v233[10] = v178 - v175.i32[0];
  v233[11] = DWORD1(v178) - v175.i32[1];
  v233[12] = DWORD2(v178) - v175.i32[2];
  v233[13] = HIDWORD(v178) - v175.i32[3];
  v233[14] = v179 - v176.i32[0];
  v233[15] = DWORD1(v179) - v176.i32[1];
  v233[16] = DWORD2(v179) - v176.i32[2];
  v233[17] = HIDWORD(v179) - v176.i32[3];
  v233[18] = v180 - v177.i32[0];
  v233[19] = HIDWORD(v180) - v177.i32[1];
  v236 = v183;
  v234 = v181;
  v235 = v182;
  sub_1003D8E14(&v237, v184, &unk_100561EC4);
  sub_1003DA258(&v185, &v162, v233);
  sub_1003D8E14(&v175, &v185, &v208);
  sub_1003D8E14(&v178, &v188, &v198);
  sub_1003D8E14(&v181, &v198, &v208);
  sub_1003D8E14(v184, &v185, &v188);
  v238[0] = v175.i32[0] + v178;
  v238[1] = v175.i32[1] + DWORD1(v178);
  v238[2] = v175.i32[2] + DWORD2(v178);
  v238[3] = v175.i32[3] + HIDWORD(v178);
  v238[4] = v176.i32[0] + v179;
  v238[5] = v176.i32[1] + DWORD1(v179);
  v238[6] = v176.i32[2] + DWORD2(v179);
  v238[7] = v176.i32[3] + HIDWORD(v179);
  v238[8] = v177.i32[0] + v180;
  v238[9] = v177.i32[1] + HIDWORD(v180);
  v238[10] = v178 - v175.i32[0];
  v238[11] = DWORD1(v178) - v175.i32[1];
  v238[12] = DWORD2(v178) - v175.i32[2];
  v238[13] = HIDWORD(v178) - v175.i32[3];
  v238[14] = v179 - v176.i32[0];
  v238[15] = DWORD1(v179) - v176.i32[1];
  v238[16] = DWORD2(v179) - v176.i32[2];
  v238[17] = HIDWORD(v179) - v176.i32[3];
  v238[18] = v180 - v177.i32[0];
  v238[19] = HIDWORD(v180) - v177.i32[1];
  v241 = v183;
  v239 = v181;
  v240 = v182;
  sub_1003D8E14(&v242, v184, &unk_100561EC4);
  sub_1003DA258(&v185, &v162, v238);
  sub_1003D8E14(&v175, &v185, &v208);
  sub_1003D8E14(&v178, &v188, &v198);
  sub_1003D8E14(&v181, &v198, &v208);
  sub_1003D8E14(v184, &v185, &v188);
  v243[0] = v175.i32[0] + v178;
  v243[1] = v175.i32[1] + DWORD1(v178);
  v243[2] = v175.i32[2] + DWORD2(v178);
  v243[3] = v175.i32[3] + HIDWORD(v178);
  v243[4] = v176.i32[0] + v179;
  v243[5] = v176.i32[1] + DWORD1(v179);
  v243[6] = v176.i32[2] + DWORD2(v179);
  v243[7] = v176.i32[3] + HIDWORD(v179);
  v243[8] = v177.i32[0] + v180;
  v243[9] = v177.i32[1] + HIDWORD(v180);
  v243[10] = v178 - v175.i32[0];
  v243[11] = DWORD1(v178) - v175.i32[1];
  v243[12] = DWORD2(v178) - v175.i32[2];
  v243[13] = HIDWORD(v178) - v175.i32[3];
  v243[14] = v179 - v176.i32[0];
  v243[15] = DWORD1(v179) - v176.i32[1];
  v243[16] = DWORD2(v179) - v176.i32[2];
  v243[17] = HIDWORD(v179) - v176.i32[3];
  v243[18] = v180 - v177.i32[0];
  v243[19] = HIDWORD(v180) - v177.i32[1];
  v246 = v183;
  v244 = v181;
  v245 = v182;
  sub_1003D8E14(&v247, v184, &unk_100561EC4);
  sub_1003DA258(&v185, &v162, v243);
  sub_1003D8E14(&v175, &v185, &v208);
  sub_1003D8E14(&v178, &v188, &v198);
  sub_1003D8E14(&v181, &v198, &v208);
  sub_1003D8E14(v184, &v185, &v188);
  v250 = v182;
  v248[0] = v175.i32[0] + v178;
  v248[1] = v175.i32[1] + DWORD1(v178);
  v248[2] = v175.i32[2] + DWORD2(v178);
  v248[3] = v175.i32[3] + HIDWORD(v178);
  v248[4] = v176.i32[0] + v179;
  v248[5] = v176.i32[1] + DWORD1(v179);
  v248[6] = v176.i32[2] + DWORD2(v179);
  v248[7] = v176.i32[3] + HIDWORD(v179);
  v248[8] = v177.i32[0] + v180;
  v248[9] = v177.i32[1] + HIDWORD(v180);
  v248[10] = v178 - v175.i32[0];
  v248[11] = DWORD1(v178) - v175.i32[1];
  v248[12] = DWORD2(v178) - v175.i32[2];
  v248[13] = HIDWORD(v178) - v175.i32[3];
  v248[14] = v179 - v176.i32[0];
  v248[15] = DWORD1(v179) - v176.i32[1];
  v248[16] = DWORD2(v179) - v176.i32[2];
  v248[17] = HIDWORD(v179) - v176.i32[3];
  v248[18] = v180 - v177.i32[0];
  v248[19] = HIDWORD(v180) - v177.i32[1];
  v251 = v183;
  v249 = v181;
  sub_1003D8E14(v252, v184, &unk_100561EC4);
  sub_1003DA258(&v185, &v162, v248);
  sub_1003D8E14(&v175, &v185, &v208);
  sub_1003D8E14(&v178, &v188, &v198);
  sub_1003D8E14(&v181, &v198, &v208);
  sub_1003D8E14(v184, &v185, &v188);
  v254 = v182;
  v252[10] = v175.i32[0] + v178;
  v252[11] = v175.i32[1] + DWORD1(v178);
  v252[12] = v175.i32[2] + DWORD2(v178);
  v252[13] = v175.i32[3] + HIDWORD(v178);
  v252[14] = v176.i32[0] + v179;
  v252[15] = v176.i32[1] + DWORD1(v179);
  v252[16] = v176.i32[2] + DWORD2(v179);
  v252[17] = v176.i32[3] + HIDWORD(v179);
  v252[18] = v177.i32[0] + v180;
  v252[19] = v177.i32[1] + HIDWORD(v180);
  v252[20] = v178 - v175.i32[0];
  v252[21] = DWORD1(v178) - v175.i32[1];
  v252[22] = DWORD2(v178) - v175.i32[2];
  v252[23] = HIDWORD(v178) - v175.i32[3];
  v252[24] = v179 - v176.i32[0];
  v252[25] = DWORD1(v179) - v176.i32[1];
  v252[26] = DWORD2(v179) - v176.i32[2];
  v252[27] = HIDWORD(v179) - v176.i32[3];
  v252[28] = v180 - v177.i32[0];
  v252[29] = HIDWORD(v180) - v177.i32[1];
  v255 = v183;
  v253 = v181;
  sub_1003D8E14(&v256, v184, &unk_100561EC4);
  v110 = 0;
  v152 = 0u;
  memset(v153, 0, sizeof(v153));
  DWORD2(v153[0]) = 1;
  v155 = 0u;
  v156 = 0u;
  v157 = 0;
  v154 = 1;
  v151 = 0u;
  while ( 1 )
  {
    v111 = (unsigned int)(v110 + 255);
    v112 = v259[v111];
    if ( v112 || v257[v111] )
      break;
    if ( (_DWORD)--v110 == -256 )
      goto LABEL_206;
  }
  if ( (int)v110 + 255 >= 0 )
  {
    sub_1003D9B58(&v185, &v151);
    if ( (char)v112 < 1 )
      goto LABEL_197;
LABEL_195:
    sub_1003D8E14(&v175, &v185, &v208);
    sub_1003D8E14(&v178, &v188, &v198);
    sub_1003D8E14(&v181, &v198, &v208);
    sub_1003D8E14(v184, &v185, &v188);
    sub_1003DA258(&v185, &v175, &v218[40 * (v112 >> 1)]);
    v113 = (char)v258[v110 + 253];
    if ( v113 < 1 )
    {
LABEL_201:
      if ( v113 < 0 )
      {
        sub_1003D8E14(&v175, &v185, &v208);
        sub_1003D8E14(&v178, &v188, &v198);
        sub_1003D8E14(&v181, &v198, &v208);
        sub_1003D8E14(v184, &v185, &v188);
        v185.i32[0] = v175.i32[0] + v178;
        v185.i32[1] = v175.i32[1] + DWORD1(v178);
        v185.i32[2] = v175.i32[2] + DWORD2(v178);
        v185.i32[3] = v175.i32[3] + HIDWORD(v178);
        v186.i32[0] = v176.i32[0] + v179;
        v186.i32[1] = v176.i32[1] + DWORD1(v179);
        v186.i32[2] = v176.i32[2] + DWORD2(v179);
        v186.i32[3] = v176.i32[3] + HIDWORD(v179);
        v187.i32[0] = v177.i32[0] + v180;
        v187.i32[1] = v177.i32[1] + HIDWORD(v180);
        v188 = v178 - v175.i32[0];
        v189 = DWORD1(v178) - v175.i32[1];
        v190 = DWORD2(v178) - v175.i32[2];
        v191 = HIDWORD(v178) - v175.i32[3];
        v192 = v179 - v176.i32[0];
        v193 = DWORD1(v179) - v176.i32[1];
        v194 = DWORD2(v179) - v176.i32[2];
        v195 = HIDWORD(v179) - v176.i32[3];
        v121 = (char *)&unk_100561B04 + 120 * ((unsigned __int8)-(char)v113 >> 1);
        v196 = v180 - v177.i32[0];
        v197 = HIDWORD(v180) - v177.i32[1];
        sub_1003D8E14(&v198, &v185, v121 + 40);
        sub_1003D8E14(&v188, &v188, v121);
        sub_1003D8E14(&v208, v121 + 80, v184);
        v185.i32[0] = v198 - v188;
        v185.i32[1] = v199 - v189;
        v185.i32[2] = v200 - v190;
        v185.i32[3] = v201 - v191;
        v186.i32[0] = v202 - v192;
        v186.i32[1] = v203 - v193;
        v186.i32[2] = v204 - v194;
        v186.i32[3] = v205 - v195;
        v187.i32[0] = v206 - v196;
        v187.i32[1] = v207 - v197;
        v188 += v198;
        v189 += v199;
        v190 += v200;
        v191 += v201;
        v192 += v202;
        v193 += v203;
        v194 += v204;
        v195 += v205;
        v196 += v206;
        v197 += v207;
        v198 = 2 * v181 - v208;
        v199 = 2 * DWORD1(v181) - v209;
        v200 = 2 * DWORD2(v181) - v210;
        v201 = 2 * HIDWORD(v181) - v211;
        v202 = 2 * v182 - v212;
        v203 = 2 * DWORD1(v182) - v213;
        v204 = 2 * DWORD2(v182) - v214;
        v205 = 2 * HIDWORD(v182) - v215;
        v206 = 2 * v183 - v216;
        v207 = 2 * HIDWORD(v183) - v217;
        v208 += 2 * v181;
        v209 += 2 * DWORD1(v181);
        v210 += 2 * DWORD2(v181);
        v211 += 2 * HIDWORD(v181);
        v212 += 2 * v182;
        v213 += 2 * DWORD1(v182);
        v214 += 2 * DWORD2(v182);
        v215 += 2 * HIDWORD(v182);
        v216 += 2 * v183;
        v217 += 2 * HIDWORD(v183);
      }
      goto LABEL_203;
    }
    while ( 1 )
    {
      sub_1003D8E14(&v175, &v185, &v208);
      sub_1003D8E14(&v178, &v188, &v198);
      sub_1003D8E14(&v181, &v198, &v208);
      sub_1003D8E14(v184, &v185, &v188);
      sub_1003D9818(&v185, &v175, (char *)&unk_100561B04 + 120 * ((unsigned __int8)v113 >> 1));
LABEL_203:
      sub_1003D8E14(&v151, &v185, &v208);
      sub_1003D8E14((char *)v153 + 8, &v188, &v198);
      sub_1003D8E14(&v154, &v198, &v208);
      if ( v110 + 255 < 1 )
        break;
      v112 = v260[v110-- + 252];
      sub_1003D9B58(&v185, &v151);
      if ( (char)v112 >= 1 )
        goto LABEL_195;
LABEL_197:
      if ( (v112 & 0x80) != 0 )
      {
        sub_1003D8E14(&v175, &v185, &v208);
        sub_1003D8E14(&v178, &v188, &v198);
        sub_1003D8E14(&v181, &v198, &v208);
        sub_1003D8E14(v184, &v185, &v188);
        v185.i32[0] = v175.i32[0] + v178;
        v185.i32[1] = v175.i32[1] + DWORD1(v178);
        v185.i32[2] = v175.i32[2] + DWORD2(v178);
        v185.i32[3] = v175.i32[3] + HIDWORD(v178);
        v186.i32[0] = v176.i32[0] + v179;
        v186.i32[1] = v176.i32[1] + DWORD1(v179);
        v186.i32[2] = v176.i32[2] + DWORD2(v179);
        v186.i32[3] = v176.i32[3] + HIDWORD(v179);
        v187.i32[0] = v177.i32[0] + v180;
        v187.i32[1] = v177.i32[1] + HIDWORD(v180);
        v188 = v178 - v175.i32[0];
        v189 = DWORD1(v178) - v175.i32[1];
        v190 = DWORD2(v178) - v175.i32[2];
        v191 = HIDWORD(v178) - v175.i32[3];
        v192 = v179 - v176.i32[0];
        v193 = DWORD1(v179) - v176.i32[1];
        v194 = DWORD2(v179) - v176.i32[2];
        v195 = HIDWORD(v179) - v176.i32[3];
        v114 = &v218[40 * ((unsigned __int8)-v112 >> 1)];
        v196 = v180 - v177.i32[0];
        v197 = HIDWORD(v180) - v177.i32[1];
        sub_1003D8E14(&v198, &v185, v114 + 10);
        sub_1003D8E14(&v188, &v188, v114);
        sub_1003D8E14(&v208, v114 + 30, v184);
        sub_1003D8E14(&v185, &v181, v114 + 20);
        v115 = v185.i32[0];
        v116 = v185.i32[1];
        v129 = v185.i32[3];
        v131 = v185.i32[2];
        v117 = v186.i32[0];
        v118 = v186.i32[1];
        v119 = v186.i32[2];
        v127 = v186.i32[3];
        v120 = v187;
        v185.i32[0] = v198 - v188;
        v185.i32[1] = v199 - v189;
        v185.i32[2] = v200 - v190;
        v185.i32[3] = v201 - v191;
        v186.i32[0] = v202 - v192;
        v186.i32[1] = v203 - v193;
        v186.i32[2] = v204 - v194;
        v186.i32[3] = v205 - v195;
        v187.i32[0] = v206 - v196;
        v187.i32[1] = v207 - v197;
        v188 += v198;
        v189 += v199;
        v190 += v200;
        v191 += v201;
        v117 *= 2;
        v118 *= 2;
        v119 *= 2;
        v192 += v202;
        v193 += v203;
        v194 += v204;
        v195 += v205;
        v196 += v206;
        v197 += v207;
        v198 = 2 * v115 - v208;
        v199 = 2 * v116 - v209;
        v200 = 2 * v131 - v210;
        v201 = 2 * v129 - v211;
        v202 = v117 - v212;
        v203 = v118 - v213;
        v204 = v119 - v214;
        v205 = 2 * v127 - v215;
        v206 = 2 * v120.i32[0] - v216;
        v207 = 2 * v120.i32[1] - v217;
        v208 += 2 * v115;
        v209 += 2 * v116;
        v210 += 2 * v131;
        v211 += 2 * v129;
        v212 += v117;
        v213 += v118;
        v214 += v119;
        v215 += 2 * v127;
        v216 += 2 * v120.i32[0];
        v217 += 2 * v120.i32[1];
        v113 = (char)v258[v110 + 253];
        if ( v113 < 1 )
          goto LABEL_201;
      }
      else
      {
        v113 = (char)v258[v110 + 253];
        if ( v113 < 1 )
          goto LABEL_201;
      }
    }
  }
LABEL_206:
  sub_1003D85B8(v218, &v154);
  sub_1003D8E14(v259, &v151, v218);
  sub_1003D8E14(v257, (char *)v153 + 8, v218);
  sub_1003D91D8(&v185, v257);
  sub_1003D91D8(v161, v259);
  v186.i8[15] ^= v161[0] << 7;
  return (unsigned int)sub_100391BA0(&v185, a3, 32) == 0;
}

/* ========================================================================
 * fallback sub_1003d5d1c
 * EA: 0x1003d5d1c
 ======================================================================== */

bool __fastcall sub_1003D5D1C(__int64 a1, __int128 *a2, unsigned int *a3)
{
  __int64 v3; // x17
  __int64 v4; // x0
  __int64 v5; // x15
  __int64 v6; // x3
  __int64 v7; // x9
  __int64 v8; // x4
  __int64 v9; // x13
  __int64 v10; // x12
  unsigned __int64 v11; // x30
  unsigned __int64 v12; // x16
  unsigned __int64 v13; // x14
  int v14; // w5
  uint8x8_t v15; // d0
  __int128 v16; // q1
  unsigned __int64 v17; // d0
  int8x16_t v18; // off
  int8x16_t v19; // q2
  uint64x2_t v20; // q1
  unsigned __int64 v21; // x19
  unsigned __int64 v22; // x7
  unsigned __int64 v23; // x20
  int8x16_t v24; // q0
  unsigned __int64 v25; // x1
  unsigned __int64 v26; // x6
  unsigned int v27; // w26
  __int64 v28; // x21
  __int64 v29; // x10
  __int64 v30; // x26
  unsigned __int64 v31; // x2
  unsigned __int64 v32; // x10
  unsigned __int64 v33; // x23
  unsigned __int64 v34; // x20
  __int64 v35; // x24
  __int64 v36; // x25
  __int64 v37; // x21
  __int64 v38; // x4
  unsigned __int64 v39; // x27
  unsigned __int64 v40; // x6
  __int64 v41; // x28
  __int64 v42; // x3
  unsigned __int64 v43; // x11
  __int64 v44; // x30
  __int64 v45; // x0
  __int64 v46; // x7
  unsigned __int64 v47; // x8
  __int64 v48; // x9
  unsigned __int64 v49; // x1
  __int64 v50; // x17
  unsigned __int64 v51; // x2
  unsigned __int64 v52; // x10
  unsigned __int64 v53; // x3
  unsigned __int64 v54; // x8
  __int64 v55; // x11
  __int64 v56; // x9
  __int64 v57; // x24
  __int64 v58; // x22
  unsigned __int64 v59; // x11
  unsigned __int64 v60; // x23
  unsigned __int64 v61; // x20
  unsigned __int64 v62; // x21
  unsigned __int64 v63; // x19
  unsigned __int64 v64; // x25
  __int64 v65; // x26
  __int64 v66; // x27
  __int64 v67; // x28
  signed __int128 v68; // kr180_16
  signed __int128 v69; // kr190_16
  signed __int128 v70; // kr1D0_16
  signed __int128 v71; // kr1E0_16
  signed __int128 v72; // kr210_16
  unsigned __int64 v73; // x12
  signed __int128 v74; // kr230_16
  signed __int128 v75; // kr250_16
  signed __int128 v76; // kr290_16
  signed __int128 v77; // kr2A0_16
  unsigned __int64 v78; // x3
  unsigned __int128 v79; // kr2C0_16
  unsigned __int64 v80; // x0
  __int64 v81; // x27
  __int64 v82; // x21
  unsigned __int64 v83; // x22
  unsigned __int64 v84; // x8
  unsigned __int64 v85; // x13
  __int64 v86; // x26
  __int64 v87; // x24
  __int64 v88; // x25
  __int64 v89; // x23
  __int64 v90; // x19
  __int64 v91; // x28
  __int64 v92; // x20
  signed __int128 v93; // kr300_16
  signed __int128 v94; // kr320_16
  signed __int128 v95; // kr360_16
  signed __int128 v96; // kr370_16
  signed __int128 v97; // kr390_16
  unsigned __int64 v98; // x9
  unsigned __int64 v99; // x8
  unsigned __int64 v100; // kr70_8
  unsigned __int64 v101; // x13
  signed __int128 v102; // kr3F0_16
  signed __int128 v103; // kr400_16
  signed __int128 v104; // kr440_16
  signed __int128 v105; // kr450_16
  signed __int128 v106; // kr480_16
  unsigned __int64 v107; // x8
  unsigned __int64 v108; // x8
  unsigned __int64 v109; // x9
  __int64 v110; // x11
  unsigned __int64 v111; // x0
  __int64 v112; // x24
  signed __int128 v113; // kr500_16
  unsigned __int64 v114; // x11
  unsigned __int64 v115; // x9
  __int64 v116; // x9
  unsigned __int64 v117; // x17
  __int64 v118; // x0
  __int64 v119; // x2
  unsigned __int64 v120; // x19
  __int64 v121; // x21
  __int64 v122; // x11
  __int64 v123; // x10
  unsigned __int64 v124; // x11
  unsigned __int64 v125; // x13
  unsigned __int64 v126; // x9
  unsigned __int64 v127; // x14
  unsigned __int64 v128; // x12
  unsigned __int64 v129; // x13
  unsigned __int64 v130; // x8
  __int64 v131; // x10
  __int64 v132; // x12
  __int64 v133; // x11
  unsigned __int64 v134; // x13
  __int64 v135; // x14
  unsigned __int128 v136; // kr5B0_16
  unsigned __int128 v137; // kr5C0_16
  unsigned __int128 v138; // kr5E0_16
  unsigned __int128 v139; // t2
  unsigned __int128 v140; // kr610_16
  signed __int128 v141; // kr630_16
  signed __int128 v142; // kr650_16
  unsigned __int64 v143; // x11
  unsigned __int64 v144; // x8
  signed __int128 v145; // kr680_16
  unsigned __int64 v146; // x11
  unsigned __int64 v147; // x9
  __int64 v148; // x11
  __int64 v149; // x0
  __int64 v150; // x24
  signed __int128 v151; // kr760_16
  unsigned __int64 v152; // x11
  signed __int128 v153; // t2
  unsigned __int64 v154; // x9
  __int64 v155; // x12
  unsigned __int64 v156; // x10
  unsigned __int64 v157; // x13
  unsigned __int64 v158; // x14
  unsigned __int64 v159; // x15
  unsigned __int64 v160; // x16
  unsigned __int64 v161; // x6
  unsigned __int64 v162; // x7
  unsigned __int64 v163; // x19
  unsigned __int64 v164; // x21
  unsigned __int64 v165; // x17
  __int64 v166; // kr7A0_8
  unsigned __int64 v167; // x0
  unsigned __int64 v168; // x6
  __int64 v169; // kr7B0_8
  unsigned __int128 v170; // kr7C0_16
  signed __int128 v171; // kr7D0_16
  __int64 v172; // x9
  unsigned __int64 v173; // x14
  unsigned __int64 v174; // x12
  unsigned __int64 v175; // x10
  __int64 v176; // x8
  __int64 v177; // x11
  unsigned __int64 v178; // x13
  unsigned __int64 v179; // x14
  unsigned __int64 v180; // x15
  __int64 v181; // x16
  unsigned __int64 v182; // x17
  unsigned __int64 v183; // x0
  unsigned __int64 v184; // x1
  __int64 v185; // x2
  unsigned __int64 v186; // x3
  __int64 v187; // x10
  unsigned __int64 v188; // x6
  __int64 v189; // x7
  unsigned __int64 v190; // x19
  __int64 v191; // x21
  unsigned __int64 v192; // x17
  unsigned __int64 v193; // kr7F0_8
  unsigned __int64 v194; // x0
  unsigned __int64 v195; // x6
  __int64 v196; // kr800_8
  unsigned __int128 v197; // kr820_16
  signed __int128 v198; // kr830_16
  unsigned __int64 v199; // x10
  unsigned __int64 v200; // x14
  unsigned __int64 v201; // x8
  unsigned __int64 v202; // x11
  __int64 v203; // x9
  __int64 v204; // x12
  unsigned __int64 v205; // x13
  unsigned __int64 v206; // x14
  unsigned __int64 v207; // x15
  __int64 v208; // x16
  unsigned __int64 v209; // x17
  unsigned __int64 v210; // x0
  unsigned __int64 v211; // x1
  __int64 v212; // x2
  unsigned __int64 v213; // x3
  unsigned __int64 v214; // x11
  unsigned __int64 v215; // x6
  __int64 v216; // x7
  unsigned __int64 v217; // x19
  __int64 v218; // x21
  unsigned __int64 v219; // x17
  unsigned __int64 v220; // kr850_8
  unsigned __int64 v221; // x0
  unsigned __int64 v222; // x6
  __int64 v223; // kr860_8
  unsigned __int64 v224; // x4
  unsigned __int64 v225; // x17
  unsigned __int128 v226; // kr880_16
  signed __int128 v227; // kr890_16
  unsigned __int64 v228; // x11
  unsigned __int64 v229; // x14
  unsigned __int64 v230; // x13
  unsigned __int64 v231; // x14
  __int64 v232; // x9
  __int64 v233; // x8
  __int64 v234; // x10
  __int64 v235; // x12
  __int64 v236; // x11
  unsigned __int64 v237; // x13
  __int64 v238; // x14
  unsigned __int128 v239; // kr8B0_16
  unsigned __int128 v240; // kr8C0_16
  unsigned __int128 v241; // kr8E0_16
  unsigned __int128 v242; // kr910_16
  signed __int128 v243; // kr930_16
  signed __int128 v244; // kr950_16
  unsigned __int64 v245; // x11
  unsigned __int64 v246; // x8
  signed __int128 v247; // kr980_16
  unsigned __int64 v248; // x12
  unsigned __int64 v249; // x9
  unsigned __int64 v250; // x8
  __int64 v251; // x10
  __int64 v252; // x11
  __int64 v253; // x13
  __int64 v254; // x12
  int v255; // w9
  __int64 v256; // x14
  __int64 v257; // x15
  __int64 v258; // x12
  __int64 v259; // x16
  __int64 v260; // x17
  unsigned __int128 v261; // krA20_16
  __int64 v262; // x13
  __int128 v263; // krA40_16
  signed __int128 v264; // krA60_16
  signed __int128 v265; // krAA0_16
  signed __int128 v266; // krAB0_16
  signed __int128 v267; // krAD0_16
  unsigned __int64 v268; // x11
  unsigned __int64 v269; // x12
  signed __int128 v270; // krB00_16
  unsigned __int64 v271; // x13
  unsigned __int64 v272; // x10
  unsigned __int64 v273; // x9
  __int64 v274; // x11
  __int64 v275; // x12
  __int64 v276; // x14
  __int64 v277; // x13
  int v278; // w10
  __int64 v279; // x15
  __int64 v280; // x16
  __int64 v281; // x13
  __int64 v282; // x17
  __int64 v283; // x0
  unsigned __int128 v284; // krBA0_16
  __int64 v285; // x14
  __int128 v286; // krBC0_16
  signed __int128 v287; // krBE0_16
  signed __int128 v288; // krC10_16
  signed __int128 v289; // krC20_16
  signed __int128 v290; // krC40_16
  unsigned __int64 v291; // x12
  unsigned __int64 v292; // x13
  __int64 v293; // x13
  __int64 v294; // x14
  __int64 v295; // x10
  __int64 v296; // x11
  int v297; // w8
  __int64 v298; // x12
  __int64 v299; // x15
  __int64 v300; // x16
  __int64 v301; // x13
  __int64 v302; // x17
  __int64 v303; // x0
  unsigned __int128 v304; // krC50_16
  __int64 v305; // x14
  __int128 v306; // krC70_16
  signed __int128 v307; // krC90_16
  signed __int128 v308; // krCD0_16
  signed __int128 v309; // krCE0_16
  signed __int128 v310; // krD00_16
  unsigned __int64 v311; // x14
  unsigned __int64 v312; // x13
  signed __int128 v313; // krD30_16
  unsigned __int64 v314; // x13
  unsigned __int64 v315; // x10
  unsigned __int64 v316; // x9
  __int64 v317; // x11
  __int64 v318; // x12
  __int64 v319; // x14
  __int64 v320; // x13
  int v321; // w10
  __int64 v322; // x15
  __int64 v323; // x16
  __int64 v324; // x13
  __int64 v325; // x17
  __int64 v326; // x0
  unsigned __int128 v327; // krDD0_16
  __int64 v328; // x14
  __int128 v329; // krDF0_16
  signed __int128 v330; // krE10_16
  signed __int128 v331; // krE50_16
  signed __int128 v332; // krE60_16
  signed __int128 v333; // krE80_16
  unsigned __int64 v334; // x12
  unsigned __int64 v335; // x13
  signed __int128 v336; // krEB0_16
  unsigned __int64 v337; // x13
  unsigned __int64 v338; // x10
  unsigned __int64 v339; // x9
  __int64 v340; // x11
  __int64 v341; // x12
  __int64 v342; // x14
  __int64 v343; // x13
  int v344; // w10
  __int64 v345; // x15
  __int64 v346; // x16
  __int64 v347; // x13
  __int64 v348; // x17
  __int64 v349; // x0
  unsigned __int128 v350; // krF50_16
  __int64 v351; // x14
  __int128 v352; // krF70_16
  signed __int128 v353; // krF90_16
  signed __int128 v354; // krFD0_16
  signed __int128 v355; // krFE0_16
  signed __int128 v356; // kr1000_16
  unsigned __int64 v357; // x12
  unsigned __int64 v358; // x13
  __int64 v359; // x13
  __int64 v360; // x14
  __int64 v361; // x10
  __int64 v362; // x11
  int v363; // w8
  __int64 v364; // x12
  __int64 v365; // x15
  __int64 v366; // x16
  __int64 v367; // x13
  __int64 v368; // x17
  __int64 v369; // x0
  unsigned __int128 v370; // kr1010_16
  __int64 v371; // x14
  __int128 v372; // kr1030_16
  signed __int128 v373; // kr1050_16
  signed __int128 v374; // kr1090_16
  signed __int128 v375; // kr10A0_16
  signed __int128 v376; // kr10C0_16
  unsigned __int64 v377; // x14
  unsigned __int64 v378; // x13
  __int64 v379; // x13
  __int64 v380; // x14
  __int64 v381; // x10
  __int64 v382; // x11
  int v383; // w8
  __int64 v384; // x12
  __int64 v385; // x15
  __int64 v386; // x16
  __int64 v387; // x13
  __int64 v388; // x17
  __int64 v389; // x0
  unsigned __int128 v390; // kr10D0_16
  __int64 v391; // x14
  __int128 v392; // kr10F0_16
  signed __int128 v393; // kr1110_16
  signed __int128 v394; // kr1150_16
  signed __int128 v395; // kr1160_16
  signed __int128 v396; // kr1180_16
  unsigned __int64 v397; // x14
  unsigned __int64 v398; // x13
  unsigned __int64 v399; // x14
  unsigned __int64 v400; // x15
  unsigned __int64 v401; // x10
  unsigned __int64 v402; // x9
  unsigned __int64 v403; // x8
  int v406; // [xsp+14h] [xbp-26Ch]
  __int64 v407; // [xsp+18h] [xbp-268h]
  __int64 v408; // [xsp+20h] [xbp-260h]
  __int64 v409; // [xsp+28h] [xbp-258h]
  __int64 v410; // [xsp+30h] [xbp-250h]
  unsigned __int64 v411; // [xsp+40h] [xbp-240h]
  unsigned __int64 v412; // [xsp+48h] [xbp-238h]
  __int64 v413; // [xsp+48h] [xbp-238h]
  __int64 v414; // [xsp+50h] [xbp-230h]
  __int64 v415; // [xsp+50h] [xbp-230h]
  unsigned int v416; // [xsp+5Ch] [xbp-224h]
  __int64 v417; // [xsp+60h] [xbp-220h]
  __int64 v418; // [xsp+60h] [xbp-220h]
  unsigned __int64 v419; // [xsp+68h] [xbp-218h]
  __int64 v420; // [xsp+68h] [xbp-218h]
  __int128 v421; // [xsp+70h] [xbp-210h] BYREF
  __int128 v422; // [xsp+80h] [xbp-200h]
  __int64 v423; // [xsp+90h] [xbp-1F0h] BYREF
  __int64 v424; // [xsp+98h] [xbp-1E8h]
  __int64 v425; // [xsp+A0h] [xbp-1E0h]
  __int64 v426; // [xsp+A8h] [xbp-1D8h]
  __int64 v427; // [xsp+B0h] [xbp-1D0h]
  __int64 v428; // [xsp+B8h] [xbp-1C8h] BYREF
  __int64 v429; // [xsp+C0h] [xbp-1C0h]
  __int64 v430; // [xsp+C8h] [xbp-1B8h]
  __int64 v431; // [xsp+D0h] [xbp-1B0h]
  __int64 v432; // [xsp+D8h] [xbp-1A8h]
  __int64 v433; // [xsp+E0h] [xbp-1A0h] BYREF
  __int64 v434; // [xsp+E8h] [xbp-198h]
  __int64 v435; // [xsp+F0h] [xbp-190h]
  __int64 v436; // [xsp+F8h] [xbp-188h]
  __int64 v437; // [xsp+100h] [xbp-180h]
  __int64 v438; // [xsp+108h] [xbp-178h] BYREF
  __int64 v439; // [xsp+110h] [xbp-170h]
  __int64 v440; // [xsp+118h] [xbp-168h]
  __int64 v441; // [xsp+120h] [xbp-160h]
  __int64 v442; // [xsp+128h] [xbp-158h]
  __int64 v443; // [xsp+130h] [xbp-150h] BYREF
  unsigned __int64 v444; // [xsp+138h] [xbp-148h]
  unsigned __int64 v445; // [xsp+140h] [xbp-140h]
  unsigned __int64 v446; // [xsp+148h] [xbp-138h]
  unsigned __int64 v447; // [xsp+150h] [xbp-130h]
  _QWORD v448[5]; // [xsp+158h] [xbp-128h] BYREF
  __int64 v449; // [xsp+180h] [xbp-100h] BYREF
  __int64 v450; // [xsp+188h] [xbp-F8h]
  __int64 v451; // [xsp+190h] [xbp-F0h]
  __int64 v452; // [xsp+198h] [xbp-E8h]
  __int64 v453; // [xsp+1A0h] [xbp-E0h]
  __int64 v454; // [xsp+1A8h] [xbp-D8h] BYREF
  __int64 v455; // [xsp+1B0h] [xbp-D0h]
  __int64 v456; // [xsp+1B8h] [xbp-C8h]
  __int64 v457; // [xsp+1C0h] [xbp-C0h]
  __int64 v458; // [xsp+1C8h] [xbp-B8h]
  __int64 v459; // [xsp+1D0h] [xbp-B0h] BYREF
  __int64 v460; // [xsp+1D8h] [xbp-A8h]
  __int64 v461; // [xsp+1E0h] [xbp-A0h]
  __int64 v462; // [xsp+1E8h] [xbp-98h]
  __int64 v463; // [xsp+1F0h] [xbp-90h]
  __int64 v464; // [xsp+1F8h] [xbp-88h] BYREF
  unsigned __int64 v465; // [xsp+200h] [xbp-80h]
  __int64 v466; // [xsp+208h] [xbp-78h]
  __int64 v467; // [xsp+210h] [xbp-70h]
  unsigned __int64 v468; // [xsp+218h] [xbp-68h]

  v3 = 0;
  v417 = 0;
  v419 = 0;
  v4 = 0;
  v5 = 0;
  v6 = 0;
  v7 = 0;
  v8 = 0;
  v9 = 0;
  v10 = 0;
  v11 = 0;
  v12 = 0;
  v13 = 0;
  v14 = 0;
  v16 = a2[1];
  v421 = *a2;
  v15.i32[1] = DWORD1(v421);
  v422 = v16;
  LOBYTE(v421) = v421 & 0xF8;
  HIBYTE(v422) = HIBYTE(v16) & 0x3F | 0x40;
  v15.i32[0] = *(unsigned int *)((char *)a3 + 26);
  v17 = vmovl_u8(v15).u64[0];
  v18.i64[0] = (unsigned __int16)v17;
  v18.i64[1] = WORD1(v17);
  v19.i64[0] = 255;
  v19.i64[1] = 255;
  v20 = (uint64x2_t)vandq_s8(v18, v19);
  v18.i64[0] = WORD2(v17);
  v18.i64[1] = HIWORD(v17);
  v21 = *a3
      | ((unsigned __int64)*((unsigned __int8 *)a3 + 4) << 32) & 0xFFFFFFFFFFLL
      | ((unsigned __int64)*((unsigned __int8 *)a3 + 5) << 40) & 0xFFFFFFFFFFFFLL
      | ((unsigned __int64)*((unsigned __int8 *)a3 + 6) << 48) & 0x7FFFFFFFFFFFFLL;
  v22 = ((*a3
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 4) << 32) & 0xFF0000FFFFFFFFFFLL
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 5) << 40) & 0xFF00FFFFFFFFFFFFLL
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 6) << 48)) >> 51)
      & 0xFFF800000000001FLL
      | (32
       * (*(unsigned int *)((char *)a3 + 7)
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 11) << 32) & 0xFFFFFFFFFFLL
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 12) << 40) & 0x3FFFFFFFFFFFLL));
  v23 = ((*(unsigned int *)((char *)a3 + 7)
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 11) << 32) & 0xFFFF00FFFFFFFFFFLL
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 12) << 40)) >> 46)
      & 3
      | (4
       * (*(unsigned int *)((char *)a3 + 13)
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 17) << 32) & 0xFFFFFFFFFFLL
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 18) << 40) & 0xFFFFFFFFFFFFLL
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 19) << 48) & 0xFFFFFFFFFFFFFFLL))
      & 0x7FFFFFFFFFFFFLL;
  v24 = vorrq_s8(
          (int8x16_t)vshlq_u64(v20, (uint64x2_t)xmmword_10055A260),
          (int8x16_t)vshlq_u64((uint64x2_t)vandq_s8(v18, v19), (uint64x2_t)xmmword_10055A250));
  v25 = *(_QWORD *)&vorr_s8(*(int8x8_t *)v24.i8, (int8x8_t)vextq_s8(v24, v24, 8u))
      | ((unsigned __int64)*((unsigned __int8 *)a3 + 30) << 36)
      | ((a3[5]
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 24) << 32) & 0xFFFF00FFFFFFFFFFLL
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 25) << 40)) >> 44)
      | ((unsigned __int64)(*((_BYTE *)a3 + 31) & 0x7F) << 44);
  v26 = ((*(unsigned int *)((char *)a3 + 13)
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 17) << 32) & 0xFF0000FFFFFFFFFFLL
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 18) << 40) & 0xFF00FFFFFFFFFFFFLL
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 19) << 48)) >> 49)
      & 0x7F
      | ((a3[5]
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 24) << 32) & 0xFFFFFFFFFFLL
        | ((unsigned __int64)*((unsigned __int8 *)a3 + 25) << 40) & 0xFFFFFFFFFFFFLL) << 7)
      & 0x7FFFFFFFFFFFFLL;
  v448[0] = v21;
  v448[1] = v22;
  v27 = 254;
  v28 = 1;
  v448[2] = v23;
  v448[3] = v26;
  v448[4] = v25;
  v29 = 1;
  while ( 1 )
  {
    v414 = v29;
    v416 = v27;
    v406 = (*((unsigned __int8 *)&v421 + ((unsigned __int64)v27 >> 3)) >> (v27 & 7)) & 1;
    v30 = -(__int64)(v406 ^ (unsigned int)v14);
    v31 = (v21 ^ v29) & v30;
    v32 = (v22 ^ v13) & v30;
    v33 = v32 ^ v22;
    v412 = (v23 ^ v12) & v30;
    v34 = v412 ^ v23;
    v35 = (v10 ^ v28) & v30;
    v36 = v35 ^ v28;
    v37 = (v9 ^ v8) & v30;
    v38 = v37 ^ v8;
    v428 = (v31 ^ v21) + 0xFFFFFFFFFFFDALL - v36;
    v429 = (v32 ^ v22) - v38 + 0xFFFFFFFFFFFFELL;
    v39 = (v26 ^ v11) & v30;
    v40 = v39 ^ v26;
    v41 = (v7 ^ v6) & v30;
    v42 = v41 ^ v6;
    v43 = v11;
    v44 = (v5 ^ v4) & v30;
    v45 = v44 ^ v4;
    v46 = v7;
    v430 = v34 - v42 + 0xFFFFFFFFFFFFELL;
    v431 = v40 - v45 + 0xFFFFFFFFFFFFELL;
    v47 = (v25 ^ v419) & v30;
    v48 = (v417 ^ v3) & v30;
    v49 = v47 ^ v25;
    v50 = v48 ^ v3;
    v432 = v49 - v50 + 0xFFFFFFFFFFFFELL;
    v433 = v36;
    v434 = v38;
    v435 = v42;
    v438 = v36 + (v31 ^ v21);
    v439 = v38 + v33;
    v51 = v31 ^ v414;
    v52 = v32 ^ v13;
    v436 = v45;
    v437 = v50;
    v440 = v42 + v34;
    v441 = v45 + v40;
    v53 = v39 ^ v43;
    v54 = v47 ^ v419;
    v442 = v50 + v49;
    v55 = v35 ^ v10;
    v56 = v48 ^ v417;
    v57 = v51 + 0xFFFFFFFFFFFDALL - (v35 ^ v10);
    v58 = v55 + v51;
    v59 = v52 - (v37 ^ v9);
    v60 = (v37 ^ v9) + v52;
    v61 = (v41 ^ v46) + (v412 ^ v12);
    v62 = (v44 ^ v5) + v53;
    v63 = v56 + v54;
    v64 = v59 + 0xFFFFFFFFFFFFELL;
    v423 = v57;
    v424 = v59 + 0xFFFFFFFFFFFFELL;
    v65 = (v412 ^ v12) - (v41 ^ v46) + 0xFFFFFFFFFFFFELL;
    v66 = v53 - (v44 ^ v5) + 0xFFFFFFFFFFFFELL;
    v425 = v65;
    v426 = v66;
    v67 = v54 - v56 + 0xFFFFFFFFFFFFELL;
    v427 = v67;
    v443 = v58;
    v444 = v60;
    v445 = v61;
    v446 = v62;
    v447 = v56 + v54;
    sub_1003DA5A4(&v433, &v428, &v443);
    sub_1003DA5A4(&v438, &v438, &v423);
    v68 = (unsigned __int64)(2 * v57) * (unsigned __int128)(unsigned __int64)v65
        + v64 * (unsigned __int128)v64
        + (unsigned __int64)(19 * v67) * (unsigned __int128)(unsigned __int64)(2 * v66);
    v69 = (unsigned __int64)(19 * v66) * (unsigned __int128)(unsigned __int64)(2 * v65)
        + (unsigned __int64)v57 * (unsigned __int128)(unsigned __int64)v57
        + (unsigned __int64)(19 * v67) * (unsigned __int128)(2 * v64);
    v70 = (unsigned __int64)(2 * v57) * (unsigned __int128)(unsigned __int64)v66
        + 2 * v64 * (unsigned __int128)(unsigned __int64)v65
        + (unsigned __int64)(19 * v67) * (unsigned __int128)(unsigned __int64)v67
        + (unsigned __int64)(v68 >> 51);
    v71 = __PAIR128__(
            (((unsigned __int64)(2 * v57) * (unsigned __int128)v64) >> 64)
          + (((unsigned __int64)(19 * v66) * (unsigned __int128)(unsigned __int64)v66 + 2 * v57 * v64) >> 64),
            19 * v66 * v66 + 2 * v57 * v64)
        + (unsigned __int64)(19 * v67) * (unsigned __int128)(unsigned __int64)(2 * v65)
        + (unsigned __int64)(v69 >> 51);
    v72 = 2 * v64 * (unsigned __int128)(unsigned __int64)v66
        + (unsigned __int64)v65 * (unsigned __int128)(unsigned __int64)v65
        + (unsigned __int64)(2 * v57) * (unsigned __int128)(unsigned __int64)v67
        + (unsigned __int64)(v70 >> 51);
    v73 = (v68 & 0x7FFFFFFFFFFFFLL) + (v71 >> 51);
    v74 = (unsigned __int64)(2 * v58) * (unsigned __int128)v61
        + v60 * (unsigned __int128)v60
        + 19 * v63 * (unsigned __int128)(2 * v62);
    v75 = 19 * v62 * (unsigned __int128)(2 * v61)
        + (unsigned __int64)v58 * (unsigned __int128)(unsigned __int64)v58
        + 19 * v63 * (unsigned __int128)(2 * v60);
    v76 = (unsigned __int64)(2 * v58) * (unsigned __int128)v62
        + 2 * v60 * (unsigned __int128)v61
        + 19 * v63 * (unsigned __int128)v63
        + (unsigned __int64)(v74 >> 51);
    v77 = 19 * v62 * (unsigned __int128)v62
        + (unsigned __int64)(2 * v58) * (unsigned __int128)v60
        + 19 * v63 * (unsigned __int128)(2 * v61)
        + (unsigned __int64)(v75 >> 51);
    v78 = v76 >> 51;
    v79 = 2 * v60 * (unsigned __int128)v62
        + v61 * (unsigned __int128)v61
        + (unsigned __int64)(2 * v58) * (unsigned __int128)v63;
    v80 = (v74 & 0x7FFFFFFFFFFFFLL) + (v77 >> 51);
    v413 = v438 + v433;
    v415 = v439 + v434;
    v81 = v440 + v435;
    v82 = v433 + 0xFFFFFFFFFFFDALL - v438;
    v83 = v441 + v436;
    v84 = (v69 & 0x7FFFFFFFFFFFFLL) + 19 * (v72 >> 51);
    v85 = (v75 & 0x7FFFFFFFFFFFFLL) + 19 * ((__int128)(v79 + v78) >> 51);
    v418 = v84 & 0x7FFFFFFFFFFFFLL;
    v420 = (v71 & 0x7FFFFFFFFFFFFLL) + (v84 >> 51);
    v428 = v84 & 0x7FFFFFFFFFFFFLL;
    v429 = v420;
    v411 = v442 + v437;
    v409 = v73 & 0x7FFFFFFFFFFFFLL;
    v410 = (v70 & 0x7FFFFFFFFFFFFLL) + (v73 >> 51);
    v430 = v73 & 0x7FFFFFFFFFFFFLL;
    v431 = v410;
    v432 = v72 & 0x7FFFFFFFFFFFFLL;
    v407 = (v76 & 0x7FFFFFFFFFFFFLL) + (v80 >> 51);
    v408 = (v79 + v78) & 0x7FFFFFFFFFFFFLL;
    v86 = v80 & 0x7FFFFFFFFFFFFLL;
    v87 = (v77 & 0x7FFFFFFFFFFFFLL) + (v85 >> 51);
    v88 = v85 & 0x7FFFFFFFFFFFFLL;
    v423 = v85 & 0x7FFFFFFFFFFFFLL;
    v424 = v87;
    v425 = v80 & 0x7FFFFFFFFFFFFLL;
    v426 = v407;
    v427 = v408;
    v89 = v434 - v439 + 0xFFFFFFFFFFFFELL;
    v90 = v435 - v440 + 0xFFFFFFFFFFFFELL;
    v91 = v436 - v441 + 0xFFFFFFFFFFFFELL;
    v92 = v437 - v442 + 0xFFFFFFFFFFFFELL;
    sub_1003DA5A4(&v443, &v423, &v428);
    v93 = (unsigned __int64)v90 * (unsigned __int128)(unsigned __int64)(2 * v82)
        + (unsigned __int64)v89 * (unsigned __int128)(unsigned __int64)v89
        + (unsigned __int64)(19 * v92) * (unsigned __int128)(unsigned __int64)(2 * v91);
    v94 = (unsigned __int64)(19 * v91) * (unsigned __int128)(unsigned __int64)(2 * v90)
        + (unsigned __int64)v82 * (unsigned __int128)(unsigned __int64)v82
        + (unsigned __int64)(19 * v92) * (unsigned __int128)(unsigned __int64)(2 * v89);
    v423 = v88 + 0xFFFFFFFFFFFDALL - v418;
    v424 = v87 - v420 + 0xFFFFFFFFFFFFELL;
    v425 = v86 - v409 + 0xFFFFFFFFFFFFELL;
    v426 = v407 - v410 + 0xFFFFFFFFFFFFELL;
    v427 = v408 - (v72 & 0x7FFFFFFFFFFFFLL) + 0xFFFFFFFFFFFFELL;
    v95 = (unsigned __int64)v91 * (unsigned __int128)(unsigned __int64)(2 * v82)
        + (unsigned __int64)v90 * (unsigned __int128)(unsigned __int64)(2 * v89)
        + (unsigned __int64)(19 * v92) * (unsigned __int128)(unsigned __int64)v92
        + (unsigned __int64)(v93 >> 51);
    v96 = __PAIR128__(
            (((unsigned __int64)v89 * (unsigned __int128)(unsigned __int64)(2 * v82)) >> 64)
          + (((unsigned __int64)(19 * v91) * (unsigned __int128)(unsigned __int64)v91 + (unsigned __int64)(v89 * 2 * v82)) >> 64),
            19 * v91 * v91 + v89 * 2 * v82)
        + (unsigned __int64)(19 * v92) * (unsigned __int128)(unsigned __int64)(2 * v90)
        + (unsigned __int64)(v94 >> 51);
    v97 = (unsigned __int64)v91 * (unsigned __int128)(unsigned __int64)(2 * v89)
        + (unsigned __int64)v90 * (unsigned __int128)(unsigned __int64)v90
        + (unsigned __int64)v92 * (unsigned __int128)(unsigned __int64)(2 * v82)
        + (unsigned __int64)(v95 >> 51);
    v98 = (v93 & 0x7FFFFFFFFFFFFLL) + (v96 >> 51);
    v99 = (v94 & 0x7FFFFFFFFFFFFLL) + 19 * (v97 >> 51);
    v438 = v99 & 0x7FFFFFFFFFFFFLL;
    v439 = (v96 & 0x7FFFFFFFFFFFFLL) + (v99 >> 51);
    v440 = v98 & 0x7FFFFFFFFFFFFLL;
    v441 = (v95 & 0x7FFFFFFFFFFFFLL) + (v98 >> 51);
    v100 = (__int128)((unsigned __int64)v423 * (unsigned __int128)0x1DB42uLL) >> 51;
    v101 = ((121666 * v425) & 0x7FFFFFFFFFFFELL)
         + ((__int128)((unsigned __int64)v424 * (unsigned __int128)0x1DB42uLL + v100) >> 51);
    v102 = (unsigned __int64)v81 * (unsigned __int128)(unsigned __int64)(2 * v413)
         + (unsigned __int64)v415 * (unsigned __int128)(unsigned __int64)v415
         + 19 * v411 * (unsigned __int128)(2 * v83);
    v103 = 19 * v83 * (unsigned __int128)(unsigned __int64)(2 * v81)
         + (unsigned __int64)v413 * (unsigned __int128)(unsigned __int64)v413
         + 19 * v411 * (unsigned __int128)(unsigned __int64)(2 * v415);
    v442 = v97 & 0x7FFFFFFFFFFFFLL;
    v104 = v83 * (unsigned __int128)(unsigned __int64)(2 * v413)
         + (unsigned __int64)v81 * (unsigned __int128)(unsigned __int64)(2 * v415)
         + 19 * v411 * (unsigned __int128)v411
         + (unsigned __int64)(v102 >> 51);
    v105 = __PAIR128__(
             (((unsigned __int64)v415 * (unsigned __int128)(unsigned __int64)(2 * v413)) >> 64)
           + ((19 * v83 * (unsigned __int128)v83 + (unsigned __int64)(v415 * 2 * v413)) >> 64),
             19 * v83 * v83 + v415 * 2 * v413)
         + 19 * v411 * (unsigned __int128)(unsigned __int64)(2 * v81)
         + (unsigned __int64)(v103 >> 51);
    v107 = ((121666 * v423) & 0x7FFFFFFFFFFFELL)
         + 19
         * ((__int128)((unsigned __int64)((__int128)((unsigned __int64)v426 * (unsigned __int128)0x1DB42uLL
                                                   + (unsigned __int64)((__int128)((unsigned __int64)v425
                                                                                 * (unsigned __int128)0x1DB42uLL) >> 51)) >> 51)
                     + (unsigned __int64)v427 * (unsigned __int128)0x1DB42uLL) >> 51);
    v433 = v107 & 0x7FFFFFFFFFFFFLL;
    v434 = ((121666 * v424 + v100) & 0x7FFFFFFFFFFFFLL) + (v107 >> 51);
    v428 = (v107 & 0x7FFFFFFFFFFFFLL) + v418;
    v429 = v434 + v420;
    v435 = v101 & 0x7FFFFFFFFFFFFLL;
    v436 = ((121666 * v426 + ((__int128)((unsigned __int64)v425 * (unsigned __int128)0x1DB42uLL) >> 51))
          & 0x7FFFFFFFFFFFFLL)
         + (v101 >> 51);
    v430 = (v101 & 0x7FFFFFFFFFFFFLL) + v409;
    v431 = v436 + v410;
    v437 = (((__int128)((unsigned __int64)v426 * (unsigned __int128)0x1DB42uLL
                      + (unsigned __int64)((__int128)((unsigned __int64)v425 * (unsigned __int128)0x1DB42uLL) >> 51)) >> 51)
          + 121666 * v427)
         & 0x7FFFFFFFFFFFFLL;
    v432 = v437 + (v72 & 0x7FFFFFFFFFFFFLL);
    v106 = v83 * (unsigned __int128)(unsigned __int64)(2 * v415)
         + (unsigned __int64)v81 * (unsigned __int128)(unsigned __int64)v81
         + v411 * (unsigned __int128)(unsigned __int64)(2 * v413)
         + (unsigned __int64)(v104 >> 51);
    sub_1003DA5A4(&v433, v448, &v438);
    sub_1003DA5A4(&v438, &v423, &v428);
    if ( !v416 )
      break;
    v108 = (v102 & 0x7FFFFFFFFFFFFLL) + (v105 >> 51);
    v29 = v443;
    v13 = v444;
    v12 = v445;
    v11 = v446;
    v109 = (v103 & 0x7FFFFFFFFFFFFLL) + 19 * (v106 >> 51);
    v27 = v416 - 1;
    v21 = v109 & 0x7FFFFFFFFFFFFLL;
    v22 = (v105 & 0x7FFFFFFFFFFFFLL) + (v109 >> 51);
    v10 = v438;
    v9 = v439;
    v23 = v108 & 0x7FFFFFFFFFFFFLL;
    v26 = (v104 & 0x7FFFFFFFFFFFFLL) + (v108 >> 51);
    v25 = v106 & 0x7FFFFFFFFFFFFLL;
    v28 = v433;
    v8 = v434;
    v7 = v440;
    v5 = v441;
    v6 = v435;
    v4 = v436;
    v417 = v442;
    v419 = v447;
    v3 = v437;
    v14 = v406;
  }
  v110 = v440;
  v111 = ((unsigned __int64)v110 * (unsigned __int128)(unsigned __int64)v110) >> 64;
  v112 = v110 * v110;
  v113 = (unsigned __int64)v441 * (unsigned __int128)(unsigned __int64)(2 * v438)
       + (unsigned __int64)(2 * v439) * (unsigned __int128)(unsigned __int64)v440
       + (unsigned __int64)(19 * v442) * (unsigned __int128)(unsigned __int64)v442
       + (unsigned __int64)((__int128)((unsigned __int64)v440 * (unsigned __int128)(unsigned __int64)(2 * v438)
                                     + (unsigned __int64)v439 * (unsigned __int128)(unsigned __int64)v439
                                     + (unsigned __int64)(19 * v442) * (unsigned __int128)(unsigned __int64)(2 * v441)) >> 51);
  v114 = ((v440 * 2 * v438 + v439 * v439 + 19 * v442 * 2 * v441) & 0x7FFFFFFFFFFFFLL)
       + ((__int128)(__PAIR128__(
                       (((unsigned __int64)(2 * v438) * (unsigned __int128)(unsigned __int64)v439) >> 64)
                     + (((unsigned __int64)(19 * v441) * (unsigned __int128)(unsigned __int64)v441
                       + (unsigned __int64)(2 * v438 * v439)) >> 64),
                       19 * v441 * v441 + 2 * v438 * v439)
                   + (unsigned __int64)(19 * v442) * (unsigned __int128)(unsigned __int64)(2 * v440)
                   + (unsigned __int64)((__int128)(__PAIR128__(
                                                     (((unsigned __int64)v438 * (unsigned __int128)(unsigned __int64)v438) >> 64)
                                                   + (((unsigned __int64)(19 * v441)
                                                     * (unsigned __int128)(unsigned __int64)(2 * v440)
                                                     + (unsigned __int64)(v438 * v438)) >> 64),
                                                     19 * v441 * 2 * v440 + v438 * v438)
                                                 + (unsigned __int64)(19 * v442)
                                                 * (unsigned __int128)(unsigned __int64)(2 * v439)) >> 51)) >> 51);
  v115 = ((19 * v441 * 2 * v440 + v438 * v438 + 19 * v442 * 2 * v439) & 0x7FFFFFFFFFFFFLL)
       + 19
       * ((__int128)((unsigned __int64)v441 * (unsigned __int128)(unsigned __int64)(2 * v439)
                   + __PAIR128__(v111, v112)
                   + (unsigned __int64)v442 * (unsigned __int128)(unsigned __int64)(2 * v438)
                   + (unsigned __int64)(v113 >> 51)) >> 51);
  v464 = v115 & 0x7FFFFFFFFFFFFLL;
  v465 = ((19 * v441 * v441
         + 2 * v438 * v439
         + 19 * v442 * 2 * v440
         + ((__int128)(__PAIR128__(
                         (((unsigned __int64)v438 * (unsigned __int128)(unsigned __int64)v438) >> 64)
                       + (((unsigned __int64)(19 * v441) * (unsigned __int128)(unsigned __int64)(2 * v440)
                         + (unsigned __int64)(v438 * v438)) >> 64),
                         19 * v441 * 2 * v440 + v438 * v438)
                     + (unsigned __int64)(19 * v442) * (unsigned __int128)(unsigned __int64)(2 * v439)) >> 51))
        & 0x7FFFFFFFFFFFFLL)
       + (v115 >> 51);
  v466 = v114 & 0x7FFFFFFFFFFFFLL;
  v467 = (v113 & 0x7FFFFFFFFFFFFLL) + (v114 >> 51);
  v468 = (v441 * 2 * v439 + v112 + v442 * 2 * v438 + (v113 >> 51)) & 0x7FFFFFFFFFFFFLL;
  v116 = 2 * (v115 & 0x7FFFFFFFFFFFFLL);
  v117 = ((unsigned __int64)v116 * (unsigned __int128)(v114 & 0x7FFFFFFFFFFFFLL)) >> 64;
  v118 = v116 * (v114 & 0x7FFFFFFFFFFFFLL);
  v119 = v116 * v467;
  v120 = (2 * v465 * (unsigned __int128)(v114 & 0x7FFFFFFFFFFFFLL)) >> 64;
  v121 = 2 * v465 * (v114 & 0x7FFFFFFFFFFFFLL);
  v122 = 2 * (v114 & 0x7FFFFFFFFFFFFLL);
  v123 = v464 * v464 + 19 * v467 * v122 + 2 * v465 * 19 * v468;
  v125 = (19 * v468 * (unsigned __int128)(unsigned __int64)v122
        + (unsigned __int64)(19 * v467) * (unsigned __int128)(unsigned __int64)v467
        + (unsigned __int64)v116 * (unsigned __int128)v465
        + (((unsigned __int64)v464 * (unsigned __int128)(unsigned __int64)v464
          + (unsigned __int64)(19 * v467) * (unsigned __int128)(unsigned __int64)v122
          + 2 * v465 * (unsigned __int128)(19 * v468)) >> 51)) >> 64;
  v124 = 19 * v468 * v122
       + 19 * v467 * v467
       + v116 * v465
       + (((unsigned __int64)v464 * (unsigned __int128)(unsigned __int64)v464
         + (unsigned __int64)(19 * v467) * (unsigned __int128)(unsigned __int64)v122
         + 2 * v465 * (unsigned __int128)(19 * v468)) >> 51);
  v127 = ((unsigned __int64)v116 * (unsigned __int128)v468
        + (unsigned __int64)v466 * (unsigned __int128)(unsigned __int64)v466
        + 2 * v465 * (unsigned __int128)(unsigned __int64)v467
        + (((unsigned __int64)v116 * (unsigned __int128)(unsigned __int64)v467
          + 19 * v468 * (unsigned __int128)v468
          + __PAIR128__(v120, v121)
          + ((v465 * (unsigned __int128)v465
            + 19 * v468 * (unsigned __int128)(unsigned __int64)(2 * v467)
            + __PAIR128__(v117, v118)) >> 51)) >> 51)) >> 64;
  v126 = v116 * v468
       + v466 * v466
       + 2 * v465 * v467
       + (((unsigned __int64)v116 * (unsigned __int128)(unsigned __int64)v467
         + 19 * v468 * (unsigned __int128)v468
         + __PAIR128__(v120, v121)
         + ((v465 * (unsigned __int128)v465
           + 19 * v468 * (unsigned __int128)(unsigned __int64)(2 * v467)
           + __PAIR128__(v117, v118)) >> 51)) >> 51);
  v128 = ((v465 * v465 + 19 * v468 * 2 * v467 + v118) & 0x7FFFFFFFFFFFFLL) + ((__int128)__PAIR128__(v125, v124) >> 51);
  v129 = (v123 & 0x7FFFFFFFFFFFFLL) + 19 * ((__int128)__PAIR128__(v127, v126) >> 51);
  v126 &= 0x7FFFFFFFFFFFFuLL;
  v130 = ((v119
         + 19 * v468 * v468
         + v121
         + ((v465 * (unsigned __int128)v465
           + 19 * v468 * (unsigned __int128)(unsigned __int64)(2 * v467)
           + __PAIR128__(v117, v118)) >> 51))
        & 0x7FFFFFFFFFFFFLL)
       + (v128 >> 51);
  v131 = v128 & 0x7FFFFFFFFFFFFLL;
  v132 = (v124 & 0x7FFFFFFFFFFFFLL) + (v129 >> 51);
  v133 = v129 & 0x7FFFFFFFFFFFFLL;
  v134 = ((unsigned __int64)v133 * (unsigned __int128)(unsigned __int64)v133) >> 64;
  v135 = v133 * v133;
  v133 *= 2;
  v136 = (unsigned __int64)v132 * (unsigned __int128)(unsigned __int64)v132 + 2 * v130 * (unsigned __int128)(19 * v126);
  v137 = v136 + (unsigned __int64)(v133 * v131);
  v138 = __PAIR128__(v134, v135)
       + 19 * v130 * (unsigned __int128)(unsigned __int64)(2 * v131)
       + (unsigned __int64)(2 * v132) * (unsigned __int128)(19 * v126);
  *((_QWORD *)&v139 + 1) = (((unsigned __int64)v133 * (unsigned __int128)(unsigned __int64)v131) >> 64)
                         + *((_QWORD *)&v137 + 1);
  *(_QWORD *)&v139 = v136 + v133 * v131;
  v140 = (unsigned __int64)v133 * (unsigned __int128)v130
       + 19 * v126 * (unsigned __int128)v126
       + (unsigned __int64)(2 * v132) * (unsigned __int128)(unsigned __int64)v131
       + (v139 >> 51);
  v141 = 19 * v130 * (unsigned __int128)v130
       + 19 * v126 * (unsigned __int128)(unsigned __int64)(2 * v131)
       + (unsigned __int64)v133 * (unsigned __int128)(unsigned __int64)v132
       + (v138 >> 51);
  v142 = (unsigned __int64)v133 * (unsigned __int128)v126
       + (unsigned __int64)v131 * (unsigned __int128)(unsigned __int64)v131
       + (unsigned __int64)(2 * v132) * (unsigned __int128)v130
       + (v140 >> 51);
  v143 = (v137 & 0x7FFFFFFFFFFFFLL) + (v141 >> 51);
  v144 = (v138 & 0x7FFFFFFFFFFFFLL) + 19 * (v142 >> 51);
  v459 = v144 & 0x7FFFFFFFFFFFFLL;
  v460 = (v141 & 0x7FFFFFFFFFFFFLL) + (v144 >> 51);
  v461 = v143 & 0x7FFFFFFFFFFFFLL;
  v462 = (v140 & 0x7FFFFFFFFFFFFLL) + (v143 >> 51);
  v463 = v142 & 0x7FFFFFFFFFFFFLL;
  sub_1003DA5A4(&v459, &v438, &v459);
  sub_1003DA5A4(&v464, &v464, &v459);
  v145 = (unsigned __int64)v466 * (unsigned __int128)(unsigned __int64)(2 * v464)
       + v465 * (unsigned __int128)v465
       + 19 * v468 * (unsigned __int128)(unsigned __int64)(2 * v467);
  v146 = (v145 & 0x7FFFFFFFFFFFFLL)
       + ((__int128)(__PAIR128__(
                       (((unsigned __int64)(2 * v464) * (unsigned __int128)v465) >> 64)
                     + (((unsigned __int64)(19 * v467) * (unsigned __int128)(unsigned __int64)v467 + 2 * v464 * v465) >> 64),
                       19 * v467 * v467 + 2 * v464 * v465)
                   + 19 * v468 * (unsigned __int128)(unsigned __int64)(2 * v466)
                   + (unsigned __int64)((__int128)(__PAIR128__(
                                                     (((unsigned __int64)v464 * (unsigned __int128)(unsigned __int64)v464) >> 64)
                                                   + (((unsigned __int64)(19 * v467)
                                                     * (unsigned __int128)(unsigned __int64)(2 * v466)
                                                     + (unsigned __int64)(v464 * v464)) >> 64),
                                                     19 * v467 * 2 * v466 + v464 * v464)
                                                 + 19 * v468 * (unsigned __int128)(2 * v465)) >> 51)) >> 51);
  v147 = ((19 * v467 * 2 * v466 + v464 * v464 + 19 * v468 * 2 * v465) & 0x7FFFFFFFFFFFFLL)
       + 19
       * ((__int128)((unsigned __int64)v467 * (unsigned __int128)(2 * v465)
                   + (unsigned __int64)v466 * (unsigned __int128)(unsigned __int64)v466
                   + v468 * (unsigned __int128)(unsigned __int64)(2 * v464)
                   + (unsigned __int64)((__int128)((unsigned __int64)v467
                                                 * (unsigned __int128)(unsigned __int64)(2 * v464)
                                                 + 2 * v465 * (unsigned __int128)(unsigned __int64)v466
                                                 + 19 * v468 * (unsigned __int128)v468
                                                 + (unsigned __int64)(v145 >> 51)) >> 51)) >> 51);
  v454 = v147 & 0x7FFFFFFFFFFFFLL;
  v455 = ((19 * v467 * v467
         + 2 * v464 * v465
         + 19 * v468 * 2 * v466
         + ((__int128)(__PAIR128__(
                         (((unsigned __int64)v464 * (unsigned __int128)(unsigned __int64)v464) >> 64)
                       + (((unsigned __int64)(19 * v467) * (unsigned __int128)(unsigned __int64)(2 * v466)
                         + (unsigned __int64)(v464 * v464)) >> 64),
                         19 * v467 * 2 * v466 + v464 * v464)
                     + 19 * v468 * (unsigned __int128)(2 * v465)) >> 51))
        & 0x7FFFFFFFFFFFFLL)
       + (v147 >> 51);
  v456 = v146 & 0x7FFFFFFFFFFFFLL;
  v457 = ((v467 * 2 * v464 + 2 * v465 * v466 + 19 * v468 * v468 + (v145 >> 51)) & 0x7FFFFFFFFFFFFLL) + (v146 >> 51);
  v458 = (v467 * 2 * v465
        + v466 * v466
        + v468 * 2 * v464
        + ((__int128)((unsigned __int64)v467 * (unsigned __int128)(unsigned __int64)(2 * v464)
                    + 2 * v465 * (unsigned __int128)(unsigned __int64)v466
                    + 19 * v468 * (unsigned __int128)v468
                    + (unsigned __int64)(v145 >> 51)) >> 51))
       & 0x7FFFFFFFFFFFFLL;
  sub_1003DA5A4(&v459, &v459, &v454);
  v148 = v461;
  v149 = ((unsigned __int64)v148 * (unsigned __int128)(unsigned __int64)v148) >> 64;
  v150 = v148 * v148;
  v151 = (unsigned __int64)v462 * (unsigned __int128)(unsigned __int64)(2 * v459)
       + (unsigned __int64)(2 * v460) * (unsigned __int128)(unsigned __int64)v461
       + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)v463
       + (unsigned __int64)((__int128)((unsigned __int64)v461 * (unsigned __int128)(unsigned __int64)(2 * v459)
                                     + (unsigned __int64)v460 * (unsigned __int128)(unsigned __int64)v460
                                     + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)(2 * v462)) >> 51);
  v152 = ((v461 * 2 * v459 + v460 * v460 + 19 * v463 * 2 * v462) & 0x7FFFFFFFFFFFFLL)
       + ((__int128)(__PAIR128__(
                       (((unsigned __int64)(2 * v459) * (unsigned __int128)(unsigned __int64)v460) >> 64)
                     + (((unsigned __int64)(19 * v462) * (unsigned __int128)(unsigned __int64)v462
                       + (unsigned __int64)(2 * v459 * v460)) >> 64),
                       19 * v462 * v462 + 2 * v459 * v460)
                   + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)(2 * v461)
                   + (unsigned __int64)((__int128)(__PAIR128__(
                                                     (((unsigned __int64)v459 * (unsigned __int128)(unsigned __int64)v459) >> 64)
                                                   + (((unsigned __int64)(19 * v462)
                                                     * (unsigned __int128)(unsigned __int64)(2 * v461)
                                                     + (unsigned __int64)(v459 * v459)) >> 64),
                                                     19 * v462 * 2 * v461 + v459 * v459)
                                                 + (unsigned __int64)(19 * v463)
                                                 * (unsigned __int128)(unsigned __int64)(2 * v460)) >> 51)) >> 51);
  v153 = __PAIR128__(
           v149
         + __CFADD__(v462 * 2 * v460, v150)
         + (((unsigned __int64)v462 * (unsigned __int128)(unsigned __int64)(2 * v460)) >> 64),
           v462 * 2 * v460 + v150)
       + (unsigned __int64)v463 * (unsigned __int128)(unsigned __int64)(2 * v459)
       + (unsigned __int64)(v151 >> 51);
  v154 = ((19 * v462 * 2 * v461 + v459 * v459 + 19 * v463 * 2 * v460) & 0x7FFFFFFFFFFFFLL) + 19 * (v153 >> 51);
  v155 = (v151 & 0x7FFFFFFFFFFFFLL) + (v152 >> 51);
  v152 &= 0x7FFFFFFFFFFFFuLL;
  v156 = ((19 * v462 * v462
         + 2 * v459 * v460
         + 19 * v463 * 2 * v461
         + ((__int128)(__PAIR128__(
                         (((unsigned __int64)v459 * (unsigned __int128)(unsigned __int64)v459) >> 64)
                       + (((unsigned __int64)(19 * v462) * (unsigned __int128)(unsigned __int64)(2 * v461)
                         + (unsigned __int64)(v459 * v459)) >> 64),
                         19 * v462 * 2 * v461 + v459 * v459)
                     + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)(2 * v460)) >> 51))
        & 0x7FFFFFFFFFFFFLL)
       + (v154 >> 51);
  v154 &= 0x7FFFFFFFFFFFFuLL;
  v157 = (v154 * (unsigned __int128)v154) >> 64;
  v158 = v154 * v154;
  v154 *= 2LL;
  v159 = (v154 * (unsigned __int128)v156) >> 64;
  v160 = v154 * v156;
  v161 = (v156 * (unsigned __int128)v156) >> 64;
  v162 = v156 * v156;
  v156 *= 2LL;
  v163 = (v156 * (unsigned __int128)v152) >> 64;
  v164 = v156 * v152;
  v166 = v162 + 19 * (v153 & 0x7FFFFFFFFFFFFLL) * 2 * v155 + v154 * v152;
  v165 = (__PAIR128__(v161, v162)
        + 19 * ((unsigned __int64)v153 & 0x7FFFFFFFFFFFFLL) * (unsigned __int128)(unsigned __int64)(2 * v155)
        + v154 * (unsigned __int128)v152) >> 64;
  v167 = (v152 * (unsigned __int128)v152) >> 64;
  v168 = v152 * v152;
  v152 *= 2LL;
  v169 = v158 + 19 * v155 * v152 + v156 * 19 * (v153 & 0x7FFFFFFFFFFFFLL);
  v170 = v154 * (unsigned __int128)(unsigned __int64)v155
       + 19 * ((unsigned __int64)v153 & 0x7FFFFFFFFFFFFLL) * (unsigned __int128)(v153 & 0x7FFFFFFFFFFFFLL)
       + __PAIR128__(v163, v164)
       + (__PAIR128__(v165, v166) >> 51);
  v171 = 19 * ((unsigned __int64)v153 & 0x7FFFFFFFFFFFFLL) * (unsigned __int128)v152
       + (unsigned __int64)(19 * v155) * (unsigned __int128)(unsigned __int64)v155
       + __PAIR128__(v159, v160)
       + ((__PAIR128__(v157, v158)
         + (unsigned __int64)(19 * v155) * (unsigned __int128)v152
         + v156 * (unsigned __int128)(19 * ((unsigned __int64)v153 & 0x7FFFFFFFFFFFFLL))) >> 51);
  v173 = (v154 * (unsigned __int128)(v153 & 0x7FFFFFFFFFFFFLL)
        + __PAIR128__(v167, v168)
        + v156 * (unsigned __int128)(unsigned __int64)v155
        + (v170 >> 51)) >> 64;
  v172 = v154 * (v153 & 0x7FFFFFFFFFFFFLL) + v168 + v156 * v155 + (v170 >> 51);
  v174 = (v166 & 0x7FFFFFFFFFFFFLL) + (v171 >> 51);
  v175 = (v169 & 0x7FFFFFFFFFFFFLL) + 19 * ((__int128)__PAIR128__(v173, v172) >> 51);
  v172 &= 0x7FFFFFFFFFFFFuLL;
  v176 = (v170 & 0x7FFFFFFFFFFFFLL) + (v174 >> 51);
  v174 &= 0x7FFFFFFFFFFFFuLL;
  v177 = (v171 & 0x7FFFFFFFFFFFFLL) + (v175 >> 51);
  v175 &= 0x7FFFFFFFFFFFFuLL;
  v178 = (v175 * (unsigned __int128)v175) >> 64;
  v179 = v175 * v175;
  v175 *= 2LL;
  v180 = (v175 * (unsigned __int128)(unsigned __int64)v177) >> 64;
  v181 = v175 * v177;
  v182 = (v175 * (unsigned __int128)v174) >> 64;
  v183 = v175 * v174;
  v184 = (v175 * (unsigned __int128)(unsigned __int64)v176) >> 64;
  v185 = v175 * v176;
  v186 = (v175 * (unsigned __int128)(unsigned __int64)v172) >> 64;
  v187 = v175 * v172;
  v188 = ((unsigned __int64)v177 * (unsigned __int128)(unsigned __int64)v177) >> 64;
  v189 = v177 * v177;
  v177 *= 2;
  v190 = ((unsigned __int64)v177 * (unsigned __int128)v174) >> 64;
  v191 = v177 * v174;
  v193 = v189 + 2 * v176 * 19 * v172 + v183;
  v192 = (__PAIR128__(v188, v189)
        + (unsigned __int64)(2 * v176) * (unsigned __int128)(unsigned __int64)(19 * v172)
        + __PAIR128__(v182, v183)) >> 64;
  v194 = (v174 * (unsigned __int128)v174) >> 64;
  v195 = v174 * v174;
  v174 *= 2LL;
  v196 = v179 + 19 * v176 * v174 + v177 * 19 * v172;
  v197 = __PAIR128__(v184, v185)
       + (unsigned __int64)(19 * v172) * (unsigned __int128)(unsigned __int64)v172
       + __PAIR128__(v190, v191)
       + (__PAIR128__(v192, v193) >> 51);
  v198 = (unsigned __int64)(19 * v176) * (unsigned __int128)(unsigned __int64)v176
       + (unsigned __int64)(19 * v172) * (unsigned __int128)v174
       + __PAIR128__(v180, v181)
       + ((__PAIR128__(v178, v179)
         + (unsigned __int64)(19 * v176) * (unsigned __int128)v174
         + (unsigned __int64)v177 * (unsigned __int128)(unsigned __int64)(19 * v172)) >> 51);
  v200 = (__PAIR128__(v186, v187)
        + __PAIR128__(v194, v195)
        + (unsigned __int64)v177 * (unsigned __int128)(unsigned __int64)v176
        + (v197 >> 51)) >> 64;
  v199 = v187 + v195 + v177 * v176 + (v197 >> 51);
  v201 = (v193 & 0x7FFFFFFFFFFFFLL) + (v198 >> 51);
  v202 = (v196 & 0x7FFFFFFFFFFFFLL) + 19 * ((__int128)__PAIR128__(v200, v199) >> 51);
  v199 &= 0x7FFFFFFFFFFFFuLL;
  v203 = (v197 & 0x7FFFFFFFFFFFFLL) + (v201 >> 51);
  v201 &= 0x7FFFFFFFFFFFFuLL;
  v204 = (v198 & 0x7FFFFFFFFFFFFLL) + (v202 >> 51);
  v202 &= 0x7FFFFFFFFFFFFuLL;
  v205 = (v202 * (unsigned __int128)v202) >> 64;
  v206 = v202 * v202;
  v202 *= 2LL;
  v207 = (v202 * (unsigned __int128)(unsigned __int64)v204) >> 64;
  v208 = v202 * v204;
  v209 = (v202 * (unsigned __int128)v201) >> 64;
  v210 = v202 * v201;
  v211 = (v202 * (unsigned __int128)(unsigned __int64)v203) >> 64;
  v212 = v202 * v203;
  v213 = (v202 * (unsigned __int128)v199) >> 64;
  v214 = v202 * v199;
  v215 = ((unsigned __int64)v204 * (unsigned __int128)(unsigned __int64)v204) >> 64;
  v216 = v204 * v204;
  v204 *= 2;
  v217 = ((unsigned __int64)v204 * (unsigned __int128)v201) >> 64;
  v218 = v204 * v201;
  v220 = v216 + 2 * v203 * 19 * v199 + v210;
  v219 = (__PAIR128__(v215, v216)
        + (unsigned __int64)(2 * v203) * (unsigned __int128)(19 * v199)
        + __PAIR128__(v209, v210)) >> 64;
  v221 = (v201 * (unsigned __int128)v201) >> 64;
  v222 = v201 * v201;
  v201 *= 2LL;
  v223 = v206 + 19 * v203 * v201 + v204 * 19 * v199;
  v224 = v219 >> 51;
  v225 = (__int128)__PAIR128__(v219, v220) >> 51;
  v226 = __PAIR128__(v211, v212) + 19 * v199 * (unsigned __int128)v199 + __PAIR128__(v217, v218);
  v227 = (unsigned __int64)(19 * v203) * (unsigned __int128)(unsigned __int64)v203
       + 19 * v199 * (unsigned __int128)v201
       + __PAIR128__(v207, v208)
       + ((__PAIR128__(v205, v206)
         + (unsigned __int64)(19 * v203) * (unsigned __int128)v201
         + (unsigned __int64)v204 * (unsigned __int128)(19 * v199)) >> 51);
  v229 = (__PAIR128__(v213, v214)
        + __PAIR128__(v221, v222)
        + (unsigned __int64)v204 * (unsigned __int128)(unsigned __int64)v203
        + ((v226 + __PAIR128__(v224, v225)) >> 51)) >> 64;
  v228 = v214 + v222 + v204 * v203 + ((v226 + __PAIR128__(v224, v225)) >> 51);
  v230 = (v220 & 0x7FFFFFFFFFFFFLL) + (v227 >> 51);
  v231 = (v223 & 0x7FFFFFFFFFFFFLL) + 19 * ((__int128)__PAIR128__(v229, v228) >> 51);
  v232 = v228 & 0x7FFFFFFFFFFFFLL;
  v233 = ((v226 + v225) & 0x7FFFFFFFFFFFFLL) + (v230 >> 51);
  v234 = v230 & 0x7FFFFFFFFFFFFLL;
  v235 = (v227 & 0x7FFFFFFFFFFFFLL) + (v231 >> 51);
  v236 = v231 & 0x7FFFFFFFFFFFFLL;
  v237 = ((unsigned __int64)v236 * (unsigned __int128)(unsigned __int64)v236) >> 64;
  v238 = v236 * v236;
  v236 *= 2;
  v239 = (unsigned __int64)v235 * (unsigned __int128)(unsigned __int64)v235
       + (unsigned __int64)(2 * v233) * (unsigned __int128)(unsigned __int64)(19 * v232);
  v240 = v239 + (unsigned __int64)(v236 * v234);
  v241 = __PAIR128__(v237, v238)
       + (unsigned __int64)(19 * v233) * (unsigned __int128)(unsigned __int64)(2 * v234)
       + (unsigned __int64)(2 * v235) * (unsigned __int128)(unsigned __int64)(19 * v232);
  *((_QWORD *)&v153 + 1) = (((unsigned __int64)v236 * (unsigned __int128)(unsigned __int64)v234) >> 64)
                         + *((_QWORD *)&v240 + 1);
  *(_QWORD *)&v153 = v239 + v236 * v234;
  v242 = (unsigned __int64)v236 * (unsigned __int128)(unsigned __int64)v233
       + (unsigned __int64)(19 * v232) * (unsigned __int128)(unsigned __int64)v232
       + (unsigned __int64)(2 * v235) * (unsigned __int128)(unsigned __int64)v234
       + ((unsigned __int128)v153 >> 51);
  v243 = (unsigned __int64)(19 * v233) * (unsigned __int128)(unsigned __int64)v233
       + (unsigned __int64)(19 * v232) * (unsigned __int128)(unsigned __int64)(2 * v234)
       + (unsigned __int64)v236 * (unsigned __int128)(unsigned __int64)v235
       + (v241 >> 51);
  v244 = (unsigned __int64)v236 * (unsigned __int128)(unsigned __int64)v232
       + (unsigned __int64)v234 * (unsigned __int128)(unsigned __int64)v234
       + (unsigned __int64)(2 * v235) * (unsigned __int128)(unsigned __int64)v233
       + (v242 >> 51);
  v245 = (v240 & 0x7FFFFFFFFFFFFLL) + (v243 >> 51);
  v246 = (v241 & 0x7FFFFFFFFFFFFLL) + 19 * (v244 >> 51);
  v454 = v246 & 0x7FFFFFFFFFFFFLL;
  v455 = (v243 & 0x7FFFFFFFFFFFFLL) + (v246 >> 51);
  v456 = v245 & 0x7FFFFFFFFFFFFLL;
  v457 = (v242 & 0x7FFFFFFFFFFFFLL) + (v245 >> 51);
  v458 = v244 & 0x7FFFFFFFFFFFFLL;
  sub_1003DA5A4(&v459, &v454, &v459);
  v247 = (unsigned __int64)v461 * (unsigned __int128)(unsigned __int64)(2 * v459)
       + (unsigned __int64)v460 * (unsigned __int128)(unsigned __int64)v460
       + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)(2 * v462);
  v248 = (v247 & 0x7FFFFFFFFFFFFLL)
       + ((__int128)(__PAIR128__(
                       (((unsigned __int64)(2 * v459) * (unsigned __int128)(unsigned __int64)v460) >> 64)
                     + (((unsigned __int64)(19 * v462) * (unsigned __int128)(unsigned __int64)v462
                       + (unsigned __int64)(2 * v459 * v460)) >> 64),
                       19 * v462 * v462 + 2 * v459 * v460)
                   + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)(2 * v461)
                   + (unsigned __int64)((__int128)((unsigned __int64)(19 * v462)
                                                 * (unsigned __int128)(unsigned __int64)(2 * v461)
                                                 + (unsigned __int64)v459 * (unsigned __int128)(unsigned __int64)v459
                                                 + (unsigned __int64)(19 * v463)
                                                 * (unsigned __int128)(unsigned __int64)(2 * v460)) >> 51)) >> 51);
  v249 = ((19 * v462 * 2 * v461 + v459 * v459 + 19 * v463 * 2 * v460) & 0x7FFFFFFFFFFFFLL)
       + 19
       * ((__int128)((unsigned __int64)v462 * (unsigned __int128)(unsigned __int64)(2 * v460)
                   + (unsigned __int64)v461 * (unsigned __int128)(unsigned __int64)v461
                   + (unsigned __int64)v463 * (unsigned __int128)(unsigned __int64)(2 * v459)
                   + (unsigned __int64)((__int128)((unsigned __int64)v462
                                                 * (unsigned __int128)(unsigned __int64)(2 * v459)
                                                 + (unsigned __int64)(2 * v460)
                                                 * (unsigned __int128)(unsigned __int64)v461
                                                 + (unsigned __int64)(19 * v463)
                                                 * (unsigned __int128)(unsigned __int64)v463
                                                 + (unsigned __int64)(v247 >> 51)) >> 51)) >> 51);
  v250 = (v462 * 2 * v460
        + v461 * v461
        + v463 * 2 * v459
        + ((__int128)((unsigned __int64)v462 * (unsigned __int128)(unsigned __int64)(2 * v459)
                    + (unsigned __int64)(2 * v460) * (unsigned __int128)(unsigned __int64)v461
                    + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)v463
                    + (unsigned __int64)(v247 >> 51)) >> 51))
       & 0x7FFFFFFFFFFFFLL;
  v251 = ((v462 * 2 * v459 + 2 * v460 * v461 + 19 * v463 * v463 + (v247 >> 51)) & 0x7FFFFFFFFFFFFLL) + (v248 >> 51);
  v252 = v248 & 0x7FFFFFFFFFFFFLL;
  v253 = ((19 * v462 * v462
         + 2 * v459 * v460
         + 19 * v463 * 2 * v461
         + ((__int128)((unsigned __int64)(19 * v462) * (unsigned __int128)(unsigned __int64)(2 * v461)
                     + (unsigned __int64)v459 * (unsigned __int128)(unsigned __int64)v459
                     + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)(2 * v460)) >> 51))
        & 0x7FFFFFFFFFFFFLL)
       + (v249 >> 51);
  v254 = v249 & 0x7FFFFFFFFFFFFLL;
  v255 = 9;
  do
  {
    v256 = ((unsigned __int64)v254 * (unsigned __int128)(unsigned __int64)v254) >> 64;
    v257 = v254 * v254;
    v258 = 2 * v254;
    v259 = ((unsigned __int64)v258 * (unsigned __int128)(unsigned __int64)v253) >> 64;
    v260 = v258 * v253;
    v261 = (unsigned __int64)v252 * (unsigned __int128)(unsigned __int64)v258
         + (unsigned __int64)v253 * (unsigned __int128)(unsigned __int64)v253;
    v262 = 2 * v253;
    v263 = v261 + 19 * v250 * (unsigned __int128)(unsigned __int64)(2 * v251);
    v264 = __PAIR128__(
             v256
           + (((unsigned __int64)(19 * v251) * (unsigned __int128)(unsigned __int64)(2 * v252) + (unsigned __int64)v257) >> 64),
             19 * v251 * 2 * v252 + v257)
         + 19 * v250 * (unsigned __int128)(unsigned __int64)v262;
    v265 = (unsigned __int64)v251 * (unsigned __int128)(unsigned __int64)v258
         + (unsigned __int64)v262 * (unsigned __int128)(unsigned __int64)v252
         + 19 * v250 * (unsigned __int128)v250
         + (unsigned __int64)(v263 >> 51);
    v266 = __PAIR128__(
             v259
           + (((unsigned __int64)(19 * v251) * (unsigned __int128)(unsigned __int64)v251 + (unsigned __int64)v260) >> 64),
             19 * v251 * v251 + v260)
         + 19 * v250 * (unsigned __int128)(unsigned __int64)(2 * v252)
         + (unsigned __int64)(v264 >> 51);
    v267 = (unsigned __int64)v251 * (unsigned __int128)(unsigned __int64)v262
         + (unsigned __int64)v252 * (unsigned __int128)(unsigned __int64)v252
         + v250 * (unsigned __int128)(unsigned __int64)v258
         + (unsigned __int64)(v265 >> 51);
    v268 = (v263 & 0x7FFFFFFFFFFFFLL) + (v266 >> 51);
    v250 = v267 & 0x7FFFFFFFFFFFFLL;
    v251 = (v265 & 0x7FFFFFFFFFFFFLL) + (v268 >> 51);
    v252 = v268 & 0x7FFFFFFFFFFFFLL;
    v269 = (v264 & 0x7FFFFFFFFFFFFLL) + 19 * (v267 >> 51);
    v253 = (v266 & 0x7FFFFFFFFFFFFLL) + (v269 >> 51);
    v254 = v269 & 0x7FFFFFFFFFFFFLL;
    --v255;
  }
  while ( v255 );
  v454 = v254;
  v455 = v253;
  v456 = v252;
  v457 = v251;
  v458 = v267 & 0x7FFFFFFFFFFFFLL;
  sub_1003DA5A4(&v454, &v454, &v459);
  v270 = (unsigned __int64)v456 * (unsigned __int128)(unsigned __int64)(2 * v454)
       + (unsigned __int64)v455 * (unsigned __int128)(unsigned __int64)v455
       + (unsigned __int64)(19 * v458) * (unsigned __int128)(unsigned __int64)(2 * v457);
  v271 = (v270 & 0x7FFFFFFFFFFFFLL)
       + ((__int128)(__PAIR128__(
                       (((unsigned __int64)(2 * v454) * (unsigned __int128)(unsigned __int64)v455) >> 64)
                     + (((unsigned __int64)(19 * v457) * (unsigned __int128)(unsigned __int64)v457
                       + (unsigned __int64)(2 * v454 * v455)) >> 64),
                       19 * v457 * v457 + 2 * v454 * v455)
                   + (unsigned __int64)(19 * v458) * (unsigned __int128)(unsigned __int64)(2 * v456)
                   + (unsigned __int64)((__int128)((unsigned __int64)(19 * v457)
                                                 * (unsigned __int128)(unsigned __int64)(2 * v456)
                                                 + (unsigned __int64)v454 * (unsigned __int128)(unsigned __int64)v454
                                                 + (unsigned __int64)(19 * v458)
                                                 * (unsigned __int128)(unsigned __int64)(2 * v455)) >> 51)) >> 51);
  v272 = ((19 * v457 * 2 * v456 + v454 * v454 + 19 * v458 * 2 * v455) & 0x7FFFFFFFFFFFFLL)
       + 19
       * ((__int128)((unsigned __int64)v457 * (unsigned __int128)(unsigned __int64)(2 * v455)
                   + (unsigned __int64)v456 * (unsigned __int128)(unsigned __int64)v456
                   + (unsigned __int64)v458 * (unsigned __int128)(unsigned __int64)(2 * v454)
                   + (unsigned __int64)((__int128)((unsigned __int64)v457
                                                 * (unsigned __int128)(unsigned __int64)(2 * v454)
                                                 + (unsigned __int64)(2 * v455)
                                                 * (unsigned __int128)(unsigned __int64)v456
                                                 + (unsigned __int64)(19 * v458)
                                                 * (unsigned __int128)(unsigned __int64)v458
                                                 + (unsigned __int64)(v270 >> 51)) >> 51)) >> 51);
  v273 = (v457 * 2 * v455
        + v456 * v456
        + v458 * 2 * v454
        + ((__int128)((unsigned __int64)v457 * (unsigned __int128)(unsigned __int64)(2 * v454)
                    + (unsigned __int64)(2 * v455) * (unsigned __int128)(unsigned __int64)v456
                    + (unsigned __int64)(19 * v458) * (unsigned __int128)(unsigned __int64)v458
                    + (unsigned __int64)(v270 >> 51)) >> 51))
       & 0x7FFFFFFFFFFFFLL;
  v274 = ((v457 * 2 * v454 + 2 * v455 * v456 + 19 * v458 * v458 + (v270 >> 51)) & 0x7FFFFFFFFFFFFLL) + (v271 >> 51);
  v275 = v271 & 0x7FFFFFFFFFFFFLL;
  v276 = ((19 * v457 * v457
         + 2 * v454 * v455
         + 19 * v458 * 2 * v456
         + ((__int128)((unsigned __int64)(19 * v457) * (unsigned __int128)(unsigned __int64)(2 * v456)
                     + (unsigned __int64)v454 * (unsigned __int128)(unsigned __int64)v454
                     + (unsigned __int64)(19 * v458) * (unsigned __int128)(unsigned __int64)(2 * v455)) >> 51))
        & 0x7FFFFFFFFFFFFLL)
       + (v272 >> 51);
  v277 = v272 & 0x7FFFFFFFFFFFFLL;
  v278 = 19;
  do
  {
    v279 = ((unsigned __int64)v277 * (unsigned __int128)(unsigned __int64)v277) >> 64;
    v280 = v277 * v277;
    v281 = 2 * v277;
    v282 = ((unsigned __int64)v281 * (unsigned __int128)(unsigned __int64)v276) >> 64;
    v283 = v281 * v276;
    v284 = (unsigned __int64)v275 * (unsigned __int128)(unsigned __int64)v281
         + (unsigned __int64)v276 * (unsigned __int128)(unsigned __int64)v276;
    v285 = 2 * v276;
    v286 = v284 + 19 * v273 * (unsigned __int128)(unsigned __int64)(2 * v274);
    v287 = __PAIR128__(
             v279
           + (((unsigned __int64)(19 * v274) * (unsigned __int128)(unsigned __int64)(2 * v275) + (unsigned __int64)v280) >> 64),
             19 * v274 * 2 * v275 + v280)
         + 19 * v273 * (unsigned __int128)(unsigned __int64)v285;
    v288 = (unsigned __int64)v274 * (unsigned __int128)(unsigned __int64)v281
         + (unsigned __int64)v285 * (unsigned __int128)(unsigned __int64)v275
         + 19 * v273 * (unsigned __int128)v273
         + (unsigned __int64)(v286 >> 51);
    v289 = __PAIR128__(
             v282
           + (((unsigned __int64)(19 * v274) * (unsigned __int128)(unsigned __int64)v274 + (unsigned __int64)v283) >> 64),
             19 * v274 * v274 + v283)
         + 19 * v273 * (unsigned __int128)(unsigned __int64)(2 * v275)
         + (unsigned __int64)(v287 >> 51);
    v290 = (unsigned __int64)v274 * (unsigned __int128)(unsigned __int64)v285
         + (unsigned __int64)v275 * (unsigned __int128)(unsigned __int64)v275
         + v273 * (unsigned __int128)(unsigned __int64)v281
         + (unsigned __int64)(v288 >> 51);
    v291 = (v286 & 0x7FFFFFFFFFFFFLL) + (v289 >> 51);
    v273 = v290 & 0x7FFFFFFFFFFFFLL;
    v274 = (v288 & 0x7FFFFFFFFFFFFLL) + (v291 >> 51);
    v275 = v291 & 0x7FFFFFFFFFFFFLL;
    v292 = (v287 & 0x7FFFFFFFFFFFFLL) + 19 * (v290 >> 51);
    v276 = (v289 & 0x7FFFFFFFFFFFFLL) + (v292 >> 51);
    v277 = v292 & 0x7FFFFFFFFFFFFLL;
    --v278;
  }
  while ( v278 );
  v449 = v277;
  v450 = v276;
  v451 = v275;
  v452 = v274;
  v453 = v290 & 0x7FFFFFFFFFFFFLL;
  sub_1003DA5A4(&v454, &v449, &v454);
  v293 = v454;
  v294 = v455;
  v295 = v456;
  v296 = v457;
  v297 = 10;
  v298 = v458;
  do
  {
    v299 = ((unsigned __int64)v293 * (unsigned __int128)(unsigned __int64)v293) >> 64;
    v300 = v293 * v293;
    v301 = 2 * v293;
    v302 = ((unsigned __int64)v301 * (unsigned __int128)(unsigned __int64)v294) >> 64;
    v303 = v301 * v294;
    v304 = (unsigned __int64)v295 * (unsigned __int128)(unsigned __int64)v301
         + (unsigned __int64)v294 * (unsigned __int128)(unsigned __int64)v294;
    v305 = 2 * v294;
    v306 = v304 + (unsigned __int64)(19 * v298) * (unsigned __int128)(unsigned __int64)(2 * v296);
    v307 = __PAIR128__(
             v299
           + (((unsigned __int64)(19 * v296) * (unsigned __int128)(unsigned __int64)(2 * v295) + (unsigned __int64)v300) >> 64),
             19 * v296 * 2 * v295 + v300)
         + (unsigned __int64)(19 * v298) * (unsigned __int128)(unsigned __int64)v305;
    v308 = (unsigned __int64)v296 * (unsigned __int128)(unsigned __int64)v301
         + (unsigned __int64)v305 * (unsigned __int128)(unsigned __int64)v295
         + (unsigned __int64)(19 * v298) * (unsigned __int128)(unsigned __int64)v298
         + (unsigned __int64)(v306 >> 51);
    v309 = __PAIR128__(
             v302
           + (((unsigned __int64)(19 * v296) * (unsigned __int128)(unsigned __int64)v296 + (unsigned __int64)v303) >> 64),
             19 * v296 * v296 + v303)
         + (unsigned __int64)(19 * v298) * (unsigned __int128)(unsigned __int64)(2 * v295)
         + (unsigned __int64)(v307 >> 51);
    v310 = (unsigned __int64)v296 * (unsigned __int128)(unsigned __int64)v305
         + (unsigned __int64)v295 * (unsigned __int128)(unsigned __int64)v295
         + (unsigned __int64)v298 * (unsigned __int128)(unsigned __int64)v301
         + (unsigned __int64)(v308 >> 51);
    v311 = (v306 & 0x7FFFFFFFFFFFFLL) + (v309 >> 51);
    v298 = v310 & 0x7FFFFFFFFFFFFLL;
    v296 = (v308 & 0x7FFFFFFFFFFFFLL) + (v311 >> 51);
    v295 = v311 & 0x7FFFFFFFFFFFFLL;
    v312 = (v307 & 0x7FFFFFFFFFFFFLL) + 19 * (v310 >> 51);
    v294 = (v309 & 0x7FFFFFFFFFFFFLL) + (v312 >> 51);
    v293 = v312 & 0x7FFFFFFFFFFFFLL;
    --v297;
  }
  while ( v297 );
  v454 = v293;
  v455 = v294;
  v456 = v295;
  v457 = v296;
  v458 = v310 & 0x7FFFFFFFFFFFFLL;
  sub_1003DA5A4(&v459, &v454, &v459);
  v313 = (unsigned __int64)v461 * (unsigned __int128)(unsigned __int64)(2 * v459)
       + (unsigned __int64)v460 * (unsigned __int128)(unsigned __int64)v460
       + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)(2 * v462);
  v314 = (v313 & 0x7FFFFFFFFFFFFLL)
       + ((__int128)(__PAIR128__(
                       (((unsigned __int64)(2 * v459) * (unsigned __int128)(unsigned __int64)v460) >> 64)
                     + (((unsigned __int64)(19 * v462) * (unsigned __int128)(unsigned __int64)v462
                       + (unsigned __int64)(2 * v459 * v460)) >> 64),
                       19 * v462 * v462 + 2 * v459 * v460)
                   + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)(2 * v461)
                   + (unsigned __int64)((__int128)((unsigned __int64)(19 * v462)
                                                 * (unsigned __int128)(unsigned __int64)(2 * v461)
                                                 + (unsigned __int64)v459 * (unsigned __int128)(unsigned __int64)v459
                                                 + (unsigned __int64)(19 * v463)
                                                 * (unsigned __int128)(unsigned __int64)(2 * v460)) >> 51)) >> 51);
  v315 = ((19 * v462 * 2 * v461 + v459 * v459 + 19 * v463 * 2 * v460) & 0x7FFFFFFFFFFFFLL)
       + 19
       * ((__int128)((unsigned __int64)v462 * (unsigned __int128)(unsigned __int64)(2 * v460)
                   + (unsigned __int64)v461 * (unsigned __int128)(unsigned __int64)v461
                   + (unsigned __int64)v463 * (unsigned __int128)(unsigned __int64)(2 * v459)
                   + (unsigned __int64)((__int128)((unsigned __int64)v462
                                                 * (unsigned __int128)(unsigned __int64)(2 * v459)
                                                 + (unsigned __int64)(2 * v460)
                                                 * (unsigned __int128)(unsigned __int64)v461
                                                 + (unsigned __int64)(19 * v463)
                                                 * (unsigned __int128)(unsigned __int64)v463
                                                 + (unsigned __int64)(v313 >> 51)) >> 51)) >> 51);
  v316 = (v462 * 2 * v460
        + v461 * v461
        + v463 * 2 * v459
        + ((__int128)((unsigned __int64)v462 * (unsigned __int128)(unsigned __int64)(2 * v459)
                    + (unsigned __int64)(2 * v460) * (unsigned __int128)(unsigned __int64)v461
                    + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)v463
                    + (unsigned __int64)(v313 >> 51)) >> 51))
       & 0x7FFFFFFFFFFFFLL;
  v317 = ((v462 * 2 * v459 + 2 * v460 * v461 + 19 * v463 * v463 + (v313 >> 51)) & 0x7FFFFFFFFFFFFLL) + (v314 >> 51);
  v318 = v314 & 0x7FFFFFFFFFFFFLL;
  v319 = ((19 * v462 * v462
         + 2 * v459 * v460
         + 19 * v463 * 2 * v461
         + ((__int128)((unsigned __int64)(19 * v462) * (unsigned __int128)(unsigned __int64)(2 * v461)
                     + (unsigned __int64)v459 * (unsigned __int128)(unsigned __int64)v459
                     + (unsigned __int64)(19 * v463) * (unsigned __int128)(unsigned __int64)(2 * v460)) >> 51))
        & 0x7FFFFFFFFFFFFLL)
       + (v315 >> 51);
  v320 = v315 & 0x7FFFFFFFFFFFFLL;
  v321 = 49;
  do
  {
    v322 = ((unsigned __int64)v320 * (unsigned __int128)(unsigned __int64)v320) >> 64;
    v323 = v320 * v320;
    v324 = 2 * v320;
    v325 = ((unsigned __int64)v324 * (unsigned __int128)(unsigned __int64)v319) >> 64;
    v326 = v324 * v319;
    v327 = (unsigned __int64)v318 * (unsigned __int128)(unsigned __int64)v324
         + (unsigned __int64)v319 * (unsigned __int128)(unsigned __int64)v319;
    v328 = 2 * v319;
    v329 = v327 + 19 * v316 * (unsigned __int128)(unsigned __int64)(2 * v317);
    v330 = __PAIR128__(
             v322
           + (((unsigned __int64)(19 * v317) * (unsigned __int128)(unsigned __int64)(2 * v318) + (unsigned __int64)v323) >> 64),
             19 * v317 * 2 * v318 + v323)
         + 19 * v316 * (unsigned __int128)(unsigned __int64)v328;
    v331 = (unsigned __int64)v317 * (unsigned __int128)(unsigned __int64)v324
         + (unsigned __int64)v328 * (unsigned __int128)(unsigned __int64)v318
         + 19 * v316 * (unsigned __int128)v316
         + (unsigned __int64)(v329 >> 51);
    v332 = __PAIR128__(
             v325
           + (((unsigned __int64)(19 * v317) * (unsigned __int128)(unsigned __int64)v317 + (unsigned __int64)v326) >> 64),
             19 * v317 * v317 + v326)
         + 19 * v316 * (unsigned __int128)(unsigned __int64)(2 * v318)
         + (unsigned __int64)(v330 >> 51);
    v333 = (unsigned __int64)v317 * (unsigned __int128)(unsigned __int64)v328
         + (unsigned __int64)v318 * (unsigned __int128)(unsigned __int64)v318
         + v316 * (unsigned __int128)(unsigned __int64)v324
         + (unsigned __int64)(v331 >> 51);
    v334 = (v329 & 0x7FFFFFFFFFFFFLL) + (v332 >> 51);
    v316 = v333 & 0x7FFFFFFFFFFFFLL;
    v317 = (v331 & 0x7FFFFFFFFFFFFLL) + (v334 >> 51);
    v318 = v334 & 0x7FFFFFFFFFFFFLL;
    v335 = (v330 & 0x7FFFFFFFFFFFFLL) + 19 * (v333 >> 51);
    v319 = (v332 & 0x7FFFFFFFFFFFFLL) + (v335 >> 51);
    v320 = v335 & 0x7FFFFFFFFFFFFLL;
    --v321;
  }
  while ( v321 );
  v454 = v320;
  v455 = v319;
  v456 = v318;
  v457 = v317;
  v458 = v333 & 0x7FFFFFFFFFFFFLL;
  sub_1003DA5A4(&v454, &v454, &v459);
  v336 = (unsigned __int64)v456 * (unsigned __int128)(unsigned __int64)(2 * v454)
       + (unsigned __int64)v455 * (unsigned __int128)(unsigned __int64)v455
       + (unsigned __int64)(19 * v458) * (unsigned __int128)(unsigned __int64)(2 * v457);
  v337 = (v336 & 0x7FFFFFFFFFFFFLL)
       + ((__int128)(__PAIR128__(
                       (((unsigned __int64)(2 * v454) * (unsigned __int128)(unsigned __int64)v455) >> 64)
                     + (((unsigned __int64)(19 * v457) * (unsigned __int128)(unsigned __int64)v457
                       + (unsigned __int64)(2 * v454 * v455)) >> 64),
                       19 * v457 * v457 + 2 * v454 * v455)
                   + (unsigned __int64)(19 * v458) * (unsigned __int128)(unsigned __int64)(2 * v456)
                   + (unsigned __int64)((__int128)((unsigned __int64)(19 * v457)
                                                 * (unsigned __int128)(unsigned __int64)(2 * v456)
                                                 + (unsigned __int64)v454 * (unsigned __int128)(unsigned __int64)v454
                                                 + (unsigned __int64)(19 * v458)
                                                 * (unsigned __int128)(unsigned __int64)(2 * v455)) >> 51)) >> 51);
  v338 = ((19 * v457 * 2 * v456 + v454 * v454 + 19 * v458 * 2 * v455) & 0x7FFFFFFFFFFFFLL)
       + 19
       * ((__int128)((unsigned __int64)v457 * (unsigned __int128)(unsigned __int64)(2 * v455)
                   + (unsigned __int64)v456 * (unsigned __int128)(unsigned __int64)v456
                   + (unsigned __int64)v458 * (unsigned __int128)(unsigned __int64)(2 * v454)
                   + (unsigned __int64)((__int128)((unsigned __int64)v457
                                                 * (unsigned __int128)(unsigned __int64)(2 * v454)
                                                 + (unsigned __int64)(2 * v455)
                                                 * (unsigned __int128)(unsigned __int64)v456
                                                 + (unsigned __int64)(19 * v458)
                                                 * (unsigned __int128)(unsigned __int64)v458
                                                 + (unsigned __int64)(v336 >> 51)) >> 51)) >> 51);
  v339 = (v457 * 2 * v455
        + v456 * v456
        + v458 * 2 * v454
        + ((__int128)((unsigned __int64)v457 * (unsigned __int128)(unsigned __int64)(2 * v454)
                    + (unsigned __int64)(2 * v455) * (unsigned __int128)(unsigned __int64)v456
                    + (unsigned __int64)(19 * v458) * (unsigned __int128)(unsigned __int64)v458
                    + (unsigned __int64)(v336 >> 51)) >> 51))
       & 0x7FFFFFFFFFFFFLL;
  v340 = ((v457 * 2 * v454 + 2 * v455 * v456 + 19 * v458 * v458 + (v336 >> 51)) & 0x7FFFFFFFFFFFFLL) + (v337 >> 51);
  v341 = v337 & 0x7FFFFFFFFFFFFLL;
  v342 = ((19 * v457 * v457
         + 2 * v454 * v455
         + 19 * v458 * 2 * v456
         + ((__int128)((unsigned __int64)(19 * v457) * (unsigned __int128)(unsigned __int64)(2 * v456)
                     + (unsigned __int64)v454 * (unsigned __int128)(unsigned __int64)v454
                     + (unsigned __int64)(19 * v458) * (unsigned __int128)(unsigned __int64)(2 * v455)) >> 51))
        & 0x7FFFFFFFFFFFFLL)
       + (v338 >> 51);
  v343 = v338 & 0x7FFFFFFFFFFFFLL;
  v344 = 99;
  do
  {
    v345 = ((unsigned __int64)v343 * (unsigned __int128)(unsigned __int64)v343) >> 64;
    v346 = v343 * v343;
    v347 = 2 * v343;
    v348 = ((unsigned __int64)v347 * (unsigned __int128)(unsigned __int64)v342) >> 64;
    v349 = v347 * v342;
    v350 = (unsigned __int64)v341 * (unsigned __int128)(unsigned __int64)v347
         + (unsigned __int64)v342 * (unsigned __int128)(unsigned __int64)v342;
    v351 = 2 * v342;
    v352 = v350 + 19 * v339 * (unsigned __int128)(unsigned __int64)(2 * v340);
    v353 = __PAIR128__(
             v345
           + (((unsigned __int64)(19 * v340) * (unsigned __int128)(unsigned __int64)(2 * v341) + (unsigned __int64)v346) >> 64),
             19 * v340 * 2 * v341 + v346)
         + 19 * v339 * (unsigned __int128)(unsigned __int64)v351;
    v354 = (unsigned __int64)v340 * (unsigned __int128)(unsigned __int64)v347
         + (unsigned __int64)v351 * (unsigned __int128)(unsigned __int64)v341
         + 19 * v339 * (unsigned __int128)v339
         + (unsigned __int64)(v352 >> 51);
    v355 = __PAIR128__(
             v348
           + (((unsigned __int64)(19 * v340) * (unsigned __int128)(unsigned __int64)v340 + (unsigned __int64)v349) >> 64),
             19 * v340 * v340 + v349)
         + 19 * v339 * (unsigned __int128)(unsigned __int64)(2 * v341)
         + (unsigned __int64)(v353 >> 51);
    v356 = (unsigned __int64)v340 * (unsigned __int128)(unsigned __int64)v351
         + (unsigned __int64)v341 * (unsigned __int128)(unsigned __int64)v341
         + v339 * (unsigned __int128)(unsigned __int64)v347
         + (unsigned __int64)(v354 >> 51);
    v357 = (v352 & 0x7FFFFFFFFFFFFLL) + (v355 >> 51);
    v339 = v356 & 0x7FFFFFFFFFFFFLL;
    v340 = (v354 & 0x7FFFFFFFFFFFFLL) + (v357 >> 51);
    v341 = v357 & 0x7FFFFFFFFFFFFLL;
    v358 = (v353 & 0x7FFFFFFFFFFFFLL) + 19 * (v356 >> 51);
    v342 = (v355 & 0x7FFFFFFFFFFFFLL) + (v358 >> 51);
    v343 = v358 & 0x7FFFFFFFFFFFFLL;
    --v344;
  }
  while ( v344 );
  v449 = v343;
  v450 = v342;
  v451 = v341;
  v452 = v340;
  v453 = v356 & 0x7FFFFFFFFFFFFLL;
  sub_1003DA5A4(&v454, &v449, &v454);
  v359 = v454;
  v360 = v455;
  v361 = v456;
  v362 = v457;
  v363 = 50;
  v364 = v458;
  do
  {
    v365 = ((unsigned __int64)v359 * (unsigned __int128)(unsigned __int64)v359) >> 64;
    v366 = v359 * v359;
    v367 = 2 * v359;
    v368 = ((unsigned __int64)v367 * (unsigned __int128)(unsigned __int64)v360) >> 64;
    v369 = v367 * v360;
    v370 = (unsigned __int64)v361 * (unsigned __int128)(unsigned __int64)v367
         + (unsigned __int64)v360 * (unsigned __int128)(unsigned __int64)v360;
    v371 = 2 * v360;
    v372 = v370 + (unsigned __int64)(19 * v364) * (unsigned __int128)(unsigned __int64)(2 * v362);
    v373 = __PAIR128__(
             v365
           + (((unsigned __int64)(19 * v362) * (unsigned __int128)(unsigned __int64)(2 * v361) + (unsigned __int64)v366) >> 64),
             19 * v362 * 2 * v361 + v366)
         + (unsigned __int64)(19 * v364) * (unsigned __int128)(unsigned __int64)v371;
    v374 = (unsigned __int64)v362 * (unsigned __int128)(unsigned __int64)v367
         + (unsigned __int64)v371 * (unsigned __int128)(unsigned __int64)v361
         + (unsigned __int64)(19 * v364) * (unsigned __int128)(unsigned __int64)v364
         + (unsigned __int64)(v372 >> 51);
    v375 = __PAIR128__(
             v368
           + (((unsigned __int64)(19 * v362) * (unsigned __int128)(unsigned __int64)v362 + (unsigned __int64)v369) >> 64),
             19 * v362 * v362 + v369)
         + (unsigned __int64)(19 * v364) * (unsigned __int128)(unsigned __int64)(2 * v361)
         + (unsigned __int64)(v373 >> 51);
    v376 = (unsigned __int64)v362 * (unsigned __int128)(unsigned __int64)v371
         + (unsigned __int64)v361 * (unsigned __int128)(unsigned __int64)v361
         + (unsigned __int64)v364 * (unsigned __int128)(unsigned __int64)v367
         + (unsigned __int64)(v374 >> 51);
    v377 = (v372 & 0x7FFFFFFFFFFFFLL) + (v375 >> 51);
    v364 = v376 & 0x7FFFFFFFFFFFFLL;
    v362 = (v374 & 0x7FFFFFFFFFFFFLL) + (v377 >> 51);
    v361 = v377 & 0x7FFFFFFFFFFFFLL;
    v378 = (v373 & 0x7FFFFFFFFFFFFLL) + 19 * (v376 >> 51);
    v360 = (v375 & 0x7FFFFFFFFFFFFLL) + (v378 >> 51);
    v359 = v378 & 0x7FFFFFFFFFFFFLL;
    --v363;
  }
  while ( v363 );
  v454 = v359;
  v455 = v360;
  v456 = v361;
  v457 = v362;
  v458 = v376 & 0x7FFFFFFFFFFFFLL;
  sub_1003DA5A4(&v459, &v454, &v459);
  v379 = v459;
  v380 = v460;
  v381 = v461;
  v382 = v462;
  v383 = 5;
  v384 = v463;
  do
  {
    v385 = ((unsigned __int64)v379 * (unsigned __int128)(unsigned __int64)v379) >> 64;
    v386 = v379 * v379;
    v387 = 2 * v379;
    v388 = ((unsigned __int64)v387 * (unsigned __int128)(unsigned __int64)v380) >> 64;
    v389 = v387 * v380;
    v390 = (unsigned __int64)v381 * (unsigned __int128)(unsigned __int64)v387
         + (unsigned __int64)v380 * (unsigned __int128)(unsigned __int64)v380;
    v391 = 2 * v380;
    v392 = v390 + (unsigned __int64)(19 * v384) * (unsigned __int128)(unsigned __int64)(2 * v382);
    v393 = __PAIR128__(
             v385
           + (((unsigned __int64)(19 * v382) * (unsigned __int128)(unsigned __int64)(2 * v381) + (unsigned __int64)v386) >> 64),
             19 * v382 * 2 * v381 + v386)
         + (unsigned __int64)(19 * v384) * (unsigned __int128)(unsigned __int64)v391;
    v394 = (unsigned __int64)v382 * (unsigned __int128)(unsigned __int64)v387
         + (unsigned __int64)v391 * (unsigned __int128)(unsigned __int64)v381
         + (unsigned __int64)(19 * v384) * (unsigned __int128)(unsigned __int64)v384
         + (unsigned __int64)(v392 >> 51);
    v395 = __PAIR128__(
             v388
           + (((unsigned __int64)(19 * v382) * (unsigned __int128)(unsigned __int64)v382 + (unsigned __int64)v389) >> 64),
             19 * v382 * v382 + v389)
         + (unsigned __int64)(19 * v384) * (unsigned __int128)(unsigned __int64)(2 * v381)
         + (unsigned __int64)(v393 >> 51);
    v396 = (unsigned __int64)v382 * (unsigned __int128)(unsigned __int64)v391
         + (unsigned __int64)v381 * (unsigned __int128)(unsigned __int64)v381
         + (unsigned __int64)v384 * (unsigned __int128)(unsigned __int64)v387
         + (unsigned __int64)(v394 >> 51);
    v397 = (v392 & 0x7FFFFFFFFFFFFLL) + (v395 >> 51);
    v384 = v396 & 0x7FFFFFFFFFFFFLL;
    v382 = (v394 & 0x7FFFFFFFFFFFFLL) + (v397 >> 51);
    v381 = v397 & 0x7FFFFFFFFFFFFLL;
    v398 = (v393 & 0x7FFFFFFFFFFFFLL) + 19 * (v396 >> 51);
    v380 = (v395 & 0x7FFFFFFFFFFFFLL) + (v398 >> 51);
    v379 = v398 & 0x7FFFFFFFFFFFFLL;
    --v383;
  }
  while ( v383 );
  v459 = v379;
  v460 = v380;
  v461 = v381;
  v462 = v382;
  v463 = v396 & 0x7FFFFFFFFFFFFLL;
  sub_1003DA5A4(&v438, &v459, &v464);
  sub_1003DA5A4(&v443, &v443, &v438);
  v399 = v443
       + 19 * ((v447 + ((v446 + ((v445 + ((v444 + ((unsigned __int64)(v443 + 19) >> 51)) >> 51)) >> 51)) >> 51)) >> 51);
  v400 = v444 + (v399 >> 51);
  v401 = v445 + (v400 >> 51);
  v402 = v446 + (v401 >> 51);
  v403 = v447 + (v402 >> 51);
  *(_WORD *)a1 = v399;
  *(_BYTE *)(a1 + 2) = BYTE2(v399);
  *(_BYTE *)(a1 + 3) = BYTE3(v399);
  *(_BYTE *)(a1 + 4) = BYTE4(v399);
  *(_BYTE *)(a1 + 5) = BYTE5(v399);
  *(_BYTE *)(a1 + 6) = BYTE6(v399) & 7 | (8 * v400);
  *(_BYTE *)(a1 + 7) = v400 >> 5;
  *(_BYTE *)(a1 + 8) = v400 >> 13;
  *(_BYTE *)(a1 + 9) = v400 >> 21;
  *(_BYTE *)(a1 + 10) = v400 >> 29;
  *(_BYTE *)(a1 + 11) = v400 >> 37;
  *(_BYTE *)(a1 + 12) = (v400 >> 45) & 0x3F | ((_BYTE)v401 << 6);
  *(_BYTE *)(a1 + 13) = v401 >> 2;
  *(_BYTE *)(a1 + 14) = v401 >> 10;
  *(_BYTE *)(a1 + 15) = v401 >> 18;
  *(_BYTE *)(a1 + 16) = v401 >> 26;
  *(_BYTE *)(a1 + 17) = v401 >> 34;
  *(_BYTE *)(a1 + 18) = v401 >> 42;
  *(_BYTE *)(a1 + 19) = ((v401 & 0x4000000000000LL) != 0) | (2 * v402);
  *(_BYTE *)(a1 + 20) = v402 >> 7;
  *(_BYTE *)(a1 + 21) = v402 >> 15;
  *(_BYTE *)(a1 + 22) = v402 >> 23;
  *(_BYTE *)(a1 + 23) = v402 >> 31;
  *(_BYTE *)(a1 + 24) = v402 >> 39;
  *(_BYTE *)(a1 + 25) = (v402 >> 47) & 0xF | (16 * v403);
  *(_BYTE *)(a1 + 26) = v403 >> 4;
  *(_BYTE *)(a1 + 27) = v403 >> 12;
  *(_BYTE *)(a1 + 28) = v403 >> 20;
  *(_BYTE *)(a1 + 29) = v403 >> 28;
  *(_BYTE *)(a1 + 30) = v403 >> 36;
  *(_BYTE *)(a1 + 31) = (v403 >> 44) & 0x7F;
  sub_100391B40(&v421, 32);
  return (unsigned int)sub_100391BA0(&unk_10055A273, a1, 32) != 0;
}

/* ========================================================================
 * fallback sub_1003dc06c
 * EA: 0x1003dc06c
 ======================================================================== */

__int64 __fastcall sub_1003DC06C(__int64 a1, __int64 a2, __int64 a3)
{
  int v3; // w8
  unsigned int v4; // w9
  int32x2_t v5; // d8
  unsigned __int8 v6; // w9
  unsigned __int8 v7; // w12
  int v8; // w4
  int v9; // w8
  int v10; // w12
  int v11; // w16
  int v12; // w17
  int v13; // w22
  int v14; // w12
  int v15; // w13
  int v16; // w12
  int v17; // w10
  int v18; // w12
  int v19; // w7
  int v20; // w10
  int v21; // w5
  int v22; // w12
  int v23; // w3
  int v24; // w8
  int v25; // w2
  int v26; // w12
  int v27; // w0
  int v28; // w8
  int v29; // w17
  int v30; // w12
  int v31; // w15
  int v32; // w8
  int v33; // w12
  __int32 v34; // w13
  __int32 v35; // w30
  __int32 v36; // w8
  __int32 v37; // w1
  __int32 v38; // w14
  __int32 v39; // w13
  __int32 v40; // w8
  __int32 v41; // w14
  __int32 v42; // w19
  __int32 v43; // w10
  __int32 v44; // w8
  __int32 v45; // w11
  __int32 v46; // w21
  __int32 v47; // w8
  __int32 v48; // w16
  __int32 v49; // w9
  __int32 v50; // w28
  __int32 v51; // w24
  __int32 v52; // w16
  __int32 v53; // w28
  __int32 v54; // w20
  __int32 v55; // w19
  __int32 v56; // w20
  __int32 v57; // w16
  __int32 v58; // w23
  __int32 v59; // w27
  __int32 v60; // w20
  __int32 v61; // w21
  __int32 v62; // w6
  __int32 v63; // w20
  __int32 v64; // w4
  __int32 v65; // w25
  int v66; // w4
  __int32 v67; // w22
  __int32 v68; // w1
  __int32 v69; // w14
  __int32 v70; // w13
  __int32 v71; // w30
  __int32 v72; // w6
  __int32 v73; // w23
  __int32 v74; // w9
  __int32 v75; // w24
  __int32 v76; // w26
  __int32 v77; // w2
  __int32 v78; // w27
  __int32 v79; // w16
  unsigned __int32 v80; // w1
  unsigned int v81; // w8
  int v82; // w17
  unsigned int v83; // w0
  unsigned int v84; // w9
  unsigned int v85; // w8
  unsigned int v86; // w11
  unsigned int v87; // w9
  unsigned int v88; // w10
  int8x16_t v89; // q1
  int32x2_t v90; // d3
  unsigned __int32 v91; // w12
  int8x16_t v92; // q4
  int32x4_t v93; // q2
  uint32x2_t v94; // d3
  int32x4_t v95; // q0
  int32x2_t v96; // d1
  unsigned int v97; // w9
  unsigned int v98; // w10
  int v99; // w9
  int8x16_t v100; // q1
  int8x16_t v101; // q4
  int32x2_t v102; // d3
  unsigned __int32 v103; // w12
  int32x4_t v104; // q2
  uint32x2_t v105; // d3
  int32x4_t v106; // q0
  int32x2_t v107; // d1
  unsigned __int32 v108; // w9
  unsigned __int32 v109; // w10
  int v110; // w12
  int v111; // w8
  int v112; // w10
  int v113; // w12
  int v114; // w12
  int v115; // w8
  int v116; // w10
  int v117; // w12
  int v118; // w12
  int v119; // w8
  int v120; // w10
  int v121; // w12
  int v122; // w12
  int v123; // w8
  int v124; // w10
  int v125; // w12
  int v126; // w12
  int v127; // w8
  int v128; // w10
  int v129; // w12
  int v130; // w12
  int v131; // w8
  int v132; // w10
  int v133; // w12
  int v134; // w12
  int v135; // w8
  int v136; // w10
  int v137; // w12
  int v138; // w12
  int v139; // w8
  int v140; // w10
  int v141; // w12
  __int32 v142; // w12
  __int32 v143; // w8
  int v144; // w10
  __int32 v145; // w12
  __int32 v146; // w12
  __int32 v147; // w8
  int v148; // w10
  __int32 v149; // w12
  __int32 v150; // w12
  __int32 v151; // w8
  int v152; // w10
  __int32 v153; // w12
  __int32 v154; // w12
  __int32 v155; // w8
  int v156; // w10
  __int32 v157; // w12
  __int32 v158; // w12
  __int32 v159; // w8
  int v160; // w10
  __int32 v161; // w12
  __int32 v162; // w12
  __int32 v163; // w8
  int v164; // w10
  __int32 v165; // w12
  __int32 v166; // w12
  __int32 v167; // w8
  int v168; // w10
  __int32 v169; // w12
  __int32 v170; // w12
  __int32 v171; // w8
  int v172; // w10
  __int32 v173; // w12
  int v174; // w19
  __int32 v178; // [xsp+14h] [xbp-37Ch]
  __int32 v179; // [xsp+18h] [xbp-378h]
  int v180; // [xsp+20h] [xbp-370h]
  __int32 v181; // [xsp+24h] [xbp-36Ch]
  __int32 v182; // [xsp+28h] [xbp-368h]
  int v183; // [xsp+2Ch] [xbp-364h]
  __int32 v184; // [xsp+30h] [xbp-360h]
  __int32 v185; // [xsp+34h] [xbp-35Ch]
  int v186; // [xsp+38h] [xbp-358h]
  int v187; // [xsp+3Ch] [xbp-354h]
  int v188; // [xsp+40h] [xbp-350h]
  int v189; // [xsp+44h] [xbp-34Ch]
  int v190; // [xsp+48h] [xbp-348h]
  int v191; // [xsp+4Ch] [xbp-344h]
  int v192; // [xsp+50h] [xbp-340h]
  int v193; // [xsp+54h] [xbp-33Ch]
  int v194; // [xsp+58h] [xbp-338h]
  int v195; // [xsp+5Ch] [xbp-334h]
  int v196; // [xsp+60h] [xbp-330h]
  int v197; // [xsp+64h] [xbp-32Ch]
  int v198; // [xsp+68h] [xbp-328h]
  int v199; // [xsp+68h] [xbp-328h]
  int v200; // [xsp+6Ch] [xbp-324h]
  int v201; // [xsp+70h] [xbp-320h]
  int v202; // [xsp+74h] [xbp-31Ch]
  int v203; // [xsp+78h] [xbp-318h]
  int v204; // [xsp+7Ch] [xbp-314h]
  int v205; // [xsp+80h] [xbp-310h]
  __int32 v206; // [xsp+80h] [xbp-310h]
  int v207; // [xsp+84h] [xbp-30Ch]
  __int32 v208; // [xsp+84h] [xbp-30Ch]
  __int64 v209; // [xsp+88h] [xbp-308h]
  int v210; // [xsp+90h] [xbp-300h]
  int v211; // [xsp+94h] [xbp-2FCh]
  int v212; // [xsp+98h] [xbp-2F8h]
  int v213; // [xsp+9Ch] [xbp-2F4h]
  int v214; // [xsp+A0h] [xbp-2F0h]
  int v215; // [xsp+A4h] [xbp-2ECh]
  int v216; // [xsp+A8h] [xbp-2E8h]
  int v217; // [xsp+ACh] [xbp-2E4h]
  int32x4_t v218; // [xsp+B0h] [xbp-2E0h]
  unsigned int v219; // [xsp+CCh] [xbp-2C4h]
  __int32 v220; // [xsp+D0h] [xbp-2C0h] BYREF
  uint32x4_t v221; // [xsp+D4h] [xbp-2BCh]
  __int64 v222; // [xsp+E4h] [xbp-2ACh]
  int v223; // [xsp+ECh] [xbp-2A4h]
  int v224; // [xsp+F0h] [xbp-2A0h]
  __int32 v225; // [xsp+F4h] [xbp-29Ch]
  uint32x4_t v226; // [xsp+F8h] [xbp-298h]
  uint32x2_t v227; // [xsp+108h] [xbp-288h]
  _BYTE v228[32]; // [xsp+110h] [xbp-280h] BYREF
  _BYTE v229[32]; // [xsp+130h] [xbp-260h]
  _BYTE v230[64]; // [xsp+150h] [xbp-240h] BYREF
  _BYTE v231[32]; // [xsp+190h] [xbp-200h] BYREF
  _BYTE v232[32]; // [xsp+1B0h] [xbp-1E0h]
  int32x4_t v233[4]; // [xsp+1D0h] [xbp-1C0h] BYREF
  _BYTE v234[64]; // [xsp+210h] [xbp-180h] BYREF
  _OWORD v235[4]; // [xsp+250h] [xbp-140h] BYREF
  _OWORD v236[4]; // [xsp+290h] [xbp-100h] BYREF
  _BYTE v237[72]; // [xsp+2D0h] [xbp-C0h] BYREF

  sub_1003DF33C(v235, a2, 1, 0);
  v3 = 0;
  *(_OWORD *)v234 = xmmword_100562070;
  memset(&v234[16], 0, 48);
  memset(v233, 0, sizeof(v233));
  *(_OWORD *)v232 = v235[2];
  *(_OWORD *)&v232[16] = v235[3];
  *(_OWORD *)v231 = v235[0];
  *(_OWORD *)&v231[16] = v235[1];
  *(_OWORD *)v230 = xmmword_100562070;
  memset(&v230[16], 0, 48);
  v4 = 447;
  v218 = vdupq_n_s32(0x1FFFFFFEu);
  v5 = vdup_n_s32(0x1FFFFFFEu);
  do
  {
    v219 = v4;
    v6 = *(_BYTE *)(a3 + ((unsigned __int64)v4 >> 3));
    v7 = v6 & 0xFC;
    if ( v219 == 447 )
      v6 = -1;
    if ( v219 < 8 )
      v6 = v7;
    v217 = -((v6 >> (v219 & 7)) & 1);
    v8 = v3 ^ v217;
    v9 = (v3 ^ v217) & (*(_DWORD *)v231 ^ *(_DWORD *)v234);
    v10 = (*(_DWORD *)&v231[4] ^ *(_DWORD *)&v234[4]) & v8;
    *(_DWORD *)v231 ^= v9;
    *(_DWORD *)&v231[4] ^= v10;
    v11 = (*(_DWORD *)&v231[8] ^ *(_DWORD *)&v234[8]) & v8;
    v12 = (*(_DWORD *)&v231[12] ^ *(_DWORD *)&v234[12]) & v8;
    *(_DWORD *)&v231[8] ^= v11;
    *(_DWORD *)&v231[12] ^= v12;
    v13 = v9 ^ *(_DWORD *)v234;
    v210 = v10 ^ *(_DWORD *)&v234[4];
    LODWORD(v209) = v11 ^ *(_DWORD *)&v234[8];
    HIDWORD(v209) = v12 ^ *(_DWORD *)&v234[12];
    v14 = (*(_DWORD *)&v231[16] ^ *(_DWORD *)&v234[16]) & v8;
    v15 = (*(_DWORD *)&v231[20] ^ *(_DWORD *)&v234[20]) & v8;
    *(_DWORD *)&v231[16] ^= v14;
    *(_DWORD *)&v231[20] ^= v15;
    v205 = v14 ^ *(_DWORD *)&v234[16];
    v207 = v15 ^ *(_DWORD *)&v234[20];
    v16 = (*(_DWORD *)&v231[24] ^ *(_DWORD *)&v234[24]) & v8;
    v204 = v16 ^ *(_DWORD *)&v234[24];
    v17 = (*(_DWORD *)&v231[28] ^ *(_DWORD *)&v234[28]) & v8;
    v203 = v17 ^ *(_DWORD *)&v234[28];
    *(_DWORD *)&v231[24] ^= v16;
    *(_DWORD *)&v231[28] ^= v17;
    v18 = (*(_DWORD *)v232 ^ *(_DWORD *)&v234[32]) & v8;
    v19 = v18 ^ *(_DWORD *)&v234[32];
    v20 = (*(_DWORD *)&v232[4] ^ *(_DWORD *)&v234[36]) & v8;
    v21 = v20 ^ *(_DWORD *)&v234[36];
    *(_DWORD *)v232 ^= v18;
    *(_DWORD *)&v232[4] ^= v20;
    v22 = (*(_DWORD *)&v232[8] ^ *(_DWORD *)&v234[40]) & v8;
    v23 = v22 ^ *(_DWORD *)&v234[40];
    v216 = v22 ^ *(_DWORD *)&v232[8];
    v24 = (*(_DWORD *)&v232[12] ^ *(_DWORD *)&v234[44]) & v8;
    v25 = v24 ^ *(_DWORD *)&v234[44];
    v215 = v24 ^ *(_DWORD *)&v232[12];
    v26 = (*(_DWORD *)&v232[16] ^ *(_DWORD *)&v234[48]) & v8;
    v27 = v26 ^ *(_DWORD *)&v234[48];
    v214 = v26 ^ *(_DWORD *)&v232[16];
    v28 = (*(_DWORD *)&v232[20] ^ *(_DWORD *)&v234[52]) & v8;
    v29 = v28 ^ *(_DWORD *)&v234[52];
    v213 = v28 ^ *(_DWORD *)&v232[20];
    v30 = (*(_DWORD *)&v232[24] ^ *(_DWORD *)&v234[56]) & v8;
    v31 = v30 ^ *(_DWORD *)&v234[56];
    v212 = v30 ^ *(_DWORD *)&v232[24];
    v32 = (*(_DWORD *)&v232[28] ^ *(_DWORD *)&v234[60]) & v8;
    v33 = v32 ^ *(_DWORD *)&v234[60];
    v211 = v32 ^ *(_DWORD *)&v232[28];
    v34 = (*(_DWORD *)v230 ^ v233[0].i32[0]) & v8;
    v35 = v34 ^ v233[0].i32[0];
    v202 = v34 ^ *(_DWORD *)v230;
    v36 = (*(_DWORD *)&v230[4] ^ v233[0].i32[1]) & v8;
    v37 = v36 ^ v233[0].i32[1];
    v201 = v36 ^ *(_DWORD *)&v230[4];
    v38 = (*(_DWORD *)&v230[8] ^ v233[0].i32[2]) & v8;
    v39 = v38 ^ v233[0].i32[2];
    v200 = v38 ^ *(_DWORD *)&v230[8];
    v40 = (*(_DWORD *)&v230[12] ^ v233[0].i32[3]) & v8;
    v41 = v40 ^ v233[0].i32[3];
    v198 = v40 ^ *(_DWORD *)&v230[12];
    v42 = (*(_DWORD *)&v230[16] ^ v233[1].i32[0]) & v8;
    v43 = v42 ^ v233[1].i32[0];
    v197 = v42 ^ *(_DWORD *)&v230[16];
    v44 = (*(_DWORD *)&v230[20] ^ v233[1].i32[1]) & v8;
    v45 = v44 ^ v233[1].i32[1];
    v196 = v44 ^ *(_DWORD *)&v230[20];
    v46 = (*(_DWORD *)&v230[24] ^ v233[1].i32[2]) & v8;
    v47 = v46 ^ v233[1].i32[2];
    v193 = v46 ^ *(_DWORD *)&v230[24];
    v48 = (*(_DWORD *)&v230[28] ^ v233[1].i32[3]) & v8;
    v49 = v48 ^ v233[1].i32[3];
    v192 = v48 ^ *(_DWORD *)&v230[28];
    v50 = (*(_DWORD *)&v230[32] ^ v233[2].i32[0]) & v8;
    v51 = v50 ^ v233[2].i32[0];
    v189 = v50 ^ *(_DWORD *)&v230[32];
    v52 = (*(_DWORD *)&v230[36] ^ v233[2].i32[1]) & v8;
    v53 = v52 ^ v233[2].i32[1];
    v188 = v52 ^ *(_DWORD *)&v230[36];
    v54 = (*(_DWORD *)&v230[40] ^ v233[2].i32[2]) & v8;
    v55 = v54 ^ v233[2].i32[2];
    v195 = v54 ^ *(_DWORD *)&v230[40];
    v56 = (*(_DWORD *)&v230[44] ^ v233[2].i32[3]) & v8;
    v57 = v56 ^ v233[2].i32[3];
    v194 = v56 ^ *(_DWORD *)&v230[44];
    v58 = (*(_DWORD *)&v230[48] ^ v233[3].i32[0]) & v8;
    v59 = v58 ^ v233[3].i32[0];
    v191 = v58 ^ *(_DWORD *)&v230[48];
    v60 = (*(_DWORD *)&v230[52] ^ v233[3].i32[1]) & v8;
    v61 = v60 ^ v233[3].i32[1];
    v190 = v60 ^ *(_DWORD *)&v230[52];
    v62 = (*(_DWORD *)&v230[56] ^ v233[3].i32[2]) & v8;
    v63 = v62 ^ v233[3].i32[2];
    v187 = v62 ^ *(_DWORD *)&v230[56];
    v64 = (*(_DWORD *)&v230[60] ^ v233[3].i32[3]) & v8;
    v65 = v64 ^ v233[3].i32[3];
    v186 = v64 ^ *(_DWORD *)&v230[60];
    v66 = v13;
    v185 = v35 + v13;
    v67 = v13 - v35;
    *(_DWORD *)v234 = v66;
    *(_DWORD *)&v234[4] = v210;
    v184 = v210 - v37;
    v180 = v39 + v209;
    v181 = v37 + v210;
    v183 = v209 - v39;
    *(_QWORD *)&v234[8] = v209;
    v68 = v41 + HIDWORD(v209);
    LODWORD(v209) = HIDWORD(v209) - v41;
    v69 = v43 + v205;
    v182 = v205 - v43;
    *(_DWORD *)&v234[16] = v205;
    *(_QWORD *)&v234[20] = __PAIR64__(v204, v207);
    v178 = v45 + v207;
    v179 = v68;
    v70 = v207 - v45;
    v71 = v47 + v204;
    v208 = v204 - v47;
    *(_DWORD *)&v234[28] = v203;
    v72 = v49 + v203;
    v206 = v203 - v49;
    v73 = v51 + v19;
    v74 = v19 - v51;
    *(_DWORD *)&v234[32] = v19;
    *(_DWORD *)&v234[36] = v21;
    *(_DWORD *)&v234[40] = v23;
    *(_DWORD *)&v234[44] = v25;
    v75 = v57 + v25;
    v76 = v25 - v57;
    v77 = v59 + v27;
    v78 = v27 - v59;
    *(_DWORD *)&v234[48] = v27;
    *(_DWORD *)&v234[52] = v29;
    v79 = v61 + v29;
    v80 = v29 - v61;
    *(_QWORD *)&v234[56] = __PAIR64__(v33, v31);
    v81 = v33 - v65 + 536870910;
    v82 = (v81 >> 28) + ((v67 + 536870910) & 0xFFFFFFF);
    *(_DWORD *)v230 = v202;
    *(_DWORD *)&v230[4] = v201;
    *(_DWORD *)&v230[8] = v200;
    *(_DWORD *)&v230[12] = v198;
    v199 = *(_DWORD *)&v231[12] - v198;
    *(_DWORD *)&v230[16] = v197;
    *(_DWORD *)&v230[20] = v196;
    *(_DWORD *)&v230[24] = v193;
    *(_DWORD *)&v230[28] = v192;
    *(_DWORD *)&v230[32] = v189;
    *(_DWORD *)&v230[36] = v188;
    *(_DWORD *)v228 = v185;
    *(_DWORD *)&v228[4] = v181;
    *(_DWORD *)&v228[8] = v180;
    *(_DWORD *)&v228[12] = v179;
    *(_DWORD *)&v228[16] = v69;
    *(_QWORD *)&v228[20] = __PAIR64__(v71, v178);
    *(_DWORD *)&v228[28] = v72;
    *(_DWORD *)v229 = v73;
    *(_DWORD *)&v229[4] = v53 + v21;
    *(_DWORD *)&v229[8] = v55 + v23;
    *(_DWORD *)&v229[12] = v75;
    *(_QWORD *)&v229[16] = __PAIR64__(v79, v77);
    v80 += 536870910;
    v83 = v74 + (v81 >> 28) + 536870908;
    *(_DWORD *)&v230[40] = v195;
    *(_DWORD *)&v232[8] = v216;
    *(_DWORD *)&v232[12] = v215;
    *(_DWORD *)&v230[44] = v194;
    *(_DWORD *)&v230[48] = v191;
    *(_DWORD *)&v232[16] = v214;
    *(_DWORD *)&v232[20] = v213;
    *(_DWORD *)&v230[52] = v190;
    *(_DWORD *)&v230[56] = v187;
    *(_QWORD *)&v232[24] = __PAIR64__(v211, v212);
    *(_DWORD *)&v230[60] = v186;
    *(_DWORD *)&v229[24] = v63 + v31;
    *(_DWORD *)&v229[28] = v65 + v33;
    v227.i32[1] = (v81 & 0xFFFFFFF) + ((unsigned int)(v31 - v63 + 536870910) >> 28);
    v227.i32[0] = ((v31 - v63 + 536870910) & 0xFFFFFFF) + (v80 >> 28);
    v226.i32[3] = (v80 & 0xFFFFFFF) + ((unsigned int)(v78 + 536870910) >> 28);
    v226.i32[1] = ((v76 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v23 - v55 + 536870910) >> 28);
    v226.i32[2] = ((v78 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v76 + 536870910) >> 28);
    v225 = ((v21 - v53 + 536870910) & 0xFFFFFFF) + (v83 >> 28);
    v226.i32[0] = ((v23 - v55 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v21 - v53 + 536870910) >> 28);
    v223 = ((v206 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v208 + 536870910) >> 28);
    v224 = (v83 & 0xFFFFFFF) + ((unsigned int)(v206 + 536870910) >> 28);
    LODWORD(v222) = ((v70 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v182 + 536870910) >> 28);
    HIDWORD(v222) = ((v208 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v70 + 536870910) >> 28);
    v221.i32[2] = ((v209 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v183 + 536870910) >> 28);
    v221.i32[3] = ((v182 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v209 + 536870910) >> 28);
    v221.i32[0] = ((v184 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v67 + 536870910) >> 28);
    v221.i32[1] = ((v183 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v184 + 536870910) >> 28);
    v84 = v212 - v187 + 536870910;
    v85 = v211 - v186 + 536870910;
    v220 = v82;
    v233[3].i32[3] = (v85 & 0xFFFFFFF) + (v84 >> 28);
    v233[3].i32[2] = (v84 & 0xFFFFFFF) + ((unsigned int)(v213 - v190 + 536870910) >> 28);
    v233[3].i32[1] = ((v213 - v190 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v214 - v191 + 536870910) >> 28);
    v233[3].i32[0] = ((v214 - v191 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v215 - v194 + 536870910) >> 28);
    v86 = *(_DWORD *)&v232[4] - v188 + 536870910;
    v233[2].i32[3] = ((v215 - v194 + 536870910) & 0xFFFFFFF) + ((unsigned int)(v216 - v195 + 536870910) >> 28);
    v233[2].i32[2] = ((v216 - v195 + 536870910) & 0xFFFFFFF) + (v86 >> 28);
    v85 >>= 28;
    v87 = v85 + *(_DWORD *)v232 - v189 + 536870908;
    v233[2].i32[1] = (v86 & 0xFFFFFFF) + (v87 >> 28);
    v233[2].i32[0] = (v87 & 0xFFFFFFF) + ((unsigned int)(*(_DWORD *)&v231[28] - v192 + 536870910) >> 28);
    v233[1].i32[3] = ((*(_DWORD *)&v231[28] - v192 + 536870910) & 0xFFFFFFF)
                   + ((unsigned int)(*(_DWORD *)&v231[24] - v193 + 536870910) >> 28);
    v233[1].i32[2] = ((*(_DWORD *)&v231[24] - v193 + 536870910) & 0xFFFFFFF)
                   + ((unsigned int)(*(_DWORD *)&v231[20] - v196 + 536870910) >> 28);
    v88 = *(_DWORD *)&v231[16] - v197 + 536870910;
    v233[1].i32[1] = ((*(_DWORD *)&v231[20] - v196 + 536870910) & 0xFFFFFFF) + (v88 >> 28);
    v233[1].i32[0] = (v88 & 0xFFFFFFF) + ((unsigned int)(v199 + 536870910) >> 28);
    v233[0].i32[3] = ((v199 + 536870910) & 0xFFFFFFF) + ((unsigned int)(*(_DWORD *)&v231[8] - v200 + 536870910) >> 28);
    v233[0].i32[2] = ((*(_DWORD *)&v231[8] - v200 + 536870910) & 0xFFFFFFF)
                   + ((unsigned int)(*(_DWORD *)&v231[4] - v201 + 536870910) >> 28);
    v233[0].i32[1] = ((*(_DWORD *)&v231[4] - v201 + 536870910) & 0xFFFFFFF)
                   + ((unsigned int)(*(_DWORD *)v231 - v202 + 536870910) >> 28);
    v233[0].i32[0] = ((*(_DWORD *)v231 - v202 + 536870910) & 0xFFFFFFF) + v85;
    sub_1003DA7B0(v234, v228, v233);
    v233[0] = vaddq_s32(*(int32x4_t *)v231, *(int32x4_t *)v230);
    v233[1] = vaddq_s32(*(int32x4_t *)&v231[16], *(int32x4_t *)&v230[16]);
    v233[2] = vaddq_s32(*(int32x4_t *)v232, *(int32x4_t *)&v230[32]);
    v233[3] = vaddq_s32(*(int32x4_t *)&v232[16], *(int32x4_t *)&v230[48]);
    sub_1003DA7B0(v231, &v220, v233);
    v89.i64[1] = *(_QWORD *)&v231[12];
    v90 = vadd_s32(vsub_s32(*(int32x2_t *)&v234[56], *(int32x2_t *)&v232[24]), v5);
    v91 = v90.u32[1];
    v92.i64[1] = v218.i64[1];
    v93 = vaddq_s32(vsubq_s32(*(int32x4_t *)&v234[40], *(int32x4_t *)&v232[8]), v218);
    *(int32x2_t *)v92.i8 = vzip1_s32(vdup_laneq_s32(v93, 3), v90);
    v94 = vsra_n_u32((uint32x2_t)(*(_QWORD *)&v90 & 0xFFFFFFF0FFFFFFFLL), *(uint32x2_t *)v92.i8, 0x1Cu);
    v92.i32[0] = *(_DWORD *)&v234[36] - *(_DWORD *)&v232[4] + 536870910;
    v95 = vaddq_s32(vsubq_s32(*(int32x4_t *)&v234[4], *(int32x4_t *)&v231[4]), v218);
    v96 = vadd_s32(vsub_s32(*(int32x2_t *)&v234[20], *(int32x2_t *)&v231[20]), v5);
    v97 = *(_DWORD *)&v234[28] - *(_DWORD *)&v231[28] + 536870910;
    v98 = *(_DWORD *)&v234[32] - *(_DWORD *)v232 + (v91 >> 28) + 536870908;
    *(uint32x2_t *)&v230[56] = v94;
    *(uint32x4_t *)&v230[40] = vsraq_n_u32(
                                 (uint32x4_t)(*(_OWORD *)&v93 & __PAIR128__(0xFFFFFFF0FFFFFFFLL, 0xFFFFFFF0FFFFFFFLL)),
                                 (uint32x4_t)vextq_s8(vextq_s8(v92, v92, 4u), (int8x16_t)v93, 0xCu),
                                 0x1Cu);
    *(_DWORD *)&v230[36] = (v92.i32[0] & 0xFFFFFFF) + (v98 >> 28);
    *(_DWORD *)&v230[32] = (v98 & 0xFFFFFFF) + (v97 >> 28);
    v99 = (v97 & 0xFFFFFFF) + ((unsigned __int32)v96.i32[1] >> 28);
    *(uint32x2_t *)v89.i8 = vsra_n_u32(
                              (uint32x2_t)(*(_QWORD *)&v96 & 0xFFFFFFF0FFFFFFFLL),
                              (uint32x2_t)vzip1_s32(vdup_laneq_s32(v95, 3), v96),
                              0x1Cu);
    *(_QWORD *)&v230[20] = v89.i64[0];
    v89.i32[0] = *(_DWORD *)v234 - *(_DWORD *)v231 + 536870910;
    *(uint32x4_t *)&v230[4] = vsraq_n_u32(
                                (uint32x4_t)(*(_OWORD *)&v95 & __PAIR128__(0xFFFFFFF0FFFFFFFLL, 0xFFFFFFF0FFFFFFFLL)),
                                (uint32x4_t)vextq_s8(vextq_s8(v89, v89, 4u), (int8x16_t)v95, 0xCu),
                                0x1Cu);
    *(_DWORD *)&v230[28] = v99;
    *(_DWORD *)v230 = (v91 >> 28) + (v89.i32[0] & 0xFFFFFFF);
    sub_1003DAD5C(v233, v230);
    sub_1003DA7B0(v230, v235, v233);
    v233[0] = vaddq_s32(*(int32x4_t *)v231, *(int32x4_t *)v234);
    v233[1] = vaddq_s32(*(int32x4_t *)&v231[16], *(int32x4_t *)&v234[16]);
    v233[2] = vaddq_s32(*(int32x4_t *)v232, *(int32x4_t *)&v234[32]);
    v233[3] = vaddq_s32(*(int32x4_t *)&v232[16], *(int32x4_t *)&v234[48]);
    sub_1003DAD5C(v231, v233);
    sub_1003DAD5C(v233, v228);
    sub_1003DAD5C(v228, &v220);
    sub_1003DA7B0(v234, v233, v228);
    v100.i64[1] = *(_QWORD *)&v228[12];
    v101.i64[1] = *(_QWORD *)&v229[16];
    v102 = vadd_s32(vsub_s32((int32x2_t)v233[3].u64[1], *(int32x2_t *)&v229[24]), v5);
    v103 = v102.u32[1];
    v104 = vaddq_s32(vsubq_s32(*(int32x4_t *)((char *)&v233[2] + 8), *(int32x4_t *)&v229[8]), v218);
    *(int32x2_t *)v101.i8 = vzip1_s32(vdup_laneq_s32(v104, 3), v102);
    v105 = vsra_n_u32((uint32x2_t)(*(_QWORD *)&v102 & 0xFFFFFFF0FFFFFFFLL), *(uint32x2_t *)v101.i8, 0x1Cu);
    v101.i32[0] = v233[2].i32[1] - *(_DWORD *)&v229[4] + 536870910;
    v106 = vaddq_s32(vsubq_s32(*(int32x4_t *)((char *)v233 + 4), *(int32x4_t *)&v228[4]), v218);
    v107 = vadd_s32(vsub_s32(*(int32x2_t *)((char *)v233[1].i64 + 4), *(int32x2_t *)&v228[20]), v5);
    v108 = v233[1].i32[3] - *(_DWORD *)&v228[28] + 536870910;
    v109 = v233[2].i32[0] - *(_DWORD *)v229 + (v103 >> 28) + 536870908;
    v227 = v105;
    v226 = vsraq_n_u32(
             (uint32x4_t)(*(_OWORD *)&v104 & __PAIR128__(0xFFFFFFF0FFFFFFFLL, 0xFFFFFFF0FFFFFFFLL)),
             (uint32x4_t)vextq_s8(vextq_s8(v101, v101, 4u), (int8x16_t)v104, 0xCu),
             0x1Cu);
    v224 = (v109 & 0xFFFFFFF) + (v108 >> 28);
    v225 = (v101.i32[0] & 0xFFFFFFF) + (v109 >> 28);
    v223 = (v108 & 0xFFFFFFF) + ((unsigned __int32)v107.i32[1] >> 28);
    *(uint32x2_t *)v100.i8 = vsra_n_u32(
                               (uint32x2_t)(*(_QWORD *)&v107 & 0xFFFFFFF0FFFFFFFLL),
                               (uint32x2_t)vzip1_s32(vdup_laneq_s32(v106, 3), v107),
                               0x1Cu);
    v222 = v100.i64[0];
    v100.i32[0] = v233[0].i32[0] - *(_DWORD *)v228 + 536870910;
    v221 = vsraq_n_u32(
             (uint32x4_t)(*(_OWORD *)&v106 & __PAIR128__(0xFFFFFFF0FFFFFFFLL, 0xFFFFFFF0FFFFFFFLL)),
             (uint32x4_t)vextq_s8(vextq_s8(v100, v100, 4u), (int8x16_t)v106, 0xCu),
             0x1Cu);
    v220 = (v103 >> 28) + (v100.i32[0] & 0xFFFFFFF);
    sub_1003DAC24(v228, &v220, 39081);
    *(int32x4_t *)v228 = vaddq_s32(v233[0], *(int32x4_t *)v228);
    *(int32x4_t *)&v228[16] = vaddq_s32(v233[1], *(int32x4_t *)&v228[16]);
    *(int32x4_t *)v229 = vaddq_s32(v233[2], *(int32x4_t *)v229);
    *(int32x4_t *)&v229[16] = vaddq_s32(v233[3], *(int32x4_t *)&v229[16]);
    sub_1003DA7B0(v233, &v220, v228);
    v4 = v219 - 1;
    v3 = v217;
  }
  while ( v219 );
  v110 = (*(_DWORD *)v231 ^ *(_DWORD *)v234) & v217;
  v111 = v110 ^ *(_DWORD *)v234;
  v112 = v110 ^ *(_DWORD *)v231;
  v113 = (*(_DWORD *)&v231[4] ^ *(_DWORD *)&v234[4]) & v217;
  *(_DWORD *)v234 = v111;
  *(_DWORD *)&v234[4] ^= v113;
  *(_DWORD *)v231 = v112;
  *(_DWORD *)&v231[4] ^= v113;
  v114 = (*(_DWORD *)&v231[8] ^ *(_DWORD *)&v234[8]) & v217;
  v115 = v114 ^ *(_DWORD *)&v234[8];
  v116 = v114 ^ *(_DWORD *)&v231[8];
  v117 = (*(_DWORD *)&v231[12] ^ *(_DWORD *)&v234[12]) & v217;
  *(_DWORD *)&v234[8] = v115;
  *(_DWORD *)&v234[12] ^= v117;
  *(_DWORD *)&v231[8] = v116;
  *(_DWORD *)&v231[12] ^= v117;
  v118 = (*(_DWORD *)&v231[16] ^ *(_DWORD *)&v234[16]) & v217;
  v119 = v118 ^ *(_DWORD *)&v234[16];
  v120 = v118 ^ *(_DWORD *)&v231[16];
  v121 = (*(_DWORD *)&v231[20] ^ *(_DWORD *)&v234[20]) & v217;
  *(_DWORD *)&v234[16] = v119;
  *(_DWORD *)&v234[20] ^= v121;
  *(_DWORD *)&v231[16] = v120;
  *(_DWORD *)&v231[20] ^= v121;
  v122 = (*(_DWORD *)&v231[24] ^ *(_DWORD *)&v234[24]) & v217;
  v123 = v122 ^ *(_DWORD *)&v234[24];
  v124 = v122 ^ *(_DWORD *)&v231[24];
  v125 = (*(_DWORD *)&v231[28] ^ *(_DWORD *)&v234[28]) & v217;
  *(_DWORD *)&v234[24] = v123;
  *(_DWORD *)&v234[28] ^= v125;
  *(_DWORD *)&v231[24] = v124;
  *(_DWORD *)&v231[28] ^= v125;
  v126 = (*(_DWORD *)v232 ^ *(_DWORD *)&v234[32]) & v217;
  v127 = v126 ^ *(_DWORD *)&v234[32];
  v128 = v126 ^ *(_DWORD *)v232;
  v129 = (*(_DWORD *)&v232[4] ^ *(_DWORD *)&v234[36]) & v217;
  *(_DWORD *)&v234[32] = v127;
  *(_DWORD *)&v234[36] ^= v129;
  *(_DWORD *)v232 = v128;
  *(_DWORD *)&v232[4] ^= v129;
  v130 = (*(_DWORD *)&v232[8] ^ *(_DWORD *)&v234[40]) & v217;
  v131 = v130 ^ *(_DWORD *)&v234[40];
  v132 = v130 ^ *(_DWORD *)&v232[8];
  v133 = (*(_DWORD *)&v232[12] ^ *(_DWORD *)&v234[44]) & v217;
  *(_DWORD *)&v234[40] = v131;
  *(_DWORD *)&v234[44] ^= v133;
  *(_DWORD *)&v232[8] = v132;
  *(_DWORD *)&v232[12] ^= v133;
  v134 = (*(_DWORD *)&v232[16] ^ *(_DWORD *)&v234[48]) & v217;
  v135 = v134 ^ *(_DWORD *)&v234[48];
  v136 = v134 ^ *(_DWORD *)&v232[16];
  v137 = (*(_DWORD *)&v232[20] ^ *(_DWORD *)&v234[52]) & v217;
  *(_DWORD *)&v234[48] = v135;
  *(_DWORD *)&v234[52] ^= v137;
  *(_DWORD *)&v232[16] = v136;
  *(_DWORD *)&v232[20] ^= v137;
  v138 = (*(_DWORD *)&v232[24] ^ *(_DWORD *)&v234[56]) & v217;
  v139 = v138 ^ *(_DWORD *)&v234[56];
  v140 = v138 ^ *(_DWORD *)&v232[24];
  v141 = (*(_DWORD *)&v232[28] ^ *(_DWORD *)&v234[60]) & v217;
  *(_DWORD *)&v234[56] = v139;
  *(_DWORD *)&v234[60] ^= v141;
  *(_DWORD *)&v232[24] = v140;
  *(_DWORD *)&v232[28] ^= v141;
  v142 = (*(_DWORD *)v230 ^ v233[0].i32[0]) & v217;
  v143 = v142 ^ v233[0].i32[0];
  v144 = v142 ^ *(_DWORD *)v230;
  v145 = (*(_DWORD *)&v230[4] ^ v233[0].i32[1]) & v217;
  v233[0].i32[0] = v143;
  v233[0].i32[1] ^= v145;
  *(_DWORD *)v230 = v144;
  *(_DWORD *)&v230[4] ^= v145;
  v146 = (*(_DWORD *)&v230[8] ^ v233[0].i32[2]) & v217;
  v147 = v146 ^ v233[0].i32[2];
  v148 = v146 ^ *(_DWORD *)&v230[8];
  v149 = (*(_DWORD *)&v230[12] ^ v233[0].i32[3]) & v217;
  v233[0].i32[2] = v147;
  v233[0].i32[3] ^= v149;
  *(_DWORD *)&v230[8] = v148;
  *(_DWORD *)&v230[12] ^= v149;
  v150 = (*(_DWORD *)&v230[16] ^ v233[1].i32[0]) & v217;
  v151 = v150 ^ v233[1].i32[0];
  v152 = v150 ^ *(_DWORD *)&v230[16];
  v153 = (*(_DWORD *)&v230[20] ^ v233[1].i32[1]) & v217;
  v233[1].i32[0] = v151;
  v233[1].i32[1] ^= v153;
  *(_DWORD *)&v230[16] = v152;
  *(_DWORD *)&v230[20] ^= v153;
  v154 = (*(_DWORD *)&v230[24] ^ v233[1].i32[2]) & v217;
  v155 = v154 ^ v233[1].i32[2];
  v156 = v154 ^ *(_DWORD *)&v230[24];
  v157 = (*(_DWORD *)&v230[28] ^ v233[1].i32[3]) & v217;
  v233[1].i32[2] = v155;
  v233[1].i32[3] ^= v157;
  *(_DWORD *)&v230[24] = v156;
  *(_DWORD *)&v230[28] ^= v157;
  v158 = (*(_DWORD *)&v230[32] ^ v233[2].i32[0]) & v217;
  v159 = v158 ^ v233[2].i32[0];
  v160 = v158 ^ *(_DWORD *)&v230[32];
  v161 = (*(_DWORD *)&v230[36] ^ v233[2].i32[1]) & v217;
  v233[2].i32[0] = v159;
  v233[2].i32[1] ^= v161;
  *(_DWORD *)&v230[32] = v160;
  *(_DWORD *)&v230[36] ^= v161;
  v162 = (*(_DWORD *)&v230[40] ^ v233[2].i32[2]) & v217;
  v163 = v162 ^ v233[2].i32[2];
  v164 = v162 ^ *(_DWORD *)&v230[40];
  v165 = (*(_DWORD *)&v230[44] ^ v233[2].i32[3]) & v217;
  v233[2].i32[2] = v163;
  v233[2].i32[3] ^= v165;
  *(_DWORD *)&v230[40] = v164;
  *(_DWORD *)&v230[44] ^= v165;
  v166 = (*(_DWORD *)&v230[48] ^ v233[3].i32[0]) & v217;
  v167 = v166 ^ v233[3].i32[0];
  v168 = v166 ^ *(_DWORD *)&v230[48];
  v169 = (*(_DWORD *)&v230[52] ^ v233[3].i32[1]) & v217;
  v233[3].i32[0] = v167;
  v233[3].i32[1] ^= v169;
  *(_DWORD *)&v230[48] = v168;
  *(_DWORD *)&v230[52] ^= v169;
  v170 = (*(_DWORD *)&v230[56] ^ v233[3].i32[2]) & v217;
  v171 = v170 ^ v233[3].i32[2];
  v172 = v170 ^ *(_DWORD *)&v230[56];
  v173 = (*(_DWORD *)&v230[60] ^ v233[3].i32[3]) & v217;
  v233[3].i32[2] = v171;
  v233[3].i32[3] ^= v173;
  *(_DWORD *)&v230[56] = v172;
  *(_DWORD *)&v230[60] ^= v173;
  sub_1003DAD5C(v237, v233);
  sub_1003DF770(v236, v237);
  sub_1003DAD5C(v237, v236);
  sub_1003DA7B0(v236, v237, v233);
  v233[0] = (int32x4_t)v236[0];
  v233[1] = (int32x4_t)v236[1];
  v233[2] = (int32x4_t)v236[2];
  v233[3] = (int32x4_t)v236[3];
  sub_1003DA7B0(v235, v234, v233);
  sub_1003DED9C(a1, v235, 1);
  v174 = sub_1003DF70C(v235, &unk_100561FF0);
  sub_100391B40(v235, 64);
  sub_100391B40(v234, 64);
  sub_100391B40(v233, 64);
  sub_100391B40(v231, 64);
  sub_100391B40(v230, 64);
  sub_100391B40(v228, 64);
  sub_100391B40(&v220, 64);
  return (unsigned int)~v174;
}

/* ========================================================================
 * fallback sub_1003d1450
 * EA: 0x1003d1450
 ======================================================================== */

__int64 __fastcall sub_1003D1450(_BYTE *a1, void *a2, size_t a3, void *a4, void *a5)
{
  unsigned __int64 v10; // x24
  unsigned __int64 v11; // x11
  unsigned __int64 v12; // x20
  unsigned __int64 v13; // x9
  unsigned __int64 v14; // x6
  unsigned __int64 v15; // x15
  unsigned __int64 v16; // x28
  unsigned __int64 v17; // x19
  unsigned __int64 v18; // x16
  unsigned __int64 v19; // x21
  unsigned __int64 v20; // x7
  unsigned __int64 v21; // x5
  unsigned __int64 v22; // x17
  unsigned __int64 v23; // x22
  unsigned __int64 v24; // x1
  unsigned __int64 v25; // x26
  unsigned __int64 v26; // x2
  unsigned __int64 v27; // x3
  unsigned __int64 v28; // x27
  unsigned __int64 v29; // x0
  unsigned __int64 v30; // x10
  unsigned __int64 v31; // x30
  unsigned __int64 v32; // x14
  unsigned __int64 v33; // x12
  unsigned __int64 v34; // x11
  __int64 v35; // x13
  unsigned __int64 v36; // x4
  unsigned __int64 v37; // x23
  unsigned __int64 v38; // x9
  __int64 v39; // x10
  unsigned __int64 v40; // x15
  unsigned __int64 v41; // x16
  unsigned __int64 v42; // x15
  unsigned __int64 v43; // x21
  unsigned __int64 v44; // x17
  unsigned __int64 v45; // x14
  unsigned __int64 v46; // x17
  unsigned __int64 v47; // x0
  unsigned __int64 v48; // x7
  unsigned __int64 v49; // x17
  unsigned __int64 v50; // x0
  unsigned __int64 v51; // x15
  __int64 v52; // x8
  unsigned __int64 v53; // x30
  __int64 v54; // x21
  unsigned __int64 v55; // x12
  unsigned __int64 v56; // x3
  unsigned __int64 v57; // x5
  unsigned __int64 v58; // x28
  unsigned __int64 v59; // x19
  unsigned __int64 v60; // x4
  unsigned __int64 v61; // x26
  unsigned __int64 v62; // x25
  unsigned __int64 v63; // x14
  unsigned __int64 v64; // x11
  unsigned __int64 v65; // x27
  unsigned __int64 v66; // x4
  unsigned __int64 v67; // x10
  unsigned __int64 v68; // x8
  unsigned __int64 v69; // x12
  unsigned __int64 v70; // x21
  unsigned __int64 v71; // x13
  unsigned __int64 v72; // x3
  unsigned __int64 v73; // x19
  unsigned __int64 v74; // x8
  unsigned __int64 v75; // x2
  unsigned __int64 v76; // x5
  unsigned __int64 v77; // x6
  unsigned __int64 v78; // x21
  unsigned __int64 v79; // x22
  unsigned __int64 v80; // x3
  unsigned __int64 v81; // x12
  unsigned __int64 v82; // x23
  unsigned __int64 v83; // x25
  unsigned __int64 v84; // x9
  __int64 v85; // x26
  __int64 v86; // x20
  unsigned __int64 v87; // x12
  unsigned __int64 v88; // x13
  unsigned __int64 v89; // x11
  unsigned __int64 v90; // x4
  unsigned __int64 v91; // x20
  __int64 v92; // x27
  unsigned __int64 v93; // x10
  unsigned __int64 v94; // x28
  unsigned __int64 v95; // x8
  unsigned __int64 v96; // x2
  signed __int64 v97; // x9
  signed __int64 v98; // x19
  signed __int64 v99; // x21
  signed __int64 v100; // x22
  signed __int64 v101; // x8
  signed __int64 v102; // x23
  __int64 v103; // x24
  signed __int64 v104; // x7
  __int64 v105; // x13
  signed __int64 v106; // x6
  signed __int64 v107; // x4
  signed __int64 v108; // x5
  __int64 v109; // x2
  __int64 v110; // x10
  __int64 v111; // x11
  __int64 v112; // x9
  __int64 v113; // x12
  __int64 v114; // x15
  __int64 v115; // x8
  __int64 v116; // x7
  __int64 v117; // x13
  __int64 v118; // x6
  __int64 v119; // x8
  unsigned __int64 v120; // x9
  unsigned __int64 v122; // [xsp+0h] [xbp-3A0h]
  unsigned __int64 v123; // [xsp+8h] [xbp-398h]
  unsigned __int64 v124; // [xsp+10h] [xbp-390h]
  unsigned __int64 v125; // [xsp+10h] [xbp-390h]
  unsigned __int64 v126; // [xsp+20h] [xbp-380h]
  unsigned __int64 v127; // [xsp+28h] [xbp-378h]
  unsigned __int64 v128; // [xsp+30h] [xbp-370h]
  unsigned __int64 v129; // [xsp+30h] [xbp-370h]
  unsigned __int64 v130; // [xsp+38h] [xbp-368h]
  unsigned __int64 v131; // [xsp+40h] [xbp-360h]
  unsigned __int64 v132; // [xsp+48h] [xbp-358h]
  unsigned __int64 v133; // [xsp+50h] [xbp-350h]
  unsigned __int64 v134; // [xsp+60h] [xbp-340h]
  unsigned __int64 v135; // [xsp+68h] [xbp-338h]
  unsigned __int64 v136; // [xsp+70h] [xbp-330h]
  _BYTE *v137; // [xsp+78h] [xbp-328h]
  unsigned __int64 v138; // [xsp+80h] [xbp-320h]
  unsigned __int64 v139; // [xsp+88h] [xbp-318h]
  unsigned __int64 v140; // [xsp+90h] [xbp-310h]
  _BYTE v141[216]; // [xsp+98h] [xbp-308h] BYREF
  _BYTE v142[40]; // [xsp+170h] [xbp-230h] BYREF
  __int64 v143; // [xsp+198h] [xbp-208h] BYREF
  __int64 v144; // [xsp+1C0h] [xbp-1E0h] BYREF
  unsigned __int16 v145; // [xsp+210h] [xbp-190h] BYREF
  unsigned __int8 v146; // [xsp+212h] [xbp-18Eh]
  unsigned __int8 v147; // [xsp+213h] [xbp-18Dh]
  unsigned __int8 v148; // [xsp+214h] [xbp-18Ch]
  unsigned __int8 v149; // [xsp+215h] [xbp-18Bh]
  unsigned __int8 v150; // [xsp+216h] [xbp-18Ah]
  unsigned __int8 v151; // [xsp+217h] [xbp-189h]
  unsigned __int8 v152; // [xsp+218h] [xbp-188h]
  unsigned __int8 v153; // [xsp+219h] [xbp-187h]
  unsigned __int8 v154; // [xsp+21Ah] [xbp-186h]
  unsigned __int8 v155; // [xsp+21Bh] [xbp-185h]
  unsigned __int8 v156; // [xsp+21Ch] [xbp-184h]
  unsigned __int8 v157; // [xsp+21Dh] [xbp-183h]
  unsigned __int8 v158; // [xsp+21Eh] [xbp-182h]
  unsigned __int8 v159; // [xsp+21Fh] [xbp-181h]
  unsigned __int8 v160; // [xsp+220h] [xbp-180h]
  unsigned __int8 v161; // [xsp+221h] [xbp-17Fh]
  unsigned __int8 v162; // [xsp+222h] [xbp-17Eh]
  unsigned __int8 v163; // [xsp+223h] [xbp-17Dh]
  unsigned __int8 v164; // [xsp+224h] [xbp-17Ch]
  unsigned __int16 v165; // [xsp+225h] [xbp-17Bh]
  unsigned __int8 v166; // [xsp+227h] [xbp-179h]
  unsigned __int8 v167; // [xsp+228h] [xbp-178h]
  unsigned __int8 v168; // [xsp+229h] [xbp-177h]
  unsigned __int8 v169; // [xsp+22Ah] [xbp-176h]
  unsigned __int8 v170; // [xsp+22Bh] [xbp-175h]
  unsigned __int8 v171; // [xsp+22Ch] [xbp-174h]
  unsigned __int8 v172; // [xsp+22Dh] [xbp-173h]
  unsigned __int8 v173; // [xsp+22Eh] [xbp-172h]
  unsigned __int8 v174; // [xsp+22Fh] [xbp-171h]
  unsigned __int16 v175; // [xsp+250h] [xbp-150h] BYREF
  unsigned __int8 v176; // [xsp+252h] [xbp-14Eh]
  unsigned __int8 v177; // [xsp+253h] [xbp-14Dh]
  unsigned __int8 v178; // [xsp+254h] [xbp-14Ch]
  unsigned __int8 v179; // [xsp+255h] [xbp-14Bh]
  unsigned __int8 v180; // [xsp+256h] [xbp-14Ah]
  unsigned __int8 v181; // [xsp+257h] [xbp-149h]
  unsigned __int8 v182; // [xsp+258h] [xbp-148h]
  unsigned __int8 v183; // [xsp+259h] [xbp-147h]
  unsigned __int8 v184; // [xsp+25Ah] [xbp-146h]
  unsigned __int8 v185; // [xsp+25Bh] [xbp-145h]
  unsigned __int8 v186; // [xsp+25Ch] [xbp-144h]
  unsigned __int8 v187; // [xsp+25Dh] [xbp-143h]
  unsigned __int8 v188; // [xsp+25Eh] [xbp-142h]
  unsigned __int8 v189; // [xsp+25Fh] [xbp-141h]
  unsigned __int8 v190; // [xsp+260h] [xbp-140h]
  unsigned __int8 v191; // [xsp+261h] [xbp-13Fh]
  unsigned __int8 v192; // [xsp+262h] [xbp-13Eh]
  unsigned __int8 v193; // [xsp+263h] [xbp-13Dh]
  unsigned __int8 v194; // [xsp+264h] [xbp-13Ch]
  unsigned __int16 v195; // [xsp+265h] [xbp-13Bh]
  unsigned __int8 v196; // [xsp+267h] [xbp-139h]
  unsigned __int8 v197; // [xsp+268h] [xbp-138h]
  unsigned __int8 v198; // [xsp+269h] [xbp-137h]
  unsigned __int8 v199; // [xsp+26Ah] [xbp-136h]
  unsigned __int8 v200; // [xsp+26Bh] [xbp-135h]
  unsigned __int8 v201; // [xsp+26Ch] [xbp-134h]
  unsigned __int8 v202; // [xsp+26Dh] [xbp-133h]
  unsigned __int8 v203; // [xsp+26Eh] [xbp-132h]
  unsigned __int8 v204; // [xsp+26Fh] [xbp-131h]
  unsigned __int16 v205; // [xsp+290h] [xbp-110h] BYREF
  unsigned __int8 v206; // [xsp+292h] [xbp-10Eh]
  unsigned __int8 v207; // [xsp+293h] [xbp-10Dh]
  unsigned __int8 v208; // [xsp+294h] [xbp-10Ch]
  unsigned __int8 v209; // [xsp+295h] [xbp-10Bh]
  unsigned __int8 v210; // [xsp+296h] [xbp-10Ah]
  unsigned __int8 v211; // [xsp+297h] [xbp-109h]
  unsigned __int8 v212; // [xsp+298h] [xbp-108h]
  unsigned __int8 v213; // [xsp+299h] [xbp-107h]
  unsigned __int8 v214; // [xsp+29Ah] [xbp-106h]
  unsigned __int8 v215; // [xsp+29Bh] [xbp-105h]
  unsigned __int8 v216; // [xsp+29Ch] [xbp-104h]
  unsigned __int8 v217; // [xsp+29Dh] [xbp-103h]
  unsigned __int8 v218; // [xsp+29Eh] [xbp-102h]
  unsigned __int8 v219; // [xsp+29Fh] [xbp-101h]
  unsigned __int8 v220; // [xsp+2A0h] [xbp-100h]
  unsigned __int8 v221; // [xsp+2A1h] [xbp-FFh]
  unsigned __int8 v222; // [xsp+2A2h] [xbp-FEh]
  unsigned __int8 v223; // [xsp+2A3h] [xbp-FDh]
  unsigned __int8 v224; // [xsp+2A4h] [xbp-FCh]
  unsigned __int16 v225; // [xsp+2A5h] [xbp-FBh]
  unsigned __int8 v226; // [xsp+2A7h] [xbp-F9h]
  unsigned __int8 v227; // [xsp+2A8h] [xbp-F8h]
  unsigned __int8 v228; // [xsp+2A9h] [xbp-F7h]
  unsigned __int8 v229; // [xsp+2AAh] [xbp-F6h]
  unsigned __int8 v230; // [xsp+2ABh] [xbp-F5h]
  unsigned __int8 v231; // [xsp+2ACh] [xbp-F4h]
  unsigned __int8 v232; // [xsp+2ADh] [xbp-F3h]
  unsigned __int8 v233; // [xsp+2AEh] [xbp-F2h]
  unsigned __int8 v234; // [xsp+2AFh] [xbp-F1h]
  __int64 v235; // [xsp+2B0h] [xbp-F0h] BYREF
  _BYTE v236[40]; // [xsp+2D0h] [xbp-D0h] BYREF
  _BYTE v237[40]; // [xsp+2F8h] [xbp-A8h] BYREF
  char v238[32]; // [xsp+320h] [xbp-80h] BYREF

  sub_100466EA0(v141);
  sub_100467180((int)v141, a5, 0x20u);
  sub_100466EE0(&v205, v141);
  LOBYTE(v205) = v205 & 0xF8;
  v234 = v234 & 0x3F | 0x40;
  sub_100466EA0(v141);
  sub_100467180((int)v141, &v235, 0x20u);
  sub_100467180((int)v141, a2, a3);
  sub_100466EE0(&v175, v141);
  sub_1003D235C(&v175);
  sub_1003D2AC0(v142, &v175);
  sub_1003D85B8(&v145, &v144);
  sub_1003D8E14(v237, v142, &v145);
  sub_1003D8E14(v236, &v143, &v145);
  sub_1003D91D8(a1, v236);
  sub_1003D91D8(v238, v237);
  a1[31] ^= v238[0] << 7;
  v137 = a1;
  sub_100466EA0(v141);
  sub_100467180((int)v141, a1, 0x20u);
  sub_100467180((int)v141, a4, 0x20u);
  sub_100467180((int)v141, a2, a3);
  sub_100466EE0(&v145, v141);
  sub_1003D235C(&v145);
  v135 = v165 | (unsigned __int64)((v166 << 16) & 0x1F0000);
  v10 = v205 | (unsigned __int64)((v206 << 16) & 0x1F0000);
  v138 = v225 | (unsigned __int64)((v226 << 16) & 0x1F0000);
  v11 = ((unsigned __int64)(v146 | (unsigned __int16)(v147 << 8) | (v148 << 16) & 0xFFFFFF | (v149 << 24)) >> 5)
      & 0x1FFFFF;
  v12 = ((unsigned __int64)(v149 | (unsigned __int16)(v150 << 8) | (v151 << 16)) >> 2) & 0x1FFFFF;
  v13 = ((unsigned __int64)(v206 | (unsigned __int16)(v207 << 8) | (v208 << 16) & 0xFFFFFF | (v209 << 24)) >> 5)
      & 0x1FFFFF;
  v14 = ((unsigned __int64)(v209 | (unsigned __int16)(v210 << 8) | (v211 << 16)) >> 2) & 0x1FFFFF;
  v15 = v145 | (unsigned __int64)((v146 << 16) & 0x1F0000);
  v140 = v10 * v12
       + v13 * v11
       + v14 * v15
       + (((unsigned __int64)(v179 | (unsigned __int16)(v180 << 8) | (v181 << 16)) >> 2) & 0x1FFFFF);
  v16 = ((unsigned __int64)(v151 | (unsigned __int16)(v152 << 8) | (v153 << 16) & 0xFFFFFF | (v154 << 24)) >> 7)
      & 0x1FFFFF;
  v17 = ((unsigned __int64)(v154 | (unsigned __int16)(v155 << 8) | (v156 << 16) & 0xFFFFFF | (v157 << 24)) >> 4)
      & 0x1FFFFF;
  v18 = ((unsigned __int64)(v211 | (unsigned __int16)(v212 << 8) | (v213 << 16) & 0xFFFFFF | (v214 << 24)) >> 7)
      & 0x1FFFFF;
  v19 = ((unsigned __int64)(v214 | (unsigned __int16)(v215 << 8) | (v216 << 16) & 0xFFFFFF | (v217 << 24)) >> 4)
      & 0x1FFFFF;
  v133 = v10 * v17
       + v13 * v16
       + v14 * v12
       + v18 * v11
       + v19 * v15
       + (((unsigned __int64)(v184 | (unsigned __int16)(v185 << 8) | (v186 << 16) & 0xFFFFFF | (v187 << 24)) >> 4)
        & 0x1FFFFF);
  v20 = ((unsigned __int64)(v157 | (unsigned __int16)(v158 << 8) | (v159 << 16)) >> 1) & 0x1FFFFF;
  v21 = ((unsigned __int64)(v159 | (unsigned __int16)(v160 << 8) | (v161 << 16) & 0xFFFFFF | (v162 << 24)) >> 6)
      & 0x1FFFFF;
  v22 = ((unsigned __int64)(v217 | (unsigned __int16)(v218 << 8) | (v219 << 16)) >> 1) & 0x1FFFFF;
  v23 = ((unsigned __int64)(v219 | (unsigned __int16)(v220 << 8) | (v221 << 16) & 0xFFFFFF | (v222 << 24)) >> 6)
      & 0x1FFFFF;
  v132 = v10 * v21
       + v13 * v20
       + v14 * v17
       + v18 * v16
       + v19 * v12
       + v22 * v11
       + v23 * v15
       + (((unsigned __int64)(v189 | (unsigned __int16)(v190 << 8) | (v191 << 16) & 0xFFFFFF | (v192 << 24)) >> 6)
        & 0x1FFFFF);
  v24 = (v162 & 0xF8 | (unsigned __int64)(unsigned __int16)(v163 << 8) | ((unsigned __int64)v164 << 16)) >> 3;
  v25 = (v222 & 0xF8 | (unsigned __int64)(unsigned __int16)(v223 << 8) | ((unsigned __int64)v224 << 16)) >> 3;
  v139 = v10 * v135
       + v13 * v24
       + v14 * v21
       + v18 * v20
       + v19 * v17
       + v22 * v16
       + v23 * v12
       + v25 * v11
       + v138 * v15
       + v195
       + ((v196 << 16) & 0x1F0000);
  v26 = ((unsigned __int64)(v166 | (unsigned __int16)(v167 << 8) | (v168 << 16) & 0xFFFFFF | (v169 << 24)) >> 5)
      & 0x1FFFFF;
  v27 = ((unsigned __int64)(v169 | (unsigned __int16)(v170 << 8) | (v171 << 16)) >> 2) & 0x1FFFFF;
  v28 = ((unsigned __int64)(v226 | (unsigned __int16)(v227 << 8) | (v228 << 16) & 0xFFFFFF | (v229 << 24)) >> 5)
      & 0x1FFFFF;
  v29 = ((unsigned __int64)(v229 | (unsigned __int16)(v230 << 8) | (v231 << 16)) >> 2) & 0x1FFFFF;
  v131 = v10 * v27
       + v13 * v26
       + v14 * v135
       + v18 * v24
       + v19 * v21
       + v22 * v20
       + v23 * v17
       + v25 * v16
       + v138 * v12
       + v28 * v11
       + v29 * v15
       + (((unsigned __int64)(v199 | (unsigned __int16)(v200 << 8) | (v201 << 16)) >> 2) & 0x1FFFFF);
  v30 = (v175 | (unsigned __int64)((v176 << 16) & 0x1F0000)) + v10 * v15;
  v124 = v10 * v11
       + v13 * v15
       + ((v30 + 0x100000) >> 21)
       + (((unsigned __int64)(v176 | (unsigned __int16)(v177 << 8) | (v178 << 16) & 0xFFFFFF | (v179 << 24)) >> 5)
        & 0x1FFFFF);
  v134 = v30 - ((v30 + 0x100000) & 0xFFFFFE00000LL);
  v31 = v10 * v16
      + v13 * v12
      + v14 * v11
      + v18 * v15
      + (((unsigned __int64)(v181 | (unsigned __int16)(v182 << 8) | (v183 << 16) & 0xFFFFFF | (v184 << 24)) >> 7)
       & 0x1FFFFF);
  v128 = v10 * v20
       + v13 * v17
       + v14 * v16
       + v18 * v12
       + v19 * v11
       + v22 * v15
       + (((unsigned __int64)(v187 | (unsigned __int16)(v188 << 8) | (v189 << 16)) >> 1) & 0x1FFFFF);
  v130 = v10 * v24
       + v13 * v21
       + v14 * v20
       + v18 * v17
       + v19 * v16
       + v22 * v12
       + v23 * v11
       + v25 * v15
       + ((v192 & 0xF8 | (unsigned __int64)(unsigned __int16)(v193 << 8) | ((unsigned __int64)v194 << 16)) >> 3);
  v127 = v10 * v26
       + v13 * v135
       + v14 * v24
       + v18 * v21
       + v19 * v20
       + v22 * v17
       + v23 * v16
       + v25 * v12
       + v138 * v11
       + v28 * v15
       + ((v139 + 0x100000) >> 21)
       + (((unsigned __int64)(v196 | (unsigned __int16)(v197 << 8) | (v198 << 16) & 0xFFFFFF | (v199 << 24)) >> 5)
        & 0x1FFFFF);
  v32 = (v171
       | (unsigned __int64)(unsigned __int16)(v172 << 8)
       | ((unsigned __int64)v173 << 16) & 0xFFFFFFFF00FFFFFFLL
       | ((unsigned __int64)v174 << 24)) >> 7;
  v33 = (v231
       | (unsigned __int64)(unsigned __int16)(v232 << 8)
       | ((unsigned __int64)v233 << 16) & 0xFFFFFFFF00FFFFFFLL
       | ((unsigned __int64)v234 << 24)) >> 7;
  v122 = v13 * v32
       + v14 * v27
       + v18 * v26
       + v19 * v135
       + v22 * v24
       + v23 * v21
       + v25 * v20
       + v138 * v17
       + v28 * v16
       + v29 * v12
       + v33 * v11;
  v123 = v10 * v32
       + v13 * v27
       + v14 * v26
       + v18 * v135
       + v19 * v24
       + v22 * v21
       + v23 * v20
       + v25 * v17
       + v138 * v16
       + v28 * v12
       + v29 * v11
       + v33 * v15
       + ((v201
         | (unsigned __int64)(unsigned __int16)(v202 << 8)
         | ((unsigned __int64)v203 << 16) & 0xFFFFFFFF00FFFFFFLL
         | ((unsigned __int64)v204 << 24)) >> 7);
  v34 = v25 * v32 + v138 * v27 + v28 * v26 + v29 * v135 + v33 * v24;
  v35 = v138 * v32 + v28 * v27 + v29 * v26 + v33 * v135 + ((__int64)(v34 + 0x100000) >> 21);
  v36 = v28 * v32 + v29 * v27 + v33 * v26;
  v37 = v29 * v32 + v33 * v27 + ((v36 + 0x100000) >> 21);
  v38 = v33 * v32 + 0x100000;
  v39 = v33 * v32 - (v38 & 0x7FFFFFFFFFE00000LL);
  v40 = v14 * v32 + v18 * v27;
  v41 = v18 * v32 + v19 * v27 + v22 * v26 + v23 * v135 + v25 * v24 + v138 * v21 + v28 * v20 + v29 * v17 + v33 * v16;
  v42 = v40 + v19 * v26 + v22 * v135 + v23 * v24 + v25 * v21 + v138 * v20 + v28 * v17 + v29 * v16;
  v43 = v19 * v32 + v22 * v27;
  v44 = v22 * v32 + v23 * v27;
  v45 = v23 * v32 + v25 * v27 + v138 * v26 + v28 * v135 + v29 * v24;
  v46 = v44 + v25 * v26 + v138 * v135 + v28 * v24 + v29 * v21;
  v47 = v43 + v23 * v26 + v25 * v135 + v138 * v24 + v28 * v21 + v29 * v20;
  v48 = v46 + v33 * v20;
  v49 = v42 + v33 * v12;
  v50 = v47 + v33 * v17;
  v51 = v31 + ((v140 + 0x100000) >> 21);
  v126 = v124 + 0x100000;
  v136 = v124 - ((v124 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v52 = v45 + v33 * v21 + ((__int64)(v48 + 0x100000) >> 21);
  v53 = v34 - ((v34 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((v52 + 0x100000) >> 21);
  v125 = v52 - ((v52 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v54 = v36 - ((v36 + 0x100000) & 0x7FFFFFFFFFE00000LL) + ((v35 + 0x100000) >> 21);
  v55 = v35 - ((v35 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v56 = v39 + ((v37 + 0x100000) >> 21);
  v57 = v37 - ((v37 + 0x100000) & 0x7FFFFFFFFFE00000LL);
  v58 = v50 + ((__int64)(v41 + 0x100000) >> 21);
  v59 = v49 + ((__int64)(v122 + 0x100000) >> 21);
  v129 = v128 + ((v133 + 0x100000) >> 21);
  v60 = v130 + ((v132 + 0x100000) >> 21);
  v61 = 666643 * v53 + ((v129 + 0x100000) >> 21) + v132 - ((v132 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v62 = 666643 * v54
      + 470296 * v55
      + 654183 * v53
      + v139
      + ((v60 + 0x100000) >> 21)
      - ((v139 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v63 = v123 + ((v131 + 0x100000) >> 21);
  v64 = 666643 * v56
      + 470296 * v57
      + 654183 * v54
      - 997805 * v55
      + 136657 * v53
      + v131
      + ((v127 + 0x100000) >> 21)
      - ((v131 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v65 = 666643 * v55 + 470296 * v53 + v60 - ((v60 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v66 = 666643 * v57
      + 470296 * v54
      + 654183 * v55
      - 997805 * v53
      + v127
      + ((__int64)(v62 + 0x100000) >> 21)
      - ((v127 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v67 = v122
      + 470296 * (v38 >> 21)
      - ((v122 + 0x100000) & 0xFFFFFFFFFFE00000LL)
      + 654183 * v56
      - 997805 * v57
      + 136657 * v54
      - 683901 * v55
      + ((__int64)(v63 + 0x100000) >> 21);
  v68 = 666643 * (v38 >> 21)
      + 470296 * v56
      + 654183 * v57
      - 997805 * v54
      + 136657 * v55
      - 683901 * v53
      + v63
      - ((v63 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v69 = v59
      + 654183 * (v38 >> 21)
      - ((v59 + 0x100000) & 0xFFFFFFFFFFE00000LL)
      - 997805 * v56
      + 136657 * v57
      - 683901 * v54
      + ((__int64)(v67 + 0x100000) >> 21);
  v70 = v48 - 683901 * (v38 >> 21) - ((v48 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((__int64)(v58 + 0x100000) >> 21);
  v71 = v41
      - 997805 * (v38 >> 21)
      - ((v41 + 0x100000) & 0xFFFFFFFFFFE00000LL)
      + ((__int64)(v59 + 0x100000) >> 21)
      + 136657 * v56
      - 683901 * v57;
  v72 = v58
      + 136657 * (v38 >> 21)
      - ((v58 + 0x100000) & 0xFFFFFFFFFFE00000LL)
      - 683901 * v56
      + ((__int64)(v71 + 0x100000) >> 21);
  v73 = v125 + ((__int64)(v70 + 0x100000) >> 21);
  v74 = v68 + ((__int64)(v64 + 0x100000) >> 21);
  v75 = v67 - ((v67 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((__int64)(v74 + 0x100000) >> 21);
  v76 = v71 - ((v71 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((__int64)(v69 + 0x100000) >> 21);
  v77 = v69 - ((v69 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v78 = v70 - ((v70 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((__int64)(v72 + 0x100000) >> 21);
  v79 = v72 - ((v72 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v80 = v64 - 683901 * v73 + ((__int64)(v66 + 0x100000) >> 21) - ((v64 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v81 = v65 + ((__int64)(v61 + 0x100000) >> 21);
  v82 = -997805LL * v73
      + 136657 * v78
      - 683901 * v79
      + v62
      + ((__int64)(v81 + 0x100000) >> 21)
      - ((v62 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v83 = 470296 * v73 + 654183 * v78 - 997805 * v79 + v61 - ((v61 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v84 = v134 + 666643 * v75;
  v85 = v136 + 470296 * v75 + 666643 * v77 + ((__int64)(v84 + 0x100000) >> 21);
  v86 = v83 + 136657 * v76 - 683901 * v77;
  v87 = 654183 * v73
      - 997805 * v78
      + 136657 * v79
      + v81
      - ((v81 + 0x100000) & 0xFFFFFFFFFFE00000LL)
      - 683901 * v76
      + ((v86 + 0x100000) >> 21);
  v88 = v86 - ((v86 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v89 = 136657 * v73
      - 683901 * v78
      + v66
      + ((__int64)(v82 + 0x100000) >> 21)
      - ((v66 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v90 = v82 - ((v82 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v91 = v74 - ((v74 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((__int64)(v80 + 0x100000) >> 21);
  v92 = (__int64)(v91 + 0x100000) >> 21;
  v93 = v140 + (v126 >> 21) - ((v140 + 0x100000) & 0xFFFFFFFFFFE00000LL) + 654183 * v75 + 666643 * v76 + 470296 * v77;
  v94 = v51
      - ((v51 + 0x100000) & 0xFFFFFFFFFFE00000LL)
      + 666643 * v79
      - 997805 * v75
      + 470296 * v76
      + 654183 * v77
      + ((__int64)(v93 + 0x100000) >> 21);
  v95 = v133
      + ((v51 + 0x100000) >> 21)
      - ((v133 + 0x100000) & 0xFFFFFFFFFFE00000LL)
      + 666643 * v78
      + 470296 * v79
      + 136657 * v75
      + 654183 * v76
      - 997805 * v77;
  v96 = v129
      + 666643 * v73
      + 470296 * v78
      + 654183 * v79
      - ((v129 + 0x100000) & 0xFFFFFFFFFFE00000LL)
      - 683901 * v75
      - 997805 * v76
      + 136657 * v77
      + ((__int64)(v95 + 0x100000) >> 21);
  v97 = v84 - ((v84 + 0x100000) & 0xFFFFFFFFFFE00000LL) + 666643 * v92;
  v98 = v85 + 470296 * v92 - ((v85 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v97 >> 21);
  v99 = v93 + 654183 * v92 - ((v93 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((v85 + 0x100000) >> 21) + (v98 >> 21);
  v100 = v94 - 997805 * v92 - ((v94 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v99 >> 21);
  v101 = v95
       + 136657 * v92
       - ((v95 + 0x100000) & 0xFFFFFFFFFFE00000LL)
       + ((__int64)(v94 + 0x100000) >> 21)
       + (v100 >> 21);
  v102 = v96 - 683901 * v92 - ((v96 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v101 >> 21);
  v103 = v88 + ((__int64)(v96 + 0x100000) >> 21) + (v102 >> 21);
  v104 = v87 - ((v87 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v103 >> 21);
  v105 = v90 + ((__int64)(v87 + 0x100000) >> 21) + (v104 >> 21);
  v106 = v89 - ((v89 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v105 >> 21);
  v107 = v80 + ((__int64)(v89 + 0x100000) >> 21) - ((v80 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v106 >> 21);
  v108 = v91 - ((v91 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v107 >> 21);
  v109 = (v97 & 0x1FFFFF) + 666643 * (v108 >> 21);
  v110 = (v98 & 0x1FFFFF) + 470296 * (v108 >> 21) + (v109 >> 21);
  v111 = (v99 & 0x1FFFFF) + 654183 * (v108 >> 21) + (v110 >> 21);
  v112 = (v100 & 0x1FFFFF) - 997805 * (v108 >> 21) + (v111 >> 21);
  v113 = (v101 & 0x1FFFFF) + 136657 * (v108 >> 21) + (v112 >> 21);
  v114 = (v102 & 0x1FFFFF) - 683901 * (v108 >> 21) + (v113 >> 21);
  v115 = (v103 & 0x1FFFFF) + (v114 >> 21);
  v116 = (v104 & 0x1FFFFF) + (v115 >> 21);
  v117 = (v105 & 0x1FFFFF) + (v116 >> 21);
  v118 = (v106 & 0x1FFFFF) + (v117 >> 21);
  *((_WORD *)v137 + 16) = v109;
  v137[34] = BYTE2(v109) & 0x1F | (32 * (v98 + 24 * (v108 >> 21) + (v109 >> 21)));
  v137[35] = (unsigned __int64)v110 >> 3;
  v137[36] = (unsigned __int64)v110 >> 11;
  v137[37] = ((unsigned int)v110 >> 19) & 3 | (4 * (v99 + 103 * (v108 >> 21) + (v110 >> 21)));
  v137[38] = (unsigned __int64)v111 >> 6;
  v137[39] = ((unsigned int)v111 >> 14) & 0x7F
           | (((_BYTE)v94
             - -83 * (_BYTE)v92
             + (unsigned __int8)(v99 >> 21)
             - -83 * (unsigned __int8)(v108 >> 21)
             + (unsigned __int8)(v111 >> 21)) << 7);
  v137[40] = (unsigned __int64)v112 >> 1;
  v137[41] = (unsigned __int64)v112 >> 9;
  v137[42] = ((unsigned int)v112 >> 17) & 0xF | (16 * v113);
  v137[43] = (unsigned __int64)v113 >> 4;
  v137[44] = (unsigned __int64)v113 >> 12;
  v137[45] = ((v113 & 0x100000) != 0) | (2 * (v102 - 125 * (v108 >> 21) + (v113 >> 21)));
  v137[46] = (unsigned __int64)v114 >> 7;
  v137[47] = ((unsigned int)v114 >> 15) & 0x3F | (((_BYTE)v103 + (unsigned __int8)(v114 >> 21)) << 6);
  v137[48] = (unsigned __int64)v115 >> 2;
  v137[49] = (unsigned __int64)v115 >> 10;
  v137[50] = ((unsigned int)v115 >> 18) & 7 | (8 * v116);
  v137[51] = (unsigned __int64)v116 >> 5;
  v137[52] = (unsigned __int64)v116 >> 13;
  *(_WORD *)(v137 + 53) = v117;
  v119 = (v107 & 0x1FFFFF) + (v118 >> 21);
  v137[55] = BYTE2(v117) & 0x1F | (32 * v118);
  v137[56] = (unsigned __int64)v118 >> 3;
  v137[57] = (unsigned __int64)v118 >> 11;
  v137[58] = ((unsigned int)v118 >> 19) & 3 | (4 * (v107 + (v118 >> 21)));
  v137[59] = (unsigned __int64)v119 >> 6;
  v120 = (v108 & 0x1FFFFF) + (v119 >> 21);
  v137[60] = ((unsigned int)v119 >> 14) & 0x7F
           | (((_BYTE)v91 + (unsigned __int8)(v107 >> 21) + (unsigned __int8)(v119 >> 21)) << 7);
  v137[61] = v120 >> 1;
  v137[62] = v120 >> 9;
  v137[63] = v120 >> 17;
  sub_100391B40(v141, 216);
  sub_100391B40(&v175, 64);
  sub_100391B40(&v205, 64);
  return 1;
}

/* ========================================================================
 * fallback sub_1003dd5cc
 * EA: 0x1003dd5cc
 ======================================================================== */

__int64 __fastcall sub_1003DD5CC(_OWORD *a1, unsigned __int16 *a2, __int64 a3, unsigned __int16 *a4)
{
  unsigned __int64 v6; // x9
  unsigned int v7; // w11
  int v8; // w8
  int v9; // w12
  int v10; // w12
  unsigned int *v11; // x13
  unsigned int v12; // w14
  unsigned int v13; // w15
  __int64 v14; // x9
  __int64 v15; // x10
  _QWORD *v16; // x12
  __int64 v17; // x9
  int v18; // w8
  unsigned __int64 v19; // x9
  unsigned int v20; // w11
  int v21; // w8
  int v22; // w12
  int v23; // w12
  unsigned int *v24; // x13
  unsigned int v25; // w14
  unsigned int v26; // w15
  __int64 v27; // x9
  __int64 v28; // x10
  _QWORD *v29; // x12
  __int64 v30; // x9
  int v31; // w8
  __int64 result; // x0
  __int64 v33; // x20
  int v34; // w23
  _BYTE *v35; // x21
  int v36; // w25
  int v37; // w24
  bool v38; // vf
  __int64 v39; // x20
  _BYTE *v40; // x21
  char *v41; // x21
  _OWORD *v42; // x21
  int v43; // w8
  int v44; // w22
  int v45; // w28
  __int64 v46; // x8
  __int64 v47; // x8
  _BYTE *v48; // x23
  __int128 v49; // q1
  __int128 v50; // q1
  unsigned __int64 v51; // x8
  _BYTE *v52; // x23
  __int128 v53; // q1
  __int128 v54; // q1
  _OWORD *v55; // x12
  int v56; // w13
  __int64 v57; // x14
  _OWORD *v58; // x15
  __int128 v59; // q1
  _OWORD *v60; // x12
  int v61; // w13
  __int64 v62; // x14
  _OWORD *v63; // x15
  __int128 v64; // q1
  _BYTE v65[64]; // [xsp+0h] [xbp-10A0h] BYREF
  __int64 v66; // [xsp+40h] [xbp-1060h] BYREF
  _BYTE v67[64]; // [xsp+80h] [xbp-1020h] BYREF
  _BYTE v68[64]; // [xsp+C0h] [xbp-FE0h] BYREF
  __int64 v69; // [xsp+100h] [xbp-FA0h] BYREF
  __int64 v70; // [xsp+140h] [xbp-F60h] BYREF
  _BYTE v71[64]; // [xsp+180h] [xbp-F20h] BYREF
  __int64 v72; // [xsp+1C0h] [xbp-EE0h] BYREF
  __int64 v73; // [xsp+200h] [xbp-EA0h] BYREF
  __int64 v74; // [xsp+240h] [xbp-E60h] BYREF
  _BYTE v75[64]; // [xsp+280h] [xbp-E20h] BYREF
  __int64 v76; // [xsp+2C0h] [xbp-DE0h] BYREF
  __int64 v77; // [xsp+300h] [xbp-DA0h] BYREF
  __int64 v78; // [xsp+340h] [xbp-D60h] BYREF
  _BYTE v79[64]; // [xsp+380h] [xbp-D20h] BYREF
  __int64 v80; // [xsp+3C0h] [xbp-CE0h] BYREF
  __int64 v81; // [xsp+400h] [xbp-CA0h] BYREF
  __int64 v82; // [xsp+440h] [xbp-C60h] BYREF
  _BYTE v83[64]; // [xsp+480h] [xbp-C20h] BYREF
  __int64 v84; // [xsp+4C0h] [xbp-BE0h] BYREF
  __int64 v85; // [xsp+500h] [xbp-BA0h] BYREF
  __int64 v86; // [xsp+540h] [xbp-B60h] BYREF
  _BYTE v87[64]; // [xsp+580h] [xbp-B20h] BYREF
  __int64 v88; // [xsp+5C0h] [xbp-AE0h] BYREF
  __int64 v89; // [xsp+600h] [xbp-AA0h] BYREF
  __int64 v90; // [xsp+640h] [xbp-A60h] BYREF
  _BYTE v91[64]; // [xsp+680h] [xbp-A20h] BYREF
  __int64 v92; // [xsp+6C0h] [xbp-9E0h] BYREF
  __int64 v93; // [xsp+700h] [xbp-9A0h] BYREF
  __int64 v94; // [xsp+740h] [xbp-960h] BYREF
  _BYTE v95[64]; // [xsp+780h] [xbp-920h] BYREF
  __int64 v96; // [xsp+7C0h] [xbp-8E0h] BYREF
  int v97; // [xsp+808h] [xbp-898h] BYREF
  _DWORD v98[3]; // [xsp+80Ch] [xbp-894h] BYREF
  _QWORD v99[75]; // [xsp+818h] [xbp-888h] BYREF
  int v100; // [xsp+A70h] [xbp-630h] BYREF
  _DWORD v101[3]; // [xsp+A74h] [xbp-62Ch] BYREF
  _QWORD v102[112]; // [xsp+A80h] [xbp-620h] BYREF
  _BYTE v103[64]; // [xsp+E00h] [xbp-2A0h] BYREF
  __int64 v104; // [xsp+E40h] [xbp-260h] BYREF
  _BYTE v105[64]; // [xsp+E80h] [xbp-220h] BYREF
  _BYTE v106[64]; // [xsp+EC0h] [xbp-1E0h] BYREF
  _BYTE v107[64]; // [xsp+F00h] [xbp-1A0h] BYREF
  _BYTE v108[64]; // [xsp+F40h] [xbp-160h] BYREF
  __int128 v109; // [xsp+F80h] [xbp-120h] BYREF
  __int128 v110; // [xsp+F90h] [xbp-110h]
  __int128 v111; // [xsp+FA0h] [xbp-100h]
  __int128 v112; // [xsp+FB0h] [xbp-F0h]
  _BYTE v113[64]; // [xsp+FC0h] [xbp-E0h] BYREF
  __int128 v114; // [xsp+1000h] [xbp-A0h] BYREF
  __int128 v115; // [xsp+1010h] [xbp-90h]
  __int128 v116; // [xsp+1020h] [xbp-80h]
  __int128 v117; // [xsp+1030h] [xbp-70h]

  v6 = *a2;
  v99[74] = 0xFFFFFFFFLL;
  v7 = 1;
  v8 = 75;
  do
  {
    if ( v7 >= 0x1C )
    {
      v9 = 16 * v7;
      if ( !(_WORD)v6 )
        goto LABEL_2;
    }
    else
    {
      v9 = 16 * v7;
      v6 += (unsigned int)(*(_QWORD *)&a2[4 * (v7 >> 2)] >> ((16 * v7) & 0x30)) << 16;
      if ( !(_WORD)v6 )
        goto LABEL_2;
    }
    v10 = v9 - 16;
    v11 = &v98[2 * v8];
    do
    {
      v12 = __clz(__rbit32(v6));
      v13 = (((unsigned int)v6 >> v12) & 0x3F) - (((unsigned int)v6 >> v12) & 0x40);
      v6 -= (int)(v13 << v12);
      *(v11 - 1) = v10 + v12;
      *v11 = v13;
      v11 -= 2;
      --v8;
    }
    while ( (_WORD)v6 );
LABEL_2:
    v6 >>= 16;
    ++v7;
  }
  while ( v7 != 30 );
  if ( v8 != 76 )
  {
    v14 = (unsigned int)(76 - v8);
    if ( (unsigned int)v14 < 4
      || -2 - v8 < (unsigned int)(75 - v8)
      || &v97 < &v98[2 * v14 - 1 + 2 * (unsigned int)(v8 + 1)] && &v98[2 * (v8 + 1) - 1] < &v98[2 * v14 - 1] )
    {
      v15 = 0;
      goto LABEL_16;
    }
    v15 = (unsigned int)v14 & 0xFFFFFFFC;
    v55 = v99;
    v56 = v8 + 1;
    v57 = v15;
    do
    {
      v58 = &v98[2 * v56 - 1];
      v59 = v58[1];
      *(v55 - 1) = *v58;
      *v55 = v59;
      v55 += 2;
      v56 += 4;
      v57 -= 4;
    }
    while ( v57 );
    if ( v15 != v14 )
    {
LABEL_16:
      v16 = &v98[2 * v15 - 1];
      v17 = v14 - v15;
      v18 = v15 + v8 + 1;
      do
      {
        *v16++ = *(_QWORD *)&v98[2 * v18++ - 1];
        --v17;
      }
      while ( v17 );
    }
  }
  v19 = *a4;
  v102[111] = 0xFFFFFFFFLL;
  v20 = 1;
  v21 = 112;
  while ( 2 )
  {
    if ( v20 >= 0x1C )
    {
      v22 = 16 * v20;
      if ( !(_WORD)v19 )
        goto LABEL_19;
    }
    else
    {
      v22 = 16 * v20;
      v19 += (unsigned int)(*(_QWORD *)&a4[4 * (v20 >> 2)] >> ((16 * v20) & 0x30)) << 16;
      if ( !(_WORD)v19 )
        goto LABEL_19;
    }
    v23 = v22 - 16;
    v24 = &v101[2 * v21];
    do
    {
      v25 = __clz(__rbit32(v19));
      v26 = (((unsigned int)v19 >> v25) & 0xF) - (((unsigned int)v19 >> v25) & 0x10);
      v19 -= (int)(v26 << v25);
      *(v24 - 1) = v23 + v25;
      *v24 = v26;
      v24 -= 2;
      --v21;
    }
    while ( (_WORD)v19 );
LABEL_19:
    v19 >>= 16;
    if ( ++v20 != 30 )
      continue;
    break;
  }
  if ( v21 != 113 )
  {
    v27 = (unsigned int)(113 - v21);
    if ( (unsigned int)v27 < 4
      || -2 - v21 < (unsigned int)(112 - v21)
      || &v100 < &v101[2 * v27 - 1 + 2 * (unsigned int)(v21 + 1)] && &v101[2 * (v21 + 1) - 1] < &v101[2 * v27 - 1] )
    {
      v28 = 0;
      goto LABEL_33;
    }
    v28 = (unsigned int)v27 & 0xFFFFFFFC;
    v60 = v102;
    v61 = v21 + 1;
    v62 = v28;
    do
    {
      v63 = &v101[2 * v61 - 1];
      v64 = v63[1];
      *(v60 - 1) = *v63;
      *v60 = v64;
      v60 += 2;
      v61 += 4;
      v62 -= 4;
    }
    while ( v62 );
    if ( v28 != v27 )
    {
LABEL_33:
      v29 = &v101[2 * v28 - 1];
      v30 = v27 - v28;
      v31 = v28 + v21 + 1;
      do
      {
        *v29++ = *(_QWORD *)&v101[2 * v31++ - 1];
        --v30;
      }
      while ( v30 );
    }
  }
  sub_1003DF514(v65, a3 + 64, a3);
  sub_1003DF124(&v66, a3, a3 + 64);
  sub_1003DAC24(v67, a3 + 192, 78164);
  sub_1003DF514(v67, &unk_100561FF0, v67);
  sub_1003DF124(v68, a3 + 128, a3 + 128);
  sub_1003DAD64(v107, a3, 0);
  sub_1003DF514(v103, v108, v107);
  sub_1003DF124(&v104, v107, v108);
  sub_1003DAC24(v105, v113, 78164);
  sub_1003DF514(v105, &unk_100561FF0, v105);
  sub_1003DF124(v106, &v109, &v109);
  sub_1003DA7B0(&v114, &v109, v68);
  v109 = v114;
  v110 = v115;
  v111 = v116;
  v112 = v117;
  sub_1003DB764(v107, v65, 0);
  sub_1003DF514(&v69, v108, v107);
  sub_1003DF124(&v70, v107, v108);
  sub_1003DAC24(v71, v113, 78164);
  sub_1003DF514(v71, &unk_100561FF0, v71);
  sub_1003DF124(&v72, &v109, &v109);
  sub_1003DA7B0(&v114, &v109, v106);
  v109 = v114;
  v110 = v115;
  v111 = v116;
  v112 = v117;
  sub_1003DB764(v107, v103, 0);
  sub_1003DF514(&v73, v108, v107);
  sub_1003DF124(&v74, v107, v108);
  sub_1003DAC24(v75, v113, 78164);
  sub_1003DF514(v75, &unk_100561FF0, v75);
  sub_1003DF124(&v76, &v109, &v109);
  sub_1003DA7B0(&v114, &v109, v106);
  v109 = v114;
  v110 = v115;
  v111 = v116;
  v112 = v117;
  sub_1003DB764(v107, v103, 0);
  sub_1003DF514(&v77, v108, v107);
  sub_1003DF124(&v78, v107, v108);
  sub_1003DAC24(v79, v113, 78164);
  sub_1003DF514(v79, &unk_100561FF0, v79);
  sub_1003DF124(&v80, &v109, &v109);
  sub_1003DA7B0(&v114, &v109, v106);
  v109 = v114;
  v110 = v115;
  v111 = v116;
  v112 = v117;
  sub_1003DB764(v107, v103, 0);
  sub_1003DF514(&v81, v108, v107);
  sub_1003DF124(&v82, v107, v108);
  sub_1003DAC24(v83, v113, 78164);
  sub_1003DF514(v83, &unk_100561FF0, v83);
  sub_1003DF124(&v84, &v109, &v109);
  sub_1003DA7B0(&v114, &v109, v106);
  v109 = v114;
  v110 = v115;
  v111 = v116;
  v112 = v117;
  sub_1003DB764(v107, v103, 0);
  sub_1003DF514(&v85, v108, v107);
  sub_1003DF124(&v86, v107, v108);
  sub_1003DAC24(v87, v113, 78164);
  sub_1003DF514(v87, &unk_100561FF0, v87);
  sub_1003DF124(&v88, &v109, &v109);
  sub_1003DA7B0(&v114, &v109, v106);
  v109 = v114;
  v110 = v115;
  v111 = v116;
  v112 = v117;
  sub_1003DB764(v107, v103, 0);
  sub_1003DF514(&v89, v108, v107);
  sub_1003DF124(&v90, v107, v108);
  sub_1003DAC24(v91, v113, 78164);
  sub_1003DF514(v91, &unk_100561FF0, v91);
  sub_1003DF124(&v92, &v109, &v109);
  sub_1003DA7B0(&v114, &v109, v106);
  v109 = v114;
  v110 = v115;
  v111 = v116;
  v112 = v117;
  sub_1003DB764(v107, v103, 0);
  sub_1003DF514(&v93, v108, v107);
  sub_1003DF124(&v94, v107, v108);
  sub_1003DAC24(v95, v113, 78164);
  sub_1003DF514(v95, &unk_100561FF0, v95);
  sub_1003DF124(&v96, &v109, &v109);
  sub_100391B40(v107, 256);
  result = sub_100391B40(v103, 256);
  v33 = (unsigned int)v100;
  if ( v100 < 0 )
  {
    a1[2] = xmmword_100561F10;
    a1[3] = xmmword_100561F20;
    *a1 = xmmword_100561EF0;
    a1[1] = xmmword_100561F00;
    a1[6] = xmmword_100561F50;
    a1[7] = xmmword_100561F60;
    a1[4] = xmmword_100561F30;
    a1[5] = xmmword_100561F40;
    a1[10] = xmmword_100561F90;
    a1[11] = xmmword_100561FA0;
    a1[8] = xmmword_100561F70;
    a1[9] = xmmword_100561F80;
    a1[14] = xmmword_100561FD0;
    a1[15] = xmmword_100561FE0;
    a1[12] = xmmword_100561FB0;
    a1[13] = xmmword_100561FC0;
    return result;
  }
  v34 = v97;
  if ( v100 <= v97 )
  {
    if ( v100 == v97 )
    {
      v40 = &v65[256 * ((__int64)v101[0] >> 1)];
      sub_1003DF124(&v114, v40 + 64, v40);
      sub_1003DF514(a1 + 4, v40 + 64, v40);
      sub_1003DA7B0(a1 + 12, a1 + 4, &v114);
      v40 += 192;
      sub_1003DA7B0(a1, v40, a1 + 4);
      sub_1003DA7B0(a1 + 4, v40, &v114);
      sub_1003DAD5C(a1 + 8, v40);
      sub_1003DB764(a1, (char *)off_1006752D0 + 192 * (v98[0] >> 1), v33);
      v37 = 1;
      v36 = 1;
      v38 = __OFSUB__((_DWORD)v33, 1);
      v39 = (unsigned int)(v33 - 1);
      if ( (int)v39 < 0 != v38 )
        goto LABEL_60;
    }
    else
    {
      v41 = (char *)off_1006752D0 + 192 * (v98[0] >> 1);
      sub_1003DF124(a1 + 4, v41 + 64, v41);
      sub_1003DF514(a1, v41 + 64, v41);
      sub_1003DA7B0(a1 + 12, a1 + 4, a1);
      v37 = 0;
      a1[8] = xmmword_100562070;
      a1[9] = xmmword_100562080;
      a1[10] = xmmword_100562090;
      a1[11] = xmmword_1005620A0;
      v36 = 1;
      v39 = (unsigned int)(v34 - 1);
      if ( v34 < 1 )
        goto LABEL_60;
    }
LABEL_47:
    v42 = a1 + 8;
    while ( 1 )
    {
      v44 = v101[2 * v37 - 1];
      v45 = v98[2 * v36 - 1];
      sub_1003DAD64(a1, a1, ((_DWORD)v39 != 0) & (unsigned __int8)((_DWORD)v39 != v44 && (_DWORD)v39 != v45));
      if ( (_DWORD)v39 == v44 )
      {
        v46 = (unsigned int)v101[2 * v37];
        if ( (int)v46 < 1 )
        {
          v52 = &v65[256 * (unsigned __int64)((unsigned int)-(int)v46 >> 1)];
          sub_1003DA7B0(&v114, a1 + 8, v52 + 192);
          v53 = v115;
          *v42 = v114;
          a1[9] = v53;
          v54 = v117;
          a1[10] = v116;
          a1[11] = v54;
          sub_1003DE200(a1, v52, ((_DWORD)v39 != 0) & (unsigned __int8)((_DWORD)v39 != v45));
          ++v37;
          if ( (_DWORD)v39 == v45 )
          {
LABEL_56:
            v51 = (unsigned int)v98[2 * v36];
            if ( (int)v51 < 1 )
              sub_1003DE200(a1, (char *)off_1006752D0 + 192 * ((unsigned int)-(int)v51 >> 1), v39);
            else
              sub_1003DB764(a1, (char *)off_1006752D0 + 192 * (v51 >> 1), v39);
            ++v36;
          }
        }
        else
        {
          v47 = (v46 << 7) & 0x7FFFFFFF00LL;
          v48 = &v65[v47];
          sub_1003DA7B0(&v114, a1 + 8, &v68[v47]);
          v49 = v115;
          *v42 = v114;
          a1[9] = v49;
          v50 = v117;
          a1[10] = v116;
          a1[11] = v50;
          sub_1003DB764(a1, v48, ((_DWORD)v39 != 0) & (unsigned __int8)((_DWORD)v39 != v45));
          ++v37;
          if ( (_DWORD)v39 == v45 )
            goto LABEL_56;
        }
      }
      else if ( (_DWORD)v39 == v45 )
      {
        goto LABEL_56;
      }
      v43 = v39 + 1;
      v39 = (unsigned int)(v39 - 1);
      if ( v43 <= 1 )
        goto LABEL_60;
    }
  }
  v35 = &v65[256 * ((__int64)v101[0] >> 1)];
  sub_1003DF124(&v114, v35 + 64, v35);
  sub_1003DF514(a1 + 4, v35 + 64, v35);
  sub_1003DA7B0(a1 + 12, a1 + 4, &v114);
  v35 += 192;
  sub_1003DA7B0(a1, v35, a1 + 4);
  sub_1003DA7B0(a1 + 4, v35, &v114);
  sub_1003DAD5C(a1 + 8, v35);
  v36 = 0;
  v37 = 1;
  v38 = __OFSUB__((_DWORD)v33, 1);
  v39 = (unsigned int)(v33 - 1);
  if ( (int)v39 < 0 == v38 )
    goto LABEL_47;
LABEL_60:
  sub_100391B40(&v100, 912);
  sub_100391B40(&v97, 616);
  return sub_100391B40(v65, 2048);
}

/* ========================================================================
 * fallback sub_1003d85b8
 * EA: 0x1003d85b8
 ======================================================================== */

__int64 __fastcall sub_1003D85B8(__int64 a1, __int64 a2)
{
  int v4; // w20
  _BYTE v6[40]; // [xsp+8h] [xbp-B8h] BYREF
  _BYTE v7[40]; // [xsp+30h] [xbp-90h] BYREF
  _BYTE v8[40]; // [xsp+58h] [xbp-68h] BYREF
  _BYTE v9[40]; // [xsp+80h] [xbp-40h] BYREF

  sub_1003DA00C(v9);
  sub_1003DA00C(v8);
  sub_1003DA00C(v8);
  sub_1003D8E14(v8, a2, v8);
  sub_1003D8E14(v9, v9, v8);
  sub_1003DA00C(v7);
  sub_1003D8E14(v8, v8, v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003D8E14(v8, v7, v8);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003D8E14(v7, v7, v8);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003DA00C(v6);
  sub_1003D8E14(v7, v6, v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003D8E14(v8, v7, v8);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003D8E14(v7, v7, v8);
  sub_1003DA00C(v6);
  v4 = 99;
  do
  {
    sub_1003DA00C(v6);
    --v4;
  }
  while ( v4 );
  sub_1003D8E14(v7, v6, v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003DA00C(v7);
  sub_1003D8E14(v8, v7, v8);
  sub_1003DA00C(v8);
  sub_1003DA00C(v8);
  sub_1003DA00C(v8);
  sub_1003DA00C(v8);
  sub_1003DA00C(v8);
  return sub_1003D8E14(a1, v8, v9);
}

/* ========================================================================
 * fallback sub_1003d235c
 * EA: 0x1003d235c
 ======================================================================== */

unsigned __int16 *__fastcall sub_1003D235C(unsigned __int16 *result)
{
  int v1; // w6
  int v2; // w13
  unsigned __int8 v3; // w14
  unsigned __int64 v4; // x2
  unsigned __int64 v5; // x17
  int v6; // w15
  int v7; // w19
  unsigned __int8 v8; // w14
  unsigned __int64 v9; // x20
  unsigned __int64 v10; // x7
  int v11; // w14
  unsigned __int64 v12; // x21
  unsigned __int64 v13; // x24
  int v14; // w26
  unsigned __int64 v15; // x2
  unsigned __int64 v16; // x10
  unsigned __int64 v17; // x23
  unsigned __int64 v18; // x4
  unsigned __int64 v19; // x17
  unsigned __int64 v20; // x9
  unsigned __int64 v21; // x10
  unsigned __int64 v22; // x8
  unsigned __int64 v23; // x23
  unsigned __int64 v24; // x24
  unsigned __int64 v25; // x25
  unsigned __int64 v26; // x26
  unsigned __int64 v27; // x27
  unsigned __int64 v28; // x22
  unsigned __int64 v29; // x30
  __int64 v30; // x20
  __int64 v31; // x4
  __int64 v32; // x9
  unsigned __int64 v33; // x8
  unsigned __int64 v34; // x19
  __int64 v35; // x7
  __int64 v36; // x21
  __int64 v37; // x17
  unsigned __int64 v38; // x4
  unsigned __int64 v39; // x20
  unsigned __int64 v40; // x19
  unsigned __int64 v41; // x22
  unsigned __int64 v42; // x7
  unsigned __int64 v43; // x25
  unsigned __int64 v44; // x23
  unsigned __int64 v45; // x8
  unsigned __int64 v46; // x9
  unsigned __int64 v47; // x3
  unsigned __int64 v48; // x11
  unsigned __int64 v49; // x12
  unsigned __int64 v50; // x13
  __int64 v51; // x6
  unsigned __int64 v52; // x10
  unsigned __int64 v53; // x7
  __int64 v54; // x6
  unsigned __int64 v55; // x21
  __int64 v56; // x3
  __int64 v57; // x8
  __int64 v58; // x22
  __int64 v59; // x11
  __int64 v60; // x12
  __int64 v61; // x10
  unsigned __int64 v62; // x13
  unsigned __int64 v63; // x6
  unsigned __int64 v64; // x4
  unsigned __int64 v65; // x17
  unsigned __int64 v66; // x7
  __int64 v67; // x23
  __int64 v68; // x11
  __int64 v69; // x12
  signed __int64 v70; // x19
  __int64 v71; // x9
  signed __int64 v72; // x20
  signed __int64 v73; // x3
  signed __int64 v74; // x22
  signed __int64 v75; // x23
  signed __int64 v76; // x24
  __int64 v77; // x25
  signed __int64 v78; // x12
  __int64 v79; // x10
  signed __int64 v80; // x11
  __int64 v81; // x9
  signed __int64 v82; // x8
  __int64 v83; // x13
  __int64 v84; // x16
  __int64 v85; // x15
  __int64 v86; // x1
  __int64 v87; // x2
  __int64 v88; // x17
  __int64 v89; // x14
  __int64 v90; // x12
  __int64 v91; // x10
  __int64 v92; // x11
  __int64 v93; // x9
  unsigned __int64 v94; // x8

  v1 = *((unsigned __int8 *)result + 5);
  v2 = *((unsigned __int8 *)result + 13);
  v3 = *((_BYTE *)result + 18);
  v4 = ((unsigned __int64)(*((unsigned __int8 *)result + 15)
                         | (unsigned __int16)(*((unsigned __int8 *)result + 16) << 8)
                         | (*((unsigned __int8 *)result + 17) << 16) & 0xFFFFFF
                         | (v3 << 24)) >> 6)
     & 0x1FFFFF;
  v5 = v3 & 0xF8
     | (unsigned __int64)(unsigned __int16)(*((unsigned __int8 *)result + 19) << 8)
     | ((unsigned __int64)*((unsigned __int8 *)result + 20) << 16);
  v6 = *((unsigned __int8 *)result + 26);
  v7 = *((unsigned __int8 *)result + 34);
  v8 = *((_BYTE *)result + 39);
  v9 = *((unsigned __int8 *)result + 36)
     | (unsigned __int16)(*((unsigned __int8 *)result + 37) << 8)
     | (*((unsigned __int8 *)result + 38) << 16) & 0xFFFFFF
     | (v8 << 24);
  v10 = v8 & 0xF8
      | (unsigned __int64)(unsigned __int16)(*((unsigned __int8 *)result + 40) << 8)
      | ((unsigned __int64)*((unsigned __int8 *)result + 41) << 16);
  v11 = *((unsigned __int8 *)result + 47);
  v12 = *((unsigned __int8 *)result + 44)
      | (unsigned __int16)(*((unsigned __int8 *)result + 45) << 8)
      | (*((unsigned __int8 *)result + 46) << 16) & 0xFFFFFF
      | ((unsigned __int8)v11 << 24);
  v13 = ((unsigned __int64)(v11 & 0xFF0000FF
                          | (unsigned __int16)(*((unsigned __int8 *)result + 48) << 8)
                          | (*((unsigned __int8 *)result + 49) << 16)) >> 2)
      & 0x1FFFFF;
  v14 = *((unsigned __int8 *)result + 55);
  LOBYTE(v11) = *((_BYTE *)result + 60);
  v15 = v4 + 666643 * v13;
  v16 = 470296 * v13 + (v5 >> 3) + ((v15 + 0x100000) >> 21);
  v17 = ((unsigned __int64)(*((unsigned __int8 *)result + 49)
                          | (unsigned __int16)(*((unsigned __int8 *)result + 50) << 8)
                          | (*((unsigned __int8 *)result + 51) << 16) & 0xFFFFFF
                          | (*((unsigned __int8 *)result + 52) << 24)) >> 7)
      & 0x1FFFFF;
  v18 = (((unsigned __int64)(*((unsigned __int8 *)result + 31)
                           | (unsigned __int16)(*((unsigned __int8 *)result + 32) << 8)
                           | (*((unsigned __int8 *)result + 33) << 16) & 0xFFFFFF
                           | ((unsigned __int8)v7 << 24)) >> 4)
       & 0x1FFFFF)
      - 683901 * v17;
  v19 = ((*((unsigned __int8 *)result + 23) << 16) & 0x1F0000
       | (unsigned __int64)*(unsigned __int16 *)((char *)result + 21))
      + 654183 * v13
      + 470296 * v17;
  v20 = (((unsigned __int64)(v6 & 0xFF0000FF
                           | (unsigned __int16)(*((unsigned __int8 *)result + 27) << 8)
                           | (*((unsigned __int8 *)result + 28) << 16)) >> 2)
       & 0x1FFFFF)
      + 136657 * v13
      - 997805 * v17;
  v21 = v16 + 666643 * v17;
  v22 = (((unsigned __int64)(*((unsigned __int8 *)result + 23)
                           | (unsigned __int16)(*((unsigned __int8 *)result + 24) << 8)
                           | (*((unsigned __int8 *)result + 25) << 16) & 0xFFFFFF
                           | ((unsigned __int8)v6 << 24)) >> 5)
       & 0x1FFFFF)
      - 997805 * v13
      + 654183 * v17;
  v23 = (((unsigned __int64)(*((unsigned __int8 *)result + 28)
                           | (unsigned __int16)(*((unsigned __int8 *)result + 29) << 8)
                           | (*((unsigned __int8 *)result + 30) << 16) & 0xFFFFFF
                           | (*((unsigned __int8 *)result + 31) << 24)) >> 7)
       & 0x1FFFFF)
      - 683901 * v13
      + 136657 * v17;
  v24 = ((unsigned __int64)(*((unsigned __int8 *)result + 52)
                          | (unsigned __int16)(*((unsigned __int8 *)result + 53) << 8)
                          | (*((unsigned __int8 *)result + 54) << 16) & 0xFFFFFF
                          | ((unsigned __int8)v14 << 24)) >> 4)
      & 0x1FFFFF;
  v25 = ((unsigned __int64)(v14 & 0xFF0000FF
                          | (unsigned __int16)(*((unsigned __int8 *)result + 56) << 8)
                          | (*((unsigned __int8 *)result + 57) << 16)) >> 1)
      & 0x1FFFFF;
  v26 = ((unsigned __int64)(*((unsigned __int8 *)result + 57)
                          | (unsigned __int16)(*((unsigned __int8 *)result + 58) << 8)
                          | (*((unsigned __int8 *)result + 59) << 16) & 0xFFFFFF
                          | ((unsigned __int8)v11 << 24)) >> 6)
      & 0x1FFFFF;
  v27 = ((unsigned __int8)v11 & 0xF8
       | (unsigned __int64)(unsigned __int16)(*((unsigned __int8 *)result + 61) << 8)
       | ((unsigned __int64)*((unsigned __int8 *)result + 62) << 16) & 0xFFFFFFFF00FFFFFFLL
       | ((unsigned __int64)*((unsigned __int8 *)result + 63) << 24)) >> 3;
  v28 = (result[21] | (unsigned __int64)((*((unsigned __int8 *)result + 44) << 16) & 0x1F0000)) - 683901 * v27;
  v29 = v19 + 666643 * v24;
  v30 = ((v9 >> 6) & 0x1FFFFF) - 683901 * v25 + 136657 * v26 - 997805 * v27;
  v31 = v18 + 136657 * v24 - 997805 * v25 + 654183 * v26 + 470296 * v27;
  v32 = v20 + 654183 * v24 + 470296 * v25 + 666643 * v26;
  v33 = v22 + 470296 * v24 + 666643 * v25 + ((v29 + 0x100000) >> 21);
  v34 = (((unsigned __int64)(v7 & 0xFF0000FF
                           | (unsigned __int16)(*((unsigned __int8 *)result + 35) << 8)
                           | (*((unsigned __int8 *)result + 36) << 16)) >> 1)
       & 0x1FFFFF)
      - 683901 * v24
      + 136657 * v25
      - 997805 * v26
      + 654183 * v27
      + ((v31 + 0x100000) >> 21);
  v35 = -683901LL * v26 + (v10 >> 3) + 136657 * v27 + ((v30 + 0x100000) >> 21);
  v36 = ((v12 >> 5) & 0x1FFFFF) + ((__int64)(v28 + 0x100000) >> 21);
  v37 = v23 - 997805 * v24 + 654183 * v25 + 470296 * v26 + 666643 * v27 + ((v32 + 0x100000) >> 21);
  v38 = v31 - ((v31 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((v37 + 0x100000) >> 21);
  v39 = v30 - ((v30 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((__int64)(v34 + 0x100000) >> 21);
  v40 = v34 - ((v34 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v41 = v28 - ((v28 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((v35 + 0x100000) >> 21);
  v42 = v35 - ((v35 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v43 = v32 + ((__int64)(v33 + 0x100000) >> 21) - ((v32 + 0x100000) & 0xFFFFFFFFFFE00000LL) - 683901 * v36;
  v44 = v33 - ((v33 + 0x100000) & 0xFFFFFFFFFFE00000LL) + 136657 * v36 - 683901 * v41;
  v45 = v29 + ((v21 + 0x100000) >> 21) - ((v29 + 0x100000) & 0x1FFFFFE00000LL) - 997805 * v36 + 136657 * v41;
  v46 = ((unsigned __int64)(*((unsigned __int8 *)result + 2)
                          | (unsigned __int16)(*((unsigned __int8 *)result + 3) << 8)
                          | (*((unsigned __int8 *)result + 4) << 16) & 0xFFFFFF
                          | ((unsigned __int8)v1 << 24)) >> 5)
      & 0x1FFFFF;
  v47 = ((unsigned __int64)(v1 & 0xFF0000FF
                          | (unsigned __int16)(*((unsigned __int8 *)result + 6) << 8)
                          | (*((unsigned __int8 *)result + 7) << 16)) >> 2)
      & 0x1FFFFF;
  v48 = (((unsigned __int64)(*((unsigned __int8 *)result + 7)
                           | (unsigned __int16)(*((unsigned __int8 *)result + 8) << 8)
                           | (*((unsigned __int8 *)result + 9) << 16) & 0xFFFFFF
                           | (*((unsigned __int8 *)result + 10) << 24)) >> 7)
       & 0x1FFFFF)
      + 666643 * v42;
  v49 = (((unsigned __int64)(*((unsigned __int8 *)result + 10)
                           | (unsigned __int16)(*((unsigned __int8 *)result + 11) << 8)
                           | (*((unsigned __int8 *)result + 12) << 16) & 0xFFFFFF
                           | ((unsigned __int8)v2 << 24)) >> 4)
       & 0x1FFFFF)
      + 666643 * v41
      + 470296 * v42;
  v50 = (((unsigned __int64)(v2 & 0xFF0000FF
                           | (unsigned __int16)(*((unsigned __int8 *)result + 14) << 8)
                           | (*((unsigned __int8 *)result + 15) << 16)) >> 1)
       & 0x1FFFFF)
      + 666643 * v36
      + 470296 * v41
      + 654183 * v42;
  v51 = v15 - ((v15 + 0x100000) & 0x7FFFFE00000LL) + 470296 * v36 + 654183 * v41 - 997805 * v42;
  v52 = v21 - ((v21 + 0x100000) & 0xFFFFFFFFFFE00000LL) + 654183 * v36 - 997805 * v41 + 136657 * v42;
  v53 = v45 - 683901 * v42;
  v54 = v51 + 136657 * v39 - 683901 * v40;
  v55 = (*result | (unsigned __int64)((*((unsigned __int8 *)result + 2) << 16) & 0x1F0000)) + 666643 * v38;
  v56 = v47 + 654183 * v38 + 666643 * v39 + 470296 * v40;
  v57 = v49 + 136657 * v38 + 654183 * v39 - 997805 * v40;
  v58 = v46 + 470296 * v38 + 666643 * v40 + ((__int64)(v55 + 0x100000) >> 21);
  v59 = v48 - 997805 * v38 + 470296 * v39 + 654183 * v40;
  v60 = v50 - 683901 * v38 - 997805 * v39 + 136657 * v40;
  v61 = v52 - 683901 * v39 + ((v54 + 0x100000) >> 21);
  v62 = v54 - ((v54 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v63 = v44 + ((__int64)(v53 + 0x100000) >> 21);
  v64 = v53 - ((v53 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v65 = v37 - ((v37 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((__int64)(v43 + 0x100000) >> 21);
  v66 = v43 - ((v43 + 0x100000) & 0xFFFFFFFFFFE00000LL);
  v67 = (__int64)(v65 + 0x100000) >> 21;
  v68 = v59 + ((v56 + 0x100000) >> 21);
  v69 = v60 + ((v57 + 0x100000) >> 21);
  v70 = v55 - ((v55 + 0x100000) & 0xFFFFFFFFFFE00000LL) + 666643 * v67;
  v71 = v69 - 683901 * v67;
  v69 += 0x100000;
  v72 = v58 + 470296 * v67 - ((v58 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v70 >> 21);
  v73 = v56 + 654183 * v67 - ((v56 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((v58 + 0x100000) >> 21) + (v72 >> 21);
  v74 = v68 - 997805 * v67 - ((v68 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v73 >> 21);
  v75 = v57 + 136657 * v67 - ((v57 + 0x100000) & 0xFFFFFFFFFFE00000LL) + ((v68 + 0x100000) >> 21) + (v74 >> 21);
  v76 = v71 - (v69 & 0xFFFFFFFFFFE00000LL) + (v75 >> 21);
  v77 = v62 + (v69 >> 21) + (v76 >> 21);
  v78 = v61 - ((v61 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v77 >> 21);
  v79 = v64 + ((v61 + 0x100000) >> 21) + (v78 >> 21);
  v80 = v63 - ((v63 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v79 >> 21);
  v81 = v66 + ((__int64)(v63 + 0x100000) >> 21) + (v80 >> 21);
  v82 = v65 - ((v65 + 0x100000) & 0xFFFFFFFFFFE00000LL) + (v81 >> 21);
  v83 = (v70 & 0x1FFFFF) + 666643 * (v82 >> 21);
  v84 = (v72 & 0x1FFFFF) + 470296 * (v82 >> 21) + (v83 >> 21);
  v85 = (v73 & 0x1FFFFF) + 654183 * (v82 >> 21) + (v84 >> 21);
  v86 = (v74 & 0x1FFFFF) - 997805 * (v82 >> 21) + (v85 >> 21);
  v87 = (v75 & 0x1FFFFF) + 136657 * (v82 >> 21) + (v86 >> 21);
  v88 = (v76 & 0x1FFFFF) - 683901 * (v82 >> 21) + (v87 >> 21);
  v89 = (v77 & 0x1FFFFF) + (v88 >> 21);
  *result = v70 + 11283 * (v82 >> 21);
  *((_BYTE *)result + 2) = BYTE2(v83) & 0x1F | (32 * (v72 + 24 * (v82 >> 21) + (v83 >> 21)));
  *((_BYTE *)result + 3) = (unsigned __int64)v84 >> 3;
  *((_BYTE *)result + 4) = (unsigned __int64)v84 >> 11;
  *((_BYTE *)result + 5) = ((unsigned int)v84 >> 19) & 3 | (4 * (v73 + 103 * (v82 >> 21) + (v84 >> 21)));
  *((_BYTE *)result + 6) = (unsigned __int64)v85 >> 6;
  *((_BYTE *)result + 7) = ((unsigned int)v85 >> 14) & 0x7F
                         | (((_BYTE)v74 - -83 * (unsigned __int8)(v82 >> 21) + (unsigned __int8)(v85 >> 21)) << 7);
  *((_BYTE *)result + 8) = (unsigned __int64)v86 >> 1;
  *((_BYTE *)result + 9) = (unsigned __int64)v86 >> 9;
  *((_BYTE *)result + 10) = ((unsigned int)v86 >> 17) & 0xF | (16 * (v75 - 47 * (v82 >> 21) + (v86 >> 21)));
  *((_BYTE *)result + 11) = (unsigned __int64)v87 >> 4;
  *((_BYTE *)result + 12) = (unsigned __int64)v87 >> 12;
  *((_BYTE *)result + 13) = ((v87 & 0x100000) != 0) | (2 * (v76 - 125 * (v82 >> 21) + (v87 >> 21)));
  *((_BYTE *)result + 14) = (unsigned __int64)v88 >> 7;
  *((_BYTE *)result + 15) = ((unsigned int)v88 >> 15) & 0x3F | (((_BYTE)v77 + (unsigned __int8)(v88 >> 21)) << 6);
  *((_BYTE *)result + 16) = (unsigned __int64)v89 >> 2;
  *((_BYTE *)result + 17) = (unsigned __int64)v89 >> 10;
  v90 = (v78 & 0x1FFFFF) + (v89 >> 21);
  *((_BYTE *)result + 18) = ((unsigned int)v89 >> 18) & 7 | (8 * v90);
  *((_BYTE *)result + 19) = (unsigned __int64)v90 >> 5;
  v91 = (v79 & 0x1FFFFF) + (v90 >> 21);
  v92 = (v80 & 0x1FFFFF) + (v91 >> 21);
  *((_BYTE *)result + 20) = (unsigned __int64)v90 >> 13;
  *(unsigned __int16 *)((char *)result + 21) = v91;
  v93 = (v81 & 0x1FFFFF) + (v92 >> 21);
  *((_BYTE *)result + 23) = BYTE2(v91) & 0x1F | (32 * v92);
  *((_BYTE *)result + 24) = (unsigned __int64)v92 >> 3;
  *((_BYTE *)result + 25) = (unsigned __int64)v92 >> 11;
  *((_BYTE *)result + 26) = ((unsigned int)v92 >> 19) & 3 | (4 * v93);
  *((_BYTE *)result + 27) = (unsigned __int64)v93 >> 6;
  v94 = (v82 & 0x1FFFFF) + (v93 >> 21);
  *((_BYTE *)result + 28) = ((unsigned int)v93 >> 14) & 0x7F | ((_BYTE)v94 << 7);
  *((_BYTE *)result + 29) = v94 >> 1;
  *((_BYTE *)result + 30) = v94 >> 9;
  *((_BYTE *)result + 31) = v94 >> 17;
  return result;
}

/* ========================================================================
 * fallback sub_10013db98
 * EA: 0x10013db98
 ======================================================================== */

void sub_10013DB98()
{
  char *v0; // x20
  char *v1; // x27
  __int64 v2; // x21
  __int64 v3; // x22
  char *v4; // x23
  __int64 v5; // x24
  __int64 v6; // x25
  __int64 v7; // x26
  char *v8; // x28
  void *v9; // x0
  id v10; // x0
  void *v11; // x20
  id v12; // x21
  __int64 v13; // x26
  __int64 v14; // x21
  __int128 v15; // q1
  __int64 v16; // x25
  void *v17; // x20
  id v18; // x22
  id v19; // x24
  __int64 v20; // x23
  unsigned int v21; // w0
  __int64 v22; // x25
  __int128 v23; // q1
  __int64 v24; // x22
  void *v25; // x20
  id v26; // x21
  id v27; // x24
  double v28; // d8
  __int64 v29; // x20
  __int128 v30; // q1
  __int64 v31; // x21
  void *v32; // x24
  id v33; // x22
  id v34; // x24
  __int64 v35; // x0
  char *v36; // x20
  id v37; // x0
  void *v38; // x21
  char *v39; // x20
  void *v40; // x21
  char *v41; // x23
  char v42; // w20
  void *v43; // x0
  __int64 v44; // x25
  __int128 v45; // q1
  __int64 v46; // x22
  void *v47; // x20
  id v48; // x24
  id v49; // x26
  __int64 v50; // x21
  void *v51; // x25
  char *v52; // x24
  __int64 v53; // x0
  void *v54; // x21
  __int64 v55; // x20
  char *v56; // x0
  __int64 v57; // x0
  char *v58; // x28
  __int64 v59; // x0
  __int64 v60; // x22
  __int64 v61; // x20
  __int64 v62; // x0
  char *v63; // x27
  __int64 v64; // x26
  __int64 v65; // [xsp+0h] [xbp-240h] BYREF
  __int64 v66; // [xsp+8h] [xbp-238h]
  __int64 v67; // [xsp+10h] [xbp-230h]
  __int64 v68; // [xsp+18h] [xbp-228h]
  char *v69; // [xsp+20h] [xbp-220h]
  __int64 v70; // [xsp+28h] [xbp-218h]
  __int64 v71; // [xsp+30h] [xbp-210h]
  char *v72; // [xsp+38h] [xbp-208h]
  char *v73; // [xsp+40h] [xbp-200h]
  __int64 v74; // [xsp+48h] [xbp-1F8h]
  _QWORD v75[5]; // [xsp+50h] [xbp-1F0h] BYREF
  __int64 v76; // [xsp+78h] [xbp-1C8h]
  char v77; // [xsp+87h] [xbp-1B9h] BYREF
  __int64 v78; // [xsp+88h] [xbp-1B8h] BYREF
  __int64 v79; // [xsp+A0h] [xbp-1A0h] BYREF
  __int64 v80; // [xsp+B8h] [xbp-188h] BYREF
  __int128 v81; // [xsp+D0h] [xbp-170h]
  __int128 v82; // [xsp+E0h] [xbp-160h]
  __int64 v83; // [xsp+F0h] [xbp-150h]
  __int128 v84; // [xsp+100h] [xbp-140h]
  __int128 v85; // [xsp+110h] [xbp-130h]
  __int64 v86; // [xsp+120h] [xbp-120h]
  __int128 v87; // [xsp+130h] [xbp-110h]
  __int128 v88; // [xsp+140h] [xbp-100h]
  __int64 v89; // [xsp+150h] [xbp-F0h]
  __int128 v90; // [xsp+160h] [xbp-E0h]
  __int128 v91; // [xsp+170h] [xbp-D0h]
  __int64 v92; // [xsp+180h] [xbp-C0h]
  __int128 v93; // [xsp+190h] [xbp-B0h] BYREF
  __int128 v94; // [xsp+1A0h] [xbp-A0h] BYREF
  __int128 v95; // [xsp+1B0h] [xbp-90h] BYREF
  _OWORD v96[2]; // [xsp+1C0h] [xbp-80h] BYREF

  v1 = v0;
  v2 = type metadata accessor for DispatchWorkItemFlags(0);
  v3 = *(_QWORD *)(v2 - 8);
  v4 = (char *)&v65 - ((*(_QWORD *)(v3 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for DispatchQoS(0);
  v6 = *(_QWORD *)(v5 - 8);
  v73 = (char *)&v65 - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v74 = type metadata accessor for DispatchTime(0);
  v7 = *(_QWORD *)(v74 - 8);
  v8 = (char *)&v65 - ((*(_QWORD *)(v7 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v72 = v8;
  sub_10014129C();
  v9 = *(void **)(*(_QWORD *)&v0[OBJC_IVAR____TtC15ExternalMonitor26VTFullScreenViewController_innerVC]
                + OBJC_IVAR____TtC15ExternalMonitor20VTBaseViewController_fsToolBar);
  if ( v9 )
    objc_msgSend(v9, "setHidden:", 1);
  v10 = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "view"));
  if ( !v10 )
  {
    __break(1u);
LABEL_22:
    __break(1u);
    return;
  }
  v11 = v10;
  v66 = v7;
  v67 = v6;
  v68 = v5;
  v69 = v4;
  v70 = v3;
  v71 = v2;
  v12 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___UIColor), "blackColor"));
  objc_msgSend(v11, "setBackgroundColor:", v12);
  objc_release(v11);
  objc_release(v12);
  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  v13 = qword_100696D00;
  v14 = qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ambientModePlacement;
  swift_beginAccess(
    qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ambientModePlacement,
    &v80,
    0,
    0);
  v15 = *(_OWORD *)(v14 + 16);
  v90 = *(_OWORD *)v14;
  v91 = v15;
  v92 = *(_QWORD *)(v14 + 32);
  v16 = *((_QWORD *)&v90 + 1);
  v17 = (void *)v15;
  v93 = *(_OWORD *)(v14 + 24);
  v18 = objc_retain((id)v90);
  swift_retain(v16);
  v19 = objc_retain(v17);
  sub_10004542C(&v93, v75);
  v20 = sub_10003E4E0(&unk_10066AFA0, &unk_10053BAB0);
  WrappedDefault.wrappedValue.getter(v75, v20);
  objc_release(v19);
  swift_release(v16);
  objc_release(v18);
  sub_10003F604(&v93);
  v21 = sub_1000AB39C(v75[0]);
  if ( (unsigned __int8)v21 == 5 )
    v22 = 0;
  else
    v22 = v21;
  v23 = *(_OWORD *)(v14 + 16);
  v87 = *(_OWORD *)v14;
  v88 = v23;
  v89 = *(_QWORD *)(v14 + 32);
  v24 = *((_QWORD *)&v87 + 1);
  v25 = (void *)v23;
  v94 = *(_OWORD *)(v14 + 24);
  v26 = objc_retain((id)v87);
  swift_retain(v24);
  v27 = objc_retain(v25);
  sub_10004542C(&v94, v75);
  WrappedDefault.wrappedValue.getter(v75, v20);
  objc_release(v27);
  swift_release(v24);
  objc_release(v26);
  sub_10003F604(&v94);
  if ( v75[0] == 4 )
    v28 = 0.4;
  else
    v28 = 0.0;
  v29 = v13 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ambientModeScalePercent;
  swift_beginAccess(v13 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__ambientModeScalePercent, &v79, 0, 0);
  v30 = *(_OWORD *)(v29 + 16);
  v84 = *(_OWORD *)v29;
  v85 = v30;
  v86 = *(_QWORD *)(v29 + 32);
  v31 = *((_QWORD *)&v84 + 1);
  v32 = (void *)v30;
  v95 = *(_OWORD *)(v29 + 24);
  v33 = objc_retain((id)v84);
  swift_retain(v31);
  v34 = objc_retain(v32);
  sub_10004542C(&v95, v75);
  WrappedDefault.wrappedValue.getter(v75, v20);
  objc_release(v34);
  swift_release(v31);
  objc_release(v33);
  sub_10003F604(&v95);
  sub_100142220(v22, v28 + (double)v75[0] / 100.0);
  v35 = type metadata accessor for VTSettingButtonWrapper(0);
  v36 = (char *)objc_msgSend(objc_allocWithZone((Class)swift_getObjCClassFromMetadata(v35)), "init");
  swift_unknownObjectWeakAssign(&v36[OBJC_IVAR____TtC15ExternalMonitor22VTSettingButtonWrapper_fsVC], v1);
  v37 = objc_retainAutoreleasedReturnValue(objc_msgSend(v1, "view"));
  if ( !v37 )
    goto LABEL_22;
  v38 = v37;
  v39 = objc_retain(objc_retain(v36));
  objc_msgSend(v38, "addSubview:", v39);
  objc_release(v38);
  ConstraintViewDSL.makeConstraints(_:)(sub_10014270C, 0, v39);
  objc_release(v39);
  v40 = *(void **)&v1[OBJC_IVAR____TtC15ExternalMonitor26VTFullScreenViewController_settingButtonWrapper];
  *(_QWORD *)&v1[OBJC_IVAR____TtC15ExternalMonitor26VTFullScreenViewController_settingButtonWrapper] = v39;
  v41 = objc_retain(v39);
  objc_release(v40);
  v42 = sub_1000E0A60();
  objc_msgSend(v41, "setHidden:", v42 & 1);
  objc_release(v41);
  v43 = *(void **)&v1[OBJC_IVAR____TtC15ExternalMonitor26VTFullScreenViewController_fsToolBar];
  if ( v43 )
    objc_msgSend(v43, "setHidden:", v42 & 1);
  if ( (v42 & 1) == 0 )
  {
    v44 = v13 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__firstEnterAmbientMode;
    swift_beginAccess(v13 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__firstEnterAmbientMode, &v78, 0, 0);
    v45 = *(_OWORD *)(v44 + 16);
    v81 = *(_OWORD *)v44;
    v82 = v45;
    v83 = *(_QWORD *)(v44 + 32);
    v46 = *((_QWORD *)&v81 + 1);
    v47 = (void *)v45;
    v96[0] = *(_OWORD *)(v44 + 24);
    v48 = objc_retain((id)v81);
    swift_retain(v46);
    v49 = objc_retain(v47);
    sub_10004542C(v96, v75);
    v50 = sub_10003E4E0(&unk_10066A970, &unk_10053BAD0);
    WrappedDefault.wrappedValue.getter(v75, v50);
    objc_release(v49);
    swift_release(v46);
    objc_release(v48);
    sub_10003F604(v96);
    if ( LOBYTE(v75[0]) == 1 )
    {
      v77 = 0;
      swift_beginAccess(v44, v75, 33, 0);
      WrappedDefault.wrappedValue.setter(&v77, v50);
      swift_endAccess(v75);
      sub_10004B4D8(0, &qword_100668B30, &classRef_OS_dispatch_queue);
      v51 = (void *)static OS_dispatch_queue.main.getter();
      static DispatchTime.now()();
      v52 = v72;
      + infix(_:_:)(v8, 0.5);
      v66 = *(_QWORD *)(v66 + 8);
      ((void (__fastcall *)(char *, __int64))v66)(v8, v74);
      v53 = swift_allocObject(&unk_1005C0E10, 24, 7);
      *(_QWORD *)(v53 + 16) = v1;
      v75[4] = sub_100142940;
      v76 = v53;
      v75[0] = _NSConcreteStackBlock;
      v75[1] = 1107296256;
      v75[2] = sub_100040600;
      v75[3] = &unk_1005C0E28;
      v54 = _Block_copy(v75);
      v55 = v76;
      v56 = objc_retain(v1);
      v57 = swift_release(v55);
      v58 = v73;
      v59 = static DispatchQoS.unspecified.getter(v57);
      v75[0] = &_swiftEmptyArrayStorage;
      v60 = sub_1000406E0(v59);
      v61 = sub_10003E4E0(&unk_100668B40, &unk_10053B8F0);
      v62 = sub_100040724();
      v63 = v69;
      v64 = v71;
      dispatch thunk of SetAlgebra.init<A>(_:)(v75, v61, v62, v71, v60);
      OS_dispatch_queue.asyncAfter(deadline:qos:flags:execute:)(v52, v58, v63, v54);
      _Block_release(v54);
      objc_release(v51);
      (*(void (__fastcall **)(char *, __int64))(v70 + 8))(v63, v64);
      (*(void (__fastcall **)(char *, __int64))(v67 + 8))(v58, v68);
      ((void (__fastcall *)(char *, __int64))v66)(v52, v74);
    }
  }
  if ( qword_1006625A0 != -1 )
    swift_once(&qword_1006625A0, sub_100240480);
  sub_10007507C();
  objc_release(v41);
}

/* ========================================================================
 * fallback sub_1003df770
 * EA: 0x1003df770
 ======================================================================== */

__int64 __fastcall sub_1003DF770(_OWORD *a1, __int64 a2)
{
  int v4; // w21
  int v5; // w21
  __int128 v6; // q1
  __int128 v7; // q1
  int8x16_t v8; // q0
  int8x16_t v10[4]; // [xsp+0h] [xbp-130h] BYREF
  _BYTE v11[64]; // [xsp+40h] [xbp-F0h] BYREF
  _OWORD v12[4]; // [xsp+80h] [xbp-B0h] BYREF
  _BYTE v13[72]; // [xsp+C0h] [xbp-70h] BYREF

  sub_1003DAD5C(v12, a2);
  sub_1003DA7B0(v11, a2, v12);
  sub_1003DAD5C(v12, v11);
  sub_1003DA7B0(v11, a2, v12);
  sub_1003DAD5C(v12, v11);
  sub_1003DAD5C(v10, v12);
  sub_1003DAD5C(v12, v10);
  sub_1003DA7B0(v13, v11, v12);
  sub_1003DAD5C(v12, v13);
  sub_1003DAD5C(v10, v12);
  sub_1003DAD5C(v12, v10);
  sub_1003DA7B0(v13, v11, v12);
  sub_1003DAD5C(v11, v13);
  sub_1003DAD5C(v10, v11);
  sub_1003DAD5C(v11, v10);
  sub_1003DAD5C(v10, v11);
  sub_1003DAD5C(v11, v10);
  sub_1003DAD5C(v10, v11);
  sub_1003DAD5C(v11, v10);
  sub_1003DAD5C(v10, v11);
  sub_1003DAD5C(v11, v10);
  sub_1003DA7B0(v12, v13, v11);
  sub_1003DAD5C(v13, v12);
  sub_1003DA7B0(v11, a2, v13);
  sub_1003DAD5C(v10, v11);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DA7B0(v11, v12, v13);
  sub_1003DAD5C(v13, v11);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DA7B0(v12, v11, v13);
  sub_1003DAD5C(v13, v12);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DAD5C(v10, v13);
  sub_1003DAD5C(v13, v10);
  sub_1003DA7B0(v12, v11, v13);
  sub_1003DAD5C(v13, v12);
  v4 = -110;
  do
  {
    sub_1003DAD5C(v10, v13);
    sub_1003DAD5C(v13, v10);
    v4 += 2;
  }
  while ( v4 );
  sub_1003DA7B0(v11, v12, v13);
  sub_1003DAD5C(v13, v11);
  sub_1003DA7B0(v12, a2, v13);
  sub_1003DAD5C(v13, v12);
  v5 = -222;
  do
  {
    sub_1003DAD5C(v10, v13);
    sub_1003DAD5C(v13, v10);
    v5 += 2;
  }
  while ( v5 );
  sub_1003DA7B0(v12, v11, v13);
  sub_1003DAD5C(v11, v12);
  sub_1003DA7B0(v13, v11, a2);
  v6 = v12[1];
  *a1 = v12[0];
  a1[1] = v6;
  v7 = v12[3];
  a1[2] = v12[2];
  a1[3] = v7;
  sub_1003DF514(v10, v13, &unk_1005674F0);
  sub_1003DEE5C(v10);
  v8 = vorrq_s8(vorrq_s8(v10[0], v10[2]), vorrq_s8(v10[1], v10[3]));
  *(int8x8_t *)v8.i8 = vorr_s8(*(int8x8_t *)v8.i8, (int8x8_t)vextq_s8(v8, v8, 8u));
  return (unsigned int)((((v8.i32[0] | v8.i32[1]) - 1) & ~(v8.i32[0] | v8.i32[1])) >> 31);
}

/* ========================================================================
 * fallback sub_10044c3dc
 * EA: 0x10044c3dc
 ======================================================================== */

__int64 __fastcall sub_10044C3DC(__int64 a1, __int64 a2)
{
  __int64 v4; // x0
  __int64 v5; // x19
  int v6; // w0
  __int64 v7; // x22
  __int64 v8; // x2
  __int64 v9; // x4
  __int64 v10; // x20
  __int64 v12; // x1
  __int64 v13; // x2
  __int64 v14; // x4
  __int64 v15; // x8
  __int64 v16; // x22
  __int64 v17; // x23
  __int64 *v18; // x8
  __int64 v19; // x0
  __int64 v20; // x8
  __int64 v21; // x0
  __int64 v22; // x22
  __int64 *v23; // x8
  _DWORD *v24; // x8
  _DWORD *v25; // x8
  __int64 v26; // x0
  __int64 v27; // x24
  __int64 v28; // x0
  _QWORD *v29; // x26
  __int64 v30; // x0
  __int64 v31; // x27
  __int64 v32; // x0
  __int64 v33; // x0
  __int64 v34; // x0
  __int64 v35; // x27
  __int64 v36; // x0
  __int64 v37; // x21
  size_t v38; // [xsp+8h] [xbp-A8h] BYREF
  unsigned int __n; // [xsp+14h] [xbp-9Ch] BYREF
  _QWORD __n_4[8]; // [xsp+18h] [xbp-98h] BYREF

  if ( !a1 )
  {
    v8 = 143;
    v9 = 657;
LABEL_9:
    sub_100420B08(33, 128, v8, "crypto/pkcs7/pk7_doit.c", v9);
    return 0;
  }
  if ( !*(_QWORD *)(a1 + 32) )
  {
    v8 = 122;
    v9 = 662;
    goto LABEL_9;
  }
  v4 = sub_10042384C();
  if ( !v4 )
  {
    v8 = 65;
    v9 = 668;
    goto LABEL_9;
  }
  v5 = v4;
  v6 = sub_1004458B4(*(_QWORD *)(a1 + 24));
  *(_DWORD *)(a1 + 16) = 0;
  switch ( v6 )
  {
    case 21:
      v7 = *(_QWORD *)(a1 + 32);
      goto LABEL_62;
    case 22:
      v15 = *(_QWORD *)(a1 + 32);
      v17 = *(_QWORD *)(v15 + 32);
      v16 = *(_QWORD *)(v15 + 40);
      if ( (unsigned int)sub_1004458B4(*(_QWORD *)(v16 + 24)) == 21 )
      {
        v18 = (__int64 *)(v16 + 32);
LABEL_15:
        v7 = *v18;
        goto LABEL_42;
      }
      if ( (unsigned int)sub_1004458B4(*(_QWORD *)(v16 + 24)) - 21 >= 6 )
      {
        v24 = *(_DWORD **)(v16 + 32);
        if ( v24 )
        {
          if ( *v24 == 4 )
          {
            v18 = (__int64 *)(v24 + 2);
            goto LABEL_15;
          }
        }
      }
      v7 = 0;
LABEL_42:
      if ( (unsigned int)sub_1004458B4(*(_QWORD *)(*(_QWORD *)(*(_QWORD *)(a1 + 32) + 40LL) + 24LL)) == 21
        && *(_DWORD *)(a1 + 20) )
      {
        sub_10039EAF0(v7);
        v7 = 0;
        *(_QWORD *)(*(_QWORD *)(*(_QWORD *)(a1 + 32) + 40LL) + 32LL) = 0;
      }
LABEL_45:
      if ( !v17 || (int)sub_10046E3BC(v17) < 1 )
        goto LABEL_62;
      v27 = 0;
      break;
    case 23:
      v7 = *(_QWORD *)(*(_QWORD *)(*(_QWORD *)(a1 + 32) + 16LL) + 16LL);
      if ( v7 )
        goto LABEL_62;
      v19 = sub_10039EAE8();
      if ( v19 )
      {
        v7 = v19;
        *(_QWORD *)(*(_QWORD *)(*(_QWORD *)(a1 + 32) + 16LL) + 16LL) = v19;
        goto LABEL_62;
      }
      v12 = 128;
      v13 = 65;
      v14 = 698;
      goto LABEL_12;
    case 24:
      v20 = *(_QWORD *)(a1 + 32);
      v17 = *(_QWORD *)(v20 + 32);
      v7 = *(_QWORD *)(*(_QWORD *)(v20 + 40) + 16LL);
      if ( v7 )
        goto LABEL_45;
      v21 = sub_10039EAE8();
      if ( v21 )
      {
        v7 = v21;
        *(_QWORD *)(*(_QWORD *)(*(_QWORD *)(a1 + 32) + 40LL) + 16LL) = v21;
        goto LABEL_45;
      }
      v12 = 128;
      v13 = 65;
      v14 = 686;
      goto LABEL_12;
    case 25:
      v22 = *(_QWORD *)(*(_QWORD *)(a1 + 32) + 16LL);
      if ( (unsigned int)sub_1004458B4(*(_QWORD *)(v22 + 24)) == 21 )
      {
        v23 = (__int64 *)(v22 + 32);
      }
      else
      {
        if ( (unsigned int)sub_1004458B4(*(_QWORD *)(v22 + 24)) - 21 < 6
          || (v25 = *(_DWORD **)(v22 + 32)) == nullptr
          || *v25 != 4 )
        {
          v7 = 0;
LABEL_34:
          if ( (unsigned int)sub_1004458B4(*(_QWORD *)(*(_QWORD *)(*(_QWORD *)(a1 + 32) + 16LL) + 24LL)) == 21
            && *(_DWORD *)(a1 + 20) )
          {
            sub_10039EAF0(v7);
            v7 = 0;
            *(_QWORD *)(*(_QWORD *)(*(_QWORD *)(a1 + 32) + 16LL) + 32LL) = 0;
          }
          v26 = sub_1004458B4(**(_QWORD **)(*(_QWORD *)(a1 + 32) + 8LL));
          if ( !sub_10044C9DC(&v38, a2, v26)
            || !(unsigned int)sub_100423C14(v38, __n_4, &__n)
            || !(unsigned int)sub_100394DD8(*(_QWORD *)(*(_QWORD *)(a1 + 32) + 24LL), (char *)__n_4, __n) )
          {
            goto LABEL_68;
          }
LABEL_62:
          if ( (unsigned int)sub_1004458B4(*(_QWORD *)(a1 + 24)) == 22 && sub_10044CDB8(a1, 2, 0, 0) )
            goto LABEL_69;
          if ( !v7 )
            goto LABEL_68;
          if ( (*(_BYTE *)(v7 + 16) & 0x10) != 0 )
          {
LABEL_69:
            v10 = 1;
          }
          else
          {
            v36 = sub_1003A33D8(a2, 1025);
            v10 = v36;
            if ( v36 )
            {
              v37 = sub_1003A308C(v36, 3, 0, __n_4);
              sub_1003A2750(v10, 512);
              sub_1003A308C(v10, 130, 0, 0);
              sub_100399688(v7, __n_4[0], v37);
              v10 = 1;
            }
            else
            {
              sub_100420B08(33, 128, 107, "crypto/pkcs7/pk7_doit.c", 800);
            }
          }
          goto LABEL_70;
        }
        v23 = (__int64 *)(v25 + 2);
      }
      v7 = *v23;
      goto LABEL_34;
    default:
      v12 = 128;
      v13 = 112;
      v14 = 726;
      goto LABEL_12;
  }
  while ( 1 )
  {
    v28 = sub_10046E3D0(v17, v27);
    if ( !*(_QWORD *)(v28 + 56) )
      goto LABEL_48;
    v29 = (_QWORD *)v28;
    v30 = sub_1004458B4(**(_QWORD **)(v28 + 16));
    if ( !sub_10044C9DC(&v38, a2, v30) || !(unsigned int)sub_100423D74(v5, v38) )
      goto LABEL_68;
    if ( (int)sub_10046E3BC(v29[3]) >= 1 )
      break;
    LODWORD(__n_4[0]) = sub_100433418(v29[7]);
    v34 = sub_10043C8B8(LODWORD(__n_4[0]), "crypto/pkcs7/pk7_doit.c", 764);
    if ( !v34 )
      goto LABEL_68;
    v35 = v34;
    if ( !(unsigned int)sub_100433D78(v5, v34, __n_4, v29[7]) )
    {
      sub_10043CA70(v35, "crypto/pkcs7/pk7_doit.c", 769);
      v12 = 128;
      v13 = 6;
      v14 = 770;
      goto LABEL_12;
    }
    sub_100399688(v29[5], v35, LODWORD(__n_4[0]));
LABEL_48:
    v27 = (unsigned int)(v27 + 1);
    if ( (int)v27 >= (int)sub_10046E3BC(v17) )
      goto LABEL_62;
  }
  v31 = v29[3];
  v32 = sub_100470BB0(v31, 52, 0xFFFFFFFFLL);
  v33 = sub_100470C5C(v31, v32);
  if ( sub_100471078(v33, 0) || (unsigned int)sub_10044BB40(v29, 0) )
  {
    if ( !(unsigned int)sub_100423C14(v5, __n_4, &__n) )
    {
      v12 = 136;
      v13 = 6;
      v14 = 630;
      goto LABEL_12;
    }
    if ( !(unsigned int)sub_10044BBAC(v29, __n_4, __n) )
    {
      v12 = 136;
      v13 = 65;
      v14 = 634;
      goto LABEL_12;
    }
    if ( !(unsigned int)sub_10044CA9C(v29) )
      goto LABEL_68;
    goto LABEL_48;
  }
  v12 = 136;
  v13 = 65;
  v14 = 623;
LABEL_12:
  sub_100420B08(33, v12, v13, "crypto/pkcs7/pk7_doit.c", v14);
LABEL_68:
  v10 = 0;
LABEL_70:
  sub_100423860(v5);
  return v10;
}

/* ========================================================================
 * fallback sub_1003d2ac0
 * EA: 0x1003d2ac0
 ======================================================================== */

__int64 __fastcall sub_1003D2AC0(__int64 a1, unsigned __int8 *a2)
{
  __int64 v3; // x8
  int v4; // w9
  unsigned int v5; // w10
  unsigned int v6; // w10
  unsigned int v7; // w10
  unsigned int v8; // w10
  unsigned int v9; // w10
  unsigned int v10; // w10
  unsigned int v11; // w10
  unsigned int v12; // w10
  unsigned int v13; // w10
  unsigned int v14; // w10
  unsigned int v15; // w10
  unsigned int v16; // w10
  unsigned int v17; // w10
  unsigned int v18; // w10
  unsigned int v19; // w10
  unsigned int v20; // w10
  unsigned int v21; // w10
  unsigned int v22; // w10
  unsigned int v23; // w10
  unsigned int v24; // w10
  unsigned int v25; // w10
  unsigned int v26; // w10
  unsigned int v27; // w10
  unsigned int v28; // w10
  unsigned int v29; // w10
  unsigned int v30; // w10
  unsigned int v31; // w10
  unsigned int v32; // w10
  unsigned int v33; // w10
  unsigned int v34; // w10
  unsigned int v35; // w10
  unsigned int v36; // w10
  int v37; // w11
  __int64 v38; // x26
  unsigned __int64 v39; // x28
  _OWORD *v40; // x21
  bool v41; // cf
  __int128 v42; // q1
  __int64 v43; // x8
  __int128 v44; // q1
  __int64 v45; // x9
  __int128 v46; // q1
  __int64 v47; // x26
  unsigned __int64 v48; // x27
  _BYTE v50[120]; // [xsp+8h] [xbp-238h] BYREF
  _BYTE v51[40]; // [xsp+80h] [xbp-1C0h] BYREF
  _BYTE v52[40]; // [xsp+A8h] [xbp-198h] BYREF
  _BYTE v53[40]; // [xsp+D0h] [xbp-170h] BYREF
  _BYTE v54[40]; // [xsp+F8h] [xbp-148h] BYREF
  _OWORD v55[2]; // [xsp+120h] [xbp-120h] BYREF
  __int64 v56; // [xsp+140h] [xbp-100h]
  _OWORD v57[2]; // [xsp+148h] [xbp-F8h] BYREF
  __int64 v58; // [xsp+168h] [xbp-D8h]
  _OWORD v59[2]; // [xsp+170h] [xbp-D0h] BYREF
  __int64 v60; // [xsp+190h] [xbp-B0h]
  _BYTE v61[63]; // [xsp+198h] [xbp-A8h] BYREF
  char v62; // [xsp+1D7h] [xbp-69h]

  v3 = 0;
  v4 = 0;
  v5 = *a2;
  v61[0] = v5 & 0xF;
  v61[1] = v5 >> 4;
  v6 = a2[1];
  v61[2] = v6 & 0xF;
  v61[3] = v6 >> 4;
  v7 = a2[2];
  v61[4] = v7 & 0xF;
  v61[5] = v7 >> 4;
  v8 = a2[3];
  v61[6] = v8 & 0xF;
  v61[7] = v8 >> 4;
  v9 = a2[4];
  v61[8] = v9 & 0xF;
  v61[9] = v9 >> 4;
  v10 = a2[5];
  v61[10] = v10 & 0xF;
  v61[11] = v10 >> 4;
  v11 = a2[6];
  v61[12] = v11 & 0xF;
  v61[13] = v11 >> 4;
  v12 = a2[7];
  v61[14] = v12 & 0xF;
  v61[15] = v12 >> 4;
  v13 = a2[8];
  v61[16] = v13 & 0xF;
  v61[17] = v13 >> 4;
  v14 = a2[9];
  v61[18] = v14 & 0xF;
  v61[19] = v14 >> 4;
  v15 = a2[10];
  v61[20] = v15 & 0xF;
  v61[21] = v15 >> 4;
  v16 = a2[11];
  v61[22] = v16 & 0xF;
  v61[23] = v16 >> 4;
  v17 = a2[12];
  v61[24] = v17 & 0xF;
  v61[25] = v17 >> 4;
  v18 = a2[13];
  v61[26] = v18 & 0xF;
  v61[27] = v18 >> 4;
  v19 = a2[14];
  v61[28] = v19 & 0xF;
  v61[29] = v19 >> 4;
  v20 = a2[15];
  v61[30] = v20 & 0xF;
  v61[31] = v20 >> 4;
  v21 = a2[16];
  v61[32] = v21 & 0xF;
  v61[33] = v21 >> 4;
  v22 = a2[17];
  v61[34] = v22 & 0xF;
  v61[35] = v22 >> 4;
  v23 = a2[18];
  v61[36] = v23 & 0xF;
  v61[37] = v23 >> 4;
  v24 = a2[19];
  v61[38] = v24 & 0xF;
  v61[39] = v24 >> 4;
  v25 = a2[20];
  v61[40] = v25 & 0xF;
  v61[41] = v25 >> 4;
  v26 = a2[21];
  v61[42] = v26 & 0xF;
  v61[43] = v26 >> 4;
  v27 = a2[22];
  v61[44] = v27 & 0xF;
  v61[45] = v27 >> 4;
  v28 = a2[23];
  v61[46] = v28 & 0xF;
  v61[47] = v28 >> 4;
  v29 = a2[24];
  v61[48] = v29 & 0xF;
  v61[49] = v29 >> 4;
  v30 = a2[25];
  v61[50] = v30 & 0xF;
  v61[51] = v30 >> 4;
  v31 = a2[26];
  v61[52] = v31 & 0xF;
  v61[53] = v31 >> 4;
  v32 = a2[27];
  v61[54] = v32 & 0xF;
  v61[55] = v32 >> 4;
  v33 = a2[28];
  v61[56] = v33 & 0xF;
  v61[57] = v33 >> 4;
  v34 = a2[29];
  v61[58] = v34 & 0xF;
  v61[59] = v34 >> 4;
  v35 = a2[30];
  v61[60] = v35 & 0xF;
  v61[61] = v35 >> 4;
  v36 = a2[31];
  v61[62] = v36 & 0xF;
  v62 = v36 >> 4;
  do
  {
    v37 = (unsigned __int8)v61[v3] + v4;
    v4 = (v37 + 8) << 24 >> 28;
    v61[v3++] = v37 - ((v37 + 8) & 0xF0);
  }
  while ( v3 != 63 );
  v38 = 0;
  v62 += v4;
  *(_OWORD *)a1 = 0u;
  *(_OWORD *)(a1 + 16) = 0u;
  *(_QWORD *)(a1 + 32) = 0;
  *(_OWORD *)(a1 + 40) = 0u;
  *(_OWORD *)(a1 + 56) = 0u;
  *(_QWORD *)(a1 + 72) = 0;
  v39 = 1;
  *(_DWORD *)(a1 + 40) = 1;
  *(_OWORD *)(a1 + 80) = 0u;
  v40 = (_OWORD *)(a1 + 80);
  *(_OWORD *)(a1 + 96) = 0u;
  *(_QWORD *)(a1 + 112) = 0;
  *(_DWORD *)(a1 + 80) = 1;
  *(_OWORD *)(a1 + 120) = 0u;
  *(_OWORD *)(a1 + 136) = 0u;
  *(_QWORD *)(a1 + 152) = 0;
  do
  {
    sub_1003D938C(v50, v38, (unsigned int)(char)v61[v39]);
    sub_1003D9818(v51, a1, v50);
    sub_1003D8E14(a1, v51, v54);
    sub_1003D8E14(a1 + 40, v52, v53);
    sub_1003D8E14(a1 + 80, v53, v54);
    sub_1003D8E14(a1 + 120, v51, v52);
    v38 = (unsigned int)(v38 + 1);
    v41 = v39 >= 0x3E;
    v39 += 2LL;
  }
  while ( !v41 );
  v42 = *(_OWORD *)(a1 + 16);
  v55[0] = *(_OWORD *)a1;
  v55[1] = v42;
  v43 = *(_QWORD *)(a1 + 32);
  v44 = *(_OWORD *)(a1 + 56);
  v57[0] = *(_OWORD *)(a1 + 40);
  v57[1] = v44;
  v45 = *(_QWORD *)(a1 + 72);
  v56 = v43;
  v58 = v45;
  v46 = *(_OWORD *)(a1 + 96);
  v59[0] = *v40;
  v59[1] = v46;
  v60 = *(_QWORD *)(a1 + 112);
  sub_1003D9B58(v51, v55);
  sub_1003D8E14(v55, v51, v54);
  sub_1003D8E14(v57, v52, v53);
  sub_1003D8E14(v59, v53, v54);
  sub_1003D9B58(v51, v55);
  sub_1003D8E14(v55, v51, v54);
  sub_1003D8E14(v57, v52, v53);
  sub_1003D8E14(v59, v53, v54);
  sub_1003D9B58(v51, v55);
  sub_1003D8E14(v55, v51, v54);
  sub_1003D8E14(v57, v52, v53);
  sub_1003D8E14(v59, v53, v54);
  sub_1003D9B58(v51, v55);
  sub_1003D8E14(a1, v51, v54);
  sub_1003D8E14(a1 + 40, v52, v53);
  sub_1003D8E14(a1 + 80, v53, v54);
  sub_1003D8E14(a1 + 120, v51, v52);
  v47 = 0;
  v48 = 0;
  do
  {
    sub_1003D938C(v50, v47, (unsigned int)(char)v61[v48]);
    sub_1003D9818(v51, a1, v50);
    sub_1003D8E14(a1, v51, v54);
    sub_1003D8E14(a1 + 40, v52, v53);
    sub_1003D8E14(a1 + 80, v53, v54);
    sub_1003D8E14(a1 + 120, v51, v52);
    v47 = (unsigned int)(v47 + 1);
    v41 = v48 >= 0x3E;
    v48 += 2LL;
  }
  while ( !v41 );
  return sub_100391B40(v61, 64);
}

/* ========================================================================
 * fallback -[SDL_uikitopenglview initWithFrame:scale:retainBacking:rBits:gBits:bBits:aBits:depthBits:stencilBits:sRGB:multisamples:context:]
 * EA: 0x100324580
 ======================================================================== */

// local variable allocation has failed, the output may be wrong!
SDL_uikitopenglview *__cdecl -[SDL_uikitopenglview initWithFrame:scale:retainBacking:rBits:gBits:bBits:aBits:depthBits:stencilBits:sRGB:multisamples:context:](
        SDL_uikitopenglview *self,
        SEL a2,
        CGRect a3,
        double a4,
        bool a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        bool a12,
        int a13,
        id a14)
{
  _BOOL8 v19; // x24
  double height; // d9
  double width; // d10
  double y; // d11
  double x; // d12
  id v26; // x19
  SDL_uikitopenglview *v27; // x0
  SDL_uikitopenglview *v28; // x20
  id v29; // x0
  id v30; // x0
  id WeakRetained; // x21
  unsigned __int8 v32; // w28
  GLint samples; // w8
  id *v34; // x9
  _BOOL4 v35; // w10
  bool v36; // zf
  NSString *const *v37; // x10
  int v38; // w11
  int v39; // w21
  id v40; // x22
  void *v41; // x23
  NSNumber *v42; // x21
  NSDictionary *v43; // x24
  GLuint *p_viewRenderbuffer; // x24
  id v45; // x21
  unsigned int v46; // w25
  GLsizei *p_backingWidth; // x25
  GLsizei *p_backingHeight; // x26
  int v49; // w8
  GLuint *p_depthRenderbuffer; // x27
  GLsizei v51; // w1
  const char *v52; // x0
  SDL_uikitopenglview *v53; // x21
  __int64 v55; // [xsp+0h] [xbp-D0h]
  GLint params; // [xsp+14h] [xbp-BCh] BYREF
  objc_super v57; // [xsp+18h] [xbp-B8h] BYREF
  _QWORD v58[2]; // [xsp+28h] [xbp-A8h] BYREF
  _QWORD v59[2]; // [xsp+38h] [xbp-98h] BYREF

  v19 = a5;
  height = a3.size.height;
  width = a3.size.width;
  y = a3.origin.y;
  x = a3.origin.x;
  v26 = objc_retain(a14);
  v57.receiver = self;
  v57.super_class = (Class)&OBJC_CLASS___SDL_uikitopenglview;
  v27 = -[SDL_uikitview initWithFrame:](&v57, "initWithFrame:", x, y, width, height);
  v28 = v27;
  if ( !v27 )
    goto LABEL_36;
  HIDWORD(v55) = a10;
  v29 = objc_storeWeak((id *)&v27->context, v26);
  v28->samples = a13;
  v28->retainedBacking = v19;
  v30 = objc_retain(v29);
  if ( !v26
    || (WeakRetained = objc_loadWeakRetained((id *)&v28->context),
        v32 = +[EAGLContext setCurrentContext:](&OBJC_CLASS___EAGLContext, "setCurrentContext:", WeakRetained),
        objc_release(WeakRetained),
        objc_release(v26),
        (v32 & 1) == 0) )
  {
    sub_10031F42C("Could not create OpenGL ES drawable (could not make context current)");
LABEL_39:
    v53 = nullptr;
    goto LABEL_40;
  }
  if ( v28->samples >= 1 )
  {
    params = 0;
    glGetIntegerv(0x8D57u, &params);
    samples = v28->samples;
    if ( samples >= params )
      samples = params;
    v28->samples = samples;
  }
  v34 = (id *)&kEAGLColorFormatSRGBA8;
  v35 = a6 > 7 || a7 > 7 || a8 > 7 || a9 > 0;
  v36 = !v35;
  if ( v35 )
    v37 = &kEAGLColorFormatRGBA8;
  else
    v37 = &kEAGLColorFormatRGB565;
  if ( v36 )
    v38 = 36194;
  else
    v38 = 32856;
  if ( a12 )
  {
    v39 = 35907;
  }
  else
  {
    v34 = (id *)v37;
    v39 = v38;
  }
  v40 = objc_retain(*v34);
  v28->colorBufferFormat = v39;
  v41 = objc_retainAutoreleasedReturnValue(-[SDL_uikitopenglview layer](v28, "layer"));
  objc_msgSend(v41, "setOpaque:", 1);
  v58[0] = kEAGLDrawablePropertyRetainedBacking;
  v42 = objc_retainAutoreleasedReturnValue(+[NSNumber numberWithBool:](&OBJC_CLASS___NSNumber, "numberWithBool:", v19));
  v58[1] = kEAGLDrawablePropertyColorFormat;
  v59[0] = v42;
  v59[1] = v40;
  v43 = objc_retainAutoreleasedReturnValue(
          +[NSDictionary dictionaryWithObjects:forKeys:count:](
            &OBJC_CLASS___NSDictionary,
            "dictionaryWithObjects:forKeys:count:",
            v59,
            v58,
            2));
  objc_msgSend(v41, "setDrawableProperties:", v43);
  objc_release(v43);
  objc_release(v42);
  -[SDL_uikitopenglview setContentScaleFactor:](v28, "setContentScaleFactor:", a4);
  p_viewRenderbuffer = &v28->viewRenderbuffer;
  glGenRenderbuffers(1, &v28->viewRenderbuffer);
  glBindRenderbuffer(0x8D41u, v28->viewRenderbuffer);
  v45 = objc_loadWeakRetained((id *)&v28->context);
  v46 = (unsigned int)objc_msgSend(v45, "renderbufferStorage:fromDrawable:", 36161, v41);
  objc_release(v45);
  if ( !v46 )
  {
    v52 = "Failed to create OpenGL ES drawable";
LABEL_38:
    sub_10031F42C(v52);
    objc_release(v41);
    objc_release(v40);
    goto LABEL_39;
  }
  glGenFramebuffers(1, &v28->viewFramebuffer);
  glBindFramebuffer(0x8D40u, v28->viewFramebuffer);
  glFramebufferRenderbuffer(0x8D40u, 0x8CE0u, 0x8D41u, *p_viewRenderbuffer);
  p_backingWidth = &v28->backingWidth;
  glGetRenderbufferParameteriv(0x8D41u, 0x8D42u, &v28->backingWidth);
  p_backingHeight = &v28->backingHeight;
  glGetRenderbufferParameteriv(0x8D41u, 0x8D43u, &v28->backingHeight);
  if ( glCheckFramebufferStatus(0x8D40u) != 36053 )
    goto LABEL_37;
  LODWORD(v55) = a11;
  if ( v28->samples >= 1 )
  {
    glGenFramebuffers(1, &v28->msaaFramebuffer);
    glBindFramebuffer(0x8D40u, v28->msaaFramebuffer);
    glGenRenderbuffers(1, &v28->msaaRenderbuffer);
    glBindRenderbuffer(0x8D41u, v28->msaaRenderbuffer);
    glRenderbufferStorageMultisample(0x8D41u, v28->samples, v28->colorBufferFormat, *p_backingWidth, *p_backingHeight);
    glFramebufferRenderbuffer(0x8D40u, 0x8CE0u, 0x8D41u, v28->msaaRenderbuffer);
  }
  if ( v55 )
  {
    if ( a11 )
      v49 = 35056;
    else
      v49 = 33190;
    v28->depthBufferFormat = v49;
    p_depthRenderbuffer = &v28->depthRenderbuffer;
    glGenRenderbuffers(1, &v28->depthRenderbuffer);
    glBindRenderbuffer(0x8D41u, v28->depthRenderbuffer);
    v51 = v28->samples;
    if ( v51 < 1 )
      glRenderbufferStorage(0x8D41u, v28->depthBufferFormat, *p_backingWidth, *p_backingHeight);
    else
      glRenderbufferStorageMultisample(0x8D41u, v51, v28->depthBufferFormat, *p_backingWidth, *p_backingHeight);
    if ( HIDWORD(v55) )
      glFramebufferRenderbuffer(0x8D40u, 0x8D00u, 0x8D41u, *p_depthRenderbuffer);
    if ( a11 )
      glFramebufferRenderbuffer(0x8D40u, 0x8D20u, 0x8D41u, *p_depthRenderbuffer);
  }
  if ( glCheckFramebufferStatus(0x8D40u) != 36053 )
  {
LABEL_37:
    v52 = "Failed creating OpenGL ES framebuffer";
    goto LABEL_38;
  }
  glBindRenderbuffer(0x8D41u, *p_viewRenderbuffer);
  -[SDL_uikitopenglview setDebugLabels](v28, "setDebugLabels");
  objc_release(v41);
  objc_release(v40);
LABEL_36:
  v53 = objc_retain(v28);
LABEL_40:
  objc_release(v26);
  objc_release(v28);
  return v53;
}

/* ========================================================================
 * fallback -[NYT360PlayerScene initWithAVPlayer:boundToView:is360TB:is180SBS:isLeft:]
 * EA: 0x10002f614
 ======================================================================== */

NYT360PlayerScene *__cdecl -[NYT360PlayerScene initWithAVPlayer:boundToView:is360TB:is180SBS:isLeft:](
        NYT360PlayerScene *self,
        SEL a2,
        id a3,
        id a4,
        bool a5,
        bool a6,
        bool a7)
{
  int v7; // w19
  int v8; // w23
  _BOOL4 v9; // w25
  id v13; // x24
  id v14; // x20
  NYT360PlayerScene *v15; // x0
  NYT360PlayerScene *v16; // x21
  int v17; // w28
  SCNCamera *v18; // x0
  SCNCamera *camera; // x8
  SCNNode *v20; // x22
  int v21; // w27
  double v22; // d0
  double v23; // d8
  double v24; // d1
  double v25; // d2
  double v26; // d9
  SCNNode *cameraNode; // x8
  void *v28; // x22
  void *v29; // x22
  void *v30; // x26
  NYTSKVideoNode *v31; // x26
  double v32; // d0
  double v33; // d8
  double v34; // d1
  NYTSKVideoNode *videoNode; // x8
  double v36; // d8
  double v37; // d0
  double v38; // d2
  double v39; // d1
  double v40; // d3
  void *v41; // x26
  void *v42; // x27
  SKCropNode *v43; // x27
  double x; // d0
  double y; // d1
  double v46; // d2
  SCNNode *v47; // x23
  double v48; // d0
  double v49; // d1
  double v50; // d2
  void *v51; // x24
  void *v52; // x25
  void *v53; // x26
  void *v54; // x27
  void *v55; // x25
  void *v56; // x26
  void *v57; // x27
  void *v58; // x25
  void *v59; // x26
  void *v60; // x27
  void *v61; // x25
  void *v62; // x26
  void *v63; // x24
  void *v64; // x24
  void *v65; // x24
  void *v66; // x25
  id v68; // [xsp+8h] [xbp-88h]
  objc_super v69; // [xsp+10h] [xbp-80h] BYREF

  v7 = a7;
  v8 = a6;
  v9 = a5;
  v13 = objc_retain(a3);
  v14 = objc_retain(a4);
  v69.receiver = self;
  v69.super_class = (Class)&OBJC_CLASS___NYT360PlayerScene;
  v15 = -[NYT360PlayerScene init](&v69, "init");
  v16 = v15;
  if ( v15 )
  {
    v17 = v8 ^ v7;
    v15->_videoPlaybackIsPaused = 1;
    objc_storeStrong((id *)&v15->_player, a3);
    v18 = objc_opt_new(&OBJC_CLASS___SCNCamera);
    camera = v16->_camera;
    v16->_camera = v18;
    objc_release(camera);
    v20 = objc_opt_new(&OBJC_CLASS___SCNNode);
    -[SCNNode setCamera:](v20, "setCamera:", v16->_camera);
    v21 = v9 | v8;
    LODWORD(v22) = 1036831949;
    if ( v8 != v7 )
      *(float *)&v22 = -0.1;
    if ( v21 )
    {
      v23 = 1440.0;
    }
    else
    {
      *(float *)&v22 = 0.0;
      v23 = 4320.0;
    }
    HIDWORD(v24) = 1086193664;
    HIDWORD(v25) = 1084489728;
    if ( v21 )
      v26 = 2560.0;
    else
      v26 = 7680.0;
    LODWORD(v24) = 0;
    LODWORD(v25) = 0;
    -[SCNNode setPosition:](v20, "setPosition:", v22, v24, v25);
    cameraNode = v16->_cameraNode;
    v16->_cameraNode = v20;
    objc_release(cameraNode);
    v28 = (void *)objc_claimAutoreleasedReturnValue(-[NYT360PlayerScene rootNode](v16, "rootNode"));
    objc_msgSend(v28, "addChildNode:", v16->_cameraNode);
    objc_release(v28);
    v29 = objc_msgSend(objc_alloc((Class)&OBJC_CLASS___SKScene), "initWithSize:", v26, v23);
    objc_msgSend(v29, "setShouldRasterize:", 1);
    objc_msgSend(v29, "setScaleMode:", 3);
    v30 = (void *)objc_claimAutoreleasedReturnValue(+[UIColor blackColor](&OBJC_CLASS___UIColor, "blackColor"));
    objc_msgSend(v29, "setBackgroundColor:", v30);
    objc_release(v30);
    v68 = v13;
    v31 = -[NYTSKVideoNode initWithAVPlayer:](objc_alloc(&OBJC_CLASS___NYTSKVideoNode), "initWithAVPlayer:", v13);
    objc_msgSend(v29, "size");
    v33 = v32 * 0.5;
    objc_msgSend(v29, "size");
    -[NYTSKVideoNode setPosition:](v31, "setPosition:", v33, v34 * 0.5);
    objc_msgSend(v29, "size");
    -[NYTSKVideoNode setSize:](v31, "setSize:");
    -[NYTSKVideoNode setYScale:](v31, "setYScale:", -1.0);
    -[NYTSKVideoNode setXScale:](v31, "setXScale:", -1.0);
    -[NYTSKVideoNode setNyt_delegate:](v31, "setNyt_delegate:", v16);
    videoNode = v16->_videoNode;
    v16->_videoNode = v31;
    objc_release(videoNode);
    if ( (v21 & 1) != 0 )
    {
      if ( v9 )
        v36 = 2.0;
      else
        v36 = 1.0;
      v37 = 1280.0;
      if ( v9 )
        v38 = 2560.0;
      else
        v38 = 1280.0;
      v39 = 720.0;
      if ( v9 )
        v40 = 720.0;
      else
        v40 = 1440.0;
      if ( !v9 )
        v39 = 0.0;
      if ( v17 )
        v39 = 0.0;
      if ( v17 | v9 )
        v37 = 0.0;
      v41 = (void *)objc_claimAutoreleasedReturnValue(
                      +[SKShapeNode shapeNodeWithRect:](
                        &OBJC_CLASS___SKShapeNode,
                        "shapeNodeWithRect:",
                        v37,
                        v39,
                        v38,
                        v40));
      v42 = (void *)objc_claimAutoreleasedReturnValue(+[UIColor blackColor](&OBJC_CLASS___UIColor, "blackColor"));
      objc_msgSend(v41, "setFillColor:", v42);
      objc_release(v42);
      v43 = objc_opt_new(&OBJC_CLASS___SKCropNode);
      -[SKCropNode setMaskNode:](v43, "setMaskNode:", v41);
      -[SKCropNode setYScale:](v43, "setYScale:", v36);
      -[SKCropNode setXScale:](v43, "setXScale:", 1.0);
      -[SKCropNode addChild:](v43, "addChild:", v16->_videoNode);
      x = CGPointZero.x;
      y = CGPointZero.y;
      if ( v9 )
      {
        if ( !v17 )
        {
          x = 0.0;
          y = -1440.0;
        }
      }
      else
      {
        v46 = 640.0;
        if ( v7 )
          v46 = -640.0;
        if ( v8 )
        {
          x = v46;
          y = 0.0;
        }
      }
      -[SKCropNode setPosition:](v43, "setPosition:", x, y);
      objc_msgSend(v29, "addChild:", v43);
      objc_release(v43);
      objc_release(v41);
    }
    else
    {
      objc_msgSend(v29, "addChild:", v16->_videoNode);
    }
    v47 = objc_opt_new(&OBJC_CLASS___SCNNode);
    LODWORD(v48) = 0;
    LODWORD(v49) = 0;
    LODWORD(v50) = 0;
    -[SCNNode setPosition:](v47, "setPosition:", v48, v49, v50);
    v51 = (void *)objc_claimAutoreleasedReturnValue(+[SCNSphere sphereWithRadius:](&OBJC_CLASS___SCNSphere, "sphereWithRadius:", 100.0));
    objc_msgSend(v51, "setSegmentCount:", 96);
    -[SCNNode setGeometry:](v47, "setGeometry:", v51);
    v52 = (void *)objc_claimAutoreleasedReturnValue(-[SCNNode geometry](v47, "geometry"));
    v53 = (void *)objc_claimAutoreleasedReturnValue(objc_msgSend(v52, "firstMaterial"));
    v54 = (void *)objc_claimAutoreleasedReturnValue(objc_msgSend(v53, "diffuse"));
    objc_msgSend(v54, "setContents:", v29);
    objc_release(v54);
    objc_release(v53);
    objc_release(v52);
    v55 = (void *)objc_claimAutoreleasedReturnValue(-[SCNNode geometry](v47, "geometry"));
    v56 = (void *)objc_claimAutoreleasedReturnValue(objc_msgSend(v55, "firstMaterial"));
    v57 = (void *)objc_claimAutoreleasedReturnValue(objc_msgSend(v56, "diffuse"));
    objc_msgSend(v57, "setMinificationFilter:", 2);
    objc_release(v57);
    objc_release(v56);
    objc_release(v55);
    v58 = (void *)objc_claimAutoreleasedReturnValue(-[SCNNode geometry](v47, "geometry"));
    v59 = (void *)objc_claimAutoreleasedReturnValue(objc_msgSend(v58, "firstMaterial"));
    v60 = (void *)objc_claimAutoreleasedReturnValue(objc_msgSend(v59, "diffuse"));
    objc_msgSend(v60, "setMagnificationFilter:", 2);
    objc_release(v60);
    objc_release(v59);
    objc_release(v58);
    v61 = (void *)objc_claimAutoreleasedReturnValue(-[SCNNode geometry](v47, "geometry"));
    v62 = (void *)objc_claimAutoreleasedReturnValue(objc_msgSend(v61, "firstMaterial"));
    objc_msgSend(v62, "setDoubleSided:", 1);
    objc_release(v62);
    objc_release(v61);
    objc_release(v51);
    v63 = (void *)objc_claimAutoreleasedReturnValue(-[NYT360PlayerScene rootNode](v16, "rootNode"));
    objc_msgSend(v63, "addChildNode:", v47);
    objc_release(v63);
    objc_msgSend(v14, "setScene:", v16);
    v64 = (void *)objc_claimAutoreleasedReturnValue(-[NYT360PlayerScene cameraNode](v16, "cameraNode"));
    objc_msgSend(v14, "setPointOfView:", v64);
    objc_release(v64);
    v65 = (void *)objc_claimAutoreleasedReturnValue(objc_msgSend(v14, "pointOfView"));
    v66 = (void *)objc_claimAutoreleasedReturnValue(objc_msgSend(v65, "camera"));
    objc_msgSend(v66, "setZFar:", 200.0);
    objc_release(v66);
    objc_release(v65);
    objc_release(v47);
    objc_release(v29);
    v13 = v68;
  }
  objc_release(v14);
  objc_release(v13);
  return v16;
}

/* ========================================================================
 * fallback sub_10043d8a0
 * EA: 0x10043d8a0
 ======================================================================== */

unsigned __int64 __fastcall sub_10043D8A0(unsigned __int64 result)
{
  _QWORD *v1; // x19
  unsigned __int64 v3; // x12
  __int64 v4; // x20
  unsigned __int64 v5; // x12
  unsigned __int64 v6; // x9
  bool v7; // cf
  __int64 v8; // x27
  __int64 v9; // x21
  __int64 v10; // x24
  __int64 v11; // x22
  unsigned __int64 v12; // x10
  unsigned __int64 v13; // x8
  __int64 v14; // x14
  unsigned __int64 v15; // x11
  unsigned __int64 v16; // x25
  unsigned __int64 v17; // x15
  __int64 v18; // x13
  unsigned __int64 v19; // x15
  __int64 v20; // x16
  _QWORD *v21; // x12
  __int64 v22; // x8
  unsigned __int64 v23; // x9
  __int64 v24; // x8
  unsigned __int64 v25; // x8
  _QWORD *v26; // x22
  __int64 v27; // x8
  unsigned __int64 v28; // x8
  _QWORD *v29; // x8
  unsigned __int64 v30; // x8
  _QWORD *v31; // x8

  if ( result )
  {
    v1 = (_QWORD *)result;
    if ( (unsigned __int64)xmmword_1006963E0 > result
      || (_QWORD)xmmword_1006963E0 + *((_QWORD *)&xmmword_1006963E0 + 1) <= result )
    {
      sub_1003C32A4((int)"assertion failed: WITHIN_ARENA(ptr)", (char)"crypto/mem_sec.c");
    }
    v3 = result + *((_QWORD *)&xmmword_1006963E0 + 1) - xmmword_1006963E0;
    v4 = *((_QWORD *)&xmmword_1006963F0 + 1) - 1LL;
    if ( qword_100696400 <= v3 )
    {
      v5 = v3 / qword_100696400;
      do
      {
        if ( ((*(unsigned __int8 *)(qword_100696408 + (v5 >> 3)) >> (v5 & 7)) & 1) != 0 )
          break;
        if ( (v5 & 1) != 0 )
          sub_1003C32A4((int)"assertion failed: (bit & 1) == 0", (char)"crypto/mem_sec.c");
        --v4;
        v7 = v5 >= 2;
        v5 >>= 1;
      }
      while ( v7 );
    }
    if ( (v4 & 0x80000000) != 0 || *((__int64 *)&xmmword_1006963F0 + 1) <= (unsigned int)v4 )
LABEL_71:
      sub_1003C32A4((int)"assertion failed: list >= 0 && list < sh.freelist_size", (char)"crypto/mem_sec.c");
    if ( (((*((_QWORD *)&xmmword_1006963E0 + 1) >> v4) - 1LL) & (result - (_QWORD)xmmword_1006963E0)) != 0 )
LABEL_72:
      sub_1003C32A4(
        (int)"assertion failed: ((ptr - sh.arena) & ((sh.arena_size >> list) - 1)) == 0",
        (char)"crypto/mem_sec.c");
    v6 = (result - (unsigned __int64)xmmword_1006963E0) / (*((_QWORD *)&xmmword_1006963E0 + 1) >> v4) + (1LL << v4);
    if ( v6 )
      v7 = v6 >= *((_QWORD *)&xmmword_100696410 + 1);
    else
      v7 = 1;
    if ( v7 )
LABEL_73:
      sub_1003C32A4((int)"assertion failed: bit > 0 && bit < sh.bittable_size", (char)"crypto/mem_sec.c");
    if ( ((*(unsigned __int8 *)(qword_100696408 + (v6 >> 3)) >> (v6 & 7)) & 1) == 0 )
      sub_1003C32A4((int)"assertion failed: sh_testbit(ptr, list, sh.bittable)", (char)"crypto/mem_sec.c");
    sub_10043E238(result, v4, xmmword_100696410);
    result = sub_10043E144(xmmword_1006963F0 + 8 * v4, v1);
    v8 = 8 * v4 - 8;
    v9 = (unsigned int)(v4 - 1);
    while ( 1 )
    {
      v10 = 1LL << v4;
      v11 = xmmword_1006963E0;
      v12 = *((_QWORD *)&xmmword_1006963E0 + 1) >> v4;
      v13 = (unsigned __int64)((unsigned __int64)v1 - xmmword_1006963E0) / (*((_QWORD *)&xmmword_1006963E0 + 1) >> v4)
          + (1LL << v4);
      v14 = 1LL << ((v13 ^ 1) & 7);
      if ( ((unsigned __int8)v14 & *(_BYTE *)(qword_100696408 + (v13 >> 3))) == 0 )
        break;
      v15 = *(unsigned __int8 *)(xmmword_100696410 + (v13 >> 3));
      if ( (v14 & v15) != 0 || !(_QWORD)xmmword_1006963E0 )
        break;
      v16 = ((v13 ^ 1) & (v10 - 1)) * v12;
      v17 = v16 / v12 + v10;
      v18 = v17 ^ 1;
      v19 = v17 >> 3;
      v20 = 1LL << (v18 & 7);
      if ( ((unsigned __int8)v20 & *(_BYTE *)(qword_100696408 + v19)) != 0 )
      {
        v21 = (_QWORD *)(xmmword_1006963E0 + (v18 & (v10 - 1)) * v12);
        if ( ((unsigned __int8)v20 & *(_BYTE *)(xmmword_100696410 + v19)) != 0 )
          v21 = nullptr;
        if ( v1 != v21 )
LABEL_74:
          sub_1003C32A4((int)"assertion failed: ptr == sh_find_my_buddy(buddy, list)", (char)"crypto/mem_sec.c");
      }
      else if ( v1 )
      {
        goto LABEL_74;
      }
      if ( (v4 & 0x80000000) != 0 || *((__int64 *)&xmmword_1006963F0 + 1) <= (unsigned int)v4 )
        goto LABEL_71;
      if ( ((v12 - 1) & (unsigned __int64)((unsigned __int64)v1 - xmmword_1006963E0)) != 0 )
        goto LABEL_72;
      if ( !v13 || v13 >= *((_QWORD *)&xmmword_100696410 + 1) )
        goto LABEL_73;
      if ( ((v15 >> (v13 & 7)) & 1) != 0 )
        sub_1003C32A4((int)"assertion failed: !sh_testbit(ptr, list, sh.bitmalloc)", (char)"crypto/mem_sec.c");
      sub_10043E238(v1, v4, qword_100696408);
      v22 = *v1;
      if ( *v1 )
        *(_QWORD *)(v22 + 8) = v1[1];
      *(_QWORD *)v1[1] = v22;
      if ( *v1 )
      {
        v23 = *(_QWORD *)(*v1 + 8LL);
        v24 = *((_QWORD *)&xmmword_1006963F0 + 1);
        if ( (v23 < (unsigned __int64)xmmword_1006963F0
           || v23 >= (__int64)xmmword_1006963F0 + 8LL * *((_QWORD *)&xmmword_1006963F0 + 1))
          && (v23 < (unsigned __int64)xmmword_1006963E0
           || v23 >= (_QWORD)xmmword_1006963E0 + *((_QWORD *)&xmmword_1006963E0 + 1)) )
        {
          goto LABEL_79;
        }
      }
      else
      {
        v24 = *((_QWORD *)&xmmword_1006963F0 + 1);
      }
      if ( v24 <= (unsigned int)v4 )
        goto LABEL_71;
      if ( (((*((_QWORD *)&xmmword_1006963E0 + 1) >> v4) - 1LL)
          & (unsigned __int64)((unsigned __int64)v1 - xmmword_1006963E0)) != 0 )
        goto LABEL_72;
      v25 = (unsigned __int64)((unsigned __int64)v1 - xmmword_1006963E0) / (*((_QWORD *)&xmmword_1006963E0 + 1) >> v4)
          + v10;
      if ( !v25 || v25 >= *((_QWORD *)&xmmword_100696410 + 1) )
        goto LABEL_73;
      if ( ((*(unsigned __int8 *)(xmmword_100696410 + (v25 >> 3)) >> (v25 & 7)) & 1) != 0 )
        sub_1003C32A4((int)"assertion failed: !sh_testbit(ptr, list, sh.bitmalloc)", (char)"crypto/mem_sec.c");
      v26 = (_QWORD *)(v11 + v16);
      sub_10043E238(v26, v4, qword_100696408);
      v27 = *v26;
      if ( *v26 )
        *(_QWORD *)(v27 + 8) = v26[1];
      *(_QWORD *)v26[1] = v27;
      if ( *v26 )
      {
        v28 = *(_QWORD *)(*v26 + 8LL);
        if ( (v28 < (unsigned __int64)xmmword_1006963F0
           || v28 >= (__int64)xmmword_1006963F0 + 8LL * *((_QWORD *)&xmmword_1006963F0 + 1))
          && (v28 < (unsigned __int64)xmmword_1006963E0
           || v28 >= (_QWORD)xmmword_1006963E0 + *((_QWORD *)&xmmword_1006963E0 + 1)) )
        {
LABEL_79:
          sub_1003C32A4(
            (int)"assertion failed: WITHIN_FREELIST(temp2->p_next) || WITHIN_ARENA(temp2->p_next)",
            (char)"crypto/mem_sec.c");
        }
      }
      if ( v1 <= v26 )
        v29 = v26;
      else
        v29 = v1;
      *v29 = 0;
      v29[1] = 0;
      if ( v1 > v26 )
        v1 = v26;
      if ( (v9 & 0x80000000) != 0 || *((__int64 *)&xmmword_1006963F0 + 1) <= (unsigned int)(v4 - 1) )
        goto LABEL_71;
      if ( (((*((_QWORD *)&xmmword_1006963E0 + 1) >> ((unsigned __int8)v4 - 1)) - 1LL)
          & (unsigned __int64)((unsigned __int64)v1 - xmmword_1006963E0)) != 0 )
        goto LABEL_72;
      v30 = (unsigned __int64)((unsigned __int64)v1 - xmmword_1006963E0)
          / (*((_QWORD *)&xmmword_1006963E0 + 1) >> ((unsigned __int8)v4 - 1))
          + (1LL << ((unsigned __int8)v4 - 1));
      if ( !v30 || v30 >= *((_QWORD *)&xmmword_100696410 + 1) )
        goto LABEL_73;
      if ( ((*(unsigned __int8 *)(xmmword_100696410 + (v30 >> 3)) >> (v30 & 7)) & 1) != 0 )
        sub_1003C32A4((int)"assertion failed: !sh_testbit(ptr, list, sh.bitmalloc)", (char)"crypto/mem_sec.c");
      sub_10043E054(v1, v9, qword_100696408);
      result = sub_10043E144(xmmword_1006963F0 + v8, v1);
      v31 = *(_QWORD **)(xmmword_1006963F0 + 8 * v4 - 8);
      v8 -= 8;
      v9 = (unsigned int)(v9 - 1);
      --v4;
      if ( v31 != v1 )
        sub_1003C32A4((int)"assertion failed: sh.freelist[list] == ptr", (char)"crypto/mem_sec.c");
    }
  }
  return result;
}

/* ========================================================================
 * fallback sub_100073d68
 * EA: 0x100073d68
 ======================================================================== */

__int64 sub_100073D68()
{
  void *v0; // x0
  id v1; // x0
  __int64 v2; // x1
  void *v3; // x22
  __int64 v4; // x1
  __int64 v5; // x19
  Swift::String v6; // x0
  id v7; // x20
  double v8; // d0
  CGFloat v9; // d8
  double v10; // d1
  CGFloat v11; // d9
  double v12; // d2
  CGFloat v13; // d10
  double v14; // d3
  CGFloat v15; // d11
  __int64 v16; // x21
  __int64 v17; // x26
  __int64 v18; // x20
  __int128 v19; // q1
  __int64 v20; // x22
  void *v21; // x24
  id v22; // x23
  id v23; // x24
  __int64 v24; // x0
  __int64 v25; // x20
  __int64 v26; // x21
  void *v27; // x23
  id v28; // x22
  id v29; // x23
  __int64 v30; // x0
  __int64 result; // x0
  unsigned int v32; // w0
  Swift::String v33; // x0
  id v34; // x19
  double v35; // d0
  CGFloat v36; // d8
  double v37; // d1
  CGFloat v38; // d9
  double v39; // d2
  CGFloat v40; // d10
  double v41; // d3
  CGFloat v42; // d11
  double Height; // d8
  __int64 v44; // x0
  __int64 inited; // x0
  char v46; // w8
  __int64 v47; // x0
  __int64 v48; // x3
  __int64 v49; // x19
  __int64 v50; // x21
  __int64 v51; // x1
  __int64 v52; // x22
  __int64 v53; // x0
  _QWORD v54[2]; // [xsp+8h] [xbp-158h] BYREF
  _BYTE v55[24]; // [xsp+18h] [xbp-148h] BYREF
  _BYTE v56[24]; // [xsp+30h] [xbp-130h] BYREF
  _BYTE v57[40]; // [xsp+48h] [xbp-118h] BYREF
  __int128 v58; // [xsp+70h] [xbp-F0h] BYREF
  __int128 v59; // [xsp+80h] [xbp-E0h]
  __int64 v60; // [xsp+90h] [xbp-D0h]
  __int128 v61; // [xsp+A0h] [xbp-C0h]
  __int128 v62; // [xsp+B0h] [xbp-B0h]
  __int64 v63; // [xsp+C0h] [xbp-A0h]
  __int128 v64; // [xsp+D0h] [xbp-90h] BYREF
  __int128 v65; // [xsp+E0h] [xbp-80h] BYREF
  CGRect v66; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8
  CGRect v67; // 0:d0.8,8:d1.8,16:d2.8,24:d3.8

  if ( qword_100662550 != -1 )
    swift_once(&qword_100662550, sub_100205F80);
  if ( qword_1006625A0 != -1 )
    swift_once(&qword_1006625A0, sub_100240480);
  v0 = *(void **)(qword_100697238 + OBJC_IVAR____TtC15ExternalMonitor12VTBLEManager_connectedPeripheral);
  if ( !v0 )
    goto LABEL_20;
  v1 = objc_retainAutoreleasedReturnValue(objc_msgSend(v0, "name"));
  if ( !v1 )
    goto LABEL_20;
  v3 = v1;
  static String._unconditionallyBridgeFromObjectiveC(_:)(v1, v2);
  v5 = v4;
  objc_release(v3);
  v6._countAndFlagsBits = 3486032;
  v6._object = (void *)0xE300000000000000LL;
  if ( !String.hasPrefix(_:)(v6) )
  {
    v33._countAndFlagsBits = 3158352;
    v33._object = (void *)0xE300000000000000LL;
    String.hasPrefix(_:)(v33);
    swift_bridgeObjectRelease(v5);
LABEL_20:
    if ( qword_100662540 != -1 )
      swift_once(&qword_100662540, sub_1001F0F34);
    v34 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(qword_100697100
                                                                + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_window), "screen"));
    objc_msgSend(v34, "bounds");
    v36 = v35;
    v38 = v37;
    v40 = v39;
    v42 = v41;
    objc_release(v34);
    v67.origin.x = v36;
    v67.origin.y = v38;
    v67.size.width = v40;
    v67.size.height = v42;
    Height = CGRectGetHeight(v67);
    v44 = sub_10003E4E0(&unk_100672970, &unk_100542070);
    inited = swift_initStackObject(v44, v57);
    *(_OWORD *)(inited + 16) = xmmword_10053B940;
    if ( Height < 1200.0 )
      v46 = 49;
    else
      v46 = 65;
    *(_BYTE *)(inited + 32) = v46;
    v47 = sub_1001D78DC(8, inited);
    v49 = v48;
    v50 = sub_1001D4D98(v47 & 0xFFFFFFFF00FFFF01LL);
    v52 = v51;
    swift_bridgeObjectRelease(v49);
    sub_1002400FC(v50, v52);
    type metadata accessor for VTLogger(0);
    v53 = static os_log_type_t.info.getter();
    sub_1001D8B44(v53, 0xD000000000000029LL, 0x80000001004DFCE0LL);
    return sub_100040E14(v50, v52);
  }
  swift_bridgeObjectRelease(v5);
  if ( qword_100662540 != -1 )
    swift_once(&qword_100662540, sub_1001F0F34);
  v7 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(qword_100697100
                                                             + OBJC_IVAR____TtC15ExternalMonitor15VTWindowManager_window), "screen"));
  objc_msgSend(v7, "bounds");
  v9 = v8;
  v11 = v10;
  v13 = v12;
  v15 = v14;
  objc_release(v7);
  v66.origin.x = v9;
  v66.origin.y = v11;
  v66.size.width = v13;
  v66.size.height = v15;
  v16 = 4 * (unsigned int)(CGRectGetHeight(v66) >= 1200.0);
  if ( qword_100662390 != -1 )
    swift_once(&qword_100662390, sub_1000D2180);
  v17 = qword_100696D00;
  v18 = qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__is120Hz;
  swift_beginAccess(qword_100696D00 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__is120Hz, v56, 0, 0);
  v19 = *(_OWORD *)(v18 + 16);
  v61 = *(_OWORD *)v18;
  v62 = v19;
  v63 = *(_QWORD *)(v18 + 32);
  v20 = *((_QWORD *)&v61 + 1);
  v21 = (void *)v19;
  v64 = *(_OWORD *)(v18 + 24);
  v22 = objc_retain((id)v61);
  swift_retain(v20);
  v23 = objc_retain(v21);
  sub_10004542C(&v64, &v58);
  v24 = sub_10003E4E0(&unk_10066A970, &unk_10053BAD0);
  WrappedDefault.wrappedValue.getter(&v58, v24);
  objc_release(v23);
  swift_release(v20);
  objc_release(v22);
  sub_10003F604(&v64);
  if ( (unsigned __int8)v58 != 1 )
    return sub_1000751CC(v16);
  v25 = v17 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__N6PDisplayModeRaw;
  swift_beginAccess(v17 + OBJC_IVAR____TtC15ExternalMonitor13VTPreferences__N6PDisplayModeRaw, v55, 0, 0);
  v58 = *(_OWORD *)v25;
  v59 = *(_OWORD *)(v25 + 16);
  v60 = *(_QWORD *)(v25 + 32);
  v26 = *((_QWORD *)&v58 + 1);
  v27 = (void *)v59;
  v65 = *(_OWORD *)(v25 + 24);
  v28 = objc_retain((id)v58);
  swift_retain(v26);
  v29 = objc_retain(v27);
  sub_10004542C(&v65, v54);
  v30 = sub_10003E4E0(&unk_10066AFA0, &unk_10053BAB0);
  WrappedDefault.wrappedValue.getter(v54, v30);
  objc_release(v29);
  swift_release(v26);
  objc_release(v28);
  sub_10003F604(&v65);
  result = v54[0];
  if ( (v54[0] & 0x8000000000000000LL) != 0 )
  {
    __break(1u);
  }
  else if ( v54[0] <= 0xFFu )
  {
    v32 = sub_1001C7210();
    if ( (unsigned __int8)v32 == 8 )
      v16 = 3;
    else
      v16 = v32;
    return sub_1000751CC(v16);
  }
  __break(1u);
  return result;
}

/* ========================================================================
 * fallback sub_1003db290
 * EA: 0x1003db290
 ======================================================================== */

__int64 __fastcall sub_1003DB290(_OWORD *a1, __int64 a2, __int64 a3)
{
  __int64 v4; // x27
  __int64 v5; // x21
  __int64 v6; // x24
  __int64 v7; // x22
  __int64 v8; // x8
  unsigned int v9; // w9
  unsigned __int64 v10; // x8
  __int64 v11; // x8
  int8x16_t v12; // q0
  unsigned int v13; // w1
  __int64 v14; // x10
  int8x16_t v15; // q1
  int8x16_t v16; // q2
  int8x16_t v17; // q3
  int8x16_t v18; // q4
  int8x16_t v19; // q5
  __int64 v20; // x9
  __int64 v21; // x10
  int8x16_t v22; // q6
  int8x16_t v23; // q7
  int8x16_t v24; // q16
  int8x16_t v25; // q17
  int8x16_t v26; // q18
  int8x16_t v27; // q19
  int8x16_t v28; // q20
  int8x16_t v29; // q6
  int8x16_t v30; // q6
  int v31; // w13
  int v32; // w9
  int v33; // w13
  int v34; // w10
  int v35; // w13
  int v36; // w9
  unsigned __int32 v37; // w13
  __int32 v38; // w10
  int v39; // w13
  int v40; // w9
  int v41; // w13
  int v42; // w10
  int v43; // w13
  int v44; // w9
  unsigned __int32 v45; // w13
  __int32 v46; // w10
  int v47; // w13
  int v48; // w9
  int v49; // w13
  int v50; // w10
  int v51; // w13
  unsigned __int32 v52; // w11
  _OWORD *v55; // [xsp+20h] [xbp-180h]
  _QWORD v56[7]; // [xsp+38h] [xbp-168h] BYREF
  __int128 v57; // [xsp+70h] [xbp-130h] BYREF
  __int128 v58; // [xsp+80h] [xbp-120h]
  __int128 v59; // [xsp+90h] [xbp-110h]
  _OWORD v60[2]; // [xsp+A0h] [xbp-100h] BYREF
  int8x16_t v61; // [xsp+C0h] [xbp-E0h]
  __int128 v62; // [xsp+D0h] [xbp-D0h]
  __int128 v63; // [xsp+E0h] [xbp-C0h]
  int8x16_t v64; // [xsp+F0h] [xbp-B0h] BYREF
  int8x16_t v65; // [xsp+100h] [xbp-A0h]
  int8x16_t v66; // [xsp+110h] [xbp-90h]
  int8x16_t v67; // [xsp+120h] [xbp-80h]

  sub_1003E0318(v56, a3, &unk_100562030);
  sub_1003E0F00(v56, v56);
  v55 = a1 + 4;
  v4 = 18;
  do
  {
    if ( v4 != 18 )
      sub_1003DAD64(a1, a1, 0);
    v5 = 0;
    v6 = v4 - 1;
    v7 = a2;
    do
    {
      while ( 1 )
      {
        v8 = v6 + 90 * v5;
        v9 = (v56[(unsigned int)v8 >> 6] >> v8) & 1
           | (2 * ((v56[(unsigned int)(v8 + 18) >> 6] >> ((unsigned __int8)v8 + 18)) & 1)) & 0xF3
           | (4 * ((v56[(unsigned int)(v8 + 36) >> 6] >> ((unsigned __int8)v8 + 36)) & 1)) & 0xF7
           | (8 * ((v56[(unsigned int)(v8 + 54) >> 6] >> ((unsigned __int8)v8 + 54)) & 1));
        v10 = v8 + 72;
        if ( v10 <= 0x1BD )
          v9 = v9 & 0xFFFFFFEF | (16 * ((*(_QWORD *)((char *)v56 + ((v10 >> 3) & 0x1FFFFFFFFFFFFFF8LL)) >> v10) & 1));
        v11 = 0;
        v12 = 0u;
        v66 = 0u;
        v67 = 0u;
        v13 = (v9 >> 4) - 1;
        v64 = 0u;
        v65 = 0u;
        v14 = ((unsigned __int8)((v9 >> 4) - 1) ^ (unsigned __int8)v9) & 0xF;
        v62 = 0u;
        v63 = 0u;
        v15 = 0u;
        v16 = 0u;
        v61 = 0u;
        memset(v60, 0, sizeof(v60));
        v17 = 0u;
        v18 = 0u;
        v19 = 0u;
        v58 = 0u;
        v59 = 0u;
        v20 = v14 - 1;
        v21 = ~v14;
        v22 = 0u;
        v23 = 0u;
        v24 = 0u;
        v25 = 0u;
        v26 = 0u;
        v27 = 0u;
        v57 = 0u;
        do
        {
          v28 = vdupq_n_s8((unsigned __int8)((v20 & v21) >> 63));
          v27 = vorrq_s8(v27, vandq_s8(*(int8x16_t *)(v7 + v11), v28));
          v26 = vorrq_s8(v26, vandq_s8(*(int8x16_t *)(v7 + v11 + 16), v28));
          v25 = vorrq_s8(v25, vandq_s8(*(int8x16_t *)(v7 + v11 + 32), v28));
          v24 = vorrq_s8(v24, vandq_s8(*(int8x16_t *)(v7 + v11 + 48), v28));
          v23 = vorrq_s8(v23, vandq_s8(*(int8x16_t *)(v7 + v11 + 64), v28));
          v22 = vorrq_s8(v22, vandq_s8(*(int8x16_t *)(v7 + v11 + 80), v28));
          v19 = vorrq_s8(v19, vandq_s8(*(int8x16_t *)(v7 + v11 + 96), v28));
          v18 = vorrq_s8(v18, vandq_s8(*(int8x16_t *)(v7 + v11 + 112), v28));
          v17 = vorrq_s8(v17, vandq_s8(*(int8x16_t *)(v7 + v11 + 128), v28));
          v16 = vorrq_s8(v16, vandq_s8(*(int8x16_t *)(v7 + v11 + 144), v28));
          v15 = vorrq_s8(v15, vandq_s8(*(int8x16_t *)(v7 + v11 + 160), v28));
          v12 = vorrq_s8(v12, vandq_s8(*(int8x16_t *)(v7 + v11 + 176), v28));
          --v20;
          ++v21;
          v11 += 192;
        }
        while ( v11 != 3072 );
        v60[0] = v24;
        LODWORD(v60[1]) = v23.i32[0];
        v61 = v22;
        v29.i64[0] = *(unsigned __int128 *)&v18 >> 32;
        v29.i64[1] = __PAIR64__(v27.u32[0], v18.u32[3]);
        v30 = vandq_s8(veorq_s8(v29, *(int8x16_t *)((char *)v60 + 4)), (int8x16_t)vdupq_n_s32(v13));
        v31 = (v23.i32[1] ^ v27.i32[1]) & v13;
        LODWORD(v57) = v30.i32[3] ^ v27.i32[0];
        DWORD1(v57) = v31 ^ v27.i32[1];
        v32 = v31 ^ v23.i32[1];
        v33 = (v23.i32[2] ^ v27.i32[2]) & v13;
        v34 = v33 ^ v27.i32[2];
        DWORD1(v60[1]) = v32;
        DWORD2(v60[1]) = v33 ^ v23.i32[2];
        v35 = (v23.i32[3] ^ v27.i32[3]) & v13;
        DWORD2(v57) = v34;
        HIDWORD(v57) = v35 ^ v27.i32[3];
        v36 = v35 ^ v23.i32[3];
        v37 = (v61.i32[0] ^ v26.i32[0]) & v13;
        v38 = v37 ^ v26.i32[0];
        HIDWORD(v60[1]) = v36;
        v61.i32[0] ^= v37;
        v39 = (v61.i32[1] ^ v26.i32[1]) & v13;
        LODWORD(v58) = v38;
        DWORD1(v58) = v39 ^ v26.i32[1];
        v40 = v39 ^ v61.i32[1];
        v41 = (v61.i32[2] ^ v26.i32[2]) & v13;
        v42 = v41 ^ v26.i32[2];
        v61.i32[1] = v40;
        v61.i32[2] ^= v41;
        v43 = (v61.i32[3] ^ v26.i32[3]) & v13;
        DWORD2(v58) = v42;
        HIDWORD(v58) = v43 ^ v26.i32[3];
        v44 = v43 ^ v61.i32[3];
        v45 = (v19.i32[0] ^ v25.i32[0]) & v13;
        v46 = v45 ^ v25.i32[0];
        v61.i32[3] = v44;
        LODWORD(v62) = v45 ^ v19.i32[0];
        v47 = (v19.i32[1] ^ v25.i32[1]) & v13;
        LODWORD(v59) = v46;
        DWORD1(v59) = v47 ^ v25.i32[1];
        v48 = v47 ^ v19.i32[1];
        v49 = (v19.i32[2] ^ v25.i32[2]) & v13;
        v50 = v49 ^ v25.i32[2];
        DWORD1(v62) = v48;
        DWORD2(v62) = v49 ^ v19.i32[2];
        v51 = (v19.i32[3] ^ v25.i32[3]) & v13;
        DWORD2(v59) = v50;
        HIDWORD(v59) = v51 ^ v25.i32[3];
        v52 = (v18.i32[0] ^ v24.i32[0]) & v13;
        LODWORD(v60[0]) = v52 ^ v24.i32[0];
        v64 = v17;
        v65 = v16;
        v66 = v15;
        v67 = v12;
        HIDWORD(v62) = v51 ^ v19.i32[3];
        LODWORD(v63) = v52 ^ v18.i32[0];
        *(int8x8_t *)((char *)&v63 + 4) = veor_s8(*(int8x8_t *)v30.i8, (int8x8_t)(*(unsigned __int128 *)&v18 >> 32));
        *(int8x16_t *)((char *)v60 + 4) = veorq_s8(v30, *(int8x16_t *)((char *)v60 + 4));
        HIDWORD(v63) = v30.i32[2] ^ v18.i32[3];
        sub_1003DBFAC(&v64);
        if ( v4 != 18 || v5 )
          break;
        sub_1003DF124(v55, &v60[1], &v57);
        sub_1003DF514(a1, &v60[1], &v57);
        sub_1003DA7B0(a1 + 12, v55, a1);
        a1[8] = xmmword_100562070;
        a1[9] = xmmword_100562080;
        a1[10] = xmmword_100562090;
        a1[11] = xmmword_1005620A0;
        v5 = 1;
        v7 += 3072;
      }
      sub_1003DB764(a1, &v57, (v5++ == 4) & (unsigned __int8)(v4 != 1));
      v7 += 3072;
    }
    while ( v5 != 5 );
    --v4;
  }
  while ( v6 );
  sub_100391B40(&v57, 192);
  return sub_100391B40(v56, 56);
}

/* ========================================================================
 * fallback sub_10013d168
 * EA: 0x10013d168
 ======================================================================== */

__int64 __fastcall sub_10013D168(__int64 a1)
{
  __int64 v1; // x20
  __int64 v3; // x0
  char *v4; // x24
  __int64 v5; // x19
  __int64 v6; // x23
  char *v7; // x21
  __int64 v8; // x26
  __int64 v9; // x22
  char *v10; // x27
  char *v11; // x28
  __int64 result; // x0
  void *v13; // x20
  __int64 v14; // x19
  unsigned __int64 v15; // x23
  __int64 v16; // x19
  __int64 v17; // x23
  __int64 v18; // x0
  __int64 v19; // x0
  __int64 v20; // x19
  void *v21; // x28
  __int64 v22; // x0
  Swift::String v23; // x0
  __int64 v24; // x19
  NSString v25; // x20
  id v26; // x28
  Class isa; // x19
  id v28; // x19
  NSString v29; // x19
  void *v30; // x22
  Class v31; // x19
  NSString v32; // x20
  __int64 v33; // x1
  __int64 v34; // x1
  __int64 v35; // x19
  __int64 v36; // [xsp+10h] [xbp-E0h] BYREF
  __int64 v37; // [xsp+18h] [xbp-D8h]
  __int64 v38; // [xsp+20h] [xbp-D0h]
  __int64 v39; // [xsp+28h] [xbp-C8h]
  __int64 v40; // [xsp+30h] [xbp-C0h]
  __int64 v41; // [xsp+38h] [xbp-B8h]
  _OWORD v42[2]; // [xsp+40h] [xbp-B0h] BYREF
  __int64 v43; // [xsp+60h] [xbp-90h]
  __int128 v44; // [xsp+70h] [xbp-80h] BYREF
  __int128 v45; // [xsp+80h] [xbp-70h]
  void *v46; // [xsp+90h] [xbp-60h]

  v3 = type metadata accessor for Calendar(0);
  v39 = *(_QWORD *)(v3 - 8);
  v40 = v3;
  v4 = (char *)&v36 - ((*(_QWORD *)(v39 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v5 = type metadata accessor for Date(0);
  v6 = *(_QWORD *)(v5 - 8);
  v7 = (char *)&v36 - ((*(_QWORD *)(v6 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v8 = type metadata accessor for Locale(0);
  v9 = *(_QWORD *)(v8 - 8);
  v10 = (char *)&v36 - ((*(_QWORD *)(v9 + 64) + 15LL) & 0xFFFFFFFFFFFFFFF0LL);
  v11 = (char *)&v36
      - ((*(_QWORD *)(*(_QWORD *)(sub_10003E4E0(&unk_100669D80, &unk_10053BF90) - 8) + 64LL) + 15LL)
       & 0xFFFFFFFFFFFFFFF0LL);
  v41 = v1;
  result = swift_unknownObjectWeakLoadStrong(v1 + OBJC_IVAR____TtC15ExternalMonitor31VTHistoryCellCollectionViewCell_iconImage);
  if ( !result )
  {
    __break(1u);
LABEL_16:
    __break(1u);
LABEL_17:
    __break(1u);
    return result;
  }
  v13 = (void *)result;
  v37 = v6;
  v38 = v5;
  if ( *(_QWORD *)(a1 + 56) )
  {
    v14 = *(_QWORD *)(a1 + 48);
    v15 = *(_QWORD *)(a1 + 56);
  }
  else
  {
    v14 = 0;
    v15 = 0xE000000000000000LL;
  }
  swift_bridgeObjectRetain();
  URL.init(string:)(v14, v15);
  swift_bridgeObjectRelease(v15);
  v16 = type metadata accessor for URL(0);
  v17 = *(_QWORD *)(v16 - 8);
  if ( (*(unsigned int (__fastcall **)(char *, __int64, __int64))(v17 + 48))(v11, 1, v16) == 1 )
  {
    sub_10005285C(v11, &unk_100669D80, &unk_10053BF90);
    v44 = 0u;
    v45 = 0u;
    v46 = nullptr;
  }
  else
  {
    *((_QWORD *)&v45 + 1) = v16;
    v46 = &protocol witness table for URL;
    v18 = sub_10004B3F4(&v44);
    (*(void (__fastcall **)(__int64, char *, __int64))(v17 + 32))(v18, v11, v16);
  }
  v19 = sub_10004F514(0);
  memset(v42, 0, sizeof(v42));
  v43 = 0;
  v20 = KingfisherWrapper<A>.setImage(with:placeholder:options:progressBlock:completionHandler:)(
          &v44,
          v42,
          0,
          0,
          0,
          0,
          0,
          v13,
          v19);
  objc_release(v13);
  swift_release(v20);
  sub_10005285C(v42, &unk_10066A9B8, &unk_10053D420);
  sub_10005285C(&v44, &unk_10066A9C0, &unk_1005413D0);
  result = swift_unknownObjectWeakLoadStrong(v41 + OBJC_IVAR____TtC15ExternalMonitor31VTHistoryCellCollectionViewCell_titleLabel);
  if ( !result )
    goto LABEL_16;
  v21 = (void *)result;
  v22 = *(_QWORD *)(a1 + 40);
  *(_QWORD *)&v44 = *(_QWORD *)(a1 + 32);
  *((_QWORD *)&v44 + 1) = v22;
  swift_bridgeObjectRetain();
  v23._countAndFlagsBits = 0x73776F7242202D20LL;
  v23._object = (void *)0xEA00000000007265LL;
  String.append(_:)(v23);
  v24 = *((_QWORD *)&v44 + 1);
  v25 = String._bridgeToObjectiveC()();
  swift_bridgeObjectRelease(v24);
  objc_msgSend(v21, "setText:", v25);
  objc_release(v21);
  objc_release(v25);
  v26 = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSDateFormatter), "init");
  objc_msgSend(v26, "setDateStyle:", 2);
  objc_msgSend(v26, "setTimeStyle:", 0);
  Locale.init(identifier:)(0x53555F6E65LL, 0xE500000000000000LL);
  isa = Locale._bridgeToObjectiveC()().super.isa;
  (*(void (__fastcall **)(char *, __int64))(v9 + 8))(v10, v8);
  objc_msgSend(v26, "setLocale:", isa);
  objc_release(isa);
  Date.init(timeIntervalSince1970:)(*(double *)(a1 + 64));
  v28 = objc_retainAutoreleasedReturnValue(objc_msgSend((id)objc_opt_self(&OBJC_CLASS___NSCalendar), "currentCalendar"));
  static Calendar._unconditionallyBridgeFromObjectiveC(_:)();
  objc_release(v28);
  LOBYTE(v28) = Calendar.isDateInToday(_:)(v7);
  (*(void (__fastcall **)(char *, __int64))(v39 + 8))(v4, v40);
  if ( ((unsigned __int8)v28 & 1) != 0 )
  {
    v29 = String._bridgeToObjectiveC()();
    objc_msgSend(v26, "setDateFormat:", v29);
    objc_release(v29);
  }
  result = swift_unknownObjectWeakLoadStrong(v41 + OBJC_IVAR____TtC15ExternalMonitor31VTHistoryCellCollectionViewCell_timeLabel);
  if ( !result )
    goto LABEL_17;
  v30 = (void *)result;
  v31 = Date._bridgeToObjectiveC()().super.isa;
  v32 = objc_retainAutoreleasedReturnValue(objc_msgSend(v26, "stringFromDate:", v31));
  objc_release(v31);
  if ( !v32 )
  {
    static String._unconditionallyBridgeFromObjectiveC(_:)(0, v33);
    v35 = v34;
    v32 = String._bridgeToObjectiveC()();
    swift_bridgeObjectRelease(v35);
  }
  objc_msgSend(v30, "setText:", v32);
  objc_release(v26);
  objc_release(v30);
  objc_release(v32);
  return (*(__int64 (__fastcall **)(char *, __int64))(v37 + 8))(v7, v38);
}

/* ========================================================================
 * fallback sub_1003d9b58
 * EA: 0x1003d9b58
 ======================================================================== */

__int64 __fastcall sub_1003D9B58(int *a1, _DWORD *a2)
{
  __int64 v4; // x4
  __int64 v5; // x5
  __int64 v6; // x2
  __int64 v7; // x6
  __int64 v8; // x15
  __int64 v9; // x17
  __int64 v10; // x12
  __int64 v11; // x13
  __int64 v12; // x9
  __int64 v13; // x10
  __int64 v14; // x11
  __int64 v15; // x14
  __int64 v16; // x16
  __int64 v17; // x0
  __int64 v18; // x3
  __int64 v19; // x21
  __int64 v20; // x23
  __int64 v21; // x5
  __int64 v22; // x22
  __int64 v23; // x6
  __int64 v24; // x24
  __int64 v25; // x4
  __int64 v26; // x5
  __int64 v27; // x2
  __int64 v28; // x24
  __int64 v29; // x7
  __int64 v30; // x17
  __int64 v31; // x22
  __int64 v32; // x6
  __int64 v33; // x15
  __int64 v34; // x3
  __int64 v35; // x16
  __int64 v36; // x0
  __int64 v37; // x15
  __int64 v38; // x11
  __int64 v39; // x10
  __int64 v40; // x16
  __int64 v41; // x14
  __int64 v42; // x17
  __int64 v43; // x2
  __int64 v44; // x13
  __int64 v45; // x3
  __int64 v46; // x0
  unsigned __int64 v47; // x14
  __int64 v48; // x15
  __int64 v49; // x11
  unsigned __int64 v50; // x8
  int v51; // w8
  int v52; // w9
  int v53; // w10
  int v54; // w11
  int v55; // w12
  int v56; // w13
  int v57; // w14
  int v58; // w15
  int v59; // w16
  int v60; // w17
  int v61; // w0
  int v62; // w1
  int v63; // w2
  int v64; // w3
  int v65; // w4
  int v66; // w5
  int v67; // w6
  int v68; // w7
  int v69; // w26
  int v70; // w14
  int v71; // w15
  int v72; // w8
  int v73; // w9
  int v74; // w16
  int v75; // w17
  int v76; // w2
  int v77; // w3
  int v78; // w4
  int v79; // w5
  int v80; // w8
  int v81; // w9
  int v82; // w12
  int v83; // w2
  int v84; // w12
  int v85; // w13
  int v86; // w12
  int v87; // w13
  int v88; // w12
  int v89; // w13
  int v90; // w12
  int v91; // w13
  int v92; // w13
  int v93; // w12
  int v94; // w12
  __int64 result; // x0
  int v96; // w13
  int v97; // w14
  int v98; // w13
  int v99; // w12
  int v100; // w13
  int v101; // w14
  int v102; // w11
  int v103; // w12
  _DWORD v104[10]; // [xsp+0h] [xbp-80h] BYREF

  sub_1003DA00C(a1);
  sub_1003DA00C(a1 + 20);
  v4 = (int)a2[20];
  v5 = (int)a2[21];
  v7 = (int)a2[22];
  v6 = (int)a2[23];
  v8 = (int)a2[24];
  v9 = (int)a2[25];
  v10 = (int)a2[26];
  v11 = (int)a2[27];
  v12 = (int)a2[28];
  v13 = (int)a2[29];
  v14 = 2 * v4;
  v15 = 2 * v5;
  v16 = 2 * v7;
  v17 = 2 * v6;
  v18 = 2 * v9;
  v19 = 38 * v13;
  v20 = 2 * v4 * v5 + 19 * v10 * 2 * v9 + 38 * v11 * v8 + 19 * v12 * 2 * v6 + 38 * v13 * v7;
  v21 = 2 * v5 * v5 + v7 * 2 * v4 + 19 * v10 * v10 + 38 * v11 * 2 * v9;
  v22 = v15 * v7 + v6 * 2 * v4 + 38 * v11 * v10;
  v23 = v7 * v7 + 2 * v6 * v15 + v8 * 2 * v4 + 38 * v11 * v11 + 2 * v10 * 19 * v12;
  v24 = v8 * v16 + 2 * v6 * v6;
  v25 = v4 * v4 + 38 * v9 * v9 + 19 * v10 * 2 * v8 + 38 * v11 * 2 * v6 + 19 * v12 * v16;
  v26 = v21 + 19 * v12 * 2 * v8;
  v27 = v16 * v6 + v8 * v15 + v9 * v14 + 19 * v12 * 2 * v11;
  v28 = v24 + 2 * v9 * v15 + v10 * v14 + 19 * v12 * v12;
  v29 = 2 * v8 * v9;
  v30 = v17 * v8 + v9 * v16;
  v31 = v22 + 19 * v12 * v18 + 38 * v13 * v8;
  v32 = v23 + 38 * v13 * v18;
  v33 = v8 * v8 + v10 * v16 + v18 * v17;
  v34 = v26 + 38 * v13 * v17;
  v35 = v29 + v10 * v17 + v11 * v16;
  v36 = v30 + v10 * v15 + v11 * v14 + 38 * v13 * v12;
  v37 = v33 + 2 * v11 * v15 + v12 * v14 + 38 * v13 * v13;
  v38 = v35 + v12 * v15 + v13 * v14;
  v39 = 2 * (v25 + 38 * v13 * v15);
  v40 = ((v39 + 0x2000000) >> 26) + 2 * v20;
  v41 = 2 * v32 + 0x2000000;
  v42 = (v41 >> 26) + 2 * (v27 + v19 * v10);
  v43 = ((v40 + 0x1000000) >> 25) + 2 * v34;
  v44 = ((v42 + 0x1000000) >> 25) + 2 * (v28 + v19 * 2 * v11);
  v45 = ((v43 + 0x2000000) >> 26) + 2 * v31;
  v46 = ((v44 + 0x2000000) >> 26) + 2 * v36;
  v47 = 2 * v32 - (v41 & 0xFFFFFFFFFC000000LL) + ((v45 + 0x1000000) >> 25);
  v48 = ((v46 + 0x1000000) >> 25) + 2 * v37;
  v49 = ((v48 + 0x2000000) >> 26) + 2 * v38;
  v50 = v39 - ((v39 + 0x2000000) & 0xFFFFFFFFFC000000LL) + 19 * ((v49 + 0x1000000) >> 25);
  a1[30] = v50 - ((v50 + 0x2000000) & 0xFC000000);
  a1[31] = v40 - ((v40 + 0x1000000) & 0xFE000000) + ((v50 + 0x2000000) >> 26);
  a1[32] = v43 - ((v43 + 0x2000000) & 0xFC000000);
  a1[33] = v45 - ((v45 + 0x1000000) & 0xFE000000);
  a1[34] = v47 - ((v47 + 0x2000000) & 0xFC000000);
  a1[35] = v42 - ((v42 + 0x1000000) & 0xFE000000) + ((v47 + 0x2000000) >> 26);
  a1[36] = v44 - ((v44 + 0x2000000) & 0xFC000000);
  a1[37] = v46 - ((v46 + 0x1000000) & 0xFE000000);
  a1[38] = v48 - ((v48 + 0x2000000) & 0xFC000000);
  a1[39] = v49 - ((v49 + 0x1000000) & 0xFE000000);
  a1[10] = a2[10] + *a2;
  a1[11] = a2[11] + a2[1];
  a1[12] = a2[12] + a2[2];
  a1[13] = a2[13] + a2[3];
  a1[14] = a2[14] + a2[4];
  a1[15] = a2[15] + a2[5];
  a1[16] = a2[16] + a2[6];
  a1[17] = a2[17] + a2[7];
  a1[18] = a2[18] + a2[8];
  a1[19] = a2[19] + a2[9];
  sub_1003DA00C(v104);
  v51 = a1[20];
  v52 = a1[21];
  v53 = *a1;
  v54 = a1[1];
  v55 = *a1 + v51;
  a1[10] = v55;
  v56 = v54 + v52;
  v57 = a1[22];
  v58 = a1[23];
  v59 = a1[2];
  v60 = a1[3];
  v61 = v59 + v57;
  v62 = v60 + v58;
  v63 = a1[24];
  v64 = a1[25];
  v65 = a1[4];
  v66 = a1[5];
  v67 = v65 + v63;
  v68 = v66 + v64;
  LODWORD(a2) = a1[26];
  LODWORD(v19) = a1[27];
  LODWORD(v31) = a1[6];
  LODWORD(v20) = a1[7];
  LODWORD(v28) = v51 - v53;
  v69 = v52 - v54;
  v70 = v57 - v59;
  v71 = v58 - v60;
  v72 = a1[28];
  v73 = a1[29];
  v74 = v63 - v65;
  v75 = v64 - v66;
  v76 = a1[8];
  v77 = a1[9];
  v78 = v76 + v72;
  v79 = v77 + v73;
  v80 = v72 - v76;
  v81 = v73 - v77;
  v82 = v104[0] - v55;
  v83 = v104[1] - v56;
  a1[11] = v56;
  a1[12] = v61;
  *a1 = v82;
  a1[1] = v83;
  v84 = v104[2] - v61;
  v85 = v104[3] - v62;
  a1[13] = v62;
  a1[14] = v67;
  a1[2] = v84;
  a1[3] = v85;
  v86 = v104[4] - v67;
  v87 = v104[5] - v68;
  a1[15] = v68;
  a1[16] = v31 + (_DWORD)a2;
  a1[4] = v86;
  a1[5] = v87;
  v88 = v104[6] - (v31 + (_DWORD)a2);
  v89 = v104[7] - (v20 + v19);
  a1[17] = v20 + v19;
  a1[18] = v78;
  a1[6] = v88;
  a1[7] = v89;
  v90 = v104[8] - v78;
  v91 = v104[9] - v79;
  a1[19] = v79;
  a1[20] = v28;
  a1[8] = v90;
  a1[9] = v91;
  v92 = a1[31];
  v93 = a1[30] - v28;
  a1[21] = v69;
  a1[22] = v70;
  a1[29] = v81;
  a1[30] = v93;
  v94 = v92 - v69;
  result = (unsigned int)a1[33];
  v96 = a1[32] - v70;
  a1[31] = v94;
  a1[32] = v96;
  a1[23] = v71;
  a1[24] = v74;
  v97 = a1[35];
  v98 = a1[34] - v74;
  a1[33] = result - v71;
  a1[34] = v98;
  a1[25] = v75;
  a1[26] = (_DWORD)a2 - v31;
  v99 = v97 - v75;
  v100 = a1[36];
  v101 = a1[37];
  a1[35] = v99;
  a1[36] = v100 - ((_DWORD)a2 - v31);
  a1[27] = v19 - v20;
  a1[28] = v80;
  v102 = a1[38];
  v103 = a1[39];
  a1[37] = v101 - (v19 - v20);
  a1[38] = v102 - v80;
  a1[39] = v103 - v81;
  return result;
}

/* ========================================================================
 * fallback sub_1003dad64
 * EA: 0x1003dad64
 ======================================================================== */

__int64 __fastcall sub_1003DAD64(__int64 a1, _DWORD *a2, int a3)
{
  int8x16_t v6; // q1
  int8x16_t v7; // q3
  int32x4_t v8; // q4
  int32x4_t v9; // q0
  int32x2_t v10; // d5
  int32x2_t v11; // d1
  unsigned int v12; // w9
  int32x2_t v13; // d3
  unsigned __int32 v14; // w13
  int32x4_t v15; // q2
  unsigned int v16; // w10
  int v17; // w9
  int v18; // w8
  int32x4_t v19; // q0
  int v20; // w9
  int v21; // w10
  int v22; // w11
  int v23; // w12
  int v24; // w13
  int32x4_t v25; // q1
  int v26; // w14
  int v27; // w15
  int v28; // w12
  int v29; // w13
  int32x4_t v30; // q1
  unsigned int v31; // w16
  int v32; // w17
  int v33; // w8
  int32x4_t v34; // q2
  int8x16_t v35; // q0
  unsigned int v36; // w9
  unsigned int v37; // w10
  unsigned int v38; // w11
  unsigned int v39; // w15
  int8x16_t v40; // q1
  unsigned int v41; // w12
  int8x16_t v42; // q1
  __int64 result; // x0
  _BYTE v44[32]; // [xsp+0h] [xbp-140h] BYREF
  _BYTE v45[32]; // [xsp+20h] [xbp-120h]
  int32x4_t v46[4]; // [xsp+40h] [xbp-100h] BYREF
  unsigned __int32 v47; // [xsp+80h] [xbp-C0h] BYREF
  int32x4_t v48; // [xsp+84h] [xbp-BCh]
  int32x2_t v49; // [xsp+94h] [xbp-ACh]
  int v50; // [xsp+9Ch] [xbp-A4h]
  int v51; // [xsp+A0h] [xbp-A0h]
  __int32 v52; // [xsp+A4h] [xbp-9Ch]
  int32x4_t v53; // [xsp+A8h] [xbp-98h]
  int32x2_t v54; // [xsp+B8h] [xbp-88h]
  _BYTE v55[32]; // [xsp+C0h] [xbp-80h] BYREF
  _BYTE v56[32]; // [xsp+E0h] [xbp-60h]

  sub_1003DAD5C(v46, a2);
  sub_1003DAD5C(v55, a2 + 16);
  *(int32x4_t *)v44 = vaddq_s32(*(int32x4_t *)v55, v46[0]);
  *(int32x4_t *)&v44[16] = vaddq_s32(*(int32x4_t *)&v55[16], v46[1]);
  *(int32x4_t *)v45 = vaddq_s32(*(int32x4_t *)v56, v46[2]);
  *(int32x4_t *)&v45[16] = vaddq_s32(*(int32x4_t *)&v56[16], v46[3]);
  *(_DWORD *)(a1 + 192) = *a2 + a2[16];
  *(_DWORD *)(a1 + 196) = a2[1] + a2[17];
  *(_DWORD *)(a1 + 200) = a2[2] + a2[18];
  *(_DWORD *)(a1 + 204) = a2[3] + a2[19];
  *(_DWORD *)(a1 + 208) = a2[4] + a2[20];
  *(_DWORD *)(a1 + 212) = a2[5] + a2[21];
  *(_DWORD *)(a1 + 216) = a2[6] + a2[22];
  *(_DWORD *)(a1 + 220) = a2[7] + a2[23];
  *(_DWORD *)(a1 + 224) = a2[8] + a2[24];
  *(_DWORD *)(a1 + 228) = a2[9] + a2[25];
  *(_DWORD *)(a1 + 232) = a2[10] + a2[26];
  *(_DWORD *)(a1 + 236) = a2[11] + a2[27];
  *(_DWORD *)(a1 + 240) = a2[12] + a2[28];
  *(_DWORD *)(a1 + 244) = a2[13] + a2[29];
  *(_DWORD *)(a1 + 248) = a2[14] + a2[30];
  *(_DWORD *)(a1 + 252) = a2[15] + a2[31];
  sub_1003DAD5C(&v47, a1 + 192);
  v6.i64[1] = *(_QWORD *)&v44[12];
  v7.i64[1] = *(_QWORD *)&v45[16];
  v8 = vdupq_n_s32(0x2FFFFFFDu);
  v9 = vaddq_s32(vsubq_s32(v48, *(int32x4_t *)&v44[4]), v8);
  v10 = vdup_n_s32(0x2FFFFFFDu);
  v11 = vadd_s32(vsub_s32(v49, *(int32x2_t *)&v44[20]), v10);
  v12 = v50 - *(_DWORD *)&v44[28] + 805306365;
  v13 = vadd_s32(vsub_s32(v54, *(int32x2_t *)&v45[24]), v10);
  v14 = (unsigned __int32)v13.i32[1] >> 28;
  v15 = vaddq_s32(vsubq_s32(v53, *(int32x4_t *)&v45[8]), v8);
  v16 = v51 - *(_DWORD *)v45 + ((unsigned __int32)v13.i32[1] >> 28) + 805306362;
  *(uint32x2_t *)v7.i8 = vsra_n_u32(
                           (uint32x2_t)(*(_QWORD *)&v13 & 0xFFFFFFF0FFFFFFFLL),
                           (uint32x2_t)vzip1_s32(vdup_laneq_s32(v15, 3), v13),
                           0x1Cu);
  v54 = *(int32x2_t *)v7.i8;
  v7.i32[0] = v52 - *(_DWORD *)&v45[4] + 805306365;
  v53 = (int32x4_t)vsraq_n_u32(
                     (uint32x4_t)(*(_OWORD *)&v15 & __PAIR128__(0xFFFFFFF0FFFFFFFLL, 0xFFFFFFF0FFFFFFFLL)),
                     (uint32x4_t)vextq_s8(vextq_s8(v7, v7, 4u), (int8x16_t)v15, 0xCu),
                     0x1Cu);
  v51 = (v16 & 0xFFFFFFF) + (v12 >> 28);
  v52 = (v7.i32[0] & 0xFFFFFFF) + (v16 >> 28);
  v17 = (v12 & 0xFFFFFFF) + ((unsigned __int32)v11.i32[1] >> 28);
  *(uint32x2_t *)v6.i8 = vsra_n_u32(
                           (uint32x2_t)(*(_QWORD *)&v11 & 0xFFFFFFF0FFFFFFFLL),
                           (uint32x2_t)vzip1_s32(vdup_laneq_s32(v9, 3), v11),
                           0x1Cu);
  v49 = *(int32x2_t *)v6.i8;
  v6.i32[0] = v47 - *(_DWORD *)v44 + 805306365;
  v48 = (int32x4_t)vsraq_n_u32(
                     (uint32x4_t)(*(_OWORD *)&v9 & __PAIR128__(0xFFFFFFF0FFFFFFFLL, 0xFFFFFFF0FFFFFFFLL)),
                     (uint32x4_t)vextq_s8(vextq_s8(v6, v6, 4u), (int8x16_t)v9, 0xCu),
                     0x1Cu);
  v50 = v17;
  v47 = v14 + (v6.i32[0] & 0xFFFFFFF);
  sub_1003DD238(a1 + 192, v55, v46);
  sub_1003DAD5C(a1, a2 + 32);
  v18 = 2 * *(_DWORD *)a1;
  *(_DWORD *)(a1 + 128) = v18;
  v19 = vshlq_n_s32(*(int32x4_t *)(a1 + 4), 1u);
  *(int32x4_t *)(a1 + 132) = v19;
  v20 = 2 * *(_DWORD *)(a1 + 20);
  v21 = 2 * *(_DWORD *)(a1 + 24);
  *(_DWORD *)(a1 + 148) = v20;
  *(_DWORD *)(a1 + 152) = v21;
  v22 = 2 * *(_DWORD *)(a1 + 28);
  v23 = 2 * *(_DWORD *)(a1 + 32);
  *(_DWORD *)(a1 + 156) = v22;
  *(_DWORD *)(a1 + 160) = v23;
  v24 = 2 * *(_DWORD *)(a1 + 36);
  *(_DWORD *)(a1 + 164) = v24;
  v25 = vshlq_n_s32(*(int32x4_t *)(a1 + 40), 1u);
  *(int32x4_t *)(a1 + 168) = v25;
  v26 = 2 * *(_DWORD *)(a1 + 56);
  v27 = 2 * *(_DWORD *)(a1 + 60);
  v28 = v23 - *(_DWORD *)(a1 + 224);
  v29 = v24 - *(_DWORD *)(a1 + 228);
  v30 = vsubq_s32(v25, *(int32x4_t *)(a1 + 232));
  v31 = v26 - *(_DWORD *)(a1 + 248);
  v32 = v27 - *(_DWORD *)(a1 + 252);
  v33 = v18 - *(_DWORD *)(a1 + 192) + 1073741820;
  v34 = vdupq_n_s32(0x3FFFFFFCu);
  v35 = (int8x16_t)vaddq_s32(vsubq_s32(v19, *(int32x4_t *)(a1 + 196)), v34);
  v36 = v20 - *(_DWORD *)(a1 + 212) + 1073741820;
  v37 = v21 - *(_DWORD *)(a1 + 216) + 1073741820;
  v38 = v22 - *(_DWORD *)(a1 + 220) + 1073741820;
  *(_DWORD *)(a1 + 184) = v26;
  *(_DWORD *)(a1 + 188) = v27;
  v39 = (unsigned int)(v32 + 1073741820) >> 28;
  v40 = (int8x16_t)vaddq_s32(v30, v34);
  v31 += 1073741820;
  v41 = v28 + v39 + 1073741816;
  *(_DWORD *)&v56[24] = (v31 & 0xFFFFFFF) + ((unsigned __int32)v40.i32[3] >> 28);
  *(_DWORD *)&v56[28] = ((v32 + 1073741820) & 0xFFFFFFF) + (v31 >> 28);
  v34.i32[0] = v29 + 1073741820;
  v42 = (int8x16_t)vsraq_n_u32(
                     (uint32x4_t)(*(_OWORD *)&v40 & __PAIR128__(0xFFFFFFF0FFFFFFFLL, 0xFFFFFFF0FFFFFFFLL)),
                     (uint32x4_t)vextq_s8(vextq_s8((int8x16_t)v34, (int8x16_t)v34, 4u), v40, 0xCu),
                     0x1Cu);
  *(int8x16_t *)&v56[8] = v42;
  *(_DWORD *)v56 = (v41 & 0xFFFFFFF) + (v38 >> 28);
  *(_DWORD *)&v56[4] = ((v29 + 1073741820) & 0xFFFFFFF) + (v41 >> 28);
  *(_DWORD *)&v55[24] = (v37 & 0xFFFFFFF) + (v36 >> 28);
  *(_DWORD *)&v55[28] = (v38 & 0xFFFFFFF) + (v37 >> 28);
  v42.i32[0] = v33;
  *(uint32x4_t *)&v55[4] = vsraq_n_u32(
                             (uint32x4_t)(*(_OWORD *)&v35 & __PAIR128__(0xFFFFFFF0FFFFFFFLL, 0xFFFFFFF0FFFFFFFLL)),
                             (uint32x4_t)vextq_s8(vextq_s8(v42, v42, 4u), v35, 0xCu),
                             0x1Cu);
  *(_DWORD *)&v55[20] = (v36 & 0xFFFFFFF) + ((unsigned __int32)v35.i32[3] >> 28);
  *(_DWORD *)v55 = v39 + (v33 & 0xFFFFFFF);
  sub_1003DA7B0(a1, v55, &v47);
  sub_1003DA7B0(a1 + 128, a1 + 192, v55);
  result = sub_1003DA7B0(a1 + 64, a1 + 192, v44);
  if ( !a3 )
    return sub_1003DA7B0(a1 + 192, &v47, v44);
  return result;
}

/* ========================================================================
 * fallback sub_1002603dc
 * EA: 0x1002603dc
 ======================================================================== */

__int64 __fastcall sub_1002603DC(__int64 a1)
{
  void *v2; // x20
  id v3; // x19
  APMMeasurement *v4; // x23
  APMDatabase *v5; // x24
  void *v6; // x25
  id v7; // x20
  id v8; // x21
  bool v9; // zf
  APMMeasurement *v10; // x23
  APMDatabase *v11; // x25
  void *v12; // x26
  unsigned int v13; // w27
  id v14; // x24
  void *v15; // x21
  __int64 v16; // x22
  id v17; // x23
  void *v18; // x22
  const __CFString *v19; // x4
  void *v20; // x0
  __int64 v21; // x3
  void *v22; // x22
  void *v23; // x0
  APMMeasurement *v24; // x23
  APMDatabase *v25; // x25
  void *v26; // x26
  unsigned int v27; // w27
  void *v28; // x23
  void *v29; // x24
  void *v30; // x26
  NSDate *v31; // x27
  double v32; // d0
  double v33; // d8
  void *v34; // x28
  void *v35; // x25
  APMEvent *v36; // x0
  __int64 v37; // x9
  void *v38; // x8
  APMEvent *v40; // [xsp+0h] [xbp-80h]
  id v41; // [xsp+8h] [xbp-78h] BYREF
  id v42; // [xsp+10h] [xbp-70h] BYREF
  id v43; // [xsp+18h] [xbp-68h] BYREF

  v2 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(a1 + 32), "name"));
  v3 = objc_retainAutoreleasedReturnValue((id)sub_100280200());
  objc_release(v2);
  v4 = objc_retainAutoreleasedReturnValue(+[APMMeasurement sharedInstance](&OBJC_CLASS___APMMeasurement, "sharedInstance"));
  v5 = objc_retainAutoreleasedReturnValue(-[APMMeasurement database](v4, "database"));
  v6 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(a1 + 32), "name"));
  v43 = nullptr;
  v7 = objc_retainAutoreleasedReturnValue(-[APMDatabase conditionalUserPropertyWithName:error:](v5, "conditionalUserPropertyWithName:error:", v6, &v43));
  v8 = objc_retain(v43);
  objc_release(v6);
  objc_release(v5);
  objc_release(v4);
  if ( v7 )
    v9 = 1;
  else
    v9 = v8 == nullptr;
  if ( !v9 )
  {
    v17 = objc_retainAutoreleasedReturnValue(+[APMMeasurement monitor](&OBJC_CLASS___APMMeasurement, "monitor"));
    v18 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(a1 + 32), "origin"));
    v19 = CFSTR("Cannot query conditional user property to remove. Name, origin, error");
    v20 = v17;
    v21 = 11017;
LABEL_10:
    objc_msgSend(v20, "logWithLevel:messageCode:message:context:context:context:", 1, v21, v19, v3, v18, v8);
    objc_release(v18);
    v16 = 0;
    goto LABEL_19;
  }
  if ( !v7 )
  {
    v17 = objc_retainAutoreleasedReturnValue(+[APMMeasurement monitor](&OBJC_CLASS___APMMeasurement, "monitor"));
    v22 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(a1 + 32), "origin"));
    objc_msgSend(
      v17,
      "logWithLevel:messageCode:message:context:context:",
      4,
      11018,
      CFSTR("Cannot remove nonexistent conditional user property. Name, origin"),
      v3,
      v22);
    v23 = v22;
    goto LABEL_18;
  }
  v10 = objc_retainAutoreleasedReturnValue(+[APMMeasurement sharedInstance](&OBJC_CLASS___APMMeasurement, "sharedInstance"));
  v11 = objc_retainAutoreleasedReturnValue(-[APMMeasurement database](v10, "database"));
  v12 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(a1 + 32), "name"));
  v42 = v8;
  v13 = -[APMDatabase deleteConditionalUserPropertyWithName:error:](
          v11,
          "deleteConditionalUserPropertyWithName:error:",
          v12,
          &v42);
  v14 = objc_retain(v42);
  objc_release(v8);
  objc_release(v12);
  objc_release(v11);
  objc_release(v10);
  if ( v13 || !v14 )
  {
    v8 = v14;
    if ( (unsigned int)objc_msgSend(v7, "isActive") )
    {
      v24 = objc_retainAutoreleasedReturnValue(+[APMMeasurement sharedInstance](&OBJC_CLASS___APMMeasurement, "sharedInstance"));
      v25 = objc_retainAutoreleasedReturnValue(-[APMMeasurement database](v24, "database"));
      v26 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(a1 + 32), "name"));
      v41 = v14;
      v27 = -[APMDatabase deleteUserAttributeWithName:error:](v25, "deleteUserAttributeWithName:error:", v26, &v41);
      v8 = objc_retain(v41);
      objc_release(v14);
      objc_release(v26);
      objc_release(v25);
      objc_release(v24);
      if ( !v27 )
      {
        if ( v8 )
        {
          v17 = objc_retainAutoreleasedReturnValue(+[APMMeasurement monitor](&OBJC_CLASS___APMMeasurement, "monitor"));
          v18 = objc_retainAutoreleasedReturnValue(objc_msgSend(v7, "origin"));
          v19 = CFSTR("Cannot remove conditional user property. Failed to delete the user property. Name, origin, error");
          v20 = v17;
          v21 = 11020;
          goto LABEL_10;
        }
      }
    }
    v28 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(a1 + 32), "expiredEvent"));
    objc_release(v28);
    if ( !v28 )
    {
      v16 = 1;
      goto LABEL_20;
    }
    v40 = objc_alloc(&OBJC_CLASS___APMEvent);
    v17 = objc_retainAutoreleasedReturnValue(objc_msgSend(v7, "origin"));
    v29 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(a1 + 32), "expiredEvent"));
    v30 = objc_retainAutoreleasedReturnValue(objc_msgSend(v29, "name"));
    v31 = objc_retainAutoreleasedReturnValue(+[NSDate date](&OBJC_CLASS___NSDate, "date"));
    -[NSDate timeIntervalSince1970](v31, "timeIntervalSince1970");
    v33 = v32;
    v34 = objc_retainAutoreleasedReturnValue(objc_msgSend(*(id *)(a1 + 32), "expiredEvent"));
    v35 = objc_retainAutoreleasedReturnValue(objc_msgSend(v34, "parameters"));
    v36 = -[APMEvent initWithOrigin:isPublic:name:timestamp:previousTimestamp:parameters:](
            v40,
            "initWithOrigin:isPublic:name:timestamp:previousTimestamp:parameters:",
            v17,
            0,
            v30,
            v35,
            v33,
            0.0);
    v37 = *(_QWORD *)(*(_QWORD *)(a1 + 40) + 8LL);
    v38 = *(void **)(v37 + 40);
    *(_QWORD *)(v37 + 40) = v36;
    objc_release(v38);
    objc_release(v35);
    objc_release(v34);
    objc_release(v31);
    objc_release(v30);
    v23 = v29;
LABEL_18:
    objc_release(v23);
    v16 = 1;
    goto LABEL_19;
  }
  v17 = objc_retainAutoreleasedReturnValue(+[APMMeasurement monitor](&OBJC_CLASS___APMMeasurement, "monitor"));
  v15 = objc_retainAutoreleasedReturnValue(objc_msgSend(v7, "origin"));
  objc_msgSend(
    v17,
    "logWithLevel:messageCode:message:context:context:context:",
    1,
    11019,
    CFSTR("Cannot delete conditional user property. Name, origin, error"),
    v3,
    v15,
    v14);
  objc_release(v15);
  v16 = 0;
  v8 = v14;
LABEL_19:
  objc_release(v17);
LABEL_20:
  objc_release(v7);
  objc_release(v8);
  objc_release(v3);
  return v16;
}

/* ========================================================================
 * fallback sub_1003d938c
 * EA: 0x1003d938c
 ======================================================================== */

// local variable allocation has failed, the output may be wrong!
int8x16_t __fastcall sub_1003D938C(__int64 a1, int a2, int a3)
{
  unsigned __int8 v3; // w14
  unsigned int v4; // w8
  int8x16_t v5; // q0
  char *v6; // x9
  int8x16_t v7; // q1
  int8x16_t v8; // q4
  unsigned int v9; // w10
  int8x16_t v10; // q2
  unsigned int v11; // w11
  int8x16_t v12; // q5
  int8x16_t v13; // q6
  unsigned int v14; // w12
  int8x16_t *v15; // x13
  int8x16_t v16; // q7
  int8x16_t v17; // q16
  unsigned int v18; // w15
  int8x16_t v19; // q17
  int8x16_t v20; // q18
  unsigned int v21; // w16
  int8x16_t *v22; // x17
  int8x16_t v23; // q19
  int8x16_t v24; // q20
  unsigned int v25; // w1
  int8x16_t v26; // q22
  int8x16_t v27; // q23
  unsigned int v28; // w14
  int8x16_t v29; // q21
  int8x16_t *v30; // x3
  int8x16_t v31; // q24
  int8x16_t v32; // q25
  int8x16_t v33; // q27
  int8x16_t v34; // q26
  int8x16_t v35; // q28
  int8x16_t v36; // q30
  int8x16_t v37; // q29
  int8x16_t v38; // q31
  int8x16_t v39; // q8
  int8x16_t v40; // q3
  int8x16_t v41; // q26
  int8x16_t v42; // q1
  int8x16_t v43; // q4
  int8x16_t v44; // q0
  int v45; // w4
  int v46; // w5
  int8x16_t v47; // q9
  int8x16_t v48; // q10
  int8x16_t v49; // q11
  int8x16_t v50; // q12
  int v51; // w6
  int v52; // w7
  int8x16_t v53; // q13
  int8x16_t v54; // q14
  int v55; // w19
  char *v56; // x20
  char *v57; // x21
  int8x16_t v58; // q15
  int8x16_t v59; // q28
  int8x16_t v60; // q27
  int8x16_t v61; // q29
  int v62; // w22
  char *v63; // x23
  int8x16_t v64; // q27
  int8x16_t v65; // q8
  int8x16_t v66; // q31
  int8x16_t v67; // q27
  int8x16_t v68; // q4
  int8x16_t v69; // q1
  int8x16_t v70; // q0
  int8x16_t v71; // q6
  int8x16_t v72; // q2
  int v73; // w4
  int v74; // w8
  int v75; // w10
  int v76; // w8
  int v77; // w11
  int v78; // w8
  int32x4_t v79; // q1
  int32x4_t v80; // q0
  int v81; // w8
  int8x16_t v82; // q6
  int8x16_t v83; // q3
  int8x16_t v84; // q5
  int8x16_t result; // q0
  __int64 v86; // [xsp+20h] [xbp-D0h] OVERLAPPED
  int8x16_t v87; // [xsp+28h] [xbp-C8h] OVERLAPPED
  __int128 v88; // [xsp+38h] [xbp-B8h] OVERLAPPED
  __int64 v89; // [xsp+48h] [xbp-A8h] OVERLAPPED

  *(_OWORD *)a1 = 0u;
  *(_OWORD *)(a1 + 16) = 0u;
  *(_QWORD *)(a1 + 32) = 0;
  *(_DWORD *)a1 = 1;
  v3 = a3 - 2 * (a3 & (a3 >> 7));
  *(_OWORD *)(a1 + 40) = 0u;
  *(_OWORD *)(a1 + 56) = 0u;
  *(_QWORD *)(a1 + 72) = 0;
  *(_DWORD *)(a1 + 40) = 1;
  v4 = ((v3 ^ 1) - 1) >> 31;
  v5 = (int8x16_t)vdupq_n_s32(v4);
  v6 = (char *)&unk_10055A294 + 960 * a2;
  v7 = *(int8x16_t *)(a1 + 48);
  v8 = *((int8x16_t *)v6 + 5);
  v9 = ((v3 ^ 2) - 1) >> 31;
  v10 = (int8x16_t)vdupq_n_s32(v9);
  v11 = ((v3 ^ 3) - 1) >> 31;
  v12 = (int8x16_t)vdupq_n_s32(v11);
  v13 = *((int8x16_t *)v6 + 18);
  v14 = ((v3 ^ 4) - 1) >> 31;
  v15 = (int8x16_t *)(v6 + 360);
  v16 = (int8x16_t)vdupq_n_s32(v14);
  v17 = *(int8x16_t *)(v6 + 440);
  v18 = ((v3 ^ 5) - 1) >> 31;
  v19 = (int8x16_t)vdupq_n_s32(v18);
  v20 = *((int8x16_t *)v6 + 35);
  v21 = ((v3 ^ 6) - 1) >> 31;
  v22 = (int8x16_t *)(v6 + 600);
  v23 = (int8x16_t)vdupq_n_s32(v21);
  v24 = *(int8x16_t *)(v6 + 680);
  v25 = ((v3 ^ 7) - 1) >> 31;
  v26 = (int8x16_t)vdupq_n_s32(v25);
  v27 = *((int8x16_t *)v6 + 50);
  v28 = ((v3 ^ 8) - 1) >> 31;
  v29 = (int8x16_t)vdupq_n_s32(v28);
  v30 = (int8x16_t *)(v6 + 840);
  v31 = *(int8x16_t *)(v6 + 920);
  v32 = vbslq_s8(
          v29,
          *(int8x16_t *)(v6 + 904),
          vbslq_s8(
            v26,
            *((int8x16_t *)v6 + 49),
            vbslq_s8(
              v23,
              *(int8x16_t *)(v6 + 664),
              vbslq_s8(
                v19,
                *((int8x16_t *)v6 + 34),
                vbslq_s8(
                  v16,
                  *(int8x16_t *)(v6 + 424),
                  vbslq_s8(
                    v12,
                    *((int8x16_t *)v6 + 19),
                    vbslq_s8(
                      v10,
                      *(int8x16_t *)(v6 + 184),
                      vbslq_s8(v5, *((int8x16_t *)v6 + 4), *(int8x16_t *)(a1 + 64)))))))));
  *(_OWORD *)(a1 + 80) = 0u;
  *(_OWORD *)(a1 + 96) = 0u;
  v33 = vbslq_s8(v5, *(int8x16_t *)v6, *(int8x16_t *)a1);
  v34 = *((int8x16_t *)v6 + 2);
  v35 = *((int8x16_t *)v6 + 3);
  v36 = *((int8x16_t *)v6 + 17);
  v37 = *(int8x16_t *)(v6 + 360);
  v38 = *((int8x16_t *)v6 + 30);
  v39 = *((int8x16_t *)v6 + 45);
  *(int8x16_t *)(a1 + 16) = vbslq_s8(
                              v29,
                              *(int8x16_t *)(v6 + 856),
                              vbslq_s8(
                                v26,
                                *((int8x16_t *)v6 + 46),
                                vbslq_s8(
                                  v23,
                                  *(int8x16_t *)(v6 + 616),
                                  vbslq_s8(
                                    v19,
                                    *((int8x16_t *)v6 + 31),
                                    vbslq_s8(
                                      v16,
                                      *(int8x16_t *)(v6 + 376),
                                      vbslq_s8(
                                        v12,
                                        *((int8x16_t *)v6 + 16),
                                        vbslq_s8(
                                          v10,
                                          *(int8x16_t *)(v6 + 136),
                                          vbslq_s8(v5, *((int8x16_t *)v6 + 1), *(int8x16_t *)(a1 + 16)))))))));
  v40 = *(int8x16_t *)(a1 + 16);
  v41 = vbslq_s8(v5, v34, *(int8x16_t *)(a1 + 32));
  v42 = vbslq_s8(v5, v35, v7);
  v43 = vbslq_s8(v5, v8, *(int8x16_t *)(a1 + 80));
  v44 = vbslq_s8(v5, *((int8x16_t *)v6 + 6), *(int8x16_t *)(a1 + 96));
  v45 = *((_DWORD *)v6 + 28);
  v46 = *((_DWORD *)v6 + 29);
  v47 = *(int8x16_t *)(v6 + 152);
  v48 = *(int8x16_t *)(v6 + 168);
  v49 = *(int8x16_t *)(v6 + 200);
  v50 = *(int8x16_t *)(v6 + 216);
  v51 = *((_DWORD *)v6 + 58);
  v52 = *((_DWORD *)v6 + 59);
  v53 = *((int8x16_t *)v6 + 20);
  v54 = *((int8x16_t *)v6 + 21);
  v55 = *((_DWORD *)v6 + 88);
  v56 = v6 + 480;
  v57 = v6 + 240;
  v59 = *((int8x16_t *)v6 + 32);
  v58 = *((int8x16_t *)v6 + 33);
  v60 = vbslq_s8(v16, v37, vbslq_s8(v12, *((int8x16_t *)v6 + 15), vbslq_s8(v10, *(int8x16_t *)(v6 + 120), v33)));
  v61 = *((int8x16_t *)v6 + 36);
  v62 = *((_DWORD *)v6 + 148);
  v63 = v6 + 720;
  v64 = vbslq_s8(
          v29,
          *(int8x16_t *)(v6 + 840),
          vbslq_s8(v26, v39, vbslq_s8(v23, *(int8x16_t *)(v6 + 600), vbslq_s8(v19, v38, v60))));
  v66 = *((int8x16_t *)v6 + 47);
  v65 = *((int8x16_t *)v6 + 48);
  *(int8x16_t *)a1 = v64;
  v67 = *((int8x16_t *)v6 + 51);
  LODWORD(v6) = *((_DWORD *)v6 + 208);
  v68 = vbslq_s8(v10, v49, v43);
  v69 = vbslq_s8(
          v26,
          v65,
          vbslq_s8(v23, v22[3], vbslq_s8(v19, v58, vbslq_s8(v16, v15[3], vbslq_s8(v12, v13, vbslq_s8(v10, v48, v42))))));
  v70 = vbslq_s8(
          v26,
          v67,
          vbslq_s8(v23, v22[6], vbslq_s8(v19, v61, vbslq_s8(v16, v15[6], vbslq_s8(v12, v54, vbslq_s8(v10, v50, v44))))));
  v71 = v30[3];
  v72 = vbslq_s8(
          v29,
          v30[2],
          vbslq_s8(
            v26,
            v66,
            vbslq_s8(
              v23,
              v22[2],
              vbslq_s8(v19, v59, vbslq_s8(v16, v15[2], vbslq_s8(v12, v36, vbslq_s8(v10, v47, v41)))))));
  *(_QWORD *)(a1 + 112) = 0;
  v73 = v45 & v4;
  v74 = v52 & v9 | v46 & v4 & ~v9;
  v75 = v55 & v11 | (v51 & v9 | v73 & ~v9) & ~v11;
  v76 = v22[7].i32[1] & v21
      | (*((_DWORD *)v56 + 29) & v18 | (v15[7].i32[1] & v14 | (*((_DWORD *)v57 + 29) & v11 | v74 & ~v11) & ~v14) & ~v18)
      & ~v21;
  v77 = ((unsigned int)a3 >> 7) & 1;
  LODWORD(v6) = (unsigned int)v6 & v25
              | (v22[7].i32[0] & v21 | (v62 & v18 | (v15[7].i32[0] & v14 | v75 & ~v14) & ~v18) & ~v21) & ~v25;
  v78 = *((_DWORD *)v63 + 29) & v25 | v76 & ~v25;
  *(int8x16_t *)(a1 + 32) = v72;
  *(int8x16_t *)(a1 + 48) = vbslq_s8(v29, v71, v69);
  *(int8x16_t *)(a1 + 64) = v32;
  v79 = (int32x4_t)vbslq_s8(
                     v29,
                     v31,
                     vbslq_s8(
                       v26,
                       v27,
                       vbslq_s8(v23, v24, vbslq_s8(v19, v20, vbslq_s8(v16, v17, vbslq_s8(v12, v53, v68))))));
  v80 = (int32x4_t)vbslq_s8(v29, v30[6], v70);
  LODWORD(v6) = v30[7].i32[0] & v28 | (unsigned int)v6 & ~v28;
  v81 = v30[7].i32[1] & v28 | v78 & ~v28;
  v86 = *(_QWORD *)(a1 + 72);
  v89 = *(_QWORD *)(a1 + 32);
  v88 = *(_OWORD *)(a1 + 16);
  v87 = *(int8x16_t *)a1;
  LODWORD(v15) = -v77;
  v82 = (int8x16_t)vdupq_n_s32(-v77);
  v83 = vbslq_s8(v82, *(int8x16_t *)(a1 + 56), v40);
  *(int8x16_t *)a1 = vbslq_s8(v82, *(int8x16_t *)(a1 + 40), *(int8x16_t *)a1);
  *(int8x16_t *)(a1 + 16) = v83;
  v84 = vbslq_s8(v82, *(int8x16_t *)((char *)&v88 - 8), *(int8x16_t *)(a1 + 48));
  *(int8x16_t *)(a1 + 32) = vbslq_s8(v82, *(int8x16_t *)&v86, *(int8x16_t *)(a1 + 32));
  *(int8x16_t *)(a1 + 48) = v84;
  result = vbslq_s8(v82, (int8x16_t)vnegq_s32(v80), (int8x16_t)v80);
  *(int8x16_t *)(a1 + 64) = vbslq_s8(v82, *(int8x16_t *)(&v89 - 1), *(int8x16_t *)(a1 + 64));
  *(int8x16_t *)(a1 + 80) = vbslq_s8(v82, (int8x16_t)vnegq_s32(v79), (int8x16_t)v79);
  --v77;
  *(int8x16_t *)(a1 + 96) = result;
  *(_DWORD *)(a1 + 112) = -(int)v6 & (unsigned int)v15 | (unsigned int)v6 & v77;
  *(_DWORD *)(a1 + 116) = -v81 & (unsigned int)v15 | v81 & v77;
  return result;
}

/* ========================================================================
 * fallback sub_1003da7b0
 * EA: 0x1003da7b0
 ======================================================================== */

unsigned int *__fastcall sub_1003DA7B0(unsigned int *result, int32x4_t *a2, int32x4_t *a3)
{
  unsigned __int64 v3; // x13
  unsigned __int64 v4; // x4
  unsigned __int64 v5; // x22
  __int64 v6; // x9
  int32x4_t v7; // q2
  int32x4_t v8; // q1
  int32x4_t v9; // q0
  __int64 v10; // x10
  __int64 v11; // x11
  __int64 v12; // x12
  unsigned __int64 v13; // x15
  _QWORD *v14; // x14
  __int8 *v15; // x20
  __int64 v16; // x23
  int32x4_t *v17; // x25
  __int64 v18; // x26
  __int64 v19; // x4
  unsigned __int64 v20; // x16
  unsigned __int64 v21; // x13
  unsigned __int64 v22; // x27
  __int64 v23; // x21
  __int64 v24; // x28
  unsigned __int64 v25; // x30
  unsigned __int64 v26; // x22
  __int64 v27; // x30
  __int64 v28; // x22
  unsigned __int64 v29; // x3
  __int64 v30; // x21
  unsigned __int64 v31; // x28
  unsigned __int64 v32; // x4
  int64x2_t v33; // q0
  int64x2_t v34; // q1
  int64x2_t v35; // q2
  uint32x2_t *v36; // x22
  int32x2_t *v37; // x5
  int32x2_t *v38; // x30
  uint32x2_t *v39; // x6
  int64x2_t v40; // q3
  int64x2_t v41; // q4
  int64x2_t v42; // q5
  uint32x2_t v43; // d7
  uint32x2_t v44; // d16
  uint32x2_t v45; // d17
  __int64 v46; // x17
  __int64 v47; // x28
  __int8 *v48; // x3
  unsigned int *v49; // x5
  __int32 *v50; // x6
  __int64 v51; // x21
  char *v52; // x28
  char *v53; // x24
  unsigned int v54; // t1
  unsigned int v55; // t1
  unsigned __int64 v56; // x8
  unsigned int v57; // w11
  unsigned __int64 v58; // x9
  unsigned int v59; // w10
  __int64 v60; // [xsp+8h] [xbp-E8h]
  __int64 v61; // [xsp+10h] [xbp-E0h]
  __int64 v62; // [xsp+18h] [xbp-D8h]
  __int64 v63; // [xsp+20h] [xbp-D0h]
  __int64 v64; // [xsp+28h] [xbp-C8h]
  __int64 v65; // [xsp+40h] [xbp-B0h]
  _OWORD v66[2]; // [xsp+50h] [xbp-A0h] BYREF
  int32x4_t v67; // [xsp+70h] [xbp-80h]
  int32x4_t v68; // [xsp+80h] [xbp-70h] BYREF

  v3 = 0;
  v4 = 0;
  v5 = 0;
  v6 = a3[2].u32[0];
  v7 = vaddq_s32(a3[2], *a3);
  v8 = vaddq_s32(a2[3], a2[1]);
  v67 = vaddq_s32(a2[2], *a2);
  v68 = v8;
  v9 = vaddq_s32(a3[3], a3[1]);
  v66[0] = v7;
  v66[1] = v9;
  v10 = a3->u32[0];
  v11 = v7.u32[0];
  v12 = v7.u32[1];
  v13 = (unsigned __int64)v66 | 0xC;
  v65 = v7.u32[2];
  v64 = v7.u32[3];
  v63 = v9.u32[0];
  v62 = v9.u32[1];
  v14 = (__int64 *)((char *)a3[2].i64 + 4);
  v15 = &v68.i8[12];
  v61 = v9.u32[2];
  v60 = v9.u32[3];
  v16 = 1;
  v17 = a2;
  v18 = 7;
  do
  {
    v22 = v3 + 8;
    v23 = v3;
    v24 = v10 * a2->u32[v3];
    v25 = v5 + v11 * v67.u32[v3];
    v26 = v4 + v6 * a2[2].u32[v3];
    if ( v16 != 1 )
    {
      v24 += a3->u32[1] * (unsigned __int64)a2->u32[v23 - 1];
      v25 += v12 * v67.u32[v23 - 1];
      v26 += a3[2].u32[1] * (unsigned __int64)a2[1].u32[v23 + 3];
      if ( v16 != 2 )
      {
        v24 += a3->u32[2] * (unsigned __int64)a2->u32[v23 - 2];
        v25 += v65 * v67.u32[v23 - 2];
        v26 += a3[2].u32[2] * (unsigned __int64)a2[1].u32[v23 + 2];
        if ( v16 != 3 )
        {
          v24 += a3->u32[3] * (unsigned __int64)a2->u32[v23 - 3];
          v25 += v64 * v67.u32[v23 - 3];
          v26 += a3[2].u32[3] * (unsigned __int64)a2[1].u32[v23 + 1];
          if ( v16 != 4 )
          {
            v24 += a3[1].u32[0] * (unsigned __int64)a2[-1].u32[v23];
            v25 += v63 * v67.u32[v23 - 4];
            v26 += a3[3].u32[0] * (unsigned __int64)a2[1].u32[v23];
            if ( v16 != 5 )
            {
              v24 += a3[1].u32[1] * (unsigned __int64)*(unsigned int *)((char *)&a2[-1] + v23 * 4 - 4);
              v25 += v62 * v67.u32[v23 - 5];
              v26 += a3[3].u32[1] * (unsigned __int64)a2->u32[v23 + 3];
              if ( v16 != 6 )
              {
                v24 += a3[1].u32[2] * (unsigned __int64)*(unsigned int *)((char *)&a2[-1] + v23 * 4 - 8);
                v25 += v61 * v67.u32[v23 - 6];
                v26 += a3[3].u32[2] * (unsigned __int64)a2->u32[v23 + 2];
                if ( v16 != 7 )
                {
                  v24 += a3[1].u32[3] * (unsigned __int64)*(unsigned int *)((char *)&a2[-1] + v23 * 4 - 12);
                  v25 += v60 * v67.u32[v23 - 7];
                  v26 += a3[3].u32[3] * (unsigned __int64)a2->u32[v23 + 1];
                }
              }
            }
          }
        }
      }
    }
    v27 = v25 - v24;
    v28 = v26 + v24;
    if ( v3 > 6 )
    {
      v19 = 0;
      goto LABEL_3;
    }
    v29 = 7 - v3;
    if ( 7 - v3 < 4 )
    {
      v19 = 0;
      v30 = v16;
      v31 = v3;
LABEL_18:
      v46 = 0;
      v47 = 4 * v31;
      v48 = &v17->i8[-v47];
      v49 = (unsigned int *)v66 + v30;
      v50 = &a3[2].i32[v30];
      v51 = v30 - 8;
      v52 = &v15[-v47];
      do
      {
        v53 = &v48[4 * v46];
        v28 -= (unsigned int)*(v50 - 8) * (unsigned __int64)*((unsigned int *)v53 + 7);
        v54 = *v49++;
        v19 += v54 * (unsigned __int64)*(unsigned int *)&v52[4 * v46];
        v55 = *v50++;
        v27 += v55 * (unsigned __int64)*((unsigned int *)v53 + 15);
        --v46;
      }
      while ( v51 != v46 );
      goto LABEL_3;
    }
    v32 = v18 & 0xFFFFFFFFFFFFFFFCLL;
    v30 = v16 + (v29 & 0xFFFFFFFFFFFFFFFCLL);
    v31 = v3 + (v29 & 0xFFFFFFFFFFFFFFFCLL);
    v33 = 0u;
    v34 = (int64x2_t)(unsigned __int64)v28;
    v35 = (int64x2_t)(unsigned __int64)v27;
    v36 = (uint32x2_t *)v13;
    v38 = (int32x2_t *)&v68.u64[1];
    v37 = (int32x2_t *)&a2[3].u64[1];
    v39 = (uint32x2_t *)v14;
    v40 = 0u;
    v41 = 0u;
    v42 = 0u;
    do
    {
      v34 = (int64x2_t)vmlsl_u32((uint64x2_t)v34, v39[-4], (uint32x2_t)vrev64_s32(v37[-4]));
      v33 = (int64x2_t)vmlsl_u32((uint64x2_t)v33, v39[-3], (uint32x2_t)vrev64_s32(v37[-5]));
      v40 = (int64x2_t)vmlal_u32((uint64x2_t)v40, v36[-1], (uint32x2_t)vrev64_s32(*v38));
      v41 = (int64x2_t)vmlal_u32((uint64x2_t)v41, *v36, (uint32x2_t)vrev64_s32(v38[-1]));
      v43 = (uint32x2_t)vrev64_s32(v37[-1]);
      v44 = *v39;
      v45 = v39[1];
      v39 += 2;
      v35 = (int64x2_t)vmlal_u32((uint64x2_t)v35, v44, (uint32x2_t)vrev64_s32(*v37));
      v37 -= 2;
      v42 = (int64x2_t)vmlal_u32((uint64x2_t)v42, v45, v43);
      v38 -= 2;
      v36 += 2;
      v32 -= 4LL;
    }
    while ( v32 );
    v27 = vaddvq_s64(vaddq_s64(v42, v35));
    v19 = vaddvq_s64(vaddq_s64(v41, v40));
    v28 = vaddvq_s64(vaddq_s64(v33, v34));
    if ( v29 != (v29 & 0xFFFFFFFFFFFFFFFCLL) )
      goto LABEL_18;
LABEL_3:
    result[v3] = (v28 + v19) & 0xFFFFFFF;
    v20 = v3 + 1;
    v21 = v19 + v27;
    result[v22] = (v19 + v27) & 0xFFFFFFF;
    v4 = (unsigned __int64)(v28 + v19) >> 28;
    v5 = v21 >> 28;
    ++v16;
    --v18;
    v14 = (_QWORD *)((char *)v14 + 4);
    v13 += 4LL;
    v17 = (int32x4_t *)((char *)v17 + 4);
    v15 += 4;
    v3 = v20;
  }
  while ( v20 != 8 );
  v56 = v4 + v5 + result[8];
  v57 = result[1];
  v58 = v5 + *result;
  v59 = result[9] + (v56 >> 28);
  result[8] = v56 & 0xFFFFFFF;
  result[9] = v59;
  *result = v58 & 0xFFFFFFF;
  result[1] = v57 + (v58 >> 28);
  return result;
}

/* ========================================================================
 * fallback sub_10036c3d4
 * EA: 0x10036c3d4
 ======================================================================== */

__int64 __fastcall sub_10036C3D4(__int16 a1, long double a2)
{
  int v2; // w0
  __int16 v3; // w8
  int v5; // [xsp+Ch] [xbp-14h]
  unsigned int v6; // [xsp+Ch] [xbp-14h]
  __int64 v7; // [xsp+10h] [xbp-10h]
  __int64 v8; // [xsp+10h] [xbp-10h]
  __int16 v10; // [xsp+1Ah] [xbp-6h]

  if ( (byte_100682C88 & 1) != 0 )
  {
    if ( a1 )
    {
      if ( (byte_100682A01 & 1) != 0 )
      {
        if ( dword_100682A04 < 0 && a1 > 0 || dword_100682A04 > 0 && a1 < 0 )
          dword_100682A04 = 0;
        for ( dword_100682A04 += a1; ; dword_100682A04 -= v10 )
        {
          abs(a2);
          if ( v2 < 120 )
            return 0;
          if ( dword_100682A04 <= 0 )
            v3 = -120;
          else
            v3 = 120;
          v10 = v3;
          v7 = sub_10036B370(0);
          if ( !v7 )
            return (unsigned int)-1;
          *(_BYTE *)(v7 + 28) = 3;
          *(_DWORD *)(v7 + 24) = 1;
          *(_DWORD *)(v7 + 29) = 167772160;
          *(_DWORD *)(v7 + 33) = dword_100697350 < 5 ? 9 : 10;
          *(_WORD *)(v7 + 37) = bswap32((unsigned __int16)v10) >> 16;
          *(_WORD *)(v7 + 39) = *(_WORD *)(v7 + 37);
          *(_WORD *)(v7 + 41) = 0;
          v5 = sub_10036EEE8(&unk_1006828D8, v7, v7);
          if ( v5 )
            break;
        }
        if ( v5 != 2 )
          __assert_rtn("LiSendHighResScrollEvent", "InputStream.c", 1213, "err == 2");
        if ( off_100697310 )
          ((void (__fastcall *)(const char *))off_100697310)("Input queue reached maximum size limit\n");
        sub_10036B45C(v7);
        return 2;
      }
      else
      {
        v8 = sub_10036B370(0);
        if ( v8 )
        {
          *(_BYTE *)(v8 + 28) = 3;
          *(_DWORD *)(v8 + 24) = 1;
          *(_DWORD *)(v8 + 29) = 167772160;
          if ( dword_100697350 < 5 )
            *(_DWORD *)(v8 + 33) = 9;
          else
            *(_DWORD *)(v8 + 33) = 10;
          *(_WORD *)(v8 + 37) = bswap32((unsigned __int16)a1) >> 16;
          *(_WORD *)(v8 + 39) = *(_WORD *)(v8 + 37);
          *(_WORD *)(v8 + 41) = 0;
          v6 = sub_10036EEE8(&unk_1006828D8, v8, v8);
          if ( v6 )
          {
            if ( v6 != 2 )
              __assert_rtn("LiSendHighResScrollEvent", "InputStream.c", 1246, "err == 2");
            if ( off_100697310 )
              ((void (__fastcall *)(const char *))off_100697310)("Input queue reached maximum size limit\n");
            sub_10036B45C(v8);
          }
          return v6;
        }
        else
        {
          return (unsigned int)-1;
        }
      }
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return (unsigned int)-2;
  }
}

/* ========================================================================
 * fallback sub_1001d3d2c
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
 * fallback sub_10027d3d0
 * EA: 0x10027d3d0
 ======================================================================== */

void __fastcall sub_10027D3D0(void *a1, void *a2, unsigned int a3, void *a4)
{
  id v7; // x19
  id v8; // x20
  id v9; // x21
  __CFString *v10; // x0
  void *v11; // x22
  NSString *v12; // x0
  NSString *v13; // x22
  NSString *v14; // x23
  NSString *v15; // x26
  void ***v16; // x23
  __int64 v17; // x27
  void *v18; // x28
  size_t v19; // x25
  char *v20; // x0
  char *v21; // x24
  void *v22; // x0
  void *v23; // x25
  const __CFString *v24; // x0
  const __CFString *v25; // x24
  id v26; // x24
  NSNumber *v27; // x25
  id v28; // x26
  APMValue *v29; // x22
  void *v30; // x22
  APMValue *v31; // x22
  void **v32; // [xsp+0h] [xbp-80h] BYREF
  __int64 v33; // [xsp+8h] [xbp-78h]
  __int64 (__fastcall *v34)(); // [xsp+10h] [xbp-70h]
  void *v35; // [xsp+18h] [xbp-68h]
  id v36; // [xsp+20h] [xbp-60h]
  unsigned int v37; // [xsp+28h] [xbp-58h]

  v7 = objc_retain(a1);
  v8 = objc_retain(a2);
  v9 = objc_retain(a4);
  v10 = objc_retainAutoreleasedReturnValue(objc_msgSend(v7, "objectForKeyedSubscript:", CFSTR("_ev")));
  if ( !v10 )
  {
    v11 = objc_retainAutoreleasedReturnValue(objc_msgSend(v7, "objectForKeyedSubscript:", CFSTR("_el")));
    objc_release(v11);
    if ( v11 )
      goto LABEL_4;
    v12 = (NSString *)objc_retain(v8);
    if ( v12 )
    {
      v13 = v12;
      if ( ((unsigned int)-[NSString isKindOfClass:](
                            v12,
                            "isKindOfClass:",
                            +[NSString class](&OBJC_CLASS___NSString, "class"))
          & 1) == 0 )
      {
        v14 = objc_retainAutoreleasedReturnValue(NSStringFromClass((Class)-[NSString class](v13, "class")));
        objc_release(v13);
        v13 = v14;
      }
      if ( -[NSString apm_UTF32Length](v13, "apm_UTF32Length") <= a3 )
      {
        v25 = objc_retain(v13);
      }
      else
      {
        v32 = _NSConcreteStackBlock;
        v33 = 3254779904LL;
        v34 = sub_10027F5CC;
        v35 = &unk_1005CA018;
        v15 = objc_retain(v13);
        v36 = v15;
        v37 = a3;
        v16 = objc_retainBlock(&v32);
        v17 = 4 * a3;
        v18 = objc_msgSend(CFSTR("..."), "length", v32, v33, v34, v35);
        v19 = v17 + 4LL * (_QWORD)v18;
        v20 = (char *)malloc(v19);
        if ( v20 )
        {
          v21 = v20;
          -[NSString apm_getUTF32Bytes:maxBytes:](v15, "apm_getUTF32Bytes:maxBytes:", v20, v17);
          objc_msgSend(CFSTR("..."), "apm_getUTF32Bytes:maxBytes:", &v21[v17], 4LL * (_QWORD)v18);
          v22 = objc_msgSend(
                  objc_alloc((Class)&OBJC_CLASS___NSString),
                  "initWithBytesNoCopy:length:encoding:freeWhenDone:",
                  v21,
                  v19,
                  2617245952LL,
                  1);
          v23 = v22;
          if ( v22 )
          {
            v24 = objc_retain(v22);
          }
          else
          {
            v28 = objc_retainAutoreleasedReturnValue(+[APMMeasurement monitor](&OBJC_CLASS___APMMeasurement, "monitor"));
            objc_msgSend(v28, "logWithLevel:messageCode:message:", 2, 18001, CFSTR("Trimmed UTF32 string was nil"));
            objc_release(v28);
            free(v21);
            v24 = (const __CFString *)objc_retainAutoreleasedReturnValue((id)((__int64 (__fastcall *)(void ***))v16[2])(v16));
          }
          v25 = v24;
          objc_release(v23);
        }
        else
        {
          v26 = objc_retainAutoreleasedReturnValue(+[APMMeasurement monitor](&OBJC_CLASS___APMMeasurement, "monitor"));
          v27 = objc_retainAutoreleasedReturnValue(+[NSNumber numberWithUnsignedLong:](&OBJC_CLASS___NSNumber, "numberWithUnsignedLong:", v19));
          objc_msgSend(
            v26,
            "logWithLevel:messageCode:message:context:",
            2,
            18000,
            CFSTR("Failed to malloc bytes for trimming. Bytes"),
            v27);
          objc_release(v27);
          objc_release(v26);
          v25 = (const __CFString *)objc_retainAutoreleasedReturnValue((id)((__int64 (__fastcall *)(void ***))v16[2])(v16));
        }
        objc_release(v16);
        objc_release(v36);
      }
      objc_release(v13);
    }
    else
    {
      v25 = CFSTR("(nil)");
    }
    v29 = -[APMValue initWithString:](objc_alloc(&OBJC_CLASS___APMValue), "initWithString:", v25);
    objc_msgSend(v7, "setObject:forKeyedSubscript:", v29, CFSTR("_ev"));
    objc_release(v29);
    if ( v9 )
    {
      if ( (unsigned int)objc_msgSend(v9, "isKindOfClass:", +[NSString class](&OBJC_CLASS___NSString, "class")) )
        v30 = objc_msgSend(v9, "apm_UTF32Length");
      else
        v30 = nullptr;
      if ( (unsigned int)objc_msgSend(v9, "isKindOfClass:", +[NSArray class](&OBJC_CLASS___NSArray, "class")) )
        v30 = objc_msgSend(v9, "count");
      v31 = -[APMValue initWithInt64:](objc_alloc(&OBJC_CLASS___APMValue), "initWithInt64:", v30);
      objc_msgSend(v7, "setObject:forKeyedSubscript:", v31, CFSTR("_el"));
      objc_release(v31);
    }
    v10 = (__CFString *)v25;
  }
  objc_release(v10);
LABEL_4:
  objc_release(v9);
  objc_release(v8);
  objc_release(v7);
}

/* ========================================================================
 * fallback sub_100443dc4
 * EA: 0x100443dc4
 ======================================================================== */

int8x16_t __fastcall sub_100443DC4(
        _QWORD *a1,
        signed __int128 *a2,
        _QWORD *a3,
        int8x16_t *a4,
        unsigned __int64 a5,
        int a6)
{
  unsigned __int64 v6; // x24
  unsigned __int64 v8; // x23
  __int64 v12; // x8
  __int64 v13; // x9
  unsigned __int128 v15; // kr00_16
  __int64 v16; // x9
  int8x16_t result; // q0
  __int128 v18; // kr10_16
  unsigned __int64 v19; // x11
  unsigned __int64 v20; // x8
  int v21; // w9
  unsigned int v22; // w9
  __int8 v23; // w11
  __int64 v24; // x8
  __int64 v25; // x22
  __int64 v26; // x24
  __int64 v27; // x9
  int8x16_t v28; // kr30_16
  unsigned __int64 v29; // x8
  int v30; // w9
  int8x16_t *v31; // x10
  unsigned __int64 v32; // x11
  __int64 *v33; // x12
  __int64 *v34; // x13
  __int64 v35; // t1
  unsigned __int64 v36; // x14
  unsigned __int64 v37; // x16
  __int64 v38; // x10
  __int64 v39; // x12
  __int64 v40; // d0
  unsigned int v41; // w9
  __int8 v42; // w12
  signed __int128 v43; // [xsp+0h] [xbp-50h] BYREF
  signed __int128 v44; // [xsp+10h] [xbp-40h] BYREF

  v6 = a5 - 16;
  if ( a5 < 0x10 )
    return result;
  v8 = a5;
  v44 = *a2;
  ((void (__fastcall *)(signed __int128 *, signed __int128 *, _QWORD))a1[3])(&v44, &v44, a1[1]);
  if ( (v8 & 0xF) != 0 && a6 == 0 )
    v8 = v6;
  if ( v8 < 0x10 )
  {
    if ( !a6 )
    {
LABEL_21:
      v24 = 135;
      if ( v44 >= 0 )
        v24 = 0;
      v25 = v24 ^ (2 * v44);
      v26 = v44 >> 63;
      v27 = a3[1] ^ v26;
      *(_QWORD *)&v43 = *a3 ^ v25;
      *((_QWORD *)&v43 + 1) = v27;
      ((void (__fastcall *)(signed __int128 *, signed __int128 *, _QWORD))a1[2])(&v43, &v43, *a1);
      *(_QWORD *)&v43 = v43 ^ v25;
      *((_QWORD *)&v43 + 1) ^= v26;
      v28 = (int8x16_t)v43;
      if ( !v8 )
        goto LABEL_43;
      if ( v8 >= 8 )
      {
        v30 = 0;
        v29 = 0;
        v36 = (unsigned __int64)a4[1].u64 + v8;
        v37 = (unsigned __int64)a3 + v8 + 16;
        if ( (v36 <= (unsigned __int64)&v43 || &a4[1] >= (int8x16_t *)((char *)&v43 + v8))
          && (v37 <= (unsigned __int64)&v43 || a3 + 2 >= (_QWORD *)((char *)&v43 + v8))
          && ((unsigned __int64)&a4[1] >= v37 || (unsigned __int64)(a3 + 2) >= v36) )
        {
          v38 = 0;
          v29 = v8 & 0xFFFFFFFFFFFFFFF8LL;
          v30 = v8 & 0xFFFFFFF8;
          do
          {
            v39 = ((_DWORD)v38 + 16) & 0xFFFFFFF8;
            v40 = *(_QWORD *)((char *)a3 + v39);
            *(__int64 *)((char *)a4->i64 + v39) = *(_QWORD *)((char *)&v43 + v38);
            *(_QWORD *)((char *)&v43 + v38) = v40;
            v38 += 8;
          }
          while ( v29 != v38 );
          if ( v8 == v29 )
            goto LABEL_42;
        }
      }
      else
      {
        v29 = 0;
        v30 = 0;
      }
      v41 = v30 + 16;
      do
      {
        v42 = *((_BYTE *)a3 + v41);
        a4->i8[v41] = *((_BYTE *)&v43 + v29);
        *((_BYTE *)&v43 + v29) = v42;
        v29 = v41 - 15;
        ++v41;
      }
      while ( v8 > v29 );
LABEL_42:
      v28 = (int8x16_t)v43;
LABEL_43:
      v43 = *(_OWORD *)&v28 ^ v44;
      ((void (__fastcall *)(signed __int128 *, signed __int128 *, _QWORD))a1[2])(&v43, &v43, *a1);
      result = veorq_s8((int8x16_t)v44, (int8x16_t)v43);
      *a4 = result;
      return result;
    }
LABEL_13:
    if ( !v8 )
    {
LABEL_19:
      *(_QWORD *)&v43 = v12 ^ v44;
      *((_QWORD *)&v43 + 1) = v13 ^ *((_QWORD *)&v44 + 1);
      ((void (__fastcall *)(signed __int128 *, signed __int128 *, _QWORD))a1[2])(&v43, &v43, *a1);
      result = veorq_s8((int8x16_t)v43, (int8x16_t)v44);
      v43 = (signed __int128)result;
      a4[-1] = result;
      return result;
    }
    if ( v8 >= 8 )
    {
      v21 = 0;
      v20 = 0;
      if ( (a4 >= (int8x16_t *)((char *)&v43 + v8) || &a4->i8[v8] <= (__int8 *)&v43)
        && (a4 >= (int8x16_t *)((char *)a3 + v8) || a3 >= (__int64 *)((char *)a4->i64 + v8))
        && ((signed __int128 *)((char *)a3 + v8) <= &v43 || a3 >= (_QWORD *)((char *)&v43 + v8)) )
      {
        v20 = v8 & 8;
        v21 = v8 & 0xFFFFFFF8;
        v31 = (int8x16_t *)&v43;
        v32 = v8 & 0xFFFFFFFFFFFFFFF8LL;
        v33 = (__int64 *)a4;
        v34 = a3;
        do
        {
          v35 = *v34++;
          *v33++ = v31->i64[0];
          v31->i64[0] = v35;
          v31 = (int8x16_t *)((char *)v31 + 8);
          v32 -= 8LL;
        }
        while ( v32 );
        if ( v8 == v20 )
          goto LABEL_18;
      }
    }
    else
    {
      v20 = 0;
      v21 = 0;
    }
    v22 = v21 + 1;
    do
    {
      v23 = *((_BYTE *)a3 + v20);
      a4->i8[v20] = *((_BYTE *)&v43 + v20);
      *((_BYTE *)&v43 + v20) = v23;
      v20 = v22++;
    }
    while ( v8 > v20 );
LABEL_18:
    v13 = *((_QWORD *)&v43 + 1);
    v12 = v43;
    goto LABEL_19;
  }
  v15 = v44;
  while ( 1 )
  {
    v16 = *((_QWORD *)&v15 + 1) ^ a3[1];
    *(_QWORD *)&v43 = v15 ^ *a3;
    *((_QWORD *)&v43 + 1) = v16;
    ((void (__fastcall *)(signed __int128 *, signed __int128 *, _QWORD))a1[2])(&v43, &v43, *a1);
    v18 = v44;
    v12 = v43 ^ v44;
    *(_QWORD *)&v43 = v12;
    *((_QWORD *)&v43 + 1) ^= *((_QWORD *)&v44 + 1);
    v13 = *((_QWORD *)&v43 + 1);
    a4->i64[0] = v12;
    a4->i64[1] = v13;
    v8 -= 16LL;
    if ( !v8 )
      return result;
    ++a4;
    a3 += 2;
    v19 = v18 >> 63;
    v44 = (*((__int64 *)&v18 + 1) >> 63) & 0x87 ^ __PAIR128__(v19, 2 * (__int64)v18);
    v15 = __PAIR128__(v19, v44);
    if ( v8 <= 0xF )
    {
      if ( !a6 )
        goto LABEL_21;
      goto LABEL_13;
    }
  }
}

/* ========================================================================
 * fallback sub_1003d8e14
 * EA: 0x1003d8e14
 ======================================================================== */

_DWORD *__fastcall sub_1003D8E14(_DWORD *result, int *a2, int *a3)
{
  __int64 v3; // x22
  __int64 v4; // x5
  __int64 v5; // x7
  __int64 v6; // x20
  __int64 v7; // x23
  __int64 v8; // x17
  __int64 v9; // x4
  __int64 v10; // x19
  __int64 v11; // x6
  __int64 v12; // x14
  __int64 v13; // x21
  __int64 v14; // x16
  __int64 v15; // x15
  __int64 v16; // x1
  __int64 v17; // x25
  __int64 v18; // x30
  __int64 v19; // x24
  __int64 v20; // x26
  __int64 v21; // x28
  __int64 v22; // x15
  __int64 v23; // x13
  __int64 v24; // x14
  __int64 v25; // x13
  __int64 v26; // x28
  __int64 v27; // x27
  __int64 v28; // x17
  __int64 v29; // x2
  __int64 v30; // x25
  __int64 v31; // x3
  __int64 v32; // x4
  __int64 v33; // x15
  __int64 v34; // x22
  __int64 v35; // x10
  __int64 v36; // x11
  __int64 v37; // x16
  __int64 v38; // x8
  __int64 v39; // x13
  __int64 v40; // x3
  __int64 v41; // x5
  __int64 v42; // x9
  __int64 v43; // x11
  __int64 v44; // x15
  __int64 v45; // x10
  unsigned __int64 v46; // x16
  unsigned __int64 v47; // x14
  __int64 v48; // [xsp+0h] [xbp-D0h]
  __int64 v49; // [xsp+8h] [xbp-C8h]
  __int64 v50; // [xsp+10h] [xbp-C0h]
  __int64 v51; // [xsp+18h] [xbp-B8h]
  __int64 v52; // [xsp+20h] [xbp-B0h]
  __int64 v53; // [xsp+28h] [xbp-A8h]
  __int64 v54; // [xsp+30h] [xbp-A0h]
  __int64 v55; // [xsp+40h] [xbp-90h]
  __int64 v56; // [xsp+48h] [xbp-88h]
  __int64 v57; // [xsp+50h] [xbp-80h]
  __int64 v58; // [xsp+58h] [xbp-78h]
  __int64 v59; // [xsp+68h] [xbp-68h]

  v3 = a2[8];
  v5 = *a3;
  v4 = a3[1];
  v6 = a3[2];
  v7 = a3[3];
  v8 = a3[4];
  v58 = a3[5];
  v9 = *a2;
  v10 = a2[2];
  v11 = a2[4];
  v12 = a2[5];
  v13 = a2[6];
  v14 = 2LL * a2[3];
  v15 = a2[3];
  v59 = v15;
  v49 = a2[7];
  v55 = 2LL * a2[9];
  v56 = a2[1];
  v16 = a2[9];
  v17 = v5 * v15 + v4 * v10 + v6 * v56 + v7 * v9 + 19 * v8 * v16;
  v50 = v12;
  v18 = 2 * v12;
  v20 = a3[6];
  v19 = a3[7];
  v21 = v5 * v9 + 19 * v4 * v55 + 19 * v6 * v3 + 19 * v7 * 2 * v49 + 19 * v8 * v13 + 19 * v58 * 2 * v12 + 19 * v20 * v11;
  v22 = v5 * v56 + v4 * v9 + 19 * v6 * v16 + 19 * v7 * v3 + 19 * v8 * v49 + 19 * v58 * v13 + 19 * v20 * v12;
  v23 = v5 * v12 + v4 * v11 + v6 * v59 + v7 * v10 + v8 * v56;
  v24 = v8;
  v53 = v21 + 19 * v19 * v14;
  v54 = v14;
  v48 = v5 * v11 + v4 * v14 + v6 * v10 + v7 * 2 * v56 + v8 * v9 + 19 * v58 * v55 + 19 * v20 * v3 + 19 * v19 * 2 * v49;
  v52 = v22 + 19 * v19 * v11;
  v25 = v23 + v58 * v9 + 19 * v20 * v16 + 19 * v19 * v3;
  v51 = v5 * v10
      + v4 * 2 * v56
      + v6 * v9
      + 19 * v7 * v55
      + 19 * v8 * v3
      + 19 * v58 * 2 * v49
      + 19 * v20 * v13
      + 19 * v19 * v18;
  v26 = v5 * v13 + v4 * v18 + v6 * v11 + v7 * v14 + v8 * v10 + v58 * 2 * v56 + v20 * v9;
  v27 = v9;
  v28 = v17 + 19 * v58 * v3 + 19 * v20 * v49 + 19 * v19 * v13;
  v30 = a3[8];
  v29 = a3[9];
  v57 = v26 + 19 * v19 * v55 + 19 * v30 * v3 + 19 * v29 * 2 * v49;
  v31 = v3;
  v32 = v5 * v3 + v4 * 2 * v49;
  v33 = v5 * v16 + v4 * v3 + v6 * v49;
  v34 = v48 + 19 * v30 * v13 + 19 * v29 * v18;
  v35 = v33 + v7 * v13 + v24 * v50;
  v36 = v5 * v49
      + v4 * v13
      + v6 * v50
      + v7 * v11
      + v24 * v59
      + v58 * v10
      + v20 * v56
      + v19 * v27
      + 19 * v30 * v16
      + 19 * v29 * v31;
  v37 = v53 + 19 * v30 * v10 + 19 * v29 * 2 * v56;
  v38 = v52 + 19 * v30 * v59 + 19 * v29 * v10 + ((v37 + 0x2000000) >> 26);
  v39 = v25 + 19 * v30 * v49 + 19 * v29 * v13 + ((v34 + 0x2000000) >> 26);
  v40 = v51 + 19 * v30 * v11 + 19 * v29 * v54 + ((v38 + 0x1000000) >> 25);
  v41 = v57 + ((v39 + 0x1000000) >> 25);
  v42 = v28 + 19 * v30 * v50 + 19 * v29 * v11 + ((v40 + 0x2000000) >> 26);
  v43 = v36 + ((v41 + 0x2000000) >> 26);
  v44 = v32
      + v6 * v13
      + v7 * v18
      + v24 * v11
      + v58 * v54
      + v20 * v10
      + v19 * 2 * v56
      + v30 * v27
      + 19 * v29 * v55
      + ((v43 + 0x1000000) >> 25);
  v45 = v35 + v58 * v11 + v20 * v59 + v19 * v10 + v30 * v56 + v29 * v27 + ((v44 + 0x2000000) >> 26);
  v46 = v37 - ((v37 + 0x2000000) & 0xFFFFFFFFFC000000LL) + 19 * ((v45 + 0x1000000) >> 25);
  *result = v46 - ((v46 + 0x2000000) & 0xFC000000);
  result[1] = v38 - ((v38 + 0x1000000) & 0xFE000000) + ((v46 + 0x2000000) >> 26);
  v47 = v34 - ((v34 + 0x2000000) & 0xFFFFFFFFFC000000LL) + ((v42 + 0x1000000) >> 25);
  result[2] = v40 - ((v40 + 0x2000000) & 0xFC000000);
  result[3] = v42 - ((v42 + 0x1000000) & 0xFE000000);
  result[4] = v47 - ((v47 + 0x2000000) & 0xFC000000);
  result[5] = v39 - ((v39 + 0x1000000) & 0xFE000000) + ((v47 + 0x2000000) >> 26);
  result[6] = v41 - ((v41 + 0x2000000) & 0xFC000000);
  result[7] = v43 - ((v43 + 0x1000000) & 0xFE000000);
  result[8] = v44 - ((v44 + 0x2000000) & 0xFC000000);
  result[9] = v45 - ((v45 + 0x1000000) & 0xFE000000);
  return result;
}

/* ========================================================================
 * fallback sub_1003db764
 * EA: 0x1003db764
 ======================================================================== */

unsigned int *__fastcall sub_1003DB764(int32x4_t *a1, int32x4_t *a2, int a3)
{
  int32x4_t *v5; // x20
  int32x4_t v6; // q1
  int32x4_t v7; // q1
  __int32 v8; // w1
  __int32 v9; // w2
  __int32 v10; // w5
  __int32 v11; // w6
  __int32 v12; // w24
  __int32 v13; // w25
  __int32 v14; // w28
  __int32 v15; // w30
  __int32 v16; // w15
  __int32 v17; // w8
  __int32 v18; // w9
  __int32 v19; // w16
  unsigned __int32 v20; // w17
  __int32 v21; // w2
  unsigned __int32 v22; // w0
  unsigned __int32 v23; // w1
  unsigned __int32 v24; // w3
  unsigned __int32 v25; // w4
  unsigned __int32 v26; // w5
  __int32 v27; // w23
  __int32 v28; // w24
  __int32 v29; // w25
  unsigned __int32 v30; // w26
  __int32 v31; // w28
  unsigned __int32 v32; // w27
  __int32 v33; // w12
  unsigned __int32 v34; // w11
  unsigned __int32 v35; // w6
  unsigned __int32 v36; // w7
  __int32 v37; // w23
  __int32 v38; // w24
  __int32 v39; // w30
  unsigned __int32 v40; // w19
  __int32 v41; // w23
  __int32 v42; // w8
  __int32 v43; // w9
  __int32 v44; // w13
  __int32 v45; // w24
  unsigned int v46; // w9
  unsigned int v47; // w8
  unsigned int v48; // w12
  int32x4_t v49; // q1
  int32x4_t v50; // q1
  unsigned int *result; // x0
  __int32 v52; // [xsp+10h] [xbp-140h]
  __int32 v53; // [xsp+14h] [xbp-13Ch]
  __int32 v54; // [xsp+18h] [xbp-138h]
  __int32 v55; // [xsp+1Ch] [xbp-134h]
  __int32 v56; // [xsp+20h] [xbp-130h]
  __int32 v57; // [xsp+24h] [xbp-12Ch]
  __int32 v58; // [xsp+28h] [xbp-128h]
  int32x4_t v60; // [xsp+30h] [xbp-120h] BYREF
  __int32 v61; // [xsp+40h] [xbp-110h]
  __int32 v62; // [xsp+44h] [xbp-10Ch]
  __int32 v63; // [xsp+48h] [xbp-108h]
  __int32 v64; // [xsp+4Ch] [xbp-104h]
  __int32 v65; // [xsp+50h] [xbp-100h]
  __int32 v66; // [xsp+54h] [xbp-FCh]
  __int32 v67; // [xsp+58h] [xbp-F8h]
  __int32 v68; // [xsp+5Ch] [xbp-F4h]
  __int32 v69; // [xsp+60h] [xbp-F0h]
  __int32 v70; // [xsp+64h] [xbp-ECh]
  __int32 v71; // [xsp+68h] [xbp-E8h]
  __int32 v72; // [xsp+6Ch] [xbp-E4h]
  int32x4_t v73; // [xsp+70h] [xbp-E0h] BYREF
  int32x4_t v74; // [xsp+80h] [xbp-D0h]
  int32x4_t v75; // [xsp+90h] [xbp-C0h]
  int32x4_t v76; // [xsp+A0h] [xbp-B0h]
  int32x4_t v77; // [xsp+B0h] [xbp-A0h] BYREF
  int32x4_t v78; // [xsp+C0h] [xbp-90h]
  int32x4_t v79; // [xsp+D0h] [xbp-80h]
  int32x4_t v80; // [xsp+E0h] [xbp-70h]

  v5 = a1 + 4;
  sub_1003DD238(&v73, &a1[4], a1);
  sub_1003DA7B0((unsigned int *)&v77, a2, &v73);
  v6 = vaddq_s32(a1[5], a1[1]);
  v73 = vaddq_s32(a1[4], *a1);
  v74 = v6;
  v7 = vaddq_s32(a1[7], a1[3]);
  v75 = vaddq_s32(a1[6], a1[2]);
  v76 = v7;
  sub_1003DA7B0((unsigned int *)v5, a2 + 4, &v73);
  sub_1003DA7B0((unsigned int *)a1, a2 + 8, a1 + 12);
  v9 = a1[4].i32[0];
  v8 = a1[4].i32[1];
  v58 = v9 + v77.i32[0];
  v57 = v8 + v77.i32[1];
  v10 = a1[4].i32[2];
  v11 = a1[4].i32[3];
  v56 = v10 + v77.i32[2];
  v55 = v11 + v77.i32[3];
  v12 = a1[5].i32[0];
  v13 = a1[5].i32[1];
  v54 = v12 + v78.i32[0];
  v53 = v13 + v78.i32[1];
  v14 = a1[5].i32[2];
  v15 = a1[5].i32[3];
  v52 = v14 + v78.i32[2];
  v16 = v15 + v78.i32[3];
  v17 = a1[6].i32[0];
  v18 = a1[6].i32[1];
  v19 = v17 + v79.i32[0];
  v20 = v9 - v77.i32[0];
  v21 = v18 + v79.i32[1];
  v22 = v8 - v77.i32[1];
  v23 = v10 - v77.i32[2];
  v24 = v11 - v77.i32[3];
  v25 = v12 - v78.i32[0];
  v26 = v13 - v78.i32[1];
  v27 = a1[6].i32[2];
  v28 = a1[6].i32[3];
  v29 = v27 + v79.i32[2];
  v30 = v14 - v78.i32[2];
  v31 = v28 + v79.i32[3];
  v32 = v15 - v78.i32[3];
  v33 = v17 - v79.i32[0];
  v34 = v18 - v79.i32[1];
  v35 = v27 - v79.i32[2];
  v36 = v28 - v79.i32[3];
  v37 = a1[7].i32[0];
  v38 = a1[7].i32[1];
  v39 = v37 + v80.i32[0];
  v40 = v37 - v80.i32[0];
  v41 = v38 + v80.i32[1];
  LODWORD(a2) = v38 - v80.i32[1];
  v42 = a1[7].i32[2];
  v43 = a1[7].i32[3];
  v44 = v42 + v80.i32[2];
  v45 = v43 + v80.i32[3];
  v20 += 536870910;
  v22 += 536870910;
  v23 += 536870910;
  v24 += 536870910;
  v25 += 536870910;
  v26 += 536870910;
  v30 += 536870910;
  v32 += 536870910;
  v46 = v43 - v80.i32[3] + 536870910;
  v34 += 536870910;
  v35 += 536870910;
  v36 += 536870910;
  v40 += 536870910;
  LODWORD(a2) = (_DWORD)a2 + 536870910;
  v47 = v42 - v80.i32[2] + 536870910;
  v48 = v33 + (v46 >> 28) + 536870908;
  v60.i64[0] = __PAIR64__(v57, v58);
  v60.i64[1] = __PAIR64__(v55, v56);
  v61 = v54;
  v62 = v53;
  v63 = v52;
  v64 = v16;
  v65 = v19;
  v66 = v21;
  v67 = v29;
  v68 = v31;
  v69 = v39;
  v70 = v41;
  v71 = v44;
  v72 = v45;
  v76.i32[2] = (v47 & 0xFFFFFFF) + ((unsigned int)a2 >> 28);
  v76.i32[3] = (v46 & 0xFFFFFFF) + (v47 >> 28);
  v76.i32[0] = (v40 & 0xFFFFFFF) + (v36 >> 28);
  v76.i32[1] = ((unsigned int)a2 & 0xFFFFFFF) + (v40 >> 28);
  v75.i32[2] = (v35 & 0xFFFFFFF) + (v34 >> 28);
  v75.i32[3] = (v36 & 0xFFFFFFF) + (v35 >> 28);
  v75.i32[0] = (v48 & 0xFFFFFFF) + (v32 >> 28);
  v75.i32[1] = (v34 & 0xFFFFFFF) + (v48 >> 28);
  v74.i32[2] = (v30 & 0xFFFFFFF) + (v26 >> 28);
  v74.i32[3] = (v32 & 0xFFFFFFF) + (v30 >> 28);
  v74.i32[0] = (v25 & 0xFFFFFFF) + (v24 >> 28);
  v74.i32[1] = (v26 & 0xFFFFFFF) + (v25 >> 28);
  v73.i32[2] = (v23 & 0xFFFFFFF) + (v22 >> 28);
  v73.i32[3] = (v24 & 0xFFFFFFF) + (v23 >> 28);
  v73.i32[0] = (v46 >> 28) + (v20 & 0xFFFFFFF);
  v73.i32[1] = (v22 & 0xFFFFFFF) + (v20 >> 28);
  sub_1003DD238(v5, &a1[8], a1);
  v49 = vaddq_s32(a1[9], a1[1]);
  v77 = vaddq_s32(a1[8], *a1);
  v78 = v49;
  v50 = vaddq_s32(a1[11], a1[3]);
  v79 = vaddq_s32(a1[10], a1[2]);
  v80 = v50;
  sub_1003DA7B0((unsigned int *)&a1[8], &v77, v5);
  sub_1003DA7B0((unsigned int *)a1, v5, &v73);
  result = sub_1003DA7B0((unsigned int *)v5, &v77, &v60);
  if ( !a3 )
    return sub_1003DA7B0((unsigned int *)&a1[12], &v73, &v60);
  return result;
}

/* ========================================================================
 * fallback sub_10033d5f0
 * EA: 0x10033d5f0
 ======================================================================== */

__int64 __fastcall sub_10033D5F0(__int64 result)
{
  int v1; // w9
  unsigned int v2; // w10
  unsigned int v3; // w11
  unsigned int v4; // w12
  int v5; // w8
  int v6; // w17
  int8x16_t *v7; // x14
  int8x16_t *v8; // x15
  int8x16_t v9; // q0
  int8x16_t v10; // q1
  int8x16_t v11; // q2
  unsigned __int64 v12; // x8
  unsigned __int64 v13; // x9
  int v14; // w10
  int v15; // w10
  __int64 v16; // x11
  bool v17; // cf
  __int64 v18; // x13
  __int64 v19; // x14
  int *v20; // x11
  int *v21; // x12
  int8x16_t *v22; // x15
  int8x16_t *v23; // x16
  __int64 v24; // x17
  int8x16_t v25; // q4
  int v26; // t1
  int32x4_t v27; // q0
  uint32x4_t v28; // q1
  int32x4_t v29; // q2
  int8x16_t v30; // q3
  int32x4_t v31; // q4
  int8x16_t v32; // q5
  int v33; // w16
  int v34; // w16
  __int64 v35; // x17
  bool v36; // cf
  __int64 v37; // x2
  __int64 v38; // x3
  int8x16_t *v39; // x17
  int8x16_t *v40; // x1
  __int64 v41; // x4
  int8x16_t v42; // t1
  uint32x4_t v43; // q7
  uint32x4_t v44; // q16
  uint32x4_t v45; // q6
  __int32 v46; // t1
  int v47; // w11
  int v48; // w11

  v1 = *(_DWORD *)(result + 72);
  v2 = *(unsigned __int8 *)(result + 80);
  v3 = *(unsigned __int8 *)(result + 81);
  v4 = *(unsigned __int8 *)(result + 82);
  v5 = *(_DWORD *)(result + 36);
  v6 = v5 - 1;
  *(_DWORD *)(result + 36) = v5 - 1;
  if ( !v5 )
    return result;
  v7 = *(int8x16_t **)result;
  v8 = *(int8x16_t **)(result + 24);
  if ( (v1 & 1) == 0 )
  {
    if ( *(_DWORD *)(result + 32) )
    {
      v9.i64[0] = 0xFF000000FFLL;
      v9.i64[1] = 0xFF000000FFLL;
      v10.i64[0] = 0xFF000000FF00LL;
      v10.i64[1] = 0xFF000000FF00LL;
      v11.i64[0] = 0xFF000000FF0000LL;
      v11.i64[1] = 0xFF000000FF0000LL;
      v12 = *(_QWORD *)(result + 24);
      v13 = *(_QWORD *)result;
      while ( 1 )
      {
        v15 = *(_DWORD *)(result + 32);
        if ( !v15 )
          goto LABEL_6;
        v16 = (unsigned int)(v15 - 1);
        if ( (unsigned int)v16 >= 7 && (v12 < v13 + 4 * v16 + 4 ? (v17 = v13 >= v12 + 4 * v16 + 4) : (v17 = 1), v17) )
        {
          v18 = v16 + 1;
          v19 = (v16 + 1) & 0x1FFFFFFF8LL;
          v15 -= v19;
          v20 = (int *)(v12 + 4 * v19);
          v21 = (int *)(v13 + 4 * v19);
          v22 = (int8x16_t *)(v13 + 16);
          v23 = (int8x16_t *)(v12 + 16);
          v24 = v19;
          do
          {
            v25 = vorrq_s8(
                    vorrq_s8(vandq_s8(*v22, v10), vandq_s8((int8x16_t)vshrq_n_u32(*(uint32x4_t *)v22, 0x10u), v9)),
                    vandq_s8((int8x16_t)vshlq_n_s32(*(int32x4_t *)v22, 0x10u), v11));
            v23[-1] = vorrq_s8(
                        vorrq_s8(
                          vandq_s8(v22[-1], v10),
                          vandq_s8((int8x16_t)vshrq_n_u32((uint32x4_t)v22[-1], 0x10u), v9)),
                        vandq_s8((int8x16_t)vshlq_n_s32((int32x4_t)v22[-1], 0x10u), v11));
            *v23 = v25;
            v22 += 2;
            v23 += 2;
            v24 -= 8;
          }
          while ( v24 );
          if ( v18 == v19 )
            goto LABEL_5;
        }
        else
        {
          v20 = (int *)v12;
          v21 = (int *)v13;
        }
        do
        {
          v26 = *v21++;
          *v20++ = v26 & 0xFF00 | BYTE2(v26) | ((unsigned __int8)v26 << 16);
          --v15;
        }
        while ( v15 );
LABEL_5:
        v6 = *(_DWORD *)(result + 36);
LABEL_6:
        v14 = v6;
        v13 += *(int *)(result + 16);
        v12 += *(int *)(result + 40);
        *(_DWORD *)(result + 36) = --v6;
        if ( !v14 )
          goto LABEL_45;
      }
    }
    v47 = 1 - v5;
    v12 = *(_QWORD *)(result + 24);
    v13 = *(_QWORD *)result;
    do
    {
      v13 += *(int *)(result + 16);
      v12 += *(int *)(result + 40);
      ++v47;
    }
    while ( v47 != 1 );
    goto LABEL_44;
  }
  if ( *(_DWORD *)(result + 32) )
  {
    v27 = vdupq_n_s32(v2);
    v28 = (uint32x4_t)vdupq_n_s32(0x80808081);
    v29 = vdupq_n_s32(v3);
    v30.i64[0] = 0xFF000000FFLL;
    v30.i64[1] = 0xFF000000FFLL;
    v31 = vdupq_n_s32(v4);
    v32.i64[0] = 0xFFFF0000FFFF0000LL;
    v32.i64[1] = 0xFFFF0000FFFF0000LL;
    v12 = *(_QWORD *)(result + 24);
    v13 = *(_QWORD *)result;
    while ( 1 )
    {
      v34 = *(_DWORD *)(result + 32);
      if ( !v34 )
        goto LABEL_24;
      v35 = (unsigned int)(v34 - 1);
      if ( (unsigned int)v35 >= 3
        && (v8 < (int8x16_t *)((char *)v7->i64 + 4 * v35 + 4)
          ? (v36 = v7 >= (int8x16_t *)((char *)v8->i64 + 4 * v35 + 4))
          : (v36 = 1),
            v36) )
      {
        v37 = v35 + 1;
        v38 = (v35 + 1) & 0x1FFFFFFFCLL;
        v34 -= v38;
        v39 = (int8x16_t *)((char *)v8 + 4 * v38);
        v40 = (int8x16_t *)((char *)v7 + 4 * v38);
        v41 = v38;
        do
        {
          v42 = *v7++;
          v43 = (uint32x4_t)vmulq_s32((int32x4_t)vandq_s8((int8x16_t)vshrq_n_u32((uint32x4_t)v42, 0x10u), v30), v27);
          v44 = (uint32x4_t)vmulq_s32((int32x4_t)vandq_s8((int8x16_t)vshrq_n_u32((uint32x4_t)v42, 8u), v30), v29);
          v45 = (uint32x4_t)vmulq_s32((int32x4_t)vandq_s8(v42, v30), v31);
          *v8++ = vorrq_s8(
                    vorrq_s8(
                      (int8x16_t)(*(_OWORD *)&vshlq_n_s32(
                                                vuzp2q_s32(
                                                  (int32x4_t)vmull_u32(*(uint32x2_t *)v44.i8, *(uint32x2_t *)v28.i8),
                                                  (int32x4_t)vmull_high_u32(v44, v28)),
                                                1u)
                                & __PAIR128__(0xFFFFFF00FFFFFF00LL, 0xFFFFFF00FFFFFF00LL)),
                      (int8x16_t)vshrq_n_u32(
                                   (uint32x4_t)vuzp2q_s32(
                                                 (int32x4_t)vmull_u32(*(uint32x2_t *)v43.i8, *(uint32x2_t *)v28.i8),
                                                 (int32x4_t)vmull_high_u32(v43, v28)),
                                   7u)),
                    vandq_s8(
                      (int8x16_t)vshlq_n_s32(
                                   vuzp2q_s32(
                                     (int32x4_t)vmull_u32(*(uint32x2_t *)v45.i8, *(uint32x2_t *)v28.i8),
                                     (int32x4_t)vmull_high_u32(v45, v28)),
                                   9u),
                      v32));
          v41 -= 4;
        }
        while ( v41 );
        if ( v37 == v38 )
          goto LABEL_23;
      }
      else
      {
        v39 = v8;
        v40 = v7;
      }
      do
      {
        v46 = v40->i32[0];
        v40 = (int8x16_t *)((char *)v40 + 4);
        v39->i32[0] = (2 * ((2155905153u * (unsigned __int64)(BYTE1(v46) * v3)) >> 32)) & 0x1FF00
                    | (BYTE2(v46) * v2 / 0xFF)
                    | ((2155905153u * (unsigned __int64)((unsigned __int8)v46 * v4)) >> 32 << 9) & 0x1FF0000;
        v39 = (int8x16_t *)((char *)v39 + 4);
        --v34;
      }
      while ( v34 );
LABEL_23:
      v6 = *(_DWORD *)(result + 36);
LABEL_24:
      v33 = v6;
      v13 += *(int *)(result + 16);
      v12 += *(int *)(result + 40);
      *(_DWORD *)(result + 36) = --v6;
      v8 = (int8x16_t *)v12;
      v7 = (int8x16_t *)v13;
      if ( !v33 )
        goto LABEL_45;
    }
  }
  v48 = 1 - v5;
  v12 = *(_QWORD *)(result + 24);
  v13 = *(_QWORD *)result;
  do
  {
    v13 += *(int *)(result + 16);
    v12 += *(int *)(result + 40);
    ++v48;
  }
  while ( v48 != 1 );
LABEL_44:
  *(_DWORD *)(result + 36) = -1;
LABEL_45:
  *(_QWORD *)result = v13;
  *(_QWORD *)(result + 24) = v12;
  return result;
}

/* ========================================================================
 * fallback sub_1003da258
 * EA: 0x1003da258
 ======================================================================== */

__int64 __fastcall sub_1003DA258(int *a1, _DWORD *a2, int *a3)
{
  int *v3; // x21
  _DWORD *v4; // x22
  int *v6; // x20
  int v7; // w11
  int v8; // w12
  int v9; // w13
  int v10; // w14
  int v11; // w15
  int v12; // w16
  int v13; // w0
  int v14; // w17
  int v15; // w2
  int v16; // w3
  int v17; // w4
  int v18; // w5
  int v19; // w6
  int v20; // w7
  int v21; // w23
  int v22; // w24
  int v23; // w25
  int v24; // w26
  int v25; // w27
  int v26; // w28
  int v27; // w9
  int v28; // w30
  int v29; // w16
  int v30; // w17
  int v31; // w1
  __int64 result; // x0
  int v33; // w9
  int v34; // w1
  int v35; // w3
  int v36; // w4
  int v37; // w5
  int v38; // w6
  int v39; // w7
  int v40; // [xsp+4h] [xbp-5Ch]
  int v41; // [xsp+8h] [xbp-58h]
  int v42; // [xsp+Ch] [xbp-54h]

  v3 = a3;
  v4 = a2;
  *a1 = *a2 + a2[10];
  a1[1] = a2[1] + a2[11];
  a1[2] = a2[2] + a2[12];
  a1[3] = a2[3] + a2[13];
  a1[4] = a2[4] + a2[14];
  a1[5] = a2[5] + a2[15];
  a1[6] = a2[6] + a2[16];
  a1[7] = a2[7] + a2[17];
  a1[8] = a2[8] + a2[18];
  a1[9] = a2[9] + a2[19];
  a1[10] = a2[10] - *a2;
  v6 = a1 + 10;
  a1[11] = a2[11] - a2[1];
  a1[12] = a2[12] - a2[2];
  a1[13] = a2[13] - a2[3];
  a1[14] = a2[14] - a2[4];
  a1[15] = a2[15] - a2[5];
  a1[16] = a2[16] - a2[6];
  a1[17] = a2[17] - a2[7];
  a1[18] = a2[18] - a2[8];
  a1[19] = a2[19] - a2[9];
  sub_1003D8E14(a1 + 20, a1, a3);
  sub_1003D8E14(v6, v6, v3 + 10);
  sub_1003D8E14(a1 + 30, v3 + 30, v4 + 30);
  sub_1003D8E14(a1, v4 + 20, v3 + 20);
  v41 = a1[1];
  v42 = *a1;
  v7 = a1[3];
  v40 = a1[2];
  v8 = a1[4];
  v9 = a1[5];
  v10 = a1[6];
  v11 = a1[7];
  v12 = *v6;
  v13 = a1[19];
  v14 = a1[20];
  v15 = a1[21];
  v16 = a1[22];
  v17 = a1[11];
  v18 = a1[12];
  *a1 = v14 - *v6;
  a1[1] = v15 - v17;
  v19 = a1[23];
  v20 = a1[24];
  LODWORD(v3) = a1[13];
  LODWORD(v4) = a1[14];
  a1[2] = v16 - v18;
  a1[3] = v19 - (_DWORD)v3;
  v21 = a1[25];
  v22 = a1[26];
  v23 = a1[15];
  v24 = a1[16];
  a1[4] = v20 - (_DWORD)v4;
  a1[5] = v21 - v23;
  v25 = a1[27];
  v26 = a1[28];
  v28 = a1[17];
  v27 = a1[18];
  a1[6] = v22 - v24;
  a1[7] = v25 - v28;
  *v6 = v12 + v14;
  v29 = a1[8];
  v30 = a1[9];
  v31 = a1[29];
  LODWORD(v6) = a1[30];
  a1[8] = v26 - v27;
  a1[9] = v31 - v13;
  a1[11] = v17 + v15;
  a1[12] = v18 + v16;
  a1[13] = (_DWORD)v3 + v19;
  a1[14] = (_DWORD)v4 + v20;
  v7 *= 2;
  v8 *= 2;
  v9 *= 2;
  v10 *= 2;
  v11 *= 2;
  v29 *= 2;
  a1[15] = v23 + v21;
  a1[16] = v24 + v22;
  a1[17] = v28 + v25;
  a1[18] = v27 + v26;
  a1[19] = v13 + v31;
  a1[20] = (_DWORD)v6 + 2 * v42;
  v33 = a1[31];
  result = (unsigned int)a1[32];
  a1[21] = v33 + 2 * v41;
  a1[22] = result + 2 * v40;
  v34 = a1[33];
  v35 = a1[34];
  a1[23] = v34 + v7;
  a1[24] = v35 + v8;
  v36 = a1[35];
  v37 = a1[36];
  a1[25] = v36 + v9;
  a1[26] = v37 + v10;
  v38 = a1[37];
  v39 = a1[38];
  a1[27] = v38 + v11;
  a1[28] = v39 + v29;
  v30 *= 2;
  LODWORD(v3) = a1[39];
  a1[29] = (_DWORD)v3 + v30;
  a1[30] = 2 * v42 - (_DWORD)v6;
  a1[31] = 2 * v41 - v33;
  a1[32] = 2 * v40 - result;
  a1[33] = v7 - v34;
  a1[34] = v8 - v35;
  a1[35] = v9 - v36;
  a1[36] = v10 - v37;
  a1[37] = v11 - v38;
  a1[38] = v29 - v39;
  a1[39] = v30 - (_DWORD)v3;
  return result;
}

/* Total decompiled blocks: 118 */
