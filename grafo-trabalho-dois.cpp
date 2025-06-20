#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <queue>
#include <set>
#include <algorithm>

using namespace std;

// ===================================================================
// TAREFA 5: IMPLEMENTAÇÃO DE ÁRVORES BINÁRIAS
// ===================================================================
class NoArvore
{
public:
    int valor;
    NoArvore *esq, *dir;
    NoArvore(int v) : valor(v), esq(nullptr), dir(nullptr) {}
};

class ArvoreBinaria
{
private:
    NoArvore *raiz;

    // Inserção recursiva
    NoArvore *inserirRec(NoArvore *no, int valor)
    {
        if (no == nullptr)
            return new NoArvore(valor);

        if (valor < no->valor)
            no->esq = inserirRec(no->esq, valor);
        else if (valor > no->valor)
            no->dir = inserirRec(no->dir, valor);

        return no;
    }

    // Encontra o nó com o menor valor na árvore
    NoArvore *minimo(NoArvore *no)
    {
        NoArvore *atual = no;
        while (atual && atual->esq != nullptr)
            atual = atual->esq;
        return atual;
    }

    // Remoção recursiva
    NoArvore *removerRec(NoArvore *no, int valor)
    {
        if (no == nullptr)
            return no;

        if (valor < no->valor)
            no->esq = removerRec(no->esq, valor);
        else if (valor > no->valor)
            no->dir = removerRec(no->dir, valor);
        else
        {
            // Caso 1: Nó folha ou com apenas um filho
            if (no->esq == nullptr)
            {
                NoArvore *temp = no->dir;
                delete no;
                return temp;
            }
            else if (no->dir == nullptr)
            {
                NoArvore *temp = no->esq;
                delete no;
                return temp;
            }

            // Caso 2: Nó com dois filhos
            NoArvore *temp = minimo(no->dir);
            no->valor = temp->valor;
            no->dir = removerRec(no->dir, temp->valor);
        }
        return no;
    }

    // Funções auxiliares de percurso
    void preOrdemRec(NoArvore *no)
    {
        if (no != nullptr)
        {
            cout << no->valor << " ";
            preOrdemRec(no->esq);
            preOrdemRec(no->dir);
        }
    }

    void emOrdemRec(NoArvore *no)
    {
        if (no != nullptr)
        {
            emOrdemRec(no->esq);
            cout << no->valor << " ";
            emOrdemRec(no->dir);
        }
    }

    void posOrdemRec(NoArvore *no)
    {
        if (no != nullptr)
        {
            posOrdemRec(no->esq);
            posOrdemRec(no->dir);
            cout << no->valor << " ";
        }
    }

public:
    ArvoreBinaria() : raiz(nullptr) {}

    void inserir(int valor)
    {
        raiz = inserirRec(raiz, valor);
    }

    void remover(int valor)
    {
        raiz = removerRec(raiz, valor);
    }

    void preOrdem()
    {
        preOrdemRec(raiz);
        cout << endl;
    }

    void emOrdem()
    {
        emOrdemRec(raiz);
        cout << endl;
    }

    void posOrdem()
    {
        posOrdemRec(raiz);
        cout << endl;
    } // Cria uma árvore de exemplo
    void criarExemplo()
    {
        // Limpa qualquer árvore existente
        raiz = nullptr;

        // Cria uma árvore exemplo balanceada
        inserir(50);
        inserir(30);
        inserir(70);
        inserir(20);
        inserir(40);
        inserir(60);
        inserir(80);
    } // Mostra os valores da árvore em uma linha (em-ordem)
    void mostrarValores()
    {
        cout << "Valores na arvore: ";
        mostrarValoresRec(raiz);
        cout << endl;
    }

    // Verifica se a árvore está vazia
    bool estaVazia() const
    {
        return raiz == nullptr;
    }

private:
    void mostrarValoresRec(NoArvore *no)
    {
        if (no != nullptr)
        {
            mostrarValoresRec(no->esq);
            cout << no->valor << " ";
            mostrarValoresRec(no->dir);
        }
    }

public:
};

// ===================================================================
// TAREFAS 1-4: GRAFOS
// ===================================================================
class Grafo
{
private:
    int V;            // Número de vértices
    list<int> *adj;   // Lista de adjacência
    bool direcionado; // Indica se o grafo é direcionado
    int tempo;        // Contador para DFS    // Função auxiliar para detectar ciclo em grafo não-direcionado
    bool temCicloUtil(int v, bool visitado[], int pai)
    {
        visitado[v] = true;

        for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            if (!visitado[*i])
            {
                if (temCicloUtil(*i, visitado, v))
                    return true;
            }
            else if (*i != pai)
                return true;
        }
        return false;
    }

    // Função auxiliar para detectar ciclo e encontrar o caminho do ciclo em grafo não-direcionado
    bool encontrarCicloUtil(int v, bool visitado[], int pai, vector<int> &caminho, vector<int> &cicloEncontrado)
    {
        visitado[v] = true;
        caminho.push_back(v);

        for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            if (!visitado[*i])
            {
                if (encontrarCicloUtil(*i, visitado, v, caminho, cicloEncontrado))
                    return true;
            }
            else if (*i != pai)
            {
                // Ciclo encontrado! Construir o caminho do ciclo
                cicloEncontrado.clear();
                auto it = find(caminho.begin(), caminho.end(), *i);
                for (auto j = it; j != caminho.end(); ++j)
                {
                    cicloEncontrado.push_back(*j);
                }
                cicloEncontrado.push_back(*i); // Fechar o ciclo

                // VERIFICAÇÃO: Ciclo deve ter pelo menos 3 vértices distintos
                if (cicloEncontrado.size() >= 4) // 4 porque inclui vértice repetido
                {
                    return true;
                }
                else
                {
                    cicloEncontrado.clear(); // Não é um ciclo válido
                }
            }
        }

        caminho.pop_back();
        return false;
    } // Função auxiliar para detectar ciclo em grafo direcionado
    bool temCicloDirigidoUtil(int v, bool visitado[], bool *recStack)
    {
        if (!visitado[v])
        {
            visitado[v] = true;
            recStack[v] = true;

            for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
            {
                if (!visitado[*i] && temCicloDirigidoUtil(*i, visitado, recStack))
                    return true;
                else if (recStack[*i])
                    return true;
            }
        }
        recStack[v] = false;
        return false;
    }

    // Função auxiliar para detectar ciclo e encontrar o caminho do ciclo em grafo direcionado
    bool encontrarCicloDirigidoUtil(int v, bool visitado[], bool *recStack, vector<int> &caminho, vector<int> &cicloEncontrado)
    {
        if (!visitado[v])
        {
            visitado[v] = true;
            recStack[v] = true;
            caminho.push_back(v);

            for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
            {
                if (!visitado[*i] && encontrarCicloDirigidoUtil(*i, visitado, recStack, caminho, cicloEncontrado))
                    return true;
                else if (recStack[*i])
                {
                    // Ciclo encontrado! Construir o caminho do ciclo
                    cicloEncontrado.clear();
                    auto it = find(caminho.begin(), caminho.end(), *i);
                    for (auto j = it; j != caminho.end(); ++j)
                    {
                        cicloEncontrado.push_back(*j);
                    }
                    cicloEncontrado.push_back(*i); // Fechar o ciclo
                    
                    // VERIFICAÇÃO: Ciclo deve ter pelo menos 3 vértices distintos
                    if (cicloEncontrado.size() >= 4) // 4 porque inclui vértice repetido
                    {
                        return true;
                    }
                    else
                    {
                        cicloEncontrado.clear();
                    }
                }
            }
        }
        recStack[v] = false;
        if (!caminho.empty() && caminho.back() == v)
            caminho.pop_back();
        return false;
    }

    // Função auxiliar para ordenação topológica
    void topoSortUtil(int v, bool visitado[], stack<int> &Stack)
    {
        visitado[v] = true;

        for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
            if (!visitado[*i])
                topoSortUtil(*i, visitado, Stack);

        Stack.push(v);
    }

    void pontoArticulacaoUtil(int u, bool visitado[], int disc[], int low[], int pai[], bool ap[])
    {
        static int time = 0;
        int filhos = 0;
        visitado[u] = true;
        disc[u] = low[u] = ++time;

        for (auto i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            int v = *i;
            if (!visitado[v])
            {
                filhos++;
                pai[v] = u;
                pontoArticulacaoUtil(v, visitado, disc, low, pai, ap);

                low[u] = min(low[u], low[v]);

                if (pai[u] == -1 && filhos > 1)
                    ap[u] = true;
                if (pai[u] != -1 && low[v] >= disc[u])
                    ap[u] = true;
            }
            else if (v != pai[u])
                low[u] = min(low[u], disc[v]);
        }
    }

    // Verificar se o grafo é conectado usando DFS
    bool ehConectado()
    {
        bool *visitado = new bool[V];
        for (int i = 0; i < V; i++)
            visitado[i] = false;

        // Start DFS from vertex 0
        dfsConectado(0, visitado);

        // Verifica se todos os vértices foram visitados
        for (int i = 0; i < V; i++)
        {
            if (!visitado[i])
            {
                delete[] visitado;
                return false;
            }
        }
        delete[] visitado;
        return true;
    }

    void dfsConectado(int v, bool visitado[])
    {
        visitado[v] = true;
        for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
            if (!visitado[*i])
                dfsConectado(*i, visitado);
    }

public:
    Grafo(int V, bool direcionado = false)
    {
        this->V = V;
        this->direcionado = direcionado;
        adj = new list<int>[V];
        tempo = 0;
    }

    ~Grafo()
    {
        delete[] adj;
    }

    void limpar()
    {
        for (int i = 0; i < V; i++)
        {
            adj[i].clear();
        }
    }

    // Adiciona uma aresta ao grafo
    void adicionarAresta(int v, int w)
    {
        adj[v].push_back(w);
        if (!direcionado)
            adj[w].push_back(v);
    } // TAREFA 1: Detecção de ciclos em grafos
    bool temCiclo()
    {
        bool *visitado = new bool[V];
        for (int i = 0; i < V; i++)
            visitado[i] = false;

        // Caso de grafo dirigido
        if (direcionado)
        {
            bool *recStack = new bool[V];
            for (int i = 0; i < V; i++)
                recStack[i] = false;

            for (int i = 0; i < V; i++)
                if (!visitado[i])
                    if (temCicloDirigidoUtil(i, visitado, recStack))
                        return true;

            delete[] recStack;
        }
        // Caso de grafo não-dirigido
        else
        {
            for (int i = 0; i < V; i++)
                if (!visitado[i])
                    if (temCicloUtil(i, visitado, -1))
                        return true;
        }

        delete[] visitado;
        return false;
    }

    // TAREFA 1 MELHORADA: Detecta ciclos e mostra onde estão
    bool detectarEMostrarCiclo()
    {
        bool *visitado = new bool[V];
        for (int i = 0; i < V; i++)
            visitado[i] = false;

        vector<int> cicloEncontrado;

        // Caso de grafo dirigido
        if (direcionado)
        {
            bool *recStack = new bool[V];
            for (int i = 0; i < V; i++)
                recStack[i] = false;

            for (int i = 0; i < V; i++)
            {
                if (!visitado[i])
                {
                    vector<int> caminho;
                    if (encontrarCicloDirigidoUtil(i, visitado, recStack, caminho, cicloEncontrado))
                    {
                        cout << "CICLO ENCONTRADO: ";
                        for (size_t j = 0; j < cicloEncontrado.size(); j++)
                        {
                            cout << cicloEncontrado[j];
                            if (j < cicloEncontrado.size() - 1)
                                cout << " -> ";
                        }
                        cout << endl;
                        delete[] recStack;
                        delete[] visitado;
                        return true;
                    }
                }
            }
            delete[] recStack;
        }

        else
        {
            for (int i = 0; i < V; i++)
            {
                if (!visitado[i])
                {
                    vector<int> caminho;
                    if (encontrarCicloUtil(i, visitado, -1, caminho, cicloEncontrado))
                    {
                        cout << "CICLO ENCONTRADO: ";
                        for (size_t j = 0; j < cicloEncontrado.size(); j++)
                        {
                            cout << cicloEncontrado[j];
                            if (j < cicloEncontrado.size() - 1)
                                cout << " -> ";
                        }
                        cout << endl;
                        delete[] visitado;
                        return true;
                    }
                }
            }
        }

        delete[] visitado;
        cout << "NENHUM CICLO ENCONTRADO" << endl;
        return false;
    }

    // TAREFA 2: Ordenação topológica
    void ordenacaoTopologica()
    {
        if (!direcionado)
        {
            cout << "Ordenacao topologica so se aplica a grafos direcionados!" << endl;
            return;
        }

        stack<int> Stack;
        bool *visitado = new bool[V];
        for (int i = 0; i < V; i++)
            visitado[i] = false;

        for (int i = 0; i < V; i++)
            if (!visitado[i])
                topoSortUtil(i, visitado, Stack);

        cout << "Ordenacao Topologica: ";
        while (!Stack.empty())
        {
            cout << Stack.top() << " ";
            Stack.pop();
        }
        cout << endl;

        delete[] visitado;
    }

    // TAREFA 4: Identificação de pontos de articulação
    vector<int> pontosArticulacao()
    {
        if (direcionado)
        {
            cout << "Pontos de articulacao se aplicam melhor a grafos nao-direcionados!" << endl;
        }

        bool *visitado = new bool[V];
        int *disc = new int[V];
        int *low = new int[V];
        int *pai = new int[V];
        bool *ap = new bool[V];

        for (int i = 0; i < V; i++)
        {
            pai[i] = -1;
            visitado[i] = false;
            ap[i] = false;
        }

        for (int i = 0; i < V; i++)
            if (!visitado[i])
                pontoArticulacaoUtil(i, visitado, disc, low, pai, ap);

        vector<int> pontos;
        for (int i = 0; i < V; i++)
            if (ap[i])
                pontos.push_back(i);

        delete[] visitado;
        delete[] disc;
        delete[] low;
        delete[] pai;
        delete[] ap;

        return pontos;
    }

    // TAREFA 3: Verificação de grafo biconexo
    bool ehBiconexo()
    {
        if (direcionado)
        {
            cout << "Biconexidade se aplica a grafos nao-direcionados!" << endl;
            return false;
        }

        if (!ehConectado())
            return false;

        vector<int> pa = pontosArticulacao();
        return pa.empty();
    } // Função para exibir o grafo
    void exibirGrafo()
    {
        cout << "\n--- Estrutura do Grafo ---\n";
        cout << "Vertices: " << V << "\n";
        cout << "Tipo: " << (direcionado ? "Direcionado" : "Nao-direcionado") << "\n";
        cout << "Lista de adjacencia:\n";
        for (int v = 0; v < V; ++v)
        {
            cout << "  " << v << (direcionado ? " -> " : " -- ");
            if (adj[v].empty())
            {
                cout << "(sem conexoes)";
            }
            else
            {
                bool primeiro = true;
                for (auto x : adj[v])
                {
                    if (!primeiro)
                        cout << ", ";
                    cout << x;
                    primeiro = false;
                }
            }
            cout << endl;
        }

        // Para grafos não direcionados, mostrar as arestas sem repetição
        if (!direcionado)
        {
            cout << "\nArestas do grafo:\n";
            set<pair<int, int>> arestas;
            for (int v = 0; v < V; ++v)
            {
                for (auto u : adj[v])
                {
                    if (v < u) // Evita duplicatas
                    {
                        arestas.insert({v, u});
                    }
                }
            }

            if (arestas.empty())
            {
                cout << "  (nenhuma aresta)\n";
            }
            else
            {
                for (auto aresta : arestas)
                {
                    cout << "  " << aresta.first << " -- " << aresta.second << endl;
                }
            }
        }
        else
        {
            cout << "\nArestas do grafo:\n";
            bool temAresta = false;
            for (int v = 0; v < V; ++v)
            {
                for (auto u : adj[v])
                {
                    cout << "  " << v << " -> " << u << endl;
                    temAresta = true;
                }
            }
            if (!temAresta)
            {
                cout << "  (nenhuma aresta)\n";
            }
        }
        cout << "-------------------------\n";
    }
};

// ===================================================================
// EXEMPLOS DIRETOS PARA DEMONSTRAÇÃO
// ===================================================================

// Exemplos para grafos não direcionados
void exemploDeteccaoCiclo()
{
    cout << "\n=== DEMONSTRACAO: DETECCAO DE CICLO ===\n"; // Cenário 1: SEM CICLO (árvore) - vértices 0,1,2,3,4,5
    Grafo semCiclo(6, false);
    semCiclo.adicionarAresta(0, 1);
    semCiclo.adicionarAresta(0, 2);
    semCiclo.adicionarAresta(1, 3);
    semCiclo.adicionarAresta(1, 4);
    semCiclo.adicionarAresta(2, 5);
    cout << "1. Grafo sem ciclo:\n";
    semCiclo.exibirGrafo();
    cout << "Deteccao de ciclo: ";
    semCiclo.detectarEMostrarCiclo();
    cout << "\n";

    // Cenário 2: COM CICLO - vértices 0,1,2,3,4,5
    Grafo comCiclo(6, false);
    comCiclo.adicionarAresta(0, 1);
    comCiclo.adicionarAresta(1, 2);
    comCiclo.adicionarAresta(2, 0); // Ciclo 0-1-2-0
    comCiclo.adicionarAresta(1, 3);
    comCiclo.adicionarAresta(3, 4);
    comCiclo.adicionarAresta(4, 5);
    cout << "2. Grafo com ciclo:\n";
    comCiclo.exibirGrafo();
    cout << "Deteccao de ciclo: ";
    comCiclo.detectarEMostrarCiclo();
    cout << "\n";

    cout << "\nPressione Enter para continuar...";
    cin.ignore();
    cin.get();
}

void exemploDeteccaoCicloDirigido()
{
    cout << "\n=== DEMONSTRACAO: DETECCAO DE CICLO EM GRAFO DIRIGIDO ===\n"; // Cenário 1: DAG (sem ciclo) - vértices 0,1,2,3,4,5
    Grafo dagSemCiclo(6, true);
    dagSemCiclo.adicionarAresta(0, 1);
    dagSemCiclo.adicionarAresta(0, 2);
    dagSemCiclo.adicionarAresta(1, 3);
    dagSemCiclo.adicionarAresta(2, 4);
    dagSemCiclo.adicionarAresta(4, 5);
    cout << "1. Grafo direcionado sem ciclo:\n";
    dagSemCiclo.exibirGrafo();
    cout << "Deteccao de ciclo: ";
    dagSemCiclo.detectarEMostrarCiclo();
    cout << "\n";

    // Cenário 2: COM CICLO - vértices 0,1,2,3,4,5
    Grafo comCicloDirigido(6, true);
    comCicloDirigido.adicionarAresta(0, 1);
    comCicloDirigido.adicionarAresta(1, 2);
    comCicloDirigido.adicionarAresta(2, 0);
    comCicloDirigido.adicionarAresta(1, 3);
    comCicloDirigido.adicionarAresta(3, 4);
    comCicloDirigido.adicionarAresta(4, 5);
    cout << "2. Grafo direcionado com ciclo:\n";
    comCicloDirigido.exibirGrafo();
    cout << "Deteccao de ciclo: ";
    comCicloDirigido.detectarEMostrarCiclo();
    cout << "\n";

    cout << "\nPressione Enter para continuar...";
    cin.ignore();
    cin.get();
}

void exemploOrdenacaoTopologica()
{
    cout << "\n=== DEMONSTRACAO: ORDENACAO TOPOLOGICA ===\n"; // DAG para ordenação topológica - vértices 0,1,2,3,4,5
    Grafo dag(6, true);
    dag.adicionarAresta(5, 2);
    dag.adicionarAresta(5, 0);
    dag.adicionarAresta(4, 0);
    dag.adicionarAresta(4, 1);
    dag.adicionarAresta(2, 3);
    dag.adicionarAresta(3, 1);

    cout << "Grafo direcionado para ordenacao topologica:\n";
    dag.exibirGrafo();
    cout << "Ordenacao topologica: ";
    dag.ordenacaoTopologica();

    cout << "\nPressione Enter para continuar...";
    cin.ignore();
    cin.get();
}

void exemploPontosArticulacao()
{
    cout << "\n=== DEMONSTRACAO: PONTOS DE ARTICULACAO ===\n";

    // vértices 0,1,2,3,4,5 - como no desenho do amigo
    Grafo gArt(6, false);
    gArt.adicionarAresta(0, 1);
    gArt.adicionarAresta(1, 2);
    gArt.adicionarAresta(1, 3);
    gArt.adicionarAresta(3, 4);
    gArt.adicionarAresta(4, 5);

    cout << "Grafo com ponto de articulação:\n";
    gArt.exibirGrafo();

    vector<int> pontos = gArt.pontosArticulacao();
    cout << "Pontos de articulacao: ";
    if (pontos.empty())
    {
        cout << "Nenhum\n";
    }
    else
    {
        for (size_t i = 0; i < pontos.size(); i++)
        {
            cout << "[" << pontos[i] << "]";
            if (i < pontos.size() - 1)
                cout << " - ";
        }
        cout << endl;
    }

    cout << "\nPressione Enter para continuar...";
    cin.ignore();
    cin.get();
}

void exemploBiconexo()
{
    cout << "\n=== DEMONSTRACAO: GRAFO BICONEXO ===\n";

    Grafo biconexo(6, false);
    biconexo.adicionarAresta(0, 1);
    biconexo.adicionarAresta(0, 2);
    biconexo.adicionarAresta(1, 2);
    biconexo.adicionarAresta(1, 3);
    biconexo.adicionarAresta(2, 3);
    biconexo.adicionarAresta(1, 4);
    biconexo.adicionarAresta(2, 4);
    biconexo.adicionarAresta(3, 4);
    biconexo.adicionarAresta(3, 5);
    biconexo.adicionarAresta(4, 5);

    cout << "1. Grafo biconexo:\n";
    biconexo.exibirGrafo();
    cout << "E biconexo? " << (biconexo.ehBiconexo() ? "SIM" : "NAO") << "\n\n";

    Grafo naoBiconexo(6, false);
    naoBiconexo.adicionarAresta(0, 1);
    naoBiconexo.adicionarAresta(1, 2);
    naoBiconexo.adicionarAresta(1, 3);
    naoBiconexo.adicionarAresta(3, 4);
    naoBiconexo.adicionarAresta(4, 5);

    cout << "2. Grafo nao biconexo:\n";
    naoBiconexo.exibirGrafo();
    cout << "E biconexo? " << (naoBiconexo.ehBiconexo() ? "SIM" : "NAO") << "\n";

    cout << "\nPressione Enter para continuar...";
    cin.ignore();
    cin.get();
}

void exemploArvore()
{
    cout << "\n=== DEMONSTRACAO: ARVORE BINARIA ===\n";

    // Criar árvore exemplo
    ArvoreBinaria arv;
    arv.criarExemplo();

    cout << "Arvore exemplo criada!\n";
    arv.mostrarValores();
    cout << "\n";

    cout << "Pre-ordem: ";
    arv.preOrdem();

    cout << "Em-ordem: ";
    arv.emOrdem();

    cout << "Pos-ordem: ";
    arv.posOrdem();

    cout << "\nRemovendo o no com valor 30...\n";
    arv.remover(30);

    cout << "Depois da remocao:\n";
    arv.mostrarValores();
    cout << "Pre-ordem: ";
    arv.preOrdem();
    cout << "Em-ordem: ";
    arv.emOrdem();
    cout << "Pos-ordem: ";
    arv.posOrdem();

    cout << "\nPressione Enter para continuar...";
    cin.ignore();
    cin.get();
}

void menuArvoreInterativo()
{
    cout << "\n=== MENU INTERATIVO - ARVORE BINARIA ===\n";

    ArvoreBinaria arvore;
    int opcao, valor;

    do
    {
        cout << "\n--- Menu de Arvore Binaria ---\n";
        cout << "1. Inserir Elemento\n";
        cout << "2. Remover Elemento\n";
        cout << "3. Percorrer em Pre-Ordem\n";
        cout << "4. Percorrer em Em-Ordem\n";
        cout << "5. Percorrer em Pos-Ordem\n";
        cout << "6. Mostrar todos os valores\n";
        cout << "7. Verificar se arvore esta vazia\n";
        cout << "0. Voltar ao menu principal\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao)
        {
        case 1:
            cout << "Digite o valor a inserir: ";
            cin >> valor;
            arvore.inserir(valor);
            cout << "Valor " << valor << " inserido!\n";
            break;

        case 2:
            if (arvore.estaVazia())
            {
                cout << "A arvore esta vazia. Nada para remover.\n";
            }
            else
            {
                cout << "Digite o valor a remover: ";
                cin >> valor;
                arvore.remover(valor);
            }
            break;

        case 3:
            if (arvore.estaVazia())
            {
                cout << "A arvore esta vazia.\n";
            }
            else
            {
                arvore.preOrdem();
            }
            break;

        case 4:
            if (arvore.estaVazia())
            {
                cout << "A arvore esta vazia.\n";
            }
            else
            {
                arvore.emOrdem();
            }
            break;

        case 5:
            if (arvore.estaVazia())
            {
                cout << "A arvore esta vazia.\n";
            }
            else
            {
                arvore.posOrdem();
            }
            break;

        case 6:
            if (arvore.estaVazia())
            {
                cout << "A arvore esta vazia.\n";
            }
            else
            {
                arvore.mostrarValores();
            }
            break;

        case 7:
            cout << "A arvore " << (arvore.estaVazia() ? "ESTA" : "NAO ESTA") << " vazia.\n";
            break;

        case 0:
            cout << "Voltando ao menu principal...\n";
            break;

        default:
            cout << "Opcao invalida! Tente novamente.\n";
        }
    } while (opcao != 0);
}

// ===================================================================
// MENU PRINCIPAL E MAIN
// ===================================================================
void menuDemonstracao()
{
    int opcao;

    do
    {
        cout << "\n===== DEMONSTRACAO DO TRABALHO 2 =====\n";
        cout << "1. Deteccao de ciclos em grafos nao dirigidos\n";
        cout << "2. Deteccao de ciclos em grafos dirigidos\n";
        cout << "3. Ordenacao Topologica\n";
        cout << "4. Pontos de Articulacao\n";
        cout << "5. Verificacao de grafo biconexo\n";
        cout << "6. Arvore Binaria (exemplo automatico)\n";
        cout << "7. Arvore Binaria (menu interativo)\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao)
        {
        case 1:
            exemploDeteccaoCiclo();
            break;
        case 2:
            exemploDeteccaoCicloDirigido();
            break;
        case 3:
            exemploOrdenacaoTopologica();
            break;
        case 4:
            exemploPontosArticulacao();
            break;
        case 5:
            exemploBiconexo();
            break;
        case 6:
            exemploArvore();
            break;
        case 7:
            menuArvoreInterativo();
            break;
        case 0:
            cout << "Obrigado por usar o programa!\n";
            break;
        default:
            cout << "Opcao invalida! Tente novamente.\n";
        }
    } while (opcao != 0);
}

int main()
{
    cout << "Bem-vindo ao sistema de demonstracao do Trabalho 2!\n";
    menuDemonstracao();
    return 0;
}