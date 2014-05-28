//
//  main.cpp
//  ReadFile
//
//  Created by Leonardo Pache on 5/22/14.
//  Copyright (c) 2014 Example. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include "pthread.h"

using namespace std;

//enum para identificação dos hospedeiros
const string path = "/Users/lpache/demuxLog/";
const string log = "/log.txt";
const string maq_1 = "maq_1";
const string maq_2 = "maq_2";
const string maq_3 = "maq_3";
const string maq_x = "maq_x";

string t_maq_1 (maq_1);
string t_maq_2 (maq_2);
string t_maq_3 (maq_3);
string t_maq_x (maq_x);


//funcoes
void realizaProcessamentoLogs();
void realizaOrdenacaoLogs();
void* iniciarProcessamentoDeArquivosDeLog(void* maquina);
void* iniciarOrdenacaoDeArquivosDeLog(void* maquina);
void tratarLinhaLog(string log, string* maq_);
string recuperarSubstring (const std::string& str, const std::string& c, const int tam);
void inserirLogArquivo(const std::string& fileName, const std::string& logLine);
tm converterStringParaData(string dateTime);
void ordanaArquivoFinalPelaData(const std::string& fileName);
int compare (string aux1, string aux2);
vector<string> split(string str, string sep);
int getIntMonthToStringMonth(string month);
void removerLogsAnteriores(string dirPath);
void inicializaAmbiente();

int main()
{
    removerLogsAnteriores(path+maq_1);
    removerLogsAnteriores(path+maq_2);
    removerLogsAnteriores(path+maq_3);
    removerLogsAnteriores(path+maq_x);
    realizaProcessamentoLogs();
	realizaOrdenacaoLogs();
    
    return 0;
}

//configura ambiente das maquinas simuladas
void realizaProcessamentoLogs()
{
    //inicializa threads
    pthread_t f1, f2, f3, fx;
    
   
    std::cout << "Processamento de arquivos iniciados \n";
    //chama Threads para varredura de arquivos de log
    pthread_create( &f1, NULL, iniciarProcessamentoDeArquivosDeLog,reinterpret_cast<void*>(&t_maq_1));
    pthread_create( &f2, NULL, iniciarProcessamentoDeArquivosDeLog,reinterpret_cast<void*>(&t_maq_2));
    pthread_create( &f3, NULL, iniciarProcessamentoDeArquivosDeLog,reinterpret_cast<void*>(&t_maq_3));
    pthread_create( &fx, NULL, iniciarProcessamentoDeArquivosDeLog,reinterpret_cast<void*>(&t_maq_x));
    
    // synchronize threads:
    pthread_join(f1,NULL);
    pthread_join(f2,NULL);
    pthread_join(f3,NULL);
    pthread_join(fx,NULL);
    std::cout << "Arquivos de log criados com sucesso!! \n";
}

//configura ambiente das maquinas simuladas
void realizaOrdenacaoLogs()
{
    //inicializa threads
	pthread_t d1, d2, d3, dx;
    
    std::cout << "Ordenacao de arquivos iniciados \n";
    //chama Threads para varredura de arquivos de log em cada maquina
    pthread_create( &d1, NULL, iniciarOrdenacaoDeArquivosDeLog,reinterpret_cast<void*>(&t_maq_1));
    pthread_create( &d2, NULL, iniciarOrdenacaoDeArquivosDeLog,reinterpret_cast<void*>(&t_maq_2));
    pthread_create( &d3, NULL, iniciarOrdenacaoDeArquivosDeLog,reinterpret_cast<void*>(&t_maq_3));
    pthread_create( &dx, NULL, iniciarOrdenacaoDeArquivosDeLog,reinterpret_cast<void*>(&t_maq_x));
    
    // synchronize threads:
    pthread_join(d1,NULL);
    pthread_join(d2,NULL);
    pthread_join(d3,NULL);
    pthread_join(dx,NULL);
    
    std::cout << "Arquivos de log ordenados com sucesso!! \n";
}

//inicia processamento paralelo
void* iniciarProcessamentoDeArquivosDeLog(void* maquina)
{
    string tmp;
    ifstream fin;
    string* maq = (reinterpret_cast<string*>(maquina));
    fin.open(path+*maq+log);
    if (fin.is_open()) {
        while (!fin.eof()) {
            getline(fin, tmp);
            tratarLinhaLog(tmp, maq);
        }
        fin.close();
    }
    else
        cout << "unable to open file";
    return 0;
}

//funcao responsavel por ser o roteador das informacoes
void tratarLinhaLog(string log, string* maq_)
{
    //separa informacoes importantes da string
    string user_id = recuperarSubstring(log, "userid=", 36);
    
    //chama funcao responsavel por ordenar e inserir no arquivo de log
    inserirLogArquivo(path+*maq_+"/"+user_id+".txt", log);
}

void* iniciarOrdenacaoDeArquivosDeLog(void* maquina)
{
    DIR *dir = 0;
    string* maq_ = (static_cast<string*>(maquina));
    string dirPath = path+*maq_;
    struct dirent *entrada = 0;
    unsigned char isFile = 0x8;
    
    dir = opendir (dirPath.c_str());
    
    if (dir == 0) {
        std::cerr << "Nao foi possivel abrir diretorio." << std::endl;
        exit (1);
    }
    
    //Iterar sobre o diretorio
    while ((entrada = readdir (dir)))
        if (entrada->d_type == isFile)
        {
            if (entrada->d_namlen == 40) {
                ordanaArquivoFinalPelaData(dirPath+"/"+entrada->d_name);
            }
        }
    closedir (dir);
    
    return 0;
}

//split log
string recuperarSubstring (const std::string& str, const std::string& c, int tam)
{
    long found = str.find(c);
    return str.substr(found+c.length(),tam);
}

//realiza a insercao ordenada
void inserirLogArquivo(const std::string& fileName, const std::string& logLine)
{
    ofstream file;
	file.open(fileName,ios_base::in); /* Abre para leitura */
 	if(!file)
	{
		file.open(fileName); /* Se não existe, cria */
	}
    file.close();
    
    fstream fileTxt(fileName, fstream::in );
    if (fileTxt)
    {
        string s;
        getline(fileTxt, s, '\0');
        fileTxt.close();
        std::ofstream out(fileName);
        out << s+logLine+"\n";
        out.close();
    }
}


//metodo responsável pela ordenacao do conteudo do arquivo
void ordanaArquivoFinalPelaData(const std::string& fileName)
{
    fstream file(fileName, fstream::in );
    vector<string> arrayOfLog;
    string tmp;
    
    if (file)
    {
        //carraga conteudo do arquivo para um vetor
        while (!file.eof()) {
            getline(file,tmp);
            if(tmp != "")
                arrayOfLog.push_back(tmp);
        }
        file.close();
        
        for (int i = 0; i < arrayOfLog.size(); i++) {
            for (int k = 0; k < arrayOfLog.size(); k++) {
                //cout << arrayOfLog[i] << "\n" << arrayOfLog[k] << "\n" << endl;
                int r_ = compare(arrayOfLog[i], arrayOfLog[k]);
                if (r_ < 0) {
                    const string tmp = arrayOfLog[i];
                    arrayOfLog[i] = arrayOfLog[k];
                    arrayOfLog[k] = tmp;
                    //cout << "changed" << endl;
                }
            }
            //cout << arrayOfLog[i] << endl;
        }
        
        string content;
        for (int i=0; i < arrayOfLog.size(); i++) {
            content = content+arrayOfLog[i]+"\n";
        }
        std::ofstream out(fileName);
        out << content;
        out.close();
    }
}

int compare (string aux1, string aux2)
{
    double difference;
    struct tm dateTime1 = converterStringParaData(recuperarSubstring(aux1,"[",20));
    struct tm dateTime2 = converterStringParaData(recuperarSubstring(aux2, "[",20));
    
    std::time_t x = std::mktime(&dateTime1);
    std::time_t y = std::mktime(&dateTime2);
    
    if ( x != (std::time_t)(-1) && y != (std::time_t)(-1) )
    {
        difference = std::difftime(x, y) / (60 * 60 * 24);
    }
    cout << difference << "\n" << endl;
    if ( difference < 0 )
    {
        return -1;
    }else if ( difference > 0 )
    {
        return 1;
    }else
    {
        return 0;
    }
}

//realiza conversao de uma string em uma estrutura do tipo dateTime
tm converterStringParaData(string dateTime)
{
    struct tm tm1;
    vector<string> dia = split(dateTime, "/");
    dia[2] = split(dia[2], ":")[0];
    vector<string> aux = split(dateTime, ":");
    vector<string> hora = {aux[1],aux[2],aux[3]};
    
    tm1.tm_mday = atoi(dia[0].c_str());
    tm1.tm_mon = getIntMonthToStringMonth(dia[1]);
    tm1.tm_year = atoi(dia[2].c_str());
    tm1.tm_hour = atoi(hora[0].c_str());
    tm1.tm_min = atoi(hora[1].c_str());
    tm1.tm_sec = atoi(hora[2].c_str());
    
    return tm1;
}

vector<string> split(string str, string sep)
{
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current != NULL){
        arr.push_back(current);
        current=strtok(NULL, sep.c_str());
    }
    return arr;
}

int getIntMonthToStringMonth(string month){
    
    std::string months[] = { "Jan", "Feb", "Mar", "April", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec" };
    for(int i=1; i <= 12; i++)
    {
        if(months[i].compare(month) == 0)
        {
            return i;
        }
    }
    return 0;
}


void removerLogsAnteriores(string dirPath)
{
    DIR *dir = 0;
    string logFileName = "log.txt";
    struct dirent *entrada = 0;
    unsigned char isFile = 0x8;
    
    dir = opendir (dirPath.c_str());
    
    if (dir == 0) {
        std::cerr << "Nao foi possivel abrir diretorio." << std::endl;
        exit (1);
    }
    
    //Iterar sobre o diretorio
    while ((entrada = readdir (dir)))
        if (entrada->d_type == isFile)
        {
            if (strcmp(entrada->d_name,logFileName.c_str())!=0) {
                string file = dirPath+"/"+entrada->d_name;
                if( remove(file.c_str()) != 0 )
                    perror( "Error deleting file" );
                else
                    puts( "File successfully deleted" );
            }
        }
    closedir (dir);
}

