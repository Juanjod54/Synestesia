# Synestesia (WIP)

Synestesia es un sistema diseñado para sentir la música sin límites.

Este sistema reconoce las frecuencias de las notas musicales del instrumento más cercano y emite la frecuencia leída a todos los dispositivos conectados. 

Además, es posible utilizarlo con módulos sensoriales, tales como un conjunto de luces o un dispositivo que produzca vibración, que potencian la experiencia musical. 

El sistema se ha diseñado para ser configurable, escalable y eficiente.

En esta primera versión se incluye un módulo gestiona un conjunto de luces LED para ofrecer diferentes colores basados en la nota proporcionada por un instrumento.


# ¿Cómo funciona?
El sistema estará compuesto por un dispositivo emisor y uno o más dispositivos clientes.
Mientras que el dispositivo emisor se encarga de reconocer las frecuencias y gestionar la configuración general y del modulo (si procede), los dispositivos clientes se conectan inalámbricamente al dispositivo emisor para recibir la frecuencia analizada por éste y producir una respuesta (diferente para cada modulo) en base a dicha entrada. 

**Synestesia esta centrado en la escalabilidad desde su inicio, para facilitar el desarrollo de futuros modulos que faciliten, potencien y proporcionen y mejoren algo tan maravilloso como la musica.**

# Requisitos 

## Materiales
Para que Synestesia funcione correctamente, es necesario disponer de los siguientes recursos materiales (o similares):
* Un controlador *NodeMCU Lolin V3* (El módulo principal, gestiona la configuración, conexiones y persistencia de ficheros)
* Un controlador *Arduino Nano* (Encargado de leer y reconocer las frecuencias leídas)
* Un micrófono *MAX9814*

## Software
Se han desarrollado casi todos los componentes necesarios para este proyecto. Dichos componentes se encuentran en las carpetas "*main*" (los componentes principales) y "*module*" (los componentes del módulo, en este caso, de luces). El componente ***ApproxFFT*** ha sido desarrollado por X. (Gracias!)

No obstante, algunos componentes han sido obtenidos desde las librerías de Arduino:
* ArduinoThread
* SPIFS

Ademas, es necesario instalarse la herramienta ESP8266 Arduino Data Uploader, para añadir los archivos de configuracion y web. 

# Primeros pasos

## Instalación
Para la instalación se debe descargar la rama ***master*** y extraer el contenido de las carpetas *main* y *module* dentro de la carpeta `libraries` de Arduino. Normalmente, esta carpeta se encuentra en *`~/Documents/Arduino/`*.

Además sera necesario instalar los componentes antes listados.

## Montaje

El montaje del sistema, en su primera version (1.0.1) se refleja en el siguiente esquematico:

## Ejecucion 
Como este repositorio contiene un trabajo en progreso, el programa definitivo no esta desarrollado. Sin embargo, es posible obtener una aproximacion del resultado final ejecutando el test SYN-24. Este test reconoce las señales, aplica la configuracion desde la web y controla el modulo de luces.

### Carga de ficheros
Para acceder a la configuracion web y poder cargar correctamente el modulo, se debe realizar una carga de ficheros.

## Configuracion

Para configurar el modulo y la configuracion global es necesario conectarse a la red WiFi Synestesia. Una vez conectados, se debe acceder a la direccion 192.168.1.1/configuration.


### Programas de pruebas
En la carpeta `synestesia/tests` se encuentran los programas realizados para realizar las pruebas del código entregado. Cada modulo desarrollado tiene asociado un programa de pruebas. Para habilitar el debug, sera necesario establecer la salida debug a *Serial*, desde Arduino IDE.

# Acerca de
Dudas, comentarios y sugerencias a juanjod54@gmail.com
