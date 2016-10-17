# pmt

Programa desenvolvido em C++, similar à ferramenta GNU Grep, para buscas de padrões em arquivos de
texto.
Projeto da disciplina de Processamento de Cadeias de Caracteres (IF767), lecionada pelo Prof. Dr.
Paulo Gustavo Soares Fonseca no período de 2016.2.

Alunos/Autores:
  - Mateus de Freitas Leite (mfl3@cin.ufpe.br)
  - Valdemir de Andrade Borges Júnior (vabj@cin.ufpe.br)

## Criando o executável

+ Na pasta pmt, abra o terminal e execute o comando `make`
+ Após a execução do `make`, uma pasta `bin/` deverá ter sido criada. Nela estará o executável da
  ferramenta.
  
## Instruções de uso

Utilização:
`$ pmt [options] pattern textfile [textfile ...]`

+ `pattern` é a cadeia que representa o padrão a ser procurado no(s) arquivo(s) de texto.
+ `textfile` é o nome do arquivo onde o texto a ser processado se encontra. Múltiplos arquivos
podem ser passados como parâmetros. No caso de utilização de *wildcards*, é necessário que o nome
do arquivo seja especificado entre aspas para procurar por múltiplos arquivos; do contrário, apenas
o primeiro arquivo encontrado será processado.

Opções disponíveis:
  `-a --algorithm`    Especifica qual o algoritmo a ser utilizado pela ferramenta. Os argumentos
                      válidos para esta opção são: `bm` (Boyer-Moore), `kmp` (Knuth-Morris-Pratt),
                      `sel` (Sellers) e `ukk` (Ukkonen). Caso a opção -e (ou --edit) esteja
                      habilitada, somente os algoritmos de Sellers e o de Ukkonen serão aceitos
                      para esta opção.

  `-c --count`        Imprime apenas a quantidade de ocorrências do padrão no texto.

  `-e --edit`         Determina a distância de edição máxima para buscas aproximadas.

  `-p --pattern`      Especifica um arquivo de texto contendo múltiplos parâmetros. Caso essa
                      opção seja habilitada, o parâmetro obrigatório `pattern` é considerado
                      como sendo um arquivo de texto.
