#include <cassert>
#include <string>
#include "hash.h"

int main() {
    TablaHash<std::string, int> th(7);

    //P1: Insercion y busqueda basica
    th.insertar("Alice", 95);
    th.insertar("Bob",   80);
    th.insertar("Carol", 88);

    assert(th.buscar("Alice").value() == 95);
    assert(th.buscar("Bob").value() == 80);
    assert(th.tamanio() == 3);

    //P2: Actualizacion de valor existente
    th.insertar("Alice", 99);

    assert(th.buscar("Alice").value() == 99);
    assert(th.tamanio() == 3);

    //P3: Clave inexistente
    assert(!th.buscar("Zara").has_value());

    //P4: Eliminacion
    assert(th.eliminar("Bob") == true);
    assert(!th.buscar("Bob").has_value());
    assert(th.eliminar("Bob") == false);
    assert(th.tamanio() == 2);

    //P5: Rehashing automatico
    TablaHash<int, std::string> th2(7);

    for (int i = 0; i < 10; ++i)
        th2.insertar(i, "v" + std::to_string(i));

    assert(th2.cubos() > 7);
    assert(th2.buscar(5).value() == "v5");
    assert(th2.factor_carga() < 0.76);

    std::cout << "Todos los test pasaron!\n\n";

    return 0;
}
