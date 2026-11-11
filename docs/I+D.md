// no subir a main
# Especificación Funcional de Desarrollo: Space Invaders (Taito, 1978)

## 1. Arquitectura del Sistema, Estados del Juego y Flujo Global

### SYS-01: Bucle de Interrupciones y Sincronización Vertical
* **Objetivo**: Garantizar el renderizado a 60 Hz y la secuenciación de tareas críticas mediante interrupciones de hardware sincronizadas con el barrido del monitor CRT.
* **Comportamiento Exacto**: El procesador Intel 8080 atiende dos interrupciones por cuadro de video (16.66 ms). INT 1 ocurre a mitad del barrido vertical (línea 96) e INT 2 ocurre al final del barrido (línea 224).
* **Condiciones de Activación**: Hardware de video disparando líneas de sincronía vertical (V-Blank y Mid-Blank).
* **Entradas y Controles**: Vectores de interrupción hardware RST 1 (`0x08`) y RST 2 (`0x10`).
* **Reglas y Lógica de Ejecución**:
  1. **INT 1 (`0x08`)**: Ejecuta lectura de puertos de entrada (`0x01`, `0x02`), procesa la entrada del jugador (movimiento y botón de disparo), actualiza la posición del Cañón Láser y su proyectil, y actualiza los registros de salida de sonido.
  2. **INT 2 (`0x10`)**: Actualiza la lógica de los alienígenas (movimiento de un alien por cuadro), proyectiles enemigos, movimiento del OVNI/UFO, detección de colisiones en VRAM y renderizado de sprites.
* **Variables y Parámetros Involucrados**:
  * Frecuencia de reloj CPU: 1.993472 MHz.
  * Tasa de refresco: 59.54 Hz (aproximadamente 60 Hz).
  * `0x2000`: Vector de estado del juego.
* **Interacciones y Dependencias**: Coordina la ejecución secuencial de `PLR-01`, `AMMO-01`, `ALI-01`, `UFO-01` y `SND-01`.
* **Resultados y Cambios de Estado**: Mantiene una cadencia de ejecución determinista de 60 ticks por segundo dividida en dos sub-ciclos.
* **Casos Límite y Excepciones**: Si el procesamiento de INT 2 se extiende más de 8.33 ms, se produce un solapamiento con el barrido de pantalla, generando el efecto "parpadeo" o ralentización por hardware.

---

### SYS-02: Máquina de Estados Finitos del Juego
* **Objetivo**: Controlar las transiciones entre la inicialización, el modo atracción, el juego activo y el fin de partida.
* **Comportamiento Exacto**: Administra la variable de estado principal (`0x2000`) para determinar qué subrutinas se ejecutan en cada ciclo principal.
* **Condiciones de Activación**: Inicio del sistema, inserción de monedas, presión de botones de inicio, o eventos de victoria/derrota.
* **Entradas y Controles**: Puertos I/O `0x01` (Monedas, Start 1P/2P), temporizadores internos de victoria/derrota.
* **Reglas y Lógica de Ejecución**:
  * **Estado 0 (`0x00`)**: Inicialización de hardware y limpieza de memoria RAM (`0x2000` - `0x23FF`).
  * **Estado 1 (`0x01`)**: Modo Atracción (Attract Mode). Ejecuta animaciones en pantalla y espera créditos.
  * **Estado 2 (`0x02`)**: Secuencia de inicio de partida (Reset de oleada, dibujo de búnkeres, reset de jugador).
  * **Estado 3 (`0x03`)**: Bucle activo de jugabilidad.
  * **Estado 4 (`0x04`)**: Secuencia de muerte del jugador (Animación de explosión).
  * **Estado 5 (`0x05`)**: Secuencia de Game Over.
* **Variables y Parámetros Involucrados**:
  * RAM `0x2000`: Estado del juego (1 byte).
  * RAM `0x2001`: Jugador activo (`0` = P1, `1` = P2).
  * RAM `0x200E`: Contador de créditos (Formato BCD, 0 a 99).
* **Interacciones y Dependencias**: Controla la visibilidad de la interfaz visual (`UI-01`) e invalida la entrada de controles cuando no está en Estado 3.
* **Resultados y Cambios de Estado**: Alterna el flujo global del software.
* **Casos Límite y Excepciones**: La inserción de monedas durante el Estado 3 incrementa el contador de créditos en RAM sin interrumpir la partida en curso.

---

### SYS-03: Gestión de Créditos y Sistema Antifraude (TILT)
* **Objetivo**: Procesar monedas físicas, mantener el contador de créditos en formato BCD y abortar partidas ante manipulaciones físicas del mueble.
* **Comportamiento Exacto**: Detecta pulsos en el mecanismo de moneda e interrupciones del switch de impacto TILT.
* **Condiciones de Activación**: Transición de bajo a alto en los bits 0 y 2 del Puerto 1 / Puerto 2.
* **Entradas y Controles**:
  * Puerto 1 Bit 0: Monedero (`1` = Moneda detectada).
  * Puerto 2 Bit 2: Switch TILT (`1` = Impacto detectado).
* **Reglas y Lógica de Ejecución**:
  1. Al recibir pulso de moneda, incrementa RAM `0x200E` utilizando suma BCD (`ADD` + `DAA`). Si el valor excede 99 (`0x99`), se congela en 99.
  2. Al activarse el bit TILT: Borra todos los sprites en VRAM, detiene el procesamiento de la partida, destruye la nave del jugador, fuerza la pérdida de créditos y muestra la palabra `TILT` en pantalla.
* **Variables y Parámetros Involucrados**:
  * RAM `0x200E`: Créditos almacenados (0x00 a 0x99).
* **Interacciones y Dependencias**: Desbloquea la lectura de los botones `1P Start` (Puerto 1 Bit 2) y `2P Start` (Puerto 1 Bit 1) cuando RAM `0x200E` >= 1 o >= 2 respectivamente.
* **Resultados y Cambios de Estado**: Actualiza el texto `CREDIT XX` en la interfaz visual.
* **Casos Límite y Excepciones**: Las monedas insertadas con el contador en 99 son aceptadas por el hardware pero no incrementan la cifra en memoria.

---

### SYS-04: Lectura y Configuración por DIP Switches
* **Objetivo**: Configurar las condiciones operativas de la partida leídas desde interruptores físicos en la placa arcade.
* **Comportamiento Exacto**: Lee la palabra de configuración del Puerto 2 al iniciar una partida y establece parámetros de vidas y puntuaciones de bonificación.
* **Condiciones de Activación**: Inicialización de nueva partida (`SYS-02` Estado 2).
* **Entradas y Controles**: Puerto de Entrada 2 (`0x02`).
* **Reglas y Lógica de Ejecución**:
  * Bits 0-1 (Vidas iniciales): `00` = 3 Vidas; `01` = 4 Vidas; `10` = 5 Vidas; `11` = 6 Vidas.
  * Bit 3 (Puntuación de Vida Extra): `0` = 1,000 Puntos; `1` = 1,500 Puntos.
  * Bit 7 (Muestra de Créditos): `0` = Mostrar mensaje `CREDIT`; `1` = Ocultar.
* **Variables y Parámetros Involucrados**:
  * RAM `0x2002`: Vidas restantes Jugador 1.
  * RAM `0x2003`: Vidas restantes Jugador 2.
  * RAM `0x2005`: Umbral de puntuación para vida extra.
* **Interacciones y Dependencias**: Determina el número inicial de sprites del cañón mostrados en la parte inferior de la interfaz (`UI-01`).
* **Resultados y Cambios de Estado**: Establece constantes operativas de la sesión.
* **Casos Límite y Excepciones**: Cambiar los DIP switches durante una partida activa no surte efecto hasta reiniciar el sistema o iniciar un nuevo juego.

---

## 2. Interface Gráfica, Pantallas y Renderizado VRAM

### UI-01: Estructura del Framebuffer VRAM y Rotación Hardware
* **Objetivo**: Mapear la matriz lógica de pantalla en la memoria de video monocromática de 7,168 bytes.
* **Comportamiento Exacto**: La pantalla física consta de 256 píxeles de alto por 224 píxeles de ancho. VRAM mapeada entre `0x2400` y `0x3FFF`.
* **Condiciones de Activación**: Continuo durante todos los estados del juego.
* **Entradas y Controles**: Registro de desplazamiento Fujitsu MB14241 (Puertos `0x02`, `0x03`, `0x04`).
* **Reglas y Lógica de Ejecución**:
  1. Cada byte de VRAM representa una columna vertical de 8 píxeles.
  2. Coordenada Y en código (0 a 255) representa el desplazamiento horizontal real en pantalla. Coordenada X en código (0 a 224) representa el desplazamiento vertical.
  3. Mapeo de dirección VRAM: `Dirección = 0x2400 + (X * 32) + (Y / 8)`.
  4. Para dibujar sprites descentrados a nivel de píxel, los bytes del mapa de bits se pasan al chip MB14241 (escribiendo el desplazamiento en puerto `0x02` y el dato en `0x04`), leyendo el resultado desplazado desde el puerto `0x03`.
* **Variables y Parámetros Involucrados**:
  * Rango VRAM: `0x2400` - `0x3FFF` (7 KB).
  * Ancho lógico: 224 píxeles (28 bytes por fila de renderizado).
  * Alto lógico: 256 píxeles (32 bytes por columna).
* **Interacciones y Dependencias**: Todos los subsistemas de renderizado (`PLR`, `ALI`, `BNK`, `UFO`, `AMMO`) escriben directamente en VRAM.
* **Resultados y Cambios de Estado**: Generación de la señal de video monocromática rasterizada.
* **Casos Límite y Excepciones**: Escrituras fuera del rango `0x2400`-`0x3FFF` sobrescriben variables críticas de RAM de trabajo (`0x2000`-`0x23FF`), corruptiendo el estado del juego.

---

### UI-02: Generador de Caracteres y Capa de Texto
* **Objetivo**: Dibujar caracteres alfanuméricos de 8x8 píxeles en coordenadas fijas de VRAM para marcadores e instrucciones.
* **Comportamiento Exacto**: Lee fuentes tipográficas cuadradas codificadas en la ROM (`0x1E00` - `0x1FFF`) y las transfiere byte a byte a VRAM.
* **Condiciones de Activación**: Solicitud de actualización de texto en pantalla.
* **Entradas y Controles**: Cadenas de caracteres ASCII terminadas en valor nulo (`0x00`).
* **Reglas y Lógica de Ejecución**:
  * Cada carácter ocupa 8 bytes consecutivos en ROM.
  * La posición de cada texto está prefijada en coordenadas de bytes de VRAM:
    * `SCORE<1>`: Coordenada VRAM `0x2702` (Fila superior izquierda).
    * `HI-SCORE`: Coordenada VRAM `0x270E` (Fila superior centro).
    * `SCORE<2>`: Coordenada VRAM `0x271A` (Fila superior derecha).
    * `CREDIT XX`: Coordenada VRAM `0x2402` (Esquina inferior derecha).
* **Variables y Parámetros Involucrados**:
  * Puntero de fuente ROM: `0x1E00`.
  * Matriz de caracteres: 8x8 bits monocromáticos.
* **Interacciones y Dependencias**: Invocado por `SCORE-01` al modificar marcadores y por `SYS-02` durante el Modo Atracción.
* **Resultados y Cambios de Estado**: Modificación de bloques de bytes en VRAM.
* **Casos Límite y Excepciones**: Las puntuaciones menores a 1000 se rellenan con ceros a la izquierda (ej. `0050`).

---

### UI-03: Secuencia de Modo Atracción (Attract Mode)
* **Objetivo**: Ejecutar la presentación cíclica de instrucciones, tabla de puntuaciones y demostración jugable cuando no hay créditos activos.
* **Comportamiento Exacto**: Alterna entre dos animaciones tipográficas interactivas y una partida de demostración guiada por ROM.
* **Condiciones de Activación**: `SYS-02` en Estado 1 y contador de créditos = 0.
* **Entradas y Controles**: Bucle de tiempo automático de la CPU.
* **Reglas y Lógica de Ejecución**:
  1. **Pantalla de Título y Puntuaciones**: Muestra la tabla de valores de los alienígenas:
     * `=? MYSTERY` (UFO)
     * `=30 POINTS` (Invasor Superior - Pulpo)
     * `=20 POINTS` (Invasor Medio - Cangrejo)
     * `=10 POINTS` (Invasor Inferior - Calamar)
  2. **Animación 1 (Corrección de Y)**: Imprime `PLAY SPACE INVADERS` con la 'Y' invertida. Un alienígena camina desde `X=200`, retira la letra errónea y coloca la letra correcta.
  3. **Animación 2 (Eliminación de C)**: Imprime `INSERT CCOIN`. Un alienígena camina, dispara contra la primera 'C', destruyéndola y dejando `INSERT COIN`.
  4. **Modo Demostración**: Carga una secuencia fija de 11 comandos desde ROM para mover la nave e infligir disparos automáticos durante 30 segundos antes de reiniciar el ciclo.
* **Variables y Parámetros Involucrados**:
  * Tabla de comandos Demo ROM: `0x1A00`.
  * Temporizador de transición de pantalla: 600 cuadros (10 segundos).
* **Interacciones y Dependencias**: Si se inserta una moneda en cualquier punto, se interrumpe inmediatamente el modo atracción y se salta a `SYS-02` Estado 2.
* **Resultados y Cambios de Estado**: Renderizado continuo de gráficos promocionales en VRAM.
* **Casos Límite e Indocumentados**:
  * **Easter Egg "TAITO COP"**: Si durante la demostración se presionan simultáneamente los botones `2P Start` + `1P Fire` + `1P Left` + `1P Right`, y posteriormente se presiona `1P Start`, la rutina en `0x199A` escribe la cadena `TAITO COP` en la coordenada VRAM `0x2E1B`.

---

## 3. Control, Movimiento y Disparo del Cañón Láser (Jugador)

### PLR-01: Control de Movimiento Horizontal del Cañón Láser
* **Objetivo**: Procesar la entrada del jugador para desplazar horizontalmente la nave sobre la línea base del suelo.
* **Comportamiento Exacto**: Mueve el sprite de la nave a la izquierda o derecha respetando los límites laterales infranqueables.
* **Condiciones de Activación**: `SYS-02` en Estado 3 (Partida activa) e INT 1 atendida.
* **Entradas y Controles**:
  * Puerto 1 Bit 5: Botón Izquierda (`1` = Presionado).
  * Puerto 1 Bit 6: Botón Derecha (`1` = Presionado).
* **Reglas y Lógica de Ejecución**:
  1. Si Bit 5 = `1`: Resta 1 píxel a la posición X del jugador (`RAM 0x2014`).
  2. Si Bit 6 = `1`: Suma 1 píxel a la posición X del jugador (`RAM 0x2014`).
  3. **Límite Izquierdo**: Si `RAM 0x2014` < `0x10` (16 decimal), se fuerza `RAM 0x2014 = 0x10`.
  4. **Límite Derecho**: Si `RAM 0x2014` > `0xD8` (216 decimal), se fuerza `RAM 0x2014 = 0xD8`.
  5. Se borra el sprite en la posición previa y se renderiza el sprite de 16x8 píxeles en la nueva coordenada X, manteniendo la coordenada Y fija en `0x20` (32 decimal).
* **Variables y Parámetros Involucrados**:
  * RAM `0x2014`: Posición X del jugador (1 byte, límites `0x10` - `0xD8`).
  * Posición Y fija: `0x20` (Línea del suelo verde).
  * Ancho del sprite del Cañón: 16 píxeles (2 bytes).
  * Alto del sprite: 8 píxeles (1 byte).
* **Interacciones y Dependencias**: Proporciona las coordenadas de origen para el nacimiento del proyectil en `PLR-02`.
* **Resultados y Cambios de Estado**: Actualización del mapa de bits del jugador en VRAM.
* **Casos Límite y Excepciones**: Si se presionan ambos botones (Izquierda y Derecha) al mismo tiempo, el código procesa primero la comprobación de la izquierda y decrementa la posición X, anulando la lectura del botón derecho.

---

### PLR-02: Generación y Control del Proyectil del Jugador
* **Objetivo**: Crear, desplazar y gestionar el ciclo de vida del proyectil del Cañón Láser.
* **Comportamiento Exacto**: Permite la existencia de un único disparo del jugador en pantalla a la vez, desplazándose verticalmente hacia arriba.
* **Condiciones de Activación**: Presión del botón de disparo con el flag de disparo activo en estado inactivo.
* **Entradas y Controles**: Puerto 1 Bit 4: Botón de Disparo (`1` = Presionado).
* **Reglas y Lógica de Ejecución**:
  1. **Comprobación de Disponibilidad**: Se verifica `RAM 0x2025` (Flag de Disparo Activo). Si es igual a `1`, la solicitud de disparo es ignorada inmediatamente (cadencia restringida a un proyectil simultáneo).
  2. **Inicialización de Proyectil**:
     * Establece `RAM 0x2025 = 1`.
     * Asigna posición X de origen: `RAM 0x2026 = RAM 0x2014 + 8` (Centro de la nave).
     * Asigna posición Y de origen: `RAM 0x2027 = 0x28` (40 decimal, justo encima del cañón).
  3. **Bucle de Movimiento (INT 1)**:
     * En cada ciclo INT 1, se incrementa la posición Y (`RAM 0x2027`) en **4 píxeles hacia arriba**.
     * Se borra el gráfico previo del proyectil (línea vertical de 1x4 píxeles) y se dibuja en la nueva posición Y.
  4. **Límite Superior y Colisión**: Si `RAM 0x2027` >= `0xD8` (216 decimal, límite de la pantalla superior / franja roja del OVNI), el proyectil impacta contra el techo, genera un gráfico de explosión de disparo de 8x8 píxeles durante 2 cuadros y se destruye haciendo `RAM 0x2025 = 0`.
* **Variables y Parámetros Involucrados**:
  * RAM `0x2025`: Flag de disparo activo (`0` = Inactivo, `1` = En vuelo).
  * RAM `0x2026`: Posición X del proyectil (1 byte).
  * RAM `0x2027`: Posición Y del proyectil (1 byte).
  * Velocidad del proyectil: 4 píxeles por cuadro (240 píxeles/segundo).
* **Interacciones y Dependencias**: Dispara el efecto de sonido de disparo del jugador en el Puerto 3 Bit 1 (`SND-01`) e interactúa con el sistema de colisiones (`COL-01`).
* **Resultados y Cambios de Estado**: Renderizado del proyectil en VRAM.
* **Casos Límite y Excepciones**: Si el jugador mantiene presionado el botón de disparo, no se emitirán nuevos proyectiles hasta que el disparo actual sea destruido por impacto o límite de pantalla y el botón sea soltado/vuelto a presionar.

---

### PLR-03: Secuencia de Destrucción y Animación de Muerte del Jugador
* **Objetivo**: Ejecutar la secuencia de destrucción del Cañón Láser al ser impactado por un proyectil enemigo o invadido.
* **Comportamiento Exacto**: Congela la lógica general del juego, alterna dos sprites de explosión, decrementa las vidas y reinicia la posición.
* **Condiciones de Activación**: Detección de colisión positiva contra proyectil enemigo (`COL-01`) o invasión completada (`ALI-03`).
* **Entradas y Controles**: Señal de impacto desde el gestor de colisiones.
* **Reglas y Lógica de Ejecución**:
  1. Cambia el estado del juego a `SYS-02` Estado 4. Desactiva temporalmente el movimiento de los alienígenas y proyectiles.
  2. Dispara el efecto de sonido de explosión del jugador en el Puerto 3 Bit 2.
  3. Carga el temporizador de animación de muerte con 120 cuadros (2 segundos).
  4. Cada 8 cuadros, conmuta el sprite de la nave entre los mapas de bits `Player Explosion A` y `Player Explosion B` (efecto de fragmentación).
  5. Transcurridos los 2 segundos:
     * Decrementa en 1 el contador de vidas (`RAM 0x2002`).
     * Si Vidas > 0: Limpia los proyectiles en pantalla, reposiciona la nave en `X=0x10` e inicia un breve período de inmunidad de 1 segundo antes de volver a `SYS-02` Estado 3.
     * Si Vidas = 0: Transita inmediatamente al Estado de Game Over (`SYS-02` Estado 5).
* **Variables y Parámetros Involucrados**:
  * RAM `0x2002`: Contador de vidas del jugador.
  * RAM `0x2028`: Temporizador/Fase de animación de muerte.
  * Duración total de secuencia: 120 cuadros de video.
* **Interacciones y Dependencias**: Invocado por `COL-01`, modifica la interfaz visual de vidas (`UI-01`) y comunica el fin de partida a `RULE-01`.
* **Resultados y Cambios de Estado**: Reducción del stock de naves y eventual transición a Game Over.
* **Casos Límite y Excepciones**: Si la destrucción ocurre debido a una invasión alienígena (los invasores tocan el suelo), las vidas restantes se fuerzan automáticamente a 0, anulando cualquier vida de reserva.

---

## 4. Enemigos: Formación, Movimiento y Comportamiento de la Flota

### ALI-01: Estructura de la Matriz de Invasores y Tipos de Alien
* **Objetivo**: Gestionar la formación rectangular de 55 alienígenas divididos en 5 filas y 11 columnas.
* **Comportamiento Exacto**: Almacena el estado de existencia, tipo y cuadro de animación de cada uno de los 55 componentes de la flota.
* **Condiciones de Activación**: Inicio de una nueva oleada (`WAVE-01`).
* **Entradas y Controles**: Parámetros de la oleada actual.
* **Reglas y Lógica de Ejecución**:
  * La flota se compone de 5 filas horizontales x 11 columnas verticales (Total: 55 alienígenas):
    * **Fila 5 (Superior)**: 11 Alienígenas Tipo 3 (Pulpo / Squid) -> 30 Puntos. Tamaño: 8x8 píxeles.
    * **Filas 3 y 4 (Intermedias)**: 22 Alienígenas Tipo 2 (Cangrejo / Crab) -> 20 Puntos. Tamaño: 11x8 píxeles.
    * **Filas 1 y 2 (Inferiores)**: 22 Alienígenas Tipo 1 (Calamar / Jellyfish) -> 10 Puntos. Tamaño: 12x8 píxeles.
  * **Array de Estado en RAM**: Mapeado entre `0x2032` y `0x2068` (55 bytes contiguos).
    * Byte = `0x01`: Alienígena vivo.
    * Byte = `0x00`: Alienígena destruido.
* **Tabla de Especificaciones de Enemigos**:

| Fila | Tipo de Alien | Nombre | Ancho x Alto | Puntuación | Mapa de Bits Frame A | Mapa de Bits Frame B |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Fila 5** | Tipo 3 | Pulpo | 8 x 8 px | 30 Puntos | Brazo Abierto | Brazo Creado |
| **Fila 4** | Tipo 2 | Cangrejo | 11 x 8 px | 20 Puntos | Patas Abiertas | Patas Juntas |
| **Fila 3** | Tipo 2 | Cangrejo | 11 x 8 px | 20 Puntos | Patas Abiertas | Patas Juntas |
| **Fila 2** | Tipo 1 | Calamar | 12 x 8 px | 10 Puntos | Antenas Arriba | Antenas Abajo |
| **Fila 1** | Tipo 1 | Calamar | 12 x 8 px | 10 Puntos | Antenas Arriba | Antenas Abajo |

* **Variables y Parámetros Involucrados**:
  * Rango RAM de estado: `0x2032` - `0x2068`.
  * Contador de alienígenas vivos: `RAM 0x2030` (Valor inicial = 55 / `0x37`).
* **Interacciones y Dependencias**: Suministra datos a las rutinas de movimiento (`ALI-02`), renderizado (`ALI-04`) y colisiones (`COL-01`).
* **Resultados y Cambios de Estado**: Definición de la matriz inicial de invasores.
* **Casos Límite y Excepciones**: Ninguna. La estructura de la matriz es fija y consta inmutablemente de 5x11 elementos.

---

### ALI-02: Mecánica de Movimiento en Zig-Zag y Aceleración por Hardware
* **Objetivo**: Mover secuencialmente la flota en un patrón horizontal con descenso en los bordes, acelerando el ritmo a medida que disminuye el número de alienígenas.
* **Comportamiento Exacto**: La CPU procesa el movimiento de **UN SOLO ALIENÍGENA POR CUADRO DE VIDEO** (1/60 seg). Mover la flota completa de 55 alienígenas requiere 55 cuadros (0.916 segundos).
* **Condiciones de Activación**: Atendida la interrupción INT 2 (`0x10`).
* **Entradas y Controles**:
  * `RAM 0x202C`: Dirección del movimiento (`0` = Derecha, `1` = Izquierda).
  * `RAM 0x2031`: Índice del alienígena procesado en el cuadro actual (0 a 54).
* **Reglas y Lógica de Ejecución**:
  1. En cada cuadro de video, la rutina lee el índice de alienígena de `RAM 0x2031`.
  2. Si el alienígena marcado en `RAM [0x2032 + Índice]` está **muerto** (`0x00`), se salta instantáneamente al siguiente índice sin redibujar.
  3. Si el alienígena está **vivo** (`0x01`):
     * Borra el gráfico actual del alienígena en VRAM.
     * Si la flota se mueve a la Derecha: Suma la velocidad horizontal `DX` a su coordenada X.
     * Si la flota se mueve a la Izquierda: Resta la velocidad horizontal `DX` a su coordenada X.
     * Conmuta el frame de animación del alienígena (Frame A <-> Frame B).
     * Dibuja el nuevo mapa de bits en VRAM.
  4. **Comprobación de Bordes**: Al finalizar el barrido del alienígena número 54:
     * Si algún alienígena vivo ha alcanzado o superado el límite derecho (`X >= 0xD0` / 208 px) o el límite izquierdo (`X <= 0x08` / 8 px):
       * Se activa la bandera de descenso (`RAM 0x202E = 1`).
       * Se invierte la dirección en `RAM 0x202C`.
     * En la siguiente pasada completa de la flota, si la bandera de descenso está activa, **NO** se aplica movimiento horizontal; en su lugar, se desciende **TODA LA FLOTA 8 PÍXELES HACIA ABAJO** (`Y = Y - 8`) y se borra la bandera de descenso.
  5. **Mecanismo de Aceleración Orgánica**: Dado que los alienígenas muertos son omitidos en fracción de microsegundos, el tiempo requerido para completar un ciclo completo de movimiento de todos los alienígenas vivos disminuye linealmente:
     * Con 55 alienígenas vivos: 1 paso de flota cada 55 cuadros (~0.91 segundos).
     * Con 10 alienígenas vivos: 1 paso de flota cada 10 cuadros (~0.16 segundos).
     * Con 1 alienígena vivo: 1 paso de flota CADA CUADRO DE VIDEO (60 pasos por segundo).
* **Variables y Parámetros Involucrados**:
  * `RAM 0x202C`: Dirección de la flota (`0` = Derecha, `1` = Izquierda).
  * `RAM 0x202D`: Desplazamiento acumulado X de la flota.
  * `RAM 0x202E`: Flag de descenso vertical pendiente.
  * `RAM 0x2030`: Cantidad de alienígenas vivos (55 a 1).
  * `RAM 0x2031`: Apuntador de índice de procesamiento (0 a 54).
  * Desplazamiento horizontal estándar (`DX`): 2 píxeles por paso en oleada 1 (se incrementa en oleadas avanzadas).
  * Desplazamiento vertical en descenso (`DY`): 8 píxeles.
* **Interacciones y Dependencias**: Al completar cada paso global de la flota, dispara en orden cíclico una de las 4 notas del sonido de marcha alienígena en el Puerto 5 (`SND-01`).
* **Resultados y Cambios de Estado**: Desplazamiento progresivo e implacable de los invasores hacia la base del jugador.
* **Casos Límite y Excepciones**:
  * **Comportamiento del Último Invasor**: Cuando solo queda 1 alienígena vivo (`RAM 0x2030 = 1`), la velocidad de desplazamiento aumenta a 60 píxeles/segundo a la derecha o izquierda. Adicionalmente, el código incrementa el desplazamiento horizontal `DX` a 3 píxeles por paso para el último alienígena en ciertas oleadas.

---

### ALI-03: Condición de Invasión y Derrota Directa
* **Objetivo**: Detectar cuándo la línea inferior de la flota alcanza la altura del suelo o de los búnkeres, provocando la pérdida inmediata de la partida.
* **Comportamiento Exacto**: Verifica la coordenada Y de los alienígenas más bajos de la formación contra la coordenada Y crítica del Cañón Láser.
* **Condiciones de Activación**: Tras completar el descenso vertical de 8 píxeles de la flota (`ALI-02`).
* **Entradas y Controles**: Coordenada Y de los alienígenas de las filas inferiores.
* **Reglas y Lógica de Ejecución**:
  1. La altura base del suelo donde opera el Cañón Láser es `Y = 0x20` (32 decimal).
  2. Si durante la rutina de renderizado de cualquier alienígena se detecta que su coordenada `Y <= 0x28` (40 decimal, nivel de los búnkeres/techo del cañón):
     * Se activa inmediatamente la rutina de **Invasión Exitosa**.
     * Se borra el sprite del jugador.
     * Se establece el contador de vidas restantes en cero absoluto (`RAM 0x2002 = 0`).
     * Se detiene la marcha de la flota y se deshabilitan todos los disparos.
     * Un alienígena de la fila inferior realiza un gráfico especial pisando los restos del cañón.
     * Transita directamente a `SYS-02` Estado 5 (Game Over).
* **Variables y Parámetros Involucrados**:
  * Umbral de invasión `Y`: `0x28` (40 decimal).
  * RAM `0x2002`: Contador de vidas (forzado a `0x00`).
* **Interacciones y Dependencias**: Anula las vidas de reserva acumuladas y fuerza la llamada a `RULE-01`.
* **Resultados y Cambios de Estado**: Fin inmediato del juego independientemente del número de naves de reserva disponibles.
* **Casos Límite y Excepciones**: Ninguna. La invasión es una condición de derrota absoluta e insuperable.

---

### ALI-04: Eliminación y Gráfico de Explosión Alienígena
* **Objetivo**: Procesar la destrucción de un alienígena impactado, sumar puntuación y mostrar el sprite temporal de explosión.
* **Comportamiento Exacto**: Reemplaza el sprite del alien por una nube de fragmentos de 16x8 píxeles durante 16 cuadros de video.
* **Condiciones de Activación**: Notificación de colisión positiva desde `COL-01`.
* **Entradas y Controles**: Índice del alienígena impactado (0 a 54).
* **Reglas y Lógica de Ejecución**:
  1. Marca el estado del alienígena en el array como muerto: `RAM [0x2032 + Índice] = 0x00`.
  2. Decrementa en 1 el contador de alienígenas vivos: `RAM 0x2030 = RAM 0x2030 - 1`.
  3. Desplaza el gráfico de la explosión alienígena (sprite de 16x8 px) a la coordenada X,Y exacta del alien impactado.
  4. Suma los puntos correspondientes al marcador del jugador activo mediante `SCORE-01`.
  5. Activa el temporizador de explosión alienígena (`RAM 0x2029 = 16 cuadros`).
  6. Emite el sonido de muerte alienígena en el Puerto 3 Bit 3.
  7. Mientras el temporizador `0x2029` esté activo (> 0), la subrutina de la flota detiene momentáneamente el avance del siguiente alienígena para permitir que la explosión sea claramente visible en pantalla. Al llegar a 0, se borra el gráfico de explosión de la VRAM.
* **Variables y Parámetros Involucrados**:
  * RAM `0x2030`: Contador de alienígenas vivos.
  * RAM `0x2029`: Temporizador de renderizado de explosión (16 cuadros / ~0.26 segundos).
  * Sprite de explosión: 16x8 bits.
* **Interacciones y Dependencias**: Acelera la velocidad global de la flota (`ALI-02`) e incrementa el contador de disparos para el cálculo del OVNI (`UFO-02`).
* **Resultados y Cambios de Estado**: Remoción permanente del enemigo de la matriz de la oleada actual.
* **Casos Límite y Excepciones**: Si el proyectil destruye el último alienígena de la flota (`RAM 0x2030 == 0`), la secuencia de explosión se ejecuta normalmente y posteriormente se invoca el reinicio de oleada (`WAVE-01`).

---

## 5. Sistema de Disparos Enemigos y Tipos de Proyectiles

### AMMO-01: Arquitectura de Proyectiles Enemigos
* **Objetivo**: Gestionar hasta tres proyectiles enemigos simultáneos en pantalla, cada uno con características y comportamientos independientes.
* **Comportamiento Exacto**: Procesa tres tipos distintos de disparos enemigos: el proyectil *Plunger* (Émbolo), el proyectil *Squiggly* (Zig-Zag / Ondulado) y el proyectil *Rolling* (Rotatorio / Perseguidor).
* **Condiciones de Activación**: Bucle de interrupción INT 2 (`0x10`) en `SYS-02` Estado 3.
* **Entradas del Sistema**: Registros temporizadores e índices de columnas alienígenas.
* **Reglas y Lógica de Ejecución**:
  * Existen 3 estructuras independientes de datos de proyectiles en RAM:
    * **Proyectil 1 (Plunger)**: RAM `0x207B` - `0x208B`.
    * **Proyectil 2 (Squiggly)**: RAM `0x208C` - `0x209C`.
    * **Proyectil 3 (Rolling)**: RAM `0x209D` - `0x20AD`.
  * Cada estructura contiene:
    * Byte 0: Flag de Estado (`0` = Inactivo, `1` = En Vuelo, `2` = En Explosión).
    * Byte 1: Coordenada X actual.
    * Byte 2: Coordenada Y actual.
    * Byte 3: Puntero de frame de animación (Paso de gráfico de 3x7 píxeles).
    * Byte 4: Temporizador interno.
  * **Velocidad de Caída**: Todos los proyectiles enemigos descienden verticalmente hacia abajo a una tasa de **3 píxeles por cuadro de video** en oleadas iniciales (incrementándose a 4 píxeles/cuadro en oleadas superiores a la oleada 1).
* **Estructura de Datos de Proyectiles Enemigos**:

| Estructura RAM | Nombre de Proyectil | Dimensión | Patrón Visual | Algoritmo de Selección de Columna |
| :--- | :--- | :--- | :--- | :--- |
| `0x207B - 0x208B` | **Plunger Shot** | 3 x 7 px | Cruz / Émbolo | Fijo según tabla preprogramada en ROM |
| `0x208C - 0x209C` | **Squiggly Shot** | 3 x 7 px | Línea Ondulada | Basado en contador aleatorio de la CPU |
| `0x209D - 0x20AD` | **Rolling Shot** | 3 x 7 px | Barra Rotatoria | Sigue la posición X actual del Cañón Láser |

* **Variables y Parámetros Involucrados**:
  * Límite de disparos enemigos en pantalla: Máximo 3 activos en paralelo.
  * Coordenada Y de suelo (límite de fallo): `Y <= 0x20` (32 decimal).
* **Interacciones y Dependencias**: Interactúa con los búnkeres (`BNK-02`), el cañón del jugador (`PLR-03`) y el disparo del jugador (`COL-02`).
* **Resultados y Cambios de Estado**: Actualización de la posición de proyectiles hostiles en VRAM.
* **Casos Límite y Excepciones**: No puede haber dos proyectiles del mismo tipo volando simultáneamente. Cada tipo tiene asignado un canal de ejecución único.

---

### AMMO-02: Lógica de Recarga y Selección de Columnas para Disparo Enemigo
* **Objetivo**: Seleccionar la columna alienígena desde donde nacerá un nuevo disparo enemigo y controlar la cadencia de fuego.
* **Comportamiento Exacto**: Evalúa temporizadores de recarga y busca el alienígena vivo posicionado en la parte más baja de la columna elegida.
* **Condiciones de Activación**: Estructura de proyectil enemigo inactiva (Flag = 0) y temporizador de recarga agotado.
* **Entradas y Controles**: Posición X de la nave del jugador y contadores de ciclos.
* **Reglas y Lógica de Ejecución**:
  1. **Regla de Recarga (Reload Timer)**: Tras la destrucción de un disparo enemigo, debe transcurrir un intervalo de tiempo antes de rearmar el canal:
     * Si puntuación del jugador < 200 puntos: Intervalo de recarga = 48 cuadros.
     * Si puntuación entre 200 y 1000 puntos: Intervalo de recarga = 16 cuadros.
     * Si puntuación >= 1000 puntos: Intervalo de recarga = 8 cuadros.
  2. **Algoritmos de Selección de Columna**:
     * **Plunger Shot**: Lee un valor de columna (0 a 10) desde una tabla estática en ROM (`0x1A80`). Al disparar, avanza el puntero de la tabla.
     * **Squiggly Shot**: Utiliza el byte menos significativo del contador de ciclos de la CPU como índice aleatorio para seleccionar una columna de 0 a 10.
     * **Rolling Shot (Targeting System)**: Examina la coordenada X actual del Cañón Láser (`RAM 0x2014`). Busca la columna de alienígenas cuya posición X esté más cercana a la posición del jugador. Si la columna elegida tiene alienígenas vivos, el disparo *Rolling* nace exactamente sobre la cabeza del jugador.
  3. **Identificación del Emisor**: Una vez determinada la columna (0 a 10), el código recorre la columna desde la fila 1 (inferior) hacia la fila 5 (superior) hasta encontrar el **PRIMER ALIENÍGENA VIVO**.
  4. **Nacimiento del Proyectil**: Se crea el proyectil en la posición X de la columna y en la posición Y del borde inferior del alienígena emisor.
* **Variables y Parámetros Involucrados**:
  * Puntero de tabla ROM Plunger: `0x1A80` (16 bytes de columnas).
  * RAM `0x207A`: Temporizador general de recarga de disparo enemigo.
* **Interacciones y Dependencias**: Requiere la verificación del array de alineación de alienígenas vivos (`ALI-01`).
* **Resultados y Cambios de Estado**: Activación de proyectiles hostiles en vuelo.
* **Casos Límite y Excepciones**: Si una columna seleccionada no tiene ningún alienígena vivo (columna completamente destruida), la rutina aborta la generación del disparo en ese cuadro y reintenta en el siguiente ciclo probando la columna contigua.

---

### AMMO-03: Explosión e Impacto de Proyectiles Enemigos contra el Suelo
* **Objetivo**: Renderizar el gráfico de explosión del proyectil enemigo al impactar contra el suelo o límite inferior y liberar el canal de disparo.
* **Comportamiento Exacto**: Muestra un gráfico de explosión en forma de cruz/puntos durante 6 cuadros de video al tocar `Y = 0x20`.
* **Condiciones de Activación**: Proyectil enemigo alcanzando la coordenada `Y <= 0x20` (32 decimal).
* **Entradas y Controles**: Coordenada Y del proyectil.
* **Reglas y Lógica de Ejecución**:
  1. Al tocar la línea verde del suelo, se detiene el movimiento descendente.
  2. Borra el sprite del proyectil (3x7 px).
  3. Dibuja el sprite de `Shot Ground Explosion` (6x8 px) centrado en la coordenada X de impacto.
  4. Establece el estado del proyectil en Flag = 2 (En explosión) y carga un contador con 6 cuadros de video.
  5. Transcurridos los 6 cuadros: borra el gráfico de explosión de la VRAM y reinicia la estructura haciendo Flag = 0 (Inactivo), permitiendo a `AMMO-02` generar un nuevo disparo.
* **Variables y Parámetros Involucrados**:
  * Coordenada Y de suelo: `0x20` (32 decimal).
  * Temporizador de explosión de suelo: 6 cuadros (~0.1 segundos).
  * Gráfico de explosión: 6x8 píxeles.
* **Interacciones y Dependencias**: Borra permanentemente una pequeña sección de 1 ó 2 píxeles de la línea verde del suelo mediante la operación XOR de dibujado.
* **Resultados y Cambios de Estado**: Liberación del canal de proyectiles para el siguiente ciclo de disparo.
* **Casos Límite y Excepciones**: Ninguna.

---

## 6. Búnkeres Defensivos: Estructura, Destrucción y Degradación por Bits

### BNK-01: Arquitectura y Dibujo Inicial de los Búnkeres
* **Objetivo**: Renderizar los cuatro búnkeres o escudos defensivos en la VRAM al comenzar una nueva partida o nivel.
* **Comportamiento Exacto**: Construye cuatro estructuras verdes idénticas compuestas de mapas de bits de alta precisión en coordenadas fijas.
* **Condiciones de Activación**: Inicialización de partida o reinicio de oleada (`SYS-02` Estado 2).
* **Entradas y Controles**: Datos de mapa de bits almacenados en la ROM (`0x1C00`).
* **Reglas y Lógica de Ejecución**:
  * Se dibujan 4 búnkeres distribuidos equitativamente a lo largo del eje X de la pantalla.
  * Dimensiones de cada búnker: **22 píxeles de ancho por 16 píxeles de alto**.
  * Coordenada Y de la base de los búnkeres: `Y = 0x30` (48 decimal, justo por encima del suelo `0x20`).
  * Coordenadas X iniciales de los búnkeres:
    * **Búnker 1**: `X = 0x20` (32 decimal).
    * **Búnker 2**: `X = 0x58` (88 decimal).
    * **Búnker 3**: `X = 0x90` (144 decimal).
    * **Búnker 4**: `X = 0xC8` (200 decimal).
  * Geometría del Búnker: Posee una bóveda superior redondeada, paredes laterales sólidas y un arco hueco inferior en el centro (forma de túnel invertido).
* **Variables y Parámetros Involucrados**:
  * Puntero ROM de sprite de Búnker: `0x1C00` (44 bytes por búnker).
  * Color efectivo: Verde (por estar ubicados en la franja física del celofán inferior).
* **Interacciones y Dependencias**: Sirve como cobertura física destruible para `PLR-01` y barrera de absorción para `AMMO-01` y `ALI-01`.
* **Resultados y Cambios de Estado**: Escritura directa del mapa de bits de los 4 escudos en VRAM.
* **Casos Límite y Excepciones**: A diferencia de las naves y alienígenas, los búnkeres **NO** se gestionan como objetos/sprites individuales en RAM. Existen **únicamente como píxeles pintados en la VRAM**.

---

### BNK-02: Mecánica de Degradación Progresiva y Máscaras de Destrucción
* **Objetivo**: Esculpir huecos y daños reales en la estructura de los búnkeres ante cualquier impacto de proyectil propio o enemigo.
* **Comportamiento Exacto**: Al detectar una colisión contra un búnker, aplica una máscara de bits de erosión en la VRAM que destruye los píxeles en el punto de contacto.
* **Condiciones de Activación**: Colisión de un proyectil del jugador (`PLR-02`) o disparo enemigo (`AMMO-01`) contra píxeles encendidos en el rango Y de búnkeres (`0x30` - `0x40`).
* **Entradas y Controles**: Coordenadas X,Y de impacto del proyectil.
* **Reglas y Lógica de Ejecución**:
  1. **Impacto de Disparo del Jugador (Desde Abajo)**:
     * El proyectil del jugador viaja hacia arriba.
     * Al tocar un píxel de búnker, se detiene el proyectil y se destruye (`RAM 0x2025 = 0`).
     * Se aplica la **Máscara de Daño Superior/Jugador** (Matriz de bits de 6x8 píxeles con forma de abanico/explosión hacia arriba).
     * Se realiza una operación `AND NOT` entre el mapa de bits del búnker en VRAM y la máscara de daño, borrando permanentemente los píxeles de esa zona.
  2. **Impacto de Disparo Enemigo (Desde Arriba)**:
     * El proyectil enemigo desciende.
     * Al tocar un píxel de búnker, se borra el proyectil y se inicia su secuencia de explosión.
     * Se aplica la **Máscara de Daño Inferior/Enemiga** (Matriz de bits de 6x8 píxeles con forma de cráter de erosión hacia abajo).
     * Se realiza una operación `AND NOT` borrando los píxeles de contacto.
  3. **Erosión Acumulativa**: Dado que no existen barras de vida ni estados predefinidos para los búnkeres, cada impacto remueve píxeles de forma orgánica. Tras recibir múltiples impactos en la misma columna, se crea un canal/hueco completo a través del búnker, permitiendo que los proyectiles posteriores pasen libremente de lado a lado.
* **Variables y Parámetros Involucrados**:
  * Rango de coordenadas Y de búnkeres: `0x30` a `0x40` (48 a 64 decimal).
  * Máscara de daño de proyectil jugador: 6x8 bits (ROM `0x1C40`).
  * Máscara de daño de proyectil enemigo: 6x8 bits (ROM `0x1C48`).
* **Interacciones y Dependencias**: Utiliza el detector de colisiones por lectura de VRAM (`COL-01`).
* **Resultados y Cambios de Estado**: Alteración irreversible del gráfico de los escudos defensivos en la VRAM.
* **Casos Límite y Excepciones**: Los disparos que pasan a través de un hueco previamente abierto en un búnker no sufren colisión y continúan su trayectoria limpia.

---

### BNK-03: Destrucción Total por Colisión Directa con la Flota Alienígena
* **Objetivo**: Eliminar instantáneamente las secciones de los búnkeres que son traslapadas por los alienígenas a medida que la flota desciende.
* **Comportamiento Exacto**: Cuando el borde inferior de los alienígenas alcanza `Y <= 0x40`, cualquier píxel de búnker ocupado por el sprite del alien es borrado de VRAM.
* **Condiciones de Activación**: Desplazamiento de un alienígena (`ALI-02`) en el rango de coordenadas `Y` entre `0x30` y `0x40`.
* **Entradas y Controles**: Coordenadas del sprite del alienígena en proceso de renderizado.
* **Reglas y Lógica de Ejecución**:
  1. Durante el paso de dibujo de cada alienígena, la rutina de renderizado escribe el mapa de bits del alienígena sobre la VRAM.
  2. Si la posición Y del alien se cruza con la zona de los búnkeres, el gráfico del alienígena (que se dibuja borrando primero su caja de delimitación previa) sobreescribe y borra todos los píxeles de búnker que encuentre a su paso.
  3. Al avanzar la flota horizontalmente sobre los búnkeres, los alienígenas "devoran" la parte superior de las estructuras, borrando capas completas de 8 píxeles de alto en cada pasada.
* **Variables y Parámetros Involucrados**:
  * Altura de intersección: `Y = 0x40` (64 decimal).
* **Interacciones y Dependencias**: Subproducto directo del renderizado de alienígenas (`ALI-02`) mediante sobrescritura de VRAM.
* **Resultados y Cambios de Estado**: Aniquilación total y no reparable de los búnkeres cuando la flota desciende a niveles bajos.
* **Casos Límite y Excepciones**: Ninguna. Los búnkeres no ofrecen resistencia física ni frenan el avance descendente de los alienígenas.

---

## 7. Platillo Volante (UFO / Mystery Ship)

### UFO-01: Aparición, Dirección y Rutina de Movimiento del Platillo
* **Objetivo**: Gestionar la entrada fortuita, el vuelo horizontal continuo y la salida del Platillo Volante bonus en la parte superior de la pantalla.
* **Comportamiento Exacto**: Una nave de 16x7 píxeles cruza la franja roja superior de la pantalla de un extremo a otro emitiendo un sonido oscilante.
* **Condiciones de Activación**: Temporizador de spawn del UFO agotado y número de alienígenas vivos >= 8.
* **Entradas y Controles**: Contador de cuadros del sistema y tabla de temporización.
* **Reglas y Lógica de Ejecución**:
  1. **Condición de Spawn**: El UFO sólo se activa si hay **al menos 8 alienígenas vivos** en la flota (`RAM 0x2030 >= 8`). Si quedan menos de 8 alienígenas, el UFO no volverá a aparecer en la oleada actual.
  2. **Temporizador de Aparición**: Transcurridos aproximadamente **600 cuadros de video** (~10 segundos de juego activo) desde el inicio del nivel o desde la desaparición del UFO anterior, se activa el flag de presencia del UFO (`RAM 0x2070 = 1`).
  3. **Dirección de Vuelo**:
     * En oleadas impares (Oleada 1, 3, 5...): El UFO aparece por la izquierda (`X = 0x08`) y vuela hacia la derecha (`DX = +2 píxeles/paso`).
     * En oleadas pares (Oleada 2, 4, 6...): El UFO aparece por la derecha (`X = 0xD0`) y vuela hacia la izquierda (`DX = -2 píxeles/paso`).
  4. **Coordenada Y Fija**: Se desplaza inalterablemente en la coordenada `Y = 0xD8` (216 decimal, dentro de la franja roja superior de celofán).
  5. **Salida de Pantalla**: Si el UFO alcanza el extremo opuesto (`X < 0x08` o `X > 0xD0`), se desactiva (`RAM 0x2070 = 0`), se silencia su sonido y se reinicia el temporizador de 600 cuadros.
* **Variables y Parámetros Involucrados**:
  * RAM `0x2070`: Flag de presencia del UFO (`0` = Inactivo, `1` = En pantalla).
  * RAM `0x2071`: Posición X del UFO (1 byte).
  * Coordenada Y fija: `0xD8` (216 decimal).
  * Velocidad del UFO: 2 píxeles por paso (procesa 1 paso cada 2 cuadros de video -> 60 píxeles/segundo).
* **Interacciones y Dependencias**: Activa el bucle de sonido de alta frecuencia del UFO en el Puerto 3 Bit 0 (`SND-01`).
* **Resultados y Cambios de Estado**: Presencia de la diana de mayor puntuación en pantalla.
* **Casos Límite y Excepciones**: Si el proyectil del jugador destruye al UFO, su sonido se interrumpe al instante y es reemplazado por el efecto de sonido de impacto UFO en el Puerto 5 Bit 4 (`SND-01`).

---

### UFO-02: Algoritmo Determinista de Puntuación (Mecánica del Disparo 23)
* **Objetivo**: Calcular el valor exacto de recompensa (50, 100, 150 o 300 puntos) otorgado al destruir el Platillo Volante.
* **Comportamiento Exacto**: La puntuación **NO ES ALEATORIA**. Se obtiene consultando una tabla circular de 16 valores apuntada por un registro que se incrementa con **CADA DISPARO EFECTUADO POR EL JUGADOR**.
* **Condiciones de Activación**: Detección de colisión del proyectil del jugador contra el UFO (`COL-01`).
* **Entradas y Controles**: Contador global de disparos del jugador (`RAM 0x2074`).
* **Reglas y Lógica de Ejecución**:
  1. Cada vez que el jugador presiona el botón de disparo y nace un proyectil (`PLR-02`), el juego incrementa en 1 el contador interno de disparos: `RAM 0x2074 = (RAM 0x2074 + 1) MOD 16`.
  2. Al impactar exitosamente sobre el UFO, la rutina de destrucción utiliza el valor actual de `RAM 0x2074` (de 0 a 15) como índice para consultar la **Tabla Rom de Puntuaciones UFO**:

| Índice `0x2074` | Disparo en Oleada | Puntuación Otorgada |
| :---: | :---: | :---: |
| **0** | Disparo 1 | 100 Puntos |
| **1** | Disparo 2 | 50 Puntos |
| **2** | Disparo 3 | 50 Puntos |
| **3** | Disparo 4 | 100 Puntos |
| **4** | Disparo 5 | 150 Puntos |
| **5** | Disparo 6 | 100 Puntos |
| **6** | Disparo 7 | 100 Puntos |
| **7** | Disparo 8 | 50 Puntos |
| **8** | Disparo 9 | 300 Puntos |
| **9** | Disparo 10 | 100 Puntos |
| **10** | Disparo 11 | 100 Puntos |
| **11** | Disparo 12 | 100 Puntos |
| **12** | Disparo 13 | 50 Puntos |
| **13** | Disparo 14 | 150 Puntos |
| **14** | Disparo 15 | 100 Puntos |
| **15** | Disparo 16 | 50 Puntos |

  3. **Estrategia del Disparo 23 (Módulo 15 Exploit)**:
     * Debido a que el primer UFO aparece exactamente cuando han transcurrido los primeros movimientos de la partida, si el jugador efectúa exactamente **22 disparos previos** (impacten o fallen) y destruye el UFO con el **disparo número 23**, el contador `0x2074` estará en el índice `8`, garantizando matemáticamente la puntuación máxima de **300 PUNTOS**.
     * Subsecuentemente, efectuar **15 disparos adicionales** (disparo 38, 53, 68...) volverá a alinearse con el índice `8`, otorgando 300 puntos de forma determinista e ininterrumpida.
* **Variables y Parámetros Involucrados**:
  * RAM `0x2074`: Contador circular de disparos del jugador (0 a 15).
  * Tabla ROM de Puntuaciones UFO: Dirección `0x1D50`.
* **Interacciones y Dependencias**: Suma el valor resultante al marcador mediante `SCORE-01` y dibuja el texto numérico temporal de la recompensa (`50`, `100`, `150` o `300`) sobre la VRAM durante 24 cuadros de video.
* **Resultados y Cambios de Estado**: Otorgamiento riguroso y verificable de la bonificación.
* **Casos Límite y Excepciones**: Si el contador `0x2074` se corrompe o se reinicia por cambio de vida, el ciclo recomienza desde el índice 0.

---

## 8. Sistema de Puntuación, Vidas, Récords y Bonificaciones

### SCORE-01: Aritmética BCD de Puntuación y Marcadores
* **Objetivo**: Actualizar la puntuación del jugador activo en codificación BCD (Decimal Codificado en Binario) y comparar contra el récord máximo.
* **Comportamiento Exacto**: Incrementa los marcadores numéricos representados por palabras de 16 bits sin utilizar formato binario estándar.
* **Condiciones de Activación**: Destrucción de un alienígena (`ALI-04`) o Platillo Volante (`UFO-02`).
* **Entradas y Controles**: Valor numérico del objetivo destruido (10, 20, 30, 50, 100, 150, 300).
* **Reglas y Lógica de Ejecución**:
  1. Identifica el jugador activo mediante `RAM 0x2001` (`0` = Jugador 1, `1` = Jugador 2).
  2. Apunta a la variable de puntuación correspondiente:
     * Jugador 1: RAM `0x2008` - `0x2009` (2 bytes BCD).
     * Jugador 2: RAM `0x200A` - `0x200B` (2 bytes BCD).
  3. Ejecuta la adición en ensamblador 8080 utilizando las instrucciones `ADD` seguidas de `DAA` (*Decimal Adjust Accumulator*) para mantener la validez BCD (ej. `0x09 + 0x01 = 0x10` en lugar de `0x0A`).
  4. **Comprobación de Récord (HI-SCORE)**:
     * Compara la puntuación del jugador activo contra la variable High Score (`RAM 0x200C` - `0x200D`).
     * Si Puntuación Activa > High Score: Copia la puntuación activa a High Score y actualiza inmediatamente los dígitos en la cabecera central de la VRAM (`UI-02`).
* **Variables y Parámetros Involucrados**:
  * RAM `0x2008` - `0x2009`: Puntuación Jugador 1 (Max: 9990 en BCD visual, desborda a 0000 internamente si pasa de 99,990).
  * RAM `0x200A` - `0x200B`: Puntuación Jugador 2.
  * RAM `0x200C` - `0x200D`: Máxima Puntuación Global (High Score).
* **Interacciones y Dependencias**: Evalúa la concesión de naves extra en `SCORE-02`.
* **Resultados y Cambios de Estado**: Renderizado inmediato de los nuevos dígitos en VRAM.
* **Casos Límite y Excepciones**: Las puntuaciones de *Space Invaders* siempre terminan en el dígito `0` (el byte inferior solo almacena decenas y centenas).

---

### SCORE-02: Gestión de Vidas Extras y Bonificaciones
* **Objetivo**: Conceder una nave de reserva adicional al alcanzar el umbral de puntos configurado por los DIP switches.
* **Comportamiento Exacto**: Incrementa el contador de vidas en 1 y emite un tono auditivo especial de bonificación al cruzar la meta de puntos.
* **Condiciones de Activación**: Actualización de puntuación en `SCORE-01`.
* **Entradas y Controles**:
  * DIP Switch 3: Umbral de Puntuación (`0` = 1,000 puntos, `1` = 1,500 puntos).
  * `RAM 0x2005`: Flag de Vida Extra ya concedida (`0` = Pendiente, `1` = Ya entregada).
* **Reglas y Lógica de Ejecución**:
  1. Si `RAM 0x2005 == 1`: Salta la comprobación (sólo se concede **UNA SOLA VIDA EXTRA POR PARTIDA**).
  2. Si `RAM 0x2005 == 0`:
     * Lee la puntuación actual BCD.
     * Si DIP SW3 = `0` y Puntuación >= 1,000 puntos (`0x1000` BCD):
       * Suma 1 a las vidas restantes (`RAM 0x2002`).
       * Establece `RAM 0x2005 = 1`.
       * Dispara el pulso de sonido de Vida Extra en Puerto 3 Bit 4 (`SND-01`).
       * Redibuja los iconos del Cañón Láser en la barra de interfaz inferior.
     * Si DIP SW3 = `1` y Puntuación >= 1,500 puntos (`0x1500` BCD):
       * Ejecuta exactamente el mismo proceso de concesión.
* **Variables y Parámetros Involucrados**:
  * RAM `0x2002`: Contador de vidas del jugador.
  * RAM `0x2005`: Flag de control de bonificación de vida extra (1 byte).
  * Máximo de vidas representables en la interfaz: 6 naves.
* **Interacciones y Dependencias**: Modifica las reservas de supervivencia de `PLR-03`.
* **Resultados y Cambios de Estado**: Incremento del stock de naves y emisión del sonido de premio.
* **Casos Límite y Excepciones**: Si el jugador ya posee 6 vidas (límite máximo visual), la vida extra se otorga internamente en la variable RAM pero no se dibuja un icono adicional para evitar sobreescribir la leyenda del texto `CREDIT`.

---

## 9. Progresión de Dificultad, Oleadas y Reglas de Nivel

### WAVE-01: Progresión de Oleadas y Variación de la Altura Inicial
* **Objetivo**: Configurar los parámetros de inicio de cada nueva oleada alienígena, incrementando la agresividad y reduciendo la distancia al suelo.
* **Comportamiento Exacto**: Al exterminar los 55 alienígenas de un nivel, se incrementa el contador de oleada y se reinicia la matriz con una coordenada Y de origen más baja.
* **Condiciones de Activación**: Destrucción del último alienígena de la oleada (`RAM 0x2030 == 0`).
* **Entradas y Controles**: Contador global de oleadas (`RAM 0x2006`).
* **Reglas y Lógica de Ejecución**:
  1. Incrementa en 1 el número de oleada actual: `RAM 0x2006 = RAM 0x2006 + 1`.
  2. Restablece la matriz de 55 alienígenas vivos (`RAM 0x2032` - `0x2068 = 0x01`).
  3. **Tabla de Altura Inicial de Spawn de la Flota**:
     * **Oleada 1**: Coordenada Y de inicio = `0x78` (120 decimal).
     * **Oleada 2**: Coordenada Y de inicio = `0x68` (104 decimal) -> La flota nace 16 píxeles más abajo.
     * **Oleada 3**: Coordenada Y de inicio = `0x60` (96 decimal).
     * **Oleada 4**: Coordenada Y de inicio = `0x58` (88 decimal).
     * **Oleada 5**: Coordenada Y de inicio = `0x50` (80 decimal).
     * **Oleada 6**: Coordenada Y de inicio = `0x48` (72 decimal).
     * **Oleada 7 a 9**: Coordenada Y de inicio = `0x48` (72 decimal, altura mínima de seguridad para no sobreescribir búnkeres).
     * **Oleada 10 en adelante**: El ciclo de alturas se reinicia idéntico al patrón de la Oleada 2 (`0x68`).
  4. Redibuja los 4 búnkeres limpios sin daños en la VRAM (`BNK-01`).
  5. Reinicia los temporizadores de proyectiles y el contador de disparos del UFO.
* **Variables y Parámetros Involucrados**:
  * RAM `0x2006`: Contador de Oleada/Nivel (1 a 255).
  * RAM `0x202E`: Posición Y base de la flota.
* **Interacciones y Dependencias**: Determina el tiempo disponible antes de que la flota alcance la zona de invasión (`ALI-03`).
* **Resultados y Cambios de Estado**: Incremento directo de la presión de juego sobre el usuario.
* **Casos Límite y Excepciones**: A partir de la Oleada 2, la proximidad inicial de los alienígenas a los búnkeres provoca que los disparos enemigos alcancen la base del jugador en una fracción del tiempo de la primera oleada.

---

### WAVE-02: Incremento de Velocidad de Proyectiles Hostiles por Nivel
* **Objetivo**: Aumentar la velocidad de caída de los disparos enemigos a medida que se superan niveles.
* **Comportamiento Exacto**: Modifica el delta de desplazamiento vertical (`DY`) de los proyectiles enemigos según el número de oleada activa.
* **Condiciones de Activación**: Carga de nueva oleada (`WAVE-01`).
* **Entradas y Controles**: Variable `RAM 0x2006`.
* **Reglas y Lógica de Ejecución**:
  * **Oleada 1**: Velocidad de caída de proyectiles enemigos = **3 píxeles por cuadro** (180 px/sec).
  * **Oleada 2 en adelante**: Velocidad de caída de proyectiles enemigos = **4 píxeles por cuadro** (240 px/sec).
* **Variables y Parámetros Involucrados**:
  * Constante `DY` de proyectil enemigo: 3 ó 4 píxeles.
* **Interacciones y Dependencias**: Alimenta la rutina de actualización de proyectiles (`AMMO-01`).
* **Resultados y Cambios de Estado**: Reducción del margen de reacción del jugador para esquivar disparos.
* **Casos Límite y Excepciones**: La velocidad del disparo del jugador no se modifica jamás (se mantiene fija inmutablemente en 4 píxeles/cuadro hacia arriba).

---

## 10. Sistema de Detección de Colisiones e Impactos

### COL-01: Detección de Colisión por Inspección Directa de VRAM (Hardware Read)
* **Objetivo**: Detectar contactos entre objetos analizando los bits encendidos en la VRAM antes de sobreescribir gráficos.
* **Comportamiento Exacto**: En lugar de utilizar hitboxes rectangulares matemáticos, el motor de *Space Invaders* comprueba si los píxeles de la VRAM donde se dibujará un sprite ya están encendidos (`!= 0x00`).
* **Condiciones de Activación**: Fase de dibujado de cualquier proyectil o sprite en INT 1 / INT 2.
* **Entradas y Controles**: Coordenadas de byte y desplazamientos de bit objetivo en VRAM.
* **Reglas y Lógica de Ejecución**:
  1. Antes de mover un proyectil a su nueva posición `(X, Y_nueva)`, el código calcula la dirección de memoria VRAM equivalente (`0x2400 + X*32 + Y/8`).
  2. Lee el byte presente en esa dirección VRAM.
  3. Se aplica una máscara de bits equivalente al patrón gráfico del objeto a dibujar.
  4. Si el resultado de la operación `(Byte_VRAM AND Mascara)` **NO ES CERO** (`!= 0x00`), significa que hay píxeles sólidos pertenecientes a otro objeto ocupando ese espacio exacto: **COLISIÓN DETECTADA**.
  5. **Branch de Identificación del Objeto Colisionado**:
     * Si la coordenada Y está en franja `Y >= 0xD0` (Zona superior) -> Colisión con **UFO** (`UFO-02`).
     * Si la coordenada Y está en franja `0x40 <= Y < 0xD0` -> Colisión con **Alienígena** (`ALI-04`).
     * Si la coordenada Y está en franja `0x30 <= Y < 0x40` -> Colisión con **Búnker** (`BNK-02`).
     * Si la coordenada Y está en franja `Y <= 0x28` -> Colisión con **Cañón Láser** (`PLR-03`).
* **Variables y Parámetros Involucrados**:
  * Puntero de lectura de VRAM.
  * Operadores lógicos de CPU 8080 (`ANA`, `ORA`).
* **Interacciones y Dependencias**: Determina el desencadenamiento de todas las rutinas de destrucción del juego.
* **Resultados y Cambios de Estado**: Identificación de impactos a nivel de píxel perfecto.
* **Casos Límite y Excepciones**:
  * **Glitch de Colisión de Bordes de Búnker**: Debido a que la detección lee VRAM abierta, si dos proyectiles (uno propio y uno enemigo) se cruzan en el mismo byte de VRAM exacto durante la misma interrupción, ambos se destruyen mutuamente sin tocar los búnkeres.

---

### COL-02: Colisión Proyectil contra Proyectil (Anulación Mutua)
* **Objetivo**: Cancelar y destruir dos disparos cuando el proyectil del jugador y un proyectil enemigo chorgan frontalmente.
* **Comportamiento Exacto**: Si el proyectil del jugador impacta contra el cuerpo de un proyectil enemigo en vuelo, ambos se borran y se cancelan.
* **Condiciones de Activación**: Proyectil del jugador ascendiendo y proyectil enemigo descendiendo en la misma columna X o adyacente (+/- 1 píxel).
* **Entradas y Controles**: Estructuras RAM de proyectiles (`RAM 0x2025` y `RAM 0x207B-0x20AD`).
* **Reglas y Lógica de Ejecución**:
  1. Durante la actualización de movimiento de `PLR-02`, se compara la coordenada X del proyectil del jugador contra las coordenadas X de los 3 proyectiles enemigos activos.
  2. Si `|X_jugador - X_enemigo| <= 1` píxel Y la coordenada `Y_jugador` cruza o supera la coordenada `Y_enemigo`:
     * Se detiene el movimiento de ambos proyectiles.
     * Se genera un sprite temporal de "Explosión de Disparo" (4x4 píxeles) en el punto medio de la colisión.
     * Se restablece `RAM 0x2025 = 0` (Disparo jugador disponible).
     * Se activa el estado de explosión del proyectil enemigo involucrado (`Flag = 2`).
     * No se otorgan puntos a ningún marcador.
* **Variables y Parámetros Involucrados**:
  * Umbral de tolerancia X: 1 píxel.
  * Duración del gráfico de anulación: 4 cuadros.
* **Interacciones y Dependencias**: Libera simultáneamente los canales de disparo de la nave del jugador y de la flota.
* **Resultados y Cambios de Estado**: Neutralización mutua de proyectiles.
* **Casos Límite y Excepciones**: Un único disparo del jugador solo puede anular **UN** disparo enemigo a la vez.

---

## 11. Condiciones de Victoria, Derrota, Game Over y Reinicio

### RULE-01: Secuencia de Game Over y Procesamiento de Fin de Partida
* **Objetivo**: Concluir formalmente la partida activa al agotarse las vidas o sufrir una invasión, mostrando mensajes y actualizando récords.
* **Comportamiento Exacto**: Imprime la leyenda `GAME OVER` en el centro de la pantalla, guarda la puntuación máxima y regresa al modo atracción.
* **Condiciones de Activación**: Contador de vidas `RAM 0x2002 == 0` o invasión completada (`ALI-03`).
* **Entradas y Controles**: Sistema de control de vidas de `PLR-03`.
* **Reglas y Lógica de Ejecución**:
  1. Invocada la condición de fin de juego, se establece `SYS-02` en Estado 5.
  2. Desactiva la lectura de controles del panel de juego.
  3. Imprime la cadena de texto `GAME OVER` en caracteres de 8x8 píxeles en la coordenada VRAM central `0x290A`.
  4. Mantiene el mensaje `GAME OVER` estático en pantalla durante **180 cuadros de video** (~3 segundos).
  5. Descuenta el crédito consumido si no se había descontado previamente.
  6. Comprueba si la puntuación final supera la cifra almacenada en High Score (`RAM 0x200C`) y persiste el nuevo récord en RAM.
  7. Transcurridos los 3 segundos: limpia la VRAM completa escribiendo `0x00` desde `0x2400` a `0x3FFF` y conmuta `SYS-02` a Estado 1 (Modo Atracción).
* **Variables y Parámetros Involucrados**:
  * Temporizador de Game Over: 180 cuadros (3 segundos).
  * Texto: `GAME OVER` (9 caracteres).
* **Interacciones y Dependencias**: Cierra la sesión de juego activa e invoca a `UI-03`.
* **Resultados y Cambios de Estado**: Restablecimiento del arcade al modo de espera de clientes.
* **Casos Límite y Excepciones**: Si en una partida de 2 jugadores (2 Players) uno de los jugadores sufre Game Over pero el otro aún posee vidas, la partida no finaliza: se conmuta el turno al jugador superviviente mediante `RULE-02`.

---

### RULE-02: Alternancia de Turnos en Modo Dos Jugadores (2 Players Start)
* **Objetivo**: Administrar la alternancia ordenada de turnos entre el Jugador 1 y el Jugador 2 en muebles con un solo conjunto de controles compartido.
* **Comportamiento Exacto**: Al perder una nave el jugador activo, se guarda su estado en RAM y se restaura el mapa de pantalla del otro jugador.
* **Condiciones de Activación**: Muerte del Cañón Láser (`PLR-03`) en una partida iniciada con `2 Players Start` (2 Créditos consumidos).
* **Entradas y Controles**: Flag de modo dos jugadores (`RAM 0x2007 = 1`).
* **Reglas del Sistema**:
  1. Al finalizar la secuencia de explosión de la nave del Jugador 1:
     * Si el Jugador 2 conserva vidas (`RAM 0x2003 > 0`):
       * Guarda la posición de la flota, alienígenas vivos y estado de búnkeres del Jugador 1 en un área de reserva en RAM (`0x2100` - `0x21FF`).
       * Establece `RAM 0x2001 = 1` (Jugador 2 activo).
       * Carga el estado previamente guardado de la flota y búnkeres del Jugador 2 desde RAM (`0x2200` - `0x22FF`).
       * Redibuja la pantalla VRAM completa para reflejar el estado exacto en que el Jugador 2 dejó su oleada.
       * Muestra el texto flotante `PLAYER 2` durante 120 cuadros antes de entregar el control.
  2. El proceso se repite inversamente cuando el Jugador 2 pierde una nave, regresando al Jugador 1.
* **Variables y Parámetros Involucrados**:
  * RAM `0x2001`: Jugador activo (`0` = P1, `1` = P2).
  * RAM `0x2100` - `0x22FF`: Buffers de intercambio de estado de VRAM/Variables de P1 y P2.
* **Interacciones y Dependencias**: Inter e intra-cambio completo del contexto de la memoria del juego.
* **Resultados y Cambios de Estado**: Alternancia transparente de la sesión de juego entre dos personas.
* **Casos Límite y Excepciones**: Cada jugador mantiene su propio nivel de oleada, posición de flota, estado de degradación de búnkeres y puntuación de forma totalmente independiente.

---

## 12. Temporizadores, Eventos y Lógica de Hardware Dedicado

### TIMER-01: Generación de Reloj y Contadores de Cuadro
* **Objetivo**: Proveer una base de tiempo unificada basada en conteo de cuadros de video para todos los eventos temporizados.
* **Comportamiento Exacto**: Decrementa contadores numéricos de 8 y 16 bits registrados en la RAM en cada llamada de interrupción.
* **Condiciones de Activación**: Atendida interrupción INT 1 o INT 2.
* **Entradas y Controles**: Frecuencia del oscilador de cristal de la placa de video.
* **Reglas y Lógica de Ejecución**:
  * Un segundo de tiempo real equivale inmutablemente a **60 ticks** de temporizador.
  * **Tabla de Temporizadores Principales del Software**:

| Variable RAM | Nombre del Temporizador | Valor Inicial | Duración en Tiempo Real | Evento Gatillado al Llegar a Cero |
| :--- | :--- | :--- | :--- | :--- |
| `RAM 0x2028` | **Player Death Timer** | 120 ticks | 2.0 Segundos | Restablece nave o gatilla Game Over |
| `RAM 0x2029` | **Alien Explosion Timer** | 16 ticks | 0.26 Segundos | Borra sprite de explosión alienígena de VRAM |
| `RAM 0x2073` | **UFO Spawn Timer** | 600 ticks | 10.0 Segundos | Activa aparición del Platillo Volante |
| `RAM 0x207A` | **Enemy Reload Timer** | 8/16/48 ticks | 0.13 a 0.8 Sec | Habilita generación de nuevo disparo enemigo |
| `RAM 0x208E` | **Shot Explosion Timer** | 6 ticks | 0.10 Segundos | Borra animación de impacto contra el suelo |

* **Variables y Parámetros Involucrados**:
  * Registros de decremetación en RAM (`DEC` en ensamblador 8080).
* **Interacciones y Dependencias**: Sincroniza la totalidad de animaciones y eventos de la jugabilidad.
* **Resultados y Cambios de Estado**: Avance uniforme del tiempo en la simulación.
* **Casos Límite y Excepciones**: Ninguna.

---

### TIMER-02: Interfaz del Registro de Desplazamiento Fujitsu MB14241
* **Objetivo**: Manipular el hardware externo desplazador de barril de 16 bits para acelerar la transferencia de mapas de bits a VRAM.
* **Comportamiento Exacto**: Recibe dos bytes contiguos, aplica un desplazamiento de 0 a 7 bits según lo ordenado en el puerto de control y devuelve el byte resultante instantáneamente.
* **Condiciones de Activación**: Solicitud de renderizado de sprites en coordenadas X no alineadas con límites de byte (múltiplos de 8).
* **Entradas y Controles**:
  * Puerto de Salida `0x02`: Desplazamiento deseado (3 bits, valores 0 a 7).
  * Puerto de Salida `0x04`: Registro de datos de entrada (escritura consecutiva de 2 bytes).
  * Puerto de Entrada `0x03`: Lectura del dato desplazado.
* **Reglas y Lógica de Ejecución**:
  1. Para dibujar un sprite en una coordenada donde el píxel X no cae en un borde exacto de 8 bits:
  2. La CPU calcula el residuo: `Offset = X MOD 8`.
  3. Escribe `Offset` en el Puerto de Salida `0x02` (`OUT 0x02`).
  4. Escribe el primer byte del sprite en el Puerto de Salida `0x04` (`OUT 0x04`).
  5. Escribe el segundo byte del sprite (o `0x00`) en el Puerto de Salida `0x04` (`OUT 0x04`).
  6. La CPU ejecuta inmediatamente una lectura del Puerto de Entrada `0x03` (`IN 0x03`).
  7. El hardware MB14241 entrega la combinación de los dos bytes desplazada exactamente `Offset` bits a la derecha en 0 ciclos de reloj de CPU.
  8. La CPU escribe el byte devuelto directamente en VRAM mediante operación `OR` o `XOR`.
* **Variables y Parámetros Involucrados**:
  * Puertos I/O: `0x02` (Control), `0x04` (Data In), `0x03` (Data Out).
  * Hardware: Circuito integrado dedicado Fujitsu MB14241.
* **Interacciones y Dependencias**: Utilizado exclusivamente por la subrutina de dibujo de sprites genérica (`0x1400`).
* **Resultados y Cambios de Estado**: Renderizado suave a nivel de píxel sin sobrecargar la CPU Intel 8080.
* **Casos Límite y Excepciones**: Si el programador no escribe los dos bytes en el Puerto `0x04` en el orden correcto (LSB primero, MSB después), el registro de desplazamiento devuelve basura gráfica, corrompiendo el sprite en pantalla.

---

## 13. Sistema de Sonido Analógico Discreto y Efectos de Audio

### SND-01: Asignación de Puertos I/O y Disparo de Circuitos de Sonido
* **Objetivo**: Generar los efectos de audio y la música de marcha mediante el envío de pulsos de control a circuitos analógicos dedicados.
* **Comportamiento Exacto**: *Space Invaders* **NO** posee un procesador de sonido programable (PSG) ni reproduce muestras digitales (PCM). Cada sonido es producido por un circuito analógico discreto especializado (basado en amplificadores operacionales, transistores y el chip de efectos SN76477) montado en la placa de sonido.
* **Condiciones de Activación**: Eventos del juego que requieren retroalimentación auditiva.
* **Entradas y Controles**: Puerto de Salida `0x03` (Sonidos de Acción) y Puerto de Salida `0x05` (Sonidos de Marcha y UFO).
* **Reglas y Lógica de Ejecución**:
  * Para activar un sonido, la CPU escribe un byte en el Puerto `0x03` o `0x05` estableciendo el bit correspondiente en `1`. Para detener el sonido, se escribe el bit en `0`.
  * **Mapeo Completo de Bits de los Puertos de Sonido**:

| Puerto I/O | Bit | Nombre del Efecto de Audio | Circuito Analógico Asociado | Tipo de Señal |
| :--- | :--- | :--- | :--- | :--- |
| **Puerto 0x03** | **Bit 0** | UFO Sound (Movimiento Platillo) | Oscilador LC / Generador Frecuencia | Bucle Continuo (ON/OFF) |
| **Puerto 0x03** | **Bit 1** | Shot Sound (Disparo Jugador) | Filtro RC de Ruido Blanco / Generador Tono | Pulso / Disparo Único |
| **Puerto 0x03** | **Bit 2** | Player Explosion (Muerte Nave) | Generador de Ruido Blanco SN76477 | Tono de Explosión Larga |
| **Puerto 0x03** | **Bit 3** | Alien Explosion (Muerte Alien) | Filtro de Pulso Breve de Ruido | Pulso Seco / Breve |
| **Puerto 0x03** | **Bit 4** | Bonus Life Sound (Vida Extra) | Oscilador Tonal de Alta Frecuencia | Tono Agudo Positivo |
| **Puerto 0x03** | **Bit 5** | Sound Amp Mute / Enable | Rele de Silenciamiento General | Control de Ganancia |
| **Puerto 0x05** | **Bit 0** | Fleet March Pitch 1 (Nota Mas Baja) | Generador de Tono Bajo (Nota Do / ~55 Hz) | Pulso de Tono Grave |
| **Puerto 0x05** | **Bit 1** | Fleet March Pitch 2 | Generador de Tono (Nota Si / ~62 Hz) | Pulso de Tono |
| **Puerto 0x05** | **Bit 2** | Fleet March Pitch 3 | Generador de Tono (Nota La / ~69 Hz) | Pulso de Tono |
| **Puerto 0x05** | **Bit 3** | Fleet March Pitch 4 (Nota Mas Alta) | Generador de Tono (Nota Sol / ~77 Hz) | Pulso de Tono Agudo |
| **Puerto 0x05** | **Bit 4** | UFO Hit Sound (Destrucción UFO) | Tono Descendente Envolvente | Pulso de Recompensa |

* **Variables y Parámetros Involucrados**:
  * Registros de salida I/O: `OUT 0x03` y `OUT 0x05`.
  * RAM `0x2069`: Índice actual de la nota de marcha alienígena (0 a 3).
* **Interacciones y Dependencias**: Cada evento del motor de juego invoca el envío del bit correspondiente.
* **Resultados y Cambios de Estado**: Emisión instantánea de sonido por el altavoz del gabinete.
* **Casos Límite y Excepciones**:
  * **Secuencia de la Marcha Alienígena**: A medida que la flota camina, el software cicla el índice `RAM 0x2069` entre 0, 1, 2, 3 de forma ininterrumpida, disparando secuencialmente los bits 0, 1, 2 y 3 del Puerto `0x05`. Al acelerar el movimiento de la flota (`ALI-02`), la frecuencia con que se envían estos bits aumenta proporcionalmente, creando el icónico latido acelerado que incrementa la tensión del jugador.

---

## 14. Matriz de Interacciones, Orden de Ejecución y Dependencias de Hardware

### INT-01: Secuencia Estricta de Ejecución por Cuadro de Video (Frame Pipeline)
* **Objetivo**: Definir el orden secuencial no negociable en que el procesador debe ejecutar los métodos del juego en cada ciclo de 16.66 ms.
* **Comportamiento Exacto**: Diagrama de flujo procedimental ejecutado dentro de las rutinas de servicio de interrupción.
* **Condiciones de Activación**: Sincronización continua de hardware.
* **Entradas y Controles**: Ticks de reloj e interrupciones RST 1 y RST 2.
* **Reglas y Lógica de Ejecución**:
  * **FASE A: Interrupción MID-FRAME (INT 1 - Vector `0x08` / Línea 96 de Pantalla)**:
    1. Guarda el estado de los registros de la CPU en el Stack (`PUSH PSW`, `PUSH B`, `PUSH D`, `PUSH H`).
    2. Lee el Puerto de Entrada `0x01` (Joysticks, Botones de Disparo, Monedas, Start).
    3. Ejecuta `PLR-01` (Actualiza posición X del Cañón Láser).
    4. Ejecuta `PLR-02` (Avanza 4 píxeles el proyectil del jugador si está activo).
    5. Ejecuta `COL-01` para el proyectil del jugador contra el techo o búnkeres.
    6. Actualiza temporizadores rápidos (`RAM 0x207A`).
    7. Actualiza los bits de sonido del Puerto `0x03`.
    8. Restaura registros de la CPU (`POP H`, `POP D`, `POP B`, `POP PSW`) y rehabilita interrupciones (`EI`).
  * **FASE B: Interrupción END-OF-FRAME (INT 2 - Vector `0x10` / Línea 224 de Pantalla)**:
    1. Guarda registros en el Stack.
    2. Ejecuta `UFO-01` (Avanza posición del Platillo Volante si está activo).
    3. Ejecuta `ALI-02` (Procesa el movimiento y dibujado del **SIGUIENTE ALIENÍGENA INDIVIDUAL** en la matriz de 55).
    4. Ejecuta `AMMO-01` (Avanza 3 ó 4 píxeles hacia abajo los proyectiles enemigos activos).
    5. Ejecuta `COL-01` para proyectiles enemigos contra búnkeres o el jugador.
    6. Ejecuta `COL-02` (Comprueba colisión entre disparo del jugador y disparos enemigos).
    7. Comprueba condición de invasión `ALI-03` (`Y_alien <= 0x28`).
    8. Actualiza el contador de trama global y los temporizadores lentos (`RAM 0x2028`, `0x2073`).
    9. Actualiza los bits de sonido del Puerto `0x05` (Notas de marcha o destrucción de UFO).
    10. Restaura registros y rehabilita interrupciones (`EI`).
* **Diagrama Procedimental del Ciclo de Cuadro (Pipeline de Ejecución)**:

```
                  [ INICIO DEL CUADRO DE VIDEO (16.66 ms) ]
                                      │
                                      ▼
           ┌─────────────────────────────────────────────────────┐
           │   INTERRUPCIÓN INT 1 (Mid-Screen Sweep Line 96)     │
           └──────────────────────────┬──────────────────────────┘
                                      │
                                      ▼
           ┌─────────────────────────────────────────────────────┐
           │ 1. PUSH Registros CPU                               │
           │ 2. Lectura de Puertos I/O 0x01 y 0x02               │
           │ 3. PLR-01: Procesar entrada y mover Cañón Láser      │
           │ 4. PLR-02: Avanzar proyectil del jugador (+4 px Y)  │
           │ 5. COL-01: Verificar colisión proyectil jugador     │
           │ 6. Actualizar temporizadores de recarga             │
           │ 7. OUT 0x03: Escribir bits de sonido de acción      │
           │ 8. POP Registros CPU / EI                           │
           └──────────────────────────┬──────────────────────────┘
                                      │
                                      ▼
           ┌─────────────────────────────────────────────────────┐
           │   INTERRUPCIÓN INT 2 (End-of-Screen Sweep Line 224) │
           └──────────────────────────┬──────────────────────────┘
                                      │
                                      ▼
           ┌─────────────────────────────────────────────────────┐
           │ 1. PUSH Registros CPU                               │
           │ 2. UFO-01: Avanzar posición de Platillo Volante     │
           │ 3. ALI-02: Mover y redibujar 1 ALIENÍGENA de los 55  │
           │ 4. AMMO-01: Avanzar proyectiles enemigos (-3/-4 px Y)│
           │ 5. COL-01 / COL-02: Colisiones enemigas y cruzadas  │
           │ 6. ALI-03: Verificar condición de invasión          │
           │ 7. TIMER-01: Decrementar contadores de cuadros      │
           │ 8. OUT 0x05: Escribir notas de marcha / sonido UFO  │
           │ 9. POP Registros CPU / EI                           │
           └──────────────────────────┬──────────────────────────┘
                                      │
                                      ▼
                   [ FIN DEL CUADRO / ESPERA SIGUIENTE V-BLANK ]
```

* **Variables y Parámetros Involucrados**:
  * Puntero de Stack (SP): Mapeado en la parte alta de la RAM de trabajo (`0x23FF`).
* **Interacciones y Dependencias**: Es la columna vertebral determinista que coordina absolutamente todos los módulos documentados de la `SYS-01` a la `SND-01`.
* **Resultados y Cambios de Estado**: Simulación perfecta, fluida e históricamente idéntica del juego *Space Invaders* de 1978.
* **Casos Límite y Excepciones**: Este pipeline de ejecución es rígido. Si un desarrollador altera el orden (por ejemplo, procesando colisiones antes de mover los proyectiles), romperá la fidelidad del juego original, alterando patrones históricos como el "Disparo 23 del UFO" o la mecánica de destrucción de búnkeres.

---

## 15. Referencias Técnicas y Fuentes Consultadas

1. **Taito Corporation (1978)**. *Space Invaders Trimline Arcade Game Manual & Service Schematics*. Taito America Corporation. Disponible en: [Internet Archive - Space Invaders Trimline Manual](https://archive.org/details/ArcadeGameManualSitriml)
2. **Computer Archeology (2022)**. *Space Invaders - Disassembled Assembly Code, Memory Map and Hardware Architecture*. Analysis of ROM files `invaders.e`, `f`, `g`, `h`. Disponible en: [Computer Archeology - Space Invaders](https://www.computerarcheology.com/Arcade/SpaceInvaders/)
3. **Oregon State University (2022)**. *Disambiguating and Disassembling the Space Invaders ROM – Et tu, code?*. Analysis of 8080 Opcode mapping and Little-Endian data handling. Disponible en: [OSU Engineering Blog](https://blogs.oregonstate.edu/ettucode/2022/04/07/disambiguating-and-disassembling-the-space-invaders-rom/)
4. **Tom's Hardware (2025)**. *Intel 8080 bottleneck made classic 47-year-old Space Invaders arcade game run faster as enemies died*. Technical breakdown of CPU processing limits and organic acceleration. Disponible en: [Tom's Hardware - Space Invaders Bottleneck Analysis](https://www.tomshardware.com/video-games/retro-gaming/space-invaders-arcade-game-ran-faster-as-enemies-died-due-to-intel-8080-bottleneck-expert-coder-asserts-hardware-accident-to-blame)
5. **Space Invaders Wiki (2024)**. *UFO / Mystery Ship Mechanics and Score Algorithm (Modulo 16 Table & Shot 23 Exploit)*. Disponible en: [Space Invaders Fandom Wiki](https://spaceinvaders.fandom.com/wiki/UFO)
6. **RevSpace Hardware Repair Group (2023)**. *Space Invaders Cabinet Restoration, Discrete Sound PCB & Monitor Timing Schematics*. Disponible en: [RevSpace - Space Invaders Repair](https://revspace.nl/Space_invaders_repair)
