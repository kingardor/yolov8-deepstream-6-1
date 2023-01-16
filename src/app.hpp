#pragma once
#include "libs.hpp"

namespace YOLOv8 {
  class Odin {
    private:
      struct fps_calculator {
        system_clock::time_point fps_timer;
        system_clock::time_point display_timer;
        gint rolling_fps;
        gint display_fps;
      };

      inline static fps_calculator fps[16];

      inline static char *PGIE_YOLO_DETECTOR_CONFIG_FILE_PATH;

      inline static char *TRACKER_CONFIG_FILE;

    public:
      // To save the frames
      gint frame_number;

      gboolean display_off;

      GOptionEntry entries[2] = {
        {"no-display", 0, 0, G_OPTION_ARG_NONE, &display_off, "Disable display", NULL},
        {NULL}
      };

      std::string PGIE_YOLO_ENGINE_PATH;

      static void
      update_fps (gint id);

      static int
      create_input_sources (gpointer pipe, gpointer mux, guint num_sources);

      static void
      changeBBoxColor (gpointer obj_meta_data, int has_bg_color, float red, float green,
                      float blue, float alpha);

      static void
      addDisplayMeta (gpointer batch_meta_data, gpointer frame_meta_data);

      static GstPadProbeReturn
      tiler_src_pad_buffer_probe (GstPad * pad, GstPadProbeInfo * info,
          gpointer u_data);

      static gboolean
      bus_call (GstBus * bus, GstMessage * msg, gpointer data);

      static void
      cb_newpad (GstElement * decodebin, GstPad * decoder_src_pad, gpointer data);

      static void
      decodebin_child_added (GstChildProxy *child_proxy, GObject *object,
                            gchar *name, gpointer user_data);

      static GstElement *
      create_source_bin (guint index, gchar * uri);

      static gchar *
      get_absolute_file_path (gchar *cfg_file_path, gchar *file_path);

      static gboolean
      set_tracker_properties (GstElement *nvtracker);

      int
      configure_element_properties(int num_sources, GstElement *streammux, GstElement *pgie_yolo_detector,
                           GstElement *nvtracker, GstElement *sink, GstElement *tiler);

      void
      setPaths(guint num_sources);

      Odin() {

        int counter;
        for (counter = 0; counter < 16; counter++) {
          fps[counter].fps_timer = system_clock::now();
          fps[counter].display_timer = system_clock::now();
          fps[counter].rolling_fps = 0;
          fps[counter].display_fps = 0;
        }

        display_off = false;
        frame_number = 0;
      }

      ~Odin() {}
  };
}