#include "operaciones.h"
#include <SDL2/SDL.h>

void cargarJuego(chip8 *chip8,char *filename);
void dibujar_pantalla(chip8 *chip, SDL_Renderer *renderer);
void emular_ciclo(chip8 *chip);
int procesar_teclado(chip8 *chip, SDL_Event *evento);

int main(int argc, char *argv[]){
    
    chip8 Michip8;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event evento;
    srand(time(NULL));
    int ciclos_por_frame = 10,corriendo;

    if(argc != 2) {
        printf("error muchos argumentos o faltante\n");
        return 1;
    }

    memset(&Michip8,0,sizeof(Michip8));
    Michip8.pc=0x200;

    cargarJuego(&Michip8, argv[1]);

    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Crear ventana (64x32 pixels, escalado 10x = 640x320)
    window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 320,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Error creando ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Crear renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    if (!renderer) {
        printf("Error creando renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    while (1)
    {
       //printf("se esta ejecutando el juego\n");
        while (SDL_PollEvent(&evento)) 
        {
            if (evento.type == SDL_QUIT) {
                corriendo = 0;
            }

            // 2. Manejar el teclado usando tu nueva función
            // Si procesar_teclado devuelve false (ESC), detenemos el bucle
            if (!procesar_teclado(&Michip8, &evento)) 
            {
                corriendo = 1;
            }
        }

       for (int i = 0; i < ciclos_por_frame; i++)
            emular_ciclo(&Michip8);

        if (Michip8.delay_timer > 0) 
            Michip8.delay_timer--;
        
        if (Michip8.sound_timer > 0) {
            Michip8.sound_timer--;
        }

       dibujar_pantalla(&Michip8, renderer); // Ahora pasas el puntero

        usleep(16666);
    }

    // Limpieza (aunque nunca llegues aquí con while(1))
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

// Devuelve false si el usuario presiona ESC para salir, true en caso contrario
int procesar_teclado(chip8 *chip, SDL_Event *evento)
{
    // Solo nos interesan los eventos de presionar (DOWN) o soltar (UP)
    if (evento->type != SDL_KEYDOWN && evento->type != SDL_KEYUP) {
        return 1; 
    }

    // Determinamos si se presionó (1) o se soltó (0)
    uint8_t estado = (evento->type == SDL_KEYDOWN) ? 1 : 0;
    
    // Mapeo switch: Tecla Física -> Índice del Array Keypad
    switch (evento->key.keysym.sym) 
    {
        case SDLK_1: chip->keypad[0x1] = estado; break;
        case SDLK_2: chip->keypad[0x2] = estado; break;
        case SDLK_3: chip->keypad[0x3] = estado; break;
        case SDLK_4: chip->keypad[0xC] = estado; break;

        case SDLK_q: chip->keypad[0x4] = estado; break;
        case SDLK_w: chip->keypad[0x5] = estado; break;
        case SDLK_e: chip->keypad[0x6] = estado; break;
        case SDLK_r: chip->keypad[0xD] = estado; break;

        case SDLK_a: chip->keypad[0x7] = estado; break;
        case SDLK_s: chip->keypad[0x8] = estado; break;
        case SDLK_d: chip->keypad[0x9] = estado; break;
        case SDLK_f: chip->keypad[0xE] = estado; break;

        case SDLK_z: chip->keypad[0xA] = estado; break;
        case SDLK_x: chip->keypad[0x0] = estado; break;
        case SDLK_c: chip->keypad[0xB] = estado; break;
        case SDLK_v: chip->keypad[0xF] = estado; break;

        case SDLK_ESCAPE: 
            return 0; // El usuario quiere salir
    }

    return 1; // Todo normal, seguimos corriendo
}

// Supongamos que ESCALA = 10 (cada pixel de Chip-8 son 10x10 reales)

void dibujar_pantalla(chip8 *chip, SDL_Renderer *renderer)
{
    // 1. Limpiar pantalla (Pintar todo de negro)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Negro
    SDL_RenderClear(renderer);

    // 2. Configurar color de dibujo a Blanco
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanco

    // 3. Recorrer tu VRAM
    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            // Si tu emulador dice que aquí hay un pixel prendido...
            if (chip->display[(y * 64) + x] == 1)
            {
                // ...creamos un cuadrado grande para que se vea
                SDL_Rect rect;
                rect.x = x * 10; // Posición X magnificada
                rect.y = y * 10; // Posición Y magnificada
                rect.w = 10;     // Ancho del "pixel"
                rect.h = 10;     // Alto del "pixel"

                // Le decimos a la tarjeta gráfica que dibuje ese cuadrado
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // 4. "Presentar" (Mostrar lo que dibujamos en la ventana real)
    SDL_RenderPresent(renderer);
}

void cargarJuego(chip8 *chip8,char *filename)
{
    FILE *arch=fopen(filename,"rb");
    long tamaño;
    if(arch != NULL)
    {
        fseek(arch,0,SEEK_END);
        tamaño=ftell(arch);
        fseek(arch,0,SEEK_SET);

        if(tamaño<MAX_MEMORIA-0x200)
            fread(&chip8->memoria[0x200],1,tamaño,arch);
        else
        {
            printf("el juego es demasiado pesado para esta consola solo se pertmiten archivos de %d bytes",MAX_MEMORIA-0x200);
            exit(1);
        }
    }
    else
    {
        printf("Error al abrir el archivo %s",filename);
        exit(1);
    }
    printf("se leyeron %ld bytes\n",ftell(arch));
}

void emular_ciclo(chip8 *chip)
{

    uint16_t opcode = (chip->memoria[chip->pc] << 8) | chip->memoria[chip->pc + 1];

    chip->pc += 2;

    uint16_t NNN = opcode & 0x0FFF;
    uint8_t NN   = opcode & 0x00FF;
    uint8_t N    = opcode & 0x000F;
    uint8_t X    = (opcode & 0x0F00) >> 8;
    uint8_t Y    = (opcode & 0x00F0) >> 4;

    // Switch basado en el primer Nibble (4 bits) del opcode
    switch (opcode & 0xF000)
    {
        case 0x0000:
            if (opcode == 0x00E0) ooEo(chip);      // CLS
            else if (opcode == 0x00EE) ooEE(chip); // RET
            break;

        case 0x1000: i1NNN(chip, NNN); break; // JP addr
        case 0x2000: i2NNN(chip, NNN); break; // CALL addr
        case 0x3000: i3XNN(chip, X, NN); break; // SE Vx, byte
        case 0x4000: i4XNN(chip, X, NN); break; // SNE Vx, byte
        case 0x5000: i5XY0(chip, X, Y); break;  // SE Vx, Vy
        case 0x6000: i6XNN(chip, X, NN); break; // LD Vx, byte
        case 0x7000: i7XNN(chip, X, NN); break; // ADD Vx, byte

        case 0x8000: // Operaciones Lógicas y Aritméticas
            switch (N) // Miramos el último bit para distinguir
            {
                case 0x0: i8XY0(chip, X, Y); break;
                case 0x1: i8XY1(chip, X, Y); break;
                case 0x2: i8XY2(chip, X, Y); break;
                case 0x3: i8XY3(chip, X, Y); break;
                case 0x4: i8XY4(chip, X, Y); break;
                case 0x5: i8XY5(chip, X, Y); break;
                case 0x6: i8XY6(chip, X);    break;
                case 0x7: i8XY7(chip, X, Y); break;
                case 0xE: i8XYE(chip, X);    break;
            }
            break;

        case 0x9000: i9XY0(chip, X, Y); break; // SNE Vx, Vy
        case 0xA000: iANNN(chip, NNN); break;  // LD I, addr
        case 0xB000: iBNNN(chip, NNN); break;  // JP V0, addr
        case 0xC000: iCXNN(chip, X, NN); break; // RND Vx, byte
        case 0xD000: iDXYN(chip, X, Y, N); break; // DRW Vx, Vy, nibble

        case 0xE000:
            if (NN == 0x9E) iEX9E(chip, X);     // SKP Vx
            else if (NN == 0xA1) iEXA1(chip, X); // SKNP Vx
            break;

        case 0xF000:
            switch (NN)
            {
                case 0x07: iFX07(chip, X); break;
                case 0x0A: iFX0A(chip, X); break; // Wait Key
                case 0x15: iFX15(chip, X); break;
                case 0x18: iFX18(chip, X); break;
                case 0x1E: iFX1E(chip, X); break;
                case 0x29: iFX29(chip, X); break;
                case 0x33: iFX33(chip, X); break;
                case 0x55: iFX55(chip, X); break;
                case 0x65: iFX65(chip, X); break;
            }
            break;

        default:
            printf("Opcode desconocido: 0x%X\n", opcode);
    }
}