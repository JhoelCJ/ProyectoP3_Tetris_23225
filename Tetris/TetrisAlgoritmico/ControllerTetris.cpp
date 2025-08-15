#include "TetrisController.h"
#include <SFML/Window/Event.hpp>
#include <ctime>
#include <cstdlib>
#include <iostream>

Controlador::Controlador(): intervaloBase(0.8f), juego(500, 0.8f){
    srand((unsigned)time(nullptr));
    tablero = new Tablero(FILAS, COLUMNAS);
    vista = new Vista(COLUMNAS * 24 + SIDEBAR_WIDTH, FILAS * 24, "Tetris");
    cola.limpiar();

    piezaTam = 4;
    acumulador = 0.0f;
    siguienteTipo = -1;
    niveles[0] = niveles[1] = 1;
    puntuaciones[0] = puntuaciones[1] = 0;
    jugadorActivo = -1;
    modoSecuencial = false;
    mostrarMensaje = false;
    mensajeTiempo = 0.f;
    mensajeTexto.clear();
    jugando = false;
    finJuego = false;
    puedeRotar = true;
}

Controlador::~Controlador() {
    delete tablero;
    delete vista;
}

Vista& Controlador::obtenerVista() { return *vista; }

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
    if (tipo < 0) {
        tipo = 0;
    }
    piezaTipoActual = tipo + 1;

    copiarFormaDesdeTipo(tipo);
    piezaX = (COLUMNAS/2) - 2;
    piezaY = -1;

    siguienteTipo = cola.verFrente();
}

void Controlador::copiarFormaDesdeTipo(int tipo) {
    for (int r=0;r<4;++r) for (int c=0;c<4;++c) piezaForma[r][c] = PIEZAS[tipo][r][c];
}

void Controlador::iniciarJuego(Modo modo) {
    modoActual = modo;
    tablero->limpiarTodo();
    cola.limpiar();
    puntuaciones[0] = puntuaciones[1] = 0;
    mostrarMensaje = false;
    mensajeTiempo = 0.f;
    mensajeTexto.clear();
    reiniciarJuegoDatos();
    jugando = true;
    finJuego = false;
    if (modoActual == Modo::DosJugadores) {
        modoSecuencial = true;
        jugadorActivo = 0;
        puntuaciones[0] = puntuaciones[1] = 0;
    } else {
        modoSecuencial = true;
        jugadorActivo = -1;
    }
    llenarSacoSiNecesario();
    nuevaPieza();
    juego = Juego(500, intervaloBase);
    intervaloCaida = intervaloBase * juego.obtenerFactorVelocidad();
    acumulador = 0.0f;
    jugando = true;
    finJuego = false;
}

void Controlador::salirDelMenu() {
    vista->obtenerVentana().close();
}

bool Controlador::estaJugando() const { return jugando && !finJuego; }

void Controlador::reiniciarJuegoDatos() {
    tablero->limpiarTodo();
    cola.limpiar();
    acumulador = 0.0f;
    intervaloCaida = intervaloBase;
    puntuaciones[0] = puntuaciones[1] = 0;
    jugadorActivo = -1;
    finJuego = false;
}

void Controlador::procesarEvento(const sf::Event& evento) {
    if (finJuego) {
        if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mp = vista->obtenerVentana().mapPixelToCoords(sf::Vector2i(evento.mouseButton.x, evento.mouseButton.y));
            sf::FloatRect rRe = vista->obtenerRectBotonReiniciar();
            sf::FloatRect rSalir = vista->obtenerRectBotonSalir();
            if (rRe.contains(mp)) {
                if (modoActual == Modo::DosJugadores) reiniciarDuelo();
                else reiniciarPartida();
                return;
            }
            if (rSalir.contains(mp)) {
                reiniciarJuegoDatos();
                jugando = false;
                finJuego = false;
                mostrarMensaje = false;
                return;
            }
        }
        return;
    }
    if (!estaJugando()) return;

    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mp = vista->obtenerVentana().mapPixelToCoords(sf::Vector2i(evento.mouseButton.x, evento.mouseButton.y));
        sf::FloatRect rMenu = vista->obtenerRectBotonMenu();
        if (rMenu.contains(mp)) {
            volverAlMenu();
            return;
        }
    }

    if (evento.type == sf::Event::KeyPressed) {

    if (evento.key.code == sf::Keyboard::Space) {
        return;
    }

        if (evento.key.code == sf::Keyboard::Left) {
            if (!tablero->colisiona(piezaForma, piezaTam, piezaX - 1, piezaY)) piezaX--;
        } else if (evento.key.code == sf::Keyboard::Right) {
            if (!tablero->colisiona(piezaForma, piezaTam, piezaX + 1, piezaY)) piezaX++;
        } else if (evento.key.code == sf::Keyboard::Down) {
            if (!tablero->colisiona(piezaForma, piezaTam, piezaX, piezaY + 1)) piezaY++;
            else fijarYProcesar();
        } else if (evento.key.code == sf::Keyboard::Up) {
            if (puedeRotar) {
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
                puedeRotar = false;
            }
        }
    }
    if (evento.type == sf::Event::KeyReleased) {
        if (evento.key.code == sf::Keyboard::Up) {
            puedeRotar = true;
        }
    }
}

void Controlador::actualizar(float dt) {
    if (!jugando) return;
    acumulador += dt;
    if (acumulador >= intervaloCaida) {
        acumulador = 0.f;
        if (!tablero->colisiona(piezaForma, piezaTam, piezaX, piezaY + 1)) piezaY++;
        else fijarYProcesar();
    }
    if (mostrarMensaje) {
        mensajeTiempo -= dt;
        if (mensajeTiempo <= 0.f) {
            mostrarMensaje = false;
            mensajeTexto.clear();
        }
    }
}

void Controlador::dibujar() {
    vista->limpiarPantalla();
    vista->dibujarTablero(tablero->obtenerHead(), FILAS, COLUMNAS);
    vista->dibujarPieza(piezaForma, piezaTam, piezaX, piezaY, piezaTipoActual);

    if (modoActual == Modo::DosJugadores) {
        vista->dibujarPanelLateral(puntuaciones[0], puntuaciones[1], jugadorActivo, niveles[0], niveles[1], siguienteTipo);
    } else {
        vista->dibujarPanelLateral(juego.obtenerPuntuacion(), 0, -1, juego.obtenerNivel(), 0, siguienteTipo);
    }

    if (mostrarMensaje) {
        vista->dibujarTextoCentral(mensajeTexto.c_str());
    }

    if (finJuego) {
        if (modoActual == Modo::DosJugadores) {
            std::string ganador;
            if (puntuaciones[0] > puntuaciones[1]) ganador = "GANA JUGADOR 1";
            else if (puntuaciones[1] > puntuaciones[0]) ganador = "GANA JUGADOR 2";
            else ganador = "EMPATE";
            std::string titulo = ganador;
            vista->dibujarPantallaFinal(titulo, puntuaciones[0], puntuaciones[1]);
        } else {
            std::string titulo = "GAME OVER";
            vista->dibujarPantallaFinal(titulo, juego.obtenerPuntuacion(), -1);
        }
    }

    vista->presentar();
}

void Controlador::fijarYProcesar() {
    tablero->fijarPieza(piezaForma, piezaTam, piezaX, piezaY, piezaTipoActual);
    int eliminadas = tablero->limpiarLineasCompletas();

    if (modoActual == Modo::DosJugadores && modoSecuencial) {
        int puntos = (eliminadas == 4) ? 500 : 100 * eliminadas;
        puntuaciones[jugadorActivo] += puntos;

        if (eliminadas > 0) {
            juego.puntosPorLinea(eliminadas);
        }
        niveles[jugadorActivo] = juego.obtenerNivel();
    }
    else {
        if (eliminadas > 0) {
            juego.puntosPorLinea(eliminadas);
            if (juego.nivelSubio()) intervaloCaida = intervaloBase * juego.obtenerFactorVelocidad();
        }
    }

    llenarSacoSiNecesario();
    nuevaPieza();

    if (tablero->colisiona(piezaForma, piezaTam, piezaX, piezaY)) {
        if (modoActual == Modo::DosJugadores && modoSecuencial) {
            if (jugadorActivo == 0) {
                jugadorActivo = 1;
                tablero->limpiarTodo();
                cola.limpiar();
                llenarSacoSiNecesario();
                nuevaPieza();
                juego = Juego(500, intervaloBase);
                niveles[1] = juego.obtenerNivel();
                mostrarMensaje = true;
                mensajeTiempo = 3.5f;
                mensajeTexto = "Fin Turno Jugador 1, Comienza Jugador 2";
            } else {
                finJuego = true;
                jugando = false;
                mostrarMensaje = false;
                mensajeTexto.clear();
            }
        } else {
            finJuego = true;
            jugando = false;
        }
    }
}

int Controlador::obtenerPuntos(int jugador) const {
    if (modoActual == Modo::DosJugadores) {
        if (jugador < 0 || jugador > 1) return 0;
        return puntuaciones[jugador];
    } else {
        return juego.obtenerPuntuacion();
    }
}

int Controlador::obtenerNivel(int jugador) const {
    if (modoActual == Modo::DosJugadores) {
        if (jugador < 0 || jugador > 1) return 1;
        return niveles[jugador];
    } else {
        return juego.obtenerNivel();
    }
}

void Controlador::volverAlMenu() {
    reiniciarJuegoDatos();
    jugando = false;
    finJuego = false;
    modoSecuencial = false;
    jugadorActivo = -1;
    mostrarMensaje = false;
    mensajeTexto.clear();
    cola.limpiar();
}

void Controlador::reiniciarPartida() {
    tablero->limpiarTodo();
    cola.limpiar();
    llenarSacoSiNecesario();
    puntuaciones[0] = puntuaciones[1] = 0;
    niveles[0] = niveles[1] = 1;
    juego = Juego(500, intervaloBase);
    intervaloCaida = intervaloBase * juego.obtenerFactorVelocidad();
    acumulador = 0.f;
    if (modoActual == Modo::DosJugadores) {
        jugadorActivo = 0;
        modoSecuencial = true;
    } else {
        jugadorActivo = -1;
        modoSecuencial = false;
    }

    finJuego = false;
    jugando = true;
    mostrarMensaje = false;
    mensajeTexto.clear();
    llenarSacoSiNecesario();
    nuevaPieza();
}

void Controlador::reiniciarDuelo() {
    tablero->limpiarTodo();
    cola.limpiar();
    llenarSacoSiNecesario();

    puntuaciones[0] = puntuaciones[1] = 0;
    niveles[0] = niveles[1] = 1;
    jugadorActivo = 0;
    modoActual = Modo::DosJugadores;
    modoSecuencial = true;

    juego = Juego(500, intervaloBase);
    intervaloCaida = intervaloBase * juego.obtenerFactorVelocidad();
    acumulador = 0.f;
    finJuego = false;
    mostrarMensaje = false;
    mensajeTexto.clear();
    jugando = true;

    nuevaPieza();
}
