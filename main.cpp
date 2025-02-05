#include <alsa/asoundlib.h>
#include <iostream>

int main() {
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    int rc;

    // Open PCM device for playback
    rc = snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        std::cerr << "Unable to open PCM device: " << snd_strerror(rc) << std::endl;
        return 1;
    }

    // Allocate a hardware parameters object
    snd_pcm_hw_params_malloc(&params);

    // Fill it in with default values
    snd_pcm_hw_params_any(pcm_handle, params);

    // Set the desired hardware parameters
    // Interleaved mode
    snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    // Signed 16-bit little-endian format
    snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    // Two channels (stereo)
    snd_pcm_hw_params_set_channels(pcm_handle, params, 2);
    // 44100 bits/second sampling rate (CD quality)
    unsigned int val = 44100;
    snd_pcm_hw_params_set_rate_near(pcm_handle, params, &val, 0);

    // Write the parameters to the driver
    rc = snd_pcm_hw_params(pcm_handle, params);
    if (rc < 0) {
        std::cerr << "Unable to set HW parameters: " << snd_strerror(rc) << std::endl;
        return 1;
    }

    std::cout << "PCM name: " << snd_pcm_name(pcm_handle) << std::endl;

    // Clean up
    snd_pcm_hw_params_free(params);
    snd_pcm_close(pcm_handle);

    return 0;
}