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

![](/classdiagram)

#Comunicacíon
</br>
Luego de realizarse la conexion entre el servidor y el cliente, el primero le envía al segundo el estado actual de la partida (en este caso para que el jugador tenga conocimiento de la misma). A partir de este punto se ingresa en un ciclo donde el cliente envía una letra (el jugador puede envíar más de una pero el procesamiento es de a letras) al servidor, este la procesa en el juego y envía el estado actualizado

![](/serverinitgame.png)
![](/game.png)
