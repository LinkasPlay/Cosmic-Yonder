#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <stdbool.h>

#include "CosmicYonder.h"

// COMMANDE TERMINAL : gcc -o ProgMain *.c -lncurses -lm -lpthread -lpulse-simple -lpulse

volatile bool stop_music = false;

void* play_music(void* arg) {
    int num_musique = *(int*)arg;
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
            return NULL;
    }

    while (!stop_music) {
        FILE *fp = fopen(filename, "rb");
        if (!fp) {
            perror("Erreur ouverture fichier");
            return NULL;
        }

        char buffer[44];
        fread(buffer, 1, 44, fp); // Lire l'en-tête du fichier WAV

        // Configuration de PulseAudio
        static const pa_sample_spec ss = {
            .format = PA_SAMPLE_S16LE,
            .rate = 44100,
            .channels = 2
        };

        int error;
        pa_simple *s = pa_simple_new(NULL, "Music Player", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error);
        if (!s) {
            fprintf(stderr, "pa_simple_new() failed: %s\n", pa_strerror(error));
            fclose(fp);
            return NULL;
        }

        int16_t data[1024];
        size_t bytes;

        while ((bytes = fread(data, sizeof(int16_t), 1024, fp)) > 0 && !stop_music) {
            if (pa_simple_write(s, data, bytes * sizeof(int16_t), &error) < 0) {
                fprintf(stderr, "pa_simple_write() failed: %s\n", pa_strerror(error));
                pa_simple_free(s);
                fclose(fp);
                return NULL;
            }
        }

        if (pa_simple_drain(s, &error) < 0) {
            fprintf(stderr, "pa_simple_drain() failed: %s\n", pa_strerror(error));
        }

        pa_simple_free(s);
        fclose(fp);
    }

    return NULL;
}

//test