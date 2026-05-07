# Defensa Técnica: Arquitectura RISC 16-bit "Gaming Edition"

Este documento detalla las especificaciones técnicas y las decisiones de diseño que transformaron un procesador educativo básico en una unidad de procesamiento capaz de ejecutar videojuegos interactivos.

## 1. El Salto Arquitectónico: De 8 a 16 Bits
Originalmente, el procesador operaba con instrucciones de 8 bits basadas en un **Acumulador**. 
- **Limitación previa**: Con solo 8 bits para instruir al procesador, estábamos obligados a usar formatos muy simples (4 bits de OpCode y 4 de operando). Esto limitaba el espacio de direccionamiento y el número de registros, haciendo que cualquier cálculo complejo requiriera decenas de ciclos de acceso a RAM.
- **La Solución (16-bit RISC)**: Al expandir la instrucción a **16 bits**, pudimos implementar una arquitectura **Load/Store** moderna con **3 operandos**. 
    - **Ventaja**: Ahora podemos referenciar dos registros de origen y uno de destino en una sola línea (`ADD Rd, Rs1, Rs2`). Esto permite que el procesador realice en un solo ciclo lo que antes tomaba cuatro o cinco, proporcionando la potencia de cálculo necesaria para mover objetos en pantalla en tiempo real.

## 2. Microarquitectura: El Pipeline de 5 Etapas
Para maximizar el rendimiento (Throughput), implementamos un **Pipeline segmentado** de 5 etapas. Esto permite que el procesador esté procesando 5 instrucciones distintas al mismo tiempo:

1.  **IF (Fetch)**: Captura la instrucción de 16 bits de la memoria. Se utiliza una memoria de doble puerto para permitir que el Fetch no colisione con los accesos a datos.
2.  **ID (Decode)**: Se extraen los campos de la instrucción. Aquí reside el **Banco de Registros (R0-R7)**, que permite acceso instantáneo a los datos.
3.  **EX (Execute)**: El corazón del procesador. La **ALU** realiza operaciones aritméticas y lógicas. Además, aquí se resuelven los saltos (JMP/Branch) y se gestiona el **Forwarding**.
4.  **MEM (Memory)**: Etapa crítica para el gaming. Aquí el procesador se comunica con la RAM o con los **Periféricos (MMIO)** como el Framebuffer y el Teclado.
5.  **WB (Write-Back)**: Los resultados finales se escriben de vuelta en los registros para ser usados por las siguientes instrucciones.

## 3. Gestión de Riesgos y Forwarding
En un pipeline, una instrucción puede necesitar un dato que aún no se ha escrito en los registros (RAW Hazard). 
- **Decisión**: Implementamos una **Forwarding Unit**. En lugar de detener el procesador (stall), los datos se "inyectan" directamente desde la etapa MEM o WB de vuelta a la ALU. 
- **Impacto**: Esto elimina el "tartamudeo" del procesador, manteniendo una ejecución fluida, vital para que la lógica de un juego no sufra retrasos perceptibles.

## 4. Periféricos Mapeados en Memoria (MMIO)
La interactividad se logró tratando el hardware como si fuera memoria:
- **Framebuffer (Video)**: Los píxeles no son una "caja negra"; son direcciones de memoria. Esto permite al programador usar instrucciones estándar (`STORE`) para dibujar.
- **Direccionamiento Indirecto (LOADI/STORI)**: Añadimos soporte para punteros. Esto permite que el código pueda "iterar" sobre la pantalla (por ejemplo, para borrar la cola de la serpiente) de forma eficiente.
- **Entrada de Teclado**: Se lee como una dirección más, permitiendo que el bucle del juego sea reactivo sin interrumpir el flujo de procesamiento.

## 5. Características Clave del Microprocesador
- **Ancho de palabra**: 16 bits (instrucción) / 8 bits (datos).
- **Frecuencia ideal**: 1 instrucción por ciclo (CPI = 1).
- **Eficiencia**: 8 registros de alta velocidad que reducen el cuello de botella de la memoria RAM.
- **Programabilidad**: Ensamblador de dos pasadas con soporte para etiquetas, permitiendo código legible y estructurado.

## 6. Bitácora de Desafíos: Problemas y Soluciones

El camino hacia un procesador funcional no estuvo exento de retos técnicos. Estos son los errores más críticos encontrados y cómo se resolvieron:

### A. El Conflicto de Múltiples Drivers (Multiple Driver Error)
- **El Problema**: Al integrar el MMIO, tanto la memoria RAM como el controlador de periféricos intentaban escribir datos en el bus al mismo tiempo. Esto generaba una colisión de señales que bloqueaba la simulación.
- **La Solución**: Implementamos un **Multiplexor Combinacional** en la etapa MEM. Creamos una señal dedicada para la RAM (`s_ram_data_out`) y solo permitimos que el bus final tome ese valor si la dirección no pertenece al rango de periféricos.

### B. Riesgos de Datos (Data Hazards) en el Pipeline
- **El Problema**: Una instrucción intentaba usar un resultado que aún no se había guardado en los registros (porque la instrucción anterior todavía estaba en la etapa de memoria). Esto hacía que los cálculos fueran erróneos.
- **La Solución**: Desarrollamos una **Unidad de Forwarding**. En lugar de esperar al Write-Back, "puenteamos" el dato directamente desde las etapas EX/MEM y MEM/WB hacia la ALU. Fue un reto especial asegurar que instrucciones como `STORE` también recibieran el dato correcto mediante forwarding.

### C. Direccionamiento en el Loader
- **El Problema**: Originalmente, los saltos (`JMP/BEZ`) requerían direcciones manuales. Si añadíamos una sola instrucción al inicio, todas las direcciones de salto se rompían, haciendo el desarrollo de juegos como el Snake casi imposible.
- **La Solución**: Evolucionamos el cargador a un **Ensamblador de Dos Pasadas**. En la primera pasada, el sistema identifica todas las etiquetas (`labels`) y sus direcciones; en la segunda, traduce los mnemónicos. Esto nos permitió usar saltos simbólicos, dándonos la libertad de programar lógica compleja sin preocuparnos por las direcciones físicas.

### D. La Transparencia del Framebuffer
- **El Problema**: Al inicio, los datos no llegaban correctamente al framebuffer porque la ALU no propagaba la dirección inmediata en las instrucciones de carga y almacenamiento.
- **La Solución**: Ajustamos la lógica de la ALU para que, en operaciones de memoria, actúe como un canal transparente para la dirección, asegurando que el dato llegara exactamente al píxel deseado.

## Conclusión Final
Cada error fue una oportunidad para profundizar en la arquitectura. Este procesador no es solo el resultado de un diseño inicial, sino de un proceso iterativo de depuración y optimización. La capacidad de resolver estos conflictos de hardware en un entorno de simulación como SystemC es lo que garantiza la estabilidad actual de la "Gaming Edition".
