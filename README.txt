- El comando "echo" devuelve un salto de linea si no recibe argumetos,
tambien es capaz de gestionar multiples opciones (aunque solo "-n" esta disponible).

- La expansión de variables sin comillas ahora se realiza antes de tokenizar
permitiendo la ejecucion de comandos complejos desde esta.

- El nuevo archivo var_expander_01.c deberia pasar Norminette.

- El prompt ya no imprime "~" si se imprime ruta completa o si se encuentra antes de $HOME.

- Makefile completamente corregido, ya imprime solo cuando debe.

- "fork_bomb" ahora se salta los espacios previos al comando.

- El .h limpio y ordenado. Revisar Norminette por el largo de las lineas y por
las tabulaciones. Valorar dividirlo en varios .h por claridad de código.

---------------------------------------------------------------------------------------------

- Valorar busqueda del archivo .secret para disponer del ascii art desde cualquier ubicación.

- Valorar la gestión de la variable global PATH en ejecución sin env.
(gnl en /etc/env... y asignar a variable)

- (Comprobar testers.. creo que es cosa de mi PC; ahora sin tocar el código, todo OK.)
Ⱦɍⱥꞩ łꝋꞩ ᵾłⱦīᵯꝋꞩ ȼⱥᵯƀīꝋꞩ ēł ⱦēꞩⱦ đⱥ ēɍɍꝋɍ ēꞥ ēł ȼꝋđīꞡꝋ đē đēꝟꝋłᵾȼīꝋꞥ đē "ēӿꝑꝋɍⱦ Ⱥ-" ⱥᵾꞥꝗᵾē ⱦɍⱥꞩ
ȼꝋᵯꝑɍꝋƀⱥɍłꝋ ēꞥ ⱦēɍᵯīꞥⱥł đēꝟᵾēłꝟē łꝋ ꝗᵾē đēƀē. Ꞧēꝟīꞩⱥɍ ɏⱥ ꝗᵾē ꞥꝋ ꞩē ħⱥ ⱦꝋȼⱥđꝋ ēł ȼꝋᵯⱥꞥđꝋ ēӿꝑꝋɍⱦ. 
Ⱥ ꝟēȼēꞩ đⱥ ꝋⱦɍꝋꞩ ēɍɍꝋɍēꞩ ꝑēɍꝋ ȼꝋᵯꝑɍꝋƀⱥꞥđꝋ ȼꝋđīꞡꝋ ⱦꝋđꝋ ӻᵾꞥȼīꝋꞥⱥ ȼꝋᵯꝋ đēƀē.

- Revisar codigos de ejecución correcta y de error de los builts in.

- Revisar todas las anotaciones marcadas con "OJO" en el código.
