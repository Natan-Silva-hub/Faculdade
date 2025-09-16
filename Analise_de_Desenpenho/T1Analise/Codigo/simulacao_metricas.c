#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    double tempo_anterior;
    unsigned long int qt_requisicoes;
    double soma_area;
} medida_little;

double aleatorio() {
    double u = rand() / ((double) RAND_MAX + 1);
    u = 1.0 - u;
    return (u);
}

double exponencial(double l){
    return (-1.0/l)*log(aleatorio());
}

double minimo(double n1, double n2){
    if(n1 < n2) return n1;
    return n2;
}

void inicia_little(medida_little * medidas){
    medidas->tempo_anterior = 0.0;
    medidas->qt_requisicoes = 0;
    medidas->soma_area = 0.0;
}

int main(){
    srand(12345); // Seed fixa para reprodutibilidade

    medida_little E_N;
    medida_little E_W_chegadas;
    medida_little E_W_saidas;
    inicia_little(&E_N);
    inicia_little(&E_W_chegadas);
    inicia_little(&E_W_saidas);

    double tempo_decorrido = 0.0;
    double tempo_simulacao = 86400.0; // 24 horas

    double media_inter_requisicoes;
    double media_tempo_servico;

    double proxima_requisicao;
    double tempo_servico;

    unsigned long int fila = 0;
    unsigned long int max_fila = 0;

    unsigned long int qtd_requisicoes = 0;
    double soma_inter_requisicoes;
    unsigned long int qtd_servicos = 0;
    double soma_tempo_servico = 0.0;

    // Exportação para arquivos
    FILE *arquivo_en, *arquivo_ew, *arquivo_erro_little, *arquivo_lambda, *arquivo_resultados;
    arquivo_en = fopen("en.txt", "w");
    arquivo_ew = fopen("ew.txt", "w");
    arquivo_erro_little = fopen("erro_little.txt", "w");
    arquivo_lambda = fopen("lambda.txt", "w");
    arquivo_resultados = fopen("resultados_finais.txt", "w");

    if (!arquivo_en || !arquivo_ew || !arquivo_erro_little || !arquivo_lambda || !arquivo_resultados) {
        printf("Erro ao criar arquivos de coleta\n");
        return 1;
    }

    fprintf(arquivo_en, "# Tempo(s) E[N]\n");
    fprintf(arquivo_ew, "# Tempo(s) E[W]\n");
    fprintf(arquivo_erro_little, "# Tempo(s) Erro_Little\n");
    fprintf(arquivo_lambda, "# Tempo(s) Lambda\n");

    printf("Informe a media de tempo entre requisicoes: ");
    scanf("%lf", &media_inter_requisicoes);
    media_inter_requisicoes = 1.0/media_inter_requisicoes;

    printf("Informe a media de tempo para atendimentos: ");
    scanf("%lf", &media_tempo_servico);
    media_tempo_servico = 1.0/media_tempo_servico;

    proxima_requisicao = exponencial(media_inter_requisicoes);
    qtd_requisicoes++;
    soma_inter_requisicoes = proxima_requisicao;

    // Coleta periódica a cada 10 segundos
    double tempo_coleta = 10.0;

    while(tempo_decorrido < tempo_simulacao){
        double proximo_evento = fila ?
            minimo(minimo(proxima_requisicao, tempo_servico), tempo_coleta) :
            minimo(proxima_requisicao, tempo_coleta);

        tempo_decorrido = proximo_evento;

        // Coleta periódica
        if (tempo_decorrido == tempo_coleta && tempo_coleta <= tempo_simulacao) {
            E_N.soma_area += (tempo_decorrido - E_N.tempo_anterior) * E_N.qt_requisicoes;
            E_N.tempo_anterior = tempo_decorrido;

            E_W_chegadas.soma_area += (tempo_decorrido - E_W_chegadas.tempo_anterior) * E_W_chegadas.qt_requisicoes;
            E_W_chegadas.tempo_anterior = tempo_decorrido;

            E_W_saidas.soma_area += (tempo_decorrido - E_W_saidas.tempo_anterior) * E_W_saidas.qt_requisicoes;
            E_W_saidas.tempo_anterior = tempo_decorrido;

            double E_N_atual = E_N.soma_area / tempo_decorrido;
            double E_W_atual = 0.0;
            if (E_W_chegadas.qt_requisicoes > 0) {
                E_W_atual = (E_W_chegadas.soma_area - E_W_saidas.soma_area) / E_W_chegadas.qt_requisicoes;
            }
            double lambda_atual = E_W_chegadas.qt_requisicoes / tempo_decorrido;
            double erro_little = E_N_atual - lambda_atual * E_W_atual;

            fprintf(arquivo_en, "%.1f %.6f\n", tempo_decorrido, E_N_atual);
            fprintf(arquivo_ew, "%.1f %.6f\n", tempo_decorrido, E_W_atual);
            fprintf(arquivo_erro_little, "%.1f %.6f\n", tempo_decorrido, erro_little);
            fprintf(arquivo_lambda, "%.1f %.6f\n", tempo_decorrido, lambda_atual);

            tempo_coleta += 10.0;
        }

        if(tempo_decorrido == proxima_requisicao){
            fila++;
            max_fila = fila > max_fila ? fila : max_fila;

            if(fila == 1){
                tempo_servico = tempo_decorrido + exponencial(media_tempo_servico);
                qtd_servicos++;
                soma_tempo_servico += tempo_servico - tempo_decorrido;
            }

            proxima_requisicao = tempo_decorrido + exponencial(media_inter_requisicoes);

            qtd_requisicoes++;
            soma_inter_requisicoes += proxima_requisicao - tempo_decorrido;

            E_N.soma_area += (tempo_decorrido - E_N.tempo_anterior) * E_N.qt_requisicoes;
            E_N.qt_requisicoes++;
            E_N.tempo_anterior = tempo_decorrido;

            E_W_chegadas.soma_area += (tempo_decorrido - E_W_chegadas.tempo_anterior) * E_W_chegadas.qt_requisicoes;
            E_W_chegadas.qt_requisicoes++;
            E_W_chegadas.tempo_anterior = tempo_decorrido;
        }else if(fila > 0 && tempo_decorrido == tempo_servico){
            fila--;

            if(fila){
                tempo_servico = tempo_decorrido + exponencial(media_tempo_servico);
                qtd_servicos++;
                soma_tempo_servico += tempo_servico - tempo_decorrido;
            }

            E_N.soma_area += (tempo_decorrido - E_N.tempo_anterior) * E_N.qt_requisicoes;
            E_N.qt_requisicoes--;
            E_N.tempo_anterior = tempo_decorrido;

            E_W_saidas.soma_area += (tempo_decorrido - E_W_saidas.tempo_anterior) * E_W_saidas.qt_requisicoes;
            E_W_saidas.qt_requisicoes++;
            E_W_saidas.tempo_anterior = tempo_decorrido;
        }
    }

    E_W_chegadas.soma_area += (tempo_decorrido - E_W_chegadas.tempo_anterior) * E_W_chegadas.qt_requisicoes;
    E_W_saidas.soma_area += (tempo_decorrido - E_W_saidas.tempo_anterior) * E_W_saidas.qt_requisicoes;

    double E_N_final = E_N.soma_area/tempo_decorrido;
    double E_W_final = (E_W_chegadas.soma_area - E_W_saidas.soma_area)/E_W_chegadas.qt_requisicoes;
    double lambda_final = E_W_chegadas.qt_requisicoes/tempo_decorrido;
    double erro_little = E_N_final - lambda_final * E_W_final;

    // Exporta resultados finais
    fprintf(arquivo_en, "%.1f %.6f\n", tempo_decorrido, E_N_final);
    fprintf(arquivo_ew, "%.1f %.6f\n", tempo_decorrido, E_W_final);
    fprintf(arquivo_erro_little, "%.1f %.6f\n", tempo_decorrido, erro_little);
    fprintf(arquivo_lambda, "%.1f %.6f\n", tempo_decorrido, lambda_final);

    fprintf(arquivo_resultados, "max fila: %lu\n", max_fila);
    fprintf(arquivo_resultados, "media entre requisicoes: %.6f\n", soma_inter_requisicoes/qtd_requisicoes);
    fprintf(arquivo_resultados, "media tempos de servico: %.6f\n", qtd_servicos > 0 ? soma_tempo_servico/qtd_servicos : 0.0);
    fprintf(arquivo_resultados, "ocupacao esperada: %.6f\n", media_inter_requisicoes/media_tempo_servico);
    fprintf(arquivo_resultados, "ocupacao calculada: %.6f\n", soma_tempo_servico/tempo_decorrido);
    fprintf(arquivo_resultados, "E[N]: %.6f\n", E_N_final);
    fprintf(arquivo_resultados, "E[W]: %.6f\n", E_W_final);
    fprintf(arquivo_resultados, "Lambda: %.6f\n", lambda_final);
    fprintf(arquivo_resultados, "Erro little: %.6f\n", erro_little);

    fclose(arquivo_en);
    fclose(arquivo_ew);
    fclose(arquivo_erro_little);
    fclose(arquivo_lambda);
    fclose(arquivo_resultados);

    return 0;
}