#include <vitasdk.h>
#include <vita2d.h> // Incluimos la librería gráfica para pintar en pantalla
#include <stdio.h>

// Reservamos 192MB de memoria RAM para el juego (suficiente para los archivos de Android)
int _newlib_heap_size_user = 192 * 1024 * 1024;

int main(int argc, char *argv[]) {
    // 1. Inicializamos la librería gráfica de la Vita
    vita2d_init();
    
    // Configuramos el color de fondo en negro absoluto (Rojo=0, Verde=0, Azul=0, Alfa=255)
    vita2d_set_clear_color(RGBA8(0, 0, 0, 255));

    // Cargamos la fuente de texto por defecto que trae el sistema de la PS Vita
    vita2d_pgf *fuente_sistema = vita2d_load_default_pgf();

    // 2. El bucle principal del juego
    while (1) {
        // Iniciamos el dibujo del cuadro actual en la pantalla
        vita2d_start_drawing();
        vita2d_clear_screen(); // Limpia la pantalla dejándola en negro absoluto

        // PINTAMOS EL TEXTO EN LA PANTALLA
        // Parámetros: (fuente, posición X, posición Y, color en RGBA, tamaño, "Texto")
        // Ponemos X=400 e Y=270 para que quede centrado en la pantalla de la Vita (960x544)
        vita2d_pgf_draw_text(
            fuente_sistema, 
            350, 270, 
            RGBA8(255, 255, 255, 255), // Texto en color blanco brillante
            1.5f,                      // Tamaño de la letra (escala)
            "Hola Mundo! Wrapper de Among Us"
        );

        // Terminamos de dibujar y mandamos el cuadro a la pantalla física de la Vita
        vita2d_end_drawing();
        vita2d_swap_buffers(); // Intercambia los buffers de video para evitar parpadeos

        // Sincroniza a 60 cuadros por segundo para que la consola no se sobrecaliente
        sceDisplayWaitVblankStart();
    }

    // 3. Limpieza de memoria (por buena práctica de programación)
    vita2d_fini();
    vita2d_free_pgf(fuente_sistema);
    
    sceKernelExitProcess(0);
    return 0;
}