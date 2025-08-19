#ifndef TETRISMODEL_H_INCLUDED
#define TETRISMODEL_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>

const int FILAS = 20;
const int COLUMNAS = 10;

struct Celda {
    int tipo;
    Celda* siguiente;
    Celda() : tipo(0), siguiente(nullptr) {}
};

struct Fila {
    Celda* primera;
    Fila* siguiente;
    Fila(int cols) : siguiente(nullptr) {
        primera = new Celda();
        Celda* curr = primera;
        for(int i=1; i<cols; ++i) {
            curr->siguiente = new Celda();
            curr = curr->siguiente;
        }
    }
};

struct NodoPieza {
    int tipo;
    NodoPieza* siguiente;
    NodoPieza(int t) : tipo(t), siguiente(nullptr) {}
};

struct ColaPiezas {
    NodoPieza* frente;
    NodoPieza* fin;
    ColaPiezas() : frente(nullptr), fin(nullptr) {}
    void encolar(int tipo);
    int desencolar();
    bool vacia();
    void limpiar();
    int verFrente();
};

void rotar90(int M[][4], int inicio, int fin);

extern int PIEZAS[7][4][4];

class Juego {
public:
    Juego(int puntosPorNivel = 500, float intervaloBase = 0.8f);
    void puntosPorLinea(int lineas);
    int obtenerPuntuacion() const;
    int obtenerNivel() const;
    float obtenerFactorVelocidad() const;
    bool nivelSubio() const;
private:
    int puntuacion;
    int nivel;
    int puntosPorNivel;
    bool cambioNivel;
    float intervaloBase;
};

class Tablero {
public:
    Tablero(int filas = FILAS, int cols = COLUMNAS);
    ~Tablero();
    void limpiarTodo();
    bool obtenerValor(int r, int c);
    void establecerValor(int r, int c, int tipo);

    bool colisiona(int shape[4][4], int tam, int px, int py);
    void fijarPieza(int shape[4][4], int tam, int px, int py, int tipoPieza);
    int limpiarLineasCompletas(int removedRows[], int maxRemoved);
    int contarFilas();
    Fila* obtenerHead();

private:
    Fila* head;
    int filas;
    int cols;

    Fila* crearFilas(int filas, int cols);
    Fila* obtenerFila(int r);
    Celda* obtenerCelda(int r, int c);
    bool filaLlena(Fila* f);
    void eliminarUnaFila(Fila*& headRef, Fila* anterior);
    void escanearYEliminar(Fila*& headRef, int colsLocal, int filasTotales);
};

#endif // TETRISMODEL_H_INCLUDED
