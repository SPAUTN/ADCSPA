# ADCarduino

Código para realizar las lecturas analógicas de los sensores de todo el sistema y convertirlas a señales digitales.

## Este algoritmo tiene 2 objetivos principales
* 1 - Tomar las señales analogicas de la Estación meteorológica y convertirlas a digitales con la mayor resolución posible
* 2 - Tomar las señales analogicas del Lisímetro de pesada y convertirlas a digitales con la mayor resolución posible
* A0 - Sensor velocidad del viento (0V -4 V)
* A1 - Sensor dirección del viento (0.2V – 3.8V)
* A2 - Sensor de radiación (0V – 3V)
* A3 - Sensor de humedad (0.5V – 4V)
* A4 - Sensor de temperatura (0.2V – 3.6V)
* A5 - Sensor de hoja mojada (seca<20mV – mojada>50mV)
* A6 - Celda de carga 1
* A7 - Celda de carga 2
* A8 - Celda de carga 3
* A9 - Celda de carga 4
* Pluviometro - Entrada digital que permita ISR (pin 18)

## Ramas
* **Rama master**: donde se agregan todas las pruebas y códigos listos y/o hardcodeados.
* **Rama production**: donde solo se encuentra el código listo para ser enviado al arduino.
* **Rama master.platformio**: donde se encuentran los códigos en C++ creados mediante PlatformIO.

> Cambio de nombre de rama main a master: 
```
git branch -m main master
git fetch origin
git branch -u origin/master master
git remote set-head origin -a
```

# CI/CD Github Actions
## Merge request recien creada
Al crear una merge request se verá lo siguiente:
![action in progress](https://i.ibb.co/SQcw5vk/actioninprogressinmerge.png)

Esto indica que se está ejecutando un job para comprobar que el cambio realizado permita la compilación del proyecto de forma correcta.

## Job en progreso
Además del cartel visto en la merge request, podemos ir a la pestaña Actions y ver el estado del job en progeso. Se verá algo similar a la siguiente pantalla, donde se indica el estado del job más actual en la parte superior de la lista:
![lista de actions](https://i.ibb.co/9T22FRw/actioninprogress.png)

## Merge request correcta
Cuando el merge request creado no genere ningún problema con la compilación se verá en el merge request del siguiente modo:
![merge passed](https://i.ibb.co/1nq60Fg/checkpassed.png)

## Merge request fallida
Cuando una merge request genere un error que impida la correcta compilación del proyecto veremos lo siguiente en la misma:
![merge no pasada](https://i.ibb.co/18Qq8Yf/checknotpassed.png)

Esto indica que el código posee algún error que debe ser corregido y posteriormente pusheado a la rama que se quiere mergear.