# Sistemas Operativos
=====================

### ITBA Cuatrimestre 1 - 2012


Filesystems, IPCs y Servidores Concurrentes
===========================================

### Objetivo

El objetivo de este trabajo es familiarizarse con el uso de sistemas cliente- servidor concurrentes, implementando el servidor mediante la creaci ́on de proce- sos hijos utilizando fork() y mediante la creaci ́on de threads. Al mismo tiempo, ejercitar el uso de los distintos tipos de primitivas de sincronizaci ́on y comuni- caci ́on de procesos (IPC) y manejar con autoridad el filesystem de Linux desde el lado usuario.

## Enunciado

La misión en este trabajo práctico es crear una aplicaci ́on cliente servidor de una liga de fantas ́ıa. Los clientes deber ́an poder registrarse a la aplicaci ́on y crear o unirse a ligas, creando en ellas equipos. Luego de que una liga est ́e llena, los clientes podr ́an acceder al draft de esa liga y, por turnos, seleccionar los jugadores para sus equipos. Una vez que todos los jugadores hayan sido drafteados, la liga se convertir ́a en activa. Sobre una liga activa, los clientes podr ́an tradear jugadores entre s ́ı y contratar jugadores de la lista de agentes libres (los que hayan sobrado del draft) reemplazando a uno de sus jugadores a cambio. Finalmente, el administrador deber ́a poder cargar partidos, que asig- nar ́an puntos a los jugadores y a aquellos equipos que dispongan de ellos al momento de la carga del partido.
La simulaci ́on tendr ́a un archivo de configuraci ́on con la lista de jugadores a draftear y varios con partidos a cargar

El servidor deber ́a soportar los siguientes comandos: 

* [register (user, password)] --  Crea una cuenta para acceder a la liga.
* [login (user, password)] --  Accede a su cuenta. *[list [leagues—teams—trades]: Accede a la lista de ligas, equipos o trades.
1
* [league show (leagueId) Muestra la tabla de posiciones de una liga y los puntajes individuales de los jugadores drafteados por esa liga
* [team show (teamId)] --  Muestra informaci ́on sobre el equipo, la liga a la que pertenece, los jugadores que drafte ́o, el puntaje individual de cada uno y su puntaje total.
* [trade show (tradeId)] --  Muestra informaci ́on sobre el trade, de que equipo a que equipo es, y qu ́e jugadores interactu ́an en el intercambio.
* [trade (teamId)] --  Genera un trade con ese equipo (solo en caso de tener un equipo en la misma liga que el equipo con quien se tradea)
* [trade withdraw (tradeId)] --  Cancela un trade creado por uno mismo
* [trade accept (tradeId)] --  Aceptar un trade del cual el usuario es destinatario.
* [trade negotiate (tradeId)] --  Cancela un trade del cual se es destinatario y crea uno nuevo como contraoferta.

Los partidos se cargar ́an copiando el archivo correspondiente a la carpeta de partidos. El servidor deber ́a estar monitoreando el directorio constantemente para detectar nuevos partidos.

### Actividades

1) Implemente la simulaci ́on utilizando procesos y threads y haga cuatro ver- siones del sistema, usando las siguientes primitivas de IPC:

* [Pipes o FIFOs]
* [Colas de mensajes - System V o POSIX]
* [Memoria compartida - System V o mmap(). Semáforos - System V o POSIX].
* [Sockets - TCP o de dominio Unix]

2) Criterios de Implementación

Los programas correr ́an en ambiente Linux. En su realizaci ́on se requiere:
Buena modularizaci ́on Buen estilo de programaci ́on Documentaci ́on adecuada Makefile para la construcci ́on.
Estos factores se toman en cuenta para la evaluaci ́on. Los trabajos podr ́an hacerse en grupos de no m ́as de cuatro integrantes.

### Material a entregar
Cada grupo deber ́a entregar todos los fuentes del trabajo debidamente comen- tados, junto a un makefile para compilarlos en un ambiente linux. Adem ́as deber ́an entregar un informe impreso detallando las decisiones respecto a los items que quedaron a elecci ́on del grupo, problemas presentados y soluci ́on de los mismos durante la realizaci ́on del trabajo

### Consideraciones

Todo punto no expl ́ıcito en este documento podr ́a ser interpretado a conve- nieniencia del alumno, siempre dentro de los m ́argenes del sentido comu ́n. Ante la duda, consultar a los docentes o enviar un mail a la c ́atedra.

### Fecha de entrega
La fecha de entrega del trabajo pr ́actico es el Lunes 9 de Abril a las 16:00.



