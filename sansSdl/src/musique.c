#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

/*
int musique1(int num_musique) {
    char *filename;

    switch (num_musique) {
        case 0:
            filename = "musique/menu.wav";
            break;
        case 1:
            filename = "musique/vaisseau.wav";
            break;
        case 2:
            filename = "musique/boss.wav";
            break;
        default:
            fprintf(stderr, "Numéro de musique invalide\n");
            return EXIT_FAILURE;
    }

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Erreur ouverture fichier");
        return EXIT_FAILURE;
    }

    char buffer[44];
    fread(buffer, 1, 44, fp); // Lire l'en-tête du fichier WAV

    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int rate = 44100; // Taux d'échantillonnage
    int dir;
    snd_pcm_uframes_t frames;
    int rc;
    char *data;
    int size;
    
    // Ouvrir le périphérique de lecture
    rc = snd_pcm_open(&handle, "hw:0,0", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr, "Erreur ouverture pcm : %s\n", snd_strerror(rc));
        return 1;
    }

    // Allouer des paramètres matériels
    snd_pcm_hw_params_alloca(&params);

    // Remplir les paramètres par défaut
    snd_pcm_hw_params_any(handle, params);

    // Définir le type d'accès
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    // Définir le format
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

    // Définir le taux d'échantillonnage
    snd_pcm_hw_params_set_rate_near(handle, params, &rate, &dir);

    // Définir le nombre de canaux
    snd_pcm_hw_params_set_channels(handle, params, 2);

    // Appliquer les paramètres
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        fprintf(stderr, "Erreur parametre hw: %s\n", snd_strerror(rc));
        return 1;
    }

    // Utiliser la période pour déterminer la taille des frames
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);

    size = frames * 4; // 2 canaux, 2 octets par échantillon
    data = (char *) malloc(size);

    while (fread(data, 1, size, fp) > 0) {
        rc = snd_pcm_writei(handle, data, frames);
        if (rc == -EPIPE) {
            // Débordement du buffer
            fprintf(stderr, "underrun occurred\n");
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
        } else if (rc != (int)frames) {
            fprintf(stderr, "short write, write %d frames\n", rc);
        }
    }

    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(data);
    fclose(fp);

    return EXIT_SUCCESS;
}

*/
