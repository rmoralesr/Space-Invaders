# ORGANIZACIÓN (POR CONFIRMAR)

- Siempre se compila desde el archivo `main.cc`.
- Se trabaja por archivos separados.
- Cada persona trabaja en su propia rama.
- La rama main no se debe tocar a menos que se sepa que todo lo que está hecho funciona y compila sin errores, y no está a medias.
- Se sube el trabajo a la rama development cuando al menos no tiene errores de compilación.
- En `main.cc` no se deben añadir funciones a menos que sea una función que controle el juego general.
- Los `#include` en `main.cc` deben ir en orden de menor a mayor, si un archivo necesita otro, su include va después de el include del otro.
- Todos los tipos y constantes que se usen en más de un módulo del juego deben estar declarados en `types.cc`.
- Si un tipo o constante no se va a usar en más de un módulo, puede declararse dentro del namespace de ese módulo.
- No deben haber variables globales, las funciones obtienen lo que necesitan por parámetros, de forma jerárquica.
- La strcut `Game` es la más alta de la jerarquía y solo las funciones directoras y las más importantes deberían acceder a ella.