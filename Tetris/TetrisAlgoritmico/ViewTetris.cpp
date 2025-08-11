#include "TetrisView.h"
#include <string>

Vista::Vista(int anchoPix, int altoPix, const char* titulo)
: ventana(sf::VideoMode(anchoPix, altoPix), titulo)
{
    ventana.setFramerateLimit(60);
    fuente.loadFromFile("fuente/MinecraftTen.ttf");
}

Vista::~Vista() {}

bool Vista::abierta() { return ventana.isOpen(); }
sf::RenderWindow& Vista::obtenerVentana() { return ventana; }
void Vista::limpiarPantalla() { ventana.clear(sf::Color::Black); }
void Vista::presentar() { ventana.display(); }

void Vista::dibujarTablero(Fila* head, int filas, int cols) {
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE-1, TILE_SIZE-1));
    Fila* f = head;
    for (int r = 0; r < filas && f; ++r) {
        Celda* c = f->primera;
        for (int col = 0; col < cols && c; ++col) {
            if (c->llena) {
                tile.setPosition((float)(col * TILE_SIZE), (float)(r * TILE_SIZE));
                tile.setFillColor(sf::Color(100, 200, 255));
                ventana.draw(tile);
            }
            c = c->siguiente;
        }
        f = f->siguiente;
    }
}

void Vista::dibujarPieza(int forma[4][4], int tam, int px, int py) {
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE-1, TILE_SIZE-1));
    for (int r = 0; r < tam; ++r) for (int c = 0; c < tam; ++c)
        if (forma[r][c]) {
            int br = py + r, bc = px + c;
            tile.setPosition((float)(bc * TILE_SIZE), (float)(br * TILE_SIZE));
            tile.setFillColor(sf::Color(200, 100, 100));
            if (br >= 0) ventana.draw(tile);
        }
}

void Vista::dibujarHUD(int puntuacion, int nivel) {
    if (fuente.getInfo().family.empty()) return;
    sf::Text t;
    t.setFont(fuente);
    t.setCharacterSize(16);
    t.setFillColor(sf::Color::White);
    t.setString("Puntos: " + std::to_string(puntuacion));
    t.setPosition(5.f, 5.f);
    ventana.draw(t);
    t.setString("Nivel: " + std::to_string(nivel));
    t.setPosition(5.f, 25.f);
    ventana.draw(t);
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
