# ADCarduino

Código para realizar las lecturas analógicas de los sensores de todo el sistema y convertirlas a señales digitales.

## Este algoritmo tiene 2 objetivos principales
* 1 - Tomar las señales analogicas de la Estación meteorológica y convertirlas a digitales con la mayor resolución posible
* 2 - Tomar las señales analogicas del Lisímetro de pesada y convertirlas a digitales con la mayor resolución posible

> [!IMPORTANT]
> Los pines utilizados son:
> **Lisimetro**:
>   - D4 - Clock
>   - RX2 - Data
> **Velocidad viento**: - D26
> **Dirección viento**: - D27
> **Pluviometro**: - D13
> **Humedad relativa**: - D14
> **Radiación**: - D34
> **BMP180**:
>   - D21 - SDA
>   - D22 - SCL
> **Hoja mojada**: - D25

> [!NOTE]
> El repositorio utiliza un github action que verifica una correcta compilación del código cada vez que se sube un nuevo commit a cualquiera de las ramas. Si la compilación no es correcta no se podrá realizar un pull request a la rama master.
