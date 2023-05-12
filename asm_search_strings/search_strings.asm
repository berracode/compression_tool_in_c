.data 
.align 2
buffer: .space 20000 # dirección de las palabras que escribirá el usuario
.align 2
archivo: .space 1024	# dirección de la url del archivo ingresada por el user
.align 2
archivoLimpio: .space 1024 #Dirección del archivo ingresado por el user pero sin el \n
.align 2
archivoSalida:	.asciiz  "archivoSalida.txt"     	# Archivo de salida


.align 2
bufferSalida: .space 69 # 


#mensajes 
pedirPlabras: .asciiz "\n Ingrese separadas por coma y sin espacio (palabra1,palabra2,palabran,)\n las palabras:\n "
pedirArchivo: .asciiz "\n Ingrese ruta del archivo: \n"
mensajeAparicion1: .asciiz " aparece en el archivo "
mensajeAparicion2: .asciiz " veces.\n"

mensaje1: .asciiz "\n retorno aquí"


cons1:  .word 32	# Caracter de espacio
cons2:  .word 13	# Caracter: Carriage Return

spaces: .asciiz ""     #aquí se almacenan los caracteres leidos del archivo.
.text

# Subrutina: main (Inicio del Programa)
main:
	jal	mainMenu # rutina principal, jump and link
	j 	exit    
# Subrutina: menu principal
mainMenu:
	addi $sp, $sp, -4	#Copia de seguridad de la dirección de la función que llama, para devolvernos en dado caso
	sw $ra, 0($sp)

# las palabras son las agujas que serán buscadas y contadas en el pajar (contenido archivo)	
solicitarPalabras:	
	#Iniciando temporales en 0 para volver a leer palabras en caso de incumplir			
	add $t0, $zero, $zero	
	
	li $v0, 4		# print string, $a0 = dirección de cadena terminada en nulo para imprimir
	la $a0,	pedirPlabras	# Mensaje para pedir las palabras
	syscall			# para que se ejecute el llamado al sistema

	li $v0, 8 		#read string,  
        la $a0, buffer		#$a0 = dirección del búfer de entrada (la dirección de "buffer" apuntará a las palabras)
        li $a1, 20000		#$a1 = número máximo de caracteres para leer
        syscall
       
        la $t0, buffer		#guardamos la dirección la dirección de memoria en el cpu, en el registro $t0
        
#
         
#solicitamos al usuario que ingrese la ruta del archivo
solicitarArchivo: 
	add $t1, $zero, $zero #guardaremos la dirección del buffer de entrada de la url del archivo
	add $t2, $zero, $zero #guardaremos la dirección donde almacenaremos el archivo sin \n (enter)
	add $t3, $zero, $zero #guardaremos la dirección de la url del archivo sucio
	add $t4, $zero, $zero #guardaremos la dirección de la url del archivo limpio
	add $t5, $zero, $zero #guardaremos la bandera \n (enter)
	add $t6, $zero, $zero #guardaremos el caracter leido de la url del archivo sucio
	
	li $v0, 4		# print string, $a0 = dirección de cadena terminada en nulo para imprimir
	la $a0,	pedirArchivo	#Mensaje de pedir archivo
	syscall

	li $v0, 8 		#leer string (para leer la url de la ruta del archivo)
        la $a0, archivo		#$a0 = dirección del búfer de entrada (la dirección de "buffer" apuntará a la url)
        li $a1, 1024		#Espacio maximo de cantidad de caracteres de la ruta del archivo
        syscall
        
        la $t1, archivo #cargamos indefinidamente en t1 la dirección de la url del archivo sucio
        la $t2, archivoLimpio #cargamos indefinidamente en t2 la dirección donde se alojará la url limpia
  	add $t3, $t1, $zero #hacemos una copia
  	add $t4, $t2, $zero #hacemos una copia
        addi $t5, $t5, 10  # para saber cuando hemos llegado al final de la cadena ingresada por el user
        
#remover de la la url o ruta o nombre del archivo el enter \n                
limpiarArchivo:	
	lbu $t6, 0($t3)	# $t6, almacena el caracter leido de t3, es decir caracter de la url que limpiaremos
	beq $t6, $t5, validarArchivo # si (caracter == \n, entonces finalizamos la lectura y validaremos si es un archivo correcto
	sb $t6, 0($t4) #cpu->memoria, enviamos el caracter leído y diferente de \n a la dirección del archivoLimpio
	addi $t3, $t3, 1 #avanzamos al siguiente caracter de la url que estamos limpiando
	addi $t4, $t4, 1 #avanzamos a una posición disponible para guardar el siguiente caracter en archivoLimpio
	j limpiarArchivo #iteramos	
	
#validamos si la ruta del archivo es correcta			
validarArchivo:
        
	li $v0, 13		#abrir archivo, v0 contiene el descriptor del archivo
	la $a0, archivoLimpio	#a0 = dirección del búfer de entrada (url limpia del archivo que ingresó el usuario)
	li $a1, 0			#Modo lectura
	li $a2, 0
	syscall
	
	add $s0, $v0, $zero		#guardamos en s0 el descriptor
	slt $t1, $v0, $zero		#si (v0 < 0)? t1=1: t1=0;
	bne $t1, $zero, solicitarArchivo #Si no encuentra el archivo, vuelve a preguntar por archivo, si( t1 != 0) solicitarArchivo
	
	#El archivo existe! Copiar datos a memoria
	#Lectura del archvo
	li $v0, 14   		# lee datos desde el archivo
	add $a0, $s0, $zero	# descriptor del archivo a0 = s0
	la $a1, spaces		# dirección del buffer de entrada (hace referencia a la dirección de memoria donde inicia el contenido 
	li $a2, 20000		# cantidad masixma de caracteres que serán volcados del archivo a memoria
	syscall
	
	add $s2, $v0, $zero	# guardamos la cantidad de caracteres
	add $a0, $s0, $zero	# pasamos el descriptor 
	li $v0, 16		# cerrar archivo
	syscall			
	
	add $s0, $s2, $zero	#Numero de caracteres
	add $s1, $a1, $zero	#Base del buffer del contenido del archivo
	add $t1, $s1, $zero	#hacemos una copia para recorrer el buffer
	
#contarCaracteres:
#	lbu $t2, 0($t1)		#$t2 = $s0[$t1]
#	beq $t2, $zero, inicializacion	#Si $t2 = '\0' terminar de contar
#	addi $t1, $t1, 1	#$t1 = $t1 + 1
#	addi $s0, $s0, 1	#$s0 = $s0 + 1, contador de caracteres
#	j contarCaracteres

inicializacion:
	addi $sp, $sp, -20 	# Separo en el Stack de la memoria 20 bytes
	#sw $s2, 16($sp)
	sw $s3, 12($sp)
	sw $s4, 8($sp)
	sw $s5, 4($sp)
	sw $s6, 0($sp)
	
	#addi $s2, $v0, 1	# $s2 contiene el limite de caracteres provistos por el ususario, +1 para futuras comparaciones 
	addi $s3,$zero,0		# Iterador a utilizar para el pajar
	addi $s4,$zero,0		# Contador de coincidencias de una aguja en el pajar
	addi $s5,$zero,0		# Iterador a utilizar para las agujas
	addi $s6,$zero,0	# Contador de palabras con más de "n" caracteres 
	addi $s7,$t0,0 		#guardamos nuestra dirección inicial de las agujas
	
	add $t9, $zero, $zero
	
	lw $t2, cons1		# Cargo el valor de 32 en la variable temporal $t2
	lw $t3, cons2		# Cargo el valor de 13 en la variable temporal $t3

#es para iterar las cadenas ingresadas por el usuario.		
bucleAgujas:
	add $t0, $s7, $s5 	#t0=s7, es decir ponemos en t0 la dirección desde donde emepzaremos a validar las agujas
	lbu $t4, 0($t0)		#t4 contiene el caracter ---- ab,o,c, ---- abcdario (9)
	li $t6, 10		# t6 contiene una nueva linea
	bne $t4, $t6, buscandoCoincidencia #validamos si el caracter != de nueva linea
	j exit	

#nos sirva para iterar el pajar y la aguja, pero llegamos aquí siempre y cuando no necesitemos  iterar a la aguja siguiente
buscandoCoincidencia:
	
	slt $t0, $s3, $s0  	# Verifico si llege al último caracter: si(s3<20)?t0=1: t0=0
	beq $t0,$0,imprimir	# Verifico si ya leí todos los caracteres, si(t0=0)?finalicé: continuo
	addi $t0, $s7, 0	#volvemos a asignar a t0 la dirección de las agujas
	j validarAguja	

	
	
#validar caracter a caracter la aguja con el contenido del archivo. 
validarAguja:
		
	add $t1, $s1, $s3 	#t1 = s1, es decir t1 tendra la dirección de pajar + s3
	add $t0, $s7, $s5	#t0 = s7, es decir t1 tendra la dirección de las agujas + s5
	lb $t4, 0($t0)		# Cargo el i-esimo byte de la aguja en $t4
	lb $t5, 0($t1)		# Cargo el i-esimo byte del pajar en $t5
	addi $s3, $s3, 1	#aumentamos iteradores
	addi $s5, $s5, 1
	beq $t4,$t5,validarAguja # t4 == t5)?validarAguja: continuar
	li $t6, 10		# asignamos nueva linea a t6 "\n"
	beq $t4, $t6, contarAparicion #verifico si t4 == nueva linea
	li $t6, 44		# asignamos a t6 una coma , ","
	beq $t4, $t6, contarAparicion #verifico si (t4 == ,)?contarAparicion: continuar
	j resetearPunteroAguja

#aumentamos el contador de apariciones
contarAparicion:
	addi $s4, $s4, 1 

#para resetear el iterador y garantizar que volveremos a leer la aguja desde su primer caracter
resetearPunteroAguja:
	add $s5, $zero, $zero 
	j buscandoCoincidencia

#subrutina para empezar a imprimir en el archivo la aguja y su cantidad de apariciones
imprimir:
# imprimire en un nuevo archivo el valor de las coincidencias de la aguja n-esima
	j leerHastaComa
#luego llamaré al metodo siguienteaGUJA y luego iniciará el ciclo siempre y cuando no llegue a null. 
	

#obtenemos la aguja que vamos a imprimir (hasta la coma, porque despues  sigue otra aguja)
leerHastaComa:
	addi $sp, $sp, -12
	sw $v0, 8($sp) 	#copia de v0
	sw $s1, 4($sp) 	#copia de la dirección del pajar
	sw $s0, 0($sp)	#copia de la cantidad de caracteres
	
	#Abre archivo de escritura
	li $v0, 13 #abrir archivo
	la $a0, archivoSalida #dirección de cadena terminada en nulo que contiene el nombre de archivo
	li $a1, 9 #escritura, y si no existe lo crea y no sobreescribe el contenido
	li $a2, 0 #ignora el modo
	syscall
	
	add $s0, $v0, $zero		#Descriptor de "archivoSalida.txt" guardado en s0
	
	#Variables necesarias para escribir
	addi $t2, $zero, 44		#Limite , hasta donde leeremos, asiganos a t2 una coma ","
	add $t0, $s7, $zero		#t0 = s7, es decir la ponemos a apuntar al inicio de la dirección de agujas
	add $t8, $zero, $zero		#inicializamos en 0 a t8
	
	add $a0, $s0, $zero		#copia del descriptor
	j escribirAguja
	
#aquí emepzamos a leer la aguja para imprimirla
escribirAguja:				
	li $v0, 15			#escribir en el archivo
	la $a1, 0($t0)			#Direccion del buffer de salida, a1 = t0[0]
	li $a2, 1			#Longitud a escribir (1 caracter a la vez)
	lbu $t8, 0($t0)			#t1 = buffer[$t0]. Lo copia para luego comparar con t6 y t2
	addi $t0, $t0, 1		#$t0++
	syscall				#Escribe en el archivo el caracter leido
	
	li $t6, 10		#asiganams a t6 nueva linea
	beq $t8, $t6, escribirResultado# Verifico si el caracter leido es un "\n" 
	bne $t8, $t2, escribirAguja # comparo si no es coma (,) para avanzar a otro caracter
	jal escribirResultado   # el que escribe las veces que se repite
	
	li $v0, 4		#imprime en consola, sólo es para saber cuantas agujas a recorrido
	la $a0,	mensaje1	#guia
	syscall
	
	#Cerrando archivo escritura
	add $a0, $s0, $zero
	
	li $v0, 16
	syscall			#Cierra "archivoSalida.txt"

	#volvamos a cargar los valores guardados previamente en memoria a la cpu
	lw $s0, 0($sp)  # vuelvo a car
	lw $s1, 4($sp)
	lw $v0, 8($sp)
	addi $sp, $sp, 12
	add $t1,$s1,$zero #asignamos nuevamente a t1 la dirección del pajar
	add $s4, $zero, $zero #reiniciamos el contador de apariciones
	add $s3, $zero, $zero #reiniciamos el iterador para volver a leer el contenido desde el inicio
	j bucleAgujas

#escribira la aguja y su cantidad de apariciones														
escribirResultado:
		
	li $v0, 15
	la $a1, mensajeAparicion1		#Direccion del buffer
	li $a2, 25			#Longitud a escribir (1 caracter a la vez)      ,kkkk,kk,
	syscall	
	
	li $v0, 15
	la $a1, mensajeAparicion2		#Direccion del buffer
	li $a2, 8			#Longitud a escribir (1 caracter a la vez)
	syscall	
	
	add $s7, $t0, $zero 		#s7 toma el valor de t0 para guardar la dirección de la aguja siguiente
	add $v0 , $v0, $zero		
	jr $ra			#retornar valor o regresar donde me invocaron
	
         
exit: 	li $v0, 10		# Constante para terminar el programa
	syscall		
