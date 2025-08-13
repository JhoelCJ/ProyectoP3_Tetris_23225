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
            menu.procesarEvento(evento);
            controlador.procesarEvento(evento);

            if (evento.type == sf::Event::Closed) {
                vista.obtenerVentana().close();
            }
        }

        float dt = reloj.restart().asSeconds();

        if (!controlador.estaJugando()) {
            vista.limpiarPantalla();
            menu.dibujar();
            vista.presentar();
        } else {
            controlador.actualizar(dt);
            controlador.dibujar();
        }
    }
    return 0;
}
