# Secreto compartido con esteganografía

## Descripción
En este Trabajo Práctico de Implementación se desarrolló una herramienta que permite 
ocultar y recuperar imágenes BMP. La misma se basa en un esquema de secreto compartido
(k, n), en dónde la presencia de al menos k imágenes portadoras permite la recontrucción
de otra imagen oculta entre las portadoras mediante esteganografía.

## Compilación
Para crear el ejecutable, es suficiente correr `make` desde la raíz del proyecto.
Esto generará un directorio `build` con todos los archivos objeto, y además crea sobre
la raíz del proyeco un ejecutable llamado `ss`. En el siguiente apartado se detalla el 
uso de dicho ejecutable. Todos los archivos objeto y ejecutables pueden ser eliminados
utilizando el comando `make clean` desde la raíz del proyecto.

## Ejecución
Para utilizar la herramienta, se debe contar primero con una imagen BMP para esconder
y un directorio con otras imagenes BMP (distintas de aquella que será escondida), que
serán las portadoras en el esquema de secreto compartido (k, n). El k será un parámetro
del programa, mientras que el n se infiere al momento de distribuir según cuántas
imágenes BMP se encuentran en el directorio.

### Distribución
Para distribuir, se debe correr: `./ss d <secret> k <directory>`, en dónde:
* _d_ representa que se quiere distribuir
* _\<secret>_ es la imagen secreta que se desea distribuir. Debe estar en formato BMP
* _k_ es el umbral del esquema de secreto compartido
* _\<directory>_ es el directorio en donde se encuentran las imágenes que serán
portadoras una vez finalizada la ejecición

De suceder que:
1. _\<secret>_ no se pueda leer
2. La cantidad de imágenes BMP que se pueden leer en _\<directory> sea menor que _k_

El programa abortará, mostrando un mensaje de error representativo. 

### Recuperación
Para recuperar, se debe correr: `./ss r <secret> k <directory>`, en dónde:
* _r_ representa que se quiere recuperar
* _\<secret>_ es donde se guardará la imagen recuperada
* _k_ es el umbral del esquema de secreto compartido
* _\<directory>_ es el directorio en donde se encuentran las imágenes portadoras

De suceder que:
1. _\<secret>_ no se pueda escribir
2. La cantidad de imágenes BMP que se pueden leer en _\<directory>_ sea menor que _k_

El programa abortará, mostrando un mensaje de error representativo. 

(Nota: tanto en la recuperación como en la distribución puede suceder que el programa 
no pueda acceder a algunos de los archivos de _\<directory>_. Si puede acceder a al 
menos _k_, continuará la ejecución y asumirá acordemente el valor de _n_.)
