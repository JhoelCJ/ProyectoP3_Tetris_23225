#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

const int FILAS = 20;
const int COLUMNAS = 10;
const int TAM_TILE = 24;

struct Celda {
    bool llena;
    Celda* siguiente;
    Celda(bool v=false) : llena(v), siguiente(nullptr) {}
};
struct Fila {
    Celda* primera;
    Fila* siguiente;
    Fila(int cols) : siguiente(nullptr) {
        primera = new Celda;
        Celda* curr = primera;
        for(int i=1; i<cols; ++i) {
            curr->siguiente = new Celda;
            curr = curr->siguiente;
        }
    }
};

struct NodoPieza {
    int tipo;
    NodoPieza* siguiente;
    NodoPieza(int t) : tipo(t), siguiente(nullptr) {}
};
struct ColaPiezas {
    NodoPieza* frente;
    NodoPieza* fin;
    ColaPiezas() : frente(nullptr), fin(nullptr) {}

    void encolar(int tipo) {
        NodoPieza* n = new NodoPieza(tipo);
        if(!fin) frente = fin = n;
        else { fin->siguiente = n; fin = n; }
    }
    int desencolar() {
        if(!frente) return -1;
        NodoPieza* tmp = frente;
        int t = tmp->tipo;
        frente = frente->siguiente;
        if(!frente) fin = nullptr;
        delete tmp;
        return t;
    }
    bool vacia() { return frente == nullptr; }
};

void rotar90(int M[][4], int inicio, int fin) {
    if(inicio >= fin) return;
    for(int i = inicio; i < fin; ++i) {
        int offset = i - inicio;
        int top = M[inicio][i];
        M[inicio][i] = M[fin-offset][inicio];
        M[fin-offset][inicio] = M[fin][fin-offset];
        M[fin][fin-offset] = M[i][fin];
        M[i][fin] = top;
    }
    rotar90(M, inicio+1, fin-1);
}

bool filaLlena(Fila* f, int cols) {
    Celda* c = f->primera;
    for(int i=0; i<cols; ++i, c=c->siguiente)
        if(!c->llena) return false;
    return true;
}
void eliminarFila(Fila*& head, Fila* anterior) {
    Fila* toDel = (anterior ? anterior->siguiente : head);
    Fila* siguiente = toDel->siguiente;

    Celda* c = toDel->primera;
    while(c) { Celda* nx = c->siguiente; delete c; c = nx; }

    if(anterior) anterior->siguiente = siguiente;
    else head = siguiente;
    delete toDel;
}
void escanearYEliminar(Fila*& head, int cols, int filasTotales) {
    Fila* curr = head;
    Fila* prev = nullptr;
    int count = 0;
    while(curr) {
        if(filaLlena(curr, cols)) {
            eliminarFila(head, prev);
            ++count;
            if (prev) curr = prev->siguiente;
            else curr = head;
        } else {
            prev = curr;
            curr = curr->siguiente;
        }
    }
}

Fila* crearFilas(int filas, int cols) {
    Fila* head = new Fila(cols);
    Fila* cur = head;
    for (int i = 1; i < filas; ++i) {
        cur->siguiente = new Fila(cols);
        cur = cur->siguiente;
    }
    return head;
}

int contarFilas(Fila* head) {
    int cnt = 0;
    Fila* f = head;
    while (f) { ++cnt; f = f->siguiente; }
    return cnt;
}

Fila* obtenerFila(Fila* head, int r) {
    if (r < 0) return nullptr;
    Fila* f = head;
    for (int i = 0; f && i < r; ++i) f = f->siguiente;
    return f;
}

Celda* obtenerCelda(Fila* head, int r, int c) {
    Fila* f = obtenerFila(head, r);
    if (!f) return nullptr;
    Celda* cl = f->primera;
    for (int j = 0; cl && j < c; ++j) cl = cl->siguiente;
    return cl;
}

bool obtenerValorCelda(Fila* head, int r, int c) {
    Celda* cl = obtenerCelda(head, r, c);
    if (!cl) return false;
    return cl->llena;
}

void establecerValorCelda(Fila*& head, int r, int c, bool val) {
    Celda* cl = obtenerCelda(head, r, c);
    if (!cl) return;
    cl->llena = val;
}

void dibujarTablero(Fila* head, sf::RenderWindow& ventana, int filas, int cols) {
    sf::RectangleShape tile(sf::Vector2f(TAM_TILE-1, TAM_TILE-1));
    Fila* f = head;
    for (int r = 0; r < filas && f; ++r) {
        Celda* c = f->primera;
        for (int col = 0; col < cols && c; ++col) {
            if (c->llena) {
                tile.setPosition((float)(col * TAM_TILE), (float)(r * TAM_TILE));
                tile.setFillColor(sf::Color(100, 200, 255));
                ventana.draw(tile);
            }
            c = c->siguiente;
        }
        f = f->siguiente;
    }
}

void bloquearPiezaEnTablero(Fila*& head, int shape[4][4], int tamanio, int px, int py, int filas, int cols) {
    for (int r = 0; r < tamanio; ++r)
        for (int c = 0; c < tamanio; ++c)
            if (shape[r][c]) {
                int br = py + r;
                int bc = px + c;
                if (br >= 0 && br < filas && bc >= 0 && bc < cols) establecerValorCelda(head, br, bc, true);
            }
}

bool colisionaConTablero(Fila* head, int shape[4][4], int tamanio, int px, int py, int filas, int cols) {
    for (int r = 0; r < tamanio; ++r) {
        for (int c = 0; c < tamanio; ++c) {
            if (!shape[r][c]) continue;
            int br = py + r;
            int bc = px + c;
            if (bc < 0 || bc >= cols) return true;
            if (br >= filas) return true;
            if (br >= 0) {
                if (obtenerValorCelda(head, br, bc)) return true;
            }
        }
    }
    return false;
}

int limpiarLineasCompletas(Fila*& head, int cols, int filasTotales) {
    int antes = contarFilas(head);
    escanearYEliminar(head, cols, filasTotales);
    int despues = contarFilas(head);
    int eliminadas = antes - despues;

    while (contarFilas(head) < filasTotales) {
        Fila* nueva = new Fila(cols);
        nueva->siguiente = head;
        head = nueva;
    }
    return eliminadas;
}

int PIEZAS[7][4][4] = {

    {{0,0,0,0},
     {1,1,1,1},
     {0,0,0,0},
     {0,0,0,0}},

    {{0,0,0,0},
     {0,1,1,0},
     {0,1,1,0},
     {0,0,0,0}},

    {{0,0,0,0},
     {1,1,1,0},
     {0,1,0,0},
     {0,0,0,0}},

    {{0,0,0,0},
     {0,1,1,0},
     {1,1,0,0},
     {0,0,0,0}},

    {{0,0,0,0},
     {1,1,0,0},
     {0,1,1,0},
     {0,0,0,0}},

    {{0,0,0,0},
     {1,1,1,0},
     {0,0,1,0},
     {0,0,0,0}},

    {{0,0,0,0},
     {1,1,1,0},
     {1,0,0,0},
     {0,0,0,0}}
};

struct Pieza {
    int forma[4][4];
    int tamanio;
    int x, y;
    int tipo;

    Pieza() : tamanio(4), x(3), y(-1), tipo(0) {
        for (int r=0;r<4;++r) for (int c=0;c<4;++c) forma[r][c]=0;
    }

    void establecerDesdeTipo(int t) {
        tipo = t % 7;
        for (int r=0;r<4;++r)
            for (int c=0;c<4;++c)
                forma[r][c] = PIEZAS[tipo][r][c];
        x = (COLUMNAS/2) - 2;
        y = -1;
    }

    void rotar() {
        rotar90(forma, 0, tamanio-1);
    }

    void dibujar(sf::RenderWindow& ventana) {
        sf::RectangleShape tile(sf::Vector2f(TAM_TILE-1, TAM_TILE-1));
        for (int r = 0; r < tamanio; ++r) {
            for (int c = 0; c < tamanio; ++c) {
                if (forma[r][c]) {
                    int br = y + r;
                    int bc = x + c;
                    tile.setPosition((float)(bc * TAM_TILE), (float)(br * TAM_TILE));
                    tile.setFillColor(sf::Color(200, 100, 100));
                    if (br >= 0) ventana.draw(tile);
                }
            }
        }
    }
};

void llenarSaco(ColaPiezas& q) {
    int arr[7];
    for (int i=0;i<7;++i) arr[i]=i;

    for (int i=6;i>0;--i) {
        int j = rand() % (i+1);
        int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
    }
    for (int i=0;i<7;++i) q.encolar(arr[i]);
}

int main() {
    srand((unsigned)time(nullptr));
    sf::RenderWindow ventana(sf::VideoMode(COLUMNAS * TAM_TILE, FILAS * TAM_TILE), "Tetris - Implementacion en Español");
    ventana.setFramerateLimit(60);

    Fila* tablero = crearFilas(FILAS, COLUMNAS);

    ColaPiezas cola;
    llenarSaco(cola);

    Pieza piezaActual;
    if (cola.vacia()) llenarSaco(cola);
    piezaActual.establecerDesdeTipo(cola.desencolar());

    sf::Clock reloj;
    float acumulador = 0.0f;
    float intervaloCaida = 0.8f;
    int lineasEliminadasTotal = 0;
    int nivel = 1;

    bool finJuego = false;

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                ventana.close();
            }
            if (evento.type == sf::Event::KeyPressed && !finJuego) {
                if (evento.key.code == sf::Keyboard::Left) {
                    if (!colisionaConTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x - 1, piezaActual.y, FILAS, COLUMNAS))
                        piezaActual.x--;
                } else if (evento.key.code == sf::Keyboard::Right) {
                    if (!colisionaConTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x + 1, piezaActual.y, FILAS, COLUMNAS))
                        piezaActual.x++;
                } else if (evento.key.code == sf::Keyboard::Down) {
                    if (!colisionaConTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x, piezaActual.y + 1, FILAS, COLUMNAS))
                        piezaActual.y++;
                    else {
                        bloquearPiezaEnTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x, piezaActual.y, FILAS, COLUMNAS);
                        int eliminadas = limpiarLineasCompletas(tablero, COLUMNAS, FILAS);
                        if (eliminadas) {
                            lineasEliminadasTotal += eliminadas;
                            nivel = 1 + (lineasEliminadasTotal / 10);
                            intervaloCaida = 0.8f * pow(0.9f, nivel-1);
                            if (intervaloCaida < 0.1f) intervaloCaida = 0.1f;
                        }
                        if (cola.vacia()) llenarSaco(cola);
                        piezaActual.establecerDesdeTipo(cola.desencolar());
                        if (colisionaConTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x, piezaActual.y, FILAS, COLUMNAS))
                            finJuego = true;
                    }
                } else if (evento.key.code == sf::Keyboard::Up) {
                    int copia[4][4];
                    for (int r=0;r<4;++r) for (int c=0;c<4;++c) copia[r][c]=piezaActual.forma[r][c];
                    piezaActual.rotar();
                    if (colisionaConTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x, piezaActual.y, FILAS, COLUMNAS)) {
                        bool kicked = false;
                        for (int dx = -2; dx <= 2 && !kicked; ++dx) {
                            if (!colisionaConTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x + dx, piezaActual.y, FILAS, COLUMNAS)) {
                                piezaActual.x += dx; kicked = true; break;
                            }
                        }
                        if (!kicked) {
                            for (int r=0;r<4;++r) for (int c=0;c<4;++c) piezaActual.forma[r][c]=copia[r][c];
                        }
                    }
                } else if (evento.key.code == sf::Keyboard::Space) {
                    while (!colisionaConTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x, piezaActual.y + 1, FILAS, COLUMNAS))
                        piezaActual.y++;
                    bloquearPiezaEnTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x, piezaActual.y, FILAS, COLUMNAS);
                    int eliminadas = limpiarLineasCompletas(tablero, COLUMNAS, FILAS);
                    if (eliminadas) {
                        lineasEliminadasTotal += eliminadas;
                        nivel = 1 + (lineasEliminadasTotal / 10);
                        intervaloCaida = 0.8f * pow(0.9f, nivel-1);
                        if (intervaloCaida < 0.1f) intervaloCaida = 0.1f;
                    }
                    if (cola.vacia()) llenarSaco(cola);
                    piezaActual.establecerDesdeTipo(cola.desencolar());
                    if (colisionaConTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x, piezaActual.y, FILAS, COLUMNAS))
                        finJuego = true;
                }
            }
        }

        float dt = reloj.restart().asSeconds();
        acumulador += dt;
        if (!finJuego && acumulador >= intervaloCaida) {
            acumulador = 0.0f;
            if (!colisionaConTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x, piezaActual.y + 1, FILAS, COLUMNAS)) {
                piezaActual.y++;
            } else {
                bloquearPiezaEnTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x, piezaActual.y, FILAS, COLUMNAS);
                int eliminadas = limpiarLineasCompletas(tablero, COLUMNAS, FILAS);
                if (eliminadas) {
                    lineasEliminadasTotal += eliminadas;
                    nivel = 1 + (lineasEliminadasTotal / 10);
                    intervaloCaida = 0.8f * pow(0.9f, nivel-1);
                    if (intervaloCaida < 0.1f) intervaloCaida = 0.1f;
                }
                if (cola.vacia()) llenarSaco(cola);
                piezaActual.establecerDesdeTipo(cola.desencolar());
                if (colisionaConTablero(tablero, piezaActual.forma, piezaActual.tamanio, piezaActual.x, piezaActual.y, FILAS, COLUMNAS)) {
                    finJuego = true;
                }
            }
        }

        ventana.clear(sf::Color::Black);
        dibujarTablero(tablero, ventana, FILAS, COLUMNAS);
        piezaActual.dibujar(ventana);

        if (finJuego) {
            sf::Font fuente;
            if (fuente.loadFromFile("fuente/MinecraftTen.ttf")) {
                sf::Text texto("GAME OVER", fuente, 24);
                texto.setFillColor(sf::Color::Red);
                texto.setPosition(10.f, (FILAS * TAM_TILE) / 2.f - 24.f);
                ventana.draw(texto);
            }
        }

        ventana.display();
    }

    while (tablero) {
        Fila* nxt = tablero->siguiente;
        Celda* c = tablero->primera;
        while (c) {
            Celda* nx = c->siguiente;
            delete c;
            c = nx;
        }
        delete tablero;
        tablero = nxt;
    }
    return 0;
}
