# ALgoritmo en ensamblador MIPS toma una cadena (DataIn) y la envia como argumento a un procedimiento para extraer y contar la cantidad de letras
# las letras pueden ser mayusuculas o minusculas, luego retorna la ejecución al procedimiento principal e imprime la cadena extraida y la cantidad de letras.

.data
DataIn:	.asciiz "mam4==-...= p4pa C0mo 3ST4n ????"
Letters: .space 1024 # reservamos 1024 byte
message: .asciiz "\nla cantidad de letras es: "

.text

#4 tab

main:
	
	la $a0, DataIn				# dirección de datain en a0
	la $a1, Letters				# dirección de Letters en a1

	
	jal findLettersAndCountit		# hacemos un salto al procedimiento
	move $s0, $v0				# s0 = v0
	
	li $v0, 4				# v0 = 4 para syscall 
	la $a0, Letters				# Cargamos en a0 la direccion de la cadena Letters
	syscall					# syscall de impresion
	
	li $v0, 4				# v0 = 4 para syscall 
	la $a0, message				# Cargamos en a0 la direccion de la cadena Letters
	syscall					# syscall de impresion
	
	li $v0, 1          			# cargamos el valor 1 en v0 para imprimir un entero
	move $a0, $s0      			# movemos s0 hacia a0
	syscall            			# syscall de impresion
	
	j exit					# terminamos


findLettersAndCountit:

	li $v0, 0				# inicializamos nuestro contador de letras en 0	

    	loop:					# bucle para recorrer DataIn
        	lb $t0, ($a0)			# cargamos en t0 un caracter de a0 (DataIn) => llevar de memoria a registros
        	beqz $t0, endLoop		# salimos dle bucle si hemos llegado al final de la cadena.
        	
        	slti $t1, $t0, 91		# si t0 es menor 91, entonces t1 = 1
        	sge $t2, $t0, 65		# si t0 es mayor que 65, entonces t2 = 1
        	and $t1, $t1, $t2		# si t1 and t2, entonces t1 = 1 = ES MAYUSCULA
        	
        	slti $t3, $t0, 123		# si t0 es menor 123, entonces t3 = 1
        	sge $t4, $t0, 97		# si t0 es mayor o igual que 97, entonces t4 = 1
        	and $t3, $t3, $t4		# si t1 and t2, entonces t3 = 1 =  ES MINUSCULA
        	
        	or $t5, $t1, $t3		# comprobamos si t1 o t2, entonces t5 = 1, significa que o es minuscula o es mayusucula.
        	
        	beqz $t5, nextChar

        	sb $t0, ($a1)			# almacenar el caracter t0 en a1 (Letters)
        	addi $a1, $a1, 1		# incrementamos para no sobreescribir
        	
        	addi $v0, $v0, 1		# Incrementar el contador de letras
        	nextChar:			# next char

        	addi $a0, $a0, 1		# incrementamos para ir por la otra letra de datain
        	j loop
        	
    	endLoop:
    	
    	sb $zero, ($a1)				# Escribir el byte nulo al final de la nueva cadena

    	jr $ra					# Retornar el número de letras encontradas en dataIN
    	
exit:
	li $v0, 10				# terminamos esta vaina
	syscall
	
	