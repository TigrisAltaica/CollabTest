
1212121212what???
1. Trabajo colaborativo

El flujo de trabajo que hasta ahora hemos visto es:

    > git init                  # inicializa un repositorio
    > git add <archivo>         # da a conocer a git un archivo nuevo o un cambio en un archivo
    > git status                # resume los cambios actuales
    > git commit -m "mensaje"   # saca una foto instantánea del estado actual del proyecto
    > git push                  # sube los cambios a un repositorio central (local o remoto)
    > git log                   # muestra la bitácora del proyecto

(Por ahora ">" indicará la línea de comandos.)

Ahora veremos un modelo posible de colaboración, que iremos complicando poco a poco.

Para empezar, la instrucción que sirve para hacer una copia local de un repositorio remoto es:

> git clone https://github.com/Usuario/proyecto.git

En la instrucción de arriba, hemos considerado un proyecto remoto que está en GitHub, por ejemplo. Sin embargo, el comando puede usar en otras situaciones, por ejemplo, un proyecto remoto en una máquina a la que tenemos acceso con ssh, o git, o un proyecto en otro directorio.

Entre otras cosas que quedan configuradas cuando uno hace la clonación de un proyecto, es dónde se encuentra el proyecto original (origin). Para verificar esto usamos:

    > git remote -v

La situación que consideraremos es la siguiente: Alicia (Alice) y Beto (Bob) colaboran en un proyecto (el que acabamos de clonar). Ambos tienen la misma versión del código.

Alicia: Alicia edita el archivo archivo.txt, y hace algún cambio que le parece conveniente. Siguiendo el esquema de trabajo que describimos arriba, Alicia sube los cambios a su repositorio local con git add y git commit, y finalmente los sube al repositorio central: git push.

Beto: Beto, por su parte y de manera independiente, hace cambios al mismo archivo en que trabajó Alicia. De la misma manera que lo hizo Alicia, Beto actualiza su repositorio local (git add y git commit) y los sube al repositorio que comparten con git push.

Sin embargo, como él editó el mismo archivo en el que Alicia hizo cambios, pero usando una versión atrasada que no incluye los cambios de Alicia, entonces git detecta que hubo cambios divergentes entre la versión local de Beto, en la rama master, y la del repositorio remoto origin/master. Esto hace que git no permita subir los cambios que propone Beto, hasta que Beto resuelva los conflictos que hayan surgido.

-----

 

**Ejercicio 1:** Trabajando en grupos de dos al menos, traten de reproducir la situación descrita arriba. La pregunta concreta es qué significa eso del conflicto que se debe resolver. 

 

1. Para llevar esto a cabo, una posibilidad es a partir de lo que hicimos la vez pasada. Sin embargo, hay ciertas sutilezas que tienen que ver con que `git init` por default crea lo que se llama un repositorio *non-bare* (no vacío), y git no permite subir los cambios a un repositorio no vacío. Suponiendo que el repositorio que creamos anteriormente está en `~/Documentos/claseLuisDavid`, entonces:

 

    (a) Primero crearemos un proyecto *bare* (vacío) a partir del anterior:

    ```

        > git clone --bare -l ~/Documentos/claseLuisDavid repo_vacio

    ```

    (Si inspeccionan el repositorio `repo_vacio` verán que contiene los archivos que 

    normalmente se encuentran en el directorio escondido ".git", y nada más, o sea, no 

    tiene los archivos propios del proyecto.)

 

    (b) A partir de este repositorio vacío, clonaremos a dos directorios independientes 

    (`Alicia/` y `Beto/`):

    ```

        > git clone repo_vacio Alicia

        > git clone repo_vacio Beto

    ```

 

    (c) Desde el directorio `Alicia/` hagan un cambio importante y súbanlo al repo 

    (`git push`); traten ahora de hacer lo mismo desde `Beto/`

 

2. La segunda opción es clonar *dos* veces el repo que subieron a GitHub, a dos máquinas distintas o a dos directorios distintos. En este caso hay que adecuar la instrucción (b), usando la dirección del repo en GitHub. Otra sutileza es que el proyecto que clonan les debe *permitir* subir los cambios a *ambos* usuarios (en el caso de que hayan clonado a cuentas distintas o a máquinas distintas). Esto se puede configurar en los `Settings` del repo en GitHub; para que esto funcione *ambos* usuarios deben estar dados de alta en GitHub.

 

(A la larga, la segunda opción es más útil que la primera.)

 

-----

 

Para entender el problema, ejecutaremos la instrucción

> git log --oneline

lo que muestra el último cambio que hizo Beto. Por otro lado, para ver cómo está el repositorio remoto hacemos:

> git log --oneline origin/master

La segunda instrucción no muestra los cambios de Alicia. Esto muestra que el repositorio de Beto no está actualizado respecto al repositorio central.

La manera en que Beto debe resolver el conflicto es, pues, actualizando su versión local respecto al repositorio remoto. Esto se hace usando:

> git fetch origin

y para ver el estado del repositorio

> git log --oneline origin/master

Entonces, para resolver el problema, Beto ha de implementar los cambios del repositorio remoto en su repositorio primero, ya que el de referencia es siempre el remoto (rama master):

> git merge origin/master

lo que hace manifiesto, nuevamente, el conflicto.

Ejercicio 2: Edita el archivo con conflictos, y resuélvelos. Después haz git add y git commit. ¿Puedes subir (push) los cambios al repositorio central?

La moraleja de esto es: antes de hacer cualquier cambio, hay que mantenerse actualizado respecto al repositorio central. Esto se hace con la combinación git fetch y git merge cuando sea necesario, en particular, antes de subir algún cambio.

Una manera "corta" y combinada de hacer los dos pasos arriba descritos (git fetch y git merge) es con el comando:

> git pull

Ejercicio 3: Actualicen el directorio Alicia/ respecto a los últimos cambios hechos por Beto.
2. Trabajando en ramas

El concepto de una rama ("branch") en git provee una forma sencilla y eficiente de trabajar en nuevas ideas, o de colaborar en un proyecto común, evitando romper cosas que a priori ya funcionan.

Para empezar, listemos las ramas existentes de un proyecto (por ejemplo, en el directorio Alicia/):

    > git branch

o usando

    > git branch -v

que brinda además el hash del último commit. Lo que esto indica es que existe únicamente la rama master, que es la rama que se crea por default (y en algún sentido es la principal), y el asterisco indica que estamos trabajando en esa rama.

Para crear una nueva rama, ejecutamos:

    > git branch <nombre_rama>

donde <nombre_rama> es el nombre de la rama, que es más o menos arbitrario y flexible. Un ejemplo es: git branch alicia, que es el que usaré en este ejemplo; otra posibilidad podría ser git branch alicia/nuevaidea.

Después de ejecutar alguna de estas instrucciones, git branch -v nos informa que ambas ramas, master y alicia existen, ambas están en el (mismo) último commit, y el asterisco indica que estamos en la rama master aún.

Para cambiarnos de rama, ejecutamos:

    > git checkout <nombre_rama>

Nuevamente, existe un atajo para crear y cambiarnos de rama de un golpe: git checkout -b <nombre_rama>.

Ejercicio 4: (a) Creen una rama, cámbiense a la nueva rama, y verifiquen que están en la nueva rama. (b) Pregunta: la instrucción git status, ¿da alguna información sobre en qué rama están?

Ejercicio 5: Hagan algunos cambios en el repositorio, tanto en los archivos que ya existen y creando un nuevo archivo y, una vez terminados, guarden este punto en la historia del desarrollo. ¿Cómo pueden verificar que el branch alicia donde hicieron los cambios tiene al menos un commit más que el branch master?

Ejercicio 6: (a) Cámbiense de rama a master. ¿Qué pasó con los cambios? (b) Vuelvan a cambiarse al branch alicia, y respondan la misma pregunta que antes.

El punto importante hasta el momento es que la historia de los dos branches (locales) ha divergido, y ambas historias están en ambas ramas.

Supongamos ahora que ya están satisfechos con los cambios que han hecho, después de muchas pruebas exhaustivas y otras fallidas (tal vez en otras ramas). Ahora queremos poner estos cambios en la rama master. Para esto, primero nos cambiamos a master, que es la rama a donde queremos pasar los cambios, y después hacemos un merge, o sea, fundimos las dos historias:

    > git checkout master   
    > git merge <nombre_rama>

Ejercicio 7: Pasen los cambios de alicia a master.

Ejercicio 8: Ya que los cambios que hicimos en alicia están en master, borren la rama alicia. Para esto, la instrucción git branch -d alicia es particularmente útil.

Ejercicio 9: Usen LearnGitBranching para jugar con esto y ver gráficamente qué significan las ramas y la divergencia de las historias. En esta misma liga hay otros tutoriales que pueden ser interesantes.
3. Resumen: Flujo colaborativo con git y GitHub
3.1 Iniciar un proyecto

    Crear un repositorio local de git en tu máquina
    Crear un repositorio en GitHub
    Empujar/subir los cambios de tu repositorio local a tu repositorio en GitHub

Nota: Vale la pena incluir un archivo LICENSE.md, donde definen la manera en que uno puede usar el contenido de su proyecto. Para código, se recomienda la licencia MIT.
3.2 Contribuir a otro proyecto en GitHub

    Desde la página (en GitHub) de algún proyecto al que quieras contribuir, aprieta el botón "Fork". Esto creará una copia del repositorio ajeno en tu cuenta de GitHub. >=D
    Clona este repositorio (el que está en tu cuenta de GitHub) a tu máquina.
    Si haces cambios al proyecto (en tu repositorio local, desde una rama distinta a master, y quieres que éstos se evalúen para usarse en el proyecto colaborativo central, debes primero actualizar tu fork (en GitHub) con tus cambios. Para esto, subirás tus cambios a tu repositorio en GitHub usando git push origin <mi_rama>, donde <mi_rama> es la rama donde hiciste los cambios que quieres subir.
    Ahora puedes hacer una petición para subir tus cambios (pull request) al proyecto central, directamente desde la página de GitHub de tu versión del repositorio, o usando el comando hub pull-request en la línea de comandos si tienes instalado hub (una versión extendida de git que funciona 
1212121212what???
cb26a04437b2d5a4f60487ed65bba7c0eb0c9cd4
 4dc513838ea41c26b30cddf21cb9b17afaed1c59
