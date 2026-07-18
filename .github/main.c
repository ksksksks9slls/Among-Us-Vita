#include <vitasdk.h>
#include <vita2d.h>
#include <stdio.h>
#include <stdlib.h>

// Reservamos 192MB de memoria RAM para el juego
int _newlib_heap_size_user = 192 * 1024 * 1024;

int main(int argc, char *argv[]) {
    // Inicializar la librería gráfica obligatoria de la Vita
    vita2d_init();
    
    // Configurar el fondo de la pantalla a color negro absoluto
    vita2d_set_clear_color(RGBA8(0, 0, 0, 255));

    // Cargar los archivos de fuentes de texto que el sistema operativo de la Vita trae por defecto
    vita2d_pgf *fuente_sistema = vita2d_load_default_pgf();

    // Bucle infinito: mantiene la consola corriendo el juego a 60 FPS estables
    while (1) {
        // Empezar a renderizar el frame actual
        vita2d_start_drawing();
        vita2d_clear_screen();

        // Pintar el texto en la mitad de la pantalla (resolución de la Vita: 960x544)
        vita2d_pgf_draw_text(
            fuente_sistema, 
            300, 270, 
            RGBA8(255, 255, 255, 255), // Letras en blanco brillante
            1.5f,                      // Tamaño del texto
            "Hola Mundo! Wrapper de Among Us"
        );

        // Finalizar el dibujado y enviar la señal física a la pantalla
        vita2d_end_drawing();
        vita2d_swap_buffers();
        
        // Evita que el procesador trabaje de más sincronizando los ciclos de la pantalla
        sceDisplayWaitVblankStart();
    }

    // Código de cierre (por si en algún momento se sale del bucle infinito)
    vita2d_fini();
    vita2d_free_pgf(fuente_sistema);
    
    sceKernelExitProcess(0);
    return 0;
}