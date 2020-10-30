# Trabajo práctico Fat12 de Sistemas Operativos y Redes II

El trabajo consiste en leer el MBR de Fat12, las Fat12Entries del Root Directory, e incluye una funcionalidad para el recupero de archivos borrados.

> Mas sobre Fat12:  [FAT12 ](http://www.c-jump.com/CIS24/Slides/FileSysDataStructs/FileSysDataStructs.html)
## Instalación

Asegurese montar/desmontar el fs 'test.img' cada vez que genere/borre archvos, ya que no hay garantias que los cambios se vean reflejados de manera instantanea.

- Clone el repositorio ***git clone https://github.com/vanesadougan/tp1_Fat.git***
- Moverse al directorio del proyecto ***cd tp1_Fat/*** 
- Ejecutar ***make all***
- Leer MBR -> ***./read_boot*** 
- Leer FileEntries del root directory ->***./read_root*** 
- Leer el contenido de las FileEntries del root directory -> ***./print_file***
- Recuperar archivos borrados -> ***./recovery*** 
