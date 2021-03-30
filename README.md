# Synestesia

Synestesia es un sistema, escalable, diseñado para sentir la música más allá de la estimulación auditiva, como sería un conjunto de luces o un dispositivo que produzca vibración.
En esta primera versión se incluirá un sistema que gestione un conjunto de luces LED para ofrecer diferentes colores basados en la nota proporcionada por un instrumento.

## Primeros pasos

### Instalación de las librerías
Para la instalación de las librerías existen dos métodos.

#### Importación de las librerías mediante el IDE de Arduino
Para importar las librerías primero es necesario empaquetarlas en archivos ZIP.

En dispositivos basados en Unix (o WSL) se debe ejecutar el script `zip_libraries.sh`, que creará el directorio `libraries` y empaquetará cada librería en un archivo ZIP.
Posteriormente, es necesario abrir el IDE de Arduino, hacer click en "Programa" -> "Incluir librería" y seleccionar las librerías que se han empaquetado posteriormente.

**NOTA:** El script instalará el paquete zip si no se encuentra previamente instalado.

#### Importación manual de las librerías
Para importar manualmente las librerías será necesario copiar el contenido de la carpeta `main` en la carpeta `libraries` de Arduino.
Por defecto, esta carpeta se encuentra en `Documents/Arduino/`.

## Programas de pruebas
En la carpeta `synestesia/tests` se encuentran los programas realizados para realizar las pruebas del código entregado.
Para probar los programas será conveniente revisar las conexiones definidas al inicio del archivo para ver que se correspondan con las conexiones activas en la placa.

## Acerca de

### ¿Cómo funciona?
El sistema estará compuesto por un dispositivo emisor y uno o más dispositivos clientes.
Los dispositivos clientes se conectarán inalámbricamente al dispositivo emisor para recibir la información analizada por éste y producir la estimulación para la que han sido diseñados.

### Repositorio
En este repositorio se encuentra el código del proyecto Synestesia. En la carpeta `synestesia` se almacena todo el código relacionado, así como un script para empaquetarlo y generar las bibliotecas que se importan al IDE de Arduino.

### Organización
El proyecto está organizado en varias carpetas:

#### Carpeta `synestesia/main`
Esta carpeta contiene el código fuente distribuído en librerías escritas en C. Cada librería se ha diseñado para realizar una tarea concreta y lo más aislada posible.

#### Carpeta `synestesia/tests`
Esta carpeta contiene los programas de pruebas desarrollados para realizar las pruebas del código entregado, por sprints.
Puede resultar útil para analizar individualmente las partes del sistema, según se han desarrollado y comprobar que todo funcione correctamente.
