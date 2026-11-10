# Guía rápida de Git en VS Code

## Flujo de trabajo (resumen)

**Antes de empezar**

1. Abrir el apartado de Git y comprobar que estamos en **nuestra rama**.
2. Compilar y comprobar que funciona.
3. **Commit** con un mensaje detallado.
4. **Sync Changes** para subirlo a nuestra rama.
5. Avisar al resto del equipo de que hemos subido algo.
6. **Fetch**: descargar la información de la nube.
7. **Merge**: elegir la rama de la nube (icono de nube) y fusionarla con la nuestra.
8. Comprobar que aparece *Outgoing Changes* en el grafo.
9. **Push**: subir el resultado a nuestra rama.

---

## 1. Dónde está Git en VS Code

El apartado de Git es este icono de la barra lateral:

![Icono de Source Control en la barra lateral de VS Code](image-1.png)

## 2. Comprobar la rama actual

En la barra inferior de VS Code se indica la rama en la que estamos:

![Barra inferior de VS Code mostrando la rama features-raul](image.png)

> **CUIDADO:** comprobad **SIEMPRE** que estáis en **vuestra rama** antes de tocar nada.

## 3. Subir nuestros cambios a nuestra rama (Commit)

Los cambios siempre se suben primero a **nuestra rama personal**. Cuando terminemos una tarea, compilemos y comprobemos que funciona, la subiremos a la rama `features`.

> **Nota:** `features` es la rama donde convergen todos los cambios (los propios y los de los compañeros), así que hay que tener cuidado con lo que se sube para no romper nada.

**Paso 1: Commit.** En el bloque superior de la pestaña de Git escribimos el mensaje y pulsamos **Commit**. El mensaje debe ser **lo más detallado posible**: así el historial es legible para todos y es más fácil localizar el commit donde se coló un bug o un error.

![Cuadro de mensaje de commit y botón Commit](image-5.png)

**Paso 2: Sync Changes.** Una vez hecho el commit, pulsamos **Sync Changes** para subirlo a la nube.

![Botón Sync Changes](image-6.png)

## 4. Bajarnos cambios de los compañeros (Fetch → Merge → Push)

Cuando un compañero suba un cambio, **avisará al resto**. Entonces cada uno debe seguir estos pasos:

### 4.1. Fetch y Merge

Desde el menú `...` de la pestaña de Git:

1. **Fetch**: descarga la información de lo que hay en la nube.
2. **Branch → Merge...**: fusiona esos cambios con nuestra rama.

![Menú de Git con las opciones Fetch y Branch > Merge](image-3.png)

### 4.2. Elegir la rama correcta (la de la nube)

Al elegir rama para el merge, hay que **elegir siempre la opción con el icono de la nube**. Es importante diferenciarlas:

| Icono | Significado |
|---|---|
| Rama (el mismo icono que usa VS Code) | Rama **local**: contiene nuestros cambios en nuestro equipo. |
| Nube | Rama **remota** (GitHub): contiene el desarrollo subido por el compañero. |

![Lista de ramas locales y remotas con sus iconos](image-2.png)

### 4.3. Comprobar que se han bajado los cambios

En la vista del grafo de ramas (debajo de la zona de commit) aparecerá un nuevo cambio con el círculo de la rama **punteado** (*Outgoing Changes*). Significa que tenemos commits pendientes de subir: los que acabamos de mergear del compañero.

![Grafo de ramas con Outgoing Changes](image-4.png)

### 4.4. Push

Para dejar esos cambios en nuestra rama, hacemos **Push**, que sube a la nube el código que tenemos commiteado.

![Menú de Git con la opción Push](image-7.png)
