# INSTRUÇÕES

## Instalação sqlite

<h4>Debian</h4>

``` bash
apt-get install sqlite3 libsqlite3-dev
```

<h4>Windows</h4>

### Instalação do SQLite no Windows

#### Passo 1: Baixar o SQLite

1. Acesse a página de download do SQLite: [SQLite Download Page](https://www.sqlite.org/download.html).
2. Na seção **Precompiled Binaries for Windows**, baixe o arquivo ZIP que contém os binários do SQLite. O arquivo geralmente é nomeado como `sqlite-tools-win32-x86-*.zip`.

#### Passo 2: Extrair os Arquivos

1. Após o download, localize o arquivo ZIP e extraia seu conteúdo para um diretório de sua escolha, como `C:\sqlite`.

#### Passo 3: Adicionar ao PATH

Para facilitar o uso do SQLite a partir do Prompt de Comando, você pode adicionar o diretório onde o SQLite foi extraído ao seu PATH:

1. Clique com o botão direito em **Este PC** ou **Meu Computador** e selecione **Propriedades**.
2. Clique em **Configurações avançadas do sistema**.
3. Na aba **Avançado**, clique em **Variáveis de Ambiente**.
4. Na seção **Variáveis do sistema**, encontre a variável `Path` e clique em **Editar**.
5. Adicione o caminho do diretório onde você extraiu o SQLite (por exemplo, `C:\sqlite`).
6. Clique em **OK** para fechar todas as janelas.

#### Passo 4: Verificar a Instalação

1. Abra o **Prompt de Comando**.
2. Digite `sqlite3` e pressione **Enter**. Se a instalação foi bem-sucedida, você verá a interface do SQLite com a versão instalada.

#### Passo 5: Usar o SQLite em seu Projeto C

Certifique-se de que você tem os arquivos de cabeçalho e a biblioteca do SQLite disponíveis para compilar seu código C que utiliza SQLite. Você pode precisar baixar a biblioteca de desenvolvimento do SQLite, que geralmente está disponível na mesma página de download.

## Para acessar o banco de dados

<p>No diretório que está banco de dados rode o comando</p>

``` bash
sqlite3 hortifruti.db
```

## Compilação

<h4>Debian</h4>

<p>Para compilar e executar basta rodar esse comando na pasta do projeto</p>

``` bash
gcc *.c -o main.o -lsqlite3 && ./main.o
```

<h4>Windows</h4>

<p>Para compilar no windows</p>
<h4>Não foi testado</h4>
<p>* Lembrar de alterar os caminhos no comando</p>

``` cmd
gcc *.c -o main.exe
``
