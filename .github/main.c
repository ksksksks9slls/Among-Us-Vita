#include <vitasdk.h>
#include <stdio.h>
#include <stdlib.h>

// Definimos los permisos que necesita el port para saltarse las restricciones de la Vita
// Esto permite que el cargador pueda leer archivos de la carpeta ux0:data/
int _newlib_heap_size_user = 192 * 1024 * 1024; // Reservamos 192MB de RAM para el juego

int main(int argc, char *argv[]) {
    // 1. Inicializamos los módulos del sistema de la PlayStation Vita
    sceSysmoduleLoadModule(SCE_SYSMODULE_NET); // Módulo de red (por si el .so lo pide)
    
    // 2. Apuntamos los gráficos a un espacio vacío (Esto genera la pantalla en negro)
    // En lugar de dibujar imágenes, le decimos a la pantalla que muestre el vacío de la memoria de video
    void *base_addr = sceDisplayGetVmtAddr(); 
    
    // 3. El bucle infinito (Mantiene la pantalla encendida y la Vita despierta)
    while (1) {
        // En los siguientes tutoriales, aquí meteremos el código que lee el 'libmain.so'
        // Por ahora, dejamos este bucle vacío para lograr la pantalla en negro estable.
        
        // Sincroniza el juego con la pantalla de la Vita a 60 cuadros por segundo (60Hz)
        sceDisplayWaitVblankStart(); 
    }

    return 0;
}