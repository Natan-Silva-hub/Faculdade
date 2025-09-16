#!/bin/bash

# Script master para executar todas as simulações e gerar gráficos
# Executa as simulações para os 4 cenários e gera os gráficos comparativos

echo "========================================================"
echo "  SISTEMA DE COLETA DE MÉTRICAS - LEI DE LITTLE"
echo "========================================================"
echo ""
#Este script irá:
#1. Compilar o código de simulação
#2. Executar simulações para 4 cenários (50%, 80%, 95%, 99.9%)
#3. Coletar métricas a cada 10 segundos
#4. Gerar gráficos comparativos

# Executa as simulações
echo "Iniciando execução das simulações..."
chmod +x executar_simulacoes.sh
./executar_simulacoes.sh

if [ $? -ne 0 ]; then
    echo "Erro na execução das simulações!"
    exit 1
fi

echo ""
echo "Arquivos organizados em '../Arquivos_gerados/'"

# Gera gráficos se GNUplot estiver disponível
if command -v gnuplot &> /dev/null; then
    echo ""
    echo "Gerando gráficos comparativos..."

    # Executa o GNUplot no diretório do script, mantendo os caminhos dos arquivos
    gnuplot script_graficos_metricas.gnu

    if [ $? -eq 0 ]; then
        echo "Gráficos gerados com sucesso!"

        # Move gráficos para diretório específico se não existir
        if [ ! -d "../Graficos" ]; then
            mkdir ../Graficos
        fi

        mv *.svg ../Graficos/ 2>/dev/null
        echo "Gráficos movidos para '../Graficos/'"
    else
        echo "Erro na geração dos gráficos!"
    fi
else
    echo ""
    echo "Para gerar os gráficos manualmente:"
    echo "1. Instale o GNUplot: sudo apt-get install gnuplot"
    echo "2. Execute: gnuplot script_graficos_metricas.gnu"
fi

echo ""
echo "========================================================"
echo "                    PROCESSO CONCLUÍDO!"
echo "========================================================"