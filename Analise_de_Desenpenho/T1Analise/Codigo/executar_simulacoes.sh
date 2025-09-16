#!/bin/bash

echo "=== Iniciando simulações para coleta de métricas ==="
echo "Compilando código..."

gcc -o simulacao_metricas simulacao_metricas.c -lm

if [ $? -ne 0 ]; then
    echo "Erro na compilação!"
    exit 1
fi

echo "Compilação concluída com sucesso!"

# Cria as pastas organizadas para cada métrica
mkdir -p ../Arquivos_gerados/en
mkdir -p ../Arquivos_gerados/ew
mkdir -p ../Arquivos_gerados/erro_little
mkdir -p ../Arquivos_gerados/resultados_finais
mkdir -p ../Arquivos_gerados/lambda

ocupacoes=(50.0 80.0 95.0 99.9)
media_servico=1

for ocupacao in "${ocupacoes[@]}"; do
    taxa_chegada=$(echo "$ocupacao * $media_servico / 100" | bc -l)
    tempo_medio_chegada=$(echo "1 / $taxa_chegada" | bc -l)
    tempo_medio_servico=$(echo "1 / $media_servico" | bc -l)
    echo ""
    echo "Ocupação: $ocupacao%"
    echo "Tempo médio entre chegadas: $tempo_medio_chegada"
    echo "Tempo médio de serviço: $tempo_medio_servico"
    echo -e "$tempo_medio_chegada\n$tempo_medio_servico" | ./simulacao_metricas

    # Move os arquivos para suas respectivas pastas
    mv en.txt ../Arquivos_gerados/en/en_${ocupacao}.txt
    mv ew.txt ../Arquivos_gerados/ew/ew_${ocupacao}.txt
    mv erro_little.txt ../Arquivos_gerados/erro_little/erro_little_${ocupacao}.txt
    mv lambda.txt ../Arquivos_gerados/lambda/lambda_${ocupacao}.txt
    mv resultados_finais.txt ../Arquivos_gerados/resultados_finais/resultados_finais_${ocupacao}.txt
done

echo "=== Simulações concluídas! ==="

# Junta os resultados finais em um CSV na pasta principal de resultados
csv_saida="../Arquivos_gerados/resultados_finais_todos.csv"
echo "ocupacao,max_fila,media_entre_requisicoes,media_tempos_de_servico,ocupacao_esperada,ocupacao_calculada,E[N],E[W],Lambda,Erro_little" > $csv_saida

for ocupacao in "${ocupacoes[@]}"; do
    arquivo="../Arquivos_gerados/resultados_finais/resultados_finais_${ocupacao}.txt"
    max_fila=$(grep "max fila:" $arquivo | awk '{print $NF}')
    media_entre_requisicoes=$(grep "media entre requisicoes:" $arquivo | awk '{print $NF}')
    media_tempos_de_servico=$(grep "media tempos de servico:" $arquivo | awk '{print $NF}')
    ocupacao_esperada=$(grep "ocupacao esperada:" $arquivo | awk '{print $NF}')
    ocupacao_calculada=$(grep "ocupacao calculada:" $arquivo | awk '{print $NF}')
    EN=$(grep -E "E\[N\]" $arquivo | sed 's/[^0-9\.]*\([0-9\.]*\).*/\1/')
    EW=$(grep -E "E\[W\]" $arquivo | sed 's/[^0-9\.]*\([0-9\.]*\).*/\1/')
    Lambda=$(grep -E "Lambda" $arquivo | sed 's/[^0-9\.]*\([0-9\.]*\).*/\1/')
    Erro_little=$(grep "Erro little:" $arquivo | awk '{print $NF}')
    echo "$ocupacao,$max_fila,$media_entre_requisicoes,$media_tempos_de_servico,$ocupacao_esperada,$ocupacao_calculada,$EN,$EW,$Lambda,$Erro_little" >> $csv_saida
done

echo "Arquivo CSV gerado em: $csv_saida"