/* GStreamer Scheduling tree
 * Copyright (C) 2015 Balázs Kreith (contact: balazs.kreith@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be ureful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/rtp/gstrtpbuffer.h>
#include <gst/rtp/gstrtcpbuffer.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "timestamp.h"


#define _now(this) (gst_clock_get_time (this->sysclock))

GST_DEBUG_CATEGORY_STATIC (timestamp_debug_category);
#define GST_CAT_DEFAULT timestamp_debug_category

G_DEFINE_TYPE (Timestamp, timestamp, G_TYPE_OBJECT);

#define REGULAR_REPORT_PERIOD_TIME (5*GST_SECOND)

//----------------------------------------------------------------------
//-------- Private functions belongs to Scheduler tree object ----------
//----------------------------------------------------------------------

static void
timestamp_finalize (GObject * object);


void
timestamp_class_init (TimestampClass * klass)
{
  GObjectClass *gobject_class;

  gobject_class = (GObjectClass *) klass;

  gobject_class->finalize = timestamp_finalize;

  GST_DEBUG_CATEGORY_INIT (timestamp_debug_category, "rndctrler", 0,
      "MpRTP Receiving Controller");

}


void
timestamp_finalize (GObject * object)
{
  Timestamp *this = TIMESTAMP (object);
  g_object_unref(this->sysclock);
}

void
timestamp_init (Timestamp * this)
{
  this->sysclock = gst_system_clock_obtain();
}

Timestamp *make_timestamp(guint32 clockrate)
{
  Timestamp *this = g_object_new(TIMESTAMP_TYPE, NULL);
  this->made = _now(this);
  this->offset = 0;
  this->clockrate = clockrate;
  return this;
}

void timestamp_set_clockrate(Timestamp* this, guint32 clockrate) {
  this->clockrate = clockrate;
}
guint32 timestmap_get(Timestamp* this) {
  GstClockTime elapsed_in_ns = _now(this) - this->made;
  GstClockTime elapsed_in_s = GST_TIME_AS_SECONDS(elapsed_in_ns);
  GstClockTime elapsed_in_clockrate;
  guint32 result;
  elapsed_in_ns -= elapsed_in_s * GST_SECOND;

  elapsed_in_clockrate = elapsed_in_s * this->clockrate;
  elapsed_in_clockrate += (gdouble)elapsed_in_ns / (gdouble)GST_SECOND * this->clockrate;
  if (4294967295 < elapsed_in_clockrate) {
    result = (elapsed_in_clockrate - 4294967296UL);
  } else {
    result = elapsed_in_clockrate;
  }
  return result;
}
