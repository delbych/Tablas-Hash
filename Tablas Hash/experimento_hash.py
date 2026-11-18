import time
import random
import bisect
import statistics

def experimento(n: int) -> dict:
    datos = random.sample(range(n * 10), n)
    objetivo = random.choice(datos)

    # Tabla Hash
    tabla = {v: v for v in datos}

    t0 = time.perf_counter_ns()
    _ = objetivo in tabla
    t_hash = time.perf_counter_ns() - t0

    # Lista ordenada + busqueda binaria
    lista = sorted(datos)

    t0 = time.perf_counter_ns()
    bisect.bisect_left(lista, objetivo)
    t_bst = time.perf_counter_ns() - t0

    return {
        'n': n,
        'hash_ns': t_hash,
        'bst_ns': t_bst,
        'ratio': t_bst / max(t_hash, 1)
    }


# Ejecutar 5 repeticiones por n y promediar
for n in [1000, 5000, 10000, 50000, 100000]:
    resultados = [experimento(n) for _ in range(5)]

    hash_avg = statistics.mean(
        r['hash_ns'] for r in resultados
    )

    bst_avg = statistics.mean(
        r['bst_ns'] for r in resultados
    )

    print(
        f"n={n:>7} | "
        f"Hash={hash_avg:>8.1f} ns | "
        f"BST={bst_avg:>8.1f} ns | "
        f"Ratio={bst_avg/max(hash_avg,1):.2f}x"
    )
