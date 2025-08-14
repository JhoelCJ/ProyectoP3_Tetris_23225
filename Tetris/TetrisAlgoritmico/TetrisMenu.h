#ifndef TETRISMENU_H_INCLUDED
#define TETRISMENU_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include "TetrisController.h"
#include "TetrisView.h"

class Menu {
public:
    Menu(Vista& vistaRef, Controlador& controladorRef);

    void procesarEvento(const sf::Event& evento);

    void dibujar();

private:
    tgui::Gui gui;
    Controlador& controlador;
    void crearWidgets();

};

#endif // TETRISMENU_H_INCLUDED
