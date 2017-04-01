#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include <gst/app/app.h>
#include "qmutex.h"
#define VGA_WIDTH 640
#define VGA_HEIGHT 480
//#define uint8_t  unsigned char

extern "C"{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
}


// Command line parameters
static long framecounts = 0;

// Buffer for incoming data (FullHD is max, you can increase it as you need)
static unsigned char *rawdata;

//extern unsigned char *yuvbuf;

// Flags to indicate to parent thread that GstreamerThread started and finished
static volatile bool bGstreamerThreadStarted = false;
static volatile bool bGstreamerThreadFinished = false;

static GstElement *appsrc;
static GstElement *appsink;
static GstElement *pipeline,*qtmux,*filesink,*filter;
static int framecount = 0;

bool isPause;
QMutex gstmutex;


GstFlowReturn play_preroll(GstAppSink *appsink, gpointer data) {
    g_print ("Got preroll!\n");
    return GST_FLOW_OK;
}


GstFlowReturn play_sample(GstAppSink *appsink, gpointer data) {
    //static

    GstSample *sample = gst_app_sink_pull_sample(appsink);
    GstCaps *caps = gst_sample_get_caps(sample);
    GstBuffer *buffer = gst_sample_get_buffer(sample);
    const GstStructure *info = gst_sample_get_info(sample);

    // ---- Read frame and convert to opencv format ---------------



    GstMapInfo map;
    gst_buffer_map (buffer, &map, GST_MAP_READ);

    memcpy(rawdata,map.data,VGA_WIDTH * VGA_HEIGHT * 3 / 2);
    g_print ("Got frame:%d\n",framecount++);
    gst_buffer_unmap(buffer, &map);

    gstmutex.unlock();

    gst_sample_unref (sample);

    return GST_FLOW_OK;
}


static gboolean my_bus_callback (GstBus *bus, GstMessage *message, gpointer data) {
    g_print ("Got %s message\n", GST_MESSAGE_TYPE_NAME (message));
    GMainLoop *loop = (GMainLoop*)data;
    switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR: {
        GError *err;
        gchar *debug;

        gst_message_parse_error (message, &err, &debug);
        g_print ("Error: %s\n", err->message);
        g_error_free (err);
        g_free (debug);
        break;
    }
    case GST_MESSAGE_EOS:
        /* end-of-stream */
        g_print("End of stream\n");
        g_main_loop_quit(loop);
        break;
    default:
        /* unhandled message */
        break;
    }
    /* we want to be notified again the next time there is a message
   * on the bus, so returning TRUE (FALSE means we want to stop watching
   * for messages on the bus and our callback should not be called again)
   */
    return TRUE;
}

// Creates and sets up Gstreamer pipeline for JPEG encoding.
void *GstreamerPlayThread(void *pThreadParam) {

    GError *error = NULL;

    gchar *descr = g_strdup_printf(
           "filesrc location=%s ! \
            matroskademux name=demux demux.audio_0 ! \
            audioconvert ! \
            queue ! \
            audioresample ! \
            autoaudiosink name=audiosink0 demux.video_0 ! \
            h264parse ! \
            v4l2video1videodec ! \
            videoconvert ! \
            video/x-raw,format=YUY2 ! \
            videoscale ! \
            video/x-raw,width=640,height=480 ! \
            videoconvert ! \
            video/x-raw,format=I420 ! \
            appsink name=appsink0 sync=false",(char *)pThreadParam);

            pipeline = gst_parse_launch (descr, &error);

            if (error != NULL) {
            g_print ("could not construct pipeline: %s\n", error->message);
            bGstreamerThreadStarted = bGstreamerThreadFinished = true;
            g_error_free (error);
            return (void *)0xDEAD;
}

    appsink = gst_bin_get_by_name (GST_BIN (pipeline), "appsink0");

    gst_app_sink_set_emit_signals((GstAppSink*)appsink, true);
    gst_app_sink_set_drop((GstAppSink*)appsink, true);
    gst_app_sink_set_max_buffers((GstAppSink*)appsink, 1);
    GstAppSinkCallbacks callbacks = { NULL, play_preroll, play_sample };
    gst_app_sink_set_callbacks (GST_APP_SINK(appsink), &callbacks, NULL, NULL);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    GstBus *bus;
    guint bus_watch_id;
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, my_bus_callback, loop);
    gst_object_unref (bus);

    isPause = false;

    // Create gstreamer loop
    fprintf(stderr, "Setting g_main_loop_run to GST_STATE_PLAYING\n");

    // Start pipeline so it could process incoming data
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Loop will run until receiving EOS (end-of-stream), will block here
    g_main_loop_run(loop);

    fprintf(stderr, "g_main_loop_run returned, stopping playback\n");

    // Stop pipeline to be released
    gst_element_set_state(pipeline, GST_STATE_NULL);

    fprintf(stderr, "Deleting pipeline\n");
    // THis will also delete all pipeline elements
    gst_object_unref(GST_OBJECT(pipeline));

    return NULL;
}

// Starts GstreamerThread that remains in memory and compresses frames as being  fed by user app.
bool StartPlayGstreamer(void *pParam) {
    // GstreamerThread(NULL);
    // return true;
    unsigned long GtkThreadId;
    pthread_attr_t GtkAttr;

    rawdata = (unsigned char *)malloc(sizeof(unsigned char) * VGA_WIDTH *  VGA_HEIGHT* 3 / 2);

    memset(rawdata,0,sizeof(unsigned char) * VGA_HEIGHT * VGA_WIDTH * 3 / 2);

    // Start thread
    int result = pthread_attr_init(&GtkAttr);
    if (result != 0) {
        fprintf(stderr, "pthread_attr_init returned error %d\n", result);
        return false;
    }

    //void *pParam = NULL;
    result = pthread_create(&GtkThreadId, &GtkAttr, GstreamerPlayThread, pParam);
    if (result != 0) {
        fprintf(stderr, "pthread_create returned error %d\n", result);
        return false;
    }

    return true;
}


bool StopGstreamer() {

    gst_element_send_event(pipeline,gst_event_new_eos());

}


bool IsPauseStatus()
{
    isPause  =  (isPause ? false : true);

    gst_element_set_state(pipeline, isPause ? GST_STATE_PAUSED:GST_STATE_PLAYING);

    return isPause;
}
