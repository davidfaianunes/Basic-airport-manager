/* segundo projeto de IAED
 * autor (de parte do codigo): David Nunes - ist1102890 - LEIC-A
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_AEROPORTOS 40	/* número máximo de areoportos */
#define MAX_NUM_VOOS 30000	/* número máximo de voos */

#define MAX_CODIGO_AEROPORTO 4	/* dimensão do código do aeroporto */
#define MAX_NOME_PAIS 31	/* dimensão do nome do pais */
#define MAX_NOME_CIDADE 51	/* dimensão do nome da cidade */

#define MAX_CODIGO_VOO 7	/* dimensão do código do voo */
#define MAX_DATA 11		/* dimensão da data */
#define MAX_HORA 6		/* dimensão da hora */

#define NAO_EXISTE -1		/* código de erro */

#define ANO_INICIO 2022		/* ano inicial do sistema */
#define DIAS_ANO 365		/* número de dias no ano */
#define HORAS_DIA 24		/* número de horas por dia */
#define MINUTOS_HORA 60		/* número de minutos numa hora */
#define MINUTOS_DIA 1440	/* número de minutos do dia */

#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

/* Tipos de Dados */


typedef struct {
	char id[MAX_CODIGO_AEROPORTO];
	char pais[MAX_NOME_PAIS];
	char cidade[MAX_NOME_CIDADE];
	int numVoos;
} Aeroporto;

typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int hora;
	int minuto;
} Hora;

typedef struct {
  char codigoVoo[MAX_CODIGO_VOO];
  Data data;
  char* codigoReserva;
  int numPassageiros;
} Reserva;

struct Node{
	Reserva reserva;
	struct Node* next;
};
typedef struct Node Node;

typedef struct {
  char id[MAX_CODIGO_VOO];
  char partida[MAX_CODIGO_AEROPORTO];
  char chegada[MAX_CODIGO_AEROPORTO];
  Data data;
  Hora hora;
  Hora duracao;
  int capacidade;
  int horaPartida;
  int horaChegada;
  Node* head;
  int preenchido; /* quantidade de lugares ja reservados */
} Voo;


/* Variaveis Globais */

int _numAeroportos = 0;		/* número de aeroportos introduzidos */
Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];	/* vetor de aeroportos */

int _numVoos = 0		/* número de voos introduzidos */;
Voo _voos[MAX_NUM_VOOS];	/* vetor de voos */

Data _hoje = { 1, 1, 2022 };	/* data atual do sistema */

const int _diasMesAc[] =	/* dias acumulados por mês (jan=1) */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

/* Funcoes Leitura */

Hora leHora() {
	Hora h;
	scanf("%d:%d", &h.hora, &h.minuto);
	return h;
}


Data leData() {
	Data d;
	scanf("%d-%d-%d", &d.dia, &d.mes, &d.ano);
	return d;
}


int leProximaPalavra(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
	return (c == '\n');
}


void lePalavraAteFimDeLinha(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
}


void leAteFimDeLinha() {
	char c = getchar();
	while (c != '\n')
		c = getchar();
}


/* Funcoes Datas e Horas */

void mostraData(Data d) {
	if (d.dia < 10)
		printf("0");
	printf("%d-", d.dia);
	if (d.mes < 10)
		printf("0");
	printf("%d-%d", d.mes, d.ano);
}


void mostraHora(Hora h) {
	if (h.hora < 10)
		printf("0");
	printf("%d:", h.hora);
	if (h.minuto < 10)
		printf("0");
	printf("%d", h.minuto);
}


int converteDataNum(Data d) {
	return (d.ano - ANO_INICIO) * DIAS_ANO + _diasMesAc[d.mes - 1] +
		d.dia - 1;
}


int converteHoraNum(Hora h) {
	return ((h.hora * MINUTOS_HORA) + h.minuto);
}


int converteDataHoraNum(Data d, Hora h) {
	return converteDataNum(d) * MINUTOS_DIA + converteHoraNum(h);
}


Hora converteNumHora(int num) {
	Hora h;
	h.minuto = num % MINUTOS_HORA;
	h.hora = ((num - h.minuto) / MINUTOS_HORA) % HORAS_DIA;
	return h;
}


Data converteNumData(int num) {
	Data d;
	int i = 0;
	num = (num - (num % MINUTOS_DIA)) / MINUTOS_DIA;
	d.ano = (num / DIAS_ANO) + ANO_INICIO;
	num = num - ((d.ano - ANO_INICIO) * DIAS_ANO);
	while (i <= 11 && num >= _diasMesAc[i])
		i++;
	d.mes = i;
	d.dia = num - _diasMesAc[i - 1] + 1;
	return d;
}


int validaData(Data d) {
	int numData = converteDataNum(d);
	Data proximoAno = _hoje;
	proximoAno.ano++;
	return !(numData < converteDataNum(_hoje)
		 || numData > converteDataNum(proximoAno));
}

/* funcao nova do teste pratico */

int dataFutura(Data d) {
	int numData = converteDataNum(d);
	return !(numData < converteDataNum(_hoje));
}

int validaHora(Hora h) {
	return !(h.hora > 12 || (h.hora == 12 && h.minuto > 0));
}


/* Algoritmo de ordenação BubbleSort */

void bubbleSort(int indexes[], int size, int (*cmpFunc) (int a, int b)) {
  int i, j, done;
  
  for (i = 0; i < size-1; i++){
    done=1;
    for (j = size-1; j > i; j--) 
      if ((*cmpFunc)(indexes[j-1], indexes[j])) {
	int aux = indexes[j];
	indexes[j] = indexes[j-1];
	indexes[j-1] = aux;
	done=0;
      }
    if (done) break;
  }
}


/* Funcoes Aeroportos */


int aeroportoInvalido(char id[]) {
	int i;
	int tamanho = strlen(id);
	printf("id: '%s'\n", id);
	printf("tamanho: %d\n", tamanho);
	if(!(3 <= tamanho && tamanho <= 5)) return TRUE;
	for (i = 0; id[i] != '\0'; i++)
		if (!((id[i] >= 'A' && id[i] <= 'Z')||(id[i]>='a' && id[i]<='z')))
		{
			printf("invalid char: %c\n", id[i]);
			return TRUE;
		}
	return FALSE;
}


int encontraAeroporto(char id[]) {
	int i;
	for (i = 0; i < _numAeroportos; i++)
		if (!strcmp(id, _aeroportos[i].id))
			return i;
	return NAO_EXISTE;
}


void adicionaAeroporto() {
	Aeroporto a;
	char id[6];

	leProximaPalavra(a.id);
	leProximaPalavra(a.pais);
	lePalavraAteFimDeLinha(a.cidade);
	printf("idAdiciona2: %s\n", a.id);
	printf("paisAdiciona2: %s\n", a.pais);
	printf("cidadeAdiciona2: %s\n", a.cidade);

	if (aeroportoInvalido(a.id))
		printf("invalid airport ID\n");
	else if (_numAeroportos == MAX_NUM_AEROPORTOS)
		printf("too many airports\n");
	else if (encontraAeroporto(a.id) != NAO_EXISTE)
		printf("duplicate airport\n");
	else {
		strcpy(_aeroportos[_numAeroportos].id, a.id);
		strcpy(_aeroportos[_numAeroportos].pais, a.pais);
		strcpy(_aeroportos[_numAeroportos].cidade, a.cidade);
		_aeroportos[_numAeroportos].numVoos = 0;
		_numAeroportos++;
		printf("airport %s\n", a.id);
	}
}


void mostraAeroporto(int index) {
	printf("%s %s %s %d\n", _aeroportos[index].id,
	       _aeroportos[index].cidade, _aeroportos[index].pais,
	       _aeroportos[index].numVoos);
}


int cmpAeroportos(int a, int b) {
  return (strcmp(_aeroportos[a].id, _aeroportos[b].id) > 0);
}


void listaTodosAeroportos() {
	int i;
	int indexAeroportos[MAX_NUM_AEROPORTOS];

	for (i = 0; i < _numAeroportos; i++)
		indexAeroportos[i] = i;

	bubbleSort(indexAeroportos, _numAeroportos, cmpAeroportos);

	for (i = 0; i < _numAeroportos; i++)
		mostraAeroporto(indexAeroportos[i]);
}


void listaAeroportos() {
	char id[MAX_CODIGO_AEROPORTO];
	int indexAeroporto, ultima = 0;

	ultima = leProximaPalavra(id);
	if (strlen(id) == 0)
		listaTodosAeroportos();
	else {
		while (strlen(id) != 0) {
			indexAeroporto = encontraAeroporto(id);
			if (indexAeroporto == NAO_EXISTE)
				printf("%s: no such airport ID\n", id);
			else
				mostraAeroporto(indexAeroporto);
			if (!ultima)
				ultima = leProximaPalavra(id);
			else
				break;
		}
	}
}



/* Funcoes Voos */

void mostraVoo(int index) {
	printf("%s %s %s ", _voos[index].id, _voos[index].partida,
	       _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooPartida(int index) {
	printf("%s %s ", _voos[index].id, _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooChegada(int index) {
	Hora h = converteNumHora(_voos[index].horaChegada);
	printf("%s %s ", _voos[index].id, _voos[index].partida);
	mostraData(converteNumData(_voos[index].horaChegada));
	printf(" ");
	mostraHora(h);
	printf("\n");
}



int encontraVoo(char id[], Data d) {
	int numData = converteDataNum(d);
	int i;

	for (i = 0; i < _numVoos; i++)
		if (!strcmp(id, _voos[i].id)
		    && numData == converteDataNum(_voos[i].data))
			return i;
	return NAO_EXISTE;
}


int validaIDVoo(char id[]) {
	int i = 0, l = strlen(id);
	if (l < 3)
		return FALSE;
	for (i = 0; i < 2; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return FALSE;

	while (id[i] != '\0') {
		if (!(id[i] >= '0' && id[i] <= '9'))
			return FALSE;
		i++;
	}
	return TRUE;
}

int validaVoo(Voo v) {
	if (validaIDVoo(v.id) == FALSE)
		printf("invalid flight code\n");
	else if (encontraVoo(v.id, v.data) != NAO_EXISTE)
		printf("flight already exists\n");
	else if (encontraAeroporto(v.partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.partida);
	else if (encontraAeroporto(v.chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.chegada);
	else if (_numVoos == MAX_NUM_VOOS)
		printf("too many flihts\n");
	else if (validaData(v.data) == FALSE)
		printf("invalid date\n");
	else if (validaHora(v.duracao) == FALSE)
		printf("invalid duration\n");
	else if (v.capacidade < 10)
		printf("invalid capacity\n");
	else
		return TRUE;
	return FALSE;
}

void criaVoo(Voo v) {
	strcpy(_voos[_numVoos].id, v.id);
	strcpy(_voos[_numVoos].partida, v.partida);
	strcpy(_voos[_numVoos].chegada, v.chegada);
	_voos[_numVoos].data = v.data;
	_voos[_numVoos].hora = v.hora;
	_voos[_numVoos].duracao = v.duracao;
	_voos[_numVoos].capacidade = v.capacidade;
	_voos[_numVoos].horaPartida =
		converteDataHoraNum(_voos[_numVoos].data,
				    _voos[_numVoos].hora);
	_voos[_numVoos].horaChegada =
		_voos[_numVoos].horaPartida +
		converteHoraNum(_voos[_numVoos].duracao);
	_numVoos++;
}

void adicionaListaVoos() {
	Voo v;
	int i;

	if (leProximaPalavra(v.id)) {
		for (i = 0; i < _numVoos; i++)
			mostraVoo(i);
		return;
	} else {
		leProximaPalavra(v.partida);
		leProximaPalavra(v.chegada);
		v.data = leData();
		v.hora = leHora();
		v.duracao = leHora();
		scanf("%d", &v.capacidade);
		leAteFimDeLinha();
	}

	if (validaVoo(v))
		criaVoo(v);
}


int cmpVoosPartida(int a, int b) {
	return (_voos[a].horaPartida > _voos[b].horaPartida);
}


int cmpVoosChegada(int a, int b) {
	return (_voos[a].horaChegada > _voos[b].horaChegada);
}


void listaVoosPartida() {

	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char partida[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(partida);

	if (encontraAeroporto(partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", partida);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].partida, partida) && dataFutura(_voos[i].data))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosPartida);

	for (i = 0; i < n; i++)
		mostraVooPartida(indexVoos[i]);
}


void listaVoosChegada() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char chegada[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(chegada);

	if (encontraAeroporto(chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", chegada);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].chegada, chegada))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosChegada);

	for (i = 0; i < n; i++)
		mostraVooChegada(indexVoos[i]);
}


void alteraData() {
	Data d;

	d = leData();
	leAteFimDeLinha();
	if (validaData(d) == FALSE)
		printf("invalid date\n");
	else {
		_hoje = d;
		mostraData(_hoje);
		printf("\n");
	}
}


/* Linked List das Reservas */


Node *criaNode(Reserva reserva)
{
	Node *node = malloc(sizeof(Node));
	node->reserva = reserva;
	node->next = NULL;
	return node;
}

/* apaga todos os nodes duma linked list cujo primeiro node eh "head"
		e desaloca estes nodes, assim como os codigosReserva
		relacionados a estes */

void apagarLinkedList(Node** head, int* preenchido)
{
	Node* atual = *head;
	Node* prox = NULL;

	while(atual != NULL)
	{
		prox = atual->next;
		(*preenchido)-=atual->reserva.numPassageiros;
		free(atual->reserva.codigoReserva);
		free(atual);
		atual = prox;
	}
	*head = NULL;
}

/* apaga as linkedlists de reservas de todos os voos */

void apagarTodasLinkedList()
{
	int i;
	for(i=0;i<_numVoos;i++)
	{
		apagarLinkedList(&_voos[i].head, &_voos[i].preenchido);
	}
}

/* insere um node no inicio da linkedlist e atualiza a head(Node returnado) */

Node *inserirNodeNoInicio(Node **head, Node *node_a_inserir)
{
	node_a_inserir->next = *head;
	*head = node_a_inserir;
	return node_a_inserir;
}

/* insere novo_node a seguir ao Node "anterior", na linked list */

void inserirNodeASeguir(Node *anterior, Node* novo_node)
{
	novo_node->next = anterior->next;
	anterior->next = novo_node;
}

/* devolve o node da reserva associada ao codigoReserva
		se esta estiver na linkedlist iniciada pela head
		(funcao pra procurar reserva associada a UM VOO ESPECIFICO) */

Node *encontraReservaNumVoo(Node *head, char codigoReserva[])
{
	Node *temp = head;

	while(temp != NULL)
	{
		if(strcmp(codigoReserva, temp->reserva.codigoReserva)==0)
		{
			return temp;
		}
		temp = temp->next;
	}
	return NULL;		 /* se nao existir uma reserva com o codigo desejado
										- no voo cuja head corresponde a linked list das suas reservas -
										a funcao devera retornar NULL */
}

/* devolve o node da reserva associada ao codigoReserva
		(funcao pra procurar reserva associada a QUALQUER VOO) */

Node *encontraReserva(char codigoReserva[])
{
	Node *resultado;
	int i;
	for(i=0;i<_numVoos;i++)
	{
		resultado = encontraReservaNumVoo(_voos[i].head, codigoReserva);
		if(resultado!=NULL) return resultado;
	}
	return NULL; 			/* se nao existir uma reserva com o codigo desejado
										- em nenhum voo -
										a funcao devera retornar NULL */
}



/* encontra e apaga reserva & return 1 OU, caso esta nao exista,
	return FALSE */

int apagaReservaNumVoo(int i, char codigoReserva[])
{
	Node** head = &_voos[i].head;
	Node* temp = *head;
	Node* anterior = NULL;

	/* se a reserva estiver no Node *head */

	if(temp != NULL && (strcmp(codigoReserva, temp->reserva.codigoReserva)==0))
	{
		*head = temp->next;
		_voos[i].preenchido -= temp->reserva.numPassageiros;
		free(temp);
		return TRUE;
	}
	/* else */
	while(temp != NULL && (strcmp(codigoReserva, temp->reserva.codigoReserva)!=0))
	{
		anterior = temp;
		temp = temp->next;
	}
	if(temp == NULL) return FALSE; /* se a reserva nao existir neste voo */
	
	/* else, se existir */
	anterior->next = temp->next;
	_voos[i].preenchido -= temp->reserva.numPassageiros;

	free(temp);

	return TRUE;
}

int apagaReserva(char codigoReserva[])
{
	int done;
	int i;
	for(i=0;i<_numVoos;i++)
	{
		done = apagaReservaNumVoo(i, codigoReserva);
		if(done) return TRUE;
	}
	return FALSE; 			/* se nao existir uma reserva com o codigo desejado
										- em nenhum voo -
										a funcao devera retornar FALSE */
}

void apagaVoo(Voo _voos[], int index, int* _numVoos)
{
	int i;

	for(i=index;i<(*_numVoos)-1;i++)
	{
		_voos[i]=_voos[i+1];
	}

	/* atualizar numero de voos*/
	
	(*_numVoos)--;

}

int apagaReservasPorCodigoVoo(char codigoVoo[])
{
	int done = FALSE;
	int i=0;
	while(i<_numVoos)
	{
		if(strcmp(_voos[i].id, codigoVoo)==0)
		{
			done = TRUE;
			apagarLinkedList(&_voos[i].head, &_voos[i].preenchido);
			apagaVoo(_voos, i, &_numVoos);
		}
		else
		{
			i++;	/* apenas incrementar indice se for apagado um voo caso
						contrario, o voo seguinte sera lido, sem alterar o "i" */
		}
	}
	return done; 		/* se nao existir um voo com o codigo desejado
										a funcao devera retornar 0 */
}


/* troca dois nodes de endereco
(usado para dar sort a linked lists de reservas) */

void troca(Node *a, Node *b)
{
    Reserva temp = a->reserva;
    a->reserva = b->reserva;
    b->reserva = temp;
}

/* dah sort a linked list de reservas por ordem lexilografica */

void bubbleSortLinkedList(Node *head)
{
  int trocado;
  Node *ptr1;
  Node *lptr = NULL;

  /* Checking for empty list */
  if (head == NULL)
    return;
  
  do
  {
    trocado = 0;
    ptr1 = head;

    while (ptr1->next != lptr)
    {
      if (strcmp(ptr1->reserva.codigoReserva,ptr1->next->reserva.codigoReserva)>0)
      { 
        troca(ptr1, ptr1->next);
        trocado = 1;
      }
      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  }
  while (trocado); /* ate nao haver trocas */
}

/* dar "print" a todas as reservas duma linked list de reservas 
		na forma "<codigoReserva> <numeroPassageiros>\n" */

void printarReservas(Node *head)
{
	Node *temp = head;

	while(temp != NULL)
	{
		printf("%s %d\n",
			temp->reserva.codigoReserva, temp->reserva.numPassageiros);
		temp = temp->next;
	}
}


/* adiciona reserva na linked list do voo respetivo
		e atualiza o numero de lugares preenchidos no voo */

void adicionaReserva(Reserva reserva)
{
	Node* node_a_inserir = criaNode(reserva);

	/* descobrir index do voo a que a reserva diz respeito */

	int indexVoo = encontraVoo(reserva.codigoVoo, reserva.data);

	/* atualizar o numero de lugares preenchidos no voo */

	_voos[indexVoo].preenchido += reserva.numPassageiros;

	/* atualizar lista de reservas */

	inserirNodeNoInicio(&_voos[indexVoo].head, node_a_inserir);
}



int upper(char c)
{
	return c >= 'A' && c <= 'Z';
}

int ehDigito(char c){
	return(c>='0' && c<='9');
}

/* este tipo de dado eh utilizado para dar return a uma string
		e a um int ao mesmo tempo, numa funcao */

typedef struct{
	char* word;
	int cond;
} StringAndBool;

/* retorna o codigo lido + um int: 0 se o codigo nao eh valido
																					 1 se o codigo eh valido */

StringAndBool leCodigoReserva(char c){
	StringAndBool output;
	int length;
	char* word = (char*) calloc(2, sizeof(char));
	int num_characters = 2;
	int i=1;
	output.cond = TRUE;
	word[0] = c;

	while((c=getchar())!='\n' && c!='\t' && c!=' ')
	{
		if(! (upper(c) || ehDigito(c)) )
		{
			output.cond = FALSE;
			return output;
		}
		num_characters++;
		word[i] = c;
		word = realloc(word, num_characters*sizeof(char));
		i++;
	}
	word[i]='\0';
	length = strlen(word);
	if(length>=10)
	{
		output.word = word;
		return output;
	}
	else
	{
		output.cond = FALSE;
		return output;
	}
}

/* retorna o primeiro caracter que nao seja ' ' nem '\t' */

char skipWhiteSpaces(){
	char c;
	while((c=getchar())==' '|| c == '\t'){/* nao executar nada ate c nao ser whitespace */}
	return c;
}

int leNumPassageiros(char c){
	int numPassageiros = (c-'0');
	while((c=getchar())!='\n' && c != ' ' && c != '\t')
	{
		numPassageiros*=10;
		numPassageiros+=(c-'0');
	}
	return numPassageiros;
}

/* funcao associada ao comando 'r' */

void adicionaListaReservas() {
	Reserva r;
	char c;

	StringAndBool InfoCodigoReserva;
	int vIndex;
	int numPassInvalido = 0;

	leProximaPalavra(r.codigoVoo);
	r.data = leData();

	vIndex = encontraVoo(r.codigoVoo, r.data);

	c = skipWhiteSpaces();
	if(c=='\n')
	{
		while(1)
		{
			/* erros */
			if(vIndex==NAO_EXISTE)
			{
				printf("%s: flight does not exist\n", r.codigoVoo);
				break;
			}
			if(validaData(r.data) == FALSE)
			{
				printf("invalid date\n");
				break;
			}
		
	


			/* ordenar linkedlist por ordem lexicografica */

			bubbleSortLinkedList(_voos[vIndex].head);

			/* dar "print" a todas as reservas da linkedlist */

			printarReservas(_voos[vIndex].head);
			break;
		}
	}
	else
	{
		while(1)
		{
			/* a funcao seguinte devolve o codigo de reserva
			e a condicao de se este e valido ou nao*/
			
			InfoCodigoReserva = leCodigoReserva(c);

			r.codigoReserva = InfoCodigoReserva.word;
			c = skipWhiteSpaces();
			if(ehDigito(c) == FALSE)
			{
				numPassInvalido = 1;
				leAteFimDeLinha();
			}
			else
			{
				r.numPassageiros = (c-'0');
				while((c=getchar())!='\n' && c != ' ' && c != '\t')
				{
					if(ehDigito(c) == FALSE)
					{
						numPassInvalido = 1;
						leAteFimDeLinha();
						break;
					}
					r.numPassageiros*=10;
					r.numPassageiros+=(c-'0');
				}
			}


			/* validar reserva */
			

			if(InfoCodigoReserva.cond == FALSE) /* se o codigo de reserva nao for valido */
			{
				printf("invalid reservation code\n");
				break;
			}

			if(vIndex==NAO_EXISTE)
			{
				printf("%s: flight does not exist\n", r.codigoVoo);
				break;
			}

			if(encontraReserva(r.codigoReserva) != NULL)
			{
				printf("%s: flight reservation already used\n", r.codigoReserva);
				break;
			}

			if(_voos[vIndex].preenchido + r.numPassageiros > _voos[vIndex].capacidade)
			{
				printf("too many reservations\n");
				break;
			}

			if(validaData(r.data) == FALSE)
			{
				printf("invalid date\n");
				break;
			}

			if(numPassInvalido)
			{
				printf("invalid passenger number\n");
				break;
			}

			/* se nao houver erros (while nao eh breaked) adiciona reserva
			ah linked list respetiva ao voo */

			adicionaReserva(r);
			break;
		}
	}
}

/* le codigo independtemente de ser um codigo de voo, reserva, ou nenhum destes */

char* leCodigo(char c)
{
	int num_characters = 2;
	int i=1;

	char* word = (char*) calloc(2, sizeof(char));

	word[0] = c;

	while((c=getchar())!='\n' && c!='\t' && c!=' ')
	{
		num_characters++;
		word[i] = c;
		word = realloc(word, num_characters*sizeof(char));
		i++;
	}
	word[i]='\0';
	return word;
}

/* funcao associada ao comando 'r' */

void eliminaVoosReservas()
{
	int codigoExiste;
	char* codigo;
	int tamanho;
	char c = skipWhiteSpaces();
	codigo = leCodigo(c);
	tamanho = strlen(codigo);
	if(tamanho >= 10) /* neste caso analisar como sendo um codigo duma reserva */
	{
		codigoExiste = apagaReserva(codigo);
		if(codigoExiste == FALSE)
		{
			printf("not found\n");
		}
	}
	else if(tamanho <= 6) /* neste caso analisar como sendo um codigo dum voo */
	{
		codigoExiste = apagaReservasPorCodigoVoo(codigo);
		if(codigoExiste == FALSE)
		{
			printf("not found\n");
		}
	}
	else printf("not found\n");	/* descartar procura do codigo nas reservas/voos */
}


int main() {
	int c;

	while ((c = getchar()) != EOF) {
		switch (c) {
		case 'q': return 0;
		case 'a': adicionaAeroporto();
			break;
		case 'l': listaAeroportos();
			break;
		case 'v': adicionaListaVoos();
			break;
		case 'p': listaVoosPartida();
			break;
		case 'c': listaVoosChegada();
			break;
		case 't': alteraData();
			break;
		case 'r': adicionaListaReservas();
			break;
		case 'e': eliminaVoosReservas();
			break;
		default: printf("Invalid comand: %c\n", c);
		}
	}

	apagarTodasLinkedList(); /* desalocar todas as linkedlists das reservas */

	return 0;
}
