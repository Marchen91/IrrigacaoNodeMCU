# Controle de Irrigação NodeMCU

Este projeto consiste em um código para controle de irrigação utilizando a placa NodeMCU. O dispositivo se comunica com o Firestore para enviar informações sobre umidade do solo e status de irrigação. Além disso, realiza a leitura dos limites mínimo e máximo estabelecidos.

## Funcionalidades

- **Envio de Dados:** O NodeMCU envia dados de umidade do solo e status de irrigação para o Firestore.
  
- **Leitura de Limites:** O dispositivo lê os limites mínimo e máximo estabelecidos no Firestore para ajustar o controle de irrigação.

## Alunos

- Francis Guedes
- Marcelo Henrique de Souza Braga

## Professor

- Fábio Viana

## Instruções de Uso

1. Clone este repositório.
2. Carregue o código no seu NodeMCU usando a Arduino IDE ou a plataforma de sua preferência.
3. Certifique-se de configurar corretamente as credenciais do Firestore no código.
4. Alimente o NodeMCU e observe o controle automático da irrigação com base nos limites definidos.

Este projeto foi desenvolvido como parte do curso ministrado pelo Professor Fábio Viana, com os alunos Francis Guedes e Marcelo Henrique de Souza Braga.
