#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAX_MEMORIA 4096
#define MAX_V 16
typedef struct  chip8
{
    uint8_t memoria[4096];
    uint8_t V[16]; // Registros V0 a VF
    uint16_t I; //registro de indice a la memoria
    uint16_t pc; //program counter
    uint16_t stack[16]; //pila de retorno
    int8_t sp; //stack pointer
    int8_t  delay_timer; 
    uint8_t  sound_timer;
    uint32_t display[64 * 32]; // Píxeles (on/off)
    uint8_t  keypad[16];
}chip8;

//funciones 
void oNNN(chip8 *chip8, uint16_t NNN);//Llamar a subrutina en la direccion NNN
void ooEo(chip8 *chip8); //limpiar pantalla
void ooEE(chip8 *chip8); //retornar de subrutina
void i1NNN(chip8 *chip8, uint16_t NNN); //saltar a la direccion NNN
void i2NNN(chip8 *chip8, uint16_t NNN); //llamar a subrutina en la direccion NNN
void i3XNN(chip8 *chip8, uint8_t X, uint8_t NN); //saltar la siguiente instruccion si el registro VX == NN
void i4XNN(chip8 *chip8, uint8_t X, uint8_t NN); //saltar la siguiente instruccion si el registro VX != NN
void i5XY0(chip8 *chip8, uint8_t X, uint8_t Y); //saltar la siguiente instruccion si el registro VX == VY
void i6XNN(chip8 *chip8, uint8_t X, uint8_t NN); //cargar el valor NN en el registro VX
void i7XNN(chip8 *chip8, uint8_t X, uint8_t NN); //sumar el valor NN al registro VX
void i8XY0(chip8 *chip8, uint8_t X, uint8_t Y); //asigna  VX = VY
void i8XY1(chip8 *chip8, uint8_t X, uint8_t Y); //asigna VX = VX OR VY
void i8XY2(chip8 *chip8, uint8_t X, uint8_t Y); //asigna VX = VX AND VY 
void i8XY3(chip8 *chip8, uint8_t X, uint8_t Y); //asigna VX = VX XOR VY
void i8XY4(chip8 *chip8, uint8_t X, uint8_t Y); //sumar VY a VX. VF es 1 si hay acarreo o 0 si no| VX=VX+VY 
void i8XY5(chip8 *chip8, uint8_t X, uint8_t Y); //restar VY a VX. VF es 0 si hay prestamo o 1 si no | VX=VX-VY
void i8XY6(chip8 *chip8, uint8_t X); //guardar el valor del bit menos significativo de vx VF=VX & 0X01 y luego desplazar VX a la derecha 1 bit | VX=VX>>1
void i8XY7(chip8 *chip8, uint8_t X, uint8_t Y); //restar VX a VY. VF es 0 si hay prestamo o 1 si no | VX=VY-VX
void i8XYE(chip8 *chip8, uint8_t X); //guardar el valor de VF=VX y luego desplazar VX a la izquierda 1 bit | VX=VX<<1
void i9XY0(chip8 *chip8, uint8_t X, uint8_t Y); //saltar la siguiente instruccion si el registro VX != VY   
void iANNN(chip8 *chip8, uint16_t NNN); //cargar la direccion NNN en el registro I
void iBNNN(chip8 *chip8, uint16_t NNN); //saltar a la direccion NNN + V0
void iCXNN(chip8 *chip8, uint8_t X, uint8_t NN); //generar un numero aleatorio AND con NN y guardarlo en VX
void iDXYN(chip8 *chip8, uint8_t X, uint8_t Y, uint8_t N); //dibujar un sprite en la pantalla
void iEX9E(chip8 *chip8, uint8_t X); //saltar la siguiente instruccion si la tecla guardada en VX esta presionada
void iEXA1(chip8 *chip8, uint8_t X); //saltar la siguiente instruccion si la tecla guardada en VX no esta presionada
void iFX07(chip8 *chip8, uint8_t X); //cargar el valor del delay timer en VX
void iFX0A(chip8 *chip8, uint8_t X); //esperar a que se presione una tecla y guardarla en VX
void iFX15(chip8 *chip8, uint8_t X); //cargar el valor de VX en el delay timer
void iFX18(chip8 *chip8, uint8_t X); //cargar el valor de VX en el sound timer
void iFX1E(chip8 *chip8, uint8_t X); //sumar VX al registro I
void iFX29(chip8 *chip8, uint8_t X); //cargar en I la direccion del sprite correspondiente al valor de VX
void iFX33(chip8 *chip8, uint8_t X); //almacenar el valor de VX en formato BCD en la memoria en las posiciones I, I+1, I+2
void iFX55(chip8 *chip8, uint8_t X); //almacenar los registros V0 a VX en la   
void iFX65(chip8 *chip8, uint8_t X); //cargar los registros V0 a VX desde la memoria en las posiciones I a I+X
