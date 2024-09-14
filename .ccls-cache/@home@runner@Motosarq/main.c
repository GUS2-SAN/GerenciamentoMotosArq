#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct RegMoto {
  char Nome[20];
  char Modelo[10];
  char Placa[7];
  char Defeito[50];
  char Status;
  float Preco;
};
typedef struct RegMoto TpRegMoto;
TpRegMoto VZonda[50];

int Quant = -1; // Controla o preenchimento do vetor
float ValorFinal = 0;
char MaiorData[20] = "";

// ARQUIVO
FILE *ArqMotos;

//Função a ser carregada ao inicio do programa para resgatar os dados do arquivo
void CarregarServicosPendentes() {

  ArqMotos = fopen("servicos_pendentes.dat", "r+b");
  if (ArqMotos == NULL) {
    printf("Arquivo de serviços pendentes não encontrado ou vazio.\n");
    return;
  }

  char buffer[100];
  while (fgets(buffer, sizeof(buffer), ArqMotos) != NULL) {
    if (strstr(buffer, "Cliente:") != NULL) {
      Quant++;
      sscanf(buffer, "Cliente: %[^\n]", VZonda[Quant].Nome);
      fgets(buffer, sizeof(buffer), ArqMotos);
      sscanf(buffer, "Modelo: %[^\n]", VZonda[Quant].Modelo);
      fgets(buffer, sizeof(buffer), ArqMotos);
      sscanf(buffer, "Placa: %[^\n]", VZonda[Quant].Placa);
      fgets(buffer, sizeof(buffer), ArqMotos);
      sscanf(buffer, "Defeito: %[^\n]", VZonda[Quant].Defeito);
      fgets(buffer, sizeof(buffer), ArqMotos);
      sscanf(buffer, "Status: %c", &VZonda[Quant].Status);
      VZonda[Quant].Status = '0';
      VZonda[Quant].Preco = 0;
    }
  }

  fclose(ArqMotos);
}

//Salvar quando acaba o expediente 
void SalvarServicosPendentes() {

  //w+b pq é bom pra limpar os dados de motos q já foram atendidas
  ArqMotos = fopen("servicos_pendentes.dat", "w+b");
  

  fprintf(ArqMotos, "Serviços Pendentes:\n");
  for (int Cont = 0; Cont <= Quant; Cont++) {
    fprintf(ArqMotos, "Cliente: %s\n", VZonda[Cont].Nome);
    fprintf(ArqMotos, "Modelo: %s\n", VZonda[Cont].Modelo);
    fprintf(ArqMotos, "Placa: %s\n", VZonda[Cont].Placa);
    fprintf(ArqMotos, "Defeito: %s\n", VZonda[Cont].Defeito);
    fprintf(ArqMotos, "Status: %c\n", VZonda[Cont].Status);
    fprintf(ArqMotos, "---------------------\n");
  }

  fclose(ArqMotos);
}

//salvar dados do dia e ganhos 
void SalvarExpediente(float valorRecebido, char *data) {
  ArqMotos = fopen("expediente.dat", "a+b");
  /* if (ArqMotos == NULL) {
     printf("Erro ao abrir o arquivo de expediente.\n");
     return;
   }*/

  fprintf(ArqMotos, "Data: %s | Valor Recebido: %.2f\n", data, valorRecebido);
  fclose(ArqMotos);
}

void ExibirHistoricoFinanceiro() {
  ArqMotos = fopen("expediente.dat", "r+b");
  /* if (ArqMotos == NULL) {
     printf("Arquivo de histórico financeiro não encontrado ou vazio.\n");
     return;
   }*/

  printf("\n\n >>> Histórico Financeiro <<<\n\n");
  float total = 0;
  float maiorValor = 0;
  char linha[100];
  while (fgets(linha, sizeof(linha), ArqMotos) != NULL) {
    printf("%s", linha);
    float valor;
    if (sscanf(linha, "Data: %*s | Valor Recebido: %f", &valor) == 1) {
      total += valor;
      if (valor > maiorValor) {
        maiorValor = valor;
        sscanf(linha, "Data: %s", MaiorData);
      }
    }
  }
  fclose(ArqMotos);

  printf("\n\nTotal Recebido: %.2f\n", total);
  printf("Dia com maior valor de vendas: %s\n", MaiorData);
}

void SolicitaServico() {
  char Sair = 'S';
  do {
    printf("\n\n >>> Motos Zonda <<< \n\n");
    printf("\n\n >>> Pedidos em Aberto <<< \n\n");
    Quant = Quant + 1;
    printf("Qual o nome do cliente? ");
    scanf(" %[^\n]s", VZonda[Quant].Nome);
    printf("Qual o modelo da moto? ");
    scanf(" %[^\n]s", VZonda[Quant].Modelo);
    printf("Qual a placa da moto? ");
    scanf(" %[^\n]s", VZonda[Quant].Placa);
    printf("Qual o defeito da moto? ");
    scanf(" %[^\n]s", VZonda[Quant].Defeito);
    VZonda[Quant].Status = '0';
    VZonda[Quant].Preco = 0;
    printf("\n\n Deseja inserir novo servico? S|N ");
    scanf(" %c", &Sair);
    Sair = toupper(Sair);
  } while (Sair != 'N');

  // SalvarServicosPendentes();
}

void IniciaServico() {
  char P[7];
  printf("Placa da moto para iniciar serviço: ");
  scanf(" %[^\n]s", P);
  int Pos = -1; // posição da moto P no vetor
  // busca da moto para iniciar serviço
  for (int Cont = 0; Cont <= Quant; Cont++)
    if (strcmp(VZonda[Cont].Placa, P) == 0)
      Pos = Cont;
  if (Pos == -1)
    printf("Moto não cadastrada!");
  else {
    printf("\n Cliente %d: %s", Pos + 1, VZonda[Pos].Nome);
    printf("\n Modelo: %s", VZonda[Pos].Modelo);
    printf("\n Placa: %s", VZonda[Pos].Placa);
    printf("\n Defeito: %s", VZonda[Pos].Defeito);
    VZonda[Pos].Status = '1';
    printf("\n Status: %c", VZonda[Pos].Status);
    if (VZonda[Pos].Preco == 0)
      printf("\n Preco: NAO DEFINIDO");
    else
      printf("\n Preco: %.2f", VZonda[Pos].Preco);
  }

  // SalvarServicosPendentes();
}

void RemoverSolicitacao() {
  // dado a placa da moto, remover a solicitação (mudar o status para '2')
  char P[7];
  printf("Placa da moto para remover a solicitação: ");
  scanf(" %[^\n]s", P);
  int Pos = -1;
  for (int Cont = 0; Cont <= Quant; Cont++) {
    if (strcmp(VZonda[Cont].Placa, P) == 0) {
      Pos = Cont;
      break;
    }
  }

  if (Pos == -1) {
    printf("Moto não cadastrada!");
  } else if (VZonda[Pos].Status == '2' || VZonda[Pos].Status == '3') {
    // caso o pedido ja tenha sido concluido/ removido
    printf("Pedido já cancelado/ iniciado!");
  } else {
    VZonda[Pos].Status = '2';
    printf("\n PEDIDO CANCELADO");
    printf("\n Cliente %d: %s", Pos + 1, VZonda[Pos].Nome);
    printf("\n Modelo: %s", VZonda[Pos].Modelo);
    printf("\n Placa: %s", VZonda[Pos].Placa);
    // printf("\n Status: %c",VZonda[Pos].Status);
  }
}

void ConsultarSolicitacoes() {
  printf("\n\n >>> Motos Zonda <<< \n\n");
  if (Quant == -1)
    printf("Não há serviços cadastrados.");
  else {
    for (int Cont = 0; Cont <= Quant; Cont++) {
      if (VZonda[Cont].Status == '0') {
        printf("\n Cliente %d: %s", Cont + 1, VZonda[Cont].Nome);
        printf("\n Modelo: %s", VZonda[Cont].Modelo);
        printf("\n Placa: %s", VZonda[Cont].Placa);
        printf("\n Defeito: %s", VZonda[Cont].Defeito);
        printf("\n Status: %c", VZonda[Cont].Status);
        if (VZonda[Cont].Preco == 0)
          printf("\n Preco: NAO DEFINIDO");
        else
          printf("\n Preco: %.2f", VZonda[Cont].Preco);
        printf("\n --------------------- \n\n");
      }
    }
  }
}

void ConcluirServico() {
  char P[7];
  printf("Digite a placa da moto: ");
  scanf(" %[^\n]s", P);
  int Pos = -1;
  for (int Cont = 0; Cont <= Quant; Cont++) {
    if (strcmp(VZonda[Cont].Placa, P) == 0) {
      Pos = Cont;
      break;
    }
  }

  if (Pos == -1) {
    printf("Moto não cadastrada!");
  } else if (VZonda[Pos].Status == '2' || VZonda[Pos].Status == '3') {
    printf("Pedido já cancelado/ concluido!");
  } else {
    VZonda[Pos].Status = '3';
    printf("Digite o valor definido pelo mecanico: ");
    scanf(" %f", &VZonda[Pos].Preco);

    printf("\n PEDIDO CONCLUIDO");
    printf("\n Cliente %d: %s", Pos + 1, VZonda[Pos].Nome);
    printf("\n Modelo: %s", VZonda[Pos].Modelo);
    printf("\n Placa: %s", VZonda[Pos].Placa);
    printf("\n Defeito: %s", VZonda[Pos].Defeito);
    printf("\n Status: %c", VZonda[Pos].Status);
    printf("\n Preco: %.2f", VZonda[Pos].Preco);

    ValorFinal = ValorFinal + VZonda[Pos].Preco;
  }

  SalvarServicosPendentes();
}

void EncerrarExpediente() {
  SalvarServicosPendentes();

  // Solicitar a data ao usuário
  char data[20];
  printf("Digite a data (no formato DD-MM-AAAA): ");
  scanf("%s", data);

  // Exibir todos os serviços efetuados e o valor total arrecadado
  printf("\n\n >>> Motos Zonda <<< \n\n");
  // printf(" >>> Relatoria dia %d/%d <<< \n\n",dia, mes);

  if (Quant == -1) {
    printf("Não há serviços cadastrados.");
  } else {
    printf("\n\n >>> Serviços Pendentes <<< \n\n");
    for (int Cont = 0; Cont <= Quant; Cont++) {
      if (VZonda[Cont].Status == '0') {
        printf("\n Cliente %d: %s", Cont + 1, VZonda[Cont].Nome);
        printf("\n Modelo: %s", VZonda[Cont].Modelo);
        printf("\n Placa: %s", VZonda[Cont].Placa);
        printf("\n Defeito: %s", VZonda[Cont].Defeito);
        printf("\n --------------------- \n\n");
      }
    }

    printf("\n\n >>> Serviços Cancelados <<< \n\n");
    for (int Cont = 0; Cont <= Quant; Cont++) {
      if (VZonda[Cont].Status == '2') {
        printf("\n Cliente %d: %s", Cont + 1, VZonda[Cont].Nome);
        printf("\n Modelo: %s", VZonda[Cont].Modelo);
        printf("\n Placa: %s", VZonda[Cont].Placa);
        printf("\n Defeito: %s", VZonda[Cont].Defeito);
        printf("\n --------------------- \n\n");
      }
    }

    printf("\n\n >>> Serviços Concluidos <<< \n\n");
    for (int Cont = 0; Cont <= Quant; Cont++) {
      if (VZonda[Cont].Status == '3') {
        printf("\n Cliente %d: %s", Cont + 1, VZonda[Cont].Nome);
        printf("\n Modelo: %s", VZonda[Cont].Modelo);
        printf("\n Placa: %s", VZonda[Cont].Placa);
        printf("\n Defeito: %s", VZonda[Cont].Defeito);
        printf("\n Preco: %.2f", VZonda[Cont].Preco);
        printf("\n --------------------- \n\n");
      }
    }
    printf("\n\n VALOR TOTAL ARRECADADO: %.2f", ValorFinal);
    SalvarExpediente(ValorFinal, data);
  }
}

int main() {

  //carregar ao inicio do programa
  CarregarServicosPendentes();

  int Opcao;
  system("clear"); // limpa tela no ReplIt
  do {
    // Exibicao de menu e leitura da opcao.
    printf("\n\n >>> Motos Zonda <<< \n\n");
    printf("1 - Solicitar Servico \n");
    printf("2 - Iniciar Servico \n");
    printf("3 - Remover Solicitacao \n");
    printf("4 - Consultar Solicitacoes \n");
    printf("5 - Concluir Servico \n");
    printf("6 - Encerrar Expediente \n");
    printf("7 - Consultar Historico Financeiro \n");
    printf("8 - Sair \n\n");
    printf("Digite a opcao desejada: ");
    scanf("%d", &Opcao);
    switch (Opcao) {
    case 1:
      SolicitaServico();
      break;
    case 2:
      IniciaServico();
      break;
    case 3:
      RemoverSolicitacao();
      break;
    case 4:
      ConsultarSolicitacoes();
      break;
    case 5:
      ConcluirServico();
      break;
    case 6:
      EncerrarExpediente();
      break;
    case 7:
      ExibirHistoricoFinanceiro();
      break;
    case 8:
      break;
    }
  } while (Opcao != 8);
  SalvarServicosPendentes(); // Salvar serviços pendentes ao finalizar o
                             // programa para não perder os dados 
  return 0;
}