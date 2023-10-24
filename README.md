# Minishell

## Description (Descripción)

**Minishell** is a self-made recreation of Bash, complete with features like input/output redirection via pipes and files, as well as logical operators such as || and &&. It also recognizes the semicolon (;) separator. This project is implemented entirely using system calls and custom functions, with the exception of the "readline" library.

**Minishell** es una recreación propia de Bash, con características como la redirección de entrada/salida a través de tuberías y archivos, así como operadores lógicos como || y &&. También reconoce el separador de punto y coma (;). Este proyecto está implementado completamente utilizando llamadas al sistema y funciones personalizadas, con la excepción de la biblioteca "readline".

## Requirements (Requisitos)

To run **Minishell**, you need the following requirement:

Para ejecutar **Minishell**, necesitas el siguiente requisito:

- **libreadline** (including installation for Ubuntu, Fedora, Arch, and macOS using Brew).

    **libreadline** (incluyendo la instalación para Ubuntu, Fedora, Arch y macOS a través de Brew).

    - **Ubuntu**:

      ```bash
      sudo apt-get install libreadline-dev
      ```

    - **Fedora**:

      ```bash
      sudo dnf install readline-devel
      ```

    - **Arch Linux**:

      ```bash
      sudo pacman -S readline
      ```

    - **macOS** (using Brew):

      ```bash
      brew install readline
      ```

## Compilation (Compilación)

To compile the project, follow these steps:

Para compilar el proyecto, sigue estos pasos:

1. Navigate to the cloned directory. (Ir al directorio clonado)
2. Run the `make` command. (Ejecutar el comando `make`)

## Execution (Ejecución)

To execute **Minishell**, use the following command:

Para ejecutar **Minishell**, utiliza el siguiente comando:

```bash
./minishell
