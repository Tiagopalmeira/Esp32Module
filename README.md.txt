# Projeto SENAIPROJECT

Este projeto utiliza um ESP32 para configurar uma rede Wi-Fi via uma interface web. O código permite que os usuários conectem o dispositivo a uma rede Wi-Fi e inclui várias funções de teste para garantir a robustez da conexão.

## Funções de Teste

### `testConnection(const String &ssid, const String &password)`

- **Descrição**: Testa a conexão com a rede Wi-Fi utilizando um SSID e uma senha fornecidos.
- **Funcionamento**: Tenta se conectar à rede por um número limitado de tentativas (10) e imprime o resultado da conexão no console serial.

### `isValidPassword(const String &password)`

- **Descrição**: Valida o comprimento da senha.
- **Funcionamento**: Usa uma expressão regular para verificar se a senha tem pelo menos 8 caracteres. Retorna `true` se a senha for válida, caso contrário, `false`.

### `testConnectionPerformance(const String &ssid, const String &password)`

- **Descrição**: Mede o tempo necessário para se conectar a uma rede Wi-Fi.
- **Funcionamento**: Registra o tempo de início e tenta se conectar. Após a tentativa, calcula e imprime a duração total no console serial, indicando se a conexão foi realizada dentro de 10 segundos.

### `testConnectionWithEmptyPassword()`

- **Descrição**: Testa a conexão com uma senha vazia.
- **Funcionamento**: Tenta se conectar a uma rede Wi-Fi válida usando uma string vazia como senha. Verifica se a conexão foi bem-sucedida e imprime o resultado no console.

### `testWiFiDisconnection()`

- **Descrição**: Testa a desconexão da rede Wi-Fi.
- **Funcionamento**: Conecta a uma rede válida e, em seguida, tenta desconectar. Imprime o resultado da operação de desconexão no console.

### `runTests()`

- **Descrição**: Executa uma série de testes de conexão e validação.
- **Funcionamento**: Chama todas as funções de teste em sequência, cada uma com um propósito específico:
  1. Testa a conexão com dados válidos.
  2. Testa a conexão com dados inválidos.
  3. Testa a conexão em uma segunda rede com dados válidos.
  4. Testa a validade de senhas com comprimento correto.
  5. Testa a validade de senhas com comprimento incorreto.
  6. Testa o desempenho da conexão com dados válidos.
  7. Testa o desempenho da conexão com dados inválidos.
  8. Testa a conexão com uma senha vazia.
  9. Testa a desconexão da rede.

## Teste de Interação com a Interface Web

Um teste automatizado foi implementado usando Selenium para validar a interação com a página de configuração do ESP32. O teste realiza as seguintes ações:

1. **Acessa a URL**: O teste abre a página principal do projeto.
2. **Ajusta o Tamanho da Janela**: Define o tamanho da janela do navegador para 1366x768 pixels, garantindo que a interface seja exibida corretamente.
3. **Preenche o Campo SSID**: Localiza o campo de entrada para o SSID e insere um valor de teste.
4. **Preenche o Campo de Senha**: Localiza o campo de entrada para a senha e insere uma senha de teste.
5. **Clica no Botão "Conectar"**: Simula um clique no botão para enviar os dados.

Esse teste assegura que a interface web funcione corretamente e que o formulário de configuração possa ser utilizado sem problemas.

## Considerações Finais

As funções de teste garantem que o dispositivo se conecte de forma confiável às redes Wi-Fi e valida as credenciais fornecidas pelo usuário. Este código serve como uma base para aplicações que requerem configuração de rede sem fio em dispositivos IoT.
