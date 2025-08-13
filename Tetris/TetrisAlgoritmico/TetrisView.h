#ifndef TETRISVIEW_H_INCLUDED
#define TETRISVIEW_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "TetrisModel.h"

class Vista {
public:
    Vista(int anchoPix, int altoPix, const char* titulo);
    ~Vista();

    bool abierta();
    sf::RenderWindow& obtenerVentana();
    void limpiarPantalla();
    void presentar();

    void dibujarTablero(Fila* head, int filas, int cols);
    void dibujarPieza(int forma[4][4], int tam, int px, int py, int tipoPieza);
    void dibujarHUD(int puntuacion0, int puntuacion1, int jugadorActivo);
    void dibujarTextoCentral(const char* texto);
    void dibujarPanelLateral(int puntuacion0, int puntuacion1, int jugadorActivo, int nivel0, int nivel1, int siguienteTipo);


private:
    sf::RenderWindow ventana;
    sf::Font fuente;
    const int TILE_SIZE = 24;
    const int SIDEBAR_WIDTH = 200;
};

#endif // TETRISVIEW_H_INCLUDED
