#include "operaciones.h"
void cargarJuego();

int main(){
    
    chip8 Michip8;
    srand(time(NULL));
    int ciclos_por_frame = 10;

    memset(&Michip8,0,sizeof(Michip8));

    Michip8.pc=0x200;

    cargarJuego();//a implementar



    while (1)
    {
       for (int i = 0; i < ciclos_por_frame; i++)
            emular_ciclo(&Michip8);

        // C. Actualizar Timers (Solo una vez por frame, a 60Hz)
        if (Michip8.delay_timer > 0) 
            Michip8.delay_timer--;
        
        if (Michip8.sound_timer > 0) {
            // reproducir_beep();
            Michip8.sound_timer--;
        }

        // D. Dibujar
        // Si usamos iDXYN, el array 'display' ya tiene los pixels.
        // dibujar_pantalla(mi_chip.display);

        // E. Control de velocidad (Delay para mantener 60 FPS)
        // Esto es rudimentario. Lo ideal es contar delta-time.
        usleep(16666); // ~16ms = 60
            }
    
}


#include "operaciones.h"

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