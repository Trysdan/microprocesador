# Microprocesador RISC v2 Gaming Edition (SystemC)

Procesador estructural de 16 bits con Pipeline de 5 etapas, diseñado específicamente para soportar videojuegos retro mediante **Memory Mapped I/O (MMIO)**.

## Características Principales
- **Arquitectura RISC**: 16 bits con pipeline de 5 etapas (IF, ID, EX, MEM, WB).
- **Video & Input**:
  - **Framebuffer 8x6** integrado en memoria (0xC0-0xEF).
  - **Captura de Teclado** no bloqueante vía MMIO (0xF0).
- **ISA Extendida**: Soporte para punteros mediante **LOADI** y **STORI** (direccionamiento indirecto).
- **Gestión de Riesgos**: Forwarding Unit completa y Hazard Detection Unit.
- **Ensamblador**: Soporte para etiquetas y carga automática de programas `.asm`.

## Cómo Jugar (Demo Snake)
Asegúrate de estar en un entorno con SystemC instalado (o vía WSL):
```cmd
wsl -e bash -c "cd build && make microprocessor_v2 && ./microprocessor_v2 ../snake.asm"
```
*Usa las teclas `w`, `a`, `s`, `d` para mover el píxel en la consola.*

## Ejecución de Pruebas
Para validar la integridad de la arquitectura:
```cmd
wsl -e bash -c "./run_tests_v2.sh"
```

## Estructura del Proyecto
- `ALU/`: Lógica aritmética y de comparación.
- `Control/`: Unidad de Control y Decodificador.
- `Peripheral/`: Manejo de Framebuffer y Teclado.
- `snake.asm`: Código fuente del videojuego demo.
- `Manual_ISA_v2.md`: Documentación completa de instrucciones.
