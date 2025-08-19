#include "TetrisController.h"
#include "TetrisMenu.h"
#include <SFML/System/Clock.hpp>

int main() {
    Controlador controlador;
    Vista& vista = controlador.obtenerVista();
    Menu menu(vista, controlador);

    sf::Clock reloj;
    while (vista.abierta()) {
        sf::Event evento;

        while (vista.obtenerVentana().pollEvent(evento)) {
            if (!controlador.estaJugando() && !controlador.estaEnPantallaFinal()) {
                menu.procesarEvento(evento);
            }
            controlador.procesarEvento(evento);

            if (evento.type == sf::Event::Closed) {
                vista.obtenerVentana().close();
            }
        }

        float dt = reloj.restart().asSeconds();

        if (controlador.estaJugando()) {
            controlador.actualizar(dt);
            controlador.dibujar();
        } else if (controlador.estaEnPantallaFinal()) {
            controlador.dibujar();
        } else {
            vista.limpiarPantalla();
            menu.dibujar();
            vista.presentar();
        }
    }
    return 0;
}
