# Gráfico 1: E[N] - Número médio no sistema
set terminal svg
set output 'en_comparacao.svg'
set title 'E[N] - Número Médio no Sistema ao Longo do Tempo'
set ylabel 'E[N] (pacotes)'

plot '../Arquivos_gerados/en/en_50.0.txt' using 1:2 with lines linewidth 2 title '50% ocupação', \
     '../Arquivos_gerados/en/en_80.0.txt' using 1:2 with lines linewidth 2 title '80% ocupação', \
     '../Arquivos_gerados/en/en_95.0.txt' using 1:2 with lines linewidth 2 title '95% ocupação', \
     '../Arquivos_gerados/en/en_99.9.txt' using 1:2 with lines linewidth 2 title '99.9% ocupação'

# Gráfico 2: E[W] - Tempo médio no sistema
set terminal svg
set output 'ew_comparacao.svg'
set title 'E[W] - Tempo Médio no Sistema ao Longo do Tempo'
set ylabel 'E[W] (s)'

plot '../Arquivos_gerados/ew/ew_50.0.txt' using 1:2 with lines linewidth 2 title '50% ocupação', \
     '../Arquivos_gerados/ew/ew_80.0.txt' using 1:2 with lines linewidth 2 title '80% ocupação', \
     '../Arquivos_gerados/ew/ew_95.0.txt' using 1:2 with lines linewidth 2 title '95% ocupação', \
     '../Arquivos_gerados/ew/ew_99.9.txt' using 1:2 with lines linewidth 2 title '99.9% ocupação'

# Gráfico 3: Erro da Lei de Little
set terminal svg
set output 'erro_little_comparacao.svg'
set title 'Erro da Lei de Little ao Longo do Tempo'
set ylabel 'Erro (E[N] - λ·E[W])'

plot '../Arquivos_gerados/erro_little/erro_little_50.0.txt' using 1:2 with lines linewidth 2 title '50% ocupação', \
     '../Arquivos_gerados/erro_little/erro_little_80.0.txt' using 1:2 with lines linewidth 2 title '80% ocupação', \
     '../Arquivos_gerados/erro_little/erro_little_95.0.txt' using 1:2 with lines linewidth 2 title '95% ocupação', \
     '../Arquivos_gerados/erro_little/erro_little_99.9.txt' using 1:2 with lines linewidth 2 title '99.9% ocupação'

# Gráfico 4: Lambda
set terminal svg
set output 'lambda_comparacao.svg'
set title 'Lambda ao Longo do Tempo'
set ylabel 'Lambda'

plot '../Arquivos_gerados/lambda/lambda_50.0.txt' using 1:2 with lines linewidth 2 title '50% ocupação', \
     '../Arquivos_gerados/lambda/lambda_80.0.txt' using 1:2 with lines linewidth 2 title '80% ocupação', \
     '../Arquivos_gerados/lambda/lambda_95.0.txt' using 1:2 with lines linewidth 2 title '95% ocupação', \
     '../Arquivos_gerados/lambda/lambda_99.9.txt' using 1:2 with lines linewidth 2 title '99.9% ocupação'