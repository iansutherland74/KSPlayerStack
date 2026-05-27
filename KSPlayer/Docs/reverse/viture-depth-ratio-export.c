/* Viture P10 — DepthRatioCalculator focused export */
/* Binary: /Users/sutherland/Downloads/com.viture.p10app-1.9.25-Decrypted/ExternalMonitor */

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
 * sub_1001A47F8
 * EA: 0x1001a47f8
 ======================================================================== */

id sub_1001A47F8()
{
  id result; // x0

  result = objc_msgSend(objc_allocWithZone((Class)&OBJC_CLASS___NSLock), "init");
  qword_10066FA70 = (__int64)result;
  return result;
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
