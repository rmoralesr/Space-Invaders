# Space Invaders — Especificación funcional del clon

Basado en la descripción del juego original y el documento de `I+D.md`, adaptada a nuestro sistema de coordenadas. (Redactado con IA)

## 0. Convenciones del proyecto

- **Resolución lógica:** 224 (ancho) × 256 (alto).
- **Origen:** esquina superior izquierda. **X** crece a la derecha, **y** crece hacia abajo.
- **Conversión desde el documento original:** `y_pantalla = 256 − Y_doc`. Lo que "sube" resta y lo que "baja" suma.
- **Tiempo:** todo se mide en ticks de 1/60 s, no en segundos reales.
- **Sprites:** en el original, Y es casi seguro el borde inferior del sprite. Con origen arriba, el borde superior que se dibuja es `y_top = (256 − Y_doc) − alto`. Por ejemplo, el cañón (alto 8) se dibuja con `y_top = 216`.
- **Comparaciones:** al invertir el eje, `>=` pasa a `<=` y al revés. Revisar con cuidado el techo del disparo, la invasión y el suelo de los disparos enemigos.
- **Hardware a no replicar:** VRAM, puertos I/O, registro de desplazamiento MB14241, DAA/BCD e interrupciones. Se replica el comportamiento, no la implementación.

### Tabla de constantes convertidas

| Elemento | Doc (Y arriba) | Nuestro (y abajo) |
|---|---|---|
| Suelo / línea verde | 32 | **224** |
| Y fija del cañón | 32 | **224** |
| Nacimiento del disparo del jugador | 40 | **216** |
| Techo del disparo del jugador | Y ≥ 216 | **y ≤ 40** |
| UFO (y fija) | 216 | **40** |
| Base de búnkeres | 48 | **208** |
| Rango vertical de búnkeres | 48 a 64 | **192 a 208** |
| Invasión (un alien llega) | Y ≤ 40 | **y ≥ 216** |
| Disparo enemigo toca el suelo | Y ≤ 32 | **y ≥ 224** |
| Intersección alien-búnker | 64 | **192** |

Las X no cambian: límites del cañón 16 a 216, límites de la flota 8 a 208, búnkeres en X = 32, 88, 144 y 200.

---

## 1. Sistema y estados (SYS)

### Bucle principal
- Bucle fijo a **60 ticks/s**, dividido en dos fases por tick.
- **Fase A:** lectura de entrada, movimiento de la nave, disparo del jugador y su colisión, temporizadores rápidos y sonidos de acción.
- **Fase B:** UFO, un alien, disparos enemigos, colisiones enemigas, colisión disparo contra disparo, comprobación de invasión, temporizadores lentos y sonidos de marcha y UFO.

### Estados del juego
- **Init.**
- **Atracción.**
- **Inicio de partida:** reinicia la oleada, dibuja búnkeres y reinicia al jugador.
- **Jugando.**
- **Muerte del jugador.**
- **Game Over.**
- La entrada de controles solo se lee en el estado "Jugando".

### Créditos
- Contador de 0 a 99 que se congela en 99.
- 1P Start exige ≥ 1 crédito; 2P Start exige ≥ 2.
- Se puede insertar moneda durante una partida sin interrumpirla.
- Al iniciar la partida se descuentan los créditos consumidos.

### TILT (opcional)
- Borra los sprites, destruye la nave del jugador, pierde los créditos y muestra `TILT`.

### Configuración (equivalente a los DIP switches)
- Vidas iniciales: 3, 4, 5 o 6.
- Umbral de vida extra: 1000 o 1500 puntos.
- Mostrar u ocultar `CREDIT`.
- Los cambios solo se aplican al empezar una partida nueva.

---

## 2. Interfaz y pantallas (UI)

### Cabecera y pie
- `SCORE<1>` arriba a la izquierda, `HI-SCORE` arriba al centro y `SCORE<2>` arriba a la derecha.
- Puntuaciones con ceros a la izquierda (por ejemplo `0050`).
- Iconos de vidas de reserva abajo (máximo 6 dibujados).
- `CREDIT XX` en la esquina inferior derecha.

### Fuente y color
- Caracteres de 8×8 px, monocromo.
- El original es monocromo con celofán: rojo en la franja del UFO y verde en la franja inferior (búnkeres, suelo y cañón). El estilo final lo decide el equipo.

### Modo atracción (en ciclo)
1. Tabla de puntos: `=? MYSTERY`, `=30 POINTS`, `=20 POINTS`, `=10 POINTS`.
2. Animación 1: `PLAY SPACE INVADERS` con la Y invertida; un alien camina, quita la letra errónea y coloca la correcta.
3. Animación 2: `INSERT CCOIN`; un alien camina y dispara a la primera C, dejando `INSERT COIN`.
4. Demo automática con una secuencia de unos 11 comandos fijos durante unos 30 s.
5. Insertar moneda en cualquier punto interrumpe todo y salta al inicio de partida.
- Easter egg "TAITO COP": opcional.

---

## 3. Jugador (PLR)

### Movimiento
- 1 px por tick, con límites X ∈ [16, 216].
- y fija: 224 (sprite de 16×8, `y_top = 216`).
- Si se pulsan izquierda y derecha a la vez, gana la izquierda.

### Disparo
- Solo **1 proyectil a la vez**; mientras haya uno en vuelo se ignora el botón.
- Nace en X = nave + 8 e y = 216.
- Sube **4 px por tick** (y −= 4); sprite de 1×4.
- Al llegar a **y ≤ 40** explota (8×8 durante 2 ticks) y se libera el canal.
- Mantener pulsado no dispara en ráfaga: hay que soltar y volver a pulsar.
- Cada disparo creado incrementa el contador del UFO (ver sección 7).

### Muerte
- El juego se congela unos **120 ticks** (2 s). La explosión alterna dos sprites cada 8 ticks.
- Después se resta 1 vida.
  - Si quedan vidas: se limpian los disparos, se recoloca la nave en X = 16 y hay 1 s de inmunidad.
  - Si no quedan: Game Over.
- Si la causa es una invasión, las vidas pasan directamente a 0.

---

## 4. Flota alienígena (ALI)

### Formación
5 filas × 11 columnas, 55 aliens. Cada alien tiene 2 frames de animación (A/B) que alternan en cada movimiento.

| Fila | Tipo | Tamaño | Puntos |
|---|---|---|---|
| 5 (arriba) | Pulpo | 8×8 | 30 |
| 3 y 4 | Cangrejo | 11×8 | 20 |
| 1 y 2 (abajo) | Calamar | 12×8 | 10 |

### Movimiento (un alien por tick)
- Un índice recorre los 55 (0 a 54). Salta los muertos sin coste y mueve, anima y redibuja el siguiente vivo.
- DX = 2 px por paso.
- Con N vivos, la flota completa se mueve cada N ticks: 55 aliens dan ~0,9 s por paso; 1 alien da 60 pasos/s.
- **La aceleración sale de esta mecánica**, no de una variable de velocidad.

### Bordes y descenso
- Al terminar la pasada, si algún alien vivo toca X ≥ 208 o X ≤ 8, se invierte la dirección y se activa un flag de descenso.
- En la siguiente pasada no hay movimiento horizontal: toda la flota baja **8 px** (y += 8) y se borra el flag. (**Por lo que he visto en los gameplays si que parece haber movimiento horizontal**)

### Último invasor
- DX sube a 3 en ciertas oleadas (el documento es ambiguo; ver apartado de incoherencias).

### Invasión
- Si algún alien llega a **y ≥ 216**: derrota inmediata. Las vidas pasan a 0 y la secuencia termina en Game Over.

### Muerte de un alien
- Se marca como muerto y se decrementa el contador de vivos.
- Aparece una explosión de 16×8 durante 16 ticks.
- Suma los puntos y suena el efecto.
- Mientras dura la explosión, la flota se pausa.
- Si era el último, se pasa a la siguiente oleada.

---

## 5. Disparos enemigos (AMMO)

### Canales
Tres canales, uno por tipo, con **máximo 3 disparos en pantalla** y sin repetir tipo a la vez.

| Tipo | Elección de columna |
|---|---|
| Plunger | Tabla fija de 16 columnas, con puntero que avanza |
| Squiggly | Aleatoria (columna 0 a 10) |
| Rolling | Columna más cercana al jugador, apuntando a su cabeza |

### Emisor y sprite
- El emisor se busca recorriendo la columna de abajo hacia arriba: dispara el primer alien vivo.
- Si la columna está vacía, se aborta y se prueba la contigua en el siguiente tick.
- Sprites de 3×7 px, animados.

### Velocidad de caída
- Oleada 1: **+3 px/tick**.
- Oleada 2 en adelante: **+4 px/tick**.

### Recarga tras destruirse un disparo
- Puntuación < 200: 48 ticks.
- Puntuación entre 200 y 1000: 16 ticks.
- Puntuación ≥ 1000: 8 ticks.

### Impacto en el suelo
- Al llegar a **y ≥ 224**: explosión de 6×8 durante 6 ticks, que borra 1 o 2 px de la línea verde.
- Después se libera el canal.

---

## 6. Búnkeres (BNK)

### Estructura
- 4 búnkeres de 22×16 px, en X = 32, 88, 144 y 200.
- Ocupan **y de 192 a 208** (base en 208).
- Forma de bóveda redondeada con arco hueco abajo.
- Se guardan como **píxeles** (un bitmap), no como objetos con vida.

### Daño por disparos
- Al chocar con un píxel encendido del búnker, el disparo desaparece.
- Se borra un parche de 6×8 mediante una máscara de daño:
  - Una para el disparo del jugador (abanico hacia arriba).
  - Otra para el disparo enemigo (cráter hacia abajo).
- La erosión es acumulativa: puede abrir canales por los que los disparos pasan limpios.

### Daño por aliens
- Donde el sprite de un alien solapa el búnker (**y entre 192 y 208**), borra esos píxeles.
- Los aliens "devoran" capas de 8 px de alto al pasar.

### Reinicio
- Los búnkeres se redibujan intactos en cada oleada nueva.

---

## 7. UFO (UFO)

### Aparición
- Cada ~600 ticks (10 s) y solo si quedan **≥ 8 aliens vivos**.
- Tamaño 16×7, en **y = 40**.
- 2 px por paso, 1 paso cada 2 ticks.

### Dirección
- Oleadas impares: sale por la izquierda (X = 8) y va a la derecha.
- Oleadas pares: sale por la derecha (X = 208) y va a la izquierda.
- Al salir de pantalla se desactiva, se silencia y se reinicia el temporizador.

### Puntuación determinista (no aleatoria)
- Contador 0 a 15, incrementado en **cada disparo del jugador**.
- Al destruir el UFO se lee una tabla de 16 valores:

| Índice | Puntos | Índice | Puntos |
|---|---|---|---|
| 0 | 100 | 8 | 300 |
| 1 | 50 | 9 | 100 |
| 2 | 50 | 10 | 100 |
| 3 | 100 | 11 | 100 |
| 4 | 150 | 12 | 50 |
| 5 | 100 | 13 | 150 |
| 6 | 100 | 14 | 100 |
| 7 | 50 | 15 | 50 |

- La puntuación se muestra como texto temporal durante 24 ticks.

### Sonido
- Bucle mientras está en pantalla.
- Se corta al destruirlo y se sustituye por el efecto de impacto.

---

## 8. Puntuación y vidas (SCORE)

- Puntuación del jugador activo, con **HI-SCORE** actualizado en vivo.
- Valores posibles: 10, 20, 30, 50, 100, 150 y 300.
- **Vida extra:** una sola por partida, al llegar a 1000 o 1500 puntos según la configuración, con sonido propio.
- Límite visual: máximo 6 iconos de vida dibujados (la vida extra se concede igualmente aunque no se dibuje).

---

## 9. Oleadas (WAVE)

### Al limpiar la flota
- Sube el contador de oleada.
- Se reponen los 55 aliens y los búnkeres.
- Se reinician los temporizadores de disparo y el contador del UFO.

### Altura inicial de la flota (y) según oleada

| Oleada | y de inicio |
|---|---|
| 1 | 136 |
| 2 | 152 |
| 3 | 160 |
| 4 | 168 |
| 5 | 176 |
| 6 a 9 | 184 |
| 10 en adelante | el ciclo vuelve a empezar en 152 |

La flota nace más abajo en cada oleada, así que la y aumenta.

### Velocidad de disparos
- Disparos enemigos: +3 px/tick en la oleada 1 y +4 px/tick desde la 2.
- El disparo del jugador no cambia nunca (−4 px/tick).

---

## 10. Colisiones (COL)

- **Original:** por píxel, leyendo qué píxeles ya están encendidos. En C++ se puede usar un framebuffer lógico (recomendado, para que la erosión de búnkeres sea natural) o hitboxes con máscaras.

### Zonas por y
| Zona | Rango |
|---|---|
| UFO | y ≤ 48 |
| Aliens | 48 < y ≤ 192 |
| Búnkeres | 192 < y ≤ 208 |
| Cañón | y ≥ 216 |

### Disparo contra disparo
- Si |ΔX| ≤ 1 y se cruzan en y, ambos se anulan con una explosión de 4×4 durante 4 ticks.
- No da puntos.
- Un disparo del jugador anula solo uno enemigo.
- Orden: comprobar primero la colisión con el UFO, ya que el techo (y ≤ 40) solapa su zona.

---

## 11. Game Over y dos jugadores (RULE)

### Game Over
- Se muestra `GAME OVER` en el centro durante 180 ticks (3 s).
- Se actualiza el HI-SCORE, se limpia la pantalla y se vuelve a la atracción.

### Dos jugadores
- Los turnos se alternan cada vez que el jugador activo pierde una nave.
- Cada jugador conserva por separado su flota, búnkeres, oleada, vidas y puntuación.
- Se muestra `PLAYER 1` o `PLAYER 2` durante 120 ticks antes de dar el control.
- Si uno agota sus vidas y el otro no, el partido continúa con el superviviente.

---

## 12. Temporizadores (TIMER)

| Evento | Ticks |
|---|---|
| Muerte del jugador | 120 |
| Explosión de alien | 16 |
| Aparición del UFO | 600 |
| Recarga enemiga | 8, 16 o 48 |
| Explosión de disparo en suelo | 6 |
| Explosión de disparo del jugador en el techo | 2 |
| Explosión por anulación de disparos | 4 |
| Texto de puntos del UFO | 24 |
| Mensaje `GAME OVER` | 180 |
| Mensaje `PLAYER N` | 120 |
| Texto animado de menu (aparición de letra) | 6 |

---

## 13. Sonido (SND)

Efectos puntuales o en bucle, sin música.

| Evento | Tipo |
|---|---|
| Disparo del jugador | pulso |
| Explosión del jugador | larga |
| Muerte de alien | breve |
| Vida extra | tono agudo |
| UFO en vuelo | bucle |
| Impacto al UFO | descendente |
| Marcha alienígena | 4 notas cíclicas |

- Cada paso completo de la flota dispara la siguiente nota de la marcha. Como los pasos se aceleran, el "latido" también.
- Se pueden usar WAV/OGG o tonos sintetizados.

---

## 14. Orden de ejecución por tick (INT)

El orden importa para reproducir bien los comportamientos.

**Fase A**
1. Leer entrada.
2. Mover la nave.
3. Mover el disparo del jugador (y −= 4).
4. Colisión de ese disparo.
5. Temporizadores rápidos.
6. Sonidos de acción.

**Fase B**
1. Mover el UFO.
2. Mover un alien.
3. Mover los disparos enemigos (y += 3 o 4).
4. Colisiones enemigas.
5. Colisión disparo contra disparo.
6. Comprobar invasión (y ≥ 216).
7. Temporizadores lentos.
8. Sonidos de marcha y UFO.

---

## 15. Incoherencias del documento (decisiones pendientes)

1. **Disparo 23 del UFO:** el documento dice que con 22 disparos previos el contador cae en el índice 8 (300 pts). Con módulo 16, el índice sería 6 o 7. Además habla de "módulo 15" y "cada 15 disparos", pero la tabla tiene 16 entradas. Decidir si se implementa la tabla tal cual con mod 16 o se ajusta el desfase.
2. **Variable `0x202E`:** aparece como flag de descenso y como Y base de la flota. En el clon son dos variables distintas.
3. **DX del último alien:** "60 px/s" y "DX = 3 en ciertas oleadas" no se explican bien. Propuesta: aplicar siempre DX = 2 y dejar el 3 como opcional.
4. **Velocidad de disparos enemigos:** 3 px/tick en la oleada 1 y 4 desde la 2 es lo que dice el documento. No se ha contrastado con otras fuentes.
5. **Techo del disparo frente a la zona del UFO:** el techo (y ≤ 40) solapa la zona del UFO (y ≤ 48), así que hay que comprobar primero la colisión con el UFO.
