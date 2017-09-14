/*
 * sink.h
 *
 *  Created on: 13 Sep 2017
 *      Author: balazskreith
 */

#ifndef TESTS_STATSRELAYER_BUFFER_H_
#define TESTS_STATSRELAYER_BUFFER_H_
#include <gst/gst.h>
#include "common.h"

typedef struct {
  GQueue* items;
  GQueue* recycle;
  guint item_size;
  PushPort* input;
  PushPort* output;
  Process* on_first_received;
  gboolean first_received;
  volatile gboolean collect;
}Buffer;

Buffer* make_buffer(guint item_size);
void buffer_dtor(Buffer* this);
void buffer_set_on_first_received(Buffer* this, Process* on_first_received);
void buffer_collect(Buffer* this);
void buffer_flush(Buffer* this);
void buffer_prepare(Buffer* this, guint num);

#endif /* TESTS_STATSRELAYER_BUFFER_H_ */