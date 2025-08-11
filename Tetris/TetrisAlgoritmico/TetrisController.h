#ifndef TETRISCONTROLLER_H_INCLUDED
#define TETRISCONTROLLER_H_INCLUDED

#include "TetrisModel.h"
#include "TetrisView.h"

class Controlador {
public:
    Controlador();
    ~Controlador();
    void ejecutar();

private:
    Tablero* tablero;
    Vista* vista;
    ColaPiezas cola;
    int piezaForma[4][4];
    int piezaTam;
    int piezaX, piezaY;

    Juego juego;
    float intervaloBase;
    float intervaloCaida;

    bool finJuego;
    void nuevaPieza();
    void fijarYProcesar();
    void copiarFormaDesdeTipo(int tipo);
    void llenarSacoSiNecesario();
};

#endif // TETRISCONTROLLER_H_INCLUDED
