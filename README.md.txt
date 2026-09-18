# 🌱 Verde Esperança — Simulador de Meio de Pagamento

Projeto acadêmico desenvolvido para a disciplina de **Algoritmos e Pensamento Computacional — 2026/2**, integrando uma interface web ao processamento de pagamentos desenvolvido em linguagem C.

## 📌 Sobre o projeto

O projeto utiliza o site fictício da ONG **Verde Esperança** como interface para demonstrar o funcionamento de um Simulador de Meio de Pagamento (SMP).

O usuário pode realizar uma doação informando:

- Código de 6 dígitos
- Valor da doação
- Meio de pagamento

Os meios de pagamento disponíveis são:

- Pix
- Débito
- Crédito

## 💳 Regras de processamento

O programa realiza o cálculo da taxa MDR de acordo com o meio de pagamento:

| Meio de pagamento | MDR |
|---|---:|
| Pix | 0% |
| Débito | 1% |
| Crédito | 5% |

Após o processamento, o sistema calcula o valor líquido e gera um arquivo JSON contendo os dados da transação.

## 🛠️ Tecnologias utilizadas

- HTML5
- CSS3
- JavaScript
- Node.js
- Linguagem C
- JSON
- Git / GitHub

## 📂 Estrutura principal

```text
ProjetoV2/
├── cprogram/
│   ├── processamento.c
│   ├── processamento.exe
│   └── unicsul.h
│
├── log/
│   └── arquivos JSON gerados
│
├── public/
│   ├── css/
│   ├── doacao/
│   ├── images/
│   ├── js/
│   ├── index.html
│   ├── contato.html
│   ├── projetos.html
│   └── obrigado.html
│
├── server.js
└── README.md
```

## ▶️ Como executar

### 1. Baixar o projeto

Baixe ou clone este repositório para o computador.

### 2. Abrir o terminal

Abra o CMD ou terminal na pasta principal do projeto, onde está localizado o arquivo:

```text
server.js
```

### 3. Iniciar o servidor

Execute:

```bash
node server.js
```

### 4. Abrir no navegador

Com o servidor em execução, acesse:

```text
http://localhost:3000
```

## ⚙️ Funcionamento

O formulário HTML envia os dados da transação para o servidor através da rota:

```text
POST /pagto
```

O servidor executa:

```text
cprogram/processamento.exe
```

O programa em C realiza os cálculos da transação e gera o registro correspondente em formato JSON.

## 🎓 Finalidade

Projeto desenvolvido exclusivamente para fins acadêmicos, demonstrando a integração entre desenvolvimento web, servidor Node.js e processamento em linguagem C.