#include "operaciones.h"


//Llamar a subrutina en la direccion NNN
void oNNN(chip8 *chip8, uint16_t NNN)
{
    //no se implementa ya que es igual a 2NNN
}
//limpiar la pantalla
void ooEo(chip8 *chip8)
{
     memset(chip8->display, 0, sizeof(chip8->display));
}
 //retornar de subrutina
void ooEE(chip8 *chip8)
{
    if (chip8->sp>=0)
    {
        chip8->pc=chip8->stack[chip8->sp];
        chip8->sp--;
    }
    else
    {
        printf("stack underflow\n");
        exit(1);
    }
    
}
//saltar a la direccion NNN
void i1NNN(chip8 *chip8, uint16_t NNN)
{
    chip8->pc=NNN;
}
//llamar a subrutina en la direccion NNN
void i2NNN(chip8 *chip8, uint16_t NNN)
{
     if(chip8->sp<=15){
        chip8->sp++;
        chip8->stack[chip8->sp]=chip8->pc;
        chip8->pc=NNN;
    }
    else
    {
        printf("stack overflow\n");
        exit(1);
    }
}
//saltar la siguiente instruccion si el registro VX == NN
void i3XNN(chip8 *chip8, uint8_t X, uint8_t NN)
{
    if(chip8->V[X] == NN)
    {    
        chip8->pc+=2;
        if(chip8->pc>=MAX_MEMORIA)
        {
            printf("memory overflow");
            exit(1);
        }
    }
}
//saltar la siguiente instruccion si el registro VX != NN
void i4XNN(chip8 *chip8, uint8_t X, uint8_t NN)
{
    if(chip8->V[X] != NN)
    {
        chip8->pc+=2;
        if(chip8->pc>=MAX_MEMORIA)
        {
            printf("memory overflow");
            exit(1);
        }
    }
}
//saltar la siguiente instruccion si el registro VX == VY
void i5XY0(chip8 *chip8, uint8_t X, uint8_t Y)
{
    if(chip8->V[X] == chip8->V[Y])
    {
        chip8->pc+=2;
          if(chip8->pc>=MAX_MEMORIA)
        {
            printf("memory overflow");
            exit(1);
        }
    }
}
//cargar el valor NN en el registro VX
void i6XNN(chip8 *chip8, uint8_t X, uint8_t NN)
{
    if(X<MAX_V)
        chip8->V[X]=NN;
    else
        printf("registro inexistente");
}
//sumar el valor NN al registro VX
void i7XNN(chip8 *chip8, uint8_t X, uint8_t NN)
{
    if(X<MAX_V)
        chip8->V[X]+=NN;
    else
        printf("registro inexistente");
}
 //asigna  VX = VY
void i8XY0(chip8 *chip8, uint8_t X, uint8_t Y)
{
    if (X<MAX_V && Y<MAX_V)
        chip8->V[X]=chip8->V[Y];
    else
        printf("registros inexistentes");
}
 //asigna VX = VX OR VY
void i8XY1(chip8 *chip8, uint8_t X, uint8_t Y)
{
    if (X<MAX_V && Y<MAX_V)
        chip8->V[X]=chip8->V[X] | chip8->V[Y];
    else
        printf("registros inexistentes");
}
//asigna VX = VX AND VY 
void i8XY2(chip8 *chip8, uint8_t X, uint8_t Y)
{
    if (X<MAX_V && Y<MAX_V)
        chip8->V[X]=chip8->V[X] & chip8->V[Y];
    else
        printf("registros inexistentes");
}
//asigna VX = VX XOR VY
void i8XY3(chip8 *chip8, uint8_t X, uint8_t Y)
{
   if (X<MAX_V && Y<MAX_V)
        chip8->V[X]=chip8->V[X] ^ chip8->V[Y];
    else
        printf("registros inexistentes");
}
//sumar VY a VX. VF es 1 si hay acarreo o 0 si no| VX=VX+VY
void i8XY4(chip8 *chip8, uint8_t X, uint8_t Y)
{
    if (X<MAX_V && Y<MAX_V)
    {
        uint16_t suma= chip8->V[X] + chip8->V[Y];
        chip8->V[0xF]=(suma > 0xFF) ? 1 : 0;
        chip8->V[X] = (uint8_t)(suma & 0x00FF);
    }
    else
        printf("registros inexistentes");
}
//restar VY a VX. VF es 0 si hay prestamo o 1 si no | VX=VX-VY
void i8XY5(chip8 *chip8, uint8_t X, uint8_t Y)
{
    if (X<MAX_V && Y<MAX_V)
    {
        chip8->V[0xF] = (chip8->V[X] >= chip8->V[Y]) ? 1 : 0;
        chip8->V[X] = chip8->V[X] - chip8->V[Y];
    }
    else
        printf("registros inexistentes");
}
//guardar el valor del bit menos significativo de vx VF=VX & 0X01 y luego desplazar VX a la derecha 1 bit | VX=VX>>1
void i8XY6(chip8 *chip8, uint8_t X)
{
    if (X<MAX_V)
    {
        chip8->V[0xF]=chip8->V[X] & 0x01;
        chip8->V[X]=chip8->V[X]>>1;
    }
    else
        printf("registro inexistente");
}
//restar VX a VY. VF es 0 si hay prestamo o 1 si no | VX=VY-VX
void i8XY7(chip8 *chip8, uint8_t X, uint8_t Y)
{
    if (X<MAX_V && Y<MAX_V)
    {
        chip8->V[0xF] = (chip8->V[Y] >= chip8->V[X]) ? 1 : 0;
        chip8->V[Y] = chip8->V[Y] - chip8->V[X];
    }
    else
        printf("registros inexistentes");
}
//guarda el valor del bit mas significativo de VX en VF y luego desplaza un bit a la izquierda
void i8XYE(chip8 *chip8, uint8_t X)
{
    if (X<MAX_V)
    {
        chip8->V[0xF] = (chip8->V[X] & 0x80)>>7; 
        chip8->V[X] = chip8->V[X]<<1;
    }
    else
        printf("registros inexistentes");
}
//saltar la siguiente instruccion si el registro VX != VY 
void i9XY0(chip8 *chip8, uint8_t X, uint8_t Y)
{
    if(X<MAX_V && Y<MAX_V)
    {
        if(chip8->V[X] != chip8->V[Y])
        {
            chip8->pc+=2;
            if(chip8->pc>MAX_MEMORIA)
            {
                printf("overflow memoria");
                exit(1);
            }
        }

    }
    else
        printf("registros inexistentes");
}
//cargar la direccion NNN en el registro I
void iANNN(chip8 *chip8, uint16_t NNN)
{
    if(NNN>MAX_MEMORIA)
        chip8->I=NNN;
    else
        printf("direccion de memoria invalido");
} 
//saltar a la direccion NNN + V0
void iBNNN(chip8 *chip8, uint16_t NNN)
{
    chip8->pc=chip8->V[0x0]+NNN;
    if(chip8->pc > MAX_MEMORIA)
    {
        printf("overflow memoria");
        exit(1);
    }
   
}
//generar un numero aleatorio AND con NN y guardarlo en VX
void iCXNN(chip8 *chip8, uint8_t X, uint8_t NN)
{
    if(X<MAX_V)
    {
        uint8_t aleatorio=rand() % 256;
        chip8->V[X]=aleatorio & NN;
    }
}
 //dibujar un sprite en la pantalla
void iDXYN(chip8 *chip8, uint8_t X, uint8_t Y, uint8_t N)
{
    if(X >= MAX_V || Y >= MAX_V) {
        printf("registro inexistente");
        return;
    }
    
    uint8_t x_pos = chip8->V[X] % 64;  // Posición X con wrapping
    uint8_t y_pos = chip8->V[Y] % 32;  // Posición Y con wrapping
    
    chip8->V[0xF] = 0;  // Reset collision flag
    
    // N es la altura del sprite (número de filas)
    for(uint8_t fila = 0; fila < N; fila++) {
        uint8_t sprite_byte = chip8->memoria[chip8->I + fila];
        
        // Verificar si no excede el límite vertical
        if((y_pos + fila) >= 32) break;
        
        // Cada byte representa 8 píxeles horizontales
        for(uint8_t col = 0; col < 8; col++) {
            // Verificar si no excede el límite horizontal
            if((x_pos + col) >= 64) break;
            
            // Obtener el bit del sprite (de izquierda a derecha)
            uint8_t sprite_pixel = (sprite_byte >> (7 - col)) & 0x01;
            
            if(sprite_pixel) {
                // Calcular posición en el display
                uint16_t pos = (y_pos + fila) * 64 + (x_pos + col);
                
                // XOR con el píxel actual
                if(chip8->display[pos] == 1) {
                    chip8->V[0xF] = 1;  // Colisión detectada
                }
                chip8->display[pos] ^= 1;
            }
        }
    }
}
//saltar la siguiente instruccion si la tecla guardada en VX esta presionada
void iEX9E(chip8 *chip8, uint8_t X)
{
    if(X<MAX_V)
    {
        uint8_t tecla=chip8->V[X];
        if(chip8->keypad[tecla] == 1)
        {
            chip8->pc+=2;
            if(chip8->pc > MAX_MEMORIA)
            {
                printf("overflow memoria");
                exit(1);
            }
        }
    }
}

//saltar la siguiente instruccion si la tecla guardada en VX no esta presionada
void iEXA1(chip8 *chip8, uint8_t X)
{
    if(X<MAX_V)
    {
        uint8_t tecla=chip8->V[X];
        if(chip8->keypad[tecla] != 1)
        {
            chip8->pc+=2;
            if(chip8->pc > MAX_MEMORIA)
            {
                printf("overflow memoria");
                exit(1);
            }
        }
    }
}
//cargar el valor del delay timer en VX
void iFX07(chip8 *chip8, uint8_t X)
{
    if(X < MAX_V)
        chip8->V[X] = chip8->delay_timer;
    else
        printf("registro inexistente");
}
//esperar a que se presione una tecla y guardarla en VX
void iFX0A(chip8 *chip8, uint8_t X)
{
    if(X<MAX_V)
    {
        int8_t tecla_presionada=-1;
        for(int i=0;i<16;i++)
            if(chip8->keypad[i] == 1)
            {
                tecla_presionada=i;
                break;
            }
        
        if(tecla_presionada == -1)
            chip8->V[X]=(uint8_t)tecla_presionada;
        else
            chip8->pc-=2; //para que se vuelva a llamar a esta funcion en busqueda de una tecla presionada
    }
    else
        printf("registro inexistente");
}
//cargar el valor de VX en el delay timer
void iFX15(chip8 *chip8, uint8_t X)
{
    if(X < MAX_V)
        chip8->delay_timer = chip8->V[X];
    else
        printf("registro inexistente");
}
//cargar el valor de VX en el sound timer
void iFX18(chip8 *chip8, uint8_t X)
{
    if(X < MAX_V)
        chip8->sound_timer = chip8->V[X];
    else
        printf("registro inexistente");
}
 //sumar VX al registro I
void iFX1E(chip8 *chip8, uint8_t X)
{
    if(X<MAX_V)
        chip8->I+=chip8->V[X];
    else
        printf("registros inexistentes");
}
//cargar en I la direccion del sprite correspondiente al valor de VX
void iFX29(chip8 *chip8, uint8_t X)
{
    if(X < MAX_V)
    {
        uint8_t digito = chip8->V[X] & 0x0F;  // Solo los dígitos 0-F son válidos
        chip8->I = digito * 5;  // Cada sprite de fuente ocupa 5 bytes
    }
    else
        printf("registro inexistente");
}
//almacenar el valor de VX en formato BCD en la memoria en las posiciones I, I+1, I+2
void iFX33(chip8 *chip8, uint8_t X)
{
    if(X<MAX_V)
    {
        chip8->memoria[chip8->I+0]=(uint8_t)(chip8->V[X] / 100);
        chip8->memoria[chip8->I+1]=(uint8_t)((chip8->V[X] / 10) %10);
        chip8->memoria[chip8->I+0]=(uint8_t)(chip8->V[X] % 10);
    }
    else
        printf("registro inexistente");
}
//almacenar los registros V0 a VX en la memoria
void iFX55(chip8 *chip8, uint8_t X)
{
    if(X<MAX_V)
    {
        for(int i=0;i<=X;i++)
            chip8->memoria[chip8->I+i]=chip8->V[i];
    }
    else
        printf("registro inexistente");
}
//cargar los registros V0 a VX desde la memoria en las posiciones I a I+X
void iFX65(chip8 *chip8, uint8_t X)
{
    if(X<MAX_V)
    {
        for(int i=0;i<=X;i++)
            chip8->V[i]=chip8->memoria[chip8->I+i];
    }
    else
         printf("registro inexistente");
}
