#include <vitasdk.h>
#include <vita2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 192 MB de heap reservados (base para el futuro cargador de Among Us)
int _newlib_heap_size_user = 192 * 1024 * 1024;

// Resolucion nativa de la pantalla de PS Vita
#define SCREEN_W 960
#define SCREEN_H 544

// Colores reutilizables
#define COLOR_BLANCO RGBA8(255, 255, 255, 255)
#define COLOR_VERDE  RGBA8(  0, 255,   0, 255)
#define COLOR_GRIS   RGBA8(160, 160, 160, 255)

int main(int argc, char *argv[]) {
    // --- Entrada del mando ---
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

    // --- Inicializacion grafica ---
    vita2d_init();
    vita2d_set_clear_color(RGBA8(0, 0, 0, 255)); // negro absoluto

    // Carga de fuente con verificacion de fallo
    vita2d_pgf *pgf = vita2d_load_default_pgf();
    if (!pgf) {
        // Sin fuente no hay nada que dibujar: cerramos limpio
        vita2d_fini();
        sceKernelExitProcess(1);
        return 1;
    }

    const char *titulo   = "Hola Mundo! Wrapper de Among Us";
    const char *subtitulo = "Base VitaSDK + vita2d  |  START para salir";

    // Centrado real segun el ancho medido de cada texto
    int titulo_w = vita2d_pgf_text_width(pgf, 1.2f, titulo);
    int sub_w    = vita2d_pgf_text_width(pgf, 0.8f, subtitulo);
    int titulo_x = (SCREEN_W - titulo_w) / 2;
    int sub_x    = (SCREEN_W - sub_w) / 2;

    // --- Estado para el contador de FPS ---
    SceCtrlData pad;
    uint32_t frames = 0;
    uint32_t fps = 0;
    uint64_t tick_prev = sceKernelGetProcessTimeWide(); // microsegundos
    char fps_buf[32];

    while (1) {
        // Salida controlada con START (deja el cleanup alcanzable)
        sceCtrlPeekBufferPositive(0, &pad, 1);
        if (pad.buttons & SCE_CTRL_START) break;

        // --- Calculo de FPS cada segundo ---
        frames++;
        uint64_t tick_now = sceKernelGetProcessTimeWide();
        if (tick_now - tick_prev >= 1000000ULL) { // 1 segundo
            fps = frames;
            frames = 0;
            tick_prev = tick_now;
        }
        snprintf(fps_buf, sizeof(fps_buf), "FPS: %u", fps);

        // --- Render ---
        vita2d_start_drawing();
        vita2d_clear_screen();

        // Titulo centrado
        vita2d_pgf_draw_text(pgf, titulo_x, SCREEN_H / 2 - 20,
                             COLOR_BLANCO, 1.2f, titulo);

        // Subtitulo centrado, mas abajo y mas pequeno
        vita2d_pgf_draw_text(pgf, sub_x, SCREEN_H / 2 + 30,
                             COLOR_GRIS, 0.8f, subtitulo);

        // Contador de FPS en la esquina superior izquierda
        vita2d_pgf_draw_text(pgf, 20, 40, COLOR_VERDE, 1.0f, fps_buf);

        vita2d_end_drawing();
        vita2d_swap_buffers();
        sceDisplayWaitVblankStart(); // sincroniza a 60 Hz
    }

    // --- Liberacion de recursos ---
    vita2d_free_pgf(pgf);
    vita2d_fini();

    sceKernelExitProcess(0);
    return 0;
}
