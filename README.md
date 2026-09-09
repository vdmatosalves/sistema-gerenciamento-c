# 🗄️ Sistema de Gerenciamento de Funcionários, Projetos e Gerentes (C)

Projeto desenvolvido para a disciplina de **Estrutura de Dados** do curso de **Bacharelado em Engenharia da Computação** no **IFSP Campus Piracicaba**.

## 📌 Sobre o Projeto

O sistema realiza a gestão completa de funcionários e projetos de uma empresa, além de mapear os e-mails dos gerentes através de uma **Tabela Hash com tratamento de colisões por Endereçamento Aberto**. 

O projeto conta com persistência completa de dados em arquivos `.txt`, consultas otimizadas e rotinas de relatórios dinâmicos.

---

## 🛠️ Estruturas de Dados e Algoritmos Utilizados

- **Tabela Hash:** Mapeamento em tempo de busca $O(1)$ para e-mails de gerentes.
- **Busca Binária:** Otimização para localização de registros em tempo $O(\log n)$.
- **Algoritmos de Ordenação:** Implementação de **Bubble Sort**, **Quick Sort** e **Insertion Sort**.
- **Manipulação de Tempo:** Integração com a biblioteca `<time.h>` para cálculo de atrasos em projetos.
- **Persistência de Dados:** Leitura e gravação automática em arquivos de texto.

---

## 💻 Como Compilar e Executar

1. **Clone este repositório:**
   ```bash
   git clone https://github.com/vdmatosalves/sistema-gerenciamento-c.git
   cd sistema-gerenciamento-c
