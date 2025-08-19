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

Vista::Vista(int anchoPix, int altoPix, const char* titulo): ventana(sf::VideoMode(anchoPix, altoPix), titulo, sf::Style::Titlebar | sf::Style::Close){
    ventana.setFramerateLimit(60);
    fuente.loadFromFile("fuente/MinecraftTen.ttf");
}

Vista::~Vista() {}

bool Vista::abierta() { return ventana.isOpen(); }
sf::RenderWindow& Vista::obtenerVentana() { return ventana; }
void Vista::limpiarPantalla() { ventana.clear(sf::Color::Black); }
void Vista::presentar() { ventana.display(); }

void Vista::dibujarPantallaFinal(const std::string& titulo, int p1, int p2){

    sf::RectangleShape overlay(sf::Vector2f((float)ventana.getSize().x, (float)ventana.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    ventana.draw(overlay);

    if (fuente.getInfo().family.empty()) return;

    sf::Text textoTitulo(titulo, fuente, FONT_SIZE_LARGE);
    textoTitulo.setFillColor(sf::Color::White);

    sf::FloatRect tb = textoTitulo.getLocalBounds();
    textoTitulo.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
    textoTitulo.setPosition((float)ventana.getSize().x/2.f, (float)ventana.getSize().y*0.28f);
    ventana.draw(textoTitulo);

    std::ostringstream ss;
    if (p2 >= 0) {
        ss << "Jugador 1: " << p1 << " | Jugador 2: " << p2;
    } else {
        ss << "Puntos: " << p1;
    }

    sf::Text textoScores(ss.str(), fuente, FONT_SIZE_MEDIUM);
    textoScores.setFillColor(sf::Color::White);
    sf::FloatRect ts = textoScores.getLocalBounds();
    textoScores.setOrigin(ts.left + ts.width/2.f, ts.top + ts.height/2.f);
    textoScores.setPosition((float)ventana.getSize().x/2.f, (float)ventana.getSize().y*0.42f);
    ventana.draw(textoScores);

    float btnW = 200.f, btnH = 48.f;
    float centerX = ventana.getSize().x / 2.f;
    float baseY = ventana.getSize().y * 0.55f;

    sf::RectangleShape btnReiniciar(sf::Vector2f(btnW, btnH));
    btnReiniciar.setFillColor(sf::Color(70, 130, 180));
    btnReiniciar.setOrigin(btnW/2.f, btnH/2.f);
    btnReiniciar.setPosition(centerX, baseY);
    ventana.draw(btnReiniciar);

    sf::Text txtRe("Reiniciar", fuente, FONT_SIZE_MEDIUM);
    txtRe.setFillColor(sf::Color::White);
    sf::FloatRect tr = txtRe.getLocalBounds();
    txtRe.setOrigin(tr.left + tr.width/2.f, tr.top + tr.height/2.f);
    txtRe.setPosition(centerX, baseY);
    ventana.draw(txtRe);

    sf::RectangleShape btnSalir(sf::Vector2f(btnW, btnH));
    btnSalir.setFillColor(sf::Color(150, 50, 50));
    btnSalir.setOrigin(btnW/2.f, btnH/2.f);
    btnSalir.setPosition(centerX, baseY + btnH + 18.f);
    ventana.draw(btnSalir);

    sf::Text txtSalir("Salir", fuente, FONT_SIZE_MEDIUM);
    txtSalir.setFillColor(sf::Color::White);
    sf::FloatRect ts2 = txtSalir.getLocalBounds();
    txtSalir.setOrigin(ts2.left + ts2.width/2.f, ts2.top + ts2.height/2.f);
    txtSalir.setPosition(centerX, baseY + btnH + 18.f);
    ventana.draw(txtSalir);
    botonReiniciarRect = sf::FloatRect(centerX - btnW/2.f, baseY - btnH/2.f, btnW, btnH);
    botonReiniciarRect = btnReiniciar.getGlobalBounds();
    botonSalirRect = sf::FloatRect(centerX - btnW/2.f, baseY + btnH/2.f + 18.f - btnH/2.f, btnW, btnH);
    botonSalirRect     = btnSalir.getGlobalBounds();
}

void Vista::dibujarPanelLateral(int puntuacion0, int puntuacion1, int jugadorActivo, int nivel0, int nivel1, int siguienteTipo){
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

    const float controlsPadding = 12.f;
    const float controlsH = 110.f;
    const float controlsW = (float)SIDEBAR - 2.f * controlsPadding;
    const float controlsPosX = (float)BOARD_W + controlsPadding;
    const float controlsPosY = (float)(FILAS * TILE) - controlsH - controlsPadding;

    sf::RectangleShape controlsBg(sf::Vector2f(controlsW, controlsH));
    controlsBg.setPosition(controlsPosX, controlsPosY);
    controlsBg.setFillColor(sf::Color(25, 25, 25, 200));
    controlsBg.setOutlineThickness(1.f);
    controlsBg.setOutlineColor(sf::Color(80, 80, 80, 200));
    ventana.draw(controlsBg);

    sf::Text ctrlTitle("CONTROLES", fuente, 14);
    ctrlTitle.setStyle(sf::Text::Bold);
    ctrlTitle.setFillColor(sf::Color::White);
    ctrlTitle.setPosition(controlsPosX + 8.f, controlsPosY + 8.f);
    ventana.draw(ctrlTitle);

    sf::Text ctrlLine("", fuente, 13);
    ctrlLine.setFillColor(sf::Color(220, 220, 220));
    float ly = controlsPosY + 30.f;
    float lx = controlsPosX + 8.f;
    ctrlLine.setString("Izqu/Dere: Mover");
    ctrlLine.setPosition(lx, ly); ventana.draw(ctrlLine);
    ly += 18.f;
    ctrlLine.setString("Arriba: Rotar");
    ctrlLine.setPosition(lx, ly); ventana.draw(ctrlLine);
    ly += 18.f;
    ctrlLine.setString("Abajo: Acelerar");
    ctrlLine.setPosition(lx, ly); ventana.draw(ctrlLine);
    ly += 18.f;

    float btnW = controlsW;
    float btnH = 28.f;
    float btnX = controlsPosX;
    float btnY = controlsPosY - btnH - 10.f;

    sf::RectangleShape btnMenu(sf::Vector2f(btnW, btnH));
    btnMenu.setPosition(btnX, btnY);
    btnMenu.setFillColor(sf::Color(70, 130, 180));
    btnMenu.setOutlineThickness(1.f);
    btnMenu.setOutlineColor(sf::Color(40,40,40));
    btnMenu.setOrigin(0.f, 0.f);
    ventana.draw(btnMenu);

    sf::Text txtMenu("Volver", fuente, 16);
    txtMenu.setFillColor(sf::Color::White);
    sf::FloatRect tb = txtMenu.getLocalBounds();
    txtMenu.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
    txtMenu.setPosition(btnX + btnW/2.f, btnY + btnH/2.f - 2.f);
    ventana.draw(txtMenu);
    botonMenuRect = sf::FloatRect(btnX, btnY, btnW, btnH);
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
    sf::Text t(texto, fuente, 18);
    t.setFillColor(sf::Color::Red);
    float centerX = ventana.getSize().x / 2.f;
    float centerY = ventana.getSize().y / 1.7f;
    sf::FloatRect lb = t.getLocalBounds();
    t.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
    t.setPosition(centerX, centerY);
    ventana.draw(t);
}

void Vista::dibujarTextoFlotante(const std::string& texto, float x, float y, int tamanio, sf::Color color, float alpha){

    if (fuente.getInfo().family.empty()) return;

    if (tamanio <= 0) tamanio = 16;

    if (alpha < 0.f) alpha = 0.f;
    if (alpha > 1.f) alpha = 1.f;

    sf::Text t(texto, fuente, tamanio);

    sf::Uint8 a = static_cast<sf::Uint8>(alpha * 255.f);
    sf::Color col = color;
    col.a = a;
    t.setFillColor(col);

    sf::FloatRect lb = t.getLocalBounds();
    t.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
    t.setPosition(x, y);

    t.setOutlineThickness(1.f);
    sf::Color outline = sf::Color(0, 0, 0, a);
    t.setOutlineColor(outline);

    ventana.draw(t);
}

sf::FloatRect Vista::obtenerRectBotonReiniciar() const {
    return botonReiniciarRect;
}
sf::FloatRect Vista::obtenerRectBotonSalir() const {
    return botonSalirRect;
}
sf::FloatRect Vista::obtenerRectBotonMenu() const {
    return botonMenuRect;
}
