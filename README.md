Pré-requisitos

Antes de compilar e executar o jogo, certifique-se de que os seguintes componentes estão instalados no seu sistema(Os exemplos de compilação e execução foram feitos no Linux Mint):

Obrigatorio ter algum compilador de C/C++ na maquina, as instruções de compilação usam o GCC(GNU Compiler Collection).

Para instalar as dependências da biblioteca Allegro 5. No Linux Mint, você pode instalar com o seguinte comando no terminal:

sudo apt update && sudo apt install liballegro5-dev build-essential -y

1- Instruções de Compilação

Clone o repositório ou copie os arquivos do jogo para uma pasta local:

git clone [https://github.com/seu-repositorio/vida-universitaria.git](https://github.com/VitorLucas007/vida_universitaria_game)

cd vida-universitaria

Observe que no codigo tem a parte dos assets(fonte e imagens), nessa parte você deve alterar o caminho que esta na imagem pelo camninho delas em sua maquina, caso não faça isso o codigo não ira compilar.

![Captura de tela 2025-01-06 163928](https://github.com/user-attachments/assets/f8562e40-72d1-4f0e-a20c-494cc199f2c3)

Apos corrigir o caminho das imagens rode o comando abaixo no seu terminal:

gcc main.c -o VidaUniversitaria -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives
  
2- Instruções de Execução

Após a compilação, execute o jogo com o seguinte comando:

./VidaUniversitaria

3- Controles do Jogo

Seta para Esquerda: Move o personagem para a esquerda.

Seta para Direita: Move o personagem para a direita.

ESC: Fecha o jogo.

4- Objetivo do Jogo

Capture o maior número possível de itens no cenário para aumentar sua pontuação. Evite perder itens e complete os desafios propostos!

5- Contribuições

Contribuições são bem-vindas! Sinta-se à vontade para abrir issues ou enviar pull requests para melhorias no jogo.
