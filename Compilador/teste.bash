#!/bin/bash

# Cria a pasta para salvar as saídas (se não existir)
PASTA_SAIDAS="saidas_testes"
mkdir -p "$PASTA_SAIDAS"

# Limpa algum a.out que possa ter sobrado de execuções anteriores
rm -f a.out

# Função para executar todos os testes de um diretório
executar_testes() {
    local diretorio_alvo=$1
    local prefixo_saida=$2

    echo ">> Testando os arquivos do diretório: $diretorio_alvo"

    for script in "$diretorio_alvo"/*.pas; do
        # Verifica se o arquivo existe (para caso a pasta esteja vazia)
        if [ -f "$script" ]; then
            nome_arquivo=$(basename "$script")
            arquivo_saida="$PASTA_SAIDAS/${prefixo_saida}_${nome_arquivo}.txt"

            echo "   Compilando $nome_arquivo..."

            # Adiciona um cabeçalho no arquivo
            echo "=== Arquivo: $script ===" > "$arquivo_saida"
            echo "--- Saída do Terminal ---" >> "$arquivo_saida"

            # Executa o compilador, capturando a saída padrão e de erro para o arquivo
            ./compilador "$script" >> "$arquivo_saida" 2>&1
            # Salva imediatamente o valor de retorno (o exit code da execução acima)
            RETORNO=$?

            # Se o arquivo a.out foi gerado pelo compilador, anexa ele no arquivo de saída
            if [ -f "a.out" ]; then
                echo -e "\n--- Conteúdo do a.out gerado ---" >> "$arquivo_saida"
                cat a.out >> "$arquivo_saida"
                # Remove o a.out para não interferir no próximo teste
                rm -f a.out
            else
                echo -e "\n--- Nenhum arquivo a.out foi gerado ---" >> "$arquivo_saida"
            fi

            # Imprime o valor de retorno final no arquivo
            echo -e "\n--- Resultado ---" >> "$arquivo_saida"
            echo "Valor de retorno: $RETORNO" >> "$arquivo_saida"
        fi
    done
    echo ""
}

# Chama a função para as duas pastas
executar_testes "exemplos_pascal/validos" "valido"
executar_testes "exemplos_pascal/erro" "erro"

echo "Todos os testes foram concluídos! As saídas estão na pasta '$PASTA_SAIDAS'."
