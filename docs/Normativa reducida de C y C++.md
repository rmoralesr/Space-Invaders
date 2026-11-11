
> [!NOTE]
> **Cómo interpretar las alertas**
> - **TIP**: ejemplo correcto o recomendación práctica.
> - **CAUTION**: ejemplo incorrecto.
> - **NOTE**: ejemplo neutro o aclaración.
> - **IMPORTANT**: regla fundamental.
> - **WARNING**: advertencia o restricción relevante.

---

# Cómo utilizar esta normativa

Este documento recoge las normas de programación de la edición 2026. Los ejemplos de código se presentan en bloques Markdown para facilitar su lectura y copia.

---

# 1. Nomenclatura

Esta sección describe las normas para poner nombre a los elementos del código fuente: las variables, las funciones, las estructuras, las clases, los ficheros, etc…

Todo el código fuente debe ser escrito y comentado **EN INGLÉS** manteniendo un juego de caracteres standard ASCII-7 (esto es, sin ñ, vocales acentuadas, ç y otros símbolos).

## 1.1 Reglas generales de nomenclatura

Los nombres de funciones, variables y ficheros deben ser descriptivos. Se deben evitar abreviaturas innecesarias.

Es mucho más importante que el código sea legible que ahorrar espacio horizontal. No se deben usar abreviaturas ambiguas, que no sean familiares a un nuevo programador que se incorpora al proyecto o forzadas quitando letras a una palabra.

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> // BAD NAMES
> int n;                // Meaningless
> int nerr;             // Ambiguous abbreviation
> int n_comp_conns;     // Ambiguous abbreviation
> int wgc_connections;  // Only your group knows what this is
> int pc_reader;        // Lots of things can be a "pc"
> int cstmr_id;         // Deletes internal letters
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // GOOD NAMES
> int price_count_reader;   // No abbreviation
> int num_errors;           // "num" is a widespread convention
> int num_dns_connections;  // Most people know what "DNS" is
> ```

## 1.2 Nombres de fichero

Todos los ficheros deben nombrarse **EN MINÚSCULAS** usando sólo caracteres ASCII-7 y pueden incluir subguiones (_) o guiones (-) en sus nombres. **NUNCA DEBEN INCLUIR ESPACIOS EN BLANCO**:

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // GOOD - Valid filenames
> my_useful_class.cc
> my-useful-class.cc
> myusefulclass.cc
> myusefulclass_test.cc
> ```

Los ficheros de código C deben tener la extensión.c, los de código C++ deben tener la extensión.cc (a no ser que se estén integrando en un motor que exija otra extensión) y los ficheros de cabecera la extensión.h. Los ficheros que vayan a ser incluidos textualmente por el pre-procesador (e.g: en una compilación multiplataforma) pueden llevar el sufijo.inc.

No se deben usar nombres de fichero conocidos, como por ejemplo string.h.

En general los nombres deben ser muy específicos. Por ejemplo, se debe usar "http_server_logs.h" en vez de sólo "logs.h".

Un caso común es nombrar ficheros por parejas, por ejemplo, foo_bar.h y foo_bar.cc para definir e implementar la clase FooBar.

Las funciones implementadas como inline deben ir en el fichero.h tras el bloque class.

## 1.3 Nombres de tipos, estructuras y clases

Los nombres de tipos empiezan con una letra mayúsucula y tienen una mayúscula al principio de cada nueva palabra, sin subguiones: MyExcitingClass, MyExcitingEnum. Los nombres de todos los tipos - y clases, structs, typedefs y enums - siguen la misma convención anterior:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> // classes and structs
> struct UrlTableProperties { … }
> class UrlTable { … }
> class UrlTableTester { … }
>
> // typedefs
> typedef hash_map<UrlTableProperties*, std::string> PropertiesMap;
>
> // enums
> enum UrlTableErrors { … }
> ```

## 1.4 Nombres de variables

Los nombres de variables y atributos se escriben siempre en minúsculas con subguiones entre palabras. Adicionalmente, los atributos de clase (pero no de struct) acaban en subguión. Por ejemplo: a_local_variable, a_struct_data_member, a_class_data_member_.

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> std::string tableName;   // Bad - mixed case
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> std::string table_name;  // OK - uses underscore.
> ```

Las variables globales deben ir prefijadas con "g_" para diferenciarlas del resto.

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> int num_enemies = 8;   // Bad - not starting with g_
>
> void count_enemies() {
>   int num_enemies = 0; // Confusing
>   …
> }
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> int g_num_enemies = 8;  // OK
>
> void count_enemies() {
>   int num_enemies = 0; // OK
>   …
> }
> ```

Las variables de una struct se llaman igual que el resto de variables (no tienen un subguión al final del nombre):

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> struct UrlTableProperties {
>   std::string name;
>   int num_entries;
>   static Pool<UrlTableProperties>* pool;
> };
> ```

## 1.5 Nombres de constantes

Se debe usar una "k" minúscula seguida de una mayúscula por cada palabra, como por ejemplo kDaysInAWeek, para constantes definidas globalmente o dentro de una clase.

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> const int kDaysInAWeek = 7;
> ```

## 1.6 Nombres de funciones

Las funciones normales deben empezar con letra mayúscula y tener una mayúscula por cada nueva palabra, sin subguiones:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> AddTableEntry();
> DeleteUrl();
> OpenFileOrDie();
> ```

## 1.7 Nombres de namespaces

Los namespaces deben estar en minúscula y basarse en el nombre (y posiblemente ruta) del proyecto (e.g: `edk`, `edk: :audio` …).

## 1.8 Nombres de enumeraciones

Las enumeraciones normales se nombran de manera similar a las constantes: el tipo en sí se nombra siguiendo las reglas de nombrado de tipos, empezando por mayúscula y poniendo una mayúscula al comienzo de cada palabra; cada uno de los valores se nombra como una constante, con la salvedad de que después de la "k" se insertan como prefijo el nombre de tipo y un subguión seguido del nombre del valor:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> enum UrlTableErrors {
>   kUrlTableErrors_OK = 0,
>   kUrlTableErrors_ErrorOutOfMemory,
>   kUrlTableErrors_ErrorMalformedInput,
> };
> ```

---

# 2. Formato

Esta sección describe las normas de formato de texto que el código fuente debe seguir, con independencia de qué editor de texto o IDE se use para el desarrollo.

## 2.1 Longitud de línea

> [!IMPORTANT]
> Cada línea de texto de un fichero debe tener como mucho 80 caracteres de longitud. Se recomienda el uso de opciones y plug-ins de los editores de código para marcar una guía visual en esa columna.

## 2.2 Caracteres no ASCII

El uso de caracteres no ASCII debe evitarse. Si no hay más remedio que usarlos, se debe usar una codificación UTF-8.

## 2.3 Espacios VS tabulaciones

> [!IMPORTANT]
> Se usarán SÓLO espacios, con una indentación standard de 2 espacios. Se debe configurar el editor de código para que emita espacios cuando se pulse el tabulador.

## 2.4 Formato de las funciones

El tipo devuelto estará en la misma línea que el nombre de la función, así como los parámetros y la apertura de llave:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> ReturnType FunctionName(Type par_name1, Type par_name2) {
>   DoSomething();
>   …
> }
>
> void CharacterAttackEnemy(Enemy* target, bool critical) {
>   …
> }
> ```

Si el texto no cabe en una sola línea, se puede partir la lista de parámetros:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> ReturnType ReallyReallyReallyLongFunctionName(Type par_name1, Type par_name2,
>                                              Type par_name3) {
>   DoSomething();
>   …
> }
> ```

O incluso:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> ReturnType VeryLongNameOfAReallyReallyReallyLongFunctionName(
>     Type par_name1,  // 4 space indent
>     Type par_name2,
>     Type par_name3) {
>   DoSomething();  // 2 space indent
>   …
> }
> ```

Algunas notas:
- Si el tipo devuelto y el nombre de la función no caben en la misma línea, introducir un salto de línea entre ellos
- Si el salto está después del tipo devuelto, no añadir indentación a la siguiente línea
- Siempre se abre el paréntesis en la misma línea en la que está el nombre de la función
- Nunca hay un espacio entre el nombre de la función y el paréntesis
- Nunca hay un espacio entre el paréntesis y los parámetros
- Se abre llave al final de la línea en la que esté el último parámetro
- Se cierra llave en una línea aparte en la que no hay nada más (a no ser que la función sea tan pequeña que toda ella quepa en una línea)
- Todos los parámetros deben tener exactamente los mismos nombres en la declaración y en la implementación
- Todos los parámetros deben estar alineados
- La indentación por defecto es 2 espacios
- Si los parámetros no caben en la primera línea, su indentación en líneas inferiores es de 4 espacios

## 2.5 Llamadas a función

Para las llamadas a función, o bien se escribe toda la llamada en un misma línea, o bien se introduce un salto de línea al abrir el paréntesis y se continúa debajo con una indentación de 4 espacios:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> bool retval = DoSomething(argument1, argument2, argument3);
> …
>
> if (…) {
>   …
>   …
>   if (…) {
>     DoSomething(
>         argument1, argument2,  // 4 space indent
>         argument3, argument4);
>   }
> }
> ```

En ocasiones, hay un parámetro especialmente más complejo que el resto que requiere más espacio y una explicación especial:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> bool retval = DoSomething(scores[x] * y + bases[x],  // Heuristic
>                           x, y, z);
> ```

En otras ocasiones, los parámetros forman una estructura que tiene sentido en sí misma (e.g: una matriz):

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> // Transform the widget by a 3x3 matrix.
> WidgetTransform(widget,
>                 x1, x2, x3,
>                 y1, y2, y3,
>                 z1, z2, z3);
> ```

## 2.6 Condicionales

Las instrucciones if y else deben ir en líneas separadas. Las llaves se abrirán al final de la línea en la que está la condición (en el caso del if) o el else. Las instrucciones, la condición y las llaves estarán separadas por un espacio. Si sólo incluyen una instrucción y todo el bloque cabe y se escribe en una sola línea, pueden no usar las llaves, pero en general todo bloque if **DEBE** usar llaves.

Se considera una buena práctica el comenzar una comparación con el valor literal, para evitar errores en los que se escribe una asignación en lugar de una comparación:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> if (5 == condition) {  // no spaces inside parentheses
>   …  // 2 space indent
> } else if (…) {  // The else goes here
>   …
> } else { …
> }
> ```

Los condicionales cortos pueden ser escritos en una sola línea para mejorar la legibilidad, pero sólo si no incluyen else:

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> // BAD -  If statement on one line when there is an ELSE
> if (x) DoThis();
> else DoThat();
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // GOOD
> if (kFoo == x) return new Foo();
> if (kBar == x) return new Bar();
> ```

Se indenta con 2 espacios aunque sólo haya una instrucción:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> if (true == condition) {
>   DoSomething();  // 2 space indent.
> }
> ```

## 2.7 Bucles y switch

Las construcciones switch deben usar llaves para delimitar cada caso. Si hay casos agrupados (fall-through), deben ser destacados (por ejemplo, con un comentario). Del mismo modo, debe haber un caso default al final del switch. Si ese caso no debería ejecutarse nunca, se debe colocar un assert en su cuerpo:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> switch (var) {
>   case 0: {  // 2 space indent
>     …      // 4 space indent
>     break;
>   }
>   case 1: {
>     …
>     break;
>   }
>   default: {
>     assert(false);
>   }
> }
> ```

En los bucles, la apertura de las llaves debe estar en la misma línea que la condición. Si no contienen código, las llaves se cierran inmediatamente después de abrirse. Nunca se debe omitir el cuerpo del bucle:

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> // BAD -  looks like part of do/while loop.
> while (condition);
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // GOOD - empty body.
> while (condition) {
>   // Repeat test until it returns false.
> }
> for (int i = 0; i < kSomeNumber; ++i) {}
> ```

## 2.8 Punteros y referencias

Al escribir punteros y referencias, no se dejan espacios entre la flecha o el punto y su campo. Del mismo modo, no se dejan espacios tampoco entre * o & y la variable:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> x = *p;
> p = &x;
> x = r.y;
> x = r->y;
> ```

Cuando se declaran variables puntero o referencia, es indiferente si el símbolo * o & está junto al tipo o el nombre de la variable, pero no debe estar separado de ambos a la vez por espacios:

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> // BAD -  spaces on both sides of *
> char * c;
> // BAD - spaces on both sides of &
> const std::string & str;
> // BAD - multiple variables declared on the same line
> char* c, *d, *e, f;
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // GOOD - space preceding
> char *c = nullptr;
> const std::string &str;
>
> // GOOD - space following.
> char* c = nullptr;
> const std::string& str;
> ```

## 2.9 Expresiones booleanas

Cuando se escriban expresiones booleanas que no quepan en una misma línea, se deben partir por el operador lógico que las une:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> if (this_one_thing > this_other_thing &&
>     a_third_thing == a_fourth_thing &&
>     yet_another && last_one) {
>   …
> }
> ```

## 2.10 return

No se debe parentizar el return, a no ser que se devuelva el resultado de una expresión compuesta:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> return (value);      // You wouldn't write var = (value);
> return(result);      // return is not a function!
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // No parentheses in the simple case
> return result;
> // Parentheses OK to make a complex expression more readable
> return (some_long_condition &&
>         another_condition);
> ```

## 2.11 Directivas de preprocesador

Aunque una directiva de preprocesador (e.g: `#if DEBUG`) esté a mitad de un bloque indentado, en líneas generales, no estará indentada, comenzará al principio de la línea. En ficheros de configuración donde todo el contenido sean directivas de preprocesador, está permitido indentarlas:

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> // BAD - indented directives
>   if (lopsided_score) {
>     #if DISASTER_PENDING  // Wrong! Should be at beginning of line
>     DropEverything();
>     #endif                // Wrong! Do not indent "#endif"
>     BackToNormal();
> }
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // GOOD - directives at beginning of line
>   if (lopsided_score) {
> #if DISASTER_PENDING
>     DropEverything();
> # if NOTIFY
>     NotifyClient();
> # endif
> #endif
>     BackToNormal();
>   }
> ```

## 2.12 Formato de struct

El nombre de la struct debe estar en la misma línea (la primera) si el ancho de línea lo permite.

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> struct MyStruct {
>   int some_var;
>   int some_other_var;
> };
> ```

## 2.14 Namespaces

Abrir un namespace **NO** añade un nivel de indentación. Si se declaran namespaces anidados, cada declaración va en su propia línea:

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> // BAD - Indented when it should not be
> namespace foo {
>   namespace bar {
>     void Paw() {
>       …
> }
>   }  // namespace bar
> }  // namespace foo
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // GOOD - No extra indentation within namespace
> namespace foo {
> namespace bar {
>
> void Paw() {
>   …
> }
>
> }  // namespace bar
> }  // namespace foo
> ```

---

# 3. Cabeceras

En general, cada fichero.c ó.cc debe llevar su cabecera.h asociada. Hay algunas excepciones comunes como los test unitarios o ficheros muy pequeños que contienen sólo la función main().

## 3.1 Guarda

Todas las cabeceras deben tener su correspondiente guarda con `#define` para prevenir que se incluyan múltiples veces. El formato de la guarda es: `__FICHERO_H__ 1`

Si dentro del mismo proyecto se encuentra dos ficheros de idéntico nombre pero distinta ruta, la ruta se incluirá como parte de la guarda de la siguiente manera:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> #ifndef __ENGINE_H__
> #define __ENGINE_H__ 1
>
> …
>
> #endif  // __ENGINE_H__
> ```

## 3.2 Declaración por adelantado (forward declaration)

Deben declararse por adelantado clases o structs de los que sólo se referencie su nombre para evitar añadir `#include` innecesarios.

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> // BAD
> #include “sprite.h”
>
> …
>
> Sprite* sprite_ptr;
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // GOOD
> class Sprite;
>
> …
>
> Sprite* sprite_ptr = nullptr;
> ```

## 3.4 Orden de los parámetros de las funciones

Cuando se defina una función, el orden de los parámetros es el siguiente: primero los de entrada, luego los de salida.

## 3.5 Nombre y orden de los `#include`

El orden de escritura de los `#include` será el siguiente:

- La cabecera correspondiente a este archivo
- Cabeceras de C
- Cabeceras de C++
- Otras librerías externas
- Otras cabeceras de este proyecto

---

# 4. Ámbitos

## 4.1 Namespaces

El uso de namespaces para crear ámbitos es una buena práctica. Los namespaces deben recibir sus nombres en relación al proyecto y su ruta. Debe evitarse la directiva using cuando no sea necesaria. En especial, debe evitarse la directiva "using namespace", siendo preferible usar sólo using con elementos concretos. Está completamente **PROHIBIDO** el uso de using en una cabecera:

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> // BAD - In a .h file
> #include <string>
>
> using namespace std;    // FORBIDDEN - You are forcing the linkage
>                         // of the whole available namespace symbols
>                         // to anyone who includes this header
> string my_string;
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // GOOD - In a .h file
> #include <string>
>
> std::string my_string;  // You are only linking std::string
> ```

En un fichero de código.cc podemos aplicar using a elementos concretos, pero en general es preferible no depender de ello:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> // In a .cc file
> using std::cout;
> using std::endl;
>
> void Foo() {
>   std::cout << “Foo!” << std::endl;
> }
> ```

## 4.4 Variables locales

Las variables de una función deben ponerse en el ámbito más cerrado posible, y deben ser inicializadas en la declaración. Sólo se declarará una variable por línea de código.

Las variables declaradas en cada pila deben, en total, mantener un tamaño reducido (e.g: < 4kB). Para necesidades de memoria más grandes se debe usar el heap (o el segmento de datos).

C++ permite declarar variables en cualquier lugar de una función, pero siguiendo esta guía, deben declararse en el ámbito más local posible y tan cerca de su primer uso como sea posible. Esto facilita enormemente la legibilidad y comprensión del código, al mostrar el tipo y valor de la variable en el lugar adecuado:

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> // BAD - initialization separate from declaration
> int i;
> i = f();
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // GOOD
> int i = f();
> ```

Las variables necesarias para if, while y for normalmente deben ser declaradas dentro de esas estructuras de control dado que están confinadas a esos ámbitos. La excepción serán las variables que sean objetos dado que, por eficiencia, debe evitarse que el constructor se ejecute en cada iteración de un bucle for o while:

> [!CAUTION]
> **Ejemplo incorrecto**
>
> ```cpp
> // BAD - Inefficient implementation:
> for (int i = 0; i < 1000000; ++i) {
>   Foo f;
>   // A Foo type variable gets instantiated
>   // 1000000 times.
>   DoSomething(i, &f);
> }
> ```

> [!TIP]
> **Ejemplo correcto**
>
> ```cpp
> // GOOD - My constuctor and destructor get called
> // only once each
> Foo f;
> for (int i = 0; i < 1000000; ++i) {
>   DoSomething(i, &f);
>   }
> ```

---

# 7. Comentarios

## 7.1 Estilo de comentario

Se debe usar la sintaxis de o bien // o bien /* \*/, mientras se sea consistente. En general todas las clases, funciones, tipos, etc… deben estar correctamente comentados.

## 7.2 Comentarios de fichero

Se deben comenzar todos los ficheros con un encabezado de comentario en el que se indique el nombre del fichero, una breve descripción de lo que se implementa en él, el autor y la fecha.

Generalmente un fichero.h describirá la clase que tiene declarada y una visión general de para qué sirve y cómo se usa. Un fichero.cc debería incluir más información acerca de detalles de implementación y explicación de los algoritmos. Si se entiende que estas explicaciones son de interés para alguien que sólo va a usar el.h, deben incluirse en él, pero debe indicarse en el.cc que los comentarios están en la cabecera.

NO se deben duplicar comentarios en el.h y en el.cc. Los comentarios duplicados divergen y crean errores de comprensión.

## 7.3 Comentarios de struct

Todas las definiciones de struct deben ir acompañadas por un comentario que describe para qué sirven y cómo se utilizan. El comentario estará situado justo sobre la struct.

## 7.4 Comentarios de funciones

Sobre su declaración, todas las funciones deben tener un comentario explicando para qué sirven y cómo se usan, así como especificando su resultado (si lo devuelven) y sus parámetros (si los tienen):

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> /** Delete a URL from the cache
> result 0 if successful or an error code
> param url The URL to be deleted
> */
> int DeleteUrl(const char* url);
> ```

## 7.5 Comentarios de implementación

Partes del código complejas o difíciles de entender deben ir precedidas de un comentario explicativo y/o de un esquema claro.

## 7.6 Comentarios de parámetros

Cuando se pase un puntero a nullptr o NULL, un booleano, o valores literales enteros a funciones como parámetros, es aconsejable incluir un comentario explicando lo que son y significan, o bien hacer el código más autoexplicativo usando constantes:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> bool success = Calculate(interesting_value,
>                          10,
>                          false,
>                          nullptr);  // What are these arguments??
> ```

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> bool success = Calculate(interesting_value,
>                          10,        // Default base value
>                          false,     // Not the first time calling
>                          nullptr);  // No callback
> ```

Alternativamente, se pueden usar constantes o variables auto-explicativas:

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> const int kDefaultBaseValue = 10;
> const bool kFirstTimeCalling = false;
> Callback* null_callback = nullptr;
> bool success = CalculateSomething(interesting_value,
>                                   kDefaultBaseValue,
>                                   kFirstTimeCalling,
>                                   null_callback);
> ```

## 7.7 Sobrecomentar

Se debe evitar el comentario excesivo o sobrante (e.g: no todos los bucles deben tener encima un comentario que explique que recorren una estructura de datos) dado que hace la lectura del código más farragosa.

## 7.8 TODO y DEPRECATED

Se deben usar comentarios TODO para marcar código que es temporal, una solución a corto plazo o que es buena de momento, pero no perfecta.

Del mismo modo, se deben usar los comentarios DEPRECATED para marcar partes de un API que van a desaparecer pronto y a las que se ha ofrecido una alternativa (incluida en el comentario):

> [!NOTE]
> **Ejemplo de código**
>
> ```cpp
> // TODO(pepe@gmail.com): Use a "*" here for concat. operator
> // TODO(Luis) change this to use relations
> …
> // DEPRECATED(Jose): Please use MyClass::DeleteUrl() now
> ```