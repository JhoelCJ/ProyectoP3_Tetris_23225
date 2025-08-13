#include "TetrisMenu.h"

Menu::Menu(Vista& vistaRef, Controlador& controladorRef): gui(vistaRef.obtenerVentana()), controlador(controladorRef){
    crearWidgets();
}

void Menu::crearWidgets() {
    auto btn1 = tgui::Button::create("1 Jugador");
    btn1->setSize(200, 40);
    btn1->setPosition("50%", 150);
    btn1->setOrigin(0.5f, 0.f);
    btn1->onPress([this](){
        controlador.iniciarJuego(Controlador::Modo::UnJugador);
    });
    gui.add(btn1);

    auto btn2 = tgui::Button::create("2 Jugadores");
    btn2->setSize(200, 40);
    btn2->setPosition("50%", 200);
    btn2->setOrigin(0.5f, 0.f);
    btn2->onPress([this](){
        controlador.iniciarJuego(Controlador::Modo::DosJugadores);
    });
    gui.add(btn2);

    auto btnC = tgui::Button::create("Controles");
    btnC->setSize(200, 40);
    btnC->setPosition("50%", 250);
    btnC->setOrigin(0.5f, 0.f);
    btnC->onPress([this](){
        auto win = tgui::ChildWindow::create("Controles");
        win->setSize(420, 180);
        win->setPosition("50%", "50%");
        win->setOrigin(0.5f, 0.5f);
        auto label = tgui::Label::create(
            "Controles:\nIzquierda/Derecha: mover\nArriba: rotar\nAbajo: bajar");
        label->setAutoSize(true);
        label->setPosition(10,10);
        win->add(label);
        auto btnCerrar = tgui::Button::create("Cerrar");
        btnCerrar->setSize(120, 30);
        btnCerrar->setPosition("50%", 140);
        btnCerrar->setOrigin(0.5f, 0.f);
        btnCerrar->onPress([this, win](){ gui.remove(win); });
        win->add(btnCerrar);
        gui.add(win);
    });
    gui.add(btnC);

    auto btnQ = tgui::Button::create("Salir");
    btnQ->setSize(200, 40);
    btnQ->setPosition("50%", 300);
    btnQ->setOrigin(0.5f, 0.f);
    btnQ->onPress([this](){
        controlador.salirDelMenu();
    });
    gui.add(btnQ);
}

void Menu::procesarEvento(const sf::Event& evento) {
    gui.handleEvent(evento);
}

void Menu::dibujar() {
    gui.draw();
}
