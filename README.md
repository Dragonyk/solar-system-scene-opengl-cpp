Simulador do Sistema Solar com OpenGL Moderno

Sistema Solar 
📝 Descrição

Este projeto é um simulador 3D interativo do sistema solar desenvolvido com OpenGL moderno (Core Profile). Ele permite visualizar os planetas, suas órbitas e movimentos com representação realista de escalas e iluminação.
✨ Funcionalidades Principais

    Renderização 3D realista de corpos celestes

    Sistema de órbitas elípticas com parâmetros ajustáveis

    Iluminação baseada no Sol com efeitos de fases

    Controle de câmera estilo FPS para navegação livre

    Interface gráfica para ajuste de parâmetros em tempo real

    Escalas relativas entre planetas e distâncias

🛠️ Tecnologias Utilizadas

    OpenGL (Core Profile 3.3+)

    GLFW - Gerenciamento de janelas e inputs

    GLM - Operações matemáticas 3D

    GLEW - Carregamento de extensões OpenGL

    Dear ImGui - Interface de usuário

    Assimp - Carregamento de modelos 3D (opcional)

📥 Instalação e Execução
Pré-requisitos

    Compilador C++ compatível com C++11

    CMake (versão 3.10+)

    OpenGL 3.3+

Compilação
bash

git clone https://github.com/seu-usuario/simulador-sistema-solar.git
cd simulador-sistema-solar
mkdir build && cd build
cmake ..
make

Execução
bash

./SolarSystemSimulator

🎮 Controles
Tecla	Ação
WASD	Movimentar a câmera
Mouse	Olhar ao redor
Scroll	Zoom in/out
F	Alternar controle de câmera
Setas	Mover objeto selecionado
Teclado numérico	Rotacionar objeto
+/-	Aumentar/diminuir escala
R	Resetar posições

📂 Estrutura de Arquivos
.
├── CMakeLists.txt          # Configuração do projeto
├── include/                # Cabeçalhos
│   ├── Camera.h            # Controle de câmera
│   ├── Shader.h            # Gerenciamento de shaders
│   └── ...                 # Outros cabeçalhos
├── res/                    # Recursos
│   ├── shaders/            # Arquivos de shaders
│   └── models/             # Modelos 3D
├── src/                    # Código fonte
└── README.md               # Este arquivo


Carlos L. S. Machado - carlos.machado.pro@gmail.com

Link do Projeto: https://github.com/Dragonyk/solar-system-scene-opengl-cpp
