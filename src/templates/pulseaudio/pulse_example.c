#include <pulse/pulseaudio.h>

int main() {
  pa_threaded_mainloop *m = pa_threaded_mainloop_new();
  pa_mainloop_api *m_api = pa_threaded_mainloop_get_api(m);

  pa_threaded_mainloop_stop(m);
  pa_threaded_mainloop_free(m);
  return 0;
}