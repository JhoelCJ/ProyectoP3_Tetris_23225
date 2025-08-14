#include "TetrisMenu.h"

Menu::Menu(Vista& vistaRef, Controlador& controladorRef): gui(vistaRef.obtenerVentana()), controlador(controladorRef){
    crearWidgets();
}

void Menu::crearWidgets() {

    auto aplicarEstilo = [](tgui::Button::Ptr btn, const tgui::Color& bg, const tgui::Color& hover, const tgui::Color& down, const tgui::Color& borderColor) {
        btn->setSize(220, 46);
        btn->setTextSize(18);

        auto r = btn->getRenderer();
        r->setBackgroundColor(bg);
        r->setBackgroundColorHover(hover);
        r->setBackgroundColorDown(down);
        r->setTextColor(tgui::Color::White);
        r->setBorderColor(borderColor);
        r->setBorders(tgui::Borders(2));
        r->setRoundedBorderRadius(12);

        btn->setOrigin(0.5f, 0.f);
    };

    auto btn1 = tgui::Button::create("1 Jugador");
    aplicarEstilo(btn1, tgui::Color(52, 152, 219), tgui::Color(72, 172, 239), tgui::Color(32, 132, 199), tgui::Color::Black);
    btn1->setSize(200, 40);
    btn1->setPosition("50%", 150);
    btn1->setOrigin(0.5f, 0.f);
    btn1->onPress([this](){
        controlador.iniciarJuego(Controlador::Modo::UnJugador);
    });
    gui.add(btn1);

    auto btn2 = tgui::Button::create("2 Jugadores");
    aplicarEstilo(btn2, tgui::Color(46, 204, 113), tgui::Color(66, 224, 133), tgui::Color(26, 184, 93), tgui::Color::Black);
    btn2->setSize(200, 40);
    btn2->setPosition("50%", 200);
    btn2->setOrigin(0.5f, 0.f);
    btn2->onPress([this](){
        controlador.iniciarJuego(Controlador::Modo::DosJugadores);
    });
    gui.add(btn2);

    auto btnC = tgui::Button::create("Controles");
    aplicarEstilo(btnC, tgui::Color(155, 89, 182), tgui::Color(175, 109, 202), tgui::Color(135, 69, 162), tgui::Color::Black);
    btnC->setSize(200, 40);
    btnC->setPosition("50%", 250);
    btnC->onPress([this](){
        auto win = tgui::ChildWindow::create("Controles");
        win->setSize(350, 175);
        win->setPosition("50%", "50%");
        win->setOrigin(0.5f, 0.5f);
        win->setTitleButtons(tgui::ChildWindow::TitleButton::None);

        win->getRenderer()->setTitleColor(tgui::Color::White);
        win->getRenderer()->setBackgroundColor(tgui::Color(30, 30, 30));

        auto label = tgui::Label::create(
            "\tControles:\n\nIzquierda/Derecha: mover\nArriba: rotar\nAbajo: bajar"
        );
        label->setAutoSize(true);
        label->setPosition(10, 10);
        label->getRenderer()->setTextColor(tgui::Color::White);
        win->add(label);

        auto btnCerrar = tgui::Button::create("Cerrar");
        btnCerrar->setSize(120, 30);
        btnCerrar->setPosition("50%", 120);
        btnCerrar->setOrigin(0.5f, 0.f);
        btnCerrar->onPress([this, win](){ gui.remove(win); });
        win->add(btnCerrar);

        gui.add(win);
    });

    gui.add(btnC);

    auto btnQ = tgui::Button::create("Salir");
     aplicarEstilo(btnQ, tgui::Color(231, 76, 60), tgui::Color(251, 96, 80), tgui::Color(211, 56, 40), tgui::Color::Black);
    btnQ->setSize(200, 40);
    btnQ->setPosition("50%", 350);
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
