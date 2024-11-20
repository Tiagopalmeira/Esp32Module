#define BLYNK_TEMPLATE_ID "TMPL2QOkkkkkxvJ"
#define BLYNK_TEMPLATE_NAME "SENAIPROJECT"
#define BLYNK_AUTH_TOKEN "f7XTX2kkkMRgy2WX_udu0kkkI"

#include <WiFi.h>
#include <WebServer.h>
#include <BlynkSimpleEsp32.h>
#include <regex>

// Define o nome e a senha do acess point.
const char *ssidAP = "ESP32-Access-Point";
const char *passwordAP = "123456789";

// Instancia o servidor para a pagina HTML.
WebServer server(80);
const int ledPin = 2;

const char *htmlForm = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configurar Wi-Fi</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            margin: 0;
            padding: 20px;
        }
        h1 {
            color: #339966;
            text-align: center;
        }
        p {
            text-align: center;
        }
        .form-container {
            background: #fff;
            border-radius: 5px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            padding: 20px;
            max-width: 500px;
            margin: 0 auto;
        }
        input[type="text"], input[type="password"] {
            width: calc(100% - 20px);
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #ccc;
            border-radius: 5px;
        }
        input[type="submit"] {
            background-color: #28a745;
            color: white;
            padding: 10px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            width: 100%;
            font-size: 16px;
        }
        input[type="submit"]:hover {
            background-color: #218838;
        }
        .warning {
            color: #ff0000;
            font-weight: bold;
        }
    </style>
</head>
<body>
    <div class="form-container">
        <h1><em>Configurar Wi-Fi</em></h1>
        <p><em>Configure a rede Wi-Fi na qual a placa irá se conectar</em></p>
        <form action="/submit" method="POST">
            <label for="ssid">SSID:</label>
            <input name="ssid" type="text" id="ssid" required />

            <label for="password">Senha:</label>
            <input name="password" type="password" id="password" required />

            <p class="warning">ATENÇÃO: CERTIFIQUE-SE QUE OS DADOS INSERIDOS ESTÃO CORRETOS ANTES DE CONECTAR.</p>
            <input type="submit" value="Conectar" />
        </form>
    </div>
</body>
</html>
)rawliteral";

// Declaração das funções que serão utilizadas.
void testConnection(const String &ssid, const String &password);
void handleFormSubmit();
void testConnectionWithEmptyPassword();
void testWiFiDisconnection();
void runTests();

void setup() {

  // Inicia a conexão serial e o led que será utilizado como status de conexão da rede.
  // O delay serve para atrasar a entradas das informações no monitor serial.
  Serial.begin(115200);
  delay(1000);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Inicia o acess point.
  WiFi.softAP(ssidAP, passwordAP);
  Serial.println("Acess Point iniciado com sucesso!\n");

  // Configura as rotas do servidor.
  // Quando a rota raiz é acessada com o GET o servidor traz o conteudo armazenado da variavel htmlForm.
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", htmlForm);
  });

  // Essa rota de post envia os dados e chamar a função handleFormSubmit para pegar os dados de SSID e PASSWORD e conectar se a rede.
  server.on("/submit", HTTP_POST, handleFormSubmit);

  // Inicia o servidor.
  server.begin();
  Serial.println("Servidor iniciado com sucesso!\n");

  // Chamado da função de testes.
  runTests();
}

void loop() {
  // Mantém o ESP32 verificando se há novas requisições HTTP.
  server.handleClient();
}

// Função chamada após o envio do form para se conectar a rede.
void handleFormSubmit() {
  if (server.arg("ssid") != "" && server.arg("password") != "") {
    String wifiSSID = server.arg("ssid");
    String wifiPassword = server.arg("password");

    // Conecta-se à rede Wi-Fi.
    WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
    Serial.println("Conectando a rede Wi-Fi...\n");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
      delay(1000);
      Serial.print(".");
      attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Conectado à rede Wi-Fi!\n");
      server.send(200, "text/html", "<h1>Conectado!</h1><p>Voce ja pode fechar esta pagina.</p>");
      Blynk.begin(BLYNK_AUTH_TOKEN, wifiSSID.c_str(), wifiPassword.c_str());
    } else {
      Serial.println("Falha na conexao");
      server.send(200, "text/html", "<h1>Falha na conexao!</h1><p>Tente novamente mais tarde.</p>");
    }
  } else {
    server.send(400, "text/html", "<h1>Dados invalidos!</h1>");
  }
}

// Função para testar a conexão com rede WI-FI.
void testConnection(const String &ssid, const String &password) {
  Serial.print("\nTestando a conexão: ");
  Serial.println(ssid);
  WiFi.begin(ssid.c_str(), password.c_str());
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nResultado: A conexão foi realizada com sucesso! ✓\n");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("\nResultado: Falha ao tentar se conectar a rede. ✕\n");
    digitalWrite(ledPin, LOW);
  }
}

// Função para validar o comprimento da senha com regex.
bool isValidPassword(const String &password) {
  std::regex regex(".{8,}");
  // A função std::regex espera uma const char então o .c_str() converte a string que é recebida como argumento.
  return std::regex_match(password.c_str(), regex);
}

// Função para medir o tempo de conexão.
void testConnectionPerformance(const String &ssid, const String &password) {
  unsigned long startTime = millis();
  WiFi.begin(ssid.c_str(), password.c_str());
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    attempts++;
  }
  unsigned long duration = millis() - startTime;
  Serial.print("Tempo de conexão: ");
  Serial.print(duration);
  Serial.println(" ms");
  if (duration <= 10000) {
    Serial.println("Resultado: Aprovado ✓\n");
  } else {
    Serial.println("Resultado: Reprovado ✕\n");
  }
}

// Função para testar a conexão com a senha vazia.
void testConnectionWithEmptyPassword() {
  String ssid = "::.ServerNet.:: Marmelada";
  String emptyPassword = "";
  WiFi.begin(ssid.c_str(), emptyPassword.c_str());
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nResultado: Conexão inesperadamente bem-sucedida! ✕\n");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("\nResultado: Conexão falhou como esperado. ✓\n");
    digitalWrite(ledPin, LOW);
  }
}

// Função para testar a desconexão da rede.
void testWiFiDisconnection() {
  String ssid = "::.ServerNet.:: Marmelada";  // SSID válido
  String password = "@i5428#!x";              // Senha válida
  WiFi.begin(ssid.c_str(), password.c_str());
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado a rede. Se desconectando da rede...\n");
    WiFi.disconnect();
    if (WiFi.status() == WL_DISCONNECTED) {
      Serial.println("Resultado: A desconexão foi realizada com sucesso! ✓\n");
      digitalWrite(ledPin, LOW);
    } else {
      Serial.println("Resultado: A desconexão falhou! ✕\n");
    }
  } else {
    Serial.println("Falha ao se conectar, não foi possível realizar o teste de desconexão. ✕\n");
  }
}

// Testes
void runTests() {
  Serial.println("\nIniciando testes...\n");

  // 1 - Teste de conexão com dados válidos
  Serial.println("\n1 - Teste de conexão com dados válidos.");
  testConnection("::.ServerNet.:: Marmelada", "@i5428#!x");  // SSID e senha válidos

  // 2 - Teste de conexão com dados inválidos
  Serial.println("\n2 - Teste de conexão com dados inválidos.");
  testConnection("SSIDErrado", "SenhaErrada");  // SSID e senha inválidos

  // 3 - Teste de conexão em uma segunda rede com dados válidos
  Serial.println("\n3 - Teste de conexão em uma segunda rede com dados válidos.");
  testConnection("OutraRede", "OutraSenha");  // SSID e senha válidos

  // 4 - Teste de senha com o comprimento correto
  Serial.println("\n4 - Teste de senha com o comprimento correto.");
  String testPassword1 = "12345678";
  Serial.print("Resultado: ");
  Serial.println(isValidPassword(testPassword1) ? "Aprovado ✓" : "Reprovado ✕\n");

  // 5 - Teste de senha com o comprimento incorreto
  Serial.println("\n5 - Teste de senha com o comprimento incorreto.");
  String testPassword2 = "1234";
  Serial.print("Resultado: ");
  Serial.println(isValidPassword(testPassword2) ? "Reprovado ✕" : "Aprovado ✓\n");

  // 6 - Teste de desempenho da conexão com dados válidos
  Serial.println("\n6 - Teste de desempenho da conexão com dados válidos.");
  testConnectionPerformance("::.ServerNet.:: Marmelada", "@i5428#!x");

  // 7 - Teste de desempenho da conexão com dados inválidos
  Serial.println("\n7 - Teste de desempenho da conexão com dados inválidos");
  unsigned long startTime = millis();
  testConnectionPerformance("RedeTeste", "NãoSeiASenha");
  unsigned long duration = millis() - startTime;
  Serial.print("Tempo total de tentativa de conexão com dados inválidos: ");
  Serial.print(duration);
  Serial.println(" ms");

  // 8 - Teste de conexão com a senha vazia
  Serial.println("\n8 - Teste de conexão com a senha vazia.");
  testConnectionWithEmptyPassword();

  // 9 - Teste de desconexão
  Serial.println("\n9 - Teste de desconexão.");
  testWiFiDisconnection();

  Serial.println("\nTestes finalizados com sucesso.");
}
