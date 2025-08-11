#include "TetrisController.h"
#include <SFML/Window/Event.hpp>
#include <ctime>
#include <cstdlib>

Controlador::Controlador(): intervaloBase(0.8f), juego(500, 0.8f){
    srand((unsigned)time(nullptr));
    tablero = new Tablero(FILAS, COLUMNAS);
    vista = new Vista(COLUMNAS * 24, FILAS * 24, "Tetris - MVC");
    cola.limpiar();

    llenarSacoSiNecesario();

    piezaTam = 4;
    nuevaPieza();

    intervaloCaida = intervaloBase * juego.obtenerFactorVelocidad();
    finJuego = false;
}

Controlador::~Controlador() {
    delete tablero;
    delete vista;
}

void Controlador::llenarSacoSiNecesario() {
    if (!cola.vacia()) return;
    int arr[7];
    for (int i=0;i<7;++i) arr[i]=i;
    for (int i=6;i>0;--i) {
        int j = rand() % (i+1);
        int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
    }
    for (int i=0;i<7;++i) cola.encolar(arr[i]);
}

void Controlador::nuevaPieza() {
    llenarSacoSiNecesario();
    int tipo = cola.desencolar();
    copiarFormaDesdeTipo(tipo);
    piezaX = (COLUMNAS/2) - 2;
    piezaY = -1;
}

void Controlador::copiarFormaDesdeTipo(int tipo) {
    for (int r=0;r<4;++r) for (int c=0;c<4;++c) piezaForma[r][c] = PIEZAS[tipo][r][c];
}

void Controlador::ejecutar() {
    sf::Clock reloj;
    float acumulador = 0.0f;

    while (vista->abierta()) {
        sf::Event evento;
        while (vista->obtenerVentana().pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) vista->obtenerVentana().close();
            if (evento.type == sf::Event::KeyPressed && !finJuego) {
                if (evento.key.code == sf::Keyboard::Left) {
                    if (!tablero->colisiona(piezaForma, piezaTam, piezaX - 1, piezaY)) piezaX--;
                } else if (evento.key.code == sf::Keyboard::Right) {
                    if (!tablero->colisiona(piezaForma, piezaTam, piezaX + 1, piezaY)) piezaX++;
                } else if (evento.key.code == sf::Keyboard::Down) {
                    if (!tablero->colisiona(piezaForma, piezaTam, piezaX, piezaY + 1)) piezaY++;
                    else fijarYProcesar();
                } else if (evento.key.code == sf::Keyboard::Up) {
                    int copia[4][4];
                    for (int r=0;r<4;++r) for (int c=0;c<4;++c) copia[r][c] = piezaForma[r][c];
                    rotar90(piezaForma, 0, piezaTam-1);
                    if (tablero->colisiona(piezaForma, piezaTam, piezaX, piezaY)) {
                        bool kicked = false;
                        for (int dx=-2; dx<=2 && !kicked; ++dx) {
                            if (!tablero->colisiona(piezaForma, piezaTam, piezaX + dx, piezaY)) {
                                piezaX += dx; kicked = true; break;
                            }
                        }
                        if (!kicked) for (int r=0;r<4;++r) for (int c=0;c<4;++c) piezaForma[r][c] = copia[r][c];
                    }
                } else if (evento.key.code == sf::Keyboard::Space) {
                    while (!tablero->colisiona(piezaForma, piezaTam, piezaX, piezaY + 1)) piezaY++;
                    fijarYProcesar();
                }
            }
        }

        float dt = reloj.restart().asSeconds();
        acumulador += dt;
        if (!finJuego && acumulador >= intervaloCaida) {
            acumulador = 0.0f;
            if (!tablero->colisiona(piezaForma, piezaTam, piezaX, piezaY + 1)) piezaY++;
            else fijarYProcesar();
        }

        vista->limpiarPantalla();
        vista->dibujarTablero(tablero->obtenerHead(), FILAS, COLUMNAS);
        vista->dibujarPieza(piezaForma, piezaTam, piezaX, piezaY);
        vista->dibujarHUD(juego.obtenerPuntuacion(), juego.obtenerNivel());
        if (finJuego) vista->dibujarTextoCentral("\t GAME OVER");
        vista->presentar();
    }
}

void Controlador::fijarYProcesar() {
    tablero->fijarPieza(piezaForma, piezaTam, piezaX, piezaY);
    int eliminadas = tablero->limpiarLineasCompletas();
    if (eliminadas > 0) {
        juego.puntosPorLinea(eliminadas);
        if (juego.nivelSubio()) {
            intervaloCaida = intervaloBase * juego.obtenerFactorVelocidad();
        }
    }
    nuevaPieza();
    if (tablero->colisiona(piezaForma, piezaTam, piezaX, piezaY)) finJuego = true;
}
