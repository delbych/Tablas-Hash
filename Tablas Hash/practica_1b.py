#Ejercicio 1B - Hash Polinomial djb2

def djb2(s: str) -> int:
    h = 5381

    for c in s:
        h = ((h << 5) + h) + ord(c)

    return h


#Cadenas a evaluar
cadena1 = "SIS"
cadena2 = "AVL"

#Tamaño de la tabla hash
m = 31

# Calcular valores hash
hash1 = djb2(cadena1)
hash2 = djb2(cadena2)

print("Cadena SIS:")
print("Hash final:", hash1)
print("Indice:", hash1 % m)

print("\nCadena AVL:")
print("Hash final:", hash2)
print("Indice:", hash2 % m)

#Verificar colisiones
if hash1 % m == hash2 % m:
    print("\nExiste colision")
else:
    print("\nNo existe colision\n")
