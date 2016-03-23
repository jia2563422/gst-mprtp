/*
 * fecdecoder.h
 *
 *  Created on: Jun 30, 2015
 *      Author: balazs
 */

#ifndef FECDECODER_H_
#define FECDECODER_H_

#include <gst/gst.h>
#include "gstmprtpbuffer.h"
#include "rtpfecbuffer.h"

typedef struct _FECDecoder FECDecoder;
typedef struct _FECDecoderClass FECDecoderClass;

#define FECDECODER_TYPE             (fecdecoder_get_type())
#define FECDECODER(src)             (G_TYPE_CHECK_INSTANCE_CAST((src),FECDECODER_TYPE,FECDecoder))
#define FECDECODER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass),FECDECODER_TYPE,FECDecoderClass))
#define FECDECODER_IS_SOURCE(src)          (G_TYPE_CHECK_INSTANCE_TYPE((src),FECDECODER_TYPE))
#define FECDECODER_IS_SOURCE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass),FECDECODER_TYPE))
#define FECDECODER_CAST(src)        ((FECDecoder *)(src))



typedef struct _FECDecoderSegment
{
  GstRTPFECSegment     base;

  GstClockTime         added;
  GstBuffer*           fec;
  guint16              base_sn;
  guint16              high_sn;
  guint16              protected;
  gint32               arrived[GST_RTPFEC_MAX_PROTECTION_NUM];
  gint32               arrived_length;
  gint32               missing[GST_RTPFEC_MAX_PROTECTION_NUM];
  gint32               missing_length;
  gint32               missing_num;
  gboolean             complete;
  GList*               items;
}FECDecoderSegment;

typedef struct _FECDecoderItem
{
  GstClockTime         added;
  guint8               bitstring[GST_RTPFEC_PARITY_BYTES_MAX_LENGTH];
  gint16               bitstring_length;
  guint16              seq_num;
  guint32              ssrc;
}FECDecoderItem;


typedef struct _FECDecoderRequest
{
  GstClockTime         added;
  guint16              seq_num;
}FECDecoderRequest;


struct _FECDecoder
{
  GObject                    object;
  GstClock*                  sysclock;
  GstClockTime               repair_window_min;
  GstClockTime               repair_window_max;
  GstClockTime               made;
  GRWLock                    rwmutex;

  guint8                     payload_type;

  GList*                     segments;
  GList*                     items;
  GList*                     requests;
};



struct _FECDecoderClass{
  GObjectClass parent_class;

};



GType fecdecoder_get_type (void);
FECDecoder *make_fecdecoder(void);
void fecdecoder_request_repair(FECDecoder *this, guint16 seq);
void fecdecoder_reset(FECDecoder *this);
gboolean fecdecoder_has_repaired_rtpbuffer(FECDecoder *this, GstBuffer** repairedbuf);
void fecdecoder_set_payload_type(FECDecoder *this, guint8 fec_payload_type);
void fecdecoder_add_rtp_packet(FECDecoder *this, GstMpRTPBuffer* buffer);
void fecdecoder_add_fec_packet(FECDecoder *this, GstMpRTPBuffer *mprtp);
void fecdecoder_clean(FECDecoder *this);
#endif /* FECDECODER_H_ */