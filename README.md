# Synestesia (WIP)

Synestesia reconoce las frecuencias musicales del instrumento más cercano y emite la frecuencia leída a todos los dispositivos conectados. Además, es posible utilizarlo con módulos sensoriales, tales como un conjunto de luces o un dispositivo que produzca vibración. 

Desde su concepto, Synestesia se ha diseñado para ser configurable, escalable y eficiente. A modo de ejemplo, se incluye un módulo sensorial que gestiona un conjunto de luces LED y ofrece diferentes colores en función de la nota producida por el instrumento.

# ¿Cómo funciona?
El sistema está compuesto por un dispositivo emisor y uno o más dispositivos clientes.
Mientras que el dispositivo emisor se encarga de reconocer las frecuencias y gestionar la configuración general y del módulo (si procede), los dispositivos clientes se conectan inalámbricamente al dispositivo emisor para recibir la frecuencia analizada por éste.

# Vídeos

[La venganza de Gaia - Mägo de Oz](https://youtu.be/UWesdEeDR3A)

[Vive - Saurom](https://youtu.be/wd7kBOEUFDI)


# Requisitos 

## Materiales
Para que Synestesia funcione correctamente, es necesario disponer de los siguientes recursos materiales (o similares):
* Un controlador *NodeMCU Lolin V3* (El módulo principal, gestiona la configuración, conexiones y persistencia de ficheros)
* Un controlador *Arduino Nano* (Encargado de leer y reconocer las frecuencias leídas)
* Un micrófono *MAX9814*

## Software
Se han desarrollado casi todos los componentes necesarios para este proyecto. Dichos componentes se encuentran en las carpetas *main* (los componentes principales) y *module* (los componentes del módulo, en este caso, de luces). Dentro de la carpeta main se encuentra el componente [***ApproxFFT***](https://create.arduino.cc/projecthub/abhilashpatel121/approxfft-fastest-fft-function-for-arduino-fd4917), desarrollado por por **Abhilash**. (Gracias!)

Además, algunos componentes obtenidos desde las librerías de Arduino:
* ArduinoThread
* SPIFFS

Por último, es necesario instalar la herramienta [Arduino ESP8266 filesystem uploader](https://github.com/esp8266/arduino-esp8266fs-plugin/), para añadir los archivos de configuracion y web, disponibles en la carpeta `data`. 

# Primeros pasos

## Instalación
Para la instalación se debe descargar la rama ***master*** y extraer el contenido de las carpetas *main* y *module* dentro de la carpeta `libraries` de Arduino. Normalmente, esta carpeta se encuentra en *`~/Documents/Arduino/`*.

Además será necesario instalar los componentes antes listados.

## Montaje

El montaje del sistema, en su primera version (1.0.1) se refleja en el siguiente esquemático:
![Syn Core Schematic](https://github.com/Juanjod54/Synestesia/blob/main/SynCore_2021-05-16.png)

## Ejecucion 
Como este repositorio contiene un trabajo en progreso, el programa definitivo no está desarrollado. Sin embargo, es posible obtener una aproximación del resultado final ejecutando el test **SYN-24**. Este test reconoce las señales, aplica la configuracion desde la web y controla el módulo de luces.

### Carga de ficheros
Para acceder a la configuracion web y poder cargar correctamente el módulo, se debe realizar una carga de ficheros.

## Configuración

Para configurar el módulo y la configuracion global es necesario conectarse a la red WiFi Synestesia. Una vez conectado, se debe acceder a la dirección _192.168.1.1/configuration_.


### Programas de pruebas
En la carpeta `synestesia/tests` se encuentran los programas realizados para realizar las pruebas del código entregado. Cada módulo desarrollado tiene asociado un programa de pruebas. Para habilitar la depuración, sera necesario establecer la salida *Debug Port* a *Serial*, desde Arduino IDE.

# Acerca de

Puedes reutilizar los componentes desarrollados en este proyecto con total libertad, mencionando mi trabajo.

#### Translation pending!
