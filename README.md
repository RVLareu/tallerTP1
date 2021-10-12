# Trabajo Práctico 1: Hangman
</br>

## Taller de programación - Veiga - 2021 2C

### Vázquez Lareu, Román. 100815

</br>
El presente trabajo implementa un programa que hace las veces de servidor por un lado y cliente por otro. El cliente enviará letras al servidor, el cual las procesará y actualizará el estado de la partida de ahorcado, devolviendo al cliente el estado de la misma según protocolo.


# Diseño
</br>

El trabajo práctico se dividió en 3 partes: el cliente, el servidor y el juego del ahorcado propiamente dicho. Este último se implementó de tal manera se poder ser jugado sin necesidad de una conexión de red. Con el objetivo de poder almacenar varios juegos o incluso poder jugar en simultaneo (con las respectivas modificaciones), al juego en si se accede a través de un administrador que tiene a cargo los juegos en curso y almacena los ya finalizados.
</br>
Tanto para el cliente como para el servidor, se hicieron de tal manera de poder ser reutilizador para otros posibles juegos. Sin embargo, con el objetivo de ser más personalizado y no tan general se implementaron módulos para el cliente y servidor enfocados a correr específicamente el juego del ahorcado.
</br>
Diagrama que representa brevemente las principales clases, métodos y atributos presentes, con el objetivo de dar una idea general
![](/classdiagram)

# Comunicacíon
</br>
Luego de realizarse la conexion entre el servidor y el cliente, el primero le envía al segundo el estado actual de la partida (en este caso para que el jugador tenga conocimiento de la misma). A partir de este punto se ingresa en un ciclo donde el cliente envía una letra (el jugador puede envíar más de una pero el procesamiento es de a letras) al servidor, este la procesa en el juego y envía el estado actualizado
</br>
Diagrama de secuencia que se da al aceptar un cliente el servidor
![](/serverinitgame.png)
</br>
Diagrama de secuencia que ocurre al jugar el usuario
![](/game.png)

# Decisiones

</br>

Algo a tener en cuenta era la representación del estado del juego al jugador, que implicaba tener la palabra con ' _ ' en los lugares donde no se sabía aún la letra correspondiente. Esto de resolvió en la clase *game*, la cual contiene tanto la palabra completa como la palabra a presentarle al jugador o *display word*, la cual está compuesta por guiones bajos y letras.

Otra decisión fue la de tener una clase *hangman* la cual pudiera tener varias partidas, permitiendo resolver fácilmente la impresión del lado del servidor del resumen de las partidas jugadas. Si se detectaba que un juego finalizaba, se identificaba el resultado y previo a enviar el estado de juego al servidor se actualizaban las victorias y derrotas.
