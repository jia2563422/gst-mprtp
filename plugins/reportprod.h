/*
 * refctrler.h
 *
 *  Created on: Jun 30, 2015
 *      Author: balazs
 */

#ifndef REPPRODER_H_
#define REPPRODER_H_

#include <gst/gst.h>
#include "gstmprtcpbuffer.h"
#include "streamjoiner.h"
#include "ricalcer.h"
#include "streamsplitter.h"

typedef struct _ReportProducer ReportProducer;
typedef struct _ReportProducerClass ReportProducerClass;

#define REPORTPRODUCER_TYPE             (report_producer_get_type())
#define REPORTPRODUCER(src)             (G_TYPE_CHECK_INSTANCE_CAST((src),REPORTPRODUCER_TYPE,ReportProducer))
#define REPORTPRODUCER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass),REPORTPRODUCER_TYPE,ReportProducerClass))
#define REPORTPRODUCER_IS_SOURCE(src)          (G_TYPE_CHECK_INSTANCE_TYPE((src),REPORTPRODUCER_TYPE))
#define REPORTPRODUCER_IS_SOURCE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass),REPORTPRODUCER_TYPE))
#define REPORTPRODUCER_CAST(src)        ((ReportProducer *)(src))


struct _ReportProducer
{
  GObject                  object;
  GstClockTime             made;
  GstClock*                sysclock;
  guint32                  ssrc;
  guint32                  sender_ssrc;
  gpointer                 databed;
  gchar                    logfile[255];
  GstMPRTCPSubflowReport*  report;
  GstMPRTCPSubflowBlock*   block;
  gpointer                 actual;
  gsize                    length;
  gboolean                 in_progress;

  Recycle*                 databeds;

  struct{
    GstRTCPXRBlock*          head_block;
    gpointer                 actual_block;
    gsize                    length;
    gpointer                 databed;
  }xr;
};

struct _ReportProducerClass{
  GObjectClass parent_class;
};

void report_producer_set_sender_ssrc(ReportProducer *this, guint32 ssrc);
void report_producer_set_logfile(ReportProducer *this, const gchar *logfile);

void report_producer_begin(ReportProducer *this, guint8 subflow_id);

void report_producer_add_rr(ReportProducer *this,
                            guint8 fraction_lost,
                            guint32 total_lost,
                            guint32 ext_hsn,
                            guint32 jitter,
                            guint32 LSR,
                            guint32 DLSR);

void report_producer_add_xr_lost_rle(ReportProducer *this,
                                          gboolean early_bit,
                                          guint8 thinning,
                                          guint16 begin_seq,
                                          guint16 end_seq,
                                          gboolean *vector);

void report_producer_add_xr_cc_rle_fb(ReportProducer *this,
                                          guint8 report_count,
                                          guint32 report_timestamp,
                                          guint16 begin_seq,
                                          guint16 end_seq,
                                          GstRTCPXRChunk* chunks,
                                          gint chunks_length);

void report_producer_add_xr_discarded_bytes(ReportProducer *this,
                                    guint8 interval_metric_flag,
                                    gboolean early_bit,
                                    guint32 payload_bytes_discarded);

void report_producer_add_xr_discarded_packets(ReportProducer *this,
                                    guint8 interval_metric_flag,
                                    gboolean early_bit,
                                    guint32 discarded_packets_num);

void report_producer_add_xr_owd(ReportProducer *this,
                                guint8 interval_metric_flag,
                                guint32 median_delay,
                                guint32 min_delay,
                                guint32 max_delay);

void report_producer_add_afb(ReportProducer *this,
                                guint32 media_source_ssrc,
                                guint32  fci_id,
                                gpointer fci_dat,
                                guint fci_dat_len);

void report_producer_add_afb_remb(ReportProducer *this,
                                  guint32 media_source_ssrc,
                                  guint32 num_ssrc,
                                  gfloat float_num,
                                  guint32 ssrc_feedback,
                                  guint16 hssn);

void report_producer_add_afb_reps(ReportProducer *this,
                                  guint32 media_source_ssrc,
                                  guint8 sampling_num,
                                  gfloat float_num);

void report_producer_add_sr(ReportProducer *this,
                                guint64 ntp_timestamp,
                                guint32 rtp_timestamp,
                                guint32 packet_count,
                                guint32 octet_count);

GstBuffer *report_producer_end(ReportProducer *this, guint *length);

GType report_producer_get_type (void);
#endif /* REPPRODER_H_ */
