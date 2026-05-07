# Defensa de la Arquitectura: RISC v2 Gaming Edition

Este procesador ha sido diseñado con un objetivo claro: **trascender el cálculo teórico y permitir la ejecución de software interactivo (videojuegos).** A continuación, se justifican las decisiones arquitectónicas clave tomadas durante el desarrollo.

## 1. Evolución de 8 a 16 bits
Aunque la memoria de datos sigue siendo de 8 bits (suficiente para una pantalla pequeña), la **ISA se expandió a 16 bits**.
- **Por qué**: Una instrucción de 8 bits era demasiado limitante (solo 4 bits para opcode y 4 para operandos). Al pasar a 16 bits, pudimos implementar una arquitectura de **3 operandos** (`Rd, Rs1, Rs2`), lo que reduce drásticamente el número de instrucciones necesarias para cálculos complejos (como la física de un juego), mejorando el rendimiento por ciclo.

## 2. Implementación de Pipeline de 5 Etapas
El salto de un diseño secuencial a un **Pipeline RISC** fue fundamental para la interactividad.
- **Por qué**: Un videojuego requiere una tasa de refresco constante. El pipeline permite que el procesador trabaje en 5 instrucciones simultáneamente. Gracias a la **Forwarding Unit**, eliminamos casi todos los "stalls", permitiendo que el bucle del juego (leer teclado -> mover -> dibujar) sea fluido y eficiente.

## 3. Direccionamiento Indirecto (LOADI / STORI)
Esta es la adición más crítica para el desarrollo de juegos.
- **Por qué**: En un videojuego, los objetos (como la comida del snake o las paredes) cambian de posición. Sin `LOADI/STORI`, el programador tendría que hardcodear cada dirección de memoria. Con estas instrucciones, podemos usar **punteros** (registros que guardan direcciones), lo que permite manipular el Framebuffer de forma dinámica y crear estructuras de datos como arrays para el cuerpo de la serpiente.

## 4. Memory Mapped I/O (MMIO)
En lugar de crear instrucciones complejas de "dibujar", mapeamos el hardware directamente a la memoria.
- **Por qué**: 
    - El **Framebuffer (0xC0-0xEF)** permite que cualquier `STORE` en ese rango se convierta automáticamente en un píxel en pantalla. Es el estándar de la industria (como en la GameBoy o NES).
    - El **Registro de Teclado (0xF0)** permite una entrada asíncrona. El procesador simplemente consulta una dirección de memoria para saber qué tecla pulsó el usuario, sin detener el reloj del sistema.

## 5. El Salto Condicional y el Delay Slot
Decidimos resolver los saltos en la etapa **EX** con un **delay slot** de un ciclo.
- **Por qué**: Esto equilibra la complejidad del hardware con la velocidad. Permite que el procesador no pierda ciclos "adivinando" hacia dónde saltar (branch prediction), manteniendo el diseño estructuralmente simple pero predecible para el programador de ensamblador.

## Conclusión
Cada compuerta lógica y cada etapa del pipeline ha sido optimizada para que este procesador no sea solo una calculadora, sino una **consola de videojuegos funcional**. La arquitectura resultante es un equilibrio perfecto entre la elegancia del RISC académico y la potencia práctica del hardware de gaming clásico.
