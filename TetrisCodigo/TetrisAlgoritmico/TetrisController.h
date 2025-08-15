#ifndef TETRISCONTROLLER_H_INCLUDED
#define TETRISCONTROLLER_H_INCLUDED

#include "TetrisModel.h"
#include "TetrisView.h"
#include "TetrisMusica.h"

class Controlador {
public:
    enum class Modo { UnJugador, DosJugadores };

    Controlador();
    ~Controlador();

    void procesarEvento(const sf::Event& evento);
    void actualizar(float dt);
    void dibujar();

    void iniciarJuego(Modo modo);
    void salirDelMenu();
    bool estaJugando() const;

    bool esFinJuego() const { return finJuego; }
    void reiniciarDuelo();

    int obtenerPuntos(int jugador) const;
    int obtenerNivel(int jugador) const;
    Modo obtenerModo() const { return modoActual; }

    bool estaEnPantallaFinal() const { return finJuego; }

    void reiniciarPartida();

    void volverAlMenu();

    void playMenuMusic();
    void playGameMusic();
    void stopMusic();
    void playSoundEffect(const std::string& id);
    void pauseMusic();

    Vista& obtenerVista();

private:
    Tablero* tablero;
    Vista* vista;
    ColaPiezas cola;

    int piezaForma[4][4];
    int piezaTam;
    int piezaX, piezaY;
    int piezaTipoActual;
    int siguienteTipo;
    int niveles[2];

    Modo modoActual;
    int puntuaciones[2];
    int jugadorActivo;
    bool modoSecuencial;

    bool mostrarMensaje;
    float mensajeTiempo;
    std::string mensajeTexto;

    float intervaloBase;
    Juego juego;
    float intervaloCaida;
    float acumulador;

    bool jugando;
    bool finJuego;

    bool puedeRotar;

    void llenarSacoSiNecesario();
    void nuevaPieza();
    void copiarFormaDesdeTipo(int tipo);
    void fijarYProcesar();
    void reiniciarJuegoDatos();

    const int TILE_SIZE = 24;
    const int SIDEBAR_WIDTH = 200;

    AudioManager audio;
};

#endif // TETRISCONTROLLER_H_INCLUDED
