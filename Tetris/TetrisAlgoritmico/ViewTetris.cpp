#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "TetrisView.h"
#include "TetrisModel.h"
#include <string>

const sf::Color COLORES_PIEZAS[7] = {
    sf::Color::Cyan,
    sf::Color::Yellow,
    sf::Color(160, 32, 240),
    sf::Color::Green,
    sf::Color::Red,
    sf::Color::Blue,
    sf::Color(255, 165, 0)
};

Vista::Vista(int anchoPix, int altoPix, const char* titulo): ventana(sf::VideoMode(anchoPix, altoPix), titulo){
    ventana.setFramerateLimit(60);
    fuente.loadFromFile("fuente/MinecraftTen.ttf");
}

Vista::~Vista() {}

bool Vista::abierta() { return ventana.isOpen(); }
sf::RenderWindow& Vista::obtenerVentana() { return ventana; }
void Vista::limpiarPantalla() { ventana.clear(sf::Color::Black); }
void Vista::presentar() { ventana.display(); }

void Vista::dibujarPanelLateral(int puntuacion0, int puntuacion1, int jugadorActivo,
                                int nivel0, int nivel1, int siguienteTipo)
{
    const int TILE = TILE_SIZE;
    const int SIDEBAR = SIDEBAR_WIDTH;
    const int BOARD_W = COLUMNAS * TILE;

    sf::RectangleShape fondo(sf::Vector2f((float)SIDEBAR, (float)(FILAS * TILE)));
    fondo.setPosition((float)BOARD_W, 0.f);
    fondo.setFillColor(sf::Color(40, 40, 40, 220));
    ventana.draw(fondo);

    if (fuente.getInfo().family.empty()) return;

    sf::Text texto;
    texto.setFont(fuente);
    texto.setCharacterSize(16);
    texto.setFillColor(sf::Color::White);

    float xBase = (float)BOARD_W + 10.f;
    float y = 12.f;

    texto.setString("ESTADO");
    texto.setCharacterSize(18);
    texto.setStyle(sf::Text::Bold);
    texto.setPosition(xBase, y);
    ventana.draw(texto);

    texto.setStyle(sf::Text::Regular);
    texto.setCharacterSize(16);
    y += 28.f;

    if (jugadorActivo >= 0) {
        texto.setString("Jugador 1: " + std::to_string(puntuacion0));
        texto.setPosition(xBase, y); ventana.draw(texto);
        y += 20.f;
        texto.setString("Nivel P1: " + std::to_string(nivel0));
        texto.setPosition(xBase, y); ventana.draw(texto);
        y += 28.f;

        texto.setString("Jugador 2: " + std::to_string(puntuacion1));
        texto.setPosition(xBase, y); ventana.draw(texto);
        y += 20.f;
        texto.setString("Nivel P2: " + std::to_string(nivel1));
        texto.setPosition(xBase, y); ventana.draw(texto);
        y += 28.f;

        sf::Text tag("Turno: Jugador " + std::to_string(jugadorActivo + 1), fuente, 14);
        tag.setFillColor(sf::Color::Yellow);
        tag.setPosition(xBase, y); ventana.draw(tag);
        y += 34.f;
    } else {
        texto.setString("Puntos: " + std::to_string(puntuacion0));
        texto.setPosition(xBase, y); ventana.draw(texto);
        y += 22.f;
        texto.setString("Nivel: " + std::to_string(nivel0));
        texto.setPosition(xBase, y); ventana.draw(texto);
        y += 36.f;
    }

    texto.setString("SIGUIENTE:");
    texto.setPosition(xBase, y); ventana.draw(texto);
    y += 22.f;

    const int PREVIEW_SIZE = 4;
    const float PREVIEW_TILE = 18.f;
    float previewX = xBase;
    float previewY = y;

    sf::RectangleShape rectPreview(sf::Vector2f(PREVIEW_TILE * PREVIEW_SIZE + 6.f, PREVIEW_TILE * PREVIEW_SIZE + 6.f));
    rectPreview.setPosition(previewX - 3.f, previewY - 3.f);
    rectPreview.setFillColor(sf::Color(20,20,20,200));
    ventana.draw(rectPreview);

    sf::RectangleShape mini(sf::Vector2f(PREVIEW_TILE - 2.f, PREVIEW_TILE - 2.f));
    if (siguienteTipo >= 0 && siguienteTipo < 7) {
        for (int r = 0; r < PREVIEW_SIZE; ++r) {
            for (int c = 0; c < PREVIEW_SIZE; ++c) {
                if (PIEZAS[siguienteTipo][r][c]) {
                    mini.setPosition(previewX + c * PREVIEW_TILE, previewY + r * PREVIEW_TILE);
                    mini.setFillColor(COLORES_PIEZAS[siguienteTipo]);
                    ventana.draw(mini);
                }
            }
        }
    }

    std::string instr = "Controles:\n\t|<-| |->| : mover\nArriba: rotar\nAbajo: bajar";
    texto.setCharacterSize(13);
    texto.setPosition(xBase, FILAS * TILE - 100.f);
    texto.setString(instr);
    ventana.draw(texto);
}

void Vista::dibujarTablero(Fila* head, int filas, int cols) {
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE-1, TILE_SIZE-1));
    Fila* f = head;
    for (int r = 0; r < filas && f; ++r) {
        Celda* c = f->primera;
        for (int col = 0; col < cols && c; ++col) {
            if (c->tipo > 0) {
                tile.setPosition((float)(col * TILE_SIZE), (float)(r * TILE_SIZE));
                tile.setFillColor(COLORES_PIEZAS[c->tipo - 1]);
                ventana.draw(tile);
            }
            c = c->siguiente;
        }
        f = f->siguiente;
    }
}
void Vista::dibujarPieza(int forma[4][4], int tam, int px, int py, int tipoPieza) {
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE-1, TILE_SIZE-1));

    if (tipoPieza >= 1 && tipoPieza <= 7)
        tile.setFillColor(COLORES_PIEZAS[tipoPieza - 1]);
    else
        tile.setFillColor(sf::Color(200, 100, 100));

    for (int r = 0; r < tam; ++r) {
        for (int c = 0; c < tam; ++c) {
            if (forma[r][c]) {
                int br = py + r, bc = px + c;
                tile.setPosition((float)(bc * TILE_SIZE), (float)(br * TILE_SIZE));
                if (br >= 0) ventana.draw(tile);
            }
        }
    }
}

void Vista::dibujarHUD(int puntuacion0, int puntuacion1, int jugadorActivo) {
    if (fuente.getInfo().family.empty()) return;
    sf::Text t;
    t.setFont(fuente);
    t.setCharacterSize(16);
    t.setFillColor(sf::Color::White);

    if (jugadorActivo < 0) {
        t.setString("Puntos: " + std::to_string(puntuacion0));
        t.setPosition(5.f, 5.f);
        ventana.draw(t);
    } else {
        t.setString("Jugador 1: " + std::to_string(puntuacion0));
        t.setPosition(5.f, 5.f);
        ventana.draw(t);
        t.setString("Jugador 2: " + std::to_string(puntuacion1));
        t.setPosition(5.f, 25.f);
        ventana.draw(t);

        sf::Text tag("Turno: \nJugador " + std::to_string(jugadorActivo + 1), fuente, 16);
        tag.setFillColor(sf::Color::Yellow);
        tag.setPosition(5.f, 45.f);
        ventana.draw(tag);
    }
}

void Vista::dibujarTextoCentral(const char* texto) {
    if (fuente.getInfo().family.empty()) return;
    sf::Text t(texto, fuente, 28);
    t.setFillColor(sf::Color::Red);
    float x = 10.f;
    float y = (FILAS * TILE_SIZE) / 2.f - 24.f;
    t.setPosition(x,y);
    ventana.draw(t);
}
