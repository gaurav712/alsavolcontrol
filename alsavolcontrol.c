#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

#define MODE_SET    0
#define MODE_INC    1
#define MODE_DEC    2

short mode = 0;

void set_volume(int vol) {

    long min_volume, max_volume, current_vol;
    snd_mixer_t *mixer_handle;
    snd_mixer_selem_id_t *selem_id;
    snd_mixer_elem_t *elem;
    const char *card = "default";
    const char *selem_name = "Master";

    snd_mixer_open(&mixer_handle, 0);
    snd_mixer_attach(mixer_handle, card);
    snd_mixer_selem_register(mixer_handle, NULL, NULL);
    snd_mixer_load(mixer_handle);
    snd_mixer_selem_id_alloca(&selem_id);
    snd_mixer_selem_id_set_index(selem_id, 0);
    snd_mixer_selem_id_set_name(selem_id, selem_name);

    elem = snd_mixer_find_selem(mixer_handle, selem_id);

    snd_mixer_selem_get_playback_volume_range(elem, &min_volume, &max_volume);

    if(mode == MODE_INC) {
        snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &current_vol);
        vol = current_vol + ((float)vol / 100 * max_volume);
    } else if(mode == MODE_DEC) {
        snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &current_vol); /* Yeah, I get it! I called it twice for efficiency */
        vol = current_vol - ((float)vol / 100 * max_volume);
    } else {
        vol = ((float)vol / 100 * max_volume);
    }

    if(vol < min_volume || vol > max_volume) {
        fprintf(stderr, "invalid value\n");
        exit(1);
    }

    snd_mixer_selem_set_playback_volume_all(elem, vol);

    snd_mixer_close(mixer_handle);
}

void show_help(char *program_name) {
    fprintf(stderr, "USAGE:\n\t%s [inc/dec/set] [volume to set in %%, not including the sign]\n", program_name);
    exit(1);
}

int main(int argc, char *argv[]) {

    if(argc != 3) {
        show_help(argv[0]);
        exit(1);
    }


    if(!(strcmp(argv[1], "inc"))) {
        mode = MODE_INC;
    } else if(!(strcmp(argv[1], "dec"))) {
        mode = MODE_DEC;
    }

    set_volume(atoi(argv[2]));

    return 0;
}
