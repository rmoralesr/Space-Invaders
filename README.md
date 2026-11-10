# ORGANIZACIÓN
## GENERAL

- Siempre se compila desde el archivo `main.cc`.
- En `main.cc` no se deben añadir funciones a menos que sea una función que controle el juego general.
- Los `#include` en `main.cc` deben ir en orden de menor a mayor, si un archivo necesita otro, su include va después de el include del otro.
- La strcut `Game` es la más alta de la jerarquía y todo lo demás cuelga de ella, solo las funciones directoras y las más importantes deberían acceder a ella.

## TRABAJO EN EQUIPO Y ORGANIZACIÓN EN GRUPO

> [!Caution]
> Este proyecto está hecho para aprender a trabajar en equipo, tenemos que comunicarnos entre nosotros y trabajar juntos, cada uno tiene sus roles y todas las opiniones cuentan por igual.

- Se trabaja por archivos separados, no más de una persona encargada de un archivo.
- Todas las funciones que no estén 100% claras con el nombre deben tener un comentario de `@brief` que explique con pocas palabras su funcionalidad.
- No se debe tocar el código de otras personas sin su consentimiento, incluso si es un bug, simplemente avisa a la persona que se encarga de esa parte.

## RAMAS

- Cada persona trabaja en su propia rama. (ej. features-ivan, features-raul...)
- La rama main no se debe tocar a menos que se sepa que todo lo que está hecho funciona y compila sin errores, y no está a medias.
- Se sube el trabajo a la rama features general cuando al menos no tiene errores de compilación.

## POR CONFIRMAR

- Todos los tipos y constantes que se usen en más de un módulo del juego deben estar declarados en `types.cc`.
- Si un tipo o constante no se va a usar en más de un módulo, puede declararse dentro del namespace de ese módulo.
- No deben haber variables globales, las funciones obtienen lo que necesitan por parámetros, de forma jerárquica.