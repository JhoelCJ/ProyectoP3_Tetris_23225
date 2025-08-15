#include "TetrisModel.h"

int PIEZAS[7][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},

    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},

    {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},

    {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},

    {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},

    {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},

    {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}}
};

const sf::Color PIEZA_COLORES[7] = {
    sf::Color(0, 255, 255),
    sf::Color(255, 255, 0),
    sf::Color(128, 0, 128),
    sf::Color(0, 255, 0),
    sf::Color(255, 0, 0),
    sf::Color(0, 0, 255),
    sf::Color(255, 165, 0)
};

int ColaPiezas::verFrente() {
    if (!frente) return -1;
    return frente->tipo;
}
void ColaPiezas::encolar(int tipo) {
    NodoPieza* n = new NodoPieza(tipo);
    if (!fin) frente = fin = n;
    else { fin->siguiente = n; fin = n; }
}
int ColaPiezas::desencolar() {
    if (!frente) return -1;
    NodoPieza* tmp = frente;
    int t = tmp->tipo;
    frente = frente->siguiente;
    if (!frente) fin = nullptr;
    delete tmp;
    return t;
}
bool ColaPiezas::vacia() { return frente == nullptr; }
void ColaPiezas::limpiar() { while (!vacia()) desencolar(); }

void rotar90(int M[][4], int inicio, int fin) {
    if (inicio >= fin) return;
    for (int i = inicio; i < fin; ++i) {
        int offset = i - inicio;
        int top = M[inicio][i];
        M[inicio][i] = M[fin-offset][inicio];
        M[fin-offset][inicio] = M[fin][fin-offset];
        M[fin][fin-offset] = M[i][fin];
        M[i][fin] = top;
    }
    rotar90(M, inicio+1, fin-1);
}

Juego::Juego(int puntosPorNivel_, float intervaloBase_)
: puntuacion(0), nivel(1), puntosPorNivel(puntosPorNivel_), cambioNivel(false), intervaloBase(intervaloBase_) {}

void Juego::puntosPorLinea(int lineas) {
    if (lineas <= 0) { cambioNivel = false; return; }
    if (lineas == 4) puntuacion += 500;
    else puntuacion += 100 * lineas;
    int nuevoNivel = 1 + (puntuacion / puntosPorNivel);
    cambioNivel = (nuevoNivel != nivel);
    nivel = nuevoNivel;
}

int Juego::obtenerPuntuacion() const { return puntuacion; }
int Juego::obtenerNivel() const { return nivel; }
float Juego::obtenerFactorVelocidad() const { return pow(0.9f, float(nivel - 1)); }
bool Juego::nivelSubio() const { return cambioNivel; }

Tablero::Tablero(int f, int c) : filas(f), cols(c) {
    head = crearFilas(filas, cols);
}

Tablero::~Tablero() {
    Fila* cur = head;
    while (cur) {
        Celda* c = cur->primera;
        while (c) {
            Celda* nx = c->siguiente;
            delete c;
            c = nx;
        }
        Fila* next = cur->siguiente;
        delete cur;
        cur = next;
    }
}

Fila* Tablero::crearFilas(int filasN, int colsN) {
    Fila* headLoc = new Fila(colsN);
    Fila* cur = headLoc;
    for (int i=1;i<filasN;++i) {
        cur->siguiente = new Fila(colsN);
        cur = cur->siguiente;
    }
    return headLoc;
}

Fila* Tablero::obtenerFila(int r) {
    if (r < 0) return nullptr;
    Fila* f = head;
    for (int i=0; f && i<r; ++i) f = f->siguiente;
    return f;
}

Celda* Tablero::obtenerCelda(int r, int c) {
    Fila* f = obtenerFila(r);
    if (!f) return nullptr;
    Celda* cl = f->primera;
    for (int j=0; cl && j<c; ++j) cl = cl->siguiente;
    return cl;
}

bool Tablero::obtenerValor(int r, int c) {
    Celda* cl = obtenerCelda(r,c);
    if (!cl) return false;
    return cl->tipo != 0;
}
void Tablero::establecerValor(int r, int c, int tipo) {
    Celda* cl = obtenerCelda(r,c);
    if (!cl) return;
    cl->tipo = tipo;
}

int Tablero::contarFilas() {
    int cnt = 0;
    Fila* f = head;
    while (f) { ++cnt; f = f->siguiente; }
    return cnt;
}

bool Tablero::filaLlena(Fila* f) {
    Celda* c = f->primera;
    for (int i=0;i<cols;++i) {
        if (!c) return false;
        if (c->tipo == 0) return false;
        c = c->siguiente;
    }
    return true;
}

void Tablero::eliminarUnaFila(Fila*& headRef, Fila* anterior) {
    Fila* toDel = (anterior ? anterior->siguiente : headRef);
    Fila* siguiente = toDel->siguiente;
    Celda* c = toDel->primera;
    while (c) { Celda* nx = c->siguiente; delete c; c = nx; }
    if (anterior) anterior->siguiente = siguiente;
    else headRef = siguiente;
    delete toDel;
}

void Tablero::escanearYEliminar(Fila*& headRef, int colsLocal, int filasTotales) {
    Fila* curr = headRef;
    Fila* prev = nullptr;
    while (curr) {
        if (filaLlena(curr)) {
            eliminarUnaFila(headRef, prev);
            if (prev) curr = prev->siguiente;
            else curr = headRef;
        } else {
            prev = curr;
            curr = curr->siguiente;
        }
    }
}

int Tablero::limpiarLineasCompletas() {
    int antes = contarFilas();
    escanearYEliminar(head, cols, filas);
    int despues = contarFilas();
    int eliminadas = antes - despues;
    while (contarFilas() < filas) {
        Fila* nueva = new Fila(cols);
        nueva->siguiente = head;
        head = nueva;
    }
    return eliminadas;
}

bool Tablero::colisiona(int shape[4][4], int tam, int px, int py) {
    for (int r=0;r<tam;++r) {
        for (int c=0;c<tam;++c) {
            if (!shape[r][c]) continue;
            int br = py + r;
            int bc = px + c;
            if (bc < 0 || bc >= cols) return true;
            if (br >= filas) return true;
            if (br >= 0) if (obtenerValor(br, bc)) return true;
        }
    }
    return false;
}
void Tablero::fijarPieza(int shape[4][4], int tam, int px, int py, int tipoPieza) {
    for (int r = 0; r < tam; ++r) {
        for (int c = 0; c < tam; ++c) {
            if (!shape[r][c]) continue;
            int br = py + r;
            int bc = px + c;
            if (br >= 0 && br < filas && bc >= 0 && bc < cols)
                establecerValor(br, bc, tipoPieza);
        }
    }
}

void Tablero::limpiarTodo() {
    Fila* f = head;
    while (f) {
        Celda* c = f->primera;
        while (c) {
            c->tipo = 0;
            c = c->siguiente;
        }
        f = f->siguiente;
    }
}

Fila* Tablero::obtenerHead() { return head; }
